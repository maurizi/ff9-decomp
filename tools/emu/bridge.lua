-- Loaded into PCSX-Redux with -dofile by tools/emu.py. Exposes Lua evaluation over the
-- web server (GET /api/v1/lua/eval?code=<urlencoded Lua>) plus the helpers below,
-- which emu.py calls as Bridge.<name>(...). Every eval returns JSON: {"ok":true,"result":...}
-- or {"ok":false,"error":"..."}. Re-running this file keeps existing state (frame count,
-- breakpoints, logs).

local ffi = require 'ffi'

Bridge = Bridge or {}
local B = Bridge

B.frame = B.frame or 0
B.pauseAt = nil
B.holds = B.holds or {}       -- button -> release frame
B.bps = B.bps or {}           -- id -> { bp = Breakpoint, kind, addr, width, mode, hits, log, max }
B.nextBpId = B.nextBpId or 1
B.lastBreak = B.lastBreak or nil

---------------------------------------------------------------- JSON

local function jsonEscape(s)
    return (s:gsub('[%c"\\]', function(c)
        if c == '"' then return '\\"' end
        if c == '\\' then return '\\\\' end
        if c == '\n' then return '\\n' end
        return string.format('\\u%04x', c:byte())
    end))
end

local function isArray(t)
    local n = 0
    for k in pairs(t) do
        if type(k) ~= 'number' then return false end
        n = n + 1
    end
    for i = 1, n do if t[i] == nil then return false end end
    return true
end

local function json(v)
    local t = type(v)
    if t == 'nil' then return 'null' end
    if t == 'boolean' then return tostring(v) end
    if t == 'number' then
        if v ~= v or v == math.huge or v == -math.huge then return 'null' end
        if v == math.floor(v) and math.abs(v) < 2 ^ 53 then return string.format('%d', v) end
        return string.format('%.17g', v)
    end
    if t == 'string' then return '"' .. jsonEscape(v) .. '"' end
    if t == 'cdata' then return json(tonumber(v)) end
    if t == 'table' then
        local parts = {}
        if next(v) == nil then return '[]' end
        if isArray(v) then
            for i = 1, #v do parts[i] = json(v[i]) end
            return '[' .. table.concat(parts, ',') .. ']'
        end
        for k, x in pairs(v) do parts[#parts + 1] = json(tostring(k)) .. ':' .. json(x) end
        return '{' .. table.concat(parts, ',') .. '}'
    end
    return json(tostring(v))
end
B.json = json

---------------------------------------------------------------- memory

-- Returns a uint8_t* for a PSX address (KUSEG/KSEG0/KSEG1 RAM mirrors, or scratchpad) and the
-- number of bytes valid from there.
local function ptr(addr)
    local phys = bit.band(addr, 0x1fffffff)
    if phys < 0x800000 then
        local off = phys % 0x200000
        return PCSX.getMemPtr() + off, 0x200000 - off
    end
    if phys >= 0x1f800000 and phys < 0x1f800400 then
        local off = phys - 0x1f800000
        return PCSX.getScratchPtr() + off, 0x400 - off
    end
    error(string.format('address %08x is not RAM or scratchpad', addr))
end

local function checked(addr, len)
    local p, avail = ptr(addr)
    if len > avail then error(string.format('%d bytes at %08x runs past the end of memory', len, addr)) end
    return p
end

function B.read(addr, len)
    local p = checked(addr, len)
    local out = {}
    for i = 0, len - 1 do out[#out + 1] = string.format('%02x', p[i]) end
    return table.concat(out)
end

function B.write(addr, hex)
    local len = #hex / 2
    local p = checked(addr, len)
    for i = 0, len - 1 do p[i] = tonumber(hex:sub(i * 2 + 1, i * 2 + 2), 16) end
    PCSX.invalidateCache()
    return len
end

local function r32(addr)
    local p = checked(addr, 4)
    return p[0] + p[1] * 0x100 + p[2] * 0x10000 + p[3] * 0x1000000
end
B.r32 = r32

---------------------------------------------------------------- registers

local gprNames = {
    'r0', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra', 'lo', 'hi',
}

function B.regs()
    local r = PCSX.getRegisters()
    local out = { pc = tonumber(r.pc) }
    for i, name in ipairs(gprNames) do out[name] = tonumber(r.GPR.r[i - 1]) end
    return out
end

function B.setReg(name, value)
    local r = PCSX.getRegisters()
    if name == 'pc' then r.pc = value return end
    for i, n in ipairs(gprNames) do
        if n == name then r.GPR.r[i - 1] = value return end
    end
    error('unknown register ' .. name)
end

-- Captured on every logged breakpoint hit: enough to see who called and with what.
local function snapshot(address)
    local r = PCSX.getRegisters().GPR.n
    return {
        frame = B.frame,
        addr = address,
        pc = tonumber(PCSX.getRegisters().pc),
        ra = tonumber(r.ra),
        sp = tonumber(r.sp),
        a0 = tonumber(r.a0), a1 = tonumber(r.a1), a2 = tonumber(r.a2), a3 = tonumber(r.a3),
        v0 = tonumber(r.v0),
    }
end

---------------------------------------------------------------- frames and input

local buttons = PCSX.CONSTS.PAD.BUTTON
local function pad() return PCSX.SIO0.slots[1].pads[1] end

local function onVsync()
    B.frame = B.frame + 1
    for button, release in pairs(B.holds) do
        if B.frame >= release then
            pad().clearOverride(buttons[button])
            B.holds[button] = nil
        end
    end
    if B.pauseAt and B.frame >= B.pauseAt then
        B.pauseAt = nil
        PCSX.pauseEmulator()
    end
end

-- Keep the listener referenced, or it gets collected and stops firing.
if B.vsyncListener == nil then
    B.vsyncListener = PCSX.Events.createEventListener('GPU::Vsync', function() onVsync() end)
end

-- Resumes and pauses again after n vsyncs (n = 0 just pauses).
function B.runFrames(n)
    B.lastBreak = nil
    if n <= 0 then PCSX.pauseEmulator() return B.frame end
    B.pauseAt = B.frame + n
    PCSX.resumeEmulator()
    return B.pauseAt
end

-- Holds the named buttons (e.g. {'CROSS','UP'}) for n frames, starting now.
function B.hold(names, n)
    for _, name in ipairs(names) do
        local id = buttons[name]
        if id == nil then error('unknown button ' .. name) end
        pad().setOverride(id)
        B.holds[name] = B.frame + n
    end
end

function B.releaseAll()
    for name in pairs(B.holds) do pad().clearOverride(buttons[name]) end
    B.holds = {}
end

---------------------------------------------------------------- breakpoints

-- The debugger halves interpreter speed, so it is only switched on while breakpoints exist.
local function updateDebugger()
    PCSX.settings.emulator.Debug.Debug = next(B.bps) ~= nil
end

-- kind: 'Exec' | 'Read' | 'Write'. mode: 'log' records hits and keeps running; 'break' pauses
-- the emulator (and records the hit in B.lastBreak). max caps the stored log (hits still counted).
function B.addBp(addr, kind, width, mode, max, label)
    local id = B.nextBpId
    B.nextBpId = id + 1
    local entry = { id = id, kind = kind, addr = addr, width = width, mode = mode, hits = 0, log = {}, max = max or 1000, label = label or '' }
    entry.bp = PCSX.addBreakpoint(addr, kind, width, 'bridge#' .. id, function(address, w, cause)
        entry.hits = entry.hits + 1
        local s = snapshot(address)
        s.bp = id
        if #entry.log < entry.max then entry.log[#entry.log + 1] = s end
        if mode == 'break' then
            B.lastBreak = s
            PCSX.pauseEmulator()
        end
        return true
    end, label or ('bridge#' .. id))
    B.bps[id] = entry
    updateDebugger()
    return id
end

function B.removeBp(id)
    local e = B.bps[id]
    if e == nil then return false end
    e.bp:remove()
    B.bps[id] = nil
    updateDebugger()
    return true
end

function B.clearBps()
    for id in pairs(B.bps) do B.removeBp(id) end
    B.lastBreak = nil
end

function B.listBps()
    local out = {}
    for id, e in pairs(B.bps) do
        out[#out + 1] = { id = id, kind = e.kind, addr = e.addr, width = e.width, mode = e.mode, hits = e.hits, label = e.label }
    end
    return out
end

-- Returns and clears a breakpoint's log.
function B.takeLog(id)
    local e = B.bps[id]
    if e == nil then error('no breakpoint #' .. id) end
    local out = { hits = e.hits, log = e.log }
    e.log = {}
    e.hits = 0
    return out
end

---------------------------------------------------------------- status

function B.status()
    return {
        frame = B.frame,
        running = PCSX.isRunning and PCSX.isRunning() or nil,
        pc = tonumber(PCSX.getRegisters().pc),
        cycles = tonumber(PCSX.getCPUCycles()),
        pauseAt = B.pauseAt,
        lastBreak = B.lastBreak,
    }
end

---------------------------------------------------------------- web handler

PCSX.WebServer = PCSX.WebServer or {}
PCSX.WebServer.Handlers = PCSX.WebServer.Handlers or {}

local function urlDecode(s)
    s = s:gsub('+', ' ')
    return (s:gsub('%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end))
end

-- Redux only hands Lua handlers the URL (not the body), and URLs over ~250 bytes get rejected,
-- so code arrives in the query string, longer code in pieces: ?append=<piece> (repeated), then
-- ?code=<last piece> runs everything appended so far plus that piece.
B.pending = {}

PCSX.WebServer.Handlers['eval'] = function(request)
    local code, append
    for k, v in (request.urlData.query or ''):gmatch('([^&=]+)=([^&]*)') do
        if k == 'code' then code = urlDecode(v) end
        if k == 'append' then append = urlDecode(v) end
    end
    if append ~= nil then
        B.pending[#B.pending + 1] = append
        return json({ ok = true })
    end
    if code == nil then return json({ ok = false, error = 'missing query parameter "code"' }) end
    B.pending[#B.pending + 1] = code
    code = table.concat(B.pending)
    B.pending = {}
    local chunk, err = loadstring(code, 'eval')
    if chunk == nil then return json({ ok = false, error = err }) end
    local ok, result = pcall(chunk)
    if not ok then return json({ ok = false, error = tostring(result) }) end
    return json({ ok = true, result = result })
end

updateDebugger()
print('ff9 bridge loaded')

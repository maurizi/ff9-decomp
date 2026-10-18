-- Coverage: exec breakpoints on function entries sharing ONE ffi callback (LuaJIT limits callbacks).
local ffi = require 'ffi'
Cov = Cov or {}
Cov.C = Cov.C or ffi.load 'PCSX'
local C = Cov.C
if Cov.cb == nil then
  Cov.cb = ffi.cast('bool (*)(uint32_t address, unsigned width, const char* cause)', function(address, w, cause)
    local a = tonumber(address)
    local d = Cov.data[a]
    if d == nil then return true end
    d.n = d.n + 1
    if d.f < 0 then d.f = Bridge.frame end
    if d.nra < 6 then
      local ra = tonumber(PCSX.getRegisters().GPR.n.ra)
      if d.ra[ra] == nil then d.ra[ra] = 1; d.nra = d.nra + 1 end
    end
    if d.n >= Cov.cap and d.on then C.disableBreakpoint(d.bp); d.on = false end
    return true
  end)
end
function Cov.stop()
  for a, d in pairs(Cov.data or {}) do if d.bp ~= nil then C.removeBreakpoint(d.bp); d.bp = nil end end
  PCSX.settings.emulator.Debug.Debug = next(Bridge.bps) ~= nil
  return true
end
function Cov.start(addrs, cap)
  Cov.stop()
  Cov.cap = cap or 2000
  Cov.data = {}
  local n = 0
  for _, a in ipairs(addrs) do
    local d = { n = 0, f = -1, ra = {}, nra = 0, on = true }
    d.bp = C.addBreakpoint(a, 'Exec', 4, 'cov', Cov.cb, 'cov')
    Cov.data[a] = d
    n = n + 1
  end
  PCSX.settings.emulator.Debug.Debug = true
  return n
end
function Cov.reset()
  for a, d in pairs(Cov.data or {}) do
    d.n = 0; d.f = -1; d.ra = {}; d.nra = 0
    if not d.on and d.bp ~= nil then C.enableBreakpoint(d.bp); d.on = true end
  end
  return true
end
function Cov.result()
  local out = {}
  for a, d in pairs(Cov.data or {}) do
    if d.n > 0 then
      local ras = {}
      for r in pairs(d.ra) do ras[#ras + 1] = r end
      out[#out + 1] = { a, d.n, d.f, ras }
    end
  end
  return out
end
return 'cov loaded'

-- list script objects: uid kind state flags actorType modelId pos (kind-4 actors)
local function r32(a) return Bridge.r32(a) end
local m = PCSX.getMemPtr()
local function u8(a) return m[bit.band(a, 0x1fffff)] end
local function u16(a) return u8(a) + u8(a + 1) * 256 end
local function s32(a) local v = r32(a); if v >= 0x80000000 then v = v - 0x100000000 end return v end
local st = r32(0x8007AECC)
local n = r32(st + 0x16D8)
local out = {}
while n ~= 0 and #out < 80 do
  local o = r32(n + 4)
  out[#out + 1] = string.format('uid=%d kind=%d state=%d flags=%02x type=%d model=%d pos=%d,%d,%d', u8(o + 7), u8(o + 5), u8(o + 14), u8(o + 15), u8(o + 17), u16(o + 52), s32(o + 20), s32(o + 24), s32(o + 28))
  n = r32(n)
end
return out

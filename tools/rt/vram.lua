-- DumpVram(path): write the whole 1 MB VRAM (PCSX.GPU.getVRAM(), 1024x512 halfwords) to a file in ~1 s.
-- `tools/emu.py lua -f tools/rt/vram.lua` defines it, then `tools/emu.py lua 'return DumpVram("build/rt/v.bin")'`.
function DumpVram(path)
  local v=PCSX.GPU.getVRAM()
  local f=io.open(path,'wb')
  local t={}
  for i=0,#v-1 do t[#t+1]=string.char(v[i]) if #t==4096 then f:write(table.concat(t)) t={} end end
  f:write(table.concat(t)) f:close()
  return #v
end
return 1

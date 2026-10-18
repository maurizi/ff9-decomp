-- Per-frame pokes: Freeze.list = { {addr, hex}, ... } written every vsync (no cache invalidation, pcall-guarded).
Freeze = Freeze or { list = {} }
if Freeze.listener == nil then
  Freeze.listener = PCSX.Events.createEventListener('GPU::Vsync', function()
    pcall(function()
      local mem = PCSX.getMemPtr()
      for _, e in ipairs(Freeze.list) do
        local off = bit.band(e[1], 0x1fffff)
        local hex = e[2]
        for i = 0, #hex / 2 - 1 do mem[off + i] = tonumber(hex:sub(i * 2 + 1, i * 2 + 2), 16) end
      end
    end)
  end)
end
return #Freeze.list

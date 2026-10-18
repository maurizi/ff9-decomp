-- Patch.set(cmd, ability): the next btl_setCmd(slot, 1=Attack, ...) call becomes (slot, cmd, ability, targets)
Patch = Patch or {}
function Patch.set(cmd, ab, tgt)
  if Patch.bp then Patch.bp:remove(); Patch.bp = nil end
  if cmd == nil then return 'cleared' end
  Patch.hits = 0
  Patch.bp = PCSX.addBreakpoint(0x800B1250, 'Exec', 4, 'patch', function()
    local r = PCSX.getRegisters().GPR.n
    if tonumber(r.a1) == 1 and Patch.hits == 0 then
      r.a1 = cmd; r.a2 = ab
      if tgt then r.a3 = tgt end
      Patch.hits = 1
    end
    return true
  end, 'patch')
  PCSX.settings.emulator.Debug.Debug = true
  return 'armed'
end
return 'patch loaded'

# Overlay constants that need a runtime check

Flag bits and state values in the overlays (not ovl_0b) that the code alone doesn't pin down, so they stay
literals for now. Each entry: value, field, hypothesis, and how to observe it in the emulator.

| overlay | field | value | hypothesis | how to observe |
|---|---|---|---|---|
| ovl_0a | `FieldMap.flags` (`ctx.field->flags`) | 0x4 | "field not running yet / loading": fld_load sets it; with it set fld_main skips fld_updateEncounter, fld_endFrame draws with DrawOTagEnv from OT_LAST, vsyncMode drops to 0 when GD_HIDE_ALL is also set, and the sound loader leaves 0x3000 more heap | watch writes to `ctx.field->flags` from a field save state; note which event op / function clears bit 2 and whether random encounters only start after that |
| ovl_0a | `FieldEvtCtx.flags` | 0x1000 | blocks fld_drawModels from restarting texture animation 2 (eye blink?) when no animation runs | set the bit on Zidane's slot in a field state and see whether his blinking stops |
| ovl_0a | `FieldEvtCtx.flags` | 0x800 | fld_setSlotColor sets 0x400 and 0x800 with the colour; the next call only clears 0x800 and returns 0 (skip-once latch?) | trace fld_setSlotColor calls and its return value during a scene that changes an object's colour |
| ovl_0a | `FieldActorExt.flags` | 0x02000000 | set together with FLD_EXT_BUILT when the mirror copy is built; no reader in ovl_0a C | watch reads of the word in a mirror scene (e.g. a reflective floor) |
| ovl_0a | `BgiAnim.flags` / `BgiFloorMove.flags` | 0x2 | bgi_resetFloorMoves and the anim reset store 2; no C reader | watch reads of the flags during a field with walkmesh animations |

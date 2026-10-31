# Round 33 ovl_0b: leads left for a runtime check

Round 33 named only what the code proves (`docs/wip/names_r33_ovl_0b.txt`). The rows below would confirm or extend
those names in the running game (`tools/emu.py`, `docs/emulator.md`; all addresses ovl_0b, resident in battle).

| # | item | hypothesis / what is open | how to observe | state |
|---|---|---|---|---|
| 1 | `EffEmit.kind` (`BTLFX_MOTION_*`) | the motion names come from the math in btlfx_updateParticles (ballistic, orbit, spiral, wobble, helix, homing). Which effects use kinds 1-5 is not known; a screenshot per kind would show whether "orbit" / "wobble" match what the player sees | breakpoint btlfx_spawnEmitter, log `*(u8 *)a2` (the kind) and `g_btlfxState.effectId`; screenshot a few frames later | `disc3_malboro_lv99_calm` (spells, Steal), any summon |
| 2 | `EffEmit.m.ballistic.floorMode` other than 0/1 | the code slows the vertical speed by the height above the floor inside `floorMargin`, cancels gravity in the lower half and zeroes it below the floor: particles settle on the floor. Which effects use it? | same breakpoint, log the halfword at file + 0x12 when kind is 0 | any battle |
| 3 | `BtlCmd8.forceCamera` (bit 30 of the ability word) | set: the enemy camera move of the ability plays even with the fixed-camera option. Which enemy abilities carry it (boss attacks?) | dump `Battle.enemyAbilities` (BtlCmd8, 0x10 bytes each) at battle start and test bit 30 | a boss battle save state |
| 4 | `BtlCmd8` bits 17-29 and 31, `Battle16CC` / `g_usableItemTableById` bits 8-28 and 30 | not read anywhere in ovl_0b | write watch / read watch on `Battle.abilities[i]` word 0 during a turn | `disc3_malboro_lv99_calm` |
| 5 | `Unk800D7C9C.arg.skipParty` / `Unk800D955C.skipParty` (jump ops 0x6C / 0x6D, op argument 2) | code: value 1 with a party-unit target ends the op at once (R30 #10). Which effects pass 1 and what the player sees (no jump for party targets?) | breakpoint btlfx_opJumpForward / btlfx_opJumpHome with a0 == 1, log `a4[1]` and the effect id | Malboro attacks on the party |
| 6 | `g_btluiState.unk9` / `unk14` | only written by `func_800EA1D0`, which has no callers in ovl_0b or the modules; check whether another overlay calls 0x800EA1D0 while ovl_0b is loaded | breakpoint 0x800EA1D0 through a battle | any battle |
| 7 | `FileEntry16.idHi` for character models | read as the sectors loaded into the party slot buffer, the rest of the file going to a second buffer; confirm with the sizes of model files | log `idHi` and `endSector - sector` at btl_reloadPartyStep | battle start |
| 8 | `WmSrcFT4.unk14`, `BtlBoneDef.unk2` | no C use; the non-matching asm btl_drawMesh / btl_calcBoneMatrices was not checked (static question: trace their loads) | read the asm, or read watch on a model's primitive list | any battle |

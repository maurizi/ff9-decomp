# Weak-name redo: questions for an emulator pass

The weak-name redo (docs/wip/names_from_port.txt, "# ===== weak-name redo") renamed the R38 Unused/Stub
placeholders from code evidence. These questions need the running game. Record answers in
docs/runtime-findings.md as usual.

1. **ovl_0c `g_worldLowBuffer` (0x800CE3F8 -> 0x80100800).** On the world map, what occupies
   0x80100800-0x80138800? Watch writes to that range for a few hundred frames from `field_worldmap_ice_exit`
   (heap of wm_heapInit / g_modeHeap? model data?). If something lives there, the pointer can be named for it.
2. **ovl_0c `g_worldCameraInitFlag` (0x800CE1E1), `g_worldMoveInitWord` (0x800CE284), `g_worldSysInitFlag`
   (0x800CF47E).** R30 found no reads. Confirm no write other than the init that clears them (write watch over
   a world-map session: walk, chocobo, airship, enter/leave a field).
3. **main `g_menu.result` (MenuState +0x1C, 0x80077DCC).** Write watch while opening each menu type (main,
   name entry, shop, save, chocograph, card): does anything store a nonzero value? (R30 saw 0 calls of menu_getResult.)
4. **ovl_15 `g_messagesFrameWord` (0x800AE4A8) and `g_theEndInitWord` (0x800AE430).** Read/write watch during the
   ending messages and "The End" (needs a save near the ending, or skip with a debug state).
5. **fx_0b8 `fx_0b8_scrollInitWord` / fx_1be `fx_1be_scrollInitWord`.** Read watch while the effect runs
   (fx_0b8: the summon using effect 0xB8; fx_1be: effect 0x1BE, see docs/effect-modules.md). Value is 2.
6. **fx_17d_4 `fx_17d_4_buf400` / fx_1bf_0 `fx_1bf_0_buf400`.** Any write past byte 0 of the 0x400 bytes while the
   effect runs? (Tells whether it is a work buffer used through a pointer.)
7. **ovl_0b `btlui_perfMark` (0x800EB578), args 0xBA / 0xBB.** No visible effect is expected (empty). Confirm it is hit
   twice per battle frame with the UI enabled, and never with other arguments.
8. **main `pad_onStartPressed` (0x8002C1E4).** Confirm it is hit when Start is pressed (and on Start auto-repeat) in a
   shop's Buy list, and nowhere else.
9. **ovl_0b export slots 0x090, 0x094, 0x0A4, 0x29C, 0x300, 0x308, 0x30C** (`btlfx_motionNopZero`,
   `btlfx_motionNopNeg`, `btlfx_motionNop`, `btlfx_imageNop`, `btlfx_vertexNopZero1`, `btlfx_vertexNop`,
   `btlfx_vertexNopZero2`). No module calls them in code; an exec breakpoint over a broad set of effects (summons
   included) would confirm they are never reached.

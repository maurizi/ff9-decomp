# Runtime findings

Evidence from the running game (`tools/emu.py`, see `docs/emulator.md`) about what symbols do.
Append entries; the decomp agent / coordinator turns confirmed ones into names in
`config/symbols*.txt` and marks them **applied**.

Format:

```
## D_XXXXXXXX → proposed_name  (status: proposed | applied | rejected)
- binary: main | ovl_XX; type: u32 / struct ... / function signature
- evidence: commands run, at which save state / scene, and what they showed
- confidence: high / medium / low, and what would confirm it
```

## D_800678EC → g_vsyncCounter?  (status: proposed)
- binary: main; type: u32
- evidence: increases by 1 per frame (`snap`/`diff` one frame apart, intro FMV). `watch D_800678EC --frames 10`:
  exactly 10 writes, all at `func_80016620+0x2C` (`sw $v0`), called from `func_80016114+0xDC`. Read ~5000×/frame
  at `func_80015DD0+0x74`, which `VSync` calls (a busy-wait loop), and also read inside `VSync` itself.
- confidence: medium. Probably the libetc vsync counter that `VSync()` polls, which would make `func_80016620`
  the vsync callback and `func_80016114` the interrupt dispatcher. Check them against the PsyQ libetc source.

## Overlay usage seen at runtime  (status: proposed)
- `ovl_14` (+ `ovl_64`) is resident during the boot logos / intro (Squaresoft logo, opening FMV), so it's probably the
  opening/logo sequencer, not a minigame.
- `ovl_39` is resident on the title screen's Continue → memory-card slot and file select screens, which confirms "save / load".
- `ovl_0a` is resident in the field (Burmecia vault after loading a save), which confirms "field".
- The top-level main menu (Triangle in the field) loads no 0x801EDF00 overlay, so its code is in the exe or `ovl_0a`.
  Opening Item loads `ovl_37`, which confirms "menu: items".
- evidence: `tools/emu.py ovl` at save states `title_screen`, `field_burmecia_vault`, `menu_main`, `menu_items`.

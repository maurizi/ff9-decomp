# Out-of-line functions that duplicate a `static inline *_inline` helper

Scope: `src/main/` and the overlays except ovl_0b (inline helpers in `include/*.h` and in the `.c` files). Found by
comparing function bodies with parameters and locals renamed to placeholders (`build/locals/inl.py` during the
locals-naming pass), then checked by hand. "Calls the inline?" was tested by replacing the out-of-line body with
`return X_inline(params);` (the inline moved above the function when it was defined below it) and running
`make verify`. Nothing was restructured; this is a to-do list.

## Identical bodies

| out-of-line function | inline twin | could the body just call the inline? |
|---|---|---|
| `bgi_getFloorMove` (ovl_0a bgi.c) | `bgi_getFloorMove_inline` (ovl_0a.h) | yes, matches |
| `chr_testSupportBit` (main chr.c) | `chr_testSupportBit_inline` (ff9.h) | yes, matches |
| `chr_updateStats` (main chr.c) | `chr_updateStats_inline` (ff9.h) | yes, matches |
| `evt_createRegion` (main event.c) | `evt_createRegion_inline` (ff9.h) | yes, matches |
| `evt_getArg8` (main event.c) | `evt_getArg8_inline` (ff9.h) | yes, matches |
| `evt_getArg16` (main event.c) | `evt_getArg16_inline` (ff9.h) | yes, matches |
| `evt_getArg24` (main event.c) | `evt_getArg24_inline` (ff9.h) | yes, matches |
| `win_close` (main window.c) | `win_close_inline` (ff9.h) | yes, matches |
| `wm_wrapPos` (ovl_0c tu_800BA528.c) | `wm_wrapPos_inline` (ovl_0c.h) | yes, matches |
| `menuChocograph_getRequiredAbility` (ovl_32) | `menuChocograph_getRequiredAbility_inline` (ovl_32.h) | yes, matches |
| `menuSave_isSlotReady` (ovl_39) | `menuSave_isSlotReady_inline` (ovl_39.h) | yes, matches |
| `menuSave_readHeader` (ovl_39) | `menuSave_readHeader_inline` (ovl_39.c, above it) | yes, matches |
| `chr_getAp` (main chr.c) | `chr_findAbilitySlot_ap_inline` (ff9.h) | no: codegen differs |
| `chr_getApNeeded` (main chr.c) | `chr_getApNeeded_inline` (ff9.h) | no: codegen differs |
| `item_findSlot` (main ui.c) | `item_findSlot_inline` (ff9.h) | no: codegen differs |
| `text_formatNumber` (main text.c) | `text_formatNumber_inline` (ff9.h) | no: codegen differs |
| `menuAbility_getSupportState` (ovl_33 menu_ability.c) | `menuAbility_getSupportState_inline` (same file) | no: codegen differs |
| `chr_cureStatusBit` (main tu_80053924.c) | `chr_cureStatusBit_inline` (same file, below it) | no: differs even with the inline moved above |
| `chr_setAp` (main chr.c, `u8 v`) | `chr_setAp_inline` (same file, below it, `s32 v`) | no: differs with the inline moved above |

## Near duplicates (same statements, different locals or argument preprocessing)

| out-of-line function | inline twin | difference | could it call the inline? |
|---|---|---|---|
| `msg_getEntry` (main msg.c) | `msg_getEntry_inline` (msg.c) | out-of-line declares two extra unused locals (`t`, `k`) | yes, matches without them |
| `ui_buildSpriteDef` (main ui.c) | `ui_buildSpriteDef_inline` (ff9.h) | the inline writes the raw GPU words (`0x66606060`, `0xE1000600`) where the out-of-line body uses `GPU_RGBC`/`get_mode` | yes, matches |
| `evt_getObjectModelData` (main event.c) | `evt_getObjectModelData_inline` (ff9.h) | out-of-line returns `s32` through an `s32 *` view of the slot (`q[2]`); the inline returns `ModelSlot.model` typed | yes, matches |
| `menuCard_showCachedImage` (ovl_36) | `menuCard_showCachedImage_inline` (ovl_36.h) | order of the RECT field stores | yes, matches |
| `evt_getArg8` (main event.c) | `evt_getArg8_inline2` (ff9.h) | a second variant of the arg reader (the plain `evt_getArg8_inline` already matches) | not tested |
| `wm_testCellType` (ovl_0c w_movement.c) | `wm_testCellType_inline2` (ovl_0c.h; takes the topography already extracted) | out-of-line applies `WM_ATTR_TOPOGRAPHY` first | no: `return wm_testCellType_inline2(allowed, WM_ATTR_TOPOGRAPHY(cellAttr));` differs |
| `wm_testCellType` (ovl_0c) | `wm_testCellType_inline` (ovl_0c.h) | word select written differently | no: differs |

## Inline helpers that duplicate each other

- `evt_rollEncounter_rand` (ff9.h), `wm_rand8` (ovl_0c.h) and `rand8_inline` (ovl_0b.h) are the same
  "refill if exhausted, return `*g_randPtr++`" reader. The pattern is also open-coded in several main functions
  (e.g. `evt_resetIdleTimer`).

## Not duplicates (already call their inline)

`akao_lfoTable`, `chr_findAbilitySlot`, `evt_finishTurn`, `evt_freeAllocNode`, `evt_getBattleVar`, `evt_getObject`,
`evt_resolveObjectId`, `msg_closeWindow` are one-line wrappers around their `_inline`. `msg_closeAll` (loops over
the windows) and `bgi_pushOutOfEdges` (the full recursive routine) only share a name prefix with
`msg_closeAll_inline` / `bgi_pushOutOfEdges_inline`.

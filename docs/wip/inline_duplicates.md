# Out-of-line functions that duplicate a `static inline *_inline` helper

Scope: `src/main/` and the overlays except ovl_0b (inline helpers in `include/*.h` and in the `.c` files). Found by
comparing function bodies with parameters and locals renamed to placeholders (`build/locals/inl.py` during the
locals-naming pass), then checked by hand. Every entry below has been dealt with: either the out-of-line function now
calls its inline twin (the logic lives once, in the inline), or it keeps its own copy with a one-line comment above it
naming the inline and saying why. ovl_0b is in `inline_duplicates_0b.md`.

## Identical bodies

| out-of-line function | inline twin | status |
|---|---|---|
| `bgi_getFloorMove` (ovl_0a bgi.c) | `bgi_getFloorMove_inline` (ovl_0a.h) | calls the inline |
| `chr_testSupportBit` (main chr.c) | `chr_testSupportBit_inline` (ff9.h) | calls the inline |
| `chr_updateStats` (main chr.c) | `chr_updateStats_inline` (ff9.h) | calls the inline |
| `evt_createRegion` (main event.c) | `evt_createRegion_inline` (ff9.h) | calls the inline |
| `evt_getArg8` (main event.c) | `evt_getArg8_inline` (ff9.h) | calls the inline |
| `evt_getArg16` (main event.c) | `evt_getArg16_inline` (ff9.h) | calls the inline |
| `evt_getArg24` (main event.c) | `evt_getArg24_inline` (ff9.h) | calls the inline |
| `win_close` (main window.c) | `win_close_inline` (ff9.h) | calls the inline |
| `wm_wrapPos` (ovl_0c tu_800BA528.c) | `wm_wrapPos_inline` (ovl_0c.h) | calls the inline |
| `menuChocograph_getRequiredAbility` (ovl_32) | `menuChocograph_getRequiredAbility_inline` (ovl_32.h) | calls the inline |
| `menuSave_isSlotReady` (ovl_39) | `menuSave_isSlotReady_inline` (ovl_39.h) | calls the inline |
| `menuSave_readHeader` (ovl_39) | `menuSave_readHeader_inline` (ovl_39.c, above it) | calls the inline |
| `chr_getAp` (main chr.c) | `chr_findAbilitySlot_ap_inline` (ff9.h) | commented copy: wrapping the inline flips the `i < 0` branch layout. The inverted form (`if (i >= 0) return ...; return -1;`) matches the out-of-line function but breaks chr_isAbilityLearned, which inlines the helper |
| `chr_getApNeeded` (main chr.c) | `chr_getApNeeded_inline` (ff9.h) | commented copy, same branch flip (inverted form breaks chr_addAp and chr_isAbilityLearned) |
| `item_findSlot` (main ui.c) | `item_findSlot_inline` (ff9.h) | commented copy: a call compiles the loop exit differently; `break`/result-variable forms don't match either. Declaring `item_findSlot` itself `inline` (gcc then inlines it into item_getCount) defers its output to the end of the object, which moves it |
| `text_formatNumber` (main text.c) | `text_formatNumber_inline` (ff9.h) | commented copy: a call saves s0 later in the prologue (also with an if/else body or reordered parameters) |
| `menuAbility_getSupportState` (ovl_33 menu_ability.c) | `menuAbility_getSupportState_inline` (same file) | commented copy |
| `chr_cureStatusBit` (main tu_80053924.c) | `chr_cureStatusBit_inline` (same file, below it) | commented copy, same branch flip: the inverted test matches the out-of-line function and breaks chr_cureStatusMask |
| `chr_setAp` (main chr.c, `u8 v`) | `chr_setAp_inline` (same file, below it, `s32 v`) | commented copy |

## Near duplicates (same statements, different locals or argument preprocessing)

| out-of-line function | inline twin | status |
|---|---|---|
| `msg_getEntry` (main msg.c) | `msg_getEntry_inline` (msg.c) | calls the inline (the two unused locals are gone) |
| `ui_buildSpriteDef` (main ui.c) | `ui_buildSpriteDef_inline` (ff9.h) | calls the inline; the inline now uses `GPU_RGBC`/`get_mode`. It keeps `s = (SprtWords *)(t + 1); return (u8 *)s;`, which one of its inlined users needs |
| `evt_getObjectModelData` (main event.c) | `evt_getObjectModelData_inline` (ff9.h) | calls the inline (`return (s32)...`) |
| `menuCard_showCachedImage` (ovl_36) | `menuCard_showCachedImage_inline` (ovl_36.h) | calls the inline (the inline's RECT store order) |
| `evt_getArg8` (main event.c) | `evt_getArg8_inline2` (ff9.h) | `evt_getArg8_inline2` removed; its one user (evt_getObjectArg) matches with `evt_getArg8_inline` |
| `wm_testCellType` (ovl_0c w_movement.c) | `wm_testCellType_inline` (ovl_0c.h) | the two inlines are one now (the old `_inline2` form, taking the topography index, serves both users). The out-of-line function keeps a commented copy: a call of the inline after `WM_ATTR_TOPOGRAPHY` compiles differently |

The inline twins took over the parameter and local names the naming pass gave the out-of-line copies.

## Inline helpers that duplicated each other

- `evt_rollEncounter_rand` (ff9.h), `wm_rand8` (ovl_0c.h) and `rand8_inline` (ovl_0b.h) were the same "refill if
  exhausted, return `*g_randPtr++`" reader. Now one `rand8_inline` in ff9.h, plus `RAND8()` (the statement-expression
  form ovl_0b already had; gcc orders operands around it differently, btl_applyReflect needs it) next to it. ovl_15's
  `RND3()` is `(RAND8() % 3)`.
- Open-coded copies (refill check, then a statement reading `*g_randPtr++`) call `rand8_inline()` where the build
  matches: event.c (evt_getSysVar, evt_execInstr, evt_updateIdleTimer, evt_resetIdleTimer), item_use.c
  fieldCalc_reviveRandom, ovl_0a fld_playAnimSfx (random pitch), ovl_0c wm_controlWalk (four forest-leaf rolls), ovl_15
  bj_shoeShuffle, and the ovl_0b sites
  in `inline_duplicates_0b.md`. The ones that change register allocation with `rand8_inline()` and with `RAND8()` keep
  the open-coded form and a comment: evt_onAnimEnd, evt_stepAnim, evt_popRandomBit (main), w_frameGetParameter
  (ovl_0c, two bytes), btl_autoAttack, btl_pickRandomUnit, btl_initEnemyState (ovl_0b).
- 16-bit rolls: `RAND_REFILL2()`/`RAND16()` moved from ovl_0b.h to ff9.h. New `rand16_inline()` there is the sequenced
  form (`b = *g_randPtr++; (b << 8) | b` after the double refill): wm_updateBlockFx (three) and btl_randomizeDefense use
  it. fldmdl_updateTexAnims (ovl_0a) keeps its copy with a comment (register allocation). main item_use.c
  fieldCalc_setMagicHeal is a different shape (two single refill checks, increment after the expression) and stays.

## Not duplicates (already call their inline)

`akao_lfoTable`, `chr_findAbilitySlot`, `evt_finishTurn`, `evt_freeAllocNode`, `evt_getBattleVar`, `evt_getObject`,
`evt_resolveObjectId`, `msg_closeWindow` are one-line wrappers around their `_inline`. `msg_closeAll` (loops over
the windows) and `bgi_pushOutOfEdges` (the full recursive routine) only share a name prefix with
`msg_closeAll_inline` / `bgi_pushOutOfEdges_inline`.

## A pattern seen while doing this

When an out-of-line function's body is a call of an inline helper that returns early (`if (x) return a; return b;`),
gcc 2.8 lays the branch out the other way round than for the same body written directly; when that helper is in turn
inlined into a bigger function it keeps the direct layout. So the one-body form only works for helpers without an
early return, or where the inlined users happen to tolerate the inverted layout.

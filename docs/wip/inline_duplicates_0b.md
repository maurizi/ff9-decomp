# ovl_0b: functions that repeated a static inline helper by hand

Found while naming ovl_0b's parameters and locals; all entries are now dealt with. "Uses the helper" means the copy
was replaced by a call and `make verify` still matches; "commented" means the copy stays, with a comment above it
naming the helper and saying why. The helpers named here live in `include/ovl_0b.h` unless noted.

## rand8_inline (now in include/ff9.h, shared with main and the other overlays)

- btlseq.c btlseq_opPlayCamera: uses the helper
- btl_cmd.c btl_rollShortSummon (twice): uses the helper (`roll = rand8_inline() < 0xE6;`)
- btl_status.c btl_rollAmbush (twice): uses the helper
- btl_model.c btl_initScene (camera roll), btl_pickEnemyGroup (`* 100 / 256`): use the helper
- btl_cmd.c btl_autoAttack, btl_status.c btl_pickRandomUnit (`% n`), btl_init.c btl_initEnemyState (`% frameCount`):
  commented. Both `rand8_inline()` and `RAND8()` change the register allocation there (a `u8`-returning variant too)
- btl_status.c btl_checkSideDefeated: commented. Square's code does the refill check, then calls item_getCount, then
  reads the byte; a helper call would move the check after the call

`RAND8()` (the statement-expression form) moved to ff9.h next to `rand8_inline`.

The hand-written double-refill 16-bit rolls in btl_cmd.c (19 refill blocks), btl_init.c btl_initPartyUnit /
btl_initEnemyState and btl_model.c btl_updateTexAnims now use the `RAND_REFILL2()` / `RAND16()` macros (moved to
ff9.h); they expand to the same tokens. btl_rollCritical's four nested refill checks are `RAND_REFILL2();
RAND_REFILL2();`, as btl_cmd.c already writes them for Steal. btl_randomizeDefense uses the new `rand16_inline()`
(sequenced form). The rolls that read `*g_randPtr`, compute, and increment it statements later (btl_setPhysicalParams,
btl_setMagicParams and others) stay as written.

## btl_clearCmd_inline

- btl_cmd.c btl_clearCmd: calls the inline

## btlui helpers

- `find_target` and `BtlFindTargetCursor` were two identical inlines; `find_target` is gone and its two users
  (btlui.c) call `BtlFindTargetCursor`
- btlui.c btlui_switchTargetSide: uses `BtlFindTargetCursor` (the `goto found` scan is gone)
- btlui.c btlui_confirmDoubleCast: uses `btlui_getTargeting_inline` in both branches
- btlui.c btlui_getTargeting (the out-of-line copy): calls `btlui_getTargeting_inline`
- btlui.c btlui_popMenuUnit: uses `BtlUnitCanAct`
- btlui.c btlui_canOpenMenu: commented. A call of `BtlUnitCanAct` schedules the prologue differently

## Repeated bodies of non-inline functions

- btl_cmd.c btl_setTargetHp, btl_checkFrozenShatter: new `btl_killUnit_inline`; both call it, and btl_killUnit is
  written as a call of it
- btl_cmd.c btl_initUndeadHitRate: new `btl_initStatusHitRate_inline`, also behind btl_initStatusHitRate
- btl_cmd.c btl_cancelTranceEidolon / btl_clearCmdQueue: the eidolon reset tail is the new `btl_resetEidolon_inline`
- btl_cmd.c btl_flushCmdQueue / btl_clearCmdQueue: commented (above btl_clearCmdQueue). Moving the per-command body
  into an inline helper (with any declaration order of its four locals, or passing `&p->next`) swaps t0/t1 in both
- btl_attr.c btl_spreadTroubleDamage: new `btl_damageHp_inline`; btl_damageHp calls it, btl_spreadTroubleDamage calls
  `btl_damageHp_inline(unit, source->hpChange >> 1, 0)` per ally
- btlui.c btlui_flushMenuQueue, btlui_pruneMenuQueue: new `btlui_clearMenuQueue_inline` and
  `btlui_queueMenuUnit_inline`; btlui_clearMenuQueue / btlui_queueMenuUnit call them, the other two use them
- btlui.c btlui_updateTargetWindow: new `btlui_openTargetWindow_inline` / `btlui_closeTargetWindow_inline`;
  btlui_openTargetWindow / btlui_closeTargetWindow call them and btlui_updateTargetWindow uses them
- btl_main.c btl_loadEnemyModelsStep case 2: commented. Its walk tests the type only and appends through `ids[cnt]`,
  so it is not btl_loadSoundsStep_collect with different arguments
- btl_main.c btl_reloadPartyStep case 7 / btl_swapPartyModelStep case 6: commented at both. btl_reloadPartyStep
  takes `&pa` for the weapon afterwards, so the locals live on the stack there and a shared helper can't reproduce both

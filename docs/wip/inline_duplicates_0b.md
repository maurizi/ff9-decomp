# ovl_0b: functions that repeat a static inline helper by hand

Found while naming ovl_0b's parameters and locals. Nothing here was restructured. Some of these open-coded forms may
be needed for the match (statement order around the refill check); try the helper with ccprobe before changing one.

## rand8_inline (include/ovl_0b.h): `if (g_randPtr >= g_randTableEnd) rand_refill(); r = *g_randPtr++;`
- btlseq.c btlseq_opPlayCamera
- btl_cmd.c btl_autoAttack
- btl_cmd.c btl_rollShortSummon (twice; refill check, read, and increment as separate statements)
- btl_status.c btl_rollAmbush (twice)
- btl_status.c btl_checkSideDefeated (an item_getCount call sits between refill check and read)
- btl_status.c btl_pickRandomUnit (`*g_randPtr++ % n`)
- btl_model.c btl_initScene (camera roll)
- btl_model.c btl_pickEnemyGroup (`*g_randPtr++ * 100 / 256`)
- btl_init.c btl_initEnemyState (second roll, `% frameCount` for obj->frame)

The double-refill 16-bit rolls (btl_cmd.c, btl_updateTexAnims, btl_initPartyUnit, btl_initEnemyState) spell out the
RAND16() macro; there's no inline helper for them.

## btl_clearCmd_inline
- btl_cmd.c btl_clearCmd: the same 14 stores. It's the out-of-line copy of the helper, so it could be written as a call.

## btlui (include/ovl_0b.h helpers)
- btlui.c btlui_switchTargetSide: repeats BtlFindTargetCursor / find_target (scan for a targetable and targeted unit,
  written with `goto found`)
- btlui.c btlui_confirmDoubleCast: repeats btlui_getTargeting_inline twice, once per branch (targetType 9..11 -> 2,
  else count targetable+targeted units >= 2)
- btlui.c btlui_popMenuUnit: repeats BtlUnitCanAct (NULL / hp == 0 / cant-act status mask / g_btlRewards.expMask
  slot bit) as an ok flag
- btlui.c btlui_canOpenMenu: repeats BtlUnitCanAct (returns the expMask bit directly)

## Repeated bodies of non-inline functions (for reference)
- btl_cmd.c btl_setTargetHp, btl_checkFrozenShatter: repeat btl_killUnit (hp = 0, KILLED, death cry, idle motion)
- btl_cmd.c btl_initUndeadHitRate: repeats btl_initStatusHitRate
- btl_cmd.c btl_flushCmdQueue / btl_clearCmdQueue: same loop, differing only in the cmdNo test;
  btl_cancelTranceEidolon repeats btl_clearCmdQueue's eidolon reset tail
- btl_main.c btl_loadEnemyModelsStep case 2: repeats btl_loadSoundsStep_collect's walk (type test only, no kind test)
- btl_main.c btl_reloadPartyStep case 7 / btl_swapPartyModelStep case 6: same per-slot model/texture/motion reload
- btl_attr.c btl_spreadTroubleDamage: repeats btl_damageHp per ally
- btlui.c btlui_flushMenuQueue, btlui_pruneMenuQueue: repeat btlui_clearMenuQueue's three stores; btlui_pruneMenuQueue
  also repeats btlui_queueMenuUnit's enqueue
- btlui.c btlui_updateTargetWindow: repeats the open/close sequences of btlui_openTargetWindow / btlui_closeTargetWindow

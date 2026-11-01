# PSX_NULLABLE sites (ff9-ds NULL-read patches moved into the decomp)

`PSX_NULLABLE(p)` (include/ff9.h) is the identity here; ff9-ds defines it as `PSX_NZ` (NULL → low RAM, as the PS1
reads). Each row maps an ff9-ds `config/patches.txt` line (as of 2026-11-01) to the decomp site that now uses the
hook (commit 5b6c1261). Line numbers drift; search for the expression.

| patches.txt line: description | decomp file:line (function): expression now `PSX_NULLABLE(...)` |
|---|---|
| 81, 82: particle `owner->fieldPos` ×16 / `owner->fieldRot` ×5 | src/ovl_0b/btlfx_heap.c:246–296 (btlfx_updateParticles): `owner->fieldPos`, `owner->fieldRot` |
| 86: opcodes 0x95/0x96 window, `po` ×2 | src/main/event.c:1134,1143 (evt_execInstr): `po` as argument to `evt_execInstr_window` |
| 87: 0x2F store, `po->scale` | src/main/event.c:1468 (evt_execInstr): `po` |
| 88: `evt_getMsgLayout` `obj = PSX_NZ(obj)` | src/main/event.c:708,714,717 (evt_getMsgLayout): `obj` in the 3 `evt_placeBalloon` calls |
| 89: btl_rollAmbush, `gd->party[i]` | src/ovl_0b/btl_cmd.c:177 (btl_partyHasSupport): `gd->party[i]` |
| 92: `btl->curCmd->cmdNo` ×6 | src/ovl_0b/btl_cmd.c:966 (btl_flushCmdQueue), 997 (btl_clearCmdQueue): `btl->curCmd` |
| 94: fx_0fb_1 `it->emit20` ×2 | src/fxmod/fx_0fb_1/fx_0fb_1.c:229,301 (fx_0fb_1_op80PoolTask): `it->emit20` |
| 101–103: `g_akaoSubSeq` noise/reverb/pitchMod masks | src/main/akao_voice.c:876–878 (akao_updateVoices): `g_akaoSubSeq` |
| 107: empty item list, `slot->id` | src/ovl_0b/btlui.c:1784 (btlui_selectItem): `slot` |
| 111: `db_lookupRegistered(DB_GET_AT_IN, modelId, DB_MODEL_INFO)` ×3 | src/ovl_0a/fld_movie.c:205 (fld_setupActorModel), 250 (fld_getModelAnimId), 265 (fld_getModelTables) |
| 120, 121: field `mdl->texAnims` count/anims[2] | src/ovl_0a/fld_movie.c:496–499 (fld_drawModels): `mdl->texAnims` |
| 124: event opcodes C0/C1/C2, `mdl->texAnims` ×7 | src/main/event.c:1475–1495 (evt_execInstr): `mdl->texAnims` |
| 127, 128: anim metadata lookup | src/main/tu_80021640.c:25 (evt_getAnimFrameCount): `db_lookupRegistered(DB_GET_ADDR, animId \| DB_ANIM, 0)` |
| 131: angle/distance opcodes, `q` | src_equiv/main/script_text.c:1009 (evt_runExpr): `(EvtObject *)SCRIPT_PTR_RESTORE(evt_popValue())` |
| 134 (F37): `evt_resolveObjectId` | src_equiv/main/script_text.c:715 (evt_popValue): `evt_resolveObjectId((t >> 8) & 0xFF)` |
| 137, 138 (F38): `cur` | src/ovl_0b/btlui.c:2239,2254 (btlui_switchTargetSide): `cur` |
| 141 (F39): `actor = g_evtActor` | src/main/event.c:375 (evt_moveObjectTo): `g_evtActor` |
| 145: collision `hit->flags` | src/main/event.c:2444 (evt_movePlayer): `hit` |
| 158: `model->texAnims[2].flags` | src/ovl_0b/btl_status.c:252 (btl_removeStatus): `model->texAnims` |
| 159: `slot->model->texAnims + off` ×4 | src/ovl_0b/hi_model.c:581–588 (summon tex-anim start): `slot->model->texAnims` |
| 160: Hi_StopSummonTexAnim | src/ovl_0b/hi_model.c:592 (Hi_StopSummonTexAnim): `btlfx_getSummonModel(...)->model->texAnims` |
| 161: `model->texAnims + i` | src/ovl_0b/btlseq.c:634 (btlseq_opStopTexAnim): `model->texAnims` |
| 165: `modelSlot->model->motion` | src/ovl_0b/btl_main.c:1288 (btl_registerModel): `modelSlot->model->motion` |
| 169, 170, 222: btl_unit `cmd` (caster / cmdNo / Vivi check) | src/ovl_0b/btl_unit.c:148,157,161 (btl_setIdleMotion): `cmd` |
| 198: `unit->model->bones` | src/ovl_0b/btl_cmd.c:1313 (btl_projectUnitPos) |
| 199: `unit->model->bones` | src/ovl_0b/btlcam_o1.c:925 (btlcam_getUnitAnchors) |
| 200: `unit->model->bones` ×2 | src/ovl_0b/btlfx_op.c:1296 (btl_getBaseBonePos), 1349 (btl_getUnitBodyPos) |
| 201: `unit->model->bones` ×2 | src/ovl_0b/btl_init.c:211,213 (btl_initEnemyState) |
| 202: `owner->model->bones` | src/ovl_0b/btl_popup.c:89 (btl_newPopup) |
| 203: `unit->model->bones` ×2 | src/ovl_0b/btl_unit.c:48,50 (btl_updateUnitModel) |
| 204: `g_btlUi->menuState` | src/ovl_0b/btlui.c:151 (btlui_isTargetSelect): `g_btlUi` |
| 205: `unit->model->texAnims` ×11 | src/ovl_0b/btl_attr.c:40–43 (btl_setKoMotion), 450–462 (btl_setUnitAttr) |
| 206: `unit->model->texAnims` ×3 | src/ovl_0b/btl_init.c:265–267 (btl_resetUnit) |
| 207: `unit->model->texAnims` ×2 | src/ovl_0b/btl_status.c:253,254 (btl_removeStatus) |
| 208: `unit->model->texAnims` ×7 | src/ovl_0b/btlfx_op.c:1838–1848 (btlfx_setUnitTexAnim) |
| 209: `unit->model->texAnims` ×6 | src/ovl_0b/btlseq.c:608–612 (btlseq_opPlayTexAnim), 620–624 (btlseq_opLoopTexAnim) |
| 210: Confuse, `curCmd->caster` | src/ovl_0b/btl_status.c:391 (btl_updateUnitStatus): `g_gameState->gameData->ctx.battle->curCmd` |
| 214, 215, 216 (F36): Venom→Sleep fall-through | src/ovl_0b/btl_status.c:116,118 (btl_addStatus): `unit->model`; :117 `g_gameState->gameData->ctx.battle->curCmd` |
| 190, 191: `file_find(DIR_WORLD_IMAGES, 0x3F3/0x3F6)->sector` | src/ovl_0c/w_frame.c:873,881 (wm_initDiscImage): the `file_find(...)` result |
| 194: `nd->block->fxId` | src/ovl_0c/tu_800BA528.c:1233 (wm_updateVisibleBlocks): `nd->block` |
| 220 (F37): fld_updateEncounter | src/ovl_0a/fld_movie.c:1488 (fld_updateEncounter): `preload_waitCurrent()` |
| 221 (F37): rain init | src/ovl_0a/tu_800B0124.c:1238 (fld_enableRain): `preload_waitCurrent()` |
| F40 | include/ovl_0b.h (~5026, ~5064, inline helpers): `g_gameState->gameData->ctx.battle->curCmd` |

Not converted (keep the port patch): line 98 (hi_model.c `model->bones` via `PSX_PTR`: the base can also be the
sound-bank-overwritten value 0x3c30, not only NULL), and the non-NULL items in that area (model_link.c
`PSX_OT_OUTSIDE`, the akao.c `g_akaoOpsFE` bounds check, `port_fatal` checks in wm_main.c / btl_cmd.c,
`btlfx_startCastEmitter`, `DrawOTag`, `PORT_IS_RAM_PTR`, `psx_overlay_patch`).

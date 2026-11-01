#include "ovl_0b.h"

void btlseq_init(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    u16 i = 0;
    u16 *header = b->scene.seqTable;
    Battle11C4 *seq = &b->seq;
    u16 *p = header + 2;
    u32 *q;
    BtlMotion *motion;
    u16 n = header[0];
    for (; i < n; i++) {
        seq->seqs[i] = (s32)header + *p++;
    }
    for (; i < 0x12; i++) {
        seq->seqs[i] = 0;
    }
    q = (u32 *)p;
    seq->anims = (BtlMotion **)q;
    n = header[1];
    for (i = 0; i < n; i++, q++) {
        motion = db_lookupRegistered(DB_GET_ADDR, (*q & 0xFFFF) | DB_ANIM, 0);
        *q = (u32)motion;
        if (IS_OFFSET(motion->rotKeys)) {
            motion->rotKeys = (s32)motion + motion->rotKeys;
            if (motion->rotKeysLo != 0) {
                motion->rotKeysLo = *q + motion->rotKeysLo;
            }
        }
    }
    seq->seqFlags = (u8 *)q;
    {
        Battle122C *c = seq->slots;
        for (i = 4; i-- != 0;) {
            c->cmd = 0;
            c++;
        }
    }
}

void btl_startEnemySeq(BtlCmd *cmd) {
    if (g_gameState->gameData->ctx.battle->seq.seqs[cmd->subNo] != 0 && btlseq_start(cmd) != NULL) {
        BtlUnit *u = cmd->caster;
        if (!(u->flags & BTL_UNIT_PARTY)) {
            u->rot[1] = 0;
        }
    }
}

Battle122C *btlseq_start(BtlCmd *cmd) {
    Battle122C *e = g_gameState->gameData->ctx.battle->seq.slots;
    GameState *g;
    s16 i;
    for (i = 0; i < 4; i++, e++) {
        if (e->cmd == NULL) {
            break;
        }
    }
    if (i < 0) {
        return NULL;
    }
    g = g_gameState;
    e->flags = 0;
    e->cmd = cmd;
    e->pc = g->gameData->ctx.battle->seq.seqs[cmd->subNo];
    e->prevPc = 0;
    e->opFrame = 0;
    e->waitLeft = 0;
    e->frame = 0;
    e->seqFlags = g->gameData->ctx.battle->seq.seqFlags[cmd->subNo];
    e->soundDelay = 0;
    e->turnFrames = 0;
    e->fxDelay = 0;
    e->fadeFrames = 0;
    g_gameState->gameData->ctx.battle->cameraNo = 0;
    return e;
}

void btlseq_startDirect(u16 unitMask, s16 targets, u16 id) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;
    Battle122C *e;

    b = g_gameState->gameData->ctx.battle;
    if (b->seq.seqs[id] == 0) {
        return;
    }
    for (u = b->unitList; u != NULL; u = u->next) {
        if (u->unitBit & unitMask) {
            break;
        }
    }
    if (u != NULL) {
        c = u->cmd;
        c->cmdNo = 0;
        c->subNo = id;
        c->targets = targets;
        c->caster = u;
        c->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[id];
        c->info.w = 0;
        e = btlseq_start(c);
        if (e != NULL) {
            e->flags |= BTLSEQ_F_DIRECT;
        }
    }
}

s32 btlseq_isBusy(void) {
    s16 i;
    Battle122C *p = g_gameState->gameData->ctx.battle->seq.slots;
    for (i = 0; i < 4; i++, p++) {
        if (p->cmd != 0) {
            return 1;
        }
    }
    return 0;
}

void btlseq_update(void) {
    s16 i = 3;
    BtlScript *script = (BtlScript *)g_gameState->gameData->ctx.battle->seq.slots;
    BtlUnit *u;
    u8 op;
    s16 fxArgs[4];

    do {
        if (script->cmd != NULL) {
            script->opFrame++;
            script->waitLeft--;
            script->frame++;
            u = script->cmd->caster;
            do {
                op = *script->pc;
                if (op >= 0x23) {
                    op = 0;
                }
                if ((script->flags & BTLSEQ_F_WAIT_HIT) && g_btlfxEnv->state->seqSync != 0) {
                    script->flags = (script->flags | BTLSEQ_F_HIT) & ~BTLSEQ_F_WAIT_HIT;
                }
                if (script->pc != script->prevPc && g_btlseqOps[op].enter != NULL) {
                    g_btlseqOps[op].enter(script, u);
                }
                script->prevPc = script->pc;
            } while ((u16)g_btlseqOps[op].step(script, u));
            if (script->turnFrames != 0) {
                u->rot[1] = script->turnFrom + script->turnBy * script->turnFrame / script->turnFrames;
                if (script->turnFrame++ >= script->turnFrames) {
                    script->turnFrames = 0;
                }
            }
            if (script->soundDelay != 0) {
                if (--script->soundDelay == 0) {
                    btl_playUnitSound(u, script->sound, script->soundVol);
                }
            }
            if (script->fxDelay != 0) {
                if (--script->fxDelay == 0) {
                    btlfx_resetAll();
                    fxArgs[0] = script->fxArg;
                    fxArgs[1] = 0;
                    fxArgs[2] = 0;
                    fxArgs[3] = 0;
                    btl_playCmdEffect((BtlCmd *)script->cmd, (u16)script->fxId, fxArgs);
                }
            }
            if (script->fadeFrames != 0) {
                script->fadeLeft--;
                btl_setUnitFade(u, (script->fadeLeft << 5) / script->fadeFrames);
                if (script->fadeLeft == 0) {
                    script->fadeFrames = 0;
                }
            }
        }
        script++;
    } while (--i != -1);
}

s32 btlseq_opEnd(BtlScript *script, BtlUnit *unit) {
    BtlModel *model = unit->model;
    if (!((script->flags >> BTLSEQ_F_END_MOTION_BIT) & 1) && script->frame != 0 && unit->obj->frame >= model->motion->frameCount) {
        model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        unit->obj->frame = 0;
        script->flags |= BTLSEQ_F_END_MOTION;
    }
    if ((unit->status & (BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) || ((script->flags >> BTLSEQ_F_END_MOTION_BIT) & 1)) {
        if (g_btlfxEnv->state->phase != BTLFX_PHASE_IDLE) {
            return 0;
        }
        if (script->turnFrames != 0) {
            return 0;
        }
        if (g_gameState->gameData->ctx.battle->popups.scanUnit != 0) {
            return 0;
        }
        if (!((script->flags >> BTLSEQ_F_DIRECT_BIT) & 1)) {
            btl_onCmdEffectDone();
        }
        script->cmd = 0;
    }
    return 0;
}

s32 btlseq_opEndNoAnimWait(BtlScript *script) {
    if (g_btlfxEnv->state->phase == BTLFX_PHASE_IDLE) {
        if (script->turnFrames != 0) {
            return 0;
        }
        if (!((script->flags >> BTLSEQ_F_DIRECT_BIT) & 1)) {
            btl_onCmdEffectDone();
        }
        script->cmd = 0;
    }
    return 0;
}

void btlseq_opWait(BtlScript *script) {
    script->waitLeft = script->pc[1];
}

s32 btlseq_stepWait(BtlScript *script) {
    if (script->waitLeft <= 0) {
        script->pc += 2;
        return 1;
    }
    return 0;
}

s32 btlseq_opExecCmd(BtlScript *script) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    u16 targets = script->cmd->targets;
    while (u != NULL) {
        if (u->unitBit & targets) {
            btl_execCmd(u);
        }
        u = u->next;
    }
    script->pc += 1;
    return 1;
}

void btlseq_opMoveToTarget(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    s16 i;
    s16 dist;
    s32 angle;
    p[7] = 4;
    script->opFrame = 1;
    p[6] = script->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        *q = unit->pos[i] >> 16;
    }
    p[4] = p[1];
    btlseq_getTargetCenter(script->cmd->targets, (u16 *)&p[3], (u16 *)&p[5]);
    dist = script->pc[2] | (script->pc[3] << 8);
    if (p[0] != p[3] || p[2] != p[5]) {
        if (script->pc[0] == 0x1E) {
            p[5] -= dist;
        } else {
            angle = ratan2(p[3] - p[0], p[5] - p[2]) & 0xFFF;
            p[3] += (math_sin(angle) * dist) >> 12;
            p[5] += (math_cos(angle) * dist) >> 12;
        }
    }
}

s32 btlseq_stepMove(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    s16 i;
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        unit->pos[i] = ((q[3] - q[0]) * script->opFrame / p[6] + q[0]) << 16;
    }
    if (script->opFrame >= p[6]) {
        script->pc += (u16)p[7];
    }
    return 0;
}

void btlseq_opMoveHome(BtlScript *script, BtlUnit *unit) {
    s32 *homePos;
    s16 i;
    s16 *p = script->work;
    p[7] = 2;
    script->opFrame = 1;
    p[6] = script->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = unit->pos[i] >> 16;
        homePos = unit->home->pos;
        q[3] = homePos[i] >> 16;
    }
}

void btlseq_opMoveTo(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    s16 i;
    p[7] = 8;
    script->opFrame = 1;
    p[6] = script->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = unit->pos[i] >> 16;
        q[3] = script->pc[i * 2 + 2] | (script->pc[i * 2 + 3] << 8);
    }
}

s32 btlseq_stepMoveSetHome(s32 script, Unk800CB090 *unit) {
    s16 i;
    btlseq_stepMove((BtlScript *)script, (BtlUnit *)unit);
    for (i = 0; i < 3; i++) {
        unit->home[i] = unit->pos[i];
    }
    return 0;
}

void btlseq_opMoveBy(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    s16 i;
    p[7] = 8;
    script->opFrame = 1;
    p[6] = script->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        s16 start = unit->pos[i] >> 16;
        q[0] = start;
        q[3] = start + (script->pc[i * 2 + 2] | (script->pc[i * 2 + 3] << 8));
    }
}

BtlUnit *btlseq_findUnit(u16 unitMask) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit & unitMask) {
            break;
        }
    }
    return u;
}

void btlseq_getTargetCenter(u16 targets, u16 *outX, u16 *outZ) {
    BtlUnit *u;
    s32 n = 0;
    *outX = 0;
    *outZ = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if (u->unitBit & targets) {
            *outX += u->pos[0] >> 16;
            n++;
            *outZ += u->pos[2] >> 16;
        }
    }
    if (n >= 2) {
        *outX = (s16)*outX / n;
        *outZ = (s16)*outZ / n;
    }
}

s32 btlseq_opPlayAnim(BtlScript *script, BtlUnit *unit) {
    u32 flags;
    u8 *p = script->pc;
    s32 idx;
    if (p[1] == 0xFF) {
        flags = unit->flags;
        unit->model->motion = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(flags)].type->motions[(flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
    } else {
        idx = script->seqFlags + p[1];
        unit->model->motion = g_gameState->gameData->ctx.battle->seq.anims[idx];
    }
    unit->obj->frame = 0;
    script->frame = 0;
    script->pc += 2;
    return 1;
}

s32 btlseq_opDelayedEffect(BtlScript *script) {
    script->fxId = script->pc[1] | (script->pc[2] << 8);
    script->fxArg = script->pc[3];
    script->fxDelay = script->pc[4] + 1;
    script->pc += 5;
    return 1;
}

s32 btlseq_opEffect(BtlScript *script) {
    s16 fxArgs[4];
    s32 effectId;
    Unk800F78A40 *state = g_btlfxEnv->state;
    if ((state->phase != BTLFX_PHASE_IDLE) & !((state->flags >> 15) & 1)) {
        return 0;
    }
    effectId = script->pc[1] | (script->pc[2] << 8);
    fxArgs[0] = script->pc[3] | (script->pc[4] << 8);
    fxArgs[1] = script->pc[5] | (script->pc[6] << 8);
    fxArgs[2] = script->pc[7] | (script->pc[8] << 8);
    fxArgs[3] = script->pc[0] == 0x1A;
    btl_playCmdEffect((BtlCmd *)script->cmd, effectId, fxArgs);
    script->flags |= BTLSEQ_F_WAIT_HIT;
    script->pc += 9;
    return 1;
}

s32 btlseq_opWaitAnim(BtlScript *script, BtlUnit *unit) {
    BtlModel *model = unit->model;
    BtlUnitA0 *obj = unit->obj;
    if (script->frame != 0 && (obj->frame >= model->motion->frameCount || (unit->status & (BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)))) {
        g_btlfxEnv->camera->paused = 0;
        script->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opWaitEffectHit(BtlScript *script) {
    if ((script->flags >> BTLSEQ_F_HIT_BIT) & 1) {
        script->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opReleaseEffect(BtlScript *script) {
    g_btlfxEnv->state->seqRelease = 1;
    script->pc += 1;
    return 1;
}

s32 btlseq_opWaitEffect(BtlScript *script) {
    Unk800F78A40 *state = g_btlfxEnv->state;
    if ((state->phase == BTLFX_PHASE_IDLE) | ((state->flags >> 15) & 1)) {
        script->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opHideMeshes(BtlScript *script, BtlUnit *unit) {
    unit->hiddenMeshes |= script->pc[1] | (script->pc[2] << 8);
    btl_hideMeshes(unit, script->pc[1] | (script->pc[2] << 8));
    script->pc += 3;
    return 1;
}

s32 btlseq_opShowMeshes(BtlScript *script, BtlUnit *unit) {
    unit->hiddenMeshes &= ~(script->pc[1] | (script->pc[2] << 8));
    btl_showMeshes(unit, script->pc[1] | (script->pc[2] << 8));
    script->pc += 3;
    return 1;
}

s32 btlseq_opShowName(BtlScript *script, BtlUnit *unit) {
    u8 *pc;
    Battle *b;
    u8 *txt;
    u16 idx;
    u16 ofs;

    pc = script->pc;
    idx = pc[1];
    b = g_gameState->gameData->ctx.battle;
    txt = b->work->texts;
    if (idx & 0x80) {
        btl_setBannerAt((s32)(txt + script->cmd->ability->nameOfs), 1);
    } else {
        idx += b->enemies[BTL_UNIT_INDEX(unit->flags)].type->firstAbility;
        ofs = ((u16 *)txt)[idx * 2 + 2] + 4;
        if (pc[0] == 0x21) {
            btl_setBannerAt((s32)(txt + ofs), 3);
        } else {
            btl_setBanner((s32)(txt + ofs), 3);
        }
    }
    script->pc += 2;
    return 1;
}

s32 btlseq_opSetStance(BtlScript *script, BtlUnit *unit) {
    unit->flags = (unit->flags & ~BTL_UNIT_WEAK_STANCE) | ((script->pc[1] & 1) << BTL_UNIT_WEAK_STANCE_BIT);
    script->pc += 2;
    return 1;
}

s32 btlseq_opSetCamera(BtlScript *script) {
    g_gameState->gameData->ctx.battle->cameraNo = script->pc[1];
    script->pc += 2;
    return 1;
}

s32 btlseq_opPlayCamera(BtlScript *script, BtlUnit *unit) {
    Battle *b = g_gameState->gameData->ctx.battle;
    Battle11C4 *seq = &b->seq;
    BtlUnit *u;
    u32 status;
    s32 roll;
    s8 one;
    if (script->pc[0] != 0x20 && !script->cmd->ability->forceCamera) {
        if (((g_gameState->config >> CFG_BIT_CAMERA) & (one = 1)) == one) {
            goto end;
        }
        status = unit->status;
        for (u = b->unitList; u != NULL; u = u->next) {
            if (u->unitBit & script->cmd->targets) {
                status |= u->status;
            }
        }
        if (status & (BTL_STATUS_VANISH | BTL_STATUS_MINI)) {
            goto end;
        }
        roll = rand8_inline();
        if (roll >= 0x80) {
            goto end;
        }
    }
    btlseq_getTargetCenter(script->cmd->targets, &seq->camCenterX, &seq->camCenterZ);
    seq->camCenterY = 0;
    seq->camCaster = unit;
    seq->camTarget = (s32)btlseq_findUnit(script->cmd->targets);
    g_gameState->gameData->ctx.battle->cameraNo = script->pc[1];
    btlcam_playSeq(g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->camera);
end:
    script->pc += 2;
    return 1;
}

s32 btlseq_opTurn(BtlScript *script, BtlUnit *unit) {
    u8 *pc = script->pc;
    s32 angleArg;
    s16 x, z;
    u16 ang;
    u16 angleOfs;
    s16 center[2];
    s32 diff;
    s16 rot;

    angleArg = pc[1] | (pc[2] << 8);
    ang = angleArg;
    angleOfs = pc[3] | (pc[4] << 8);
    script->turnFrames = pc[5];
    rot = unit->rot[1];
    script->turnFrame = 1;
    script->turnFrom = rot;
    if (angleArg & 0x8000) {
        switch (angleArg & 0x7FFF) {
        case 0:
            ang = unit->obj->rotY;
            break;
        case 1:
            btlseq_getTargetCenter(script->cmd->targets, (u16 *)&center[0], (u16 *)&center[1]);
            x = (unit->pos[0] >> 16) - center[0];
            z = (unit->pos[2] >> 16) - center[1];
            center[0] = x;
            center[1] = z;
            if (x == 0 && z == 0) {
                ang = unit->rot[1];
            } else {
                ang = ratan2(center[0], center[1]);
            }
            break;
        default:
            ang = unit->rot[1];
            break;
        }
    }
    ang = (ang + angleOfs) & 0xFFF;
    diff = ((u16)unit->rot[1] - ang) & 0xFFF;
    if (diff > 0x800) {
        script->turnBy = 0x1000 - diff;
    } else {
        script->turnBy = -diff;
    }
    script->pc += 6;
    return 1;
}

void btlseq_opScale(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    s16 curScale = unit->model->scaleX;
    s16 factor;
    s16 targetScale;
    p[0] = curScale;
    factor = script->pc[1] | (script->pc[2] << 8);
    if (factor == -1) {
        targetScale = 0x1000;
    } else {
        targetScale = curScale * factor / 0x1000;
    }
    p[1] = targetScale - p[0];
    script->opFrame = 1;
    p[2] = script->pc[3];
}

s32 btlseq_stepScale(BtlScript *script, BtlUnit *unit) {
    s16 *p = script->work;
    u16 scale = script->work[0] + p[1] * script->opFrame / p[2];
    btl_setUnitAttr((s32)unit, BTL_ATTR_SCALE, scale);
    if (scale == 0x1000) {
        unit->model->flags &= ~BTL_MODEL_SCALED;
    }
    if (script->opFrame >= p[2]) {
        script->pc += 4;
        return 1;
    }
    return 0;
}

s32 btlseq_opPlayTexAnim(BtlScript *script, BtlUnit *unit) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &unit->model->texAnims[script->pc[1]];
    e->flags |= BTL_TEXANIM_PLAY;
    g = &unit->model->texAnims[script->pc[1]];
    g->pos = 0;
    e = &unit->model->texAnims[script->pc[1]];
    e->lastFrame = 0x1000;
    script->pc += 2;
    return 1;
}

s32 btlseq_opLoopTexAnim(BtlScript *script, BtlUnit *unit) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &unit->model->texAnims[script->pc[1]];
    e->flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    g = &unit->model->texAnims[script->pc[1]];
    g->pos = 0;
    e = &unit->model->texAnims[script->pc[1]];
    e->lastFrame = 0x1000;
    script->pc += 2;
    return 1;
}

s32 btlseq_opStopTexAnim(BtlScript *script, BtlUnit *unit) {
    u8 i = script->pc[1];
    BtlModel *m = unit->model;
    BtlUnit9C50 *e;
    (*(e = m->texAnims + i)).flags &= 0xFC;
    script->pc += 2;
    return 1;
}

s32 btlseq_opDelayedSound(BtlScript *script) {
    script->sound = script->pc[1] | (script->pc[2] << 8);
    script->soundDelay = script->pc[3] + 1;
    script->soundVol = script->pc[5];
    script->pc += 6;
    return 1;
}

s32 btlseq_opSetBaseBone(BtlScript *script, BtlUnit *unit) {
    unit->baseBone = script->pc[1];
    script->pc += 2;
    return 1;
}

s32 btlseq_opFadeOut(BtlScript *script, BtlUnit *unit) {
    script->fadeFrames = script->fadeLeft = script->pc[1];
    script->pc += 2;
    return 1;
}

s32 btlseq_opShowShadow(BtlScript *script, BtlUnit *unit) {
    unit->flags = (unit->flags & ~BTL_UNIT_SHADOW) | ((script->pc[1] & 1) << BTL_UNIT_SHADOW_BIT);
    script->pc += 2;
    return 1;
}

void btl_clearPopups(void) {
    s16 i;
    BtlPopup *p;
    g_gameState->gameData->ctx.battle->popups.last = 0;
    g_gameState->gameData->ctx.battle->popups.tick = 0;
    g_gameState->gameData->ctx.battle->popups.hiddenMask = 0xFF;
    p = g_gameState->gameData->ctx.battle->popups.entries;
    for (i = 15; i != -1; i--) {
        p->unit = NULL;
        p++;
    }
    g_gameState->gameData->ctx.battle->popups.scanUnit = 0;
    g_gameState->gameData->ctx.battle->popups.detectEnemy = 0;
}

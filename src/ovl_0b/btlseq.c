#include "ovl_0b.h"

void btlseq_init(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    u16 i = 0;
    u16 *h = b->scene.seqTable;
    Battle11C4 *t = &b->seq;
    u16 *p = h + 2;
    u32 *q;
    BtlUnit9CC *m;
    u16 n = h[0];
    for (; i < n; i++) {
        t->seqs[i] = (s32)h + *p++;
    }
    for (; i < 0x12; i++) {
        t->seqs[i] = 0;
    }
    q = (u32 *)p;
    t->anims = (BtlUnit9CC **)q;
    n = h[1];
    for (i = 0; i < n; i++, q++) {
        m = db_lookupRegistered(4, (*q & 0xFFFF) | 0x30000, 0);
        *q = (u32)m;
        if (IS_OFFSET(m->rotKeys)) {
            m->rotKeys = (s32)m + m->rotKeys;
            if (m->rotKeysLo != 0) {
                m->rotKeysLo = *q + m->rotKeysLo;
            }
        }
    }
    t->seqFlags = (u8 *)q;
    {
        Battle122C *c = t->slots;
        for (i = 4; i-- != 0;) {
            c->cmd = 0;
            c++;
        }
    }
}

void btl_startEnemySeq(BtlCmd *arg0) {
    if (g_gameState->gameData->ctx.battle->seq.seqs[arg0->subNo] != 0 && btlseq_start(arg0) != NULL) {
        BtlUnit *u = arg0->caster;
        if (!(u->flags & 1)) {
            u->rot[1] = 0;
        }
    }
}

Battle122C *btlseq_start(BtlCmd *arg0) {
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
    e->cmd = arg0;
    e->pc = g->gameData->ctx.battle->seq.seqs[arg0->subNo];
    e->prevPc = 0;
    e->opFrame = 0;
    e->waitLeft = 0;
    e->frame = 0;
    e->seqFlags = g->gameData->ctx.battle->seq.seqFlags[arg0->subNo];
    e->soundDelay = 0;
    e->turnFrames = 0;
    e->fxDelay = 0;
    e->fadeFrames = 0;
    g_gameState->gameData->ctx.battle->cameraNo = 0;
    return e;
}

void btlseq_startDirect(u16 mask, s16 arg1, u16 id) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;
    Battle122C *e;

    b = g_gameState->gameData->ctx.battle;
    if (b->seq.seqs[id] == 0) {
        return;
    }
    for (u = b->unitList; u != NULL; u = u->next) {
        if (u->unitBit & mask) {
            break;
        }
    }
    if (u != NULL) {
        c = u->cmd;
        c->cmdNo = 0;
        c->subNo = id;
        c->targets = arg1;
        c->caster = u;
        c->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[id];
        c->info.w = 0;
        e = btlseq_start(c);
        if (e != NULL) {
            e->flags |= 8;
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
    BtlScript *s = (BtlScript *)g_gameState->gameData->ctx.battle->seq.slots;
    BtlUnit *u;
    u8 op;
    s16 v[4];

    do {
        if (s->cmd != NULL) {
            s->opFrame++;
            s->waitLeft--;
            s->frame++;
            u = s->cmd->caster;
            do {
                op = *s->pc;
                if (op >= 0x23) {
                    op = 0;
                }
                if ((s->flags & 1) && g_btlfxEnv->state->seqSync != 0) {
                    s->flags = (s->flags | 2) & ~1;
                }
                if (s->pc != s->prevPc && g_btlseqOps[op].enter != NULL) {
                    g_btlseqOps[op].enter(s, u);
                }
                s->prevPc = s->pc;
            } while ((u16)g_btlseqOps[op].step(s, u));
            if (s->turnFrames != 0) {
                u->rot[1] = s->turnFrom + s->turnBy * s->turnFrame / s->turnFrames;
                if (s->turnFrame++ >= s->turnFrames) {
                    s->turnFrames = 0;
                }
            }
            if (s->soundDelay != 0) {
                if (--s->soundDelay == 0) {
                    btl_playUnitSound(u, s->sound, s->soundVol);
                }
            }
            if (s->fxDelay != 0) {
                if (--s->fxDelay == 0) {
                    btlfx_resetAll();
                    v[0] = s->fxArg;
                    v[1] = 0;
                    v[2] = 0;
                    v[3] = 0;
                    btl_playCmdEffect((BtlCmd *)s->cmd, (u16)s->fxId, v);
                }
            }
            if (s->fadeFrames != 0) {
                s->fadeLeft--;
                btl_setUnitFade(u, (s->fadeLeft << 5) / s->fadeFrames);
                if (s->fadeLeft == 0) {
                    s->fadeFrames = 0;
                }
            }
        }
        s++;
    } while (--i != -1);
}

s32 btlseq_opEnd(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->model;
    if (!((arg0->flags >> 2) & 1) && arg0->frame != 0 && arg1->obj->frame >= m->motion->frameCount) {
        m->motion = (BtlUnit9CC *)arg1->motions[(arg1->flags >> 18) & 1];
        arg1->obj->frame = 0;
        arg0->flags |= 4;
    }
    if ((arg1->status & (BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) || ((arg0->flags >> 2) & 1)) {
        if (g_btlfxEnv->state->phase != -1) {
            return 0;
        }
        if (arg0->turnFrames != 0) {
            return 0;
        }
        if (g_gameState->gameData->ctx.battle->popups.scanUnit != 0) {
            return 0;
        }
        if (!((arg0->flags >> 3) & 1)) {
            btl_onCmdEffectDone();
        }
        arg0->cmd = 0;
    }
    return 0;
}

s32 btlseq_opEndNoAnimWait(BtlScript *arg0) {
    if (g_btlfxEnv->state->phase == -1) {
        if (arg0->turnFrames != 0) {
            return 0;
        }
        if (!((arg0->flags >> 3) & 1)) {
            btl_onCmdEffectDone();
        }
        arg0->cmd = 0;
    }
    return 0;
}

void btlseq_opWait(BtlScript *arg0) {
    arg0->waitLeft = arg0->pc[1];
}

s32 btlseq_stepWait(BtlScript *arg0) {
    if (arg0->waitLeft <= 0) {
        arg0->pc += 2;
        return 1;
    }
    return 0;
}

s32 btlseq_opExecCmd(BtlScript *arg0) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    u16 mask = arg0->cmd->targets;
    while (u != NULL) {
        if (u->unitBit & mask) {
            btl_execCmd(u);
        }
        u = u->next;
    }
    arg0->pc += 1;
    return 1;
}

void btlseq_opMoveToTarget(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    s16 i;
    s16 spd;
    s32 a;
    p[7] = 4;
    arg0->opFrame = 1;
    p[6] = arg0->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        *q = arg1->pos[i] >> 16;
    }
    p[4] = p[1];
    btlseq_getTargetCenter(arg0->cmd->targets, (u16 *)&p[3], (u16 *)&p[5]);
    spd = arg0->pc[2] | (arg0->pc[3] << 8);
    if (p[0] != p[3] || p[2] != p[5]) {
        if (arg0->pc[0] == 0x1E) {
            p[5] -= spd;
        } else {
            a = ratan2(p[3] - p[0], p[5] - p[2]) & 0xFFF;
            p[3] += (math_sin(a) * spd) >> 12;
            p[5] += (math_cos(a) * spd) >> 12;
        }
    }
}

s32 btlseq_stepMove(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    s16 i;
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        arg1->pos[i] = ((q[3] - q[0]) * arg0->opFrame / p[6] + q[0]) << 16;
    }
    if (arg0->opFrame >= p[6]) {
        arg0->pc += (u16)p[7];
    }
    return 0;
}

void btlseq_opMoveHome(BtlScript *arg0, BtlUnit *arg1) {
    s32 *v;
    s16 i;
    s16 *p = arg0->work;
    p[7] = 2;
    arg0->opFrame = 1;
    p[6] = arg0->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = arg1->pos[i] >> 16;
        v = arg1->home->pos;
        q[3] = v[i] >> 16;
    }
}

void btlseq_opMoveTo(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    s16 i;
    p[7] = 8;
    arg0->opFrame = 1;
    p[6] = arg0->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = arg1->pos[i] >> 16;
        q[3] = arg0->pc[i * 2 + 2] | (arg0->pc[i * 2 + 3] << 8);
    }
}

s32 btlseq_stepMoveSetHome(s32 arg0, Unk800CB090 *arg1) {
    s16 i;
    btlseq_stepMove((BtlScript *)arg0, (BtlUnit *)arg1);
    for (i = 0; i < 3; i++) {
        arg1->home[i] = arg1->pos[i];
    }
    return 0;
}

void btlseq_opMoveBy(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    s16 i;
    p[7] = 8;
    arg0->opFrame = 1;
    p[6] = arg0->pc[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        s16 v = arg1->pos[i] >> 16;
        q[0] = v;
        q[3] = v + (arg0->pc[i * 2 + 2] | (arg0->pc[i * 2 + 3] << 8));
    }
}

BtlUnit *btlseq_findUnit(u16 arg0) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit & arg0) {
            break;
        }
    }
    return u;
}

void btlseq_getTargetCenter(u16 arg0, u16 *arg1, u16 *arg2) {
    BtlUnit *u;
    s32 n = 0;
    *arg1 = 0;
    *arg2 = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if (u->unitBit & arg0) {
            *arg1 += u->pos[0] >> 16;
            n++;
            *arg2 += u->pos[2] >> 16;
        }
    }
    if (n >= 2) {
        *arg1 = (s16)*arg1 / n;
        *arg2 = (s16)*arg2 / n;
    }
}

s32 btlseq_opPlayAnim(BtlScript *arg0, BtlUnit *arg1) {
    u32 f;
    u8 *p = arg0->pc;
    s32 idx;
    if (p[1] == 0xFF) {
        f = arg1->flags;
        arg1->model->motion = g_gameState->gameData->ctx.battle->enemies[(f >> 1) & 0xF].type->motions[(f >> 18) & 1];
    } else {
        idx = arg0->seqFlags + p[1];
        arg1->model->motion = g_gameState->gameData->ctx.battle->seq.anims[idx];
    }
    arg1->obj->frame = 0;
    arg0->frame = 0;
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opDelayedEffect(BtlScript *arg0) {
    arg0->fxId = arg0->pc[1] | (arg0->pc[2] << 8);
    arg0->fxArg = arg0->pc[3];
    arg0->fxDelay = arg0->pc[4] + 1;
    arg0->pc += 5;
    return 1;
}

s32 btlseq_opEffect(BtlScript *arg0) {
    s16 v[4];
    s32 id;
    Unk800F78A40 *e = g_btlfxEnv->state;
    if ((e->phase != -1) & !((e->flags >> 15) & 1)) {
        return 0;
    }
    id = arg0->pc[1] | (arg0->pc[2] << 8);
    v[0] = arg0->pc[3] | (arg0->pc[4] << 8);
    v[1] = arg0->pc[5] | (arg0->pc[6] << 8);
    v[2] = arg0->pc[7] | (arg0->pc[8] << 8);
    v[3] = arg0->pc[0] == 0x1A;
    btl_playCmdEffect((BtlCmd *)arg0->cmd, id, v);
    arg0->flags |= 1;
    arg0->pc += 9;
    return 1;
}

s32 btlseq_opWaitAnim(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->model;
    BtlUnitA0 *a = arg1->obj;
    if (arg0->frame != 0 && (a->frame >= m->motion->frameCount || (arg1->status & (BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)))) {
        g_btlfxEnv->camera->paused = 0;
        arg0->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opWaitEffectHit(BtlScript *arg0) {
    if ((arg0->flags >> 1) & 1) {
        arg0->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opReleaseEffect(BtlScript *arg0) {
    g_btlfxEnv->state->seqRelease = 1;
    arg0->pc += 1;
    return 1;
}

s32 btlseq_opWaitEffect(BtlScript *arg0) {
    Unk800F78A40 *p = g_btlfxEnv->state;
    if ((p->phase == -1) | ((p->flags >> 15) & 1)) {
        arg0->pc += 1;
        return 1;
    }
    return 0;
}

s32 btlseq_opHideMeshes(BtlScript *arg0, BtlUnit *arg1) {
    arg1->hiddenMeshes |= arg0->pc[1] | (arg0->pc[2] << 8);
    btl_hideMeshes(arg1, arg0->pc[1] | (arg0->pc[2] << 8));
    arg0->pc += 3;
    return 1;
}

s32 btlseq_opShowMeshes(BtlScript *arg0, BtlUnit *arg1) {
    arg1->hiddenMeshes &= ~(arg0->pc[1] | (arg0->pc[2] << 8));
    btl_showMeshes(arg1, arg0->pc[1] | (arg0->pc[2] << 8));
    arg0->pc += 3;
    return 1;
}

s32 btlseq_opShowName(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc;
    Battle *b;
    u8 *txt;
    u16 idx;
    u16 ofs;

    pc = arg0->pc;
    idx = pc[1];
    b = g_gameState->gameData->ctx.battle;
    txt = b->work->texts;
    if (idx & 0x80) {
        btl_setBannerAt((s32)(txt + arg0->cmd->ability->nameOfs), 1);
    } else {
        idx += b->enemies[(arg1->flags >> 1) & 0xF].type->firstAbility;
        ofs = ((u16 *)txt)[idx * 2 + 2] + 4;
        if (pc[0] == 0x21) {
            btl_setBannerAt((s32)(txt + ofs), 3);
        } else {
            btl_setBanner((s32)(txt + ofs), 3);
        }
    }
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opSetStance(BtlScript *arg0, BtlUnit *arg1) {
    arg1->flags = (arg1->flags & ~0x40000) | ((arg0->pc[1] & 1) << 18);
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opSetCamera(BtlScript *arg0) {
    g_gameState->gameData->ctx.battle->cameraNo = arg0->pc[1];
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opPlayCamera(BtlScript *arg0, BtlUnit *arg1) {
    Battle *b = g_gameState->gameData->ctx.battle;
    Battle11C4 *s = &b->seq;
    BtlUnit *u;
    u32 f;
    s32 r;
    s8 one;
    if (arg0->pc[0] != 0x20 && !((arg0->cmd->ability->unk0_17 >> 13) & 1)) {
        if (((g_gameState->config >> 4) & (one = 1)) == one) {
            goto end;
        }
        f = arg1->status;
        for (u = b->unitList; u != NULL; u = u->next) {
            if (u->unitBit & arg0->cmd->targets) {
                f |= u->status;
            }
        }
        if (f & 0x14000000) {
            goto end;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        if (r >= 0x80) {
            goto end;
        }
    }
    btlseq_getTargetCenter(arg0->cmd->targets, &s->camCenterX, &s->camCenterZ);
    s->camCenterY = 0;
    s->camCaster = arg1;
    s->camTarget = (s32)btlseq_findUnit(arg0->cmd->targets);
    g_gameState->gameData->ctx.battle->cameraNo = arg0->pc[1];
    btlcam_playSeq(g_gameState->gameData->ctx.battle->enemies[(arg1->flags >> 1) & 0xF].type->camera);
end:
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opTurn(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc = arg0->pc;
    s32 a;
    s16 x, z;
    u16 ang;
    u16 b;
    s16 d[2];
    s32 v;
    s16 w;

    a = pc[1] | (pc[2] << 8);
    ang = a;
    b = pc[3] | (pc[4] << 8);
    arg0->turnFrames = pc[5];
    w = arg1->rot[1];
    arg0->turnFrame = 1;
    arg0->turnFrom = w;
    if (a & 0x8000) {
        switch (a & 0x7FFF) {
        case 0:
            ang = arg1->obj->rotY;
            break;
        case 1:
            btlseq_getTargetCenter(arg0->cmd->targets, (u16 *)&d[0], (u16 *)&d[1]);
            x = (arg1->pos[0] >> 16) - d[0];
            z = (arg1->pos[2] >> 16) - d[1];
            d[0] = x;
            d[1] = z;
            if (x == 0 && z == 0) {
                ang = arg1->rot[1];
            } else {
                ang = ratan2(d[0], d[1]);
            }
            break;
        default:
            ang = arg1->rot[1];
            break;
        }
    }
    ang = (ang + b) & 0xFFF;
    v = ((u16)arg1->rot[1] - ang) & 0xFFF;
    if (v > 0x800) {
        arg0->turnBy = 0x1000 - v;
    } else {
        arg0->turnBy = -v;
    }
    arg0->pc += 6;
    return 1;
}

void btlseq_opScale(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    s16 a = arg1->model->scaleX;
    s16 v;
    s16 r;
    p[0] = a;
    v = arg0->pc[1] | (arg0->pc[2] << 8);
    if (v == -1) {
        r = 0x1000;
    } else {
        r = a * v / 0x1000;
    }
    p[1] = r - p[0];
    arg0->opFrame = 1;
    p[2] = arg0->pc[3];
}

s32 btlseq_stepScale(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->work;
    u16 v = arg0->work[0] + p[1] * arg0->opFrame / p[2];
    btl_setUnitAttr((s32)arg1, BTL_ATTR_SCALE, v);
    if (v == 0x1000) {
        arg1->model->flags &= ~2;
    }
    if (arg0->opFrame >= p[2]) {
        arg0->pc += 4;
        return 1;
    }
    return 0;
}

s32 btlseq_opPlayTexAnim(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->model->texAnims[arg0->pc[1]];
    e->flags |= 1;
    g = &arg1->model->texAnims[arg0->pc[1]];
    g->pos = 0;
    e = &arg1->model->texAnims[arg0->pc[1]];
    e->lastFrame = 0x1000;
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opLoopTexAnim(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->model->texAnims[arg0->pc[1]];
    e->flags |= 3;
    g = &arg1->model->texAnims[arg0->pc[1]];
    g->pos = 0;
    e = &arg1->model->texAnims[arg0->pc[1]];
    e->lastFrame = 0x1000;
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opStopTexAnim(BtlScript *arg0, BtlUnit *arg1) {
    u8 i = arg0->pc[1];
    BtlUnit9C *m = arg1->model;
    BtlUnit9C50 *e;
    (*(e = m->texAnims + i)).flags &= 0xFC;
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opDelayedSound(BtlScript *arg0) {
    arg0->sound = arg0->pc[1] | (arg0->pc[2] << 8);
    arg0->soundDelay = arg0->pc[3] + 1;
    arg0->soundVol = arg0->pc[5];
    arg0->pc += 6;
    return 1;
}

s32 btlseq_opSetBaseBone(BtlScript *arg0, BtlUnit *arg1) {
    arg1->baseBone = arg0->pc[1];
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opFadeOut(BtlScript *arg0, BtlUnit *arg1) {
    arg0->fadeFrames = arg0->fadeLeft = arg0->pc[1];
    arg0->pc += 2;
    return 1;
}

s32 btlseq_opShowShadow(BtlScript *arg0, BtlUnit *arg1) {
    arg1->flags = (arg1->flags & ~0x1000000) | ((arg0->pc[1] & 1) << 24);
    arg0->pc += 2;
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

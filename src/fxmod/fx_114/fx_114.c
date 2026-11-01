#include "fxmod/fx_114.h"

static s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_114_pinPoints[0], frame, n);
}

static s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_114_pinPoints[1], frame, n);
}

static s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_114_pinPoints[2], frame, n);
}

static s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_114_pinPoints[3], frame, n);
}

static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

static void fxm_buildPathTrail(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR pos;
    SVECTOR *pt;
    s32 count;
    s32 i;
    s32 keyIdx;
    s32 next;

    count = (size - 4) / 8;
    pt = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = count - 1;
    buf[1] = 0;
    if (n < count) {
        for (i = 0; i < count - n; i++) {
            pt->pad = 0;
            pt++;
        }
    } else {
        n = count;
    }
    for (i = 0; i < n; i++) {
        next = i + 1;
        keyIdx = n - next + start;
        if (clip != 0 && keyIdx >= keys->count2) {
            break;
        }
        fxm_posPathGet(keys, keyIdx, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        pt->pad = from + (to - from) / keys->count2 * (n - i);
        pt++;
    }
}

s32 fx_114_op80(s32 mode, Fx114Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v40;
    SVECTOR v48;
    SVECTOR v50;
    SVECTOR v58;
    SVECTOR r;
    VECTOR s;
    CVECTOR col;
    s32 frame;
    s32 i;
    s32 q;
    s32 k;
    s32 c;
    s32 t;
    DR_STP *dp;
    s32 n;
    s32 q1;
    s32 k1;
    s32 t1;
    s32 q2;
    s32 k2;
    s32 k3;
    s32 k4;

    if (mode == 0) {
        *args = 0x198;
    } else if (mode == 1) {
        fx_114_env = env;
        ex->btlfx_initEffModels(fx_114_effModels, 0x20);
        work->rot20.vx = 0;
        work->rot20.vy = 0x800;
        work->rot20.vz = 0;
        work->pos28.vx = 0;
        work->pos28.vy = 0;
        work->pos28.vz = -0x4B0;
        work->scale38.vx = 0x1800;
        work->scale38.vy = 0x1800;
        work->scale38.vz = 0x1800;
        work->rot4C.pad = 1;
        work->rot4C.vx = 0x400;
        work->rot4C.vy = 0;
        work->rot4C.vz = 0;
        work->pos54.vx = 0;
        work->pos54.vy = 0;
        work->pos54.vz = -0x4B0;
        work->scale5C = 0x1000;
        work->rot60.vx = 0;
        work->rot60.vy = 0;
        work->rot60.vz = 0;
        work->pos68.vx = 0;
        work->pos68.vy = 0;
        work->pos68.vz = 0;
        work->scale78.vx = 0x1000;
        work->scale78.vy = 0x1000;
        work->scale78.vz = 0x1000;
        fxm_vecPathInit(&fx_114_vecPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x95));
        work->model178 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 1);
        work->rot88.vx = 0;
        work->rot88.vy = 0;
        work->rot88.vz = 0;
        work->pos90.vx = 0;
        work->pos90.vy = 0;
        work->pos90.vz = -0x4B0;
        work->scaleA0.vx = 0x1000;
        work->scaleA0.vy = 0x1000;
        work->scaleA0.vz = 0x1000;
        fxm_vecTrackInit(&fx_114_vecTrack3, (u16 *)ex->btlfx_getFileData(entry->file, 0x84));
        work->model17A = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x85));
        work->rotB4.vx = 0;
        work->rotB4.vy = 0;
        work->rotB4.vz = 0;
        work->posBC.vx = 0;
        work->posBC.vy = 0x50;
        work->posBC.vz = -0x4B0;
        work->scaleCC.vx = 0x1000;
        work->scaleCC.vy = 0x1000;
        work->scaleCC.vz = 0x1000;
        fxm_vecTrackInit(&fx_114_vecTrack0, (u16 *)ex->btlfx_getFileData(entry->file, 0x86));
        work->model17C = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->file, 0x81), fx_114_texInfo0, fx_114_texPos0);
        work->rotE0.vx = 0;
        work->rotE0.vy = 0;
        work->rotE0.vz = 0;
        work->posE8.vx = 0;
        work->posE8.vy = -0xC8;
        work->posE8.vz = -0x4B0;
        work->scaleF8.vx = 0x1000;
        work->scaleF8.vy = 0x1000;
        work->scaleF8.vz = 0x1000;
        fxm_vecTrackInit(&fx_114_vecTrack1, (u16 *)ex->btlfx_getFileData(entry->file, 0x87));
        work->model17E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x82), 0x9A, 0x3DC0, 0, 0);
        work->rot10C.vx = 0;
        work->rot10C.vy = 0;
        work->rot10C.vz = 0;
        work->pos114.vx = 0;
        work->pos114.vy = -0xC8;
        work->pos114.vz = -0x4B0;
        work->scale124.vx = 0x1000;
        work->scale124.vy = 0x1000;
        work->scale124.vz = 0x1000;
        fxm_vecTrackInit(&fx_114_vecTrack2, (u16 *)ex->btlfx_getFileData(entry->file, 0x88));
        work->model180 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x83), 0x117, 0, 0, 0);
        work->color15C = 0;
        work->color15D = 0;
        work->color15E = 0;
        work->frame16C = 0;
        work->timer16E = -1;
        work->timer170 = -1;
        work->timer172 = -1;
        work->timer174 = -1;
        work->timer176 = -1;
    } else {
    frame = entry->frame;
    switch (frame) {
    case 0:
        work->index160 = 0;
        work->loopMotion164 = 1;
        work->frame168 = 0x10;
        fxm_posPathInit(&fx_114_posPath0, (u16 *)ex->btlfx_getFileData(entry->file, 0x96));
        fxm_posPathInit(&fx_114_posPath1, (u16 *)ex->btlfx_getFileData(entry->file, 0x8A));
        fxm_posPathInit(&fx_114_posPath4, (u16 *)ex->btlfx_getFileData(entry->file, 0x94));
        break;
    case 0x2F:
        work->pos28.vy = 0;
        work->frame168 = 0;
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack0, (u16 *)ex->btlfx_getFileData(entry->file, 0x90));
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack1, (u16 *)ex->btlfx_getFileData(entry->file, 0x8F));
        work->posBC.vy = 0x50;
        work->posBC.vx = 0;
        work->posBC.vz = -0x320;
        break;
    case 0x30:
        work->index160 = 1;
        work->loopMotion164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->loader)->summon54->motion180[work->index160],
                               ((Fx114Summon80 *)env->loader)->summonModel60);
        break;
    case 0x61:
        work->frame168 = 0;
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack0, (u16 *)ex->btlfx_getFileData(entry->file, 0x91));
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack1, (u16 *)ex->btlfx_getFileData(entry->file, 0x8D));
        break;
    case 0x62:
        work->index160 = 2;
        work->loopMotion164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->loader)->summon54->motion180[work->index160],
                               ((Fx114Summon80 *)env->loader)->summonModel60);
        break;
    case 0x7A:
        work->frame168 = 0;
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack0, (u16 *)ex->btlfx_getFileData(entry->file, 0x92));
        fxm_posTrackInit((Fx114Track12 *)&fx_114_posTrack1, (u16 *)ex->btlfx_getFileData(entry->file, 0x8E));
        break;
    case 0x7B:
        work->index160 = 3;
        work->loopMotion164 = 0;
        ex->Hi_SetSummonMotion(((Fx114Summon80 *)env->loader)->summon54->motion180[work->index160],
                               ((Fx114Summon80 *)env->loader)->summonModel60);
        break;
    case 0xA7:
        work->frame168 = 0x10;
        fxm_posPathInit(&fx_114_posPath2, (u16 *)ex->btlfx_getFileData(entry->file, 0x2B));
        fxm_posPathInit(&fx_114_posPath3, (u16 *)ex->btlfx_getFileData(entry->file, 0x2C));
        break;
    }
    switch (frame) {
    case 0:
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&fx_114_emitPos0);
        break;
    case 0x28:
        work->pos13C.vx = 0;
        work->pos13C.vy = 0;
        work->pos13C.vz = -0x4B0;
        work->unk144.vx = 0;
        work->unk144.vy = 0;
        work->unk144.vz = -0x4B0;
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos13C);
        work->timer172 = 6;
        break;
    case 0x36:
        work->timer174 = 0xD;
        work->timer176 = 0xD;
        break;
    case 0x2A:
        work->timer16E = 6;
        break;
    case 0x30:
        work->timer170 = 6;
        break;
    case 0x7B:
        work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&fx_114_emitPos1[0]);
        work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1B), (s32)&fx_114_emitPos1[1]);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&fx_114_emitPos1[2]);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1C), (s32)&fx_114_emitPos1[3]);
        if (work->emit10 != NULL) {
            work->emit10->trails = (s32)fx_114_op80Trails1;
            work->emit10->trailSlots = 1;
            work->emit10->trailStride = 0xD0;
        }
        if (work->emit14 != NULL) {
            work->emit14->trails = (s32)fx_114_op80Trails2;
            work->emit14->trailSlots = 1;
            work->emit14->trailStride = 0xD0;
        }
        if (work->emit18 != NULL) {
            work->emit18->trails = (s32)fx_114_op80Trails3;
            work->emit18->trailSlots = 1;
            work->emit18->trailStride = 0xD0;
        }
        if (work->emit1C != NULL) {
            work->emit1C->trails = (s32)fx_114_op80Trails4;
            work->emit1C->trailSlots = 1;
            work->emit1C->trailStride = 0xD0;
        }
        if (work->emit10 != NULL) {
            work->emit10->file->m.module.update = fxm_pinParticle0;
        }
        if (work->emit14 != NULL) {
            work->emit14->file->m.module.update = fxm_pinParticle1;
        }
        if (work->emit18 != NULL) {
            work->emit18->file->m.module.update = fxm_pinParticle2;
        }
        if (work->emit1C != NULL) {
            work->emit1C->file->m.module.update = fxm_pinParticle3;
        }
        break;
    case 0x2C:
        ex->btlfx_stopEmitter(work->emit0);
        work->pos14C.vx = 0;
        work->pos14C.vy = 0;
        work->pos14C.vz = -0x4B0;
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos14C);
        if (work->emit8 != NULL) {
            work->emit8->trails = (s32)fx_114_op80Trails0;
            work->emit8->trailSlots = 0x55;
            work->emit8->trailStride = 0x28;
        }
        break;
    case 0xA8:
        ex->btlfx_stopEmitter(work->emit10);
        ex->btlfx_stopEmitter(work->emit14);
        ex->btlfx_stopEmitter(work->emit18);
        ex->btlfx_stopEmitter(work->emit1C);
        break;
    }
    switch (frame) {
    case 0x2C:
        ex->Hi_FreeEffModel(work->model178);
        break;
    case 0x2E:
        ex->Hi_FreeEffModel(work->model17A);
        break;
    case 0x43:
        ex->Hi_FreeEffModel(work->model17C);
        break;
    }
    if (frame >= 0 && frame < 0x2F) {
        fxm_posPathGet(&fx_114_posPath0, work->frame168, &v40);
        env->camera->points[0].vx = 0;
        env->camera->points[0].vy = v40.vy;
        env->camera->points[0].vz = -0x4B0;
        fxm_posPathGet(&fx_114_posPath1, work->frame168, &v40);
        env->camera->points[1].vx = v40.vx;
        env->camera->points[1].vy = v40.vy;
        env->camera->points[1].vz = v40.vz;
    }
    if (frame >= 0x2F && frame < 0xA8) {
        fxm_posTrackGet(&fx_114_posTrack0, work->frame168, &v48);
        env->camera->points[0].vx = work->pos28.vx + v48.vx * 3 / 2;
        env->camera->points[0].vy = work->pos28.vy + v48.vy * 3 / 2;
        env->camera->points[0].vz = work->pos28.vz + v48.vz * 3 / 2;
        fxm_posTrackGet(&fx_114_posTrack1, work->frame168, &v48);
        env->camera->points[1].vx = work->pos28.vx + v48.vx * 3 / 2;
        env->camera->points[1].vy = work->pos28.vy + v48.vy * 3 / 2;
        env->camera->points[1].vz = work->pos28.vz + v48.vz * 3 / 2;
    }
    if (frame >= 0xA7 && frame < 0xDE) {
        fxm_posPathGet(&fx_114_posPath2, work->frame168, &v50);
        env->camera->points[0].vx = v50.vx;
        env->camera->points[0].vy = v50.vy;
        env->camera->points[0].vz = v50.vz;
        fxm_posPathGet(&fx_114_posPath3, work->frame168, &v50);
        env->camera->points[1].vx = v50.vx;
        env->camera->points[1].vy = v50.vy;
        env->camera->points[1].vz = v50.vz;
    }
    if (frame >= 0 && frame < 0xA8) {
        fxm_posPathGet(&fx_114_posPath4, frame + 0x10, &v58);
        work->pos28.vy = v58.vy;
        if (work->index160 != 0) {
            ex->Hi_DrawSummonModel(&work->rot20, (VECTOR *)&work->pos28, (s32)&work->scale38,
                                   ((Fx114Summon80 *)env->loader)->summonModel60, work->loopMotion164);
            if (work->index160 > 0) {
                k1 = frame / 16 % 8;
                q1 = frame % 16;
                t1 = q1 << 9;
                if (q1 >= 9) {
                    t1 = 0x1000 - ((q1 - 8) << 9);
                }
                fxm_drawTexelQuadsB(&work->pos54, &work->rot4C, 0x11A, 0, 0xC0, 2, k1, 0x100, 0x10, 0x10, work->scale5C, t1, 1,
                              fx_114_texelColors, 0);
            }
        }
    }
    if (work->timer16E >= 0) {
        fxm_tintScreenLerp(0, 0xFF, 6 - work->timer16E, 6, 1, 0);
        work->timer16E--;
    }
    if (work->timer170 >= 0) {
        fxm_tintScreenLerp(0xFF, 0, 6 - work->timer170, 6, 1, 0);
        work->timer170--;
    }
    if (frame >= 0x30 && frame < 0xF3) {
        work->color15C = 0x40;
        work->color15D = 0x20;
        work->color15E = 0;
        if (frame < 0xDE) {
            if ((frame / 16) & 1) {
                work->color15C -= (frame % 16) * 2;
                work->color15D -= frame % 16;
            } else {
                work->color15C += (frame % 16) * 2 - 0x1E;
                work->color15D += frame % 16 - 0xF;
            }
        }
        if (frame < 0xA8) {
            work->color48 = 0x20;
            ex->Hi_ModifySummonModelRGB(((Fx114Summon80 *)env->loader)->summonModel60, 0x20, 0x20, 0x20);
        }
        if (frame >= 0xE3) {
            k3 = frame - 0xE3;
            work->color15C -= work->color15C * k3 / 16;
            work->color15D -= work->color15D * k3 / 16;
        }
        c = 0x40;
        if (frame >= 0xE3) {
            c = 0x40 - ((frame - 0xE3) << 6) / 16;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, c, c, c);
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->color15C, work->color15D, work->color15E);
    }
    if (frame >= 0 && frame < 0x2C) {
        work->pos68.vx = work->pos28.vx;
        work->pos68.vy = work->pos28.vy;
        work->pos68.vz = work->pos28.vz;
        fxm_vecPathGet(&fx_114_vecPath, frame + 0x10, (VECTOR *)&work->scale78);
        ex->Hi_DrawEffModel(&work->rot60, (VECTOR *)&work->pos68, (s32)&work->scale78, work->model178);
        ex->Hi_ModifyEffModelAbr(work->model178, 1);
        work->pos134.vx = work->pos68.vx;
        work->pos134.vy = work->pos68.vy;
        work->pos134.vz = work->pos68.vz;
        if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
            work->emit0 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->pos134);
        work->rot60.vy += 0x40;
    }
    if (frame >= 0x28 && frame < 0x2E) {
        fxm_vecTrackGet(&fx_114_vecTrack3, frame, (VECTOR *)&work->scaleA0);
        ex->Hi_DrawEffModel(&work->rot88, (VECTOR *)&work->pos90, (s32)&work->scaleA0, work->model17A);
        ex->Hi_ModifyEffModelAbr(work->model17A, 1);
        if (work->timer172 >= 0) {
            work->colorB0 = fxm_lerpRatio(0, -0xFF, 6 - work->timer172, 6);
            ex->Hi_ModifyEffModelRGB(work->model17A, work->colorB0, work->colorB0, work->colorB0);
            work->timer172--;
        }
    }
    if (frame >= 0x29 && frame < 0x43) {
        n = frame - 0x29;
        if (work->colorDC >= -0x7F) {
            fxm_vecTrackGet(&fx_114_vecTrack0, frame + 0x10, (VECTOR *)&work->scaleCC);
            ex->Hi_DrawEffModel(&work->rotB4, (VECTOR *)&work->posBC, (s32)&work->scaleCC, work->model17C);
            ex->Hi_ModifyEffModelAbr(work->model17C, 1);
            if (work->timer174 >= 0) {
                work->colorDC = fxm_lerpRatio(0, -0x80, 0xD - work->timer174, 0xD);
                ex->Hi_ModifyEffModelRGB(work->model17C, work->colorDC, work->colorDC, work->colorDC);
                work->timer174--;
            }
        }
        if (work->color108 >= -0x7F) {
            fxm_vecTrackGet(&fx_114_vecTrack1, frame + 0x10, (VECTOR *)&work->scaleF8);
            fxm_vecTrackGet(&fx_114_vecTrack2, frame + 0x10, (VECTOR *)&work->scale124);
            ex->Hi_DrawEffModel(&work->rotE0, (VECTOR *)&work->posE8, (s32)&work->scaleF8, work->model17E);
            ex->Hi_DrawEffModel(&work->rot10C, (VECTOR *)&work->pos114, (s32)&work->scale124, work->model180);
            ex->Hi_ModifyEffModelAbr(work->model17E, 1);
            ex->Hi_ModifyEffModelAbr(work->model180, 1);
            if (work->timer176 >= 0) {
                work->color108 = fxm_lerpRatio(0, -0x80, 0xD - work->timer176, 0xD);
                ex->Hi_ModifyEffModelRGB(work->model17E, work->color108, work->color108, work->color108);
                ex->Hi_ModifyEffModelRGB(work->model180, work->color108, work->color108, work->color108);
                work->timer176--;
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 6), n);
        ex->btlfx_drawScrollMove(0x1180 - work->frame16C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
    }
    if (frame >= 0x7B && frame < 0xB7) {
        for (i = 0; i < 4; i++) {
            ex->Hi_GetSummonBonePos(((Fx114Summon80 *)env->loader)->summonModel60, fx_114_pinBones[i], (u16 *)&fx_114_pinPoints[i]);
        }
    }
    if (frame >= 0xA2 && frame < 0xA8) {
        k4 = frame - 0xA2;
        dp = (DR_STP *)env->prim;
        env->prim = (u8 *)(dp + 1);
        setlen(dp, 2);
        dp->code[0] = 0xE6000001;
        dp->code[1] = 0;
        addPrim(env->ot + 1, dp);
        s.vz = s.vy = s.vx = (k4 << 7) + 0x1000;
        col.r = col.g = col.b = 0x3C;
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &r, &s, &col, 1, 0);
        env->camera->projDist = 0x208 - k4 * k4 * 6;
    }
    work->frame168++;
    work->frame16C++;
    if (frame >= 0x118) {
        ex->btlfx_clearEffModels();
        return 1;
    }
    }
    return 0;
}

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *args = 0x48;
    } else if (mode == 1) {
        work->rot8.vx = 0;
        work->rot8.vy = 0;
        work->rot8.vz = 0;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->scale20.vx = 0x1000;
        work->scale20.vy = 0x1000;
        work->scale20.vz = 0x1000;
        fx_114_env = env;
        fxm_vecTrackInit(&fx_114_vecTrack3, (u16 *)ex->btlfx_getFileData(entry->file, 0x89));
        work->model46 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x85));
        work->timer44 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            ex->Hi_GetSummonBonePos(((Fx114Summon *)env->loader)->summonModel60, 0x1B, (u16 *)&work->pos34);
            work->pos10.vx = work->pos34.vx;
            work->pos10.vy = work->pos34.vy;
            work->pos10.vz = work->pos34.vz;
            work->pos3C.vx = work->pos34.vx;
            work->pos3C.vy = work->pos34.vy;
            work->pos3C.vz = work->pos34.vz;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos34);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos3C);
            work->timer44 = 0xC;
            break;
        case 0xD:
            ex->Hi_FreeEffModel(work->model46);
            ex->btlfx_stopEmitter(work->emit0);
            ex->btlfx_stopEmitter(work->emit4);
            break;
        }
        if (frame >= 1 && frame < 0xD) {
            fxm_vecTrackGet(&fx_114_vecTrack3, frame - 1, &work->scale20);
            ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model46);
            ex->Hi_ModifyEffModelAbr(work->model46, 1);
            if (work->timer44 >= 0) {
                work->color30 = c = fxm_lerpRatio(0, -0xFF, 0xC - work->timer44, 0xC);
                ex->Hi_ModifyEffModelRGB(work->model46, c, c, c);
                work->timer44--;
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_114/nonmatchings/fx_114", fx_114_headerPtr);

s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    DR_MODE *dr;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *args = 0x6C;
    } else if (mode == 1) {
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0x320;
        work->scale1C.vx = 0x1000;
        work->scale1C.vy = 0x1000;
        work->scale1C.vz = 0x1000;
        fx_114_env = env;
        work->model66 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x2A), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->model66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x20), 0x17, 0x3D00, 0x80, 0xE0);
        fx_114_pathTrail.fadeFrom10 = 0x60;
        fx_114_pathTrail.start8 = 0;
        fx_114_pathTrail.countA = 0;
        fx_114_pathTrail.fadeC = 0x1000;
        fx_114_pathTrail.scaleE = 0x1000;
        fx_114_pathTrail.fadeTo12 = 0x30;
        work->timer60 = -1;
        work->timer62 = -1;
        work->frame64 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x80, 0xA0, fx_114_bgOt, 0, 0x168, 0xF6E);
            break;
        case 9:
            work->pos58.vx = -3;
            work->pos58.vy = 0;
            work->pos58.vz = 0x320;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos58);
            break;
        case 8:
            work->timer60 = 4;
            break;
        case 2:
            work->timer62 = 6;
            break;
        case 29:
            ex->Hi_FreeEffModel(work->model66);
            break;
        case 18:
            ex->Hi_FreeEffModel(work->model68);
            break;
        }
        if ((u32)frame < 0x1D) {
            ex->Hi_DrawEffModel(&work->rot4, &work->posC, (s32)&work->scale1C, work->model66);
            ex->Hi_ModifyEffModelRGB(work->model66, 6, 6, 6);
        }
        if ((u32)(frame - 8) < 10) {
            fx_114_pathTrail.countA = (frame - 8) * 2;
            fx_114_pathTrail.countA = fx_114_pathTrail.countA >= 10 ? 9 : fx_114_pathTrail.countA;
            if (work->timer60 >= 0) {
                fx_114_pathTrail.fadeC = fxm_lerpRatio(0, 0x1000, 4 - work->timer60, 4);
                work->timer60--;
            }
            if (work->timer62 >= 0) {
                fx_114_pathTrail.fadeC = fxm_lerpRatio(0x1000, 0, 6 - work->timer62, 6);
                work->timer62--;
            }
            for (i = 0, buf = fx_114_op82Trails[0]; i < 13; i++) {
                fxm_posPathInit(&fx_114_pathTrail.path0, (u16 *)fxm_offsetTableEntry(ex->btlfx_getFileData(entry->file, 0x17), (u16)i));
                fxm_buildPathTrail(&fx_114_pathTrail.path0, buf, 0x50, fx_114_pathTrail.start8, fx_114_pathTrail.countA, fx_114_pathTrail.fadeFrom10, fx_114_pathTrail.fadeTo12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, 0, fx_114_pathTrail.scaleE, fx_114_pathTrail.fadeC, 1,
                                    buf, 0);
                buf += 0x50;
            }
        }
        if ((u32)(t = frame - 0xE) < 4) {
            dr = (DR_MODE *)env->prim;
            env->prim = (u8 *)(dr + 1);
            setlen(dr, 2);
            dr->code[0] = 0xE6000001;
            dr->code[1] = 0;
            addPrim(&env->ot[1], dr);
            sc.vz = sc.vy = sc.vx = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vz = rot.vy = rot.vx = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            env->camera->projDist = 0x208 - t * t * 4;
        }
        work->frame64++;
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_114_op83(s32 mode, Fx114Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = 0x150;
    } else if (mode == 1) {
        work->unkC.vx = 0x400;
        work->pos14.vy = -0x78;
        work->pos14.vz = 0x320;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unkC.pad = 1;
        work->pos14.vx = 0;
        work->unk1C = 0x1800;
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->pos28.vx = 0;
        work->pos28.vy = 0;
        work->pos28.vz = 0x320;
        work->scale38.vx = 0x1000;
        work->scale38.vy = 0x1000;
        work->scale38.vz = 0x1000;
        fx_114_env = env;
        work->model118[0] = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->file, 0x12), fx_114_texInfo1, fx_114_texPos1);
        ex->Hi_SetEffModelOffset(work->model118[0], 0x6E);
        work->rot48.vx = 0;
        work->rot48.vy = 0;
        work->rot48.vz = 0;
        work->pos50.vx = 0;
        work->pos50.vy = 0;
        work->pos50.vz = 0x320;
        work->scale60.vx = 0x1000;
        work->scale60.vy = 0x1000;
        work->scale60.vz = 0x1000;
        for (j = 0; j < 23; j++) {
            u8 *tex;
            s16 *uv;
            s32 a;
            s32 b;

            if (fx_114_op83TexSelect[j] != 0) {
                tex = fx_114_texInfo3;
                uv = fx_114_texPos3;
            } else {
                tex = fx_114_texInfo2;
                uv = fx_114_texPos2;
            }
            a = fxm_offsetTableEntry(ex->btlfx_getFileData(entry->file, 0x28), j & 0xFFFF);
            b = fxm_offsetTableEntry(ex->btlfx_getFileData(entry->file, 0x29), j & 0xFFFF);
            work->model118[j + 1] = REG_TEX_LIST(ex)((void *)a, tex, uv);
            ex->Hi_SetEffModelOffset(work->model118[j + 1], 0x6E);
            fxm_rotPosTrackInit(&fx_114_rotPosTrack[j], (u16 *)b);
        }
        work->rot70.vx = 0;
        work->rot70.vy = 0;
        work->rot70.vz = 0;
        work->pos78.vx = 0;
        work->pos78.vy = 0;
        work->pos78.vz = 0x320;
        work->scale88.vx = 0x1000;
        work->scale88.vy = 0x1000;
        work->scale88.vz = 0x1000;
        fxm_vecTrackInit(&fx_114_vecTrack3, (u16 *)ex->btlfx_getFileData(entry->file, 0x1D));
        work->model148 = REG_TEX_LIST(ex)((void *)ex->btlfx_getFileData(entry->file, 0x1E), fx_114_texInfo4, fx_114_texPos4);
        work->rot9C.vx = 0;
        work->rot9C.vy = 0;
        work->rot9C.vz = 0;
        work->posA4.vx = 0;
        work->posA4.vy = 0;
        work->posA4.vz = 0x320;
        work->scaleB4.vx = 0x1000;
        work->scaleB4.vy = 0x1000;
        work->scaleB4.vz = 0x1000;
        work->rotCC.vx = 0;
        work->rotCC.vy = 0;
        work->rotCC.vz = 0;
        work->posD4.vx = 0;
        work->posD4.vy = 0;
        work->posD4.vz = 0x320;
        work->scaleE4.vx = 0x1000;
        work->scaleE4.vy = 0x1000;
        work->scaleE4.vz = 0x1000;
        work->model14A = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x27), 0x9A, 0x3DC0, 0, 0);
        work->model14C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x22), 0x9A, 0x3DC0, 0, 0);
        fxm_posTrackInit(&fx_114_posTrack2, (u16 *)ex->btlfx_getFileData(entry->file, 0x23));
        fxm_vecTrackInit(&fx_114_vecTrack4, (u16 *)ex->btlfx_getFileData(entry->file, 0x24));
        fxm_vecTrackInit(&fx_114_vecTrack5, (u16 *)ex->btlfx_getFileData(entry->file, 0x25));
        work->frame10C = 0;
        work->unk104.vy = work->pos14.vy;
        work->timer10E = -1;
        work->timer110 = -1;
        work->timer112 = -1;
        work->timer114 = -1;
        work->timer116 = -1;
        work->unk104.vx = work->pos14.vx;
        work->unk104.vz = work->pos14.vz;
        work->unk104.vy -= 0xF0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->posF4.vx = work->pos28.vx;
            work->posF4.vy = work->pos28.vy;
            work->posF4.vz = work->pos28.vz;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->posF4);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->posF4);
            break;
        case 3:
            work->posD4.vx = work->pos28.vx;
            work->posD4.vy = work->pos28.vy;
            work->posD4.vz = work->pos28.vz;
            work->posFC.vx = work->pos28.vx;
            work->posFC.vy = work->pos28.vy;
            work->posFC.vz = work->pos28.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->posFC);
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x100;
            }
            break;
        case 4:
            work->timer110 = 3;
            work->timer114 = 6;
            break;
        case 0x15:
            work->timer112 = 0x10;
            work->timer116 = 0x10;
            work->timer10E = 0x10;
            break;
        }
        if (frame >= 6 && frame < 0x14) {
            Fx114Rock *r;
            s32 t;

            ex->Hi_DrawEffModel(&work->rot20, (VECTOR *)&work->pos28, (s32)&work->scale38, work->model118[0]);
            ex->Hi_ModifyEffModelRGB(work->model118[0], 6, 6, 6);
            for (i = 0, r = fx_114_op83Pieces; i < 23; i++, r++) {
                t = frame - 2;
                if (r->done2 != 0) {
                    t = 0;
                }
                if (t == r->frame0) {
                    v.vx = r->pos18.vx;
                    v.vy = r->pos18.vy;
                    v.vz = r->pos18.vz - 0x190;
                    ex->btlfx_aimAngles((s16 *)&work->pos50, (s16 *)&v, (s16 *)&r->angle8);
                    r->speed4 = ex->btlfx_distance(&v, (SVECTOR *)&work->pos50) / 30;
                    r->vel6 = 0xC8;
                    r->vel6 += ex->btlfx_rand() % 100;
                    r->speed4 += ex->btlfx_rand() % 50;
                    r->vel6 /= 2;
                    r->speed4 /= 2;
                    r->angle8.vx &= 0xFFF;
                    r->angle8.vy &= 0xFFF;
                    r->angle8.vz &= 0xFFF;
                    if (r->angle8.vy < 0x800) {
                        r->flags1 |= 1;
                    } else {
                        r->flags1 |= 2;
                    }
                    if (r->angle8.vy < 0x800) {
                        r->flags1 |= 4;
                    } else {
                        r->flags1 |= 8;
                    }
                }
                if (t >= r->frame0) {
                    v.vx = r->pos18.vx;
                    v.vy = r->pos18.vy;
                    v.vz = r->pos18.vz;
                    ex->btlfx_pointAhead((s16 *)&v, (s16 *)&r->angle8, r->speed4, (s16 *)&v);
                    r->pos18.vx = v.vx;
                    r->pos18.vy = v.vy;
                    r->pos18.vz = v.vz;
                    r->pos18.vy -= r->vel6;
                    if (t < r->frame0 + 2) {
                        if (r->flags1 & 1) {
                            r->rot10.vx += 0x40;
                        } else {
                            r->rot10.vx -= 0x40;
                        }
                        if (r->flags1 & 4) {
                            r->rot10.vz += 0x40;
                        } else {
                            r->rot10.vz -= 0x40;
                        }
                    }
                    work->rot48.vx = r->rot10.vx;
                    work->rot48.vy = r->rot10.vy;
                    work->rot48.vz = r->rot10.vz;
                    r->vel6 -= 0x14;
                    if (r->vel6 >= 4) {
                        r->vel6 -= t * 5;
                    }
                    if (r->pos18.vy >= 0x32) {
                        r->done2 = 1;
                        continue;
                    }
                } else {
                    fxm_rotPosTrackGet(&fx_114_rotPosTrack[i], t, &work->rot48, &v);
                    v.vx = -v.vx;
                    work->pos50.vx = 0;
                    work->pos50.vy = 0;
                    work->pos50.vz = 0x320;
                    work->rot48.vz = -work->rot48.vz;
                    work->pos50.vx += v.vx;
                    work->pos50.vy += v.vy;
                    work->pos50.vz += v.vz;
                    r->pos18.vx = work->pos50.vx;
                    r->pos18.vy = work->pos50.vy;
                    r->pos18.vz = work->pos50.vz;
                    work->pos50.vx = 0;
                    work->pos50.vy = 0;
                    work->pos50.vz = 0x320;
                }
                ex->Hi_DrawEffModel(&work->rot48, (VECTOR *)&r->pos18, (s32)&work->scale60, work->model118[i + 1]);
                ex->Hi_ModifyEffModelRGB(work->model118[i + 1], 6, 6, 6);
            }
        }
        if (frame >= 4 && frame < 0x25) {
            fxm_vecTrackGet(&fx_114_vecTrack3, frame - 4, (VECTOR *)&work->scale88);
            ex->Hi_DrawEffModel(&work->rot70, (VECTOR *)&work->pos78, (s32)&work->scale88, work->model148);
            ex->Hi_ModifyEffModelAbr(work->model148, 1);
            if (work->timer10E >= 0) {
                work->color98 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer10E, 0x10);
                ex->Hi_ModifyEffModelRGB(work->model148, work->color98, work->color98, work->color98);
                work->timer10E--;
            }
            fxm_vecTrackGet(&fx_114_vecTrack4, frame - 4, (VECTOR *)&work->scaleB4);
            ex->Hi_DrawEffModel(&work->rot9C, (VECTOR *)&work->posA4, (s32)&work->scaleB4, work->model14A);
            ex->Hi_ModifyEffModelAbr(work->model14A, 1);
            if (work->timer110 >= 0 || work->timer112 >= 0) {
                if (work->timer110 >= 0) {
                    work->colorC4 = fxm_lerpRatio(-0x80, 0, 3 - work->timer110, 3);
                    work->timer110--;
                }
                if (work->timer112 >= 0) {
                    work->colorC4 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer112, 0x10);
                    work->timer112--;
                }
                ex->Hi_ModifyEffModelRGB(work->model14A, work->colorC4, work->colorC4, work->colorC4);
            }
            work->rot9C.vy += 0x40;
            if (frame >= 4 && frame < 0x25) {
                work->posD4.vx = 0;
                work->posD4.vy = 0;
                work->posD4.vz = 0x320;
                fxm_posTrackGet((Fx114Track6 *)&fx_114_posTrack2, frame - 4, &w);
                work->posD4.vx += w.vx;
                work->posD4.vy += w.vy;
                work->posD4.vz += w.vz;
                fxm_vecTrackGet(&fx_114_vecTrack5, frame - 4, (VECTOR *)&work->scaleE4);
                ex->Hi_DrawEffModel(&work->rotCC, (VECTOR *)&work->posD4, (s32)&work->scaleE4, work->model14C);
                ex->Hi_ModifyEffModelAbr(work->model14C, 1);
                if (work->timer114 >= 0 || work->timer116 >= 0) {
                    if (work->timer114 >= 0) {
                        work->colorC8 = fxm_lerpRatio(-0x80, 0, 6 - work->timer114, 6);
                        work->timer114--;
                    }
                    if (work->timer116 >= 0) {
                        work->colorC8 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer116, 0x10);
                        work->timer116--;
                    }
                    ex->Hi_ModifyEffModelRGB(work->model14C, work->colorC8, work->colorC8, work->colorC8);
                }
            }
        }
        ex->btlfx_drawScrollMove(0x3C0 - work->frame10C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
        work->frame10C++;
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_114_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 frame;

    if (mode == 0) {
        *args = 2;
    } else if (mode == 1) {
        fx_114_env = env;
        *work = -1;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_114_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posTrackInit(Fx114Track12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx114Key12 *)data;
}

static s32 fxm_posTrackGet(Fx114Track6 *track, s32 t, SVECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx114Key6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = -key->x;
    out->vy = key->y;
    out->vz = -key->z;
    return done;
}

static void fxm_posPathInit(Fx114Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx114Point *)data;
}

static s32 fxm_posPathGet(Fx114Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx114Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosTrackInit(Fx114Track12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx114Key12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx114Key12 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6;
        rot->vy = key->rot8;
        rot->vz = key->rotA;
    }
    if (pos != NULL) {
        pos->vx = -key->pos0;
        pos->vy = key->pos2;
        pos->vz = -key->pos4;
    }
    return done;
}

static void fxm_vecTrackInit(Fx114Track6 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx114Key6 *)data;
}

static s32 fxm_vecTrackGet(Fx114Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx114Key6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_vecPathInit(Fx114Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx114Point *)data;
}

static s32 fxm_vecPathGet(Fx114Path *path, s32 t, VECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx114Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static s32 fxm_offsetTableEntry(s32 base, s32 idx) {
    return (s32)base + ((s32 *)base)[(u16)idx + 1];
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_114_env)->btlfx_drawScreenTint((u32 *)(fx_114_env->ot + ot), abr, bright, bright, bright);
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_114_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_114_camKeys[i].timer6 = freeMark;
    }
    for (i = 3; i >= 0; i--) {
        fx_114_camChanFrames[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < 64; i++) {
        if (fx_114_camKeys[i].timer6 == FX_114_KEY_FREE) {
            fx_114_camKeys[i].timer6 = 0;
            fx_114_camKeys[i].keys0 = data;
            fx_114_camKeys[i].frame4 = start;
            fx_114_camKeys[i].dstSlotsA = dst;
            fx_114_camKeys[i].indexC = 0;
            fx_114_camKeys[i].countE = count;
            fx_114_camKeys[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_114_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < 64; i++) {
        if (fx_114_camKeys[i].chan8 == ch) {
            if (fx_114_camKeys[i].timer6 == 0 && fx_114_camKeys[i].frame4 == fx_114_camChanFrames[ch]) {
                fx_114_camKeys[i].timer6 = *fx_114_camKeys[i].keys0;
                fx_114_camKeys[i].keys0++;
            }
            if (fx_114_camKeys[i].timer6 > 0) {
                fx_114_camKeys[i].timer6--;
                for (k = 0; k < 3; k++) {
                    dstSlot = ((fx_114_camKeys[i].dstSlotsA >> (k * 4)) & 0xF) - 1;
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_114_camKeys[i].keys0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_114_camKeys[i].keys0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_114_camKeys[i].keys0[2];
                        if (fx_114_camKeys[i].indexC < fx_114_camKeys[i].countE) {
                            fx_114_camKeys[i].keys0 += 3;
                            fx_114_camKeys[i].indexC++;
                        }
                    }
                }
            }
        }
    }
    fx_114_camChanFrames[ch]++;
}

static void fxm_drawTexelQuadsB(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_114_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_bm();
                *(s32 *)&colTL = *(s32 *)&colTR;
                *(s32 *)&colBL = *(s32 *)&colBR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(0x8000000, otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, otz, tpage);
    }
    w--;
    h--;
    left = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_114_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

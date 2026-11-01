#include "fxmod/fx_12b.h"

static s32 fx_12b_op80ParticleMotion0(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 slot = pt->index1F - 1;
    Fx12bShard *shard = &fx_12b_op80Shards0[slot];
    Fx12bPath *path = &fx_12b_op80PosPaths0[slot];
    SVECTOR *ofs = &fx_12b_bonePoints0[slot];

    if (shard->frame20 >= 2 && shard->frame20 < 7) {
        return fx_12b_pathParticle(pt, pos, shard, path, ofs, frame, n);
    }
    if ((u16)(shard->frame20 - 7) < 0xE) {
        return fx_12b_arcParticle0(pt, pos, shard, frame, n);
    }
    return 0;
}

static s32 fx_12b_op80ParticleMotion1(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 slot = pt->index1F - 1;
    Fx12bShard *shard = &fx_12b_op80Shards1[slot];
    Fx12bPath *path = &fx_12b_op80PosPaths1[slot];
    SVECTOR *ofs = &fx_12b_bonePoints1[slot];

    if (shard->frame20 >= 2 && shard->frame20 < 7) {
        return fx_12b_pathParticle(pt, pos, shard, path, ofs, frame, n);
    }
    if ((u16)(shard->frame20 - 7) < 0xE) {
        return fx_12b_arcParticle1(pt, pos, shard, frame, n);
    }
    return 0;
}

static s32 fx_12b_op81ParticleMotion0(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 slot = pt->index1F - 1;
    Fx12bShard *shard = &fx_12b_op81Shards0[slot];
    Fx12bPath *path = &fx_12b_op81PosPath0;
    SVECTOR *ofs = &fx_12b_bonePoints0[slot];

    if (shard->frame20 >= 2 && shard->frame20 < 7) {
        return fx_12b_pathParticle(pt, pos, shard, path, ofs, frame, n);
    }
    return fx_12b_arcParticle0(pt, pos, shard, frame, n);
}

static s32 fx_12b_op81ParticleMotion1(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 slot = pt->index1F - 1;
    Fx12bShard *shard = &fx_12b_op81Shards1[slot];
    Fx12bPath *path = &fx_12b_op81PosPath1;
    SVECTOR *ofs = &fx_12b_bonePoints1[slot];

    if (shard->frame20 >= 2 && shard->frame20 < 7) {
        return fx_12b_pathParticle(pt, pos, shard, path, ofs, frame, n);
    }
    return fx_12b_arcParticle1(pt, pos, shard, frame, n);
}

s32 fx_12b_pathParticle(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR pathPos;

    if (frame == 0) {
        ex->btlfx_playFileSound(0x39, 0, -1);
        ex->btlfx_playFileSound(0x3A, 0, -1);
    }
    fxm_posPathGet(path, shard->frame20 - 2, &pathPos);
    pathPos.vx += ofs->vx;
    pathPos.vy += ofs->vy;
    pathPos.vz += ofs->vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    shard->pos8.vx = pathPos.vx;
    shard->pos8.vy = pathPos.vy;
    shard->pos8.vz = pathPos.vz;
    pt->pos0.vx = pos->vx << 12;
    pt->pos0.vy = pos->vy << 12;
    pt->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_12b_arcParticle0(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR arcPos;
    s32 t = ((shard->frame20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&shard->pos8, (s16 *)&shard->pos0, t, (s16 *)&arcPos);
    t /= 2;
    arcPos.vx += shard->width10 * ex->math_sin(t) / 4096;
    arcPos.vy -= shard->height12 * ex->math_sin(t) / 4096;
    pos->vx = arcPos.vx;
    pos->vy = arcPos.vy;
    pos->vz = arcPos.vz;
    pt->pos0.vx = pos->vx << 12;
    pt->pos0.vy = pos->vy << 12;
    pt->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_12b_arcParticle1(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_12b_env);
    SVECTOR arcPos;
    s32 t = ((shard->frame20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&shard->pos8, (s16 *)&shard->pos0, t, (s16 *)&arcPos);
    t /= 2;
    arcPos.vx -= shard->width10 * ex->math_sin(t) / 4096;
    arcPos.vy -= shard->height12 * ex->math_sin(t) / 4096;
    pos->vx = arcPos.vx;
    pos->vy = arcPos.vy;
    pos->vz = arcPos.vz;
    pt->pos0.vx = pos->vx << 12;
    pt->pos0.vy = pos->vy << 12;
    pt->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

static s32 fx_12b_op80ParticleMotion2(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR target;
    SVECTOR *bonePoint = (SVECTOR *)((u8 *)fx_12b_bonePoints0 + (pt->index1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    target.vx = bonePoint->vx;
    target.vy = bonePoint->vy;
    target.vz = bonePoint->vz;
    target.vx += 0xC8;
    return fxm_pinParticle(pt, pos, &target, frame, n);
}

static s32 fx_12b_op80ParticleMotion3(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR target;
    SVECTOR *bonePoint = (SVECTOR *)((u8 *)fx_12b_bonePoints1 + (pt->index1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    target.vx = bonePoint->vx;
    target.vy = bonePoint->vy;
    target.vz = bonePoint->vz;
    target.vx -= 0xC8;
    return fxm_pinParticle(pt, pos, &target, frame, n);
}

static s32 fx_12b_op81ParticleMotion2(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR target;
    SVECTOR *bonePoint = (SVECTOR *)((u8 *)fx_12b_bonePoints0 + (pt->index1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    target.vx = bonePoint->vx;
    target.vy = bonePoint->vy;
    target.vz = bonePoint->vz;
    target.vx += 0xC8;
    return fxm_pinParticle(pt, pos, &target, frame, n);
}

static s32 fx_12b_op81ParticleMotion3(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR target;
    SVECTOR *bonePoint = (SVECTOR *)((u8 *)fx_12b_bonePoints1 + (pt->index1F - 1) * 8);

    do {
    } while (0); /* scheduling barrier: a statement boundary in the original */

    target.vx = bonePoint->vx;
    target.vy = bonePoint->vy;
    target.vz = bonePoint->vz;
    target.vx -= 0xC8;
    return fxm_pinParticle(pt, pos, &target, frame, n);
}

static s32 fxm_pinParticle(Fx12bParticle *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->pos0.vx = pos->vx << 12;
    pt->pos0.vy = pos->vy << 12;
    pt->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_12b_op80(s32 mode, Fx12bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX boneMat0;
    MATRIX boneMat1;
    Fx12bShard *shard0Alias;
    Fx12bShard *shard0;
    Fx12bShard *shard1Alias;
    Fx12bShard *shard1;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_12b_env = env;
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitBodyPos(0, 0, &work->pos10.vx);
        for (i = 0, shard0Alias = fx_12b_op80Shards0, shard1Alias = fx_12b_op80Shards1, shard0 = fx_12b_op80Shards0, shard1 = fx_12b_op80Shards1; i < 6; i++, shard0Alias++, shard1Alias++, shard0++, shard1++) {
            shard0->frame18 = i * 4;
            shard1->frame18 = i * 4 + 1;
            fxm_posPathInit(&fx_12b_op80PosPaths0[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_12b_pathFileIds0[i]));
            fxm_posPathInit(&fx_12b_op80PosPaths1[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_12b_pathFileIds1[i]));
            shard0Alias->pos0.vx = work->pos10.vx;
            shard0->pos0.vy = work->pos10.vy;
            shard0->pos0.vz = work->pos10.vz;
            shard1Alias->pos0.vx = work->pos10.vx;
            shard1->pos0.vy = work->pos10.vy;
            shard1->pos0.vz = work->pos10.vz;
            if (ex->btlfx_rand() & 1) {
                shard0Alias->pos0.vx += ex->btlfx_rand() % 200;
            } else {
                shard0Alias->pos0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0->pos0.vy += ex->btlfx_rand() % 200;
            } else {
                shard0->pos0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0->pos0.vz += ex->btlfx_rand() % 200;
            } else {
                shard0->pos0.vz -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0Alias->pos0.vx += ex->btlfx_rand() % 200;
            } else {
                shard0Alias->pos0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard1->pos0.vy += ex->btlfx_rand() % 200;
            } else {
                shard1->pos0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard1->pos0.vz += ex->btlfx_rand() % 200;
            } else {
                shard1->pos0.vz -= ex->btlfx_rand() % 200;
            }
            shard0->width10 = ex->btlfx_rand() % 1500 + 200;
            shard0->height12 = ex->btlfx_rand() % 800 + 400;
            shard0->unk14 = 0;
            shard1->width10 = ex->btlfx_rand() % 1500 + 200;
            shard0->height12 = ex->btlfx_rand() % 800 + 400; /* shard0, not shard1: as in the original */
            shard1->unk14 = 0;
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x24), (s32)&fx_12b_emitPos0);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x37), (s32)&fx_12b_emitPos2);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_12b_op80Trails0;
                work->emit0[0]->trailSlots = 6;
                work->emit0[0]->trailStride = 0xA4;
            }
            if (work->emit0[0] != NULL) {
                ((s32 *)work->emit0[0]->file)[2] = (s32)fx_12b_op80ParticleMotion0;
            }
            if (work->emit0[2] != NULL) {
                ((s32 *)work->emit0[2]->file)[2] = (s32)fx_12b_op80ParticleMotion2;
            }
            break;
        case 1:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&fx_12b_emitPos1);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x38), (s32)&fx_12b_emitPos2);
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_12b_op80Trails1;
                work->emit0[1]->trailSlots = 6;
                work->emit0[1]->trailStride = 0xA4;
            }
            if (work->emit0[1] != NULL) {
                ((s32 *)work->emit0[1]->file)[2] = (s32)fx_12b_op80ParticleMotion1;
            }
            if (work->emit0[3] != NULL) {
                ((s32 *)work->emit0[3]->file)[2] = (s32)fx_12b_op80ParticleMotion3;
            }
            break;
        }
        for (shard0 = fx_12b_op80Shards0, shard1 = fx_12b_op80Shards1, j = 0; j < 6; j++, shard0++, shard1++) {
            if (frame == shard0->frame18) {
                ex->btlfx_getUnitBoneMatrix(0x10, shard0->bone1A, (BtlBone *)&boneMat0);
                ex->btlfx_matrixPointAhead(&boneMat0, shard0->dist1C, (s32)&fx_12b_bonePoints0[j]);
            }
            if (frame == shard1->frame18) {
                ex->btlfx_getUnitBoneMatrix(0x10, shard1->bone1A, (BtlBone *)&boneMat1);
                ex->btlfx_matrixPointAhead(&boneMat1, shard1->dist1C, (s32)&fx_12b_bonePoints1[j]);
            }
            if (frame >= shard0->frame18) {
                if (++shard0->frame20 == 0x13) {
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)shard0);
                    if (emitter != NULL) {
                        emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                    }
                }
            }
            if (frame >= shard1->frame18) {
                if (++shard1->frame20 == 0x13) {
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)shard1);
                    if (emitter != NULL) {
                        emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                    }
                    /* the do-while(0) adds a loop-depth-weighted ref to j, which puts it ahead of shard0 and shard1 in
                     * global-alloc (j in s4) */
                    do {
                        if (j == 5) {
                            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
                        }
                    } while (0);
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12b_op81(s32 mode, Fx12bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX boneMat0;
    MATRIX boneMat1;
    Fx12bShard *shard0Alias;
    Fx12bShard *shard0;
    Fx12bShard *shard1Alias;
    Fx12bShard *shard1;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_12b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, &work->pos10.vx);
        fxm_posPathInit(&fx_12b_op81PosPath0, (u16 *)ex->btlfx_getFileData(entry->file, 0x22));
        fxm_posPathInit(&fx_12b_op81PosPath1, (u16 *)ex->btlfx_getFileData(entry->file, 0x21));
        for (i = 0, shard0Alias = fx_12b_op81Shards0, shard1Alias = fx_12b_op81Shards1, shard0 = fx_12b_op81Shards0, shard1 = fx_12b_op81Shards1; i < 5; i++, shard0Alias++, shard1Alias++, shard0++, shard1++) {
            shard0Alias->pos0.vx = work->pos10.vx;
            shard0->pos0.vy = work->pos10.vy;
            shard0->pos0.vz = work->pos10.vz;
            shard1Alias->pos0.vx = work->pos10.vx;
            shard1->pos0.vy = work->pos10.vy;
            shard1->pos0.vz = work->pos10.vz;
            if (ex->btlfx_rand() & 1) {
                shard0Alias->pos0.vx += ex->btlfx_rand() % 200;
            } else {
                shard0Alias->pos0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0->pos0.vy += ex->btlfx_rand() % 200;
            } else {
                shard0->pos0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0->pos0.vz += ex->btlfx_rand() % 200;
            } else {
                shard0->pos0.vz -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard0Alias->pos0.vx += ex->btlfx_rand() % 200;
            } else {
                shard0Alias->pos0.vx -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard1->pos0.vy += ex->btlfx_rand() % 200;
            } else {
                shard1->pos0.vy -= ex->btlfx_rand() % 200;
            }
            if (ex->btlfx_rand() & 1) {
                shard1->pos0.vz += ex->btlfx_rand() % 200;
            } else {
                shard1->pos0.vz -= ex->btlfx_rand() % 200;
            }
            shard0->width10 = ex->btlfx_rand() % 1500 + 200;
            shard0->height12 = ex->btlfx_rand() % 800 + 400;
            shard0->unk14 = 0;
            shard1->width10 = ex->btlfx_rand() % 1500 + 200;
            shard1->height12 = ex->btlfx_rand() % 800 + 400;
            shard1->unk14 = 0;
        }
        return 0;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&fx_12b_emitPos0);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&fx_12b_emitPos1);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&fx_12b_emitPos2);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&fx_12b_emitPos2);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_12b_op81Trails0;
                work->emit0[0]->trailSlots = 6;
                work->emit0[0]->trailStride = 0xA4;
            }
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_12b_op81Trails1;
                work->emit0[1]->trailSlots = 6;
                work->emit0[1]->trailStride = 0xA4;
            }
            if (work->emit0[0] != NULL) {
                ((s32 *)work->emit0[0]->file)[2] = (s32)fx_12b_op81ParticleMotion0;
            }
            if (work->emit0[1] != NULL) {
                ((s32 *)work->emit0[1]->file)[2] = (s32)fx_12b_op81ParticleMotion1;
            }
            if (work->emit0[2] != NULL) {
                ((s32 *)work->emit0[2]->file)[2] = (s32)fx_12b_op81ParticleMotion2;
            }
            if (work->emit0[3] != NULL) {
                ((s32 *)work->emit0[3]->file)[2] = (s32)fx_12b_op81ParticleMotion3;
            }
        }
        for (j = 0, shard1 = fx_12b_op81Shards1, shard0 = fx_12b_op81Shards0; j < 5; j++, shard0++, shard1++) {
            if (frame == shard0->frame18) {
                ex->btlfx_getUnitBoneMatrix(0x10, shard0->bone1A, (BtlBone *)&boneMat0);
                ex->btlfx_matrixPointAhead(&boneMat0, shard0->dist1C, (s32)&fx_12b_bonePoints0[j]);
            }
            if (frame == shard1->frame18) {
                ex->btlfx_getUnitBoneMatrix(0x10, shard1->bone1A, (BtlBone *)&boneMat1);
                ex->btlfx_matrixPointAhead(&boneMat1, shard1->dist1C, (s32)&fx_12b_bonePoints1[j]);
            }
            if (frame >= shard0->frame18) {
                shard0->frame20++;
            }
            if (frame >= shard1->frame18) {
                shard1->frame20++;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12b_op82(s32 mode, Fx12bWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x24;
    } else if (mode == 1) {
        fx_12b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
        work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos14);
        work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos14);
        work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos14);
        work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos14);
        work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->pos14);
        if (work->emit0[0] != NULL) {
            work->emit0[0]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit0[1] != NULL) {
            work->emit0[1]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit0[2] != NULL) {
            work->emit0[2]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_12b_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx12bPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx12bPoint *)data;
}

static s32 fxm_posPathGet(Fx12bPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx12bPoint *key;

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

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_headerPtr);

#include "fxmod/fx_1b4.h"

s32 fx_1b4_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[0], frame, n);
}

s32 fx_1b4_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[1], frame, n);
}

s32 fx_1b4_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[2], frame, n);
}

s32 fx_1b4_op80ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[3], frame, n);
}

s32 fx_1b4_op80ParticleMotion4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[4], frame, n);
}

s32 fx_1b4_op80ParticleMotion5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[5], frame, n);
}

s32 fx_1b4_op80ParticleMotion6(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[6], frame, n);
}

s32 fx_1b4_op80ParticleMotion7(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1b4_op80ParticlePaths[7], frame, n);
}

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames) {
    SVECTOR pathPos;

    fxm_posPathGet(path, frame, &pathPos);
    pathPos.vx += fx_1b4_pathOrigin.vx;
    pathPos.vy += fx_1b4_pathOrigin.vy;
    pathPos.vz += fx_1b4_pathOrigin.vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1b4_op80(s32 mode, Fx1b4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR *pathPos;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x94;
    } else if (mode == 1) {
        work->pos44.vx = env->targets->centerX;
        work->pos44.vy = env->targets->centerY;
        work->pos44.vz = env->targets->centerZ;
        fx_1b4_pathOrigin.vx = work->pos44.vx;
        fx_1b4_pathOrigin.vy = work->pos44.vy;
        fx_1b4_pathOrigin.vz = work->pos44.vz;
        fx_1b4_env = env;
        work->pos8C.vx = work->pos44.vx;
        work->pos8C.vy = work->pos44.vy;
        work->pos8C.vz = work->pos44.vz;
        work->pos8C.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            for (i = 0; i < 8; i++) {
                fxm_posPathInit(&fx_1b4_posPath[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_1b4_pathFileIds[i]));
                fxm_posPathGet(&fx_1b4_posPath[i], 0, &work->pos4C[i]);
                pathPos = (SVECTOR *)((u8 *)work + i * 8 + 0x4C);
                pathPos->vx += work->pos44.vx;
                pathPos->vy += work->pos44.vy;
                pathPos->vz += work->pos44.vz;
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1b4_emitFileIds0[i]), (s32)&work->pos4C[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->trails = (s32)fx_1b4_op80Trails0[i];
                    work->emit0[i]->trailSlots = 5;
                    work->emit0[i]->trailStride = 0x30;
                }
            }
            break;
        case 0x17:
            for (i = 0; i < 8; i++) {
                fxm_posPathInit(&fx_1b4_op80ParticlePaths[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_1b4_pathFileIds[i]));
                work->emit20[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1b4_emitFileIds1[i]), (s32)&fx_1b4_emitPos);
                if (work->emit20[i] != NULL) {
                    work->emit20[i]->trails = (s32)fx_1b4_op80Trails1[i];
                    work->emit20[i]->trailSlots = 8;
                    work->emit20[i]->trailStride = 0x70;
                    if (work->emit20[i] != NULL) {
                        work->emit20[i]->file->m.module.update = fx_1b4_op80ParticleMotions[i];
                    }
                }
            }
            break;
        case 0x25:
            work->emit40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&work->pos8C);
            break;
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_1b4_op81(s32 mode, Fx1b4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    VECTOR scale2;
    SVECTOR rot2;
    CVECTOR col2;
    DrMode2Words *dm;
    DrMode2Words *dm2;
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        fx_1b4_env = env;
        work->pos10.vz = env->targets->centerZ;
        work->pos10.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos10);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos10);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = 0x100;
            }
            if (work->emit0[1] != NULL) {
                work->emit0[1]->otZ = 0x80;
            }
            break;
        case 0x1C:
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
            if (work->emit0[2] != NULL) {
                work->emit0[2]->trails = (s32)fx_1b4_op81Trails;
                work->emit0[2]->trailSlots = 0xC0;
                work->emit0[2]->trailStride = 0x30;
                if (work->emit0[2] != NULL) {
                    work->emit0[2]->otZ = 0x80;
                }
            }
            if (work->emit0[3] != NULL) {
                work->emit0[3]->otZ = 0x100;
            }
            break;
        case 0x1E:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        if ((u32)(frame - 0x1F) < 0x3C) {
            dm = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->ot[1], dm);
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            col.r = col.g = col.b = 0x28;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        elapsed = frame - 0x5B;
        if ((u32)elapsed < 0xF) {
            dm2 = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm2, 2);
            dm2->code[0] = 0xE6000001;
            dm2->code[1] = 0;
            addPrim(&env->ot[1], dm2);
            scale2.vz = scale2.vy = scale2.vx = (elapsed << 7) + 0x1000;
            col2.r = col2.g = col2.b = 0x28;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot2, &scale2, &col2, 1, 0);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1C), frame);
        return frame >= 0x8C;
    }
    return 0;
}

s32 fx_1b4_op82(s32 mode, Fx1b4Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x24;
    } else if (mode == 1) {
        work->pos1C.vx = env->targets->centerX;
        work->pos1C.vy = env->targets->centerY;
        fx_1b4_env = env;
        work->pos1C.vz = env->targets->centerZ;
        work->pos1C.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos1C);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos1C);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos1C);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = 0x100;
            }
            break;
        case 20:
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos1C);
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos1C);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos1C);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2A), (s32)&work->pos1C);
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_1b4_op82Trails0;
                work->emit0[4]->trailSlots = 0x21;
                work->emit0[4]->trailStride = 0x90;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)fx_1b4_op82Trails1;
                work->emit0[5]->trailSlots = 0x58;
                work->emit0[5]->trailStride = 0x30;
            }
            if (work->emit0[3] != NULL) {
                work->emit0[3]->otZ = 0x80;
            }
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1b4_op83(s32 mode, Fx1b4Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->pos4.vx = env->targets->centerX;
        work->pos4.vy = env->targets->centerY;
        fx_1b4_env = env;
        work->pos4.vz = env->targets->centerZ;
        work->pos4.vy -= 0x258;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2B), (s32)&work->pos4);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

static void fxm_posPathInit(Fx1b4Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1b4Point *)data;
}

static s32 fxm_posPathGet(Fx1b4Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1b4Point *key;

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

INCLUDE_RODATA("asm/us/fx_1b4/nonmatchings/fx_1b4", fx_1b4_headerPtr);

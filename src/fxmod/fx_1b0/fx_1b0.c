#include "fxmod/fx_1b0.h"

void fx_1b0_initSplit(Fx1b0Split *split, SVECTOR (*faces)[3]) {
    Fx1b0Piece *piece;
    s32 i;

    fx_1b0_split = split;
    fx_1b0_pieces = split->pieces4C;
    piece = split->pieces4C;
    split->count0 = 0;
    split->count2 = 0;
    split->nextFace28 = faces;
    split->faces24 = faces;
    for (i = 0; i < 0x7BC; i++) {
        piece->frame18 = -1;
        piece++;
    }
}

static s32 fxm_splitModelAdd(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b0_env);
    Fx1b0Split *split = fx_1b0_split;
    SVECTOR (*face)[3] = split->nextFace28;
    s32 n;
    s32 i;
    s32 slot;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)face);
    slot = split->count0;
    split->modelFaces2C[slot] = face;
    split->counts4[slot] = n;
    split->nextFace28 = face + n;
    split->count2 += n;
    split->count0++;
    for (i = 0; i < n; i++) {
        (*face)[0].pad = 0;
        face++;
    }
    return split->count2;
}

Fx1b0Piece *fx_1b0_findFreePiece(void) {
    Fx1b0Piece *piece = fx_1b0_pieces;
    s32 i;

    for (i = 0; i < 0x7BC; i++, piece++) {
        if (piece->frame18 < 0) {
            return piece;
        }
    }
    return NULL;
}

s32 fx_1b0_launchPieces(SVECTOR *center) {
    Fx1b0Split *split = fx_1b0_split;
    SVECTOR (*face)[3] = split->faces24;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b0_env);
    SVECTOR offset;
    Fx1b0Piece *piece;
    s32 faceIdx;
    s32 i;
    s32 count;

    faceIdx = 0;
    i = 0;
    count = 0;
    for (; i < split->count2; i++, face++, faceIdx++) {
        piece = fx_1b0_findFreePiece();
        if (piece != NULL) {
            (*face)[0].pad = 1;
            piece->posX0 = (((*face)[0].vx + (*face)[1].vx + (*face)[2].vx) / 3) << 12;
            piece->posY4 = (((*face)[0].vy + (*face)[1].vy + (*face)[2].vy) / 3) << 12;
            piece->posZ8 = (((*face)[0].vz + (*face)[1].vz + (*face)[2].vz) / 3) << 12;
            piece->frame18 = 0;
            piece->face1A = faceIdx;
            piece->velC.pad = 0x1000;
            piece->rotX14 = ex->btlfx_randCentered(0x40);
            piece->rotY16 = ex->btlfx_randCentered(0x80);
            offset.vx = (piece->posX0 >> 12) - center->vx;
            offset.vy = (piece->posY4 >> 12) - center->vy;
            offset.vz = (piece->posZ8 >> 12) - center->vz;
            piece->velC.vx = offset.vx / 10 + ex->btlfx_randCentered(0x10);
            piece->velC.vy = offset.vy / 10 + ex->btlfx_randCentered(0x10);
            piece->velC.vz = offset.vz / 10 + ex->btlfx_randCentered(0x10);
            count++;
        }
    }
    return count;
}

void fx_1b0_updatePieces(void) {
    Fx1b0Split *split;
    Fx1b0Piece *piece;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR rel[3];
    s32 zero[2];
    VECTOR world;
    VECTOR scale;
    MATRIX m;
    SVECTOR (*face)[3];
    s32 i;

    split = fx_1b0_split;
    piece = fx_1b0_pieces;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(fx_1b0_env);
    scale.vx = scale.vy = 0x1000;
    scale.vz = 0xF00;
    rot.vz = 0;
    for (i = 0; i < 0x7BC; i++, piece++) {
        if (piece->frame18 >= 0) {
            face = &split->faces24[piece->face1A];
            center.vx = piece->posX0 >> 12;
            center.vy = piece->posY4 >> 12;
            center.vz = piece->posZ8 >> 12;
            rel[0].vx = (*face)[0].vx - center.vx;
            rel[0].vy = (*face)[0].vy - center.vy;
            rel[0].vz = (*face)[0].vz - center.vz;
            rel[1].vx = (*face)[1].vx - center.vx;
            rel[1].vy = (*face)[1].vy - center.vy;
            rel[1].vz = (*face)[1].vz - center.vz;
            rel[2].vx = (*face)[2].vx - center.vx;
            rel[2].vy = (*face)[2].vy - center.vy;
            rel[2].vz = (*face)[2].vz - center.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)piece, 0, (SVECTOR *)zero, &out);
            if (piece->posY4 >= 0) {
                *(s32 *)&rot.vx = *(s32 *)&piece->rotX14;
                ex->math_rotMatrixYXZ(&rot, &m);
                m.t[0] = piece->posX0 >> 12;
                m.t[1] = piece->posY4 >> 12;
                m.t[2] = piece->posZ8 >> 12;
                ex->ScaleMatrix(&m, &scale);
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                gte_ldv0(&rel[0]);
                gte_rtv0tr();
                gte_stlvnl(&world);
                gte_ldv0(&rel[1]);
                gte_rtv0tr();
                (*face)[0].vx = world.vx;
                (*face)[0].vy = world.vy;
                (*face)[0].vz = world.vz;
                gte_stlvnl(&world);
                gte_ldv0(&rel[2]);
                gte_rtv0tr();
                (*face)[1].vx = world.vx;
                (*face)[1].vy = world.vy;
                (*face)[1].vz = world.vz;
                gte_stlvnl(&world);
                (*face)[2].vx = world.vx;
                (*face)[2].vy = world.vy;
                (*face)[2].vz = world.vz;
                piece->frame18++;
                if (piece->frame18 < 0x80) {
                    continue;
                }
            }
            piece->frame18 = -1;
            (*face)[2].vz = 0;
            *(s32 *)&(*face)[2].vx = 0;
            (*face)[1].vz = 0;
            *(s32 *)&(*face)[1].vx = 0;
            (*face)[0].vz = 0;
            *(s32 *)&(*face)[0].vx = 0;
        }
    }
}

static s32 fx_1b0_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 slot = pt->slot - 1;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b0_env);
    Fx1b0Orbit *orbit = &fx_1b0_orbits[slot];
    SVECTOR orbitPos;

    if (frame == 0) {
        orbit->angle0.vx = slot << 9;
        orbit->angle0.vy = slot * 0x140;
        orbit->angle0.vz = 0;
        orbit->spin8 = ex->btlfx_randCentered(0x20) + 0x40;
        orbit->spinA = ex->btlfx_randCentered(0x20) + 0x190;
    }
    ex->btlfx_pointAhead((s16 *)&fx_1b0_orbitCenter, (s16 *)&orbit->angle0, (ex->math_sin(((frame << 12) / n) >> 2) * 0xBB8) >> 12,
                      (s16 *)&orbitPos);
    orbit->angle0.vx += orbit->spin8;
    orbit->angle0.vy += orbit->spinA;
    orbit->spin8 = (orbit->spin8 * 0x11) >> 4;
    orbit->spinA = (orbit->spinA * 7) >> 3;
    pos->vx = orbitPos.vx;
    pos->vy = orbitPos.vy;
    pos->vz = orbitPos.vz;
    pt->pos.vx = orbitPos.vx << 12;
    pt->pos.vy = orbitPos.vy << 12;
    pt->pos.vz = orbitPos.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_1b0_op80(s32 mode, Fx1b0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR scratch;
    SVECTOR origin;
    VECTOR scale;
    Pool6CEntry *emitter;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 gridBright;
    s32 gridGrow;
    s32 tint;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = 0x5C;
    } else if (mode == 1) {
        fx_1b0_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&scratch);
        work->pos8[0] = scratch.vx;
        work->pos8[1] = scratch.vy;
        work->pos8[2] = scratch.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        ex->btlfx_getUnitScale(0x10, work->scale1C);
        work->rot14.vy += 0x800;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos28);
        fx_1b0_orbitCenter.vx = work->pos28.vx;
        fx_1b0_orbitCenter.vy = work->pos28.vy;
        fx_1b0_orbitCenter.vz = work->pos28.vz;
        work->unk30.vx = work->pos28.vx;
        work->unk30.vy = work->pos28.vy;
        work->unk30.vz = work->pos28.vz;
        work->unk30.vy = 0;
        env->camera->points[3].vx = scratch.vx;
        env->camera->points[3].vy = scratch.vy;
        fx_1b0_task = entry;
        env->camera->points[3].vz = scratch.vz;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        gridBright = 0;
        gridGrow = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            env->otZ = 0xC8;
            t = (frame << 12) / 90;
            if (frame < 0x3D) {
                scratch.vz = 0;
                scratch.vy = 0;
                scratch.vx = 0;
                scratch.pad = 1;
                scratch.vx = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &work->pos28, &scratch, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    scratch.vx += 0x200;
                }
                scratch.vx = 0;
                scratch.vy = ex->math_cos(t >> 2);
                for (i = 0; i < 4; i++) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &work->pos28, &scratch, 0x4000, 0, 1,
                                      ex->math_sin(t >> 1));
                    scratch.vy += 0x200;
                }
            }
            env->otZ = 0;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x40, 0x40, 0x55);
            }
            if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x15) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x16) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x40, 0x40, 0x55);
            }
            if ((u32)(frame - 0x20) < 8) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xC8 - (frame - 0x20) * 0x19, 0xC8 - (frame - 0x20) * 0x19, 0xC8 - (frame - 0x20) * 0x19);
            }
            if (frame == 0x2A || frame == 0x30) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x80, 0x80, 0xAA);
            }
            if (frame == 0x2B || frame == 0x31) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x4A) {
                gridBright = ((frame - 0x4A) * 0xFF) / 16;
                gridGrow = gridBright * 2 + gridBright;
            }
            if (frame == 0x3C || frame == 0x46 || frame == 0x50) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0, 0x80, 0xFF);
            }
            if (frame == 0x3D || frame == 0x47 || frame == 0x51) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x80, 0x80, 0xAA);
            }
            if (frame >= 0x46 && (ex->btlfx_rand() & 3) == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x40, 0x40, 0x20);
            }
            if (frame >= 0x3C) {
                ex->btlfx_sampleGradient(fx_1b0_colorGradient, &work->color38, frame - 0x3C);
            }
            if (frame == 0x3B) {
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->btlfx_moveSummonToHeap(0x64);
                ex->btlfx_initEffModels(fx_1b0_effModels, 0xA);
                for (j = 0; j < 8; j++) {
                    work->model3C[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1b0_modelFileIds[j]), 0);
                }
            }
            if (frame >= 0x3C) {
                work->rot14.vy += t >> 2;
                for (i = 0; i < 8; i++) {
                    ex->Hi_DrawEffModel(&work->rot14, (VECTOR *)work->pos8, (s32)work->scale1C, work->model3C[i]);
                    ex->Hi_ModifyEffModelRGB(work->model3C[i], work->color38.r - 0x80, work->color38.g - 0x80, work->color38.b);
                }
            }
            if (frame == 0x10) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x28) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x31) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->file->m.module.update = fx_1b0_op80ParticleMotion;
                    emitter->trails = (s32)fx_1b0_op80Trails0;
                    emitter->trailSlots = 0x20;
                    emitter->trailStride = 0xAC;
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x3C || frame == 0x46) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x59) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x5A) {
                work->state0 = 1;
                entry->frame = -1;
                buf = ((Fx1b0Summon *)env->loader)->summon54->workBuf50 + 0x19000;
                fx_1b0_initSplit((Fx1b0Split *)buf, (SVECTOR (*)[3])(buf + 0xDB88));
                for (i = 0; i < 8; i++) {
                    fxm_splitModelAdd(work->model3C[i]);
                }
                ex->btlfx_initParticleSystem();
            }
            break;
        case 1:
            origin.vx = 0;
            origin.vy = 0;
            origin.vz = 0;
            if (frame == 0) {
                fx_1b0_launchPieces(&origin);
            }
            work->rot14.vy += 0x18;
            for (i = 0; i < 8; i++) {
                ex->Hi_DrawEffModel(&work->rot14, (VECTOR *)work->pos8, (s32)work->scale1C, work->model3C[i]);
            }
            fx_1b0_updatePieces();
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0, 0xFF, 0xFF);
            }
            if (frame < 8) {
                tint = 0xFF - (frame * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, tint, tint, tint >> 1);
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1b0_op80Trails1;
                    emitter->trailSlots = 0x18;
                    emitter->trailStride = 0x8C;
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1b0_op80Trails2;
                    emitter->trailSlots = 0x1C;
                    emitter->trailStride = 0x8C;
                    emitter->otZ = 0x140;
                }
            }
            if (frame >= 0x80) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (gridBright != 0) {
            ((CVECTOR *)&scratch)->r = ((CVECTOR *)&scratch)->g = ((CVECTOR *)&scratch)->b = gridBright;
            origin.vx = 0;
            origin.vy = 0;
            origin.vz = 0;
            scale.vx = scale.vy = scale.vz = gridGrow + 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &origin, &scale, (CVECTOR *)&scratch, 1, 0);
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_1b0_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1b0_env = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b0/nonmatchings/fx_1b0", fx_1b0_headerPtr);

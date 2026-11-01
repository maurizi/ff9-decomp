#include "fxmod/fx_1b7.h"

void fx_1b7_initSplit(Fx1b7Split *split, SVECTOR (*faces)[3]) {
    Fx1b7Piece *piece;
    s32 i;

    fx_1b7_split = split;
    fx_1b7_pieces = split->pieces4C;
    piece = split->pieces4C;
    split->count0 = 0;
    split->count2 = 0;
    split->nextFace28 = faces;
    split->faces24 = faces;
    for (i = 0; i < 0x6A4; i++) {
        piece->frame18 = -1;
        piece++;
    }
}

static s32 fxm_splitModelAdd(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b7_env);
    Fx1b7Split *split = fx_1b7_split;
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

Fx1b7Piece *fx_1b7_findFreePiece(void) {
    Fx1b7Piece *piece = fx_1b7_pieces;
    s32 i;

    for (i = 0; i < 0x6A4; i++, piece++) {
        if (piece->frame18 < 0) {
            return piece;
        }
    }
    return NULL;
}

s32 fx_1b7_launchPieces(void) {
    Fx1b7Split *split = fx_1b7_split;
    SVECTOR (*face)[3] = split->faces24;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b7_env);
    s32 sweepY = fx_1b7_sweepLine;
    SVECTOR center;
    Fx1b7Piece *piece;
    s32 faceIdx;
    s32 i;
    s32 count;

    faceIdx = 0;
    i = 0;
    count = 0;
    for (; i < split->count2; i++, face++, faceIdx++) {
        if (sweepY < (*face)[1].vy && (*face)[0].pad == 0) {
            piece = fx_1b7_findFreePiece();
            if (piece != NULL) {
                (*face)[0].pad = 1;
                piece->posX0 = (((*face)[0].vx + (*face)[1].vx + (*face)[2].vx) / 3) << 12;
                piece->posY4 = (((*face)[0].vy + (*face)[1].vy + (*face)[2].vy) / 3) << 12;
                piece->posZ8 = (((*face)[0].vz + (*face)[1].vz + (*face)[2].vz) / 3) << 12;
                piece->frame18 = 0;
                piece->face1A = faceIdx;
                piece->velC.pad = 0x1000;
                piece->rotX14 = ex->btlfx_randCentered(0x100);
                piece->rotY16 = ex->btlfx_randCentered(0x200);
                center.vx = piece->posX0 >> 12;
                center.vy = piece->posY4 >> 12;
                center.vz = piece->posZ8 >> 12;
                piece->velC.vx = (center.vx >> 3) + ex->btlfx_randCentered(0x10);
                count++;
                piece->velC.vy = (center.vy >> 5) + (ex->btlfx_rand() & 0xF);
                piece->velC.vz = (center.vz >> 3) + ex->btlfx_randCentered(0x10);
            }
        }
    }
    return count;
}

void fx_1b7_updatePieces(void) {
    SVECTOR dir;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR ang;
    SVECTOR rel[3];
    s32 zero[2];
    VECTOR world;
    VECTOR scale;
    MATRIX m;
    Fx1b7Split *split;
    Fx1b7Piece *piece;
    BtlfxExports *ex;
    SVECTOR (*face)[3];
    s32 i;

    split = fx_1b7_split;
    piece = fx_1b7_pieces;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(fx_1b7_env);
    dir.vx = 0;
    dir.vy = 0;
    dir.vz = -0x2BC;
    scale.vx = scale.vy = scale.vz = 0xF80;
    rot.vz = 0;
    for (i = 0; i < 0x6A4; i++, piece++) {
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
            ex->btlfx_aimAngles((s16 *)&out, (s16 *)&dir, (s16 *)&ang);
            ang.vx = 0;
            ang.vy += 0x400;
            ex->btlfx_rotateOffset(&ang, 0xA0, (s32)&rot);
            piece->posX0 += rot.vx << 12;
            piece->posY4 += rot.vy << 12;
            piece->posZ8 += rot.vz << 12;
            *(s32 *)&rot.vx = *(s32 *)&piece->rotX14;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = piece->posX0 >> 12;
            m.t[1] = piece->posY4 >> 12;
            m.t[2] = piece->posZ8 >> 12;
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
            if (piece->frame18 >= 0x20) {
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
}

s32 fx_1b7_op80(s32 mode, Fx1b7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 sp;
    Pool6CEntry *emitter;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 ang;
    s32 tint;
    s32 r;
    s32 g;
    s32 b;
    s32 i;

    if (mode == 0) {
        *args = 0x64;
    } else if (mode == 1) {
        fx_1b7_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vz -= 0x258;
        work->pos8[0] = pos.vx;
        work->pos8[1] = pos.vy;
        work->pos8[2] = pos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        ex->btlfx_getUnitScale(0x10, work->scale1C);
        work->rot14.vy += 0x800;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos28);
        work->pos28.vy -= 0x180;
        work->pos30.vx = work->pos28.vx;
        work->pos30.vy = work->pos28.vy;
        work->pos30.vz = work->pos28.vz;
        work->pos30.vy = 0;
        work->pos38.vx = work->pos30.vx;
        work->pos38.vy = work->pos30.vy;
        work->pos38.vz = work->pos30.vz;
        work->pos38.vy = -0x280;
        pos.vy -= 0x80;
        env->camera->points[3].vx = pos.vx;
        env->camera->points[3].vy = pos.vy;
        fx_1b7_task = entry;
        env->camera->points[3].vz = pos.vz;
        work->state0 = 0;
        work->unk4 = 0;
        work->emit40 = NULL;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 7) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1b7_op80Trails0;
                    emitter->trailSlots = 0x20;
                    emitter->trailStride = 0xAC;
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0xB) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x1D) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x27) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x20) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x10) {
                work->emit40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos28);
                if (work->emit40 != NULL) {
                    work->emit40->otZ = 0x140;
                }
            }
            if (work->emit40 != NULL && work->emit40->state == BTLFX_EMIT_DONE) {
                work->emit40 = NULL;
            }
            if (frame == 1 || frame == 0xF || frame == 0x14 || frame == 0x24) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 0x40, 0x80, 0x80);
            }
            if (frame == 2 || frame == 0x10 || frame == 0x15 || frame == 0x25) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x64, 0x64, 0x40);
            }
            if (frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x2C) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit40);
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->btlfx_moveSummonToHeap(0x6E);
                ex->btlfx_initEffModels(fx_1b7_effModels, 8);
                for (i = 0; i < 8; i++) {
                    work->model44[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1b7_modelFileIds[i]), 0);
                }
                buf = ((Fx1b7Summon *)env->loader)->summon54->workBuf50 + 0x1B800;
                fx_1b7_initSplit((Fx1b7Split *)buf, (SVECTOR (*)[3])(buf + 0xDBD8));
                for (i = 0; i < 8; i++) {
                    fxm_splitModelAdd(work->model44[i]);
                }
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xC0, 0xAA, 0x20);
            }
            break;
        case 1:
            t = (frame << 12) / 130;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0x60, 0x55, 0x10);
            }
            if ((u32)(frame - 4) < 0x18) {
                tint = 0xFF - ((frame - 4) * 0xFF) / 24;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, tint, tint, tint >> 1);
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos38);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos30);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1b7_op80Trails1;
                    emitter->trailSlots = 0x1C;
                    emitter->trailStride = 0xAC;
                    emitter->otZ = 0x140;
                }
            }
            if (frame > 0) {
                ang = t >> 2;
                r = (ex->math_cos(ang) * 2) >> 5;
                g = ex->math_cos(ang);
                b = t + 0x1000;
                for (i = 0; i < 8; i++) {
                    ex->Hi_DrawEffModel(&work->rot14, (VECTOR *)work->pos8, (s32)work->scale1C, work->model44[i]);
                    ex->Hi_ModifyEffModelRGB(work->model44[i], r - 0x80, ((g * 3 / 2) >> 5) - 0x80, (b >> 5) - 0x80);
                }
                fx_1b7_sweepLine = 0x898 - ((t << 12) >> 12);
                FX1B7_SPAD_STACK(&sp);
                fx_1b7_launchPieces();
                fx_1b7_updatePieces();
                FX1B7_RESET_STACK(&sp);
            }
            if (frame >= 0x82) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1b7_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1b7_env = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_headerPtr);

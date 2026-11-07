#include "fxmod/fx_1b6.h"

void fx_1b6_initSplit(Fx1b6Split *split, SVECTOR (*faces)[3]) {
    Fx1b6Piece *piece;
    s32 i;

    fx_1b6_split = split;
    fx_1b6_pieces = split->pieces44;
    piece = split->pieces44;
    split->count0 = 0;
    split->count2 = 0;
    split->nextFace24 = faces;
    split->faces20 = faces;
    for (i = 0; i < FX_DEATH_PIECES; i++) {
        piece->frame18 = -1;
        piece++;
    }
}

static s32 fxm_splitModelAdd(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b6_env);
    Fx1b6Split *split = fx_1b6_split;
    SVECTOR (*face)[3] = split->nextFace24;
    s32 n;
    s32 i;
    s32 slot;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / (3 * sizeof(SVECTOR));
    ex->Hi_SplitMdlVertex(model, -1, (s32)face);
    slot = split->count0;
    split->modelFaces28[slot] = face;
    split->counts4[slot] = n;
    split->nextFace24 = face + n;
    split->count2 += n;
    split->count0++;
    for (i = 0; i < n; i++) {
        (*face)[0].pad = 0;
        face++;
    }
    return split->count2;
}

Fx1b6Piece *fx_1b6_findFreePiece(void) {
    Fx1b6Piece *piece = fx_1b6_pieces;
    s32 i;

    for (i = 0; i < FX_DEATH_PIECES; i++, piece++) {
        if (piece->frame18 < 0) {
            return piece;
        }
    }
    return NULL;
}

void fx_1b6_launchPieces(void) {
    Fx1b6Split *split = fx_1b6_split;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b6_env);
    s32 sweepZ = fx_1b6_sweepLine;
    SVECTOR (*face)[3] = split->faces20;
    SVECTOR center;
    Fx1b6Piece *piece;
    s32 faceIdx;
    s32 i;

    faceIdx = 0;
    i = 0;
    for (; i < split->count2; i++, face++, faceIdx++) {
        if (sweepZ < (*face)[1].vz && (*face)[0].pad == 0) {
            piece = fx_1b6_findFreePiece();
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
                piece->velC.vx = (center.vx >> 2) + ex->btlfx_randCentered(0x10);
                piece->velC.vy = (center.vy >> 5) + ex->btlfx_randCentered(0x10);
                piece->velC.vz = ex->btlfx_randCentered(0x10) + 0x80;
            }
        }
    }
}

void fx_1b6_updatePieces(void) {
    Fx1b6Split *split;
    Fx1b6Piece *piece;
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

    split = fx_1b6_split;
    piece = fx_1b6_pieces;
    zero[0] = 0;
    zero[1] = 0;
    ex = BTLFX_EXPORTS(fx_1b6_env);
    scale.vx = scale.vy = scale.vz = 0xE80;
    rot.vz = 0;
    for (i = 0; i < FX_DEATH_PIECES; i++, piece++) {
        if (piece->frame18 >= 0) {
            face = &split->faces20[piece->face1A];
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
            ex->btlfx_stepModuleParticle((Pool6CEntry *)piece, 0xB, (SVECTOR *)zero, &out);
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
            if (piece->frame18 >= FX_DEATH_PIECE_FRAMES) {
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

s32 fx_1b6_op80(s32 mode, Fx1b6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 sp;
    Pool6CEntry *emitter;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 tint;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1b6Work80);
    } else if (mode == 1) {
        fx_1b6_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vy -= 0x110;
        work->pos8[0] = pos.vx;
        work->pos8[1] = pos.vy;
        work->pos8[2] = pos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        ex->btlfx_getUnitScale(0x10, work->scale1C);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos44);
        work->pos44.vy -= 0x397;
        BTLFX_COPY_SVEC(&work->pos4C, &work->pos44);
        work->pos4C.vz += 0x12C;
        work->state0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&work->pos54, &work->pos4C);
        work->rot14.vy += 0x800;
        fx_1b6_task = entry;
        work->emit5C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_CYAN_RINGS), (s32)&work->pos44);
        if (emitter != NULL) {
            emitter->otZ = 0x140;
        }
        work->emit60 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_YELLOW_RINGS), (s32)&work->pos44);
        if (emitter != NULL) {
            emitter->otZ = 0x140;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_RED_LIGHTNING), (s32)&work->pos44);
        if (emitter != NULL) {
            emitter->otZ = 0x140;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_DEATH_STATE_CHARGE:
            t = (frame << 12) / FX_DEATH_CHARGE_FRAMES;
            if (work->emit5C != NULL) {
                if (work->emit5C->state == BTLFX_EMIT_DONE) {
                    work->emit5C = NULL;
                }
                if (work->emit5C != NULL) {
                    work->emit5C->alpha = 0x1000 - t;
                }
            }
            if (work->emit60 != NULL) {
                if (work->emit60->state == BTLFX_EMIT_DONE) {
                    work->emit60 = NULL;
                }
                if (work->emit60 != NULL) {
                    work->emit60->alpha = t;
                }
            }
            if (frame == FX_DEATH_FLICKER_FRAME || frame == FX_DEATH_FLICKER_2_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, FX_DEATH_FLICKER_RGB);
            }
            if (frame == FX_DEATH_CHARGE_FRAMES - 1) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, FX_DEATH_WHITE_RGB);
            }
            if (frame >= FX_DEATH_CHARGE_FRAMES) {
                ex->btlfx_initParticleSystem();
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->btlfx_moveSummonToHeap(0x64);
                work->state0 = FX_DEATH_STATE_SHATTER;
                entry->frame = -1;
                ex->btlfx_initEffModels(fx_1b6_effModels, FX_DEATH_MODELS);
                for (i = 0; i < FX_DEATH_MODELS; i++) {
                    work->model28[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1b6_modelFileIds[i]), 0);
                }
                buf = ((Fx1b6Summon *)env->loader)->summon54->workBuf50 + 0x19000;
                fx_1b6_initSplit((Fx1b6Split *)buf, (SVECTOR (*)[3])(buf + 0xC5A8));
                for (i = 0; i < FX_DEATH_MODELS; i++) {
                    fxm_splitModelAdd(work->model28[i]);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_STAR_SPECKS), (s32)&work->pos54);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_BUBBLE_RINGS), (s32)&work->pos44);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SPIRAL_TRAIL), (s32)&work->pos4C);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1b6_op80Trails;
                    emitter->trailSlots = FX_DEATH_SPIRAL_TRAIL_TRAIL_SLOTS;
                    emitter->trailStride = FX_DEATH_SPIRAL_TRAIL_TRAIL_STRIDE;
                    emitter->otZ = 0x140;
                }
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, FX_DEATH_FLASH_RGB);
            }
            break;
        case FX_DEATH_STATE_SHATTER:
            t = (frame << 12) / FX_DEATH_SHATTER_FRAMES;
            if (frame < FX_DEATH_WHITE_FADE_FRAMES) {
                tint = (ex->math_cos((frame << 10) >> 3) << 7) >> 12;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, tint, tint, tint);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, FX_DEATH_BLUE_RGB);
            }
            if (frame > 0) {
                for (i = 0; i < FX_DEATH_MODELS; i++) {
                    ex->Hi_DrawEffModel(&work->rot14, (VECTOR *)work->pos8, (s32)work->scale1C, work->model28[i]);
                }
                if (frame < FX_DEATH_SWEEP_FRAMES) {
                    fx_1b6_sweepLine = 0x514 - ((t * 0x960) >> 12);
                } else {
                    fx_1b6_sweepLine = -0x1000;
                }
                FX1B6_SPAD_STACK(&sp);
                fx_1b6_launchPieces();
                fx_1b6_updatePieces();
                FX1B6_RESET_STACK(&sp);
            }
            if (frame >= FX_DEATH_SHATTER_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1b6_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1b6_env = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

s32 fx_1b6_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_1b6_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vy += 0x180;
        env->camera->points[3].vx = pos.vx;
        env->camera->points[3].vy = pos.vy;
        env->camera->points[3].vz = pos.vz;
        return 1;
    } else {
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b6/nonmatchings/fx_1b6", fx_1b6_headerPtr);

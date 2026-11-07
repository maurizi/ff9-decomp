#include "fxmod/fx_034.h"

void fx_034_explodeModel(s32 model, s32 scale, u8 *buf) {
    s32 meshCount;
    s32 j = 0;

    meshCount = BTLFX_EXPORTS(fx_034_env)->btlfx_getEffModelMeshCount(model & 0xFFFF);
    for (; j < meshCount; j++) {
        for (model = 0; model < FX_COLD_BREATH_PRIM_KINDS; model++) {
            fx_034_pushPrims(model, fx_034_primCounts[model], scale, fx_034_primVerts[model]);
        }
    }
}

INCLUDE_RODATA("asm/us/fx_034/nonmatchings/fx_034", fx_034_headerPtr);

void fx_034_pushPrims(s32 kind, s32 count, s32 scale, SVECTOR *verts) {
    SVECTOR moved;
    SVECTOR centre;
    SVECTOR offset;
    s32 vertsPerPrim;
    s32 k;
    s32 m;

    switch (kind) {
    case BTL_MESH_PRIM_FT4:
    case BTL_MESH_PRIM_GT4:
    case BTL_MESH_PRIM_G4:
    case BTL_MESH_PRIM_F4:
        vertsPerPrim = 4;
        break;
    case BTL_MESH_PRIM_FT3:
    case BTL_MESH_PRIM_GT3:
    case BTL_MESH_PRIM_G3:
    case BTL_MESH_PRIM_F3:
        vertsPerPrim = 3;
        break;
    default:
        vertsPerPrim = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        moved.vx = fx_034_primCentres[k].x;
        moved.vy = fx_034_primCentres[k].y;
        moved.vz = fx_034_primCentres[k].z;
        centre.vx = fx_034_primCentres[k].x;
        centre.vy = fx_034_primCentres[k].y;
        centre.vz = fx_034_primCentres[k].z;
        offset.vx = scale * moved.vx / 200;
        moved.vx += offset.vx;
        offset.vy = scale * moved.vy / 200;
        moved.vy += offset.vy;
        offset.vz = scale * moved.vz / 200;
        moved.vz += offset.vz;
        for (m = 0; m < vertsPerPrim; m++, verts++) {
            verts->vx -= centre.vx;
            verts->vy -= centre.vy;
            verts->vz -= centre.vz;
            verts->vx += moved.vx;
            verts->vy += moved.vy;
            verts->vz += moved.vz;
        }
    }
}

void fx_034_splitModel(s32 model, u8 *buf) {
    BtlfxExports *ex;
    s32 meshCount;
    s32 i;
    s32 j;
    s32 handle;
    s32 mdl;

    ex = BTLFX_EXPORTS(fx_034_env);
    mdl = model;
    j = 0;
    handle = mdl & 0xFFFF;
    ex->Hi_SplitMdlVertex(handle, -1, (s32)buf);
    meshCount = ex->btlfx_getEffModelMeshCount(handle);
    for (; j < meshCount; j++) {
        for (i = 0; i < FX_COLD_BREATH_PRIM_KINDS; i++) {
            fx_034_primCounts[i] = ex->Hi_GetMdlVertexPtr(mdl & 0xFFFF, j, (s32)buf, (s32 *)&fx_034_primVerts[i], i);
            fx_034_calcPrimCentres(i, fx_034_primCounts[i], fx_034_primVerts[i]);
        }
    }
}

void fx_034_calcPrimCentres(s32 kind, s32 count, SVECTOR *verts) {
    SVECTOR sum;
    s32 vertsPerPrim;
    s32 k;
    s32 m;

    switch (kind) {
    case BTL_MESH_PRIM_FT4:
    case BTL_MESH_PRIM_GT4:
    case BTL_MESH_PRIM_G4:
    case BTL_MESH_PRIM_F4:
        vertsPerPrim = 4;
        break;
    case BTL_MESH_PRIM_FT3:
    case BTL_MESH_PRIM_GT3:
    case BTL_MESH_PRIM_G3:
    case BTL_MESH_PRIM_F3:
    default:
        vertsPerPrim = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (m = 0; m < vertsPerPrim; m++, verts++) {
            sum.vx += verts->vx;
            sum.vy += verts->vy;
            sum.vz += verts->vz;
        }
        fx_034_primCentres[k].x = sum.vx / vertsPerPrim;
        fx_034_primCentres[k].y = sum.vy / vertsPerPrim;
        fx_034_primCentres[k].z = sum.vz / vertsPerPrim;
    }
}

s32 fx_034_op80(s32 mode, Fx034Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR landPos;
    SVECTOR backAim;
    Pool6CEntry *emitter;
    s32 frame;
    s32 elapsed;
    s32 size;
    s32 early;

    if (mode == 0) {
        *args = sizeof(Fx034Work);
    } else if (mode == 1) {
        fx_034_env = env;
        ex->btlfx_initEffModels(fx_034_effModels, FX_COLD_BREATH_MODEL_COUNT);
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos34);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot3C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos44);
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos4C);
        work->emit0 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COLD_BREATH_ICE_CHUNKS), (s32)&work->pos4C);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_034_trails;
            work->emit0->trailSlots = FX_COLD_BREATH_ICE_CHUNKS_TRAIL_SLOTS;
            work->emit0->trailStride = FX_COLD_BREATH_ICE_CHUNKS_TRAIL_STRIDE;
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        ex->btlfx_aimAngles((s16 *)&work->pos4C, (s16 *)&work->pos44, (s16 *)&work->aim54);
        backAim.vx = work->aim54.vx;
        backAim.vy = work->aim54.vy;
        backAim.vz = work->aim54.vz;
        backAim.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->pos44, (s16 *)&backAim, FX_COLD_BREATH_CRYSTAL_DIST, (s16 *)&landPos);
        ex->btlfx_aimAngles((s16 *)&work->pos34, (s16 *)&landPos, (s16 *)&work->aim5C);
        work->aim5C.vy += 0x800;
        work->pos64.vx = landPos.vx;
        work->pos64.vy = landPos.vy;
        work->pos64.vz = landPos.vz;
        work->rot8.vx = 0;
        work->rot8.vz = 0;
        work->rot8.vy = work->aim54.vy;
        work->pos10.vx = landPos.vx;
        work->pos10.vy = landPos.vy;
        work->pos10.vz = landPos.vz;
        work->scale20.vx = 0x800;
        work->scale20.vy = 0x800;
        work->scale20.vz = 0x800;
        work->rgb30 = -0x20;
        work->model6E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_COLD_BREATH_ICE_CRYSTAL), FX_COLD_BREATH_ICE_CRYSTAL_TPAGE, FX_COLD_BREATH_ICE_CRYSTAL_CLUT, 0, 0);
        work->model70 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_COLD_BREATH_ICE_CRYSTAL_2), FX_COLD_BREATH_ICE_CRYSTAL_2_TPAGE, FX_COLD_BREATH_ICE_CRYSTAL_2_CLUT, 0,
                                                 FX_COLD_BREATH_ICE_CRYSTAL_2_V);
        work->scroll6C = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_COLD_BREATH_FROST_FRAME:
            work->emit4 = emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COLD_BREATH_FROST_BREATH), (s32)&work->pos64);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            break;
        case FX_COLD_BREATH_SHATTER_FRAME:
            fx_034_splitModel(work->model6E, fx_034_splitBuf);
            break;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim54.vx;
                work->emit0->rot.vy = work->aim54.vy;
                work->emit0->rot.vz = work->aim54.vz;
            }
        }
        if (frame >= FX_COLD_BREATH_FROST_FRAME && work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim5C.vx;
                work->emit4->rot.vy = work->aim5C.vy;
                work->emit4->rot.vz = work->aim5C.vz;
            }
        }
        elapsed = frame - FX_COLD_BREATH_FROST_FRAME;
        if (elapsed >= 0 && elapsed < FX_COLD_BREATH_CRYSTAL_FRAMES) {
            if (elapsed < FX_COLD_BREATH_GROW_FRAMES) {
                size = (elapsed << 11) / (FX_COLD_BREATH_GROW_FRAMES - 1);
                work->scale20.vx = size;
                work->scale20.vy = size;
                work->scale20.vz = size;
            }
            early = frame < FX_COLD_BREATH_SHATTER_FRAME;
            if (!early) {
                fx_034_explodeModel(work->model6E, frame, fx_034_splitBuf);
            }
            ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model6E);
            if (early) {
                ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model70);
                ex->Hi_ModifyEffModelAbr(work->model70, BTLFX_ABR_ADD);
            } else {
                work->scale20.vx -= 0x10;
                work->scale20.vy -= 0x10;
                work->scale20.vz -= 0x10;
                ex->Hi_ModifyEffModelAbr(work->model6E, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelRGB(work->model6E, work->rgb30, work->rgb30, work->rgb30);
                work->rgb30 -= 8;
            }
            if (frame >= FX_COLD_BREATH_GLEAM_FRAME && frame < FX_COLD_BREATH_SHATTER_FRAME) {
                ex->btlfx_drawScrollMove(0x3C0 - work->scroll6C * 0x10, FX_COLD_BREATH_GLEAM_VRAM_X, FX_COLD_BREATH_GLEAM_VRAM_Y,
                                          FX_COLD_BREATH_GLEAM_VRAM_W, FX_COLD_BREATH_GLEAM_VRAM_H, FX_COLD_BREATH_GLEAM_SRC_X,
                                          FX_COLD_BREATH_GLEAM_SRC_Y);
                work->scroll6C++;
            }
        }
        work->scroll6C++;
        if (frame >= FX_COLD_BREATH_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

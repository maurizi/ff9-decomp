#include "fxmod/fx_096.h"

void fx_096_explodeModel(u16 model, s32 idx, s32 t, u16 *verts, Fx096Vec3 *centers) {
    s32 meshCount;
    s32 i;
    s32 j;

    meshCount = BTLFX_EXPORTS(fx_096_env)->btlfx_getEffModelMeshCount(model);
    for (j = 0; j < meshCount; j++) {
        for (i = 0; i < 8; i++) {
            fx_096_explodePolys(i, fx_096_polyCounts[idx][i], t, (SVECTOR *)fx_096_polyVerts[idx][i], centers);
        }
    }
}

INCLUDE_RODATA("asm/us/fx_096/nonmatchings/fx_096", fx_096_headerPtr);

void fx_096_explodePolys(u32 kind, s32 count, s32 t, SVECTOR *verts, Fx096Vec3 *centers) {
    Fx096Vec3 offset;
    Fx096Vec3 to;
    Fx096Vec3 from;
    s32 nv;
    s32 k;
    s32 j;
    s32 scaled;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        do {
            nv = 4;
        } while (0); /* loop notes: changes the register allocation of nv */
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        nv = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        to.vx = centers[k].vx;
        to.vy = centers[k].vy;
        to.vz = centers[k].vz;
        from.vx = centers[k].vx;
        from.vy = centers[k].vy;
        from.vz = centers[k].vz;
        scaled = to.vx * 100;
        offset.vx = t * scaled / 200;
        to.vx += offset.vx;
        scaled = to.vy * 100;
        offset.vy = t * scaled / 200;
        to.vy += offset.vy;
        scaled = to.vz * 100;
        offset.vz = t * scaled / 200;
        to.vz += offset.vz;
        for (j = 0; j < nv; j++, verts++) {
            verts->vx -= from.vx;
            verts->vy -= from.vy;
            verts->vz -= from.vz;
            verts->vx += to.vx;
            verts->vy += to.vy;
            verts->vz += to.vz;
        }
    }
}

void fx_096_splitModel(u16 model, s32 idx, u16 *verts, Fx096Vec3 *centers) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_096_env);
    s32 polyCount;
    s32 meshCount;
    s32 i;
    s32 j;

    ex->Hi_SplitMdlVertex(model, -1, (s32)verts);
    meshCount = ex->btlfx_getEffModelMeshCount(model);
    for (j = 0; j < meshCount; j++) {
        for (i = 0; i < 8; i++) {
            polyCount = ex->Hi_GetMdlVertexPtr(model, j, (s32)verts, &fx_096_polyVerts[idx][i], i);
            fx_096_polyCounts[idx][i] = polyCount;
            fx_096_calcPolyCenters(i, fx_096_polyCounts[idx][i], (SVECTOR *)fx_096_polyVerts[idx][i], centers);
        }
    }
}

void fx_096_calcPolyCenters(u32 kind, s32 count, SVECTOR *verts, Fx096Vec3 *centers) {
    Fx096Vec3 sum;
    s32 nv;
    s32 k;
    s32 j;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        nv = 4;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        nv = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (j = 0; j < nv; j++, verts++) {
            sum.vx += verts->vx;
            sum.vy += verts->vy;
            sum.vz += verts->vz;
        }
        centers[k].vx = sum.vx / nv;
        centers[k].vy = sum.vy / nv;
        centers[k].vz = sum.vz / nv;
    }
}

s32 fx_096_op80(s32 mode, Fx096Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        fx_096_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FROST_ICE_GLINTS), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x50;
    }
    return 0;
}

s32 fx_096_op81(s32 mode, Fx096Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u32 i;
    s32 t;
    s32 size;

    if (mode == 0) {
        *args = 0x68;
    } else if (mode == 1) {
        fx_096_env = env;
        ex->btlfx_initEffModels(fx_096_effModels, 2);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos58);
        work->scale34.vx = 0x221;
        work->scale34.vy = 0x221;
        work->scale34.vz = 0x221;
        work->scale44.vy = 0x200;
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->rotC.vx = 0;
        work->rotC.vy = 0;
        work->rotC.vz = 0;
        work->scale44.vx = 0;
        work->scale44.vz = 0;
        work->pos14.vx = work->pos58.vx;
        work->pos14.vy = work->pos58.vy;
        work->pos14.vz = work->pos58.vz;
        work->pos24.vx = work->pos58.vx;
        work->pos24.vy = work->pos58.vy;
        work->pos24.vz = work->pos58.vz;
        work->pos14.vy = -0x258;
        work->model62 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FROST_ICE_MODEL), 1);
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FROST_ICE_MODEL_2), 1);
        work->fade60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 7:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FROST_SNOW_BURST), (s32)&work->pos58);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0:
            fx_096_splitModel(work->model62, 0, fx_096_verts, fx_096_polyCenters);
            for (i = 0; i < 0xA28; i++) {
                fx_096_restVerts[i] = fx_096_verts[i];
            }
            break;
        case 0x18:
            work->fade60 = 0x10;
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            if (frame >= 7) {
                size = ((frame - 7) << 9) / 3;
                work->scale44.vx = size;
                work->scale44.vz = size;
                work->scale44.vx = (work->scale44.vx > 0x200) ? 0x200 : work->scale44.vx;
                work->scale44.vz = (work->scale44.vz > 0x200) ? 0x200 : work->scale44.vz;
            }
            if (frame >= 0 && frame < 6) {
                t = frame;
                for (i = 0; i < 0xA28; i++) {
                    fx_096_verts[i] = fx_096_restVerts[i];
                }
                fx_096_explodeModel(work->model62, 0, 5 - t, fx_096_verts, fx_096_polyCenters);
                work->rot4.vy += 0x200;
            }
            ex->Hi_DrawEffModel(&work->rot4, &work->pos14, (s32)&work->scale34, work->model62);
            ex->Hi_DrawEffModel(&work->rotC, &work->pos24, (s32)&work->scale44, work->model64);
            if (work->fade60 >= 0) {
                work->bright54 = fxm_lerpRatio(0, -0x80, 0x10 - work->fade60, 0x10);
                work->fade60--;
                ex->Hi_ModifyEffModelRGB(work->model62, work->bright54, work->bright54, work->bright54);
                ex->Hi_ModifyEffModelRGB(work->model64, work->bright54, work->bright54, work->bright54);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FROST_BACKDROP), frame);
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_096_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

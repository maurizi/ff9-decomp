#include "fxmod/fx_17f.h"

void fx_17f_op80MeltVertex(SVECTOR *v, s32 i) {
    v->vy -= fx_17f_vertexDrops[i];
    if (v->vy < 0) {
        v->vy = 0;
    }
    if (v->vy < 0x1E) {
        v->vx = v->vx * 0x5C / 90;
        v->vz = (s16)(v->vz - 0xC8) * 0x5C / 90 + 0xC8;
    }
}

s32 fx_17f_op80(s32 mode, Fx17fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR ang;
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_17f_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos20);
        if (ex->btlfx_isEnemyUnit(0x10) != 0) {
            ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
            ex->btlfx_getUnitScale(0x10, (VECTOR *)work->scale0);
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
            ex->btlfx_setPartnerByArg(0);
            ex->btlfx_fadeUnitById(0x10, -1);
            work->model1C = (BtlUnit9C8 *)ex->btlfx_swapModelData(0xE, fx_17f_modelBackup);
            ex->btlfx_fadeUnitById(0x11, 0x1000);
            ex->btlfx_fadeUnitById(0x11, -2);
            ex->btlfx_setUnitRot(0x11, (s16 *)&work->rot14);
            ex->btlfx_setUnitScale(0x11, work->scale0);
            ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&work->pos20, (s16 *)&ang);
            ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&ang, -0x104, (s16 *)&work->posC);
            ex->btlfx_setUnitPos(0x11, (s16 *)&work->posC);
        }
        work->pos20.vx = (work->pos20.vx + work->posC.vx) / 2;
        work->pos20.vy = (work->pos20.vy + work->posC.vy) / 2 - 0x11C;
        work->pos20.vz = (work->pos20.vz + work->posC.vz) / 2;
        for (i = 0; i < 0x2BC; i++) {
            fx_17f_vertexDrops[i] = (ex->btlfx_rand() & 7) + 4;
        }
    } else {
        frame = entry->frame;
        ex->btl_forEachModelVertex(work->model1C, fx_17f_op80MeltVertex);
        if (frame >= 0x37) {
            c = 0x80 - (frame - 0x37) * 8;
            if (c < 0x10) {
                c = 0x10;
            }
            ex->btlfx_fadeUnitById(0x11, c);
        }
        if (frame == 3) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos20);
            if (p != NULL) {
                p->otZ = 0x80;
            }
        }
        if (frame == 6) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos20);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos20);
            if (p != NULL) {
                p->otZ = 0x80;
            }
        }
        if (frame == 0x1C) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
        }
        work->pos20.vy += 5;
        if (frame >= 0x47) {
            ex->btlfx_fadeUnitById(0x11, -1);
            ex->btlfx_swapModelData(0xE, fx_17f_modelBackup);
            work->posC.vx = 0;
            work->posC.vy = 0;
            work->posC.vz = 0x3A98;
            ex->btlfx_setUnitPos(0x11, (s16 *)&work->posC);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17f/nonmatchings/fx_17f", fx_17f_headerPtr);

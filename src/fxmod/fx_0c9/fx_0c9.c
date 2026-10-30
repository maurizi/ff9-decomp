#include "fxmod/fx_0c9.h"

s32 fx_0c9_op80(s32 mode, Fx0c9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7D38 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 func_801E7828(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    pos->vx = D_801E7E10.vx;
    pos->vy = D_801E7E10.vy;
    pos->vz = D_801E7E10.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c9_op81(s32 mode, Fx0c9Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 f;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x58;
    } else if (mode == 1) {
        D_801E7D38 = env;
        ex->btlfx_initEffModels(D_801E7D48, 1);
        ex->btlfx_getUnitBodyPos(0x10, 1, (s16 *)&work->unkC);
        work->unkC.vy -= 0x50;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk14);
        work->unk1C.vx = work->unk14.vx;
        work->unk1C.vy = work->unk14.vy;
        work->unk1C.vz = work->unk14.vz;
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk14);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        work->unk24.vx = 0;
        work->unk24.vy = 0;
        work->unk24.vz = 0;
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk3C.vx = 0x1B;
        work->unk3C.vy = 0x1B;
        work->unk3C.vz = 0x1B;
        work->unk54 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0x117, 0, 0, 0xC0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xA:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801E7E18);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7E28;
                work->unk0->unk12 = 0x12;
                work->unk0->unkC = 0xB0;
            }
            if (work->unk0 != NULL) {
                ((EffMotion *)work->unk0->file)->unk8 = func_801E7828;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x10);
            }
            break;
        case 0x1A:
            work->unk4C.vx = work->unk1C.vx;
            work->unk4C.vy = work->unk1C.vy;
            work->unk4C.vz = work->unk1C.vz;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk4C);
            break;
        }
        if (frame >= 0xA) {
            f = frame - 0xA;
            if (f < 0x11) {
                work->unk2C.vx = work->unk1C.vx;
                work->unk2C.vy = work->unk1C.vy;
                work->unk2C.vz = work->unk1C.vz;
            }
            ex->Hi_DrawEffModel(&work->unk24, &work->unk2C, (s32)&work->unk3C, work->unk54);
            ex->Hi_ModifyEffModelAbr(work->unk54, 1);
            if (f < 0xC) {
                work->unk3C.vx++;
                work->unk3C.vy++;
                work->unk3C.vz++;
            } else if (f >= 0x2E) {
                work->unk3C.vx -= 8;
                work->unk3C.vy -= 8;
                work->unk3C.vz -= 8;
                if (work->unk3C.vx < 0) {
                    work->unk3C.vx = 0;
                    work->unk3C.vy = 0;
                    work->unk3C.vz = 0;
                }
            }
            if (f < 0x11) {
                D_801E7E10.vx = work->unk1C.vx;
                D_801E7E10.vy = work->unk1C.vy;
                D_801E7E10.vz = work->unk1C.vz;
                t = (f << 12) / 15;
                ex->btlfx_lerpVec((s16 *)&work->unk14, (s16 *)&work->unkC, t, (s16 *)&work->unk1C);
                work->unk1C.vy -= (ex->math_sin(t >> 1) * 0x258) >> 12;
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 6), f);
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c9/nonmatchings/fx_0c9", fx_0c9_headerPtr);

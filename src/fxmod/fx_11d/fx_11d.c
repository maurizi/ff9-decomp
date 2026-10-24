#include "fxmod/fx_11d.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    if (frame >= 4) {
        frame = 3;
    }
    return func_801E77A4(out, pos, &D_801E7D24[frame], frame, n);
}

s32 func_801E7754(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    if (frame >= 4) {
        frame = 3;
    }
    return func_801E77A4(out, pos, &D_801E7D3C[frame], frame, n);
}

s32 func_801E77A4(VECTOR *out, SVECTOR *pos, Fx11dPathKey *key, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D9C);
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;

    ex->func_800DC818(0x10, key->unk0, (u16 *)&a);
    ex->func_800DC818(0x10, key->unk2, (u16 *)&b);
    ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, key->unk4, (s16 *)&c);
    c.vz -= 0x32;
    pos->vx = c.vx;
    pos->vy = c.vy;
    pos->vz = c.vz;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_11d_op80(s32 mode, Fx11dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        D_801E7D9C = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk1C);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk1C);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = -0x80;
        }
    } else {
        frame = entry->unk0;
        if (frame >= 0 && frame < 0x18 && frame % 12 == 0) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&D_801E914C);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801E9154);
            if (work->unk4 != NULL) {
                ((Fx11dEffHdr *)work->unk4->unk0)->unk8 = func_801E7704;
            }
            if (work->unk8 != NULL) {
                ((Fx11dEffHdr *)work->unk8->unk0)->unk8 = func_801E7754;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x100);
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x100);
            }
        }
        if (frame >= 0 && frame < 0x18 && frame % 12 == 0) {
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E914C);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E9154);
            if (work->unkC != NULL) {
                work->unkC->unk14 = (s32)D_801E7DAC;
                work->unkC->unk12 = 4;
                work->unkC->unkC = 0x70;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk14 = (s32)D_801E7F6C;
                work->unk10->unk12 = 4;
                work->unk10->unkC = 0x70;
            }
            if (work->unkC != NULL) {
                ((Fx11dEffHdr *)work->unkC->unk0)->unk8 = func_801E7704;
            }
            if (work->unk10 != NULL) {
                ((Fx11dEffHdr *)work->unk10->unk0)->unk8 = func_801E7754;
            }
            /* sets unkC twice; unk10 keeps its default */
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0x10, 0x100);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0x10, 0x100);
            }
        }
        switch (frame) {
        case 0:
            ex->func_800DC818(0x10, 0x37, (u16 *)&work->unk24);
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk24);
            break;
        case 0x1E:
            ex->func_800DC818(0x10, 0, (u16 *)&work->unk2C);
            work->unk2C.vy -= 0xC8;
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk2C);
            if (work->unk18 != NULL) {
                work->unk18->unk14 = (s32)D_801E812C;
                work->unk18->unk12 = 8;
                work->unk18->unkC = 0x204;
            }
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11d/nonmatchings/fx_11d", fx_11d_headerPtr);

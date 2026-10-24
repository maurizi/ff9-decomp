#include "fxmod/fx_198.h"

s32 fx_198_op80(s32 mode, Fx198Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    SVECTOR r;
    SVECTOR p;
    Fx198Key *key;
    Fx198Key *k1;
    s32 frame;
    s32 c;
    s32 b;
    s32 i;
    s32 t;
    s32 z;
    s32 j;
    Fx198Key *key2;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E7E54 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_initEffModels(D_801E7E64, 5);
        key = (Fx198Key *)ex->btlfx_getFileData(entry->file, 0x15);
        work->unk1C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)key, 0x137, 0xFFFF, 0, 0x80);
        work->unk14 = 0;
        work->unk16 = 0;
        work->unk18 = 0;
        work->unk8 = 0x2000;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame < 0x1E) {
                key = (Fx198Key *)(ex->btlfx_getFileData(entry->file, 0x16) + 4);
                key += frame;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                pos.vz += (work->unk8 << 9) >> 12;
                rot.vx = key->unk2 + 0x400;
                rot.vy = key->unk0 + 0x800;
                rot.vz = key->unk4;
                scale.vx = scale.vy = scale.vz = work->unk8;
                if (frame < 0x18) {
                    b = 0x1000;
                } else {
                    b = ex->math_cos(((frame - 0x18) << 10) / 6);
                }
                ex->Hi_SetEffModelSlice(work->unk1C, 0);
                ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&scale, work->unk1C);
                ex->Hi_ModifyEffModelRGB(work->unk1C, (b >> 5) - 0x80, (b >> 5) - 0x80, (b >> 5) - 0x80);
                r.vx = 0;
                r.vy = 0;
                z = work->unk4;
                r.vz = z << 5;
                r.pad = 0;
                env->unk28 = 0x180;
                if (frame < 0x14) {
                    c = 0x1000;
                } else {
                    c = ex->math_cos(((frame - 0x14) << 10) / 10);
                }
                for (j = 0; j < 0x11; j++) {
                    k1 = (Fx198Key *)ex->btlfx_getFileData(entry->file, D_801E7DE8[j]);
                    k1 = (Fx198Key *)((u8 *)k1 + 4);
                    if (frame < 0x15) {
                        k1 += frame;
                    } else {
                        k1 += 0x15;
                    }
                    p.vx = work->unkC.vx + ((k1->unk0 * work->unk8) >> 12);
                    p.vy = work->unkC.vy + ((k1->unk2 * work->unk8) >> 12);
                    p.vz = work->unkC.vz + ((k1->unk4 * work->unk8) >> 12);
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 6), &p, NULL, 0x2000, work->unk4 + j * 4 + j, 1, c);
                    r.vz += 0x40;
                }
                if (frame < 0x14) {
                    c = ex->math_cos((frame << 10) / 20);
                    j = 0;
                    for (; j < 0x11; j++) {
                        k1 = (Fx198Key *)ex->btlfx_getFileData(entry->file, D_801E7DE8[j]);
                        k1 = (Fx198Key *)((u8 *)k1 + 4);
                        p.vx = work->unkC.vx + ((k1->unk0 * work->unk8) >> 12);
                        p.vy = work->unkC.vy + ((k1->unk2 * work->unk8) >> 12);
                        p.vz = work->unkC.vz + ((k1->unk4 * work->unk8) >> 12);
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 6), &p, NULL, 0x1800, work->unk4 + j * 4 + j, 1, c);
                        r.vz += 0x40;
                    }
                }
                env->unk28 = 0;
            }
            if (frame >= 0x14) {
                r.vx = 0;
                r.vy = 0;
                z = work->unk4;
                r.vz = z << 5;
                r.pad = 0;
                env->unk28 = 0x180;
                t = ((frame - 0x14) << 12) / 20;
                key2 = (Fx198Key *)(ex->btlfx_getFileData(entry->file, 0x18) + 4);
                key2 += frame;
                p.vy = key2->unk2;
                p.vz = key2->unk4;
                i = 0;
                for (; i < 0xA; i++) {
                    ex->btlfx_getFileData(entry->file, D_801E7DE8[i]);
                    p.vx = i * 0x100 - 0x500;
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 6), &p, NULL, 0x1800, work->unk4 + i * 4 + i, 1, ex->math_cos(t >> 2));
                    r.vz += 0x40;
                }
                env->unk28 = 0;
            }
            if (frame >= 0x28) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame > 0) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        key2 = (Fx198Key *)ex->btlfx_getFileData(entry->file, 0x17);
        ex->btlfx_drawBackdrop((BtlBgLayers *)key2, work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_198/nonmatchings/fx_198", fx_198_headerPtr);

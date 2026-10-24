#include "fxmod/fx_0da.h"

s32 fx_0da_op80(s32 mode, Fx0daWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 pos[3];
    s32 scale[3];
    SVECTOR rot;
    Pool6CEntry *p;
    s32 frame;
    s32 v;
    s32 t;
    s32 mm;
    s32 b;
    s32 m;
    s32 c;
    s32 model;
    s32 u;
    s32 k;
    s32 d;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E8220 = env;
        ex->btlfx_initEffModels(D_801E8230, 4);
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xBA, 0x3DC0, 0, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0xBA, 0x3DC0, 0, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0xBA, 0x3DC0, 0, 0);
        work->unk4 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0xB8, 0x3D40, 0x80,
                                                    0x80);
        work->unk0 = 1;
        work->unk24 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk1C);
        return 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 1:
            t = frame << 10;
            v = ((0x1000 - ex->math_cos(t >> 2)) * 0xC00) >> 12;
            scale[0] = scale[1] = scale[2] = v;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos[0] = work->unk14.vx;
            pos[1] = work->unk14.vy;
            pos[2] = work->unk14.vz;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0);
            ex->Hi_ModifyEffModelRGB(work->unk8, 0, 0, 0);
            if (frame >= 4) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = frame << 8;
            scale[0] = scale[1] = scale[2] = 0xC00;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            pos[0] = work->unk14.vx;
            pos[1] = work->unk14.vy;
            pos[2] = work->unk14.vz;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0);
            ex->Hi_ModifyEffModelRGB(work->unk8, 0, 0, 0);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->unk4);
            ex->Hi_ModifyEffModelAbr(work->unk4, 1);
            u = t >> 1;
            ex->Hi_ModifyEffModelRGB(work->unk4, (ex->math_sin(u) >> 5) - 0x80, (ex->math_sin(u) >> 5) - 0x80,
                                     (ex->math_sin(u) >> 5) - 0x80);
            ex->func_800E6624(((0x1000 - ex->math_cos(u)) >> 9) + frame * 5, 0x240, 0x180, 0x40, 0x80, 0x240, 0x100);
            if (frame >= 0x10) {
                work->unk0 = 3;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
            }
            break;
        case 3:
            scale[0] = scale[1] = scale[2] = 0xC00;
            rot.vx = 0;
            rot.vy = 0x200;
            rot.vz = 0;
            pos[0] = work->unk14.vx;
            pos[1] = work->unk14.vy;
            pos[2] = work->unk14.vz;
            if (frame < 2) {
                m = 0x2000 - (frame << 11);
            } else {
                m = 0x1000 - ((frame - 2) << 12) / 14;
            }
            if (m > 0x1000) {
                {
                    register s32 mc REG_ASM("$3") = work->unkC; /* the original passes it in v1 */
                    ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)pos, (s32)scale, work->unk8, mc, m - 0x1000);
                }
                c = -0x40;
                d = -0x40;
                model = work->unk8;
            } else {
                ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)pos, (s32)scale, work->unkC, work->unk10, m);
                c = m - 0x800;
                c >>= 5;
                c -= 0x80;
                d = c;
                model = work->unkC;
            }
            ex->Hi_ModifyEffModelRGB(model, c, d, c);
            if (frame == 1) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0xC) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk24++;
    }
    return 0;
}

s32 fx_0da_op84(s32 mode, Fx0daWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E8220 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->unk0;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
        ry = 0;
        switch (work->unk0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->unk28 = 0x80;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0da/nonmatchings/fx_0da", fx_0da_headerPtr);

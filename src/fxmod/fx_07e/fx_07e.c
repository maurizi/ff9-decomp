#include "fxmod/fx_07e.h"

INCLUDE_ASM("asm/us/fx_07e/nonmatchings/fx_07e", func_801E7704);

INCLUDE_ASM("asm/us/fx_07e/nonmatchings/fx_07e", func_801E7EB8);

s32 fx_07e_op80(s32 mode, Fx07eWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx07ePart *part;
    s32 frame;
    s32 t;
    s32 r;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx07eWork80);
        ((EffInit *)arg4)->unk8 = sizeof(Fx07ePart);
        ((EffInit *)arg4)->unk14 = (s32)func_801E7EB8;
        ((EffInit *)arg4)->unk4 = 0xB;
    } else if (mode == 1) {
        D_801E8AB8 = env;
        arg4 = (s32 *)*arg4;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk1C = 0x2EE;
        work->unk4 = 0;
        work->unk0 = 0;
        work->unk8 = 0;
        work->unk24 = 0;
        work->unk20 = 0x8000;
        if (arg4 != NULL) {
            work->unk14.vx = 0x200;
        } else {
            work->unk14.vx = -0x200;
        }
        do {
            work->unk14.vy = 0;
        } while (0);
        work->unk14.vz = 0;
        ex->btlfx_initEffModels(D_801E8AC8, 1);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0xBB, 0x3E40, 0,
                                                 0x80);
    } else {
        frame = entry->unk0;
        work->unk14.vz += 0x18;
        work->unk14.vy = work->unk8 * 0x14;
        work->unk14.vx -= 0xC;
        switch (work->unk0) {
        case 0:
            t = (work->unk4 << 12) / 11;
            part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (part != NULL) {
                part->unk8 = work->unk4;
                part->unk0 = t;
            }
            if (++work->unk4 >= 11) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 22;
            r = 0x2EE - ((t * 0x2CE) >> 12);
            work->unk1C = r;
            work->unk20 = ((((r << 12) / 750) << 15) >> 12);
            if (frame >= 22) {
                work->unk0 = 2;
                entry->unk0 = -1;
                work->unk24 = 1;
            }
            break;
        case 2:
            if (frame >= 17) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk8++;
    }
    return 0;
}

void func_801E8498(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8AB8);
    s32 x;
    s32 y;
    s32 t;

    t = ((0x3C - r) << 12) / 60;
    t = (t * D_801E8B90) >> 12;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT r1;
    RECT r2;
    SVECTOR *pos;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = sizeof(Fx07eWork81);
    } else if (mode == 1) {
        D_801E8AB8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk8 = ex->func_800E7CC4(0, 0x20);
        work->unk0 = 0;
    } else {
        frame = entry->unk0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        pos = &work->unkC;
        gte_ldv0(pos);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 12;
            env->unk28 = work->unk8;
            s = ex->math_sin(t >> 2);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, 0x2000, 0, 1, s, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, 0x2000, 0, 2, s);
            env->unk28 = 0;
            if (frame >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            r1.x = 0x1C0;
            r1.y = 0x180;
            r1.w = 0x7A;
            r1.h = 0x7A;
            t = frame << 8;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r1, func_801E8498);
            D_801E8B90 = ex->math_sin(t >> 2) >> 1;
            env->unk28 = work->unk8;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, 0x2000, 0, 1, 0x1000, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, 0x2000, 0, 2, 0x1000);
            env->unk28 = 0;
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            r2.x = 0x1C0;
            r2.y = 0x180;
            r2.w = 0x7A;
            r2.h = 0x7A;
            t = (frame << 12) / 12;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r2, func_801E8498);
            a = t >> 2;
            D_801E8B90 = ex->math_cos(a) >> 1;
            env->unk28 = work->unk8;
            c = 0x1000 - ex->math_sin(a);
            s = ex->math_cos(a);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, c * 2, 0, 1, s, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, c * 2, 0, 2, s);
            env->unk28 = 0;
            if (frame >= 12) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xA), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07e/nonmatchings/fx_07e", fx_07e_headerPtr);

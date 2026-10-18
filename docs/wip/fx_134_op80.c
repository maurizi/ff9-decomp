/* Draft of fx_134 fx_134_op80 (first draft from m2c; same family as fx_0e0 op80, whose draft gave the work layout).
 * Status: 2461 aligned mismatches, 853 ignoring registers (of ~3600 instructions); frame 0x150 vs 0x148.
 * Stack layout notes from the original: locals 0x38..0x107, spills 0x108 (frame), 0x10C (t), 0x110 (c, tail
 * alpha), 0x114 (s, tail scale), 0x118/0x11C (x/y offsets). Cases 0-2 and the tail reuse 0x38 (wrapped blocks),
 * case 3 uses 0x38..0x8F and case 4 0x90..0xC7 without being freed in between, cases 5, 6 and 10 reuse 0xC8. */
#include "fxmod/fx_134.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx134Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10; /* target */
    SVECTOR unk18; /* camera target */
    u8 pad20[4];
    Fx134Vec3 unk24; /* model position */
    Fx134Vec3 unk30; /* model scale */
    SVECTOR unk3C; /* model rotation */
    Fx134Vec3 unk44; /* scale */
    SVECTOR unk50; /* rotation */
    s32 unk58; /* scroll */
    s32 unk5C; /* models */
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    u8 pad78[4];
    Pool6CEntry *unk7C;
    Pool6CEntry *unk80;
} Fx134Work; /* 0x84 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* phase */
    SVECTOR unk4; /* velocity; pad = size index */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C;
    s16 unk1E;
    Pool6CEntry *unk20;
} Fx134Orb; /* 0x24 */

extern SVECTOR D_801EC860; /* caster position */
extern CVECTOR D_801EC878[];
extern s16 D_801EC880[4];
extern CVECTOR D_801EC888[];
extern CVECTOR D_801EC894[2];
extern Fx134Bolt D_801EC8F4[];
extern BtlModel D_801ECB64[];
extern u8 D_801ED1A4[];
extern s32 D_801EF97C;
extern s32 D_801EF980;

s32 func_801E8658(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent);

s32 fx_134_op80(s32 mode, Fx134Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx134Orb *o;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 c;
    s32 s;
    s32 xo;
    s32 yo;
    s32 i;
    s32 a;
    s32 b;
    s32 k;
    s32 n;

    if (mode == 0) {
        init->unk0 = 0x84;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E8658;
        init->unk4 = 0x31;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC8E4 = env;
        ex->btlfx_initEffModels(D_801ECB64, 8);
        work->unk5C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x98, 0x3D80, 0, 0);
        work->unk60 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x118, 0xFFFF, 0x40,
                                                 0);
        work->unk64 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), 0x118, 0xFFFF,
                                                 0x40, 0);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x37, 0x3D07, 0,
                                                 0);
        work->unk6C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0x37, 0x3D07, 0,
                                                 0);
        work->unk74 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0xBC, 0x3E00, 0x80,
                                                 0);
        work->unk8.vx = D_801EC860.vx;
        work->unk8.vy = D_801EC860.vy - 0x3000;
        work->unk8.vz = D_801EC860.vz;
        work->unk7C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801ED1A4;
            work->unk7C->unk12 = 0x40;
            work->unk7C->unkC = 0xCC;
        }
    } else {
        c = 0;
        s = 0;
        xo = 0;
        yo = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            switch (frame) {
            default: {
                SVECTOR v;

                t = (frame << 12) / 40;
                if (frame < 0x10) {
                    work->unk18.vx = D_801EC860.vx;
                    a = frame << 6;
                    work->unk18.vz = D_801EC860.vz;
                    work->unk18.vy -= ex->math_sin(a) * 3;
                    env->unk2C->unk0[3].vx = work->unk18.vx;
                    env->unk2C->unk0[3].vy = work->unk18.vy;
                    env->unk2C->unk0[3].vz = work->unk18.vz;
                    b = (ex->math_sin(a) >> 3) - 0x200;
                } else {
                    b = 0;
                }
                v.vx = ex->btlfx_randCentered(0x44C) + 0x400;
                v.vy = 0;
                v.vz = 0;
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
                k = 0;
                if (frame >= 8) {
                    if (frame < 0xC) {
                        k = (frame - 8) << 10;
                    } else {
                        k = 0x1000;
                    }
                }
                if (k != 0) {
                    a = (t >> 7) + 0xA0;
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), a, b, 0, 1, k);
                    k += ex->btlfx_randCentered(0x18);
                    if (k < 0) {
                        k = 0;
                    }
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), a, b, 0, 1, k);
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), a, b + 0x60, 0, 1, k);
                }
                if (frame == 0x18) {
                    work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
                }
                if (frame >= 8) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk8, NULL, 0x1800, frame, 2, -1);
                }
                if (frame >= 0x28) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                    ex->func_800E3D54(work->unk7C);
                    ex->func_800E3D54(work->unk80);
                    work->unk8.vx = D_801EC860.vx;
                    work->unk30.vx = 0x1800;
                    work->unk30.vy = 0x1800;
                    work->unk30.vz = 0x1800;
                    work->unk3C.vx = 0x3C0;
                    work->unk8.vy -= 0x3000;
                    work->unk3C.vy = 0x100;
                    work->unk8.vz = D_801EC860.vz;
                    work->unk3C.vz = 0x180;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vz = work->unk8.vz;
                    ex->func_800E63E4(1, 1);
                }
                break;
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR ang;

                t = frame << 6;
                work->unk18.vx = work->unk8.vx;
                a = 0x1000 - t;
                work->unk18.vz = work->unk8.vz;
                work->unk18.vy += 0x400 + (a >> 1);
                s = a >> 4;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk60);
                a = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk60, (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), work->unk4);
                a = t >> 6;
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), a + 0xA0, (t >> 9) + 0x78, 0, 1, -1);
                b = t >> 8;
                func_801E7748(0xFFD, (t >> 5) + 0x78, b + 0xA9, 0x100, 0x40, 0x80, 0x98, 0x3DC0, 0, 0xC0, 0xFF);
                ex->func_800D0B18((s32)(env->unk20 + 8), b + 0x78, 1, 2, 0x78, 1, D_801EC878, 0, 0, 0, 0);
                c = 0x93;
                yo = 0;
                xo = 0;
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), (frame << 6) / 40 + 0xBE, 0x73, 0,
                                  0xFF, -1);
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), a + 0x20, 0x40, 0, 0xFF, -1);
                n = (frame << 7) + 0x2000;
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    i = frame & 3;
                    k = (frame * 0x140 + (i << 5) - 0x20) % 3072 + 0x600;
                    a = D_801EC880[i] + 0x300;
                    o->unkC.vx = work->unk8.vx + ((ex->math_cos(k) * n) >> 12);
                    o->unkC.vz = work->unk8.vz + ((ex->math_sin(k) * n) >> 12);
                    o->unkC.vy = work->unk8.vy - 0x1400 + (((frame + 4) % 6) << 11);
                    ex->btlfx_aimAngles((s16 *)&o->unkC, (s16 *)&work->unk8, (s16 *)&ang);
                    ex->func_800CFCA8(&ang, a, (s32)&o->unk4);
                    o->unk4.pad = i;
                    o->unk0 = frame & 1;
                }
                D_801EF97C = b + 0x116;
                if (frame == 0x3E) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x46, 0x46, 0x46);
                }
                if (frame == 0x3F) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0x80);
                }
                if (frame >= 0x40) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                    D_801EF980 = 1;
                    ex->func_800E3C58();
                    work->unk3C.vx = 0x400;
                    work->unk3C.vy = 0;
                    work->unk3C.vz = 0;
                    ex->func_800E63E4(1, 1);
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                SVECTOR rot;
                Fx134Vec3 pos;
                Fx134Vec3 scale;

                t = (frame << 12) / 6;
                if (frame == 0) {
                    ex->btlfx_poolClear(entry->unk10);
                }
                c = 0x97;
                s = 0x10;
                yo = 0;
                xo = 0;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xC0, 0xC0, 0xC0);
                }
                if (frame == 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x60, 0x60, 0x60);
                }
                work->unk18.vx = work->unk8.vx;
                work->unk18.vz = work->unk8.vz;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vy += 4;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), work->unk4);
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x120, 0x58, 0, 1, -1);
                ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x78, 0xA0, 0, 1, -1);
                ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 0x78, 0x46, 0, 1, -1);
                a = (ex->math_sin(t >> 2) >> 3) + 0x1300;
                scale.vz = 0x1000;
                b = (((t >> 1) + 0x800) >> 5) - 0x80;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy + 0xC00;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame << 5;
                pos.vz = work->unk24.vz;
                for (i = 0; i < 4; i++) {
                    scale.vx = scale.vy = (a * ((ex->math_sin((i << 11) / 3) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, b, b, b);
                    rot.vz += 0x400;
                    pos.vy -= 0x800;
                }
                if (frame >= 6) {
                    work->unk0 = 3;
                    entry->unk0 = -1;
                    work->unk50.vx = 0x400;
                    work->unk44.vx = 0x3000;
                    work->unk44.vy = 0x3000;
                    work->unk50.vy = 0;
                    work->unk50.vz = 0;
                    work->unk44.vz = 0x1800;
                }
                break;
            }
            }
            break;
        case 3: {
            Fx134Vec3 pos;
            Fx134Vec3 scale;
            SVECTOR rot;
            Fx134Vec3 pos2;
            Fx134Vec3 scale2;
            SVECTOR rot2;

            a = (frame << 12) / 96;
            t = (frame << 12) / 24;
            k = ex->math_cos(a);
            work->unk18.vx = work->unk8.vx;
            work->unk18.vz = work->unk8.vz;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            c = 0x97;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            s = (k >> 4) + 0x20;
            work->unk3C.vy += 4;
            n = t >> 7;
            work->unk3C.vz += n;
            yo = 0;
            xo = 0;
            work->unk24.vy += (((0x1000 - ex->math_cos(a)) << 9) >> 12) + (ex->math_cos(a) >> 6) * ((frame & 1) * 2 - 1);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, -0x2B, -0x2B, -0x2B);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = t + 0x2000;
            work->unk44.vz = 0x1800;
            work->unk50.vz += ex->math_sin(a) >> 7;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a) >> 6) - 0x80, (ex->math_sin(a) >> 6) - 0x80,
                                     (ex->math_sin(a) >> 6) - 0x80);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                     (ex->math_sin(a) >> 5) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            n -= 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, n, n, n);
            scale.vx = scale.vy = ex->math_sin(a) * 2 + 0x400;
            scale.vz = 0x1000;
            pos.vx = work->unk24.vx;
            a = t >> 1;
            pos.vy = work->unk24.vy;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 7;
            pos.vz = work->unk24.vz;
            for (i = 0; i < 4; i++) {
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                         (ex->math_sin(a) >> 5) - 0x80);
                pos.vy += 0x40;
                rot.vz += 0x200;
                scale.vx = scale.vy = scale.vx * 2 / 3;
            }
            a = t >> 2;
            k = ex->math_sin(a);
            scale2.vz = (t >> 1) + 0x1000;
            pos2.vx = work->unk24.vx;
            pos2.vy = work->unk24.vy + 0xC00;
            rot2.vx = -0x400;
            rot2.vy = 0;
            rot2.vz = 0xC0 - (frame << 6);
            pos2.vz = work->unk24.vz;
            for (i = 0; i < 4; i++) {
                scale2.vx = scale2.vy = ((k * 3 / 2 + 0x1500) * ((ex->math_sin((i << 10) / 3) >> 1) + 0x800)) >> 12;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_cos(a) >> 5) - 0x80, (ex->math_cos(a) >> 5) - 0x80,
                                         (ex->math_cos(a) >> 5) - 0x80);
                rot2.vz += 0x400;
                pos2.vy -= 0x800;
            }
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x120, 0x58, 0, 1, -1);
            ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x78, 0xA0, 0, 1, -1);
            ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 0x78, 0x46, 0, 1, -1);
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy = work->unk8.vy;
                work->unk24.vy -= 0x800;
                work->unk30.vx = 0x36D;
                work->unk30.vy = 0x36D;
                work->unk30.vz = 0x36D;
                work->unk10.vx = work->unk24.vx;
                work->unk10.vz = work->unk24.vz;
                work->unk10.vy = work->unk24.vy;
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk10);
                work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk10);
            }
            break;
        }
        case 4: {
            SVECTOR rot;
            Fx134Vec3 scale;
            Fx134Vec3 pos;
            SVECTOR pos3;
            SVECTOR rot3;

            t = (frame << 12) / 40;
            if (frame > 0x10) {
                a = ((frame - 0x10) << 10) / 24;
                c = (ex->math_sin(a) >> 6) + 0x8D;
                s = a + 0x10;
            } else {
                c = 0x8D;
                s = 0x10;
            }
            work->unk18.vx = work->unk8.vx;
            work->unk18.vz = work->unk8.vz;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            yo = 0;
            work->unk3C.vz += 0x20;
            work->unk24.vy += 0x60 + ex->btlfx_randCentered(8);
            work->unk10.vz = work->unk24.vz;
            work->unk10.vy = work->unk24.vy;
            work->unk10.vx = work->unk24.vx;
            work->unk10.vy += 0x578;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, 0, 0, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x492;
            work->unk44.vz = 0xAAA;
            work->unk24.vy -= 0x578;
            work->unk50.vz += 0x18;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk44.vx = work->unk44.vy = 0x924;
            work->unk44.vz = 0x1000;
            work->unk50.vz -= 0x200;
            work->unk24.vy -= 0x60E;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            a = t >> 2;
            ex->Hi_ModifyEffModelRGB(work->unk6C, (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a) >> 1) + 0x800) >> 5) - 0x80);
            xo = 0;
            work->unk24.vy += 0xB86;
            if (frame > 0x1C) {
                b = frame - 0x1C;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy + 0x6A4;
                rot.vx = 0;
                rot.vy = frame << 6;
                rot.vz = 0;
                pos.vz = work->unk24.vz;
                k = (b << 12) / 48;
                scale.vx = scale.vz = ((ex->math_sin(k) * 7) >> 3) + 0x578;
                scale.vy = ex->math_sin(k) * 2 / 3 + 0x400;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                k = (b << 12) / 24;
                ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(k) >> 5) - 0x80, (ex->math_sin(k) >> 5) - 0x80,
                                         (ex->math_sin(k) >> 5) - 0x80);
            }
            env->unk28 = 0x100;
            k = ex->math_sin(a);
            pos3.vx = work->unk24.vx;
            pos3.vy = work->unk24.vy;
            rot3.pad = 0;
            rot3.vy = 0;
            rot3.vx = 0;
            rot3.vz = frame << 5;
            pos3.vz = work->unk24.vz;
            for (i = 0; i < 0x10; i++) {
                pos3.vy -= 0x19F;
                ex->btlfx_rand();
                func_801E7B38(&pos3, &rot3, 0x600, k, 0x1A, 0x3D02, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
            }
            env->unk28 = 0;
            n = frame % 5;
            if (n == 2) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = work->unk24.vx;
                    o->unkC.vz = work->unk24.vz;
                    o->unk14.vx = -0x400;
                    o->unk14.vy = 0;
                    o->unk0 = n;
                    o->unk14.vz = frame * 0x154;
                    o->unkC.vy = work->unk24.vy + 0x400;
                    o->unk4.pad = t;
                }
            }
            a = t >> 6;
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xA0 - a, 0x58, 0, 1, -1);
            ex->func_800D0B18((s32)(env->unk20 + 0x3E8), 0x28, 1, 3, 0x64, 1, D_801EC888, 0, 0, 0, 0);
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0xDC - t / 55, 0x78, 0, 1, -1);
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0x28 - a, 0x28, 0, 1, -1);
            work->unk58 += 4 + ((t * 10) >> 12);
            func_801E7940(0, work->unk58 % 320, 0xA0 - (t >> 8), 0x80, 0x80, 0x80, 0x9C, 0x3E00, 0x80, 0, 1, 0);
            func_801E7940(0xFFF, (work->unk58 / 320) * 0xA0 - (work->unk58 >> 1), (t >> 9) + 0x8C, 0x80, 0x80, 0x80, 0x9C,
                          0x3E00, 0x80, 0, 0, 1);
            if (frame >= 0x28) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->btlfx_poolClear(entry->unk10);
                ex->func_800E3C58();
                work->unk24.vy = work->unk8.vy;
                work->unk30.vx = 0x400;
                work->unk30.vy = 0x400;
                work->unk30.vz = 0x400;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy -= 0x280;
                ex->func_800E63E4(0, 1);
            }
            break;
        }
        case 5:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                SVECTOR rot2;
                Fx134Bolt *bolt;

                t = (frame << 12) / 23;
                c = (frame << 12) / 2944 + 0x8D;
                s = (t >> 4) + 0x20;
                ex->func_800E6624(frame * 0x14, 0x1C0, 0x180, 0x18, 0x60, 0x320, 0x100);
                work->unk18.vx = work->unk8.vx;
                work->unk18.vz = work->unk8.vz;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vz += 0x18;
                work->unk24.vy += 0x20;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                k = (t >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk64, k, k, k);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x800;
                work->unk44.vz = 0xAAA;
                work->unk24.vy -= 0x578;
                work->unk50.vz += 0x18;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k, k, k);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k, k, k);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k, k, k);
                work->unk44.vx = work->unk44.vy = 0x6DB;
                work->unk44.vz = 0x1000;
                work->unk50.vz += 0x2AA;
                work->unk24.vy -= 0x578;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                a = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                         (ex->math_sin(a) >> 5) - 0x80);
                work->unk24.vy += 0xAF0;
                yo = 0;
                xo = 0;
                scale.vz = 0x555;
                scale.vx = scale.vy = ex->math_sin(a) * 2 / 3 + 0x100;
                pos.vx = work->unk24.vx;
                a = t >> 1;
                pos.vy = work->unk24.vy + 0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame << 7;
                pos.vz = work->unk24.vz;
                for (i = 0; i < 3; i++) {
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                             (ex->math_sin(a) >> 5) - 0x80);
                    scale.vx = scale.vy = (scale.vx * 3) >> 2;
                    rot.vz += 0x300;
                    pos.vy += 0x20;
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy + 0x400;
                from.vz = work->unk24.vz;
                k = ex->btlfx_rand();
                for (i = 0, bolt = D_801EC8F4; i < 3; i++, bolt++) {
                    n = ex->btlfx_randCentered(0x400);
                    to.vx = work->unk8.vx + (ex->math_cos(k) >> 1);
                    to.vz = work->unk8.vz + (ex->math_sin(k) >> 1);
                    func_801E8460(entry, bolt, &from, &to, 0x100, 0x200, k, 0xC0, 0, n + 0xC00, (ex->btlfx_rand() & 0xF) + 0xF);
                    k += 0x554 + (ex->btlfx_rand() & 0xFF);
                }
                rot2.vx = -0x400;
                rot2.vy = 0;
                a = t >> 2;
                rot2.vz = ex->math_sin(a) >> 3;
                func_801E7E48(&work->unk8, &rot2, (ex->math_sin(a) >> 3) + 0x1000, 0x40, 0x17, 0x3D00, 0, 0x80, a, 1);
                if (frame >= 0x17) {
                    work->unk0 = 6;
                    entry->unk0 = -1;
                    D_801EC860.vz += 0x400;
                    work->unk8.vx = D_801EC860.vx;
                    work->unk8.vz = D_801EC860.vz;
                    D_801EC860.vz -= 0x400;
                    work->unk30.vx = 0x2800;
                    work->unk30.vy = 0x2800;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vz = work->unk8.vz;
                    work->unk24.vz -= 0x1C00;
                    work->unk24.vy -= 0x3800;
                    work->unk30.vz = 0x1800;
                    work->unk10.vx = work->unk24.vx;
                    work->unk10.vy = work->unk24.vy;
                    work->unk10.vz = work->unk24.vz;
                    ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk3C);
                    work->unk50.vx = work->unk3C.vx;
                    work->unk50.vz = work->unk3C.vz;
                    ex->func_800E63E4(1, 1);
                    work->unk3C.vz += 0x320;
                }
                break;
            }
            }
            break;
        case 6:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                Fx134Bolt *bolt;

                t = (frame << 12) / 56;
                c = ((t << 5) >> 12) + 0x8D;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, (t * 0x58) >> 12, 0, 0);
                s = 0x80;
                work->unk18.vx = work->unk8.vx;
                work->unk18.vz = work->unk8.vz;
                a = t >> 2;
                yo = 0;
                work->unk18.vy = work->unk18.vy - 0x400 - (ex->math_sin(a) >> 1);
                xo = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vz += 8;
                work->unk24.vz += 0x4D;
                work->unk24.vy += 0x9B;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x3800;
                work->unk44.vz = 0x2000;
                work->unk24.vy -= 0x578;
                work->unk50.vz += 0x14;
                work->unk24.vz += 0x2BC;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80);
                work->unk50.vz -= 0x200;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a) >> 1) + 0x800) >> 5) - 0x80);
                work->unk44.vz = 0x3000;
                work->unk44.vx = work->unk44.vy = 0x2000;
                work->unk50.vz += 0x200;
                work->unk24.vz -= 0x2BC;
                work->unk24.vz -= 0x9C4;
                work->unk24.vy += 0x578;
                work->unk24.vy -= 0x1388;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                ex->Hi_ModifyEffModelRGB(work->unk6C, -0x2B, -0x2B, -0x2B);
                work->unk24.vy += 0x1388;
                work->unk24.vz += 0x9C4;
                if (frame > 0x28) {
                    b = (frame - 0x28) << 8;
                    k = ex->math_sin(b >> 2) * 5 / 2 + 0x400;
                    scale.vz = b + 0x1800;
                    scale.vx = scale.vy = k;
                    rot.vx = -0x400;
                    rot.vy = 0;
                    rot.vz = frame * 0x8D;
                    pos.vx = D_801EC860.vx;
                    pos.vy = D_801EC860.vy - 0x100;
                    pos.vz = D_801EC860.vz;
                    for (i = 0; i < 1; i++) {
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                        ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(b >> 2) >> 5) - 0x80,
                                                 (ex->math_sin(b >> 2) >> 5) - 0x80, (ex->math_sin(b >> 2) >> 5) - 0x80);
                        scale.vx = scale.vy = (scale.vx * 3) >> 2;
                        rot.vz += 0x400;
                        pos.vy += 0x20;
                    }
                    scale.vx = scale.vz = ex->math_sin(b >> 2) * 5 / 2 + 0x1000;
                    scale.vy = (((frame - 0x28) * 0x300 << 8) >> 12) + 0x800;
                    rot.vx = 0;
                    rot.vz = 0;
                    rot.vy = frame * 0x8D;
                    pos.vx = D_801EC860.vx;
                    pos.vy = D_801EC860.vy;
                    pos.vz = D_801EC860.vz;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                    ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(b >> 1) >> 5) - 0x80,
                                             (ex->math_sin(b >> 1) >> 5) - 0x80, (ex->math_sin(b >> 1) >> 5) - 0x80);
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy + 0x1838;
                from.vz = work->unk24.vz + 0xC1C;
                for (i = 0, bolt = D_801EC8F4; i < 2; i++, bolt++) {
                    k = ex->btlfx_rand();
                    n = ex->btlfx_randCentered(0x400);
                    ((s32 *)&to)[0] = work->unk24.vx;
                    ((s32 *)&to)[1] = work->unk24.vy;
                    ex->func_800D044C(&from, (s32)&to, 0x1000, (XY32 *)&work->unk3C, 0x3000, k);
                    func_801E8460(entry, bolt, &from, &to, 0x100, 0x200, k, 0xC0, 0, n + 0xC00, (ex->btlfx_rand() & 0x1F) + 0x58);
                }
                if (frame >= 0x38) {
                    work->unk0 = 7;
                    entry->unk0 = -1;
                    work->unk8.vx = work->unk10.vx = work->unk24.vx;
                    work->unk8.vy = work->unk10.vy = work->unk24.vy;
                    work->unk8.vz = work->unk10.vz = work->unk24.vz;
                }
                break;
            }
            }
            break;
        case 7:
            c = 0xAD;
            yo = 0;
            xo = 0;
            t = frame << 8;
            a = t >> 2;
            s = a + 0x80;
            D_801EC894[1].r = (ex->math_sin(a) * 0xFF) >> 12;
            D_801EC894[1].g = (ex->math_sin(a) * 0xFF) >> 12;
            D_801EC894[1].b = ex->math_sin(a) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x58, 0, 0);
            ex->func_800D0B18((s32)(env->unk20 + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, D_801EC894, 0, 0, 0, 0);
            if (frame > 8) {
                k = (frame - 8) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, k, k, k);
            }
            a = t >> 2;
            work->unk24.vy = work->unk8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(a)) >> 6);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, ((((ex->math_cos(a) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(a) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(a) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x578;
            work->unk50.vz += 8;
            work->unk24.vz += 0x2BC;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(a) >> 6) - 0x80, (ex->math_cos(a) >> 6) - 0x80,
                                     (ex->math_cos(a) >> 6) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(a) >> 6) - 0x80, (ex->math_cos(a) >> 6) - 0x80,
                                     (ex->math_cos(a) >> 6) - 0x80);
            work->unk44.vz = 0x3000;
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vz -= 0x2BC;
            work->unk24.vz -= 0x9C4;
            work->unk24.vy += 0x578;
            work->unk24.vy -= 0x1388;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(a) >> 5) - 0x80, (ex->math_cos(a) >> 5) - 0x80,
                                     (ex->math_cos(a) >> 5) - 0x80);
            work->unk24.vy += 0x1388;
            work->unk24.vz += 0x9C4;
            env->unk28 = 0x898;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &D_801EC860, NULL, t * 3 + 0x1000, frame, 1,
                              ex->math_sin(a));
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &D_801EC860, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(a));
            env->unk28 = 0;
            if (frame >= 0x10) {
                work->unk0 = 8;
                entry->unk0 = -1;
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vx -= 0x2800;
                work->unk10.vy += 0xB86;
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk10);
            }
            break;
        case 8:
            c = 0xBC;
            yo = 0;
            xo = 0;
            t = frame << 7;
            s = 0x200;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                k = ((frame - 0x1A) << 7) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k, k, k);
            }
            if (frame >= 0xA) {
                for (i = 0; i < 3; i++) {
                    o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (o != NULL) {
                        k = ex->btlfx_randCentered(0x200);
                        o->unkC.vy = 0x6A4 - ((0x1000 - t) >> 1);
                        o->unkC.vx = work->unk18.vx - ((t >> 4) + 0x1800) + k;
                        o->unkC.vz = work->unk18.vz + ex->btlfx_randCentered(0x1C00);
                        o->unk4.vx = ex->btlfx_randCentered(0x100);
                        o->unk4.vy = -0x140 - (ex->btlfx_rand() & 0xFF);
                        o->unk4.vz = ex->btlfx_randCentered(0x100);
                        o->unk14.vx = ex->btlfx_rand();
                        o->unk14.vy = ex->btlfx_rand();
                        o->unk14.vz = ex->btlfx_rand();
                        o->unk0 = 3;
                        o->unk4.pad = t / 5 + (ex->btlfx_rand() & 0x3FF) + 0x80;
                    }
                }
            }
            if (frame == 1) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk10);
            }
            if (frame == 4) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                work->unk0 = 0xA;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->unk10);
                ex->func_800E3C58();
                work->unk18.vx = 0;
                work->unk18.vy = -0x200;
                work->unk18.vz = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
            }
            break;
        case 10:
            switch (frame) {
            default: {
                CVECTOR col;
                SVECTOR rot;
                Fx134Vec3 scale;

                t = frame << 7;
                k = 0xF5 - ((frame * 0x7A80) >> 12);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k, k, k);
                rot.vx = 0;
                rot.vy = 0;
                k = 0xA0 - ((frame << 14) >> 12);
                rot.vz = 0x20;
                scale.vx = 0x1100;
                scale.vy = 0x1100;
                scale.vz = 0x1000;
                col.r = col.g = col.b = k;
                ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 1, NULL);
                if (frame >= 0x20) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        if (c != 0) {
            CVECTOR col;
            SVECTOR rot;
            Fx134Vec3 scale;

            col.r = col.g = col.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vx = scale.vy = s + 0x1000;
            scale.vz = 0x1000;
            ex->func_800E6764(1, xo + 0xA0, yo + 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 0, NULL);
        }
        if ((u32)(work->unk0 - 3) < 5) {
            ex->func_800E6624((0x1F4 - work->unk4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->unk4++;
    }
    return 0;
}

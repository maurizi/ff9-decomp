#include "fxmod/fx_1cc.h"

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7720);

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7CBC);

void func_801E8284(s16 *from, s16 *to, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9C34)->func_800CFD78(from, to, -(D_801E9BD0[i] * 4), out);
}

s32 func_801E82D0(s32 mode, Fx1ccObj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5,
                  Fx1ccInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 k;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk8 = 0;
    } else if (o->unk2 == 0) {
        SVECTOR a;
        VECTOR sc;
        SVECTOR p;
        SVECTOR q;
        SVECTOR b;
        SVECTOR c;
        SVECTOR d;
        SVECTOR *tab;
        Fx1ccObj *ch;
        Pool6CEntry *e;
        u8 *f;
        s32 n;
        s32 i;
        s32 r;
        s32 m;
        s32 w;
        s32 x;
        s32 u;

        tab = &D_801E9B68[o->unk4];
        switch (o->unk0) {
        case 0:
            if (o->unk8 == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&o->unk18);
                o->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            sc.vx = 0;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (++o->unk8 > 0) {
                o->unk0 = 1;
                o->unk8 = 0;
            }
            break;
        case 1:
            w = o->unk8;
            sc.vx = sc.vy = ((w << 10) * 0x1000) >> 12;
            sc.vz = (ex->math_sin((w << 10) >> 2) * 0x1000) >> 12;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (++o->unk8 >= 4) {
                o->unk0 = 2;
                o->unk8 = 0;
            }
            break;
        case 2:
            sc.vx = sc.vy = sc.vz = 0x1000;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (frame < 12) {
                u = (frame << 10) / 12;
                a.pad = 1;
                p.vx = o->unkC;
                p.vy = o->unk10;
                p.vz = o->unk14;
                ex->func_800CFD78((s16 *)&p, (s16 *)&a, 0x578, (s16 *)&q);
                f = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x26);
                k = (ex->math_sin(u) * 0x2000) >> 12;
                ex->func_800D23D8(f, &q, &a, k, 0, 1, ex->math_cos(u) * 2 / 3);
            }
            if (inst->unk78 != 0) {
                n = 6;
                o->unk8 = 0;
                if (o->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&c, &o->unk18);
                for (i = 0, m = 0; i < 4; m += 0x29, i++, n++) {
                    ch = o;
                    if (i != 0) {
                        ch = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (ch != NULL) {
                        ch->unk2 = 1;
                        ch->unk0 = n;
                        ch->unk4 = i;
                        ch->unk8 = 0;
                        ch->unk2C = inst->unk20[n];
                        func_801E8284((s16 *)&c, (s16 *)tab, n, (s16 *)&b);
                        BTLFX_COPY_SVEC(&ch->unk20, tab);
                        ch->unkC = b.vx << 4;
                        ch->unk10 = b.vy << 4;
                        ch->unk14 = b.vz << 4;
                        ch->unk28 = ex->btlfx_randCentered(0x84);
                        ch->unk2A = ex->btlfx_randCentered(0x84);
                        x = (ex->btlfx_rand() & 0xF) + 0x80;
                        r = (x - m) << 4;
                        BTLFX_COPY_SVEC(&d, tab);
                        d.vx += 0x80;
                        ex->func_800CFCA8(&d, -r, (s32)&ch->unk18);
                        ch->unk18 += ex->btlfx_randCentered(0x200);
                        ch->unk1A += ex->btlfx_randCentered(0x200);
                        ch->unk1C += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (sc.vx != 0) {
            sc.vx *= 2;
            sc.vy *= 2;
            sc.vz = sc.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(o->unk2C, 2);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->unkC, (s32)&sc, o->unk2C);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->unkC, (s32)&sc, o->unk30);
        }
    } else {
        VECTOR pos;
        VECTOR sc;
        s32 c;

        t = ((o->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        o->unkC += o->unk18;
        o->unk10 += o->unk1A;
        o->unk14 += o->unk1C;
        o->unk1A += 0x40;
        o->unk20.vx += o->unk28;
        o->unk20.vy += o->unk2A;
        c = (c * 3) >> 1;
        if (o->unk0 < 6) {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x3000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2000) >> 12;
        } else {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x2000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2800) >> 12;
        }
        pos.vx = o->unkC >> 4;
        pos.vy = o->unk10 >> 4;
        pos.vz = o->unk14 >> 4;
        ex->Hi_DrawEffModel(&o->unk20, &pos, (s32)&sc, o->unk2C);
        ex->Hi_ModifyEffModelAbr(o->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(o->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++o->unk8 >= 0x16) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_headerPtr);

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_op80);

#include "fxmod/fx_17d_0.h"

void func_801E7704(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b) {
    POLY_G4 *p;
    BtlfxExports *ex;
    s32 i;
    s32 x;

    h >>= 1;
    p = (POLY_G4 *)D_801E9470->unk24;
    ex = BTLFX_EXPORTS(D_801E9470);
    D_801E9470->unk24 = (u8 *)(p + 4);
    for (i = 0; i < 4; i++, p++) {
        setlen(p, 8);
        setcode(p, 0x3A);
        if (!(i & 1)) {
            p->r0 = p->r1 = 0;
            p->g0 = p->g1 = 0;
            p->b0 = p->b1 = b;
            p->r2 = p->r3 = r;
            p->g2 = p->g3 = g;
            p->b2 = p->b3 = b;
            p->y0 = p->y1 = 0x70 - h;
            p->y2 = p->y3 = 0x70;
        } else {
            p->r2 = p->r3 = 0;
            p->g2 = p->g3 = 0;
            p->b2 = p->b3 = b;
            p->r0 = p->r1 = r;
            p->g0 = p->g1 = g;
            p->b0 = p->b1 = b;
            p->y0 = p->y1 = 0x70;
            p->y2 = p->y3 = 0x70 + h;
        }
        x = (i >> 1) * 160;
        p->x0 = p->x2 = x;
        p->x1 = p->x3 = x + 160;
        ex->btlfx_linkPrimAbr(0x08000000, ot, p, abr);
    }
}

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E7894);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E79E8);

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", func_801E7D18);

s32 func_801E7FE0(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk0 = 0;
    } else {
        p->unk8.vz += p->unk2 + 0x186;
        env->unk28 = -0x80;
        if (frame == 0) {
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&p->unk8, 0, -1, 0x800, 0x1000, 1,
                              D_801EBC90[p->unk4], 0);
        }
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&p->unk8, 0, 0, 0x800, 0x1000, 1,
                          D_801EBC90[p->unk4], 0);
        env->unk28 = 0;
        if (frame >= 0x10) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_op80);

s32 fx_17d_0_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s16 *f;
    s32 n;
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E9470 = env;
    } else {
        frame = entry->unk0;
        f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1A);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        v.vx = f[0];
        v.vy = f[1];
        v.vz = f[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x25);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        env->unk2C->unk30[0].vz = f[2];
        f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x19);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        v.vx = f[0];
        v.vy = f[1];
        v.vz = f[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0xA9) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);

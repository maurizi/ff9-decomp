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

INCLUDE_ASM("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_op82);

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);

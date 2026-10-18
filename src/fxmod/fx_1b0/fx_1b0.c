#include "fxmod/fx_1b0.h"

void func_801E7704(Fx1b0Split *s, SVECTOR (*faces)[3]) {
    Fx1b0Piece *p;
    s32 i;

    D_801EBF3C = s;
    D_801EBF40 = s->unk4C;
    p = s->unk4C;
    s->unk0 = 0;
    s->unk2 = 0;
    s->unk28 = faces;
    s->unk24 = faces;
    for (i = 0; i < 0x7BC; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EBF34);
    Fx1b0Split *s = D_801EBF3C;
    SVECTOR (*f)[3] = s->unk28;
    s32 n;
    s32 i;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)f);
    k = s->unk0;
    s->unk2C[k] = f;
    s->unk4[k] = n;
    s->unk28 = f + n;
    s->unk2 += n;
    s->unk0++;
    for (i = 0; i < n; i++) {
        (*f)[0].pad = 0;
        f++;
    }
    return s->unk2;
}

Fx1b0Piece *func_801E7830(void) {
    Fx1b0Piece *p = D_801EBF40;
    s32 i;

    for (i = 0; i < 0x7BC; i++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E7868(SVECTOR *center) {
    Fx1b0Split *s = D_801EBF3C;
    SVECTOR (*f)[3] = s->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EBF34);
    SVECTOR d;
    Fx1b0Piece *p;
    s32 j;
    s32 i;
    s32 n;

    j = 0;
    i = 0;
    n = 0;
    for (; i < s->unk2; i++, f++, j++) {
        p = func_801E7830();
        if (p != NULL) {
            (*f)[0].pad = 1;
            p->unk0 = (((*f)[0].vx + (*f)[1].vx + (*f)[2].vx) / 3) << 12;
            p->unk4 = (((*f)[0].vy + (*f)[1].vy + (*f)[2].vy) / 3) << 12;
            p->unk8 = (((*f)[0].vz + (*f)[1].vz + (*f)[2].vz) / 3) << 12;
            p->unk18 = 0;
            p->unk1A = j;
            p->unkC.pad = 0x1000;
            p->unk14 = ex->btlfx_randCentered(0x40);
            p->unk16 = ex->btlfx_randCentered(0x80);
            d.vx = (p->unk0 >> 12) - center->vx;
            d.vy = (p->unk4 >> 12) - center->vy;
            d.vz = (p->unk8 >> 12) - center->vz;
            p->unkC.vx = d.vx / 10 + ex->btlfx_randCentered(0x10);
            p->unkC.vy = d.vy / 10 + ex->btlfx_randCentered(0x10);
            p->unkC.vz = d.vz / 10 + ex->btlfx_randCentered(0x10);
            n++;
        }
    }
    return n;
}

INCLUDE_ASM("asm/us/fx_1b0/nonmatchings/fx_1b0", func_801E7B04);

INCLUDE_ASM("asm/us/fx_1b0/nonmatchings/fx_1b0", func_801E7E48);

INCLUDE_ASM("asm/us/fx_1b0/nonmatchings/fx_1b0", fx_1b0_op80);

s32 fx_1b0_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EBF34 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b0/nonmatchings/fx_1b0", fx_1b0_headerPtr);

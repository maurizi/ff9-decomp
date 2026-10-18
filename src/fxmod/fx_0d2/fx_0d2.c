#include "fxmod/fx_0d2.h"

void func_801E77A4(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801E9E88(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op80);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op81);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op82);

INCLUDE_RODATA("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_headerPtr);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op83);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op84);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op85);

s32 func_801E9DBC(u8 *base, s32 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801E9DD4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801ED8E0)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E9E18(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9E70);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9E88);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9EEC);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9F04);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9F98);

s32 func_801E9FB0(Fx0d2Keys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801E9FF4);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801EA00C);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", func_801EA070);

s32 func_801EA108(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ED8E0);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801E9DD4(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801EA1D4(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801F0060[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801F0460[i] = 0;
    }
}

void func_801EA220(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk6 == -1) {
            D_801F0060[i].unk6 = 0;
            D_801F0060[i].unk0 = data;
            D_801F0060[i].unk4 = start;
            D_801F0060[i].unkA = dst;
            D_801F0060[i].unkC = 0;
            D_801F0060[i].unkE = c;
            D_801F0060[i].unk8 = ch;
            return;
        }
    }
}

void func_801EA27C(s32 ch) {
    Unk800F78A4 *env = D_801ED8E0;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk8 == ch) {
            if (D_801F0060[i].unk6 == 0 && D_801F0060[i].unk4 == D_801F0460[ch]) {
                D_801F0060[i].unk6 = *D_801F0060[i].unk0;
                D_801F0060[i].unk0++;
            }
            if (D_801F0060[i].unk6 > 0) {
                D_801F0060[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801F0060[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801F0060[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801F0060[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801F0060[i].unk0[2];
                        if (D_801F0060[i].unkC < D_801F0060[i].unkE) {
                            D_801F0060[i].unk0 += 3;
                            D_801F0060[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801F0460[ch]++;
}

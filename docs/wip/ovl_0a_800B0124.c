/*
 * ovl_0a func_800B0124 (0x18FC bytes): field music/sound request handler (GameData.unk83C),
 * the field version of ovl_0c func_800AB0C0 (matched) with the two sound-effect bank slots
 * (unk828/unk830/unk8B4/unk8B8/unk8BA), the 0x18xxxx ambient sounds and the 9 (resume) case.
 * WIP: 1599 of 1599 instructions, 12 aligned diff rows (build/s/cmp.py), all in the kind-9
 * block of case 0: the first D_8006794C load is in v0 (original a1), so its %hi lands in a1
 * (a2) and the reload/0x70000 temps shift with it. Pass 9: `d = g->unk850; g->unk826[0] = d;`
 * (permuter) fixed `ok` (t0) and took it from 17 to 12 rows; moving any single line of the
 * kind-9 block anywhere inside it (build/mvall.py) and 60 more permuter minutes found nothing.
 * Everything else matches,
 * including the jump tables (one `.word 0` pad before the second table).
 *  - the `addu v1,a0,zero` copy of D_8006794C->unk1C into `g` only survives CSE when `g` is also
 *    used after the block (cse.c make_regs_eqv makes it the canonical register only if it lives
 *    beyond the extended basic block), hence `g` in state 2.
 * How the rest was found (dump with `cc1 -dl -dg`, then priority = floor_log2(refs) * refs /
 * live_length from the .lreg "Register N used X times across Y insns" lines):
 *  - arg0 must lose its register (the original reloads it from its home slot 0x58 everywhere);
 *    that happens once case 1 of the kind switch has a dead `if ((arg0 & 0x3F) == 1)` around
 *    two identical bodies (the original keeps a dead `lw t2,0x58(sp)` there);
 *  - id (s7) vs the case-1 persistent flag (fp): id is at 36/199 vs the flag's 45/247; moving
 *    `t = -1` above the case-0 id and `h = 0` above the case-1 id shortens id to 197 insns and
 *    puts it above the flag but still below arg1 (42/217);
 *  - case 2's subcommand switch is a decision tree in the original: it only works as
 *    `case 1 ... 3:` around an inner switch {1,2,3,4} (so subcommand 4 is unreachable there);
 *  - GameData.unk828/unk82C must be an array (`void *unk828[2]`) for the `base + slot*4`
 *    operand order; GameDataSfx below stands in until ff9.h has it;
 *  - the case-2 sound handle reuses `slot`, the case-1 kind-9 flag reuses `t`, the division
 *    temps in states 2/3/4 need the `x = ...; x = x / 2048 * 2048;` two-statement form.
 * The declarations above the function belong in ovl_0a.h when this is integrated.
 */
#include "ovl_0a.h"

extern s8 D_800C9D4C;
extern s32 D_800C9D50;
extern u8 D_800C9D52;
extern s32 D_800C9D54;
s32 func_800B29F8(Unk800B29F8 *arg0);
s32 func_80055290(void *);
void func_800552B8(s32);
void func_80055310(void);
s32 func_800553BC(void *, s32);
void func_800553EC(s32, s32, s32, s32);
void func_8005546C(s32, s32);
void func_80055604(s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800558DC(s32);
void func_80055908(s32, s32);
void func_8005593C(s32, s32, s32);
void func_80055980(s32, s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);
void func_80055B6C(s32);
void func_80055B98(s32, s32);
void func_80055BCC(s32, s32, s32);
void func_80055C08(s32);
void func_80055C34(s32, s32);
void func_80055C68(s32, s32, s32);
void func_80055CA4(s32);
void func_80055CD0(s32, s32);
void func_80055D04(s32, s32, s32);
s32 func_80055DCC(void);
void func_800561C4(s32, s32, s32);
void func_80056228(void);
void func_800562B0(s32);

typedef struct {
    u8 pad[0x828];
    s32 unk828[2];
} GameDataSfx;
#define SFXH(i) (((GameDataSfx *)D_8006794C->unk1C)->unk828[i])
#define SCENE (D_8006794C->unk1C->unk8E4.field->unkC->unk14)

s32 func_800B0124(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 id;
    s32 t;
    s32 h;
    s32 ok;
    s32 slot;
    s32 fp;
    s32 mask;
    s32 mask2;
    s32 sz;
    s32 n;
    s32 m;
    s32 d;

    kind = (arg0 >> 8) & 0xF;
    if (kind == 12 && D_8006794C->unk1C->unk851 != 0) {
        goto state;
    }
    switch ((arg0 >> 12) & 3) {
    case 0:
        t = -1;
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 9:
            ok = 0;
            if (D_8006794C->unk1C->unk7E8 != 0xFFFF && D_8006794C->unk1C->unk7EA == (s16)D_8006794C->unk1C->unkC) {
                g = D_8006794C->unk1C;
                ok = 1;
                D_800C9D4C = kind;
                d = g->unk850;
                g->unk826[0] = d;
                arg1 = g->unk7E8;
                t = g->unk840;
                id = arg1 | 0x70000;
                D_8006794C->unk1C->unk8A8 = D_8006794C->unk1C->unk8AC;
                t |= 0x90000;
                D_8006794C->unk1C->unk84A = D_8006794C->unk1C->unk8B0;
            }
            D_8006794C->unk1C->unk7E8 = 0xFFFF;
            D_8006794C->unk1C->unk840 = 0xFFFF;
            D_8006794C->unk1C->unk7EA = 0xFFFF;
            D_8006794C->unk1C->unk8AC = -1;
            D_8006794C->unk1C->unk8B0 = 0xFFFF;
            if (!ok) {
                return 0;
            }
        case 0:
        case 7:
            if (D_8006794C->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800C9D4C == -1) {
                D_800C9D4C = kind;
            }
            if (D_800C9D4C != 9) {
                t = func_800B2328(id);
            }
            if (D_8006794C->unk1C->unk81C[0] != arg1 || D_8006794C->unk1C->unk820[0] != (u16)t) {
                if (D_8006794C->unk1C->unk0 & 0x20000000) {
                    D_8006794C->unk1C->unk0 |= 0x200000;
                }
                D_8006794C->unk1C->unk0 &= 0xCFFFFFFF;
                SCENE->unk0 &= ~0x10;
                D_8006794C->unk1C->unk820[0] = t;
                D_800C9D50 = id;
                D_8006794C->unk1C->unk81C[0] = arg1;
                D_8006794C->unk1C->unk851 = 1;
                return D_8006794C->unk1C->unk851;
            }
            if (D_800C9D4C == 7) {
                goto done;
            }
        start:
            D_8006794C->unk1C->unk824[0] = func_80055290(D_8006794C->unk1C->unk814);
            if (!(D_8006794C->unk1C->unk0 & 0x20000000)) {
                func_80055A34(D_8006794C->unk1C->unk824[0], 0xA, 0, D_8006794C->unk1C->unk826[0]);
            }
            D_8006794C->unk1C->unk0 |= 0x30000000;
        done:
            D_800C9D4C = -1;
            return 0;
        case 12:
            if (D_8006794C->unk1C->unk851 != 0) {
                goto state;
            }
            return func_800B2C80() ? 6 : 0;
        case 8:
            func_80055310();
            D_8006794C->unk1C->unk7E8 = D_8006794C->unk1C->unk81C[0];
            D_8006794C->unk1C->unk840 = D_8006794C->unk1C->unk820[0];
            D_8006794C->unk1C->unk850 = D_8006794C->unk1C->unk826[0];
            D_8006794C->unk1C->unk7EA = D_8006794C->unk1C->unkC;
            D_8006794C->unk1C->unk8AC = D_8006794C->unk1C->unk8A8;
            D_8006794C->unk1C->unk8B0 = D_8006794C->unk1C->unk84A;
        case 1:
            if ((arg0 & 0x3F) == 1) {
                func_800552B8(D_8006794C->unk1C->unk824[0]);
                D_8006794C->unk1C->unk826[0] = 0x7F;
                D_8006794C->unk1C->unk0 &= 0xDFFFFFFF;
                return 0;
            }
            func_800552B8(D_8006794C->unk1C->unk824[0]);
            D_8006794C->unk1C->unk826[0] = 0x7F;
            D_8006794C->unk1C->unk0 &= 0xDFFFFFFF;
            return 0;
        case 2:
            if ((arg0 & 0x3F) == 8) {
                return 0;
            }
            switch (arg0 & 0x3F) {
            case 1 ... 3:
                switch (arg0 & 0x3F) {
                case 1:
                    func_800559C8(D_8006794C->unk1C->unk824[0], arg2);
                    D_8006794C->unk1C->unk826[0] = arg2;
                    return 0;
                case 2:
                    func_80055B6C(arg2);
                    return 0;
                case 3:
                    func_80055C08(arg2);
                    return 0;
                case 4:
                    func_80055CA4(arg2);
                    return 0;
                }
                break;
            case 7:
                D_8006794C->unk1C->unk824[0] = func_800553BC(D_8006794C->unk1C->unk814, arg2);
                D_8006794C->unk1C->unk0 |= 0x20000000;
                return 0;
            }
            break;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800559FC(D_8006794C->unk1C->unk824[0], arg2, arg3);
                func_800B2C40(arg3, arg2);
                return 0;
            case 2:
                func_80055B98(arg2, arg3);
                return 0;
            case 3:
                func_80055C34(arg2, arg3);
                return 0;
            case 4:
                func_80055CD0(arg2, arg3);
                return 0;
            }
            break;
        case 4:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055A34(D_8006794C->unk1C->unk824[0], arg2, arg3, arg4);
                func_800B2C40(arg4, arg2);
                return 0;
            case 2:
                func_80055BCC(arg2, arg3, arg4);
                return 0;
            case 3:
                func_80055C68(arg2, arg3, arg4);
                return 0;
            case 4:
                func_80055D04(arg2, arg3, arg4);
                return 0;
            }
            break;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            func_800B2C40(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            func_800B2C40(arg4, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            func_80055604(arg1);
            return 0;
        }
        return -1;
    case 1:
        fp = 0;
        slot = 0;
        mask = 0;
        h = 0;
        id = (arg1 & 0xFFFF) | 0x80000;
        if (kind != 5 && kind != 6) {
            fp = (arg0 >> 7) & 1;
            if (fp != 0) {
                if (kind == 0 || kind == 8 || kind == 9) {
                    slot = (arg0 >> 6) & 1;
                } else {
                    for (slot = 0; slot < 2; slot++) {
                        if (D_8006794C->unk1C->unk830[slot] == arg1) {
                            break;
                        }
                    }
                    if (slot >= 2) {
                        return -1;
                    }
                }
                arg2 = (0x100000 << slot) & 0x300000;
                mask = 0x2000000 << (slot * 2);
                mask2 = mask >> 1;
                if (kind == 0 && D_8006794C->unk1C->unk830[slot] != arg1) {
                    if (D_8006794C->unk1C->unk0 & mask) {
                        func_8005546C(SFXH(slot), arg2);
                        D_8006794C->unk1C->unk0 ^= mask | mask2;
                    }
                    func_80015C28((void *)SFXH(slot), func_8001C7FC(4, id, 0), (s32)func_8001C7FC(5, id, 0));
                    D_8006794C->unk1C->unk830[slot] = arg1;
                    D_8006794C->unk1C->unk8B8[slot] = 0x7F;
                    D_8006794C->unk1C->unk8BA[slot] = 0x80;
                    D_8006794C->unk1C->unk0 |= mask2;
                }
                h = SFXH(slot);
            } else {
                h = (s32)func_8001C7FC(4, id, 0);
            }
        }
        switch (kind) {
        case 9:
            t = 0;
            if (D_8006794C->unk1C->unk830[slot] != 0xFFFF && D_8006794C->unk1C->unk8B4[slot] == (s16)D_8006794C->unk1C->unkC) {
                arg2 = (0x100000 << slot) & 0x300000;
                t = 1;
                arg3 = D_8006794C->unk1C->unk8BA[slot];
                arg4 = D_8006794C->unk1C->unk8B8[slot];
            }
            D_8006794C->unk1C->unk8B4[slot] = 0xFFFF;
            if (!t) {
                return 0;
            }
        case 0:
            if (!fp || !(D_8006794C->unk1C->unk0 & mask)) {
                func_800553EC(h, arg2, arg3, arg4);
            }
            if (fp) {
                D_8006794C->unk1C->unk8BA[slot] = arg3;
                D_8006794C->unk1C->unk8B8[slot] = arg4;
                D_8006794C->unk1C->unk0 |= mask;
            }
            return 0;
        case 8:
            if (!(D_8006794C->unk1C->unk0 & mask)) {
                return 0;
            }
            D_8006794C->unk1C->unk8B4[slot] = D_8006794C->unk1C->unkC;
        case 1:
            if ((arg0 & 0x3F) == 9 && fp) {
                if (D_8006794C->unk1C->unk0 & 0x2000000) {
                    func_8005546C((s32)D_8006794C->unk1C->unk828, 0x100000);
                }
                if (D_8006794C->unk1C->unk0 & 0x8000000) {
                    func_8005546C((s32)D_8006794C->unk1C->unk82C, 0x200000);
                }
                D_8006794C->unk1C->unk0 &= 0xF5FFFFFF;
            } else {
                func_8005546C(h, arg2);
                if (fp) {
                    D_8006794C->unk1C->unk0 &= ~mask;
                }
            }
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                if (fp) {
                    D_8006794C->unk1C->unk8B8[slot] = arg3;
                }
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                if (fp) {
                    D_8006794C->unk1C->unk8BA[slot] = arg3;
                }
                return 0;
            case 3:
                func_8005593C(h, arg2, arg3);
                return 0;
            case 8:
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8((void *)h, arg2, arg3, arg4);
                if (fp) {
                    D_8006794C->unk1C->unk8B8[slot] = arg4;
                }
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                if (fp) {
                    D_8006794C->unk1C->unk8BA[slot] = arg4;
                }
                return 0;
            case 3:
                func_80055980(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                if (fp) {
                    D_8006794C->unk1C->unk8B8[0] = D_8006794C->unk1C->unk8B8[1] = arg2;
                }
                return 0;
            case 5:
                func_800557F0(arg2);
                if (fp) {
                    D_8006794C->unk1C->unk8BA[0] = D_8006794C->unk1C->unk8BA[1] = arg2;
                }
                return 0;
            case 3:
                func_800558DC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                if (fp) {
                    D_8006794C->unk1C->unk8B8[0] = D_8006794C->unk1C->unk8B8[1] = arg3;
                }
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                if (fp) {
                    D_8006794C->unk1C->unk8BA[0] = D_8006794C->unk1C->unk8BA[1] = arg3;
                }
                return -1;
            case 3:
                func_80055908(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    case 2:
        id = (arg1 & 0xFFFF) | 0x180000;
        switch (kind) {
        case 0:
            slot = (s32)func_8001C7FC(4, id, 0);
            if (D_8006794C->unk1C->unk851 != 0) {
                return -1;
            }
            func_80056248(func_800AE68C() ? SCENE->unk5D : SCENE->unk5C);
            func_800561C4(slot, arg2, arg3);
            SCENE->unk5F = 0;
            return 0;
        case 1:
            func_80056228();
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                SCENE->unk5C = arg2;
                if (func_800AE68C() == 0) {
                    func_80056248(arg2);
                }
                return 0;
            case 10:
                SCENE->unk5D = arg2;
                if (func_800AE68C() != 0) {
                    func_80056248(arg2);
                }
                return 0;
            case 5:
                func_800562B0(arg2);
                return 0;
            case 12:
                SCENE->unk5F = arg2;
                return 0;
            case 8:
                return 0;
            }
            return -1;
        }
        return -1;
    case 3:
        __asm__(".rdata\n\t.word 0\n\t.text");
        id = (arg1 & 0xFFFF) | 0x90000;
        switch (kind) {
        case 7:
            SCENE->unk0 &= ~0x10;
            if (arg2 == 4) {
                SCENE->unk5E = arg3;
                SCENE->unk0 |= 0x10;
            }
            D_800C9D4C = kind;
            D_800C9D50 = id;
            D_8006794C->unk1C->unk851 = 1;
            return D_8006794C->unk1C->unk851;
        case 12:
            goto state;
        }
        return -1;
    }
    return -1;

state:
    switch (D_8006794C->unk1C->unk851) {
    case 1:
        if (func_800B2C80() == 0) {
            if (D_8006794C->unk1C->unk0 & 0x200000) {
                func_800552B8(D_8006794C->unk1C->unk824[0]);
                D_8006794C->unk1C->unk0 &= ~0x200000;
            }
            D_8006794C->unk1C->unk851 = 2;
        }
        break;
    case 2:
        if (((D_800C9D50 >> 16) & 0xFF) == 7) {
            D_8006794C->unk1C->unk852 = D_8006794C->unk1C->unk820[0];
        } else {
            D_8006794C->unk1C->unk852 = D_800C9D50;
        }
        if (D_800C9D4C == 9) {
            D_800C9D54 = D_8006794C->unk1C->unk8A8;
            sz = D_8006794C->unk1C->unk84A << 11;
        } else {
            D_800C9D54 = func_800B29A0(D_800C9D50, &sz);
            if (((D_800C9D50 >> 16) & 0xFF) == 7) {
                D_8006794C->unk1C->unk8A8 = D_800C9D54;
                D_8006794C->unk1C->unk84A = sz >> 11;
            }
        }
        g = D_8006794C->unk1C;
        if (!(g->unk8E4.field->unkC->unk14->unk0 & 0x10) && g->unk852 != 0x84) {
            func_80055DF4();
        }
        D_8006794C->unk1C->unk8C0 = SCENE->unk20 ? SCENE->unk20 : D_80067948;
        D_8006794C->unk1C->unk878 = 0x156F00 - (D_8006794C->unk1C->unk8C0 - (s32)D_80067944);
        if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 4)) {
            D_8006794C->unk1C->unk878 -= 0x3000;
        }
        m = D_8006794C->unk1C->unk878;
        D_8006794C->unk1C->unk878 = m / 2048 * 2048;
        D_8006794C->unk1C->unk87C = 0;
        func_8002177C(D_8006794C->unk1C->unk854, D_800C9D54, sz, (void *)D_8006794C->unk1C->unk8C0, 0,
                      D_8006794C->unk1C->unk878, func_800B29F8);
        D_8006794C->unk1C->unk851 = 3;
        break;
    case 3:
        if (func_800217C4(D_8006794C->unk1C->unk854) == 0 && func_80055DCC() == 0) {
            func_800AC67C((s32)D_8006794C->unk1C->unk854[0].unk20);
            func_80022280((s32)D_8006794C->unk1C->unk854[0].unk8, D_8006794C->unk1C->unk854[0].unk10);
            if (D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10 < D_8006794C->unk1C->unk87C) {
                D_8006794C->unk1C->unk8C0 = (s32)D_8006794C->unk1C->unk854[0].unk8;
                d = 0x156F00 - ((s32)D_8006794C->unk1C->unk854[0].unk8 - (s32)D_80067944);
                if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 4)) {
                    d -= 0x3000;
                }
                d = d / 2048 * 2048;
                func_8002177C(D_8006794C->unk1C->unk854, D_800C9D54, D_8006794C->unk1C->unk854[0].unk4, (void *)D_8006794C->unk1C->unk8C0,
                              D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10, d, func_800B29F8);
            } else if (D_800C9D52 == 7) {
                if (D_800C9D4C != 9) {
                    D_8006794C->unk1C->unk826[0] = 0x7F;
                }
                D_8006794C->unk1C->unk851 = 4;
            } else {
                D_800C9D4C = -1;
                D_8006794C->unk1C->unk851 = 0;
            }
        }
        break;
    case 4:
        n = D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10 - D_8006794C->unk1C->unk87C;
        if (n > 0) {
            func_80015C28(D_8006794C->unk1C->unk814, (u8 *)D_8006794C->unk1C->unk8C0 + (D_8006794C->unk1C->unk87C - D_8006794C->unk1C->unk854[0].unkC), n);
        }
        t = 0x6800 - n;
        D_8006794C->unk1C->unk8C0 = (s32)((u8 *)D_8006794C->unk1C->unk814 + n);
        t = t / 2048 * 2048;
        func_8002177C(D_8006794C->unk1C->unk854, D_800C9D54, D_8006794C->unk1C->unk854[0].unk4, (void *)D_8006794C->unk1C->unk8C0,
                      D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10, t, NULL);
        D_8006794C->unk1C->unk851 = 5;
        break;
    case 5:
        if (func_800217C4(D_8006794C->unk1C->unk854) == 0) {
            if (D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10 < D_8006794C->unk1C->unk854[0].unk4) {
                m = D_8006794C->unk1C->unk854[0].unkC + D_8006794C->unk1C->unk854[0].unk10 - D_8006794C->unk1C->unk87C;
                D_8006794C->unk1C->unk8C0 = (s32)((u8 *)D_8006794C->unk1C->unk814 + m);
                func_8002177C(D_8006794C->unk1C->unk854, D_800C9D54, D_8006794C->unk1C->unk854[0].unk4, (void *)D_8006794C->unk1C->unk8C0, m, 0x6800 - m, NULL);
            } else {
                func_800B2BC4((s32)D_8006794C->unk1C->unk814);
                D_8006794C->unk1C->unk0 |= 0x10000000;
                D_8006794C->unk1C->unk851 = 0;
                if (D_800C9D4C != 7) {
                    goto start;
                }
                D_800C9D4C = -1;
            }
        }
        break;
    }
    return D_8006794C->unk1C->unk851;
}

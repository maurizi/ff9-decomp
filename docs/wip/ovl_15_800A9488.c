/* STATUS: first draft, 400 of 1575 rows differ (aligned, cmp.py; was 413). ovl_15 func_800A9488 (0x189C, Blackjack main loop: 14-state machine in D_800B0DF8 plus the bankroll/wager/button text). Control flow, frame (0x1C8: buf[16] at sp18, ofsX[3] sp28, ofsY[3] sp38, hands sp48/spB0/sp118, spilled pseudos at sp180-sp19C) and most blocks line up. Findings: case 7 comes before case 6 in the source; the bankroll updates are 'D_800AE0E8 -= (h.unk0 & 4) ? bet * 2 : bet' (the ternary makes gcc keep &D_800AE0E8 in a0); the random byte must be a statement-expression macro with an s32 temp (the ff9.h inline function evaluates the rhs before the &ofsX[i] address and misses an s-reg in case 7); func_800AB1B8/func_800ACF48 must be undeclared (implicit int: no andi 0xFF), they are defined after this function in ovl_15.c. Left: (1) global alloc puts nsplit in s8 and the hoisted 0x88888889 (/30) in s7; the original has nsplit in s7 and the constant in fp. Two extra nsplit references flip it (381 rows), so nsplit's ref count is slightly low or the constant's slightly high; (2) cases 6/8-11 (0xC00-0xFFF, ~230 rows): the original keeps more separate copies of the win/lose/bankroll blocks (cross-jumping only merged tails because reload picked different t1/t2 spill regs), ours merges them; (3) the hoisted constants (-1, 1, 4, 0xF00000, %hi(D_800B0DF8), ...) are spilled and rematerialised in t1/t2, and which of t1/t2 each reload gets differs in places. INTEGRATION NEEDS A CONFIG CHANGE: the function's .rodata holds UI strings ("%2d", "Bankroll", "%ld", "Wager", "Stand", "Hit", "Double", "Split" at D_800A7018-D_800A7048) before its jump table; they need their own rodata split / INCLUDE_RODATA file so the C can reference them as extern symbols (no UI literals in C). The header change s32 func_800A9488(void) is already in ovl_15.h.
 Pass 16: the local externs of the UI strings conflicted with ovl_15.h (const char); removed, 400 rows. The
 five copies of the fade ramp (`t < 0x10 ? t * 16 : t < 0x85 ? 0xFF : (0x94 - t) * 16`) as an inline or a
 macro score 378 rows but only 1541 instructions (more merging), so the original has them written out.
 In case 6 the original stores res (spilled, 0x188(sp)) in the jal delay slot of func_8002F488, i.e. before
 the call, and every win/lose arm has its own `lui/lw/lw; j` pair into the shared D_800AE0E8 store; ours
 jumps from the first arm into code shared with a later block. */
#include "ovl_15.h"
extern Ovl15Shoe D_800B0E08;

s32 func_800A8F58(Ovl15Shoe *);
s32 func_800A8FA4(Ovl15Shoe *);
s32 func_800A90B8(Ovl15Hand *, Ovl15Shoe *);
s32 func_800A92FC(Ovl15Hand *);
s32 func_800A9334(Ovl15Hand *, s32, s32, s32);
void func_800ACEB8(void);
s32 func_800A7ABC(s32, s32, s32, s32);
void func_800AAFAC(void);
void func_800AAFFC(void);
s32 func_800AAFA4(void);
s32 func_800AAD24(void);
s32 func_800A8F00(s32, s32);

#define RND3() (({ s32 r_; if (D_8007B720 >= D_8007B92D) { func_8004BF20(); } r_ = *D_8007B720++; r_; }) % 3)

s32 func_800A9488(void) {
    char buf[16];
    s32 ofsX[3];
    s32 ofsY[3];
    Ovl15Hand dealer;
    Ovl15Hand player;
    Ovl15Hand split;
    s32 t;
    Ovl15Hand *cur;
    s32 nsplit;
    s32 cnt;
    s32 i;
    s32 res;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    Ovl15Hand *p1;
    s32 r;
    s32 a;
    s32 c;

    t = -1;
    cnt = 0;
    func_800AAD24();
    D_800B0F48 = 10;
    func_800A91F0(&player, 0x1E, 0x78);
    func_800A91F0(&dealer, 0x1E, 0xA);
    cur = &player;
    nsplit = 0;
    x = 0xF0;
    y = 0xA;
    res = 0;
    i = 0;
    dx = 0x10000;
    dy = 0x10000;
    p1 = cur;
    ofsX[0] = ofsX[1] = ofsX[2] = 0;
    ofsY[0] = ofsY[1] = ofsY[2] = 0;
    do {
        func_800AAFAC();
        func_80019194();
        switch (D_800B0DF8) {
        case 0:
            t = -1;
            D_800B0DF8 = 1;
            break;
        case 1:
            if (t == -1) {
                t = 0x4E;
                func_800A8F58(&D_800B0E08);
                func_800A8FA4(&D_800B0E08);
                func_800320B8(1, 1, 0x3C, 0);
                i = 0;
            } else if (t != 0) {
                c = D_800B0E08.unk4[i];
                i += 4;
                t--;
                func_800A8630(c >> 2, c & 3, 0xF0, 0xA);
            } else {
                t = -1;
                D_800B0DF8 = 2;
                func_800ACF58(2);
            }
            break;
        case 2:
            func_800A91F0(&player, 0x1E, 0x78);
            func_800A91F0(&dealer, 0x1E, 0xA);
            func_800ACEB8();
            if (func_800ACF48() == 4) {
                t = -1;
                D_800B0DF8 = 3;
            }
            break;
        case 3:
            if (t == -1) {
                dealer.unk0 |= 0x10;
                func_800A90B8(p1, &D_800B0E08);
                func_800A90B8(&dealer, &D_800B0E08);
                t = 0x78;
                func_800A90B8(p1, &D_800B0E08);
                func_800A90B8(&dealer, &D_800B0E08);
                cur = p1;
                i = 0;
                x = 0xF00000;
                y = 0xA0000;
                dx = (((player.unk8 - 0xF0) << 8) / 30) << 8;
                dy = (((player.unkA - 0xA) << 8) / 30) << 8;
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                func_800A9334(&dealer, 0, 0, i >= 2);
                if (i > 0) {
                    a = 2;
                    if (i < 3) {
                        a = 1;
                    }
                } else {
                    a = 0;
                }
                func_800A9334(p1, 0, 0, a);
                x += dx;
                y += dy;
                if ((t - 1) % 30 == 0 && ++i < 4) {
                    x = 0xF00000;
                    y = 0xA0000;
                    switch (i) {
                    case 1:
                        dx = (((dealer.unk8 - 0xF0) << 8) / 30) << 8;
                        dy = (((dealer.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    case 2:
                        dx = (((player.unk8 - 0xE3) << 8) / 30) << 8;
                        dy = (((player.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    case 3:
                        dx = (((dealer.unk8 - 0xE3) << 8) / 30) << 8;
                        dy = (((dealer.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    }
                }
                t--;
            } else if (func_800A945C(p1)) {
                t = -1;
                if (func_800A945C(&dealer)) {
                    D_800B0DF8 = 10;
                } else {
                    D_800B0DF8 = 8;
                }
            } else {
                t = -1;
                if (func_800A945C(&dealer)) {
                    if (func_800A945C(p1)) {
                        D_800B0DF8 = 10;
                    } else {
                        D_800B0DF8 = 8;
                    }
                } else {
                    D_800B0DF8 = 4;
                    func_800ACF58(3);
                }
            }
            break;
        case 4:
            r = func_800A92FC(cur);
            D_800B0F44 = 0;
            D_800B0F4C = 0;
            if ((r & 2) && D_800AE0E8 >= D_800B0F48 * 2) {
                D_800B0F44 = 1;
            }
            if ((r & 1) && nsplit == 0 && D_800AE0E8 >= D_800B0F48 * 2) {
                D_800B0F4C = 1;
            }
            if (t == -1) {
                func_800ACEB8();
                if (func_800ACF48() == 4) {
                    if (func_800AB1B8() == 0) {
                        if (nsplit == 1) {
                            cur = &split;
                            D_800B0DF8 = 4;
                            t = 30;
                            func_800ACF58(3);
                            x = 0xF00000;
                            y = 0xA0000;
                            dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                            dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                            nsplit = 2;
                        } else {
                            D_800B0DF8 = 5;
                            dealer.unk0 &= ~0x10;
                        }
                    } else if (func_800AB1B8() == 1) {
                        t = 30;
                        x = 0xF00000;
                        y = 0xA0000;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (func_800AB1B8() == 2) {
                        t = 30;
                        x = 0xF00000;
                        y = 0xA0000;
                        cur->unk0 |= 4;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (func_800AB1B8() == 3) {
                        if (func_800A92FC(cur) & 1) {
                            func_800A9228(&player, &split);
                            t = 30;
                            x = 0xF00000;
                            y = 0xA0000;
                            dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                            dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                            nsplit = 1;
                        }
                    }
                }
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                func_800A90B8(cur, &D_800B0E08);
                t = -1;
                if (cur->unk4 >= 22) {
                    D_800B0DF8 = 7;
                } else if (cur->unk4 == 21) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        t = 30;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else if (cur->unk0 & 8) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        t = 30;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else if (cur->unk0 & 4) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        t = 30;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else {
                    D_800B0DF8 = 4;
                    func_800ACF58(3);
                }
            }
            break;
        case 5:
            if (t == -1) {
                if (dealer.unk4 < 17) {
                    t = 30;
                    x = 0xF00000;
                    y = 0xA0000;
                    dy = 0;
                    dx = (((dealer.unk2 * 13 - 0xD2) << 8) / 30) << 8;
                } else if (dealer.unk0 & 2) {
                    D_800B0DF8 = 7;
                } else if (nsplit > 0) {
                    nsplit = 1;
                    D_800B0DF8 = 6;
                } else if (player.unk4 == dealer.unk4) {
                    D_800B0DF8 = 10;
                } else if (dealer.unk4 < player.unk4) {
                    D_800B0DF8 = 9;
                } else {
                    D_800B0DF8 = 11;
                }
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 5;
                func_800A90B8(&dealer, &D_800B0E08);
            }
            break;
        case 7:
            if (cur->unk4 >= 22) {
                i = 1;
                if (nsplit == 2) {
                    i = 2;
                }
            } else {
                i = 0;
            }
            if (t == -1) {
                t = 10;
                ofsX[i] = RND3();
                ofsY[i] = RND3();
            } else if (t > 0) {
                if (ofsX[i] < 0) {
                    ofsX[i] = RND3();
                } else {
                    ofsX[i] = -RND3();
                }
                if (ofsY[i] < 0) {
                    ofsY[i] = RND3();
                } else {
                    ofsY[i] = -RND3();
                }
                t--;
            } else {
                t = -1;
                ofsX[0] = ofsX[1] = ofsX[2] = 0;
                ofsY[0] = ofsY[1] = ofsY[2] = 0;
                if (nsplit == 1) {
                    cur = &split;
                    D_800B0DF8 = 4;
                    t = 30;
                    func_800ACF58(3);
                    x = 0xF00000;
                    y = 0xA0000;
                    dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                    dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    nsplit = 2;
                } else if (nsplit == 2) {
                    nsplit = 1;
                    D_800B0DF8 = 6;
                } else if (cur->unk4 >= 22) {
                    D_800B0DF8 = 11;
                } else {
                    D_800B0DF8 = 9;
                }
            }
            break;
        case 6:
            if (t == -1) {
                if (nsplit == 1) {
                    if (player.unk4 >= 22) {
                        res = 1;
                        func_8002F488(0x6B);
                        D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < player.unk4) {
                        res = 0;
                        func_8002F488(0x6C);
                        D_800AE0E8 += (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else {
                        res = 2;
                        if (player.unk4 != dealer.unk4) {
                            res = 1;
                            func_8002F488(0x6B);
                            D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                        }
                    }
                } else {
                    if (split.unk4 >= 22) {
                        res = 1;
                        func_8002F488(0x6B);
                        D_800AE0E8 -= (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < split.unk4) {
                        res = 0;
                        func_8002F488(0x6C);
                        D_800AE0E8 += (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else {
                        res = 2;
                        if (split.unk4 != dealer.unk4) {
                            res = 1;
                            func_8002F488(0x6B);
                            D_800AE0E8 -= (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                        }
                    }
                }
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(res, a, 0x6C, (nsplit == 1 ? player.unkA : split.unkA) + 12);
                t--;
            } else {
                t = -1;
                if (nsplit == 1) {
                    nsplit = 2;
                } else {
                    D_800B0DF8 = 12;
                }
            }
            break;
        case 8:
            if (t == -1) {
                if (dealer.unk4 < player.unk4) {
                    func_8002F488(0x6C);
                    D_800AE0E8 += D_800B0F48 + (D_800B0F48 >> 1);
                } else {
                    func_8002F488(0x6B);
                    D_800AE0E8 -= D_800B0F48;
                }
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(dealer.unk4 < player.unk4 ? 0 : 1, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 9:
            if (t == -1) {
                func_8002F488(0x6C);
                D_800AE0E8 += (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                t--;
                func_800A7ABC(0, a, 0x6C, player.unkA + 12);
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 10:
            if (t == -1) {
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                t--;
                func_800A7ABC(2, a, 0x6C, player.unkA + 12);
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 11:
            if (t == -1) {
                func_8002F488(0x6B);
                D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                t--;
                func_800A7ABC(1, a, 0x6C, player.unkA + 12);
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 12:
            if (D_800B0E08.unk0 & 4) {
                t = -1;
                D_800B0DF8 = 13;
            } else {
                nsplit = 0;
                if (D_800AE0E8 < D_800B0F48) {
                    D_800B0F48 = D_800AE0E8 / 10 * 10;
                }
                t = -1;
                D_800B0DF8 = 2;
                func_800ACF58(2);
            }
            break;
        case 13:
            if (t == -1) {
                func_800320B8(1, 1, 0x3C, 0xFFFFFF);
                t = 0x3C;
                if (D_800AE0E8 < 10) {
                    D_800AE0E8 = 1000;
                }
            } else if (t > 0) {
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 14;
            }
            break;
        }
        if (D_800B0DF8 >= 4) {
            func_800A9334(&dealer, ofsX[0], ofsY[0], -1);
            if (dealer.unk2 != 0 && D_800B0DF8 >= 5) {
                sprintf(buf, D_800A7018, dealer.unk4);
                func_8002B56C(10, dealer.unkA + 0x20, 0x10, buf, 0);
            }
            if (nsplit == 2) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    func_8002B56C(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        func_8002B56C(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
            func_800A9334(&player, ofsX[1], ofsY[1], -1);
            if (player.unk2 != 0) {
                sprintf(buf, D_800A7018, player.unk4);
                func_8002B56C(10, player.unkA + 0x20, 0x10, buf, 0);
                if (player.unk4 != 21 && (player.unk0 & 1)) {
                    sprintf(buf, D_800A7018, player.unk6);
                    func_8002B56C(10, player.unkA + 0x2A, 0x10, buf, 5);
                }
            }
            if (nsplit == 1) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    func_8002B56C(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        func_8002B56C(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
        }
        func_800A8F00(0xF0, 0xA);
        addPrim(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] + 10, &D_800AE4E8);
        sprintf(buf, D_800A701C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x58, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800AE0E8);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x64, 0x10, buf, 5);
        sprintf(buf, D_800A702C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x70, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800B0F48);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x7C, 0x10, buf, 0);
        sprintf(buf, D_800A7034);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x90, 0x10, buf, 0);
        sprintf(buf, D_800A703C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xA0, 0x10, buf, 0);
        sprintf(buf, D_800A7040);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xB0, 0x10, buf, D_800B0F44 ? 0 : 2);
        sprintf(buf, D_800A7048);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xC0, 0x10, buf, D_800B0F4C ? 0 : 2);
        D_800739B0[0].unk0 = 0;
        if (cnt > 0) {
            cnt--;
        }
        if (D_800B0DF8 < 14) {
            if (D_800AE0E8 < 10) {
                t = -1;
                D_800B0DF8 = 13;
            }
            if (D_80073990[0] & 8) {
                t = -1;
                D_800B0DF8 = 13;
            }
        }
        func_80032098();
        func_800AAFFC();
    } while (D_800B0DF8 < 14);
    func_800AAFA4();
    return 0;
}

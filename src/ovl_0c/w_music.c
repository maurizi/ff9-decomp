#include "ovl_0c.h"

void w_musicUpdate(void) {
    s32 st;

    switch (D_800D4818) {
    case 1:
        if (D_800CF454 == 0) {
            if (g_worldMapDebugFlags & 4) {
                printf("w_musicUpdate : Play\n");
            }
            D_800CF454 = 2;
            if (D_800D481C != -1) {
                func_800C46D8(D_800D481C);
            }
            func_800C465C(D_800D4828);
            D_800D4818 = 2;
            D_800D481C = D_800D4828;
        }
        break;
    case 2:
        st = func_800AB0C0(0xC00, 0, 0, 0, 0);
        if (st == 0) {
            D_800CF454 = 0;
            D_800D4818 = 0;
            if (g_worldMapDebugFlags & 4) {
                printf("w_musicUpdate : Ready\n");
            }
        } else if (g_worldMapDebugFlags & 4) {
            printf("w_musicUpdate : Waiting [ State : %d ] \n", st);
        }
        break;
    }
}

/* sound effect ids of world-map sound arg0 */
s32 w_musicGetID(s32 arg0, Unk800C3FE4 *arg1) {
    Unk800C3FE4 s;

    s.unk8[0] = -1;
    switch (arg0) {
    case 0:
        s.unk8[0] = 0x68;
        break;
    case 1:
        s.unk8[0] = 0x66;
        break;
    case 2:
        s.unk8[0] = 0x6C;
        break;
    case 3:
        s.unk8[0] = 0x27D;
        break;
    case 4:
        s.unk8[0] = 0x27E;
        break;
    case 5:
        s.unk8[0] = 0x2AA;
        break;
    case 39:
        s.unk8[0] = 0x67;
        break;
    }
    if (s.unk8[0] != -1) {
        s.unk0 = 0x800000;
        s.unk4 = 1;
        *arg1 = s;
        return 1;
    } else {
        switch (arg0) {
        case 30:
            s.unk8[0] = 0x44B;
            break;
        case 31:
            s.unk8[0] = 0x4CF;
            break;
        case 32:
            s.unk8[0] = 0x596;
            break;
        case 34:
            s.unk8[0] = 0x44C;
            break;
        case 35:
            s.unk8[0] = 0x37C;
            break;
        case 36:
            s.unk8[0] = 0x44A;
            break;
        case 37:
            s.unk8[0] = 0x37A;
            break;
        case 38:
            s.unk8[0] = 0x5DB;
            break;
        case 40:
            s.unk8[0] = 0x37B;
            break;
        case 41:
            s.unk8[0] = 0xA47;
            break;
        case 42:
            s.unk8[0] = 0xC26;
            break;
        }
        if (s.unk8[0] != -1) {
            s.unk0 = 0;
            s.unk4 = 1;
            *arg1 = s;
            return 1;
        } else {
            switch (arg0) {
            case 6:
                s.unk8[0] = 0x552;
                break;
            case 7:
                s.unk8[0] = 0x553;
                break;
            }
            if (s.unk8[0] != -1) {
                s.unk0 = 0x8000;
                s.unk4 = 1;
                *arg1 = s;
                return 1;
            } else {
                /* the original third jump table is 8-aligned */
                __asm__(".rdata\n\t.word 0\n\t.text");
                switch (arg0) {
                case 33:
                    s.unk8[0] = 0x37A;
                    s.unk0 = 0;
                    s.unk8[1] = 0x37B;
                    break;
                case 20:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x374;
                    s.unk8[1] = 0x375;
                    break;
                case 21:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x376;
                    s.unk8[1] = 0x377;
                    break;
                case 22:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x378;
                    s.unk8[1] = 0x379;
                    break;
                case 23:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x554;
                    s.unk8[1] = 0x555;
                    break;
                case 24:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x5D9;
                    s.unk8[1] = 0x5DA;
                    break;
                case 25:
                    s.unk0 = 0x800;
                    s.unk8[0] = 0x37D;
                    s.unk8[1] = 0x37E;
                    break;
                case 26:
                    s.unk0 = 0x800;
                    s.unk8[0] = 0x37F;
                    s.unk8[1] = 0x380;
                    break;
                }
                if (s.unk8[0] != -1) {
                    s.unk4 = 2;
                    *arg1 = s;
                    return 1;
                }
                printf("w_musicGetID : Illigal SE Number #%d\n", arg0);
                return 0;
                printf(""); /* unreachable; its empty string is in .rodata */
            }
        }
    }
}

const char D_800A832C[] = "w_musicRequestSend : Normal Song";
const char D_800A8350[] = "w_musicRequestSend : Air Ship Song";
const char D_800A8374[] = "w_musicRequestSend : Chocobo Song";
const char D_800A8398[] = "w_musicRequestSend : Disc4 Song";
const char D_800A83B8[] = "w_musicRequestSend : Field Song 1";
const char D_800A83DC[] = "w_musicRequestSend : Field Song 2";
const char D_800A8400[] = "w_musicRequestSend : Unknown Error";
const char D_800A8424[] = "(Accepted)\n";
const char D_800A8430[] = "(Denied / same request)\n";
/* padded: the jump table that follows is 8-aligned in its object */
const char D_800A844C[0x38] = "w_musicRequestSend : music status is not ready now\n";

void w_musicRequestSend(u32 arg0) {
    if (D_800D4818 == 0) {
        if (g_worldMapDebugFlags & 4) {
            switch (arg0) {
            case 0:
                printf(D_800A832C);
                break;
            case 2:
                printf(D_800A8350);
                break;
            case 1:
                printf(D_800A8374);
                break;
            case 3:
                printf(D_800A8398);
                break;
            case 4:
                printf(D_800A83B8);
                break;
            case 5:
                printf(D_800A83DC);
                break;
            default:
                printf(D_800A8400);
                break;
            }
        }
        if (D_800D4828 != arg0) {
            D_800D4818 = 1;
            D_800D4828 = arg0;
            if (g_worldMapDebugFlags & 4) {
                printf(D_800A8424);
            }
        } else if (g_worldMapDebugFlags & 4) {
            printf(D_800A8430);
        }
    } else if (g_worldMapDebugFlags & 4) {
        printf(D_800A844C);
    }
}

void func_800C43F0(void) {
    Unk800C3FE4 s;
    s32 i;
    s32 j0;
    s32 m0;
    s32 j1;
    s32 m1;
    s32 j2;
    s32 m2;

    for (i = 0; i < 7; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m0 = s.unk0;
            for (j0 = 0; j0 < s.unk4; j0++) {
                func_800AB0C0(0x5100, s.unk8[j0], m0, 0, 0);
                m0 >>= 1;
            }
        }
    }
    for (i = 20; i < 26; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m1 = s.unk0;
            for (j1 = 0; j1 < s.unk4; j1++) {
                func_800AB0C0(0x5100, s.unk8[j1], m1, 0, 0);
                m1 >>= 1;
            }
        }
    }
    for (i = 30; i < 42; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m2 = s.unk0;
            for (j2 = 0; j2 < s.unk4; j2++) {
                func_800AB0C0(0x5100, s.unk8[j2], m2, 0, 0);
                m2 >>= 1;
            }
        }
    }
    if ((u8)func_800C49EC() && D_800CF49C != 3) {
        g_gameState->unk1C->unk0 |= 0x400000;
    }
}

s32 func_800C45D8(void) {
    u16 sc = g_evtGlobalVars.h[0];
    u8 v = g_evtGlobalVars.b[0xBE];

    if (sc - 0x2580U < 0x122) {
        return 1;
    }
    if (sc - 0x26A2U < 0x14) {
        return 1;
    }
    return v - 8U < 2;
}

void func_800C461C(void) {
    D_800D4818 = 0;
    D_800D4824 = -1;
    D_800D481C = -1;
    D_800D4828 = -1;
    D_800D4820 = 0;
}

void func_800C464C(void) {
}

void func_800C4654(void) {
}

void func_800C465C(u16 arg0) {
    u8 v = g_evtGlobalVars.b[0x68];

    D_800D4824 = func_800C497C(arg0);
    D_800D4820 = 1;
    func_800AB0C0(0, D_800D4824, 0, 0, 0);
    if (v == 0) {
        v = 0x7D;
    }
    func_800AB0C0(0x8601, 0, 0x3C, v, 0);
}

void func_800C46D8(u16 arg0) {
    func_800AB0C0(0x100, func_800C497C(arg0), 0, 0, 0);
}

void func_800C4710(void) {
}

void func_800C4718(s32 arg0, u8 arg1) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0xD000, s.unk8[i], v, 0x7F, arg1);
            v >>= 1;
        }
    }
}

void func_800C47B0(s32 arg0, u8 arg1) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (D_800D4818 == 0 && (w_musicGetID(arg0, &s) & 0xFF)) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0x9201, s.unk8[i], v, arg1, 0);
            v >>= 1;
        }
    }
}

void func_800C4850(s32 arg0, u16 arg1, u8 arg2) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0xD301, s.unk8[i], v, arg1, arg2);
            v >>= 1;
        }
    }
}

void func_800C48F4(s32 arg0) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0x5100, s.unk8[i], v, 0, 0);
            v >>= 1;
        }
    }
}

s32 func_800C497C(u16 arg0) {
    u16 sc;

    if (arg0 == 0) {
        sc = *D_800CF430;
        if (sc - 0x2580U < 0x122) {
            arg0 = 4;
        }
        if (sc - 0x26A2U < 0x14) {
            arg0 = 5;
        }
        if (g_worldDataSet == 4) {
            arg0 = 3;
        }
    }
    return D_800CDF30[arg0];
}

s32 func_800C49EC(void) {
    u8 r = D_800D4824 == D_800CDF30[0];

    if (D_800D4824 == D_800CDF30[1]) {
        r = 1;
    }
    if (D_800CE3C4 == 0) {
        if (D_800D4824 == D_800CDF30[3]) {
            r = 1;
        }
        if (D_800D4824 == D_800CDF30[4]) {
            r = 1;
        }
    }
    return r;
}

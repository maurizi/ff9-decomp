#include "ovl_3a.h"

void func_801F3874(void) {
    u32 id;
    s32 i;

    func_8003575C(D_801EDF18);
    func_80035580(&D_801F4B48);
    func_8003556C(func_801F4A9C);
    D_801F4BBC.unk0 = 1;
    D_801F4BBC.unk1 = 0;
    D_801F4BBC.unk2 = 0;
    D_801F4BBC.unk3 = 0;
    D_801F4BBC.unk24 = D_80077DBC < 12;
    D_801F4BBC.unk12C = func_801F4A68(D_801F4BBC.unk24 ? D_80077DBC : D_80077DBC - 12);
    D_801F4BBC.unk130 = func_801F4A84(D_801F4BBC.unk12C);
    for (i = 0; i < 2; i++) {
        D_801F4BBC.unk5[i] = D_801F4B50[i];
    }
    func_80033D38(D_801F4BBC.unk11, D_801F4BBC.unk12C);
    memset(D_801F4BBC.unk7, 0xFF, 10);
    if (D_801F4BBC.unk24) {
        func_80033D38(D_801F4BBC.unk7, D_801F4BBC.unk12C);
    } else {
        func_8002BD54(D_801F4BBC.unk7, g_gameState->unk1C->unk3C[D_801F4BBC.unk130].unk0);
    }
    D_801F4BBC.unk4 = func_8002BCC0(D_801F4BBC.unk7);
    for (i = 0; i < 2; i++) {
        D_801F4BBC.unk1C[i] = func_80035774(i + 0xD);
    }
    setRECT(&D_801F4BBC.unk134[0], 0xF0, 8, 0x44, 0x1A);
    setRECT(&D_801F4BBC.unk134[1], 0x16, 0x1A, 0x24, 0x32);
    setRECT(&D_801F4BBC.unk134[2], 0xE, 0x52, 0x44, 0x69);
    setRECT(&D_801F4BBC.unk134[3], 0x1C, 0x26, 0x118, 0x18);
    setRECT(&D_801F4BBC.unk134[4], 0x1C, 0x3E, 0x118, 0xA0);
    func_801F49A0(D_801F4BBC.unk130);
}

void func_801F3A98(void) {
    if (!func_8002F810()) {
        if (D_80077528 & 8) {
            func_8002F488(0x67);
            D_801F4BBC.unk0 = 0;
            D_801F4BBC.unk1 = 3;
            func_80035580(NULL);
            return;
        }
        if (D_80077528 & 0x100000) {
            func_8002F488(0x67);
            D_801F4BBC.unk2 = D_801F4BBC.unk2 == 0 ? 1 : D_801F4BBC.unk2 - 1;
            func_801F4AE0();
            return;
        }
        if (D_80077528 & 0x200000) {
            func_8002F488(0x67);
            D_801F4BBC.unk2 = D_801F4BBC.unk2 != 0 ? 0 : D_801F4BBC.unk2 + 1;
            func_801F4AE0();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F3D9C();
            return;
        }
        if (D_801F4BBC.unk0 == 0) {
            func_801F47E8();
            return;
        }
        func_801F3BBC();
    }
}

void func_801F3BBC(void) {
    if ((D_80077528 & 0x10) && D_801F4BBC.unk3 >= 15) {
        func_8002F488(0x67);
        D_801F4BBC.unk3 -= 15;
        return;
    }
    if ((D_80077528 & 0x40) && D_801F4BBC.unk3 < D_801F4BBC.unk5[D_801F4BBC.unk2] - 15) {
        func_8002F488(0x67);
        D_801F4BBC.unk3 += 15;
        return;
    }
    if ((D_80077528 & 0x20) && (u8)(D_801F4BBC.unk3 % 15) != 14) {
        func_8002F488(0x67);
        D_801F4BBC.unk3++;
        return;
    }
    if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        if ((u8)(D_801F4BBC.unk3 % 15) != 0) {
            D_801F4BBC.unk3--;
            return;
        }
        D_801F4BBC.unk0 = 0;
        func_80035580(NULL);
        return;
    }
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        if (D_801F4BBC.unk4 != 0) {
            D_801F4BBC.unk4--;
            D_801F4BBC.unk7[D_801F4BBC.unk4] = 0xFF;
        }
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EDF00);

void func_801F3D9C(void) {
    if (D_801F4BBC.unk0 == 0) {
        switch (D_801F4BBC.unk1) {
        case 0:
        case 1:
            D_801F4BBC.unk2 = D_801F4BBC.unk1;
            func_801F4AE0();
            func_8002F488(0x67);
            break;
        case 2:
            func_8002F488(0x65);
            if (D_801F4BBC.unk4 != 0) {
                D_801F4BBC.unk4--;
                D_801F4BBC.unk7[D_801F4BBC.unk4] = 0xFF;
            }
            break;
        case 3:
            if (D_801F4BBC.unk4 != 0) {
                func_8002F488(0x67);
                memcpy(g_gameState->unk1C->unk3C[D_801F4BBC.unk130].unk0, D_801F4BBC.unk7, 8);
                func_800325E0();
            } else {
                func_8002F488(0x66);
            }
            break;
        case 4:
            func_8002F488(0x67);
            D_801F4BBC.unk4 = func_8002BCC0(D_801F4BBC.unk11);
            memset((s32)D_801F4BBC.unk7, 0xFF, 8);
            memcpy(D_801F4BBC.unk7, D_801F4BBC.unk11, D_801F4BBC.unk4);
            break;
        }
    } else {
        func_8002F488(0x67);
        D_801F4BBC.unk7[D_801F4BBC.unk4 >= 7 ? D_801F4BBC.unk4 - 1 : D_801F4BBC.unk4] =
            D_801F4BBC.unk1C[D_801F4BBC.unk2][D_801F4BBC.unk3];
        D_801F4BBC.unk4 = D_801F4BBC.unk4 >= 7 ? D_801F4BBC.unk4 : D_801F4BBC.unk4 + 1;
    }
}

void func_801F3FAC(void) {
    SprtWords *p;
    u32 clut = (GetClut(0, 0x100) << 16) | 0xB000;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = (D_801F4B58->x + 2) | ((D_801F4B58->y + 2) << 16);
    p->uvclut = clut;
    p->wh = 0x2E0020;
    addPrim(&g_ot[4], p);
    g_primPtr = (u8 *)(p + 1);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, 0x9F);
    addPrim(&g_ot[4], g_primPtr);
    g_primPtr += 8;
    func_8002DEE4(D_801F4B58, 6, 0);
    func_8002705C(D_801F4B58, 6, 0);
}

void func_801F4110(void) {
    s32 i;

    func_800269F4(D_801F4B5C->x + 4, D_801F4B5C->y + 0x2A, 5, D_801F4B5C->w - 9);
    func_8002DEE4(D_801F4B5C, 5, 2);
    func_8002705C(D_801F4B5C, 5, 2);
    for (i = 0; i < 2; i++) {
        func_8002B5FC(D_801F4B5C->x + 0xA, D_801F4B5C->y + 9 + i * 0x10, 4, func_80035774(i + 2),
                      D_801F4BBC.unk2 == i ? 4 : 0);
    }
    for (i = 0; i < 3; i++) {
        func_8002B5FC(D_801F4B5C->x + 0xA, D_801F4B5C->y + 0x31 + i * 0x10, 4, func_80035774(i + 4), 0);
    }
}

void func_801F4238(void) {
    LINE_F2 *p;
    s32 cursor;
    s32 i;
    s32 j;
    s32 k;
    s32 y;

    k = 0;
    cursor = 6;
    if (D_801F4BBC.unk4 < 6) {
        cursor = D_801F4BBC.unk4;
    }
    func_8002B524(D_801F4B64->x + 0x23, D_801F4B64->y - 4, 5, D_8007047C, 0);
    func_800269F4(D_801F4B64->x + 4, D_801F4B64->y + 0x1A, 6, D_801F4B64->w - 8);
    func_8002DEE4(D_801F4B64, 6, 1);
    func_8002705C(D_801F4B64, 6, 1);
    for (i = 0; i < 7; i++) {
        if (cursor != i || (D_80077DB0.unk10 & 4)) {
            for (j = 0; j < 2; j++) {
                p = (LINE_F2 *)g_primPtr;
                SetLineF2(p);
                if (j == 0) {
                    setRGB0(p, 0xB0, 0xB0, 0xB0);
                } else {
                    setRGB0(p, 0x40, 0x40, 0x40);
                }
                y = j + 0x15;
                p->x0 = D_801F4B64->x + (i * 16 + 0x3E) + j;
                p->y0 = p->y1 = D_801F4B64->y + y;
                p->x1 = p->x0 + 0xB;
                g_primPtr += sizeof(LINE_F2);
                addPrim(&g_ot[5], p);
            }
        }
    }
    for (i = 0; i < D_801F4BBC.unk4; i++) {
        func_80028F3C(D_801F4B64->x + 0x3F + i * 16, D_801F4B64->y + 7, 5, D_801F4BBC.unk7[i], 0);
    }
    for (i = 0; i < (u8)(D_801F4BBC.unk5[D_801F4BBC.unk2] / 15); i++) {
        for (j = 0; j < 15; j++) {
            func_80028F3C(D_801F4B64->x + 0x3E + j * 14, D_801F4B64->y + 0x22 + i * 21, 5,
                          D_801F4BBC.unk1C[D_801F4BBC.unk2][k], 0);
            k++;
        }
    }
    g_primPtr = func_8002BA90(&g_ot[5], g_primPtr);
}

void func_801F459C(void) {
    RECT *r;
    s32 y;
    s32 row;
    u32 idx;
    u32 col;
    u32 sub;

    if (D_801F4BBC.unk0 == 0) {
        Unk80077DE8 *h;
        D_80077DE8.unk0 = D_801F4BBC.unk134[2].x - 0xE;
        h = &D_80077DE8;
        y = D_801F4BBC.unk134[2].y + D_801F4BBC.unk1 * 16;
        h->unk2 = D_801F4BBC.unk1 >= 2 ? y + 0x15 : y + 0xD;
    } else {
        r = &D_801F4BBC.unk134[4];
        if (D_801F4BBC.unk2 != 0) {
            row = D_801F4BBC.unk3 / 15;
            idx = D_801F4BBC.unk3 % 15;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
            col = (u8)(idx / 5);
            sub = (u8)(idx % 5);
            D_80077DE8.unk0 = r->x + grp_x(col) + sub * 13 - 0x17;
        } else {
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 % 15;
            D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}

void func_801F4748(void) {
    func_801F3A98();
    func_801F47A0();
}

void func_801F4770(void) {
    func_8003556C(NULL);
    func_80035580(NULL);
    func_801F4A34();
}

void func_801F47A0(void) {
    func_801F48A0();
    func_801F3FAC();
    func_801F4110();
    func_801F490C();
    func_801F4238();
    func_801F459C();
}

void func_801F47E8(void) {
    if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_801F4BBC.unk1 = D_801F4BBC.unk1 == 0 ? 4 : D_801F4BBC.unk1 - 1;
        return;
    }
    if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_801F4BBC.unk1 = D_801F4BBC.unk1 >= 4 ? 0 : D_801F4BBC.unk1 + 1;
        return;
    }
    if (D_80077528 & 0x20) {
        func_8002F488(0x67);
        D_801F4BBC.unk0 = 1;
        func_80035580(&D_801F4B48);
    }
}

void func_801F48A0(void) {
    func_8002DEE4(D_801F4B54, 6, 2);
    func_8002705C(D_801F4B54, 6, 2);
    func_8002B5FC(D_801F4B54->x + 0xA, D_801F4B54->y + 6, 5, func_80035774(0), 0);
}

void func_801F490C(void) {
    func_8002DEE4(D_801F4B60, 7, 1);
    func_8002705C(D_801F4B60, 7, 1);
    func_8002B5FC(D_801F4B60->x + 0x3E, D_801F4B60->y + 6, 6, func_80035774(1), 0);
    func_8002B524(D_801F4B60->x + 0x23, D_801F4B60->y - 4, 6, D_80070450, 0);
}

void func_801F49A0(s32 chr) {
    u8 *img;

    img = D_801EE0B4[func_80028954(chr)];
    StoreImage(&D_801F4B68, (u32 *)D_801F4BBC.unk28);
    DrawSync(0);
    LoadImage(&D_801F4B68, (u32 *)img);
    DrawSync(0);
    LoadImage(&D_801F4B70, (u32 *)(img + 0x100));
    DrawSync(0);
}

void func_801F4A34(void) {
    LoadImage(&D_801F4B78, (u32 *)D_801F4BBC.unk28);
    DrawSync(0);
}

s32 func_801F4A68(s32 i) {
    return D_801F4B80[i];
}

s32 func_801F4A84(s32 i) {
    return D_801F4BB0[i];
}

void func_801F4A9C(void) {
    D_80077DE8.unk8 = func_80035774(D_801F4BBC.unk0 == 0 ? D_801F4BBC.unk1 + 7 : 0xC);
}

void func_801F4AE0(void) {
    if (D_801F4BBC.unk2 == 1 && D_801F4BBC.unk3 >= D_801F4BBC.unk5[1]) {
        D_801F4BBC.unk3 = D_801F4BBC.unk5[1] + (u8)(D_801F4BBC.unk3 % 15 - 15);
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EDF18);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EE0B4);

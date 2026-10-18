#include "ovl_3e.h"

void func_801F605C(void) {
    s32 *src;
    Ovl3eState *st;
    s32 i;
    u8 r;
    u8 state;

    src = D_80073998;
    st = &D_801F6BBC;
    st->unk2 = 0;
    st->unk10 = 0;
    g_menu.unk9 = 0;
    st->unk4 = (g_menu.unkC >> 14) & 3;
    st->unk6 = (u16)g_menu.unkC & 0x3FFF;
    for (i = 0; i < 2; i++) {
        st->unk8[i] = src[i];
        src[i] |= 0x300;
    }
    func_8003556C(0);
    func_801F673C();
    cd_stopMotor();
    D_801F6BBC.unk0 = r = cd_checkShellOpen();
    state = 1;
    if (r) {
        state = 2;
    }
    D_801F6BBC.unk1 = state;
}

void func_801F6124(void) {
    SprtWords *p;
    s32 i;
    RECT unused[2];

    for (i = 0; i < 2; i++) {
        setlen(g_primPtr, 4);
        p = (SprtWords *)g_primPtr;
        p->rgbc = 0x64808080;
        p->xy = i * 160;
        p->uvclut = (i * 160) & 0x3F;
        p->wh = 0xE000A0;
        g_primPtr = (u8 *)(p + 1);
        addPrim(&g_ot[6], p);
        SetDrawTPage(g_primPtr, 1, 1, (((i * 160) >> 6) + 0x10) | 0x100);
        addPrim(&g_ot[6], g_primPtr);
        g_primPtr += 8;
    }
}

void func_801F6254(void) {
    SprtWords *p;
    u32 clut = GetClut(0, 0x1FF) << 16;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = 0x1C004E;
    p->uvclut = clut;
    p->wh = 0xA800C2;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[6], p);
    SetDrawTPage(g_primPtr, 1, 1, 0x95);
    addPrim(&g_ot[6], g_primPtr);
    g_primPtr += 8;
}

u8 func_801F6368(void) {
    CdlLOC loc;
    u8 result[8];
    s32 mask;
    s32 i;
    s32 ret;
    s32 type;

    mask = 1 << D_801F6BBC.unk4;
    CdControlB(1, NULL, result);
    i = 0;
    if (result[0] & 0x10) {
        return 1;
    }
    CdControlB(7, NULL, NULL);
    do {
        VSync(0);
        if (i >= 600) {
            return 2;
        }
        CdControlB(1, NULL, result);
        i++;
    } while (!(result[0] & 2));

    ret = CdDiskReady(0);
    switch (ret) {
    case 5:
        return 3;
    case 2:
        break;
    case 0x10:
        return 1;
    default:
        return 4;
    }
    type = CdGetDiskType();
    switch (type) {
    case 1:
        return 3;
    case 0:
        return 2;
    case 2:
        break;
    case 0x10:
        return 1;
    default:
        return 4;
    }
    CdIntToPos(5, &loc);
    CdControlB(0x15, (u8 *)&loc, result);
    if (result[0] & 1) {
        return 4;
    }
    if (result[1] & 0x40) {
        return 4;
    }
    CdControlB(0xE, (u8 *)0x80, result);
    VSync(3);
    for (i = 0; i < 10; i++) {
        func_8001DCB8();
        if (g_gameState->unkB != 0xFF) {
            break;
        }
    }
    if (g_gameState->unkB == 0xFF || !(g_gameState->unkB & mask)) {
        cd_stopMotor();
        return 6;
    }
    snd_playSe(0x4ED);
    return 7;
}

void func_801F65A4(void) {
    func_801F6654();
    func_801F66AC();
    func_801F66F4();
}

void func_801F65D4(void) {
    s32 *dst;
    Ovl3eState *st;
    s32 i;

    if (D_801F6BBC.unk6 != 15000) {
        evt_setNextLocation(D_801F6BBC.unk6);
    }
    dst = D_80073998;
    st = &D_801F6BBC;
    g_gameState->unk0 |= 0x100000;
    for (i = 0; i < 2; i++) {
        dst[i] = st->unk8[i];
    }
}

void func_801F6654(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = D_801F6B60[D_801F6BBC.unk1];
        if (f != NULL) {
            f();
        }
    }
}

void func_801F66AC(void) {
    void (*f)(void) = D_801F6B74[D_801F6BBC.unk1];

    if (f != NULL) {
        f();
    }
}

void func_801F66F4(void) {
    void (*f)(void) = D_801F6B88[D_801F6BBC.unk1];

    if (f != NULL) {
        f();
    }
}

void func_801F673C(void) {
    u32 *buf = (u32 *)0x801CDF00;
    s32 size;
    u32 *clut = D_801EDF0C;
    s32 i;
    s32 sector;

    sector = g_gameState->unk20 + file_find(1, D_801F6BAC[D_801F6BBC.unk4 & 3])->unk4;
    size = 0x11800;
    for (i = 0; i < 2; i++) {
        cd_readSync(sector, size, buf);
        func_801F682C(buf, i);
        sector += 0x23;
    }
    LoadImage(&D_801F6B9C, clut);
    DrawSync(0);
    LoadImage(&D_801F6BA4, clut + 0x80);
    DrawSync(0);
}

void func_801F682C(u32 *p, s32 i) {
    RECT r = D_801EDF04;

    r.x += i * 0xA0;
    LoadImage(&r, p);
    DrawSync(0);
}

void func_801F689C(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        func_801F6A3C();
    }
}

void func_801F68D8(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        func_801F6A3C();
    }
}

void func_801F6914(void) {
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        D_801F6BBC.unk1 = 4;
        func_8002FAAC(0x18);
        func_8002F8A8(func_801F6AF0);
    }
}

void func_801F6968(void) {
    u8 r;

    D_801F6BBC.unk0 = r = cd_checkShellOpen();
    if (r) {
        D_801F6BBC.unk1 = 2;
    }
}

void func_801F69A4(void) {
    u8 r;

    D_801F6BBC.unk0 = r = cd_checkShellOpen();
    if (!r) {
        snd_playSe(0x67);
        func_801F6A3C();
    }
}

void func_801F69E8(void) {
    if (++D_801F6BBC.unk10 >= 180) {
        D_801F6BBC.unk1 = 4;
        func_8002FAAC(0x18);
        func_8002F8A8(func_801F6AF0);
    }
}

void func_801F6A3C(void) {
    D_801F6BBC.unk2 = 0;
    D_801F6BBC.unk1 = 0;
    func_8002FAAC(0x18);
    func_8002F8A8(func_801F6A78);
}

void func_801F6A78(void) {
    u8 r;

    D_801F6BBC.unk3 = r = func_801F6368();
    D_801F6BBC.unk10 = 0;
    switch (r) {
    case 1:
    default:
        D_801F6BBC.unk1 = 3;
        func_8002FA50(0x18);
        break;
    case 7:
        func_80032640(0);
        func_80032634(0);
        func_800325E0();
        break;
    }
}

void func_801F6AF0(void) {
    D_801F6BBC.unk1 = 0;
    func_8002FA50(0x18);
    func_8002F8A8(func_801F6B24);
}

void func_801F6B24(void) {
    D_801F6BBC.unk1 = cd_checkShellOpen() ? 2 : 1;
}

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_3e/nonmatchings/ovl_3e", D_801EDF0C);

#include "ovl_65.h"

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", D_801EF260);

void gameover_main(s32 arg0) {
    s16 c = 0xFF;
    u8 state = 0;
    DispBuf b[2];
    RECT rect = D_801EF260;
    DispBuf *cur;

    func_801F3818(b);
    cur = b;
    DrawSync(0);
    func_801F3A20(arg0);
    while (1) {
        cur = (cur == &b[0]) ? &b[1] : &b[0];
        ClearOTag(cur->unk70, 2);
        switch (state) {
        case 0:
            state++;
            break;
        case 1:
            setRGB0(&cur->unkB8, c, c, c);
            c -= 4;
            addPrim(&cur->unk70[0], &cur->unk78);
            addPrim(&cur->unk70[0], &cur->unk94);
            addPrim(&cur->unk70[1], &cur->unkB0);
            if (c < 0) {
                c = 0;
                state++;
            }
            break;
        case 2:
            setRGB0(&cur->unkB8, 0, 0, 0);
            addPrim(&cur->unk70[0], &cur->unk78);
            addPrim(&cur->unk70[0], &cur->unk94);
            addPrim(&cur->unk70[1], &cur->unkB0);
            if (g_padPortPressed[0] & BTN_CROSS) {
                state++;
            }
            break;
        case 3:
            c += 4;
            if (c >= 0x100) {
                ClearImage(&rect, 0, 0, 0);
                DrawSync(0);
                if (c > 0x100) {
                    state++;
                }
            } else {
                setRGB0(&cur->unkB8, c, c, c);
                addPrim(&cur->unk70[0], &cur->unk78);
                addPrim(&cur->unk70[0], &cur->unk94);
                addPrim(&cur->unk70[1], &cur->unkB0);
            }
            break;
        case 4:
            ClearImage(&rect, 0, 0, 0);
            DrawSync(0);
            return;
        }
        pad_update();
        DrawSync(0);
        VSync(0);
        PutDispEnv(&cur->disp);
        DrawOTagEnv(cur->unk70, &cur->draw);
    }
}

void func_801F3818(DispBuf *b) {
    RECT rect = D_801EF27C;
    u32 i;

    ClearImage(&rect, 0, 0, 0);
    SetDefDrawEnv(&b[0].draw, 0, 0, 320, 224);
    SetDefDispEnv(&b[0].disp, 320, 0, 320, 224);
    SetDefDrawEnv(&b[1].draw, 320, 0, 320, 224);
    SetDefDispEnv(&b[1].disp, 0, 0, 320, 224);
    for (i = 0; i < 2; i++) {
        b[i].draw.dtd = 1;
        b[i].draw.isbg = 1;
        b[i].disp.isinter = 0;
        setRECT(&b[i].disp.screen, 0, 8, 256, 224);
        func_80063580(&b[i].unk78, GetTPage(2, 0, 0, 256));
        setXY0(&b[i].unk80, 0, 0);
        setUV0(&b[i].unk80, 0, 0);
        setWH(&b[i].unk80, 256, 224);
        setRGB0(&b[i].unk80, 0x80, 0x80, 0x80);
        func_80063580(&b[i].unk94, GetTPage(2, 0, 256, 256));
        setXY0(&b[i].unk9C, 256, 0);
        setUV0(&b[i].unk9C, 0, 0);
        setWH(&b[i].unk9C, 64, 224);
        setRGB0(&b[i].unk9C, 0x80, 0x80, 0x80);
        func_800635E0(&b[i].unkB0, GetTPage(0, 2, 0, 0));
        setXY0(&b[i].unkB8, 0, 0);
        setWH(&b[i].unkB8, 320, 224);
        setRGB0(&b[i].unkB8, 0, 0, 0);
        SetSemiTrans(&b[i].unkB8, 1);
    }
}

void func_801F3A20(s32 arg0) {
    FileEntry *e = file_find(1, 0xB);

    cd_readSync(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, arg0);
    file_loadTim(arg0);
}

s32 battleEnd_update(void) {
    GameData *g;

    g = g_gameState->unk1C;
    func_8004C204();
    switch (g->unk56E) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        if (!rewards_update()) {
            if (g->unk56E == 2) {
                g->unk56E = 1;
            }
            func_800F4E68();
            func_801F0FC4();
            return 1;
        }
        func_8004BC38();
        return 0;
    case 6:
        if (func_800240E8()) {
            func_800240B0(0);
        }
        func_800F4E68();
        gameover_main((s32)g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk1C);
        return 2;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/gameover", D_801EF27C);

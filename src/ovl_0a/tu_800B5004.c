#include "ovl_0a.h"

void func_800B5004(s32 arg0, s16 arg1) {
    D_800CA058.unk1 = 1 << arg0;
    D_800CA058.unk2 = arg0;
    D_800CA058.unk4 = arg1;
    D_800CA058.unk6 = -1;
    g_gameState->unk1C->unk8E4.field->unk0 |= 0x100000;
    g_gameState->unk0 |= 8;
}

u8 func_800B5064(void) {
    return D_800CA058.unk2;
}

u8 func_800B5074(void) {
    return D_800CA058.unk1;
}

s16 func_800B5084(void) {
    return D_800CA058.unk4;
}

s16 func_800B5094(void) {
    return D_800CA058.unk6;
}

void func_800B50A4(void) {
    func_800B5114();
    while (D_800CA058.unk0 != 4) {
        func_800B5290();
        func_800B52DC();
        func_800B532C();
    }
    func_800B51D0();
}

void func_800B5114(void) {
    s32 i;

    DrawSync(0);
    for (i = 0; i < 2; i++) {
        D_800CA058.unk8[i] = g_padPortMask[i];
        g_padPortMask[i] = 0x04000300;
    }
    D_800CA058.unk0 = 1;
    D_800CA058.unk3 = 0;
    g_gameState->unk1C->unk8E4.field->unk0 = 0;
    func_8002DE60();
    D_800C9D5C[D_800CA058.unk0].unk0();
}

/* leave the current field mode */
void func_800B51D0(void) {
    FieldMap *f;
    s32 i;

    if (D_800C9D5C[D_800CA058.unk0].unk8 != NULL) {
        D_800C9D5C[D_800CA058.unk0].unk8();
    }
    DrawSync(0);
    for (i = 0; i < 2; i++) {
        g_padPortMask[i] = D_800CA058.unk8[i];
    }
    f = g_gameState->unk1C->unk8E4.field;
    f->unk0 |= 0x10000;
    file_loadDirTables((GameState *)f);
    func_800A9570();
}

void func_800B5290(void) {
    GameData *gd = g_gameState->unk1C;

    ClearOTagR(gd->unk10[gd->unk8], 0x1000);
    pad_update();
}

void func_800B52DC(void) {
    if (D_800C9D5C[D_800CA058.unk0].unk4 != NULL) {
        D_800C9D5C[D_800CA058.unk0].unk4(D_800CA058.unk0);
    }
}

void func_800B532C(void) {
    func_80032100();
    game_updateWindows();
    func_80032098();
    game_updateTimer();
    DrawSync(0);
    VSync(0);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    DrawOTagEnv(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][0xFFF], &g_gameState->unk10[g_gameState->unk1C->unk8]);
    g_gameState->unk1C->unk8 ^= 1;
    gfx_setBuffer(g_gameState->unk1C->unk8);
}

void func_800B5430(s32 arg0) {
    if (arg0 != D_800CA058.unk0) {
        if (D_800C9D5C[D_800CA058.unk0].unk8 != NULL) {
            D_800C9D5C[D_800CA058.unk0].unk8();
        }
        D_800CA058.unk0 = arg0;
        if (D_800C9D5C[D_800CA058.unk0].unk0 != NULL) {
            D_800C9D5C[D_800CA058.unk0].unk0();
        }
    }
}

s32 func_800B54CC(void) {
    return (g_gameState->unk1C->unk8E4.field->unk0 >> 20) & 1;
}

void func_800B54FC(void) {
    func_8002F9D0();
    help_init();
    help_setHandler(func_800B57C8);
    text_setBank(D_800A717C);
}

void func_800B553C(void) {
    pad_updateRepeat(0);
    func_800B559C();
    func_800B5660();
    help_update();
}

void func_800B5574(void) {
    help_setBounds(NULL);
    help_setHandler(NULL);
}

void func_800B559C(void) {
    if (!func_8002F810()) {
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(0x67);
            D_800CA058.unk3 ^= 1;
        } else if (g_padRepeat & BTN_CIRCLE) {
            if (D_800CA058.unk3 == 0) {
                snd_playSe(0x67);
                D_800CA058.unk3 = 1;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            snd_playSe(0x67);
            func_8002F9F0();
            func_8002F8A8(func_800B5794);
        }
    }
}

/* DISC CHANGE label */
INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/tu_800B5004", D_800A7170);

void func_800B5660(void) {
    s32 y;
    HelpState *m;

    ui_drawLabel(D_800C9D98.x + 8, D_800C9D98.y - 3, 0xA, D_800A7170, 0);
    text_drawBlock(D_800C9D98.x + 0xE, D_800C9D98.y + 0xA, 0xA, text_get(0), 0);
    text_drawBlock(D_800C9D98.x + 0x30, D_800C9D98.y + 0x2E, 0xA, text_get(1), 0);
    text_drawBlock(D_800C9D98.x + 0x30, D_800C9D98.y + 0x3D, 0xA, text_get(2), 0);
    if (!func_8002F810()) {
        m = &g_help;
        m->x = D_800C9D98.x + 0x16;
        y = D_800C9D98.y;
        if (D_800CA058.unk3 == 0) {
            m->y = y + 0x32;
        } else {
            m->y = y + 0x41;
        }
        D_80077DEC = 9;
        func_800355E4();
    }
    func_8002DEE4(&D_800C9D98, 0xA, 2);
}

void func_800B5794(void) {
    if (D_800CA058.unk3 == 0) {
        func_800B5430(2);
    } else {
        func_800B5430(3);
    }
}

void func_800B57C8(void) {
    RECT r;
    s32 id;
    HelpState *m = &g_help;

    m->text = NULL;
    if (!func_8002F810()) {
        if (D_800CA058.unk3 == 0) {
            help_setBounds(NULL);
            id = 3;
        } else {
            r.x = 0;
            r.y = m->y;
            r.w = g_gameState->unkC->disp.w;
            r.h = g_gameState->unkC->disp.h - m->y;
            help_setBounds(&r);
            id = 4;
        }
        m->text = text_get(id);
    }
}

void func_800B5870(void) {
    menu_open(4, 2);
    func_80032628(func_800B58B8);
    func_80032634(0);
}

void func_800B58A8(void) {
}

void func_800B58B0(void) {
}

void func_800B58B8(void) {
    func_800B5430(1);
}

void func_800B58D8(void) {
    menu_open(8, ((D_800CA058.unk2 & 3) << 14) | D_800CA058.unk4);
    func_80032628(func_800B5938);
    func_80032634(0);
}

void func_800B5928(void) {
}

void func_800B5930(void) {
}

void func_800B5938(void) {
    func_800B5430(4);
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/tu_800B5004", D_800A717C);

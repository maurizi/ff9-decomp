#include "ovl_0a.h"

void fld_requestDiscChange(s32 arg0, s16 arg1) {
    g_fieldDiscChange.discMask = 1 << arg0;
    g_fieldDiscChange.disc = arg0;
    g_fieldDiscChange.arg = arg1;
    g_fieldDiscChange.unk6 = -1;
    g_gameState->unk1C->unk8E4.field->unk0 |= 0x100000;
    g_gameState->unk0 |= 8;
}

u8 fld_getDiscChangeDisc(void) {
    return g_fieldDiscChange.disc;
}

u8 fld_getDiscChangeMask(void) {
    return g_fieldDiscChange.discMask;
}

s16 fld_getDiscChangeFieldId(void) {
    return g_fieldDiscChange.arg;
}

s16 fld_getDiscChangeAreaId(void) {
    return g_fieldDiscChange.unk6;
}

void fld_runDiscChange(void) {
    fld_discChangeEnter();
    while (g_fieldDiscChange.mode != 4) {
        fld_discChangeBeginFrame();
        fld_discChangeUpdate();
        fld_discChangeEndFrame();
    }
    fld_discChangeLeave();
}

void fld_discChangeEnter(void) {
    s32 i;

    DrawSync(0);
    for (i = 0; i < 2; i++) {
        g_fieldDiscChange.savedFlags[i] = D_80073998[i];
        D_80073998[i] = 0x04000300;
    }
    g_fieldDiscChange.mode = 1;
    g_fieldDiscChange.cursor = 0;
    g_gameState->unk1C->unk8E4.field->unk0 = 0;
    func_8002DE60();
    g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter();
}

/* leave the current field mode */
void fld_discChangeLeave(void) {
    FieldMap *f;
    s32 i;

    if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave != NULL) {
        g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave();
    }
    DrawSync(0);
    for (i = 0; i < 2; i++) {
        D_80073998[i] = g_fieldDiscChange.savedFlags[i];
    }
    f = g_gameState->unk1C->unk8E4.field;
    f->unk0 |= 0x10000;
    func_8001DE50((GameState *)f);
    fld_refindSectors();
}

void fld_discChangeBeginFrame(void) {
    GameData *gd = g_gameState->unk1C;

    ClearOTagR(gd->unk10[gd->unk8], 0x1000);
    pad_update();
}

void fld_discChangeUpdate(void) {
    if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].update != NULL) {
        g_fieldDiscChangeModes[g_fieldDiscChange.mode].update(g_fieldDiscChange.mode);
    }
}

void fld_discChangeEndFrame(void) {
    func_80032100();
    func_80023A0C();
    func_80032098();
    func_80023A4C();
    DrawSync(0);
    VSync(0);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    DrawOTagEnv(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][0xFFF], &g_gameState->unk10[g_gameState->unk1C->unk8]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}

void fld_discChangeSetMode(s32 arg0) {
    if (arg0 != g_fieldDiscChange.mode) {
        if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave != NULL) {
            g_fieldDiscChangeModes[g_fieldDiscChange.mode].leave();
        }
        g_fieldDiscChange.mode = arg0;
        if (g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter != NULL) {
            g_fieldDiscChangeModes[g_fieldDiscChange.mode].enter();
        }
    }
}

s32 fld_isDiscChangePending(void) {
    return (g_gameState->unk1C->unk8E4.field->unk0 >> 20) & 1;
}

void fld_discPromptEnter(void) {
    func_8002F9D0();
    func_80035430();
    func_8003556C(fld_discPromptHelp);
    text_setBank(g_fieldDiscChangeText);
}

void fld_discPromptUpdate(void) {
    func_8002C308(0);
    fld_discPromptInput();
    fld_discPromptDraw();
    func_80035490();
}

void fld_discPromptLeave(void) {
    func_80035580(NULL);
    func_8003556C(NULL);
}

void fld_discPromptInput(void) {
    if (!func_8002F810()) {
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(0x67);
            g_fieldDiscChange.cursor ^= 1;
        } else if (g_padRepeat & BTN_CIRCLE) {
            if (g_fieldDiscChange.cursor == 0) {
                snd_playSe(0x67);
                g_fieldDiscChange.cursor = 1;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            snd_playSe(0x67);
            func_8002F9F0();
            func_8002F8A8(fld_discPromptSelect);
        }
    }
}

/* DISC CHANGE label */
INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/fld_disc", g_fieldDiscChangeLabel);

void fld_discPromptDraw(void) {
    s32 y;
    Unk80077DE8 *m;

    ui_drawLabel(g_fieldDiscChangeRect.x + 8, g_fieldDiscChangeRect.y - 3, 0xA, g_fieldDiscChangeLabel, 0);
    text_drawBlock(g_fieldDiscChangeRect.x + 0xE, g_fieldDiscChangeRect.y + 0xA, 0xA, text_get(0), 0);
    text_drawBlock(g_fieldDiscChangeRect.x + 0x30, g_fieldDiscChangeRect.y + 0x2E, 0xA, text_get(1), 0);
    text_drawBlock(g_fieldDiscChangeRect.x + 0x30, g_fieldDiscChangeRect.y + 0x3D, 0xA, text_get(2), 0);
    if (!func_8002F810()) {
        m = &D_80077DE8;
        m->unk0 = g_fieldDiscChangeRect.x + 0x16;
        y = g_fieldDiscChangeRect.y;
        if (g_fieldDiscChange.cursor == 0) {
            m->unk2 = y + 0x32;
        } else {
            m->unk2 = y + 0x41;
        }
        D_80077DEC = 9;
        func_800355E4();
    }
    func_8002DEE4(&g_fieldDiscChangeRect, 0xA, 2);
}

void fld_discPromptSelect(void) {
    if (g_fieldDiscChange.cursor == 0) {
        fld_discChangeSetMode(2);
    } else {
        fld_discChangeSetMode(3);
    }
}

void fld_discPromptHelp(void) {
    RECT r;
    s32 id;
    Unk80077DE8 *m = &D_80077DE8;

    m->unk8 = NULL;
    if (!func_8002F810()) {
        if (g_fieldDiscChange.cursor == 0) {
            func_80035580(NULL);
            id = 3;
        } else {
            r.x = 0;
            r.y = m->unk2;
            r.w = g_gameState->unkC->disp.w;
            r.h = g_gameState->unkC->disp.h - m->unk2;
            func_80035580(&r);
            id = 4;
        }
        m->unk8 = text_get(id);
    }
}

void fld_discChangeOpenMenu4(void) {
    menu_open(4, 2);
    func_80032628(fld_discChangeBackToPrompt);
    func_80032634(0);
}

void func_800B58A8(void) {
}

void func_800B58B0(void) {
}

void fld_discChangeBackToPrompt(void) {
    fld_discChangeSetMode(1);
}

void fld_discChangeOpenMenu8(void) {
    menu_open(8, ((g_fieldDiscChange.disc & 3) << 14) | g_fieldDiscChange.arg);
    func_80032628(fld_discChangeFinish);
    func_80032634(0);
}

void func_800B5928(void) {
}

void func_800B5930(void) {
}

void fld_discChangeFinish(void) {
    fld_discChangeSetMode(4);
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/fld_disc", g_fieldDiscChangeText);

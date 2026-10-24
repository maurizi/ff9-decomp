#include "ff9.h"

s32 func_80050D38(s32 r) {
    GameData *gd;

    if (D_8007BC50.unk24 < 2) {
        func_80053658(D_8007BC50.unkC == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (D_8007BC50.unk24 != 0) {
        D_8007BC50.unk24++;
    }
    if (D_8007BC50.unk4 != 0) {
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        r = func_80053764(D_8007BC50.unk4, gd->unk8, gd->unk10[gd->unk8] + 3, r);
    }
    return r;
}

u8 *func_80050E34(Unk8007BC20Node *buf) {
    Unk8007BC20Node *p = buf;
    Unk8007BC20Node *end;
    D_8007BC50.unk0 = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->unk0 = p + 1;
        }
        p[-1].unk0 = NULL;
    }
    D_8007BC50.unk4 = 0;
    SetDrawTPage(&D_8007BC50.unk10, 0, 0, GetTPage(0, 3, 0, 0));
    D_8007BC50.unk18 = D_8007BC50.unk10;
    D_8007BC50.unk8 = 0;
    D_8007BC50.unkC = 0;
    D_8007BC50.unk24 = 0;
    D_8007BC48 = (u8 *)(buf + 222);
    return (u8 *)p;
}

void func_80050EFC(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        Unk8007BC20 *p = &D_8007BC50;

        if (a < 0x83) {
            p->unk8 = a;
        } else {
            p->unk8 = 0x82;
        }
    }
    D_8007BC50.unkC = b * 16;
    D_8007BC50.unk24 = a == 0 && b != 0;
}

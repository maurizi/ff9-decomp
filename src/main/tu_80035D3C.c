#include "ff9.h"

void func_80035D3C(void) {
    Unk80077E44 *t;

    printf("path=%s\n", "f:/code/dms/loc_us/ff9db.h");
    D_80077E50.unk2 = 0;
    D_80077E50.unk0 = 1;
    D_80077E50.unk1 = 1;
    D_80077E50.unk4 = 0;
    D_80077E50.unk8 = func_8002F904();
    D_80077E50.unk3 = party_getFirstMember();
    D_80077E28.unk14 = D_8006E508;
    D_80077E28.unk8 = 0;
    D_80077E28.unk0 = 0;
    D_80077E28.unk10 = 0xFF;
    D_80077E28.unk1 = 0;
    D_80077E28.unk4 = 0;
    D_80077E28.unk6 = 0;
    D_80077E28.unk5 = 0;
    D_80077E28.unk1C = g_gameState->mode != 3 ? D_8006E530 : D_8006E5B0;
    t = D_80077E44;
    t[D_80077E52].unk0();
}

void func_80035E40(void) {
    u8 all = 0;
    RECT *r = &D_80077E28.unk14;
    s32 sel;
    s32 i;
    s32 y;

    if (D_80077E52 == 0 || (D_80077E28.unk0 != 0 && (*(u32 *)&D_80077E28 & 0xFFFF00))) {
        all = 1;
    }
    win_setClip(NULL, 2, 2);
    if (D_80077E28.unk7 != 0) {
        sel = D_80077E28.unk7 - 1;
    } else {
        sel = D_80077E52 - 1;
    }
    for (i = 0; i < 7; i++) {
        y = i * 15 + 6;
        if (all || i == sel) {
            text_drawBlock(r->x + 10, r->y + y + D_80077E28.unk8, 2,
                          (u8 *)text_getFrom(D_800110A0, i), 0);
        }
    }
    win_setClip(r, 2, 2);
    func_8002DEE4(r, 2, 2);
    func_8002705C(r, 2, 2);
}

void func_80035F94(void) {
    if (D_80077E28.unk0 != 0) {
        if (D_80077E28.unk3 != 0) {
            func_80036444();
        }
        if (D_80077E28.unk1 != 0) {
            func_80036344();
        }
        if (D_80077E28.unk2 != 0) {
            func_800360B0();
        }
        if (D_80077E28.unk6 == 0 && D_80077E28.unk3 == 0 && *(u16 *)&D_80077E28.unk4 == 0 &&
            !(*(u32 *)&D_80077E28 & 0xFFFF00)) {
            D_80077E28.unk0 = 0;
            D_80077E50.unk4 = 0;
            D_80077E50.unk2 = D_80077E28.unk7;
            func_8002F9D0();
            DrawSync(0);
            EnterCriticalSection();
            FlushCache();
            ExitCriticalSection();
            D_80077E28.unk1C[D_80077E50.unk2].unk0();
        }
    }
}

void func_800360B0(void) {
    D_80077E28.unk8 += D_80077E28.unkC;
    if (D_80077E28.unkC >= 0) {
        if (D_80077E28.unk8 >= D_80077E28.unkA) {
            D_80077E28.unk8 = D_80077E28.unkA;
            D_80077E28.unk2 = 0;
            D_80077E28.unkE = 0;
        }
    } else if (D_80077E28.unkA >= D_80077E28.unk8) {
        D_80077E28.unk8 = D_80077E28.unkA;
        D_80077E28.unk2 = 0;
        D_80077E28.unkE = 0;
    }
    D_80077E28.unkE++;
}

void func_8003613C(void) {
    void (*fn)(void);
    Unk80077E44 *t;
    func_80035E40();
    func_80035F94();
    t = D_80077E44;
    fn = t[D_80077E50.unk2].unk4;
    if (fn != NULL) {
        fn();
    }
    D_80077E50.unk4++;
}

void func_800361A8(void) {
    func_8003556C(0);
    D_80077E50.unk0 = 0;
}

void func_800361CC(s32 arg0) {
    s32 prev = D_80077E52;

    if (D_80077E28.unk1C[arg0].unk4 != 0) {
        D_80077E28.unk7 = arg0;
        D_80077E28.unk0 = 1;
        D_80077E28.unk1 = 1;
        D_80077E28.unk5 = 1;
        D_80077E28.unk3 = 0;
        D_80077E28.unk4 = 0;
        D_80077E28.unk6 = 0;
        func_800363BC();
        func_80036444();
        D_80077E28.unk20 = D_80077E28.unk1C[prev].unk8;
        func_8002F9F0();
        func_8002F8E4(3);
        func_8002F8A8(func_80036528);
    }
}

void func_80036270(u8 a) {
    D_80077E28.unk0 = 1;
    D_80077E28.unk6 = 1;
    D_80077E28.unk7 = a;
    func_800363BC();
    func_8002FAAC(0xC);
    func_8002F8A8(func_80036560);
}

u8 func_800362B8(void) {
    return D_80077E52;
}

void func_800362C8(void) {
    func_8003556C(NULL);
    func_8002F8E4(D_80077E58);
    func_800325E0();
}

u8 func_80036300(s32 i) {
    return D_80077E44[i].unkC;
}

s32 func_8003631C(s32 a) {
    return text_getFrom(D_800110A0, a + 9);
}

void func_80036344(void) {
    if (D_80077E28.unk7 == 0) {
        D_80077E28.unk14.h += 8;
        if (D_80077E28.unk14.h >= 0x74) {
            D_80077E28.unk14.h = 0x74;
            D_80077E28.unk1 = 0;
        }
    } else {
        D_80077E28.unk14.h -= 8;
        if (D_80077E28.unk14.h < 0x1B) {
            D_80077E28.unk14.h = 0x1A;
            D_80077E28.unk1 = 0;
        }
    }
}

void func_800363BC(void) {
    s32 v;
    s32 d;
    s32 q;
    s32 t = D_80077E2F;
    if (t != 0) {
        v = (1 - t) * 15;
    } else {
        v = 0;
    }
    d = v - D_80077E30;
    if (d >= 0) {
        q = (d + 11) / 12;
    } else {
        q = (d - 11) / 12;
    }
    D_80077E28.unkC = q;
    D_80077E28.unkA = v;
    D_80077E28.unkE = 0;
    D_80077E28.unk2 = 1;
}

void func_80036444(void) {
    if (D_80077E2F != 0) {
        func_80036470();
    }
}

void func_80036470(void) {
    s32 id = D_8006E510[D_80077E28.unk7];
    FileEntry *f;

    if (id != 0xFF && id != D_80077E28.unk10) {
        D_80077E28.unk3 = 1;
        D_80077E28.unk10 = id;
        f = file_find(0, id);
        if (cd_readAsync(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, func_80036634)) {
            D_80077E28.unk4 = 1;
            D_80077E28.unk3 = 0;
        }
    }
}

void func_80036528(void) {
    D_80077E28.unk5 = 0;
    if (D_80077E28.unk20 != NULL) {
        D_80077E28.unk20();
    }
}

void func_80036560(void) {
    D_80077E28.unk1C[D_80077E50.unk2].unk8();
    D_80077E28.unk0 = 0;
    D_80077E50.unk2 = D_80077E28.unk7;
    D_80077E28.unk1C[D_80077E50.unk2].unk0();
    if (g_gameState->mode != 3) {
        func_800C9510(D_80077E50.unk2 - 1);
    } else {
        func_800CC820(D_80077E50.unk2 - 1);
    }
    func_8002FA50(12);
    func_8002F8A8(NULL);
}

void func_80036634(u8 *a) {
    if (*a == 0) {
        D_80077E2C = 0;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/tu_80035D3C", D_800110A0);

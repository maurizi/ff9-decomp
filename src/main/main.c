#include "ff9.h"

/* __main and __do_global_dtors (SN startup) walk an empty constructor list
 * that starts at the beginning of .rodata, where main's jump table sits. */
__asm__(".section .rodata\n.globl jtbl_80010000\njtbl_80010000:\n.section .text");

void func_800127A0(void) {
    __main();
    func_800169B8();
    for (;;) {
        func_80016A28();
        func_8001CB08();
        func_8001D268();
        do {
            switch (g_gameState->mode) {
            case 0:
                func_80012900(0);
                func_800A7088();
                break;
            case 1:
                func_80012900(1);
                func_800A7A48();
                break;
            case 2:
                func_80012900(2);
                func_800A859C();
                break;
            case 8:
                func_80012900(8);
                func_801EF284();
                break;
            case 3:
                func_80012900(3);
                func_800A8860();
                break;
            case 9:
                func_80012900(9);
                func_800A752C();
                break;
            case 7:
                func_80012900(7);
                func_800A80A4();
                break;
            case 4:
                func_80012900(4);
                func_800A713C();
                break;
            }
        } while (!(g_gameState->unk0 & 1));
        func_8001D30C();
        func_8001CB70();
        func_80016B6C();
    }
}

void func_80012900(s32 mode) {
    u32 loaded = 0;
    u8 files[10] = { 20, 10, 11, 12, 21, 0xFF, 0xFF, 101, 101, 13 };
    LoadAddrs addrs = D_80010034;
    s32 id;
    Unk80021938 *e;
    FileEntry *f;

    func_8001CFB4();
    id = files[mode] | 0x10000;
    while (func_80022B18()) {
    }
    if (func_800220E8(id)) {
        e = func_80021E5C(id);
        if (func_80021AE0(e)) {
            StoreImage((RECT *)&e->unk20, addrs.unk0[mode]);
        } else if ((s32)addrs.unk0[mode] != e->unk8) {
            func_8001DAA4((s32)addrs.unk0[mode], e->unk8, e->unkC, NULL);
        }
        loaded = e->unkC >> 11;
        func_80022390(id);
        DrawSync(0);
    }
    f = func_8001E218(0, id);
    if (loaded < f->unkC - f->unk4) {
        func_80022DF0(g_gameState->unk20 + f->unk4 + loaded, (f->unkC - (f->unk4 + loaded)) << 11,
                      (u8 *)addrs.unk0[mode] + (loaded << 11));
    }
    func_80012D08();
    FlushCache();
    func_80012D18();
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010034);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80012AD4);

s8 func_80012B58(void) {
    return D_80071E32;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", __SN_ENTRY_POINT);

INCLUDE_ASM("asm/us/main/nonmatchings/main", __main);

INCLUDE_ASM("asm/us/main/nonmatchings/main", __do_global_dtors);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitHeap);

INCLUDE_ASM("asm/us/main/nonmatchings/main", FlushCache);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80012D08);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80012D18);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ResetGraph);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetGraphDebug);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetGraphQueue);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetGraphDebug);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawSyncCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDispMask);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawSync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001310C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ClearImage);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ClearImage2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadImage);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StoreImage);

INCLUDE_ASM("asm/us/main/nonmatchings/main", MoveImage);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010198);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800101A4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ClearOTag);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ClearOTagR);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawPrim);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawOTag);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800101E0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PutDrawEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawOTagEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDrawEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PutDispEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDispEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetODE);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawArea);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawOffset);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800140E8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014358);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014378);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014410);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800144A8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800144C4);

s32 func_80014544(void) {
    return *D_80066764;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001455C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001463C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001486C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014AA8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014D28);

void func_80014D3C(void) {
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014D44);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014D84);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014DCC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014DFC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80014E20);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800150D0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80015330);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001546C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800155A8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800155DC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80015720);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadImage2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StoreImage2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", MoveImage2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DrawOTag2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _GPU_ResetCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80015C00);

INCLUDE_ASM("asm/us/main/nonmatchings/main", memcpy);

INCLUDE_ASM("asm/us/main/nonmatchings/main", printf);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GPU_cw);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VSync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80015DD0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80015E68);

INCLUDE_ASM("asm/us/main/nonmatchings/main", puts);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ChangeClearPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ChangeClearRCnt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ResetCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InterruptCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DMACallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VSyncCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VSyncCallbacks);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StopCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", RestartCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CheckCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetIntrMask);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetIntrMask);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001603C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016114);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800162E4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001642C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800164CC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016544);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016568);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _96_remove);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ReturnFromException);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ResetEntryInt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", HookEntryInt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", startIntrVSync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016620);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001668C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800166B8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", startIntrDMA);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016734);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800168B4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016960);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetVideoMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetVideoMode);

void func_800169B8(void) {
    ResetCallback();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    InitGeom();
    SpuInit();
    InitCARD(0);
    StartCARD();
    func_80016BA0();
    func_80018F20();
    func_80022A28();
}

void func_80016A28(void) {
    RECT r;
    s32 i;

    SetDispMask(0);
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    SetGeomOffset(0xA0, 0x70);
    r.w = 0x3FF;
    r.x = 0;
    r.y = 0;
    r.h = 0x1FF;
    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    func_80022A80();
    func_8001C6F8();
    func_80018F50();
    D_80073690.mask = 0x19;
    D_80073690.mode = 0x100;
    D_80073690.depth.left = 0;
    D_80073690.depth.right = 0;
    D_80073690.delay = 0;
    D_80073690.feedback = 0;
    SpuSetReverbModeParam(&D_80073690);
    SpuSetReverbDepth(&D_80073690);
    func_80018750(&D_800736A4);
    SpuClearReverbWorkArea(D_800736A4);
    func_800551F0();
    func_80018750(&D_800736A4);
    SpuClearReverbWorkArea(D_800736A4);
    for (i = 0; i < 90; i++) {
        VSync(0);
    }
    func_8004BBF0();
    func_80053924();
    func_800320D8();
    func_80032078();
    func_80021770();
}

void func_80016B6C(void) {
    func_80055210();
    func_8001C718();
    D_80067950++;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016BA0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitGeom);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetGeomOffset);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _patch_gte);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80016D0C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuInit);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _SpuInit);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuStart);

INCLUDE_ASM("asm/us/main/nonmatchings/main", OpenEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/main", EnableEvent);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010330);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_init);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80017160);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_FiDMA);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_Fr_);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_t);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_Fw);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_Fr);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_FsetRXX);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_FsetRXXa);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_FgetRXXa);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_FsetPCR);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80017968);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80017990);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_Fw1ts);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DeliverEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _SpuDataCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetReverbModeParam);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _SpuIsInAllocateArea);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _SpuIsInAllocateArea_);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _spu_setReverbAttr);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetReverbDepth);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuClearReverbWorkArea);

INCLUDE_ASM("asm/us/main/nonmatchings/main", WaitEvent);

void func_80018750(s32 *a) {
    *a = D_8006798C;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StartCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StopCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetInitPadFlag);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ReadInitPadFlag);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PAD_init);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StartPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800189CC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80018A44);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80018AAC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitPAD2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StartPAD2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PAD_init2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SysEnqIntRP);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SysDeqIntRP);

INCLUDE_ASM("asm/us/main/nonmatchings/main", EnablePAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DisablePAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _patch_pad);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _remove_ChgclrPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InitCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StartCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StopCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _patch_card_info);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80018CC4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80018CF0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80018D34);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _patch_card);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _patch_card2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _copy_memcard_patch);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _ExitCard);

void func_80018F20(void) {
    PadInitDirect((u8 *)&D_80073940[0], (u8 *)&D_80073940[1]);
    PadStartCom();
}

void func_80018F50(void) {
    s32 i;
    s32 j;

    D_80073720 = 0x8000;
    D_80068158.unk0[0] = 0x5A;
    D_80068158.unk0[1] = 0xA6;
    D_80068158.unk0[2] = 0x5A;
    D_80068158.unk0[3] = 0xA6;
    for (i = 0; i < 2; i++) {
        g_padPortHeld[i] = g_padPortPressed[i] = D_80073988[i] = D_80073998[i] = D_800739A8[i] = 0;
        D_80073930[i] = 0;
        for (j = 0; j < 32; j++) {
            D_80073730[i][j] = 0;
            D_80073830[i][j] = 0;
        }
        func_80019708(i, 0x10, 0x10);
        func_80019708(i, 0x40, 0x40);
        func_80019708(i, 0x80, 0x80);
        func_80019708(i, 0x20, 0x20);
        func_80019708(i, 0x1000, 0x1000);
        func_80019708(i, 0x2000, 0x2000);
        func_80019708(i, 0x4000, 0x4000);
        func_80019708(i, 0x8000, 0x8000);
        func_80019708(i, 0x400, 0x400);
        func_80019708(i, 0x100, 0x100);
        func_80019708(i, 0x800, 0x800);
        func_80019708(i, 0x200, 0x200);
        func_80019708(i, 1, 1);
        func_80019708(i, 8, 8);
        D_80073720 |= 1 << (i * 4);
    }
}

u16 func_80019150(void) {
    return D_80073720;
}

void func_80019160(u16 a, s32 b) {
    if (b) {
        D_80073720 |= a;
    } else {
        D_80073720 &= ~a;
    }
}

void func_80019194(void) {
    s32 i, j;
    s32 state;
    s32 b;
    u32 buttons;

    for (i = 0; i < 2; i++) {
        state = PadGetState(i << 4);
        D_800739A8[i] = state;
        if (state == 0) {
            D_80073720 |= 1 << (i * 4);
        } else if (state == 1) {
            D_80073720 |= 4 << (i * 4);
        }
        if (PADBUF(i)->unk0 == 0 && (((D_80073940[i].unk0 >> 12) & 0xF) == 7 || ((D_80073940[i].unk0 >> 12) & 0xF) == 4)) {
            if ((D_80073720 >> (i * 4)) & 1) {
                if (D_800739A8[i] == 2) {
                    D_80073720 &= ~(1 << (i * 4));
                } else if (D_800739A8[i] == 6) {
                    if (PadInfoMode(i << 4, 2, 0)) {
                        b = 2 << (i * 4);
                        if (!(D_80073720 & b)) {
                            PadSetMainMode(i << 4, 1, 0);
                            D_80073720 |= b;
                        } else {
                            PadSetAct(i << 4, &D_800739B0[i].unk0, 2);
                            PadSetActAlign(i << 4, D_8006815C);
                            D_80073720 = D_80073720 & ~((1 << (i * 4)) | b) & ~(4 << (i * 4));
                        }
                    }
                }
            }
            b = 4 << (i * 4);
            if ((D_80073720 & b) && D_800739A8[i] == 6) {
                PadSetAct(i << 4, &D_800739B0[i].unk0, 2);
                PadSetActAlign(i << 4, D_8006815C);
                D_800739A8[i] = 4;
                D_80073720 &= ~b;
            }
            j = 0;
            buttons = ~PADBUF(i)->unk2;
            buttons &= 0xFFFF;
            D_80073938[i].unk0[0] = PADBUF(i)->unk4[0];
            D_80073938[i].unk0[1] = PADBUF(i)->unk4[1];
            D_80073938[i].unk0[2] = PADBUF(i)->unk4[2];
            D_80073938[i].unk0[3] = PADBUF(i)->unk4[3];
            D_80073930[i] = g_padPortHeld[i];
            g_padPortHeld[i] = 0;
            for (; j < 32; j++) {
                if (buttons & D_80073730[i][j]) {
                    g_padPortHeld[i] |= 1 << j;
                    D_80073830[i][j]++;
                } else {
                    D_80073830[i][j] = 0;
                }
            }
            if ((D_80073720 & 0x8000) && ((D_80073940[i].unk0 >> 12) & 0xF) == 7) {
                if (D_80073938[i].unk0[2] < D_80068158.unk0[2]) {
                    g_padPortHeld[i] |= 0x80;
                }
                if (D_80073938[i].unk0[2] > D_80068158.unk0[3]) {
                    g_padPortHeld[i] |= 0x20;
                }
                if (D_80073938[i].unk0[3] < D_80068158.unk0[0]) {
                    g_padPortHeld[i] |= 0x10;
                }
                if (D_80073938[i].unk0[3] > D_80068158.unk0[1]) {
                    g_padPortHeld[i] |= 0x40;
                }
            }
            g_padPortHeld[i] &= ~D_80073998[i];
            if ((D_80073998[i] & 0x10) && (s8)D_80073938[i].unk0[3] >= 0) {
                D_80073938[i].unk0[3] = 0x80;
            }
            if ((D_80073998[i] & 0x40) && D_80073938[i].unk0[3] > 0x80) {
                D_80073938[i].unk0[3] = 0x80;
            }
            if ((D_80073998[i] & 0x80) && (s8)D_80073938[i].unk0[2] >= 0) {
                D_80073938[i].unk0[2] = 0x80;
            }
            if ((D_80073998[i] & 0x20) && D_80073938[i].unk0[2] > 0x80) {
                D_80073938[i].unk0[2] = 0x80;
            }
            g_padPortPressed[i] = (g_padPortHeld[i] ^ D_80073930[i]) & g_padPortHeld[i];
            D_80073988[i] = (g_padPortHeld[i] ^ D_80073930[i]) & D_80073930[i];
        } else {
            D_80073988[i] = 0;
            g_padPortPressed[i] = 0;
            g_padPortHeld[i] = 0;
        }
    }
}

void func_80019708(s32 a, u32 b, s32 c) {
    s32 n = 31;
    gte_ldlzc(b);
    if (b != 0x80000000) {
        gte_stlzc(&b);
        n -= b;
    }
    D_80073730[a][n] = c;
}

void func_80019750(Unk4 *a) {
    *a = D_80068158;
}

void func_80019774(Unk4 *a) {
    D_80068158 = *a;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadSetAct);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadSetActAlign);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80019854);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001987C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadSetMainMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800199E4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80019A3C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadGetState);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadInfoMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadStartCom);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadStopCom);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PadInitDirect);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padInitDirPort);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80019F28);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80019F90);

void func_8001A084(Unk8001A084 *a) {
    a->unk38 = a->unk37;
    a->unk37 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A094);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A150);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A3CC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A3EC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A4FC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _dirFailAuto);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001A848);

INCLUDE_ASM("asm/us/main/nonmatchings/main", bzero);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padIsVsync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padIntPad);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padInitSioMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001ADB4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padSioRW);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padSioRW2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padClrIntSio0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padWaitRXready);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padSetCmd);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padSendAtLoadInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padRecvAtLoadInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padGetActSize);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padLoadActInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001B80C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001B8B4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padCmdParaMode);

void func_8001BCCC(Unk8001A084 *a) {
    a->unk37 = 0x45;
    a->unk2C = 0;
    a->unk36 = 0;
}

void func_8001BCE0(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x4C;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD00(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x46;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD20(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x47;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD40(Unk8001A084 *a) {
    a->unk37 = 0x4B;
    a->unk2C = 0;
    a->unk36 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padSetRC2wait);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _padChkRC2wait);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001BE14);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001BE5C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001BF34);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001BFE0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001C0FC);

void func_8001C6F8(void) {
    func_8001C720();
}

void func_8001C718(void) {
}

void func_8001C720(void) {
    s32 i;

    D_80073C40.unk0 = 0;
    D_80073C40.unk2 = 0;
    for (i = 15; i >= 0; i--) {
        D_80073C40.unk4[i] = 0;
    }
}

s32 func_8001C750(s32 a, s32 b) {
    D_80073C40.unk4[D_80073C40.unk2] = a;
    D_80073C40.unk2++;
    return 0;
}

s32 func_8001C784(void *arg0) {
    s32 i;

    for (i = D_80073C40.unk2 - 1; i >= 0; i--) {
        if (D_80073C40.unk4[i] == arg0) {
            D_80073C40.unk4[i] = D_80073C40.unk4[D_80073C40.unk2 - 1];
            D_80073C40.unk2--;
            return 0;
        }
    }
    return -1;
}

void *func_8001C7FC(s32 mode, s32 a1, s32 a2) {
    s32 i;
    s32 r = -1;
    void *ret;

    for (i = D_80073C40.unk2 - 1; i >= 0; i--) {
        r = func_8001C8B0((s32)D_80073C40.unk4[i], mode, a1, a2);
        if (r != -1) {
            return (void *)r;
        }
    }
    ret = (void *)-1;
    if (r == -1) {
        r = mode & 1;
        ret = NULL;
        if (r != 0) {
            ret = (void *)-1;
        }
    }
    return ret;
}

s32 func_8001C8B0(s32 arg0, s32 mode, s32 key, s32 arg3) {
    u8 *base = (u8 *)arg0;
    u32 *ent;
    s32 i;
    s32 n;
    s32 j;
    s32 cnt;
    s32 type;
    u8 *blk;
    u16 *ids;
    u32 *ofs;
    u32 *p;
    s32 hi;
    s32 lo;
    s32 r;

    if (base == NULL) {
        return 0;
    }
    ent = (u32 *)(base + 4);
    hi = (key >> 16) & 0xFF;
    n = base[1];
    lo = key & 0xFFFF;
    for (i = 0; i < n; i++, ent++) {
        blk = (u8 *)*ent;
        type = ((u32)blk >> 24) & 0x7F;
        if (type == hi) {
            blk = (u8 *)ent + ((u32)blk & 0xFFFFFF);
            cnt = blk[1];
            if (mode == 0) {
                return (s32)blk;
            }
            ids = (u16 *)(blk + 4);
            if (mode == 1) {
                return cnt;
            }
            ofs = (u32 *)((u8 *)ids + cnt * 2);
            if ((s32)ofs & 3) {
                ofs = (u32 *)((u8 *)ofs - (((s32)ofs & 3) - 4));
            }
            if (mode == 3) {
                return (type << 16) | ids[lo];
            }
            if (mode == 2) {
                p = &ofs[lo];
                return (s32)p + (*p & 0xFFFFFF);
            }
            for (j = 0; j < cnt; j++) {
                if (*ids == lo) {
                    switch (mode) {
                    case 4:
                        do {
                        } while (0);
                        return (s32)ofs + (*ofs & 0xFFFFFF);
                    case 5:
                        return (ofs[1] & 0xFFFFFF) - (ofs[0] & 0xFFFFFF) + 4;
                    case 9:
                        r = func_8001C8B0(arg0, 3, arg3, 0);
                        return r;
                    case 8:
                        r = func_8001C8B0(arg0, 2, arg3, 0);
                        return r;
                    case 11:
                        r = func_8001C8B0(arg0, 1, arg3, 0);
                        return r;
                    default:
                        return -1;
                    }
                }
                ids++;
                ofs++;
            }
        }
    }
    return -1;
}

s32 func_8001CA70(s32 a) {
    u8 *p = (u8 *)a;
    u32 *q;
    u8 *e;
    u32 *r;
    s32 k;
    s32 n;
    s32 m;

    if (p == NULL || p[0] != 0xDB) {
        return -1;
    }
    n = p[1];
    if (n <= 0) {
        return 4;
    }
    q = (u32 *)p + n;
    e = (u8 *)q + (*q & 0xFFFFFF);
    k = e[1];
    n = k * 2 + 4;
    m = n & 3;
    if (m != 0) {
        n += 4 - m;
    }
    r = (u32 *)(e + n + k * 4);
    return (u8 *)r + (*r & 0xFFFFFF) - p;
}

void func_8001CB08(void) {
    GameState *gs;

    func_8001CB78();
    gs = g_gameState;
    func_8001CBD0();
    func_8001CD04(0xE0);
    func_8001DCB8();
    func_8001DE50();
    func_80023918();
    func_80023A2C();
    gs->unk4 = g_allocTop;
}

void func_8001CB70(void) {
}

void func_8001CB78(void) {
    g_allocTop = 0x8008B000;
    g_gameState = (GameState *)0x8008B000;
    g_gameState->unkC = (DISPENV *)0x8008B044;
    g_gameState->unk10 = (DRAWENV *)0x8008B06C;
    g_gameState->unk14 = (GameStateUnk14 *)0x8008B124;
    g_gameState->unk18 = (void *)0x8008B214;
    g_allocTop = 0x8008B250;
}

void func_8001CBD0(void) {
    GameState *gs = g_gameState;
    s32 i;

    gs->unk0 = 0x100000;
    gs->unkB = 0xFF;
    gs->unk4 = 0;
    gs->mode = 0;
    gs->unkA = 0;
    gs->unk20 = -1;
    gs->unk1C = NULL;
    func_8001CCB4(0, 0, 0);
    func_8001CD04(0xE0);
    gs->unk9 = 0;
    for (i = 14; i >= 0; i--) {
        ((DirInfo *)gs->unk14)[i].unk0 = i;
        ((DirInfo *)gs->unk14)[i].unk4 = 0;
        ((DirInfo *)gs->unk14)[i].unk8 = 0;
        ((DirInfo *)gs->unk14)[i].unkC = 0;
        gs->unk18[i] = NULL;
    }
    gs->unk30 = 0;
    for (i = 0; (u32)i < 15; i++) {
        gs->unk34[i] = 0;
    }
}

void func_8001CCB4(s32 r, s32 g, s32 b) {
    DRAWENV *env;

    env = g_gameState->unk10;
    env[1].r0 = r;
    env[0].r0 = r;
    env = g_gameState->unk10;
    env[1].g0 = g;
    env[0].g0 = g;
    env = g_gameState->unk10;
    env[1].b0 = b;
    env[0].b0 = b;
}

void func_8001CD04(s32 h) {
    Unk4 func_8002F9A8(void);
    Unk4 col;
    RECT r;
    s32 fade;
    s32 busy;
    DRAWENV *draw;

    fade = 0;
    if (func_8002F800() || func_8002F810()) {
        fade = 1;
    }
    busy = func_8002F864();
    col = func_8002F9A8();
    if (busy == 1) {
        col.unk0[0] = 0xFF - col.unk0[0];
        col.unk0[1] = 0xFF - col.unk0[1];
        col.unk0[2] = 0xFF - col.unk0[2];
    }
    if (!fade) {
        col.unk0[0] = g_gameState->unk10->r0;
        col.unk0[1] = g_gameState->unk10->g0;
        col.unk0[2] = g_gameState->unk10->b0;
    }
    if (g_gameState->unkC->disp.h < h) {
        r.x = 0;
        r.y = g_gameState->unkC->disp.h;
        r.w = 0x280;
        r.h = h - g_gameState->unkC->disp.h;
        ClearImage(&r, col.unk0[0], col.unk0[1], col.unk0[2]);
        DrawSync(0);
    }
    SetDefDispEnv(&g_gameState->unkC[0], 0x140, 0, 0x140, h);
    SetDefDispEnv(&g_gameState->unkC[1], 0, 0, 0x140, h);
    g_gameState->unkC[0].isinter = g_gameState->unkC[1].isinter = 0;
    g_gameState->unkC[0].screen.x = g_gameState->unkC[1].screen.x = 0;
    g_gameState->unkC[0].screen.y = g_gameState->unkC[1].screen.y = (0xF0 - h) / 2;
    g_gameState->unkC[0].screen.h = g_gameState->unkC[1].screen.h = h;
    SetDefDrawEnv(&g_gameState->unk10[0], 0, 0, 0x140, h);
    SetDefDrawEnv(&g_gameState->unk10[1], 0x140, 0, 0x140, h);
    draw = g_gameState->unk10;
    draw[1].dfe = 1;
    draw[0].dfe = 1;
    draw = g_gameState->unk10;
    draw[1].dtd = 1;
    draw[0].dtd = 1;
    draw = g_gameState->unk10;
    draw[1].isbg = 1;
    draw[0].isbg = 1;
    VSync(0);
    if (g_gameState->unk1C != NULL) {
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    }
}

void func_8001CFB4(void) {
    Unk4 func_8002F9A8(void); /* ovl_0a declares it with an out-pointer */
    Unk4 col;
    s32 fade;
    s32 busy;

    if (g_gameState->unk1C != NULL) {
        VSync(0);
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
        fade = 0;
        memmove(&D_80073C88, &g_gameState->unk10[g_gameState->unk1C->unk8 ^ 1], sizeof(DRAWENV));
        if (func_8002F800() || func_8002F810()) {
            fade = 1;
        }
        busy = func_8002F864();
        col = func_8002F9A8();
        if (fade && !busy) {
            D_80073C88.isbg = 1;
            D_80073C88.r0 = col.unk0[0];
            D_80073C88.g0 = col.unk0[1];
            D_80073C88.b0 = col.unk0[2];
        } else {
            D_80073C88.isbg = 0;
        }
        PutDrawEnv(&D_80073C88);
        DrawSync(0);
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", memmove);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDefDrawEnv);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDefDispEnv);

void func_8001D268(void) {
    GameData *gd;

    g_allocTop = g_gameState->unk4;
    func_8001D32C();
    gd = g_gameState->unk1C;
    func_8001D3C4();
    PutDispEnv(&g_gameState->unkC[gd->unk8]);
    func_80021C60();
    func_8001D768();
    func_8001D610();
    func_8001D67C();
    gd->unk4 = g_allocTop;
}

void func_8001D30C(void) {
    func_8001D848();
}

void func_8001D32C(void) {
    GameData *gd;
    u8 *q;
    s32 i;
    s32 w;

    g_gameState->unk1C = (GameData *)g_allocTop;
    gd = g_gameState->unk1C;
    q = (u8 *)(gd + 1);
    for (i = 0; i < 2; i++) {
        gd->unk10[i] = (u32 *)(q + 0x20);
        q += 0x4040;
    }
    gd->unk8D8 = (void *)(q + 0x558);
    gd->unk8D4 = (void *)q;
    gd->unk7E0 = (s32)(q + 0x60C);
    g_allocTop = gd->unk7E0 + 0x8000;
    gd->unk8C8 = (void *)g_allocTop;
    g_allocTop += 0x120;
    gd->unk814 = (void *)g_allocTop;
    w = g_allocTop + 0x6800;
    gd->unk818 = w;
    gd->unk828 = (void *)(w + 0x2000);
    gd->unk82C = (void *)(w + 0x2800);
    g_allocTop = w + 0x3000;
}

void func_8001D3C4(void) {
    GameData *gd = g_gameState->unk1C;
    s32 i;

    gd->unkA = 0x200;
    gd->unk18.m[0][0] = gd->unk18.m[1][1] = gd->unk18.m[2][2] = 0x1000;
    gd->unk3A = 1;
    gd->unkE = 0x235;
    gd->unk848 = -1;
    gd->unk7DC = 0x2331;
    gd->unk7DE = 0x44;
    gd->unk0 = 0;
    gd->unk4 = 0;
    gd->unk8 = 0;
    gd->unk9 = 0;
    gd->unk38 = 0;
    gd->unk39 = 0;
    gd->unk18.t[0] = gd->unk18.t[1] = gd->unk18.t[2] = 0;
    gd->unk18.m[0][1] = gd->unk18.m[0][2] = gd->unk18.m[1][0] = gd->unk18.m[1][2] = gd->unk18.m[2][0] =
        gd->unk18.m[2][1] = 0;
    gd->fieldId = 0;
    gd->unk7D4 = NULL;
    gd->unk84C = NULL;
    gd->unk80C = NULL;
    gd->unk810 = NULL;
    gd->unk844 = 0;
    gd->unk851 = 0;
    gd->unk8C4 = 0;
    gd->unk852 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->unk81C[i] = 0xFFFF;
        gd->unk820[i] = 0xFFFF;
        gd->unk824[i] = 0;
        gd->unk826[i] = 0x7F;
    }
    gd->unk842 = 0x1E;
    gd->unk7E8 = 0xFFFF;
    gd->unk840 = 0xFFFF;
    gd->unk7EA = 0xFFFF;
    gd->unk850 = 0;
    gd->unk8A8 = -1;
    gd->unk84A = 0xFFFF;
    gd->unk8AC = -1;
    gd->unk8B0 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->unk830[i] = 0xFFFF;
        (&gd->unk834)[i] = 0xFFFF;
        gd->unk8B8[i] = 0x7F;
        gd->unk8BA[i] = 0x80;
        gd->unk8B4[i] = 0xFFFF;
    }
    gd->unk838 = NULL;
    gd->unk83C = NULL;
    for (i = 8; i >= 0; i--) {
        func_8001E3A4(&gd->unk8D8[i]);
        gd->unk8D8[i].unk8 = (struct BtlUnit9C *)&gd->unk8D4[i];
    }
    gd->unk8DC = NULL;
    gd->unk8E0 = NULL;
    gd->unk7D8 = NULL;
    for (i = 1; i >= 0; i--) {
        gd->unk7EC[i] = -1;
        gd->unk7F4[i] = -1;
        gd->unk7FC[i] = -1;
        gd->unk804[i] = -1;
    }
    gd->unk7E4 = (struct FileTable *)-1;
    gd->unk8C0 = 0;
    gd->unk878 = 0;
    gd->unk87C = 0;
    memset(&gd->unk880, 0, 0x28);
    gd->unk8B2 = 0;
    gd->unk8BC = NULL;
    gd->unk8E4.ovl65 = NULL;
    for (i = 0; i >= 0; i--) {
        gd->unk849[i] = 0;
    }
    for (i = 7; i >= 0; i--) {
        gd->unk8CC[i] = 0;
    }
}

s32 func_8001D610(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x90084, 0);
    s32 size = (s32)func_8001C7FC(5, 0x90084, 0);

    func_80055DF4();
    func_80055E14(addr, size, 1);
    return 0;
}

s32 func_8001D67C(void) {
    GameData *gd = g_gameState->unk1C;
    u16 ids[8] = { 0, 0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716 };
    u32 i;
    s32 addr;

    gd->unk7D4 = func_8001C7FC(4, 0xD0000, 0);
    for (i = 0; i < 8; i++) {
        addr = (s32)func_8001C7FC(4, ids[i] | 0x40000, 0);
        if (addr != 0) {
            func_8001D8A8(addr);
            DrawSync(0);
        }
    }
    return 0;
}

s32 func_8001D768(void) {
    FileEntry *f = func_8001E218(1, 0x271A);

    func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)g_allocTop, 0,
                  -1, NULL);
    while (func_800217C4(g_gameState->unk1C->unk854)) {
    }
    g_gameState->unk1C->unk7D8 = (void *)g_allocTop;
    func_8001C750((s32)g_gameState->unk1C->unk7D8, 0);
    g_allocTop = func_8001C8B0((s32)g_gameState->unk1C->unk7D8, 0, 0x90000, 0);
    return 0;
}

s32 func_8001D848(void) {
    func_8001C784(g_gameState->unk1C->unk7D8);
    g_gameState->unk1C->unk7D8 = 0;
    return 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", memset);

void func_8001D8A8(s32 addr) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *b = &tim->clut;

    if (tim->flag & 8) {
        LoadImage((RECT *)&tim->clut.x, (u32 *)(&tim->clut + 1));
        b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
    }
    LoadImage((RECT *)&b->x, (u32 *)(b + 1));
}

void func_8001D910(TimHeader *tim, RECT **clut, RECT **image) {
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            if (clut != NULL) {
                *clut = (RECT *)&tim->clut.x;
            }
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        if (image != NULL) {
            *image = (RECT *)&b->x;
        }
    }
}

void func_8001D960(TimHeader *tim, s32 *cx, s32 *cy, s32 *px, s32 *py) {
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            *cx = tim->clut.x;
            *cy = tim->clut.y;
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        *px = b->x;
        *py = b->y;
    }
}

void func_8001D9CC(s32 addr, s32 cx, s32 cy, s32 px, s32 py) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            tim->clut.x = cx;
            tim->clut.y = cy;
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        b->x = px;
        b->y = py;
    }
}

u32 func_8001DA18(u32 a) {
    return (a >> 7) & 3;
}

u32 func_8001DA24(u32 a) {
    return (a >> 5) & 3;
}

s32 func_8001DA30(s32 a) {
    return (a << 6) & 0x3C0;
}

s32 func_8001DA3C(s16 arg0) {
    return ((arg0 << 4) & 0x100) | (s16)((arg0 >> 2) & 0x200);
}

s32 func_8001DA60(s32 a) {
    return (a & 0x3F) << 4;
}

s32 func_8001DA6C(s32 a) {
    return (s16)a >> 6;
}

void func_8001DA78(u8 **arg0, s32 arg1) {
    *arg0 += arg1 + 0xC;
    if (arg1 & 3) {
        *arg0 += 4;
        *arg0 -= arg1 & 3;
    }
}

s32 func_8001DAA4(s32 dst, s32 src, s32 size, RECT *rc) {
    s32 done = 0;
    RECT *r;
    u32 left;
    u32 h;
    u32 n;
    u32 i;

    if (size != 0) {
        while (done < size) {
            left = size - done;
            i = D_80068234;
            D_80068234 = i + 1;
            r = &D_80073CE8[i];
            if (i >= 5) {
                D_80068234 = 0;
            }
            if (rc != NULL) {
                r->x = rc->x;
                r->y = rc->y;
                r->w = rc->w;
                r->h = rc->h;
            } else {
                r->x = g_gameState->unk10[g_gameState->unk1C->unk8].clip.x;
                r->y = g_gameState->unk10[g_gameState->unk1C->unk8].clip.y;
                r->w = 320;
                r->h = 218;
            }
            h = left >> 1;
            n = h / r->h;
            if (n == 0 && left != 0) {
                n = 1;
                r->h = h;
                if ((s16)h == 0) {
                    r->h = h + 1;
                }
            } else if (r->w < (s32)n) {
                n = r->w;
            }
            r->w = n;
            LoadImage(r, (u32 *)(src + done));
            while (func_80022B18() != 0) {
            }
            StoreImage(r, (u32 *)(dst + done));
            done += r->w * r->h * 2;
        }
    }
    return done;
}

void func_8001DCB8(void) {
    CdlFILE fp;
    char *names[4] = { "\\SLUS_012.51;1", "\\SLUS_012.95;1", "\\SLUS_012.96;1", "\\SLUS_012.97;1" };
    s32 i;
    Ff9ImgHeader *hdr;

    g_gameState->unkB = 0xFF;
    for (i = 0; i < ARRAY_COUNT(names); i++) {
        if (CdSearchFile(&fp, names[i])) {
            g_gameState->unkB = 1 << (i % 4);
            break;
        }
    }
    if (g_gameState->unkB != 0xFF) {
        while (func_80022B18()) {
        }
        while ((g_gameState->unk20 = func_80022AC4("\\FF9.IMG;1")) == -1) {
            while (func_80022B18()) {
            }
        }
        while (func_80022B18()) {
        }
        while (func_80022B18()) {
        }
        hdr = (Ff9ImgHeader *)((u8 *)g_gameState - 0x1800);
        func_80022C60(g_gameState->unk20, 0x800, hdr, NULL);
        while (func_80022B18()) {
        }
        g_gameState->unk30 = hdr->unk8;
        memmove(g_gameState->unk14, hdr->unk10, g_gameState->unk30 * sizeof(DirInfo));
    }
}

void func_8001DE50(void) {
    GameState *gs;
    u8 *buf;
    DirInfo *dir;
    s32 base;
    s32 size;
    s32 cnt;
    s32 t;
    u8 *top;
    u8 *data;
    u8 *list8;
    u8 *list11;
    Unk8001DE50 *s;
    Unk8001DE50 *e;
    Unk8001DE50b *dst;
    u16 key;
    s32 key2; /* s32, unlike key: no zero_extend in the second inner loop, so loop.c can still hoist the constant 2 */

    gs = g_gameState;
    if (!(gs->unk0 & 0x100000)) {
        return;
    }
    cnt = 0;
    buf = D_80067944 + 0x14C700;
    size = (((DirInfo *)gs->unk14)[13].unk8 << 11) + ((DirInfo *)gs->unk14)[13].unk4 * 2;
    base = ((DirInfo *)gs->unk14)[0].unk8;
    t = size & 0x800;
    if (t != 0) {
        size += 0x800 - t;
    }

    func_80022C60(gs->unk20, size, buf, NULL);
    while (func_80022B18() != 0) {
    }
    dir = (DirInfo *)(buf + 0x10);
    buf += base << 11;
    do { } while (0); /* empty macro residue: ends cse1's block so the data copy below is kept */

    size = (dir[0].unk4 + 1) * 8;
    /* data == buf; the redundant +/- keeps CSE from folding the copy (the
       original passes a copy of buf in s0 to the first two memmoves) */
    data = buf + (base << 11) - (base << 11);
    top = (u8 *)g_gameState - size;
    gs->unk18[0] = top;
    memmove(gs->unk18[0], data, size);

    size = (dir[1].unk4 + 1) * 8;
    top -= size;
    gs->unk18[1] = top;
    memmove(gs->unk18[1], data + ((dir[1].unk8 - base) << 11), size);

    size = (dir[10].unk4 + 1) * 8;
    top -= size;
    gs->unk18[10] = top;
    memmove(gs->unk18[10], buf + ((dir[10].unk8 - base) << 11), size);

    size = (dir[8].unk4 + 1) * 8;
    top -= size;
    gs->unk18[8] = top;
    memmove(gs->unk18[8], buf + ((((DirInfo *)gs->unk14)[8].unk8 - base) << 11), size);

    list8 = buf + ((dir[8].unk8 - base) << 11);
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    list11 = buf + ((dir[11].unk8 - base) << 11);
    do {
        key = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key) {
                if (e->unk2 == 0) {
                    e->unk2 = 1;
                    cnt++;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    size = cnt * 0x10;
    top -= size;
    gs->unk18[11] = top;
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    dst = gs->unk18[11];
    do {
        key2 = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key2) {
                if (e->unk2 == 1) {
                    memmove(dst, e, 0x10);
                    dst->unk8 = 0xFFFF;
                    dst++;
                    e->unk2 = 2;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    ((DirInfo *)gs->unk14)[11].unk4 = cnt * 2;

    size = (dir[12].unk4 + 1) * 8;
    top -= size;
    gs->unk18[12] = top;
    memmove(gs->unk18[12], buf + ((dir[12].unk8 - base) << 11), size);

    size = dir[13].unk4 * 2;
    top -= size;
    gs->unk18[13] = top;
    memmove(gs->unk18[13], buf + ((dir[13].unk8 - base) << 11), size);
    gs->unk0 &= ~0x100000;
}

FileEntry *func_8001E218(s32 a, s32 id) {
    FileDirEntry *d = (FileDirEntry *)g_gameState->unk14 + a;
    FileIdEntry *base;
    FileIdEntry *e;

    if (d->unk0 == 3) {
        return (FileEntry *)((u16 *)g_gameState->unk18[13] + id);
    }
    id &= 0xFFFF;
    base = g_gameState->unk18[a];
    e = base + d->unk4 - 1;
loop:
    if (e->unk0 == id) {
        return (FileEntry *)e;
    }
    e--;
    if (e >= base) {
        goto loop;
    }
    return NULL;
}

void func_8001E2AC(u16 arg0) {
    switch (g_gameState->mode) {
    case 1:
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unkA = arg0;
        break;
    case 2:
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk4 = arg0;
        break;
    case 3:
        g_gameState->unk1C->unk8E4.ovl65->unk8->unk24 = arg0;
        break;
    }
}

s32 func_8001E350(s32 arg0) {
    if (arg0 < 9) {
        return (s32)&g_gameState->unk1C->unk8D8[arg0];
    }
    return (s32)&g_gameState->unk1C->unk8E0[arg0 - 9];
}

void func_8001E3A4(Unk8001E3A4 *a) {
    a->unk0 = 0;
    a->unk4 = -1;
    a->unk8 = 0;
    a->unkC = 0;
    a->unk10 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdSearchFile);

s32 func_8001E698(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001E6B8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001E97C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001EA20);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001ECBC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", strcmp);

INCLUDE_ASM("asm/us/main/nonmatchings/main", strncmp);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800105B4);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800105B8);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800105BC);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800105D8);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010738);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010748);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8001ED50);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_sync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_ready);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_cw);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_vol);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_flush);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_initvol);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_initintr);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_init);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_datasync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_set_test_parmnum);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800201F4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdIntToPos);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdControl);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdControlF);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdControlB);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800207A0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800207D4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80020A48);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80020B18);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdReadBreak);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdRead);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdReadSync);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdInit);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800211D0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8002120C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021234);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8002125C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdPosToInt);

u8 func_80021310(void) {
    return D_8006825C;
}

s32 func_80021320(void) {
    return D_8006826C;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021330);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021340);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021360);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021380);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800213A0);

s32 func_800213C0(void *a, s32 b) {
    return CD_getsector(a, b) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_getsector);

s32 func_800214E0(void *a, s32 b) {
    return CD_getsector2(a, b) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CD_getsector2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdDataCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80021620);

void func_80021640(s32 a) {
    func_8001E2AC(a);
}

void func_80021660(u16 a) {
    func_8001E2AC(a);
}

void func_80021680(a)
    s16 a;
{
    g_gameState->unk1C->unk8B2 = a;
}

s32 func_80021698(Unk8004AB64 *a) {
    return g_gameState->unk1C->unk838(a->unk34 | 0x20000, a);
}

s32 func_800216D8(a, b)
    s32 a;
    u16 b;
{
    return ((u16 *)func_8001C7FC(4, b | 0x30000, 0))[1];
}

s32 func_8002170C(Unk8004AB64 *p) {
    if (g_gameState->mode == 3) {
        goto ret0;
    }
    if (p->unk5 != 4) {
        return 0;
    }
    if (p->unk34 == 0xFFFF) {
    ret0:
        return 0;
    }
    return func_800C44F4(p->unk3B);
}

void func_80021770(void) {
    D_80071E32 = 0;
}

CdReadCtx *func_8002177C(void *arg0, s32 a, s32 b, void *c, s32 d, s32 e, void (*f)(CdReadCtx *)) {
    CdReadCtx *p = arg0;
    if (p != NULL) {
        p->unk0 = a;
        p->unk4 = b;
        p->unk8 = c;
        p->unkC = d;
        p->unk20 = c;
        p->unk10 = e;
        p->unk14 = f;
        p->unk18 = b;
        p->unk1C = 1;
    }
    return p;
}

s32 func_800217C4(void *arg0) {
    CdReadCtx *p = arg0;
    s32 n;

    if (p != NULL) {
        switch (p->unk1C) {
        case 1:
            if (func_80022B18() == 0) {
                p->unk1C = 2;
            }
            break;
        case 2:
            n = p->unk4 - p->unkC;
            if (p->unk10 != -1 && p->unk10 < n) {
                n = p->unk10;
            }
            if (n != 0) {
                func_80022C60(g_gameState->unk20 + p->unk0 + p->unkC / 2048, n, p->unk8, NULL);
            }
            p->unk10 = n;
            p->unk1C = 3;
            p->unk20 = p->unk8 + n;
            return 3;
        case 3:
            if (func_80022B18() == 0) {
                if (p->unk14 != NULL) {
                    p->unk14(p);
                }
                p->unk1C = 0;
            }
            break;
        default:
            return 0;
        }
        return p->unk1C;
    }
    return 0;
}

s32 func_80021910(Unk80021910 *a) {
    if (a->unk1C >= 3) {
        return D_800761D0.unkC;
    }
    return 0;
}

void func_80021938(Unk80021938 *e) {
    func_80021974(e, 0, -1, 0, 0, 0, -1, -1);
}

void func_80021974(Unk80021938 *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 t;
    p->unk0 = 0;
    p->unk2 = a;
    p->unk4 = b;
    p->unk8 = c;
    p->unkC = 0;
    t = d;
    p->unk10 = t / 2048 * 2048;
    p->unk14 = e;
    p->unk18 = t = f;
    p->unk1C = g;
    p->unk20 = 0;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk26 = 0;
}

void func_800219D8(Unk800219D8 *a, s16 b, s16 c, s16 d, s16 e) {
    a->unk20 = b;
    a->unk22 = c;
    a->unk24 = d;
    a->unk0 |= 0x20;
    a->unk26 = e;
}

s32 func_800219FC(Unk80021938 *a) {
    s32 r = 0;
    if (a != NULL) {
        r = a->unk4 != -1;
    }
    return r;
}

s32 func_80021A1C(Unk80021938 *a) {
    return a->unk0 & 1;
}

s32 func_80021A28(Unk80021938 *p) {
    u16 f;
    if (p->unk2 == 0) {
        return func_80021A8C(p);
    }
    if (p->unk2 != 1) {
        return 0;
    }
    f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        return 1;
    }
    return 0;
}

s32 func_80021A8C(Unk80021938 *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1) && p->unk2 == 0 && !(f & 0x20) && p->unk8 != 0) {
        return 1;
    }
    return 0;
}

s32 func_80021AE0(Unk80021938 *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        if ((f & 0x20) && p->unk24 != 0 && p->unk26 != 0) {
            return 1;
        }
    }
    return 0;
}

Unk80021938 *func_80021B34(Unk80021938 *dst, Unk80021938 *src) {
    s32 len;
    s32 t;

    if (func_80021A8C(src)) {
        func_80021938(dst);
    } else {
        func_80021938(dst);
        dst->unk2 = src->unk2;
        dst->unk4 = src->unk4;
        t = src->unk8;
        t += src->unkC;
        dst->unk8 = t;
        len = src->unk8 + src->unk10 - t;
        dst->unk10 = len;
        t = src->unk14;
        if (len < t) {
            t = len;
        }
        dst->unk14 = t;
        dst->unk14 = t / 2048 * 2048;
        t = dst->unk14;
        dst->unk18 = src->unk18 + (src->unkC >> 11);
        dst->unk1C = dst->unk14;
    }
    return dst;
}

s32 func_80021C20(Unk80021938 *e) {
    s32 r = 0;
    if (e->unk0 & 1) {
        while (func_80022D3C(NULL) == 0) {
        }
        r = 1;
    }
    return r;
}

void func_80021C60() {
    s32 i;
    Unk8C8 *p;

    p = g_gameState->unk1C->unk8C8;
    p->unk1 = 4;
    p->unk0 = 0;
    p->unk2 = 0;
    p->unk3 = -1;
    for (i = 5; i >= 0; i--) {
        func_80021938(&p->unk4[i]);
    }
    func_80021938(&p->unk4[6]);
    p->unk11C = 0;
}

void func_80021CDC(s32 a) {
    g_gameState->unk1C->unk8C8->unk11C = a;
}

Unk80021938 *func_80021CFC(void) {
    Unk8C8 *t = g_gameState->unk1C->unk8C8;
    Unk80021938 *e = NULL;
    if (t->unk2 < 6) {
        e = &t->unk4[t->unk2];
        func_80021938(e);
        t->unk2++;
    }
    return e;
}

Unk80021938 *func_80021D7C(void) {
    Unk8C8 *p = g_gameState->unk1C->unk8C8;
    Unk80021938 *e;

    while (p->unk3 < p->unk2 - 1) {
        p->unk3++;
        e = &p->unk4[p->unk3];
        if (e->unk4 != -1) {
            e->unk0 |= 0x40;
            return e;
        }
    }
    return NULL;
}

Unk80021938 *func_80021E20(void) {
    Unk8C8 *p = g_gameState->unk1C->unk8C8;
    return &p->unk4[p->unk3];
}

Unk80021938 *func_80021E5C(s32 id) {
    Unk8C8 *t = g_gameState->unk1C->unk8C8;
    s32 i;
    id &= 0xFFFFFF;
    for (i = t->unk2 - 1; i >= 0; i--) {
        Unk80021938 *e = &t->unk4[i];
        if ((e->unk4 & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

s32 func_80021ED4(void) {
    Unk80021938 *e = &g_gameState->unk1C->unk8C8->unk4[6];
    Unk80021938 *p = func_80021E20();
    u8 r;
    s32 ret;

    if (p == NULL) {
        return 0;
    }
    do {
        if (!func_800219FC(p)) {
            return -1;
        }
        if (p->unk2 == 0) {
            r = func_80022C60(g_gameState->unk20 + e->unk18, e->unk1C, (void *)e->unk8, func_800225BC);
        } else if (p->unk2 == 1) {
            r = func_80022C10(g_gameState->unk20 + e->unk18, func_800225BC);
        } else {
            goto done;
        }
        if (r == 0) {
            return -1;
        }
    done:
        p->unk0 |= 1;
        ret = 0;
    } while (0);
    return ret;
}

s32 func_80021FB4(void) {
    Unk80021938 *p = func_80021E20();
    s32 t;

    if (p == NULL) {
        return 0;
    }
    if (func_800219FC(p) == 0) {
        return -1;
    }
    if (p->unk0 & 1) {
        if (p->unk0 & 0x80) {
            return -1;
        }
        t = p->unk0 & 2;
        return t == 0;
    }
    return 0;
}

void func_80022020(void) {
    while (func_80021FB4() > 0) {
    }
    func_80022050();
}

void func_80022050(void) {
    Unk80021938 *e = func_80021E20();
    if (e != NULL) {
        e->unk0 &= 0xFF7E;
    }
}

Unk80021938 *func_8002208C(void) {
    Unk80021938 *p = func_80021E20();
    if (func_800219FC(p) == 0) {
        return NULL;
    }
    if (func_80021A1C(p) != 0) {
        func_80021C20(p);
    }
    while (func_80021FB4() > 0) {
    }
    return p;
}

s32 func_800220E8(s32 id) {
    s32 r = 0;
    Unk80021938 *p = func_80021E5C(id);

    if (p != NULL) {
        if (func_80021A1C(p)) {
            func_80022020();
        }
        if (func_80021AE0(p) || func_80021A8C(p)) {
            r = 1;
        }
    } else {
        p = func_80021E20();
        if (p != NULL && func_800219FC(p)) {
            if (((p->unk4 >> 16) & 0xFF) == ((id >> 16) & 0xFF) && (p->unk4 & 0xFFFF) != (id & 0xFFFF)) {
                func_80021C20(p);
            }
        }
    }
    return r;
}

s32 func_800221C0(u32 addr, s32 size) {
    Unk8C8 *c = g_gameState->unk1C->unk8C8;
    s32 i;
    Unk80021938 *e;
    u32 end;
    s32 n;

    end = addr + size;
    for (i = c->unk2 - 1; i >= 0; i--) {
        e = &c->unk4[i];
        if (e->unk4 != -1 && e->unk2 == 0) {
            if (e->unk1C < e->unk10) {
                n = e->unk1C;
            } else {
                n = e->unk10;
            }
            if (e->unk8 < end && addr < e->unk8 + n) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80022280(u32 addr, s32 size) {
    Unk8C8 *q = g_gameState->unk1C->unk8C8;
    Unk80021938 *e;
    s32 ret = 0;
    s32 i;
    u32 end = addr + size;

    for (i = q->unk2 - 1; i >= 0; i--) {
        e = &q->unk4[i];
        if (e->unk4 != -1 && (*(u32 *)&e->unk0 & 0x60) == 0x40 && e->unk8 != 0 && e->unkC != 0 &&
            e->unk8 < end && addr < e->unk8 + e->unkC) {
            if (func_80021C20(e)) {
                ret = 1;
            }
            func_80021938(e);
        }
    }
    return ret;
}

void func_80022390(s32 id) {
    Unk80021938 *e = func_80021E5C(id);
    if (e != NULL) {
        func_80021C20(e);
        func_80021938(e);
    }
}

void func_800223CC(void) {
    Unk8C8 *p = g_gameState->unk1C->unk8C8;
    s32 i;

    for (i = p->unk2 - 1; i >= 0; i--) {
        func_80021C20(&p->unk4[i]);
        func_80021938(&p->unk4[i]);
    }
    p->unk1 = 4;
    p->unk2 = 0;
    p->unk3 = -1;
}

s32 func_8002246C(void) {
    Unk8C8 *t = g_gameState->unk1C->unk8C8;
    Unk80021938 *e;

    if (t->unk11C != NULL && t->unk11C(t) != 0) {
        return 1;
    }
    for (;;) {
        switch (t->unk1) {
        case 4:
            if (func_80021D7C() != NULL) {
                t->unk1 = 5;
            } else {
                t->unk1 = 0;
            }
            break;
        case 5:
            e = func_80021E20();
            if (func_800219FC(e) == 0 || func_80021A28(e) != 0) {
                t->unk1 = 4;
                break;
            }
            func_80021B34(&t->unk4[6], e);
            t->unk1 = 8;
            break;
        case 8:
            if (func_80022B18() != 0) {
                return 0;
            }
            t->unk1 = 9;
            return 0;
        case 9:
            if (func_80021ED4() == -1) {
                t->unk1 = 5;
                return 0;
            }
            t->unk1 = 10;
            return 0;
        case 10:
            if (func_80021FB4() > 0) {
                return 0;
            }
            func_80022050();
            t->unk1 = 5;
            return 0;
        case 0:
        default:
            return 0;
        }
    }
}

void func_800225BC(u8 *arg0) {
    Unk8C8 *p = g_gameState->unk1C->unk8C8;
    Unk80021938 *e;

    if (p != NULL) {
        e = func_80021E20();
        if (e != NULL) {
            if (func_800219FC(e) && *arg0 == 0) {
                Unk80021938 *x = &p->unk4[6];
                if (e->unk2 == 0) {
                    e->unkC += x->unk1C;
                }
                e->unk0 |= 2;
            } else {
                e->unk0 |= 0x80;
            }
        }
    }
}

void func_8002267C(CdRequest *r) {
    volatile s32 st = D_800761D0.unk0;
    u32 n;

    D_800761D0.unk2 = 0;
    D_800761D0.unk3 = 0;
    D_800761D0.unk944 = *r;
    D_800761D0.unk0 = 1;
    n = (u32)(r->unk8 + 0x7FF) >> 11;
    D_800761D0.unk14 = 0;
    D_800761D0.unk10 = 0;
    D_800761D0.unkC = 0;
    D_800761D0.unk4 = n;
    func_800213A0(NULL);
    func_80021380(r->unk10);
    if (r->unk0 & 1) {
        D_800761D0.unk0 = 1;
        CdIntToPos(r->unk4, &D_800761D0.unk8);
        func_80023580(2, (u8 *)&D_800761D0.unk8);
    } else if (st != 4) {
        D_800761D0.unk0 = 4;
        func_80023580(9, NULL);
    } else {
        D_800761D0.unk0 = 0;
        if (D_800761D0.unk944.unk18 != NULL) {
            D_800761D0.unk944.unk18(&D_800761D0);
        }
    }
}

void func_800227C8(u8 intr, u8 *result) {
    u8 err = 0;
    u32 pos[3];
    s32 prev;
    s32 n;

    if (result != NULL) {
        D_80076B0C = *(Unk8 *)result;
    }
    if (intr == 1) {
        func_800213C0(pos, 3);
        n = CdPosToInt((CdlLOC *)pos);
        prev = D_800761D0.unk18;
        if (prev == 0) {
            if (n != D_800761D0.unk944.unk4) {
                err = 1;
            }
        } else {
            if (n != prev + 1) {
                err = 1;
            }
        }
        if (err == 0) {
            D_800761D0.unk18 = n;
            func_8002291C();
            if (D_800761D0.unk4 == 0 || D_800761D0.unk2 != 0) {
                func_800213A0(0);
                D_800761D0.unk0 = 4;
                func_80023580(9, 0);
            }
        }
    } else {
        err = 1;
    }
    if (err != 0) {
        func_800213A0(0);
        D_800761D0.unk0 = 4;
        D_800761D0.unk3 = 1;
        D_800761D0.unk1++;
        func_80023580(9, 0);
    }
}

void func_8002291C(void) {
    CdChunk *c;

    if (D_800761D0.unk944.unkC == (CdChunk *)-1) {
        c = &D_800761D0.unk944.unk1C;
    } else {
        c = D_800761D0.unk944.unkC;
    }
    if (c[D_800761D0.unk14].unk4 != NULL) {
        func_800213C0((u32 *)c[D_800761D0.unk14].unk4 + D_800761D0.unk10 / 4, 0x200);
    }
    D_800761D0.unk10 += 0x800;
    D_800761D0.unkC += 0x800;
    D_800761D0.unk4--;
    if (D_800761D0.unk10 >= c[D_800761D0.unk14].unk0) {
        D_800761D0.unk10 = 0;
        D_800761D0.unk14++;
        if (D_800761D0.unk4 != 0) {
            while (c[D_800761D0.unk14].unk0 == 0) {
                D_800761D0.unk14++;
            }
        }
    }
}

s32 func_80022A28(void) {
    s32 v = 1;
    s32 ok;

    while (CdInit() == 0) {
    }
    ok = v;
    if (ok) {
        func_800236A4(0);
        v = 0x80; /* CdlModeSpeed */
        CdControlB(0xE, (u8 *)v, 0);
        VSync(3);
    }
    return ok;
}

void func_80022A80(void) {
    CdReset(1);
    func_80021380(0);
    func_800213A0(NULL);
    memset(&D_800761D0, 0, 0x98C);
}

s32 func_80022AC4(char *name) {
    CdlFILE f;
    s32 r;
    if (CdSearchFile(&f, name) != NULL) {
        r = CdPosToInt(&f.pos);
    } else {
        printf("Cannot find file. (%s)\n", name);
        r = -1;
    }
    return r;
}

s32 func_80022B18(void) {
    return D_800761D0.unk0;
}

s32 func_80022B28(void) {
    u8 buf[8];
    s32 r = D_800761D0.unk0;
    if (r != 0) {
        func_800213A0(NULL);
        func_80021380(0);
        D_800761D0.unk2 = 0;
        D_800761D0.unk3 = 0;
        D_800761D0.unk0 = 0;
        while (CdControlB(9, NULL, buf) == 0) {
        }
    }
    return r;
}

void func_80022B9C(void) {
    func_80021340();
    func_80021380(0);
    func_800213A0(NULL);
    D_800761D0.unk0 = 0;
}

void func_80022BD0(void) {
    CdControlB(0xE, NULL, NULL);
    VSync(3);
    CdControlB(8, NULL, NULL);
}

u8 func_80022C10(s32 a, void (*b)(u8 *)) {
    CdRequest r;
    r.unk0 = 1;
    r.unk10 = func_800231A0;
    r.unk4 = a;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk14 = func_800227C8;
    r.unk18 = b;
    return func_800230B8(&r);
}

u8 func_80022C60(s32 a, s32 b, void *c, void (*d)(u8 *)) {
    D_80076148.unk0 = 3;
    D_80076148.unkC = (CdChunk *)-1;
    D_80076148.unk10 = func_800231A0;
    D_80076148.unk4 = a;
    D_80076148.unk8 = b;
    D_80076148.unk1C.unk0 = b;
    D_80076148.unk1C.unk4 = c;
    D_80076148.unk14 = func_800227C8;
    D_80076148.unk18 = d;
    return func_800230B8(&D_80076148);
}

u8 func_80022CC4(s32 sector, CdChunk *chunks, s32 n, void (*cb)(u8 *)) {
    CdRequest r;
    s32 i;
    s32 total = 0;

    for (i = 0; i < n; i++) {
        total += chunks[i].unk0;
    }
    r.unk0 = 3;
    r.unk10 = func_800231A0;
    r.unk4 = sector;
    r.unk8 = total;
    r.unkC = chunks;
    r.unk14 = func_800227C8;
    r.unk18 = cb;
    return func_800230B8(&r);
}

u8 func_80022D3C(void (*a)(u8 *)) {
    CdRequest r;
    r.unk0 = 4;
    r.unk18 = a;
    r.unk4 = 0;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk10 = func_800231A0;
    r.unk14 = 0;
    return func_800230B8(&r);
}

s32 func_80022D84(s32 a) {
    CdRequest r;
    r.unk0 = 1;
    r.unk10 = func_800231A0;
    r.unk4 = a;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk14 = func_800227C8;
    r.unk18 = NULL;
    if (func_800230B8(&r) == 0) {
        return 0;
    }
    while (func_80022B18() != 0) {
    }
    return 1;
}

s32 func_80022DF0(s32 a, s32 b, void *c) {
    D_80076148.unk0 = 3;
    D_80076148.unkC = (CdChunk *)-1;
    D_80076148.unk10 = func_800231A0;
    D_80076148.unk4 = a;
    D_80076148.unk8 = b;
    D_80076148.unk1C.unk0 = b;
    D_80076148.unk1C.unk4 = c;
    D_80076148.unk14 = func_800227C8;
    D_80076148.unk18 = NULL;
    if (func_800230B8(&D_80076148) == 0) {
        return 0;
    }
    while (func_80022B18() != 0) {
    }
    return 1;
}

s32 func_80022E70(void) {
    CdRequest r;
    r.unk0 = 4;
    r.unk4 = 0;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk10 = func_800231A0;
    r.unk14 = 0;
    r.unk18 = NULL;
    if (func_800230B8(&r) == 0) {
        return 0;
    }
    while (func_80022B18() != 0) {
    }
    return 1;
}

u8 func_80022ED4(s32 a) {
    CdRequest r;
    r.unk0 = 9;
    r.unk4 = a;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk18 = NULL;
    r.unk10 = func_800231A0;
    r.unk14 = 0;
    return func_800230B8(&r);
}

u8 func_80022F1C(s32 a, void (*b)()) {
    CdRequest r;
    r.unk0 = 0xB;
    r.unk4 = a;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk10 = func_800231A0;
    r.unk14 = b;
    r.unk18 = NULL;
    return func_800230B8(&r);
}

s32 func_80022F64(s32 a) {
    if (D_800761D0.unk0 != 5) {
        func_800213A0(NULL);
        D_800761D0.unk944.unk4 = a;
        D_800761D0.unk0 = 5;
        func_80023580(9, NULL);
    }
    return 1;
}

u8 func_80022FD0(void (*a)(u8 *)) {
    CdRequest r;
    r.unk0 = 4;
    r.unk18 = a;
    r.unk4 = 0;
    r.unk8 = 0;
    r.unkC = NULL;
    r.unk10 = func_800231A0;
    r.unk14 = 0;
    return func_800230B8(&r);
}

s32 func_80023018(void) {
    if (D_800761D0.unk0 == 0) {
        return 1;
    }
    D_800761D0.unk2 = 0;
    D_800761D0.unk3 = 0;
    func_800213A0(NULL);
    D_800761D0.unk0 = 4;
    func_80023580(9, NULL);
    return 1;
}

u8 func_8002306C(void) {
    CdControlB(1, NULL, D_80076B0C.unk0);
    return func_800230A0();
}

s32 func_800230A0(void) {
    return (D_80076B0C.unk0[0] >> 4) & 1;
}

u8 func_800230B8(CdRequest *r) {
    volatile s32 st = D_800761D0.unk0;
    D_800761D0.unk1 = 0;
    if (st == 0) {
        if (!(r->unk0 & 4)) {
            goto run;
        }
        if (r->unk18 != NULL) {
            r->unk18(&D_800761D0);
        }
        return 1;
    }
    if (D_800761D0.unk2 == 0) {
        D_800761D0.unk968 = *r;
        D_800761D0.unk2 = 1;
        if (D_800761D0.unk0 == 0) {
        run:
            func_8002267C(r);
        }
        return 1;
    }
    return 0;
}

void func_800231A0(u8 com, u8 *param) {
    void (*fn)(s32);

    if (param != NULL) {
        D_80076B0C = *(Unk8 *)param;
    }
    if (D_800761D0.unk2 != 0) {
        func_8002325C(com);
        return;
    }
    if (com == 5) {
        func_80023304(5);
        return;
    }
    if (com == 2) {
        fn = D_80068668[D_800761D0.unk0];
        if (fn != NULL) {
            fn(2);
        }
    }
}

void func_8002325C(u8 arg0) {
    if (arg0 == 5 || arg0 == 2) {
        if (D_800761D0.unk0 != 3) {
            if (D_800761D0.unk944.unk18 != NULL) {
                D_800761D0.unk944.unk18(&D_800761D0);
            }
            func_8002267C(&D_800761D0.unk968);
        } else if (!(D_800761D0.unk944.unk0 & 8)) {
            func_800213A0(NULL);
            D_800761D0.unk0 = 4;
            func_80023580(9, 0);
        }
    }
}

void func_80023304(s32 unused) {
    if (D_800761D0.unk0 != 3 || !(D_800761D0.unk944.unk0 & 8)) {
        D_800761D0.unk3 = 1;
        D_800761D0.unk1++;
        func_800213A0(NULL);
        func_80023580(9, NULL);
    }
}

void func_8002336C(u8 a) {
    u8 buf[8];
    s32 st;
    s32 k;

    k = 0xA0;
    if (func_80021320() != k) {
        st = 2;
        buf[0] = k;
        k = 0xE;
        D_800761D0.unk0 = st;
        func_80023580(k, buf);
    } else {
        func_800233CC(a);
    }
}

s32 func_800233CC() {
    s32 arg;

    if (D_800761D0.unk944.unk0 & 2) {
        D_800761D0.unk18 = 0;
        D_800761D0.unk0 = 3;
        if (D_800761D0.unk944.unk0 & 8) {
            func_800213A0((void (*)())func_800235A0);
            arg = 0x1B;
        } else {
            func_800213A0(D_800761D0.unk944.unk14);
            arg = 6;
        }
    } else {
        D_800761D0.unk0 = 4;
        arg = 0x15;
        if (D_800761D0.unk944.unk0 & 8) {
            arg = 0x16;
        }
    }
    func_80023580(arg, 0);
}

void func_8002345C(void) {
    func_800213A0(NULL);
    func_80021380(0);
    if (D_800761D0.unk3 == 0) {
        D_800761D0.unk0 = 0;
        if (D_800761D0.unk944.unk18 != NULL) {
            D_800761D0.unk944.unk18(&D_800761D0);
        }
    } else {
        D_80076178 = D_800761D0.unk944;
        func_8002267C(&D_80076178);
    }
}

void func_8002350C(void) {
    D_800761A8 = D_800761D0.unk944;
    func_8002267C(&D_800761A8);
}

void func_80023580(u8 com, u8 *param) {
    CdControlF(com, param);
}

void func_800235A0(u8 com, u8 *param) {
    if (param != NULL) {
        D_80076B0C = *(Unk8 *)param;
    }
    if (D_800761D0.unk2 != 0) {
        func_800213A0(NULL);
        D_800761D0.unk0 = 4;
        func_80023580(9, 0);
    } else {
        D_800761D0.unk944.unk14(com, param);
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdReset);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800236A4);

void func_800236C4(void) {
    GameStateUnk24 *p = (GameStateUnk24 *)&g_gameState->unk24;
    s32 i;

    p->unk0_0 = 0;
    p->unk0_1 = 0;
    p->unk0_2 = 0;
    p->unk0_3 = 1;
    p->unk0_4 = 0;
    p->unk0_5 = 0;
    p->unk0_6 = 0;
    p->unk0_7 = 1;
    p->unk0_8 = 1;
    p->unk0_11 = 3;
    p->unk0_14 = 3;
    p->unk0_17 = 0;
    p->unk4 = 0;
    for (i = 0; i < 8; i++) {
        p->unk4 |= i << (i * 4);
    }
    func_8002397C();
}

void func_80023784(void) {
    s32 *act;
    s32 port;
    s32 i;
    u32 *tbl;

    port = 0;
    tbl = D_80068680;
    for (; port < 2; port++) {
        for (i = 0; i < 8; i++) {
            act = D_800686A0;
            func_80019708(port, tbl[i], act[func_80023784_remap(i)]);
        }
        for (i = 0; i < 1; i++) {
            func_80019708(port, D_800686C0[i], act[func_80023784_remap(D_800686C4[i])]);
        }
        func_80019708(port, 0x04000000, 8);
        func_80019708(port, 0x02000000, 1);
        func_80019708(port, 0x08000000, 1);
    }
}

void func_80023918(void) {
    func_800236C4();
}

/* K&R: func_8002BE10 calls it without an argument */
s32 func_80023938(arg0)
s32 arg0;
{
    GameStateUnk24 *p = (GameStateUnk24 *)&g_gameState->unk24;

    if (p->unk0_1 != 1) {
        return arg0;
    }
    return (p->unk4 >> (arg0 * 4)) & 0xF;
}

void func_8002397C(void) {
    func_800239A4();
    func_80023784();
}

void func_800239A4(void) {
    func_800555A8(g_gameState->unk24 & 1);
}

void func_800239D0(void) {
    GameData *d = g_gameState->unk1C;
    func_8002BEEC(d->unk10[0], d->unk10[1], d->unk7E0, d->unk7E0 + 0x4000);
}

void func_80023A0C(void) {
    func_8002BFD0();
}

void func_80023A2C(void) {
    func_80023EB8();
}

void func_80023A4C(void) {
    func_80023F00();
}

void func_80023A6C(void) {
    UnkTimer *t;
    u8 blink;
    u8 *pkt;
    u32 *ot;
    u8 shown;
    u8 colon;
    s32 w;
    s32 d;
    s32 v;
    s32 dg;
    s32 hr;
    s32 min;

    t = &D_80076B60.unk408;
    d = D_80076B60.unk0 - t->unk4;
    blink = 0;
    min = t->unkC / 60;
    hr = min / 60;
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 1;
    D_800686C8.x = 4;
    pkt = D_80076B60.unk404;
    min %= 60;
    if (d < 8) {
        D_800686C8.x = d * 8 - 0x3C;
        blink = 1;
    }
    if (t->unk1 == 0) {
        v = VSync(-1) % 60;
        colon = 0;
        if (blink || v >= 30) {
            colon = 1;
        }
        shown = colon;
    } else if (hr != 0 || min != 0) {
        v = t->unkC % 60;
        shown = 1;
        colon = v >= 30;
    } else {
        v = VSync(-1) % 60;
        colon = v >= 30;
        shown = colon;
    }
    if (shown) {
        dg = hr / 10;
        if (dg != 0) {
            pkt = func_80027834(D_800686C8.x + 5, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        }
        dg = hr - dg * 10;
        pkt = func_80027834(D_800686C8.x + 0xF, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        dg = min / 10;
        pkt = func_80027834(D_800686C8.x + 0x23, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        dg = min - dg * 10;
        pkt = func_80027834(D_800686C8.x + 0x2D, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
    }
    if (colon) {
        pkt = func_80027834(D_800686C8.x + 0x19, 8, ot, 0xEA, t->unk1 == 0, &w, pkt);
    }
    pkt = func_800253D4(&D_800686C8, ot, 1, 1, 1, (g_gameState->unk24 >> 18) & 1, pkt);
    setlen(pkt, 3);
    ((TileWords *)pkt)->rgbc = 0x62000000;
    ((TileWords *)pkt)->xy = ((D_800686C8.x + 2) & 0xFFFF) | ((D_800686C8.y + 2) << 16);
    ((TileWords *)pkt)->wh = (D_800686C8.w - 4) | ((D_800686C8.h - 4) << 16);
    addPrim(ot, pkt);
    pkt += sizeof(TileWords);
    SetDrawTPage(pkt, 1, 1, 0);
    addPrim(ot, pkt);
}

void func_80023EB8(void) {
    func_80024070();
    D_80076B60.unk404 = D_80076B60.unk4;
    g_gameState->playTime = 0;
    D_80076B60.unk0 = VSync(-1);
}

void func_80023F00(void) {
    D_80076B60.unk404 = &D_80076B60.unk4[g_gameState->unk1C->unk8 << 9];
    if (func_800240E8() != 0) {
        func_80023A6C();
    }
    func_8002417C();
}

void func_80023F60(u8 arg0) {
    s32 now = VSync(-1);
    s32 d;
    UnkTimer *t = &D_80076B60.unk408;

    if (arg0) {
        d = now - D_80076B60.unk0;
        if (t->unk0 != 0 && t->unk1 != 0 && t->unkC != 0) {
            t->unkC -= d;
            if (t->unkC <= 0) {
                t->unkC = 0;
            }
        }
        g_gameState->playTime += d;
    } else {
        D_80076B60.unk0 = now;
    }
}

void func_80024000(s32 arg0) {
    g_gameState->playTime = arg0;
    D_80076B60.unk0 = VSync(-1);
}

s32 func_80024030(void) {
    return g_gameState->playTime;
}

u32 func_80024048(void) {
    return g_gameState->playTime / 60;
}

void func_80024070(void) {
    D_80076F68.unk0 = 0;
    D_80076F68.unkC = 0;
    D_80076F68.unk8 = 0;
    D_80076F68.unk1 = 0;
    D_80076F68.unk4 = VSync(-1);
}

void func_800240B0(u8 arg0) {
    D_80076F68.unk0 = arg0;
    D_80076F68.unk1 = 0;
    D_80076F68.unk4 = VSync(-1);
}

u8 func_800240E8(void) {
    return D_80076F68.unk0;
}

void func_800240F8(u8 arg0) {
    if (D_80076B60.unk408.unk0 != 0) {
        D_80076B60.unk408.unk1 = arg0;
    }
}

u8 func_8002411C(void) {
    return D_80076F68.unk1;
}

void func_8002412C(s32 arg0) {
    Unk80076B60 *p = &D_80076B60;
    s32 t = arg0 * 60;

    p->unk408.unkC = t;
    p->unk408.unk8 = t;
}

s32 func_8002414C(void) {
    return (D_80076F68.unkC + 59) / 60;
}

void func_8002417C(void) {
    s32 now;
    s32 dt;
    UnkTimer *t;

    now = VSync(-1);
    dt = now - D_80076B60.unk0;
    t = &D_80076B60.unk408;
    if (t->unk0 != 0 && t->unk1 != 0 && t->unkC != 0) {
        t->unkC -= dt;
        if (t->unkC <= 0) {
            t->unkC = 0;
        }
    }
    g_gameState->playTime += dt;
    D_80076B60.unk0 = now;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawTPage);

void func_8002423C(s32 x, s32 y, s32 otz, Character *c) {
    u8 buf[16];
    s32 col;

    if (c == NULL) {
        return;
    }
    text_formatNumber(c->level, buf, 2, 1);
    func_8002B5FC(x, y, otz, c->unk0, 0);
    func_8002B56C(x + 0x24, y + 0xE, otz, buf, 0);
    func_800275DC(x, y + 0xE, otz, 0xB4, 0);
    func_800275DC(x, y + 0x17, otz, 0xAF, 0);
    func_800275DC(x, y + 0x20, otz, 0xB0, 0);
    if (c->hp != 0) {
        col = ((u16)(c->maxHp / 6) >= c->hp) << 2;
    } else {
        col = 3;
    }
    text_formatNumber(c->hp, buf, 4, 1);
    buf[4] = '/';
    text_formatNumber(c->maxHp, buf + 5, -1, 1);
    func_8002B56C(x + 0x16, y + 0x17, otz, buf, col);
    col = ((s16)((s16)c->maxMp / 6) >= (s16)c->mp) << 2;
    text_formatNumber((s16)c->mp, buf, 3, 1);
    buf[3] = '/';
    text_formatNumber((s16)c->maxMp, buf + 4, -1, 1);
    func_8002B56C(x + 0x1D, y + 0x20, otz, buf, col);
}

void func_80024428(s32 x, s32 y, s32 otz, Unk80024428 *s) {
    Character *a = g_gameState->unk1C->party[s->unk2];
    Character *b = g_gameState->unk1C->party[s->unk3];
    RECT r1;
    RECT r2;
    s32 d;
    s32 px;
    s32 t;

    if (s->unk0 != 0) {
        r1.w = 0x20;
        r1.x = x;
        r1.y = y;
        r1.h = 0x2E;
        func_8002D9B4(NULL, otz, 4);
        d = 11 - s->unk4;
        if (s->unk1 == 1) {
            t = -(d * 3);
        } else {
            t = d * 3;
        }
        px = x + t;
        r2.x = px - 2;
        r2.y = y - 2;
        r2.w = 0x24;
        r2.h = 0x32;
        func_80025108(&r2, otz, 0, (g_gameState->unk24 >> 18) & 1);
        func_80028AF0(px, y, otz, s->unk3, b->hp == 0);
        func_8002D9B4(&r1, otz, 4);
        func_8002423C(x + 0x26, y + 3, otz, g_gameState->unk1C->party[s->unk3]);
    } else {
        func_8002423C(x + 0x26, y + 3, otz, a);
    }
    func_80028AF0(x, y, otz, s->unk2, a->hp == 0);
}

void func_800245F0(RECT *r, s32 otz) {
    TileWords *p;
    DrTpageWords *t;

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    p = (TileWords *)g_primPtr;
    p->xy = (r->x + 3) | ((r->y + r->h) << 16);
    p->wh = r->w | 0x30000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    p = (TileWords *)g_primPtr;
    p->xy = (r->x + r->w) | ((r->y + 3) << 16);
    p->wh = 3 | ((r->h - 3) << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = 0xE1000600;
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_80024764(RECT *r, s32 otz) {
    PolyF4Half *p;
    DrTpageWords *t;

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = 0x2A000000;
    p->x0 = r->x + 4;
    p->y0 = r->y + r->h - 1;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + r->h - 1;
    p->x2 = r->x + 7;
    p->y2 = r->y + r->h + 2;
    p->x3 = r->x + r->w - 3;
    p->y3 = r->y + r->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = 0x2A000000;
    p->x0 = r->x + r->w - 3;
    p->y0 = r->y + 2;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + 7;
    p->x2 = r->x + r->w - 3;
    p->y2 = r->y + r->h - 1;
    p->x3 = r->x + r->w + 2;
    p->y3 = r->y + r->h - 1;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = 0xE1000600;
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_800249A8(RECT *r, s32 otz) {
    PolyF4Half *p;
    DrTpageWords *t;

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = 0x2A000000;
    p->x0 = r->x + 2;
    p->y0 = r->y + r->h;
    p->x1 = r->x + r->w + 1;
    p->y1 = r->y + r->h;
    p->x2 = r->x + 4;
    p->y2 = r->y + r->h + 2;
    p->x3 = r->x + r->w - 1;
    p->y3 = r->y + r->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = 0x2A000000;
    p->x0 = r->x + r->w - 3;
    p->y0 = r->y;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + 5;
    p->x2 = r->x + r->w - 3;
    p->y2 = r->y + r->h;
    p->x3 = r->x + r->w + 2;
    p->y3 = r->y + r->h;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = 0xE1000600;
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_80024BD8(void) {
    s32 w;
    RECT tw;
    PolyG4Words *p;

    w = g_gameState->unkC->disp.w;
    setlen(g_primPtr, 8);
    p = (PolyG4Words *)g_primPtr;
    p->rgbc0 = 0x3A000000;
    p->rgb2 = 0x505050;
    p->rgb3 = 0x505050;
    p->xy1 = w;
    p->rgb1 = 0;
    p->xy0 = 0;
    p->xy2 = 0x800000;
    p->xy3 = w | 0x800000;
    addPrim(&g_ot[0xFFF], p);
    g_primPtr += sizeof(PolyG4Words);

    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = 0xE1000640;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);

    tw.x = D_80068DC0.unk0;
    tw.y = D_80068DC0.unk1;
    tw.w = D_80068DC0.unk4;
    tw.h = D_80068DC0.unk5;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = 0;
    ((SprtWords *)g_primPtr)->uvclut = D_80068DC0.unk2 << 16;
    ((SprtWords *)g_primPtr)->wh = w | 0x800000;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, D_80068DC0.unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

u8 *func_80024EB4(RECT *r, u32 *ot, u8 u, u8 v, s32 pal, u8 *pkt) {
    s32 clut = func_800274B4(D_80068CE8.unk2, pal);
    RECT tw;

    tw.x = D_80068CE8.unk0;
    tw.y = D_80068CE8.unk1;
    tw.w = D_80068CE8.unk4;
    tw.h = D_80068CE8.unk5;

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = 0x64808080;
    ((SprtWords *)pkt)->xy = (u16)r->x | (r->y << 16);
    ((SprtWords *)pkt)->uvclut = u | (v << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = r->w | (r->h << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_mode(1, 1, D_80068CE8.unk6);
    ((DrMode2Words *)pkt)->code[1] = get_tw(&tw);
    addPrim(ot, pkt);
    return pkt + sizeof(DrMode2Words);
}

void func_80025108(RECT *r, s32 otz, u8 skip, s32 abr) {
    LineXY l[4];
    s32 i;
    u32 *col;
    LineF2W *p;

    l[0].x0 = r->x + 1;
    l[0].y0 = r->y + 1;
    l[0].x1 = r->x + r->w - 3;
    l[0].y1 = r->y + 1;
    l[1].x0 = r->x + 1;
    l[1].y0 = r->y + 2;
    l[1].x1 = r->x + 1;
    l[1].y1 = r->y + r->h - 3;
    l[2].x0 = r->x + 1;
    l[2].y0 = r->y + r->h - 2;
    l[2].x1 = r->x + r->w - 2;
    l[2].y1 = r->y + r->h - 2;
    l[3].x0 = r->x + r->w - 2;
    l[3].y0 = r->y;
    l[3].x1 = r->x + r->w - 2;
    l[3].y1 = r->y + r->h - 3;
    col = D_80068F20[abr];
    for (i = skip != 0; i < 4; i++) {
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[i >> 1];
        p->xy0 = (u16)l[i].x0 | (l[i].y0 << 16);
        p->xy1 = (u16)l[i].x1 | (l[i].y1 << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[(i >> 1) + 2];
        p->xy0 = ((l[i].x0 + D_80068F94[i][0]) & 0xFFFF) | ((l[i].y0 + D_80068F94[i][1]) << 16);
        p->xy1 = ((l[i].x1 + D_80068F94[i][2]) & 0xFFFF) | ((l[i].y1 + D_80068F94[i][3]) << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
    }
}

u8 *func_800253D4(RECT *r, u32 *ot, u8 flag, u8 abe, s32 style, s32 pal, u8 *pkt) {
    s32 n = 1;
    s32 semi = (abe != 0) << 25;
    Unk800689B0 *d;
    s32 clut;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    RECT tw;

    if (flag) {
        n = 2;
    }
    d = NULL;
    if (style != 2) {
        clut = func_800274B4(D_800686D0[D_80068FB4[style][0]].unk0 >> 16, pal) << 16;
    } else {
        clut = 0x3EA40000;
    }
    for (i = 0, y = r->y; i < 2; i++) {
        for (j = 0, x = r->x; j < 2; j++) {
            if (flag || i || !j) {
                d = (Unk800689B0 *)&D_800686D0[D_80068FB4[style][i * 2 + j]];
                setlen(pkt, 3);
                ((Sprt8Words *)pkt)->rgbc = D_80068FAC[flag] | semi;
                ((Sprt8Words *)pkt)->xy = (u16)x | (y << 16);
                ((Sprt8Words *)pkt)->uvclut = d->unk0 | (d->unk1 << 8) | clut;
                addPrim(ot, pkt);
                pkt += sizeof(Sprt8Words);
                x += r->w - 8;
            }
        }
        y += r->h - 8;
    }

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    if (r->w - 16 > 0) {
        y = r->y + r->h - 8;
        for (i = 0; i < n; i++) {
            d = (Unk800689B0 *)&D_800686D0[D_80068FC0[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = D_80068FA4[flag] | semi;
            ((SprtWords *)pkt)->xy = ((r->x + 8) & 0xFFFF) | (y << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = (r->w - 16) | 0x80000;
            addPrim(ot, pkt);
            y -= r->h - 8;
            tw.x = d->unk0;
            tw.y = d->unk1;
            tw.w = d->unk4;
            tw.h = d->unk5;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    if (r->h - 16 > 0) {
        x = r->x;
        for (i = 0; i < n; i++) {
            d = (Unk800689B0 *)&D_800686D0[D_80068FC8[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = D_80068FA4[flag] | semi;
            ((SprtWords *)pkt)->xy = (x & 0xFFFF) | ((r->y + 8) << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = ((r->h - 16) << 16) | 8;
            addPrim(ot, pkt);
            x += r->w - 8;
            tw.x = d->unk0;
            tw.y = d->unk1;
            tw.w = d->unk4;
            tw.h = d->unk5;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, d->unk6);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void func_80025A28(s32 x, s32 y, s32 otz, s32 style, s32 w, s32 abr) {
    Unk800689B0 *d;
    u16 clut;
    s32 i;
    RECT tw;
    Unk800689B0 *tbl;
    Unk800689B0 *tbl2;
    s32 yy;

    if (style == 1) {
        tbl = (Unk800689B0 *)D_800686D0;
        clut = tbl[D_80068FD0[1][0]].unk2;
    } else {
        tbl2 = (Unk800689B0 *)D_800686D0;
        clut = func_800274B4(tbl2[D_80068FD0[style][0]].unk2, abr);
    }
    for (i = 0; i < 2; i++) {
        d = &((Unk800689B0 *)D_800686D0)[D_80068FD0[style][i]];
        setlen(g_primPtr, 4);
        yy = y << 16;
        ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
        if (i != 0) {
            ((SprtWords *)g_primPtr)->xy = ((x - (8 - w)) & 0xFFFF) | yy;
        } else {
            ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | yy;
        }
        ((SprtWords *)g_primPtr)->uvclut = d->unk0 | (d->unk1 << 8) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = d->unk4 | (d->unk5 << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    if (w > 0x10) {
        setlen(g_primPtr, 2);
        ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
        ((DrMode2Words *)g_primPtr)->code[1] = 0;
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrMode2Words);
        d = &((Unk800689B0 *)D_800686D0)[D_80068FD0[style][2]];
        tw.x = d->unk0;
        tw.y = d->unk1;
        tw.w = d->unk4;
        tw.h = d->unk5;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
        ((SprtWords *)g_primPtr)->xy = ((x + 8) & 0xFFFF) | (y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (style << 10) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = (w - 0x10) | (d->unk5 << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d->unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(w > 0x10 ? &tw : &D_80068F40);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void func_80025ED4(s32 x, s32 y, s32 otz, s32 cur, s32 max, s32 style, u8 blink) {
    CVECTOR c1;
    CVECTOR c2;
    GaugeColors *e;
    CVECTOR *f1, *f2, *t1, *t2;
    s32 w;
    s32 k;
    s32 right;
    PolyG4Words *p;
    TileWords *t;

    w = cur * 36 / max;
    e = &D_80068FD8[style];
    if (cur != 0) {
        if (blink && cur >= max) {
            k = D_80077150 & 0x1F;
            if (k < 16) {
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[0];
                t2 = &e->c[0];
            } else {
                k -= 16;
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[1];
                t2 = t1;
            }
            CVECTOR_LERP(c1, f1, t1, k, 7);
            CVECTOR_LERP(c2, f2, t2, k, 7);
        } else {
            t1 = &e->c[3];
            c1 = e->c[2];
            CVECTOR_LERP(c2, &c1, t1, cur, max);
        }
        setlen(g_primPtr, 8);
        p = (PolyG4Words *)g_primPtr;
        p->rgbc0 = p->rgb2 = c1.r | (c1.g << 8) | (c1.b << 16) | 0x38000000;
        p->rgb1 = p->rgb3 = c2.r | (c2.g << 8) | (c2.b << 16);
        right = w + 2;
        p->xy0 = (x + 2) | ((y + 2) << 16);
        p->xy2 = (x + 2) | ((y + 4) << 16);
        p->xy1 = (x + right) | ((y + 2) << 16);
        p->xy3 = (x + right) | ((y + 4) << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr = (u8 *)(p + 1);
    }
    func_800275DC(x, y, otz, 2, 0);
    setlen(g_primPtr, 3);
    t = (TileWords *)g_primPtr;
    t->rgbc = *(u32 *)&e->c[0];
    t->xy = (x + 1) | ((y + 1) << 16);
    t->wh = (w < 36 ? 0x30000 : 0x10000) | 0x26;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
}

void func_8002642C(s16 x, s16 y, s32 otz, s32 idx, u8 flag, u8 mode) {
    Unk800689B0 *g = &D_80068D70[idx];
    u32 clut;
    SprtWords *p;
    DrTpageWords *t;

    if (!mode) {
        clut = (g->unk2 - ((g_gameState->unk24 >> 18) & 1) * 0xC0) << 16;
    } else {
        clut = 0x3EA40000;
    }
    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = flag ? 0x66808080 : 0x64808080;
    p = (SprtWords *)g_primPtr;
    p->xy = x | (y << 16);
    p->uvclut = g->unk0 | (g->unk1 << 8) | clut;
    p->wh = g->unk4 | (g->unk5 << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = (g->unk6 & 0x9FF) | 0xE1000600;
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_800265B0(s32 x, s32 y, s32 otz, s32 shape, s32 u, s32 v) {
    RECT tw;
    Unk80068F48 *o;
    u16 clut;

    clut = D_80068CE8.unk2 - ((g_gameState->unk24 >> 18) & 1) * 0xC0;
    tw.x = D_80068CE8.unk0;
    tw.y = D_80068CE8.unk1;
    tw.w = D_80068CE8.unk4;
    tw.h = D_80068CE8.unk5;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 9);
    o = &D_80068F48[shape];
    ((PolyFT4Words *)g_primPtr)->rgbc = 0x2C808080;
    ((PolyFT4Words *)g_primPtr)->xy0 = (x + o->x0) | ((y + o->y0) << 16);
    ((PolyFT4Words *)g_primPtr)->uv0clut = (u + o->x0) | ((v + o->y0) << 8) | (clut << 16);
    ((PolyFT4Words *)g_primPtr)->xy1 = (x + o->x1) | ((y + o->y1) << 16);
    ((PolyFT4Words *)g_primPtr)->uv1tpage = (u + o->x1) | ((v + o->y1) << 8) | (D_80068CE8.unk6 << 16);
    ((PolyFT4Words *)g_primPtr)->xy2 = (x + o->x2) | ((y + o->y2) << 16);
    ((PolyFT4Words *)g_primPtr)->uv2 = (u + o->x2) | ((v + o->y2) << 8);
    ((PolyFT4Words *)g_primPtr)->xy3 = (x + o->x3) | ((y + o->y3) << 16);
    ((PolyFT4Words *)g_primPtr)->uv3 = (u + o->x3) | ((v + o->y3) << 8);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(PolyFT4Words);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&tw);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void func_80026900(s32 x, s32 y, s32 otz, s32 idx) {
    PolyF4Words *p;
    Unk80068F48 *t;
    setlen(g_primPtr, 5);
    p = (PolyF4Words *)g_primPtr;
    p->rgbc = 0x28A8B078;
    t = &D_80068F48[idx];
    p->xy0 = (x + t->x0) | ((y + t->y0) << 16);
    p->xy1 = (x + t->x1) | ((y + t->y1) << 16);
    p->xy2 = (x + t->x2) | ((y + t->y2) << 16);
    p->xy3 = (x + t->x3) | ((y + t->y3) << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

void func_800269F4(s32 x, s32 y, s32 otz, s32 len) {
    u32 col[3];
    s32 i;
    LineF2W *p;

    col[0] = D_80068F20[(g_gameState->unk24 >> 18) & 1][1];
    col[1] = 0x40403838;
    col[2] = D_80068F20[(g_gameState->unk24 >> 18) & 1][0];

    for (i = 0; i < 3; i++) {
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[i];
        if (len < 0) {
            p->xy0 = (x + i) | (y << 16);
            p->xy1 = (x + i) | ((y - len) << 16);
        } else {
            p->xy0 = x | ((y + i) << 16);
            p->xy1 = (x + len - 1) | ((y + i) << 16);
        }
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
    }
}

void func_80026B30(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = func_800253D4(arg0, &g_ot[otz], arg2, arg3, 0, arg4, g_primPtr);
}

void func_80026B84(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = func_800253D4(arg0, &g_ot[otz], arg2, arg3, 1, arg4, g_primPtr);
}

void func_80026BDC(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = func_800253D4(arg0, &g_ot[otz], arg2, arg3, 2, arg4, g_primPtr);
}

void func_80026C34(s32 arg0, s32 arg1, s32 otz, s32 arg3, u8 arg4) {
    g_primPtr = func_80026C7C(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

u8 *func_80026C7C(s32 x, s32 y, u32 *ot, s32 item, u8 flag, u8 *pkt) {
    Unk80069018 *e = &D_80069018[item];
    Unk800689B0 *d = &D_800689B0[e->unk8];
    s32 clut;

    clut = d->unk2 + (flag ? e->unk9 << 6 : 0x3C0);
    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = (flag == 0 ? 0x66808080 : 0x64808080);
    ((SprtWords *)pkt)->xy = (x & 0xFFFF) | (y << 16);
    ((SprtWords *)pkt)->uvclut = d->unk0 | (d->unk1 << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, d->unk6);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void func_80026DCC(s32 arg0, s32 arg1, s32 otz, s32 arg3, u8 arg4) {
    g_primPtr = func_80026C7C(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void func_80026E14(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Character *p = g_gameState->unk1C->party[arg3];

    if (p != NULL) {
        func_80028AF0(arg0, arg1, arg2, arg3, p->hp == 0);
        func_80026F8C(arg0 + 0x26, arg1 + 3, arg2, arg3);
    }
}

void func_80026EA0(s32 x, s32 y, s32 otz, s32 slot, s32 mode) {
    Character *p = g_gameState->unk1C->party[slot];
    if (p != NULL && mode >= 0) {
        if (mode < 3) {
            func_800283E0(x, y, otz, slot, 0x29, p->hp == 0);
            func_80026F8C(x + 0x26, y, otz, slot);
        } else if (mode < 5) {
            func_800283E0(x, y, otz, slot, 0x29, p->hp == 0);
            func_80027A4C(x + 0x26, y, otz, slot);
        }
    }
}

void func_80026F8C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8002423C(arg0, arg1, arg2, g_gameState->unk1C->party[arg3]);
}

void func_80026FC4(s32 arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if ((arg3 >> i) & 1) {
            func_800275DC(arg0, arg1, arg2, D_80068F70[i], 0);
            arg0 += 12;
        }
    }
}

void func_8002705C(RECT *arg0, s32 arg1, s32 arg2) {
    void (*fn)(RECT *, s32) = D_80068F80[arg2];

    if (fn != 0) {
        fn(arg0, arg1);
    }
}

void func_8002709C(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = func_80024EB4(arg0, &g_ot[otz], arg2, arg3, arg4, g_primPtr);
}

void func_800270EC(s32 arg0, s32 arg1, s32 arg2) {
    func_800275DC(arg0, arg1, arg2, 1, 0);
}

void func_80027110(s32 arg0, s32 arg1, s32 otz, s32 arg3, s32 arg4, u8 arg5, u8 arg6) {
    g_primPtr = func_80027170(arg0, arg1, &g_ot[otz], arg3, arg4, arg5, arg6, g_primPtr);
}

u8 *func_80027170(s32 x, s32 y, u32 *ot, s32 cur, s32 max, u8 show, u8 flag, u8 *pkt) {
    u8 buf[16];
    s32 tmp[2];
    s32 w;
    s32 t;
    u32 rgbc;
    Unk800686E8 *d = &D_800686E8;

    if (max < cur) {
        cur = max;
    }
    if (cur != 0 && cur >= max) {
        pkt = func_80027834(x + 4, y, ot, 0xC0, flag, tmp, pkt);
    } else if (show) {
        text_formatNumber(cur, buf, 3, 1);
        text_formatNumber(max, buf + 4, -1, 1);
        buf[3] = 0x2F;
        pkt = func_80027B5C(x + 3, y + 2, ot, buf, flag ? 0x66606060 : 0x64808080, (SprtWords *)pkt);
    }
    if (cur != 0) {
        t = d->unk4 - 6;
        if (max != 0) {
            w = cur * t / max;
        } else {
            w = t;
        }
        setlen(pkt, 3);
        rgbc = 0x6070A838;
        if (cur >= max) {
            rgbc = 0x602838A8;
        }
        if (flag) {
            rgbc |= 0x02000000;
        }
        ((TileWords *)pkt)->rgbc = rgbc;
        ((TileWords *)pkt)->xy = ((x + 3) & 0xFFFF) | ((y + 1) << 16);
        ((TileWords *)pkt)->wh = w | 0x30000;
        addPrim(ot, pkt);
        pkt += sizeof(TileWords);
    }
    return func_80027948(x, y, ot, d, flag, tmp, (SprtWords *)pkt);
}

void func_800273C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 buf[8];

    text_formatNumber(arg3, &buf[0], 2, 1);
    text_formatNumber(arg4, &buf[3], -1, 1);
    buf[2] = '/';
    func_800275DC(arg0, arg1, arg2, 0xC1, 0);
    func_8002B5B4(arg0 + 15, arg1 + 1, arg2, buf, arg3 == 0 ? 4 : 0);
}

u16 func_80027480(s32 arg0) {
    return arg0 - ((g_gameState->unk24 >> 18) & 1) * 0xC0;
}

u16 func_800274B4(s32 arg0, s32 arg1) {
    return arg0 - arg1 * 0xC0;
}

void func_800274CC(RECT *rect, s32 otz) {
    TileWords *p;

    setlen(g_primPtr, 3);
    p = (TileWords *)g_primPtr;
    p->rgbc = 0x60A8B078;
    p->xy = (u16)rect->x | (rect->y << 16);
    p->wh = rect->w | (rect->h << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

/* K&R definition: callers pass plain ints */
void func_80027568(arg0, arg1, arg2, arg3, arg4)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    s32 arg3;
    u8 arg4;
{
    func_8002642C(arg0, arg1, arg2, arg3, arg4, 1);
}

/* K&R definition: callers pass plain ints */
void func_800275A4(arg0, arg1, arg2, arg3, arg4)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    s32 arg3;
    u8 arg4;
{
    func_8002642C(arg0, arg1, arg2, arg3, arg4, 0);
}

s32 func_800275DC(s32 x, s32 y, s32 otz, s32 id, u8 flag) {
    s32 w;
    Unk800686E8 *d = &D_800686D0[id];

    g_primPtr = func_80027948_inline(x, y, &g_ot[otz], d, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

s32 func_80027710(s32 x, s32 y, s32 otz, void *d, u8 flag) {
    s32 w;

    g_primPtr = func_80027948_inline(x, y, &g_ot[otz], d, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

u8 *func_80027834(s32 x, s32 y, u32 *ot, s32 i, u8 flag, s32 *out, void *pkt) {
    SprtWords *s = pkt;
    DrTpageWords *t;
    Unk800686E8 *d;
    setlen(s, 4);
    d = &D_800686D0[i];
    if (flag) {
        s->rgbc = 0x66606060;
    } else {
        s->rgbc = 0x64808080;
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->unk0;
    s->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = (d->unk6 & 0x9FF) | 0xE1000600;
    addPrim(ot, t);
    *out = d->unk4;
    return (u8 *)(t + 1);
}

u8 *func_80027948(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s) {
    DrTpageWords *t;
    setlen(s, 4);
    if (flag) {
        s->rgbc = 0x66606060;
    } else {
        s->rgbc = 0x64808080;
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->unk0;
    s->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = (d->unk6 & 0x9FF) | 0xE1000600;
    addPrim(ot, t);
    *out = d->unk4;
    return (u8 *)(t + 1);
}

void func_80027A4C(s32 x, s32 y, s32 otz, s32 slot) {
    Character *c = g_gameState->unk1C->party[slot];
    u8 buf[16];
    RECT r;

    if (c == NULL) {
        return;
    }
    text_formatNumber(c->level, buf, 2, 1);
    func_8002B5FC(x, y, otz, c->unk0, 0);
    func_8002B56C(x + 0x26, y + 0xE, otz, buf, 0);
    func_800275DC(x, y + 0xE, otz, 0xB4, 0);
    func_80026FC4(x, y + 0x1C, otz, c->unk38, 0);
    r.x = x - 4;
    r.y = y + 0x18;
    r.w = 0x60;
    r.h = 0x12;
    func_8002DEE4(&r, otz, 0);
}

u8 *func_80027B5C(s32 x, s32 y, u32 *ot, u8 *s, s32 unused, SprtWords *p) {
    s32 idx;
    s32 adv;
    Unk800686E8 *g;
    DrTpageWords *t;

    while (*s != 0) {
        if (*s != ' ') {
            if (*s == '/') {
                idx = 11;
                adv = 4;
            } else {
                adv = 5;
                if (*s != '%') {
                    idx = *s - '0';
                } else {
                    idx = 10;
                }
            }
            setlen(p, 4);
            p->rgbc = 0x64808080;
            p->xy = (x & 0xFFFF) | (y << 16);
            g = &D_800687E0[idx];
            p->wh = g->unk4 | (g->unk5 << 16);
            p->uvclut = g->unk0;
            addPrim(ot, p);
            p++;
        } else {
            adv = 5;
        }
        s++;
        x += adv;
    }
    t = (DrTpageWords *)p;
    setlen(t, 1);
    t->code = 0xE100061F;
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

void func_80027CB0(void) {
    u8 *p = D_8006BB74;

    func_80027D18();
    func_80027D4C();
    while (*p != 0xFF) {
        func_80027D84(p[0], p[1]);
        p += 2;
    }
}

void func_80027D18(void) {
    s32 i;

    for (i = 0; i < 0x100; i++) {
        g_gameState->unk1C->unk574.unk0[i].unk1 = 0;
    }
}

void func_80027D4C(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        g_gameState->unk1C->unk774.unk0[i] = 0;
    }
}

s32 func_80027D84(s32 id, s32 n) {
    UnkPair *e;
    s32 i;
    if (id == 0xFF) {
        return 0;
    }
    e = g_gameState->unk1C->unk574.unk0;
    for (i = 0; i < 0x100; e++, i++) {
        if (e->unk1 != 0 && e->unk0 == id) {
            if (e->unk1 + n >= 100) {
                n = 99 - e->unk1;
            }
            e->unk1 += n;
            return n;
        }
    }
    e = g_gameState->unk1C->unk574.unk0;
    for (i = 0; i < 0x100; i++, e++) {
        if (e->unk1 == 0) {
            e->unk0 = id;
            e->unk1 = n;
            return n;
        }
    }
    return 0;
}

s32 func_80027E5C(s32 id, s32 count) {
    s32 i;
    UnkPair *e = g_gameState->unk1C->unk574.unk0;

    for (i = 0; i < 0x100; e++, i++) {
        if (e->unk1 != 0 && e->unk0 == id) {
            if (e->unk1 < count) {
                count = e->unk1;
            }
            e->unk1 -= count;
            return count;
        }
    }
    return 0;
}

UnkPair *func_80027EDC(s32 id) {
    s32 i;
    UnkPair *e = g_gameState->unk1C->unk574.unk0;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 == id) {
            return e;
        }
    }
    return NULL;
}

s32 func_80027F34(s32 id) {
    UnkPair *e = func_80027EDC_inline(id);

    return e != NULL ? e->unk1 : 0;
}

s32 func_80027F94(s32 id) {
    s32 i, j;
    s32 n = 0;

    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
            for (j = 0; j < 5; j++) {
                if (id == g_gameState->unk1C->characters[i].unk39[j]) {
                    n++;
                }
            }
        }
    }
    return n;
}

s32 func_80028014(s32 arg0) {
    s32 i;
    Unk80069018 *e = &D_80069018[arg0];

    for (i = 0; i < 5; i++) {
        if (e->unkF & D_8006BB84[i]) {
            return i;
        }
    }
    return -1;
}

void func_80028074(s32 id) {
    s32 idx = id >> 2;

    g_gameState->unk1C->unk774.unk0[idx] |= 1 << ((id & 3) * 2);
    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void func_800280CC(s32 id) {
    s32 idx = id >> 2;

    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2));
    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void func_80028128(s32 id) {
    g_gameState->unk1C->unk774.unk0[id >> 2] |= 1 << ((id & 3) * 2 + 1);
}

void func_80028164(s32 id) {
    g_gameState->unk1C->unk774.unk0[id >> 2] &= ~(1 << ((id & 3) * 2 + 1));
}

s32 func_800281A4(s32 arg0) {
    return (g_gameState->unk1C->unk774.unk0[arg0 >> 2] >> ((arg0 & 3) * 2)) & 1;
}

s32 func_800281D0(s32 id) {
    return (g_gameState->unk1C->unk774.unk0[id >> 2] >> ((id & 3) * 2 + 1)) & 1;
}

void func_80028204(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 flag) {
    u16 clut = GetClut(0, id + 0x1F3);
    s32 u;
    s32 v;
    s32 tpage;

    if (id >= 0 && id < D_80076F7C) {
        u = id << 5;
        if (id < 4) {
            v = 0xB0;
            tpage = 0x9F;
        } else {
            u = ((id - 4) & 3) << 5;
            v = ((id - 4) >> 2) * 46;
            tpage = 0x90;
        }
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? 0x64808080 : 0x66606060;
        ((SprtWords *)g_primPtr)->xy = x | (y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = (h << 16) | 0x20;
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 2);
        SetDrawTPage(g_primPtr, 1, 1, tpage);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrTpageWords);
    }
}

void func_800283E0(s32 x, s32 y, s32 otz, s32 slot, s32 h, u8 flag) {
    s16 h0;
    s32 cx;
    s32 cy;
    u16 clut;
    s32 u, v;
    SprtWords *p;
    SprtWords *q;

    h0 = 0x20;
    if (h < 0x20) {
        h0 = h;
    }
    if (g_gameState->mode == 3) {
        cx = 0x280;
        cy = 0xDC;
    } else {
        cx = 0x100;
        cy = 0xFD;
    }
    clut = GetClut(cx + ((slot & 1) << 7), cy + (slot >> 1));
    u = slot << 5;
    v = 0xE0;
    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? 0x64808080 : 0x66808080;
    p = (SprtWords *)g_primPtr;
    g_primPtr += sizeof(SprtWords);
    p->xy = (x & 0xFFFF) | (y << 16);
    p->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
    p->wh = (h0 << 16) | 0x20;
    addPrim(&g_ot[otz], p);
    SetDrawTPage(g_primPtr, 1, 1, 0x88);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    if (h > 0x20) {
        u = ((slot & 1) << 5) + 0x20;
        v = (slot >> 1) * 14 - 0x20;
        h -= h0;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? 0x64808080 : 0x66808080;
        q = (SprtWords *)g_primPtr;
        g_primPtr += sizeof(SprtWords);
        q->xy = (x & 0xFFFF) | ((y + 0x20) << 16);
        q->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
        q->wh = (h << 16) | 0x20;
        addPrim(&g_ot[otz], q);
        SetDrawTPage(g_primPtr, 1, 1, 0x89);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrTpageWords);
    }
}

void func_80028720(void *data, s32 n) {
    RECT r0;
    RECT r1;
    s32 i;
    u32 *prev = data;
    u32 *q = prev;
    u32 *cur;

    D_80076F78.unk4 = n;
    D_80076F78.unk0 = prev;
    func_80028994(0, &r0, &r1);
    StoreImage(&r0, D_80076F80);
    DrawSync(0);
    LoadImage(&r0, prev);
    DrawSync(0);
    LoadImage(&r1, prev + 0x40);
    DrawSync(0);
    cur = prev + 0x1B0;
    for (i = 1; i < D_80076F78.unk4; i++) {
        func_80028994(i, &r0, &r1);
        StoreImage(&r0, prev);
        DrawSync(0);
        if (i >= 4) {
            StoreImage(&r1, q + 0x40);
            DrawSync(0);
            q += 0x1B0;
        }
        LoadImage(&r0, cur);
        DrawSync(0);
        LoadImage(&r1, cur + 0x40);
        DrawSync(0);
        cur += 0x1B0;
        prev += 0x1B0;
    }
}

void func_8002886C(void) {
    RECT clut;
    RECT img;
    u32 *p = D_80076F78.unk0;
    u32 *q;
    s32 i;
    func_80028994(0, &clut, &img);
    LoadImage(&clut, D_80076F80);
    DrawSync(0);
    q = p;
    for (i = 1; i < D_80076F78.unk4; i++) {
        func_80028994(i, &clut, &img);
        LoadImage(&clut, p);
        DrawSync(0);
        if (i >= 4) {
            LoadImage(&img, q + 0x40);
            DrawSync(0);
            q += 0x1B0;
        }
        p += 0x1B0;
    }
}

u8 func_80028954(s32 arg0) {
    return D_8006BB8C[g_gameState->unk1C->characters[arg0].unk34.b.unk4];
}

void func_80028994(s32 arg0, RECT *clut, RECT *img) {
    clut->y = arg0 + 0x1F3;
    clut->w = 0x80;
    clut->h = 1;
    clut->x = 0;
    if (arg0 < 4) {
        img->x = arg0 * 16 + 0x3C0;
        img->y = 0x1B0;
    } else {
        s32 n = arg0 - 4;

        img->x = (n & 3) * 16;
        img->y = (n >> 2) * 0x2E + 0x100;
    }
    img->w = 0x10;
    img->h = 0x2E;
}

void func_80028A10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4) {
    func_80028204(arg0, arg1, arg2, arg3, 0x2E, arg4);
}

void func_80028A40(s32 arg0, RECT *a, RECT *b, RECT *c) {
    s32 x, y;

    if (g_gameState->mode == 3) {
        x = 0x280;
        y = 0xDC;
    } else {
        x = 0x100;
        y = 0xFD;
    }
    a->x = x + ((arg0 & 1) << 7);
    a->y = y + (arg0 >> 1);
    a->w = 0x80;
    a->h = 1;
    b->x = arg0 * 16 + 0x200;
    b->y = 0xE0;
    b->w = 0x10;
    b->h = 0x20;
    c->x = (arg0 & 1) * 16 + 0x250;
    c->y = (arg0 >> 1) * 14 + 0xE0;
    c->w = 0x10;
    c->h = 0xE;
}

void func_80028AF0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4) {
    func_800283E0(arg0, arg1, arg2, arg3, 0x2E, arg4);
}

void func_80028B20(u8 *buf) {
    s32 i;

    for (i = 0; i < 4; i++) {
        GameData *d = g_gameState->unk1C;
        Character *p = d->party[i];

        if (p != NULL) {
            func_80028BE8(buf + D_8006BB8C[d->characters[p->unk34.b.id].unk34.b.unk4] * 0x6C0, i);
        }
    }
}

void func_80028BE8(u8 *buf, s32 arg1) {
    RECT a, b, c;

    func_80028A40(arg1, &a, &b, &c);
    LoadImage(&a, (u32 *)buf);
    DrawSync(0);
    LoadImage(&b, (u32 *)(buf + 0x100));
    DrawSync(0);
    LoadImage(&c, (u32 *)(buf + 0x500));
    DrawSync(0);
}

void func_80028C70(u8 *buf, s32 arg1) {
    RECT a, b, c;

    func_80028A40(arg1, &a, &b, &c);
    StoreImage(&a, (u32 *)buf);
    DrawSync(0);
    StoreImage(&b, (u32 *)(buf + 0x100));
    DrawSync(0);
    StoreImage(&c, (u32 *)(buf + 0x500));
    DrawSync(0);
}

void func_80028CF8(UnkBlink *b, s32 arg1, void (*cb)(void)) {
    b->unk0 = 0;
    b->unk2 = arg1;
    b->unk8 = cb;
}

void func_80028D08(UnkBlink *b, s32 arg1, s32 arg2, s32 arg3) {
    b->unk1 = arg3;
    b->unk4 = 0;
    if (arg3 != 0) {
        b->unk2 = arg1;
        b->unk3 = arg2;
        b->unk0 = 1;
    }
}

void func_80028D2C(UnkBlink *b) {
    void (*cb)(void);

    if (b->unk0 != 0) {
        if (++b->unk4 >= 12) {
            b->unk2 = b->unk3;
            cb = b->unk8;
            b->unk4 = 0;
            b->unk0 = 0;
            if (cb != NULL) {
                cb();
            }
        }
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetClut);

s32 func_80028DB0(s32 x, s32 hi, s32 c) {
    u8 buf[16];
    u8 w;
    u8 start;
    u8 width;
    u8 *dst;
    u8 *src;
    s32 row;
    s32 i;
    s32 j;
    u8 b;
    u8 *p;
    s32 px;
    u8 v, o;
    u8 t;

    dst = (u8 *)D_80077144;
    w = D_80010BD0[c];
    start = w >> 4;
    width = (w & 0xF) + 1;
    src = (u8 *)D_80077140 + (c << 6);
    memset(buf, 0, 0x10);
    for (row = 0; row < 12; row++) {
        for (i = 0; i < 4; i++) {
            b = src[row * 4 + i];
            for (j = 0; j < 4; j++) {
                buf[i * 4 + j] = b & 3;
                b >>= 2;
            }
        }
        for (i = 0; i < width; i++) {
            px = x + i;
            if (!(px & 1)) {
                p = dst + (px >> 1);
                v = buf[start + i];
                o = *p;
                if (hi) { *p = o | (v << 2); } else { *p = o | v; }
            } else {
                v = buf[start + i];
                px >>= 1;
                p = dst + px;
                o = *p;
                t = v << 4; if (hi) { *p = o | (v << 6); } else { *p = o | t; }
            }
        }
        dst += 0x54;
    }
    return width + x;
}

s32 func_80028F3C(s32 x, s32 y, s32 otz, u8 c, s32 mode) {
    u32 *tbl = g_gameState->unk1C->unk7D4->unkC;
    u32 g;
    s32 w;
    SprtWords *p;

    if (c == 0xFF) {
        return 0;
    }
    g = tbl[c];
    setlen(g_primPtr, 4);
    w = (g >> 16) & 0xF;
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = (x + ((10 - w) >> 1)) | (y << 16);
    p->uvclut = (g & 0x40FFFF) | (D_8006BBA0[mode] << 16);
    p->wh = w | 0xD0000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    return g >> 28;
}

void func_80029058(s32 x, s32 y, s32 otz, u8 *s, s32 pal) {
    Unk7D4 *f = g_gameState->unk1C->unk7D4;
    s32 th = f->unk4 >> 8;
    SprtWords *p = (SprtWords *)g_primPtr;
    u32 *ot = &g_ot[otz];
    s32 spacing;
    s32 c;
    u32 g;
    u32 w;
    s32 gx;
    s32 k;

    while ((c = *s) != 0xFF) {
        if (c >= th) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
        }
        g = *(f->unkC + c);
        setlen(p, 4);
        k = (g >> 20) & 3;
        p->rgbc = 0x64808080;
        w = g >> 28;
        spacing = 1;
        gx = x + k;
        x = x + spacing + w;
        p->xy = (gx & 0xFFFF) | (y << 16);
        p->uvclut = (g & 0x40FFFF) + (D_8006BBA0[pal] << 16);
        p->wh = ((g >> 16) & 0xF) | 0xD0000;
        addPrim(ot, p);
        p++;
        s++;
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = 0xE100061E;
    addPrim(ot, p);
    g_primPtr = (u8 *)p + 8;
}

u8 *func_8002920C(s32 x, s32 y, u32 *ot, u8 *s, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    s32 c;
    s32 k;

    font = g_gameState->unk1C->unk7D4;
    D_800770AC = mode;
    x0 = x;
    th = font->unk4 >> 8;
    while ((c = *s) != 0xFF) {
        if (c == 0xF9) {
            s++;
            if (*s == 0x43) {
                s++;
                x += (s8)*s;
            } else if (*s == 0x44) {
                s++;
                x = x0 + *s;
            } else if (*s == 0x49) {
                s++;
                y += *s;
            } else if (*s == 0x4A) {
                s++;
                y -= *s;
            } else if (*s == 0x40) {
                s++;
                func_8002BBE8(D_8007BBA8[*s & 7], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x68) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 0);
                pkt = func_8002995C(x, y, ot, (Character *)D_80077080, &w, pkt);
                x += w;
            } else if (*s == 0x69) {
                s++;
                pkt = func_8002995C(x, y, ot, (Character *)D_800770B0[*s], &w, pkt);
                x += w;
            } else if (*s == 0x6A) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_80029D60(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002B9B8(D_80077080);
            } else if (*s == 0x6B) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = func_80029D60(x, y, ot, D_800770A8, D_800770AC, pkt);
                x += func_8002B9B8(D_800770A8);
            } else if (*s == 0x6E) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_80029B4C(x, y, ot, (s32)D_80077080, D_800770AC, pkt);
                x += func_8002AE7C(D_80077080);
            } else if (*s == 0x6F) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_80029B4C(x, y, ot, (s32)D_800770A8, D_800770AC, pkt);
                x += func_8002AE7C(D_800770A8);
            } else if (*s == 0x6C) {
                s++;
                func_8002BBE8(D_800770F0[*s], D_80077080, 1);
                pkt = func_8002B644(x, y, ot, D_80077080, D_800770AC, pkt);
                x += func_8002BA10(D_80077080);
            } else if (*s == 0x6D) {
                s++;
                D_800770A8 = D_800770B0[*s];
                pkt = func_8002B644(x, y, ot, D_800770A8, D_800770AC, pkt);
                x += func_8002BA10(D_800770A8);
            } else if (*s == 0x4B) {
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = func_80027834(x, y, ot, *++s, 0, &w, pkt);
                x += w;
            } else if (*s == 0x4C) {
                s++;
                k = *s;
                if (k != 0xFF) {
                    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                    pkt = func_80026C7C(x, y, ot, k, 1, pkt);
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else if (*s == 0x4D) {
                pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                pkt = func_80027834(x, y, ot, D_80077130[*++s], 0, &w, pkt);
                x += w;
            } else if (*s == 0x4E) {
                s++;
                k = D_80077130[*s];
                if (k != 0xFF) {
                    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
                    pkt = func_80026C7C(x, y, ot, k, 1, pkt);
                    x += D_800686D0[D_80069018[k].unk8 + 0x5C].unk4;
                }
            } else {
                pkt = func_80029FE4(x, y, ot, *s, &w, pkt);
                x += w;
            }
        } else if (c == 0xF7) {
            x = x0;
            y += 15;
        } else {
            if (c >= th) {
                c = (c << 8) + s[1] - font->unk4 + font->unk6;
                s++;
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
                x += w;
            } else {
                pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &w, (SprtWords *)pkt);
                x += w;
            }
        }
        s++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = 0xE100061E;
    addPrim(ot, pkt);
    pkt += sizeof(DrTpageWords);
    return pkt;
}

u8 *func_8002995C(s32 x, s32 y, u32 *ot, Character *ch, s32 *w, u8 *pkt) {
    s32 adv;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    Unk7D4 *kanji;
    s32 th2;
    u8 *s;
    s32 c;
    s32 code;

    font = g_gameState->unk1C->unk7D4;
    s = ch->unk0;
    x0 = x;
    th = font->unk4 >> 8;
    kanji = g_gameState->unk1C->unk84C;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != 0xFF) {
        if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            s++;
            c = code - kanji->unk0;
            pkt = func_8002B7B0(x, y, ot, *(kanji->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        } else if (c >= th) {
            code = (c << 8) + s[1];
            s++;
            c = code - font->unk4 + font->unk6;
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        } else {
            pkt = func_8002B7B0(x, y, ot, *(font->unkC + c), D_800770AC, &adv, (SprtWords *)pkt);
            x += adv;
        }
        s++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = 0xE100061E;
    addPrim(ot, pkt);
    *w = x - x0;
    return pkt + 8;
}

u8 *func_80029B4C(s32 x, s32 y, u32 *ot, s32 str, s32 mode, u8 *pkt) {
    u8 *s = (u8 *)str;
    SprtWords *p = (SprtWords *)pkt;
    SpriteDesc16 *g = NULL;
    SpriteDesc16 *tbl;
    u32 rgbc;
    u16 clut;
    s32 c;
    s32 w;

    rgbc = 0x64404040;
    if (mode == 0) {
        rgbc = 0x64808080;
    }
    tbl = (SpriteDesc16 *)D_800686D0;
    clut = tbl[46].clut;
    if (mode == 1) {
        clut += 0x40;
        rgbc = 0x66000000;
    }
    if (s == NULL) {
        return (u8 *)p;
    }
    while ((c = *s) != 0) {
        if ((u32)(c - 'A') < 26) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 8];
        } else if ((u32)(c - 'a') < 26) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 0x28];
        } else if ((u32)(c - '0') < 10) {
            g = &((SpriteDesc16 *)D_800686D0)[*s - 2];
        } else if ((u8)c == '&') {
            g = (SpriteDesc16 *)&D_80068890;
        } else if ((u8)c == '.') {
            g = (SpriteDesc16 *)&D_80068CE0;
        } else if ((u8)c == '_') {
            g = (SpriteDesc16 *)&D_80068DC8;
        }
        if (*s != ' ') {
            setlen(p, 4);
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            w = g->w;
            p->uvclut = g->uv | (clut << 16);
            p->wh = g->w | (g->h << 16);
            addPrim(ot, p);
            p++;
        } else {
            w = 5;
        }
        s++;
        x += w - 1;
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = 0xE100061F;
    addPrim(ot, p);
    return (u8 *)p + 8;
}

u8 *func_80029D60(s32 x, s32 y, u32 *ot, u8 *s, s32 pal, u8 *pkt) {
    Sprt8Fields *p = (Sprt8Fields *)pkt;
    u8 kana = 0;
    s32 hi = 0;
    s32 c;
    s32 code;
    s32 row;
    s32 u;
    s32 v;
    s32 k;
    Sprt8Fields *q;

    if (*s == 0x40) {
        kana = 1;
        s++;
    }
    while ((c = *s) != 0) {
        if ((u8)c == 0x40) {
            kana ^= 1;
            s++;
        } else if ((u8)c == 0x20) {
            x += 7;
            s++;
        } else {
            if ((u32)(c - 0x21) < 0x3F) {
                code = *s - 0x20;
                if (*s == 0x5C) {
                    code = *s - 0x4D;
                }
                goto calc;
            } else if ((u32)(c - 0x61) < 0x1A) {
                code = *s - 0x40;
            calc:
                row = code >> 5;
                hi = (row >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (row % 3) * 8;
            } else if ((u8)c >= 0xA0) {
                code = *s - 0xA0;
                k = (code >> 5) + 2;
                if (!kana) {
                    k = (code >> 5) + 4;
                }
                hi = (k >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (k % 3) * 8;
            } else {
                v = 0;
                u = 0;
            }
            setlen(p, 3);
            p->rgbc = 0x74808080;
            q = p;
            if ((u8)(*s + 0x22) < 2) {
                p->x = x - 7;
                p->y = y - 7;
            } else {
                p->x = x;
                x += 7;
                p->y = y;
            }
            q->u = u;
            q->v = v - 0x80;
            q->clut = D_8006BBA0[pal] | hi;
            addPrim(ot, p);
            p++;
            s++;
        }
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = 0xE100061F;
    hi = getaddr(ot); /* reuses hi as the tag temp */
    setaddr(p, hi), setaddr(ot, p);
    return (u8 *)p + 8;
}

u8 *func_80029FE4(s32 x, s32 y, u32 *ot, s32 code, s32 *w, u8 *pkt) {
    pkt = func_8002BA90(ot, (DrTpageWords *)pkt);
    switch (code & 0xFF) {
    case 0x21:
        D_800770AC = 1;
        *w = 0;
        break;
    case 0x23:
        D_800770AC = 3;
        *w = 0;
        break;
    case 0x26:
        D_800770AC = 6;
        *w = 0;
        break;
    case 0x27:
        D_800770AC = 7;
        *w = 0;
        break;
    case 0x25:
        D_800770AC = 5;
        *w = 0;
        break;
    case 0x24:
        D_800770AC = 4;
        *w = 0;
        break;
    case 0x20:
        D_800770AC = 0;
        *w = 0;
        break;
    case 0x22:
        D_800770AC = 2;
        *w = 0;
        break;
    case 0x28:
        D_800770AC = 8;
        *w = 0;
        break;
    case 0x29:
        D_800770AC = 9;
        *w = 0;
        break;
    case 0x30:
        pkt = func_80027834(x, y + 2, ot, 5, 0, w, pkt);
        break;
    case 0x31:
        pkt = func_80027834(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x32:
        pkt = func_80027834(x, y + 2, ot, 7, 0, w, pkt);
        break;
    case 0x33:
        pkt = func_80027834(x, y + 2, ot, 6, 0, w, pkt);
        break;
    case 0x34:
        pkt = func_80027834(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x35:
        pkt = func_80027834(x, y + 2, ot, 10, 0, w, pkt);
        break;
    case 0x36:
        pkt = func_80027834(x, y + 2, ot, 9, 0, w, pkt);
        break;
    case 0x37:
        pkt = func_80027834(x, y + 2, ot, 11, 0, w, pkt);
        break;
    case 0x60:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(1), 0, w, pkt);
        break;
    case 0x61:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(0), 0, w, pkt);
        break;
    case 0x62:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(2), 0, w, pkt);
        break;
    case 0x63:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(3), 0, w, pkt);
        break;
    case 0x64:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(4), 0, w, pkt);
        break;
    case 0x65:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(5), 0, w, pkt);
        break;
    case 0x66:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(6), 0, w, pkt);
        break;
    case 0x67:
        pkt = func_80027834(x, y + 2, ot, func_8002BE10(7), 0, w, pkt);
        break;
    case 0x38:
        pkt = func_80027834(x, y + 2, ot, 0x11, 0, w, pkt);
        break;
    case 0x39:
        pkt = func_80027834(x, y + 2, ot, 0x12, 0, w, pkt);
        break;
    case 0x3A:
        pkt = func_8002B890(x, y + 2, ot, 1, 0, w, pkt);
        break;
    case 0x3B:
        pkt = func_8002B890(x, y + 2, ot, 2, 0, w, pkt);
        break;
    case 0x3C:
        pkt = func_8002B890(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x3D:
        pkt = func_8002B890(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x3E:
        pkt = func_8002B890(x, y + 2, ot, 3, 0, w, pkt);
        break;
    case 0x3F:
        pkt = func_8002B890(x, y + 2, ot, 12, 0, w, pkt);
        break;
    case 0x10:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[0], w, pkt);
        break;
    case 0x11:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[1], w, pkt);
        break;
    case 0x12:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[2], w, pkt);
        break;
    case 0x13:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[3], w, pkt);
        break;
    case 0x14:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[4], w, pkt);
        break;
    case 0x15:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[5], w, pkt);
        break;
    case 0x16:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[6], w, pkt);
        break;
    case 0x17:
        pkt = func_8002995C(x, y, ot, &g_gameState->unk1C->characters[7], w, pkt);
        break;
    case 0x18:
        if (g_gameState->unk1C->party[0] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[0], w, pkt);
        }
        break;
    case 0x19:
        if (g_gameState->unk1C->party[1] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[1], w, pkt);
        }
        break;
    case 0x1A:
        if (g_gameState->unk1C->party[2] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[2], w, pkt);
        }
        break;
    case 0x1B:
        if (g_gameState->unk1C->party[3] != NULL) {
            pkt = func_8002995C(x, y, ot, g_gameState->unk1C->party[3], w, pkt);
        }
        break;
    }
    return pkt;
}

s32 func_8002A730(s32 c) {
    s32 r = 0;

    switch (c & 0xFF) {
    case 0x30:
        r = D_800686D0[5].unk4;
        break;
    case 0x31:
        r = D_800686D0[4].unk4;
        break;
    case 0x32:
        r = D_800686D0[7].unk4;
        break;
    case 0x33:
        r = D_800686D0[6].unk4;
        break;
    case 0x34:
        r = D_800686D0[8].unk4;
        break;
    case 0x35:
        r = D_800686D0[10].unk4;
        break;
    case 0x36:
        r = D_800686D0[9].unk4;
        break;
    case 0x37:
        r = D_800686D0[11].unk4;
        break;
    case 0x60:
        r = D_800686D0[func_8002BE10(0)].unk4;
        break;
    case 0x61:
        r = D_800686D0[func_8002BE10(1)].unk4;
        break;
    case 0x62:
        r = D_800686D0[func_8002BE10(2)].unk4;
        break;
    case 0x63:
        r = D_800686D0[func_8002BE10(3)].unk4;
        break;
    case 0x64:
        r = D_800686D0[func_8002BE10(4)].unk4;
        break;
    case 0x65:
        r = D_800686D0[func_8002BE10(5)].unk4;
        break;
    case 0x66:
        r = D_800686D0[func_8002BE10(6)].unk4;
        break;
    case 0x67:
        r = D_800686D0[func_8002BE10(7)].unk4;
        break;
    case 0x38:
        r = D_800686D0[17].unk4;
        break;
    case 0x39:
        r = D_800686D0[18].unk4;
        break;
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3D:
    case 0x3E:
    case 0x3F:
        r = D_800686D0[12].unk4;
        break;
    case 0x10:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[0]);
        break;
    case 0x11:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[1]);
        break;
    case 0x12:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[2]);
        break;
    case 0x13:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[3]);
        break;
    case 0x14:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[4]);
        break;
    case 0x15:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[5]);
        break;
    case 0x16:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[6]);
        break;
    case 0x17:
        r = func_8002BA68((s32)&g_gameState->unk1C->characters[7]);
        break;
    case 0x18:
        if (g_gameState->unk1C->party[0] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[0]);
        } else {
            r = 0;
        }
        break;
    case 0x19:
        if (g_gameState->unk1C->party[1] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[1]);
        } else {
            r = 0;
        }
        break;
    case 0x1A:
        if (g_gameState->unk1C->party[2] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[2]);
        } else {
            r = 0;
        }
        break;
    case 0x1B:
        if (g_gameState->unk1C->party[3] != NULL) {
            r = func_8002BA68((s32)g_gameState->unk1C->party[3]);
        } else {
            r = 0;
        }
        break;
    }
    return r;
}

s32 func_8002AA04(s32 frames, u8 *buf, s32 blink) {
    s32 r = 0;
    s32 sec;
    s32 min;
    s32 hr;
    u8 sub;

    sec = (u32)frames / 60;
    min = sec / 60;
    sec %= 60;
    hr = min / 60;
    min %= 60;
    if (hr >= 601) {
        hr = 599;
        sec = 59;
        min = 59;
    }
    if (hr >= 100) {
        r = D_8006BBD4[hr / 100];
        hr %= 100;
    }
    if ((u8)blink) {
        sub = (u32)frames % 60;
    } else {
        sub = 0;
    }
    text_formatNumber_inline(hr, buf, 2, 1);
    text_formatNumber_inline(min, buf + 3, 2, 1);
    if (buf[3] == ' ') {
        buf[3] = '0';
    }
    text_formatNumber_inline(sec, buf + 6, 2, 1);
    if (buf[6] == ' ') {
        buf[6] = '0';
    }
    if ((u8)sub < 30) {
        buf[2] = ':';
        buf[5] = ':';
    } else {
        buf[5] = ' ';
        buf[2] = ' ';
    }
    if (hr < 10) {
        buf[0] = '0';
    }
    return r;
}

u8 *func_8002AD38(u8 *dst, u8 *src) {
    s32 c;
    u8 th;
    u8 *p;
    u8 *ret;
    u8 *s;
    s32 c2;
    u8 th2;

    ret = dst;
    s = ret;
    p = s;
    th = g_gameState->unk1C->unk7D4->unk4 >> 8;
    while ((c = *p) != 0xFF) {
        p++;
        if (c == 0xF9) {
            p++;
            if ((u32)(p[-2] - 0x43) < 2) {
                p++;
            }
        } else if (c >= th) {
            p++;
        }
    }
    dst += p - s;
    th2 = g_gameState->unk1C->unk7D4->unk4 >> 8;
    while ((c2 = *src) != 0xFF) {
        *dst++ = *src++;
        if (c2 == 0xF9) {
            *dst++ = *src++;
            if ((u32)(src[-2] - 0x43) < 2) {
                *dst++ = *src++;
            }
        } else if (c2 >= th2) {
            *dst++ = *src++;
        }
    }
    *dst = 0xFF;
    return ret;
}

s32 func_8002AE7C(u8 *s) {
    s32 w;
    Glyph8 *g;
    u8 c;
    s32 adv;

    w = 0;
    g = NULL;
    while ((c = *s) != 0) {
        if (c >= 'A' && c <= 'Z') {
            g = &D_80068690[*s];
        } else if (c >= 'a' && c <= 'z') {
            g = &D_80068690[*s - 32];
        } else if (c >= '0' && c <= '9') {
            g = &D_80068690[*s + 6];
        } else if (c == '&') {
            g = &D_80068890;
        } else if (c == '.') {
            g = &D_80068CE0;
        } else if (c == '_') {
            g = &D_80068DC8;
        }
        if (*s == ' ') {
            adv = 5;
        } else {
            adv = g->unk4;
        }
        s++;
        w += adv - 1;
    }
    return w;
}

TextSize func_8002AF70(u8 *s) {
    TextSize r;
    s32 w;
    s32 maxw;
    s32 h;
    s32 c;
    s32 code;
    Unk7D4 *font;
    Unk7D4 *kanji;
    u8 th;
    u8 th2;
    s32 idx;
    Unk7D4 *tbl;

    memset(&r, 0, 4);
    w = 0;
    maxw = 0;
    font = g_gameState->unk1C->unk7D4;
    kanji = g_gameState->unk1C->unk84C;
    h = 15;
    th2 = 0;
    if (s == NULL) {
        return r;
    }
    th = font->unk4 >> 8;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != 0xFF) {
        if (c == 0xF9) {
            s++;
            if (*s == 0x43) {
                s++;
                w += (s8)*s;
            } else if (*s == 0x44) {
                s++;
                w = *s;
            } else if (*s == 0x40) {
                s++;
                w += func_8002BEA4(D_8007BBA8[*s], 0);
            } else if (*s == 0x68) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 0);
            } else if (*s == 0x69) {
                s++;
                w += D_8006BBEC[0](D_800770B0[*s]);
            } else if (*s == 0x6A) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 1);
            } else if (*s == 0x6B) {
                s++;
                w += D_8006BBEC[1](D_800770B0[*s]);
            } else if (*s == 0x6E) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 2);
            } else if (*s == 0x6F) {
                s++;
                w += D_8006BBEC[2](D_800770B0[*s]);
            } else if (*s == 0x6C) {
                s++;
                w += func_8002BEA4(D_800770F0[*s], 3);
            } else if (*s == 0x6D) {
                s++;
                w += D_8006BBEC[3](D_800770B0[*s]);
            } else if (*s == 0x4B) {
                s++;
                w += D_800686D0[*s].unk4;
            } else if (*s == 0x4C) {
                s++;
                if (*s == 0xFF) {
                    w += D_800686D0[D_8006A20E + 0x5C].unk4;
                }
            } else if (*s == 0x4D) {
                s++;
                w += D_800686D0[D_80077130[*s]].unk4;
            } else if (*s == 0x4E) {
                s++;
                if (D_80077130[*s] == 0xFF) {
                    w += D_800686D0[D_8006A20E + 0x5C].unk4;
                }
            } else if (*s == 0x49) {
                s++;
                h += *s;
            } else if (*s == 0x4A) {
                s++;
                h -= *s;
            } else {
                w += func_8002A730(*s);
            }
        } else if (c == 0xF7) {
            h += 15;
            if (maxw < w) {
                maxw = w;
            }
            w = 0;
        } else if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            c = code - kanji->unk0;
            s++;
            w += 1 + (*(kanji->unkC + c) >> 28);
        } else if (c < th) {
            w += 1 + (*(font->unkC + c) >> 28);
        } else {
            code = (c << 8) + s[1];
            c = code - font->unk4 + font->unk6;
            s++;
            w += 1 + (*(font->unkC + c) >> 28);
        }
        s++;
    }
    r.w = maxw > w ? maxw : w;
    r.h = h;
    return r;
}

s32 func_8002B43C(s32 arg0) {
    D_80077140 = arg0;
    D_80077144 = arg0 + 0x4000;
    memset(D_80077144, 0, 0x3F0);
    return D_80077144;
}

void func_8002B47C(s32 arg0) {
    s32 x = (arg0 & 1) * 0x54;
    s32 y = arg0 >> 1;
    s32 i;
    u8 *name = g_gameState->unk1C->party[arg0]->unk0;

    for (i = 0; i < 7; i++) {
        if (*name == 0xFF) {
            break;
        }
        x = func_80028DB0(x, y, *name++);
    }
}

void func_8002B524(s32 arg0, s32 arg1, s32 otz, s32 arg3, s32 arg4) {
    g_primPtr = func_80029B4C(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void func_8002B56C(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_80029D60(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void func_8002B5B4(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_8002B644(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void func_8002B5FC(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = func_8002920C(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

u8 *func_8002B644(s32 x, s32 y, u32 *ot, u8 *s, s32 style, void *pkt) {
    SprtWords *p = pkt;
    DrTpageWords *t;
    u32 rgbc = 0x64808080;
    s32 c;
    s32 u;

    if (style == 2) {
        rgbc = 0x66606060;
    }
    while ((c = *s) != 0) {
        if ((u8)c != ' ') {
            do { } while (0);
            if ((u8)c != '/') {
                u = 0x1E8;
                u = c * 8 - u;
            } else {
                u = 0xE8;
            }
            setlen(p, 4);
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            p->wh = 0xA0008;
            p->uvclut = ((u & 0xFF) | 0x4000) | ((D_8006BBB4[style] + 0x3EE4) << 16);
            addPrim(ot, p);
            p++;
        }
        s++;
        x += 7;
    }
    t = (DrTpageWords *)p;
    setlen(t, 1);
    t->code = 0xE100061F;
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

u8 *func_8002B7B0(s32 x, s32 y, u32 *ot, u32 a, s32 mode, s32 *out, SprtWords *p) {
    u32 rgbc = 0x66000000;
    s32 dx = (a >> 20) & 3;
    s32 h = (a >> 16) & 0xF;
    s32 n = a >> 28;
    if (mode != 1) {
        rgbc = 0x64808080;
    }
    setlen(p, 4);
    p->xy = ((x + dx) & 0xFFFF) | (y << 16);
    p->rgbc = rgbc;
    p->uvclut = (a & 0x40FFFF) + (D_8006BBA0[mode] << 16);
    p->wh = h | 0xD0000;
    addPrim(ot, p);
    if (out != NULL) {
        *out = n + 1;
    }
    return (u8 *)(p + 1);
}

u8 *func_8002B890(s32 x, s32 y, u32 *ot, u32 mask, u8 b, s32 *c, u8 *pkt) {
    s32 tmp[2];
    s32 i;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            pkt = func_80027834(x + D_8006BC0C[i].unk0, y + D_8006BC0C[i].unk1, ot, D_8006BC0C[i].unk2, b, tmp, pkt);
        }
    }
    return func_80027834(x, y, ot, 0xC, b, c, pkt);
}

s32 func_8002B980(u8 *s, s32 idx) {
    return D_8006BBEC[idx](s);
}

s32 func_8002B9B8(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 func_8002BA10(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 func_8002BA68(s32 arg0) {
    return func_8002AF70((u8 *)arg0).w;
}

u8 *func_8002BA90(u32 *ot, DrTpageWords *p) {
    setlen(p, 1);
    p->code = 0xE100061E;
    addPrim(ot, p);
    return (u8 *)(p + 1);
}

void text_formatNumber(s32 n, u8 *buf, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 q;
    s32 d;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        func_8002BBE8(n, buf, ascii);
        return;
    }
    memset(buf, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        buf[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = buf + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}

void func_8002BBE8(s32 v, u8 *buf, s32 ascii) {
    u8 tmp[16];
    s32 n = 0;
    s32 i;
    s32 base;
    s32 minus;
    s32 term;
    if (ascii == 0) {
        base = 0;
        minus = 0xB;
        term = 0xFF;
    } else {
        base = '0';
        minus = '-';
        term = 0;
    }
    if (v == 0) {
        *buf++ = base;
    } else {
        if (v < 0) {
            *buf++ = minus;
            v = -v;
        }
        while (v != 0) {
            tmp[n++] = base + v % 10;
            v /= 10;
        }
        for (i = 0; i < n;) {
            *buf++ = tmp[n - ++i];
        }
    }
    *buf = term;
}

s32 func_8002BCC0(u8 *s) {
    u8 *start = s;
    u8 lim = g_gameState->unk1C->unk7D4->unk4 >> 8;
    s32 c;

    while ((c = *s) != 0xFF) {
        s++;
        if (c == 0xF9) {
            s++;
            if ((u32)(s[-2] - 0x43) < 2) {
                s++;
            }
        } else if (c >= lim) {
            s++;
        }
    }
    return s - start;
}

u8 *func_8002BD54(u8 *dst, u8 *src) {
    u8 *start = dst;
    u8 lim = g_gameState->unk1C->unk7D4->unk4 >> 8;
    s32 c;

    while ((c = *src) != 0xFF) {
        *dst++ = *src++;
        if (c == 0xF9) {
            *dst++ = *src++;
            if ((u32)(src[-2] - 0x43) < 2) {
                *dst++ = *src++;
            }
        } else if (c >= lim) {
            *dst++ = *src++;
        }
    }
    *dst = 0xFF;
    return start;
}

s32 func_8002BE10(void) {
    return D_8006BC18[func_80023938()];
}

void func_8002BE44(s32 i, u8 *v) {
    D_800770B0[i] = v;
}

void func_8002BE5C(s32 i, s32 v) {
    D_800770F0[i] = v;
}

void func_8002BE74(s32 i, s32 v) {
    D_80077130[i] = v;
}

s32 func_8002BE88(void) {
    return D_800770AC;
}

void func_8002BE98(s32 v) {
    D_800770AC = v;
}

s32 func_8002BEA4(s32 n, s32 idx) {
    s32 r = 0;
    s32 step = D_8006BBFC[idx];

    while (n != 0) {
        n /= 10;
        r += step;
    }
    return r - 1;
}

void func_8002BEEC(u32 *ot0, u32 *ot1, s32 pk0, s32 pk1) {
    GameState *gs = g_gameState;
    g_drawBufs.unk14[0] = ot0;
    g_drawBufs.unk14[1] = ot1;
    g_drawBufs.unkC[0] = (u8 *)pk0;
    g_drawBufs.unkC[1] = (u8 *)pk1;
    g_drawBufs.unk1C[0].vx = gs->unkC[0].disp.x;
    g_drawBufs.unk1C[0].vy = gs->unkC[0].disp.y;
    g_drawBufs.unk1C[1].vx = gs->unkC[1].disp.x;
    g_drawBufs.unk1C[1].vy = gs->unkC[1].disp.y;
    g_drawBufs.unk0 = gs->unk1C->unk8;
    g_primPtr = g_drawBufs.unkC[g_drawBufs.unk0];
    g_ot = g_drawBufs.unk14[g_drawBufs.unk0];
    func_8002C210();
    func_8002E490();
    g_drawBufs.unk1 = 0;
    g_drawBufs.unk8 = VSync(-1);
    g_drawBufs.unk2 = 0;
}

void func_8002BFD0(void) {
    s32 now = VSync(-1);
    s32 dt = now - D_80077150;
    s32 i;

    if (dt == 0) {
        dt = 1;
    }
    if (dt >= 16) {
        dt = 1;
    }
    for (i = 0; i < dt; i++) {
        func_8002E180();
    }
    func_8002C9B0();
    D_80077150 = now;
}

void func_8002C05C(void) {
}

void func_8002C064(s32 arg0) {
    g_drawBufs.unk1 = arg0;
    if (arg0 == 2) {
        g_drawBufs.unk8 = 0;
    }
}

void gfx_selectBuffer(s32 i) {
    g_drawBufs.unk4 = (g_drawBufs.unkC[1] - g_drawBufs.unkC[0]) - (g_primPtr - g_drawBufs.unkC[i ^ 1]);
    g_ot = g_drawBufs.unk14[i];
    g_primPtr = g_drawBufs.unkC[i];
}

void func_8002C0DC(s32 i) {
    u8 *prev = g_drawBufs.unkC[i ^ 1];
    g_drawBufs.unk0 = i;
    g_drawBufs.unk4 = (g_drawBufs.unkC[1] - g_drawBufs.unkC[0]) - (g_primPtr - prev);
    g_ot = g_drawBufs.unk14[i];
    g_primPtr = g_drawBufs.unkC[i];
}

void func_8002C138(u8 a) {
    g_drawBufs.unk0 = a;
}

void func_8002C144(u32 *a, u8 *b) {
    g_ot = a;
    g_primPtr = b;
}

void func_8002C158(s32 a) {
    s32 buf = func_8002B43C(a);
    s32 i;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            func_8002B47C(i);
        }
    }
    LoadImage(&D_8006BC38, (u32 *)buf);
    DrawSync(0);
}

void func_8002C1E4(void) {
}

s32 func_8002C1EC(void) {
    return D_80077150 = VSync(-1);
}

void func_8002C210(void) {
    s32 p;
    s32 i;
    u32 v;
    D_80077178 = 1;
    v = g_padPortHeld[0];
    g_padRepeat = v;
    g_padPressed = v;
    g_padHeld = v;
    func_80019750(&D_8007717C);
    for (p = 0; p < 2; p++) {
        func_8002C52C(p);
        for (i = 0; i < 23; i++) {
            func_8002C5CC(p, i);
            func_8002C740(p, i, D_8006BC40[i].unk4);
            func_8002C974(p, i, D_8006BC40[i].unk0);
            func_8002C870(p, i, D_8006BC40[i].unk5);
            func_8002C8AC(p, i, D_8006BC40[i].unk6);
        }
    }
}

void func_8002C308(s32 port) {
    u32 res = 0;
    Unk80077188 *e = D_80077188[port];
    u32 now = VSync(-1);
    s32 i;
    u32 m;
    u32 out;
    u32 d;

    g_padHeld = g_padPortHeld[port];
    g_padPressed = g_padPortPressed[port];
    for (i = 0; i < 23; e++, i++) {
        if (e->unk0 != 0) {
            m = g_padHeld & e->unkC & e->unk4;
            e->unk4 = g_padHeld;
            out = 0;
            if (m != 0) {
                if (e->unk2 != 0) {
                    d = now - e->unk8;
                    if (e->unk1 == 0) {
                        if (d >= e->unk10) {
                            out = m;
                            e->unk1 = 1;
                            e->unk8 = now;
                        }
                    } else if (d >= e->unk12) {
                        out = m;
                        e->unk8 = now;
                    }
                }
            } else {
                e->unk1 = 0;
                e->unk2 = 1;
                e->unk8 = now;
            }
            res |= out;
        }
    }
    g_padRepeat = res | g_padPressed;
}

s32 func_8002C47C(s32 i) {
    return ((D_80073940[i].unk0 >> 12) & 0xF) == 7;
}

void func_8002C4B0(u8 on) {
    if (on) {
        D_80077178 = func_80019150() >> 15;
        func_80019160(0x8000, 1);
        func_80019750(&D_8007717C);
        func_80019774(&D_8006BCF8);
    } else {
        func_80019160(0x8000, D_80077178);
        func_80019774(&D_8007717C);
    }
}

void func_8002C52C(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C5CC(a, i);
        func_8002C740(a, i, 0);
    }
}

void func_8002C584(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C5CC(a, i);
    }
}

void func_8002C5CC(s32 a, s32 b) {
    Unk80077188 *e = &D_80077188[a][b];
    e->unk4 = g_padPortHeld[a];
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
}

void func_8002C64C(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C694(a, i);
    }
}

void func_8002C694(s32 a, s32 b) {
    Unk80077188 *e = &D_80077188[a][b];
    e->unk4 = 0;
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
}

u8 func_8002C700(s32 a, s32 b) {
    return D_80077188[a][b].unk0;
}

void func_8002C740(s32 a, s32 b, s32 c) {
    Unk80077188 *e = &D_80077188[a][b];

    e->unk4 = g_padPortHeld[a];
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
    D_80077188[a][b].unk0 = c;
}

u16 func_8002C7F0(s32 a, s32 b) {
    return D_80077188[a][b].unk10;
}

u16 func_8002C830(s32 a, s32 b) {
    return D_80077188[a][b].unk12;
}

void func_8002C870(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unk10 = v;
}

void func_8002C8AC(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unk12 = v;
}

void func_8002C8E8(s32 a, s32 b, s32 c, s32 d) {
    func_8002C870(a, b, c);
    func_8002C8AC(a, b, d);
}

s32 func_8002C934(s32 a, s32 b) {
    return D_80077188[a][b].unkC;
}

void func_8002C974(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unkC = v;
}

void func_8002C9B0(void) {
    Window *w;
    s32 i;

    w = D_80077530;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && func_8002E4BC(w)) {
            if (w->unk24 != 0) {
                func_8002E588(w);
            }
            if (w->unk44 != NULL) {
                w->unk44(w);
            }
            if (w->unk1C == 3 && w->unk30.unk3 < w->unk30.unk2) {
                func_8002E718(&w->unk8, w->unk18, &w->unk30);
            }
            func_8002D9B4(NULL, w->unk18, w->unk1A);
            if (w->unk40 != NULL && w->unk1E != 0) {
                w->unk40(w);
            }
            func_8002D9B4(&w->unk8, w->unk18, w->unk1A);
            if (w->unk1A != 4) {
                func_8002CB18(w);
            }
            if (w->unk1F != 0) {
                func_8002705C(&w->unk8, w->unk18, w->unk1A);
            }
        }
    }
}

void func_8002CB18(Window *w) {
    s32 abr;
    RECT t;

    abr = g_gameState->unk24 >> 18;
    abr &= 1;

    if (w->unk1A == 2) {
        func_80026B30(&w->unk8, w->unk18, w->unk1E, w->unk1D, abr);
        if (w->unk1E == 0) {
            return;
        }
        t.x = w->unk8.x + 4;
        t.y = w->unk8.y + 4;
        t.w = w->unk8.w - 8;
        t.h = w->unk8.h - 8;
    } else if (w->unk1A == 3) {
        if (w->unk29 != 4) {
            func_8002E660(w);
        }
        func_80026B84(&w->unk8, w->unk18, w->unk1E, w->unk1D, abr);
        if (w->unk1E == 0) {
            return;
        }
        t.x = w->unk8.x + 2;
        t.y = w->unk8.y + 2;
        t.w = w->unk8.w - 4;
        t.h = w->unk8.h - 4;
    } else {
        if (w->unk1A == 1) {
            t.x = w->unk8.x;
            t.y = w->unk8.y + 4;
            t.w = w->unk8.w;
            t.h = w->unk8.h - 4;
            func_80025A28(w->unk8.x, w->unk8.y, w->unk18, 0, w->unk8.w, abr);
            func_80025108(&t, w->unk18, 1, abr);
        } else {
            t = w->unk8;
            func_80025108(&t, w->unk18, 0, abr);
        }
        if (w->unk1E == 0) {
            return;
        }
        if (w->unk30.unk8 & 4) {
            func_8002E7FC(&t, w->unk18, &w->unk30, abr);
            return;
        }
    }
    func_8002709C(&t, w->unk18, w->unk30.unkA, w->unk30.unkB, abr);
}

void func_8002CD4C(RECT *r, s32 otz, s32 style, s32 abr) {
    RECT t;

    switch (style) {
    case 2:
        func_80026B30(r, otz, 1, 0, abr);
        t.x = r->x + 4;
        t.y = r->y + 4;
        t.w = r->w - 8;
        t.h = r->h - 8;
        func_8002709C(&t, otz, 0, 0, abr);
        break;
    case 1:
        t.x = r->x;
        t.y = r->y + 4;
        t.w = r->w;
        t.h = r->h - 4;
        func_80025A28(r->x, r->y, otz, 0, r->w - 2, abr);
        func_80025108(&t, otz, 1, abr);
        func_8002709C(&t, otz, 0, 0, abr);
        break;
    case 0:
        func_80025108(r, otz, 0, abr);
        func_8002709C(r, otz, 0, 0, abr);
        break;
    case 3:
        func_80026B84(r, otz, 1, 0, abr);
        t.x = r->x + 2;
        t.y = r->y + 2;
        t.w = r->w - 3;
        t.h = r->h - 3;
        func_8002709C(&t, otz, 0, 0, abr);
        break;
    }
}

void func_8002CF20(Window *w, s32 mode) {
    s16 h;

    w->unk28 = mode;
    if (mode == 0) {
        w->unk14 = ((w->unk4 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk16 = ((w->unk6 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk8.w = w->unk4 - w->unk14 * w->unk10;
        w->unk8.h = w->unk6 - w->unk16 * w->unk10;
        w->unk8.x = w->unk0 + w->unk10 * (w->unk14 >> 1);
        w->unk8.y = w->unk2 + w->unk10 * (w->unk16 >> 1);
    } else if (mode == 4) {
        w->unk14 = ((w->unk4 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk16 = 0;
        w->unk8.h = w->unk6;
        w->unk8.y = w->unk2;
        w->unk8.w = w->unk4 - w->unk14 * w->unk10;
        w->unk8.x = w->unk0 + w->unk10 * (w->unk14 >> 1);
    } else if (mode == 1) {
        w->unk14 = 0;
        w->unk30.unkB = 0;
        w->unk30.unkA = 0;
        h = w->unk6 >> 2;
        w->unk8.x = w->unk0;
        w->unk8.y = w->unk2;
        w->unk8.w = w->unk4;
        w->unk8.h = h;
        w->unk16 = (w->unk6 - h + (w->unk10 - 1)) / w->unk10;
    } else if (mode == 3) {
        w->unk8 = *(RECT *)w;
        w->unk1C = mode;
    }
}

void func_8002D1AC(Window *w, s32 mode) {
    w->unk28 = mode;
    if (mode == 0 || mode == 4) {
        w->unk14 = (w->unk4 + w->unk12 - 0x11) / w->unk12;
        w->unk16 = (w->unk6 + w->unk12 - 0x11) / w->unk12;
    } else if (mode == 1) {
        w->unk14 = 0;
        w->unk16 = (w->unk8.h + w->unk12 - 1) / w->unk12;
    } else if (mode == 2) {
        w->unk2C = (w->unk2A + w->unk12 - 9) / w->unk12;
        w->unk2E = (w->unk4 - (w->unk2A + 0x18) + w->unk12 - 1) / w->unk12;
        w->unk16 = (w->unk6 + w->unk12 - 0x11) / w->unk12;
    } else if (mode == 3) {
        w->unk1C = 0;
    }
}

void func_8002D398(Window *w, s32 mode, s32 off) {
    RECT *r = (RECT *)w;

    if (w->unk1A != 4) {
        w->unk28 = 2;
        if (mode == 1 || mode == 3) {
            off -= 0x10;
        }
        w->unk29 = mode;
        w->unk2A = off;
        w->unk2C = (off + w->unk10 - 9) / w->unk10;
        w->unk2E = (r->w - (off + 0x18) + w->unk10 - 1) / w->unk10;
        w->unk16 = (w->unk10 + r->h - 0x11) / w->unk10;
        w->unk8.w = r->w - (w->unk2C + w->unk2E) * w->unk10;
        w->unk8.h = r->h - w->unk16 * w->unk10;
        w->unk8.x = r->x + w->unk2C * w->unk10;
        if ((u32)(mode - 2) < 2) {
            w->unk8.y = r->y + r->h - (s16)(w->unk8.h + 1);
        } else {
            w->unk8.y = r->y;
        }
    }
}

void func_8002D550(Window *w) {
    if (w->unk28 == 0 || w->unk28 == 4) {
        w->unk8.w += w->unk14;
        w->unk8.h += w->unk16;
        w->unk8.x -= w->unk14 >> 1;
        w->unk8.y -= w->unk16 >> 1;
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        if (w->unk8.x < w->unk0) {
            w->unk8.x = w->unk0;
        }
        if (w->unk8.y < w->unk2) {
            w->unk8.y = w->unk2;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    } else if (w->unk28 == 1) {
        w->unk8.w += w->unk14;
        w->unk8.h += w->unk16;
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        w->unk30.unkB = (w->unk6 - w->unk8.h) & 0x1F;
    } else if (w->unk28 == 2) {
        s16 a = w->unk2C + w->unk2E;
        w->unk8.w += a;
        w->unk8.h += w->unk16;
        w->unk8.x -= w->unk2C;
        if (w->unk29 >= 2) {
            w->unk8.y -= w->unk16;
        }
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        if (w->unk8.x < w->unk0) {
            w->unk8.x = w->unk0;
        }
        if (w->unk8.y < w->unk2) {
            w->unk8.y = w->unk2;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    }
    if (*(s32 *)&w->unk8.w == *(s32 *)&w->unk4) {
        w->unk1C = 3;
        if (w->unk48 != NULL) {
            w->unk48(w);
        }
    }
}

void func_8002D7E8(Window *w) {
    if (w->unk28 == 0 || w->unk28 == 4) {
        w->unk8.w -= w->unk14;
        w->unk8.h -= w->unk16;
        w->unk8.x += w->unk14 >> 1;
        w->unk8.y += w->unk16 >> 1;
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    } else if (w->unk28 == 1) {
        w->unk8.h -= w->unk16;
        w->unk8.w -= w->unk14;
        w->unk30.unkB = (w->unk6 - w->unk8.h) & 0x1F;
    } else if (w->unk28 == 2) {
        w->unk8.w -= w->unk2C + w->unk2E;
        w->unk8.h -= w->unk16;
        w->unk8.x += w->unk2C;
        if (w->unk29 >= 2) {
            w->unk8.y += w->unk16;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    }
    if (w->unk8.w < 0) {
        w->unk8.w = 0;
    }
    if (w->unk8.h < 0) {
        w->unk8.h = 0;
    }
    if (w->unk8.w == 0 || w->unk8.h == 0) {
        w->unk1C = 0;
        if (w->unk4C != NULL) {
            w->unk4C(w);
        }
    }
}

void func_8002D9B4(RECT *r, s32 otz, s32 style) {
    RECT t;
    Unk80010CE0 offs;
    DR_AREA *p;
    RECT *clip;
    s8 *o;

    clip = &g_gameState->unkC[g_drawBufs.unk0 ^ 1].disp;
    offs = D_80010CE0;
    p = (DR_AREA *)g_primPtr;
    o = offs.unk0[style];
    if (r != NULL) {
        t.x = g_drawBufs.unk1C[g_drawBufs.unk0 ^ 1].vx + (r->x + o[0]);
        t.y = g_drawBufs.unk1C[g_drawBufs.unk0 ^ 1].vy + (r->y + o[1]);
        t.w = r->w + o[2];
        t.h = r->h + o[3];
        if (t.x < clip->x) {
            t.w -= clip->x - t.x;
            t.x = clip->x;
        }
        if (t.y < clip->y) {
            t.h -= clip->y - t.y;
            t.y = clip->y;
        }
        if (t.x + t.w > clip->x + clip->w) {
            t.w = clip->x + clip->w - t.x;
        }
        if (t.y + t.h > clip->y + clip->h) {
            t.h = clip->y + clip->h - t.y;
        }
    } else {
        t = *clip;
    }
    SetDrawArea(p, &t);
    addPrim(&g_ot[otz], p);
    g_primPtr += sizeof(DR_AREA);
}

Window *func_8002DC54(RECT *r, s32 a, s32 b) {
    s32 i;
    Window *w = D_80077530;
    Window *res;
    for (i = 0; i < 10; i++, w++) {
        if (w->unk1C == 0) {
            break;
        }
    }
    if (i < 10) {
        res = func_8002EA7C(r, a, b, w);
    } else {
        res = NULL;
    }
    return res;
}

Window *func_8002DCAC(RECT *r, s32 a, s32 b, u32 i) {
    Window *res;
    if (i < 10) {
        res = func_8002EA7C(r, a, b, &D_80077530[i]);
    } else {
        res = NULL;
    }
    return res;
}

void func_8002DD00(Window *w) {
    if (w != NULL && w->unk1C != 0 && w->unk1C != 2) {
        w->unk1C = 2;
        if (g_drawBufs.unk1 != 2) {
            func_8002D1AC(w, w->unk28);
        }
    }
}

void func_8002DD54(void) {
    s32 i;
    Window *w = D_80077530;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && w->unk1C != 2) {
            func_8002DD00_inline(w);
        }
    }
}

void func_8002DE10(Window *w) {
    if (w != NULL && w->unk1C != 0) {
        if (w->unk4C != NULL) {
            w->unk4C(w);
        }
        w->unk1C = 0;
    }
}

void func_8002DE60(void) {
    s32 i;
    Window *w = D_80077530;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && w != NULL) {
            if (w->unk4C != NULL) {
                w->unk4C(w);
            }
            w->unk1C = 0;
        }
    }
}

void func_8002DEE4(RECT *a, s32 b, s32 c) {
    func_8002CD4C(a, b, c, (g_gameState->unk24 >> 18) & 1);
}

void func_8002DF18(RECT *r, s32 otz, s32 style, s32 xoff) {
    RECT t;
    s32 y;
    if (style != 4) {
        if (style >= 2) {
            y = r->y + r->h - 2;
        } else {
            y = r->y - 11;
        }
        func_80027568(r->x + xoff, y, otz, style, 0);
        func_80026900(r->x + xoff, y, otz, style);
    }
    func_80026BDC(r, otz, 1, 0, (g_gameState->unk24 >> 18) & 1);
    t.x = r->x + 2;
    t.y = r->y + 2;
    t.w = r->w - 4;
    t.h = r->h - 4;
    func_800274CC(&t, otz);
}

void func_8002E040(RECT *r, s32 otz, s32 mode, MenuList *m) {
    RECT t;
    s32 abr;

    abr = g_gameState->unk24 >> 18;
    abr &= 1;

    if (mode == 1) {
        t.x = r->x;
        t.y = r->y + 4;
        t.w = r->w;
        t.h = r->h - 4;
        func_8002E718(r, otz, m);
        func_80025A28(r->x, r->y, otz, 0, r->w - 2, abr);
        func_80025108(&t, otz, 1, abr);
    } else {
        t = *r;
        func_80025108(&t, otz, 0, abr);
    }
    if (m->unk8 & 4) {
        func_8002E7FC(&t, otz, m, abr);
    } else {
        func_8002709C(&t, otz, 0, m->unkB, abr);
    }
}

void func_8002E180(void) {
    s32 i;
    Window *w = D_80077530;
    for (i = 0; i < 10; w++, i++) {
        u8 st = w->unk1C;
        if (st == 3) {
            if (w->unk30.unk3 < w->unk30.unk2 && w->unk30.unk8 != 0) {
                func_8002EB74(&w->unk30);
            }
        } else if (st == 1) {
            func_8002D550(w);
        } else if (st == 2 && g_drawBufs.unk1 != 2) {
            func_8002D7E8(w);
        }
    }
}

void func_8002E260(void) {
    s32 i;
    Window *p = D_80077530;
    for (i = 0; i < 10; p++, i++) {
        if (p->unk1C == 2) {
            p->unk1C = 0;
            if (p->unk4C != NULL) {
                p->unk4C(p);
            }
        }
    }
}

void func_8002E2E4(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk48 = arg1;
}

void func_8002E2EC(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk40 = arg1;
}

void func_8002E2F4(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk44 = arg1;
}

void func_8002E2FC(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk4C = arg1;
}

void func_8002E304(Window *arg0, s32 arg1) {
    arg0->unk20 = 6;
    arg0->unk24 = arg1;
    arg0->unk22 = -3;
}

void func_8002E31C(Window *arg0, u8 arg1) {
    arg0->unk1F = arg1;
}

void func_8002E324(Window *arg0, u8 arg1) {
    arg0->unk1D = arg1;
}

void func_8002E32C(Window *arg0, u8 arg1) {
    arg0->unk23 = arg1;
}

void func_8002E334(Window *arg0, s16 arg1) {
    arg0->unk10 = arg1;
    if (arg0->unk1A != 3) {
        func_8002CF20(arg0, arg0->unk28);
    } else {
        func_8002D398(arg0, arg0->unk29, arg0->unk2A);
    }
}

void func_8002E380(Window *arg0, s16 arg1) {
    arg0->unk10 = arg1;
    func_8002D1AC(arg0, arg0->unk28);
}

void func_8002E3A8(Window *arg0, MenuList *arg1) {
    arg0->unk30 = *arg1;
}

void func_8002E3F0(Window *arg0, s32 arg1, s16 arg2, s8 arg3) {
    arg0->unk24 = arg1;
    arg0->unk20 = arg2;
    arg0->unk22 = arg3;
}

void func_8002E400(RECT *a, s32 b) {
    func_8002709C(a, b, 0, 0, (g_gameState->unk24 >> 18) & 1);
}

Window *func_8002E43C(s32 i) {
    if (i >= 10) {
        SystemError(0x59, 0x53);
    }
    return &D_80077530[i];
}

void func_8002E490(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        D_80077530[i].unk1C = 0;
        D_80077530[i].unk1B = i;
    }
}

u8 func_8002E4BC(Window *w) {
    Unk80010CD0 min = D_80010CD0;
    u8 r = 0;

    if (w->unk23 == 0) {
        return r;
    }
    if (w->unk1A == 2 || w->unk1A == 3) {
        if (w->unk8.w >= min.unk0[w->unk1A] && w->unk8.h >= 16) {
            r = 1;
        }
    } else if ((u16)w->unk8.w | (u16)w->unk8.h) {
        r = 1;
    }
    return r;
}

void func_8002E588(Window *w) {
    RECT r;
    if (w->unk24 == 0) {
        return;
    }
    if (w->unk1C != 3) {
        func_8002D9B4(NULL, w->unk18, 4);
    }
    func_8002B524(w->unk0 + w->unk20, w->unk8.y + w->unk22, w->unk18, w->unk24, w->unk1E ? 0 : 2);
    if (w->unk1C != 3) {
        RECT *s = &w->unk8;
        r.x = s->x;
        r.y = s->y - 3;
        r.w = s->w;
        r.h = 8;
        func_8002D9B4(&r, w->unk18, 4);
    }
}

void func_8002E660(Window *w) {
    RECT *r = &w->unk8;
    s32 y;
    if (w->unk29 >= 2) {
        y = r->y + r->h - 2;
    } else {
        y = r->y - 11;
    }
    func_800275A4(w->unk0 + w->unk2A, y, w->unk18, w->unk29, w->unk1D);
    func_800265B0(w->unk0 + w->unk2A, y, w->unk18, w->unk29, (w->unk30.unkA + w->unk2A) & 0x1F, w->unk30.unkB);
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8002E718);

void func_8002E7FC(RECT *r, s32 otz, MenuList *m, s32 abr) {
    RECT t;

    func_8002E960(r, otz, m, abr);
    t.x = r->x;
    t.w = r->w;
    if (m->unk8 & 2) {
        t.y = r->y;
        t.h = m->unk9 + 1;
        func_8002709C(&t, otz, 0, (m->unkB + (r->h - m->unk9)) & 0x1F, abr);
        t.y = r->y + m->unk9 - 1;
        t.h = r->h - m->unk9;
        func_8002709C(&t, otz, 0, (m->unkB + (m->unk9 - 1)) & 0x1F, abr);
    } else {
        t.y = r->y;
        t.h = r->h - m->unk9;
        func_8002709C(&t, otz, 0, m->unkB, abr);
        t.y = r->y + t.h;
        t.h = m->unk9;
        func_8002709C(&t, otz, 0, m->unkB, abr);
    }
}

void func_8002E960(RECT *r, s32 otz, MenuList *m, s32 style) {
    LineF2Words *p;
    u32 c;
    s16 y;

    c = D_80068F20[style][(m->unk8 & 2) ? 1 : 0];
    p = (LineF2Words *)g_primPtr;
    setlen(p, 3);
    ((LineF2Words *)g_primPtr)->rgbc = c;
    p->x0 = r->x + 2;
    p->x1 = r->x + r->w - 3;
    if (m->unk8 & 2) {
        y = r->y + m->unk9 + 4;
    } else {
        y = r->y + r->h - (m->unk9 + 1) - 2;
    }
    p->y1 = y;
    p->y0 = y;
    addPrim(&g_ot[otz], p);
    g_primPtr += sizeof(LineF2Words);
}

Window *func_8002EA7C(RECT *r, s32 otz, s32 style, Window *w) {
    w->unk1A = style;
    *(RECT *)w = *r;
    w->unk1C = 1;
    w->unk10 = 7;
    w->unk12 = 5;
    w->unk23 = 1;
    w->unk1D = 0;
    w->unk18 = otz;
    w->unk4C = 0;
    w->unk44 = 0;
    w->unk40 = 0;
    w->unk48 = 0;
    func_8002CF20(w, D_8006BCFC[style]);
    w->unk1F = w->unk1A == 3;
    func_8002EDFC(&w->unk30, 0, 0, 0, r->h);
    w->unk29 = 4;
    w->unk1E = 1;
    w->unk24 = 0;
    w->unk2A = 0;
    return w;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SystemError);

void func_8002EB74(MenuList *p) {
    s32 d;
    if (p->unk8 == 0) {
        return;
    }
    if (p->unk8 & 4) {
        p->unk9 += p->unkE;
        if (p->unk9 >= p->unkD - 6) {
            s32 t = (p->unkC + p->cursor) * p->unk4;
            p->unk8 = 0;
            p->unk9 = 0;
            p->unk5 = p->unkC;
            if (t + p->unk7 >= p->unk0) {
                p->cursor--;
            }
        }
        return;
    }
    if (p->unk8 & 2) {
        d = -3;
    } else {
        d = 3;
    }
    p->unk9 += 3;
    p->unkB = (p->unkB + d) & 0x1F;
    if (p->unk9 < 15) {
        return;
    }
    if (p->unk8 & 8) {
        p->unk7 = (p->unk8 & 2) ? p->unk4 - 1 : 0;
    }
    {
        u8 c = p->unk5;
        s32 e;
        if (p->unk8 & 2) {
            e = c - 1;
        } else {
            e = c + 1;
        }
        p->unk5 = e;
    }
    p->unk8 = 0;
    p->unk9 = 0;
}

MenuView *func_8002ECA0(MenuList *m, MenuView *v) {
    s32 d;

    if (m->unk8 & 4) {
        if (m->unk8 & 2) {
            d = m->unkD - m->unk9;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->unk5 + (m->unk3 - v->unk0);
            v->unk2 = (m->unk3 - v->unk0) * 15 + 6;
        } else {
            d = m->unkD - m->unk9;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->unk5;
            v->unk2 = 6;
        }
    } else if (m->unk8 & 2) {
        v->unk0 = m->unk3 + 1;
        v->unk1 = m->unk5 - 1;
        v->unk2 = m->unk9 - 9;
    } else if (m->unk8 & 1) {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3 + 1;
        v->unk2 = -m->unk9 + 6;
    } else {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3;
        v->unk2 = 6;
    }
    return v;
}

void func_8002EDFC(MenuList *m, s32 count, s32 rows, s32 cols, s32 e) {
    m->unk0 = count;
    if (cols != 0) {
        m->unk2 = (count + cols - 1) / cols;
    } else {
        m->unk2 = 0;
    }
    m->unk4 = cols;
    if (m->unk2 < rows) {
        rows = m->unk2;
    }
    m->unk3 = rows;
    m->unk8 = 0;
    m->unkC = 0;
    m->unkB = 0;
    m->unkA = 0;
    m->unk9 = 0;
    m->unk7 = 0;
    m->cursor = 0;
    m->unk5 = 0;
    m->unkD = e;
}

void func_8002EE8C(MenuList *m, s32 b) {
    if (m->unk8 == 0) {
        func_8002EEF0(m, b);
        func_8002EF58(m, b);
        func_8002F1CC(m, b);
        func_8002F284(m, b);
    }
}

void func_8002EEF0(MenuList *m, u32 pad) {
    if (pad & 0x10) {
        if (m->cursor == 0) {
            if (m->unk5 != 0) {
                snd_playSe(0x67);
                m->unk8 = 2;
            }
        } else {
            m->cursor--;
            snd_playSe(0x67);
        }
    }
}

void func_8002EF58(MenuList *m, u32 pad) {
    if (pad & 0x40) {
        if ((m->unk5 + m->cursor) * m->unk4 + m->unk7 + m->unk4 < m->unk0) {
            if (m->cursor >= m->unk3 - 1) {
                if (m->unk5 < m->unk2 - m->unk3) {
                    m->unk8 = 1;
                    snd_playSe(0x67);
                }
            } else {
                snd_playSe(0x67);
                m->cursor++;
            }
        } else if (m->cursor >= m->unk3 - 1 && m->unk5 < m->unk2 - m->unk3) {
            m->unk8 = 9;
            snd_playSe(0x67);
        }
    }
}

void func_8002F044(MenuList *arg0, s32 arg1) {
    arg1 &= 0x80;
    if (arg1 != 0) {
        if (arg0->unk7 == 0) {
            if (arg0->cursor != 0) {
                arg0->unk7 = arg0->unk4 - 1;
                arg0->cursor = arg0->cursor - 1;
                snd_playSe(0x67);
                return;
            }
            if (arg0->unk5 != 0) {
                snd_playSe(0x67);
                arg0->unk8 = 0xA;
            }
        } else {
            snd_playSe(0x67);
            arg0->unk7--;
        }
    }
}

void func_8002F0EC(MenuList *m, s32 pad) {
    if (pad & 0x20) {
        if ((m->unk5 + m->cursor) * m->unk4 + m->unk7 < m->unk0 - 1) {
            if (m->unk7 >= m->unk4 - 1) {
                if (m->cursor < m->unk3 - 1) {
                    snd_playSe(0x67);
                    m->unk7 = 0;
                    m->cursor++;
                } else if (m->unk5 < m->unk2 - m->unk3) {
                    snd_playSe(0x67);
                    m->unk8 = 9;
                }
            } else {
                snd_playSe(0x67);
                m->unk7++;
            }
        }
    }
}

void func_8002F1CC(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 c;

    if (!(g_padPortHeld[0] & BTN_R1) && (arg1 & 0x100000) && arg0->unk5 != 0) {
        t = arg0->unkD - 6;
        arg0->unk9 = t >> 2;
        t -= arg0->unk9;
        arg0->unk8 = 7;
        c = arg0->unk5 - arg0->unk3;
        arg0->unkE = (t + 14) / 15;
        if (c < 0) {
            c = 0;
        }
        arg0->unkC = c;
        snd_playSe(0x417);
    }
}

void func_8002F284(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 a;
    s32 c;
    s32 u3;

    if ((arg1 & 0x200000) && arg0->unk5 < arg0->unk2 - arg0->unk3) {
        t = arg0->unkD - 6;
        arg0->unk9 = t >> 2;
        t -= arg0->unk9;
        u3 = arg0->unk3;
        a = arg0->unk2 - u3;
        arg0->unk8 = 5;
        c = arg0->unk5 + u3;
        arg0->unkE = (t + 14) / 15;
        if (c < a) {
            a = c;
        }
        arg0->unkC = a;
        snd_playSe(0x417);
    }
}

MenuView *func_8002F344(MenuList *m, MenuView *v) {
    s32 q;
    if (m->unk8 & 2) {
        q = (m->unk9 + 12) / 15;
        v->unk0 = q;
        v->unk1 = m->unkC + (m->unk3 - v->unk0);
        v->unk2 = m->unk9 - v->unk0 * 15 + 7;
    } else {
        s32 n = m->unk9;
        v->unk2 = m->unkD - n;
        v->unk0 = (n + 14) / 15;
        v->unk1 = m->unkC;
    }
    return v;
}

void func_8002F40C(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = m->unk9 + src->y + 5;
        dst->h = src->h - (m->unk9 + 4) - 1;
    } else {
        dst->y = src->y;
        dst->h = src->h - m->unk9 - 1;
    }
}

void snd_playSe(s32 a) {
    g_gameState->unk1C->unk83C(0xD000, a, 0x800000, 0x80, 0x7F);
}

void func_8002F4D0(s32 a) {
    g_gameState->unk1C->unk83C(0x5100, a, 0x800000, 0, 0);
}

void func_8002F514(s32 a) {
    g_gameState->unk1C->unk83C(0xD000, a, 0, 0x80, 0x7F);
}

void func_8002F55C(s32 a) {
    g_gameState->unk1C->unk83C(0x5100, a, 0, 0, 0);
}

s32 func_8002F5A0(void (*fn)(void)) {
    Unk80077C78 *p = &D_80077C78;
    s32 d = p->unk8 - p->unkC;
    Unk4 c1;
    Unk4 c2;

    if (p->unk2 == 0) {
        return 0;
    }
    c1 = p->unk20;
    c2 = *(Unk4 *)&p->unk24;
    func_8002F93C(p->unkA ^ 0x800);
    func_8002FC8C(D_80077C78.unk0, p->unk1, 0, c2.unk0[0] | (c2.unk0[1] << 8) | (c2.unk0[2] << 16));
    func_8002FC8C(D_80077C78.unk0, p->unk1, p->unk8, c1.unk0[0] | (c1.unk0[1] << 8) | (c1.unk0[2] << 16));
    D_8006BD28[D_80077C78.unk0](p, d);
    p->unk10 = (s32)fn;
    p->unkC += d;
    return 1;
}

void func_8002F6C8(void) {
    u8 *buf = D_80077878[0];
    Unk80077C78 *f = &D_80077C78;
    FadeFn fn;

    if (g_gameState->unk1C->unk8 != 0) {
        buf = D_80077878[1];
    }
    f->unk18 = buf;
    if (f->unk2 != 0) {
        fn = (FadeFn)D_8006BD04[f->unk0].unk4;
        if (fn != NULL) {
            fn(f);
        }
    }
    if (*(u32 *)&D_80077C78 & 0xFFFF0000) {
        fn = (FadeFn)D_8006BD04[D_80077C78.unk0].unk8;
        if (fn != NULL) {
            fn(&D_80077C78);
        }
    }
    if (D_80077C78.unk2 != 0) {
        if (++D_80077C78.unkC >= (u32)D_80077C78.unk8) {
            D_80077C78.unk2 = 0;
            fn = (FadeFn)D_80077C78.unk10;
            if (fn != NULL) {
                fn(&D_80077C78);
            }
        }
    }
}

u8 func_8002F7E8(u8 a) {
    u8 old = D_80077C78.unk2;
    D_80077C78.unk2 = a;
    return old;
}

u8 func_8002F800(void) {
    return D_80077C78.unk2;
}

u8 func_8002F810(void) {
    if (D_80077C78.unk2 != 0) {
        return 1;
    }
    return D_80077C78.unk3 != 0;
}

void func_8002F83C(void) {
    D_80077C78.unk2 = 0;
    D_80077C78.unk3 = 0;
    D_80077C78.unk0 = 0;
}

u8 func_8002F854(void) {
    return D_80077C78.unk0;
}

u8 func_8002F864(void) {
    return D_80077C78.unk1;
}

s32 func_8002F874(void) {
    return D_80077C78.unkC;
}

void func_8002F884(void) {
    D_80077C78.unk3 = 0;
    D_80077C78.unk2 = 0;
    D_80077C78.unkC = 0;
    D_80077C78.unk8 = 0;
    D_80077C78.unk0 = 0;
    D_80077C78.unk10 = 0;
}

s32 func_8002F8A8(void (*a)(void)) {
    s32 old = D_80077C78.unk10;
    D_80077C78.unk10 = (s32)a;
    return old;
}

u8 func_8002F8BC() {
    u8 old = D_80077C78.unk4;
    D_80077C78.unk4 = 0;
    return old;
}

u8 func_8002F8D4(void) {
    return D_80077C78.unk4;
}

s32 func_8002F8E4(s32 a) {
    s32 old = D_80077C78.unk1C;
    if (a == 0) {
        a = 1;
    }
    D_80077C78.unk1C = a;
    return old;
}

s32 func_8002F904(void) {
    return D_80077C78.unk1C;
}

u8 func_8002F914(u8 a) {
    u8 old = D_80077C78.unk3;
    D_80077C78.unk3 = a;
    return old;
}

u8 func_8002F92C(void) {
    return D_80077C78.unk3;
}

u16 func_8002F93C(u16 a) {
    u16 old = D_80077C78.unkA;
    D_80077C78.unkA = a;
    return old;
}

u16 func_8002F954(void) {
    return D_80077C78.unkA;
}

void func_8002F964(u8 a, u8 b, u8 c) {
    Unk80077C78 *p = &D_80077C78;
    Unk80077C78 *q;
    s32 r = 0;
    p->unk24 = a;
    p->unk25 = b;
    p->unk26 = c;
    q = p;
    if (a != 0 || b != 0 || c != 0) {
        r = 1;
    }
    q->unk3 = r;
}

Unk4 func_8002F9A8(void) {
    return D_80077C78.unk28;
}

void func_8002F9D0(void) {
    func_8002FA50(0x10);
}

void func_8002F9F0(void) {
    func_8002FAAC(0xC);
}

void func_8002FA10(void) {
    func_8002FB08(0x10);
}

void func_8002FA30(void) {
    func_8002FB64(0xC);
}

void func_8002FA50(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 1, 0, 0xFFFFFF);
    func_8002FC8C(1, 1, a, 0);
}

void func_8002FAAC(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 1, 0, 0);
    func_8002FC8C(1, 1, a, 0xFFFFFF);
}

void func_8002FB08(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 0, 0, 0xFFFFFF);
    func_8002FC8C(1, 0, a, 0);
}

void func_8002FB64(s32 a) {
    func_8002F93C(0);
    func_8002F8BC(1);
    func_8002FC8C(1, 0, 0, 0);
    func_8002FC8C(1, 0, a, 0xFFFFFF);
}

u32 *func_8002FBC0(s32 i) {
    if (g_gameState->mode != 2) {
        return &g_gameState->unk1C->unk10[i][D_80077C78.unk1C];
    }
    return &D_80077CA4[i];
}

void func_8002FC10(void *a) {
    func_80030770(a);
}

void func_8002FC30(void) {
}

void func_8002FC38(void) {
    D_80077C78.unk1C = 1;
    D_80077C78.unkA = -1;
    D_80077C78.unk2 = 0;
    D_80077C78.unk10 = 0;
    D_80077C78.unk4 = 0;
    D_80077C78.unk3 = 0;
    D_80077C78.unk5 = 0;
    D_80077C78.unk0 = 0;
    func_8002FC38_clear3(D_80077C78.unk28.unk0);
    func_8002FC38_clear3(&D_80077C78.unk24);
}

void func_8002FC8C(s32 a, s32 b, s32 c, s32 col) {
    u8 old;
    void (*fn)(Unk80077C78 *);

    if (c == 0) {
        D_80077C78.unk28 = *(Unk4 *)&col;
    } else {
        D_80077C78.unk20 = D_80077C78.unk28;
        old = D_80077C78.unk0;
        D_80077C78.unk0 = a;
        D_80077C78.unk1 = b;
        D_80077C78.unkC = 0;
        D_80077C78.unk8 = c;
        D_80077C78.unk5 = old;
        *(Unk4 *)&D_80077C78.unk24 = *(Unk4 *)&col;
        D_80077C78.unk2 = 1;
        D_80077C78.unk10 = 0;
        D_80077C78.unk3 = (*(s32 *)&D_80077C78.unk24 & 0xFFFFFF) != 0;
        fn = D_8006BD04[a].unk0;
        if (fn != NULL) {
            fn(&D_80077C78);
        }
    }
}

s32 func_8002FD70(s32 a, s32 b, s32 lim) {
    a += b;
    if (b < 0) {
        if (a < lim) {
            a = lim;
        }
    } else {
        if (a > lim) {
            a = lim;
        }
    }
    return a;
}

s32 func_8002FD98(s32 a, s32 b) {
    s32 t;

    if (a >= 0) {
        t = a - 1;
        t += b;
        b = t / b;
    } else {
        t = a + 1;
        t -= b;
        b = t / b;
    }
    return b;
}

s32 func_8002FDE8(s32 x, s32 a, s32 b) {
    s32 t;

    if (b != 0) {
        if (b < 0) {
            t = x - a - b;
            x = a + (b - t * t / b);
        } else {
            t = x - a - b;
            x = a + (b - t * t / b);
        }
    }
    return x;
}

void func_8002FEA4(void) {
}

void func_8002FEAC(void) {
}

void func_8002FEB4(void) {
    func_8002F5A0(0);
}

void func_8002FED4(Unk80077C78 *p) {
    s32 half;
    s32 n;
    Unk80077D00 *d;

    if (p->unk8 == 0) {
        p->unk8 = 0x10;
    }
    D_80077D00.unk0 = p->unk28.unk0[0];
    D_80077D00.unk3C.unk0 = D_80077D00.unk0;
    D_80077D00.unk30.unk0 = D_80077D00.unk0;
    D_80077D00.unk4 = p->unk28.unk0[1];
    D_80077D00.unk3C.unk4 = D_80077D00.unk4;
    D_80077D00.unk30.unk4 = D_80077D00.unk4;
    D_80077D00.unk8 = p->unk28.unk0[2];
    D_80077D00.unk3C.unk8 = D_80077D00.unk8;
    D_80077D00.unk30.unk8 = D_80077D00.unk8;
    D_80077D00.unkC = p->unk24;
    D_80077D00.unk10 = p->unk25;
    D_80077D00.unk14 = p->unk26;
    D_80077D00.unk54 = D_80077D00.unkC - D_80077D00.unk0;
    D_80077D00.unk58 = D_80077D00.unk10 - D_80077D00.unk4;
    D_80077D00.unk5C = D_80077D00.unk14 - D_80077D00.unk8;
    half = p->unk8 >> 1;
    n = 1;
    if (half > 0) {
        n = half;
    }
    D_80077D00.unk60 = n;
    d = &D_80077D00;
    d->unk64 = p->unk8 - n;
    d->unk18 = func_8002FD98(d->unk54, d->unk60);
    d->unk1C = func_8002FD98(d->unk58, d->unk60);
    d->unk20 = func_8002FD98(d->unk5C, d->unk60);
    if (d->unk64 != 0) {
        d->unk24 = func_8002FD98(d->unk54, d->unk64);
        d->unk28 = func_8002FD98(d->unk58, d->unk64);
        d->unk2C = func_8002FD98(d->unk5C, d->unk64);
    } else {
        do {
        } while (0); /* empty macro in the original; needed for the s0/s1 split */
        d->unk2C = 0;
        d->unk28 = 0;
        d->unk24 = 0;
    }
}

void func_80030030(Unk80077C78 *p) {
    if ((u32)p->unkC < D_80077D00.unk60) {
        D_80077D00.unk30.unk0 = func_8002FD70(D_80077D00.unk30.unk0, D_80077D00.unk18, D_80077D00.unkC);
        D_80077D00.unk30.unk4 = func_8002FD70(D_80077D00.unk30.unk4, D_80077D00.unk1C, D_80077D00.unk10);
        D_80077D00.unk30.unk8 = func_8002FD70(D_80077D00.unk30.unk8, D_80077D00.unk20, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk30;
    } else {
        D_80077D00.unk3C.unk0 = func_8002FD70(D_80077D00.unk3C.unk0, D_80077D00.unk24, D_80077D00.unkC);
        D_80077D00.unk3C.unk4 = func_8002FD70(D_80077D00.unk3C.unk4, D_80077D00.unk28, D_80077D00.unk10);
        D_80077D00.unk3C.unk8 = func_8002FD70(D_80077D00.unk3C.unk8, D_80077D00.unk2C, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk3C;
    }
    p->unk28.unk0[0] = D_80077D00.unk48.unk0;
    p->unk28.unk0[1] = D_80077D00.unk48.unk4;
    p->unk28.unk0[2] = D_80077D00.unk48.unk8;
}

void func_8003015C(Unk80077C78 *p, s32 n) {
    if (n < (s32)D_80077D00.unk60) {
        D_80077D00.unk30.unk0 = func_8002FD70(D_80077D00.unk30.unk0, D_80077D00.unk18 * n, D_80077D00.unkC);
        D_80077D00.unk30.unk4 = func_8002FD70(D_80077D00.unk30.unk4, D_80077D00.unk1C * n, D_80077D00.unk10);
        D_80077D00.unk30.unk8 = func_8002FD70(D_80077D00.unk30.unk8, D_80077D00.unk20 * n, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk30;
    } else {
        n -= D_80077D00.unk60;
        D_80077D00.unk3C.unk0 = func_8002FD70(D_80077D00.unk3C.unk0, D_80077D00.unk24 * n, D_80077D00.unkC);
        D_80077D00.unk3C.unk4 = func_8002FD70(D_80077D00.unk3C.unk4, D_80077D00.unk28 * n, D_80077D00.unk10);
        D_80077D00.unk3C.unk8 = func_8002FD70(D_80077D00.unk3C.unk8, D_80077D00.unk2C * n, D_80077D00.unk14);
        D_80077D00.unk48 = D_80077D00.unk3C;
    }
    p->unk28.unk0[0] = D_80077D00.unk48.unk0;
    p->unk28.unk0[1] = D_80077D00.unk48.unk4;
    p->unk28.unk0[2] = D_80077D00.unk48.unk8;
}

void func_800302D0(Unk80077C78 *arg0) {
    DVECTOR xy[4];
    MATRIX m;
    long ofx, ofy;
    long p, flag;
    s32 tpage;
    u32 *ot;
    s32 h;
    s32 hw, hh;
    s32 r;
    s32 i, j;
    POLY_G4 *poly;
    DR_TPAGE *tp;

    tpage = 64;
    ot = func_8002FBC0(g_gameState->unk1C->unk8);
    if (arg0->unk1 == 0) {
        tpage = 32;
    }
    ReadGeomOffset(&ofx, &ofy);
    SetGeomOffset(g_gameState->unkC->disp.w >> 1, g_gameState->unkC->disp.h >> 1);
    gte_ReadGeomScreen_r(h);
    hw = g_gameState->unkC->disp.w >> 1;
    hh = g_gameState->unkC->disp.h >> 1;
    PushMatrix();
    D_80077CB0.vx = 0;
    D_80077CB0.vy = 0;
    D_80077CB0.vz = arg0->unkA;
    func_80030AF4(&D_80077CB0, &m);
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    SetRotMatrix(&m);
    SetTransMatrix(&m);
    r = SquareRoot0(hw * hw + hh * hh);
    setVector(&D_80077CC0[0][0], -r, -r, h);
    setVector(&D_80077CC0[0][1], 0, -r, h);
    setVector(&D_80077CC0[0][2], -r, r, h);
    setVector(&D_80077CC0[0][3], 0, r, h);
    setVector(&D_80077CC0[1][0], 0, -r, h);
    setVector(&D_80077CC0[1][1], r, -r, h);
    setVector(&D_80077CC0[1][2], 0, r, h);
    setVector(&D_80077CC0[1][3], r, r, h);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            RotTransPers(&D_80077CC0[i][j], (long *)&xy[j], &p, &flag);
        }
        poly = (POLY_G4 *)D_80077C78.unk18;
        SetPolyG4(poly);
        if ((u32)arg0->unkC < D_80077D00.unk60) {
            setRGB0(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB1(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB2(poly, D_80077D00.unk0, D_80077D00.unk4, D_80077D00.unk8);
            setRGB3(poly, D_80077D00.unk0, D_80077D00.unk4, D_80077D00.unk8);
        } else {
            setRGB0(poly, D_80077D00.unkC, D_80077D00.unk10, D_80077D00.unk14);
            setRGB1(poly, D_80077D00.unkC, D_80077D00.unk10, D_80077D00.unk14);
            setRGB2(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
            setRGB3(poly, D_80077D00.unk48.unk0, D_80077D00.unk48.unk4, D_80077D00.unk48.unk8);
        }
        poly->x0 = xy[0].vx;
        poly->y0 = xy[0].vy;
        poly->x1 = xy[1].vx;
        poly->y1 = xy[1].vy;
        poly->x2 = xy[2].vx;
        poly->y2 = xy[2].vy;
        poly->x3 = xy[3].vx;
        poly->y3 = xy[3].vy;
        SetSemiTrans(poly, 1);
        addPrim(ot, poly);
        D_80077C78.unk18 += sizeof(POLY_G4);
    }
    tp = (DR_TPAGE *)D_80077C78.unk18;
    SetDrawTPage(tp, 0, 1, tpage);
    addPrim(ot, tp);
    D_80077C78.unk18 += sizeof(DR_TPAGE);
    PopMatrix();
    SetGeomOffset(ofx, ofy);
}

void func_80030770(u8 *arg0) {
    if ((u32)D_80077C78.unkC < D_80077D00.unk60) {
        arg0[0] = D_80077D00.unk48.unk0;
        arg0[1] = D_80077D00.unk48.unk4;
        arg0[2] = D_80077D00.unk48.unk8;
        arg0[4] = D_80077D00.unk0;
        arg0[5] = D_80077D00.unk4;
        arg0[6] = D_80077D00.unk8;
        return;
    }
    arg0[0] = D_80077D00.unkC;
    arg0[1] = D_80077D00.unk10;
    arg0[2] = D_80077D00.unk14;
    arg0[4] = D_80077D00.unk48.unk0;
    arg0[5] = D_80077D00.unk48.unk4;
    arg0[6] = D_80077D00.unk48.unk8;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetSemiTrans);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetPolyG4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SquareRoot0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PushMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", PopMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetRotMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetTransMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ReadGeomOffset);

INCLUDE_ASM("asm/us/main/nonmatchings/main", RotTransPers);

void func_80030AF4(void *a, void *b) {
    func_80030B34(a, b, 0);
}

void func_80030B14(void *a, void *b) {
    func_80030B34(a, b, 1);
}

MATRIX *func_80030B34(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    /* Both branches are identical; jump2 merges them, leaving only the dead copy of mode. */
    if (mode) {
        t1 = (sx * sy) >> 12;
        t2 = (cx * sy) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-cy * sz) >> 12;
        m->m[0][2] = sy;
        m->m[1][0] = (t1 * cz + cx * sz) >> 12;
        m->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        m->m[1][2] = (-sx * cy) >> 12;
        m->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        m->m[2][1] = (t2 * sz + sx * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sy) >> 12;
        t2 = (cx * sy) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-cy * sz) >> 12;
        m->m[0][2] = sy;
        m->m[1][0] = (t1 * cz + cx * sz) >> 12;
        m->m[1][1] = (-t1 * sz + cx * cz) >> 12;
        m->m[1][2] = (-sx * cy) >> 12;
        m->m[2][0] = (-t2 * cz + sx * sz) >> 12;
        m->m[2][1] = (t2 * sz + sx * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    }
    return m;
}

void func_80030EA4(void *a, void *b) {
    func_80030EE4(a, b, 0);
}

void func_80030EC4(void *a, void *b) {
    func_80030EE4(a, b, 1);
}

MATRIX *func_80030EE4(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 tx, ty, tz;
    s16 t1, t2;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    /* Both branches are identical; jump2 merges them, leaving only the dead copy of mode. */
    if (mode) {
        t1 = (sx * sz) >> 12;
        t2 = (sx * cz) >> 12;
        m->m[0][0] = (cz * cy - t1 * sy) >> 12;
        m->m[0][1] = (-cx * sz) >> 12;
        m->m[0][2] = (cz * sy + t1 * cy) >> 12;
        m->m[1][0] = (sz * cy + t2 * sy) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = (sz * sy - t2 * cy) >> 12;
        m->m[2][0] = (-cx * sy) >> 12;
        m->m[2][1] = sx;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        t1 = (sx * sz) >> 12;
        t2 = (sx * cz) >> 12;
        m->m[0][0] = (cz * cy - t1 * sy) >> 12;
        m->m[0][1] = (-cx * sz) >> 12;
        m->m[0][2] = (cz * sy + t1 * cy) >> 12;
        m->m[1][0] = (sz * cy + t2 * sy) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = (sz * sy - t2 * cy) >> 12;
        m->m[2][0] = (-cx * sy) >> 12;
        m->m[2][1] = sx;
        m->m[2][2] = (cx * cy) >> 12;
    }
    return m;
}

void func_80031244(void *a, void *b) {
    func_80031284(a, b, 0);
}

void func_80031264(void *a, void *b) {
    func_80031284(a, b, 1);
}

MATRIX *func_80031284(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 t;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 a, b, c, d, e, f;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    if (y & 0x400) {
        sy = D_8006C1C4[0x400 - (y & 0x3FF)];
    } else {
        sy = D_8006C1C4[y & 0x3FF];
    }
    if (y & 0x800) {
        /* identical branches (jump2 merges them), as in func_80030EE4 */
        if (mode) {
            sy = -sy;
        } else {
            sy = -sy;
        }
    }
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    if (mode == 0) {
        t1 = (sy * cz) >> 12;
        t2 = (sy * sz) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-sz * cx + t1 * sx) >> 12;
        m->m[0][2] = (sz * sx + t1 * cx) >> 12;
        m->m[1][0] = (cy * sz) >> 12;
        m->m[1][1] = (cz * cx + t2 * sx) >> 12;
        m->m[1][2] = (-cz * sx + t2 * cx) >> 12;
        m->m[2][0] = -sy;
        m->m[2][1] = (cy * sx) >> 12;
        m->m[2][2] = (cy * cx) >> 12;
    } else {
        gte_ldir0123(cy, cz, sz, sx);
        m->m[2][0] = -sy;
        gte_gpf12_n1();
        a = cz * sx;
        gte_stmac123(d, e, f);
        gte_ldir0123(cx, cy, sz, cz);
        do {
            b = sz * sx;
            m->m[0][0] = d;
            gte_gpf12_b();
            a >>= 12;
            b >>= 12;
            m->m[1][0] = e;
            m->m[2][1] = f;
        } while (0);
        gte_stmac123(d, c, t);
        gte_ldir0123(sy, a, b, t);
        m->m[2][2] = d;
        gte_gpf12_n1();
        gte_stmac123(d, e, f);
        gte_ldir12(t, c);
        d -= c;
        m->m[0][1] = d;
        gte_gpf12_b();
        e += t;
        m->m[1][1] = e;
        gte_stmac12(d, e);
        d += b;
        e -= a;
        m->m[0][2] = d;
        m->m[1][2] = e;
    }
    return m;
}

void func_800316D8(void *a, void *b) {
    func_80031718(a, b, 0);
}

void func_800316F8(void *a, void *b) {
    func_80031718(a, b, 1);
}

MATRIX *func_80031718(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 t;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 a, b, c, d, e, f;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    if (mode == 0) {
        t1 = (sx * sy) >> 12;
        t2 = (sx * cy) >> 12;
        m->m[0][0] = (cy * cz + t1 * sz) >> 12;
        m->m[0][1] = (-cy * sz + t1 * cz) >> 12;
        m->m[0][2] = (cx * sy) >> 12;
        m->m[1][0] = (cx * sz) >> 12;
        m->m[1][1] = (cx * cz) >> 12;
        m->m[1][2] = -sx;
        m->m[2][0] = (-sy * cz + t2 * sz) >> 12;
        m->m[2][1] = (sy * sz + t2 * cz) >> 12;
        m->m[2][2] = (cx * cy) >> 12;
    } else {
        gte_ldir0123(cx, sy, cy, cz);
        m->m[1][2] = -sx;
        gte_gpf12_n1();
        a = cy * cz;
        gte_stmac123(d, e, f);
        gte_ldir0123(sz, cx, cy, sy);
        do {
            b = sy * cz;
            m->m[0][2] = d;
            gte_gpf12_b();
            a >>= 12;
            b >>= 12;
            m->m[2][2] = e;
            m->m[1][1] = f;
        } while (0);
        gte_stmac123(d, c, t);
        gte_ldir0123(sx, b, a, t);
        m->m[1][0] = d;
        gte_gpf12_n1();
        gte_stmac123(d, e, f);
        gte_ldir12(t, c);
        d -= c;
        m->m[0][1] = d;
        gte_gpf12_b();
        e += t;
        m->m[2][1] = e;
        gte_stmac12(d, e);
        d += a;
        e -= b;
        m->m[0][0] = d;
        m->m[2][0] = e;
    }
    return m;
}

void func_80031B6C(Unk80077C78 *p) {
    s32 n;
    if (p->unk8 == 0) {
        p->unk8 = 16;
    }
    D_80077D68.unk24.unk0 = D_80077D68.unk0 = p->unk28.unk0[0];
    D_80077D68.unk24.unk4 = D_80077D68.unk4 = p->unk28.unk0[1];
    D_80077D68.unk24.unk8 = D_80077D68.unk8 = p->unk28.unk0[2];
    D_80077D68.unkC = p->unk24;
    D_80077D68.unk10 = p->unk25;
    D_80077D68.unk14 = p->unk26;
    D_80077D68.unk3C = D_80077D68.unkC - D_80077D68.unk0;
    D_80077D68.unk40 = D_80077D68.unk10 - D_80077D68.unk4;
    D_80077D68.unk44 = D_80077D68.unk14 - D_80077D68.unk8;
    n = p->unk8 - 1;
    if (n <= 0) {
        n = 1;
    }
    D_80077D68.unk18 = func_8002FD98(D_80077D68.unk3C, n);
    n = p->unk8 - 1;
    if (n <= 0) {
        n = 1;
    }
    D_80077D68.unk1C = func_8002FD98(D_80077D68.unk40, n);
    n = p->unk8 - 1;
    if (n <= 0) {
        n = 1;
    }
    D_80077D68.unk20 = func_8002FD98(D_80077D68.unk44, n);
}

void func_80031C88(Unk80077C78 *p) {
    D_80077D68.unk24.unk0 = func_8002FD70(D_80077D68.unk24.unk0, D_80077D68.unk18, D_80077D68.unkC);
    D_80077D68.unk24.unk4 = func_8002FD70(D_80077D68.unk24.unk4, D_80077D68.unk1C, D_80077D68.unk10);
    D_80077D68.unk24.unk8 = func_8002FD70(D_80077D68.unk24.unk8, D_80077D68.unk20, D_80077D68.unk14);
    if (p->unk4 != 0) {
        D_80077D68.unk30.unk0 = func_8002FDE8(D_80077D68.unk24.unk0, D_80077D68.unk0, D_80077D68.unk3C);
        D_80077D68.unk30.unk4 = func_8002FDE8(D_80077D68.unk24.unk4, D_80077D68.unk4, D_80077D68.unk40);
        D_80077D68.unk30.unk8 = func_8002FDE8(D_80077D68.unk24.unk8, D_80077D68.unk8, D_80077D68.unk44);
    } else {
        D_80077D68.unk30 = D_80077D68.unk24;
    }
    p->unk28.unk0[0] = D_80077D68.unk30.unk0;
    p->unk28.unk0[1] = D_80077D68.unk30.unk4;
    p->unk28.unk0[2] = D_80077D68.unk30.unk8;
}

void func_80031D98(Unk80077C78 *p, s32 n) {
    D_80077D68.unk24.unk0 = func_8002FD70(D_80077D68.unk24.unk0, D_80077D68.unk18 * n, D_80077D68.unkC);
    D_80077D68.unk24.unk4 = func_8002FD70(D_80077D68.unk24.unk4, D_80077D68.unk1C * n, D_80077D68.unk10);
    D_80077D68.unk24.unk8 = func_8002FD70(D_80077D68.unk24.unk8, D_80077D68.unk20 * n, D_80077D68.unk14);
    if (p->unk4 != 0) {
        D_80077D68.unk30.unk0 = func_8002FDE8(D_80077D68.unk24.unk0, D_80077D68.unk0, D_80077D68.unk3C);
        D_80077D68.unk30.unk4 = func_8002FDE8(D_80077D68.unk24.unk4, D_80077D68.unk4, D_80077D68.unk40);
        D_80077D68.unk30.unk8 = func_8002FDE8(D_80077D68.unk24.unk8, D_80077D68.unk8, D_80077D68.unk44);
    } else {
        D_80077D68.unk30 = D_80077D68.unk24;
    }
    p->unk28.unk0[0] = D_80077D68.unk30.unk0;
    p->unk28.unk0[1] = D_80077D68.unk30.unk4;
    p->unk28.unk0[2] = D_80077D68.unk30.unk8;
}

void func_80031ED8(Unk80077C78 *p) {
    u32 *ot = func_8002FBC0(g_gameState->unk1C->unk8);
    s32 tp = 0x40;
    TILE *t;
    u8 *dr;

    if (p->unk1 == 0) {
        tp = 0x20;
    }
    t = (TILE *)D_80077C78.unk18;
    SetTile(t);
    t->r0 = D_80077D68.unk30.unk0;
    t->g0 = D_80077D68.unk30.unk4;
    t->b0 = D_80077D68.unk30.unk8;
    t->y0 = 0;
    t->x0 = 0;
    t->w = g_gameState->unkC->disp.w;
    t->h = g_gameState->unkC->disp.h;
    SetSemiTrans(t, 1);
    addPrim(ot, t);
    D_80077C78.unk18 += sizeof(TILE);
    dr = D_80077C78.unk18;
    SetDrawTPage(dr, 0, 1, tp);
    addPrim(ot, dr);
    D_80077C78.unk18 += sizeof(DR_TPAGE);
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetTile);

void func_80032078(void) {
    func_8002FC38();
}

void func_80032098(void) {
    func_8002F6C8();
}

void func_800320B8(s32 a, s32 b, s32 c, s32 d) {
    func_8002FC8C(a, b, c, d);
}

void func_800320D8(void) {
    func_800325A4();
    func_8003264C();
}

void func_80032100(void) {
    func_80032658();
}

void func_80032120(s32 a, s32 b) {
    func_80032140(a, b);
}

void func_80032140(s32 a, s32 b) {
    GameState *st = g_gameState;

    if (!(st->unk1C->unk0 & 1) && D_80077DB0.unk5 == 0 && D_80077DB0.unk0 == 0 && (u32)a < 9) {
        D_80077DB0.unk5 = 1;
        D_80077DB0.unk1 = a;
        D_80077DB0.unkC = b;
        D_80077DB0.unk1C = 0;
        D_80077DB0.unk6 = 0;
        D_80077DB0.unk7 = 0;
        D_80077DB0.unk9 = 1;
        D_80077DB0.unkA = 1;
        D_80077DB0.unkB = 1;
        D_80077DB0.unk24 = 0;
        D_80077DB0.unk0 = 1;
        if (st->mode == 1) {
            st->unk1C->unk8E4.ovl65->unk0 |= 0x1000;
        }
        g_gameState->unk1C->unk0 |= 2;
        func_80035430();
        func_800326B0();
        func_80032980();
    }
}

void func_80032238(void) {
    GameState *g;
    s32 i;

    D_80077DB0.unk7 = 0;
    g = g_gameState;
    g->unk1C->unk0 |= 0x1C;
    D_80077DB0.unk14 = g->unk9;
    g->unk9 = 0;
    D_80077DB0.unk20 = g_gameState->unk10[0].r0;
    D_80077DB0.unk21 = g_gameState->unk10[0].g0;
    D_80077DB0.unk22 = g_gameState->unk10[0].b0;
    g_gameState->unk10[0].r0 = 0;
    g_gameState->unk10[0].g0 = 0;
    g_gameState->unk10[0].b0 = 0;
    g_gameState->unk10[1].r0 = 0;
    g_gameState->unk10[1].g0 = 0;
    g_gameState->unk10[1].b0 = 0;
    func_8002DE60();
    D_80077DB0.unk18 = D_80073998[0];
    D_80073998[0] = 0x4000000;
    func_8002C4B0(1);
    for (i = 0; i < 2; i++) {
        func_8002C584(i);
    }
}

void func_80032384(void) {
    void (*fn)(u8);
    u8 idx;

    if (g_gameState->mode == 1) {
        g_gameState->unk1C->unk8E4.ovl65->unk0 &= ~0x1000;
    }
    g_gameState->unk1C->unk0 &= ~2;
    g_gameState->unk1C->unk0 &= ~0x1C;
    func_8002DE60();
    g_gameState->unk9 = D_80077DB0.unk14;
    g_gameState->unk10[0].r0 = D_80077DB0.unk20;
    g_gameState->unk10[0].g0 = D_80077DB0.unk21;
    g_gameState->unk10[0].b0 = D_80077DB0.unk22;
    g_gameState->unk10[1].r0 = D_80077DB0.unk20;
    g_gameState->unk10[1].g0 = D_80077DB0.unk21;
    g_gameState->unk10[1].b0 = D_80077DB0.unk22;
    D_80077DB0.unk0 = 0;
    D_80077DB0.unk5 = 0;
    fn = (void (*)(u8))D_8006C9D4[D_80077DB0.unk1].unk8;
    idx = D_80077DB0.unk1;
    if (fn != NULL) {
        fn(idx);
    }
    func_8002C4B0(0);
    D_80073998[0] = D_80077DB0.unk18;
    if (D_80077DB0.unk24 != 0) {
        ((void (*)(void))D_80077DB0.unk24)();
    }
    if (D_80077DB0.unkA != 0) {
        func_8002F9D0();
    }
}

void func_8003253C(void) {
    func_80023918();
    func_80032078();
    func_80023A2C();
    func_800325A4();
}

void func_80032574(void) {
    func_80023918();
    func_80032078();
    func_80035430();
}

void func_800325A4(void) {
    D_80077DB0.unk0 = 0;
    D_80077DB0.unk5 = 0;
    D_80077DB0.unk1C = 0;
    D_80077DB0.unk24 = 0;
    D_80077DB0.unk4 = 0;
    D_80077DB0.unk3 = 0;
    func_80035430();
}

void func_800325E0(void) {
    if (D_80077DBB != 0) {
        func_8002F9F0();
        func_8002F8A8(func_80032384);
    } else {
        func_80032384();
    }
}

void func_80032628(void (*a)(void)) {
    D_80077DD4 = (s32)a;
}

void func_80032634(u8 a) {
    D_80077DBA = a;
}

void func_80032640(u8 a) {
    D_80077DBB = a;
}

void func_8003264C(void) {
    D_80077DB2 = 0;
}

void func_80032658(void) {
    void (*fn)(void);
    if (D_80077DB0.unk5 != 0) {
        fn = D_8006CA64[D_80077DB0.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_800326B0(void) {
    if (D_8006CA40[D_80077DB0.unk1] != 0xFF) {
        func_80032708();
    } else {
        func_80032798();
    }
}

void func_80032708(void) {
    FileEntry *f;
    if (func_80022B18() == 0) {
        D_80077DB0.unk0 = 2;
        D_80077DB0.unk6 = 1;
        f = func_8001E218(0, D_8006CA40[D_80077DB0.unk1]);
        func_80022C60(f->unk4 + g_gameState->unk20, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, func_800329FC);

    }
}

void func_80032798(void) {
    s32 busy = func_80022B18();
    FileEntry *f;

    if (D_80077DB0.unk1 == 0) {
        if (func_80033D28() == 0) {
            D_80077DB0.unk0 = 2;
        } else if (busy == 0 || g_gameState->mode != 3) {
            D_80077DB0.unk6 = 1;
            f = func_8001E218(1, 30);
            if (func_80022C60(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00,
                              func_80032A18)) {
                D_80077DB0.unk0 = 2;
            }
        }
    }
}

void func_8003286C(void) {
    if (g_gameState->mode != 3 || func_80022B18() == 0) {
        func_800326B0();
    }
}

void func_800328B4(void) {
    if ((D_80077DB4 & 0xFFFF0000) == 0) {
        func_80032A44();
    }
}

void func_800328E8(void) {
    void (*fn)(u8);
    u8 idx;

    func_8002C308(0);
    if (D_80077DB0.unk9 != 0) {
        func_80024BD8();
    }
    fn = D_8006C9D4[D_80077DB0.unk1].unk4;
    idx = D_80077DB0.unk1;
    if (fn != NULL) {
        fn(idx);
    }
    func_80035490();
    func_8002C1EC();
    D_80077DB0.unk10++;
}

void func_80032980(void) {
    D_80077DB0.unk7 = 1;
    if (func_8002F800() == 0) {
        if (func_8002F810() == 0) {
            func_8002F9F0();
            func_8002F8A8(func_80032238);
        } else {
            func_80032238();
        }
    } else {
        func_8002F5A0(func_80032238);
    }
}

void func_800329FC(u8 *a) {
    if (*a == 0) {
        D_80077DB6 = 0;
    }
}

void func_80032A18(u8 *a) {
    if (*a == 0) {
        D_80077DB6 = 0;
    }
}

s32 func_80032A34(void) {
    return D_80077DCC;
}

void func_80032A44(void) {
    void (*fn)(u8);

    D_80077DB0.unk10 = 0;
    DrawSync(0);
    func_80012D08();
    FlushCache();
    func_80012D18();
    D_80077DB0.unk0 = 3;
    if (D_80077DB0.unk1 == 0 && func_80033D28()) {
        func_80028B20((u8 *)0x801EDF00);
        func_80033D1C(0);
    }
    func_8002FA50(0x10);
    fn = D_8006C9D4[D_80077DB0.unk1].unk0;
    if (fn != NULL) {
        fn(D_80077DB0.unk1);
    }
}

void func_80032B08(s32 ch) {
    Character *c = &g_gameState->unk1C->characters[ch];
    u32 *init;
    u32 *tbl;
    s32 r;

    memset(c, 0, 0x90);
    tbl = D_8006CA78;
    init = tbl + ch;
    c->unk34.w = *init;
    c->unk38 = 0;
    c->unkA = func_80033A50(c->unk34.b.row);
    memset(c->unk3E, 0, 10);
    func_80034394(ch);
    r = func_80033A1C(c);
    func_80033D38(c->unk0, r);
    func_80033D8C(c->unk39, r);
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->unk39);
    func_80032C4C(ch, 1, init, 0);
    c->hp = c->maxHp;
    c->mp = c->maxMp;
    c->stones = c->maxStones;
}

void func_80032C4C(s32 ch, s32 lv, u32 *heal, u8 f) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 hp;
    s32 mp;
    s32 d;

    if (!f && c->level != lv) {
        c->exp = g_levelExp[lv - 1];
    }
    hp = c->hp;
    mp = c->mp;
    d = c->maxStones - c->stones;
    c->level = lv;
    memset(&c->hp, 0, 8);
    memset(&c->maxHp, 0, 8);
    c->unk16 = 10;
    c->unk30 = func_80034A64(ch, c->level, f);
    c->unk31 = func_80034B50(ch, c->level, f);
    c->unk32 = func_80034C64(ch, c->level, f);
    c->unk33 = func_80034D78(ch, c->level, f);
    c->maxHp = c->unk2C = func_80034F7C(c->level, c->unk31);
    c->maxMp = c->unk2E = func_80034FD4(c->level, c->unk32);
    c->maxStones = func_80034E80(ch, c->level, f);
    c->hp = (hp < c->maxHp) ? hp : c->maxHp;
    c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    c->stones = c->maxStones - d;
    if (heal) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
        c->stones = c->maxStones;
    }
    func_8003384C(c);
    if (f) {
        c->hp = (hp < c->maxHp) ? hp : c->maxHp;
        c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    }
    if (c->maxStones < d) {
        func_80033DCC(c);
    }
}

Unk80032E84Out *func_80032E84(Unk80032E84In *in, Unk80032E84Out *out) {
    s32 i;
    u8 *a;
    u8 *e;
    s32 id;

    memset(out, 0, sizeof(Unk80032E84Out));
    out->unk0 = in->unk10;
    out->unk2 = in->unk12;
    out->unk4 = *(u16 *)&in->unk0.unk0[0];
    out->unk6 = *(u16 *)&in->unk0.unk0[2];
    out->unk8[0] = in->unk0.unk0[4];
    out->unk8[1] = in->unk0.unk0[5];
    out->unk8[2] = in->unk0.unk0[6];
    out->unk8[3] = in->unk0.unk0[7];
    id = in->unk14.unk0[0];
    if (id != 0xFF) {
        out->unkC[0] = D_8006E0E8[id].unk5;
    }
    for (i = 0; i < 4; i++) {
        id = (&in->unk14.unk0[1])[i];
        if (id != 0xFF) {
            if ((u32)(id - 0x58) < 0x88) {
                a = D_8006D1C8[id];
                out->unkC[1] += a[0];
                out->unkC[2] += a[1];
                out->unkC[3] += a[2];
                out->unkC[4] += a[3];
            }
        }
    }
    for (i = 0; i < 5; i++) {
        id = in->unk14.unk0[i];
        if (id != 0xFF) {
            e = D_8006D548[D_80069018[id].unkB].unk0;
            out->unk8[0] += e[0];
            out->unk8[1] += e[1];
            out->unk8[2] += e[2];
            out->unk8[3] += e[3];
        }
    }
    for (i = 0; i < 4; i++) {
        if (func_80034A04(in->unk8.unk0, D_8006CA9C[i])) {
            switch (D_8006CA9C[i]) {
            case 0xC5:
                out->unk4 += out->unk4 / 10;
                break;
            case 0xC6:
                out->unk4 += out->unk4 / 5;
                break;
            case 0xC7:
                out->unk6 += out->unk6 / 10;
                break;
            case 0xC8:
                out->unk6 += out->unk6 / 5;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (out->unk8[i] > D_8006CAA0[i]) {
            out->unk8[i] = D_8006CAA0[i];
        }
    }
    for (i = 0; i < 5; i++) {
        if (out->unkC[i] >= 1000) {
            out->unkC[i] = 999;
        }
    }
    if (out->unk4 >= 10000) {
        out->unk4 = 9999;
    }
    if (out->unk6 >= 1000) {
        out->unk6 = 999;
    }
    if (out->unk4 < out->unk0) {
        out->unk0 = out->unk4;
    }
    if (out->unk6 < out->unk2) {
        out->unk2 = out->unk6;
    }
    return out;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010BD0);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010CD0);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010CE0);

s32 func_80033280(s32 kind, s32 flag, u8 *item) {
    u8 t = D_8006CAA4[kind][(u8)flag != 0];
    u8 k = D_80069018[*item].unk8;
    s32 r;

    switch (t) {
    case 0:
        return k != 1;
    case 1:
        return 2;
    case 2:
        if ((u16)D_8007AEF0.h[0] >= 10300) {
            r = 5;
            if (k == 7) {
                r = 6;
            }
            return r;
        }
        r = 3;
        if (k == 7) {
            r = 4;
        }
        return r;
    case 3:
        return 7;
    case 4:
        return 12;
    case 5:
        return 9;
    case 6:
        r = 10;
        if (k == 7) {
            r = 11;
        }
        return r;
    case 7:
        return 13;
    case 8:
        return 14;
    case 9:
        return 15;
    case 10:
        r = 16;
        if ((u16)(D_8007AEF0.h[0] - 1500) < 100) {
            r = 17;
        }
        return r;
    case 11:
        return 18;
    }
    return -1;
}

void func_800333B0(s32 ch, u8 avg, s32 eq) {
    Character *c = &g_gameState->unk1C->characters[ch];
    u8 *p;
    s32 i;
    s32 n;
    s32 sum;
    s32 lv;

    if (eq != 0xFF) {
        p = c->unk39;
        for (i = 0; i < 5; i++) {
            p[i] = D_8006CAD8[eq][i];
        }
    }
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->unk39);
    if (avg == 0) {
        if (eq == 0xFF) {
            return;
        }
        goto calc;
    }
    n = 0;
    sum = 0;
    for (i = 0; i < 9; i++) {
        if (ch != i) {
            if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
                n++;
                sum += g_gameState->unk1C->characters[i].level;
            }
        }
    }
    if (n != 0) {
        lv = sum / n;
    } else {
        lv = 1;
    }
    if ((u32)(c->unk34.b.unk4 - 10) >= 2) {
        if (lv < c->level) {
            lv = c->level;
        }
    }
    func_80032C4C(ch, lv, NULL, 0);
    return;
calc:
    func_8003384C_inline(c);
}

void func_80033690(void) {
    Character **party = g_gameState->unk1C->party;
    s32 i;
    func_80033D1C(0);
    memset(party, 0, 0x1C);
    for (i = 0; i < 9; i++) {
        func_80032B08(i);
    }
    func_80033BD0(0);
    func_80033BD0(1);
    func_80033BD0(2);
    func_80033BD0(3);
    func_80033BD0(4);
    func_80033BD0(5);
    func_80033BD0(7);
    func_80033BD0(6);
    func_80033AF4(0, 0);
    func_80033AF4(1, 1);
    func_80033AF4(2, 2);
    func_80033AF4(3, 3);
    g_gameState->unk1C->gil = 500;
    g_gameState->unk1C->unk55E = 0;
}

s32 func_8003378C(void) {
    GameData *gd = g_gameState->unk1C;
    s32 i = 0;
    do {
    loop:
        if (gd->party[i] == NULL) {
            i = i < 3 ? i + 1 : 0;
            goto loop;
        }
    } while (0);
    return i;
}

s32 func_800337D0(s32 i) {
    do {
        i = i == 0 ? 3 : i - 1;
    } while (g_gameState->unk1C->party[i] == NULL);
    return i;
}

s32 func_8003380C(s32 i) {
    do {
        i = i < 3 ? i + 1 : 0;
    } while (g_gameState->unk1C->party[i] == NULL);
    return i;
}

void func_8003384C(Character *c) {
    Unk80032E84In in;
    Unk80032E84Out out;

    in.unk0 = *(Bytes8 *)&c->unk2C;
    in.unk10 = c->hp;
    in.unk12 = c->mp;
    in.unk8 = *(Words2 *)c->unk88;
    in.unk14 = *(Bytes5 *)c->unk39;
    func_80032E84(&in, &out);
    c->unk24 = out.unk8[0];
    c->unk25 = out.unk8[1];
    c->unk26 = out.unk8[2];
    c->unk27 = out.unk8[3];
    c->unk28 = out.unkC[1];
    c->unk29 = out.unkC[2];
    c->unk2A = out.unkC[3];
    c->unk2B = out.unkC[4];
    c->hp = out.unk0;
    c->mp = out.unk2;
    c->maxHp = out.unk4;
    c->maxMp = out.unk6;
}

s32 func_80033964(u32 x) {
    switch (x) {
    case 0:
    case 16:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    case 8:
    case 9:
    case 17:
        return 8;
    case 10:
    case 11:
    case 18:
        return 9;
    case 12:
    case 13:
    case 19:
        return 10;
    case 14:
    case 15:
        return 11;
    }
    return -1;
}

u8 func_800339F4(s32 a, u8 b) {
    return D_8006CAA4[a][b != 0];
}

s32 func_80033A1C(Character *c) {
    return D_8006CAB8[c->unk34.b.id][(c->unkA & 0x10) != 0];
}

u8 func_80033A50(u32 x) {
    u8 *tbl = D_8006CACC;
    s32 i;
    switch (x) {
    case 0:
    case 16:
        i = 0;
        break;
    case 1:
        i = 1;
        break;
    case 2:
        i = 2;
        break;
    case 3:
        i = 3;
        break;
    case 4:
        i = 4;
        break;
    case 5:
        i = 5;
        break;
    case 6:
        i = 6;
        break;
    case 7:
        i = 7;
        break;
    case 8:
    case 9:
    case 17:
        i = 8;
        break;
    case 10:
    case 11:
    case 18:
        i = 9;
        break;
    case 12:
    case 13:
    case 19:
        i = 10;
        break;
    case 14:
    case 15:
        i = 11;
        break;
    default:
        i = -1;
        break;
    }
    return tbl[i];
}

void func_80033AF4(s32 slot, s32 ch) {
    Character *old = g_gameState->unk1C->party[slot];
    if (ch >= 9) {
        ch = -1;
    }
    if (ch < 0) {
        g_gameState->unk1C->party[slot] = NULL;
    } else {
        func_80033BD0(ch);
        g_gameState->unk1C->party[slot] = &g_gameState->unk1C->characters[ch];
    }
    if (old != g_gameState->unk1C->party[slot]) {
        func_80033D1C(1);
    }
}

void func_80033BD0(u32 i) {
    if (i < 9) {
        g_gameState->unk1C->characters[i].unk34.b.unkB = 1;
    }
}

void func_80033C0C(u32 i) {
    if (i < 9) {
        g_gameState->unk1C->characters[i].unk34.b.unkB = 0;
    }
}

s32 func_80033C48(s32 ch, s32 b, u8 flag) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 lv = c->level;
    c->level = 0;
    func_80032C4C(ch, b, 0, 0);
    if (flag) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
    }
    return lv;
}

s32 func_80033CCC(s32 ch, s32 b) {
    Character *c = &g_gameState->unk1C->characters[ch];
    s32 r = c->level;
    func_80032C4C(ch, b, 0, 1);
    return r;
}

void func_80033D1C(u8 a) {
    D_80077DE0 = a;
}

u8 func_80033D28(void) {
    return D_80077DE0;
}

/* default character names: count, then {offset, length} entries */
INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010DC4);

void func_80033D38(u8 *dst, s32 id) {
    memset(dst, 0xFF, 10);
    func_8002BD54(dst, (u8 *)text_getFrom((u8 *)D_80010DC4, id));
}

void func_80033D8C(u8 *dst, s32 b) {
    s32 i;
    for (i = 0; i < 5; i++) {
        dst[i] = D_8006CAD8[b][i];
    }
}

void func_80033DCC(Character *c) {
    s32 i;
    Unk8006D128 *e;
    s32 id;

    i = 0;
    e = D_8006D128;
    id = c->unk34.b.id;
    c->stones = c->maxStones;
    do {
        if (func_800349BC(id, i + 0xC0)) {
            if (c->stones >= e->unk1) {
                c->stones -= e->unk1;
            } else {
                func_8003493C(id, i + 0xC0, 0);
            }
        }
        i++;
        e++;
    } while (i < 64);
}

s32 func_80033E84(s32 ch, s32 id, s32 add) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    s32 need;

    if (i < 0) {
        return 0;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    need = func_800347D8_inline(ch, id);
    if (need - g_gameState->unk1C->characters[ch].unk58[i] < add) {
        g_gameState->unk1C->characters[ch].unk58[i] = need;
    } else {
        g_gameState->unk1C->characters[ch].unk58[i] += add;
    }
    return g_gameState->unk1C->characters[ch].unk58[i] - old;
}

s32 func_80034070(s32 ch, s32 id) {
    s32 ap = func_80034430_ap_inline(ch, id);
    s32 need = func_800347D8_inline(ch, id);

    if (ap < 0 || need < 0) {
        return 0;
    }
    return ap >= need;
}

s32 func_80034220(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    Character *c = g_gameState->unk1C->characters + ch;
    if (i < 0) {
        return 0;
    }
    c->unk58[i] = D_8006CB28[c->unk34.b.row][i].unk1;
    return 1;
}

void func_8003431C(void) {
}

void func_80034324(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        func_800343C0(i);
    }
}

void func_8003435C(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        func_80034404(i);
    }
}

void func_80034394(s32 a) {
    func_800343C0(a);
    func_80034404(a);
}

void func_800343C0(s32 ch) {
    s32 i;
    Character *c;

    if (ch < 8) {
        c = &g_gameState->unk1C->characters[ch];
        for (i = 0; i < 0x30; i++) {
            c->unk58[i] = 0;
        }
    }
}

void func_80034404(s32 i) {
    Character *c = &g_gameState->unk1C->characters[i];
    c->unk88[1] = 0;
    c->unk88[0] = 0;
}

s32 func_80034430(s32 ch, s32 id) {
    return func_80034430_inline(ch, id);
}

u8 *func_800344C0(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    if (i < 0) {
        return NULL;
    }
    return &g_gameState->unk1C->characters[ch].unk58[i];
}

s32 func_80034584(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->unk1C->characters[ch].unk58[i];
}

s32 func_80034648(s32 ch, s32 id, u8 v) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    g_gameState->unk1C->characters[ch].unk58[i] = v;
    return old;
}

static inline s32 func_80034648_inline(s32 ch, s32 id, s32 v) {
    s32 i = func_80034430_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->unk1C->characters[ch].unk58[i];
    g_gameState->unk1C->characters[ch].unk58[i] = v;
    return old;
}

s32 func_8003470C(s32 ch, s32 id) {
    s32 r = func_80034648_inline(ch, id, 0);
    return r;
}

u8 func_800347D8(s32 ch, s32 id) {
    s32 i = func_80034430_inline(ch, id);
    Character *c = g_gameState->unk1C->characters + ch;
    if (i < 0) {
        return 0;
    }
    return D_8006CB28[c->unk34.b.row][i].unk1;
}

s32 func_800348C0(s32 ch) {
    s32 sum = 0;
    s32 i;
    u32 *bits = g_gameState->unk1C->characters[ch].unk88;
    for (i = 0; i < 0x40; i++) {
        if (bits[i >> 5] & (1 << (i & 0x1F))) {
            sum += D_8006D128[i].unk1;
        }
    }
    return sum;
}

void func_8003493C(s32 ch, s32 id, u8 on) {
    u32 *bits = g_gameState->unk1C->characters[ch].unk88;
    id -= 0xC0;
    if (on) {
        bits[id >> 5] |= 1 << (id & 0x1F);
    } else {
        bits[id >> 5] &= ~(1 << (id & 0x1F));
    }
}

u8 func_800349BC(s32 ch, s32 id) {
    return func_80034A04_inline(g_gameState->unk1C->characters[ch].unk88, id);
}

u8 func_80034A04(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

u8 func_80034A30(Character *a) {
    if (a->unkA & 0x10) {
        return 0;
    }
    return (s32)a->unk34.b.row < 8;
}

s32 func_80034A64(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    Unk8006DD88 *e = &D_8006DD88[func_80033964(c->unk34.b.row)];
    s32 r;
    if (add) {
        c->unk3E[0] += func_8003502C(c->unk39, 0);
    }
    r = e->unk0 + lv / 10 + (c->unk3E[0] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 func_80034B50(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[1] += n + func_8003502C(c->unk39, 1);
    }
    r = e->unk1 + lv * 3 / 10 + (bonus[1] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 func_80034C64(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[2] += n + func_8003502C(c->unk39, 2);
    }
    r = e->unk2 + lv * 3 / 10 + (bonus[2] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 func_80034D78(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    s32 k = func_80033964(c->unk34.b.row);
    u16 *bonus = c->unk3E;
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;
    if (add) {
        n = c->stones == 0;
        bonus[3] += n + func_8003502C(c->unk39, 3);
    }
    r = e->unk3 + lv * 3 / 20 + (bonus[3] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 func_80034E80(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->unk1C->characters + ch;
    u16 *bonus = c->unk3E;
    s32 k = func_80033964(c->unk34.b.row);
    Unk8006DD88 *e = &D_8006DD88[k];
    s32 r;
    s32 n;

    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 5;
        }
        bonus[4] += n;
    }
    r = e->unk4 + lv * 4 / 10 + (bonus[4] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 func_80034F7C(s32 lv, s32 m) {
    s32 r = D_8006DDD0[lv - 1].unk0 * m / 50;
    if (r >= 10000) {
        r = 9999;
    }
    return r;
}

s32 func_80034FD4(s32 lv, s32 m) {
    s32 r = D_8006DDD0[lv - 1].unk2 * m / 100;
    if (r >= 1000) {
        r = 999;
    }
    return r;
}

s32 func_8003502C(u8 *eq, s32 k) {
    s32 sum = 0;
    s32 i;
    u8 *e;

    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            e = D_8006D548[D_80069018[eq[i]].unkB].unk0;
            switch (k) {
            case 0:
                sum += e[0];
                break;
            case 1:
                sum += e[1];
                break;
            case 2:
                sum += e[2];
                break;
            case 3:
                sum += e[3];
                break;
            }
        }
    }
    return sum;
}

void func_80035108(void) {
    s32 otz;
    Unk80035214 r;

    otz = 1;
    if (D_80077DE8.unk1D == 0) {
        otz = D_80077DE8.unk4;
    }
    do {
    } while (0); /* empty debug macro in the original; changes the register allocation */
    if (D_80077DE8.unk1C != 0) {
        if (D_80077DE8.unk1D != 0) {
            func_800270EC(D_80077DE8.unk0, D_80077DE8.unk2, otz);
        }
        func_80035C2C(D_80077DE8.unk0 + 5, D_80077DE8.unk2 + 2, otz, &D_80077DE8.unk20);
    }
    func_8003573C();
    if (D_80077DE8.unk1A != 0) {
        func_80035214(&r);
        func_800275DC(r.unk0.x + 8, r.unk0.y, otz, 0xB7, 0);
        func_8002B5FC(r.unk0.x + 6, r.unk0.y + 8, otz, D_80077DE8.unk8, 8);
        func_8002DF18(&r.unk0, otz, r.unk8, r.unkA);
    }
}

void func_80035214(Unk80035214 *w) {
    RECT *r = &w->unk0;
    RECT *c = &D_80077DE8.unk10;
    Unk80077DE8 *m = &D_80077DE8;
    TextSize sz;
    s32 x;
    s32 v;
    s32 d1;
    s32 d2;
    s32 t;

    sz = func_8002AF70(m->unk8);
    w->unk0.w = sz.w + 12;
    w->unk0.h = sz.h + 12;
    if (w->unk0.w >= c->w) {
        w->unk0.w = c->w;
    }
    if (w->unk0.h >= c->h) {
        w->unk0.h = c->h;
    }
    if (m->unk1C == 0) {
        w->unk8 = 4;
        w->unk0.x = m->unk0;
        w->unk0.y = m->unk2;
        return;
    }
    v = m->unk2;
    t = c->y + 16;
    d2 = v - t;
    d1 = c->y + c->h - 10;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 = 2;
        w->unk0.y = m->unk2 - (s16)(w->unk0.h + 16);
    } else {
        w->unk8 = 0;
        w->unk0.y = m->unk2 + 10;
    }
    v = D_80077DE8.unk0;
    t = c->x + 19;
    d2 = v - t;
    d1 = c->x + c->w - 18;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 |= 1;
        x = D_80077DE8.unk0 - 19;
    } else {
        x = v + 18;
    }
    r->x = x - (r->w >> 1);
    if (r->x < c->x) {
        r->x = c->x;
    }
    if (r->x + r->w > c->x + c->w) {
        r->x = c->x + c->w - r->w;
    }
    if (x - r->x < 8) {
        w->unkA = 8;
    } else {
        w->unkA = x - r->x;
    }
}

void func_80035430(void) {
    D_80077DE8.unk18 = 0;
    D_80077DE8.unkC = 0;
    D_80077DE8.unk8 = 0;
    D_80077DE8.unk1A = 0;
    D_80077DE8.unk19 = 0;
    D_80077DE8.unk1B = 0;
    D_80077DE8.unk1C = 1;
    D_80077DE8.unk1D = 1;
    func_80035580(NULL);
    func_80035B24(&D_80077DE8.unk20, &D_80010E48);
}

void func_80035490(void) {
    Unk80077DE8 *p;
    if (D_80077DE8.unk1B == 0 && func_8002F810() == 0 && (g_padRepeat & 0x8000000)) {
        func_80035680();
    }
    p = &D_80077DE8;
    if (p->unk18 != 0 && func_8002F810() == 0 && p->unkC != NULL) {
        p->unkC();
        if (p->unk8 != NULL && *p->unk8 != 0xFF) {
            func_80035108();
        }
    }
    func_80035B90(&D_80077DE8.unk20);
}

void func_80035564(void) {
}

void func_8003556C(void (*a)(void)) {
    D_80077DE8.unk8 = 0;
    D_80077DE8.unkC = a;
}

void func_80035580(RECT *r) {
    if (r == NULL) {
        D_80077DE8.unk10.x = 8;
        D_80077DE8.unk10.y = 8;
        D_80077DE8.unk10.w = 0x131;
        D_80077DE8.unk10.h = 0xD1;
    } else {
        D_80077DE8.unk10 = *r;
    }
}

void func_800355DC(void) {
}

void func_800355E4(Unk80077DE8 *unused) {
    func_800275DC(D_80077DE8.unk0, D_80077DE8.unk2, D_80077DE8.unk4, 1, 0);
}

u8 func_8003561C(void) {
    return D_80077DE8.unk18;
}

u8 func_8003562C(void) {
    return D_80077DE8.unk1B;
}

void func_8003563C(u8 a) {
    D_80077DE8.unk1B = a;
}

u8 func_80035648(void) {
    return D_80077DE8.unk1C;
}

void func_80035658(u8 a) {
    D_80077DE8.unk1C = a;
}

u8 func_80035664(void) {
    return D_80077DE8.unk1D;
}

void func_80035674(u8 a) {
    D_80077DE8.unk1D = a;
}

void func_80035680(void) {
    if (D_80077DE8.unk19 == 0 && D_80077DE8.unkC != NULL) {
        if (D_80077DE8.unk18 == 0) {
            snd_playSe(0x2AA);
            D_80077DE8.unk18 = 1;
            D_80077DE8.unk19 = 1;
            D_80077DE8.unk1A = 0;
            func_80035B7C(&D_80077DE8.unk20);
            func_80035B74(&D_80077DE8.unk20, func_80035744);
            func_80035B64(&D_80077DE8.unk20, 1);
        } else {
            snd_playSe(0x65);
            D_80077DE8.unk18 = 0;
            D_80077DE8.unk1A = 0;
            func_80035B64(&D_80077DE8.unk20, 0);
        }
    }
}

void func_8003573C(void) {
}

void func_80035744(void) {
    D_80077DE8.unk1A = 1;
    D_80077DE8.unk19 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", text_setBank);

INCLUDE_ASM("asm/us/main/nonmatchings/main", text_get);

INCLUDE_ASM("asm/us/main/nonmatchings/main", text_getInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/main", text_getFrom);

void func_800357E0(s32 x, s32 y, s32 otz, s32 unused, Unk800357E0 *d) {
    u32 t = d->unk4;
    SprtWords *s;

    setlen(g_primPtr, 4);
    s = (SprtWords *)g_primPtr;
    s->rgbc = 0x64808080;
    s->xy = ((x + d->unk2 - 0x80) & 0xFFFF) | ((y + d->unk3 - 0x80) << 16);
    s->uvclut = d->unk0 | (d->unk1 << 8) | ((u16)d->unk4 << 16);
    g_primPtr = (u8 *)(s + 1);
    s->wh = d->unk8 | (d->unkA << 16);
    addPrim(&g_ot[otz], s);
    SetDrawTPage(g_primPtr, 1, 1, ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void func_8003595C(s32 x, s32 y, s32 otz, Unk80035B90 *unused, AnimCell *c) {
    u32 t;
    s32 sw;
    u32 tp;
    s32 sh;
    PolyFT4Words *p;

    t = c->unk0.unk4;
    sw = (c->unk0.unk8 * c->unk10) >> 12;
    tp = ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7);
    sh = (c->unk0.unkA * c->unk12) >> 12;
    x = x + c->unk0.unk2 - 0x80 + ((c->unk0.unk8 - sw) >> 1);
    y = y + c->unk0.unk3 - 0x80 + ((c->unk0.unkA - sh) >> 1);
    setlen(g_primPtr, 9);
    p = (PolyFT4Words *)g_primPtr;
    p->rgbc = 0x2C808080;
    p->xy0 = (x & 0xFFFF) | (y << 16);
    p->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    p->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    p->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    p->uv0clut = c->unk0.unk0 | (c->unk0.unk1 << 8) | ((u16)c->unk0.unk4 << 16);
    p->uv1tpage = (c->unk0.unk0 + c->unk0.unk8 - 1) | (c->unk0.unk1 << 8) | (tp << 16);
    p->uv2 = c->unk0.unk0 | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    p->uv3 = (c->unk0.unk0 + c->unk0.unk8 - 1) | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

void func_80035B24(Unk80035B90 *a, Unk80035B90Data *d) {
    a->unk4 = d;
    a->unk8 = d->unk4;
    a->unkC = &a->unk8[a->unk4->unk0];
    a->unk10 = d;
    a->unk0 = 0;
    a->unk1 = 0;
    a->unk2 = 0;
    a->unk14 = 0;
    a->unk3 = d->unk0;
}

u8 func_80035B64(Unk80035B90 *a, u8 b) {
    u8 old = a->unk0;
    a->unk0 = b;
    return old;
}

void func_80035B74(Unk80035B90 *a, void (*b)()) {
    a->unk14 = b;
}

void func_80035B7C(Unk80035B90 *a) {
    a->unk0 = 0;
    a->unk1 = 0;
    a->unk2 = 0;
    a->unk14 = 0;
}

void func_80035B90(Unk80035B90 *arg0) {
    Unk80035B90Frame *f = &arg0->unk8[arg0->unk1];

    if (arg0->unk0 != 0) {
        if (++arg0->unk2 >= f->unk2 - 1) {
            arg0->unk2 = 0;
            if (++arg0->unk1 >= arg0->unk3 - 1) {
                arg0->unk0 = 0;
                if (arg0->unk14 != NULL) {
                    arg0->unk14(arg0);
                }
            }
        }
    }
}

void func_80035C2C(s32 x, s32 y, s32 otz, Unk80035B90 *a) {
    s32 i;
    Unk80035B90Frame *f = &a->unk8[a->unk1];
    AnimCellList *cl = &((AnimCellList *)a->unkC)[f->unk0];
    AnimCell *c = (AnimCell *)((u8 *)a->unk10 + cl->unk2);

    for (i = 0; i < cl->unk0; i++) {
        if (!((c->unk0.unk4 >> 26) & 1)) {
            func_800357E0(x + f->unk4, y + f->unk6, otz, (s32)a, &c->unk0);
        } else {
            func_8003595C(x + f->unk4, y + f->unk6, otz, a, c);
        }
        c++;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010E48);

void func_80035D3C(void) {
    Unk80077E44 *t;

    printf("path=%s\n", "f:/code/dms/loc_us/ff9db.h");
    D_80077E50.unk2 = 0;
    D_80077E50.unk0 = 1;
    D_80077E50.unk1 = 1;
    D_80077E50.unk4 = 0;
    D_80077E50.unk8 = func_8002F904();
    D_80077E50.unk3 = func_8003378C();
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
    func_8002D9B4(NULL, 2, 2);
    if (D_80077E28.unk7 != 0) {
        sel = D_80077E28.unk7 - 1;
    } else {
        sel = D_80077E52 - 1;
    }
    for (i = 0; i < 7; i++) {
        y = i * 15 + 6;
        if (all || i == sel) {
            func_8002B5FC(r->x + 10, r->y + y + D_80077E28.unk8, 2,
                          (u8 *)text_getFrom(D_800110A0, i), 0);
        }
    }
    func_8002D9B4(r, 2, 2);
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
            func_80012D08();
            FlushCache();
            func_80012D18();
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
        f = func_8001E218(0, id);
        if (func_80022C60(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, func_80036634)) {
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

Unk8004AB64 *func_80036650(s32 n) {
    Unk8007AECCNode *cur = D_8007AECC->unk15D0;
    Unk8007AECCNode *nn = NULL;
    Unk8007AECCNode *next;

    if (cur != NULL) {
        if ((u32 *)cur->unk0 >= D_8007AECC->unk50 + n) {
            nn = D_8007AECC->unk15D4;
            D_8007AECC->unk15D4 = nn->unk8;
            nn->unk8 = D_8007AECC->unk15D0;
            D_8007AECC->unk15D0 = nn;
            {
                u32 *p1 = D_8007AECC->unk50;
                nn->unk0 = (Unk8004AB64 *)p1;
                nn->unk4 = p1 + n;
            }
        } else {
            for (next = cur->unk8; next != NULL; next = cur->unk8) {
                if ((u32 *)next->unk0 >= cur->unk4 + n) {
                    break;
                }
                cur = next;
            }
            if (next != NULL) {
                nn = D_8007AECC->unk15D4;
                D_8007AECC->unk15D4 = nn->unk8;
                nn->unk8 = cur->unk8;
                cur->unk8 = nn;
                {
                    u32 *p2 = cur->unk4;
                    nn->unk0 = (Unk8004AB64 *)p2;
                    nn->unk4 = p2 + n;
                }
            } else if (cur->unk4 + n <= D_8007AECC->unk50 + 0x500) {
                nn = D_8007AECC->unk15D4;
                D_8007AECC->unk15D4 = nn->unk8;
                nn->unk8 = NULL;
                cur->unk8 = nn;
                {
                    u32 *p3 = cur->unk4;
                    nn->unk0 = (Unk8004AB64 *)p3;
                    nn->unk4 = p3 + n;
                }
            }
        }
    } else if (n <= 0x500) {
        nn = D_8007AECC->unk15D4;
        D_8007AECC->unk15D4 = nn->unk8;
        nn->unk8 = NULL;
        D_8007AECC->unk15D0 = nn;
        {
            u32 *p4 = D_8007AECC->unk50;
            nn->unk0 = (Unk8004AB64 *)p4;
            nn->unk4 = p4 + n;
        }
    }
    if (nn != NULL) {
        return nn->unk0;
    }
    return NULL;
}

Unk8004AB64 *func_800367D0(s32 idx, s32 id, s32 a2, s32 a3) {
    Unk8004AB64 *o;
    Unk16D8 *n;
    Unk8007AECC *d;
    s32 nw, nv;
    u32 *p;
    u32 *end;

    if (id == 0) {
        id = idx;
    }
    o = func_8004B040(id);
    if (o != NULL) {
        func_80036978(o);
    }
    nw = (a2 + 3) >> 2;
    nv = (((ScriptEntry *)D_8007ADF8)[idx].unk4 + 3) >> 2;
    o = func_80036650(nw + nv + a3);
    end = (u32 *)o + nw + nv;
    for (p = (u32 *)o; p < end; p++) {
        *p = 0;
    }
    d = D_8007AECC;
    n = d->unk16E0;
    d->unk16E0 = n->unk0;
    if (d->unk16DC != NULL) {
        d->unk16DC->unk0 = n;
        d->unk16DC = n;
    } else {
        d->unk16D8 = n;
        d->unk16DC = n;
    }
    n->unk0 = NULL;
    n->unk4 = o;
    o->unk6 = idx;
    o->unk7 = id;
    o->unk5 = 0;
    {
        u8 *h = D_8007ADF8 + ((ScriptEntry *)D_8007ADF8)[idx].unk0;
        s32 c = *(u16 *)h >> 8;
        u16 *q = (u16 *)(h + 2);

        while (c != 0) {
            if (*q++ == 0) {
                break;
            }
            c--;
            q++;
        }
        if (c != 0) {
            h += *q + 2;
        } else {
            h = NULL;
        }
        o->unk0 = h;
    }
    o->unk9 = nw + nv;
    o->unk8 = nw;
    o->unkC = a3;
    o->unkE = 0;
    o->unk10 = 0xFF;
    return o;
}

Unk16D8 *func_80036978(Unk8004AB64 *obj) {
    Unk16D8 *n;
    Unk16D8 *prev;
    Unk16D8 *next = NULL;

    prev = NULL;
    for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
        if (n->unk4 == obj) {
            break;
        }
        prev = n;
    }
    if (n != NULL) {
        next = n->unk0;
        if (prev != NULL) {
            prev->unk0 = next;
        } else {
            D_8007AECC->unk16D8 = next;
        }
        if (D_8007AECC->unk16DC == n) {
            D_8007AECC->unk16DC = prev;
        }
        n->unk0 = D_8007AECC->unk16E0;
        D_8007AECC->unk16E0 = n;
        func_8004B2F4_inline(obj);
        if (D_8007AECC->playerUid == n->unk4->unk7) {
            D_8007AECC->playerUid = 0;
        }
    }
    return next;
}

s32 func_80036AAC(s32 op) {
    s32 r = 0;
    s32 w = D_8007B6F0[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0--;
        mask = func_80052D40();
        D_8007B6F0 += 2;
    }
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            D_8007B700 = D_8007AED0[i];
            switch (op) {
            case 8:
                r = func_80052D40();
                D_8007B6F0++;
                func_80052D70(r + 1);
                break;
            case 9:
                r = func_80052D40();
                D_8007B6F0++;
                func_80052D70(r - 1);
                break;
            case 10:
                r = func_80052D40() + 1;
                D_8007B6F0++;
                func_80052D70(r);
                break;
            case 11:
                r = func_80052D40() - 1;
                D_8007B6F0++;
                func_80052D70(r);
                break;
            }
            D_8007B6F0++;
        }
        mask >>= 1;
    }
    D_8007B6F0 -= 2;
    return r;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800110A0);

s32 func_80036CCC(s32 op) {
    s32 r = 0;
    s32 w = D_8007B6F0[-3];
    s32 mask;
    s32 v;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0 -= 2;
        mask = func_80052D40();
        D_8007B6F0 += 3;
    }
    v = func_80052D40();
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            D_8007B700 = D_8007AED0[i];
            switch (op) {
            case 0x2D:
                r = v;
                break;
            case 0x36:
                r = func_80052D40() * v;
                D_8007B6F0++;
                break;
            case 0x37:
                if (v != 0) {
                    r = func_80052D40() / v;
                } else {
                    r = func_80052D40();
                }
                D_8007B6F0++;
                break;
            case 0x38:
                if (v != 0) {
                    r = func_80052D40() % v;
                } else {
                    r = func_80052D40();
                }
                D_8007B6F0++;
                break;
            case 0x39:
                r = func_80052D40() + v;
                D_8007B6F0++;
                break;
            case 0x3A:
                r = func_80052D40() - v;
                D_8007B6F0++;
                break;
            case 0x3B:
                r = func_80052D40() << v;
                D_8007B6F0++;
                break;
            case 0x3C:
                r = func_80052D40() >> v;
                D_8007B6F0++;
                break;
            case 0x40:
                r = func_80052D40() & v;
                D_8007B6F0++;
                break;
            case 0x41:
                r = func_80052D40() ^ v;
                D_8007B6F0++;
                break;
            case 0x42:
                r = func_80052D40() | v;
                D_8007B6F0++;
                break;
            }
            func_80052D70(r);
            D_8007B6F0++;
        }
        mask >>= 1;
    }
    D_8007B6F0 -= 2;
    return r;
}

s32 func_80036FFC(s32 op) {
    s32 r = 0;
    s32 idx = r;
    s32 w = D_8007B6F0[-2];
    s32 mask;
    s32 i;
    s32 x;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0--;
        mask = func_80052D40();
        D_8007B6F0 += 2;
    }
    switch (op) {
    case 0xF:
        for (i = 0; i < 8; i++) {
            r >>= 1;
            if (mask & 1) {
                D_8007B700 = D_8007AED0[i];
                if (func_80052D40() == 0) {
                    r |= 0x80;
                }
                D_8007B6F0++;
            }
            mask >>= 1;
        }
        break;
    case 0x4B:
        r = -0x8000;
        for (i = 0; i < 8; i++) {
            if (mask & 1) {
                D_8007B700 = D_8007AED0[i];
                x = func_80052D40();
                D_8007B6F0++;
                if (r < x) {
                    r = x;
                    idx = i;
                }
            }
            mask >>= 1;
        }
        r = 1 << idx;
        break;
    case 0x4C:
        r = 0x7FFF;
        for (i = 0; i < 8; i++) {
            if (mask & 1) {
                D_8007B700 = D_8007AED0[i];
                x = func_80052D40();
                D_8007B6F0++;
                if (x < r) {
                    r = x;
                    idx = i;
                }
            }
            mask >>= 1;
        }
        r = 1 << idx;
        break;
    }
    D_8007B6F0 -= 2;
    return r;
}

s32 func_800372A4(s32 op) {
    s32 r = 0;
    s32 c = r;
    s32 w = D_8007B6F0[-3];
    s32 mask;
    s32 v;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0 -= 2;
        mask = func_80052D40();
        D_8007B6F0 += 3;
    }
    v = func_80052D40();
    for (i = 0; i < 8; i++) {
        r >>= 1;
        if (mask & 1) {
            D_8007B700 = D_8007AED0[i];
            switch (op) {
            case 0x1C:
                c = func_80052D40() < v;
                break;
            case 0x1D:
                c = func_80052D40() > v;
                break;
            case 0x1E:
                c = func_80052D40() <= v;
                break;
            case 0x1F:
                c = func_80052D40() >= v;
                break;
            case 0x22:
                c = func_80052D40() == v;
                break;
            case 0x23:
                c = func_80052D40() != v;
                break;
            case 0x54:
                c = (func_80052D40() & v) != 0;
                break;
            case 0x55:
                c = (func_80052D40() & v) == 0;
                break;
            case 0x56:
                c = (func_80052D40() ^ v) != 0;
                break;
            case 0x57:
                c = (func_80052D40() | v) != 0;
                break;
            }
            r |= c << 7;
            D_8007B6F0++;
        }
        mask >>= 1;
    }
    D_8007B6F0 -= 2;
    return r;
}

s32 func_80037528(s32 op) {
    s32 r = 0;
    s32 w = D_8007B6F0[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0--;
        mask = func_80052D40();
        D_8007B6F0 += 2;
    }
    for (i = 0; i < 8; i++) {
        r >>= 1;
        if (mask & 1) {
            D_8007B700 = D_8007AED0[i];
            if (func_80052D40() != 0) {
                r |= 0x80;
            }
            D_8007B6F0++;
        }
        mask >>= 1;
    }
    D_8007B6F0 -= 2;
    switch (op) {
    case 0x43:
        r &= func_80052D40();
        D_8007B6F0++;
        break;
    case 0x44:
        r ^= func_80052D40();
        D_8007B6F0++;
        break;
    case 0x45:
        r |= func_80052D40();
        D_8007B6F0++;
        break;
    }
    func_80052D70(r);
    return r;
}

s32 func_80037738(void) {
    s32 r = 0;
    s32 w = D_8007B6F0[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        D_8007B6F0--;
        mask = func_80052D40();
        D_8007B6F0 += 2;
    }
    i = 0;
    if (!(mask & 1)) {
        i = 1;
        mask >>= 1;
        while (i < 8 && !(mask & 1)) {
            mask >>= 1;
            i++;
        }
    }
    if (i < 8) {
        D_8007B700 = D_8007AED0[i];
        r = func_80052D40();
    } else {
        D_8007B6F0--;
    }
    D_8007B6F0--;
    return r;
}

s32 func_800378C4(s32 slot) {
    s32 i;
    s32 r;

    if (func_8004B85C_find(func_8004B85C_key(slot), slot)) {
        r = 1;
    } else {
        slot = func_8004B85C_key(slot);
        if (slot >= 0 && slot < 9) {
            for (i = 0; i < 4; i++) {
                if (g_gameState->unk1C->party[i] == NULL) {
                    break;
                }
            }
            r = i >= 4;
            if (r == 0) {
                func_80033AF4(i, slot);
                func_80039C38_party();
            }
        } else {
            r = 1;
        }
    }
    return r;
}

s32 func_80037ACC(s32 op) {
    s32 r = 0;
    s16 px;
    s16 py;
    s32 x;
    GameData *gd;
    Unk8004AB64 *o;

    switch (op) {
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
        r = func_80036AAC(op);
        break;
    case 0xF:
    case 0x4B:
    case 0x4C:
        r = func_80036FFC(op);
        break;
    case 0x2E:
    case 0x43:
    case 0x44:
    case 0x45:
        r = func_80037528(op);
        break;
    case 0x64:
        x = func_80052D40();
        if (x < 0x100) {
            r = func_80027F34(x);
        } else if (x < 0x200) {
            r = (u8)func_800281A4(x - 0x100);
        } else {
            r = func_80062560(x - 0x200);
        }
        break;
    case 0x65:
        func_800C7EE8(func_80052D40(), &r);
        break;
    case 0x52:
        gd = g_gameState->unk1C;
        r = gd->characters[func_80037ACC_chr(func_80052D40())].hp;
        break;
    case 0x53:
        gd = g_gameState->unk1C;
        r = gd->characters[func_80037ACC_chr(func_80052D40())].maxHp;
        break;
    case 0x6E:
        gd = g_gameState->unk1C;
        r = gd->characters[func_80037ACC_chr(func_80052D40())].mp;
        break;
    case 0x6F:
        gd = g_gameState->unk1C;
        r = gd->characters[func_80037ACC_chr(func_80052D40())].maxMp;
        break;
    case 0x70:
        px = -1;
        o = (Unk8004AB64 *)func_80052D40();
        if (o != NULL && o->unk5 == 4) {
            func_800C76E8(o->unk3B, (u16 *)&px, (u16 *)&py);
        }
        r = px;
        break;
    case 0x71:
        py = -1;
        o = (Unk8004AB64 *)func_80052D40();
        if (o != NULL && o->unk5 == 4) {
            func_800C76E8(o->unk3B, (u16 *)&px, (u16 *)&py);
        }
        r = py;
        break;
    case 0x6C:
        func_80052D40();
    case 0x5A:
    case 0x5B:
    case 0x5C:
        func_80052D40();
        break;
    }
    return r;
}

s32 func_80037D6C(s32 op) {
    s32 r;
    u8 buf[8];
    Unk8004AB64 *o;
    s32 id;

    switch ((u32)op) {
    case 0:
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        break;
    case 1:
        func_800C1B5C(buf);
        r = buf[0];
        break;
    case 2:
        r = D_8007AECC->unk16EC;
        break;
    case 3:
        r = func_800B0124(0xC00, -1, 0, 0, 0);
        break;
    case 4:
        r = D_8007AE20;
        break;
    case 5:
        id = ((u8 *)&((u32 *)D_8007B70C + D_8007B70C->unk9)[D_8007B70C->unkA])[-1];
        o = func_8004B040_inline(id);
        r = -1;
        if (o != NULL) {
            r = o->unk6 - (D_8007ADFC - 9);
        }
        break;
    case 6:
        r = g_gameState->unk1C->gil;
        break;
    case 7:
        r = D_8007AEC4;
        break;
    case 8:
        r = D_8007BBD8;
        break;
    case 9:
        r = func_8004ECC4();
        break;
    case 10:
        r = D_8007AE24;
        break;
    case 11:
        r = D_8007AE28;
        break;
    case 12:
        r = D_8007AE2C;
        break;
    case 13:
        r = D_8007AE30;
        break;
    case 14:
        r = func_800AD944(4, 0, 0);
        break;
    case 15:
        r = func_800AD944(3, 0, 0);
        break;
    case 16:
        r = g_gameState->unk1C->unk568;
        break;
    case 17:
        r = func_8002414C();
        break;
    case 18:
        r = func_8006275C();
        break;
    case 19:
        r = func_800625F8();
        break;
    case 20:
        r = func_80024048();
        if (r > 0x7FFFFF) {
            r = 0x7FFFFF;
        }
        break;
    case 21:
        r = func_800240E8();
        break;
    case 22:
        r = D_8007AE38;
        break;
    case 23:
        r = D_8007AE3C;
        break;
    case 24:
        r = (s16)g_gameState->unk1C->unkE;
        break;
    case 25:
        r = (u8)func_800CA774();
        break;
    case 26:
        r = D_8007B708 == 4;
        break;
    case 27:
        r = g_gameState->unk1C->unk56E;
        break;
    case 28:
        r = (D_8007AE34 >> 8) & 0xFF;
        break;
    case 29:
        r = (u8)D_8007AE34;
        break;
    case 30:
        r = g_gameState->unk1C->unk8E4.ovl65->unkD80;
        break;
    case 31:
        r = g_gameState->unk1C->unk8E4.ovl65->unk1340;
        break;
    default:
        if (op >= 0xC0) {
            r = func_800B7A14(op);
        } else {
            r = func_800BE784(op);
        }
        break;
    }
    return r;
}

Unk8004AB64 *func_80038150(void) {
    return func_8004B4C4_inline(func_8004AEDC_inline2());
}

/* Find the entry point for `key` in script `idx`. */
static inline u8 *func_800384D0_find(s32 idx, s32 key) {
    u8 *h = D_8007ADF8 + *(u16 *)(D_8007ADF8 + idx * 8);
    s32 n = *(u16 *)h >> 8;
    u16 *p = (u16 *)(h + 2);

    while (n != 0) {
        if (*p++ == key) {
            break;
        }
        n--;
        p++;
    }
    return n != 0 ? h + (*p + 2) : NULL;
}

void func_80038278(Unk8004AB64 *o) {
    s32 v;
    Unk16D8 *n;
    Unk16D8 *m;
    Unk8004AB64 *t;
    Unk8004AB64 *q;
    u32 *base;
    u32 *p;

    if (o->unkA >= 2) {
        p = (u32 *)o + o->unk9 + o->unkA;
        p--;
        v = p[0];
        if (!(*(u32 *)&o->unk4 & 0xFF0000FF)) {
            for (m = D_8007AECC->unk16D8; m != NULL; m = m->unk0) {
                t = m->unk4;
                if (t->unk7 != 0) {
                    t->unkE = t->unk12;
                }
            }
        }
        o->unk0 = D_8007AECC->unk16F0 + p[-1];
        o->unkD = v;
        o->unkA -= 2;
        o->unk4 = v >> 8;
        if (((v >> 16) & 0xFF) != 0xFF) {
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                if (n->unk4->unk7 == (u32)v >> 24) {
                    break;
                }
            }
            q = NULL;
            if (n != NULL) {
                q = n->unk4;
            }
            if (q != NULL) {
                if (q->unkD == 0xFF) {
                    q->unkD = 0;
                    return;
                }
                base = (u32 *)q + q->unk9;
                p = base + (q->unkA - 1);
                while (base < p) {
                    if (*(u8 *)p == 0xFF) {
                        break;
                    }
                    p -= 2;
                }
                if (base < p) {
                    *p &= ~0xFF;
                }
            }
        }
    } else if (o->unkE == 2) {
        o->unkE = 1;
        o->unk0 = func_800384D0_find(o->unk6, 1);
        o->unk4 = 7;
    } else {
        o->unk0 = NULL;
    }
}

s32 func_800384D0(Unk8004AB64 *o, s32 lv, s32 key, s32 save) {
    u8 *e = NULL;
    u32 *slot;
    u16 w;
    u32 hi;
    u32 x;

    if (o != NULL && lv < o->unk4) {
        e = func_800384D0_find(o->unk6, key);
        if (e != NULL) {
            slot = &((u32 *)o + o->unk9)[o->unkA];
            slot[0] = o->unk0 - D_8007AECC->unk16F0;
            w = o->unkD | (o->unk4 << 8);
            hi = D_8007B70C->unk7 << 24;
            if (save) {
                x = (D_8007B70C->unk4 << 16);
                x = x | w;
                x |= hi;
            } else {
                x = hi | 0xFF0000;
                x = x | w;
            }
            slot[1] = x;
            if (save) {
                D_8007B70C->unkD = 0xFF;
            }
            o->unk0 = e;
            o->unk4 = lv;
            o->unkD = 0;
            o->unkA += 2;
        }
    }
    return e != NULL;
}

void *func_80038618(Unk8004AB64 *o, s32 flags, s32 *dist) {
    Unk8004AB64 *best = NULL;
    s32 bestd = 0;
    s32 x, y, z;
    s32 back;
    s32 r;
    Unk16D8 *n;
    Unk8004AB64 *q;
    s32 a;
    s32 b;
    s32 dx, dz;
    s32 rr;
    s32 d;
    s32 s;
    u8 f;
    u8 g;
    s32 key;

    x = o->pos.vx;
    y = o->pos.vy;
    back = flags & 4;
    z = o->pos.vz;
    if (back) {
        r = o->unk3D * 4;
    } else {
        r = o->unk3C * 4;
    }
    for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
        q = n->unk4;
        a = 0;
        if (back) {
            a = 1;
        } else {
            g = o->unkF;
            if (q->unk7 == D_8007AECC->playerUid ? (g & 2) : (g & 4)) {
                a = 1;
            }
        }
        s = q->unkF;
        if (best) {
            f = s;
        } else {
            f = q->unkF;
        }
        if (back) {
            b = f & 8;
        } else if (o->unk7 == D_8007AECC->playerUid) {
            b = f & 2;
        } else {
            b = f & 4;
        }
        if (q == o || (a && b)) {
            continue;
        }
        if ((flags & 6) && func_800384D0_find(q->unk6, back ? 3 : 2) == NULL) {
            continue;
        }
        if (q->unk5 != 4) {
            continue;
        }
        {
            Unk8004AB64 *p = q;
        if ((u32)(p->pos.vy - y + 399) >= 799) {
            continue;
        }
        dx = p->pos.vx - x;
        dz = p->pos.vz - z;
        if ((dx < 0 ? -dx : dx) >= 0x800) {
            continue;
        }
        if ((dz < 0 ? -dz : dz) >= 0x800) {
            continue;
        }
        if (back) {
            rr = p->unk3D * 4;
        } else {
            if (a) {
                rr = p->unk3C * 4;
            } else {
                rr = p->unk3C * 4;
            }
        }
        if (p->unk2D != 0) {
            s = func_8004BB7C(func_800531D0(p, o, p->pos.rotY));
            rr = (rr * func_80052EEC(((s * s) >> 4) * p->unk2D + 0x1000000)) >> 12;
        }
        rr += r;
        if (flags & 6) {
            rr += 0x3C + p->unk5C;
        }
        d = func_80052DEC(dx, dz, rr);
        if (back) {
            s = func_800531D0(o, p, o->pos.rotY);
            if (s < 0) {
                s = -s;
            }
            s -= 640;
            d += s;
            if (s >= 0) {
                continue;
            }
        }
        if (d < bestd) {
            bestd = d;
            best = p;
        }
        }
    }
    if (dist != NULL) {
        *dist = bestd;
    }
    return best;
}

s32 func_80038990(s32 x, s32 y, s32 z, s32 flags) {
    Unk8004AB64 *p;
    s32 dz;
    s32 dx;
    s32 dy;
    s32 speed;
    s16 v[4];
    s32 dist;
    s32 ry;
    s32 rx;
    s32 cur;
    s32 d;
    s32 step;
    s32 hit;
    s32 r;

    p = D_8007B704;
    speed = p->unk5C;
    dx = x - p->pos.vx;
    dz = z - p->pos.vz;
    if (p->unk50 & 0x20) {
        ry = p->pos.rotY & 0xFFF;
    } else {
        ry = func_80052F24(dx, dz);
        if (!(flags & 1)) {
            cur = p->pos.rotY & 0xFFF;
            d = ry - cur;
            if (d < -0x800) {
                d += 0x1000;
            } else if (d >= 0x800) {
                d -= 0x1000;
            }
            if (d == 0) {
                p->unk50 |= 0x20;
            }
            step = p->unk5F * 8;
            if (d > 0) {
                if (step < d) {
                    ry = cur + step;
                }
            } else {
                if (d < -step) {
                    ry = cur - step;
                }
            }
            ry &= 0xFFF;
            p->pos.rotY = ry;
        }
    }
    if (flags & 2) {
        dy = y - p->pos.vy;
        rx = func_80052F24(-dy, -func_80052EC0(dx, 0, dz)) & 0xFFF;
        cur = p->unk30 & 0xFFF;
        d = rx - cur;
        if (d < -0x800) {
            d += 0x1000;
        } else if (d >= 0x800) {
            d -= 0x1000;
        }
        step = p->unk5F * 8;
        if (d > 0) {
            if (step < d) {
                rx = cur + step;
            }
        } else {
            if (d < -step) {
                rx = cur - step;
            }
        }
        rx &= 0xFFF;
        p->unk30 = rx;
    } else {
        rx = 0;
        dy = 0;
    }
    func_8005309C(v, rx, ry, speed);
    p->pos.vx += v[0];
    p->pos.vy += v[1];
    p->pos.vz += v[2];
    hit = func_80038618(p, 0, &dist);
    if (hit != 0 && dist <= 0) {
        p->pos.vx -= v[0];
        p->pos.vy -= v[1];
        p->pos.vz -= v[2];
    }
    if (p->unk63 != 0 && p->unk63 != 0xFF) {
        p->unk63--;
    }
    r = p->unk63 != 0;
    if (flags < 0) {
        r = 0;
        if (hit != flags || dist > 0) {
            r = 1;
        }
    }
    dist = func_80052E48(dx, dy, dz);
    if (dist < speed * speed || ((p->unk50 & 0x10) && p->unk78 < dist)) {
        r = 0;
    }
    if (p->unk78 < 0x7FFFFFFF && dist < p->unk78) {
        p->unk50 |= 0x10;
    }
    p->unk78 = dist;
    if (r == 0) {
        p->unk78 = 0x7FFFFFFF;
        p->unk30 = 0;
        p->unk50 &= ~0x30;
    }
    if (p->unk7 == D_8007AECC->playerUid) {
        D_8007B710++;
    }
    return r;
}

s32 func_80038D3C(Unk8004AB64 *p) {
    s32 id = 0;

    if (D_8007AECC->unk16EC != 0 && D_8007AE4C > 0x3C0) {
        D_8007AE4C = 0;
        D_8007AE50 += D_8007AECC->unk16EF;
        if (func_80038D3C_rand() < (D_8007AE50 >> 3)) {
            D_8007AE50 = 0;
            id = func_80038D3C_pick();
            if (id == D_8007AE54) {
                id = func_80038D3C_pick();
            }
            D_8007AE54 = id;
            func_80021660(id);
            g_gameState->unk1C->unk848 = -1;
        }
    }
    return id != 0;
}

/* screen position of an actor's speech-bubble anchor */
static inline void func_80038FD8_pos(Unk8004AB64 *obj, s32 *x, s32 *y) {
    SVECTOR sv;

    sv.vx = obj->pos.vx;
    sv.vy = obj->pos.vy + ((obj->unk32 * (s8)obj->unk2F) >> 6) - 50;
    sv.vz = obj->pos.vz;
    if (obj->unk5 == 4) {
        sv.vx += obj->unk8A;
        sv.vy += obj->unk8C;
        sv.vz += obj->unk8E;
    }
    func_8004BA20(&sv, x, y);
}

static inline void func_80038FD8_x(WinLayout *w, s32 sx, s32 right, s32 *ptx) {
    s32 width = w->unk2;
    s32 tx, l, r;

    if (right) {
        tx = sx - 6;
        if (tx < 32) {
            tx = 32;
        }
        if (tx > 304) {
            tx = 304;
        }
        l = tx + 8 - width;
        r = tx - 24;
    } else {
        tx = sx + 6;
        if (tx > 288) {
            tx = 288;
        }
        if (tx < 16) {
            tx = 16;
        }
        l = tx + 24 - width;
        r = tx - 8;
    }
    *ptx = tx;
    if (l < 8) {
        l = 8;
    }
    if (r > 312 - width) {
        r = 312 - width;
    }
    w->unk0 = (l + r) >> 1;
}

static inline void func_80038FD8_y(WinLayout *w, s32 sy, s32 *below, s32 fixed) {
    s32 h = w->unk9;
    s32 ty;

    if (*below) {
        ty = sy - h - 16;
        if (!fixed && ty < 8) {
            *below ^= 1;
            ty = sy + 32;
        }
    } else {
        ty = sy + 32;
        if (!fixed && ty > 212 - h) {
            *below = 1;
            ty = sy - h - 16;
        }
    }
    if (ty < 8) {
        ty = 8;
    } else if (ty > 212 - h) {
        ty = 212 - h;
    }
    w->unk8 = ty;
}

void func_80038FD8(Unk8004AB64 *obj, WinLayout *w, s32 dir, s32 fixed) {
    s32 sx, sy;
    s32 ox, oy;
    s32 tx;
    s32 below;
    s32 right;
    s32 id;
    Unk8004AB64 *other;

    if (obj->unk5 == 4) {
        obj->unk8A = D_8007AE5C;
        obj->unk8C = D_8007AE60;
        obj->unk8E = D_8007AE64;
    }
    func_80038FD8_pos(obj, &sx, &sy);
    if (dir < 0) {
        if (obj->unk7 == D_8007AECC->playerUid) {
            id = obj->unk93;
        } else {
            id = D_8007AECC->playerUid;
        }
        other = func_8004B040_inline(id);
        if (other != NULL) {
            func_80038FD8_pos(other, &ox, &oy);
            below = sy < oy;
            right = sx < ox;
        } else {
            below = 1;
            right = 0;
        }
    } else {
        below = (dir >> 1) & 1;
        right = dir & 1;
    }
    if (fixed == 0) {
        right = right ? right ^ (sx < 40) : sx > 280;
    }
    func_80038FD8_x(w, sx, right, &tx);
    func_80038FD8_y(w, sy, &below, fixed);
    if (fixed == 0 && func_8004EBC0((Unk8004EBC0 *)w)) {
        below ^= 1;
        func_80038FD8_y(w, sy, &below, 0);
    }
    w->unk4 = tx - w->unk0;
    w->unk6 = (below << 1) | right;
}

/* bubble x/y targets for func_80039444 (func_80038FD8_x/_y without the flip logic) */
static inline s32 func_80039444_x(s32 width, s32 sx, s32 right, s32 *ptx) {
    s32 tx, l, r;

    if (right) {
        tx = sx - 6;
        if (tx < 32) {
            tx = 32;
        }
        if (tx > 304) {
            tx = 304;
        }
        l = tx + 8 - width;
        r = tx - 24;
    } else {
        tx = sx + 6;
        if (tx > 288) {
            tx = 288;
        }
        if (tx < 16) {
            tx = 16;
        }
        l = tx + 24 - width;
        r = tx - 8;
    }
    *ptx = tx;
    if (l < 8) {
        l = 8;
    }
    if (r > 312 - width) {
        r = 312 - width;
    }
    return (l + r) >> 1;
}

static inline s32 func_80039444_y(s32 h, s32 *sy, s32 below) {
    s32 ty;

    if (below) {
        ty = *sy - h - 16;
    } else {
        ty = *sy + 32;
    }
    if (ty < 8) {
        ty = 8;
    } else if (ty > 212 - h) {
        ty = 212 - h;
    }
    return ty;
}

void func_80039444(Unk8004AB64 *obj, Window *w) {
    s32 sx, sy;
    s32 tx;
    s32 ox, oy;
    s32 right, below;
    s32 x, y;

    ox = w->unk0;
    oy = w->unk2;
    func_80038FD8_pos(obj, &sx, &sy);
    below = (w->unk29 >> 1) & 1;
    right = w->unk29 & 1;
    x = func_80039444_x(w->unk4 - 3, sx, right, &tx);
    y = func_80039444_y(w->unk6 - 3, &sy, below);
    w->unk0 = (ox + x) >> 1;
    w->unk2 = (oy + y) >> 1;
    w->unk2A = tx - w->unk0 - right * 16;
    w->unk8.x += w->unk0 - ox;
    w->unk8.y += w->unk2 - oy;
}

WinLayout *func_80039660(Unk8004AB64 *obj, s32 id, s32 flags) {
    WinLayout *w = func_8004E248(id, &D_8007AE5C, &D_8007AE60, &D_8007AE64);
    s32 ww;
    s32 wh;

    w->unk4 = 0;
    w->unk6 = 0;
    if ((flags & 0x40) && w->unk2 < 0x72) {
        w->unk2 = 0x72;
    }
    if (flags & 0x80) {
        if (w->unk2 < 0x1D) {
            w->unk2 = 0x1D;
        }
        switch (w->unkA) {
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
            func_80038FD8(obj, w, w->unkA & 3, 0);
            break;
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
            func_80038FD8(obj, w, w->unkA & 3, 1);
            break;
        default:
            func_80038FD8(obj, w, -1, 0);
            break;
        }
    } else {
        ww = w->unk2;
        wh = w->unk9;
        switch (w->unkA) {
        case 0x53:
            w->unk0 = 12;
            w->unk8 = 12;
            break;
        case 0x55:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 12;
            break;
        case 0x52:
            w->unk0 = 308 - ww;
            w->unk8 = 12;
            break;
        case 0x5C:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 74 - (wh >> 1);
            break;
        case 0x51:
            w->unk0 = 12;
            w->unk8 = -48 - wh;
            break;
        case 0x54:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = -48 - wh;
            break;
        case 0x50:
            w->unk0 = 308 - ww;
            w->unk8 = -48 - wh;
            break;
        case 0:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 110 - (wh >> 1);
            break;
        }
    }
    if ((flags & 4) && w->unk2 < 0x1D) {
        w->unk2 = 0x1D;
    }
    w->unk6 |= flags & 0x30;
    return w;
}

void func_800398D0(Unk8004AB64 *p, s32 d, s32 wrap, s32 speed) {
    s32 cur;
    s32 rate;
    s32 frames;
    s16 a;

    p->unk52 = d & 0xFFF;
    cur = p->pos.rotY & 0xFFF;
    d -= cur;
    if (wrap) {
        if (d > 0x800) {
            d -= 0x1000;
        } else if (d < -0x800) {
            d += 0x1000;
        }
    }
    if (d == 0) {
        return;
    }
    p->unk60 = 0;
    p->unk61 = 0xFF;
    p->unk80 = cur;
    p->unk5A = p->unk36;
    p->unkF |= 0x80;
    if (speed == 0) {
        speed = 0x10;
    }
    if (d < 0) {
        frames = func_800216D8(p->unk34, (u16)p->unk40.unk20);
        rate = frames * 16 / speed;
        if (-d < 0x400) {
            rate = rate * -d >> 10;
        }
        if (rate >= 6) {
            p->unk6C = (d + 0x400) / rate;
            p->unk62 = 0;
            func_80039C38_anim(p, (u16)p->unk40.unk20);
            if (p->unk7 == D_8007AECC->playerUid) {
                D_8007B710++;
            }
            p->unk5D = frames * 16 / rate;
        } else {
            p->unk6C = 0x7FFE;
            a = cur;
            if (p->unk52 >= cur) {
                a = cur | 0x1000;
            }
            p->pos.rotY = a;
        }
    } else {
        frames = func_800216D8(p->unk34, (u16)p->unk40.rotY);
        rate = frames * 16 / speed;
        if (d < 0x400) {
            rate = rate * d >> 10;
        }
        if (rate >= 6) {
            p->unk6C = (d - 0x400) / rate;
            p->unk62 = 0;
            func_80039C38_anim(p, (u16)p->unk40.rotY);
            if (p->unk7 == D_8007AECC->playerUid) {
                D_8007B710++;
            }
            p->unk5D = frames * 16 / rate;
        } else {
            p->unk6C = 0x7FFF;
            a = cur;
            if (cur >= p->unk52) {
                a = cur | 0xF000;
            }
            p->pos.rotY = a;
        }
    }
    if (rate <= 0) {
        rate = 1;
    }
    p->unk82 = d / rate;
}

s32 func_80039C38(void) {
    Unk8004AB64 *po = D_8007B704;
    Unk8004AB64 *actor = po;
    Unk8004AB64 *obj;
    Unk8004AB64 *o;
    Unk16D8 *n;
    s32 op;
    s32 a, b, c, d, e, f, i;
    SVECTOR sv;
    Unk80039C38Pt *pt;
    Unk8004AFF0 *m, *m2;
    u8 *str;

    D_8007AEA0 = D_8007B70C->unk0;
    op = *D_8007B70C->unk0++;
    D_8007B6F4 = *D_8007B70C->unk0++;
    D_8007B6F8 = 0;

    switch (op) {
    case 0x07:
        func_8004B3A8_inline(D_8007B6F4, *D_8007B70C->unk0++);
        D_8007B6F8 = 1;
        return 0;
    case 0x08:
        func_8004B3D0_inline(D_8007B6F4, *D_8007B70C->unk0++);
        D_8007B6F8 = 1;
        return 0;
    case 0x09:
        a = D_8007B6F4;
        b = *D_8007B70C->unk0++;
        if ((u32)(a - 0xFB) < 4) {
            a = D_8007AECC->unk16E4[a - 0xFB];
        }
        func_80039C38_newactor(a, b);
        D_8007B6F8 = 1;
        return 0;
    case 0x46:
    case 0x79:
    case 0xD2:
    case 0xD3:
        return 0;
    case 0x10:
        a = func_8004AEDC_inline();
        func_800384D0(func_80038150(), a, *D_8007B70C->unk0++, 0);
        return 0;
    case 0x12:
        a = func_8004AEDC_inline();
        obj = func_80038150();
        c = *D_8007B70C->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, c, 0);
            return 0;
        }
        D_8007B70C->unk0 = D_8007AEA0;
        D_8007B6F8 = 1;
        return 1;
    case 0x14:
        a = func_8004AEDC_inline();
        obj = func_80038150();
        c = *D_8007B70C->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, c, 1);
        } else {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x16:
        a = func_8004AEDC_inline();
        func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 0);
        return 0;
    case 0x18:
        a = func_8004AEDC_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 0);
            return 0;
        }
        D_8007B70C->unk0 = D_8007AEA0;
        D_8007B6F8 = 1;
        return 1;
    case 0x1A:
        a = func_8004AEDC_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *D_8007B70C->unk0++, 1);
        } else {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x97:
        obj = func_80038150();
        if (obj != NULL) {
            obj->unkA = 0;
            obj->unkE = 2;
            func_80038278(obj);
        }
        return obj == D_8007B70C;
    case 0x93:
        D_8007B704->unkF &= 0xC0;
        D_8007B704->unkF |= func_8004AEDC_inline() & 0x3F;
        return 0;
    case 0x6A:
        D_8007AECC->unk16F8 = 1;
        return 0;
    case 0xF0:
        D_8007AECC->unk16F8 = 0;
        return 0;
    case 0xD5:
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            Unk8004AB64 *o = n->unk4;
            if (o->unk5 == 4) {
                o->unk2C = o->unkF;
                if (!(o->unkF & 0x20)) {
                    o->unkF &= 0xFE;
                }
            }
        }
        return 0;
    case 0xD6:
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            Unk8004AB64 *o = n->unk4;
            if (o->unk5 == 4) {
                o->unkF &= 0xFE;
                o->unkF |= o->unk2C & 1;
            }
        }
        return 0;
    case 0x4B:
        if (D_8007B704->unk5 == 4) {
            a = func_8004AEDC_inline();
            if (D_8007B708 == 1 && po->unk34 != 0xFFFF) {
                func_800C7570(po->unk3B, (a << 2) & 0xFFFC);
            }
            po->unk3C = func_8004AEDC_inline();
            po->unk3D = func_8004AEDC_inline();
        } else {
            func_8004AEDC_inline();
            func_8004AEDC_inline();
            func_8004AEDC_inline();
        }
        return 0;
    case 0xA3:
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        func_8004AEDC_inline();
        return 0;
    case 0xDF:
        po->unk2D = func_8004AEDC_inline();
        return 0;
    case 0x3B:
        D_8007B70C->unk11 = func_8004AEDC_inline();
        return 0;
    case 0xBF:
        po = func_80038150();
    case 0x1D:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = D_8007B708 == 1 && po->unk34 != 0xFFFF;
        if (c) {
            func_800C3E18(po->unk3B, 0);
        }
        po->pos.vx = po->unk40.vx = a;
        po->pos.vy = po->unk40.vy = -0x8000;
        po->pos.vz = po->unk40.vz = b;
        if (c) {
            func_800C3E18(po->unk3B, 1);
        }
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        D_8007AEA4 = 1;
        return 0;
    case 0xAD:
        po = func_80038150();
    case 0xA1:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = D_8007B708 == 1 && po->unk34 != 0xFFFF;
        if (d) {
            func_800C3E18(po->unk3B, 0);
        }
        po->pos.vx = po->unk40.vx = a;
        po->pos.vy = po->unk40.vy = b;
        po->pos.vz = po->unk40.vz = c;
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        return 0;
    case 0x87:
        po = func_80038150();
    case 0x36:
        po->pos.rotY = func_8004AEDC_inline() << 4;
        return 0;
    case 0x37:
        po->pos.unk20 = func_8004AEDC_inline() << 4;
        po->unk24 = func_8004AEDC_inline() << 4;
        return 0;
    case 0x8B:
        actor->unk6A = func_8004AEDC_inline();
        actor->unk6B = func_8004AEDC_inline();
        actor->unk50 |= 3;
        return 0;
    case 0x47:
        actor->unk50 &= 0xFFF8;
        actor->unk50 |= func_8004AEDC_inline() & 7;
        return 0;
    case 0x91:
        actor->unk6E = func_8004AEDC_inline() ? 0xA7 : 4;
        return 0;
    case 0x99:
        actor->unk73 = func_8004AEDC_inline();
        if (actor->unk73 == 0) {
            actor->unk73 = 0x10;
        }
        return 0;
    case 0xBB:
        actor = func_80038150();
    case 0x56:
        a = func_8004AEDC_inline() << 4;
        func_800398D0(actor, a, 1, func_8004AEDC_inline());
        return 0;
    case 0xA7:
        func_800398D0(actor, func_8004AEDC_inline() << 4, 1, actor->unk73);
        return 0;
    case 0x9B:
        a = func_8004AF58_inline() - actor->pos.vx;
        b = func_8004AF58_inline() - actor->pos.vz;
        if (a | b) {
            func_800398D0(actor, func_80052F24(a, b), 1, actor->unk73);
        }
        return 0;
    case 0x51:
        obj = func_80038150();
        a = func_8004AEDC_inline();
        if (obj != NULL) {
            func_800398D0(actor, func_80052F24(obj->pos.vx - actor->pos.vx, obj->pos.vz - actor->pos.vz), 1, a);
        }
        return 0;
    case 0xBC:
        actor = func_80038150();
    case 0x50:
        if (actor->unkF & 0x80) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x66:
        a = func_8004AEDC_inline();
        func_8004EC74(a, func_8004AF58_inline());
        return 0;
    case 0xE3:
        D_8007BBD8 = func_8004AEDC_inline();
        return 0;
    case 0x7C:
        a = func_8004AF58_inline();
        func_8004ED00(a, func_8004AEDC_inline());
        return 0;
    case 0x53:
        func_8004EFA0();
        return 0;
    case 0x1F:
        D_8007B704->unk10 = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_80039C38_window(D_8007B704, c, D_8007B704->unk10);
        b = func_8004AF58_inline();
        func_8004DCEC(b, D_8007B704->unk10, func_80039660(D_8007B704, b, c), c);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x20:
        D_8007B704->unk10 = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_80039C38_window(D_8007B704, c, D_8007B704->unk10);
        b = func_8004AF58_inline();
        func_8004DCEC(b, D_8007B704->unk10, func_80039660(D_8007B704, b, c), c);
        return 0;
    case 0x95:
        po = func_80038150();
        D_8007B704->unk10 = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_80039C38_window(po, c, D_8007B704->unk10);
        b = func_8004AF58_inline();
        func_8004DCEC(b, D_8007B704->unk10, func_80039660(po, b, c), c);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x96:
        po = func_80038150();
        D_8007B704->unk10 = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_80039C38_window(po, c, D_8007B704->unk10);
        b = func_8004AF58_inline();
        func_8004DCEC(b, D_8007B704->unk10, func_80039660(po, b, c), c);
        return 0;
    case 0x21:
        a = func_8004AEDC_inline();
        func_8004EA4C(a);
        return 0;
    case 0xEB:
        func_8004E924();
        func_8002DD54();
        return 0;
    case 0x54:
        D_8007B704->unk10 = func_8004AEDC_inline();
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x8E:
        func_8004ED3C();
        return 0;
    case 0xD0:
        func_800F4F04((s32)func_8004F058(func_8004AF58_inline()), 3);
        return 0;
    case 0x25:
        actor->unk63 = 0xFF;
        actor->unk78 = 0x7FFFFFFF;
        actor->unk30 = 0;
        actor->unk50 &= 0xFFCF;
        return 0;
    case 0x98:
        actor->unk63 = func_8004AEDC_inline();
        return 0;
    case 0x23:
        a = func_8004AF58_inline();
        if (func_80038990(a, 0, func_8004AF58_inline(), 0)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xA5:
        a = func_8004AF58_inline();
        if (func_80038990(a, 0, func_8004AF58_inline(), 1)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x24:
        po = func_80038150();
        if (func_80038990(po->pos.vx, po->pos.vy, po->pos.vz, (s32)po)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        D_8007B6F8 = 1;
        return 1;
    case 0xA0:
        if (func_80038990(D_8007AE24, 0, D_8007AE28, 0)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xA2:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (func_80038990(a, b, c, 2)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xE8:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (func_80038990(a, b, c, 3)) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x55:
        a = func_8004AEDC_inline();
        if (a == 0) {
            a = 0xFF;
        }
        actor->unk5F = a;
        return 0;
    case 0x26:
        actor->unk5C = func_8004AEDC_inline();
        return 0;
    case 0xA6:
        actor->unk5E = func_8004AEDC_inline();
        return 0;
    case 0x9C:
        actor->unk62 = 8;
        actor->unk60 = 0;
        actor->unk61 = actor->unk90 - 1;
        func_80039C38_anim(actor, actor->unk76);
        if (actor->unk7 == D_8007AECC->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0x9D:
        ((Unk8E0 *)func_8001E350(actor->unk3B))->unk0 &= ~0x10;
        actor->unk61 = 0xFF;
        actor->unk62 = 0;
        actor->unk60 = actor->unk91 + 1;
        func_80039C38_anim(actor, actor->unk76);
        if (actor->unk7 == D_8007AECC->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0xE2:
        actor->unk94 = func_8004AF58_inline();
        actor->unkAA = func_8004AF58_inline();
        actor->unk96 = func_8004AF58_inline();
        a = func_8004AEDC_inline();
        if (a == 0) {
            a = 8;
        }
        actor->unk50 |= 8;
        ((Unk8E0 *)func_8001E350(actor->unk3B))->unk0 |= 0x10;
        actor->unk60 = actor->unk90;
        actor->unk61 = actor->unk91;
        func_80039C38_anim(actor, actor->unk76);
        if (actor->unk7 == D_8007AECC->playerUid) {
            D_8007B710++;
        }
        actor->unk68 = 0;
        actor->unk92 = a;
        actor->unkA4 = actor->pos.vx;
        actor->unkA6 = actor->pos.vy;
        actor->unkA8 = actor->pos.vz;
        actor->unk5D = ((actor->unk61 - actor->unk60) << 4) / a;
        if (D_8007B708 == 1 && po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, 0);
        }
        return 0;
    case 0xDC:
        b = actor->unk92;
        a = ++actor->unk68;
        actor->pos.vx = ((s16)actor->unkA4 * (b - a) + actor->unk94 * a) / b;
        actor->pos.vy = (s16)actor->unkA6 + a * a * 8 + a * (actor->unkAA - (s16)actor->unkA6) / b - a * (b << 3);
        actor->pos.vz = ((s16)actor->unkA8 * (b - a) + actor->unk96 * a) / b;
        if (a >= b) {
            return 0;
        }
        D_8007B70C->unk0 = D_8007AEA0;
        D_8007B6F8 = 1;
        return 1;
    case 0x29: {
        u32 cnt;
        pt = ((Unk80039C38Quad *)D_8007B704)->unk18;
        cnt = ((Unk80039C38Quad *)D_8007B704)->unk14 = func_8004AEDC_inline();
        while (cnt-- != 0) {
            pt->unk0 = func_8004AF58_inline();
            pt->unk2 = func_8004AF58_inline();
            pt++;
        }
        return 0;
    }
    case 0xA4:
        po = func_8004B040_inline(D_8007AECC->playerUid);
        if (po != NULL) {
            Unk80039C38Pt *p = ((Unk80039C38Quad *)D_8007B704)->unk18;
            s32 dx = p[1].unk0 - p[0].unk0;
            s32 dz = p[1].unk2 - p[0].unk2;
            s32 t = (dx * dx + dz * dz) >> 8;
            if (t != 0) {
                t = (dx * (po->pos.vx - p[0].unk0) + dz * (po->pos.vz - p[0].unk2)) / t;
                if (t < 0) {
                    t = 0;
                } else if (t > 0x100) {
                    t = 0x100;
                }
            }
            D_8007AE24 = ((t * dx) >> 8) + p[0].unk0;
            D_8007AE28 = ((t * dz) >> 8) + p[0].unk2;
        } else {
            D_8007AE28 = 0;
            D_8007AE24 = 0;
        }
        return 0;
    case 0x9E:
        po = func_8004B040_inline(D_8007AECC->playerUid);
        if (po != NULL) {
            s32 *sp = (s32 *)po + po->unk9 + po->unkA;
            Unk16D8 *nn;
            sp[0] = po->unk0 - D_8007AECC->unk16F0;
            sp[1] = po->unkD | po->unk4 << 8 | 0xFF << 16 | D_8007B70C->unk7 << 24;
            po->unk4 = 0;
            po->unkD = 0;
            po->unkA += 2;
            po->unk0 = D_8006E658;
            D_8007AECC->unk16EC = 0;
            for (nn = D_8007AECC->unk16D8; nn != NULL; nn = nn->unk0) {
                nn->unk4->unkF |= 6;
            }
        }
        return 0;
    case 0x76: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        q->unk14 = 2;
        q->unk18[0].unk0 = q->unk18[1].unk0 = func_8004AF58_inline();
        q->unk18[0].unk2 = q->unk18[1].unk2 = func_8004AF58_inline();
        return 0;
    }
    case 0x77: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        Unk80039C38Pt *p;
        a = q->unk14;
        p = &q->unk18[(a > 0 ? a : 1) - 1];
        p->unk0 = func_8004AF58_inline();
        p->unk2 = func_8004AF58_inline();
        return 0;
    }
    case 0x78: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        s32 k = q->unk14;
        if (k >= 1 && k < 8) {
            q->unk18[k] = q->unk18[k - 1];
            q->unk14++;
        }
        return 0;
    }
    case 0x2B:
        func_80021640(func_8004AF58_inline());
        return 4;
    case 0xB6:
        func_80021640(func_8004AF58_inline());
        return 5;
    case 0xAE:
        func_80021680(func_8004AF58_inline() & 0xFFFF);
        return 7;
    case 0xF5:
        return 8;
    case 0x2A:
        D_8007BD2C = func_8004AEDC_inline();
        g_gameState->unk1C->unk848 = -1;
        a = func_8004AF58_inline();
        g_gameState->unk1C->unk571 = (a >> 15) & 1;
        func_80021660(a & 0x7FFF);
        return 3;
    case 0x8C:
        D_8007BD2C = func_8004AEDC_inline();
        g_gameState->unk1C->unk848 = func_8004AEDC_inline();
        a = func_8004AF58_inline();
        g_gameState->unk1C->unk571 = (a >> 15) & 1;
        func_80021660(a & 0x7FFF);
        return 3;
    case 0x57:
        if ((D_8007AECC->unk16EF = func_8004AEDC_inline()) == 0) {
            D_8007AE50 = 0;
        }
        return 0;
    case 0x3C:
        D_8007AE40.unk8 = func_8004AEDC_inline();
        D_8007AE40.unk0 = func_8004AF58_inline();
        D_8007AE40.unk2 = func_8004AF58_inline();
        D_8007AE40.unk4 = func_8004AF58_inline();
        D_8007AE40.unk6 = func_8004AF58_inline();
        return 0;
    case 0x2C:
        D_8007AECC->playerUid = D_8007B70C->unk7;
        return 0;
    case 0x2D:
        D_8007AECC->unk16EC = 0;
        func_8004FF98(0);
        D_8007B710 = 0;
        D_8007BBC8 = 0;
        return 0;
    case 0x2E: {
        Unk8007AECC *e;
        D_8007AECC->unk16EC = 1;
        func_8004FF98(1);
        e = D_8007AECC;
        if (e->unk1700 >= 0) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            e->unk1700 = (*g_randPtr++ + 0xC8) * 4;
        }
        return 0;
    }
    case 0x2F:
        po->unk34 = func_8004AF58_inline();
        po->unk3B = func_80021698(D_8007B70C);
        D_8007B70C->unkF |= 1;
        po->unk32 = -(func_8004AEDC_inline() << 2);
        return 0;
    case 0xD4:
        m = func_8004AFF0_inline(D_8007B704);
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        if (m != NULL) {
            m->unk74 = a;
            m->unk76 = b;
            m->unk78 = c;
        }
        return 0;
    case 0x4C:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        o = func_80038150();
        m2 = func_8004AFF0_inline(o);
        a = func_8004AEDC_inline();
        if (m != NULL && m2 != NULL) {
            m->unk1C = m2;
            m->unk4 = a;
        }
        return 0;
    case 0x4D:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        if (m != NULL) {
            m->unk1C = NULL;
        }
        return 0;
    case 0x9F:
        po = func_80038150();
        m = func_8004AFF0_inline(po);
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk54 = (s8)a << 6;
            m->unk56 = (s8)b << 6;
            m->unk58 = (s8)c << 6;
            m->unk0 |= 2;
        }
        po->unk2F = b;
        return 0;
    case 0xC0:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 |= 1;
            m->unk50->unk0[b].unkC = 0;
            m->unk50->unk0[b].unk12 = 0x1000;
        }
        return 0;
    case 0xC1:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 |= 3;
            m->unk50->unk0[b].unkC = 0;
            m->unk50->unk0[b].unk12 = 0x1000;
        }
        return 0;
    case 0xC2:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 &= 0xFC;
        }
        return 0;
    case 0xCC: {
        ((Unk8E0 *)func_8001E350(po->unk3B))->unk0 |= func_8004AF58_inline();
        return 0;
    }
    case 0xCD: {
        ((Unk8E0 *)func_8001E350(po->unk3B))->unk0 &= ~func_8004AF58_inline();
        return 0;
    }
    case 0x4E:
        str = D_8007B70C->unk0 - 1;
        while (*str++ != 0) {
        }
        func_80052D0C(&str);
        D_8007B6F8 = 1;
        D_8007B70C->unk0 = str + 1;
        return 0;
    case 0x4F:
        D_8007B6F8 = 1;
        D_8007B70C->unk0++;
        return 6;
    case 0x33:
        actor->unk54 = func_8004AF58_inline();
        return 0;
    case 0x34:
        actor->unk56 = func_8004AF58_inline();
        return 0;
    case 0x35:
        actor->unk58 = func_8004AF58_inline();
        return 0;
    case 0x7A:
        actor->unk40.unk20 = func_8004AF58_inline();
        return 0;
    case 0x7B:
        actor->unk40.rotY = func_8004AF58_inline();
        return 0;
    case 0x52:
        actor->unk74 = func_8004AF58_inline();
        return 0;
    case 0x94:
        actor->unk76 = func_8004AF58_inline();
        actor->unk90 = func_8004AEDC_inline();
        actor->unk91 = func_8004AEDC_inline();
        return 0;
    case 0x3F:
        actor->unk62 = (func_8004AEDC_inline() << 3) & 0x38;
        actor->unk63 = func_8004AEDC_inline();
        return 0;
    case 0x3E:
        actor->unk69 = func_8004AEDC_inline();
        return 0;
    case 0x86:
        actor->unk7C.b[0] = func_8004AEDC_inline();
        actor->unk7C.b[1] = func_8004AEDC_inline();
        actor->unk7C.b[2] = func_8004AEDC_inline();
        actor->unk7C.b[3] = func_8004AEDC_inline();
        return 0;
    case 0x3D:
        actor->unk60 = func_8004AEDC_inline();
        actor->unk61 = func_8004AEDC_inline();
        return 0;
    case 0xBD:
        actor = func_80038150();
    case 0x40:
        func_80039C38_anim(actor, func_8004AF58_inline());
        if (actor->unk7 == D_8007AECC->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0xBE:
        actor = func_80038150();
    case 0x41:
        if (!(actor->unk62 & 0x80)) {
            return 0;
        }
        D_8007B70C->unk0 = D_8007AEA0;
        D_8007B6F8 = 1;
        return 1;
    case 0x42:
        func_800499D8((Unk8004B9B8 *)actor);
        return 0;
    case 0xB5:
        obj = func_80038150();
        if (obj != NULL) {
            if (obj != po) {
                actor->unkA0 = obj;
                actor->unk62 = 0x80;
            } else {
                actor->unkA0 = NULL;
                actor->unk62 = 0;
            }
        }
        return 0;
    case 0x43: {
        Unk8004AB64 *o;
        a = D_8007B70C->unk7 + 0x40;
        o = func_8004B040_inline(a);
        if (o != NULL) {
            func_80036978(o);
        }
        func_800367D0(func_8004AEDC_inline(), a, 0x14, 0)->unk5 = 1;
        return 0;
    }
    case 0x44:
        if (func_8004B040_inline(D_8007B70C->unk7 + 0x40) != NULL) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
            return 1;
        }
        return 0;
    case 0x45: {
        Unk8004AB64 *o = func_8004B040_inline(D_8007B70C->unk7 + 0x40);
        if (o != NULL) {
            func_80036978(o);
        }
        return 0;
    }
    case 0x38:
        switch (D_8007B70C->unk4) {
        case 0: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            func_800B1520(x, z, 0x36, func_8004AEDC_inline());
            break;
        }
        case 1: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            func_800B1520(x, z, 0x35, func_8004AEDC_inline());
            break;
        }
        case 3: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            func_800B1520(x, z, 0x2F, func_8004AEDC_inline());
            D_8007B70C->unkB = 0;
            break;
        }
        }
        return 0;
    case 0xE5: {
        s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
        func_800CA694(x, z, func_8004AEDC_inline());
        return 0;
    }
    case 0x4A:
        a = func_8004AEDC_inline();
        func_800BE99C(a, func_8004AF58_inline());
        return 0;
    case 0x1B:
        if (func_8004AEDC_inline()) {
            g_gameState->unk1C->unk56F |= 8;
        } else {
            g_gameState->unk1C->unk56F &= 0xF7;
        }
        return 0;
    case 0xE1:
        func_801F0FF4();
        D_8007B718 = 1;
        return 0;
    case 0xDE:
        a = func_80039C38_chr(func_8004AEDC_inline());
        if ((u32)a < 9) {
            u8 *src = func_8004F058(func_8004AF58_inline());
            u8 *dst = g_gameState->unk1C->characters[a].unk0;
            u8 *end = dst + 9;
            while (dst < end) {
                *dst++ = *src++;
            }
            *dst = 0xFF;
        } else {
            func_8004AF58_inline();
        }
        return 0;
    case 0xDB:
        a = func_80039C38_chr(func_8004AEDC_inline());
        if ((u32)a < 9) {
            g_gameState->unk1C->characters[a].unk34.w = (g_gameState->unk1C->characters[a].unk34.w & ~0x400) | ((func_8004AEDC_inline() & 1) << 10);
        }
        return 0;
    case 0x67:
        D_8007AECC->unk16E8 = func_8004AEDC_inline() << 4;
        D_8007AECC->unk16EA = func_8004AEDC_inline() << 4;
        return 0;
    case 0x6B:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_8001CCB4(a, b, func_8004AEDC_inline());
        return 0;
    case 0x58:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        func_800C0DB0(a, b, func_8004AF58_inline());
        return 0;
    case 0x6F:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004AEDC_inline();
        func_800C0DF4(a, b, c, func_8004AEDC_inline());
        return 0;
    case 0x70:
        a = func_8004AEDC_inline();
        func_800C0038(a, func_8004AEDC_inline());
        return 0;
    case 0x71:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C0E94(a, b, func_8004AEDC_inline());
        return 0;
    case 0x72:
        a = func_8004AF58_inline();
        func_800C1024(a);
        return 0;
    case 0x73:
        func_800C1100();
        return 0;
    case 0x74:
        func_800C104C();
        return 0;
    case 0x59:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        func_800C1368(a, b, c, func_8004AEDC_inline());
        return 0;
    case 0x5A:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        func_800C11E8(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5B:
        a = func_8004AEDC_inline();
        func_800C142C(a, func_8004AEDC_inline());
        return 0;
    case 0x5C:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        func_800C147C(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5D:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        func_800C15BC(a, b, c, func_8004AF58_inline());
        return 0;
    case 0x5E:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800C1614(a, b, func_8004AF58_inline());
        return 0;
    case 0x92:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AEDC_inline();
        f = func_8004AEDC_inline();
        {
            s32 g = func_8004AEDC_inline();
            e = func_8004AEDC_inline();
            func_800C02D0(a, b, c, f, g, e, func_8004AEDC_inline());
        }
        return 0;
    case 0xE6:
        a = func_8004AEDC_inline();
        func_800C14DC(a, func_8004AEDC_inline());
        return 0;
    case 0x5F:
        a = func_8004AEDC_inline();
        func_800C16B0(a, func_8004AEDC_inline());
        return 0;
    case 0x60:
        a = func_8004AEDC_inline();
        func_800C17AC(a, func_8004AEDC_inline());
        return 0;
    case 0x61:
        a = func_8004AEDC_inline();
        func_800C17F0(a, func_8004AF58_inline());
        return 0;
    case 0x63:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C1868(a, b, func_8004AEDC_inline());
        return 0;
    case 0x64:
        a = func_8004AEDC_inline();
        func_800C18A0(a, func_8004AEDC_inline());
        return 0;
    case 0x65:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_800C18D4(a, b, func_8004AEDC_inline());
        return 0;
    case 0xCA:
        a = func_8004AEDC_inline();
        func_800C196C(a, func_8004AEDC_inline());
        return 0;
    case 0xE7:
        a = func_8004AEDC_inline();
        func_800C16F4(a, func_8004AEDC_inline());
        return 0;
    case 0x9A:
        a = func_8004AF58_inline();
        func_800C778C(a, func_8004AEDC_inline());
        return 0;
    case 0xCB:
        a = func_8004AEDC_inline();
        func_800C47E4(a, func_8004AEDC_inline());
        return 0;
    case 0xA8:
        a = func_8004AEDC_inline();
        if (po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, a);
        }
        return 0;
    case 0x27:
        a = func_8004AEDC_inline();
        func_800C7464(a);
        return 0;
    case 0x1E:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C1A8C(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0xC3:
        a = func_8004AEDC_inline();
        func_800C1668(a, func_8004AEDC_inline());
        return 0;
    case 0xC9:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C119C(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0xED:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800C11C8(a, b, func_8004AF58_inline());
        return 0;
    case 0xE4:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800C152C(a, b, c, d, func_8004AEDC_inline());
        return 0;
    case 0x7E:
        a = func_8004AEDC_inline();
        func_800C0BCC(a);
        return 0;
    case 0xEA: {
        s16 x, z;
        func_800C0E6C(&x, &z);
        D_8007AE2C = x;
        D_8007AE30 = z;
        return 0;
    }
    case 0x8F:
        obj = func_80038150();
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        if (func_800B2CC0(obj->unk3B, a, b, func_8004AEDC_inline())) {
            D_8007B70C->unk0 = D_8007AEA0;
            D_8007B6F8 = 1;
            return 1;
        }
        return 0;
    case 0x88:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        c = func_8004AF58_inline();
        func_800ABA98(po->unk3B, a, b, c, func_8004AF58_inline());
        return 0;
    case 0x3A:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk14 |= 1 << a;
        }
        return 0;
    case 0x39:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        a = func_8004AEDC_inline();
        if (m != NULL) {
            m->unk14 &= ~(1 << a);
        }
        return 0;
    case 0x68:
        func_80050068(func_8004AEDC_inline());
        return 0;
    case 0xD7:
        func_80050080(func_8004AEDC_inline());
        return 0;
    case 0xEF:
        func_8004FF98(func_8004AEDC_inline());
        return 0;
    case 0x90:
        D_8007AECC->unk1700 = -1;
        return 0;
    case 0xEE:
        D_8007AECC->unk1700 = 0;
        return 0;
    case 0xB9:
        a = func_8004AEDC_inline();
        D_80073998[a] |= func_8004AF58_inline();
        return 0;
    case 0xBA:
        a = func_8004AEDC_inline();
        D_80073998[a] &= ~func_8004AF58_inline();
        return 0;
    case 0xC5:
        a = func_8004AF58_inline();
        ((s32 (*)(s32, s32, s32, s32, s32))g_gameState->unk1C->unk83C)(a, func_8004AF58_inline(), 0, 0, 0);
        return 0;
    case 0xC6:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        ((s32 (*)(s32, s32, s32, s32, s32))g_gameState->unk1C->unk83C)(a, b, c, 0, 0);
        return 0;
    case 0xC7:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        ((s32 (*)(s32, s32, s32, s32, s32))g_gameState->unk1C->unk83C)(a, b, c, func_8004AEDC_inline(), 0);
        return 0;
    case 0xC8:
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        c = func_8004B914_inline();
        d = func_8004AEDC_inline();
        ((s32 (*)(s32, s32, s32, s32, s32))g_gameState->unk1C->unk83C)(a, b, c, d, func_8004AEDC_inline());
        return 0;
    case 0x89: {
        u8 vol, pan;
        a = func_8004AF58_inline();
        b = func_8004AF58_inline();
        func_800B1A20(a, b, func_8004AF58_inline(), &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * func_8004AEDC_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0x8A: {
        u8 vol, pan;
        po = func_80038150();
        func_800B1A20(po->pos.vx, po->pos.vy, po->pos.vz, &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * func_8004AEDC_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0xB3:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AF58_inline();
        d = func_8004AF58_inline();
        func_800AFAC0(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0xDA:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        d = func_8004AF58_inline();
        func_800AFAC0(a, b, c, d, func_8004AF58_inline());
        return 0;
    case 0x75:
        a = func_8004AEDC_inline();
        func_80032120(a, func_8004AEDC_inline());
        func_8004A9E8();
        return 1;
    case 0xAC:
        a = func_8004AF58_inline();
        func_800B5004((a >> 14) & 3, a & 0x3FFF);
        return 1;
    case 0xAA:
        D_80073998[0] &= ~0x40000;
        return 0;
    case 0xAB:
        D_80073998[0] |= 0x40000;
        return 1;
    case 0x48:
        a = func_8004AF58_inline();
        b = func_8004AEDC_inline();
        if (a < 0x100) {
            func_80027D84(a, b);
        } else if (a < 0x200) {
            func_80028074(a - 0x100);
        } else {
            while (--b != -1) {
                func_8006238C(a - 0x200);
            }
        }
        return 0;
    case 0x49:
        a = func_8004AF58_inline();
        b = func_8004AEDC_inline();
        if (a < 0x100) {
            func_80027E5C(a, b);
        } else if (a < 0x200) {
            func_800280CC(a - 0x100);
        }
        return 0;
    case 0xCE:
        if ((g_gameState->unk1C->gil += func_8004B914_inline()) > 9999999) {
            g_gameState->unk1C->gil = 9999999;
        }
        return 0;
    case 0xCF:
        if ((g_gameState->unk1C->gil -= func_8004B914_inline()) > 9999999) {
            g_gameState->unk1C->gil = 0;
        }
        return 0;
    case 0xB4:
        a = func_8004AF58_inline();
        a |= (a >> 4) & 0xE0;
        for (i = 0; i < 9; i++) {
            func_80033C0C(i);
        }
        for (i = 0; i < 9; i++) {
            if (a & 1) {
                func_80033BD0(i);
            }
            a >>= 1;
        }
        return 0;
    case 0xFE:
        a = func_80039C38_chr(func_8004AEDC_inline());
        b = func_8004AEDC_inline();
        c = func_8004AEDC_inline();
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            d = func_8004AEDC_inline();
            if (d != 0xFF) {
                ch->unkA = d;
            }
            d = func_8004AEDC_inline();
            if (d != 0xFF) {
                ch->unk34.w = (ch->unk34.w & 0xFFFE0FFF) | ((d & 0x1F) << 12);
            }
            func_800333B0(a, b & 0xFF, c);
        } else {
            func_8004AEDC_inline();
            func_8004AEDC_inline();
        }
        return 0;
    case 0xDD:
        a = func_80039C38_chr(func_8004AEDC_inline());
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->unk1C->party[i];
            if (ch != NULL && (ch->unk34.w & 0xF) == a) {
                break;
            }
        }
        if (i < 4) {
            func_80033AF4(i, -1);
            func_80039C38_party();
        }
        return 0;
    case 0xB2:
        D_8007AE70.unk0 = func_8004AEDC_inline();
        a = 0;
        for (i = 8; i >= 0; i--) {
            a <<= 1;
            a |= (g_gameState->unk1C->characters[i].unk34.w >> 11) & 1;
        }
        b = func_8004AF58_inline();
        for (i = 0; i < 9; i++) {
            D_8007AE70.unk10[i] = b & 1;
            b >>= 1;
        }
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->unk1C->party[i];
            if (ch != NULL) {
                b = ch->unk34.w & 0xF;
                a &= ~(1 << b);
                D_8007AE70.unk4[i] = b;
            } else {
                D_8007AE70.unk4[i] = 0xFF;
            }
        }
        for (b = 0, i = 0; i < 9 && b < 8 && a != 0; i++, a >>= 1) {
            if (a & 1) {
                D_8007AE70.unk8[b++] = i;
            }
        }
        for (; b < 8; b++) {
            D_8007AE70.unk8[b] = 0xFF;
        }
        func_80032120(3, (s32)&D_8007AE70);
        return 1;
    case 0xE9:
        func_80039C38_party();
        return 0;
    case 0xF1:
        a = func_80039C38_chr(func_8004AEDC_inline());
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            a = func_8004AF58_inline();
            if (ch->maxHp < a) {
                a = ch->maxHp;
            }
            ch->hp = a;
        }
        return 0;
    case 0xF2:
        a = func_80039C38_chr(func_8004AEDC_inline());
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            a = func_8004AF58_inline();
            if (ch->maxMp < a) {
                a = ch->maxMp;
            }
            ch->mp = a;
        }
        return 0;
    case 0xF3:
        a = func_80039C38_chr(func_8004AEDC_inline());
        func_8003470C(a, func_8004AEDC_inline());
        return 0;
    case 0xF4:
        a = func_80039C38_chr(func_8004AEDC_inline());
        func_80034220(a, func_8004AEDC_inline());
        return 0;
    case 0xD9:
        a = func_80039C38_chr(func_8004AEDC_inline());
        func_80053960(&g_gameState->unk1C->characters[a], func_8004AEDC_inline() & 0xFF);
        return 0;
    case 0x62:
        a = func_80039C38_chr(func_8004AEDC_inline());
        if ((u32)a < 9) {
            g_gameState->unk1C->characters[a].unk34.w = (g_gameState->unk1C->characters[a].unk34.w & ~0x200) | ((func_8004AEDC_inline() & 1) << 9);
        }
        return 0;
    case 0xE0:
        if (g_gameState->unk1C->unk568 < 0x7FFF) {
            g_gameState->unk1C->unk568++;
        }
        return 0;
    case 0xB0: {
        u8 *src = func_8004F058(func_8004AF58_inline());
        u8 *dst = D_8007AE90;
        u8 *end = dst + 0xF;
        while (dst < end) {
            *dst++ = *src++;
        }
        *dst = 0xFF;
        g_gameState->unk1C->unk80C = D_8007AE90;
        return 0;
    }
    case 0xB1:
        g_gameState->unk1C->unk80C = D_8007AE8C;
        return 0;
    case 0x69:
        func_8002412C(func_8004AF58_inline());
        return 0;
    case 0x8D:
        func_800240B0(func_8004AEDC_inline());
        return 0;
    case 0x7D:
        func_800240F8(func_8004AEDC_inline());
        return 0;
    case 0xEC:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_8002F8BC(a & 1);
        func_8002F93C(((0x80 - func_8004AEDC_inline()) << 4) & 0xFFF0);
        c = func_8004AEDC_inline();
        d = func_8004AEDC_inline();
        func_8002F8E4(1);
        func_800320B8(((a >> 2) & 1) + 1, (a >> 1) & 1, b, c | (d << 8) | (func_8004AEDC_inline() << 16));
        return 0;
    case 0xA9:
        obj = func_80038150();
        if (obj != NULL) {
            sv.vx = obj->pos.vx;
            sv.vy = obj->pos.vy;
            sv.vz = obj->pos.vz;
            func_8004BA20(&sv, &D_8007AE2C, &D_8007AE30);
        }
        return 0;
    case 0x28:
        a = func_8004AEDC_inline();
        b = func_8004AF58_inline();
        func_800AD944(a, b, func_8004AEDC_inline());
        return 0;
    case 0xB7:
        if (D_8007B708 == 3) {
            VECTOR *v = func_800AE750();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->unk50 |= 0x40;
        }
        return 0;
    case 0xB8:
        if (D_8007B708 == 3) {
            VECTOR *v = func_800AE744();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->unk50 |= 0x80;
        }
        return 0;
    case 0xC4:
        a = func_8004AEDC_inline();
        func_800B73F8(a, func_8004AF58_inline());
        return 0;
    case 0xD1:
        func_80050A84();
        return 0;
    case 0xF6:
        func_80054FB8(func_8004AEDC_inline());
        return 0;
    case 0xF7:
        func_80054FE0(func_8004AEDC_inline());
        return 0;
    case 0xF8:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_80054E88(a, b, func_8004AEDC_inline());
        return 0;
    case 0xF9:
        a = func_8004AEDC_inline();
        b = func_8004AEDC_inline();
        func_80055060(a, b, func_8004AEDC_inline());
        return 0;
    case 0xFA:
        func_800550C0(func_8004AF58_inline());
        return 0;
    case 0xFB:
        func_800550D0(func_8004AEDC_inline());
        return 0;
    case 0xFC:
        a = func_8004AEDC_inline();
        func_800550F0(a, func_8004AEDC_inline());
        return 0;
    case 0xFD:
        a = func_8004AEDC_inline();
        ((s32 (*)(s32, s32))g_gameState->unk1C->unk8BC)(a & 0xFF, func_8004AF58_inline() & 0xFFFF);
        return 0;
    case 0xFF:
        /* padding between the two jump tables in the original rodata */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        op = D_8007B6F4 | 0xFF00;
        D_8007B6F4 = *D_8007B70C->unk0++;
        switch (op) {
        case 0xFF00:
            a = func_8004AEDC_inline();
            func_800C7F34(a, func_8004AEDC_inline());
            return 0;
        case 0xFF01:
            a = func_8004AEDC_inline();
            func_800C7F7C(a, func_8004AEDC_inline());
            return 0;
        case 0xFF02:
            a = func_8004AEDC_inline();
            func_800C7FBC(a, func_8004AEDC_inline());
            return 0;
        case 0xFF03:
            a = func_8004AEDC_inline();
            func_800C8010(a, func_8004AEDC_inline());
            return 0;
        case 0xFF04:
            a = func_8004AEDC_inline();
            func_800C8044(a, func_8004AEDC_inline());
            return 0;
        case 0xFF05:
            a = func_8004AEDC_inline();
            func_800C8068(a, func_8004AF58_inline());
            return 0;
        case 0xFF06:
            a = func_8004AEDC_inline();
            func_800C808C(a, func_8004AEDC_inline());
            return 0;
        case 0xFF07:
            a = func_8004AEDC_inline();
            b = func_8004AF58_inline();
            func_800C80B0(a, b, func_8004AF58_inline());
            return 0;
        case 0xFF08:
            a = func_8004AEDC_inline();
            func_800C80E4(a, func_8004AEDC_inline());
            return 0;
        case 0xFF09:
            a = func_8004AEDC_inline();
            func_800C7AF8(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0A:
            a = func_8004AEDC_inline();
            func_800C6408(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0B:
            a = func_8004AEDC_inline();
            func_800C7B68(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0C:
            a = func_8004AEDC_inline();
            func_800C7CDC(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0D:
            a = func_8004AEDC_inline();
            func_800C7BC4(a, func_8004AF58_inline());
            return 0;
        case 0xFF0E:
            a = func_8004AEDC_inline();
            func_800C7C04(a, func_8004AEDC_inline());
            return 0;
        case 0xFF0F:
            a = func_8004AEDC_inline();
            b = func_8004AEDC_inline();
            func_800C7C70(a, b, func_8004AEDC_inline());
            return 0;
        case 0xFF10:
            a = func_8004AEDC_inline();
            b = func_8004AEDC_inline();
            func_800C7D30(a, b, func_8004AEDC_inline());
            return 0;
        case 0xFF11:
            a = func_8004AEDC_inline();
            func_800C7DF0(a, func_8004AEDC_inline());
            return 0;
        }
        return 0;
    default:
        switch (D_8007B708) {
        case 1:
            return func_800500A8(po, op);
        case 2:
            return func_80050410(po, op);
        case 3:
            return func_80050744(po, op);
        }
        return 1;
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800475D8);

s32 func_80047AB8(Unk8004AB64 *p, s32 b) {
    s32 *sp;
    s32 ok;
    s32 x;
    u32 hi;
    s32 w;

    if (!(p->unk50 & 0x400)) {
        ok = 0;
        if (!(p->unk62 & 0x80) && p->unk6E == 0xA7) {
            ok = 0 < p->unk4;
        }
        if (ok) {
            p->unk70 = b >> 4;
            sp = &((s32 *)p + p->unk9)[p->unkA];
            sp[0] = p->unk0 - D_8007AECC->unk16F0;
            w = p->unkD | (p->unk4 << 8);
            hi = D_8007B70C->unk7 << 24;
            x = hi | 0xFF0000;
            sp[1] = w | x;
            p->unk0 = (u8 *)&p->unk6E;
            p->unk4 = 0;
            p->unkD = 0;
            p->unkA += 2;
        }
    }
    return 1;
}

/* point 500 units ahead of o at angle a, at head height */
static inline void func_80047E1C_ahead(Unk8004AB64 *o, s32 a, VECTOR *out) {
    SVECTOR v;
    MATRIX m;

    m.m[0][2] = func_8004BB80(a);
    m.m[1][1] = 0x1000;
    m.m[2][2] = func_8004BB7C(a);
    m.m[0][0] = m.m[0][1] = m.m[1][0] = m.m[1][2] = m.m[2][0] = m.m[2][1] = 0;
    m.t[0] = o->pos.vx;
    m.t[1] = o->pos.vy;
    m.t[2] = o->pos.vz;
    SetRotMatrix(&m);
    SetTransMatrix(&m);
    v.vx = 0;
    v.vy = (o->unk32 * (s8)o->unk2F) >> 6;
    v.vz = -500;
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(out);
}

void func_80047B90(Unk8004AB64 *o, Unk80047B90 *m) {
    VECTOR out;
    s32 d;

    func_80047E1C_ahead(o, o->pos.rotY, &out);
    d = func_80052EC0(out.vx - o->unk84, out.vy - o->unk86, out.vz - o->unk88);
    if (d < 100) {
        d = 100;
        o->unk50 &= 0xF8FF;
    }
    o->unk84 += (out.vx - o->unk84) * 100 / d;
    o->unk86 += (out.vy - o->unk86) * 100 / d;
    o->unk88 += (out.vz - o->unk88) * 100 / d;
    m->unk0 |= 1;
    m->unk48 = o->unk84;
    m->unk4A = o->unk86;
    m->unk4C = o->unk88;
}

/* move the look-at point towards t */
static inline void func_80047E1C_approach(Unk8004AB64 *o, Unk80047B90 *mdl, VECTOR *t) {
    s32 d;

    d = func_80052EC0(t->vx - o->unk84, t->vy - o->unk86, t->vz - o->unk88);
    if (d < 100) {
        d = 100;
    }
    o->unk84 += (t->vx - o->unk84) * 100 / d;
    o->unk86 += (t->vy - o->unk86) * 100 / d;
    o->unk88 += (t->vz - o->unk88) * 100 / d;
    mdl->unk0 |= 1;
    mdl->unk48 = o->unk84;
    mdl->unk4A = o->unk86;
    mdl->unk4C = o->unk88;
}

/* is the look-at point still in front of o (within ~80 degrees)? */
static inline s32 func_80047E1C_infront(Unk8004AB64 *o) {
    s32 ok = 1;
    s32 d;

    if (!(o->unk50 & 0x400)) {
        d = (func_80052F24(o->unk84 - o->pos.vx, o->unk88 - o->pos.vz) - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->unk50 &= ~0x100;
        }
    }
    return ok;
}

static inline s32 func_80047E1C_infront2(Unk8004AB64 *o, s32 *pa) {
    s32 ok = 1;
    s32 a;
    s32 d;

    if (!(o->unk50 & 0x400)) {
        a = func_80052F24(o->unk84 - o->pos.vx, o->unk88 - o->pos.vz);
        d = (a - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->unk50 &= ~0x100;
        }
        *pa = a;
    }
    return ok;
}

void func_80047E1C(Unk8004AB64 *o) {
    VECTOR t;
    s32 ang;
    s32 save;
    s32 x, y, z;
    Unk80047B90 *mdl;
    Unk8004AB64 *tgt;
    Unk8004AB64 *best;
    Unk8004AB64 *q;
    Unk16D8 *n;
    u8 mask;
    s32 bestd;
    s32 d;
    s32 a;
    s32 ok;

    save = 0;
    mdl = (Unk80047B90 *)((Unk8E0 *)func_8001E350(o->unk3B))->unk8;
    tgt = NULL;
    if ((o->unk50 & 4) && D_8007AE68 != NULL && D_8007AE68 != o &&
        func_80052E48(o->pos.vx - D_8007AE68->pos.vx, o->pos.vz - D_8007AE68->pos.vz, 0) >= 1000) {
        tgt = D_8007AE68;
        o->unk50 |= 0x200;
    }
    if (tgt != NULL || ((o->unk50 & 1) && !(o->unk62 & 2))) {
        if (o->unkF & 0x80) {
            save = o->pos.rotY;
            o->pos.rotY = o->unk80;
        }
        if (tgt == NULL) {
            best = NULL;
            x = o->pos.vx;
            y = o->pos.vy;
            z = o->pos.vz;
            mask = o->unk6B;
            bestd = 0;
            for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
                q = n->unk4;
                if (q != o && q->unk5 == 4 && (u32)(q->pos.vy - y + 399) < 799 && (q->unk50 & 2) &&
                    (mask & q->unk6A)) {
                    d = func_80052DEC(q->pos.vx - x, q->pos.vz - z, 0);
                    if (d >= 1000) {
                        a = func_800531D0(o, q, o->pos.rotY);
                        d -= 4000000;
                        if (a < 0) {
                            a = -a;
                        }
                        a -= 640;
                        if (a < 0) {
                            d += a;
                            if (d < bestd) {
                                bestd = d;
                                best = q;
                            }
                        }
                    }
                }
            }
            tgt = best;
        }
        if (tgt != NULL) {
            if (!(o->unk50 & 0x100)) {
                if (!(o->unkF & 0x80)) {
                    o->unk50 |= 0x100;
                    func_80047E1C_ahead(o, o->pos.rotY, &t);
                    o->unk84 = t.vx;
                    o->unk86 = t.vy;
                    o->unk88 = t.vz;
                }
            }
            if ((o->unk50 & 0x100) && func_80047E1C_infront(o)) {
                {
                    t.vx = tgt->pos.vx;
                    t.vy = tgt->pos.vy;
                    t.vz = tgt->pos.vz;
                    ang = func_80052F24(t.vx - o->pos.vx, t.vz - o->pos.vz);
                    d = (ang - o->pos.rotY) & 0xFFF;
                    if (d > 0x800) {
                        d -= 0x1000;
                    }
                    if ((u32)(d + 0x27F) < 0x4FF) {
                        t.vy = (((o->pos.vy << 6) + o->unk32 * (s8)o->unk2F) * 3 + (t.vy << 6) +
                                tgt->unk32 * (s8)tgt->unk2F) >> 8;
                        func_80047E1C_approach(o, mdl, &t);
                    } else if (func_80047AB8(o, ang)) {
                        a = d > 0x280 ? 0x280 : -0x280;
                        func_80047E1C_ahead(o, a + o->pos.rotY, &t);
                        func_80047E1C_approach(o, mdl, &t);
                    }
                }
            }
        } else if ((o->unk50 & 0x100) && func_80047E1C_infront2(o, &ang) && func_80047AB8(o, ang)) {
            {
                func_80047B90(o, mdl);
            }
        }
        if (o->unkF & 0x80) {
            o->pos.rotY = save;
        }
    } else if ((o->unk50 & 0x100) && func_80047E1C_infront(o)) {
        func_80047B90(o, mdl);
    }
    if (!(o->unk50 & 0x100)) {
        mdl->unk0 &= ~1;
    }
}

void func_8004883C(void) {
    Unk8007AECC *e = D_8007AECC;
    Unk8004AB64 *p;
    Unk8004AB64 *o;
    Unk8007AECC *e2;

    if (e == NULL || e->unk16EC == 0) {
        return;
    }
    e->unk1700 -= e->unk1700 > 0;
    if (e->unk1700 != 0) {
        return;
    }
    o = func_8004B040_inline(e->playerUid);
    if (o == NULL || o->unk5 != 4) {
        return;
    }
    p = o;
    if (p->unk3A != p->unk38 - 1) {
        return;
    }
    e2 = D_8007AECC;
    if (e2->unk1700 >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        e2->unk1700 = (*g_randPtr++ + 0xC8) * 2;
    }
    if (p->unk74 != 0 && !(p->unk62 & 0x84)) {
        func_8004883C_idle(p);
        if (p->unk7 == D_8007AECC->playerUid) {
            D_8007B710++;
        }
        p->unk62 |= 2;
    }
}

/* Nearest kind-3 object whose script has entry `key` and that func_80053124
 * accepts for p's position. */
static inline Unk8004AB64 *func_80048A5C_find(Unk8004AB64 *p, s32 key) {
    Unk8004AB64 *found = NULL;
    s32 x = p->pos.vx;
    Unk16D8 *n = D_8007AECC->unk16D8;
    s32 z = p->pos.vz;
    Unk8004AB64 *o;

    for (; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->unk5 == 3 && func_800384D0_find(o->unk6, key) != NULL && func_80053124(o, x, z)) {
            found = o;
            break;
        }
    }
    return found;
}

void func_80048A5C(Unk8004AB64 *p) {
    s32 btn;
    s32 t;
    Unk8004AB64 *o;

    t = func_8004E8EC();
    btn = t & (D_8007B708 == 1 ? 0xA0000 : 0x20000);
    if (btn && (o = func_80038618(p, 4, NULL)) != NULL) {
        D_8007AE20 = func_800531D0(p, o, p->pos.rotY);
        if ((u32)(D_8007AE20 + 0x3FF) < 0x7FF) {
            p->unk93 = o->unk7;
            if (btn == 0x80000 && func_800384D0(o, 1, 8, 0)) {
                return;
            }
            if (func_800384D0(o, 1, 3, 0)) {
                return;
            }
        }
        o = NULL;
    } else {
        o = func_80038618(p, 2, NULL);
    }
    if (o != NULL) {
        func_800384D0(o, 1, 2, 0);
    }
    if (btn) {
        o = func_80048A5C_find(p, 3);
        if (o != NULL) {
            if (btn == 0x80000 && func_800384D0(o, 1, 8, 0)) {
                return;
            }
            func_800384D0(o, 1, 3, 0);
            o = NULL;
        } else {
            o = func_80048A5C_find(p, 2);
        }
    } else {
        o = func_80048A5C_find(p, 2);
    }
    if (o != NULL) {
        func_800384D0(o, 1, 2, 0);
    }
}

void func_80048E80(void) {
    s32 i;
    s32 m;
    Unk8007AED0 *o;

    if (g_gameState->unk1C->unk8E4.ovl65->unkD80 != 0) {
        i = 3;
        D_8007AE10[i] = func_800BEC64(1);
        m = (u16)D_8007AE10[i];
        for (i = 4; i < 8; i++) {
            o = D_8007AED0[i];
            if (o != NULL) {
                switch (o->unkE) {
                case 1:
                    if (!(m & 0x10)) {
                        o->unkE = 3;
                    }
                    break;
                case 3:
                    if (m & 0x10) {
                        o->unkE = 1;
                    }
                    break;
                }
            }
            m >>= 1;
        }
    }
}

/* turn towards unk52 by unk82 per frame (unk6C 0x7FFE / 0x7FFF: direction) */
static inline void func_80048F54_turn(Unk8004AB64 *p) {
    s32 a;

    if ((s16)p->unk6C == 0x7FFE) {
        a = p->pos.rotY + (s16)p->unk82;
        if (p->unk52 < a) {
            p->pos.rotY = a;
        } else {
            p->pos.rotY = p->unk52;
            p->unkF &= 0x7F;
        }
    } else if ((s16)p->unk6C == 0x7FFF) {
        a = p->pos.rotY + (s16)p->unk82;
        if (a >= p->unk52) {
            p->pos.rotY = p->unk52;
            p->unkF &= 0x7F;
        } else {
            p->pos.rotY = a;
        }
    }
}

/* remember the position at the start of the frame */
static inline void func_80048F54_save(Unk8004AB64 *p) {
    p->unk40.vx = p->pos.vx;
    p->unk40.vy = p->pos.vy;
    p->unk40.vz = p->pos.vz;
}

s32 func_80048F54(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    Unk8004AB64 *p;
    Unk8004AB64 *t;
    Unk8004AB64 *t1;
    Unk8004AB64 *x;
    Unk16D8 *y;
    Unk8004AB64 *z;
    Unk800C748C *q;
    s32 world;
    s32 r;
    s32 dist;
    u8 b;
    s32 h;
    s32 moved;
    s32 dx, dy, dz;
    s32 f;
    u32 anim;
    Window *w;
    Unk16D8 *l;
    Unk16D8 *m;
    Unk8007AECC *s;

    D_8007AEAC = 0;
    world = D_8007B708 == 2;
    for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->unk5 == 4) {
            z = o;
            if (o->unk7 == D_8007AECC->playerUid && o->unkE == 1) {
                if (D_8007AECC->unk16EC != 0) {
                    if (D_8007B708 == 1) {
                        o->unk40.vx = o->pos.vx;
                        o->unk40.vy = o->pos.vy;
                        o->unk40.vz = o->pos.vz;
                        func_800475D8(o);
                    } else if (D_8007B708 == 3) {
                        D_8007AEAC = D_800CCE54;
                        func_80048A5C(o);
                        t1 = func_80038618(o, 0, &dist);
                        if (t1 != NULL && dist <= 0 &&
                            (u32)(func_800531D0(o, t1, func_80052F24(o->pos.vx - o->unk40.vx, o->pos.vz - o->unk40.vz)) + 0x400) <= 0x800) {
                            o->pos.vx = o->unk40.vx;
                            o->pos.vy = o->unk40.vy;
                            o->pos.vz = o->unk40.vz;
                        }
                    }
                    if (D_8007AEAC != 0) {
                        s = D_8007AECC;
                        if (s->unk1700 >= 0) {
                            s->unk1700 = (func_80038D3C_rand() + 200) * 4;
                        }
                    }
                } else {
                    func_80048F54_save(o);
                }
            } else {
                func_80048F54_save(z);
            }
            if (o->unkE == 1 && !world) {
                func_80049C14(o);
            }
        }
    }
    if (world) {
        func_80048E80();
    }
    l = (Unk16D8 *)D_8007B6FC;
    D_8007AEA4 = 0;
    if (l == NULL) {
        l = D_8007AECC->unk16D8;
    }
    r = func_80052870(l);
    if (r == 6) {
        r = 0;
    } else {
        D_8007B6FC = 0;
    }
    D_8007AEBC = NULL;
    D_8007AEB8 = NULL;
    if (D_8007B708 == 1) {
        func_800C1B5C(&b);
        D_8007AEC8 = b != D_8007AEC0;
        D_8007AEC0 = b;
    }
    if (world) {
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unkB == 2 && func_800384D0(o, 3, 5, 0)) {
                o->unkB = 1;
            }
        }
    } else {
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unk5 == 4) {
                p = o;
                if (D_8007B708 == 1) {
                    f = func_8002170C(p);
                    if ((p->unk7 != D_8007AECC->playerUid || D_8007AEB0 >= 0) && (f & 1) &&
                        (t = func_80038618(p, 0, NULL)) != NULL) {
                        func_800C7720(p->unk3B);
                        if (t->unkF & 0x10) {
                            D_8007AEB0 = 0;
                        }
                        if (o->unk7 == D_8007AECC->playerUid) {
                            D_8007AEA8 = 2;
                        }
                    }
                    if (o->unk7 == D_8007AECC->playerUid) {
                        func_8004F74C(p);
                        if (D_8007AECC->unk16EC != 0) {
                            func_80048A5C(p);
                        }
                    }
                } else if (D_8007B708 == 3) {
                    if (p->unk50 & 0x40) {
                        VECTOR *v = func_800AE750();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEB8 = p;
                        v->vz = p->pos.vz;
                    } else if (p->unk50 & 0x80) {
                        VECTOR *v = func_800AE744();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEBC = p;
                        v->vz = p->pos.vz;
                    }
                }
                if (p->unk3E != 0xFF) {
                    w = func_8004ED94(p->unk3E);
                    if (w != NULL) {
                        func_80039444(p, w);
                    } else {
                        p->unk3E = 0xFF;
                        if (D_8007AE68 == p) {
                            D_8007AE6C = 30;
                        }
                    }
                }
                moved = 0;
                dx = p->pos.vx - p->unk40.vx;
                dy = p->pos.vy - p->unk40.vy;
                dz = p->pos.vz - p->unk40.vz;
                if (dx != 0 || dz != 0) {
                    moved = 1;
                }
                if (D_8007B708 == 1) {
                    if (p->unkF & 0x80) {
                        func_80048F54_turn(p);
                    }
                    if (p->unk34 != 0xFFFF) {
                        func_80047E1C(p);
                    }
                }
                if (D_8007B708 == 3 && (p->unkF & 0x80)) {
                    func_80048F54_turn(p);
                }
                if (moved) {
                    if ((p->unk62 & 0x86) == 0 || (p->unk62 & 0x86) == 0x82) {
                        p->unk62 &= 0x7D;
                        anim = p->unk5C >= p->unk5E ? (u16)p->unk58 : (u16)p->unk56;
                        goto set_anim;
                    }
                } else if (!(p->unk62 & 0x84) && (D_8007AEA8 == 0 || o->unk7 != D_8007AECC->playerUid)) {
                    anim = (u16)p->unk54;
                set_anim:
                    if (anim != p->unk36) {
                        p->unk36 = anim;
                        p->unk3A = 0;
                        p->unk64 = 0;
                        p->unk38 = func_800216D8(p->unk34, anim);
                        p->unk5D = p->unk69;
                    }
                }
                if (moved && o->unk7 == D_8007AECC->playerUid && D_8007AEAC != 0) {
                    D_8007AE4C += func_80052EC0(dx, dy, dz);
                    if (func_80038D3C(p)) {
                        D_8007AE58 = 1;
                    }
                }
            }
        }
    }
    if (D_8007AEA8 > 0) {
        D_8007AEA8--;
    }
    if (D_8007AE6C > 0 && --D_8007AE6C == 0) {
        for (y = D_8007AECC->unk16D8; y != NULL; y = y->unk0) {
            x = y->unk4;
            if (x->unk5 == 4 && (x->unk50 & 0x200)) {
                x->unk50 |= 0x400;
            }
        }
        D_8007AE68 = NULL;
    }
    if (D_8007AE58 != 0 && D_8007AEA4 == 0) {
        D_8007AE58 = 0;
        r = 3;
    }
    if (r == 3 || r == 7) {
        if (D_8007B708 == 1) {
            for (m = D_8007AECC->unk16D8; m != NULL; m = m->unk0) {
                p = m->unk4;
                if (p->unk5 == 4 && p->unk34 != 0xFFFF) {
                    q = func_800C748C(p->unk3B);
                    func_800C75B4(p->unk3B, &h);
                    p->unk26 = h / 4;
                    if (q != NULL) {
                        p->pos.vx = p->unk40.vx;
                        p->pos.vy = p->unk40.vy;
                        p->pos.vz = p->unk40.vz;
                        p->unk2E = q->unk8;
                        p->unk28 = q->unkA;
                        p->unk2A = q->unk0;
                    }
                }
            }
            func_800B3444(0, 0, 0);
        }
        if (r == 7) {
            D_80079630 = D_80077E60;
        }
    }
    return r;
}

void func_800499D8(Unk8004B9B8 *p) {
    s32 v;
    u32 *q;

    if (!(p->unk62 & 8)) {
        v = p->unk54;
        if (v != p->unk36) {
            p->unk36 = v;
            p->unk3A = 0;
            p->unk64 = 0;
            p->unk38 = func_800216D8(p->unk34, v);
            p->unk5D = p->unk69;
        }
        p->unk3A = 0;
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        p->unk5D = p->unk7C[*g_randPtr++ & 3];
        p->unk62 &= 0x39;
        if (p->unkF & 0x80) {
            func_8004B9B8_inline(p);
        }
        if (p->unk50 & 8) {
            p->unk50 &= ~8;
            q = (u32 *)func_8001E350(p->unk3B);
            *q &= ~0x10;
            p->unk60 = 0;
            p->unk61 = 0xFF;
        }
    } else {
        p->unk62 = (p->unk62 | 4) & 0x7D;
        p->unk3A = p->unk39;
    }
}

static inline void func_80049B48_bounce(Unk8004AB64 *o, u8 f) {
    u8 t;
    u8 pos;

    if (f & 0x20) {
        pos = o->unk3A;
        if (!(f & 0x40)) {
            o->unk3A = pos - 2;
        } else {
            o->unk3A = pos + 2;
        }
        o->unk62 ^= 0x40;
        t = o->unk60;
        o->unk60 = o->unk61;
        o->unk61 = t;
    } else if (f & 0x10) {
        o->unk3A = o->unk60;
    }
}

void func_80049B48(Unk8004AB64 *o) {
    u8 f = o->unk62;

    if (f & 0x30) {
        if (o->unk63 != 0) {
            o->unk63--;
            if (o->unk63 != 0) {
                func_80049B48_bounce(o, f);
            } else {
                func_800499D8(o);
            }
        } else {
            func_80049B48_bounce(o, f);
        }
    } else {
        func_800499D8(o);
    }
}

void func_80049C14(Unk8004AB64 *p) {
    Unk8004AB64 *q;
    u8 f;
    s32 last;
    s32 t;
    s32 lo;
    u8 old;
    s32 t2, lo2, t3, lo3;
    u8 old2, old3;

    f = p->unk62;
    if (f & 4) {
        return;
    }
    if (f & 0x80) {
        if (p->unkA0 != NULL) {
            q = p->unkA0;
            if (q->unk36 == (u16)q->unk54) {
                p->unk36 = p->unk54;
            } else if (q->unk36 == (u16)q->unk56) {
                p->unk36 = p->unk56;
            } else if (q->unk36 == (u16)q->unk58) {
                p->unk36 = p->unk58;
            }
            p->unk3A = q->unk3A;
        } else if (!(f & 0x40)) {
            last = p->unk61;
            if (p->unk38 - 1 < last) {
                last = p->unk38 - 1;
            }
            t = p->unk3A * 16 + (s8)p->unk64 + p->unk5D;
            lo = t & 0xF;
            p->unk64 = lo;
            if (p->unk62 & 0x40) {
                p->unk64 = lo - 0xF;
            }
            old = p->unk3A;
            t >>= 4;
            p->unk3A = t;
            p->unk39 = old;
            if (last < t) {
                func_80049B48(p);
            }
            if (p->unkF & 0x80) {
                p->pos.rotY = (p->pos.rotY + p->unk6C) & 0xFFF;
                p->unk80 = (p->unk80 + p->unk82) & 0xFFF;
            }
        } else {
            t2 = p->unk3A * 16 + (s8)p->unk64 - p->unk5D;
            t2 += 0xF;
            lo2 = t2 & 0xF;
            p->unk64 = lo2;
            if (p->unk62 & 0x40) {
                p->unk64 = lo2 - 0xF;
            }
            old2 = p->unk3A;
            t2 >>= 4;
            p->unk3A = t2;
            p->unk39 = old2;
            if (t2 < p->unk61) {
                func_80049B48(p);
            }
        }
    } else {
        lo3 = p->unk3A * 16 + (s8)p->unk64;
        if (f & 0x40) {
            t3 = lo3 - p->unk5D;
        } else {
            t3 = lo3 + p->unk5D;
        }
        if (p->unk62 & 0x40) {
            t3 += 0xF;
        }
        lo3 = t3 & 0xF;
        p->unk64 = lo3;
        if (p->unk62 & 0x40) {
            p->unk64 = lo3 - 0xF;
        }
        old3 = p->unk3A;
        t3 >>= 4;
        p->unk3A = t3;
        p->unk39 = old3;
        if (t3 >= p->unk38) {
            p->unk3A = 0;
            if (p->unk36 == (u16)p->unk54) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                p->unk5D = p->unk7C.b[*g_randPtr++ & 3];
            }
        }
    }
}

u8 *func_80049ED8(u8 *p) {
    s32 mask = 0;
    s32 i = 0;
    s32 c;
    s32 n;
    u8 *dst;
    ScriptEntry *e;
    u16 *sz;
    s32 bit;
    u8 *src;

    for (; i < 4; i++) {
        c = func_8004E7A4(i);
        if (c >= 0) {
            mask |= 1 << c;
        }
    }
    n = p[3];
    D_8007ADF8 = p + 0x80;
    D_8007ADFC = n;
    if (D_8007ADFC >= 10) {
        dst = D_8007ADF8 + ((ScriptEntry *)D_8007ADF8)[D_8007ADFC - 9].unk0;
    } else {
        dst = D_8007ADF8 + D_8007ADFC * 8;
    }
    bit = 1;
    for (e = &((ScriptEntry *)D_8007ADF8)[D_8007ADFC - 9]; e < &((ScriptEntry *)D_8007ADF8)[D_8007ADFC]; e++) {
        if (!(e->unk5 & 1)) {
            mask |= bit;
        }
        if (mask & bit) {
            if (e->unk0 != 0) {
                src = D_8007ADF8 + e->unk0;
                if (src != dst && e->unk2 != 0) {
                    memmove(dst, src, e->unk2);
                    e->unk0 = dst - D_8007ADF8;
                }
                dst += e->unk2 & ~3;
            }
        }
        bit <<= 1;
    }
    D_8007AE00 = mask;
    return dst;
}

void func_8004A06C(u8 *data) {
    Unk4 t;
    s32 k;
    u8 mode;
    Unk8007AECCNode *n;
    Unk16D8 *m;
    Unk16D8 *q;
    Unk16D8 *l;
    s32 i;
    s32 r;
    s32 d;
    Unk8004AB64 *o;
    Unk8007AECC *p;
    GameState *gs;

    t = D_80011A44;
    func_80019774(&t);
    k = data[3];
    D_8007ADF8 = data + 0x80;
    gs = g_gameState;
    D_8007AE8C = data + 0x2C;
    gs->unk1C->unk810 = data + 4;
    D_8007ADFC = k;
    gs->unk1C->unk80C = data + 0x2C;
    mode = g_gameState->mode;
    switch (mode) {
    case 1:
        D_8007B708 = mode;
        break;
    case 2:
        D_8007B708 = mode;
        break;
    case 3:
        D_8007B708 = mode;
        break;
    case 8:
        D_8007B708 = 4;
        break;
    }
    if ((D_80077E60.unk16EE == 1 || D_80077E60.unk16EE == 3) && D_8007B708 == 2) {
        D_80079630 = D_80077E60;
    }
    D_8007AECC = &D_80077E60;
    for (n = D_8007AECC->unk1450; n < &D_8007AECC->unk1450[31]; n++) {
        n->unk8 = n + 1;
    }
    n->unk8 = NULL;
    D_8007AECC->unk15D4 = D_8007AECC->unk1450;
    D_8007AECC->unk15D0 = NULL;
    for (m = D_8007AECC->unk15D8; m < &D_8007AECC->unk15D8[31]; m++) {
        m->unk0 = m + 1;
    }
    m->unk0 = NULL;
    D_8007AECC->unk16E0 = D_8007AECC->unk15D8;
    D_8007AECC->unk16DC = NULL;
    D_8007AECC->unk16D8 = NULL;
    func_80050044();
    func_80050080(0);
    for (i = 0; i < 0x50; i++) {
        D_8007AECC->unk0[i] = 0;
    }
    D_8007AECC->unk16EC = 0;
    D_8007AECC->playerUid = 0;
    D_8007AECC->unk1700 = 0;
    func_8004FF98(0);
    D_8007B6FC = 0;
    D_8007AECC->unk16F8 = 0;
    D_8007AE68 = NULL;
    D_8007B710 = 10;
    D_8007BBC8 = 10;
    D_8007BBD4 = 0;
    D_8007B718 = 0;
    D_8007AEC8 = 0;
    D_8007AEC0 = 0;
    D_8007AE4C = -0x5A0;
    p = D_8007AECC;
    p->unk16EF = 0;
    D_8007AE50 = 0;
    D_8007AE58 = 0;
    p->unk16E8 = 0;
    p->unk16EA = 0;
    D_8007BD2C = 0;
    func_8004B3A8_inline(0, 0);
    D_8007AECC->unk16D8->unk4->unkE = 2;
    func_80039C38_party();
    for (i = 7; i >= 0; i--) {
        D_8007AED0[i] = NULL;
    }
    D_8007AE04 = 0;
    if (D_8007B708 == 2) {
        for (i = 0; i < 4; i++) {
            r = func_8004E7A4(i);
            if (r >= 0) {
                func_80039C38_newactor(r + D_8007ADFC - 9, 0);
            }
        }
        D_8007AECC->unk16FC = (s32)D_8007AECC->unk16DC;
    } else {
        g_gameState->unk1C->unk570 = 0;
    }
    D_8007AE04 = 4;
    if ((D_8007B708 == 1 && D_80079630.unk16EE == D_8007B708 && D_80079630.unk16F4 == (s16)g_gameState->unk1C->fieldId) ||
        (D_8007B708 == 3 && D_80079630.unk16EE == D_8007B708 && D_80079630.unk16F4 == g_gameState->unk1C->unk7DC) ||
        (g_gameState->unkA == 9 && D_8007AD1E != 0)) {
        d = data - D_80079630.unk16F0;
        D_80077E60 = D_80079630;
        for (q = D_8007AECC->unk16D8; q != NULL; q = q->unk0) {
            o = q->unk4;
            if (o->unk0 != NULL) {
                o->unk0 += d;
            }
            if (o->unk5 == 4 && D_8007B708 == 1) {
                func_800AA3F4(o->unk6);
            }
        }
        func_800384D0(func_8004B040_inline(0), 0, 10, 0);
        for (l = D_8007AECC->unk16D8; l != NULL; l = l->unk0) {
            o = l->unk4;
            if (o->unk7 != 0) {
                o->unk12 = o->unkE;
                o->unkE = 3;
            }
        }
    } else {
        func_80048F54();
    }
    D_8007AECC->unk16F0 = data;
    D_8007AECC->unk16EE = D_8007B708;
    D_8007AECC->unk16F4 = D_8007B708 == 1   ? g_gameState->unk1C->fieldId
                          : D_8007B708 == 3 ? g_gameState->unk1C->unk7DC
                                            : 0;
}

void func_8004A8A4(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    s16 *q;

    if ((D_8007B708 == 1 || D_8007B708 == 3 || g_gameState->unkA == 9) && D_8007AD1E != 0) {
        for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unk5 == 4 && o->unk34 != 0xFFFF) {
                o->unk3B = func_80021698(o);
                if (D_8007B708 == 1) {
                    q = (s16 *)func_800C748C(o->unk3B);
                    func_800C7570(o->unk3B, o->unk26 * 4);
                    if (q != NULL) {
                        q[4] = q[2] = o->unk2E;
                        q[5] = q[3] = o->unk28;
                        q[0] = o->unk2A;
                    }
                }
            }
        }
        D_8007AD1E = 0;
    }
}

void func_8004A9E8(void) {
    u8 *s;
    s32 n;
    func_8004E958();
    func_8004FF98(0);
    if (D_8007B708 == 3) {
        s = D_800CF478;
        n = (func_80037D6C(0xC0) * 2) & 0x7F;
        while (n != 0) {
            while (*s++ != 0) {
            }
            n--;
        }
        g_gameState->unk1C->unk80C = s;
        while (*s++ != 0) {
        }
        g_gameState->unk1C->unk810 = s;
        func_800384D0(func_8004B040_inline(0), 1, 4, 0);
    }
}

s32 func_8004AAF0(void) {
    Unk8007AECC *p = D_8007AECC;
    Unk16D8 *n;
    s32 lim;
    s32 cnt = 0;
    if (p != NULL) {
        n = p->unk16D8;
        if (n != NULL) {
            lim = D_8007ADFC - 9;
            do {
                Unk8004AB64 *e = n->unk4;
                s32 t = 0;
                if (e->unk6 < lim) {
                    t = e->unk5 == 4;
                }
                cnt += t;
                n = n->unk0;
            } while (n != NULL);
        }
    }
    return cnt;
}

s32 func_8004AB64(Unk8004AB64 *a) {
    s32 t = a->unk6 + 9;
    return t - D_8007ADFC;
}

s32 func_8004AB7C(s32 op, s32 bits, s32 v, s32 w) {
    s32 r = 0;
    s32 i;
    Unk8004AB64 *o;
    s32 a;
    s32 b;

    i = 0;
    if (!(bits & 1)) {
        bits >>= 1;
        i = 1;
        while (i < 8 && !(bits & 1)) {
            bits >>= 1;
            i++;
        }
    }
    if (i < 8) {
        o = D_8007AED0[i];
        if (op == 0x2F) {
            if (o->unk4 >= 4) {
                o->unkB = 2;
                r = 1;
            }
        } else {
            a = 2;
            b = 7;
            switch (op) {
            case 0x35:
                b = 6;
                D_8007AE10[0] = v;
                a = 1;
                break;
            case 0x36:
                b = 9;
                D_8007AE10[0] = v;
                a = 0;
                break;
            case 0x37:
                break;
            }
            D_8007AE34 = w;
            r = func_800384D0(o, a, b, 0);
        }
    }
    return r;
}

void func_8004AC80(void) {
    D_8007AECC = NULL;
    D_8007AE54 = 0;
    D_8007954E = 0;
    D_8007AD1E = 0;
}

void func_8004ACA4(Unk8004AB64 *o, s32 dx, s32 dy) {
    SVECTOR v;
    SVECTOR *pv = &v;
    s32 p[2];
    volatile s32 sxy;
    GameData *gd;

    v.vx = o->pos.vx;
    v.vy = o->pos.vy + ((o->unk32 * (s8)o->unk2F) >> 6) - 50;
    v.vz = o->pos.vz;
    if (D_8007B708 == 1) {
        func_800C0F08();
    }
    gd = g_gameState->unk1C;
    gte_SetRotMatrix(&gd->unk18);
    gte_SetTransMatrix(&gd->unk18);
    gte_ldv0(pv);
    gte_rtps();
    gte_stsxy((s32 *)&sxy);
    if (D_8007B708 == 1) {
        func_800C0FB0();
    }
    p[0] = *(s16 *)&sxy;
    p[1] = sxy >> 16;
    func_800515B8();
    func_800512B8(p[0] + dx, p[1] + dy);
    func_80051610();
}

Unk8004AB64 *func_8004AE00(void) {
    Unk8004AB64 *p = NULL;
    Unk8004AB64 *r;

    if (D_8007AECC != NULL) {
        p = func_8004B040_inline(D_8007AECC->playerUid);
    }
    if (p != NULL && p->unk5 == 4) {
        r = p;
    } else {
        r = NULL;
    }
    return r;
}

s32 func_8004AE84(void) {
    return D_8007AECC->unk16EC;
}

Unk16D8 *func_8004AE9C(void) {
    Unk8007AECC *p = D_8007AECC;
    Unk16D8 *r = NULL;
    if (p != NULL) {
        r = p->unk16D8;
    }
    return r;
}

s32 func_8004AEBC(void) {
    Unk8007AECC *p = D_8007AECC;
    s32 r = 0;
    if (p != NULL) {
        r = p->unk16FC;
    }
    return r;
}

s32 func_8004AEDC(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        r = *D_8007B70C->unk0++;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

s32 func_8004AF58(void) {
    s32 r;
    if (D_8007B6F4 & 1) {
        func_80052DA0();
        r = func_80052D40();
    } else {
        Unk8004AB64 *pp = D_8007B70C;
        r = *pp->unk0++;
        r |= *pp->unk0++ << 8;
        r = (s16)r;
    }
    D_8007B6F4 >>= 1;
    D_8007B6F8 = 1;
    return r;
}

s32 func_8004AFF0(Unk8004AB64 *p) {
    s32 *q;

    if (p != NULL) {
        if (p->unk34 == 0xFFFF) {
            return 0;
        }
        q = (s32 *)func_8001E350(p->unk3B);
        if (q != NULL) {
            return q[2];
        }
    }
    return 0;
}

Unk8004AB64 *func_8004B040(s32 id) {
    return func_8004B040_inline(id);
}

s32 func_8004B098(s32 id) {
    Unk8004AB64 *p = func_8004B040_inline(id);
    s32 r;

    if (p != NULL) {
        if (p->unk5 == 4) {
            r = p->unk3B;
        } else {
            r = -1;
        }
    } else {
        r = -1;
    }
    return r;
}

s32 func_8004B120(s32 i) {
    s32 t = D_8007AECC->unk16E4[i] + 9;
    u32 r = t - D_8007ADFC;
    s32 ret = 0;
    if (r < 9) {
        ret = r;
    }
    return ret;
}

void func_8004B158(s32 a, s32 b, s32 c) {
    func_800384D0(func_8004B040_inline(0), 1, 0x8000 | ((b << 8) & 0x3F00) | ((a << 2) & 0xFC) | (c & 3), 0);
}

s32 func_8004B1EC(void) {
    return (s32)D_8007AEB8;
}

s32 func_8004B1FC(void) {
    return (s32)D_8007AEBC;
}

void func_8004B20C(s32 a) {
    Unk8007AECC *p = D_8007AECC;
    s32 v;
    if (p->unk1700 >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        v = *g_randPtr++ + 200;
        if (a == 0) {
            p->unk1700 = v * 4;
        } else {
            p->unk1700 = v * 2;
        }
    }
}

void func_8004B2A0(void) {
    Unk8007AECCNode *p = D_8007AECC->unk1450;
    for (; p < &D_8007AECC->unk1450[31]; p++) {
        p->unk8 = p + 1;
    }
    p->unk8 = NULL;
    D_8007AECC->unk15D4 = D_8007AECC->unk1450;
    D_8007AECC->unk15D0 = NULL;
}

void func_8004B2F4(Unk8004AB64 *key) {
    func_8004B2F4_inline(key);
}

void func_8004B378(void) {
    Unk16D8 *p = D_8007AECC->unk15D8;
    for (; p < &D_8007AECC->unk15D8[31]; p++) {
        p->unk0 = p + 1;
    }
    p->unk0 = NULL;
    D_8007AECC->unk16E0 = D_8007AECC->unk15D8;
    D_8007AECC->unk16DC = NULL;
    D_8007AECC->unk16D8 = NULL;
}

void func_8004B3D0(s32 a, s32 b) {
    Unk800367D0 *p = func_800367D0(a, b, 0x38, 0x10);
    p->unk5 = 3;
    p->unkF = 1;
}

u16 func_8004B404(s32 arg0) {
    return func_8004B404_inline(arg0);
}

void func_8004B4A8(s32 i, s16 v) {
    D_8007AE10[i & 7] = v;
}

Unk8004AB64 *func_8004B4C4(s32 id) {
    return func_8004B4C4_inline(id);
}

s32 func_8004B574(void) {
    s32 n = 0;
    s32 v = func_80052D40();
    s16 bit;
    for (bit = 1; bit != 0; bit <<= 1) {
        n += (v & bit) != 0;
    }
    return n;
}

s32 func_8004B5CC(void) {
    u8 buf[8];
    s32 bits = func_80052D40();
    s32 n = 0;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (bits & 1) {
            buf[n++] = i;
        }
        bits >>= 1;
    }
    if (n != 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        return 1 << buf[(n * *g_randPtr++) >> 8];
    }
    return 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80011A44);

s32 func_8004B68C(Unk8004AB64 *o, s32 prop) {
    s32 r;
    s32 i;
    void *u;

    switch (prop) {
    case 0:
        return o->pos.vx;
    case 1:
        return o->pos.vy;
    case 2:
        return o->pos.vz;
    case 3:
        return (u8)((u16)o->pos.rotY >> 4);
    case 4:
        return o->unkF;
    case 5:
        return o->unk7;
    case 6:
        return o->unk4;
    case 7:
        return o->unk3A;
    }
    r = 0;
    for (i = 0; i < 8; i++) {
        if (D_8007AED0[i] == o) {
            break;
        }
    }
    if (i < 8) {
        u = ((void *(*)(u16))func_800BED10)(1 << i);
        if (u != NULL) {
            r = func_800BDED4(u, prop);
        }
    }
    return r;
}

void func_8004B7A4(Unk800219D8 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 (*fn)(s32, s32);
    s32 r;

    if (arg0 != NULL) {
        i = 0;
        if (arg1 == 2) {
            arg0->unk22 = arg2 * 16;
            return;
        }
        for (; i < 8; i++) {
            if (D_8007AED0[i] == arg0) {
                break;
            }
        }
        if (i < 8) {
            if (arg1 == 0x20) {
                fn = D_800BED6C;
            } else {
                fn = func_800BED10;
            }
            r = fn((1 << i) & 0xFFFF, i);
            if (r != 0) {
                func_800BE1E0(r, arg1, arg2);
            }
        }
    }
}

s32 func_8004B85C(s32 slot) {
    return func_8004B85C_find(func_8004B85C_key(slot), slot);
}

s32 func_8004B914(void) {
    s32 v;
    Unk8004AB64 *pp;

    if (D_8007B6F4 & 1) {
        func_80052DA0();
        v = func_80052D40();
    } else {
        pp = D_8007B70C;
        v = *pp->unk0++;
        v |= *pp->unk0++ << 8;
        v |= *pp->unk0++ << 16;
    }
    D_8007B6F4 >>= (D_8007B6F8 = 1);
    return v;
}

void func_8004B9B8(Unk8004B9B8 *p) {
    func_8004B9B8_inline(p);
}

void func_8004BA20(SVECTOR *v, s32 *x, s32 *y) {
    DVECTOR s;
    GameData *gd;

    if (x != NULL && y != NULL) {
        if (D_8007B708 == 1) {
            func_800C0F08();
        }
        gd = g_gameState->unk1C;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        gte_ldv0(v);
        gte_rtps();
        gte_stsxy(&s);
        if (D_8007B708 == 1) {
            func_800C0FB0();
        }
        *x = s.vx;
        *y = s.vy;
    }
}

void func_8004BB30(void *p) {
    s32 i;
    s32 bit;
    for (i = 0; i < 8; i++) {
        if (D_8007AED0[i] == p) {
            break;
        }
    }
    if (i < 8) {
        bit = 1 << i;
        D_8007AE12 = bit;
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BB7C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BB80);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BBBC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BBD0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BBDC);

void func_8004BBF0(void) {
    func_80051568();
    func_8004E924();
    func_8004E7E0();
    rand_init(GetRCnt(2));
    func_8004AC80();
}

s32 func_8004BC38(a)
u8 a;
{
    s32 r = 0;
    if (D_8007B718 == 0) {
        func_8004E7FC();
        func_8004883C();
        r = func_80048F54();
        func_8004FBD8();
        func_8004FE0C();
    }
    return r;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetRCnt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetRCnt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StartRCnt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", StopRCnt);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ResetRCnt);

void rand_init(s32 seed) {
    u32 tbl[0x209];
    u32 v = 0;
    s32 i;
    s32 j;
    for (i = 0; i < 17; i++) {
        for (j = 31; j >= 0; j--) {
            seed = seed * 0x5D588B65 + 1;
            v = (v >> 1) | (seed & 0x80000000);
        }
        tbl[i] = v;
    }
    tbl[16] = (tbl[16] << 23) ^ (tbl[0] >> 9) ^ tbl[15];
    for (i = 17; i < 0x209; i++) {
        tbl[i] = (tbl[i - 17] << 23) ^ (tbl[i - 16] >> 9) ^ tbl[i - 1];
    }
    for (i = 0; i < 0x209; i++) {
        g_randTable[i] = tbl[i];
    }
    rand_refill();
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", rand_refill);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004BF74);

void func_8004BFB0(void) {
    GameState *s;
    Unk7D4 *f;

    if (D_8007B950 != 0) {
        D_8007B950 = 0;
    } else {
        func_8004E924();
        func_8002DD54();
    }
    s = g_gameState;
    D_8007BBD4 = 0;
    f = s->unk1C->unk7D4;
    D_8007BB98.unk0 = 0;
    D_8007BBCC = f->unkC;
    D_8007BB98.unk4 = *(s32 *)&f->unk4;
    D_8007B94C = f->unk8;
    D_8007BB98.unk8 = -1;
    switch (s->mode) {
    case 1:
        D_8007B954 = s->unk1C->unk8E4.ovl65->unkC->unk8;
        break;
    case 3:
        D_8007B954 = (Unk8007B954 *)s->unk1C->unk8E4.ovl65->unk8->unk14;
        break;
    case 2:
        D_8007B954 = (Unk8007B954 *)s->unk1C->unk8E4.ovl65->unk11C0->unk14;
        break;
    }
    D_8007BBD4 = 1;
    func_8004E16C();
}

static inline void func_8004C0E8_set(s32 type, u32 bits) {
    D_8007B988.unk0 = type;
    D_8007B988.unk4 = bits;
}

void func_8004C0E8(void) {
    s32 f = 0;
    u32 cur = g_padPortHeld[0] & 0x3FFFFFF;
    u32 x;

    x = ~cur & D_8007B978;
    D_8007B97C = cur & ~D_8007B978;
    D_8007B980 = x;
    if (x != 0) {
        func_8004C0E8_set(4, x);
    }
    if (D_8007B978 != cur) {
        D_8007B990 = 16;
    }
    D_8007B990 = g_gameState->unk9 != 0 ? D_8007B990 - 2 : D_8007B990 - 1;
    if (D_8007B990 < 0 && !(D_8007B990 & 3)) {
        f = 1;
        D_8007B978 &= ~0xF0;
    }
    x = cur & ~D_8007B978;
    if (x != 0) {
        s32 v = 1;
        if (f) {
            v = 2;
        }
        func_8004C0E8_set(v, x);
    }
    D_8007B978 = cur;
}

void func_8004C204(void) {
    GameState *s = g_gameState;
    GameData *g;
    u8 **pp;
    u32 *tbl;
    u8 *cur;

    if (s->mode == 2 || s->mode == 6) {
        D_8007B9A0 = 1;
        pp = &D_8007B9A8;
        tbl = &D_8007B9B0[s->unk1C->unk8];
        D_8007BCA8 = pp;
        D_8007B9A4 = D_8007B9A8 = (u8 *)tbl;
    } else {
        D_8007B9A0 = 0x4000;
        cur = g_primPtr;
        D_8007BCA8 = &g_primPtr;
        D_8007B9A4 = cur;
    }
    g = g_gameState->unk1C;
    D_8007BCA4 = g->unk10[g->unk8];
}

/* width of an embedded string, up to 0xF7 / 0xFF */
static inline s32 func_8004C2C0_width(u8 *s) {
    s32 w = 0;
    s32 multi = 0;
    s32 c;
    u32 g;

    D_8007BB98.unk0 = 0;
    c = *s++;
    while (c != 0xF7 && c != 0xFF) {
        do {
            g = func_80050F68(c);
            if (g != 0) {
                w += 1 + (g >> 28);
            }
            multi = multi ? 0 : (c >= 0xFA ? c < 0xFE : 0);
            c = *s++;
        } while (multi);
    }
    return w;
}

/* copy character ch's name into D_8007B930, converting the case */
static inline void func_8004C2C0_name(s32 ch) {
    u8 *name = g_gameState->unk1C->characters[ch].unk0;
    s32 k;
    u32 c;

    for (k = 0; k < 8; k++) {
        c = name[k];
        if (c - 0x30 < 0x1A) {
            c -= 0x20;
        } else if (c - 0x64 < 0x14) {
            c -= 0x14;
        } else if (c - 0x7A < 2) {
            c -= 2;
        }
        D_8007B930[k] = c;
    }
}

void func_8004C2C0(u8 **pp, s32 *pw) {
    u8 *s;
    u8 *t;
    u8 *tbl;
    u8 *str;
    u8 *p;
    s32 w;
    s32 c;
    s32 n, k;
    u32 g, i;

    if (pp != NULL && pw != NULL) {
        s = *pp;
        w = *pw;
        c = *s++;
        while (c != 0xFF) {
            switch (c) {
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                w += func_8004C2C0_width(g_gameState->unk1C->characters[c - 0x10].unk0);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                w += func_8004C2C0_width(g_gameState->unk1C->characters[func_8004B120(c - 0x18)].unk0);
                break;
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                func_8004C2C0_name(c - 0x80);
                w += func_8004C2C0_width(D_8007B930);
                break;
            case 0xE:
                n = D_8007BBA8[*s++];
                if (n < 0x100) {
                    str = D_8006A218 + D_80069018[n].unk0;
                } else if (n < 0x200) {
                    str = D_8006ADAC + D_8006B374[n - 0x100].unk0;
                } else {
                    str = (u8 *)func_8006276C(n - 0x200);
                }
                w += func_8004C2C0_width(str);
                break;
            case 0x40:
                n = D_8007BBA8[*s++];
                if (n < 0) {
                    n = -n;
                    w += 9;
                }
                do {
                    n /= 10;
                    w += 9;
                } while (n != 0);
                break;
            case 0x6:
                c = *s++;
                g = (c >> 4) & 3;
                i = c & 7;
                t = NULL;
                if (g < 4 && i < 8 && (tbl = D_8007BB80[g]) != NULL) {
                    k = D_8007BBA8[i];
                    if (k < *tbl) {
                        tbl++;
                        k <<= 1;
                        p = tbl + k;
                        t = tbl + (p[0] | (p[1] << 8));
                    }
                }
                w += func_8004C2C0_width(t);
                break;
            case 0x70:
                if ((u32)D_8007BBA8[0] & (1 << *s++)) {
                    w += 0x19;
                }
                break;
            }
            c = *s++;
        }
        *pp = s;
        *pw = w + 9;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80011A6C);

/* skip the choices hidden by D_8007B964 once the window reaches the choice line */
static inline void func_8004C8C4_skip(s32 id, u8 **pp) {
    s32 c;

    if (D_8007B968 >= 0 && D_8007B970 == id && D_8007BB7C >= D_8007B968) {
        if (!(D_8007B964 & 1)) {
            do {
                while ((c = *(*pp)++) != 0xF7 && c != 0xFF) {
                    if (c < 0xFE) {
                        if (c >= 0xF9) {
                            (*pp)++;
                        }
                    }
                }
                if (c == 0xFF) {
                    (*pp)--;
                    break;
                }
                D_8007B964 >>= 1;
            } while (!(D_8007B964 & 1));
        }
        D_8007B964 >>= 1;
    }
}

/* typewriter: advance the shown character count of an embedded value of n characters */
static inline void func_8004C8C4_type(Unk8007B9C0 *w, s32 n) {
    w->unk24 += w->unk29;
    w->unk2A += w->unk24 >> 3;
    w->unk24 &= 7;
    if (w->unk2A >= n) {
        w->unk2A = 0;
    }
}

/* text b of message bank a (D_8007BB80) */
static inline u8 *func_8004C8C4_text(u32 a, u32 b) {
    u8 *t = NULL;
    u8 *tbl;
    u8 *q;
    s32 k;

    if (a < 4 && b < 8 && (tbl = D_8007BB80[a]) != NULL) {
        k = D_8007BBA8[b];
        if (k < *tbl) {
            tbl++;
            k <<= 1;
            q = tbl + k;
            t = tbl + (q[0] | (q[1] << 8));
        }
    }
    return t;
}

/* print an embedded value, typewriter-style when it ends the text */
static inline void func_8004C8C4_value(Unk8007B9C0 *w, s32 arg, s32 (*fn)(), s32 last) {
    if (last && w->unk2A != 0) {
        func_8004C8C4_type(w, fn(arg, w->unk2A));
    } else {
        fn(arg, 0xFF);
    }
}

void func_8004C8C4(Unk8007B9C0 *w, u8 *s) {
    u8 *p;
    u8 *end;
    s32 c;
    s32 last;
    s32 v;
    u8 *t;
    u8 *tbl;
    u32 b;
    s32 k;
    u8 *q;

    D_8007BB7C = 0;
    end = s + w->unk20;
    p = s + w->unk22;
    func_8004C8C4_skip(w->unkA, &p);
    while (p < end) {
        c = *p++;
        switch (c) {
        case 0xF8:
            func_800512C8(1, 0);
            break;
        case 0xF7:
            func_80051380();
            func_800512C8(0, 14);
            func_80051300();
            D_8007BB7C++;
            func_8004C8C4_skip(w->unkA, &p);
            if (p >= end) {
                w->unk20 = p - s;
            }
            break;
        case 0xF9:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case 0x71:
                func_800512C8(0x12, 0);
                break;
            case 0x44:
                func_800512B4(D_8007BC98[-1].vx);
            case 0x43:
                func_800512C8(*p++, 0);
                break;
            case 0x49:
                func_800512C8(0, *p++);
                break;
            case 0x4A:
                func_800512C8(0, -*p++);
                break;
            case 0x48:
                c = *p++;
                c |= *p++ << 8;
                func_8004C2C0(&p, &c);
                func_800512C8((w->unk18 - c) >> 1, 0);
                break;
            case 0x20:
            case 0x21:
            case 0x22:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
                func_80050F4C(c - 0x20);
                break;
            case 0x40:
                last = end - p == 1;
                v = D_8007BBA8[*p++];
                func_8004C8C4_value(w, v, func_8004F0C8, last);
                break;
            case 0x6:
                c = *p++;
                last = end - p == 1;
                t = func_8004C8C4_text(c, *p++);
                func_8004C8C4_value(w, (s32)t, func_8004F1F0, last);
                break;
            case 0xE:
                last = end - p == 1;
                func_80050F4C(4);
                func_8004C8C4_value(w, D_8007BBA8[*p++], func_8004F2B4, last);
                func_80050F4C(0);
                break;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                func_8004C8C4_value(w, c - 0x10, func_8004F3E8, end == p);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                last = end - p == 0;
                func_8004C8C4_value(w, func_8004B120(c - 0x18), func_8004F3E8, last);
                break;
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                func_8004C8C4_value(w, c - 0x80, func_8004F4A4, end == p);
                break;
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
                func_800512C8(0, 1);
                func_800513CC(D_8006E67C[c - 0x30]);
                func_800512C8(1, -1);
                break;
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
                func_800512C8(0, 1);
                if (c < 0x62) {
                    c ^= 1;
                }
                c = (g_gameState->unk24 >> 1) & 1 ? (g_gameState->unk28 >> (c * 4)) & 0xF : c & 0xF;
                if (c < 2) {
                    c ^= 1;
                }
                func_800513CC(D_8006E67C[c]);
                func_800512C8(1, -1);
                break;
            case 0x3A:
                func_800512C8(4, 2);
                func_800513CC(13);
                func_800512C8(-8, -1);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3B:
                func_800512C8(4, 7);
                func_800513CC(14);
                func_800512C8(-8, -6);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3C:
                func_800512C8(1, 5);
                func_800513CC(15);
                func_800512C8(-5, -4);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3D:
                func_800512C8(6, 5);
                func_800513CC(16);
                func_800512C8(-10, -4);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x2F:
                func_800512C8(0, 1);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x4B:
                func_800513CC(*p++);
                break;
            case 0x70:
                if ((u32)D_8007BBA8[0] & (1 << *p++)) {
                    func_800512C8(6, 3);
                    func_800513CC(0xB6);
                    if (VSync(-1) % 48 >= 16) {
                        func_80051444();
                    }
                    func_800512C8(-24, -3);
                    func_800513CC(0xB5);
                }
                break;
            case 0x2:
            case 0xF:
            case 0x41:
                p++;
                break;
            }
            break;
        case 0xFE:
        case 0xFF:
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
            func_80051004(c);
            c = *p++;
        default:
            func_80051004(c);
            break;
        }
    }
}

void func_8004D2C0(Unk8007B9C0 *w, u8 *e) {
    u8 *p;
    s32 c;
    u8 *start;
    Unk4 tmp;
    s32 n;

    p = e + w->unk20;
    start = p;
    while (w->unk26 == 0 && w->unk2A == 0 && w->unk24 >= 8) {
        start = p;
        c = *p++;
        switch (c) {
        case 0xF9:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case 0x48:
                p += 2;
                func_8004C2C0(&p, &c);
                break;
            case 0x41:
                if (!(w->unk28 & 4)) {
                    w->unk26 = *p++;
                } else {
                    p++;
                }
                break;
            case 0:
            case 1:
                w->unk28 = (w->unk28 & ~4) | 8;
            set26:
                w->unk26 = *p++;
                break;
            case 2:
                w->unk29 = *p++;
                break;
            case 0x42:
                w->unk29 = D_8006E688[(g_gameState->unk24 >> 14) & 7];
                break;
            case 0xC:
                w->unk28 |= 4;
                break;
            case 7:
                p += 2;
                break;
            case 8:
                tmp = D_80011A6C;
                n = D_8007BB7C;
                func_80019750(&D_8007B99C);
                func_80019774(&tmp);
                D_8007B968 = n;
                w->unk28 |= 2;
                break;
            case 9:
                w->unk26 = 0xFF;
            case 0x45:
                w->unk28 |= 0x40;
                break;
            case 0xA:
                w->unk28 |= 8;
                break;
            case 0xF:
                D_8007BBD8 = *p++;
                break;
            case 0x46:
                D_8007BBD8++;
                break;
            case 6:
                p++;
            case 0xE:
            case 0x40:
                p++;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                if (!(w->unk28 & 4)) {
                    w->unk2A = 1;
                    w->unk24 = 0;
                }
                break;
            case 0x43:
            case 0x44:
            case 0x49:
            case 0x4A:
            case 0x4B:
            case 0x70:
            skip1:
                p++;
                break;
            }
            break;
        case 0xFE:
        case 0xFF:
            w->unk26 = 0xFF;
            break;
        case 0xF7:
            D_8007BB7C++;
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
            p++;
        default:
            if (!(w->unk28 & 4)) {
                w->unk24 -= 8;
            }
            break;
        }
    }
    w->unk20 = (w->unk26 != 0 ? start : p) - e;
    if (w->unk26 == 0xFF) {
        w->unk28 &= ~4;
    }
    w->unk24 += w->unk29;
}

void func_8004D69C(Unk8007B9C0 *w, u8 *e) {
    u8 *p = e + w->unk20;
    s32 c;

    c = *p++;
    switch (c) {
    case 0xF9:
        c = *p++;
        switch (c) {
        case 0x41:
            w->unk20 = p + 1 - e;
            break;
        case 1:
            func_8004EEA0_inline(w);
            break;
        case 0:
            w->unk22 = p + 1 - e;
            w->unk28 &= ~8;
            break;
        }
        break;
    case 0xFF:
        func_8004EEA0_inline(w);
        break;
    case 0xFE:
        w->unk22 = p - e;
        break;
    }
}

void func_8004D940(Unk8007B9C0 *w) {
    u8 *e = NULL;
    s32 i = w->unk1E;

    if (i >= 0 && i < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        e += *(u16 *)&((u32 *)e)[(D_8007B708 == 2 || D_8007B708 == 4) ? i : i * 2 + 1];
    }
    if (e != NULL) {
        func_800512C8(((w->unk18 - w->unk1C) >> 1) + 4, 2);
        func_80051300();
        if (w->unk20 < w->unk22) {
            w->unk20 = w->unk22;
        }
        D_8007BB98.unk0 = 0;
        D_8007B964 = D_8007B960;
        func_8004C8C4(w, e);
        if (w->unk26 == 0) {
            if (w->unk2A == 0) {
                func_8004D2C0(w, e);
            } else if (w->unk28 & 4) {
                w->unk2A = 0;
                w->unk24 = 8;
            }
        } else {
            w->unk26 -= (w->unk26 != 0xFF);
            if (w->unk28 & 4) {
                w->unk26 = 0;
            }
            if (w->unk26 == 0) {
                func_8004D69C(w, e);
            }
        }
        func_80051380();
        func_80050F4C(0);
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8004DAD0);

static inline Unk8007B9C0 *func_8004DCEC_init(s32 id, WinLayout *l) {
    Unk8007B9C0 *w;
    Unk8007B9C0 *parent;

    func_8004EEA0_inline(&D_8007B9C0[id]);
    parent = D_8007BB78;
    w = &D_8007B9C0[id];
    w->unk8 = 1;
    w->unkC = NULL;
    w->unk10 = 0;
    w->unk6 = 0;
    w->unk4 = 0;
    w->unkA = 0;
    w->unk0 = parent;
    if (l->unk0 != 0) {
        w->unk4 = l->unk0;
        w->unk6 = (u8)l->unk8;
    } else {
        w->unk4 = 0xA0 - (l->unk2 >> 1);
        w->unk6 = 0x78 - (l->unk9 >> 1);
    }
    w->unk18 = l->unk2;
    w->unk27 = l->unk9;
    w->unk28 = 0;
    w->unk1E = 0;
    w->unk20 = 0;
    w->unk1A = 0;
    w->unk2B = 0;
    w->unk14 = NULL;
    return w;
}

/* window titles */
INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80011EFC);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80011F10);

void func_8004DCEC(s32 msg, s32 id, WinLayout *l, s32 flags) {
    Unk8007B9C0 *w;
    RECT r;
    s32 style;
    s32 n;
    u8 *e;

    e = NULL;
    if (msg >= 0 && msg < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        e += *(u16 *)&((u32 *)e)[(D_8007B708 == 2 || D_8007B708 == 4) ? msg : msg * 2 + 1];
    }
    if (e == NULL) {
        return;
    }
    w = func_8004DCEC_init(id, l);
    w->unkC = func_8004DAD0;
    w->unk26 = 0;
    w->unkA = id;
    w->unk1E = msg;
    w->unk29 = D_8006E688[(g_gameState->unk24 >> 14) & 7];
    w->unk22 = 0;
    w->unk24 = 8;
    w->unk2A = 0;
    w->unk1A = l->unk4;
    w->unk2B = l->unk6;
    w->unk1C = l->unk2;
    w->unk28 |= 0x10;
    if (!(l->unkB & 1)) {
        D_8007BB78 = w;
    }
    r.x = w->unk4;
    r.y = w->unk6;
    r.w = w->unk18 + 3;
    r.h = w->unk27 + 3;
    style = 4;
    if (!(l->unk6 & 0x10)) {
        style = 3;
        if (l->unk4 == 0 && !(flags & 4)) {
            style = 2;
        }
    }
    w->unk14 = func_8002DCAC(&r, 2, style, id);
    if (w->unk14 != NULL) {
        w->unk14->unk50 = (s32)w;
        func_8002E2EC(w->unk14, (void (*)(Window *))func_8004F5D4);
        func_8002E2F4(w->unk14, func_8004F660);
        if (l->unk4 != 0 && !(flags & 4)) {
            func_8002D398(w->unk14, l->unk6 & 3, l->unk4);
        }
        if (D_8007BB90 != 0) {
            func_8002CF20(w->unk14, 3);
        }
        if (flags & 0x40) {
            func_8002E304(w->unk14, (s32)D_80011EFC);
        } else if (flags & 8) {
            func_8002E304(w->unk14, (s32)D_80011F10);
        }
    }
    if (!(l->unkB & 1)) {
        D_8007B968 = -1;
        n = D_8007BB94;
        D_8007BB94 = 0;
        D_8007B970 = id;
        D_8007B958 = n;
    }
    D_8007BBC8++;
    func_8004FF98(0);
}

void func_8004E16C(void) {
    s32 i;
    u8 *e;

    D_8007B958 = 0;
    D_8007B974 = -1;
    D_8007B968 = -1;
    for (i = 0; i < 4; i++) {
        e = func_8004D940_msg(i);
        if (e == NULL || e[0] != 0xF9 || e[1] != 4) {
            break;
        }
        D_8007BB80[i] = e + 2;
    }
    D_8007BB94 = 0;
    D_8007BBC8 = 0;
}

/* Inlined copy of func_8004EFB0 (message i, its size into out). */
static inline u8 *func_8004EFB0_inline(s32 i, WinLayout *out) {
    u8 *e = NULL;
    s32 v;

    if (i >= 0 && i < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        v = ((u32 *)e)[D_8007B708 == 4 ? i : i * 2];
        if (out != NULL) {
            out->unk2 = ((v >> 16) & 0xFFF) + 9;
            out->unk9 = (D_8007B708 == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].unk6) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

WinLayout *func_8004E248(s32 id, s32 *x, s32 *y, s32 *z) {
    u8 *p;
    s32 v;
    s32 done;
    s32 t;
    s32 n, i;
    s32 mask;
    s32 cnt;

    p = func_8004EFB0_inline(id, &D_8007B940);
    if (p != NULL) {
        done = 0;
        D_8007BB90 = 0;
        D_8007B940.unkA = 0;
        D_8007B940.unkB = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*p++ != 0xF9) {
                break;
            }
            switch (*p++) {
            case 0x7:
                D_8007B960 = -1;
                D_8007B95C = *p++;
                D_8007B96C = *p++ - 1;
                break;
            case 0x4F:
                D_8007B960 = D_8007B974;
                n = i = *p;
                mask = D_8007B974;
                while (--i != -1) {
                    n -= (mask ^ 1) & 1;
                    mask >>= 1;
                }
                D_8007B95C = n;
                D_8007B940.unk9 -= (*p++ - n) * 14;
                n = -1;
                mask = D_8007B960;
                cnt = *p++;
                while (--cnt != -1) {
                    n += mask & 1;
                    mask >>= 1;
                }
                if (n >= D_8007B95C) {
                    n = D_8007B95C - 1;
                }
                D_8007B96C = n;
                break;
            case 0x3:
                D_8007B940.unk0 = *p++;
                D_8007B940.unk0 |= *p++ << 8;
                D_8007B940.unk8 = *p++;
                D_8007B940.unkA = 0xFF;
                break;
            case 0x50:
            case 0x51:
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x58:
            case 0x59:
            case 0x5A:
            case 0x5B:
            case 0x5C:
                D_8007B940.unkA = p[-1];
                break;
            case 0x5F:
                *x = *p;
                *x |= *(s8 *)++p << 8;
                *y = *++p;
                *y |= *(s8 *)++p << 8;
                *z = *++p;
                *z |= *(s8 *)++p << 8;
                p++;
                break;
            case 0xB:
                D_8007BB90 = 1;
                break;
            case 0x45:
                D_8007B940.unkB = 1;
                break;
            case 0xD:
                for (;;) {
                    v = *p++;
                    v |= *p++ << 8;
                    if (v == 0xFFFF) break;
                    t = (v >> 12) & 0xF;
                    v &= 0xFFF;
                    if (t == 0 || ((D_8007B960 >> (t - 1)) & 1)) {
                        func_8004C2C0(&p, &v);
                        if (D_8007B940.unk2 < v) {
                            D_8007B940.unk2 = v;
                        }
                    } else {
                        while (*p++ != 0xFF) {
                        }
                    }
                }
                break;
            default:
                done = 1;
                break;
            }
        }
    }
    return &D_8007B940;
}

void func_8004E76C(s32 a) {
    GameData *d = g_gameState->unk1C;
    D_8007BCA4 = d->unk10[d->unk8] + a;
}

s32 func_8004E7A4(s32 i) {
    Character *p = g_gameState->unk1C->party[i];
    if (p == NULL) {
        return -1;
    }
    return p->unk34.b.id;
}

void func_8004E7E0(void) {
    D_8007B978 = 0;
    D_8007B988.unk0 = 0;
    D_8007B988.unk4 = 0;
}

void func_8004E7FC(void) {
    Unk8007B9C0 *p = func_8004EA3C();
    s32 a;
    func_8004C0E8();
    if (D_8007B988.unk0 < 3) {
        if (D_8007B988.unk0 > 0) {
            while (p != NULL) {
                a = D_8007B988.unk4;
                if (D_8007B988.unk0 == 2) {
                    a |= 0x80000000;
                }
                if (p->unkC(p, a) != 0) {
                    break;
                }
                p = p->unk0;
            }
        }
    }
    D_8007B988.unk0 = 0;
    D_8007B988.unk4 = 0;
    D_8007B998 = D_8007B994;
}

u32 func_8004E8D4(void) {
    return g_padPortHeld[0] & 0x3FFFFFF;
}

u32 func_8004E8EC(void) {
    return D_8007B97C & 0x3FFFFFF;
}

u32 func_8004E904(void) {
    return D_8007B980 & 0x3FFFFFF;
}

void func_8004E91C(void) {
}

void func_8004E924(void) {
    Unk8007B9C0 *p;
    for (p = D_8007B9C0; p < D_8007B9C0 + 10; p++) {
        p->unk8 = 0;
    }
    D_8007BB78 = NULL;
}

void func_8004E958(void) {
    Unk8007B9C0 *p;
    Unk8007B9C0 *q;
    for (p = D_8007B9C0; p < D_8007B9C0 + 8; p++) {
        if (p != NULL && p->unk8 != 0) {
            p->unk8 = 0;
            if (func_8004EA3C() == p) {
                func_8004EA30(p->unk0);
            }
            for (q = D_8007B9C0; q < D_8007B9C0 + 10; q++) {
                if (q->unk8 != 0 && q->unk0 == p) {
                    q->unk0 = NULL;
                }
            }
        }
    }
}

void func_8004EA30(Unk8007B9C0 *a) {
    D_8007BB78 = a;
}

Unk8007B9C0 *func_8004EA3C(void) {
    return D_8007BB78;
}

void func_8004EA4C(s32 i) {
    func_8004EEA0_inline(&D_8007B9C0[i]);
}

Unk8007B9C0 *func_8004EB68(void) {
    Unk8007B9C0 *p;
    Unk8007B9C0 *r = NULL;
    for (p = D_8007B9C0; p < D_8007B9C0 + 10; p++) {
        if (p->unk8 != 0 && (p->unk28 & 2)) {
            r = p;
        }
    }
    return r;
}

s32 func_8004EBC0(Unk8004EBC0 *r) {
    Unk8007B9C0 *p;
    for (p = D_8007B9C0; p < D_8007B9C0 + 8; p++) {
        if (p->unk8 != 0 && r->unk0 < p->unk4 + p->unk18 && p->unk4 < r->unk0 + r->unk2 &&
            r->unk8 < p->unk6 + p->unk27 && p->unk6 < r->unk8 + r->unk9) {
            return 1;
        }
    }
    return 0;
}

void func_8004EC74(u32 i, s32 v) {
    if (i < 8) {
        D_8007BBA8[i] = v;
    }
}

s16 func_8004EC98(s32 i) {
    return D_8007B9C0[i].unk8;
}

s32 func_8004ECC4(void) {
    s32 i = -1;
    s32 b = D_8007B960;
    s32 a = D_8007B958;
    for (; i < 15;) {
        if (a < 0) {
            break;
        }
        a -= b & 1;
        i++;
        b >>= 1;
    }
    return i;
}

void func_8004ED00(s32 a, s32 b) {
    s32 r;
    D_8007B974 = a;
    r = -1;
    for (; b >= 0; b--) {
        if (a == 0) {
            break;
        }
        r += a & 1;
        a >>= 1;
    }
    if (r < 0) {
        r = 0;
    }
    D_8007BB94 = r;
}

void func_8004ED3C(void) {
    Unk8007B9C0 *p;
    for (p = D_8007B9C0; p < D_8007B9C0 + 10; p++) {
        if (p->unk8 != 0 && p->unk14 != NULL) {
            p->unk14->unk18 = 0;
        }
    }
}

Window *func_8004ED94(s32 i) {
    Unk8007B9C0 *e = &D_8007B9C0[i];
    Window *r = NULL;
    if (e->unk8 != 0) {
        r = e->unk14;
    }
    return r;
}

Unk8007B9C0 *func_8004EDD0(Unk8004EBC0 *r, Unk8007B9C0 *a, s32 idx, s32 flag) {
    Unk8007B9C0 *e = &D_8007B9C0[idx];
    e->unk8 = 1;
    e->unk0 = a;
    e->unkC = 0;
    e->unk10 = 0;
    e->unk6 = 0;
    e->unk4 = 0;
    e->unkA = 0;
    if (r->unk0 != 0) {
        e->unk4 = r->unk0;
        e->unk6 = r->unk8;
    } else {
        e->unk4 = 160 - (r->unk2 >> 1);
        e->unk6 = 120 - (r->unk9 >> 1);
    }
    e->unk18 = r->unk2;
    e->unk27 = r->unk9;
    if (flag != 0) {
        e->unk28 = 2;
    } else {
        e->unk28 = 0;
    }
    e->unk1E = 0;
    e->unk20 = 0;
    e->unk1A = 0;
    e->unk2B = 0;
    e->unk14 = NULL;
    return e;
}

void func_8004EEA0(Unk8007B9C0 *p) {
    func_8004EEA0_inline(p);
}

void func_8004EFA0(void) {
    D_8007B950 = 1;
}

u8 *func_8004EFB0(s32 i, MsgInfo *out) {
    u8 *e = NULL;
    u16 *t;
    s32 v;
    u32 k;

    if (i >= 0 && i < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        v = ((u32 *)e)[D_8007B708 == 4 ? i : i * 2];
        if (out != NULL) {
            out->unk2 = ((v >> 16) & 0xFFF) + 9;
            out->unk9 = (D_8007B708 == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].unk6) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

void *func_8004F058(s32 i) {
    u8 *e = NULL;

    if (i >= 0 && i < D_8007B954->unk0) {
        e = (u8 *)D_8007B954->unk4;
        e += *(u16 *)&((u32 *)e)[(D_8007B708 == 2 || D_8007B708 == 4) ? i : i * 2 + 1];
    }
    return e;
}

s32 func_8004F0C8(s32 v, s32 n) {
    s32 d;
    s32 digits;
    s32 i;
    s32 q;
    s32 ch;

    if (v < 0) {
        v = 0;
    }
    d = 10;
    digits = 1;
    while (v >= d) {
        d *= 10;
        digits++;
    }
    for (i = 0; i < n; i++) {
        d /= 10;
        if (d == 0) {
            break;
        }
        q = v / d;
        if (d < 2 || q != 0) {
            ch = q % 10;
        } else {
            ch = 0xF;
        }
        func_80051004(ch);
    }
    return digits;
}

s32 func_8004F1F0(u8 *s, s32 max) {
    s32 n = 0;
    s32 i;
    u8 *p;
    u32 c;
    if (s != NULL) {
        p = s;
        c = *p++;
        while (c != 0xF7) {
            if (c == 0xFF) {
                break;
            }
            if (c - 0xF9 < 5) {
                p++;
                n++;
            }
            n++;
            c = *p++;
        }
        p = s;
        if (n < max) {
            max = n;
        }
        for (i = 0; i < max; i++) {
            func_80051004(*p++);
        }
    }
    return n;
}

s32 func_8004F2B4(s32 id, s32 n) {
    s32 esc = 0;
    u8 *s;
    s32 len;
    s32 i;
    u8 *t;

    if (id < 0x100) {
        t = D_8006A218 + D_80069018[id].unk0;
    } else if (id < 0x200) {
        t = D_8006ADAC + D_8006B374[id - 0x100].unk0;
    } else {
        t = (u8 *)func_8006276C(id - 0x200);
    }
    s = t;
    for (len = 0; len < 32; len++) {
        if (!esc && s[len] == 0xFF) {
            break;
        }
        esc = !esc && s[len] == 0xFA;
    }
    if (len < n) {
        n = len;
    }
    for (i = 0; i < n; i++) {
        func_80051004(s[i]);
    }
    return len;
}

s32 func_8004F3E8(s32 arg0, s32 n) {
    u8 *name = g_gameState->unk1C->characters[arg0].unk0;
    s32 len;
    s32 i;

    for (len = 0; len < 8; len++) {
        if (name[len] == 0xFF) {
            break;
        }
    }
    if (len < n) {
        n = len;
    }
    for (i = 0; i < n; i++) {
        func_80051004(name[i]);
    }
    return len;
}

s32 func_8004F4A4(s32 ch, s32 n) {
    s32 i;
    s32 j;
    s32 k;
    u8 *name = g_gameState->unk1C->characters[ch].unk0;
    u32 c;
    u8 *buf;

    for (k = 0; k < 8; k++) {
        c = name[k];
        if (c - 0x30 < 0x1A) {
            c -= 0x20;
        } else if (c - 0x64 < 0x14) {
            c -= 0x14;
        } else if (c - 0x7A < 2) {
            c -= 2;
        }
        D_8007B930[k] = c;
    }
    buf = D_8007B930;
    for (i = 0; i < 8; i++) {
        if (buf[i] == 0xFF) {
            break;
        }
    }
    if (i < n) {
        n = i;
    }
    for (j = 0; j < n; j++) {
        func_80051004(buf[j]);
    }
    return i;
}

void func_8004F5D4(Unk8004F5D4 *p) {
    GameData *g = g_gameState->unk1C;

    D_8007BCA4 = g->unk10[g->unk8] + p->unk18;
    func_800515B8();
    func_80051300();
    func_800512B8(p->unk0 + 2, p->unk2 + 2);
    func_8004D940(p->unk50);
    func_80051380();
    func_80051600();
}

void func_8004F660(Window *w) {
    GameData *g;
    s32 ofs;

    if (D_8007B968 >= 0 && w->unk1C == 3 && (((Window *)w->unk50)->unk28 & 2)) {
        g = g_gameState->unk1C;
        D_8007BCA4 = g->unk10[g->unk8] + w->unk18;
        func_800515B8();
        func_80051300();
        ofs = 8;
        func_800512B8(w->unk0 - 3, w->unk2 + ((D_8007B968 + D_8007B958) * 14 + ofs));
        func_800513CC(1);
        func_80051380();
        func_800515EC();
    }
}

void func_8004F74C(Unk8004AB64 *o) {
    POLY_FT4 *p;
    Unk80068790 *s;
    s32 pos;
    s32 n;
    s32 x;
    s32 nx;

    if (D_8007AECC->unk16EC != 0 && (func_8004E8EC() & 1)) {
        D_8007BBE8 = 60;
        D_8007BBE0 = 1;
    }
    if (D_8007BBE8 > 0 && D_8007AEC8 == 0) {
        n = --D_8007BBE8;
        if (n == 0) {
            D_8007BBE0 = 0;
        }
        if (D_8007B708 == 1 && !(g_gameState->unk1C->unk0 & 0x1C) && n < 58) {
            s = &D_80068790;
            func_800515B8();
            func_80051300();
            func_8004ACA4(o, 0, 0);
            if (D_8007BBE8 == 57) {
                D_8007BBE4 = D_8007BC9C.s.x < 24;
            }
            pos = D_8007BC9C.w;
            if ((pos >> 16) < 16) {
                func_800512B8(pos & 0xFFFF, 16);
            }
            func_800512C8(0, -((D_8007BBE8 >> 2) % 3));
            p = func_80051530();
            p->x1 = p->x3 = p->x0;
            x = p->x0;
            if (D_8007BBE4) {
                nx = x + s->unk4;
            } else {
                nx = x - s->unk4;
            }
            p->x2 = p->x0 = nx;
            p->y2 = p->y3 = p->y0;
            p->y1 = p->y0 = p->y0 - s->unk5;
            p->u0 = p->u2 = s->unk0;
            p->v0 = p->v1 = s->unk1;
            p->u1 = p->u3 = s->unk0 + s->unk4 - (u8)D_8007BBE4;
            p->v2 = p->v3 = s->unk1 + s->unk5;
            p->tpage = 0x5F;
            p->clut = s->unk2;
            func_80051380();
            func_80051610();
        }
    }
}

void func_8004F9A0(void) {
    Unk80068788 *s = &D_80068788;
    s32 c = D_8007BC00;
    s32 hw = D_8007BBF8;
    s32 h = D_8007BBFC;
    s32 n;
    POLY_FT4 *p;
    POLY_FT4 *q;
    s32 y;
    s32 x;
    s32 d;

    if (c == 0x80) {
        n = D_8007BBF4 + 0x19;
        func_80051300();
        func_800512C8(-4, -0x10 - D_8007BC04);
        func_800513CC(n);
        func_80051380();
    }
    p = func_80051530();
    q = func_80051530();
    setSemiTrans(p, 1);
    setSemiTrans(q, 1);
    p->r0 = p->g0 = p->b0 = c;
    q->r0 = q->g0 = q->b0 = c;
    y = p->y0 - D_8007BC04;
    x = p->x0;
    p->y2 = p->y3 = y;
    p->y0 = y - D_8007BC04;
    p->x1 = p->x3 = x + hw;
    p->x2 = p->x0 = x - hw;
    p->y1 = p->y0 = y - h;
    q->y0 = q->y1 = y;
    q->y2 = q->y3 = y + h / 2;
    d = hw / 3;
    q->x0 = q->x2 = x - d;
    q->x1 = q->x3 = x + d;
    p->u0 = p->u2 = s->unk0;
    p->v0 = p->v1 = s->unk1;
    p->u1 = p->u3 = s->unk0 + 0x18;
    p->v2 = p->v3 = s->unk1 + 0x10;
    q->u0 = q->u2 = s->unk0 + 8;
    /* identical branches (jump2 merges them), as in func_80030EE4 */
    if (q) {
        q->u1 = q->u3 = s->unk0 + 0x10;
    } else {
        q->u1 = q->u3 = s->unk0 + 0x10;
    }
    q->v0 = q->v1 = s->unk1 + 0x10;
    q->v2 = q->v3 = s->unk1 + 0x18;
    q->tpage = p->tpage = 0x5F;
    q->clut = p->clut = s->unk2;
}

void func_8004FBD8(void) {
    Unk8004AB64 *o;

    if (D_8007BBF0 == 0 && D_8007BBEC != 0) {
        D_8007BC0C = D_8007BBF4 ? D_8006E6D8 : D_8006E690;
    } else if (D_8007BBF0 != 0 && D_8007BBEC == 0) {
        D_8007BC0C = D_8006E6F0;
    }
    D_8007BBF0 = D_8007BBEC;
    D_8007BBEC = 0;
    if (D_8007BC0C == NULL || D_8007AEC8 != 0) {
        return;
    }
    if (D_8007BC08 > 0) {
        D_8007BC08--;
    } else {
        switch (*D_8007BC0C) {
        case 0xFD:
            D_8007BC0C = NULL;
            return;
        case 0xFE:
            D_8007BC08 = D_8007BC0C[1];
            D_8007BC0C += 4;
            goto draw;
        case 0xFF:
            D_8007BC0C -= D_8007BC0C[1] * 4;
            break;
        }
        D_8007BC00 = D_8007BC0C[0];
        D_8007BBF8 = D_8007BC0C[1];
        D_8007BBFC = D_8007BC0C[2];
        D_8007BC04 = D_8007BC0C[3];
        D_8007BC0C += 4;
    }
    draw:
    if (!(g_gameState->unk1C->unk0 & 0x1C)) {
        o = func_8004B040(D_8007AECC->playerUid);
        if (o != NULL && o->unk5 == 4) {
            func_800515B8();
            func_8004ACA4(o, -2, 0);
            func_8004F9A0();
            func_80051610();
        }
    }
    if (D_8007BBE0 == 0) {
        D_8007BBE8 = 0;
    }
}

void func_8004FE0C(void) {
    if (D_8007BC10 == 0) {
        return;
    }
    if (!(D_8007BC10 & 4) && !func_8004AE84()) {
        return;
    }
    D_8007BC14++;
    if (g_gameState->unk1C->unk0 & 0x1C) {
        return;
    }
    func_800515B8();
    func_800512B8(0x10, 200);
    if ((D_8007BC10 & 3) != 2) {
        if ((D_8007BC14 / 44) & 1) {
            func_800513CC(0x80);
            func_800512C8(2, 0);
            func_800513CC(0x81);
            func_800512C8(2, 0);
            func_800513CC(0x82);
        } else {
            func_800512C8(-2, 0);
            func_800513CC(0x53);
            func_800512C8(2, 0);
            func_800513CC(0x12);
            func_800513CC(0x54);
        }
    } else if ((D_8007BC14 / 15) & 1) {
        func_800513CC(0x1F);
        func_800512C8(2, 0);
        func_800513CC(0x20);
        func_800512C8(2, 0);
        func_800513CC(0x21);
    }
    func_80051610();
}

void func_8004FF98(s32 arg0) {
    s32 mode;

    if (arg0 == 0) {
        D_8007BBE0 = 0;
        D_8007BBE8 = 0;
        return;
    }
    mode = (g_gameState->unk24 >> 17) & 1;
    mode = mode == 0;
    if (arg0 >= 3 || (D_8007AECC->unk16EC != 0 &&
                      (mode >= 2 || (mode == 1 && (arg0 == 2 || D_8007BBC8 >= 3 || D_8007B710 != 0))))) {
        D_8007BBE8 = 60;
    }
}

void func_80050044(void) {
    D_8007BC0C = 0;
    D_8007BC08 = 0;
    D_8007BBF0 = 0;
    D_8007BBEC = 0;
}

void func_80050068(s32 a) {
    D_8007BBEC = 1;
    D_8007BBF4 = a;
}

void func_80050080(s32 a) {
    if (D_8007BC10 != a) {
        D_8007BC10 = a;
        D_8007BC14 = 0x2C;
    }
}

s32 func_800500A8(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)func_8001E350(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = func_8004AEDC();
        b = func_8004AEDC();
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_8 = a * 14;
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = func_8004AF58();
        b = func_8004AF58();
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_12 = a;
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = func_8004AEDC();
        p = (u32 *)func_8001E350(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        gs->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_0 = func_8004AEDC();
        return 0;
    case 0xD8:
        func_800B3444(0, 0, 1);
        a = func_8004AEDC();
        func_80050CA8(a, func_8004AEDC());
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 func_80050410(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)func_8001E350(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = func_8004AEDC();
        b = func_8004AEDC();
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_8 = a * 14;
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = func_8004AF58();
        b = func_8004AF58();
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_12 = a;
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = func_8004AEDC();
        p = (u32 *)func_8001E350(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        gs->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_0 = func_8004AEDC();
        return 0;
    case 0xD8:
        a = func_8004AEDC();
        func_8004AEDC();
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 func_80050744(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)func_8001E350(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = func_8004AEDC();
        b = func_8004AEDC();
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_8 = a * 14;
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = func_8004AF58();
        b = func_8004AF58();
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_12 = a;
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = func_8004AEDC();
        p = (u32 *)func_8001E350(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        ((Unk11C0Slot *)gs->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)func_8001E350(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_0 = func_8004AEDC();
        return 0;
    case 0xD8:
        a = func_8004AEDC();
        func_80050EFC(a, func_8004AEDC());
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

void func_80050A84(void) {
    bzero(&D_8007AEF0, 0x800);
}

s32 func_80050AAC(void) {
    u8 *r = D_8007BC18;
    GameData *gd;
    s32 n;

    if (g_gameState->unk1C->unk8 != 0) {
        r += 0x22B0;
    }
    if (D_8007BC20.unk24 < 2) {
        func_8005338C(D_8007BC20.unkC == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (D_8007BC20.unk24 != 0) {
        D_8007BC20.unk24++;
    }
    n = D_8007BC20.unk4;
    if (n != 0) {
        func_800C0F08();
        gd = g_gameState->unk1C;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        r = (u8 *)func_80053498(n, gd->unk8, gd->unk10[gd->unk8] + 3, (s32)r);
        func_800C0FB0();
    }
    return (s32)r;
}

u8 *func_80050BE0(Unk8007BC20Node *buf) {
    Unk8007BC20Node *p = buf;
    Unk8007BC20Node *end;
    D_8007BC20.unk0 = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->unk0 = p + 1;
        }
        p[-1].unk0 = NULL;
    }
    D_8007BC20.unk4 = 0;
    SetDrawTPage(&D_8007BC20.unk10, 0, 0, GetTPage(0, 3, 0, 0));
    D_8007BC20.unk18 = D_8007BC20.unk10;
    D_8007BC20.unk8 = 0;
    D_8007BC20.unkC = 0;
    D_8007BC20.unk24 = 0;
    D_8007BC18 = (u8 *)(buf + 222);
    return (u8 *)p + 0x4560;
}

void func_80050CA8(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        Unk8007BC20 *p = &D_8007BC20;

        if (a < 0x83) {
            p->unk8 = a;
        } else {
            p->unk8 = 0x82;
        }
    }
    D_8007BC20.unkC = b * 16;
    D_8007BC20.unk24 = a == 0 && b != 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetTPage);

s32 func_80050D38(s32 r) {
    GameData *gd;

    if (D_8007BC50.unk24 < 2) {
        func_80053658(D_8007BC50.unkC == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (D_8007BC50.unk24 != 0) {
        D_8007BC50.unk24++;
    }
    if (D_8007BC50.unk4 != 0) {
        gd = g_gameState->unk1C;
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

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80050F4C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80050F68);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051004);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051084);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051094);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005116C);

void func_80051274(void) {
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005127C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800512B4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800512B8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800512C8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800512E4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051300);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051324);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051358);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051380);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800513A4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800513CC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051444);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005145C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800514AC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800514E8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051530);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051568);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800515B8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800515EC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051600);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051610);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051638);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051668);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800516A0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005179C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051830);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051878);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051920);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051B58);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051CA8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051D4C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051DF4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051E8C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051F18);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80051F54);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052834);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052870);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052CD0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052D0C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052D40);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052D70);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052DA0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052DEC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052E48);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052EC0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052EEC);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80052F24);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005309C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800530A8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80053124);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800531D0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ratan2);

void func_80053384(void) {
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005338C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005345C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80053498);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80053658);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80053728);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80053764);

void func_80053924(void) {
}

void func_8005392C(void) {
    GameData *d = g_gameState->unk1C;
    d->unk568 = 0;
    d->unk56A = 0;
    d->unk56C = 0;
    d->unk56E = 0;
    d->unk56F = 0;
    d->unk570 = 0;
    d->unk571 = 0;
}

s32 func_80053960(Unk800539FC *a, s32 b) {
    if (a->unk38 & b) {
        a->unk38 &= ~b;
        return 2;
    }
    return 1;
}

static inline s32 func_80053960_inline(Unk800539FC *a, s32 b) {
    if (a->unk38 & b) {
        a->unk38 &= ~b;
        return 2;
    }
    return 1;
}

s32 func_8005398C(Character *a, s32 mask) {
    s32 r = 1;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 m = mask & (1 << i);
        if (m) {
            if (func_80053960_inline(a, 1 << i) == 2) {
                r = 2;
            }
        }
    }
    return r;
}

u8 func_800539FC(Character *a, s32 b) {
    return (a->unk38 & b) != 0;
}

s32 func_80053A10(s16 arg0, s16 arg1) {
    s16 y;
    s16 x;
    s32 c;
    u8 i;
    s16 a, b;
    GameState *gs;

    x = arg0;
    y = arg1;
    D_8007BD08 = D_8007BD2C ? &D_8006F2E0[1] : &D_8006F2E0[0];
    D_8007BD0C = D_8007BD10 = D_8007BD30 + (g_gameState->unk1C->unk8 << 10);
    ClearOTag(D_8007BD0C, 5);
    D_8007BD10 += 5;
    if (D_8007BD28 < 0x79) {
        if (D_8007BD28 == 0) {
            gs = g_gameState;
            gs->unk1C->unk0 &= ~0x20;
            *(u32 *)&gs->unk10[0].isbg = *(u32 *)&gs->unk10[1].isbg = 1;
            D_8007BD1E = 0;
            D_8007BD1C = D_8007BD08->unkA;
            D_8007BD20 = a = D_8007BD08->unk0;
            D_8007BD24 = D_8007BD08->unk6;
            D_8007BD22 = b = D_8007BD08->unk2;
            if ((arg0 ^ arg1) & 1) {
                D_8007BD20 = -a;
                D_8007BD22 = -b;
            }
            func_8002F884();
        }
        if (D_8007BD28 & 1) {
            x += x < 160 ? 1 : -1;
        }
        if (D_8007BD28 & 2) {
            y += y < g_gameState->unk10->clip.h / 2 ? 1 : -1;
        }
        if (D_8007BD2C) {
            c = func_8004BB80(D_8007BD28 * 8);
            x += c * (160 - x) / 4096;
            y += c * (g_gameState->unk10->clip.h / 2 - y) / 4096;
        }
        D_8007BD10 = func_80054294(D_8007BD10, x, y);
        i = g_gameState->unk1C->unk8;
        SetDrawMove(D_8007BD10, &g_gameState->unkC[i].disp, g_gameState->unk10[i].clip.x, g_gameState->unk10[i].clip.y);
        addPrim(D_8007BD0C, D_8007BD10);
        D_8007BD10 += 8;
        func_80054650();
        if (D_8007BD28 >= D_8007BD08->unkC) {
            D_8007BD1C -= D_8007BD08->unkE;
            if ((s16)D_8007BD1C < 0) {
                D_8007BD1C = 0;
            }
        }
        if (D_8007BD28 >= D_8007BD08->unk10) {
            func_800540E8(x, y);
            D_8007BD1E += D_8007BD08->unk12;
            if ((s16)D_8007BD1E >= 0x1000) {
                D_8007BD1E = 0xFFF;
            }
        }
        D_8007BD20 += D_8007BD22;
        D_8007BD24 += D_8007BD08->unk8;
        if (D_8007BD24 > 0x1400) {
            D_8007BD24 = 0x1400;
        }
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000001;
        D_8007BD10[2] = 0;
        addPrim(D_8007BD0C, D_8007BD10);
        D_8007BD10 += 4;
    }
    SetDrawEnv((DR_ENV *)D_8007BD10, &g_gameState->unk10[g_gameState->unk1C->unk8]);
    addPrim(D_8007BD0C, D_8007BD10);
    D_8007BD10 += 16;
    setlen(D_8007BD10, 2);
    D_8007BD10[1] = 0xE6000000;
    D_8007BD10[2] = 0;
    addPrim(&D_8007BD0C[3], D_8007BD10);
    D_8007BD10 += 4;
    setaddr(&D_8007BD0C[4], getaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8]));
    setaddr(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], D_8007BD0C);
    if (D_8007BD28 == 0) {
        DrawSync(0);
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000001;
        D_8007BD10[2] = 0;
        DrawPrim(D_8007BD10);
        D_8007BD10 += 3;
        setlen(D_8007BD10, 1);
        D_8007BD10[1] = 0xE1000120;
        DrawPrim(D_8007BD10);
        D_8007BD10 += 2;
        D_8007BD10[0] = 0x03000000;
        D_8007BD10[1] = 0x62000000;
        D_8007BD10[2] = 0;
        D_8007BD10[3] = (g_gameState->unk10->clip.h << 16) | 0x140;
        DrawPrim(D_8007BD10);
        D_8007BD10 += 4;
        setlen(D_8007BD10, 2);
        D_8007BD10[1] = 0xE6000000;
        D_8007BD10[2] = 0;
        DrawPrim(D_8007BD10);
    }
    return D_8007BD28++ > 0x78;
}

void func_800540E8(s32 x, s32 y) {
    u32 *p;
    s32 i;
    u32 v;
    u8 c1, c2;
    u32 xy;
    u32 g2;

    p = D_8007BD10;
    v = D_8007BD1E << 16;
    c1 = v >> 20;
    c2 = v >> 23;
    g2 = (c2 << 16) | (c2 << 8) | c2;
    setlen(p, 1);
    p[1] = 0xE1000340;
    p += 7;
    for (i = 0; i < 4; i++) {
        p[0] = 0x06000000;
        p[1] = ((c1 << 16) | (c1 << 8) | c1) | 0x32000000;
        p[2] = ((D_8006F310[i] * g_gameState->unk10->clip.h) << 16) | ((D_8006F308[i] * 320) & 0xFFFF);
        p[3] = (c1 << 16) | (c1 << 8) | c1;
        p[4] = ((D_8006F310[i + 1] * g_gameState->unk10->clip.h) << 16) | ((D_8006F308[i + 1] * 320) & 0xFFFF);
        p[5] = g2;
        p[6] = (y << 16) | (x & 0xFFFF);
        setaddr(p - 7, p);
        p += 7;
    }
    setaddr(p - 7, getaddr(&D_8007BD0C[2]));
    setaddr(&D_8007BD0C[2], D_8007BD10);
    D_8007BD10 = p;
}

u32 *func_80054294(u32 *buf, s32 x, s32 y) {
    s16 xs[9];
    s16 ys[9];
    SVECTOR sv;
    VECTOR v;
    MATRIX m;
    s32 i;
    s32 dx, dy;
    s32 t;
    s32 ex, ey;
    s16 *py;
    s16 *q;
    DVECTOR *pts;
    WarpVert *p;

    D_8007BD14 = (DVECTOR *)buf;
    D_8007BD18 = (WarpVert *)((u8 *)buf + 0x24);
    buf = (u32 *)((u8 *)buf + 0x2AC);
    dx = g_gameState->unkC->disp.w - x;
    dy = g_gameState->unkC->disp.h - y;
    for (i = 0; i < 5; i++) {
        xs[4 - i] = x - (x * i) / 4;
        xs[4 + i] = x + (dx * i) / 4;
        ys[4 - i] = y - (y * i) / 4;
        ys[4 + i] = y + (dy * i) / 4;
    }
    for (i = 0; i < 5; i++) {
        t = func_8004BB7C(i << 8);
        pts = D_8007BD14;
        pts[i].vx = x - (x * t) / 4096;
        pts[8 - i].vx = x + (dx * t) / 4096;
        pts[i].vy = y - (y * t) / 4096;
        pts[8 - i].vy = y + (dy * t) / 4096;
    }
    for (i = 1; i < 8; i++) {
        ex = ((pts[i].vx - xs[i]) * D_8007BD24) / 4096;
        py = ys;
        ey = ((pts[i].vy - ys[i]) * D_8007BD24) / 4096;
        pts[i].vx = xs[i] + ex;
        pts[i].vy = ys[i] + ey;
    }
    sv.vy = 0;
    sv.vx = 0;
    sv.vz = (s16)D_8007BD20 >> 4;
    func_80030AF4(&sv, &m);
    v.vx = v.vy = D_8007BD08->unk4;
    v.vz = 0;
    ScaleMatrix(&m, &v);
    m.t[0] = x;
    m.t[1] = y;
    m.t[2] = 0;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    p = D_8007BD18;
    for (ex = 0; ex < 9; ex++) {
        q = py;
        sv.vx = xs[ex] - x;
        for (ey = 0; ey < 9; ey++) {
            sv.vy = q[ey] - y;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&v);
            p->x = v.vx;
            p->y = v.vy;
            p++;
        }
    }
    return buf;
}

void func_80054650(void) {
    u32 *p;
    WarpVert *v;
    s32 i;
    s32 j;
    s32 c;
    u32 rgbc;
    s32 u0;
    s32 u1;
    u32 tp;

    p = D_8007BD10;
    c = (D_8007BD1C >> 4) & 0xFF;
    rgbc = (c << 16) | (c << 8) | c | 0x2E000000;
    p += 10;
    for (i = 0; i < 8; i++) {
        v = &D_8007BD18[i * 9];
        tp = ((((D_8007BD14[i].vx + g_gameState->unkC[g_gameState->unk1C->unk8].disp.x) & 0x3FF) >> 6) | 0x100) << 16;
        u0 = D_8007BD14[i].vx & 0x3F;
        u1 = u0 + (D_8007BD14[i + 1].vx - D_8007BD14[i].vx) - 1;
        for (j = 0; j < 8; j++) {
            p[0] = 0x09000000;
            p[1] = rgbc;
            p[2] = (v->y << 16) | (u16)v->x;
            p[3] = (D_8007BD14[j].vy << 8) | (u8)u0;
            p[4] = (v[9].y << 16) | (u16)v[9].x;
            p[5] = (D_8007BD14[j].vy << 8) | (u8)u1 | tp;
            p[6] = (v[1].y << 16) | (u16)v[1].x;
            p[7] = ((D_8007BD14[j + 1].vy - 1) << 8) | (u8)u0;
            p[8] = (v[10].y << 16) | (u16)v[10].x;
            p[9] = ((D_8007BD14[j + 1].vy - 1) << 8) | (u8)u1;
            setaddr(p - 10, p);
            p += 10;
            v++;
        }
    }
    setaddr(p - 10, getaddr(&D_8007BD0C[1]));
    setaddr(&D_8007BD0C[1], D_8007BD10 + 10);
    D_8007BD10 = p;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawMove);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ScaleMatrix);

s32 func_80054A28(Unk8007BD44 *t) {
    u32 max;
    u32 *pm = &max;
    u16 *r;
    s32 i;
    s32 j;
    Unk8007BD44 *c;

    D_8007BD38.unk0 = 1;
    D_8007BD38.unk6 = 0x100;
    D_8007BD38.unk8 = 0;
    D_8007BD38.unkC = t;
    D_8007BD38.unk2 = 0;
    *pm = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    for (i = 0; i < t->unk4; r += 6, i++) {
        func_80054A28_max(pm, r[2]);
        func_80054A28_max(pm, r[3]);
    }
    D_8007BD38.unk4 = max - 1;
    c = D_8007BD38.unkC;
    for (j = 0; j < c->unk4; j++) {
        func_80054E88(j, 0, 0);
        func_80054E88(j, 1, 0);
    }
    func_80054E88(0, 0, 1);
    func_80054E88(0, 1, 1);
    D_800739B0[0].unk0 = 0;
    D_800739B0[0].unk1 = 0;
    D_800739B0[1].unk0 = 0;
    D_800739B0[1].unk1 = 0;
    return 0;
}

s32 func_80054B78(void) {
    Unk8007BD44 *t;
    u16 *r;
    s32 pos;
    s32 i;
    s32 a;
    s32 b;
    u16 f;

    if (!(D_8007BD38.unk0 & 1)) {
        return 3;
    }
    if (D_8007BD38.unkC == NULL) {
        return 1;
    }
    t = D_8007BD38.unkC;
    if (!(D_8007BD38.unk0 & 2) || !(D_8007BD38.unk0 & 0xC)) {
        return 0;
    }
    b = 0;
    i = 0;
    a = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    pos = D_8007BD38.unk8 >> 8;
    for (; i < t->unk4; r += 6, i++) {
        if ((r[0] & 1) && r[2] >= pos) {
            a += *((u8 *)t + r[4] + pos);
        }
        if ((r[1] & 1) && r[3] >= pos) {
            b += *((u8 *)t + r[5] + pos);
        }
    }
    if (a >= 0x100) {
        a = 0xFF;
    }
    if (b >= 0x80) {
        b = 1;
    }
    if (!((g_gameState->unk24 & 0xFF) >> 7) || g_gameState->mode == 5) {
        D_800739B0[0].unk0 = b;
        D_800739B0[0].unk1 = a;
        if (g_gameState->mode == 2 && func_800EA27C()) {
            D_800739B0[1].unk0 = b;
            D_800739B0[1].unk1 = a;
        }
    }
    D_8007BD38.unk8 += D_8007BD38.unk6;
    if (D_8007BD38.unk4 < (D_8007BD38.unk8 >> 8)) {
        if (D_8007BD38.unk0 & 0x10) {
            D_8007BD38.unk8 = D_8007BD38.unk4 << 8;
            func_800550C0(-D_8007BD38.unk6);
        } else {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
        }
        f = D_8007BD38.unk0;
        D_8007BD38.unk0 = f & ~4;
        if (!(f & 8)) {
            D_800739B0[0].unk0 = 0;
            D_800739B0[0].unk1 = 0;
            D_800739B0[1].unk0 = 0;
            D_800739B0[1].unk1 = 0;
            D_8007BD38.unk0 = f & ~6;
        }
    }
    if ((D_8007BD38.unk8 >> 8) < D_8007BD38.unk2) {
        if (D_8007BD38.unk0 & 0x10) {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
            func_800550C0(-D_8007BD38.unk6);
        } else {
            D_8007BD38.unk8 = D_8007BD38.unk4 << 8;
        }
        f = D_8007BD38.unk0;
        D_8007BD38.unk0 = f & ~4;
        if (!(f & 8)) {
            D_800739B0[0].unk0 = 0;
            D_800739B0[0].unk1 = 0;
            D_800739B0[1].unk0 = 0;
            D_800739B0[1].unk1 = 0;
            D_8007BD38.unk0 = f & ~6;
        }
    }
    return 0;
}

s32 func_80054E88(s32 row, s32 col, s32 sel) {
    Unk8007BD44Row *r;
    s32 i;

    r = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6);
    for (i = 0; i < D_8007BD38.unkC->unk4; r++, i++) {
        r->unk0[col] &= ~1;
    }
    r = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6) + row;
    if (sel) {
        r->unk0[col] |= 1;
        D_800739B0[0].unk0 = 0;
        D_800739B0[0].unk1 = 0;
        D_800739B0[1].unk0 = 0;
        D_800739B0[1].unk1 = 0;
        D_8007BD38.unk0 &= ~6;
        D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
    } else {
        r->unk0[col] &= ~1;
    }
    return 0;
}

s32 func_80054F80(void) {
    D_800739B0[0].unk0 = 0;
    D_800739B0[0].unk1 = 0;
    D_800739B0[1].unk0 = 0;
    D_800739B0[1].unk1 = 0;
    D_8007BD38.unk0 = 0;
    D_8007BD38.unk2 = 0;
    D_8007BD38.unk4 = 0;
    D_8007BD38.unkC = 0;
    return 0;
}

s32 func_80054FB8(s32 a) {
    D_8007BD38.unk2 = a;
    D_8007BD38.unk8 = a << 8;
    D_8007BD38.unk0 |= 6;
    return 0;
}

s32 func_80054FE0(s32 on) {
    s32 v;
    if (on) {
        D_8007BD38.unk0 |= 6;
        v = D_8007BD38.unk8 >> 8;
        if (v < D_8007BD38.unk2 || D_8007BD38.unk4 < v) {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
        }
    } else {
        D_800739B0[0].unk0 = 0;
        D_800739B0[0].unk1 = 0;
        D_800739B0[1].unk0 = 0;
        D_800739B0[1].unk1 = 0;
        D_8007BD38.unk0 &= ~6;
    }
    return 0;
}

s32 func_80055060(s32 a, s32 b, s32 on) {
    Unk8007BD44Row *e;

    e = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6) + a;
    if (on) {
        e->unk0[b] |= 1;
    } else {
        e->unk0[b] &= ~1;
    }
    return 0;
}

s32 func_800550C0(s16 a) {
    D_8007BD38.unk6 = a;
    return 0;
}

s32 func_800550D0(s32 a) {
    D_8007BD38.unk0 = (D_8007BD38.unk0 & ~0x18) | (a & 0x18);
    return 0;
}

s32 func_800550F0(s32 a, s32 b) {
    if (a >= 0) {
        D_8007BD38.unk2 = a;
    }
    if (b >= 0) {
        D_8007BD38.unk4 = b;
    }
    return 0;
}

s32 func_80055110(s32 *a) {
    *a = D_8007BD38.unk0;
    return 0;
}

s32 func_80055128(s32 a, s32 b, s32 *out) {
    Unk8007BD44 *h = D_8007BD38.unkC;
    u16 *p = (u16 *)((u8 *)h + h->unk6) + a * 6 + b;
    if (*p & 1) {
        *out = 1;
    } else {
        *out = 0;
    }
    return 0;
}

s32 func_8005517C(Unk8007BD44 **a) {
    *a = D_8007BD38.unkC;
    return 0;
}

s32 func_80055194(s32 *a, s32 *b) {
    *a = D_8007BD38.unk2;
    *b = D_8007BD38.unk4;
    return 0;
}

s32 func_800551BC(s16 *a) {
    *a = D_8007BD38.unk6;
    return 0;
}

s32 func_800551D4(s32 *a) {
    *a = D_8007BD38.unk8 >> 8;
    return 0;
}

s32 func_800551F0(void) {
    akao_init();
    return 0;
}

s32 func_80055210(void) {
    akao_quit();
    return 0;
}

s32 func_80055230(u8 *a) {
    s32 r;
    u8 *p;

    r = func_800565DC((s32 *)a);
    if (r == 0) {
        p = a + 0x10;
        D_800809DC = (u16 *)p;
        p = a + 0x210;
        D_800809E4 = (u16 *)p;
        p = a + 0x310;
        D_80083148 = (u16 *)p;
        p = a + 0x410;
        D_800809E8 = p;
    }
    return r;
}

void func_80055290(s32 a) {
    D_80080AA0.unk0 = a;
    akao_command(0x10);
}

void func_800552B8(s32 a) {
    D_80080AA0.unk0 = a;
    akao_command(0x11);
}

void func_800552E0(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    akao_command(0x14);
}

void func_80055310(void) {
    akao_command(0x40);
}

s32 func_80055330(void) {
    if (D_8007F790 == 0) {
        D_8007F95A = 0;
    }
    return 0;
}

void func_80055350(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0x7F;
    akao_command(0x19);
}

void func_80055384(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    D_80080AA0.unk8 = c & 0x7F;
    akao_command(0x1A);
}

void func_800553BC(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    akao_command(0x12);
}

s32 func_800553EC(s32 a, s32 b, s32 c, s32 d) {
    s32 r;

    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c & 0xFF;
    D_80080AA0.unkC = d & 0x7F;
    if ((u32)a >= 0x400) {
        r = func_800565DC((s32 *)a);
        if (r != 0) {
            return r;
        }
        akao_command(0x24);
        return a;
    }
    return akao_command(0x20);
}

void func_8005546C(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    akao_command(0x21);
}

void func_800554A8(s32 a) {
    D_80080AA0.unk0 = a & 0x3FF;
    akao_command(0x30);
}

u32 func_800554D4(void) {
    u32 mask = D_80080A70.unk0;
    AkaoChannel *c;
    u32 r;
    u32 bit;

    if (mask == 0) {
        return 0;
    }
    c = D_8007E3D8;
    r = 0;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if (mask & bit) {
            r |= c->unk28;
        }
        c++;
    }
    r &= 0xFFFFFF;
    return r;
}

s32 func_8005553C(u32 id) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (id == 0) {
        return 0;
    }
    mask = D_80080A70.unk0;
    if (mask == 0) {
        return 0;
    }
    c = D_8007E3D8;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if ((mask & bit) && id == c->unk28) {
            return 1;
        }
        c++;
    }
    return 0;
}

void func_800555A8(s32 a) {
    if (a == 1) {
        a = 0x81;
    } else {
        a = 0x80;
    }
    akao_command(a);
}

void func_800555DC(s32 a) {
    D_80080AA0.unk0 = a;
    akao_command(0x90);
}

void func_80055604(s32 a) {
    D_80080AA0.unk0 = a;
    akao_command(0x92);
}

void func_8005562C(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9B;
        break;
    case 2:
        cmd = 0x9D;
        break;
    case 3:
        cmd = 0x9F;
        break;
    default:
        cmd = 0x99;
        break;
    }
    akao_command(cmd);
}

void func_80055698(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9A;
        break;
    case 2:
        cmd = 0x9C;
        break;
    case 3:
        cmd = 0x9E;
        break;
    default:
        cmd = 0x98;
        break;
    }
    akao_command(cmd);
}

void func_80055704(s32 a) {
    D_80080AA0.unk0 = a & 0x7F;
    akao_command(0xA8);
}

void func_80055730(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0x7F;
    akao_command(0xA9);
}

void func_80055764(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c & 0x7F;
    akao_command(0xA0);
}

void func_800557A8(s32 a, s32 b, s32 c, s32 d) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c;
    D_80080AA0.unkC = d & 0x7F;
    akao_command(0xA1);
}

void func_800557F0(s32 a) {
    D_80080AA0.unk0 = a & 0xFF;
    akao_command(0xAA);
}

void func_8005581C(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    akao_command(0xAB);
}

void func_80055850(s32 a, s32 b, u8 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c;
    akao_command(0xA2);
}

void func_80055894(s32 a, s32 b, s32 c, u8 d) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c;
    D_80080AA0.unkC = d;
    akao_command(0xA3);
}

void func_800558DC(s32 a) {
    D_80080AA0.unk0 = a & 0xFF;
    akao_command(0xAC);
}

void func_80055908(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    akao_command(0xAD);
}

void func_8005593C(s32 a, s32 b, u8 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c;
    akao_command(0xA4);
}

void func_80055980(s32 a, s32 b, s32 c, u8 d) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFFFFFF;
    D_80080AA0.unk8 = c;
    D_80080AA0.unkC = d;
    akao_command(0xA5);
}

void func_800559C8(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0x7F;
    akao_command(0xC0);
}

void func_800559FC(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    D_80080AA0.unk8 = c & 0x7F;
    akao_command(0xC1);
}

void func_80055A34(s32 a, s32 b, s32 c, s32 d) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    D_80080AA0.unk8 = c & 0x7F;
    D_80080AA0.unkC = d & 0x7F;
    akao_command(0xC2);
}

void func_80055A74(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0x7F;
    akao_command(0xC4);
}

void func_80055AA8(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    D_80080AA0.unk8 = c & 0x7F;
    akao_command(0xC5);
}

void func_80055AE0(s32 a) {
    D_80080AA0.unk0 = a;
    akao_command(0x70);
}

void func_80055B08(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    akao_command(0x71);
}

void func_80055B38(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    D_80080AA0.unk8 = c;
    akao_command(0x72);
}

void func_80055B6C(s32 a) {
    D_80080AA0.unk0 = a & 0xFF;
    akao_command(0xD0);
}

void func_80055B98(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    akao_command(0xD1);
}

void func_80055BCC(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    D_80080AA0.unk8 = c & 0xFF;
    akao_command(0xD2);
}

void func_80055C08(s32 a) {
    D_80080AA0.unk0 = a & 0xFF;
    akao_command(0xD4);
}

void func_80055C34(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    akao_command(0xD5);
}

void func_80055C68(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    D_80080AA0.unk8 = c & 0xFF;
    akao_command(0xD6);
}

void func_80055CA4(s32 a) {
    D_80080AA0.unk0 = a & 0xFF;
    akao_command(0xD8);
}

void func_80055CD0(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    akao_command(0xD9);
}

void func_80055D04(s32 a, s32 b, s32 c) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b & 0xFF;
    D_80080AA0.unk8 = c & 0xFF;
    akao_command(0xDA);
}

void func_80055D40(void) {
    akao_command(0xF0);
}

void func_80055D60(void) {
    akao_command(0xF1);
}

void func_80055D80(s32 a, s32 b) {
    while (func_80056700(a, b) == 1) {
    }
}

s32 func_80055DCC(void) {
    s32 a[2];
    do {
        a[0] = D_800809E0;
        a[1] = D_800809E0;
    } while (a[0] != a[1]);
    return a[0];
}

s32 func_80055DF4(void) {
    D_8008322C = 0;
    D_80083158.unk0 |= 1;
    return 0;
}

s32 func_80055E14(s32 *buf, u32 size, s32 wait) {
    u32 n;
    u32 m;
    s32 slot;

    if (D_80083158.unk0 & 1) {
        if (D_80083228.unk4 == 0) {
            if (buf[0] == 0x4F414B41) {
                func_8005CE04(buf, D_80080A30, 0x40);
                buf += 16;
                size -= 0x40;
                D_80083228.unk4 = D_80080A30[4];
                D_80080A30[6] = slot = D_80080A30[6];
                D_80083228.unk8 = D_80080A30[5];
                D_80083228.unk0 = &D_8007F970[slot];
                D_80083228.unkC = D_80080A30[7] * 16;
            } else {
                size = 0;
                D_80083228.unk8 = 0;
                D_80083228.unkC = 0;
            }
        }
        if (D_80083228.unkC != 0) {
            n = D_80083228.unkC;
            if (size == 0) {
                goto end;
            }
            if (n >= size) {
                n = size;
            }
            func_8005CE04(buf, D_80083228.unk0, n);
            buf += n >> 2;
            size -= n;
            D_80083228.unk0 = (Unk8005EBB4 *)((s32 *)D_80083228.unk0 + (n >> 2));
            D_80083228.unkC -= n;
            if (D_80083228.unkC == 0) {
                func_8005658C((s32 *)&D_8007F970[D_80080A30[6]], &D_8007F970[D_80080A30[6]], D_80080A30[4], D_80080A30[7]);
            }
        }
        if (size != 0) {
            if (D_80083228.unk8 == 0) {
                goto clear;
            }
            m = D_80083228.unk8;
            if (m >= size) {
                m = size;
            }
            size = m;
            SpuSetTransferStartAddr(D_80083228.unk4);
            func_80056644(buf, size);
            D_80083228.unk4 += size;
            D_80083228.unk8 -= size;
            if (wait != 0) {
                func_800566D0();
            }
        }
    end:
        if (D_80083230 == 0) {
        clear:
            D_80083158.unk0 &= ~1;
        }
    }
    return D_80083230;
}

void func_80056028(s32 a, s32 b) {
    func_80055D80(a, b);
}

s32 func_80056048(s32 *a, s32 b, s32 c) {
    s32 *h = a;
    s32 y;
    s32 id;
    u32 i;

    for (i = 0; i < 7; i++) {
        if (D_800809C0[i] == h[1]) {
            D_800809C0[i] = 0;
        }
    }
    i = h[6] + b * 8;
    y = h[4];
    if (b != 0) {
        id = h[1];
        if ((u32)(id - 500) < 100) {
            y += b << 12;
        } else if ((u32)(id - 200) < 100) {
            y += (b * 5) << 12;
        } else if ((u32)(id - 300) < 200) {
            y += b << 14;
            b += 4;
        }
    }
    D_800809C0[b] = h[1];
    func_80056750(a, c, i, y);
    return 0;
}

s32 func_80056130(s32 vol) {
    u8 v;

    if (D_8008315C & 2) {
        CdlATV *atv = &D_8007F718;

        v = (u32)(vol * 46448) >> 17;
        atv->val3 = v;
        atv->val1 = v;
        atv->val2 = v;
        D_8007F718.val0 = v;
    } else {
        D_8007F718.val2 = vol;
        D_8007F718.val0 = vol;
        D_8007F718.val3 = 0;
        D_8007F718.val1 = 0;
    }
    CdMix(&D_8007F718);
    return 0;
}

void func_800561C4(s32 *a, s32 b, s32 c) {
    if (func_800565DC(a) == 0) {
        D_80080AA0.unk0 = (s32)a;
        D_80080AA0.unk4 = (b & 0xFF) << 8;
        D_80080AA0.unk8 = c;
        akao_command(0xE0);
    }
}

void func_80056228(void) {
    akao_command(0xE2);
}

void func_80056248(s32 a) {
    D_80080AA0.unk0 = (a & 0x7F) << 8;
    akao_command(0xE4);
}

void func_80056278(s32 a, s32 b) {
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = (b & 0x7F) << 8;
    akao_command(0xE5);
}

void func_800562B0(s32 a) {
    D_80080AA0.unk0 = (a & 0xFF) << 8;
    akao_command(0xE6);
}

s32 func_800562E0(s32 *a, s32 b) {
    s32 r = func_800565DC(a);
    s32 addr;
    s32 *h;

    if (r == 0) {
        func_800566D0();
        addr = 0x59100;
        if (b == 0) {
            addr = 0x4F100;
        }
        h = a;
        a += 0x10;
        SpuSetTransferStartAddr(addr);
        func_80056644(a, h[4]);
        h[8] = addr;
        func_8005CE04(h, D_8007F720, 0x70);
    } else {
        D_8007F740 = 0;
    }
    return r;
}

void func_80056388(s32 a, s32 b) {
    D_80080AA0.unk0 = (a & 0xFF) << 8;
    D_80080AA0.unk4 = b;
    akao_command(0xED);
}

void func_800563C0(s32 *a, s32 b, s32 c, s32 d) {
    s32 x;
    if (func_800565DC(a) == 0) {
        x = 0x59100;
        if (c == 0) {
            x = 0x4F100;
        }
        D_80080AA0.unk0 = (s32)a;
        D_80080AA0.unk4 = (b & 0xFF) << 8;
        D_80080AA0.unk8 = x;
        D_80080AA0.unkC = d;
        akao_command(0xEC);
    }
}

s32 func_80056448(s32 a, s32 b) {
    if (b == 0) {
        return -1;
    }
    D_80080AA0.unk0 = a;
    D_80080AA0.unk4 = b;
    akao_command(0xE8);
    return 0;
}

s32 func_80056488(void) {
    D_80083168.unk24++;
    D_80083168.unk38++;
    if (D_80083168.unk3C - 1 < D_80083168.unk38) {
        D_80083168.unk38 = 0;
    }
    if ((D_80083168.unk8 & 0x1000000) && D_80083168.unk38 >= 2) {
        func_80061C1C();
    }
    return D_80083168.unk34;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdMix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetTransferStartAddr);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005658C);

s32 func_800565DC(s32 *a) {
    return *a - 0x4F414B41; /* "AKAO" */
}

void func_800565F0(void) {
    SpuSetTransferCallback(NULL);
    D_800809E0 = 0;
}

void func_80056614(void) {
    D_800809E0 = 1;
    SpuSetTransferCallback(func_800565F0);
}

void func_80056644(void *a, s32 b) {
    D_800809E0 = 1;
    SpuSetTransferCallback(func_800565F0);
    func_8005709C(a, b);
}

s32 func_80056694(s32 a, s32 b) {
    func_80056614();
    return func_8005703C(a, b);
}

void func_800566D0(void) {
    s32 a[2];
    do {
        do {
            a[0] = D_800809E0;
            a[1] = D_800809E0;
        } while (a[0] != a[1]);
    } while (a[0] == 1);
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80056700);

s32 func_80056750(s32 *buf, s32 wait, s32 slot, u32 addr) {
    s32 *h;

    func_800566D0();
    h = buf;
    if (h[0] == 0x4F414B41) {
        SpuSetTransferStartAddr(addr);
        buf += 16;
        func_80056644(buf + h[7] * 4, h[5]);
        func_8005658C(buf, &D_8007F970[slot], addr, h[7]);
        if (wait != 0) {
            func_800566D0();
        }
        return 0;
    }
    D_800809E0 = -1;
    return -1;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005681C);

void akao_init(void) {
    SpuStart();
    SpuInitMalloc(4, D_80080990);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1010);
    func_80056644(D_8006F334, 0x40);
    func_800566D0();
    func_8005681C();
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    while (SetRCnt(0xF2000002, 0x44E8, 0x1000) == 0) {
    }
    while (StartRCnt(0xF2000002) == 0) {
    }
    while ((g_akaoTickEvent = OpenEvent(0xF2000002, 2, 0x1000, (void (*)(void))akao_tick)) == -1) {
    }
    while (EnableEvent(g_akaoTickEvent) == 0) {
    }
}

void akao_quit(void) {
    if (D_800809E0 == 1) {
        func_80056644(D_8006F334, 0x40);
        func_800566D0();
    }
    while (StopRCnt(0xF2000002) == 0) {
    }
    UnDeliverEvent(0xF2000002, 2);
    while (DisableEvent(g_akaoTickEvent) == 0) {
    }
    while (CloseEvent(g_akaoTickEvent) == 0) {
    }
    func_800575D0(0xFFFFFF);
    SpuQuit();
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", CloseEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/main", DisableEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/main", UnDeliverEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuQuit);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuInitMalloc);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetReverb);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetIRQ);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetIRQCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _SpuCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005703C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8005709C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetTransferMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetTransferCallback);

s32 func_8005715C(s32 x) {
    s32 n = 5;
    u32 xs = x << 8;
    u32 r = 0;
    u32 q = 0;
    u32 rem = 0;
    u32 d;

    do {
        rem |= xs >> 24;
        q <<= 4;
        r <<= 4;
        if (rem >= (q | 1)) {
            if ((q | 12) * 12 < rem) {
                d = 15;
            } else if ((q | 8) * 8 < rem) {
                d = 12;
            } else if ((q | 4) * 4 < rem) {
                d = 8;
            } else {
                d = 4;
            }
            while (rem < (q | d) * d) {
                d--;
            }
            q |= d;
            rem -= q * d;
            q += d;
            r |= d;
        }
        xs <<= 8;
        rem <<= 8;
        if (--n == 0) {
            goto end;
        }
    } while (rem | xs);
    r <<= n * 4;
end:
    return r;
}

s32 func_80057234(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = func_8005715C(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    return r;
}

static inline s32 func_800574A0_curve(s32 r, u32 x) {
    if (x < 3) {
        switch (x) {
        case 1:
            r /= 2;
            break;
        case 2:
            r = r * 3 / 4;
            break;
        default:
            r /= 4;
            break;
        }
    }
    return r;
}

s32 func_8005729C(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = func_8005715C(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    x >>= 2;
    return func_800574A0_curve(r, x);
}

s32 func_80057364(u32 x) {
    s32 v = (x & 0xFF) << 8;

    switch ((x >> 8) & 3) {
    case 3:
        v -= 0x10000;
        break;
    case 2:
        v = -v;
        break;
    case 1:
        v = 0x10000 - v;
        break;
    }
    return v;
}

s32 func_800573CC(u32 x) {
    s32 v = (x & 0xFF) << 8;

    x >>= 8;
    switch (x & 3) {
    case 3:
        v -= 0x10000;
        break;
    case 2:
        v = -v;
        break;
    case 1:
        v = 0x10000 - v;
        break;
    }
    x >>= 2;
    return func_800574A0_curve(v, x);
}

s32 func_80057488(s32 a) {
    s32 r = 0x10000;
    if (a & 0x100) {
        r = 0xFFFF0000;
    }
    return r;
}

s32 func_800574A0(u32 x) {
    s32 r = (x & 0x100) ? -0x10000 : 0x10000;

    x >>= 9;
    if (x < 3) {
        switch (x) {
        case 1:
            r /= 2;
            break;
        case 2:
            r = r * 3 / 4;
            break;
        default:
            r /= 4;
            break;
        }
    }
    return r;
}

static inline s32 func_80057518_inline(s32 x) {
    s32 i = x & 0xFF;
    return D_8006F440[i] << 8;
}

s32 func_80057518(s32 x) {
    return func_80057518_inline(x);
}

s32 func_80057534(u32 code) {
    s32 v;
    u32 sel;

    v = code & 0xFF;
    v = D_8006F440[v] << 8;

    sel = code >> 9;
    if (sel < 3) {
        switch (sel) {
        case 1:
            v /= 2;
            break;
        case 2:
            v = v * 3 / 4;
            break;
        default:
            v /= 4;
            break;
        }
    }

    return v;
}

void func_800575B4(u32 x) {
    SPU_REG16(0x1F801D88) = x;
    SPU_REG16(0x1F801D8A) = x >> 16;
}

void func_800575D0(s32 x) {
    SPU_REG16(0x1F801D8C) = x;
    SPU_REG16(0x1F801D8E) = (u32)x >> 16;
}

void func_800575EC(u32 x) {
    SPU_REG16(0x1F801D98) = x;
    SPU_REG16(0x1F801D9A) = x >> 16;
}

void func_80057608(u32 x) {
    SPU_REG16(0x1F801D94) = x;
    SPU_REG16(0x1F801D96) = x >> 16;
}

void func_80057624(u32 x) {
    SPU_REG16(0x1F801D90) = x;
    SPU_REG16(0x1F801D92) = x >> 16;
}

void func_80057640(s32 v, u32 l, u32 r, s32 vol) {
    if (vol != 0) {
        l *= vol;
        r *= vol;
        l >>= 7;
        r >>= 7;
    }
    ((SpuVoiceReg *)0x1F801C00)[v].unk0 = l & 0x7FFF;
    ((SpuVoiceReg *)0x1F801C00)[v].unk2 = r & 0x7FFF;
}

void func_80057688(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x4) = x;
}

void func_800576A0(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x6) = (u32)x >> 3;
}

void func_800576BC(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0xE) = (u32)x >> 3;
}

void func_800576D8(s32 v, u16 x) {
    SPU_VOICE_REG(v, 0x8) = x;
}

void func_800576F0(s32 v, u16 x) {
    SPU_VOICE_REG(v, 0xA) = x;
}

void func_80057708(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0x8) = SPU_VOICE_REG8(v, 0x8) | ((((u32)b >> 2) << 15) | (a << 8));
}

void func_80057738(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x8) = (SPU_VOICE_REG(v, 0x8) & 0xFF0F) | (x << 4);
}

void func_80057760(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x8) = (SPU_VOICE_REG(v, 0x8) & 0xFFF0) | x;
}

void func_80057788(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0xA) = (SPU_VOICE_REG(v, 0xA) & 0x3F) | ((((u32)b >> 1) << 14) | (a << 6));
}

void func_800577BC(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0xA) = (SPU_VOICE_REG(v, 0xA) & 0xFFC0) | ((((u32)b >> 2) << 5) | a);
}

void func_800577EC(s32 v, Unk800577EC *p, s32 vol) {
    u16 *r;
    s32 a;
    s32 b;

    p->unk4 = 0;
    r = (u16 *)(0x1F801C00 + v * 16);
    if (vol == 0) {
        a = p->unk18;
        b = p->unk1A;
    } else {
        a = p->unk18 * vol;
        a >>= 7;
        b = p->unk1A * vol;
        b >>= 7;
    }
    *r = a & 0x7FFF;
    *++r = b & 0x7FFF;
    *++r = p->unk10;
    *++r = p->unk8 >> 3;
    *++r = p->unk12;
    *++r = p->unk14;
    r[2] = p->unkC >> 3;
}

void func_80057898(s32 v, Unk800577EC *p) {
    s32 f = p->unk4;

    if (f == 0) {
        return;
    }
    p->unk4 = 0;
    if (f & 0x10) {
        f &= ~0x10;
        func_80057688(v, p->unk10);
        if (f == 0) {
            return;
        }
    }
    if (f & 3) {
        f &= ~3;
        func_80057640(v, p->unk18, p->unk1A, p->unk16);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x80) {
        f &= ~0x80;
        func_800576A0(v, p->unk8);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x10000) {
        f &= ~0x10000;
        func_800576BC(v, p->unkC);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x6600) {
        f &= ~0x6600;
        func_800576F0(v, p->unk14);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x9900) {
        func_800576D8(v, p->unk12);
    }
}

void func_800579B4(AkaoChannel *c, u32 mask, s32 kind) {
    s32 nw;
    if ((u16)c->unkB8 != 0) {
        s32 old;
        c->unkB8 = (u16)c->unkB8 - 1;
        old = c->unk78;
        nw = old + c->unk7C;
        if ((nw & 0xFFE00000) != (old & 0xFFE00000)) {
            c->unk11C |= 3;
        }
        c->unk78 = nw;
    }
    if (kind == 0 && c->unkB4 != 0) {
        u16 old;
        c->unkB4--;
        old = c->unkB2;
        nw = old + c->unkFC;
        if ((nw & 0x7F00) != (old & 0x7F00)) {
            c->unk11C |= 3;
        }
        c->unkB2 = nw;
    }
    if (c->unkC0 != 0) {
        u16 old;
        c->unkC0--;
        old = c->unkBE;
        nw = old + c->unk104;
        if ((nw & 0xFF00) != (old & 0xFF00)) {
            c->unk11C |= 3;
        }
        c->unkBE = nw;
    }
    if (c->unkD0 != 0) {
        c->unkD0--;
    }
    if (c->unkDE != 0) {
        c->unkDE--;
    }
    if ((u16)c->unkD2 != 0) {
        c->unkD2 = (u16)c->unkD2 - 1;
        c->unk3C += c->unk40;
        c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
    }
    if ((u16)c->unkE0 != 0) {
        c->unkE0 = (u16)c->unkE0 - 1;
        c->unk4C += c->unk50;
        c->unk58 = 0x1000000 / ((u32)c->unk4C >> 12);
    }
    if (c->unkE8 != 0) {
        c->unkE8--;
        c->unk5C += c->unk60;
        c->unk68 = 0x1000000 / ((u32)c->unk5C >> 12);
    }
    if (c->unkF0 != 0) {
        if (--c->unkF0 == 0) {
            if (kind == 0) {
                g_akaoSeq->unk3C ^= mask;
            } else {
                D_80080A70.unk1C ^= mask;
            }
            D_80083158.unk8 |= 0x110;
        }
    }
    if (c->unkF2 != 0) {
        if (--c->unkF2 == 0) {
            if (kind == 0) {
                g_akaoSeq->unk44 ^= mask;
            } else {
                D_80080A70.unk24 ^= mask;
            }
            D_80083158.unk8 |= 0x100;
        }
    }
    if ((u16)c->unkD8 != 0) {
        u32 v;
        u32 n;
        c->unkD8 = (u16)c->unkD8 - 1;
        v = c->unkD6 + (u16)c->unkDA;
        c->unkD6 = v;
        n = (v & 0x7F00) >> 8;
        if (v & 0x8000) {
            c->unkD4 = (n * c->unk2C) >> 7;
        } else {
            c->unkD4 = (n * ((c->unk2C * 15) >> 8)) >> 7;
        }
    }
    if ((u16)c->unkE4 != 0) {
        c->unkE4 = (u16)c->unkE4 - 1;
        c->unkE2 += c->unkE6;
    }
    if ((u16)c->unkEC != 0) {
        c->unkEC = (u16)c->unkEC - 1;
        c->unkEA += c->unkEE;
    }
    if ((u16)c->unkC2 != 0) {
        s32 old;
        c->unkC2 = (u16)c->unkC2 - 1;
        old = c->unk30;
        nw = old + c->unk80;
        if ((nw & 0xFFFF0000) != (old & 0xFFFF0000)) {
            c->unk11C |= 0x10;
        }
        c->unk30 = nw;
    }
}

void func_80057D80(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;

    f = c->unk34;
    vol = ((c->unk78 >> 16) * (c->unkB2 >> 8)) >> 7;
    if ((f & 1) && c->unkD0 == 0) {
        c->unk44 += c->unk48;
        t = c->unk1C((u32)c->unk44 >> 16);
        pan = (s16)((u32)(c->unkD4 * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->unk110 = x;
        c->unk11C |= 0x10;
    }
    if ((f & 2) && c->unkDE == 0) {
        c->unk54 += c->unk58;
        t = c->unk20((u32)c->unk54 >> 16);
        pan = (vol * (c->unkE2 >> 8)) << 9 >> 16;
        c->unk11C |= 3;
        c->unk112 = (pan * t) >> 16;
    }
    if (f & 4) {
        c->unk64 += c->unk68;
        t = c->unk24((u32)c->unk64 >> 16);
        c->unk11C |= 3;
        c->unk114 = ((c->unkEA >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->unk11C |= 3;
        vol = ((s16)(c[-1].unk128 << 1) * (c->unkB2 >> 8)) >> 7;
    }
    if (c->unk11C & 3) {
        vol += c->unk112;
        vol = (vol * (s32)(((u32)g_akaoSeq->unk50 >> 16) & 0x7F)) >> 7;
        pan = (c->unkBE >> 8) + c->unk114;
        pan = (pan + (g_akaoSeq->unk5C >> 16) - 0x40) & 0xFF;
        if (D_8008315C & 2) {
            c->unk130 = c->unk132 = (vol * D_8006F640) >> 15;
        } else {
            c->unk130 = (vol * D_8006F540[pan]) >> 15;
            c->unk132 = (vol * D_8006F540[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].unk128 + c->unk110 + (c->unk30 >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->unk128 = ((u16)c->unk84 + pitch) & 0x3FFF;
        c->unk11C |= 0x10;
    } else if (c->unk11C & 0x10) {
        pitch = c->unk2C + c->unk110 + (c->unk30 >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->unk128 = ((u16)c->unk84 + pitch) & 0x3FFF;
    }
}

void func_80058118(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;
    s32 k;

    f = c->unk34;
    vol = ((c->unk78 >> 16) * (c->unkB2 >> 8)) >> 7;
    if (f & 1) {
        c->unk44 += c->unk48;
        t = c->unk1C((u32)c->unk44 >> 16);
        pan = (s16)((u32)(c->unkD4 * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->unk110 = x;
        c->unk11C |= 0x10;
    }
    if (f & 2) {
        c->unk54 += c->unk58;
        t = c->unk20((u32)c->unk54 >> 16);
        pan = (vol * (c->unkE2 >> 8)) << 9 >> 16;
        c->unk11C |= 3;
        c->unk112 = (pan * t) >> 16;
    }
    if (f & 4) {
        c->unk64 += c->unk68;
        t = c->unk24((u32)c->unk64 >> 16);
        c->unk11C |= 3;
        c->unk114 = ((c->unkEA >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->unk11C |= 3;
        vol = ((s16)(c[-1].unk128 << 1) * (c->unkB2 >> 8)) >> 7;
    }
    if (c->unk11C & 3) {
        vol += c->unk112;
        if (!(c->unk28 & 0x2000000)) {
            vol = (vol * ((s16)c->unk100 >> 8)) >> 7;
            pan = ((c->unkBE + c->unk9C) >> 8) + c->unk114 + 0x80;
            pan &= 0xFF;
        } else {
            pan = 0x80;
        }
        if (D_8008315C & 2) {
            c->unk130 = c->unk132 = (vol * D_8006F640) >> 15;
        } else {
            c->unk130 = (vol * D_8006F540[pan]) >> 15;
            c->unk132 = (vol * D_8006F540[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].unk128 + c->unk110 + (c->unk30 >> 16);
        k = c->unk70 & 0xFF00;
        if (!(c->unk28 & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->unk128 = ((u16)c->unk84 + pitch) & 0x3FFF;
        c->unk11C |= 0x10;
    } else if (c->unk11C & 0x10) {
        pitch = c->unk2C + c->unk110 + (c->unk30 >> 16);
        k = c->unk70 & 0xFF00;
        if (!(c->unk28 & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->unk128 = ((u16)c->unk84 + pitch) & 0x3FFF;
    }
}

s32 func_800584C8(s32 arg0) {
    s32 i;
    s32 min;
    s32 best;
    u32 m;
    Unk8007F798 *e;

    if (arg0 != 0) {
        i = 0;
    } else {
        i = g_akaoSeq->unk38;
    }
    min = 0x7FFF;
    best = 24;
    e = &D_8007F798[i];
    do {
        m = (u16)e->unk4;
        if (e->unk4 < (s16)min) {
            min = m;
            best = i;
        }
        i++;
        e++;
    } while (i < 24);
    if ((s16)min == 0x7FFF) {
        return 24;
    }
    func_80058830(g_akaoChannels, best, min);
    return best;
}

s32 func_80058578(s32 a) {
    Unk8007F798 *p;
    if (a != 0) {
        a = 0;
    } else {
        a = g_akaoSeq->unk38;
    }
    while ((p = &D_8007F798[a])->unk4 != 0) {
        a++;
        if (a >= 0x18) {
            break;
        }
    }
    return a;
}

void func_800585E0(AkaoChannel *ch, u32 mask, u32 keep, u32 *used) {
    u32 bit;
    s32 i;
    u32 on;
    s32 f;
    s32 v;

    bit = 1;
    i = 0;
    on = mask & g_akaoSeq->unk10;

    do {
        if (mask & bit) {
            func_80057D80(ch, bit);
            if (ch->unk11C != 0) {
                if (D_80080AC4 & bit) {
                    ch->unk132 = 0;
                    ch->unk130 = 0;
                }
                if (on & bit) {
                    if (keep & bit) {
                        do {
                            *used |= 1 << i;
                        } while (0);
                        ch->unk118 = i;
                    } else {
                        f = (g_akaoSeq->unk8 & bit) != 0;
                        v = func_80058578(f);
                        if (v == 24) {
                            g_akaoSeq->unk0 |= 2;
                            v = func_800584C8(f);
                        }
                        if (v == 24) {
                            ch->unk118 = v;
                            g_akaoSeq->unk0 |= 1;
                        } else {
                            *used |= 1 << v;
                            ch->unk118 = v;
                            D_8007F798[v].unk4 = 0x7FFF;
                        }
                    }
                    if (ch->unk118 < 24) {
                        func_800577EC(ch->unk118, (Unk800577EC *)&ch->unk118, (u16)ch->unk12E);
                        D_800831C8[ch->unk118] = g_akaoSeq;
                        D_80083158.unk8 |= 0x100;
                    }
                } else if (ch->unk118 < 24) {
                    /* identical branches (jump2 merges them), as in func_80030EE4 */
                    if (mask) {
                        ((void (*)(s32, Unk800577EC *, u32))func_80057898)(ch->unk118, (Unk800577EC *)&ch->unk118, ch->unk34);
                    } else {
                        ((void (*)(s32, Unk800577EC *, u32))func_80057898)(ch->unk118, (Unk800577EC *)&ch->unk118, ch->unk34);
                    }
                }
            }
            mask &= ~bit;
        }
        bit <<= 1;
        ch++;
        i++;
    } while (mask != 0);
}

/* K&R definition: callers pass two or three arguments */
void func_80058830(c, voice)
    AkaoChannel *c;
    s32 voice;
{
    u32 i;
    for (i = 0; i < 32; i++) {
        if (voice == c->unk118) {
            c->unk118 = 0x18;
        }
        c++;
    }
    if (D_8007F790 != NULL) {
        c = D_8007F71C;
        for (i = 0; i < 32; i++) {
            if (voice == c->unk118) {
                c->unk118 = 0x18;
            }
            c++;
        }
    }
}

void func_800588B0(u32 extra) {
    u32 m = (g_akaoSeq->unk4 & g_akaoSeq->unkC) | extra;
    u32 i;
    Unk8007F798 *p;
    if (D_8007F790 != NULL) {
        m |= D_8007F790->unk4 & D_8007F790->unkC;
    }
    for (i = 0, p = D_8007F798; i < 24; i++, p++) {
        if (m & (1 << i)) {
            p->unk4 = 0x7FFF;
        } else {
            SpuGetVoiceEnvelope(i, &p->unk4);
            if (p->unk4 == 0) {
                func_80058830(g_akaoChannels, i);
            }
        }
    }
}

void func_80058990(AkaoChannel *c) {
    s32 v;
    D_80080A18.unkC = v = c->unk50;
    v >>= 16;
    v *= D_80080A18.unk0 >> 8;
    v /= 127;
    v <<= 8;
    c->unk50 = v;
}

void func_800589E0(Unk800589E0 *a) {
    a->unk50 = D_80080A24;
}

void func_800589F0(void) {
    u32 keyon;
    u32 ex;
    u32 m1;
    u32 m2;
    u32 b1;
    u32 b2;
    u32 bit;
    AkaoChannel *ch;
    Unk80080A10 *s;
    Unk80080A10 *t;

    ex = D_80080A70.unk0 | D_80080A70.unk10 | D_80083174;
    keyon = 0;
    if ((g_akaoSeq->unk4 & g_akaoSeq->unk10) ||
        (D_8007F790 != NULL && (D_8007F790->unk4 & D_8007F790->unk10))) {
        func_800588B0(ex);
    }
    if (D_8007F790 != NULL) {
        if (D_80083158.unk4 & 0x100) {
            ((void (*)(Unk80080A10 *, AkaoChannel *))func_80058990)(D_8007F790, D_8007F71C);
        }
        t = D_8007F790;
        m1 = t->unk4 & t->unk14 & ~(t->unkC & ex);
        b1 = m1 & t->unkC & ~ex;
        g_akaoSeq = t;
        if (m1 & g_akaoSeq->unk8) {
            func_800585E0(D_8007F71C, m1 & g_akaoSeq->unk8, b1, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m1 &= ~g_akaoSeq->unk8;
        }
        s = &g_akaoMainSeq;
        g_akaoSeq = s;
        m2 = s->unk4 & s->unk14 & ~(s->unkC & (b1 | ex));
        b2 = m2 & s->unkC & ~(b1 | ex);
        if (m2 & g_akaoSeq->unk8) {
            func_800585E0(g_akaoChannels, m2 & g_akaoSeq->unk8, b2, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m1) {
            g_akaoSeq = D_8007F790;
            b1 &= ~b2;
            func_800585E0(D_8007F71C, m1, b1, &keyon);
            g_akaoSeq->unk10 = 0;
            g_akaoSeq = s;
        }
        if (m2) {
            func_800585E0(g_akaoChannels, m2, b2, &keyon);
            g_akaoSeq->unk10 = 0;
        }
        if (D_80083158.unk4 & 0x100) {
            if (m2) {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(D_8007F790, D_8007F71C);
            } else {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(D_8007F790, D_8007F71C);
            }
        }
    } else {
        m2 = g_akaoSeq->unk4 & g_akaoSeq->unk14 & ~(g_akaoSeq->unkC & ex);
        bit = m2 & g_akaoSeq->unkC & ~ex;
        if (m2 & g_akaoSeq->unk8) {
            func_800585E0(g_akaoChannels, m2 & g_akaoSeq->unk8, bit, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m2) {
            func_800585E0(g_akaoChannels, m2, bit, &keyon);
            g_akaoSeq->unk10 = 0;
        }
    }
    m2 = D_80080A70.unk0 & D_80080A70.unk8;
    if (m2) {
        keyon |= D_80080A70.unk4;
        bit = 0x100;
        ch = D_8007E3D8;
        for (; m2 != 0; bit <<= 1, ch++) {
            if (m2 & bit) {
                func_80058118(ch, bit);
                if (ch->unk11C) {
                    ((void (*)(s32, Unk800577EC *, u32))func_80057898)(ch->unk118, (Unk800577EC *)&ch->unk118, ch->unk34);
                }
                m2 &= ~bit;
            }
        }
        D_80080A74 = 0;
    }
    m2 = D_80083158.unk8;
    if (m2 & 0x80) {
        SpuSetReverbModeDepth(g_akaoSeq->unk48 >> 12, g_akaoSeq->unk48 >> 12);
        D_80083158.unk8 &= ~0x80;
    }
    if (m2 & 0x10) {
        SpuSetNoiseClock(D_80080A70.unk0 ? D_80080A70.unk28 : g_akaoSeq->unk6E);
        D_80083158.unk8 &= ~0x10;
    }
    if (m2 & 0x100) {
        func_80059140((u32 *)&D_80083238[1], D_8007F790->unk3C, g_akaoSeq->unk3C, D_80080A70.unk1C);
        func_80059140((u32 *)&D_80083238[0], D_8007F790->unk40, g_akaoSeq->unk40, D_80080A70.unk20);
        func_80059140((u32 *)&D_80083238[2], D_8007F790->unk44, g_akaoSeq->unk44, D_80080A70.unk24);
        func_800575EC(D_80083238[0]);
        func_80057608(D_80083238[1]);
        func_80057624(D_80083238[2]);
        D_80083158.unk8 &= ~0x100;
    }
    if (keyon) {
        func_800575B4(keyon);
    }
}

void func_80058F34(AkaoChannel *c, u32 *out, u32 mask, u32 keep) {
    u32 bit = 1;
    do {
        if (mask & bit) {
            if (c->unk118 < 0x18) {
                *out |= 1 << c->unk118;
            }
        }
        mask &= ~bit;
        c++;
        bit <<= 1;
    } while (mask != 0);
    *out &= keep;
}

void func_80058F94(void) {
    u32 out;
    u32 keep;
    u32 m;
    u32 a;
    u32 b;

    a = 0;
    out = 0;
    keep = ~((D_80080A70.unk0 | D_80080A70.unk10) | D_80083174);
    if (D_8007F790 != NULL) {
        a = D_8007F790->unk18;
        m = a & D_8007F790->unk8;
        if (m != 0) {
            func_80058F34(D_8007F71C, &out, m, keep);
            D_8007F790->unk18 &= ~D_8007F790->unk8;
            a &= ~D_8007F790->unk8;
        }
    }
    b = g_akaoSeq->unk18;
    m = b & g_akaoSeq->unk8;
    if (m != 0) {
        func_80058F34(g_akaoChannels, &out, m, keep);
        g_akaoSeq->unk18 &= ~g_akaoSeq->unk8;
        b &= ~g_akaoSeq->unk8;
    }
    if (D_8007F790 != NULL && a != 0) {
        func_80058F34(D_8007F71C, &out, a, keep);
        D_8007F790->unk18 = 0;
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &out, b, keep);
        g_akaoSeq->unk18 = 0;
    }
    out |= D_80080A70.unkC;
    D_80080A70.unkC = 0;
    if (out != 0) {
        func_800575D0(out);
    }
}

void func_80059140(u32 *out, u32 m1, u32 m2, u32 extra) {
    u32 a;
    u32 r;
    u32 keep;
    u32 b;
    Unk80080A10 *s;

    keep = ~((D_80080A70.unk0 | D_80080A70.unk10) | D_80083174);
    a = 0;
    r = 0;
    s = D_8007F790;
    if (s != NULL) {
        a = s->unk4 & m1;
        if (a & s->unk8) {
            func_80058F34(D_8007F71C, &r, a & s->unk8, keep);
            a &= ~D_8007F790->unk8;
        }
    }
    b = g_akaoSeq->unk4 & m2;
    if (b & g_akaoSeq->unk8) {
        func_80058F34(g_akaoChannels, &r, b & g_akaoSeq->unk8, keep);
        b &= ~g_akaoSeq->unk8;
    }
    if (D_8007F790 != NULL && a != 0) {
        func_80058F34(D_8007F71C, &r, a, keep);
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &r, b, keep);
    }
    *out = r |= extra;
    D_80083158.unk8 |= 0x100;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetNoiseClock);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuGetVoiceEnvelope);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetReverbModeDepth);

u16 func_8005936C(u32 flags, AkaoChannel *c) {
    if ((flags & 0x20) && (u16)(c->unk9A - 0x20) < 0x60) {
        c->unk120 += 0x20000;
        c->unk124 += 0x20000;
        c->unk9A += 0x30;
    }
    return c->unk9A;
}

u16 func_800593C0(u32 flags, AkaoChannel *c) {
    if ((flags & 0x20) && (u16)(c->unk9A - 0x20) < 0x60) {
        c->unk120 -= 0x20000;
        c->unk124 -= 0x20000;
        c->unk9A -= 0x30;
    }
    return c->unk9A;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80059414);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80059504);

void func_80059548(AkaoChannel *arg0, u8 *arg1) {
    arg0->unkB2 = 0x6E00;
    arg0->unk78 = 0x23000000;
    arg0->unk0 = arg1;
    arg0->unk106 = 0;
    arg0->unk108 = 0;
    arg0->unkCA = 0;
    arg0->unk30 = 0;
    arg0->unk10C = 0;
    arg0->unkC2 = 0;
    arg0->unkFA = 0;
    arg0->unkF8 = 0;
    arg0->unkB8 = 0;
    arg0->unk84 = 0;
    arg0->unkF6 = 0;
    arg0->unkF4 = 0;
    arg0->unk34 = 0;
    arg0->unk114 = 0;
    arg0->unkCC = 0;
    arg0->unkA0 = 0xFFFF;
    arg0->unk12E = 0;
    arg0->unkEA = 0;
    arg0->unkE2 = 0;
    arg0->unkD6 = 0;
    arg0->unkEC = 0;
    arg0->unkE4 = 0;
    arg0->unkD8 = 0;
    arg0->unkE8 = 0;
    arg0->unkE0 = 0;
    arg0->unkD2 = 0;
    arg0->unkF2 = 0;
    arg0->unkF0 = 0;
    func_8005EBE8(arg0, 0);
}

u32 func_800595F0(AkaoChannel *c, u32 mask) {
    u32 r;
    u32 i;
    r = i = 0;
    for (; i < 32; i++) {
        if (mask & (1 << i)) {
            if (c->unk118 < 0x18) {
                r |= 1 << c->unk118;
            }
        }
        c++;
    }
    return r;
}

void func_8005963C(s32 addr, s32 mask) {
    AkaoSeqHeader *seq;
    u32 active;
    AkaoChannel *c;
    u16 *ofs;
    u32 bit;
    u32 i;
    u32 x;
    u8 *p;

    g_akaoSeq->unk2C = (u8 *)addr;
    active = ((AkaoSeqHeader *)addr)->unk20;
    seq = (AkaoSeqHeader *)addr;
    if (D_8007F790 != NULL) {
        bit = func_800595F0(D_8007F71C, D_8007F790->unk4);
    } else {
        bit = 0;
    }
    x = 0xFFFFFF;
    D_80080A70.unkC |= ~bit & (~(D_80080A70.unk0 | D_80083174) & x);
    g_akaoSeq->unk18 = 0;
    if (D_800831C4 & 1) {
        g_akaoSeq->unk4 = 0;
        g_akaoSeq->unk1C |= active & mask;
    } else {
        g_akaoSeq->unk1C = 0;
        g_akaoSeq->unk4 |= active & mask;
    }
    g_akaoSeq->unk8 = seq->unk24;
    g_akaoSeq->unkC = seq->unk28;
    g_akaoSeq->unk0 &= ~0x33;
    g_akaoSeq->unk0 = seq->unk14 == D_8007F8DC ? g_akaoSeq->unk0 | 0x20 : g_akaoSeq->unk0 | 0x10;
    p = (u8 *)seq + (seq->unk30 + 0x30);
    g_akaoSeq->unk30 = seq->unk30 != 0 ? (u16 *)p : NULL;
    p = (u8 *)seq + (seq->unk34 + 0x34);
    g_akaoSeq->unk34 = seq->unk34 != 0 ? (s32)p : 0;
    g_akaoSeq->unk38 = 0;
    bit = 1;
    i = 0;
    c = g_akaoChannels;
    ofs = (u16 *)(addr + 0x40);
    for (; i < 32; i++) {
        if (active & bit & mask) {
            c->unk0 = (u8 *)ofs + *ofs;
            c->unk96 = 4;
            c->unk98 = 2;
            c->unkB2 = 0x7F00;
            c->unk78 = 0x3FFF0000;
            c->unk100 = 0x4000;
            c->unk108 = 0;
            c->unk106 = 0;
            c->unkCA = 0;
            c->unk30 = 0;
            c->unk10C = 0;
            c->unkC2 = 0;
            c->unkFA = 0;
            c->unkF8 = 0;
            c->unkBE = 0x8000;
            c->unkC0 = 0;
            c->unkCA = 0;
            c->unkBC = 0;
            c->unkB8 = 0;
            c->unk84 = 0;
            c->unkBA = 0;
            c->unkF6 = 0;
            c->unkCC = 0;
            c->unk114 = 0;
            c->unkF4 = 0;
            c->unk34 = !(g_akaoSeq->unkC & bit) << 6;
            ofs++;
            c->unkEA = 0;
            c->unkE2 = 0;
            c->unkD6 = 0;
            c->unkEC = 0;
            c->unkE4 = 0;
            c->unkD8 = 0;
            c->unkE8 = 0;
            c->unkE0 = 0;
            c->unkD2 = 0;
            c->unkF2 = 0;
            c->unkF0 = 0;
            func_8005EBE8(c, 0);
        } else {
            if ((active & bit) && !(bit & mask)) {
                ofs++;
            }
            c->unk96 = 3;
            c->unk98 = 1;
            c->unk0 = D_8006F40C;
            c->unk11C |= 0x4400;
            c->unk12C = (c->unk12C & 0xFFE0) | 5;
        }
        c->unk118 = 0x18;
        active &= ~bit;
        c++;
        bit <<= 1;
    }
    g_akaoSeq->unk20.w = 0xFFFF0000;
    g_akaoSeq->unk28 = 1;
    g_akaoSeq->unk68 = 0;
    g_akaoSeq->unk48 = 0;
    g_akaoSeq->unk66 = 0;
    g_akaoSeq->unk4C = 0;
    D_80083158.unk8 = 0;
    g_akaoSeq->unk76 = 0;
    g_akaoSeq->unk74 = 0;
    g_akaoSeq->unk72 = 0;
    g_akaoSeq->unk78 = 0;
    g_akaoSeq->unk3C = 0;
    g_akaoSeq->unk40 = 0;
    g_akaoSeq->unk44 = 0;
    g_akaoSeq->unk6C = 0;
    g_akaoSeq->unk14 = 0;
    g_akaoSeq->unk10 = 0;
    D_80083158.unk8 |= 0x100;
}

void func_800599D4(Unk80080A10 *a, AkaoChannel *c, s32 id) {
    u32 i;
    if (a->unk4 != 0 && (id == 0 || id == a->unk6A)) {
        a->unk18 = -1;
        for (i = 32; i != 0; i--) {
            c->unk96 = 3;
            c->unk98 = 1;
            c->unk0 = D_8006F40C;
            c++;
        }
        a->unk6A = 0;
        a->unk14 = 0;
        a->unk10 = 0;
        for (i = 0; i < 24; i++) {
            if (D_800831C8[i] == a) {
                D_800831C8[i] = NULL;
                func_800577BC(i, 5, 3);
            }
        }
    }
}

void func_80059AA8(s32 id, s32 mask) {
    AkaoChannel *c;
    u32 bit;
    u32 active;
    u32 i;
    s32 max;

    bit = 0x100;
    c = D_8007E3D8;
    active = D_80080A70.unk0 | D_80080A70.unk10;
    if (mask & 0x0FFFFFFF) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->unk28 & mask)) {
                if (c->unk34 & 0x100000) {
                    c->unk34 |= 0x200000;
                } else {
                    D_80080A70.unkC |= bit;
                    func_8005EC20(c, bit);
                    c->unk34 = 0;
                }
            }
        }
    } else if (mask < 0) {
        c = &D_8007E3D8[id];
        bit = 0x100 << id;
        if (active & bit) {
            func_80059AA8(c->unk6C, 0);
        }
        bit <<= 1;
        c++;
        if (active & bit) {
            func_80059AA8(c->unk6C, 0);
        }
        return;
    } else if (mask & 0x40000000) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (c->unk28 != 0) {
                active &= ~bit;
            }
        }
        c = D_8007E3D8;
        bit = 0x100;
        max = 0;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (max < c->unk88) {
                    max = c->unk88;
                }
            }
        }
        c = D_8007E3D8;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && max == c->unk88) {
                if (c->unk34 & 0x100000) {
                    c->unk34 |= 0x200000;
                } else {
                    D_80080A70.unkC |= bit;
                    func_8005EC20(c, bit);
                    c->unk34 = 0;
                }
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (id == -1) {
                    if (c->unk6C < 0) {
                        if (c->unk34 & 0x100000) {
                            c->unk34 |= 0x200000;
                        } else {
                            D_80080A70.unkC |= bit;
                            func_8005EC20(c, bit);
                            c->unk34 = 0;
                        }
                    }
                } else if (c->unk6C == id) {
                    if (c->unk34 & 0x100000) {
                        c->unk34 |= 0x200000;
                    } else {
                        D_80080A70.unkC |= bit;
                        func_8005EC20(c, bit);
                        c->unk34 = 0;
                    }
                }
            }
        }
    }
    D_80083158.unk8 |= 0x110;
}

void func_80059E10(AkaoChannel *c, VoiceParams *p, s32 mask, u8 *data) {
    AkaoChannel *ch;
    s32 bit;
    s32 n;

    c->unk6C = p->unk0;
    c->unk28 = p->unk4;
    c->unk9C = p->unk8 << 8;
    c->unk9E = 0;
    c->unkBE = 0x8000;
    c->unkC0 = 0;
    c->unk100 = (p->unkC & 0x7F) << 8;
    c->unkBC = 0;
    c->unk38 = p->unk10;
    c->unk96 = 2;
    c->unk98 = 1;
    c->unk94 = 1;
    c->unk88 = -2;
    c->unk70 = 0;
    c->unkB6 = 0;
    func_80059548(c, data);
    D_800831C8[c->unk118] = NULL;
    func_800577BC(c->unk118, 5, 3);
    D_80080A70.unk0 |= mask;
    D_80080A70.unkC |= mask;
    mask = ~mask;
    D_80080A70.unk4 &= mask;
    D_80080A70.unk8 &= mask;
    D_80080A70.unk1C &= mask;
    D_80080A70.unk20 &= mask;
    D_80080A70.unk24 &= mask;
    if (D_800831C4 & 2) {
        mask = 0x100;
        c = D_8007E3D8;
        n = 16;
        do {
            if ((D_80080A70.unk0 & mask) && !(c->unk28 & 0x2000000)) {
                D_80080A70.unk0 &= ~mask;
                D_80080A70.unk10 |= mask;
            }
            n--;
            c++;
            mask <<= 1;
        } while (n != 0);
    }
}

void func_80059FB8(AkaoChannel *c, u32 voice) {
    u32 i;
    if (voice < 0x18) {
        for (i = 0; i < 32; i++) {
            if (c->unk118 == voice) {
                c->unk118 = 0x18;
                g_akaoSeq->unk14 &= ~(1 << i);
            }
            c++;
        }
    }
}

void func_8005A014(VoiceParams *p, u8 *a, u8 *b, s32 flag) {
    AkaoChannel *c;
    u32 bit;
    s32 n;
    u32 busy;

    if (a == NULL && b == NULL) {
        return;
    }
    if (flag == 0 && p->unk4 != 0) {
        func_80059AA8(0, p->unk4);
    }
    do {
        c = &D_8007E3D8[15];
        bit = 0x800000;
        busy = D_80080A70.unk0 | D_80080A70.unk10 | D_80083168.unkC;
        if (a != NULL && b != NULL) {
            n = 15;
            c--;
            bit = 0x400000;
        loop:
            if (busy & (bit | (bit << 1))) {
                n--;
                c--;
                bit >>= 1;
                if (n == 0) {
                    goto fail;
                }
                goto loop;
            }
        } else {
            n = 16;
        loop2:
            if (busy & bit) {
                n--;
                c--;
                bit >>= 1;
                if (n != 0) {
                    goto loop2;
                }
            }
        }
        if (n == 0) {
        fail:
            func_80059AA8(0, 0x40000000);
            if (busy == (D_80080A70.unk0 | D_80080A70.unk10 | D_80083168.unkC)) {
                return;
            }
        }
    } while (n == 0);
    if (a != NULL) {
        func_80059E10(c, p, bit, a);
        func_80059FB8(g_akaoChannels, c->unk118);
    }
    if (b != NULL) {
        if (a != NULL) {
            c++;
            bit <<= 1;
        }
        func_80059E10(c, p, bit, b);
        func_80059FB8(g_akaoChannels, c->unk118);
        if (a != NULL) {
            c->unk34 |= 0x10000;
        }
    }
    D_80083158.unk8 |= 0x110;
}

static inline u8 *func_8005A20C_get(s32 i) {
    return D_800809DC[i] != 0xFFFF ? D_800809E8 + D_800809DC[i] : NULL;
}

void func_8005A20C(u8 **a, u8 **b, s32 idx) {
    idx &= 0x3FF;
    idx *= 2;
    *a = func_8005A20C_get(idx);
    idx++;
    *b = func_8005A20C_get(idx);
}

/* K&R definition: callers pass two or three arguments */
void func_8005A284(a, c)
    Unk80080A10 *a;
    AkaoChannel *c;
{
    u32 mask;
    u32 bit;
    if (a->unk4 != 0) {
        mask = a->unk4;
        bit = 1;
        do {
            if (mask & bit) {
                mask ^= bit;
                c->unk11C |= 3;
            }
            c++;
            bit <<= 1;
        } while (mask != 0);
    }
}

void func_8005A2CC(void) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (D_80080A70.unk0 != 0) {
        mask = D_80080A70.unk0;
        bit = 0x100;
        c = D_8007E3D8;
        do {
            if (mask & bit) {
                mask ^= bit;
                c->unk11C |= 3;
            }
            c++;
            bit <<= 1;
        } while (mask != 0);
    }
}

void func_8005A320(s32 addr, s32 restore) {
    AkaoSeqHeader *seq = (AkaoSeqHeader *)addr;
    u32 flags;
    u32 x;
    u8 *old;
    s32 d;
    AkaoChannel *c;
    s32 n;
    u32 bit;
    u32 active;

    if (restore == 0) {
        func_8005CE04(&D_8007F8F0, g_akaoSeq, 0x7C);
        func_8005CE04(D_80080AC8, g_akaoChannels, 0x2680);
    } else {
        func_8005CE8C(&D_8007F8F0, g_akaoSeq, 0x7C);
        func_8005CE8C(D_80080AC8, g_akaoChannels, 0x2680);
    }
    flags = g_akaoSeq->unk0 & ~0x30;
    g_akaoSeq->unk0 = flags;
    g_akaoSeq->unk0 = seq->unk14 == D_8007F8DC ? flags | 0x20 : flags | 0x10;
    c = g_akaoChannels;
    n = 32;
    old = g_akaoSeq->unk2C;
    g_akaoSeq->unk2C = (u8 *)seq;
    g_akaoSeq->unk10 = 0;
    bit = 1;
    D_80083158.unk8 |= 0x90;
    d = (u8 *)seq - old;
    active = g_akaoSeq->unk4;
    g_akaoSeq->unk30 = (u16 *)((u8 *)g_akaoSeq->unk30 + d);
    g_akaoSeq->unk34 += d;
    g_akaoSeq->unk10 = g_akaoSeq->unk14;
    do {
        if (active & bit) {
            c->unk0 += d;
            c->unk18 += d;
            c->unk4[0] += d;
            c->unk4[1] += d;
            c->unk4[2] += d;
            c->unk4[3] += d;
            c->unk96 += 2;
            c->unk98 += 2;
            c->unk11C |= 0x1FF93;
            func_8005936C(g_akaoSeq->unk0, c);
        } else {
            c->unk96 = 4;
            c->unk98 = 2;
            c->unk0 = D_8006F40C;
        }
        c->unk118 = 0x18;
        n--;
        c++;
        bit <<= 1;
    } while (n != 0);
    if (D_8007F790 != NULL) {
        active = func_800595F0(D_8007F71C, D_8007F790->unk4 & D_8007F790->unk8);
    } else {
        active = 0;
    }
    g_akaoSeq->unk18 = 0;
    x = 0xFFFFFF;
    D_8007F8F0.unk6A = 0;
    D_80080A70.unkC |= ~active & (~(D_80080A70.unk0 | D_80083168.unkC) & x);
    D_80083158.unk8 |= 0x100;
    if (D_800831C4 & 1) {
        g_akaoSeq->unk1C = g_akaoSeq->unk4;
        g_akaoSeq->unk4 = 0;
    }
}

s32 func_8005A5FC(s32 a) {
    s32 i = 0;

    if (a != 0) {
        i = 7;
        if (a == D_800809D8) {
            i = 6;
        } else {
        loop:
            i--;
            if (i != 0) {
                if (a != D_800809C0[i - 1]) {
                    goto loop;
                }
                i = i - 1;
            }
        }
    }
    return i;
}

void func_8005A658(Unk80080AA0 *a) {
    if (D_8007F8F0.unk6A != 0 && D_8007F8F0.unk6A == a->unk8) {
        func_8005A320(a->unk0, 0);
    } else {
        func_8005963C(a->unk0, -1);
        g_akaoSeq->unk6A = a->unk8;
    }
}

void func_8005A6D4(Unk80080AA0 *a) {
    if (D_8007F8F0.unk6A != 0 && D_8007F8F0.unk6A == a->unk8) {
        func_8005A320(a->unk0, 0);
    } else {
        func_8005963C(a->unk0, a->unkC);
        g_akaoSeq->unk6A = a->unk8;
    }
}

void func_8005A754(void) {
    u32 i;
    AkaoChannel *c;

    if (g_akaoSeq->unk4 != 0) {
        func_8005CE04(g_akaoSeq, &D_8007F8F0, 0x7C);
        func_8005CE04(g_akaoChannels, D_80080AC8, 0x2680);
        for (i = 0, c = D_80080AC8; i < 0x20; i++, c++) {
            func_800593C0(D_8007F8F0.unk0, c);
        }
    }
}

void func_8005A7E4(Unk80080AA0 *a) {
    Unk80080A10 *t;
    if (g_akaoSeq->unk4 != 0 && (D_8007F790 == NULL || D_8007F790->unk6A == 0)) {
        D_8007F790 = &D_8007F8F0;
        D_8007F71C = D_80080AC8;
        func_8005CE04(g_akaoSeq, &D_8007F8F0, 0x7C);
        func_8005CE04(g_akaoChannels, D_8007F71C, 0x2680);
    }
    func_8005963C(a->unk0, -1);
    t = g_akaoSeq;
    t->unk50 = (a->unk10 & 0x7F) << 16;
    t->unk58 = 0;
    t->unk6A = a->unk8;
    func_80059414();
}

void func_8005A8AC(Unk80080AA0 *a) {
    s32 d;

    if (D_8007F8F0.unk6A != 0 && D_8007F8F0.unk6A == a->unk8) {
        func_8005A320(a->unk0, 1);
        D_8007F790 = &D_8007F8F0;
        D_8007F71C = D_80080AC8;
    } else {
        if (g_akaoSeq->unk4 != 0 && (D_8007F790 == NULL || D_8007F790->unk6A == 0)) {
            D_8007F790 = &D_8007F8F0;
            D_8007F71C = D_80080AC8;
            func_8005CE04(g_akaoSeq, &D_8007F8F0, 0x7C);
            func_8005CE04(g_akaoChannels, D_8007F71C, 0x2680);
        }
        func_8005963C(a->unk0, -1);
        g_akaoSeq->unk6A = a->unk8;
    }
    if (D_8007F790 != NULL) {
        d = a->unk10;
        D_80080A18.unk0 = 0x7F8000;
        D_80080A18.unk8 = d;
        D_80083158.unk4 |= 0x100;
        D_80080A18.unk4 = -0x7F8000 / d;
    }
    g_akaoSeq->unk50 = 0;
    g_akaoSeq->unk58 = a->unk10;
    g_akaoSeq->unk54 = (u32)(((a->unk14 & 0x7F) << 16) | 0x8000) / (u32)a->unk10;
    func_80059414();
}

void func_8005AA78(Unk80080AA0 *a) {
    s32 r;
    func_8005A658(a);
    r = 0;
    if (a->unkC != 0) {
        r = a->unkC - 1;
    }
    D_800809B8 = r;
}

void func_8005AAB8(Unk80080AA0 *p) {
    s32 a = p->unk0;
    s32 b = p->unk4;
    p->unk0 = 0x400;
    p->unk4 = 0x1000000;
    p->unk8 = 0x80;
    p->unkC = 0x7F;
    p->unk10 = 0;
    func_8005A014(p, a, b, 0);
}

void func_8005AB04(Unk80080AA0 *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->unk0];

    p->unk4 = 0x2000000;
    p->unk8 = 0x80;
    p->unkC = 0x7F;
    p->unk10 = func_8005A5FC(D_800809E4[p->unk0]);
    func_8005A20C(&a, &b, p->unk0);
    func_8005A014(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            func_8005A20C(&a, &b, p->unk0 + i);
            n--;
            func_8005A014(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void func_8005ABEC(Unk80080AA0 *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->unk0];

    p->unk10 = func_8005A5FC(D_800809E4[p->unk0]);
    func_8005A20C(&a, &b, p->unk0);
    func_8005A014(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            func_8005A20C(&a, &b, p->unk0 + i);
            n--;
            func_8005A014(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void func_8005ACB4(Unk80080AA0 *s) {
    AkaoSeqHdr *h = (AkaoSeqHdr *)s->unk0;
    u32 *offs = h->unk10;
    s32 i;
    u8 *base;
    u16 *p;
    u8 *a;
    u8 *b;
    s32 n;

    s->unk10 = func_8005A5FC(h->unk8);
    n = h->unk4;
    base = (u8 *)(offs + n);
    p = (u16 *)(base + offs[0]);
    if (*p != 0xFFFF) {
        a = (u8 *)(*p + (s32)p + 4);
    } else {
        a = NULL;
    }
    p++;
    if (*p != 0xFFFF) {
        b = (u8 *)(*p + (s32)p + 2);
    } else {
        b = NULL;
    }
    func_8005A014(s, a, b, 0);
    for (i = 1; --n != 0; i++) {
        p = (u16 *)(base + offs[i]);
        if (*p != 0xFFFF) {
            a = (u8 *)(*p + (s32)p + 4);
        } else {
            a = NULL;
        }
        p++;
        if (*p != 0xFFFF) {
            b = (u8 *)(*p + (s32)p + 2);
        } else {
            b = NULL;
        }
        func_8005A014(s, a, b, 1);
    }
}

void func_8005ADE8(s32 *a) {
    func_80059AA8(a[0], a[1]);
}

void func_8005AE14(Unk80080AA0 *p) {
    Unk80080A10 *s;
    s32 v;

    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        s->unk50 = (p->unk4 & 0x7F) << 16;
        s->unk58 = 0;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = D_8007F790;
        if (s != NULL && p->unk0 != 0 && p->unk0 == s->unk6A) {
            v = p->unk4;
            s->unk58 = 0;
            s->unk50 = (v & 0x7F) << 16;
            func_8005A284(s, D_8007F71C);
        }
    }
}

void func_8005AEC8(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    Unk80080A10 *t;
    s32 v;
    s32 cur;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = ((p->unk8 & 0x7F) << 16) | 0x8000;
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        do { } while (0);
        cur = s->unk50 & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        v /= n;
        s->unk58 = n;
        s->unk50 = cur;
        s->unk54 = v;
        func_8005A284(s, g_akaoChannels);
    } else {
        t = D_8007F790;
        if (t == NULL || p->unk0 == 0 || p->unk0 != t->unk6A) {
            return;
        }
        cur = t->unk50 & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        do { } while (0);
        v /= n;
        s = t;
        s->unk58 = n;
        s->unk50 = cur;
        s->unk54 = v;
        func_8005A284(s, D_8007F71C);
    }
}

void func_8005B004(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    AkaoChannel *c;
    s32 v;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        c = g_akaoChannels;
    } else {
        s = D_8007F790;
        if (s == NULL || p->unk0 == 0 || p->unk0 != s->unk6A) {
            return;
        }
        c = D_8007F71C;
    }
    v = ((p->unk8 & 0x7F) << 16) | 0x8000;
    s->unk50 = v;
    s->unk54 = ((((p->unkC & 0x7F) << 16) | 0x8000) - v) / n;
    s->unk58 = n;
    func_8005A284(s, c);
}

void func_8005B104(Unk80080AA0 *p) {
    Unk80080A10 *s;
    s32 v;

    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        s->unk5C = (p->unk4 & 0x7F) << 16;
        s->unk64 = 0;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = D_8007F790;
        if (s != NULL && p->unk0 != 0 && p->unk0 == s->unk6A) {
            v = p->unk4;
            s->unk64 = 0;
            s->unk5C = (v & 0x7F) << 16;
            func_8005A284(s, D_8007F71C);
        }
    }
}

void func_8005B1B8(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    s32 v;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = (p->unk8 & 0x7F) << 16;
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        s->unk64 = n;
        s->unk60 = v;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = D_8007F790;
        if (s == NULL || p->unk0 == 0 || p->unk0 != s->unk6A) {
            return;
        }
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        s->unk64 = n;
        s->unk60 = v;
        func_8005A284(s, D_8007F71C);
    }
}

void func_8005B2D8(u16 *a) {
    D_80080A2C = 0;
    D_80080AC0 = *a << 16;
    func_8005CDE4();
}

void func_8005B30C(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_80080A28 = (((u16)p->unk4 << 16) - D_80080AC0) / n;
    D_80080A2C = n;
}

void func_8005B374(Unk80080AA0 *p) {
    s32 n = 1;
    s32 v;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_80080A28 = (((u16)p->unk8 << 16) - (v = (u16)p->unk4 << 16)) / n;
    D_80080A2C = n;
    D_80080AC0 = v;
}

void func_8005B3E4(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk100 = ((u16)a->unk8 & 0x7F) << 8;
                c->unkBC = 0;
                c->unk11C |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk100 = ((u16)a->unk8 & 0x7F) << 8;
                c->unkBC = 0;
                c->unk11C |= 3;
            }
        }
    }
}

void func_8005B4CC(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk102 = (s16)((((u16)a->unkC & 0x7F) << 8) - c->unk100) / n;
                c->unkBC = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk102 = (s16)((((u16)a->unkC & 0x7F) << 8) - c->unk100) / n;
                c->unkBC = n;
            }
        }
    }
}

void func_8005B64C(u16 *a) {
    u32 bit = 0x100;
    u32 mask = D_80080A70.unk0;
    u32 i;
    AkaoChannel *c = D_8007E3D8;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->unk28 & 0x2000000)) {
                c->unk100 = (*a & 0x7F) << 8;
                c->unkBC = 0;
                c->unk11C |= 3;
            }
        }
    }
}

void func_8005B6C8(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = D_80080A70.unk0;
    u32 i;
    AkaoChannel *c = D_8007E3D8;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unk102 = (s16)((((u16)p->unk4 & 0x7F) << 8) - c->unk100) / n;
            c->unkBC = n;
        }
    }
}

void func_8005B790(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk9C = (u8)a->unk8 << 8;
                c->unk9E = 0;
                c->unk11C |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk9C = (u8)a->unk8 << 8;
                c->unk9E = 0;
                c->unk11C |= 3;
            }
        }
    }
}

void func_8005B870(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unkFE = (s16)(((u8)a->unkC << 8) - c->unk9C) / n;
                c->unk9E = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unkFE = (s16)(((u8)a->unkC << 8) - c->unk9C) / n;
                c->unk9E = n;
            }
        }
    }
}

void func_8005B9E8(u8 *a) {
    u32 bit = 0x100;
    u32 mask = D_80080A70.unk0;
    u32 i;
    AkaoChannel *c = D_8007E3D8;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->unk28 & 0x2000000)) {
                c->unk9C = *a << 8;
                c->unk9E = 0;
                c->unk11C |= 3;
            }
        }
    }
}

void func_8005BA60(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = D_80080A70.unk0;
    u32 i;
    AkaoChannel *c = D_8007E3D8;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unkFE = (s16)(((u8)p->unk4 << 8) - c->unk9C) / n;
            c->unk9E = n;
        }
    }
}

void func_8005BB24(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk70 = (u8)a->unk8 << 8;
                c->unkB6 = 0;
                c->unk11C |= 0x10;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk70 = (u8)a->unk8 << 8;
                c->unkB6 = 0;
                c->unk11C |= 0x10;
            }
        }
    }
}

void func_8005BC04(Unk80080AA0 *a) {
    AkaoChannel *c = D_8007E3D8;
    u32 mask = D_80080A70.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk74 = (s16)((s16)(((u8)a->unkC << 8) - (u16)c->unk70) / n);
                c->unkB6 = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk74 = (s16)((s16)(((u8)a->unkC << 8) - (u16)c->unk70) / n);
                c->unkB6 = n;
            }
        }
    }
}

void func_8005BD8C(u8 *a) {
    s32 i;
    AkaoChannel *c = D_8007E3D8;
    for (i = 16; i != 0; i--) {
        if (!(c->unk28 & 0x2000000)) {
            c->unk70 = *a << 8;
            c->unkB6 = 0;
            c->unk11C |= 0x10;
        }
        c++;
    }
}

void func_8005BDE4(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = D_80080A70.unk0;
    u32 i;
    AkaoChannel *c = D_8007E3D8;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unk74 = (s16)((s16)(((u8)p->unk4 << 8) - (u16)c->unk70) / n);
            c->unkB6 = n;
        }
    }
}

void func_8005BEB0(s8 *a) {
    s32 v = *a;
    D_8007F96E = 0;
    D_80083154 = v << 16;
}

void func_8005BECC(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_8007F8E8 = (((s8)p->unk4 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void func_8005BF34(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk4 != 0) {
        n = p->unk0;
    }
    D_80083154 = (s8)p->unk4 << 16;
    D_8007F8E8 = (((s8)p->unk8 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void func_8005BFA8(s8 *a) {
    s32 v = *a;
    D_8007F96C = 0;
    D_80083150 = v << 16;
}

void func_8005BFC4(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_8007F8E4 = (((s8)p->unk4 << 16) - D_80083150) / n;
    D_8007F96C = n;
}

void func_8005C02C(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk4 != 0) {
        n = p->unk0;
    }
    D_80083150 = (s8)p->unk4 << 16;
    D_8007F8E4 = (((s8)p->unk8 << 16) - D_80083150) / n;
    D_8007F96C = n;
}

void func_8005C0A0(void) {
    func_800599D4(g_akaoSeq, g_akaoChannels, 0);
    if (D_8007F790 != NULL) {
        func_800599D4(D_8007F790, D_8007F71C, 0);
    }
}

void func_8005C0F0(s32 *p) {
    func_800599D4(g_akaoSeq, g_akaoChannels, *p);
    if (D_8007F790 != NULL && *p != 0) {
        func_800599D4(D_8007F790, D_8007F71C, *p);
    }
}

void func_8005C158(void) {
    AkaoChannel *c = D_8007E3D8;
    u32 bit = 0x100;
    u32 i;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((D_80080A70.unk0 & bit) && !(c->unk28 & 0x2000000)) {
            D_80080A70.unkC |= bit;
            func_8005EC20(c, bit);
            c->unk34 = 0;
        }
    }
    D_80083158.unk8 |= 0x110;
}

void func_8005C234(void) {
    D_80083158.unk4 = 1;
    func_8005A284(g_akaoSeq, g_akaoChannels);
    if (D_8007F790 != NULL) {
        func_8005A284(D_8007F790, D_8007F71C);
    }
    func_8005A2CC();
}

void func_8005C294(void) {
    D_80083158.unk4 = 2;
    func_8005A284(g_akaoSeq, g_akaoChannels);
    if (D_8007F790 != NULL) {
        func_8005A284(D_8007F790, D_8007F71C);
    }
    func_8005A2CC();
}

void func_8005C2F4(s32 *arg0) {
    u32 i;
    AkaoChannel *ch;
    D_80080AC4 = *arg0;
    ch = g_akaoChannels;
    for (i = 0; i < 0x20; i++) {
        ch->unk11C |= 3;
        ch++;
    }
}

void func_8005C334(u16 *a) {
    g_akaoSeq->unk6C = *a;
}

void func_8005C348(void) {
    u32 m;
    u32 bit;
    s32 i;
    Unk80080A10 *t;
    s32 v;
    if (g_akaoSeq->unk4 != 0) {
        m = ~(D_80080A70.unk0 | D_80083168.unkC) & 0xFFFFFF;
        if (m != 0) {
            bit = 1;
            i = 0;
            do {
                if (m & bit) {
                    func_80057640(i, 0, 0, 0);
                    func_80057688(i, 0);
                    func_80057708(i, 0x7F, 1);
                    func_80057788(i, 0x7F, 3);
                    m &= ~bit;
                }
                bit <<= 1;
                i++;
            } while (m != 0);
        }
        t = g_akaoSeq;
        v = t->unk4;
        t->unk4 = 0;
        t->unk1C = v;
    }
    D_800831C4 |= 1;
}

void func_8005C438(void) {
    AkaoChannel *ch;
    s32 bit;
    s32 mask;
    s32 t;

    t = g_akaoSeq->unk1C;
    mask = t;
    if (t != 0) {
        bit = 1;
        ch = g_akaoChannels;
        do {
            if (mask & bit) {
                mask &= ~bit;
                ch->unk11C |= 0x2B13;
            }
            bit <<= 1;
            ch++;
        } while (mask != 0);
        t = g_akaoSeq->unk4 = g_akaoSeq->unk1C;
        g_akaoSeq->unk1C = 0;
        D_80083158.unk8 |= 0x100;
    }
    D_800831C4 &= ~1;
}

void func_8005C4D4(void) {
    u32 m;
    u32 bit;
    u32 i;
    AkaoChannel *c;
    s32 v;

    if (D_80080A70.unk0 != 0) {
        m = D_80080A70.unk0;
        c = D_8007E3D8;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((m & bit) && (c->unk28 & 0x2000000)) {
                m &= ~bit;
            }
        }
        bit = 0x100;
        v = 8;
        D_80080A70.unk10 = m;
        D_80080A70.unk0 &= ~m;
        while (m != 0) {
            if (m & bit) {
                func_80057640(v, 0, 0, 0);
                func_80057688(v, 0);
                func_80057708(v, 0x7F, 1);
                func_80057788(v, 0x7F, 3);
                m &= ~bit;
            }
            bit <<= 1;
            v++;
        }
    }
    D_800831C4 |= 2;
}

void func_8005C5F0(void) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    s32 m;
    if (D_80080A80 != 0) {
        mask = D_80080A80;
        bit = 0x100;
        c = D_8007E3D8;
        do {
            if (mask & bit) {
                mask &= ~bit;
                c->unk11C |= 0x2B13;
            }
            bit <<= 1;
            c++;
        } while (mask != 0);
        m = D_80080A70.unk10;
        D_80080A70.unk10 = 0;
        D_80080A70.unk0 |= m;
        D_80083158.unk8 |= 0x100;
    }
    D_800831C4 &= ~2;
}

void func_8005C684(void) {
    if (D_80083168.unkC != 0) {
        func_80057688(D_80083168.unk10, 0);
        func_80057688(D_80083168.unk10 + 1, 0);
        func_80057640(D_80083168.unk10, 0, 0, 0);
        func_80057640(D_80083168.unk10 + 1, 0, 0, 0);
    }
}

void func_8005C700(void) {
    Unk80083168 *d = &D_80083168;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    if (d->unkC == 0) {
        return;
    }
    func_80057688(d->unk10, d->unk58);
    func_80057688(d->unk10 + 1, d->unk58);
    if (D_8008315C & 2) {
        v = d->unk40 * D_8006F640 >> 16;
        func_80057640(d->unk10, v, v, 0);
        func_80057640(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v = d->unk40;
        v <<= 15;
        v >>= 16;
        func_80057640(d->unk10, v, 0, 0);
        func_80057640(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (u8)(d->unk4C >> 8);
        l = d->unk40 * D_8006F540[pan] >> 16;
        r = d->unk40 * D_8006F540[pan ^ 0xFF] >> 16;
        func_80057640(d->unk10, l, r, 0);
        func_80057640(d->unk10 + 1, l, r, 0);
    }
}

void func_8005C864(void) {
}

void func_8005C86C(s32 mode) {
    s32 t;
    func_80018750(&t);
    if (t != mode) {
        SpuSetReverb(0);
        SpuSetReverbModeType(mode | 0x100);
        SpuSetReverb(1);
    }
}

s32 akao_command(u32 cmd) {
    s32 r = 0;
    AkaoSeqHeader *seq;
    Unk80080AA0 *blk;

    DisableEvent(g_akaoTickEvent);
    blk = &g_akaoCommandBlock;
    cmd &= 0xFF;
    switch (cmd) {
    case 0x10:
    case 0x12:
    case 0x14:
    case 0x19:
    case 0x1A:
        if (func_800565DC((s32 *)D_80080AA0.unk0) == 0) {
        seq = (AkaoSeqHeader *)D_80080AA0.unk0;
        if (g_akaoSeq->unk6A != seq->unk4 || (D_8007F790 != NULL && D_8007F790->unk6A != g_akaoSeq->unk6A)) {
            if (cmd != 0x19) {
                func_8005C86C(seq->unk8);
            }
            blk->unk0 = (s32)seq;
            blk->unk8 = seq->unk4;
            switch (cmd) {
            case 0x14:
                blk->unkC = D_80080AA0.unk4 != 0 ? D_80080AA0.unk4 | 1 : -1;
                break;
            case 0x12:
                blk->unkC = D_80080AA0.unk4;
                break;
            case 0x19:
                blk->unk10 = D_80080AA0.unk4;
                break;
            case 0x1A:
                blk->unk10 = D_80080AA0.unk4;
                blk->unk14 = D_80080AA0.unk8;
                break;
            }
            r = seq->unk4;
        } else {
            cmd = 0;
            r = 0;
        }
        } else {
            cmd = 0;
            r = -1;
        }
        break;
    case 0xD8:
        blk->unk0 = D_80080AA0.unk0;
        g_akaoCommandHandlers[0xD0](blk);
        cmd = 0xD4;
        break;
    case 0xD9:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        g_akaoCommandHandlers[0xD1](blk);
        cmd = 0xD5;
        break;
    case 0xDA:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        blk->unk8 = D_80080AA0.unk8;
        g_akaoCommandHandlers[0xD2](blk);
        cmd = 0xD6;
        break;
    case 0x99:
        g_akaoCommandHandlers[0x9B](blk);
        g_akaoCommandHandlers[0x9D](blk);
        cmd = 0x9F;
        break;
    case 0x98:
        g_akaoCommandHandlers[0x9A](blk);
        g_akaoCommandHandlers[0x9C](blk);
        cmd = 0x9E;
        break;
    default:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        blk->unk8 = D_80080AA0.unk8;
        blk->unkC = D_80080AA0.unkC;
        blk->unk10 = D_80080AA0.unk10;
        blk->unk14 = D_80080AA0.unk14;
        break;
    }
    g_akaoCommandHandlers[cmd](blk);
    EnableEvent(g_akaoTickEvent);
    return r;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetReverbModeType);

void func_8005CDE4(void) {
    s32 v = D_80080AC2;
    SPU_REG16(0x1F801DB0) = v;
    SPU_REG16(0x1F801DB2) = v;
}

void func_8005CE04(u32 *src, u32 *dst, u32 n) {
    u32 t0, t1, t2, t3;
    for (n >>= 2; n & ~3; n -= 4) {
        t0 = src[0];
        t1 = src[1];
        t2 = src[2];
        t3 = src[3];
        dst[0] = t0;
        dst[1] = t1;
        dst[2] = t2;
        dst[3] = t3;
        src += 4;
        dst += 4;
    }
    for (; n != 0; n--) {
        *dst++ = *src++;
    }
}

void func_8005CE8C(u32 *a, u32 *b, u32 n) {
    u32 t0, t1, t2, t3;
    for (n >>= 2; n >> 1; n -= 2) {
        t0 = a[0];
        t1 = b[0];
        t2 = a[1];
        t3 = b[1];
        b[0] = t0;
        b[1] = t2;
        a[0] = t1;
        a[1] = t3;
        a += 2;
        b += 2;
    }
    if (n != 0) {
        t0 = *a;
        t1 = *b;
        *b = t0;
        *a = t1;
    }
}

void func_8005CF04(void) {
    s32 v;
    AkaoChannel *c;
    u32 mask;
    Unk80083168 *s;
    s32 old;
    s32 k;

    D_8008314C = (D_8008314C + 1) & 0xFF;
    if (D_80080A2C != 0) {
        D_80080A2C--;
        D_80080AC0 += D_80080A28;
        func_8005CDE4();
    }
    s = &D_80083168;
    if (s->unkC != 0 && s->unk48 != 0) {
        s->unk48--;
        old = s->unk40;
        v = old + s->unk44;
        if ((v & 0xFF00) != (old & 0xFF00)) {
            if (D_8008315C & 2) {
                old = (old * D_8006F640) >> 16;
                func_80057640(s->unk10, old, old, 0);
                func_80057640(s->unk10 + 1, old, old, 0);
            } else {
                old = v << 15;
                old >>= 16;
                func_80057640(s->unk10, old, 0, 0);
                func_80057640(s->unk10 + 1, 0, old, 0);
            }
        }
        D_800831A8 = v & 0xFFFF;
    }
    if (D_8007F96E != 0) {
        D_8007F96E--;
        D_80083154 += D_8007F8E8;
    }
    if (D_8007F96C != 0) {
        D_8007F96C--;
        v = D_80083150 + D_8007F8E4;
        if ((v & 0xFF0000) != (D_80083150 & 0xFF0000)) {
            k = 32;
            c = g_akaoChannels;
            do {
                k--;
                c->unk11C |= 0x10;
                c++;
            } while (k != 0);
        }
        D_80083150 = v;
    }
    if (g_akaoSeq->unk4 != 0) {
        if (g_akaoSeq->unk58 != 0) {
            g_akaoSeq->unk58--;
            v = g_akaoSeq->unk50 + g_akaoSeq->unk54;
            if ((v & 0x7F0000) != (g_akaoSeq->unk50 & 0x7F0000)) {
                func_8005A284(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->unk50 = v;
        }
        if (g_akaoSeq->unk64 != 0) {
            g_akaoSeq->unk64--;
            v = g_akaoSeq->unk5C + g_akaoSeq->unk60;
            if ((v & 0x7F0000) != (g_akaoSeq->unk5C & 0x7F0000)) {
                func_8005A284(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->unk5C = v;
        }
    }
    if (D_8007F790 != NULL && D_8007F790->unk4 != 0) {
        if (D_8007F790->unk58 != 0) {
            D_8007F790->unk58--;
            v = D_8007F790->unk50 + D_8007F790->unk54;
            if ((v & 0x7F0000) != (D_8007F790->unk50 & 0x7F0000)) {
                func_8005A284(D_8007F790, D_8007F71C);
            }
            D_8007F790->unk50 = v;
        }
        if (D_8007F790->unk64 != 0) {
            D_8007F790->unk58--;
            v = D_8007F790->unk5C + D_8007F790->unk60;
            if ((v & 0x7F0000) != (D_8007F790->unk5C & 0x7F0000)) {
                func_8005A284(D_8007F790, D_8007F71C);
            }
            D_8007F790->unk5C = v;
        }
    }
    if (D_80080A70.unk0 != 0) {
        mask = D_80080A70.unk0;
        c = D_8007E3D8;
        k = 0x100;
        do {
            if (mask & k) {
                if ((u16)c->unkBC != 0) {
                    c->unkBC = (u16)c->unkBC - 1;
                    v = (s16)c->unk100 + c->unk102;
                    if ((v & 0xFF00) != ((s16)c->unk100 & 0xFF00)) {
                        c->unk11C |= 3;
                    }
                    c->unk100 = v;
                }
                if (c->unk9E != 0) {
                    c->unk9E--;
                    v = c->unk9C + c->unkFE;
                    if ((v & 0xFF00) != (c->unk9C & 0xFF00)) {
                        c->unk11C |= 3;
                    }
                    c->unk9C = v;
                }
                if ((u16)c->unkB6 != 0) {
                    c->unkB6 = (u16)c->unkB6 - 1;
                    v = c->unk70 + c->unk74;
                    if ((v & 0xFF00) != (c->unk70 & 0xFF00)) {
                        c->unk11C |= 0x10;
                    }
                    c->unk70 = v;
                }
                /* identical branches (jump2 merges them), as in func_80030EE4 */
                if (v) {
                    mask ^= k;
                } else {
                    mask ^= k;
                }
            }
            k <<= 1;
            c++;
        } while (mask != 0);
    }
}

u32 func_8005D384(AkaoChannel *chans, s32 sub) {
    u32 tempo;
    u32 m;
    s32 acc;
    AkaoChannel *c;
    u32 bit;
    u32 active;
    s32 flags;

    tempo = g_akaoSeq->unk20.h[1];
    m = D_80083156;
    if (m != 0) {
        if (m < 0x80) {
            tempo += (tempo * m) >> 7;
        } else {
            tempo = (tempo * m) >> 8;
        }
    }
    acc = g_akaoSeq->unk28 + tempo;
    g_akaoSeq->unk28 = acc;
    if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
        g_akaoSeq->unk28 = acc & 0xFFFF;
        do {
            for (;;) {
                c = chans;
                bit = 1;
                active = g_akaoSeq->unk4;
                do {
                    if (active & bit) {
                        c->unk96--;
                        c->unk98--;
                        if (c->unk96 == 0) {
                            func_8005E3FC(c, bit);
                        } else if (c->unk98 == 0) {
                            g_akaoSeq->unk18 |= bit;
                        }
                        func_800579B4(c, bit, 0);
                        active &= ~bit;
                    }
                    c++;
                    bit <<= 1;
                } while (active != 0);
                if (g_akaoSeq->unk68 != 0) {
                    g_akaoSeq->unk68--;
                    g_akaoSeq->unk20.w += g_akaoSeq->unk24;
                }
                if (g_akaoSeq->unk66 != 0) {
                    g_akaoSeq->unk66--;
                    g_akaoSeq->unk48 += g_akaoSeq->unk4C;
                    flags = D_80083158.unk8;
                    flags |= 0x80;
                    if (sub == 0) {
                        D_80083158.unk8 = flags;
                    }
                }
                if (g_akaoSeq->unk74 != 0) {
                    if (++g_akaoSeq->unk76 == g_akaoSeq->unk74) {
                        g_akaoSeq->unk76 = 0;
                        if (++g_akaoSeq->unk72 == g_akaoSeq->unk70) {
                            g_akaoSeq->unk72 = 0;
                            g_akaoSeq->unk78++;
                            if (sub == 0 && D_800809B8 != 0) {
                                D_800809B8--;
                            }
                        }
                    }
                }
                if (sub != 0 || D_800809B8 == 0) {
                    break;
                }
            }
        } while (0);
    }
    return g_akaoSeq->unk4;
}

void func_8005D628(Unk80080A10 *arg0, AkaoChannel *arg1) {
    s32 t;

    if (D_80080A18.unk8 != 0) {
        if (--D_80080A18.unk8 == 0) {
            s32 v = arg0->unk4;
            arg0->unk6A = 0;
            arg0->unk4 = 0;
            arg0->unk10 = 0;
            arg0->unk14 = 0;
            arg0->unk18 = v;
            return;
        }
        t = D_80080A18.unk0 + D_80080A18.unk4;
        if ((t & 0xFFFF0000) != (D_80080A18.unk0 & 0xFFFF0000)) {
            func_8005A284(arg0, arg1, arg0);
        }
        D_80080A18.unk0 = t;
    }
}

s32 akao_tick(void) {
    s32 t0;
    s32 n;
    u32 now;
    Unk80080A10 *tmp;
    AkaoChannel *c;
    u32 mask;
    u32 bit;
    u32 acc;
    s32 a;
    s32 b;
    s32 e;

    t0 = GetRCnt(0xF2000002);
    D_8007E3C8[1] = D_8007E3C8[0];
    now = VSync(-1);
    n = 1;
    mask = now - D_8007E3C8[1];
    D_8007E3C8[0] = now;
    if (mask != 0) {
        if (mask < 9) {
            bit = mask * 4;
            if (D_8007E3D0 < bit) {
                n = bit - D_8007E3D0 + 1;
            }
            D_8007E3D0 %= bit;
        }
    }
    do {
        n--;
        D_8007E3D0++;
        g_akaoTickCount++;
        if ((g_akaoSeq->unk18 | D_80080A70.unkC) != 0 || (D_8007F790 != NULL && D_8007F790->unk18 != 0)) {
            func_80058F94();
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 == 0) {
                D_8007F790 = NULL;
            } else if ((g_akaoSeq->unk4 | g_akaoSeq->unk1C) == 0) {
                func_8005CE04(D_8007F790, g_akaoSeq, 0x7C);
                func_8005CE04(D_8007F71C, g_akaoChannels, 0x2680);
                tmp = D_8007F790;
                D_8007F790 = NULL;
                tmp->unk6A = 0;
                tmp->unk4 = 0;
            }
        }
        if ((D_80083158.unk8 | g_akaoSeq->unk14 | D_80080A70.unk8) != 0 || (D_8007F790 != NULL && D_8007F790->unk14 != 0)) {
            func_800589F0();
        }
        if (g_akaoSeq->unk4 != 0) {
            func_8005D384(g_akaoChannels, 0);
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 != 0) {
                g_akaoSeq = D_8007F790;
                func_8005D384(D_8007F71C, 1);
                g_akaoSeq = &g_akaoMainSeq;
            }
            if ((D_80083158.unk4 & 0x100) && !(g_akaoTickCount & 3)) {
                func_8005D628(D_8007F790, D_8007F71C);
                if (D_8007F790->unk4 == 0) {
                    D_80083158.unk4 &= ~0x100;
                }
            }
        }
        if (D_80080A70.unk0 != 0) {
            mask = D_80080A70.unk0;
            acc = D_80080A70.unk18 + D_80080A70.unk14.h[1];
            D_80080A70.unk18 = acc;
            if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
                D_80080A70.unk18 = acc & 0xFFFF;
                bit = 0x100;
                c = D_8007E3D8;
                do {
                    if (mask & bit) {
                        if (!(D_800831C4 & 2) || (c->unk28 & 0x2000000)) {
                            c->unk88++;
                            c->unk96--;
                            c->unk98--;
                            if (c->unk96 == 0) {
                                func_8005E3FC(c, bit);
                            } else if (c->unk98 == 0) {
                                D_80080A70.unkC |= bit;
                                D_80080A70.unk8 &= ~bit;
                            }
                            func_800579B4(c, bit, 1);
                        }
                        mask ^= bit;
                    }
                    c++;
                    bit <<= 1;
                } while (mask != 0);
            }
        }
        if (!(g_akaoTickCount & 3)) {
            func_8005CF04();
        }
    } while (n != 0);
    t0 = GetRCnt(0xF2000002) - t0;
    if (t0 <= 0) {
        t0 += 0x44E8;
    }
    a = g_akaoTickTimes[1];
    b = g_akaoTickTimes[2];
    e = g_akaoTickTimes[3];
    g_akaoTickTimes[3] = t0;
    g_akaoTickTimes[0] = a;
    g_akaoTickTimes[1] = b;
    g_akaoTickTimes[2] = e;
    g_akaoTickTimeSum = a + b + e + t0;
    return t0;
}

/* Looks ahead in the channel's sequence for the next note opcode (0xA0: rest or end). */
s32 func_8005DB40(AkaoChannel *c) {
    u8 *p;
    u32 n;
    u32 op;
    s32 len;

    p = c->unk0;
    n = c->unkF4;
    for (;;) {
        if (*p < 0x9A) {
            if (*p >= 0x8F) {
                c->unkCC = (u16)c->unkCC & ~5;
            }
            return *p;
        }
        if (*p < 0xA0) {
            return 0xA0;
        }
        op = *p;
        len = D_8006F374[op - 0xA0];
        if (len != 0) {
            p += len;
            continue;
        }
        switch (op) {
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
            return 0x83;
        case 0xFC:
            return 0x84;
        case 0xFD:
            return 0x8F;
        case 0xFE:
            p++;
            op = *p;
            len = D_8006F3D4[op];
            if (len != 0) {
                p += len;
                continue;
            }
            switch (op) {
            case 0x8:
            case 0x9:
                p++;
                if (*p == c->unkA2[n] + 1) {
                    p++;
                    n--;
                    n &= 3;
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 3;
                continue;
            case 0x6:
            case 0xE:
                p++;
                op = p[0] + (p[1] << 8);
                p += (s16)op;
                continue;
            case 0x7:
                p++;
                if (*p++ <= g_akaoSeq->unk6C) {
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 2;
                continue;
            case 0xF:
                p = c->unk14;
                continue;
            }
            continue;
        case 0xC9:
            p++;
            if (*p == c->unkA2[n] + 1) {
                p++;
                n--;
                n &= 3;
                continue;
            }
            p = c->unk4[n];
            continue;
        case 0xCB:
        case 0xCD:
        case 0xD1:
        case 0xDB:
            c->unkCC = (u16)c->unkCC & ~5;
            p++;
            continue;
        case 0xCA:
            if (c->unk34 & 0x200000) {
                goto end;
            }
            p = c->unk4[n];
            continue;
        default:
        end:
            c->unkCC = (u16)c->unkCC & ~5;
            return 0xA0;
        }
    }
}

s32 func_8005DD54(s32 a, s32 b) {
    if ((a & 0x20) && (u32)(b - 0x20) < 0x60) {
        return b + 0x30;
    }
    return b;
}

void func_8005DD7C(AkaoChannel *c, u32 key) {
    AkaoRegion *e;
    Unk8005EBB4 *p;
    u32 f;
    s32 v;

    if (c->unk10A < key || c->unk10A == 0xFF) {
        e = (AkaoRegion *)c->unk18;
        while (e[1].unk5 != 0 && e->unk2 < key) {
            e++;
        }
    } else if (key < c->unk10A) {
        e = (AkaoRegion *)c->unk18;
        while (e[1].unk5 != 0 && !(key < e[1].unk1)) {
            e++;
        }
    } else {
        return;
    }
    f = c->unk34;
    v = func_8005DD54(g_akaoSeq->unk0, e->unk0);
    c->unk9A = v;
    p = &D_8007F970[v];
    c->unk120 = p->unk0;
    c->unk124 = p->unk4;
    if (!(f & 0x1000000)) {
        c->unk12A = e->unk3 << 8;
    } else {
        c->unk12A &= 0x7F00;
    }
    c->unk12A |= p->unkC & 0x80FF;
    if (!(f & 0x8000000)) {
        c->unk12C &= 0x201F;
        c->unk12C |= e->unk4 << 6;
    } else {
        c->unk12C &= 0x3FDF;
    }
    switch (e->unk5) {
    case 3:
        c->unk12C |= 0x4000;
        break;
    case 5:
        c->unk12C |= 0x8000;
        break;
    case 7:
        c->unk12C |= 0xC000;
        break;
    }
    if (!(f & 0x10000000)) {
        c->unk12C &= 0xFFE0;
        c->unk12C |= e->unk6;
    }
    c->unk12C |= p->unkE & 0x20;
    c->unk12E = e->unk7;
}

u16 func_8005DFE0(Unk8005EBB4 *p, s32 note, u8 fine, s32 *out) {
    s32 n = note - p->unkA;
    s32 freq;
    u32 f;

    while (n < 0) {
        n += 12;
    }
    n %= 12;
    if (p->unk8 == 0) {
        freq = D_8006F410[n] << 8;
    } else if (p->unk8 < 0) {
        freq = (D_8006F410[n] * (u16)p->unk8) >> 8;
    } else {
        freq = (D_8006F410[n] * p->unk8) >> 7;
        freq += D_8006F410[n] << 8;
    }
    f = fine;
    if (f != 0) {
        if (f < 0x80) {
            *out = ((u32)freq * f) >> 7;
        } else {
            *out = (((u32)freq * f) >> 8) - freq;
        }
    }
    if (note < p->unkA) {
        do {
            *out >>= 1;
            freq >>= 1;
        } while ((note += 12) < p->unkA);
    } else {
        n = (note - p->unkA) / 12;
        if (n != 0) {
            freq <<= n;
            *out <<= n;
        }
    }
    *out >>= 8;
    freq >>= 8;
    return freq;
}

s32 func_8005E170(AkaoChannel *c, u32 mask, s32 key) {
    AkaoDrumKey *e;
    Unk8005EBB4 *p;
    u32 f;
    s32 v;
    s32 r;

    e = (AkaoDrumKey *)g_akaoSeq->unk34;
    e += key;
    g_akaoSeq->unk10 |= mask;
    if (g_akaoSeq->unk14 & mask) {
        g_akaoSeq->unk18 |= mask;
    }
    f = c->unk34;
    v = func_8005DD54(g_akaoSeq->unk0, e->unk0);
    c->unk9A = v;
    p = &D_8007F970[v];
    c->unk120 = p->unk0;
    c->unk124 = p->unk4;
    if (!(f & 0x1000000)) {
        c->unk12A = e->unk2 << 8;
    } else {
        c->unk12A &= 0x7F00;
    }
    c->unk12A |= p->unkC & 0x80FF;
    if (!(f & 0x8000000)) {
        c->unk12C &= 0x201F;
        c->unk12C |= e->unk3 << 6;
    } else {
        c->unk12C &= 0x3FDF;
    }
    switch (e->unk4) {
    case 3:
        c->unk12C |= 0x4000;
        break;
    case 5:
        c->unk12C |= 0x8000;
        break;
    case 7:
        c->unk12C |= 0xC000;
        break;
    }
    if (!(f & 0x10000000)) {
        c->unk12C &= 0xFFE0;
        c->unk12C |= e->unk5;
    }
    c->unk12C |= p->unkE & 0x20;
    r = ((s32 (*)())func_8005DFE0)(p, e->unk1, c->unk108, &c->unk84);
    c->unk12E = e->unk6;
    c->unkBE = ((e->unk7 & 0x7F) + 0x40) << 8;
    if (e->unk7 & 0x80) {
        g_akaoSeq->unk40 |= mask;
    } else {
        g_akaoSeq->unk40 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
    return r;
}

void func_8005E3FC(AkaoChannel *c, u32 mask) {
    u32 op;
    u32 vel;
    u32 tmp;
    u32 f;
    u16 len;
    u32 v;

    do {
        op = *c->unk0++;
        if (op >= 0xA0) {
            if (op == 0xFE) {
                vel = *c->unk0++;
                D_8006FCE0[vel](c, mask);
            } else if (op - 0xF0 < 14) {
                op = (op - 0xF0) * 11;
                c->unk96 = *c->unk0++;
            } else {
                if (op == 0xFF) {
                    op = 0xA0;
                } else if (op == 0xCA && (c->unk34 & 0x200000)) {
                    op = 0xA0;
                    D_80080A70.unkC |= mask;
                }
                D_8006FB60[op - 0xA0](c, mask);
            }
        }
        c->unkA0++;
    } while (op > 0xA0);
    if (op == 0xA0) {
        if (c->unk94 == 0) {
            g_akaoSeq->unk18 |= mask;
        }
        return;
    }
    vel = func_8005DB40(c) & 0xFF;
    if (c->unkFA != 0) {
        c->unk96 = c->unk98 = c->unkFA;
    }
    if (c->unk96 != 0) {
        if (vel >= 0x8F || (vel < 0x84 && !(c->unkCC & 5))) {
            c->unk98 -= 2;
        }
    } else {
        c->unk96 = D_8006F3F4[op % 11];
        len = c->unk96;
        if ((vel < 0x84 || vel >= 0x8F) && !(c->unkCC & 5)) {
            len -= 2;
        }
        c->unk98 = len;
    }
    if (c->unk94 == 0 && (c->unk34 & 0x40)) {
        c->unk98 = c->unk96;
    }
    c->unkF8 = c->unk96;
    c->unk11C |= 0x4000;
    if (op >= 0x8F) {
        if (c->unk94 == 0) {
            g_akaoSeq->unk14 &= ~mask;
            if (c->unk118 < 0x18) {
                g_akaoSeq->unk18 |= mask;
            }
        }
        c->unkCA = 0;
        c->unk110 = 0;
        c->unk112 = 0;
        c->unkCC &= ~2;
        return;
    }
    if (op < 0x84) {
        op /= 11;
        op += c->unkC4 * 12;
        f = c->unk34;
        if (f & 8) {
            vel = func_8005E170(c, mask, op);
        } else {
            if (!(c->unkCC & 2)) {
                if (c->unk94 == 0) {
                    if (f & 0x1000) {
                        func_8005DD7C(c, op);
                    }
                    g_akaoSeq->unk10 |= mask;
                    if ((g_akaoSeq->unk14 & mask) && c->unk118 < 0x18) {
                        g_akaoSeq->unk18 |= mask;
                    }
                    if (c->unkBA != 0) {
                        c->unkB8 = c->unkBA;
                        c->unk78 = c->unk8C;
                        c->unk7C = c->unk90;
                    }
                } else {
                    D_80080A70.unk4 |= mask;
                }
                c->unkC2 = 0;
            }
            if (c->unkCA != 0 && c->unkC8 != 0) {
                c->unkC6 = c->unkCA;
                c->unk10C = c->unk106 + op - c->unkC8 - c->unk10E;
                c->unk10A = c->unkC8 - (c->unk106 - c->unk10E);
                op = c->unkC8 + c->unk10E;
            } else {
                c->unk10A = op;
                op += c->unk106;
            }
            vel = ((s32 (*)())func_8005DFE0)(&D_8007F970[c->unk9A], op, c->unk108, &c->unk84);
            if (c->unkF6 != 0) {
                tmp = (u32)(vel * c->unkF6) >> 8;
                tmp *= (u8)D_8006F440[D_8008314C];
                if ((u8)D_8006F440[D_8008314C] & 0x80) {
                    tmp >>= 9;
                    vel -= tmp;
                } else {
                    tmp >>= 7;
                    vel += tmp;
                }
            }
        }
        c->unk2C = vel;
        if (c->unk94 == 0) {
            g_akaoSeq->unk14 |= mask;
        } else {
            D_80080A70.unk8 |= mask;
        }
        op = c->unk34;
        c->unk11C |= 0x13;
        if (op & 1) {
            v = (c->unkD6 & 0x7F00) >> 8;
            if (!(c->unkD6 & 0x8000)) {
                v = (v * ((u32)(vel * 15) >> 8)) >> 7;
            } else {
                v = (v * vel) >> 7;
            }
            c->unkD4 = v;
            if (!(c->unkCC & 2)) {
                c->unk44 = 0;
                c->unkD0 = c->unkCE;
                c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
            }
        }
        if ((op & 2) && !(c->unkCC & 2)) {
            c->unk54 = 0;
            c->unkDE = c->unkDC;
            c->unk58 = 0x1000000 / ((u32)c->unk4C >> 12);
        }
        c->unk110 = 0;
        c->unk112 = 0;
        c->unk30 = 0;
    }
    c->unkCC = (c->unkCC & ~2) | ((c->unkCC & 1) << 1);
    if (c->unk10C != 0) {
        c->unk10A += c->unk10C;
        vel = ((s32 (*)())func_8005DFE0)(&D_8007F970[c->unk9A], c->unk10A + c->unk106, c->unk108, &tmp) << 16;
        c->unkC2 = c->unkC6;
        c->unk10C = 0;
        c->unk80 = (s32)(vel - ((c->unk2C << 16) + c->unk30)) / (u16)c->unkC2;
    }
    c->unkC8 = c->unk10A;
    c->unk10E = c->unk106;
}

s32 func_8005EB40(s32 *p, s32 target, s32 steps, s32 shift) {
    s32 mask = (1 << shift) - 1;
    s32 cur = *p & ~mask;
    s32 step;
    target <<= shift;
    step = target - cur;
    step = step / steps;
    *p = cur;
    if (target < cur) {
        *p = cur | mask;
        step--;
    } else {
        step++;
    }
    return step;
}

void func_8005EBB4(AkaoChannel *ch, Unk8005EBB4 *p, s32 arg2) {
    ch->unk120 = arg2;
    ch->unk124 = p->unk4;
    ch->unk12A = p->unkC;
    ch->unk12C = p->unkE;
    ch->unk11C |= 0x1FF80;
}

void func_8005EBE8(AkaoChannel *c, s32 i) {
    c->unk9A = i;
    func_8005EBB4(c, &D_8007F970[i], D_8007F970[i].unk0);
}

void func_8005EC20(AkaoChannel *c, u32 mask) {
    D_80080A70.unk0 &= ~mask;
    D_80080A70.unk10 &= ~mask;
    D_80080A70.unk1C &= ~mask;
    D_80080A70.unk20 &= ~mask;
    D_80080A70.unk24 &= ~mask;
    D_80080A70.unk4 &= ~mask;
    D_80080A70.unk8 &= ~mask;
    c->unk28 = 0;
    c->unk6C = 0;
}

s32 func_8005EC8C(s32 a, s32 b) {
    if (a != 0) {
        if ((u32)(b - 0xC0) < 0x20) {
            return b + a * 8;
        }
        if ((u32)(b - 0xE0) < 0x18) {
            return b + (a - 4) * 8;
        }
    }
    return b;
}

void func_8005ECCC(AkaoChannel *c, s32 mask) {
    Unk80080A10 *t;

    if (c->unk94 == 0) {
        t = g_akaoSeq;
        if ((t->unk4 &= ~mask) == 0) {
            D_800809B8 = 0;
            t->unk6A = 0;
            t->unk0 = 0;
        }
        g_akaoSeq->unk14 &= ~mask;
        g_akaoSeq->unk8 &= ~mask;
        g_akaoSeq->unkC &= ~mask;
        g_akaoSeq->unk3C &= ~mask;
        g_akaoSeq->unk40 &= ~mask;
        g_akaoSeq->unk44 &= ~mask;
    } else {
        func_8005EC20(c, mask);
    }
    c->unk34 = 0;
    D_80083158.unk8 |= 0x110;
}

void func_8005EDA0(AkaoChannel *c) {
    Unk80080A10 *a = g_akaoSeq;
    s32 v = c->unk0[0] << 16;
    a->unk20.w = v;
    a->unk20.w = v | (c->unk0[1] << 24);
    c->unk0 += 2;
    a->unk68 = 0;
}

void func_8005EDEC(AkaoChannel *c) {
    s32 x;
    s32 m;

    if ((g_akaoSeq->unk68 = *c->unk0++) == 0) {
        g_akaoSeq->unk68 = 0x100;
    }
    x = (c->unk0[0] << 16) | (c->unk0[1] << 24);
    c->unk0 += 2;
    m = g_akaoSeq->unk20.w & 0xFFFF0000;
    g_akaoSeq->unk24 = (x - m) / g_akaoSeq->unk68;
    g_akaoSeq->unk20.w = m;
}

void func_8005EE8C(AkaoChannel *c) {
    Unk80080A10 *a = g_akaoSeq;
    u8 *p = c->unk0;
    s32 hi = (s8)p[1];
    s32 lo = p[0];
    s32 v;
    c->unk0 = p + 2;
    v = hi << 20;
    v |= lo << 12;
    a->unk66 = 0;
    D_80083158.unk8 |= 0x80;
    a->unk48 = v;
}

void func_8005EED4(AkaoChannel *arg0) {
    u8 *p;
    s32 v;

    g_akaoSeq->unk66 = *arg0->unk0++;
    if (g_akaoSeq->unk66 == 0) {
        g_akaoSeq->unk66 = 0x100;
    }
    p = arg0->unk0;
    v = ((s8)p[1] << 20) | (p[0] << 12);
    arg0->unk0 = p + 2;
    g_akaoSeq->unk48 &= ~0xFFF;
    g_akaoSeq->unk4C = (v - g_akaoSeq->unk48) / g_akaoSeq->unk66;
}

void func_8005EF7C(AkaoChannel *c) {
    u8 *p = c->unk0;
    c->unk0 = p + (s16)(p[0] | (p[1] << 8));
}

void func_8005EFA8(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (g_akaoSeq->unk6C >= v) {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
    } else {
        c->unk0 = p + 2;
    }
}

void func_8005F000(AkaoChannel *c) {
    u8 *p = c->unk0;
    s16 off = p[0] | (p[1] << 8);
    c->unk14 = p + 2;
    c->unk0 += off;
}

void func_8005F038(Unk8005F038 *a) {
    a->unk0 = a->unk14;
}

void func_8005F044(AkaoChannel *c) {
    c->unkB2 = *c->unk0++ << 8;
    c->unk11C |= 3;
}

void func_8005F06C(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->unk0++;
    ch->unkB4 = v;
    if (v == 0) {
        ch->unkB4 = 0x100;
    }
    p = ch->unk0;
    base = ch->unkB2 & 0x7F00;
    ch->unkFC = ((*p << 8) - base) / ch->unkB4;
    ch->unk0 = p + 1;
    v = base;
    ch->unkB2 = v;
}

void func_8005F0F0(AkaoChannel *c) {
    s32 v = (s8)*c->unk0++;
    c->unkB8 = 0;
    c->unk11C |= 3;
    c->unkBA = 0;
    c->unk78 = v << 23;
}

void func_8005F120(AkaoChannel *c) {
    s32 v;

    if ((c->unkB8 = *c->unk0++) == 0) {
        c->unkB8 = 0x100;
    }
    v = c->unk78 & 0xFFFF0000;
    c->unk7C = (((s8)*c->unk0++ << 23) - v) / (u16)c->unkB8;
    c->unk78 = v;
    c->unkBA = 0;
}

void func_8005F1AC(AkaoChannel *c) {
    c->unk8C = (s8)*c->unk0++ << 23;
    if ((c->unkBA = *c->unk0++) == 0) {
        c->unkBA = 0x100;
    }
    c->unk90 = (((s8)*c->unk0++ << 23) - c->unk8C) / (u16)c->unkBA;
}

void func_8005F23C(AkaoChannel *c) {
    c->unk34 |= 0x40;
}

void func_8005F250(AkaoChannel *c) {
    c->unk34 &= ~0x40;
}

void func_8005F264(AkaoChannel *ch) {
    u8 *p = ch->unk0;
    s32 v = *p;
    ch->unk0 = p + 1;
    ch->unkC0 = 0;
    ch->unk11C |= 3;
    ch->unkBE = ((v + 0x40) & 0xFF) << 8;
}

void func_8005F298(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->unk0++;
    ch->unkC0 = v;
    if (v == 0) {
        ch->unkC0 = 0x100;
    }
    p = ch->unk0;
    base = ch->unkBE & 0xFF00;
    ch->unk104 = ((((*p + 0x40) & 0xFF) << 8) - base) / ch->unkC0;
    ch->unk0 = p + 1;
    v = base;
    ch->unkBE = v;
}

void func_8005F324(AkaoChannel *c) {
    c->unkC4 = *c->unk0++;
}

void func_8005F340(AkaoChannel *c) {
    c->unkC4 = (c->unkC4 + 1) & 0xF;
}

void func_8005F358(AkaoChannel *c) {
    c->unkC4 = (c->unkC4 - 1) & 0xF;
}

void func_8005F370(AkaoChannel *arg0) {
    s32 i;
    Unk8005EBB4 *e;
    s32 b = *arg0->unk0++;

    if (arg0->unk94 == 0) {
        i = func_8005DD54(g_akaoSeq->unk0, b);
    } else {
        i = func_8005EC8C(arg0->unk38, b);
    }
    e = &D_8007F970[i];
    func_8005EBB4(arg0, e, e->unk0);
    arg0->unk9A = i;
    arg0->unk12E = 0;
    arg0->unk34 &= 0xE6FFEFF7;
}

void func_8005F418(AkaoChannel *c) {
    s32 i = func_8005DD54(g_akaoSeq->unk0, *c->unk0++);
    func_8005EBB4(c, &D_8007F970[i], 0x1010);
    c->unk9A = i;
    c->unk12E = 0;
    c->unk34 &= 0xE6FFEFF7;
}

void func_8005F4A0(AkaoChannel *c) {
    u8 v = *c->unk0++;
    u16 *tbl = g_akaoSeq->unk30;
    if (tbl != NULL) {
        if (tbl[v] > 0x8000) {
            c->unk12E = 0;
            c->unk34 &= ~0x1000;
        } else {
            u8 *p = (u8 *)tbl + tbl[v];
            c->unk10A = 0xFF;
            c->unk18 = p + 0x20;
            c->unk34 = (c->unk34 & 0xE6FFEFF7) | 0x1000;
        }
    }
}

void func_8005F534(AkaoChannel *c) {
    Unk8005EBB4 *e = &D_8007F970[c->unk9A];
    c->unk12A = e->unkC;
    c->unk12C = e->unkE;
    c->unk11C |= 0xFF00;
    c->unk34 &= 0xE6FFFFFF;
}

void func_8005F57C(AkaoChannel *c) {
    c->unk106 = (s8)*c->unk0++;
}

void func_8005F5A0(AkaoChannel *c) {
    c->unk106 += (s8)*c->unk0++;
}

void func_8005F5CC(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkC6 = v;
    if (v == 0) {
        c->unkC6 = 0x100;
    }
    c->unk10C = (s8)*c->unk0++;
}

void func_8005F614(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkCA = v;
    if (v == 0) {
        c->unkCA = 0x100;
    }
    c->unk10E = 0;
    c->unkC8 = 0;
    c->unkCC = 1;
}

void func_8005F64C(AkaoChannel *a) {
    a->unkCA = 0;
}

void func_8005F654(AkaoChannel *c) {
    u8 *p = c->unk0;
    u32 k;
    u32 prod;
    u32 r;
    c->unk108 = (s8)*p;
    k = c->unk2C;
    r = (u8)c->unk108;
    prod = k * r;
    c->unk0 = p + 1;
    if (c->unk108 < 0) {
        r = (prod >> 8) - k;
    } else {
        r = prod >> 7;
    }
    c->unk84 = r;
    c->unk11C |= 0x10;
}

void func_8005F6B4(AkaoChannel *c) {
    u32 r;

    c->unk108 += (s8)*c->unk0++;
    r = c->unk2C * (u8)c->unk108;
    if (c->unk108 < 0) {
        r = (r >> 8) - c->unk2C;
    } else {
        r >>= 7;
    }
    c->unk84 = r;
    c->unk11C |= 0x10;
}

void func_8005F718(AkaoChannel *c) {
    u32 b;
    s16 v;
    u32 n;
    u32 r;
    u32 k;
    u32 t;

    c->unk34 |= 1;
    if (c->unk94 != 0) {
        c->unkCE = 0;
        b = *c->unk0++;
        if (b != 0) {
            c->unkD6 = b << 8;
        }
    } else {
        c->unkCE = *c->unk0++;
    }
    c->unk3C = t = *c->unk0++ << 12;
    if (t == 0) {
        c->unk3C = 0x100000;
    }
    b = *c->unk0++;
    v = c->unkD6;
    k = (u16)c->unk2C;
    n = (v & 0x7F00) >> 8;
    if (!(v & 0x8000)) {
        r = (n * ((s32)(k * 15) >> 8)) >> 7;
    } else {
        r = (n * k) >> 7;
    }
    c->unkD4 = r;
    c->unkD2 = 0;
    c->unkD0 = c->unkCE;
    c->unk1C = D_8006FB40[b & 7];
    c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
}

void func_8005F834(AkaoChannel *c) {
    s32 k;
    s16 v;
    u32 n;
    u32 r;

    c->unkD6 = *c->unk0++ << 8;
    k = c->unk2C;
    v = c->unkD6;
    n = (v & 0x7F00) >> 8;
    if (!(v & 0x8000)) {
        r = n * ((s32)(k * 15) >> 8);
    } else {
        r = n * k;
    }
    c->unkD4 = r >> 7;
}

void func_8005F894(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkDA = ((*c->unk0++ << 8) - c->unkD6) / n;
    c->unkD8 = n;
}

void func_8005F900(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkD2 = v;
    c->unk40 = func_8005EB40(&c->unk3C, *c->unk0++, v, 0xC);
}

void func_8005F960(AkaoChannel *c) {
    c->unk110 = 0;
    c->unk34 &= ~1;
    c->unk11C |= 0x10;
}

void func_8005F984(AkaoChannel *c) {
    s32 v;
    u32 d;

    c->unk34 |= 2;
    v = *c->unk0++;
    if (c->unk94 != 0) {
        c->unkDC = 0;
        if (v != 0) {
            c->unkE2 = (v & 0x7F) << 8;
        }
    } else {
        c->unkDC = v;
    }
    c->unk4C = *c->unk0++ << 12;
    if (c->unk4C == 0) {
        c->unk4C = 0x100000;
    }
    v = *c->unk0++;
    d = 0x1000000 / ((u32)c->unk4C >> 12);
    c->unkDE = c->unkDC;
    c->unkE0 = 0;
    c->unk20 = D_8006FB40[v & 7];
    c->unk58 = d;
}

void func_8005FA54(AkaoChannel *c) {
    c->unkE2 = (*c->unk0++ & 0x7F) << 8;
}

void func_8005FA78(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkE6 = (((*c->unk0++ & 0x7F) << 8) - c->unkE2) / n;
    c->unkE4 = n;
}

void func_8005FAE8(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkE0 = v;
    c->unk50 = func_8005EB40(&c->unk4C, *c->unk0++, v, 0xC);
}

void func_8005FB48(AkaoChannel *c) {
    c->unk112 = 0;
    c->unk34 &= ~2;
    c->unk11C |= 3;
}

void func_8005FB6C(AkaoChannel *arg0) {
    arg0->unk34 |= 4;
    arg0->unk5C = *arg0->unk0++ << 12;
    if (arg0->unk5C == 0) {
        arg0->unk5C = 0x100000;
    }
    {
        u32 d = 0x1000000 / ((u32)arg0->unk5C >> 12);
        u8 b = *arg0->unk0++;
        arg0->unkE8 = 0;
        arg0->unk64 = 0;
        arg0->unk68 = d;
        arg0->unk24 = D_8006FB40[b & 7];
    }
}

void func_8005FC00(AkaoChannel *c) {
    c->unkEA = *c->unk0++ << 7;
}

void func_8005FC20(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkEE = ((*c->unk0++ << 7) - c->unkEA) / n;
    c->unkEC = n;
}

void func_8005FC8C(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkE8 = v;
    c->unk60 = func_8005EB40(&c->unk5C, *c->unk0++, v, 0xC);
}

void func_8005FCEC(AkaoChannel *c) {
    c->unk114 = 0;
    c->unk34 &= ~4;
    c->unk11C |= 3;
}

void func_8005FD10(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk3C |= mask;
    } else {
        D_80080A70.unk1C |= mask;
    }
    D_80083158.unk8 |= 0x110;
}

void func_8005FD70(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk3C &= ~mask;
    } else {
        D_80080A70.unk1C &= ~mask;
    }
    D_80083158.unk8 |= 0x110;
    c->unkF0 = 0;
}

void func_8005FDD8(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk44 |= mask;
    } else if (c->unk34 & 0x10000) {
        D_80080A70.unk24 |= mask;
    }
    D_80083158.unk8 |= 0x100;
}

void func_8005FE4C(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk44 &= ~mask;
    } else {
        D_80080A70.unk24 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
    c->unkF2 = 0;
}

void func_8005FEB4(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk40 |= mask;
    } else {
        D_80080A70.unk20 |= mask;
    }
    D_80083158.unk8 |= 0x100;
}

void func_8005FF14(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk40 &= ~mask;
    } else {
        D_80080A70.unk20 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
}

void func_8005FF74(AkaoChannel *a) {
    a->unkCC = 1;
}

void func_8005FF80(void) {
}

void func_8005FF88(AkaoChannel *c) {
    if (c->unk94 != 0) {
        c->unkCC = 4;
    }
}

void func_8005FFA4(void) {
}

void func_8005FFAC(AkaoChannel *arg0) {
    u32 b = *arg0->unk0++;

    if (arg0->unk94 == 0) {
        if (b & 0xC0) {
            g_akaoSeq->unk6E = (g_akaoSeq->unk6E + (b & 0x3F)) & 0x3F;
        } else {
            g_akaoSeq->unk6E = b;
        }
    } else if (b & 0xC0) {
        D_80080A70.unk28 = (D_80080A70.unk28 + (b & 0x3F)) & 0x3F;
    } else {
        D_80080A98 = b;
    }
    D_80083158.unk8 |= 0x10;
}

void func_80060048(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unk11C |= 0x900;
    c->unk34 |= 0x1000000;
    c->unk12A = (c->unk12A & 0x80FF) | (v << 8);
}

void func_8006008C(AkaoChannel *c, s32 arg1) {
    u16 t = c->unk12A & 0xFF0F;
    c->unk12A = t | (*c->unk0++ << 4);
    c->unk11C |= 0x1000;
}

void func_800600C0(AkaoChannel *c, s32 arg1) {
    u16 t = c->unk12A & 0xFFF0;
    c->unk12A = t | *c->unk0++;
    c->unk11C |= 0x8000;
}

void func_800600F0(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unk11C |= 0x2200;
    c->unk34 |= 0x8000000;
    c->unk12C = (c->unk12C & 0xE03F) | (v << 6);
}

void func_80060134(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unk11C |= 0x4400;
    c->unk34 |= 0x10000000;
    c->unk12C = (c->unk12C & 0xFFE0) | v;
}

void func_80060174(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unk12A &= 0x7FFF;
    if (v == 5) {
        c->unk12A |= 0x8000;
    }
    c->unk11C |= 0x100;
}

void func_800601B8(AkaoChannel *c) {
    u16 v = *c->unk0++;
    u16 x = c->unk12C & 0x3FFF;
    c->unk12C = x;
    switch (v) {
    case 3:
        c->unk12C = x | 0x4000;
        break;
    case 5:
        c->unk12C = x | 0x8000;
        break;
    case 7:
        c->unk12C = x | 0xC000;
        break;
    }
    c->unk11C |= 0x200;
}

void func_80060230(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unk12C &= 0xFFDF;
    if (v == 7) {
        c->unk12C |= 0x20;
    }
    c->unk11C |= 0x400;
}

void func_80060274(AkaoChannel *c) {
    g_akaoSeq->unk38 = *c->unk0++;
}

void func_80060294(void) {
    g_akaoSeq->unk38 = 0;
}

void func_800602A4(AkaoChannel *c) {
    c->unkF4 = (c->unkF4 + 1) & 3;
    c->unk4[c->unkF4] = c->unk0;
    c->unkA2[c->unkF4] = 0;
    c->unkAA[c->unkF4] = c->unkA0;
}

void func_800602F4(AkaoChannel *arg0) {
    u32 n = *arg0->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    if (++arg0->unkA2[arg0->unkF4] != n) {
        arg0->unk0 = arg0->unk4[arg0->unkF4];
        arg0->unkA0 = arg0->unkAA[arg0->unkF4];
        return;
    }
    arg0->unkF4 = (arg0->unkF4 - 1) & 3;
}

void func_80060388(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->unkA2[c->unkF4] + 1 != v) {
        c->unk0 = p + 2;
    } else {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
    }
}

void func_800603F4(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->unkA2[c->unkF4] + 1 != v) {
        c->unk0 = p + 2;
    } else {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
        c->unkF4 = (c->unkF4 - 1) & 3;
    }
}

void func_80060470(AkaoChannel *c) {
    c->unkA2[c->unkF4]++;
    c->unk0 = c->unk4[c->unkF4];
    c->unkA0 = c->unkAA[c->unkF4];
}

void func_800604C4(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkFA = 0;
    c->unk98 = v;
    c->unk96 = v;
    c->unkF8 = v;
}

void func_800604EC(AkaoChannel *c) {
    s32 v = (s8)*c->unk0++;
    if (v != 0) {
        v += (s16)c->unkF8;
        if (v < 1) {
            v = 1;
        } else if (v >= 0x100) {
            v = 0xFF;
        }
    }
    c->unkFA = v;
}

void func_80060534(AkaoChannel *c) {
    if (g_akaoSeq->unk34 != 0) {
        c->unk34 = (c->unk34 & 0xE6FFEFF7) | 8;
    }
}

void func_8006056C(AkaoChannel *c) {
    c->unk12E = 0;
    c->unk34 &= ~8;
}

void func_80060584(AkaoChannel *c) {
    Unk80080A10 *d = g_akaoSeq;
    d->unk74 = *c->unk0++;
    d->unk70 = *c->unk0++;
    d->unk76 = 0;
    d->unk72 = 0;
}

void func_800605BC(AkaoChannel *c) {
    Unk80080A10 *d = g_akaoSeq;
    d->unk78 = *c->unk0++;
    d->unk78 |= *c->unk0++ << 8;
}

void func_800605F8(AkaoChannel *c, s32 b) {
    func_8006008C(c, b);
    func_800600C0(c, b);
}

void func_80060634(AkaoChannel *c, u32 mask) {
    s32 v = *c->unk0++;
    c->unkF0 = v ? v + 1 : 0x101;
    func_8005FD10(c, mask);
}

void func_80060678(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v != 0) {
        c->unkF0 = v + 1;
    } else {
        c->unkF0 = 0x101;
    }
}

void func_800606A8(AkaoChannel *c, u32 mask) {
    s32 v = *c->unk0++;
    c->unkF2 = v ? v + 1 : 0x101;
    func_8005FDD8(c, mask);
}

void func_800606EC(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v != 0) {
        c->unkF2 = v + 1;
    } else {
        c->unkF2 = 0x101;
    }
}

void func_8006071C(AkaoChannel *c, u32 mask) {
    c->unk34 &= ~0x37;
    func_8005FD70(c, mask);
    func_8005FE4C(c, mask);
    func_8005FF14(c, mask);
    c->unkCC &= ~5;
}

void func_80060784(AkaoChannel *c) {
    c->unk34 |= 0x10;
}

void func_80060798(AkaoChannel *c) {
    c->unk34 &= ~0x10;
}

void func_800607AC(AkaoChannel *c) {
    c->unk34 |= 0x20;
}

void func_800607C0(AkaoChannel *c) {
    c->unk34 &= ~0x20;
}

static inline u8 *func_800607D4_ref(u8 *p) {
    s32 o = (p[1] << 8) | p[0];
    return o != 0 ? p + o + 2 : NULL;
}

void func_800607D4(AkaoChannel *c) {
    u8 *p = c->unk0;
    u8 *a;
    u8 *b;

    a = func_800607D4_ref(p);
    p += 2;
    b = func_800607D4_ref(p);
    D_800809F0.unk0 = 0;
    D_800809F0.unk4 = 0;
    D_800809F0.unk8 = c->unkBE >> 8;
    D_800809F0.unkC = c->unk78 >> 23;
    func_8005A014(&D_800809F0, a, b, 0);
    c->unk0 += 4;
}

void func_8006088C(AkaoChannel *c) {
    c->unk34 |= 0x100000;
}

void func_800608A0(AkaoChannel *c) {
    c->unk0++;
}

void func_800608B4(AkaoChannel *c, s32 b) {
    g_akaoSeq->unk8 |= b;
}

void func_800608D4(AkaoChannel *c, s32 b) {
    g_akaoSeq->unk8 &= ~b;
}

void func_800608F4(AkaoChannel *c) {
    c->unkF6 = *c->unk0++;
}

void func_80060910(AkaoChannel *a) {
    a->unkF6 = 0;
}

void func_80060918(AkaoChannel *c, s32 b) {
    func_8005ECCC(c, b);
}

void func_80060938(void) {
    u32 m;
    if (D_80083168.unkC != 0) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        func_800575D0(D_80083168.unkC);
        if (D_80083168.unk8 & 0x400) {
            func_800576BC(D_80083168.unk10 - 2, 0x1030);
            func_800576BC(D_80083168.unk10 - 1, 0x1030);
        }
        func_800576BC(D_80083168.unk10, 0x1030);
        func_800576BC(D_80083168.unk10 + 1, 0x1030);
        m = D_80083168.unkC;
        D_80083168.unkC = 0;
        D_80080A70.unk20 &= ~m;
        D_80083158.unk8 |= 0x100;
    }
}

s32 func_80060A08(void) {
    u32 mask;
    s32 i;
    u32 bit;

    do {
        bit = 0xC00000;
        mask = D_80080A70.unk0 | D_80080A70.unk10;
        for (i = 15; i != 0; i--, bit >>= 1) {
            if (!(mask & bit)) {
                break;
            }
        }
        if (i != 0) {
            return i + 7;
        }
        func_80059AA8(0, 0x40000000);
    } while (mask != (D_80080A70.unk0 | D_80080A70.unk10));
    return -1;
}

void func_80060A98(void) {
    u8 *p = D_80083168.unk0 + 0x800;
    SpuSetTransferStartAddr(0x2100);
    SpuSetTransferCallback(func_80060FA8);
    func_8005709C(p, 0x800);
}

void func_80060AE0(s32 addr, s32 vol, s32 flag) {
    StreamHeader *h;
    s32 v;
    s32 mask;
    s32 size;
    s32 t;

    v = func_80060A08();
    if (v == -1) {
        return;
    }
    D_80083168.unk4C = vol;
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)addr;
    D_80083168.unk0 = (u8 *)h + 0x40;
    D_80083168.unk14 = h->unk10;
    mask = (1 << v) | (1 << (v + 1));
    t = h->unk20;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    D_80083168.unk18 = t;
    func_800575D0(mask);
    D_80083168.unk8 = h->unk18;
    D_80083168.unk58 = h->unk1C;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    func_800575D0(mask);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    D_800809E0 = 1;
    D_80083168.unk4 = (D_80083168.unk8 & 2) ? (s32)(D_80083168.unk0 + h->unk14) : 0;
    if (D_80083168.unk8 & 1) {
        D_80083168.unk1C = (D_80083168.unk8 & 2) ? D_80083168.unk14 - (h->unk14 >> 1) : 0;
        SpuSetTransferCallback(func_8006100C);
        size = 0x2000;
    } else {
        D_80083168.unk1C = (D_80083168.unk8 & 2) ? D_80083168.unk14 - h->unk14 : 0;
        SpuSetTransferCallback(func_80060A98);
        size = 0x800;
    }
    func_8005709C(D_80083168.unk0, size);
    if (flag) {
        D_80080A70.unk20 |= D_80083168.unkC;
    } else {
        D_80080A70.unk20 &= ~D_80083168.unkC;
    }
    D_80080A70.unk24 &= ~D_80083174;
    D_80080A70.unk1C &= ~D_80083174;
    D_80083158.unk8 |= 0x100;
}

void func_80060D1C(s32 v, s32 mode, s32 a, s32 b) {
    s16 l;
    s16 r;
    s16 h;
    s32 pan;

    if (D_8008315C & 2) {
        l = r = (D_800831A8 * D_8006F640) >> 16;
    } else if (mode == 1) {
        r = 0;
        l = (u32)D_800831A8 >> 1;
    } else if (mode == 2) {
        l = 0;
        r = (u32)D_800831A8 >> 1;
    } else if (mode == 3) {
        h = D_800831A8 >> 1;
        l = r = (h >> 1) + (h >> 2);
    } else {
        pan = (u8)(D_80083168.unk4C >> 8);
        l = (u32)(D_80083168.unk40 * D_8006F540[pan]) >> 16;
        r = (u32)(D_80083168.unk40 * D_8006F540[pan ^ 0xFF]) >> 16;
    }
    func_80057640(v, l, r, 0);
    func_80057688(v, D_800831C0);
    func_800576A0(v, a);
    func_800576BC(v, b);
    func_80057708(v, 0, 1);
    func_80057738(v, 0xF);
    func_80057760(v, 0xF);
    func_80057788(v, 0x7F, 3);
    func_800577BC(v, 6, 3);
}

void func_80060ED8(s32 a, s32 b, void (*cb)(void)) {
    if (D_80083168.unkC != 0) {
        SpuSetTransferCallback(NULL);
        D_800809E0 = 0;
        if (D_80083168.unk14 > 0x1000) {
            D_80083168.unk14 -= 0x1000;
            D_80083168.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(func_80060938);
            b = 0x1030;
            D_80083168.unk14 = 0;
        }
        SpuSetIRQAddr(b + 8);
        func_800575B4(D_80083168.unkC);
        SpuSetIRQ(1);
    }
}

void func_80060FA8(void) {
    func_80060D1C(D_80083168.unk10, 0, 0x1100, 0x2100);
    func_80060D1C(D_80083168.unk10 + 1, 0, 0x1100, 0x2100);
    func_80060ED8(0x1000, 0x2100, func_800611BC);
}

void func_8006100C(void) {
    func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x2100);
    func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x2900);
    func_80060ED8(0x2000, 0x2100, func_8006121C);
}

void func_80061070(s32 a, s32 b, s32 n, void (*cb)(void)) {
    if (D_80083168.unkC == 0 || D_80083168.unk14 == 0) {
        return;
    }
    SpuSetTransferStartAddr(a);
    func_80056614();
    func_8005709C(D_80083168.unk0, n);
    SpuSetIRQ(0);
    if (D_80083168.unk14 > 0x800) {
        SpuSetIRQCallback(cb);
        D_80083168.unk14 -= 0x800;
        D_80083168.unk0 += n;
    } else if (D_80083168.unk4 != NULL) {
        SpuSetIRQCallback(cb);
        D_80083168.unk0 = D_80083168.unk4;
        D_80083168.unk14 = D_80083168.unk1C;
    } else {
        SpuSetIRQCallback(func_80060938);
        b = 0x1030;
        a = b;
        D_80083168.unk14 = 0;
    }
    func_800576BC(D_80083168.unk10, a);
    func_800576BC(D_80083168.unk10 + 1, b);
    SpuSetIRQAddr(a + 8);
    SpuSetIRQ(1);
}

void func_800611BC(void) {
    func_80061070(0x1100, 0x1100, 0x800, func_800611EC);
}

void func_800611EC(void) {
    func_80061070(0x2100, 0x2100, 0x800, func_800611BC);
}

void func_8006121C(void) {
    func_80061070(0x1100, 0x1900, 0x1000, func_8006124C);
}

void func_8006124C(void) {
    func_80061070(0x2100, 0x2900, 0x1000, func_8006121C);
}

void func_8006127C(s32 *a) {
    func_80060938();
    func_80060AE0(a[0], a[1], a[2]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

void func_800612D0(void) {
    func_80060938();
}

void func_800612F0(s32 *a) {
    Unk80083168 *d = &D_80083168;
    s32 v = *a;
    s32 l;
    s32 r;
    s32 pan;

    d->unk48 = 0;
    d->unk40 = v;
    if (d->unkC == 0) {
        return;
    }
    if (D_8008315C & 2) {
        v = v * D_8006F640 >> 16;
        func_80057640(d->unk10, v, v, 0);
        func_80057640(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v <<= 15;
        v >>= 16;
        func_80057640(d->unk10, v, 0, 0);
        func_80057640(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (u8)(d->unk4C >> 8);
        l = v * D_8006F540[pan] >> 16;
        r = v * D_8006F540[pan ^ 0xFF] >> 16;
        func_80057640(d->unk10, l, r, 0);
        func_80057640(d->unk10 + 1, l, r, 0);
    }
}

void func_8006142C(Unk80080AA0 *p) {
    s16 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_80083168.unk44 = (s16)((s16)((u16)p->unk4 - (u16)D_80083168.unk40) / n);
    D_80083168.unk48 = n;
}

void func_800614AC(s32 *a) {
    Unk80083168 *d = &D_80083168;
    s32 p = *a;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    d->unk4C = p;
    if (d->unkC == 0) {
        return;
    }
    if (D_8008315C & 2) {
        v = d->unk40 * D_8006F640 >> 16;
        func_80057640(d->unk10, v, v, 0);
        func_80057640(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v = d->unk40;
        v <<= 15;
        v >>= 16;
        func_80057640(d->unk10, v, 0, 0);
        func_80057640(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (p >> 8) & 0xFF;
        l = d->unk40 * D_8006F540[pan] >> 16;
        r = d->unk40 * D_8006F540[pan ^ 0xFF] >> 16;
        func_80057640(d->unk10, l, r, 0);
        func_80057640(d->unk10 + 1, l, r, 0);
    }
}

void func_800615F8(void) {
    SpuSetIRQAddr(0x1038);
    SpuSetIRQCallback(func_80060938);
}

void func_80061624(void) {
    u32 irq;
    if (D_80083168.unk14 > 0x2000) {
        SpuSetTransferStartAddr(D_80083168.unk2C + 0x2000);
        func_80056644(D_80083168.unk0, D_80083168.unk14 - 0x2000);
        irq = D_80083168.unk2C + 0x1FF8;
    } else {
        irq = D_80083168.unk2C + (D_80083168.unk14 >> 1) + 8;
    }
    func_80060D1C(D_80083168.unk10, 0, D_80083168.unk2C, 0x1030);
    func_80060D1C(D_80083168.unk10 + 1, 0, D_80083168.unk2C, 0x1030);
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    func_800575B4(D_80083168.unkC);
    SpuSetIRQ(1);
}

void func_800616F8(s32 arg0, s32 b, s32 addr, s32 loop) {
    AkaoStream *h = (AkaoStream *)arg0;
    Unk80083168 *d;
    s32 v;
    u32 n;
    u32 m;

    v = func_80060A08();
    if (v != -1) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        d = &D_80083168;
        d->unk4C = b;
        D_80083168.unk0 = h->unk40;
        d->unk14 = h->unk10;
        d->unk18 = h->unk20;
        m = (1 << v) | (1 << (v + 1));
        n = d->unkC;
        d->unk10 = v;
        d->unkC = m;
        func_800575D0(m | n);
        d->unk8 = h->unk18;
        d->unk58 = h->unk1C;
        d->unk2C = addr;
        SpuSetTransferMode(0);
        SpuSetTransferStartAddr(addr);
        D_800809E0 = 1;
        SpuSetTransferCallback(func_80061624);
        n = d->unk14;
        if (n > 0x2000) {
            n = 0x2000;
        }
        func_8005709C(D_80083168.unk0, n);
        D_80083168.unk0 += n;
        if (loop) {
            D_80080A70.unk20 |= d->unkC;
        } else {
            D_80080A70.unk20 &= ~d->unkC;
        }
        D_80080A70.unk24 &= ~D_80083174;
        D_80080A70.unk1C &= ~D_80083174;
        D_80083158.unk8 |= 0x100;
    }
}

void func_800618C0(s32 unused, s32 flag) {
    s32 *h = D_8007F720;
    s32 *h2;
    s32 v;
    s32 mask;
    s32 irq;
    s32 old;

    if (h[8] == 0) {
        return;
    }
    v = func_80060A08();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    mask = (1 << v) | (1 << (v + 1));
    old = D_80083168.unkC;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    func_800575D0(mask | old);
    h2 = h;
    D_80083168.unk8 = h2[6];
    D_80083168.unk58 = (u16)h2[7];
    if (flag) {
        D_80080A70.unk20 |= D_80083168.unkC;
    } else {
        D_80080A70.unk20 &= ~D_80083168.unkC;
    }
    irq = h2[8] + ((u32)h2[4] >> 1) + 8;
    D_80080A70.unk24 &= ~D_80083168.unkC;
    D_80080A70.unk1C &= ~D_80083168.unkC;
    D_80083158.unk8 |= 0x100;
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    if (h2[6] & 1) {
        if (h2[6] & 2) {
            func_80060D1C(D_80083168.unk10, 1, h2[8], h2[8] + h2[5]);
            func_80060D1C(D_80083168.unk10 + 1, 2, h2[8] + h2[9], h2[8] + h2[9] + h2[5]);
        } else {
            func_80060D1C(D_80083168.unk10, 1, h2[8], 0x1030);
            func_80060D1C(D_80083168.unk10 + 1, 2, h2[8] + h2[9], 0x1030);
        }
    } else {
        if (h2[6] & 2) {
            func_80060D1C(D_80083168.unk10, 3, h2[8], h2[8] + h2[5]);
            func_80060D1C(D_80083168.unk10 + 1, 3, h2[8], h2[8] + h2[5]);
        } else {
            func_80060D1C(D_80083168.unk10, 3, h2[8], 0x1030);
            func_80060D1C(D_80083168.unk10 + 1, 3, h2[8], 0x1030);
        }
    }
    func_800575B4(D_80083174);
    SpuSetIRQ(1);
}

void func_80061B20(s32 *a) {
    func_80060938();
    func_800616F8(a[0], a[1], a[2], a[3]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

void func_80061B78(s32 *a) {
    func_80060938();
    func_800618C0(a[0], a[1]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

/* K&R definition: callers pass one or two arguments */
s32 func_80061BC8(p)
    s32 *p;
{
    D_80083168.unk28++;
    (*p)++;
    if (D_80083168.unk3C - 1 < *p) {
        *p = 0;
    }
    return *p;
}

void func_80061C1C(void) {
    StreamHeader *h;
    Unk80083168 *d;
    s32 v;
    s32 vol;
    s32 mask;

    v = func_80060A08();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)(D_80083168.unk2C + 0x80);
    if (h->unk28 != 0 && D_80083168.unk48 == 0) {
        vol = D_80083168.unk40;
        D_80083168.unk40 = 0;
        func_80056278(h->unk28, vol >> 8);
    }
    d = &D_80083168;
    D_80083168.unk0 = (u8 *)d->unk2C;
    d->unk14 = h->unk10;
    d->unk18 = h->unk20;
    mask = (1 << v) | (1 << (v + 1));
    d->unk10 = v;
    d->unkC = mask;
    d->unk34 = 0;
    func_800575D0(mask);
    d->unk8 = h->unk18;
    d->unk58 = h->unk1C;
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    D_800809E0 = 1;
    SpuSetTransferCallback(func_80061EE4);
    if (d->unk8 & 0x400) {
        func_8005709C(D_80083168.unk0 + 0xD0, 0x4000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        d->unkC |= (1 << (v - 2)) | (1 << (v - 1));
    } else {
        func_8005709C(D_80083168.unk0 + 0xD0, 0x2000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
    }
    D_80080A70.unk20 &= ~D_80083174;
    D_80080A70.unk24 &= ~D_80083174;
    D_80080A70.unk1C &= ~D_80083174;
    D_80083158.unk8 |= 0x100;
}

void func_80061E1C(s32 a, s32 b, void (*cb)(void)) {
    if (D_80083168.unkC != 0) {
        SpuSetTransferCallback(NULL);
        D_800809E0 = 0;
        if (D_80083168.unk14 >= 0xE61) {
            D_80083168.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(func_80060938);
            b = 0x1030;
        }
        SpuSetIRQAddr(b + 8);
        func_800575B4(D_80083168.unkC);
        SpuSetIRQ(1);
    }
}

void func_80061EE4(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x3100);
        func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x3900);
        func_80060D1C(D_80083168.unk10 - 2, 1, 0x2100, 0x4100);
        func_80060D1C(D_80083168.unk10 - 1, 2, 0x2900, 0x4900);
        a = 0x4000;
        b = 0x3100;
    } else {
        func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x2100);
        func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x2900);
        a = 0x2000;
        b = 0x2100;
    }
    func_80061E1C(a, b, func_80062118);
}

s32 func_80061FBC(s32 a, s32 b, s32 n, void (*cb)(void)) {
    AkaoStream *h;

    if (D_80083168.unkC != 0) {
        h = (AkaoStream *)(D_80083168.unk0 + 0x80);
        if (h->unk0 == 0x4F414B41) {
            SpuSetIRQ(0);
            SpuSetTransferStartAddr(a);
            func_80056614();
            func_80061BC8(&D_80083168.unk34);
            func_8005709C(D_80083168.unk0 + 0xD0, n - 0xD0);
            D_80083168.unk20 = h->unk4;
            D_80083168.unk18 = h->unk20;
            if (h->unk10 > h->unk20) {
                SpuSetIRQCallback(cb);
                D_80083168.unk0 += n;
                if (D_80083168.unk34 == 0) {
                    D_80083168.unk0 = (u8 *)D_80083168.unk2C;
                }
            } else {
                SpuSetIRQCallback(func_80060938);
                b = 0x1030;
                a = b;
            }
            func_800576BC(D_80083168.unk10, a);
            func_800576BC(D_80083168.unk10 + 1, b);
            SpuSetIRQAddr(a + 8);
            SpuSetIRQ(1);
        }
    }
    return a;
}

void func_80062118(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80061BC8(&D_80083168.unk34);
        a = func_80061FBC(0x1100, 0x1900, 0x2000, func_800621D4);
        b = 0x2900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x2100;
        }
        func_800576BC(D_80083168.unk10 - 2, a);
        func_800576BC(D_80083168.unk10 - 1, b);
    } else {
        func_80061FBC(0x1100, 0x1900, 0x1000, func_800621D4);
    }
}

void func_800621D4(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80061BC8(&D_80083168.unk34);
        a = func_80061FBC(0x3100, 0x3900, 0x2000, func_80062118);
        b = 0x4900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x4100;
        }
        func_800576BC(D_80083168.unk10 - 2, a);
        func_800576BC(D_80083168.unk10 - 1, b);
    } else {
        func_80061FBC(0x2100, 0x2900, 0x1000, func_80062118);
    }
}

void func_80062290(s32 *a) {
    func_80060938();
    D_80083168.unk34 = -1;
    D_80083168.unk20 = 0;
    D_80083168.unk24 = 0;
    D_80083168.unk28 = 0;
    D_80083168.unk38 = 0;
    D_80083168.unk8 = 0x1000000;
    D_80083168.unk3C = (u32)D_80080AA0.unk4 >> 12;
    D_80083168.unk2C = a[0];
    D_80083168.unk30 = a[1];
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SpuSetIRQAddr);

void func_80062344(void) {
    s32 v = 0xFF;
    s32 i;
    D_80083248.unk0 = 0;
    D_80083248.unk2 = 0;
    D_80083248.unk4 = 0;
    for (i = 0x68; i >= 0; i--) {
        D_80083248.unk6[i].unk0 = v;
    }
}

void func_8006237C(void) {
    D_800834C8 = 1;
}

s32 func_8006238C(s32 a) {
    s32 i;

    if (func_800625F8() >= 100) {
        return -1;
    }
    for (i = 0; i < 100; i++) {
        if (D_80083248.unk6[i].unk0 == 0xFF) {
            func_80062830(&D_80083248.unk6[i], a);
            return func_80062560(a) == 1;
        }
    }
    return -1;
}

s32 func_80062420(u8 id, u8 a, u8 b, u8 c, u8 d, u8 e) {
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == 0xFF) {
            D_80083248.unk6[i].unk0 = id;
            D_80083248.unk6[i].unk1 = a;
            D_80083248.unk6[i].unk2 = b;
            D_80083248.unk6[i].unk3 = c;
            D_80083248.unk6[i].unk4 = d;
            D_80083248.unk6[i].unk5 = e;
            return func_80062560(id) == 1;
        }
    }
    return -1;
}

s16 func_80062498(void) {
    return D_80083248.unk0;
}

void func_800624A8(void) {
    if (++D_80083248.unk0 == 10000) {
        D_80083248.unk0 = 9999;
    }
}

s16 func_800624D8(void) {
    return D_80083248.unk2;
}

void func_800624E8(void) {
    if (++D_80083248.unk2 == 10000) {
        D_80083248.unk2 = 9999;
    }
}

s16 func_8006251C(void) {
    return D_80083248.unk4;
}

void func_8006252C(void) {
    if (++D_80083248.unk4 == 10000) {
        D_80083248.unk4 = 9999;
    }
}

s32 func_80062560(a)
    u8 a;
{
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == a) {
            n++;
        }
    }
    return n;
}

Unk6 *func_800625A0(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == (u8)id) {
            if (n == k) {
                return &D_80083248.unk6[i];
            }
            k++;
        }
    }
    return (Unk6 *)-1;
}

s32 func_800625F8(void) {
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 != 0xFF) {
            n++;
        }
    }
    return n;
}

s32 func_80062638(void) {
    u8 cnt[100];
    u8 *p;
    s32 i;
    s32 n;

    i = 99;
    p = &cnt[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 != 0xFF) {
            cnt[D_80083248.unk6[i].unk0]++;
        }
    }
    n = 0;
    for (i = 0; i < 100; i++) {
        if (cnt[i] != 0) {
            n++;
        }
    }
    return n;
}

s32 func_800626D0(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == (u8)id) {
            if (k == n) {
                D_80083248.unk6[i].unk0 = 0xFF;
                return 0;
            }
            k++;
        }
    }
    return -1;
}

void func_80062724(void) {
    s32 v = 0xFF;
    s32 i;
    for (i = 0x68; i >= 0; i--) {
        D_80083248.unk6[i].unk0 = v;
    }
}

void func_80062750(s32 a) {
    D_800834C8 = a;
}

s32 func_8006275C(void) {
    return D_800834C8;
}

s32 func_8006276C(s32 a) {
    return text_getFrom(D_8006FD60, a);
}

void func_80062794(s32 a) {
    func_8002BA68(func_8006276C(a));
}

s32 func_800627BC(s32 x) {
    if (x < 0x38) {
        return 0;
    }
    if (x < 0x46) {
        return 1;
    }
    if (x < 0x50) {
        return 2;
    }
    if (x < 0x58) {
        return 3;
    }
    if (x < 0x5D) {
        return 4;
    }
    if (x < 0x5F) {
        return 5;
    }
    return 6;
}

void *func_80062824(void) {
    return &D_80083248;
}

void func_80062830(Unk6 *card, u8 id) {
    Bytes8 bits;
    u8 v;
    s32 r;
    s32 n;
    s32 i;
    s32 done;
    u8 *b;
    CardInfo *tbl;

    bits = D_800121C0;
    tbl = D_800121C8;
    card->unk0 = id;
    v = tbl[id].unk0 >> 1;
    if (v != 0) {
        card->unk2 = v + rand() % v;
    } else {
        card->unk2 = 0;
    }
    v = tbl[id].unk2 >> 1;
    if (v != 0) {
        card->unk4 = v + rand() % v;
    } else {
        card->unk4 = 0;
    }
    v = tbl[id].unk3 >> 1;
    if (v != 0) {
        card->unk5 = v + rand() % v;
    } else {
        card->unk5 = 0;
    }
    r = rand() % 100;
    if (r <= 0) {
        n = 0;
    } else if (r < 9) {
        n = 1;
    } else if (r < 0x22) {
        n = 2;
    } else if (r < 0x41) {
        n = 3;
    } else if (r < 0x53) {
        n = 4;
    } else if (r < 0x5C) {
        n = 5;
    } else if (r < 0x61) {
        n = 6;
    } else if (r < 0x63) {
        n = 7;
    } else {
        n = 8;
    }
    card->unk1 = 0;
    for (i = 0; i < n; i++) {
        done = 0;
        do {
            b = &bits.unk0[rand() & 7];
            if (!(card->unk1 & *b)) {
                card->unk1 |= *b;
                done = 1;
            }
        } while (!done);
    }
    card->unk3 = tbl[id].unk1;
    if (!(rand() & 0x7F)) {
        card->unk3 = 2;
    }
}

CardInfo *func_80062B30(void) {
    return D_800121C8;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", rand);

INCLUDE_ASM("asm/us/main/nonmatchings/main", MargePrim);

INCLUDE_ASM("asm/us/main/nonmatchings/main", InvSquareRoot);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VectorNormalS);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VectorNormal);

INCLUDE_ASM("asm/us/main/nonmatchings/main", VectorNormalSS);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80062C8C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", MatrixNormal);

s32 func_80062E3C(s32 prim, s32 cx, s32 cy, s32 u, s32 v, s32 n, s32 flags) {
    Unk11C0Slot *s;
    Unk11C0Slot *q;
    POLY_FT4 *p;
    s32 clut;
    s32 i, j, k;

    clut = cy << 6;
    clut |= (cx >> 4) & 0x3F;
    cx = getTPage(1, 2, u, v);
    u = (u % 64) * 2;
    v = v % 256;
    s = (Unk11C0Slot *)prim;
    prim += n * (s32)sizeof(Unk11C0Slot);
    for (i = 0; i < n; i++) {
        q = &s[i];
        q->unk4_0 = 0;
        q->unk0_8 = 0xE0;
        q->unk0_20 = 0xC0;
        q->unk0_0 = 0x80;
        q->unk4_12 = 0;
        q->unk4_22 = 0;
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 4; k++) {
                if (!(flags & 1)) {
                    q->unk8[j][k] = NULL;
                    continue;
                }
                q->unk8[j][k] = (POLY_FT4 *)prim;
                if (flags & 2) {
                    prim += 0xC;
                }
                p = (POLY_FT4 *)prim;
                prim += (s32)sizeof(POLY_FT4);
                if (flags & 2) {
                    prim += 0xC;
                }
                setlen(p, 9);
                setcode(p, 0x2E);
                switch (k) {
                case 0:
                    p->u0 = u;
                    p->v0 = v;
                    p->u1 = u + 0x1F;
                    p->v1 = v;
                    p->u2 = u;
                    p->v2 = v + 0x1F;
                    p->u3 = u + 0x1F;
                    p->v3 = v + 0x1F;
                    break;
                case 1:
                    p->u0 = u + 0x1F;
                    p->v0 = v;
                    p->u1 = u;
                    p->v1 = v;
                    p->u2 = u + 0x1F;
                    p->v2 = v + 0x1F;
                    p->u3 = u;
                    p->v3 = v + 0x1F;
                    break;
                case 2:
                    p->u0 = u;
                    p->v0 = v + 0x1F;
                    p->u1 = u + 0x1F;
                    p->v1 = v + 0x1F;
                    p->u2 = u;
                    p->v2 = v;
                    p->u3 = u + 0x1F;
                    p->v3 = v;
                    break;
                case 3:
                    p->u0 = u + 0x1F;
                    p->v0 = v + 0x1F;
                    p->u1 = u;
                    p->v1 = v + 0x1F;
                    p->u2 = u + 0x1F;
                    p->v2 = v;
                    p->u3 = u;
                    p->v3 = v;
                    break;
                }
                p->tpage = cx;
                p->clut = clut;
            }
        }
    }
    return prim;
}

FileEntry *func_80063088(FileTable *t, s32 id) {
    FileEntry *e;
    FileEntry *end;

    id &= 0xFFFFFF;
    e = t->unk4;
    end = e + t->unk2;
    for (; e < end; e++) {
        if ((e->unk0 & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetPolyF4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80063108);

s32 func_800631AC(WinDef *t) {
    if (t->unkC == NULL) {
        return 0;
    }
    if (t->unk0 != 0) {
        t->unk0--;
    } else {
        if (t->unkC->unk4 != NULL) {
            t->unk4.x += t->unkC->unk4[t->unk2];
        }
        if (t->unkC->unk8 != NULL) {
            t->unk4.y += t->unkC->unk8[t->unk2];
        }
        if (++t->unk2 < t->unkC->unk2) {
            return 0;
        }
        if (t->unkC->unkC != NULL) {
            t->unkC->unkC(t->unkC);
        }
        t->unkC = NULL;
        return 1;
    }
    return 0;
}

s32 func_800632B4(s32 n, s8 *p) {
    s32 sum = 0;
    s32 i;
    if (n == 0 || p == NULL) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        sum += *p++;
    }
    return sum;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", MulRotMatrix);

u16 *func_800633E4(s32 id) {
    s32 i;
    s32 j;
    u8 *blk;
    DbEntry12 *tab;
    DbEntry12 *e;

    for (i = D_80073C40.unk2 - 1; i >= 0; i--) {
        blk = (u8 *)func_8001C8B0((s32)D_80073C40.unk4[i], 2, 0x120000, 0);
        if (blk != (u8 *)-1) {
            j = blk[1] - 1;
            tab = (DbEntry12 *)(blk + 4);
            for (; j >= 0; j--) {
                e = &tab[j];
                if (e->unk0 == (u16)id) {
                    return (u16 *)e;
                }
            }
        }
    }
    return NULL;
}

u8 *func_800634A8(s32 a) {
    u16 *p = func_800633E4(a);
    return (u8 *)p + (((u32 *)p)[1] & 0xFFFFFF);
}

u8 *func_800634D8(s32 a) {
    u8 *p = (u8 *)func_800633E4(a);
    return p + (((u32 *)p)[1] & 0xFFFFFF) + p[7] * 4;
}

u8 *func_80063514(s32 a) {
    DbEntry12 *p = (DbEntry12 *)func_800633E4(a);
    s32 n = p->unk7;
    u8 *q = (u8 *)p + p->unk4;
    return (long long)n * 8 + q;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", NormalClip);

void func_80063580(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetSprt((SPRT *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_800635E0(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetTile((TILE *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063640(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF3((POLY_F3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_800636A0(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF4((POLY_F4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063700(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG3((POLY_G3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063760(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG4((POLY_G4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", exit);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetPolyF3);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetPolyG3);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetSprt);

s32 func_80063830(s32 id) {
    DbEntry12 *e = (DbEntry12 *)func_800633E4(id);

    if (e != NULL) {
        return e->unk7;
    }
    return 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", bcopy);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CompMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800639D4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadAverageShort12);

INCLUDE_ASM("asm/us/main/nonmatchings/main", AddPrim);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetLineG2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetDrawStp);

INCLUDE_ASM("asm/us/main/nonmatchings/main", MulMatrix2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ApplyMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", TransMatrix);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800121C0);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_800121C8);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80012578);

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_8001258C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", sprintf);

INCLUDE_ASM("asm/us/main/nonmatchings/main", strlen);

INCLUDE_ASM("asm/us/main/nonmatchings/main", memchr);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadAverageCol);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800645C4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetTexWindow);

INCLUDE_ASM("asm/us/main/nonmatchings/main", BreakDraw);

INCLUDE_ASM("asm/us/main/nonmatchings/main", IsIdleGPU);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ContinueDraw);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDrawEnv2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDrawMode);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetTexWindow);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDrawArea);

INCLUDE_ASM("asm/us/main/nonmatchings/main", GetDrawOffset);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80064A2C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", ApplyMatrixLV);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetFarColor);

void func_80064BE4(void) {
}

void func_80064BEC(s32 a, u32 *b) {
    func_8004C204();
    D_8007BCA4 = b;
    func_800512E4(0x808080);
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80064C24);

void func_80064CA4(u8 r, u8 g, u8 b) {
    func_800512E4(r | (g << 8) | (b << 16));
}

void func_80064CDC(u16 a, u16 b) {
    func_800515B8();
    func_800512B8(a, b);
    D_8007BCB0 = a;
    func_80051610();
}

void func_80064D28(void) {
    TimHeader *tim = &D_800834D0;
    TimBlock *b;
    LoadImage((RECT *)&tim->clut.x, (u32 *)(tim + 1));
    b = &tim->clut;
    b = (TimBlock *)((u8 *)b + tim->clut.bnum);
    LoadImage((RECT *)&b->x, (u32 *)(b + 1));
    DrawSync(0);
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetFogNearFar);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetBackColor);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80064EA8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80064EB8);

INCLUDE_ASM("asm/us/main/nonmatchings/main", OuterProduct0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetLightMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetColorMatrix);

INCLUDE_ASM("asm/us/main/nonmatchings/main", RotTrans);

INCLUDE_ASM("asm/us/main/nonmatchings/main", RotTransPers4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetPolyFT4);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadTPage);

INCLUDE_ASM("asm/us/main/nonmatchings/main", LoadClut);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetSprt16);

void func_800651D8(void) {
    func_80027CB0();
    func_8003431C();
    func_80033690();
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80065208);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdDiskReady);

INCLUDE_ASM("asm/us/main/nonmatchings/main", CdGetDiskType);

u8 func_800654F8(Character *a, Character *t, MainCalcCmd *cmd, u8 kind, s32 e) {
    MainCalc c;

    c.unk0 = a;
    c.unk4 = t;
    c.unk8 = cmd;
    c.unkC = e;
    c.unk10 = 0;
    c.unk1A = 0;
    c.unk18 = 0;
    switch (kind) {
    case 10:
        if (func_800657F0(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            func_800658DC(&c);
            func_80065A44((Unk80065A44 *)&c);
            func_800659EC(&c);
            func_80065A80(&c);
            func_80065ADC((Unk80065ADC *)&c);
        }
        break;
    case 12:
        func_80065CBC(&c);
        break;
    case 13:
        if (func_80065850(&c)) {
            func_80065B14(&c);
        }
        break;
    case 69:
        if (func_800657F0(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            func_800659D0(&c);
            func_80065ADC((Unk80065ADC *)&c);
        }
        break;
    case 70:
        if (func_800657F0(&c)) {
            if (t->mp == t->maxMp) {
                goto full;
            }
            func_800659D0(&c);
            func_80065B9C((Unk80065ADC *)&c);
        }
        break;
    case 71:
        if (func_800657F0(&c)) {
            if (*(u32 *)&t->hp == *(u32 *)&t->maxHp) { /* hp and mp both full */
            full:
                c.unk10 |= 1;
                break;
            }
            func_80065BD4(&c);
        }
        break;
    case 72:
        if (func_80065850(&c)) {
            func_80065BFC(&c);
        }
        break;
    case 62:
    case 73:
        if (((cmd->unk0 >> 8) & 0x1FF) == 0x121) {
            return func_801F1A7C();
        }
        func_80065D40(&c);
        break;
    case 76:
        if (func_800657F0(&c)) {
            func_80065C8C(&c);
        }
        break;
    }
    return func_8006576C(&c);
}

s32 func_8006573C(Unk8006573C *a, s16 b) {
    if (a->unk12 < b) {
        return 0;
    }
    a->unk12 -= b;
    return 1;
}

u8 func_8006576C(MainCalc *c) {
    if (c->unk10 & 1) {
        c->unk11 |= 0x20;
        return 0;
    }
    if (c->unk18 > 0) {
        func_80065D90(c->unk4, c->unk18);
    }
    if (c->unk1A > 0) {
        func_80065DF4(c->unk4, c->unk1A);
    }
    return 1;
}

u8 func_800657F0(MainCalc *c) {
    if (func_800539FC(c->unk4, 0x41) || c->unk4->hp == 0) {
        c->unk10 |= 1;
        return 0;
    }
    return 1;
}

u8 func_80065850(MainCalc *c) {
    if (func_800539FC(c->unk4, 1) || c->unk4->hp != 0 ||
        (func_800539FC(c->unk4, 0x40) && c->unk4->hp == 0)) {
        c->unk10 |= 1;
        return 0;
    }
    return 1;
}

void func_800658DC(MainCalc *c) {
    Character *a;
    s32 lv;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    a = c->unk0;
    lv = a->unk26;
    c->unk16 = lv + ((*g_randPtr << 8) | *g_randPtr) % (((a->level + lv) >> 3) + 1);
    g_randPtr++;
    c->unk12 = c->unk8->unk5;
    c->unk14 = c->unk4->unk2A;
}

void func_800659D0(MainCalc *c) {
    c->unk16 = 10;
    c->unk12 = c->unk8->unk5;
    c->unk14 = 0;
}

void func_800659EC(MainCalc *c) {
    if (func_800539FC(c->unk0, 0)) {
        c->unk16 /= 2;
    }
}

void func_80065A44(Unk80065A44 *a) {
    if (a->unk0->unk8C & 2) {
        a->unk16 = (a->unk16 * 3) >> 1;
    }
}

void func_80065A80(MainCalc *c) {
    if (c->unkC == 1 && (u32)((c->unk8->unk0 & 0xF) - 3) < 3) {
        c->unk16 /= 2;
    }
}

void func_80065ADC(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk18 = v;
}

void func_80065B14(MainCalc *c) {
    Character *t = c->unk4;
    s32 v = t->maxHp * (t->unk27 + c->unk8->unk5);

    if (c->unk0->unk88[1] & 2) {
        v /= 50;
    } else {
        v /= 100;
    }
    if (v >= 10000) {
        v = 9999;
    }
    c->unk18 = v;
}

void func_80065B9C(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk1A = v;
}

void func_80065BD4(MainCalc *c) {
    c->unk4->hp = c->unk4->maxHp;
    c->unk4->mp = c->unk4->maxMp;
}

void func_80065BFC(MainCalc *c) {
    Character *t = c->unk4;
    s32 r;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = *g_randPtr++;
    t->hp = r % 10 + 1;
}

void func_80065C8C(MainCalc *c) {
    c->unk18 = c->unk4->maxHp >> 1;
    c->unk1A = c->unk4->maxMp >> 1;
}

void func_80065CBC(MainCalc *c) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (func_8005398C(c->unk4, masks[c->unk8->unk9]) != 2) {
        c->unk10 |= 1;
    }
}

void func_80065D40(MainCalc *c) {
    if (func_8005398C(c->unk4, c->unk8->unk8) != 2) {
        c->unk10 |= 1;
    }
}

void func_80065D90(Character *u, s32 n) {
    if (!func_800539FC(u, 1)) {
        u->hp += n;
        if (u->hp > u->maxHp) {
            u->hp = u->maxHp;
        }
    }
}

void func_80065DF4(Character *u, s32 n) {
    if (!func_800539FC(u, 1)) {
        u->mp += n;
        if (u->mp > u->maxMp) {
            u->mp = u->maxMp;
        }
    }
}

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80065E60);

INCLUDE_ASM("asm/us/main/nonmatchings/main", firstfile);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_8006605C);

INCLUDE_ASM("asm/us/main/nonmatchings/main", firstfile2);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_load);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_clear);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_write);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _new_card);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_800661E0);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_format);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_read);

INCLUDE_ASM("asm/us/main/nonmatchings/main", _card_status);

INCLUDE_ASM("asm/us/main/nonmatchings/main", SetLineF2);


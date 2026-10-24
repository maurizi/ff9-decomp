#include "ff9.h"

void func_8005658C(s32 *src, Unk8005EBB4 *dst, u32 base, s32 n) {
    s32 *d = (s32 *)dst;

    do {
        d[0] = src[0] + base;
        d[1] = src[1] + base;
        d[2] = src[2];
        d[3] = src[3];
        src += 4;
        d += 4;
    } while (--n != 0);
}

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
    SpuWrite(a, b);
}

s32 func_80056694(s32 a, s32 b) {
    func_80056614();
    return SpuRead(a, b);
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

s32 func_80056700(s32 bank, s32 wait) {
    if (func_800565DC((s32 *)bank) != 0) {
        return -1;
    }
    func_80056750((s32 *)bank, wait, ((s32 *)bank)[6], ((s32 *)bank)[4]);
    return 0;
}

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

void akao_initState(void) {
    u16 i;
    u16 *cnt;
    AkaoChannel *c;
    u8 *p; /* &channel->unk88: the original's first-loop pointer is biased by 0x88 */
    s32 v;

    g_akaoSeq = &g_akaoMainSeq;
    cnt = (u16 *)0x1F801DAA;
    v = 0x18;
    D_8007F790 = NULL;
    D_800809EC = g_akaoChannels;
    D_8007F71C = NULL;
    D_8007F8D8[1] = 0;
    D_8007F8D8[0] = 0;
    D_800809C0[5] = 0;
    D_800809C0[4] = 0;
    D_800809C0[3] = 0;
    D_800809C0[2] = 0;
    D_800809C0[1] = 0;
    D_8008314C = 0;
    D_800809C0[0] = 0;
    D_8007F8E0 = 1;
    D_80083158.unk0 = 0;
    D_80083158.unk4 = 1;
    D_80080A70.unk0 = 0;
    g_akaoMainSeq.unk4 = 0;
    g_akaoMainSeq.unk8 = 0;
    g_akaoMainSeq.unk6A = 0;
    D_80080A70.unk10 = 0;
    g_akaoMainSeq.unk1C = 0;
    D_8007F8F0.unk6A = 0;
    D_8007F8F0.unk4 = 0;
    g_akaoMainSeq.unk50 = 0x7F0000;
    g_akaoMainSeq.unk5C = 0x400000;
    g_akaoCdVolume = 0x7FFF0000;
    D_8007F96C = 0;
    D_80083150 = 0;
    g_akaoMainSeq.unk58 = 0;
    g_akaoMainSeq.unk64 = 0;
    D_8007F96E = 0;
    D_80083154 = 0;
    g_akaoCdVolumeFadeTicks = 0;
    p = (u8 *)g_akaoChannels + 0x88;
    i = *cnt;
    D_80080A70.unk1C = 0;
    g_akaoMainSeq.unk3C = 0;
    D_80080A70.unk20 = 0;
    g_akaoMainSeq.unk40 = 0;
    D_80080A70.unk24 = 0;
    g_akaoMainSeq.unk44 = 0;
    g_akaoMainSeq.unk74 = 0;
    g_akaoMainSeq.unk72 = 0;
    g_akaoMainSeq.unk70 = 0;
    g_akaoMainSeq.unk78 = 0;
    *(u16 *)0x1F801D80 = 0x3FFF;
    *(u16 *)0x1F801D82 = 0x3FFF;
    *(u16 *)0x1F801DB0 = 0x7FFF;
    *(u16 *)0x1F801DB2 = 0x7FFF;
    D_800809B8 = 0;
    D_80080AC4 = 0;
    D_800831C4 = 0;
    D_80083168.unk40 = 0x7F00;
    D_80083168.unk48 = 0;
    D_80083238[2] = 0;
    D_80083238[1] = 0;
    D_80083238[0] = 0;
    D_80080A20 = 0;
    /* The nested do-while(0)s emit no code. flow weights references inside loop notes by depth, so they
     * raise cnt's reference count until local-alloc ranks it above &g_akaoChannels and gives it a2. The
     * notes are also scheduling barriers, which is why the loop below is a goto loop with an explicit
     * pointer and constant (a for loop's preheader insns would be stuck below the barrier). */
    do { do { do { do { do {
        *cnt = (i & ~5) | 1;
        i = 0;
    } while (0); } while (0); } while (0); } while (0); } while (0);
loop1:
    i++;
    ((AkaoChannel *)(p - 0x88))->unk34 = 0;
    v &= 0x7FFFFFFF; /* no code: combine turns it into a self-move, so v stays multi-set (no sched1 boost) */
    ((AkaoChannel *)(p - 0x88))->voice = v;
    ((AkaoChannel *)(p - 0x88))->unk94 = 0;
    ((AkaoChannel *)(p - 0x88))->unk88 = 0;
    p += sizeof(AkaoChannel);
    if (i < 32) {
        goto loop1;
    }
    c = D_8007E3D8;
    for (i = 8; i < 24;) {
        c->unk34 = 0;
        c->voice = i;
        i++;
        c->unk94 = 1;
        c->unk88 = 0;
        c->unk100 = 0x7F00;
        c->unkBC = 0;
        c->unkB6 = 0;
        c->unk70 = 0;
        c->unkBA = 0;
        c++;
    }
    g_akaoSeq->unk18 = 0;
    g_akaoSeq->unk14 = 0;
    g_akaoSeq->unk10 = 0;
    D_80080A70.unk18 = 1;
    D_80080A70.unk14.w = 0x66A80000;
    D_80080A70.unkC = 0;
    D_80080A70.unk8 = 0;
    D_80080A70.unk4 = 0;
    g_akaoSeq->unk48 = 0x3FFF000;
    g_akaoSeq->unk4C = 0;
    g_akaoSeq->unk66 = 0;
    D_80083158.unk8 |= 0x80;
    akao_setReverbMode(4);
    SpuSetReverb(1);
    for (i = 0; i < 24; i++) {
        akao_setVoiceLoopAddr(i, 0x1030);
    }
    D_8007E3C8[1] = 0;
    D_8007E3C8[0] = 0;
    D_8007E3D0 = 0;
}

void akao_init(void) {
    SpuStart();
    SpuInitMalloc(4, D_80080990);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1010);
    func_80056644(D_8006F334, 0x40);
    func_800566D0();
    akao_initState();
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
    akao_keyOff(0xFFFFFF);
    SpuQuit();
}

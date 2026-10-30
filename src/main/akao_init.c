#include "ff9.h"

void akao_relocInstruments(s32 *src, Unk8005EBB4 *dst, u32 base, s32 n) {
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

s32 akao_checkHeader(s32 *a) {
    return *a - AKAO_MAGIC;
}

void akao_onSpuTransferDone(void) {
    SpuSetTransferCallback(NULL);
    g_akaoSpuBusy = 0;
}

void akao_beginSpuTransfer(void) {
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_onSpuTransferDone);
}

void akao_spuWrite(void *a, s32 b) {
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_onSpuTransferDone);
    SpuWrite(a, b);
}

s32 akao_spuRead(s32 a, s32 b) {
    akao_beginSpuTransfer();
    return SpuRead(a, b);
}

void akao_waitSpuTransfer(void) {
    s32 a[2];
    do {
        do {
            a[0] = g_akaoSpuBusy;
            a[1] = g_akaoSpuBusy;
        } while (a[0] != a[1]);
    } while (a[0] == 1);
}

s32 akao_loadBank(s32 bank, s32 wait) {
    if (akao_checkHeader((s32 *)bank) != 0) {
        return -1;
    }
    akao_loadBankAt((s32 *)bank, wait, ((AkaoBankHeader *)bank)->slot, ((AkaoBankHeader *)bank)->spuAddr);
    return 0;
}

s32 akao_loadBankAt(s32 *buf, s32 wait, s32 slot, u32 addr) {
    AkaoBankHeader *h;

    akao_waitSpuTransfer();
    h = (AkaoBankHeader *)buf;
    if (h->magic == AKAO_MAGIC) {
        SpuSetTransferStartAddr(addr);
        buf += 16;
        akao_spuWrite(buf + h->instCount * 4, h->sampleSize);
        akao_relocInstruments(buf, &g_akaoInstruments[slot], addr, h->instCount);
        if (wait != 0) {
            akao_waitSpuTransfer();
        }
        return 0;
    }
    g_akaoSpuBusy = -1;
    return -1;
}

void akao_initState(void) {
    u16 i;
    u16 *cnt;
    AkaoChannel *c;
    u8 *p; /* &channel->unk88: the original's first-loop pointer is biased by 0x88 */
    s32 v;

    g_akaoSeq = &g_akaoMainSeq;
    cnt = (u16 *)SPU_CTRL;
    v = 0x18;
    g_akaoSubSeq = NULL;
    D_800809EC = g_akaoChannels;
    g_akaoSubChannels = NULL;
    D_8007F8D8[1] = 0;
    D_8007F8D8[0] = 0;
    g_akaoLoadedBanks[5] = 0;
    g_akaoLoadedBanks[4] = 0;
    g_akaoLoadedBanks[3] = 0;
    g_akaoLoadedBanks[2] = 0;
    g_akaoLoadedBanks[1] = 0;
    D_8008314C = 0;
    g_akaoLoadedBanks[0] = 0;
    D_8007F8E0 = 1;
    g_akaoFlags.unk0 = 0;
    g_akaoFlags.unk4 = 1;
    g_akaoSeState.unk0 = 0;
    g_akaoMainSeq.activeMask = 0;
    g_akaoMainSeq.priorityMask = 0;
    g_akaoMainSeq.musicId = 0;
    g_akaoSeState.unk10 = 0;
    g_akaoMainSeq.pausedMask = 0;
    g_akaoSavedSeq.musicId = 0;
    g_akaoSavedSeq.activeMask = 0;
    g_akaoMainSeq.volume = 0x7F0000;
    g_akaoMainSeq.pan = 0x400000;
    g_akaoCdVolume = 0x7FFF0000;
    D_8007F96C = 0;
    D_80083150 = 0;
    g_akaoMainSeq.volumeFadeLen = 0;
    g_akaoMainSeq.panSlideLen = 0;
    D_8007F96E = 0;
    D_80083154 = 0;
    g_akaoCdVolumeFadeTicks = 0;
    p = (u8 *)g_akaoChannels + 0x88;
    i = *cnt;
    g_akaoSeState.unk1C = 0;
    g_akaoMainSeq.noiseMask = 0;
    g_akaoSeState.unk20 = 0;
    g_akaoMainSeq.reverbMask = 0;
    g_akaoSeState.unk24 = 0;
    g_akaoMainSeq.pitchModMask = 0;
    g_akaoMainSeq.ticksPerBeat = 0;
    g_akaoMainSeq.beat = 0;
    g_akaoMainSeq.beatsPerMeasure = 0;
    g_akaoMainSeq.measure = 0;
    SPU_REG16(SPU_MAIN_VOL_L) = 0x3FFF;
    SPU_REG16(SPU_MAIN_VOL_R) = 0x3FFF;
    SPU_REG16(SPU_CD_VOL_L) = 0x7FFF;
    SPU_REG16(SPU_CD_VOL_R) = 0x7FFF;
    D_800809B8 = 0;
    D_80080AC4 = 0;
    D_800831C4 = 0;
    g_akaoStream.unk40 = 0x7F00;
    g_akaoStream.unk48 = 0;
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
    ((AkaoChannel *)(p - 0x88))->flags = 0;
    v &= 0x7FFFFFFF; /* no code: combine turns it into a self-move, so v stays multi-set (no sched1 boost) */
    ((AkaoChannel *)(p - 0x88))->voice = v;
    ((AkaoChannel *)(p - 0x88))->isSe = 0;
    ((AkaoChannel *)(p - 0x88))->age = 0;
    p += sizeof(AkaoChannel);
    if (i < 32) {
        goto loop1;
    }
    c = g_akaoSeChannels;
    for (i = 8; i < 24;) {
        c->flags = 0;
        c->voice = i;
        i++;
        c->isSe = 1;
        c->age = 0;
        c->seVolume = 0x7F00;
        c->seVolumeSlideLen = 0;
        c->sePitchSlideLen = 0;
        c->sePitch = 0;
        c->noteVolumeSlideLen = 0;
        c++;
    }
    g_akaoSeq->keyOffMask = 0;
    g_akaoSeq->playingMask = 0;
    g_akaoSeq->keyOnMask = 0;
    g_akaoSeState.unk18 = 1;
    g_akaoSeState.unk14.w = 0x66A80000;
    g_akaoSeState.unkC = 0;
    g_akaoSeState.unk8 = 0;
    g_akaoSeState.unk4 = 0;
    g_akaoSeq->reverbDepth = 0x3FFF000;
    g_akaoSeq->reverbDepthStep = 0;
    g_akaoSeq->reverbDepthSlideLen = 0;
    g_akaoFlags.unk8 |= 0x80;
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
    akao_spuWrite(D_8006F334, 0x40);
    akao_waitSpuTransfer();
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
    if (g_akaoSpuBusy == 1) {
        akao_spuWrite(D_8006F334, 0x40);
        akao_waitSpuTransfer();
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

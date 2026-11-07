#include "ff9.h"

void akao_relocInstruments(s32 *src, AkaoInstrument *dst, u32 base, s32 count) {
    s32 *out = (s32 *)dst;

    do {
        out[0] = src[0] + base;
        out[1] = src[1] + base;
        out[2] = src[2];
        out[3] = src[3];
        src += 4;
        out += 4;
    } while (--count != 0);
}

s32 akao_checkHeader(s32 *data) {
    return *data - AKAO_MAGIC;
}

void akao_onSpuTransferDone(void) {
    SpuSetTransferCallback(NULL);
    g_akaoSpuBusy = 0;
}

void akao_beginSpuTransfer(void) {
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_onSpuTransferDone);
}

void akao_spuWrite(void *data, s32 size) {
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_onSpuTransferDone);
    SpuWrite(data, size);
}

s32 akao_spuRead(s32 data, s32 size) {
    akao_beginSpuTransfer();
    return SpuRead(data, size);
}

void akao_waitSpuTransfer(void) {
    s32 busy[2];
    do {
        do {
            busy[0] = g_akaoSpuBusy;
            busy[1] = g_akaoSpuBusy;
        } while (busy[0] != busy[1]);
    } while (busy[0] == 1);
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
    AkaoChannel *chan;
    u8 *biasedChan; /* &channel->unk88: the original's first-loop pointer is biased by 0x88 */
    s32 voiceVal;

    g_akaoSeq = &g_akaoMainSeq;
    cnt = (u16 *)SPU_CTRL;
    voiceVal = AKAO_NO_VOICE;
    g_akaoSubSeq = NULL;
    g_akaoChannelsPtr = g_akaoChannels;
    g_akaoSubChannels = NULL;
    g_akaoInstrumentSets[1] = 0;
    g_akaoInstrumentSets[0] = 0;
    g_akaoLoadedBanks[5] = 0;
    g_akaoLoadedBanks[4] = 0;
    g_akaoLoadedBanks[3] = 0;
    g_akaoLoadedBanks[2] = 0;
    g_akaoLoadedBanks[1] = 0;
    g_akaoRandIndex = 0;
    g_akaoLoadedBanks[0] = 0;
    g_akaoInitDone = 1;
    g_akaoFlags.bankUpload = 0;
    g_akaoFlags.mode = AKAO_MODE_STEREO;
    g_akaoSeState.activeMask = 0;
    g_akaoMainSeq.activeMask = 0;
    g_akaoMainSeq.priorityMask = 0;
    g_akaoMainSeq.musicId = 0;
    g_akaoSeState.pausedMask = 0;
    g_akaoMainSeq.pausedMask = 0;
    g_akaoSavedSeq.musicId = 0;
    g_akaoSavedSeq.activeMask = 0;
    g_akaoMainSeq.volume = 0x7F0000;
    g_akaoMainSeq.pan = 0x400000;
    g_akaoCdVolume = 0x7FFF0000;
    g_akaoPitchScaleLen = 0;
    g_akaoPitchScale = 0;
    g_akaoMainSeq.volumeFadeLen = 0;
    g_akaoMainSeq.panSlideLen = 0;
    g_akaoTempoScaleLen = 0;
    g_akaoTempoScale = 0;
    g_akaoCdVolumeFadeTicks = 0;
    biasedChan = (u8 *)g_akaoChannels + 0x88;
    i = *cnt;
    g_akaoSeState.noiseMask = 0;
    g_akaoMainSeq.noiseMask = 0;
    g_akaoSeState.reverbMask = 0;
    g_akaoMainSeq.reverbMask = 0;
    g_akaoSeState.pitchModMask = 0;
    g_akaoMainSeq.pitchModMask = 0;
    g_akaoMainSeq.ticksPerBeat = 0;
    g_akaoMainSeq.beat = 0;
    g_akaoMainSeq.beatsPerMeasure = 0;
    g_akaoMainSeq.measure = 0;
    SPU_REG16(SPU_MAIN_VOL_L) = SPU_MAIN_VOL_MAX;
    SPU_REG16(SPU_MAIN_VOL_R) = SPU_MAIN_VOL_MAX;
    SPU_REG16(SPU_CD_VOL_L) = SPU_CD_VOL_MAX;
    SPU_REG16(SPU_CD_VOL_R) = SPU_CD_VOL_MAX;
    g_akaoSkipMeasures = 0;
    g_akaoMuteMask = 0;
    g_akaoPauseFlags = 0;
    g_akaoStream.volume = 0x7F00;
    g_akaoStream.volumeFadeLen = 0;
    g_akaoVoiceModeMasks[2] = 0;
    g_akaoVoiceModeMasks[1] = 0;
    g_akaoVoiceModeMasks[0] = 0;
    g_akaoCrossfade.fadeLen = 0;
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
    ((AkaoChannel *)(biasedChan - 0x88))->flags = 0;
    voiceVal &= 0x7FFFFFFF; /* no code: combine turns it into a self-move, so voiceVal stays multi-set (no sched1 boost) */
    ((AkaoChannel *)(biasedChan - 0x88))->voice = voiceVal;
    ((AkaoChannel *)(biasedChan - 0x88))->isSe = 0;
    ((AkaoChannel *)(biasedChan - 0x88))->age = 0;
    biasedChan += sizeof(AkaoChannel);
    if (i < AKAO_NUM_CHANNELS) {
        goto loop1;
    }
    chan = g_akaoSeChannels;
    for (i = AKAO_SE_FIRST_VOICE; i < AKAO_SE_FIRST_VOICE + AKAO_NUM_SE_CHANNELS;) {
        chan->flags = 0;
        chan->voice = i;
        i++;
        chan->isSe = 1;
        chan->age = 0;
        chan->seVolume = 0x7F00;
        chan->seVolumeSlideLen = 0;
        chan->sePitchSlideLen = 0;
        chan->sePitch = 0;
        chan->noteVolumeSlideLen = 0;
        chan++;
    }
    g_akaoSeq->keyOffMask = 0;
    g_akaoSeq->playingMask = 0;
    g_akaoSeq->keyOnMask = 0;
    g_akaoSeState.tempoAcc = 1;
    g_akaoSeState.tempo.w = 0x66A80000;
    g_akaoSeState.keyOffMask = 0;
    g_akaoSeState.playingMask = 0;
    g_akaoSeState.keyOnMask = 0;
    g_akaoSeq->reverbDepth = 0x3FFF000;
    g_akaoSeq->reverbDepthStep = 0;
    g_akaoSeq->reverbDepthSlideLen = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_REVERB_DEPTH;
    akao_setReverbMode(4);
    SpuSetReverb(1);
    for (i = 0; i < SPU_NUM_VOICES; i++) {
        akao_setVoiceLoopAddr(i, AKAO_SPU_SILENCE_LOOP);
    }
    g_akaoVsyncs[1] = 0;
    g_akaoVsyncs[0] = 0;
    g_akaoTickPhase = 0;
}

void akao_init(void) {
    SpuStart();
    SpuInitMalloc(4, g_akaoSpuMallocTable);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(AKAO_SPU_SILENCE);
    akao_spuWrite(g_akaoSilenceSample, 0x40);
    akao_waitSpuTransfer();
    akao_initState();
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    while (SetRCnt(RCntCNT2, AKAO_TICK_RCNT_TARGET, RCntMdINTR) == 0) {
    }
    while (StartRCnt(RCntCNT2) == 0) {
    }
    while ((g_akaoTickEvent = OpenEvent(RCntCNT2, EvSpINT, EvMdINTR, (void (*)(void))akao_tick)) == -1) {
    }
    while (EnableEvent(g_akaoTickEvent) == 0) {
    }
}

void akao_quit(void) {
    if (g_akaoSpuBusy == 1) {
        akao_spuWrite(g_akaoSilenceSample, 0x40);
        akao_waitSpuTransfer();
    }
    while (StopRCnt(RCntCNT2) == 0) {
    }
    UnDeliverEvent(RCntCNT2, EvSpINT);
    while (DisableEvent(g_akaoTickEvent) == 0) {
    }
    while (CloseEvent(g_akaoTickEvent) == 0) {
    }
    akao_keyOff(SPU_VOICE_ALL);
    SpuQuit();
}

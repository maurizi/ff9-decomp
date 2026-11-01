#include "ff9.h"

void akao_resetChannel(AkaoChannel *c, u8 *pc) {
    c->expression = 0x6E00;
    c->volume = 0x23000000;
    c->pc = pc;
    c->transpose = 0;
    c->fineTune = 0;
    c->portamentoLen = 0;
    c->pitchSlide = 0;
    c->slideKeys = 0;
    c->pitchSlideLeft = 0;
    c->fixedLen = 0;
    c->noteLen = 0;
    c->volumeSlideLen = 0;
    c->fineTuneOfs = 0;
    c->pitchRandom = 0;
    c->loopDepth = 0;
    c->flags = 0;
    c->panLfoOut = 0;
    c->legato = 0;
    c->opCount = 0xFFFF;
    c->voiceVolScale = 0;
    c->panLfoDepth = 0;
    c->tremoloDepth = 0;
    c->vibratoDepthParam = 0;
    c->panLfoDepthSlideLen = 0;
    c->tremoloDepthSlideLen = 0;
    c->vibratoDepthSlideLen = 0;
    c->panLfoPeriodSlideLen = 0;
    c->tremoloPeriodSlideLen = 0;
    c->vibratoPeriodSlideLen = 0;
    c->pitchModTimer = 0;
    c->noiseTimer = 0;
    akao_setChannelProgram(c, 0);
}

u32 akao_getVoiceMask(AkaoChannel *c, u32 chBits) {
    u32 r;
    u32 i;
    r = i = 0;
    for (; i < 32; i++) {
        if (chBits & (1 << i)) {
            if (c->voice < 0x18) {
                r |= 1 << c->voice;
            }
        }
        c++;
    }
    return r;
}

void akao_startSeq(s32 addr, s32 channelMask) {
    AkaoSeqHeader *seq;
    u32 active;
    AkaoChannel *c;
    u16 *ofs;
    u32 bit;
    u32 i;
    u32 x;
    u8 *p;

    g_akaoSeq->data = (u8 *)addr;
    active = ((AkaoSeqHeader *)addr)->channelMask;
    seq = (AkaoSeqHeader *)addr;
    if (g_akaoSubSeq != NULL) {
        bit = akao_getVoiceMask(g_akaoSubChannels, g_akaoSubSeq->activeMask);
    } else {
        bit = 0;
    }
    x = 0xFFFFFF;
    g_akaoSeState.keyOffMask |= ~bit & (~(g_akaoSeState.activeMask | g_akaoStreamVoices) & x);
    g_akaoSeq->keyOffMask = 0;
    if (g_akaoPauseFlags & AKAO_PAUSED_MUSIC) {
        g_akaoSeq->activeMask = 0;
        g_akaoSeq->pausedMask |= active & channelMask;
    } else {
        g_akaoSeq->pausedMask = 0;
        g_akaoSeq->activeMask |= active & channelMask;
    }
    g_akaoSeq->priorityMask = seq->priorityMask;
    g_akaoSeq->keepVoiceMask = seq->keepVoiceMask;
    g_akaoSeq->flags &= ~(AKAO_SEQ_NO_VOICE | AKAO_SEQ_VOICE_STOLEN | AKAO_SEQ_INSTR_MAIN | AKAO_SEQ_INSTR_ALT);
    g_akaoSeq->flags = seq->instrumentSet == g_akaoAltInstrumentSet ? g_akaoSeq->flags | AKAO_SEQ_INSTR_ALT : g_akaoSeq->flags | AKAO_SEQ_INSTR_MAIN;
    p = (u8 *)seq + (seq->regionTableOfs + 0x30);
    g_akaoSeq->regionTable = seq->regionTableOfs != 0 ? (u16 *)p : NULL;
    p = (u8 *)seq + (seq->drumKitOfs + 0x34);
    g_akaoSeq->drumKit = seq->drumKitOfs != 0 ? (s32)p : 0;
    g_akaoSeq->firstVoice = 0;
    bit = 1;
    i = 0;
    c = g_akaoChannels;
    ofs = (u16 *)(addr + 0x40);
    for (; i < 32; i++) {
        if (active & bit & channelMask) {
            c->pc = (u8 *)ofs + *ofs;
            c->noteTicks = 4;
            c->gateTicks = 2;
            c->expression = 0x7F00;
            c->volume = 0x3FFF0000;
            c->seVolume = 0x4000;
            c->fineTune = 0;
            c->transpose = 0;
            c->portamentoLen = 0;
            c->pitchSlide = 0;
            c->slideKeys = 0;
            c->pitchSlideLeft = 0;
            c->fixedLen = 0;
            c->noteLen = 0;
            c->pan = 0x8000;
            c->panSlideLen = 0;
            c->portamentoLen = 0;
            c->seVolumeSlideLen = 0;
            c->volumeSlideLen = 0;
            c->fineTuneOfs = 0;
            c->noteVolumeSlideLen = 0;
            c->pitchRandom = 0;
            c->legato = 0;
            c->panLfoOut = 0;
            c->loopDepth = 0;
            c->flags = !(g_akaoSeq->keepVoiceMask & bit) << 6;
            ofs++;
            c->panLfoDepth = 0;
            c->tremoloDepth = 0;
            c->vibratoDepthParam = 0;
            c->panLfoDepthSlideLen = 0;
            c->tremoloDepthSlideLen = 0;
            c->vibratoDepthSlideLen = 0;
            c->panLfoPeriodSlideLen = 0;
            c->tremoloPeriodSlideLen = 0;
            c->vibratoPeriodSlideLen = 0;
            c->pitchModTimer = 0;
            c->noiseTimer = 0;
            akao_setChannelProgram(c, 0);
        } else {
            if ((active & bit) && !(bit & channelMask)) {
                ofs++;
            }
            c->noteTicks = 3;
            c->gateTicks = 1;
            c->pc = g_akaoEmptyTrack;
            c->voiceDirty |= AKAO_VD_RELEASE_MODE | AKAO_VD_RELEASE_RATE;
            c->voiceAdsr2 = (c->voiceAdsr2 & ~SPU_ADSR2_RELEASE_RATE) | 5;
        }
        c->voice = 0x18;
        active &= ~bit;
        c++;
        bit <<= 1;
    }
    g_akaoSeq->tempo.w = 0xFFFF0000;
    g_akaoSeq->tempoAcc = 1;
    g_akaoSeq->tempoSlideLen = 0;
    g_akaoSeq->reverbDepth = 0;
    g_akaoSeq->reverbDepthSlideLen = 0;
    g_akaoSeq->reverbDepthStep = 0;
    g_akaoFlags.dirty = 0;
    g_akaoSeq->tick = 0;
    g_akaoSeq->ticksPerBeat = 0;
    g_akaoSeq->beat = 0;
    g_akaoSeq->measure = 0;
    g_akaoSeq->noiseMask = 0;
    g_akaoSeq->reverbMask = 0;
    g_akaoSeq->pitchModMask = 0;
    g_akaoSeq->condition = 0;
    g_akaoSeq->playingMask = 0;
    g_akaoSeq->keyOnMask = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_stopSeq(AkaoSeq *a, AkaoChannel *c, s32 id) {
    u32 i;
    if (a->activeMask != 0 && (id == 0 || id == a->musicId)) {
        a->keyOffMask = -1;
        for (i = 32; i != 0; i--) {
            c->noteTicks = 3;
            c->gateTicks = 1;
            c->pc = g_akaoEmptyTrack;
            c++;
        }
        a->musicId = 0;
        a->playingMask = 0;
        a->keyOnMask = 0;
        for (i = 0; i < 24; i++) {
            if (g_akaoVoiceOwner[i] == a) {
                g_akaoVoiceOwner[i] = NULL;
                akao_setVoiceRelease(i, 5, AKAO_ENV_LINEAR_DEC);
            }
        }
    }
}

void akao_stopSeChannels(s32 id, s32 seFilter) {
    AkaoChannel *c;
    u32 bit;
    u32 active;
    u32 i;
    s32 max;

    bit = 0x100;
    c = g_akaoSeChannels;
    active = g_akaoSeState.activeMask | g_akaoSeState.pausedMask;
    if (seFilter & 0x0FFFFFFF) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & seFilter)) {
                if (c->flags & AKAO_CH_STOP_AT_LOOP) {
                    c->flags |= AKAO_CH_STOP_REQUESTED;
                } else {
                    g_akaoSeState.keyOffMask |= bit;
                    akao_stopSeChannel(c, bit);
                    c->flags = 0;
                }
            }
        }
    } else if (seFilter < 0) {
        c = &g_akaoSeChannels[id];
        bit = 0x100 << id;
        if (active & bit) {
            akao_stopSeChannels(c->seHandle, 0);
        }
        bit <<= 1;
        c++;
        if (active & bit) {
            akao_stopSeChannels(c->seHandle, 0);
        }
        return;
    } else if (seFilter & 0x40000000) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (c->seMask != 0) {
                active &= ~bit;
            }
        }
        c = g_akaoSeChannels;
        bit = 0x100;
        max = 0;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (max < c->age) {
                    max = c->age;
                }
            }
        }
        c = g_akaoSeChannels;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && max == c->age) {
                if (c->flags & AKAO_CH_STOP_AT_LOOP) {
                    c->flags |= AKAO_CH_STOP_REQUESTED;
                } else {
                    g_akaoSeState.keyOffMask |= bit;
                    akao_stopSeChannel(c, bit);
                    c->flags = 0;
                }
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (id == -1) {
                    if (c->seHandle < 0) {
                        if (c->flags & AKAO_CH_STOP_AT_LOOP) {
                            c->flags |= AKAO_CH_STOP_REQUESTED;
                        } else {
                            g_akaoSeState.keyOffMask |= bit;
                            akao_stopSeChannel(c, bit);
                            c->flags = 0;
                        }
                    }
                } else if (c->seHandle == id) {
                    if (c->flags & AKAO_CH_STOP_AT_LOOP) {
                        c->flags |= AKAO_CH_STOP_REQUESTED;
                    } else {
                        g_akaoSeState.keyOffMask |= bit;
                        akao_stopSeChannel(c, bit);
                        c->flags = 0;
                    }
                }
            }
        }
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_startSeChannel(AkaoChannel *c, VoiceParams *p, s32 chBit, u8 *data) {
    AkaoChannel *ch;
    s32 bit;
    s32 n;

    c->seHandle = p->handle;
    c->seMask = p->mask;
    c->sePan = p->pan << 8;
    c->sePanSlideLen = 0;
    c->pan = 0x8000;
    c->panSlideLen = 0;
    c->seVolume = (p->volume & 0x7F) << 8;
    c->seVolumeSlideLen = 0;
    c->seBank = p->bank;
    c->noteTicks = 2;
    c->gateTicks = 1;
    c->isSe = 1;
    c->age = -2;
    c->sePitch = 0;
    c->sePitchSlideLen = 0;
    akao_resetChannel(c, data);
    g_akaoVoiceOwner[c->voice] = NULL;
    akao_setVoiceRelease(c->voice, 5, AKAO_ENV_LINEAR_DEC);
    g_akaoSeState.activeMask |= chBit;
    g_akaoSeState.keyOffMask |= chBit;
    chBit = ~chBit;
    g_akaoSeState.keyOnMask &= chBit;
    g_akaoSeState.playingMask &= chBit;
    g_akaoSeState.noiseMask &= chBit;
    g_akaoSeState.reverbMask &= chBit;
    g_akaoSeState.pitchModMask &= chBit;
    if (g_akaoPauseFlags & AKAO_PAUSED_SE) {
        chBit = 0x100;
        c = g_akaoSeChannels;
        n = 16;
        do {
            if ((g_akaoSeState.activeMask & chBit) && !(c->seMask & AKAO_SE_DEFAULT)) {
                g_akaoSeState.activeMask &= ~chBit;
                g_akaoSeState.pausedMask |= chBit;
            }
            n--;
            c++;
            chBit <<= 1;
        } while (n != 0);
    }
}

void akao_releaseVoice(AkaoChannel *c, u32 voice) {
    u32 i;
    if (voice < 0x18) {
        for (i = 0; i < 32; i++) {
            if (c->voice == voice) {
                c->voice = 0x18;
                g_akaoSeq->playingMask &= ~(1 << i);
            }
            c++;
        }
    }
}

void akao_startSe(VoiceParams *p, u8 *track0, u8 *track1, s32 flag) {
    AkaoChannel *c;
    u32 bit;
    s32 n;
    u32 busy;

    if (track0 == NULL && track1 == NULL) {
        return;
    }
    if (flag == 0 && p->mask != 0) {
        akao_stopSeChannels(0, p->mask);
    }
    do {
        c = &g_akaoSeChannels[15];
        bit = 0x800000;
        busy = g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask;
        if (track0 != NULL && track1 != NULL) {
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
            akao_stopSeChannels(0, 0x40000000);
            if (busy == (g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask)) {
                return;
            }
        }
    } while (n == 0);
    if (track0 != NULL) {
        akao_startSeChannel(c, p, bit, track0);
        akao_releaseVoice(g_akaoChannels, c->voice);
    }
    if (track1 != NULL) {
        if (track0 != NULL) {
            c++;
            bit <<= 1;
        }
        akao_startSeChannel(c, p, bit, track1);
        akao_releaseVoice(g_akaoChannels, c->voice);
        if (track0 != NULL) {
            c->flags |= AKAO_CH_SE_SECOND;
        }
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

static inline u8 *akao_getSeData_get(s32 i) {
    return g_akaoSeTrackOfs[i] != 0xFFFF ? g_akaoSeData + g_akaoSeTrackOfs[i] : NULL;
}

void akao_getSeData(u8 **outTrack0, u8 **outTrack1, s32 idx) {
    idx &= 0x3FF;
    idx *= 2;
    *outTrack0 = akao_getSeData_get(idx);
    idx++;
    *outTrack1 = akao_getSeData_get(idx);
}

/* K&R definition: callers pass two or three arguments */
void akao_markSeqDirty(a, c)
    AkaoSeq *a;
    AkaoChannel *c;
{
    u32 pending;
    u32 bit;
    if (a->activeMask != 0) {
        pending = a->activeMask;
        bit = 1;
        do {
            if (pending & bit) {
                pending ^= bit;
                c->voiceDirty |= AKAO_VD_VOL;
            }
            c++;
            bit <<= 1;
        } while (pending != 0);
    }
}

void akao_markSeDirty(void) {
    u32 pending;
    u32 bit;
    AkaoChannel *c;
    if (g_akaoSeState.activeMask != 0) {
        pending = g_akaoSeState.activeMask;
        bit = 0x100;
        c = g_akaoSeChannels;
        do {
            if (pending & bit) {
                pending ^= bit;
                c->voiceDirty |= AKAO_VD_VOL;
            }
            c++;
            bit <<= 1;
        } while (pending != 0);
    }
}

void akao_resumeSeq(s32 addr, s32 restore) {
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
        akao_copy(&g_akaoSavedSeq, g_akaoSeq, 0x7C);
        akao_copy(g_akaoSavedChannels, g_akaoChannels, 0x2680);
    } else {
        akao_swap(&g_akaoSavedSeq, g_akaoSeq, 0x7C);
        akao_swap(g_akaoSavedChannels, g_akaoChannels, 0x2680);
    }
    flags = g_akaoSeq->flags & ~(AKAO_SEQ_INSTR_MAIN | AKAO_SEQ_INSTR_ALT);
    g_akaoSeq->flags = flags;
    g_akaoSeq->flags = seq->instrumentSet == g_akaoAltInstrumentSet ? flags | AKAO_SEQ_INSTR_ALT : flags | AKAO_SEQ_INSTR_MAIN;
    c = g_akaoChannels;
    n = 32;
    old = g_akaoSeq->data;
    g_akaoSeq->data = (u8 *)seq;
    g_akaoSeq->keyOnMask = 0;
    bit = 1;
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_REVERB_DEPTH;
    d = (u8 *)seq - old;
    active = g_akaoSeq->activeMask;
    g_akaoSeq->regionTable = (u16 *)((u8 *)g_akaoSeq->regionTable + d);
    g_akaoSeq->drumKit += d;
    g_akaoSeq->keyOnMask = g_akaoSeq->playingMask;
    do {
        if (active & bit) {
            c->pc += d;
            c->regions += d;
            c->loopStart[0] += d;
            c->loopStart[1] += d;
            c->loopStart[2] += d;
            c->loopStart[3] += d;
            c->noteTicks += 2;
            c->gateTicks += 2;
            c->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH | AKAO_VD_START_ADDR | AKAO_VD_ADSR1 | AKAO_VD_ADSR2 | AKAO_VD_LOOP_ADDR;
            akao_shiftInstrumentUp(g_akaoSeq->flags, c);
        } else {
            c->noteTicks = 4;
            c->gateTicks = 2;
            c->pc = g_akaoEmptyTrack;
        }
        c->voice = 0x18;
        n--;
        c++;
        bit <<= 1;
    } while (n != 0);
    if (g_akaoSubSeq != NULL) {
        active = akao_getVoiceMask(g_akaoSubChannels, g_akaoSubSeq->activeMask & g_akaoSubSeq->priorityMask);
    } else {
        active = 0;
    }
    g_akaoSeq->keyOffMask = 0;
    x = 0xFFFFFF;
    g_akaoSavedSeq.musicId = 0;
    g_akaoSeState.keyOffMask |= ~active & (~(g_akaoSeState.activeMask | g_akaoStream.voiceMask) & x);
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    if (g_akaoPauseFlags & AKAO_PAUSED_MUSIC) {
        g_akaoSeq->pausedMask = g_akaoSeq->activeMask;
        g_akaoSeq->activeMask = 0;
    }
}

s32 akao_findBankSlot(s32 bankId) {
    s32 i = 0;

    if (bankId != 0) {
        i = 7;
        if (bankId == g_akaoLoadedBanks[6]) {
            i = 6;
        } else {
        loop:
            i--;
            if (i != 0) {
                if (bankId != g_akaoLoadedBanks[i - 1]) {
                    goto loop;
                }
                i = i - 1;
            }
        }
    }
    return i;
}

void akao_cmdPlayMusic(AkaoArgs *args) {
    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == args->arg2) {
        akao_resumeSeq(args->arg0, 0);
    } else {
        akao_startSeq(args->arg0, -1);
        g_akaoSeq->musicId = args->arg2;
    }
}

void akao_cmdPlayMusicFrom(AkaoArgs *args) {
    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == args->arg2) {
        akao_resumeSeq(args->arg0, 0);
    } else {
        akao_startSeq(args->arg0, args->arg3);
        g_akaoSeq->musicId = args->arg2;
    }
}

void akao_cmdSaveMusic(void) {
    u32 i;
    AkaoChannel *c;

    if (g_akaoSeq->activeMask != 0) {
        akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        akao_copy(g_akaoChannels, g_akaoSavedChannels, 0x2680);
        for (i = 0, c = g_akaoSavedChannels; i < 0x20; i++, c++) {
            akao_shiftInstrumentDown(g_akaoSavedSeq.flags, c);
        }
    }
}

void akao_cmdPushMusic(AkaoArgs *args) {
    AkaoSeq *t;
    if (g_akaoSeq->activeMask != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->musicId == 0)) {
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
        akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        akao_copy(g_akaoChannels, g_akaoSubChannels, 0x2680);
    }
    akao_startSeq(args->arg0, -1);
    t = g_akaoSeq;
    t->volume = (args->arg4 & 0x7F) << 16;
    t->volumeFadeLen = 0;
    t->musicId = args->arg2;
    akao_freeSubSeqVoices();
}

void akao_cmdCrossfadeMusic(AkaoArgs *args) {
    s32 d;

    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == args->arg2) {
        akao_resumeSeq(args->arg0, 1);
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
    } else {
        if (g_akaoSeq->activeMask != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->musicId == 0)) {
            g_akaoSubSeq = &g_akaoSavedSeq;
            g_akaoSubChannels = g_akaoSavedChannels;
            akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
            akao_copy(g_akaoChannels, g_akaoSubChannels, 0x2680);
        }
        akao_startSeq(args->arg0, -1);
        g_akaoSeq->musicId = args->arg2;
    }
    if (g_akaoSubSeq != NULL) {
        d = args->arg4;
        g_akaoCrossfade.volume = 0x7F8000;
        g_akaoCrossfade.fadeLen = d;
        g_akaoFlags.mode |= AKAO_MODE_CROSSFADE;
        g_akaoCrossfade.volumeStep = -0x7F8000 / d;
    }
    g_akaoSeq->volume = 0;
    g_akaoSeq->volumeFadeLen = args->arg4;
    g_akaoSeq->volumeStep = (u32)(((args->arg5 & 0x7F) << 16) | 0x8000) / (u32)args->arg4;
    akao_freeSubSeqVoices();
}

void akao_cmdPlayMusicCount(AkaoArgs *args) {
    s32 r;
    akao_cmdPlayMusic(args);
    r = 0;
    if (args->arg3 != 0) {
        r = args->arg3 - 1;
    }
    g_akaoSkipMeasures = r;
}

void akao_cmdPlaySeData(AkaoArgs *args) {
    s32 a = args->arg0;
    s32 b = args->arg1;
    args->arg0 = 0x400;
    args->arg1 = 0x1000000;
    args->arg2 = 0x80;
    args->arg3 = 0x7F;
    args->arg4 = 0;
    akao_startSe(args, a, b, 0);
}

void akao_cmdPlaySeDefault(AkaoArgs *args) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = g_akaoSeExtraCounts[args->arg0];

    args->arg1 = AKAO_SE_DEFAULT;
    args->arg2 = 0x80;
    args->arg3 = 0x7F;
    args->arg4 = akao_findBankSlot(g_akaoSeBankIds[args->arg0]);
    akao_getSeData(&a, &b, args->arg0);
    akao_startSe(args, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            akao_getSeData(&a, &b, args->arg0 + i);
            n--;
            akao_startSe(args, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySe(AkaoArgs *args) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = g_akaoSeExtraCounts[args->arg0];

    args->arg4 = akao_findBankSlot(g_akaoSeBankIds[args->arg0]);
    akao_getSeData(&a, &b, args->arg0);
    akao_startSe(args, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            akao_getSeData(&a, &b, args->arg0 + i);
            n--;
            akao_startSe(args, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySeBank(AkaoArgs *args) {
    AkaoSeqHdr *h = (AkaoSeqHdr *)args->arg0;
    u32 *offs = h->trackOfs;
    s32 i;
    u8 *base;
    u16 *p;
    u8 *a;
    u8 *b;
    s32 n;

    args->arg4 = akao_findBankSlot(h->id);
    n = h->trackCount;
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
    akao_startSe(args, a, b, 0);
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
        akao_startSe(args, a, b, 1);
    }
}

void akao_cmdStopSe(s32 *args) {
    akao_stopSeChannels(args[0], args[1]);
}

void akao_cmdSetMusicVolume(AkaoArgs *args) {
    AkaoSeq *s;
    s32 v;

    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        s->volume = (args->arg1 & 0x7F) << 16;
        s->volumeFadeLen = 0;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && args->arg0 != 0 && args->arg0 == s->musicId) {
            v = args->arg1;
            s->volumeFadeLen = 0;
            s->volume = (v & 0x7F) << 16;
            akao_markSeqDirty(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicVolume(AkaoArgs *args) {
    s32 n = 1;
    AkaoSeq *s;
    AkaoSeq *t;
    s32 v;
    s32 cur;

    if (args->arg1 != 0) {
        n = args->arg1;
    }
    v = ((args->arg2 & 0x7F) << 16) | 0x8000;
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        do { } while (0);
        cur = s->volume & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        v /= n;
        s->volumeFadeLen = n;
        s->volume = cur;
        s->volumeStep = v;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        t = g_akaoSubSeq;
        if (t == NULL || args->arg0 == 0 || args->arg0 != t->musicId) {
            return;
        }
        cur = t->volume & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        do { } while (0);
        v /= n;
        s = t;
        s->volumeFadeLen = n;
        s->volume = cur;
        s->volumeStep = v;
        akao_markSeqDirty(s, g_akaoSubChannels);
    }
}

void akao_cmdFadeMusicVolumeFrom(AkaoArgs *args) {
    s32 n = 1;
    AkaoSeq *s;
    AkaoChannel *c;
    s32 v;

    if (args->arg1 != 0) {
        n = args->arg1;
    }
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        c = g_akaoChannels;
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || args->arg0 == 0 || args->arg0 != s->musicId) {
            return;
        }
        c = g_akaoSubChannels;
    }
    v = ((args->arg2 & 0x7F) << 16) | 0x8000;
    s->volume = v;
    s->volumeStep = ((((args->arg3 & 0x7F) << 16) | 0x8000) - v) / n;
    s->volumeFadeLen = n;
    akao_markSeqDirty(s, c);
}

void akao_cmdSetMusicPan(AkaoArgs *args) {
    AkaoSeq *s;
    s32 v;

    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        s->pan = (args->arg1 & 0x7F) << 16;
        s->panSlideLen = 0;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && args->arg0 != 0 && args->arg0 == s->musicId) {
            v = args->arg1;
            s->panSlideLen = 0;
            s->pan = (v & 0x7F) << 16;
            akao_markSeqDirty(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicPan(AkaoArgs *args) {
    s32 n = 1;
    AkaoSeq *s;
    s32 v;

    if (args->arg1 != 0) {
        n = args->arg1;
    }
    v = (args->arg2 & 0x7F) << 16;
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        v = v - s->pan;
        do { } while (0);
        v /= n;
        s->panSlideLen = n;
        s->panStep = v;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || args->arg0 == 0 || args->arg0 != s->musicId) {
            return;
        }
        v = v - s->pan;
        do { } while (0);
        v /= n;
        s->panSlideLen = n;
        s->panStep = v;
        akao_markSeqDirty(s, g_akaoSubChannels);
    }
}

void akao_cmdSetCdVolume(u16 *args) {
    g_akaoCdVolumeFadeTicks = 0;
    g_akaoCdVolume = *args << 16;
    akao_applyCdVolume();
}

void akao_cmdFadeCdVolume(AkaoArgs *args) {
    s32 n = 1;

    if (args->arg0 != 0) {
        n = args->arg0;
    }
    g_akaoCdVolumeStep = (((u16)args->arg1 << 16) - g_akaoCdVolume) / n;
    g_akaoCdVolumeFadeTicks = n;
}

void akao_cmdFadeCdVolumeFrom(AkaoArgs *args) {
    s32 n = 1;
    s32 v;

    if (args->arg0 != 0) {
        n = args->arg0;
    }
    g_akaoCdVolumeStep = (((u16)args->arg2 << 16) - (v = (u16)args->arg1 << 16)) / n;
    g_akaoCdVolumeFadeTicks = n;
    g_akaoCdVolume = v;
}

void akao_cmdSetSeVolume(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                c->seVolume = ((u16)args->arg2 & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                c->seVolume = ((u16)args->arg2 & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeSeVolume(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->seVolumeStep = (s16)((((u16)args->arg3 & 0x7F) << 8) - c->seVolume) / n;
                c->seVolumeSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->seVolumeStep = (s16)((((u16)args->arg3 & 0x7F) << 8) - c->seVolume) / n;
                c->seVolumeSlideLen = n;
            }
        }
    }
}

void akao_cmdSetAllSeVolume(u16 *args) {
    u32 bit = 0x100;
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (active & bit) {
            if (!(c->seMask & AKAO_SE_DEFAULT)) {
                c->seVolume = (*args & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeAllSeVolume(AkaoArgs *args) {
    u32 bit = 0x100;
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((active & bit) && !(c->seMask & AKAO_SE_DEFAULT)) {
            n = 1;
            if (args->arg0 != 0) {
                n = args->arg0;
            }
            c->seVolumeStep = (s16)((((u16)args->arg1 & 0x7F) << 8) - c->seVolume) / n;
            c->seVolumeSlideLen = n;
        }
    }
}

void akao_cmdSetSePan(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                c->sePan = (u8)args->arg2 << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                c->sePan = (u8)args->arg2 << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeSePan(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->sePanStep = (s16)(((u8)args->arg3 << 8) - c->sePan) / n;
                c->sePanSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->sePanStep = (s16)(((u8)args->arg3 << 8) - c->sePan) / n;
                c->sePanSlideLen = n;
            }
        }
    }
}

void akao_cmdSetAllSePan(u8 *args) {
    u32 bit = 0x100;
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (active & bit) {
            if (!(c->seMask & AKAO_SE_DEFAULT)) {
                c->sePan = *args << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeAllSePan(AkaoArgs *args) {
    u32 bit = 0x100;
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((active & bit) && !(c->seMask & AKAO_SE_DEFAULT)) {
            n = 1;
            if (args->arg0 != 0) {
                n = args->arg0;
            }
            c->sePanStep = (s16)(((u8)args->arg1 << 8) - c->sePan) / n;
            c->sePanSlideLen = n;
        }
    }
}

void akao_cmdSetSePitch(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                c->sePitch = (u8)args->arg2 << 8;
                c->sePitchSlideLen = 0;
                c->voiceDirty |= AKAO_VD_PITCH;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                c->sePitch = (u8)args->arg2 << 8;
                c->sePitchSlideLen = 0;
                c->voiceDirty |= AKAO_VD_PITCH;
            }
        }
    }
}

void akao_cmdFadeSePitch(AkaoArgs *args) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & args->arg1)) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->sePitchStep = (s16)((s16)(((u8)args->arg3 << 8) - (u16)c->sePitch) / n);
                c->sePitchSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && c->seHandle == args->arg0) {
                n = 1;
                if (args->arg2 != 0) {
                    n = args->arg2;
                }
                c->sePitchStep = (s16)((s16)(((u8)args->arg3 << 8) - (u16)c->sePitch) / n);
                c->sePitchSlideLen = n;
            }
        }
    }
}

void akao_cmdSetAllSePitch(u8 *args) {
    s32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 16; i != 0; i--) {
        if (!(c->seMask & AKAO_SE_DEFAULT)) {
            c->sePitch = *args << 8;
            c->sePitchSlideLen = 0;
            c->voiceDirty |= AKAO_VD_PITCH;
        }
        c++;
    }
}

void akao_cmdFadeAllSePitch(AkaoArgs *args) {
    u32 bit = 0x100;
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((active & bit) && !(c->seMask & AKAO_SE_DEFAULT)) {
            n = 1;
            if (args->arg0 != 0) {
                n = args->arg0;
            }
            c->sePitchStep = (s16)((s16)(((u8)args->arg1 << 8) - (u16)c->sePitch) / n);
            c->sePitchSlideLen = n;
        }
    }
}

void akao_cmdSetTempoScale(s8 *args) {
    s32 v = *args;
    g_akaoTempoScaleLen = 0;
    g_akaoTempoScale = v << 16;
}

void akao_cmdFadeTempoScale(AkaoArgs *args) {
    s32 n = 1;

    if (args->arg0 != 0) {
        n = args->arg0;
    }
    g_akaoTempoScaleStep = (((s8)args->arg1 << 16) - g_akaoTempoScale) / n;
    g_akaoTempoScaleLen = n;
}

void akao_cmdFadeTempoScaleFrom(AkaoArgs *args) {
    s32 n = 1;

    if (args->arg1 != 0) {
        n = args->arg0;
    }
    g_akaoTempoScale = (s8)args->arg1 << 16;
    g_akaoTempoScaleStep = (((s8)args->arg2 << 16) - g_akaoTempoScale) / n;
    g_akaoTempoScaleLen = n;
}

void akao_cmdSetPitchScale(s8 *args) {
    s32 v = *args;
    g_akaoPitchScaleLen = 0;
    g_akaoPitchScale = v << 16;
}

void akao_cmdFadePitchScale(AkaoArgs *args) {
    s32 n = 1;

    if (args->arg0 != 0) {
        n = args->arg0;
    }
    g_akaoPitchScaleStep = (((s8)args->arg1 << 16) - g_akaoPitchScale) / n;
    g_akaoPitchScaleLen = n;
}

void akao_cmdFadePitchScaleFrom(AkaoArgs *args) {
    s32 n = 1;

    if (args->arg1 != 0) {
        n = args->arg0;
    }
    g_akaoPitchScale = (s8)args->arg1 << 16;
    g_akaoPitchScaleStep = (((s8)args->arg2 << 16) - g_akaoPitchScale) / n;
    g_akaoPitchScaleLen = n;
}

void akao_cmdStopMusicNow(void) {
    akao_stopSeq(g_akaoSeq, g_akaoChannels, 0);
    if (g_akaoSubSeq != NULL) {
        akao_stopSeq(g_akaoSubSeq, g_akaoSubChannels, 0);
    }
}

void akao_cmdStopMusic(s32 *p) {
    akao_stopSeq(g_akaoSeq, g_akaoChannels, *p);
    if (g_akaoSubSeq != NULL && *p != 0) {
        akao_stopSeq(g_akaoSubSeq, g_akaoSubChannels, *p);
    }
}

void akao_cmdStopAllSe(void) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 bit = 0x100;
    u32 i;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((g_akaoSeState.activeMask & bit) && !(c->seMask & AKAO_SE_DEFAULT)) {
            g_akaoSeState.keyOffMask |= bit;
            akao_stopSeChannel(c, bit);
            c->flags = 0;
        }
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_cmdSetOutputMode1(void) {
    g_akaoFlags.mode = AKAO_MODE_STEREO;
    akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
    }
    akao_markSeDirty();
}

void akao_cmdSetOutputMode2(void) {
    g_akaoFlags.mode = AKAO_MODE_MONO;
    akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
    }
    akao_markSeDirty();
}

void akao_cmdSetMuteMask(s32 *args) {
    u32 i;
    AkaoChannel *ch;
    g_akaoMuteMask = *args;
    ch = g_akaoChannels;
    for (i = 0; i < 0x20; i++) {
        ch->voiceDirty |= AKAO_VD_VOL;
        ch++;
    }
}

void akao_cmdSetCondition(u16 *args) {
    g_akaoSeq->condition = *args;
}

void akao_cmdPauseMusic(void) {
    u32 m;
    u32 bit;
    s32 i;
    AkaoSeq *t;
    s32 v;
    if (g_akaoSeq->activeMask != 0) {
        m = ~(g_akaoSeState.activeMask | g_akaoStream.voiceMask) & 0xFFFFFF;
        if (m != 0) {
            bit = 1;
            i = 0;
            do {
                if (m & bit) {
                    akao_setVoiceVolume(i, 0, 0, 0);
                    akao_setVoicePitch(i, 0);
                    akao_setVoiceAttack(i, 0x7F, AKAO_ENV_LINEAR_INC);
                    akao_setVoiceSustain(i, 0x7F, AKAO_ENV_LINEAR_DEC);
                    m &= ~bit;
                }
                bit <<= 1;
                i++;
            } while (m != 0);
        }
        t = g_akaoSeq;
        v = t->activeMask;
        t->activeMask = 0;
        t->pausedMask = v;
    }
    g_akaoPauseFlags |= AKAO_PAUSED_MUSIC;
}

void akao_cmdResumeMusic(void) {
    AkaoChannel *ch;
    s32 bit;
    s32 pending;
    s32 t;

    t = g_akaoSeq->pausedMask;
    pending = t;
    if (t != 0) {
        bit = 1;
        ch = g_akaoChannels;
        do {
            if (pending & bit) {
                pending &= ~bit;
                ch->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH | AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE | AKAO_VD_SUSTAIN_MODE | AKAO_VD_SUSTAIN_RATE;
            }
            bit <<= 1;
            ch++;
        } while (pending != 0);
        t = g_akaoSeq->activeMask = g_akaoSeq->pausedMask;
        g_akaoSeq->pausedMask = 0;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
    g_akaoPauseFlags &= ~AKAO_PAUSED_MUSIC;
}

void akao_cmdPauseSe(void) {
    u32 m;
    u32 bit;
    u32 i;
    AkaoChannel *c;
    s32 v;

    if (g_akaoSeState.activeMask != 0) {
        m = g_akaoSeState.activeMask;
        c = g_akaoSeChannels;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((m & bit) && (c->seMask & AKAO_SE_DEFAULT)) {
                m &= ~bit;
            }
        }
        bit = 0x100;
        v = 8;
        g_akaoSeState.pausedMask = m;
        g_akaoSeState.activeMask &= ~m;
        while (m != 0) {
            if (m & bit) {
                akao_setVoiceVolume(v, 0, 0, 0);
                akao_setVoicePitch(v, 0);
                akao_setVoiceAttack(v, 0x7F, AKAO_ENV_LINEAR_INC);
                akao_setVoiceSustain(v, 0x7F, AKAO_ENV_LINEAR_DEC);
                m &= ~bit;
            }
            bit <<= 1;
            v++;
        }
    }
    g_akaoPauseFlags |= AKAO_PAUSED_SE;
}

void akao_cmdResumeSe(void) {
    u32 pending;
    u32 bit;
    AkaoChannel *c;
    s32 m;
    if (g_akaoSeState.pausedMask != 0) {
        pending = g_akaoSeState.pausedMask;
        bit = 0x100;
        c = g_akaoSeChannels;
        do {
            if (pending & bit) {
                pending &= ~bit;
                c->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH | AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE | AKAO_VD_SUSTAIN_MODE | AKAO_VD_SUSTAIN_RATE;
            }
            bit <<= 1;
            c++;
        } while (pending != 0);
        m = g_akaoSeState.pausedMask;
        g_akaoSeState.pausedMask = 0;
        g_akaoSeState.activeMask |= m;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
    g_akaoPauseFlags &= ~AKAO_PAUSED_SE;
}

void akao_cmdPauseStream(void) {
    if (g_akaoStream.voiceMask != 0) {
        akao_setVoicePitch(g_akaoStream.voice, 0);
        akao_setVoicePitch(g_akaoStream.voice + 1, 0);
        akao_setVoiceVolume(g_akaoStream.voice, 0, 0, 0);
        akao_setVoiceVolume(g_akaoStream.voice + 1, 0, 0, 0);
    }
}

void akao_cmdResumeStream(void) {
    AkaoStreamState *d = &g_akaoStream;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    if (d->voiceMask == 0) {
        return;
    }
    akao_setVoicePitch(d->voice, d->pitch);
    akao_setVoicePitch(d->voice + 1, d->pitch);
    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        v = d->volume * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(d->voice, v, v, 0);
        akao_setVoiceVolume(d->voice + 1, v, v, 0);
    } else if (d->flags & AKAO_STREAM_STEREO) {
        v = d->volume;
        v <<= 15;
        v >>= 16;
        akao_setVoiceVolume(d->voice, v, 0, 0);
        akao_setVoiceVolume(d->voice + 1, 0, v, 0);
    } else {
        pan = (u8)(d->pan >> 8);
        l = d->volume * g_akaoPanCurve[pan] >> 16;
        r = d->volume * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->voice, l, r, 0);
        akao_setVoiceVolume(d->voice + 1, l, r, 0);
    }
}

void akao_cmdNop(void) {
}

void akao_setReverbMode(s32 mode) {
    s32 t;
    SpuGetReverbModeType(&t);
    if (t != mode) {
        SpuSetReverb(0);
        SpuSetReverbModeType(mode | 0x100);
        SpuSetReverb(1);
    }
}

s32 akao_command(u32 cmd) {
    s32 r = 0;
    AkaoSeqHeader *seq;
    AkaoArgs *blk;

    DisableEvent(g_akaoTickEvent);
    blk = &g_akaoCommandBlock;
    cmd &= 0xFF;
    switch (cmd) {
    case AKAO_CMD_PLAY_MUSIC:
    case AKAO_CMD_PLAY_MUSIC_COUNT:
    case AKAO_CMD_PLAY_MUSIC_FROM:
    case AKAO_CMD_PUSH_MUSIC:
    case AKAO_CMD_CROSSFADE_MUSIC:
        if (akao_checkHeader((s32 *)g_akaoArgs.arg0) == 0) {
        seq = (AkaoSeqHeader *)g_akaoArgs.arg0;
        if (g_akaoSeq->musicId != seq->id || (g_akaoSubSeq != NULL && g_akaoSubSeq->musicId != g_akaoSeq->musicId)) {
            if (cmd != AKAO_CMD_PUSH_MUSIC) {
                akao_setReverbMode(seq->reverbMode);
            }
            blk->arg0 = (s32)seq;
            blk->arg2 = seq->id;
            switch (cmd) {
            case AKAO_CMD_PLAY_MUSIC_FROM:
                blk->arg3 = g_akaoArgs.arg1 != 0 ? g_akaoArgs.arg1 | 1 : -1;
                break;
            case AKAO_CMD_PLAY_MUSIC_COUNT:
                blk->arg3 = g_akaoArgs.arg1;
                break;
            case AKAO_CMD_PUSH_MUSIC:
                blk->arg4 = g_akaoArgs.arg1;
                break;
            case AKAO_CMD_CROSSFADE_MUSIC:
                blk->arg4 = g_akaoArgs.arg1;
                blk->arg5 = g_akaoArgs.arg2;
                break;
            }
            r = seq->id;
        } else {
            cmd = 0;
            r = 0;
        }
        } else {
            cmd = 0;
            r = -1;
        }
        break;
    case AKAO_CMD_SET_TEMPO_PITCH_SCALE:
        blk->arg0 = g_akaoArgs.arg0;
        g_akaoCommandHandlers[AKAO_CMD_SET_TEMPO_SCALE](blk);
        cmd = AKAO_CMD_SET_PITCH_SCALE;
        break;
    case AKAO_CMD_FADE_TEMPO_PITCH_SCALE:
        blk->arg0 = g_akaoArgs.arg0;
        blk->arg1 = g_akaoArgs.arg1;
        g_akaoCommandHandlers[AKAO_CMD_FADE_TEMPO_SCALE](blk);
        cmd = AKAO_CMD_FADE_PITCH_SCALE;
        break;
    case AKAO_CMD_FADE_TEMPO_PITCH_SCALE_FROM:
        blk->arg0 = g_akaoArgs.arg0;
        blk->arg1 = g_akaoArgs.arg1;
        blk->arg2 = g_akaoArgs.arg2;
        g_akaoCommandHandlers[AKAO_CMD_FADE_TEMPO_SCALE_FROM](blk);
        cmd = AKAO_CMD_FADE_PITCH_SCALE_FROM;
        break;
    case AKAO_CMD_PAUSE_ALL:
        g_akaoCommandHandlers[AKAO_CMD_PAUSE_MUSIC](blk);
        g_akaoCommandHandlers[AKAO_CMD_PAUSE_SE](blk);
        cmd = AKAO_CMD_PAUSE_STREAM;
        break;
    case AKAO_CMD_RESUME_ALL:
        g_akaoCommandHandlers[AKAO_CMD_RESUME_MUSIC](blk);
        g_akaoCommandHandlers[AKAO_CMD_RESUME_SE](blk);
        cmd = AKAO_CMD_RESUME_STREAM;
        break;
    default:
        blk->arg0 = g_akaoArgs.arg0;
        blk->arg1 = g_akaoArgs.arg1;
        blk->arg2 = g_akaoArgs.arg2;
        blk->arg3 = g_akaoArgs.arg3;
        blk->arg4 = g_akaoArgs.arg4;
        blk->arg5 = g_akaoArgs.arg5;
        break;
    }
    g_akaoCommandHandlers[cmd](blk);
    EnableEvent(g_akaoTickEvent);
    return r;
}

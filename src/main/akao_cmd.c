#include "ff9.h"

void akao_resetChannel(AkaoChannel *chan, u8 *pc) {
    chan->expression = 0x6E00;
    chan->volume = 0x23000000;
    chan->pc = pc;
    chan->transpose = 0;
    chan->fineTune = 0;
    chan->portamentoLen = 0;
    chan->pitchSlide = 0;
    chan->slideKeys = 0;
    chan->pitchSlideLeft = 0;
    chan->fixedLen = 0;
    chan->noteLen = 0;
    chan->volumeSlideLen = 0;
    chan->fineTuneOfs = 0;
    chan->pitchRandom = 0;
    chan->loopDepth = 0;
    chan->flags = 0;
    chan->panLfoOut = 0;
    chan->legato = 0;
    chan->opCount = 0xFFFF;
    chan->voiceVolScale = 0;
    chan->panLfoDepth = 0;
    chan->tremoloDepth = 0;
    chan->vibratoDepthParam = 0;
    chan->panLfoDepthSlideLen = 0;
    chan->tremoloDepthSlideLen = 0;
    chan->vibratoDepthSlideLen = 0;
    chan->panLfoPeriodSlideLen = 0;
    chan->tremoloPeriodSlideLen = 0;
    chan->vibratoPeriodSlideLen = 0;
    chan->pitchModTimer = 0;
    chan->noiseTimer = 0;
    akao_setChannelProgram(chan, 0);
}

u32 akao_getVoiceMask(AkaoChannel *chan, u32 chBits) {
    u32 voices;
    u32 i;
    voices = i = 0;
    for (; i < 32; i++) {
        if (chBits & (1 << i)) {
            if (chan->voice < 0x18) {
                voices |= 1 << chan->voice;
            }
        }
        chan++;
    }
    return voices;
}

void akao_startSeq(s32 addr, s32 channelMask) {
    AkaoSeqHeader *seq;
    u32 active;
    AkaoChannel *chan;
    u16 *ofs;
    u32 bit;
    u32 i;
    u32 x;
    u8 *tbl;

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
    tbl = (u8 *)seq + (seq->regionTableOfs + 0x30);
    g_akaoSeq->regionTable = seq->regionTableOfs != 0 ? (u16 *)tbl : NULL;
    tbl = (u8 *)seq + (seq->drumKitOfs + 0x34);
    g_akaoSeq->drumKit = seq->drumKitOfs != 0 ? (s32)tbl : 0;
    g_akaoSeq->firstVoice = 0;
    bit = 1;
    i = 0;
    chan = g_akaoChannels;
    ofs = (u16 *)(addr + 0x40);
    for (; i < 32; i++) {
        if (active & bit & channelMask) {
            chan->pc = (u8 *)ofs + *ofs;
            chan->noteTicks = 4;
            chan->gateTicks = 2;
            chan->expression = 0x7F00;
            chan->volume = 0x3FFF0000;
            chan->seVolume = 0x4000;
            chan->fineTune = 0;
            chan->transpose = 0;
            chan->portamentoLen = 0;
            chan->pitchSlide = 0;
            chan->slideKeys = 0;
            chan->pitchSlideLeft = 0;
            chan->fixedLen = 0;
            chan->noteLen = 0;
            chan->pan = 0x8000;
            chan->panSlideLen = 0;
            chan->portamentoLen = 0;
            chan->seVolumeSlideLen = 0;
            chan->volumeSlideLen = 0;
            chan->fineTuneOfs = 0;
            chan->noteVolumeSlideLen = 0;
            chan->pitchRandom = 0;
            chan->legato = 0;
            chan->panLfoOut = 0;
            chan->loopDepth = 0;
            chan->flags = !(g_akaoSeq->keepVoiceMask & bit) << 6;
            ofs++;
            chan->panLfoDepth = 0;
            chan->tremoloDepth = 0;
            chan->vibratoDepthParam = 0;
            chan->panLfoDepthSlideLen = 0;
            chan->tremoloDepthSlideLen = 0;
            chan->vibratoDepthSlideLen = 0;
            chan->panLfoPeriodSlideLen = 0;
            chan->tremoloPeriodSlideLen = 0;
            chan->vibratoPeriodSlideLen = 0;
            chan->pitchModTimer = 0;
            chan->noiseTimer = 0;
            akao_setChannelProgram(chan, 0);
        } else {
            if ((active & bit) && !(bit & channelMask)) {
                ofs++;
            }
            chan->noteTicks = 3;
            chan->gateTicks = 1;
            chan->pc = g_akaoEmptyTrack;
            chan->voiceDirty |= AKAO_VD_RELEASE_MODE | AKAO_VD_RELEASE_RATE;
            chan->voiceAdsr2 = (chan->voiceAdsr2 & ~SPU_ADSR2_RELEASE_RATE) | 5;
        }
        chan->voice = 0x18;
        active &= ~bit;
        chan++;
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

void akao_stopSeq(AkaoSeq *seq, AkaoChannel *chan, s32 id) {
    u32 i;
    if (seq->activeMask != 0 && (id == 0 || id == seq->musicId)) {
        seq->keyOffMask = -1;
        for (i = 32; i != 0; i--) {
            chan->noteTicks = 3;
            chan->gateTicks = 1;
            chan->pc = g_akaoEmptyTrack;
            chan++;
        }
        seq->musicId = 0;
        seq->playingMask = 0;
        seq->keyOnMask = 0;
        for (i = 0; i < 24; i++) {
            if (g_akaoVoiceOwner[i] == seq) {
                g_akaoVoiceOwner[i] = NULL;
                akao_setVoiceRelease(i, 5, AKAO_ENV_LINEAR_DEC);
            }
        }
    }
}

void akao_stopSeChannels(s32 id, s32 seFilter) {
    AkaoChannel *chan;
    u32 bit;
    u32 active;
    u32 i;
    s32 max;

    bit = AKAO_SE_CH_BIT(0);
    chan = g_akaoSeChannels;
    active = g_akaoSeState.activeMask | g_akaoSeState.pausedMask;
    if (seFilter & 0x0FFFFFFF) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & seFilter)) {
                if (chan->flags & AKAO_CH_STOP_AT_LOOP) {
                    chan->flags |= AKAO_CH_STOP_REQUESTED;
                } else {
                    g_akaoSeState.keyOffMask |= bit;
                    akao_stopSeChannel(chan, bit);
                    chan->flags = 0;
                }
            }
        }
    } else if (seFilter < 0) {
        chan = &g_akaoSeChannels[id];
        bit = AKAO_SE_CH_BIT(id);
        if (active & bit) {
            akao_stopSeChannels(chan->seHandle, 0);
        }
        bit <<= 1;
        chan++;
        if (active & bit) {
            akao_stopSeChannels(chan->seHandle, 0);
        }
        return;
    } else if (seFilter & 0x40000000) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if (chan->seMask != 0) {
                active &= ~bit;
            }
        }
        chan = g_akaoSeChannels;
        bit = AKAO_SE_CH_BIT(0);
        max = 0;
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if (active & bit) {
                if (max < chan->age) {
                    max = chan->age;
                }
            }
        }
        chan = g_akaoSeChannels;
        bit = AKAO_SE_CH_BIT(0);
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && max == chan->age) {
                if (chan->flags & AKAO_CH_STOP_AT_LOOP) {
                    chan->flags |= AKAO_CH_STOP_REQUESTED;
                } else {
                    g_akaoSeState.keyOffMask |= bit;
                    akao_stopSeChannel(chan, bit);
                    chan->flags = 0;
                }
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if (active & bit) {
                if (id == -1) {
                    if (chan->seHandle < 0) {
                        if (chan->flags & AKAO_CH_STOP_AT_LOOP) {
                            chan->flags |= AKAO_CH_STOP_REQUESTED;
                        } else {
                            g_akaoSeState.keyOffMask |= bit;
                            akao_stopSeChannel(chan, bit);
                            chan->flags = 0;
                        }
                    }
                } else if (chan->seHandle == id) {
                    if (chan->flags & AKAO_CH_STOP_AT_LOOP) {
                        chan->flags |= AKAO_CH_STOP_REQUESTED;
                    } else {
                        g_akaoSeState.keyOffMask |= bit;
                        akao_stopSeChannel(chan, bit);
                        chan->flags = 0;
                    }
                }
            }
        }
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_startSeChannel(AkaoChannel *chan, VoiceParams *params, s32 chBit, u8 *data) {
    AkaoChannel *ch;
    s32 bit;
    s32 left;

    chan->seHandle = params->handle;
    chan->seMask = params->mask;
    chan->sePan = params->pan << 8;
    chan->sePanSlideLen = 0;
    chan->pan = 0x8000;
    chan->panSlideLen = 0;
    chan->seVolume = (params->volume & 0x7F) << 8;
    chan->seVolumeSlideLen = 0;
    chan->seBank = params->bank;
    chan->noteTicks = 2;
    chan->gateTicks = 1;
    chan->isSe = 1;
    chan->age = -2;
    chan->sePitch = 0;
    chan->sePitchSlideLen = 0;
    akao_resetChannel(chan, data);
    g_akaoVoiceOwner[chan->voice] = NULL;
    akao_setVoiceRelease(chan->voice, 5, AKAO_ENV_LINEAR_DEC);
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
        chan = g_akaoSeChannels;
        left = 16;
        do {
            if ((g_akaoSeState.activeMask & chBit) && !(chan->seMask & AKAO_SE_DEFAULT)) {
                g_akaoSeState.activeMask &= ~chBit;
                g_akaoSeState.pausedMask |= chBit;
            }
            left--;
            chan++;
            chBit <<= 1;
        } while (left != 0);
    }
}

void akao_releaseVoice(AkaoChannel *chan, u32 voice) {
    u32 i;
    if (voice < 0x18) {
        for (i = 0; i < 32; i++) {
            if (chan->voice == voice) {
                chan->voice = 0x18;
                g_akaoSeq->playingMask &= ~(1 << i);
            }
            chan++;
        }
    }
}

void akao_startSe(VoiceParams *params, u8 *track0, u8 *track1, s32 flag) {
    AkaoChannel *chan;
    u32 bit;
    s32 left;
    u32 busy;

    if (track0 == NULL && track1 == NULL) {
        return;
    }
    if (flag == 0 && params->mask != 0) {
        akao_stopSeChannels(0, params->mask);
    }
    do {
        chan = &g_akaoSeChannels[15];
        bit = AKAO_SE_CH_BIT(15);
        busy = g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask;
        if (track0 != NULL && track1 != NULL) {
            left = 15;
            chan--;
            bit = AKAO_SE_CH_BIT(14);
        loop:
            if (busy & (bit | (bit << 1))) {
                left--;
                chan--;
                bit >>= 1;
                if (left == 0) {
                    goto fail;
                }
                goto loop;
            }
        } else {
            left = 16;
        loop2:
            if (busy & bit) {
                left--;
                chan--;
                bit >>= 1;
                if (left != 0) {
                    goto loop2;
                }
            }
        }
        if (left == 0) {
        fail:
            akao_stopSeChannels(0, 0x40000000);
            if (busy == (g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask)) {
                return;
            }
        }
    } while (left == 0);
    if (track0 != NULL) {
        akao_startSeChannel(chan, params, bit, track0);
        akao_releaseVoice(g_akaoChannels, chan->voice);
    }
    if (track1 != NULL) {
        if (track0 != NULL) {
            chan++;
            bit <<= 1;
        }
        akao_startSeChannel(chan, params, bit, track1);
        akao_releaseVoice(g_akaoChannels, chan->voice);
        if (track0 != NULL) {
            chan->flags |= AKAO_CH_SE_SECOND;
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
void akao_markSeqDirty(seq, chan)
    AkaoSeq *seq;
    AkaoChannel *chan;
{
    u32 pending;
    u32 bit;
    if (seq->activeMask != 0) {
        pending = seq->activeMask;
        bit = 1;
        do {
            if (pending & bit) {
                pending ^= bit;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
            chan++;
            bit <<= 1;
        } while (pending != 0);
    }
}

void akao_markSeDirty(void) {
    u32 pending;
    u32 bit;
    AkaoChannel *chan;
    if (g_akaoSeState.activeMask != 0) {
        pending = g_akaoSeState.activeMask;
        bit = AKAO_SE_CH_BIT(0);
        chan = g_akaoSeChannels;
        do {
            if (pending & bit) {
                pending ^= bit;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
            chan++;
            bit <<= 1;
        } while (pending != 0);
    }
}

void akao_resumeSeq(s32 addr, s32 restore) {
    AkaoSeqHeader *seq = (AkaoSeqHeader *)addr;
    u32 flags;
    u32 x;
    u8 *old;
    s32 delta;
    AkaoChannel *chan;
    s32 left;
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
    chan = g_akaoChannels;
    left = 32;
    old = g_akaoSeq->data;
    g_akaoSeq->data = (u8 *)seq;
    g_akaoSeq->keyOnMask = 0;
    bit = 1;
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_REVERB_DEPTH;
    delta = (u8 *)seq - old;
    active = g_akaoSeq->activeMask;
    g_akaoSeq->regionTable = (u16 *)((u8 *)g_akaoSeq->regionTable + delta);
    g_akaoSeq->drumKit += delta;
    g_akaoSeq->keyOnMask = g_akaoSeq->playingMask;
    do {
        if (active & bit) {
            chan->pc += delta;
            chan->regions += delta;
            chan->loopStart[0] += delta;
            chan->loopStart[1] += delta;
            chan->loopStart[2] += delta;
            chan->loopStart[3] += delta;
            chan->noteTicks += 2;
            chan->gateTicks += 2;
            chan->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH | AKAO_VD_START_ADDR | AKAO_VD_ADSR1 | AKAO_VD_ADSR2 | AKAO_VD_LOOP_ADDR;
            akao_shiftInstrumentUp(g_akaoSeq->flags, chan);
        } else {
            chan->noteTicks = 4;
            chan->gateTicks = 2;
            chan->pc = g_akaoEmptyTrack;
        }
        chan->voice = 0x18;
        left--;
        chan++;
        bit <<= 1;
    } while (left != 0);
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
    AkaoChannel *chan;

    if (g_akaoSeq->activeMask != 0) {
        akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        akao_copy(g_akaoChannels, g_akaoSavedChannels, 0x2680);
        for (i = 0, chan = g_akaoSavedChannels; i < 0x20; i++, chan++) {
            akao_shiftInstrumentDown(g_akaoSavedSeq.flags, chan);
        }
    }
}

void akao_cmdPushMusic(AkaoArgs *args) {
    AkaoSeq *seq;
    if (g_akaoSeq->activeMask != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->musicId == 0)) {
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
        akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        akao_copy(g_akaoChannels, g_akaoSubChannels, 0x2680);
    }
    akao_startSeq(args->arg0, -1);
    seq = g_akaoSeq;
    seq->volume = (args->arg4 & 0x7F) << 16;
    seq->volumeFadeLen = 0;
    seq->musicId = args->arg2;
    akao_freeSubSeqVoices();
}

void akao_cmdCrossfadeMusic(AkaoArgs *args) {
    s32 fadeLen;

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
        fadeLen = args->arg4;
        g_akaoCrossfade.volume = 0x7F8000;
        g_akaoCrossfade.fadeLen = fadeLen;
        g_akaoFlags.mode |= AKAO_MODE_CROSSFADE;
        g_akaoCrossfade.volumeStep = -0x7F8000 / fadeLen;
    }
    g_akaoSeq->volume = 0;
    g_akaoSeq->volumeFadeLen = args->arg4;
    g_akaoSeq->volumeStep = (u32)(((args->arg5 & 0x7F) << 16) | 0x8000) / (u32)args->arg4;
    akao_freeSubSeqVoices();
}

void akao_cmdPlayMusicCount(AkaoArgs *args) {
    s32 skip;
    akao_cmdPlayMusic(args);
    skip = 0;
    if (args->arg3 != 0) {
        skip = args->arg3 - 1;
    }
    g_akaoSkipMeasures = skip;
}

void akao_cmdPlaySeData(AkaoArgs *args) {
    s32 track0 = args->arg0;
    s32 track1 = args->arg1;
    args->arg0 = 0x400;
    args->arg1 = 0x1000000;
    args->arg2 = 0x80;
    args->arg3 = 0x7F;
    args->arg4 = 0;
    akao_startSe(args, track0, track1, 0);
}

void akao_cmdPlaySeDefault(AkaoArgs *args) {
    u8 *track0;
    u8 *track1;
    s32 i;
    s32 extra = g_akaoSeExtraCounts[args->arg0];

    args->arg1 = AKAO_SE_DEFAULT;
    args->arg2 = 0x80;
    args->arg3 = 0x7F;
    args->arg4 = akao_findBankSlot(g_akaoSeBankIds[args->arg0]);
    akao_getSeData(&track0, &track1, args->arg0);
    akao_startSe(args, track0, track1, 0);
    if (extra != 0) {
        i = 1;
        do {
            akao_getSeData(&track0, &track1, args->arg0 + i);
            extra--;
            akao_startSe(args, track0, track1, 1);
            i++;
        } while (extra != 0);
    }
}

void akao_cmdPlaySe(AkaoArgs *args) {
    u8 *track0;
    u8 *track1;
    s32 i;
    s32 extra = g_akaoSeExtraCounts[args->arg0];

    args->arg4 = akao_findBankSlot(g_akaoSeBankIds[args->arg0]);
    akao_getSeData(&track0, &track1, args->arg0);
    akao_startSe(args, track0, track1, 0);
    if (extra != 0) {
        i = 1;
        do {
            akao_getSeData(&track0, &track1, args->arg0 + i);
            extra--;
            akao_startSe(args, track0, track1, 1);
            i++;
        } while (extra != 0);
    }
}

void akao_cmdPlaySeBank(AkaoArgs *args) {
    AkaoSeqHdr *h = (AkaoSeqHdr *)args->arg0;
    u32 *offs = h->trackOfs;
    s32 i;
    u8 *base;
    u16 *ofsPtr;
    u8 *track0;
    u8 *track1;
    s32 count;

    args->arg4 = akao_findBankSlot(h->id);
    count = h->trackCount;
    base = (u8 *)(offs + count);
    ofsPtr = (u16 *)(base + offs[0]);
    if (*ofsPtr != 0xFFFF) {
        track0 = (u8 *)(*ofsPtr + (s32)ofsPtr + 4);
    } else {
        track0 = NULL;
    }
    ofsPtr++;
    if (*ofsPtr != 0xFFFF) {
        track1 = (u8 *)(*ofsPtr + (s32)ofsPtr + 2);
    } else {
        track1 = NULL;
    }
    akao_startSe(args, track0, track1, 0);
    for (i = 1; --count != 0; i++) {
        ofsPtr = (u16 *)(base + offs[i]);
        if (*ofsPtr != 0xFFFF) {
            track0 = (u8 *)(*ofsPtr + (s32)ofsPtr + 4);
        } else {
            track0 = NULL;
        }
        ofsPtr++;
        if (*ofsPtr != 0xFFFF) {
            track1 = (u8 *)(*ofsPtr + (s32)ofsPtr + 2);
        } else {
            track1 = NULL;
        }
        akao_startSe(args, track0, track1, 1);
    }
}

void akao_cmdStopSe(s32 *args) {
    akao_stopSeChannels(args[0], args[1]);
}

void akao_cmdSetMusicVolume(AkaoArgs *args) {
    AkaoSeq *seq;
    s32 vol;

    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        seq = g_akaoSeq;
        seq->volume = (args->arg1 & 0x7F) << 16;
        seq->volumeFadeLen = 0;
        akao_markSeqDirty(seq, g_akaoChannels);
    } else {
        seq = g_akaoSubSeq;
        if (seq != NULL && args->arg0 != 0 && args->arg0 == seq->musicId) {
            vol = args->arg1;
            seq->volumeFadeLen = 0;
            seq->volume = (vol & 0x7F) << 16;
            akao_markSeqDirty(seq, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicVolume(AkaoArgs *args) {
    s32 frames = 1;
    AkaoSeq *seq;
    AkaoSeq *subSeq;
    s32 step;
    s32 cur;

    if (args->arg1 != 0) {
        frames = args->arg1;
    }
    step = ((args->arg2 & 0x7F) << 16) | 0x8000;
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        seq = g_akaoSeq;
        do { } while (0);
        cur = seq->volume & 0xFFFF0000; cur |= 0x8000;
        step = step - cur;
        step /= frames;
        seq->volumeFadeLen = frames;
        seq->volume = cur;
        seq->volumeStep = step;
        akao_markSeqDirty(seq, g_akaoChannels);
    } else {
        subSeq = g_akaoSubSeq;
        if (subSeq == NULL || args->arg0 == 0 || args->arg0 != subSeq->musicId) {
            return;
        }
        cur = subSeq->volume & 0xFFFF0000; cur |= 0x8000;
        step = step - cur;
        do { } while (0);
        step /= frames;
        seq = subSeq;
        seq->volumeFadeLen = frames;
        seq->volume = cur;
        seq->volumeStep = step;
        akao_markSeqDirty(seq, g_akaoSubChannels);
    }
}

void akao_cmdFadeMusicVolumeFrom(AkaoArgs *args) {
    s32 frames = 1;
    AkaoSeq *seq;
    AkaoChannel *chans;
    s32 startVol;

    if (args->arg1 != 0) {
        frames = args->arg1;
    }
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        seq = g_akaoSeq;
        chans = g_akaoChannels;
    } else {
        seq = g_akaoSubSeq;
        if (seq == NULL || args->arg0 == 0 || args->arg0 != seq->musicId) {
            return;
        }
        chans = g_akaoSubChannels;
    }
    startVol = ((args->arg2 & 0x7F) << 16) | 0x8000;
    seq->volume = startVol;
    seq->volumeStep = ((((args->arg3 & 0x7F) << 16) | 0x8000) - startVol) / frames;
    seq->volumeFadeLen = frames;
    akao_markSeqDirty(seq, chans);
}

void akao_cmdSetMusicPan(AkaoArgs *args) {
    AkaoSeq *seq;
    s32 pan;

    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        seq = g_akaoSeq;
        seq->pan = (args->arg1 & 0x7F) << 16;
        seq->panSlideLen = 0;
        akao_markSeqDirty(seq, g_akaoChannels);
    } else {
        seq = g_akaoSubSeq;
        if (seq != NULL && args->arg0 != 0 && args->arg0 == seq->musicId) {
            pan = args->arg1;
            seq->panSlideLen = 0;
            seq->pan = (pan & 0x7F) << 16;
            akao_markSeqDirty(seq, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicPan(AkaoArgs *args) {
    s32 frames = 1;
    AkaoSeq *seq;
    s32 step;

    if (args->arg1 != 0) {
        frames = args->arg1;
    }
    step = (args->arg2 & 0x7F) << 16;
    if (args->arg0 == 0 || args->arg0 == g_akaoSeq->musicId) {
        seq = g_akaoSeq;
        step = step - seq->pan;
        do { } while (0);
        step /= frames;
        seq->panSlideLen = frames;
        seq->panStep = step;
        akao_markSeqDirty(seq, g_akaoChannels);
    } else {
        seq = g_akaoSubSeq;
        if (seq == NULL || args->arg0 == 0 || args->arg0 != seq->musicId) {
            return;
        }
        step = step - seq->pan;
        do { } while (0);
        step /= frames;
        seq->panSlideLen = frames;
        seq->panStep = step;
        akao_markSeqDirty(seq, g_akaoSubChannels);
    }
}

void akao_cmdSetCdVolume(u16 *args) {
    g_akaoCdVolumeFadeTicks = 0;
    g_akaoCdVolume = *args << 16;
    akao_applyCdVolume();
}

void akao_cmdFadeCdVolume(AkaoArgs *args) {
    s32 frames = 1;

    if (args->arg0 != 0) {
        frames = args->arg0;
    }
    g_akaoCdVolumeStep = (((u16)args->arg1 << 16) - g_akaoCdVolume) / frames;
    g_akaoCdVolumeFadeTicks = frames;
}

void akao_cmdFadeCdVolumeFrom(AkaoArgs *args) {
    s32 frames = 1;
    s32 startVol;

    if (args->arg0 != 0) {
        frames = args->arg0;
    }
    g_akaoCdVolumeStep = (((u16)args->arg2 << 16) - (startVol = (u16)args->arg1 << 16)) / frames;
    g_akaoCdVolumeFadeTicks = frames;
    g_akaoCdVolume = startVol;
}

void akao_cmdSetSeVolume(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                chan->seVolume = ((u16)args->arg2 & 0x7F) << 8;
                chan->seVolumeSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                chan->seVolume = ((u16)args->arg2 & 0x7F) << 8;
                chan->seVolumeSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeSeVolume(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    s16 frames;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->seVolumeStep = (s16)((((u16)args->arg3 & 0x7F) << 8) - chan->seVolume) / frames;
                chan->seVolumeSlideLen = frames;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->seVolumeStep = (s16)((((u16)args->arg3 & 0x7F) << 8) - chan->seVolume) / frames;
                chan->seVolumeSlideLen = frames;
            }
        }
    }
}

void akao_cmdSetAllSeVolume(u16 *args) {
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if (active & bit) {
            if (!(chan->seMask & AKAO_SE_DEFAULT)) {
                chan->seVolume = (*args & 0x7F) << 8;
                chan->seVolumeSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeAllSeVolume(AkaoArgs *args) {
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    s16 frames;

    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if ((active & bit) && !(chan->seMask & AKAO_SE_DEFAULT)) {
            frames = 1;
            if (args->arg0 != 0) {
                frames = args->arg0;
            }
            chan->seVolumeStep = (s16)((((u16)args->arg1 & 0x7F) << 8) - chan->seVolume) / frames;
            chan->seVolumeSlideLen = frames;
        }
    }
}

void akao_cmdSetSePan(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                chan->sePan = (u8)args->arg2 << 8;
                chan->sePanSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                chan->sePan = (u8)args->arg2 << 8;
                chan->sePanSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeSePan(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    s16 frames;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->sePanStep = (s16)(((u8)args->arg3 << 8) - chan->sePan) / frames;
                chan->sePanSlideLen = frames;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->sePanStep = (s16)(((u8)args->arg3 << 8) - chan->sePan) / frames;
                chan->sePanSlideLen = frames;
            }
        }
    }
}

void akao_cmdSetAllSePan(u8 *args) {
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if (active & bit) {
            if (!(chan->seMask & AKAO_SE_DEFAULT)) {
                chan->sePan = *args << 8;
                chan->sePanSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_VOL;
            }
        }
    }
}

void akao_cmdFadeAllSePan(AkaoArgs *args) {
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    s16 frames;

    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if ((active & bit) && !(chan->seMask & AKAO_SE_DEFAULT)) {
            frames = 1;
            if (args->arg0 != 0) {
                frames = args->arg0;
            }
            chan->sePanStep = (s16)(((u8)args->arg1 << 8) - chan->sePan) / frames;
            chan->sePanSlideLen = frames;
        }
    }
}

void akao_cmdSetSePitch(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                chan->sePitch = (u8)args->arg2 << 8;
                chan->sePitchSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_PITCH;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                chan->sePitch = (u8)args->arg2 << 8;
                chan->sePitchSlideLen = 0;
                chan->voiceDirty |= AKAO_VD_PITCH;
            }
        }
    }
}

void akao_cmdFadeSePitch(AkaoArgs *args) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 active = g_akaoSeState.activeMask;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    s16 frames;

    if (args->arg1 != 0) {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && (chan->seMask & args->arg1)) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->sePitchStep = (s16)((s16)(((u8)args->arg3 << 8) - (u16)chan->sePitch) / frames);
                chan->sePitchSlideLen = frames;
            }
        }
    } else {
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((active & bit) && chan->seHandle == args->arg0) {
                frames = 1;
                if (args->arg2 != 0) {
                    frames = args->arg2;
                }
                chan->sePitchStep = (s16)((s16)(((u8)args->arg3 << 8) - (u16)chan->sePitch) / frames);
                chan->sePitchSlideLen = frames;
            }
        }
    }
}

void akao_cmdSetAllSePitch(u8 *args) {
    s32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    for (i = 16; i != 0; i--) {
        if (!(chan->seMask & AKAO_SE_DEFAULT)) {
            chan->sePitch = *args << 8;
            chan->sePitchSlideLen = 0;
            chan->voiceDirty |= AKAO_VD_PITCH;
        }
        chan++;
    }
}

void akao_cmdFadeAllSePitch(AkaoArgs *args) {
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 active = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *chan = g_akaoSeChannels;
    s16 frames;

    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if ((active & bit) && !(chan->seMask & AKAO_SE_DEFAULT)) {
            frames = 1;
            if (args->arg0 != 0) {
                frames = args->arg0;
            }
            chan->sePitchStep = (s16)((s16)(((u8)args->arg1 << 8) - (u16)chan->sePitch) / frames);
            chan->sePitchSlideLen = frames;
        }
    }
}

void akao_cmdSetTempoScale(s8 *args) {
    s32 scale = *args;
    g_akaoTempoScaleLen = 0;
    g_akaoTempoScale = scale << 16;
}

void akao_cmdFadeTempoScale(AkaoArgs *args) {
    s32 frames = 1;

    if (args->arg0 != 0) {
        frames = args->arg0;
    }
    g_akaoTempoScaleStep = (((s8)args->arg1 << 16) - g_akaoTempoScale) / frames;
    g_akaoTempoScaleLen = frames;
}

void akao_cmdFadeTempoScaleFrom(AkaoArgs *args) {
    s32 frames = 1;

    if (args->arg1 != 0) {
        frames = args->arg0;
    }
    g_akaoTempoScale = (s8)args->arg1 << 16;
    g_akaoTempoScaleStep = (((s8)args->arg2 << 16) - g_akaoTempoScale) / frames;
    g_akaoTempoScaleLen = frames;
}

void akao_cmdSetPitchScale(s8 *args) {
    s32 scale = *args;
    g_akaoPitchScaleLen = 0;
    g_akaoPitchScale = scale << 16;
}

void akao_cmdFadePitchScale(AkaoArgs *args) {
    s32 frames = 1;

    if (args->arg0 != 0) {
        frames = args->arg0;
    }
    g_akaoPitchScaleStep = (((s8)args->arg1 << 16) - g_akaoPitchScale) / frames;
    g_akaoPitchScaleLen = frames;
}

void akao_cmdFadePitchScaleFrom(AkaoArgs *args) {
    s32 frames = 1;

    if (args->arg1 != 0) {
        frames = args->arg0;
    }
    g_akaoPitchScale = (s8)args->arg1 << 16;
    g_akaoPitchScaleStep = (((s8)args->arg2 << 16) - g_akaoPitchScale) / frames;
    g_akaoPitchScaleLen = frames;
}

void akao_cmdStopMusicNow(void) {
    akao_stopSeq(g_akaoSeq, g_akaoChannels, 0);
    if (g_akaoSubSeq != NULL) {
        akao_stopSeq(g_akaoSubSeq, g_akaoSubChannels, 0);
    }
}

void akao_cmdStopMusic(s32 *args) {
    akao_stopSeq(g_akaoSeq, g_akaoChannels, *args);
    if (g_akaoSubSeq != NULL && *args != 0) {
        akao_stopSeq(g_akaoSubSeq, g_akaoSubChannels, *args);
    }
}

void akao_cmdStopAllSe(void) {
    AkaoChannel *chan = g_akaoSeChannels;
    u32 bit = AKAO_SE_CH_BIT(0);
    u32 i;
    for (i = 0; i < 16; i++, chan++, bit <<= 1) {
        if ((g_akaoSeState.activeMask & bit) && !(chan->seMask & AKAO_SE_DEFAULT)) {
            g_akaoSeState.keyOffMask |= bit;
            akao_stopSeChannel(chan, bit);
            chan->flags = 0;
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
    u32 pending;
    u32 bit;
    s32 i;
    AkaoSeq *seq;
    s32 active;
    if (g_akaoSeq->activeMask != 0) {
        pending = ~(g_akaoSeState.activeMask | g_akaoStream.voiceMask) & 0xFFFFFF;
        if (pending != 0) {
            bit = 1;
            i = 0;
            do {
                if (pending & bit) {
                    akao_setVoiceVolume(i, 0, 0, 0);
                    akao_setVoicePitch(i, 0);
                    akao_setVoiceAttack(i, 0x7F, AKAO_ENV_LINEAR_INC);
                    akao_setVoiceSustain(i, 0x7F, AKAO_ENV_LINEAR_DEC);
                    pending &= ~bit;
                }
                bit <<= 1;
                i++;
            } while (pending != 0);
        }
        seq = g_akaoSeq;
        active = seq->activeMask;
        seq->activeMask = 0;
        seq->pausedMask = active;
    }
    g_akaoPauseFlags |= AKAO_PAUSED_MUSIC;
}

void akao_cmdResumeMusic(void) {
    AkaoChannel *ch;
    s32 bit;
    s32 pending;
    s32 paused;

    paused = g_akaoSeq->pausedMask;
    pending = paused;
    if (paused != 0) {
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
        paused = g_akaoSeq->activeMask = g_akaoSeq->pausedMask;
        g_akaoSeq->pausedMask = 0;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
    g_akaoPauseFlags &= ~AKAO_PAUSED_MUSIC;
}

void akao_cmdPauseSe(void) {
    u32 paused;
    u32 bit;
    u32 i;
    AkaoChannel *chan;
    s32 voice;

    if (g_akaoSeState.activeMask != 0) {
        paused = g_akaoSeState.activeMask;
        chan = g_akaoSeChannels;
        bit = AKAO_SE_CH_BIT(0);
        for (i = 0; i < 16; i++, chan++, bit <<= 1) {
            if ((paused & bit) && (chan->seMask & AKAO_SE_DEFAULT)) {
                paused &= ~bit;
            }
        }
        bit = AKAO_SE_CH_BIT(0);
        voice = 8;
        g_akaoSeState.pausedMask = paused;
        g_akaoSeState.activeMask &= ~paused;
        while (paused != 0) {
            if (paused & bit) {
                akao_setVoiceVolume(voice, 0, 0, 0);
                akao_setVoicePitch(voice, 0);
                akao_setVoiceAttack(voice, 0x7F, AKAO_ENV_LINEAR_INC);
                akao_setVoiceSustain(voice, 0x7F, AKAO_ENV_LINEAR_DEC);
                paused &= ~bit;
            }
            bit <<= 1;
            voice++;
        }
    }
    g_akaoPauseFlags |= AKAO_PAUSED_SE;
}

void akao_cmdResumeSe(void) {
    u32 pending;
    u32 bit;
    AkaoChannel *chan;
    s32 paused;
    if (g_akaoSeState.pausedMask != 0) {
        pending = g_akaoSeState.pausedMask;
        bit = AKAO_SE_CH_BIT(0);
        chan = g_akaoSeChannels;
        do {
            if (pending & bit) {
                pending &= ~bit;
                chan->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH | AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE | AKAO_VD_SUSTAIN_MODE | AKAO_VD_SUSTAIN_RATE;
            }
            bit <<= 1;
            chan++;
        } while (pending != 0);
        paused = g_akaoSeState.pausedMask;
        g_akaoSeState.pausedMask = 0;
        g_akaoSeState.activeMask |= paused;
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
    AkaoStreamState *stream = &g_akaoStream;
    s32 vol;
    s32 volL;
    s32 volR;
    s32 pan;

    if (stream->voiceMask == 0) {
        return;
    }
    akao_setVoicePitch(stream->voice, stream->pitch);
    akao_setVoicePitch(stream->voice + 1, stream->pitch);
    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        vol = stream->volume * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(stream->voice, vol, vol, 0);
        akao_setVoiceVolume(stream->voice + 1, vol, vol, 0);
    } else if (stream->flags & AKAO_STREAM_STEREO) {
        vol = stream->volume;
        vol <<= 15;
        vol >>= 16;
        akao_setVoiceVolume(stream->voice, vol, 0, 0);
        akao_setVoiceVolume(stream->voice + 1, 0, vol, 0);
    } else {
        pan = (u8)(stream->pan >> 8);
        volL = stream->volume * g_akaoPanCurve[pan] >> 16;
        volR = stream->volume * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(stream->voice, volL, volR, 0);
        akao_setVoiceVolume(stream->voice + 1, volL, volR, 0);
    }
}

void akao_cmdNop(void) {
}

void akao_setReverbMode(s32 mode) {
    s32 curMode;
    SpuGetReverbModeType(&curMode);
    if (curMode != mode) {
        SpuSetReverb(0);
        SpuSetReverbModeType(mode | 0x100);
        SpuSetReverb(1);
    }
}

s32 akao_command(u32 cmd) {
    s32 result = 0;
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
            result = seq->id;
        } else {
            cmd = 0;
            result = 0;
        }
        } else {
            cmd = 0;
            result = -1;
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
    return result;
}

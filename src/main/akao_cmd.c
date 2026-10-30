#include "ff9.h"

void akao_resetChannel(AkaoChannel *arg0, u8 *arg1) {
    arg0->expression = 0x6E00;
    arg0->volume = 0x23000000;
    arg0->pc = arg1;
    arg0->transpose = 0;
    arg0->fineTune = 0;
    arg0->portamentoLen = 0;
    arg0->pitchSlide = 0;
    arg0->slideKeys = 0;
    arg0->pitchSlideLeft = 0;
    arg0->fixedLen = 0;
    arg0->noteLen = 0;
    arg0->volumeSlideLen = 0;
    arg0->fineTuneOfs = 0;
    arg0->pitchRandom = 0;
    arg0->loopDepth = 0;
    arg0->flags = 0;
    arg0->panLfoOut = 0;
    arg0->legato = 0;
    arg0->opCount = 0xFFFF;
    arg0->voiceVolScale = 0;
    arg0->panLfoDepth = 0;
    arg0->tremoloDepth = 0;
    arg0->vibratoDepthParam = 0;
    arg0->panLfoDepthSlideLen = 0;
    arg0->tremoloDepthSlideLen = 0;
    arg0->vibratoDepthSlideLen = 0;
    arg0->panLfoPeriodSlideLen = 0;
    arg0->tremoloPeriodSlideLen = 0;
    arg0->vibratoPeriodSlideLen = 0;
    arg0->pitchModTimer = 0;
    arg0->noiseTimer = 0;
    akao_setChannelProgram(arg0, 0);
}

u32 akao_getVoiceMask(AkaoChannel *c, u32 mask) {
    u32 r;
    u32 i;
    r = i = 0;
    for (; i < 32; i++) {
        if (mask & (1 << i)) {
            if (c->voice < 0x18) {
                r |= 1 << c->voice;
            }
        }
        c++;
    }
    return r;
}

void akao_startSeq(s32 addr, s32 mask) {
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
    if (D_800831C4 & 1) {
        g_akaoSeq->activeMask = 0;
        g_akaoSeq->pausedMask |= active & mask;
    } else {
        g_akaoSeq->pausedMask = 0;
        g_akaoSeq->activeMask |= active & mask;
    }
    g_akaoSeq->priorityMask = seq->priorityMask;
    g_akaoSeq->keepVoiceMask = seq->keepVoiceMask;
    g_akaoSeq->flags &= ~0x33;
    g_akaoSeq->flags = seq->instrumentSet == D_8007F8DC ? g_akaoSeq->flags | 0x20 : g_akaoSeq->flags | 0x10;
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
        if (active & bit & mask) {
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
            if ((active & bit) && !(bit & mask)) {
                ofs++;
            }
            c->noteTicks = 3;
            c->gateTicks = 1;
            c->pc = D_8006F40C;
            c->voiceDirty |= 0x4400;
            c->voiceAdsr2 = (c->voiceAdsr2 & 0xFFE0) | 5;
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
    g_akaoFlags.dirty |= 0x100;
}

void akao_stopSeq(AkaoSeq *a, AkaoChannel *c, s32 id) {
    u32 i;
    if (a->activeMask != 0 && (id == 0 || id == a->musicId)) {
        a->keyOffMask = -1;
        for (i = 32; i != 0; i--) {
            c->noteTicks = 3;
            c->gateTicks = 1;
            c->pc = D_8006F40C;
            c++;
        }
        a->musicId = 0;
        a->playingMask = 0;
        a->keyOnMask = 0;
        for (i = 0; i < 24; i++) {
            if (g_akaoVoiceOwner[i] == a) {
                g_akaoVoiceOwner[i] = NULL;
                akao_setVoiceRelease(i, 5, 3);
            }
        }
    }
}

void akao_stopSeChannels(s32 id, s32 mask) {
    AkaoChannel *c;
    u32 bit;
    u32 active;
    u32 i;
    s32 max;

    bit = 0x100;
    c = g_akaoSeChannels;
    active = g_akaoSeState.activeMask | g_akaoSeState.pausedMask;
    if (mask & 0x0FFFFFFF) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->seMask & mask)) {
                if (c->flags & 0x100000) {
                    c->flags |= 0x200000;
                } else {
                    g_akaoSeState.keyOffMask |= bit;
                    akao_stopSeChannel(c, bit);
                    c->flags = 0;
                }
            }
        }
    } else if (mask < 0) {
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
    } else if (mask & 0x40000000) {
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
                if (c->flags & 0x100000) {
                    c->flags |= 0x200000;
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
                        if (c->flags & 0x100000) {
                            c->flags |= 0x200000;
                        } else {
                            g_akaoSeState.keyOffMask |= bit;
                            akao_stopSeChannel(c, bit);
                            c->flags = 0;
                        }
                    }
                } else if (c->seHandle == id) {
                    if (c->flags & 0x100000) {
                        c->flags |= 0x200000;
                    } else {
                        g_akaoSeState.keyOffMask |= bit;
                        akao_stopSeChannel(c, bit);
                        c->flags = 0;
                    }
                }
            }
        }
    }
    g_akaoFlags.dirty |= 0x110;
}

void akao_startSeChannel(AkaoChannel *c, VoiceParams *p, s32 mask, u8 *data) {
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
    akao_setVoiceRelease(c->voice, 5, 3);
    g_akaoSeState.activeMask |= mask;
    g_akaoSeState.keyOffMask |= mask;
    mask = ~mask;
    g_akaoSeState.keyOnMask &= mask;
    g_akaoSeState.playingMask &= mask;
    g_akaoSeState.noiseMask &= mask;
    g_akaoSeState.reverbMask &= mask;
    g_akaoSeState.pitchModMask &= mask;
    if (D_800831C4 & 2) {
        mask = 0x100;
        c = g_akaoSeChannels;
        n = 16;
        do {
            if ((g_akaoSeState.activeMask & mask) && !(c->seMask & 0x2000000)) {
                g_akaoSeState.activeMask &= ~mask;
                g_akaoSeState.pausedMask |= mask;
            }
            n--;
            c++;
            mask <<= 1;
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

void akao_startSe(VoiceParams *p, u8 *a, u8 *b, s32 flag) {
    AkaoChannel *c;
    u32 bit;
    s32 n;
    u32 busy;

    if (a == NULL && b == NULL) {
        return;
    }
    if (flag == 0 && p->mask != 0) {
        akao_stopSeChannels(0, p->mask);
    }
    do {
        c = &g_akaoSeChannels[15];
        bit = 0x800000;
        busy = g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask;
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
            akao_stopSeChannels(0, 0x40000000);
            if (busy == (g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStream.voiceMask)) {
                return;
            }
        }
    } while (n == 0);
    if (a != NULL) {
        akao_startSeChannel(c, p, bit, a);
        akao_releaseVoice(g_akaoChannels, c->voice);
    }
    if (b != NULL) {
        if (a != NULL) {
            c++;
            bit <<= 1;
        }
        akao_startSeChannel(c, p, bit, b);
        akao_releaseVoice(g_akaoChannels, c->voice);
        if (a != NULL) {
            c->flags |= 0x10000;
        }
    }
    g_akaoFlags.dirty |= 0x110;
}

static inline u8 *akao_getSeData_get(s32 i) {
    return D_800809DC[i] != 0xFFFF ? D_800809E8 + D_800809DC[i] : NULL;
}

void akao_getSeData(u8 **a, u8 **b, s32 idx) {
    idx &= 0x3FF;
    idx *= 2;
    *a = akao_getSeData_get(idx);
    idx++;
    *b = akao_getSeData_get(idx);
}

/* K&R definition: callers pass two or three arguments */
void akao_markSeqDirty(a, c)
    AkaoSeq *a;
    AkaoChannel *c;
{
    u32 mask;
    u32 bit;
    if (a->activeMask != 0) {
        mask = a->activeMask;
        bit = 1;
        do {
            if (mask & bit) {
                mask ^= bit;
                c->voiceDirty |= 3;
            }
            c++;
            bit <<= 1;
        } while (mask != 0);
    }
}

void akao_markSeDirty(void) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (g_akaoSeState.activeMask != 0) {
        mask = g_akaoSeState.activeMask;
        bit = 0x100;
        c = g_akaoSeChannels;
        do {
            if (mask & bit) {
                mask ^= bit;
                c->voiceDirty |= 3;
            }
            c++;
            bit <<= 1;
        } while (mask != 0);
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
    flags = g_akaoSeq->flags & ~0x30;
    g_akaoSeq->flags = flags;
    g_akaoSeq->flags = seq->instrumentSet == D_8007F8DC ? flags | 0x20 : flags | 0x10;
    c = g_akaoChannels;
    n = 32;
    old = g_akaoSeq->data;
    g_akaoSeq->data = (u8 *)seq;
    g_akaoSeq->keyOnMask = 0;
    bit = 1;
    g_akaoFlags.dirty |= 0x90;
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
            c->voiceDirty |= 0x1FF93;
            akao_shiftInstrumentUp(g_akaoSeq->flags, c);
        } else {
            c->noteTicks = 4;
            c->gateTicks = 2;
            c->pc = D_8006F40C;
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
    g_akaoFlags.dirty |= 0x100;
    if (D_800831C4 & 1) {
        g_akaoSeq->pausedMask = g_akaoSeq->activeMask;
        g_akaoSeq->activeMask = 0;
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
                if (a != g_akaoLoadedBanks[i - 1]) {
                    goto loop;
                }
                i = i - 1;
            }
        }
    }
    return i;
}

void akao_cmdPlayMusic(AkaoArgs *a) {
    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == a->arg2) {
        akao_resumeSeq(a->arg0, 0);
    } else {
        akao_startSeq(a->arg0, -1);
        g_akaoSeq->musicId = a->arg2;
    }
}

void akao_cmdPlayMusicFrom(AkaoArgs *a) {
    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == a->arg2) {
        akao_resumeSeq(a->arg0, 0);
    } else {
        akao_startSeq(a->arg0, a->arg3);
        g_akaoSeq->musicId = a->arg2;
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

void akao_cmdPushMusic(AkaoArgs *a) {
    AkaoSeq *t;
    if (g_akaoSeq->activeMask != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->musicId == 0)) {
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
        akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        akao_copy(g_akaoChannels, g_akaoSubChannels, 0x2680);
    }
    akao_startSeq(a->arg0, -1);
    t = g_akaoSeq;
    t->volume = (a->arg4 & 0x7F) << 16;
    t->volumeFadeLen = 0;
    t->musicId = a->arg2;
    func_80059414();
}

void akao_cmdCrossfadeMusic(AkaoArgs *a) {
    s32 d;

    if (g_akaoSavedSeq.musicId != 0 && g_akaoSavedSeq.musicId == a->arg2) {
        akao_resumeSeq(a->arg0, 1);
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
    } else {
        if (g_akaoSeq->activeMask != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->musicId == 0)) {
            g_akaoSubSeq = &g_akaoSavedSeq;
            g_akaoSubChannels = g_akaoSavedChannels;
            akao_copy(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
            akao_copy(g_akaoChannels, g_akaoSubChannels, 0x2680);
        }
        akao_startSeq(a->arg0, -1);
        g_akaoSeq->musicId = a->arg2;
    }
    if (g_akaoSubSeq != NULL) {
        d = a->arg4;
        D_80080A18.volume = 0x7F8000;
        D_80080A18.fadeLen = d;
        g_akaoFlags.mode |= 0x100;
        D_80080A18.volumeStep = -0x7F8000 / d;
    }
    g_akaoSeq->volume = 0;
    g_akaoSeq->volumeFadeLen = a->arg4;
    g_akaoSeq->volumeStep = (u32)(((a->arg5 & 0x7F) << 16) | 0x8000) / (u32)a->arg4;
    func_80059414();
}

void akao_cmdPlayMusicCount(AkaoArgs *a) {
    s32 r;
    akao_cmdPlayMusic(a);
    r = 0;
    if (a->arg3 != 0) {
        r = a->arg3 - 1;
    }
    D_800809B8 = r;
}

void akao_cmdPlaySeData(AkaoArgs *p) {
    s32 a = p->arg0;
    s32 b = p->arg1;
    p->arg0 = 0x400;
    p->arg1 = 0x1000000;
    p->arg2 = 0x80;
    p->arg3 = 0x7F;
    p->arg4 = 0;
    akao_startSe(p, a, b, 0);
}

void akao_cmdPlaySeDefault(AkaoArgs *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->arg0];

    p->arg1 = 0x2000000;
    p->arg2 = 0x80;
    p->arg3 = 0x7F;
    p->arg4 = func_8005A5FC(D_800809E4[p->arg0]);
    akao_getSeData(&a, &b, p->arg0);
    akao_startSe(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            akao_getSeData(&a, &b, p->arg0 + i);
            n--;
            akao_startSe(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySe(AkaoArgs *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->arg0];

    p->arg4 = func_8005A5FC(D_800809E4[p->arg0]);
    akao_getSeData(&a, &b, p->arg0);
    akao_startSe(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            akao_getSeData(&a, &b, p->arg0 + i);
            n--;
            akao_startSe(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySeBank(AkaoArgs *s) {
    AkaoSeqHdr *h = (AkaoSeqHdr *)s->arg0;
    u32 *offs = h->unk10;
    s32 i;
    u8 *base;
    u16 *p;
    u8 *a;
    u8 *b;
    s32 n;

    s->arg4 = func_8005A5FC(h->unk8);
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
    akao_startSe(s, a, b, 0);
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
        akao_startSe(s, a, b, 1);
    }
}

void akao_cmdStopSe(s32 *a) {
    akao_stopSeChannels(a[0], a[1]);
}

void akao_cmdSetMusicVolume(AkaoArgs *p) {
    AkaoSeq *s;
    s32 v;

    if (p->arg0 == 0 || p->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        s->volume = (p->arg1 & 0x7F) << 16;
        s->volumeFadeLen = 0;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && p->arg0 != 0 && p->arg0 == s->musicId) {
            v = p->arg1;
            s->volumeFadeLen = 0;
            s->volume = (v & 0x7F) << 16;
            akao_markSeqDirty(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicVolume(AkaoArgs *p) {
    s32 n = 1;
    AkaoSeq *s;
    AkaoSeq *t;
    s32 v;
    s32 cur;

    if (p->arg1 != 0) {
        n = p->arg1;
    }
    v = ((p->arg2 & 0x7F) << 16) | 0x8000;
    if (p->arg0 == 0 || p->arg0 == g_akaoSeq->musicId) {
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
        if (t == NULL || p->arg0 == 0 || p->arg0 != t->musicId) {
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

void akao_cmdFadeMusicVolumeFrom(AkaoArgs *p) {
    s32 n = 1;
    AkaoSeq *s;
    AkaoChannel *c;
    s32 v;

    if (p->arg1 != 0) {
        n = p->arg1;
    }
    if (p->arg0 == 0 || p->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        c = g_akaoChannels;
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || p->arg0 == 0 || p->arg0 != s->musicId) {
            return;
        }
        c = g_akaoSubChannels;
    }
    v = ((p->arg2 & 0x7F) << 16) | 0x8000;
    s->volume = v;
    s->volumeStep = ((((p->arg3 & 0x7F) << 16) | 0x8000) - v) / n;
    s->volumeFadeLen = n;
    akao_markSeqDirty(s, c);
}

void akao_cmdC4(AkaoArgs *p) {
    AkaoSeq *s;
    s32 v;

    if (p->arg0 == 0 || p->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        s->pan = (p->arg1 & 0x7F) << 16;
        s->panSlideLen = 0;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && p->arg0 != 0 && p->arg0 == s->musicId) {
            v = p->arg1;
            s->panSlideLen = 0;
            s->pan = (v & 0x7F) << 16;
            akao_markSeqDirty(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdC5(AkaoArgs *p) {
    s32 n = 1;
    AkaoSeq *s;
    s32 v;

    if (p->arg1 != 0) {
        n = p->arg1;
    }
    v = (p->arg2 & 0x7F) << 16;
    if (p->arg0 == 0 || p->arg0 == g_akaoSeq->musicId) {
        s = g_akaoSeq;
        v = v - s->pan;
        do { } while (0);
        v /= n;
        s->panSlideLen = n;
        s->panStep = v;
        akao_markSeqDirty(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || p->arg0 == 0 || p->arg0 != s->musicId) {
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

void akao_cmdSetCdVolume(u16 *a) {
    g_akaoCdVolumeFadeTicks = 0;
    g_akaoCdVolume = *a << 16;
    akao_applyCdVolume();
}

void akao_cmdFadeCdVolume(AkaoArgs *p) {
    s32 n = 1;

    if (p->arg0 != 0) {
        n = p->arg0;
    }
    g_akaoCdVolumeStep = (((u16)p->arg1 << 16) - g_akaoCdVolume) / n;
    g_akaoCdVolumeFadeTicks = n;
}

void akao_cmdFadeCdVolumeFrom(AkaoArgs *p) {
    s32 n = 1;
    s32 v;

    if (p->arg0 != 0) {
        n = p->arg0;
    }
    g_akaoCdVolumeStep = (((u16)p->arg2 << 16) - (v = (u16)p->arg1 << 16)) / n;
    g_akaoCdVolumeFadeTicks = n;
    g_akaoCdVolume = v;
}

void akao_cmdSetSeVolume(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                c->seVolume = ((u16)a->arg2 & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                c->seVolume = ((u16)a->arg2 & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdFadeSeVolume(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->seVolumeStep = (s16)((((u16)a->arg3 & 0x7F) << 8) - c->seVolume) / n;
                c->seVolumeSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->seVolumeStep = (s16)((((u16)a->arg3 & 0x7F) << 8) - c->seVolume) / n;
                c->seVolumeSlideLen = n;
            }
        }
    }
}

void akao_cmdSetAllSeVolume(u16 *a) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->seMask & 0x2000000)) {
                c->seVolume = (*a & 0x7F) << 8;
                c->seVolumeSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdFadeAllSeVolume(AkaoArgs *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->seMask & 0x2000000)) {
            n = 1;
            if (p->arg0 != 0) {
                n = p->arg0;
            }
            c->seVolumeStep = (s16)((((u16)p->arg1 & 0x7F) << 8) - c->seVolume) / n;
            c->seVolumeSlideLen = n;
        }
    }
}

void akao_cmdA2(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                c->sePan = (u8)a->arg2 << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                c->sePan = (u8)a->arg2 << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdA3(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->sePanStep = (s16)(((u8)a->arg3 << 8) - c->sePan) / n;
                c->sePanSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->sePanStep = (s16)(((u8)a->arg3 << 8) - c->sePan) / n;
                c->sePanSlideLen = n;
            }
        }
    }
}

void akao_cmdAA(u8 *a) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->seMask & 0x2000000)) {
                c->sePan = *a << 8;
                c->sePanSlideLen = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdAB(AkaoArgs *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->seMask & 0x2000000)) {
            n = 1;
            if (p->arg0 != 0) {
                n = p->arg0;
            }
            c->sePanStep = (s16)(((u8)p->arg1 << 8) - c->sePan) / n;
            c->sePanSlideLen = n;
        }
    }
}

void akao_cmdA4(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                c->sePitch = (u8)a->arg2 << 8;
                c->sePitchSlideLen = 0;
                c->voiceDirty |= 0x10;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                c->sePitch = (u8)a->arg2 << 8;
                c->sePitchSlideLen = 0;
                c->voiceDirty |= 0x10;
            }
        }
    }
}

void akao_cmdA5(AkaoArgs *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.activeMask;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->arg1 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->seMask & a->arg1)) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->sePitchStep = (s16)((s16)(((u8)a->arg3 << 8) - (u16)c->sePitch) / n);
                c->sePitchSlideLen = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->seHandle == a->arg0) {
                n = 1;
                if (a->arg2 != 0) {
                    n = a->arg2;
                }
                c->sePitchStep = (s16)((s16)(((u8)a->arg3 << 8) - (u16)c->sePitch) / n);
                c->sePitchSlideLen = n;
            }
        }
    }
}

void akao_cmdAC(u8 *a) {
    s32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 16; i != 0; i--) {
        if (!(c->seMask & 0x2000000)) {
            c->sePitch = *a << 8;
            c->sePitchSlideLen = 0;
            c->voiceDirty |= 0x10;
        }
        c++;
    }
}

void akao_cmdAD(AkaoArgs *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.activeMask;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->seMask & 0x2000000)) {
            n = 1;
            if (p->arg0 != 0) {
                n = p->arg0;
            }
            c->sePitchStep = (s16)((s16)(((u8)p->arg1 << 8) - (u16)c->sePitch) / n);
            c->sePitchSlideLen = n;
        }
    }
}

void akao_cmdD0(s8 *a) {
    s32 v = *a;
    D_8007F96E = 0;
    D_80083154 = v << 16;
}

void akao_cmdD1(AkaoArgs *p) {
    s32 n = 1;

    if (p->arg0 != 0) {
        n = p->arg0;
    }
    D_8007F8E8 = (((s8)p->arg1 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void akao_cmdD2(AkaoArgs *p) {
    s32 n = 1;

    if (p->arg1 != 0) {
        n = p->arg0;
    }
    D_80083154 = (s8)p->arg1 << 16;
    D_8007F8E8 = (((s8)p->arg2 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void akao_cmdD4(s8 *a) {
    s32 v = *a;
    D_8007F96C = 0;
    D_80083150 = v << 16;
}

void akao_cmdD5(AkaoArgs *p) {
    s32 n = 1;

    if (p->arg0 != 0) {
        n = p->arg0;
    }
    D_8007F8E4 = (((s8)p->arg1 << 16) - D_80083150) / n;
    D_8007F96C = n;
}

void akao_cmdD6(AkaoArgs *p) {
    s32 n = 1;

    if (p->arg1 != 0) {
        n = p->arg0;
    }
    D_80083150 = (s8)p->arg1 << 16;
    D_8007F8E4 = (((s8)p->arg2 << 16) - D_80083150) / n;
    D_8007F96C = n;
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
        if ((g_akaoSeState.activeMask & bit) && !(c->seMask & 0x2000000)) {
            g_akaoSeState.keyOffMask |= bit;
            akao_stopSeChannel(c, bit);
            c->flags = 0;
        }
    }
    g_akaoFlags.dirty |= 0x110;
}

void akao_cmdSetOutputMode1(void) {
    g_akaoFlags.mode = 1;
    akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
    }
    akao_markSeDirty();
}

void akao_cmdSetOutputMode2(void) {
    g_akaoFlags.mode = 2;
    akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
    }
    akao_markSeDirty();
}

void akao_cmd90(s32 *arg0) {
    u32 i;
    AkaoChannel *ch;
    D_80080AC4 = *arg0;
    ch = g_akaoChannels;
    for (i = 0; i < 0x20; i++) {
        ch->voiceDirty |= 3;
        ch++;
    }
}

void akao_cmd92(u16 *a) {
    g_akaoSeq->condition = *a;
}

void akao_cmd9B(void) {
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
                    akao_setVoiceAttack(i, 0x7F, 1);
                    akao_setVoiceSustain(i, 0x7F, 3);
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
    D_800831C4 |= 1;
}

void akao_cmd9A(void) {
    AkaoChannel *ch;
    s32 bit;
    s32 mask;
    s32 t;

    t = g_akaoSeq->pausedMask;
    mask = t;
    if (t != 0) {
        bit = 1;
        ch = g_akaoChannels;
        do {
            if (mask & bit) {
                mask &= ~bit;
                ch->voiceDirty |= 0x2B13;
            }
            bit <<= 1;
            ch++;
        } while (mask != 0);
        t = g_akaoSeq->activeMask = g_akaoSeq->pausedMask;
        g_akaoSeq->pausedMask = 0;
        g_akaoFlags.dirty |= 0x100;
    }
    D_800831C4 &= ~1;
}

void akao_cmd9D(void) {
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
            if ((m & bit) && (c->seMask & 0x2000000)) {
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
                akao_setVoiceAttack(v, 0x7F, 1);
                akao_setVoiceSustain(v, 0x7F, 3);
                m &= ~bit;
            }
            bit <<= 1;
            v++;
        }
    }
    D_800831C4 |= 2;
}

void akao_cmd9C(void) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    s32 m;
    if (D_80080A80 != 0) {
        mask = D_80080A80;
        bit = 0x100;
        c = g_akaoSeChannels;
        do {
            if (mask & bit) {
                mask &= ~bit;
                c->voiceDirty |= 0x2B13;
            }
            bit <<= 1;
            c++;
        } while (mask != 0);
        m = g_akaoSeState.pausedMask;
        g_akaoSeState.pausedMask = 0;
        g_akaoSeState.activeMask |= m;
        g_akaoFlags.dirty |= 0x100;
    }
    D_800831C4 &= ~2;
}

void akao_cmd9F(void) {
    if (g_akaoStream.voiceMask != 0) {
        akao_setVoicePitch(g_akaoStream.voice, 0);
        akao_setVoicePitch(g_akaoStream.voice + 1, 0);
        akao_setVoiceVolume(g_akaoStream.voice, 0, 0, 0);
        akao_setVoiceVolume(g_akaoStream.voice + 1, 0, 0, 0);
    }
}

void akao_cmd9E(void) {
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
    if (g_akaoOutputFlags & 2) {
        v = d->volume * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(d->voice, v, v, 0);
        akao_setVoiceVolume(d->voice + 1, v, v, 0);
    } else if (d->flags & 1) {
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
    case 0x10:
    case 0x12:
    case 0x14:
    case 0x19:
    case 0x1A:
        if (akao_checkHeader((s32 *)g_akaoArgs.arg0) == 0) {
        seq = (AkaoSeqHeader *)g_akaoArgs.arg0;
        if (g_akaoSeq->musicId != seq->id || (g_akaoSubSeq != NULL && g_akaoSubSeq->musicId != g_akaoSeq->musicId)) {
            if (cmd != 0x19) {
                akao_setReverbMode(seq->reverbMode);
            }
            blk->arg0 = (s32)seq;
            blk->arg2 = seq->id;
            switch (cmd) {
            case 0x14:
                blk->arg3 = g_akaoArgs.arg1 != 0 ? g_akaoArgs.arg1 | 1 : -1;
                break;
            case 0x12:
                blk->arg3 = g_akaoArgs.arg1;
                break;
            case 0x19:
                blk->arg4 = g_akaoArgs.arg1;
                break;
            case 0x1A:
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
    case 0xD8:
        blk->arg0 = g_akaoArgs.arg0;
        g_akaoCommandHandlers[0xD0](blk);
        cmd = 0xD4;
        break;
    case 0xD9:
        blk->arg0 = g_akaoArgs.arg0;
        blk->arg1 = g_akaoArgs.arg1;
        g_akaoCommandHandlers[0xD1](blk);
        cmd = 0xD5;
        break;
    case 0xDA:
        blk->arg0 = g_akaoArgs.arg0;
        blk->arg1 = g_akaoArgs.arg1;
        blk->arg2 = g_akaoArgs.arg2;
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

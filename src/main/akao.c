#include "ff9.h"

void akao_applyCdVolume(void) {
    s32 v = g_akaoCdVolumeHi;
    SPU_REG16(SPU_CD_VOL_L) = v;
    SPU_REG16(SPU_CD_VOL_R) = v;
}

void akao_copy(u32 *src, u32 *dst, u32 n) {
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

void akao_swap(u32 *a, u32 *b, u32 n) {
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

void akao_tickSe(void) {
    s32 v;
    AkaoChannel *c;
    u32 pending;
    AkaoStreamState *s;
    s32 old;
    s32 k;

    g_akaoRandIndex = (g_akaoRandIndex + 1) & 0xFF;
    if (g_akaoCdVolumeFadeTicks != 0) {
        g_akaoCdVolumeFadeTicks--;
        g_akaoCdVolume += g_akaoCdVolumeStep;
        akao_applyCdVolume();
    }
    s = &g_akaoStream;
    if (s->voiceMask != 0 && s->volumeFadeLen != 0) {
        s->volumeFadeLen--;
        old = s->volume;
        v = old + s->volumeStep;
        if ((v & 0xFF00) != (old & 0xFF00)) {
            if (g_akaoOutputFlags & AKAO_MODE_MONO) {
                old = (old * g_akaoMonoGain) >> 16;
                akao_setVoiceVolume(s->voice, old, old, 0);
                akao_setVoiceVolume(s->voice + 1, old, old, 0);
            } else {
                old = v << 15;
                old >>= 16;
                akao_setVoiceVolume(s->voice, old, 0, 0);
                akao_setVoiceVolume(s->voice + 1, 0, old, 0);
            }
        }
        g_akaoSeVolume = v & 0xFFFF;
    }
    if (g_akaoTempoScaleLen != 0) {
        g_akaoTempoScaleLen--;
        g_akaoTempoScale += g_akaoTempoScaleStep;
    }
    if (g_akaoPitchScaleLen != 0) {
        g_akaoPitchScaleLen--;
        v = g_akaoPitchScale + g_akaoPitchScaleStep;
        if ((v & 0xFF0000) != (g_akaoPitchScale & 0xFF0000)) {
            k = 32;
            c = g_akaoChannels;
            do {
                k--;
                c->voiceDirty |= AKAO_VD_PITCH;
                c++;
            } while (k != 0);
        }
        g_akaoPitchScale = v;
    }
    if (g_akaoSeq->activeMask != 0) {
        if (g_akaoSeq->volumeFadeLen != 0) {
            g_akaoSeq->volumeFadeLen--;
            v = g_akaoSeq->volume + g_akaoSeq->volumeStep;
            if ((v & 0x7F0000) != (g_akaoSeq->volume & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->volume = v;
        }
        if (g_akaoSeq->panSlideLen != 0) {
            g_akaoSeq->panSlideLen--;
            v = g_akaoSeq->pan + g_akaoSeq->panStep;
            if ((v & 0x7F0000) != (g_akaoSeq->pan & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->pan = v;
        }
    }
    if (g_akaoSubSeq != NULL && g_akaoSubSeq->activeMask != 0) {
        if (g_akaoSubSeq->volumeFadeLen != 0) {
            g_akaoSubSeq->volumeFadeLen--;
            v = g_akaoSubSeq->volume + g_akaoSubSeq->volumeStep;
            if ((v & 0x7F0000) != (g_akaoSubSeq->volume & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
            }
            g_akaoSubSeq->volume = v;
        }
        if (g_akaoSubSeq->panSlideLen != 0) {
            g_akaoSubSeq->volumeFadeLen--;
            v = g_akaoSubSeq->pan + g_akaoSubSeq->panStep;
            if ((v & 0x7F0000) != (g_akaoSubSeq->pan & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
            }
            g_akaoSubSeq->pan = v;
        }
    }
    if (g_akaoSeState.activeMask != 0) {
        pending = g_akaoSeState.activeMask;
        c = g_akaoSeChannels;
        k = 0x100;
        do {
            if (pending & k) {
                if ((u16)c->seVolumeSlideLen != 0) {
                    c->seVolumeSlideLen = (u16)c->seVolumeSlideLen - 1;
                    v = (s16)c->seVolume + c->seVolumeStep;
                    if ((v & 0xFF00) != ((s16)c->seVolume & 0xFF00)) {
                        c->voiceDirty |= AKAO_VD_VOL;
                    }
                    c->seVolume = v;
                }
                if (c->sePanSlideLen != 0) {
                    c->sePanSlideLen--;
                    v = c->sePan + c->sePanStep;
                    if ((v & 0xFF00) != (c->sePan & 0xFF00)) {
                        c->voiceDirty |= AKAO_VD_VOL;
                    }
                    c->sePan = v;
                }
                if ((u16)c->sePitchSlideLen != 0) {
                    c->sePitchSlideLen = (u16)c->sePitchSlideLen - 1;
                    v = c->sePitch + c->sePitchStep;
                    if ((v & 0xFF00) != (c->sePitch & 0xFF00)) {
                        c->voiceDirty |= AKAO_VD_PITCH;
                    }
                    c->sePitch = v;
                }
                /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                if (v) {
                    pending ^= k;
                } else {
                    pending ^= k;
                }
            }
            k <<= 1;
            c++;
        } while (pending != 0);
    }
}

u32 akao_tickChannels(AkaoChannel *chans, s32 sub) {
    u32 tempo;
    u32 m;
    s32 acc;
    AkaoChannel *c;
    u32 bit;
    u32 active;
    s32 flags;

    tempo = g_akaoSeq->tempo.h[1];
    m = g_akaoTempoScaleHi;
    if (m != 0) {
        if (m < 0x80) {
            tempo += (tempo * m) >> 7;
        } else {
            tempo = (tempo * m) >> 8;
        }
    }
    acc = g_akaoSeq->tempoAcc + tempo;
    g_akaoSeq->tempoAcc = acc;
    if ((acc & 0xFFFF0000) || (g_akaoPauseFlags & AKAO_PAUSE_FORCE_TICK)) {
        g_akaoSeq->tempoAcc = acc & 0xFFFF;
        do {
            for (;;) {
                c = chans;
                bit = 1;
                active = g_akaoSeq->activeMask;
                do {
                    if (active & bit) {
                        c->noteTicks--;
                        c->gateTicks--;
                        if (c->noteTicks == 0) {
                            akao_execChannel(c, bit);
                        } else if (c->gateTicks == 0) {
                            g_akaoSeq->keyOffMask |= bit;
                        }
                        akao_stepChannelSlides(c, bit, 0);
                        active &= ~bit;
                    }
                    c++;
                    bit <<= 1;
                } while (active != 0);
                if (g_akaoSeq->tempoSlideLen != 0) {
                    g_akaoSeq->tempoSlideLen--;
                    g_akaoSeq->tempo.w += g_akaoSeq->tempoStep;
                }
                if (g_akaoSeq->reverbDepthSlideLen != 0) {
                    g_akaoSeq->reverbDepthSlideLen--;
                    g_akaoSeq->reverbDepth += g_akaoSeq->reverbDepthStep;
                    flags = g_akaoFlags.dirty;
                    flags |= AKAO_DIRTY_REVERB_DEPTH;
                    if (sub == 0) {
                        g_akaoFlags.dirty = flags;
                    }
                }
                if (g_akaoSeq->ticksPerBeat != 0) {
                    if (++g_akaoSeq->tick == g_akaoSeq->ticksPerBeat) {
                        g_akaoSeq->tick = 0;
                        if (++g_akaoSeq->beat == g_akaoSeq->beatsPerMeasure) {
                            g_akaoSeq->beat = 0;
                            g_akaoSeq->measure++;
                            if (sub == 0 && g_akaoSkipMeasures != 0) {
                                g_akaoSkipMeasures--;
                            }
                        }
                    }
                }
                if (sub != 0 || g_akaoSkipMeasures == 0) {
                    break;
                }
            }
        } while (0);
    }
    return g_akaoSeq->activeMask;
}

void akao_tickCrossfade(AkaoSeq *seq, AkaoChannel *ch) {
    s32 t;

    if (g_akaoCrossfade.fadeLen != 0) {
        if (--g_akaoCrossfade.fadeLen == 0) {
            s32 v = seq->activeMask;
            seq->musicId = 0;
            seq->activeMask = 0;
            seq->keyOnMask = 0;
            seq->playingMask = 0;
            seq->keyOffMask = v;
            return;
        }
        t = g_akaoCrossfade.volume + g_akaoCrossfade.volumeStep;
        if ((t & 0xFFFF0000) != (g_akaoCrossfade.volume & 0xFFFF0000)) {
            akao_markSeqDirty(seq, ch, seq);
        }
        g_akaoCrossfade.volume = t;
    }
}

s32 akao_tick(void) {
    s32 t0;
    s32 n;
    u32 now;
    AkaoSeq *sub;
    AkaoChannel *c;
    u32 elapsed;
    u32 bit;
    u32 acc;
    s32 a;
    s32 b;
    s32 e;

    t0 = GetRCnt(0xF2000002);
    g_akaoVsyncs[1] = g_akaoVsyncs[0];
    now = VSync(-1);
    n = 1;
    elapsed = now - g_akaoVsyncs[1];
    g_akaoVsyncs[0] = now;
    if (elapsed != 0) {
        if (elapsed < 9) {
            bit = elapsed * 4;
            if (g_akaoTickPhase < bit) {
                n = bit - g_akaoTickPhase + 1;
            }
            g_akaoTickPhase %= bit;
        }
    }
    do {
        n--;
        g_akaoTickPhase++;
        g_akaoTickCount++;
        if ((g_akaoSeq->keyOffMask | g_akaoSeState.keyOffMask) != 0 || (g_akaoSubSeq != NULL && g_akaoSubSeq->keyOffMask != 0)) {
            akao_flushKeyOffs();
        }
        if (g_akaoSubSeq != NULL) {
            if (g_akaoSubSeq->activeMask == 0) {
                g_akaoSubSeq = NULL;
            } else if ((g_akaoSeq->activeMask | g_akaoSeq->pausedMask) == 0) {
                akao_copy(g_akaoSubSeq, g_akaoSeq, 0x7C);
                akao_copy(g_akaoSubChannels, g_akaoChannels, 0x2680);
                sub = g_akaoSubSeq;
                g_akaoSubSeq = NULL;
                sub->musicId = 0;
                sub->activeMask = 0;
            }
        }
        if ((g_akaoFlags.dirty | g_akaoSeq->playingMask | g_akaoSeState.playingMask) != 0 || (g_akaoSubSeq != NULL && g_akaoSubSeq->playingMask != 0)) {
            akao_updateVoices();
        }
        if (g_akaoSeq->activeMask != 0) {
            akao_tickChannels(g_akaoChannels, 0);
        }
        if (g_akaoSubSeq != NULL) {
            if (g_akaoSubSeq->activeMask != 0) {
                g_akaoSeq = g_akaoSubSeq;
                akao_tickChannels(g_akaoSubChannels, 1);
                g_akaoSeq = &g_akaoMainSeq;
            }
            if ((g_akaoFlags.mode & AKAO_MODE_CROSSFADE) && !(g_akaoTickCount & 3)) {
                akao_tickCrossfade(g_akaoSubSeq, g_akaoSubChannels);
                if (g_akaoSubSeq->activeMask == 0) {
                    g_akaoFlags.mode &= ~AKAO_MODE_CROSSFADE;
                }
            }
        }
        if (g_akaoSeState.activeMask != 0) {
            elapsed = g_akaoSeState.activeMask;
            acc = g_akaoSeState.tempoAcc + g_akaoSeState.tempo.h[1];
            g_akaoSeState.tempoAcc = acc;
            if ((acc & 0xFFFF0000) || (g_akaoPauseFlags & AKAO_PAUSE_FORCE_TICK)) {
                g_akaoSeState.tempoAcc = acc & 0xFFFF;
                bit = 0x100;
                c = g_akaoSeChannels;
                do {
                    if (elapsed & bit) {
                        if (!(g_akaoPauseFlags & AKAO_PAUSED_SE) || (c->seMask & AKAO_SE_DEFAULT)) {
                            c->age++;
                            c->noteTicks--;
                            c->gateTicks--;
                            if (c->noteTicks == 0) {
                                akao_execChannel(c, bit);
                            } else if (c->gateTicks == 0) {
                                g_akaoSeState.keyOffMask |= bit;
                                g_akaoSeState.playingMask &= ~bit;
                            }
                            akao_stepChannelSlides(c, bit, 1);
                        }
                        elapsed ^= bit;
                    }
                    c++;
                    bit <<= 1;
                } while (elapsed != 0);
            }
        }
        if (!(g_akaoTickCount & 3)) {
            akao_tickSe();
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
s32 akao_peekNextNote(AkaoChannel *c) {
    u8 *p;
    u32 n;
    u32 op;
    s32 len;

    p = c->pc;
    n = c->loopDepth;
    for (;;) {
        if (*p < AKAO_NOTE_END) {
            if (*p >= AKAO_NOTE_REST) {
                c->legato = (u16)c->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
            }
            return *p;
        }
        if (*p < AKAO_OP_FIRST_CMD) {
            return AKAO_OP_END;
        }
        op = *p;
        len = g_akaoOpLengths[op - AKAO_OP_FIRST_CMD];
        if (len != 0) {
            p += len;
            continue;
        }
        switch (op) {
        case AKAO_OP_NOTE_LEN:
        case AKAO_OP_NOTE_LEN + 1:
        case AKAO_OP_NOTE_LEN + 2:
        case AKAO_OP_NOTE_LEN + 3:
        case AKAO_OP_NOTE_LEN + 4:
        case AKAO_OP_NOTE_LEN + 5:
        case AKAO_OP_NOTE_LEN + 6:
        case AKAO_OP_NOTE_LEN + 7:
        case AKAO_OP_NOTE_LEN + 8:
        case AKAO_OP_NOTE_LEN + 9:
        case AKAO_OP_NOTE_LEN + 10:
        case AKAO_OP_NOTE_LEN + 11:
            return 0x83; /* a note */
        case AKAO_OP_TIE_LEN:
            return AKAO_NOTE_TIE;
        case AKAO_OP_REST_LEN:
            return AKAO_NOTE_REST;
        case AKAO_OP_EXT:
            p++;
            op = *p;
            len = g_akaoFeOpLengths[op];
            if (len != 0) {
                p += len;
                continue;
            }
            switch (op) {
            case AKAO_OPFE_LOOP_PASS_JUMP:
            case AKAO_OPFE_LOOP_PASS_EXIT:
                p++;
                if (*p == c->loopCount[n] + 1) {
                    p++;
                    n--;
                    n &= 3;
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 3;
                continue;
            case AKAO_OPFE_JUMP:
            case AKAO_OPFE_CALL:
                p++;
                op = p[0] + (p[1] << 8);
                p += (s16)op;
                continue;
            case AKAO_OPFE_JUMP_IF_GE:
                p++;
                if (*p++ <= g_akaoSeq->condition) {
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 2;
                continue;
            case AKAO_OPFE_RETURN:
                p = c->returnAddr;
                continue;
            }
            continue;
        case AKAO_OP_LOOP_END:
            p++;
            if (*p == c->loopCount[n] + 1) {
                p++;
                n--;
                n &= 3;
                continue;
            }
            p = c->loopStart[n];
            continue;
        case AKAO_OP_EFFECTS_OFF:
        case AKAO_OP_NOP_CD:
        case AKAO_OP_NOP_D1:
        case AKAO_OP_PORTAMENTO_OFF:
            c->legato = (u16)c->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
            p++;
            continue;
        case AKAO_OP_LOOP_FOREVER:
            if (c->flags & AKAO_CH_STOP_REQUESTED) {
                goto end;
            }
            p = c->loopStart[n];
            continue;
        default:
        end:
            c->legato = (u16)c->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
            return AKAO_OP_END;
        }
    }
}

s32 akao_remapInstrument(s32 seqFlags, s32 instr) {
    if ((seqFlags & AKAO_SEQ_INSTR_ALT) && (u32)(instr - 0x20) < 0x60) {
        return instr + 0x30;
    }
    return instr;
}

void akao_applyKeySplit(AkaoChannel *c, u32 key) {
    AkaoRegion *e;
    AkaoInstrument *p;
    u32 f;
    s32 v;

    if (c->curKey < key || c->curKey == 0xFF) {
        e = (AkaoRegion *)c->regions;
        while (e[1].sustainMode != 0 && e->highKey < key) {
            e++;
        }
    } else if (key < c->curKey) {
        e = (AkaoRegion *)c->regions;
        while (e[1].sustainMode != 0 && !(key < e[1].lowKey)) {
            e++;
        }
    } else {
        return;
    }
    f = c->flags;
    v = akao_remapInstrument(g_akaoSeq->flags, e->sample);
    c->instrument = v;
    p = &g_akaoInstruments[v];
    c->voiceStartAddr = p->startAddr;
    c->voiceLoopAddr = p->loopAddr;
    if (!(f & AKAO_CH_ATTACK_SET)) {
        c->voiceAdsr1 = e->attack << 8;
    } else {
        c->voiceAdsr1 &= SPU_ADSR1_ATTACK_RATE;
    }
    c->voiceAdsr1 |= p->adsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL);
    if (!(f & AKAO_CH_SUSTAIN_RATE_SET)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->sustainRate << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->sustainMode) {
    case AKAO_ENV_LINEAR_DEC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    if (!(f & AKAO_CH_RELEASE_RATE_SET)) {
        c->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_RATE;
        c->voiceAdsr2 |= e->releaseRate;
    }
    c->voiceAdsr2 |= p->adsr2 & SPU_ADSR2_RELEASE_EXP;
    c->voiceVolScale = e->volume;
}

u16 akao_calcPitch(AkaoInstrument *p, s32 note, u8 fine, s32 *out) {
    s32 n = note - p->baseNote;
    s32 freq;
    u32 f;

    while (n < 0) {
        n += 12;
    }
    n %= 12;
    if (p->fineTune == 0) {
        freq = g_akaoSemitonePitch[n] << 8;
    } else if (p->fineTune < 0) {
        freq = (g_akaoSemitonePitch[n] * (u16)p->fineTune) >> 8;
    } else {
        freq = (g_akaoSemitonePitch[n] * p->fineTune) >> 7;
        freq += g_akaoSemitonePitch[n] << 8;
    }
    f = fine;
    if (f != 0) {
        if (f < 0x80) {
            *out = ((u32)freq * f) >> 7;
        } else {
            *out = (((u32)freq * f) >> 8) - freq;
        }
    }
    if (note < p->baseNote) {
        do {
            *out >>= 1;
            freq >>= 1;
        } while ((note += 12) < p->baseNote);
    } else {
        n = (note - p->baseNote) / 12;
        if (n != 0) {
            freq <<= n;
            *out <<= n;
        }
    }
    *out >>= 8;
    freq >>= 8;
    return freq;
}

s32 akao_playDrumNote(AkaoChannel *c, u32 chBit, s32 key) {
    AkaoDrumKey *e;
    AkaoInstrument *p;
    u32 f;
    s32 v;
    s32 r;

    e = (AkaoDrumKey *)g_akaoSeq->drumKit;
    e += key;
    g_akaoSeq->keyOnMask |= chBit;
    if (g_akaoSeq->playingMask & chBit) {
        g_akaoSeq->keyOffMask |= chBit;
    }
    f = c->flags;
    v = akao_remapInstrument(g_akaoSeq->flags, e->sample);
    c->instrument = v;
    p = &g_akaoInstruments[v];
    c->voiceStartAddr = p->startAddr;
    c->voiceLoopAddr = p->loopAddr;
    if (!(f & AKAO_CH_ATTACK_SET)) {
        c->voiceAdsr1 = e->attack << 8;
    } else {
        c->voiceAdsr1 &= SPU_ADSR1_ATTACK_RATE;
    }
    c->voiceAdsr1 |= p->adsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL);
    if (!(f & AKAO_CH_SUSTAIN_RATE_SET)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->sustainRate << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->sustainMode) {
    case AKAO_ENV_LINEAR_DEC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        c->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    if (!(f & AKAO_CH_RELEASE_RATE_SET)) {
        c->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_RATE;
        c->voiceAdsr2 |= e->releaseRate;
    }
    c->voiceAdsr2 |= p->adsr2 & SPU_ADSR2_RELEASE_EXP;
    r = ((s32 (*)())akao_calcPitch)(p, e->note, c->fineTune, &c->fineTuneOfs);
    c->voiceVolScale = e->volume;
    c->pan = ((e->pan & 0x7F) + 0x40) << 8;
    if (e->pan & AKAO_DRUM_REVERB) {
        g_akaoSeq->reverbMask |= chBit;
    } else {
        g_akaoSeq->reverbMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    return r;
}

void akao_execChannel(AkaoChannel *c, u32 chBit) {
    u32 op;
    u32 vel;
    u32 tmp;
    u32 f;
    u16 len;
    u32 v;

    do {
        op = *c->pc++;
        if (op >= AKAO_OP_FIRST_CMD) {
            if (op == AKAO_OP_EXT) {
                vel = *c->pc++;
                /* Not bounded, as in the original: g_akaoOpsFE has 0x20 handlers, but any byte the sequence holds
                 * indexes it. A channel reading garbage (e.g. a replaced overlay, ff9-ds d1_f0310: 0xFE 0x4F) jumps
                 * through whatever word follows the table; on the PS1 that is data (an address error there).
                 * A port may send indices >= 0x20 to akao_opInvalid. */
                g_akaoOpsFE[vel](c, chBit);
            } else if (op - AKAO_OP_NOTE_LEN < 14) {
                op = (op - AKAO_OP_NOTE_LEN) * AKAO_NOTE_LENGTHS;
                c->noteTicks = *c->pc++;
            } else {
                if (op == 0xFF) {
                    op = AKAO_OP_END;
                } else if (op == AKAO_OP_LOOP_FOREVER && (c->flags & AKAO_CH_STOP_REQUESTED)) {
                    op = AKAO_OP_END;
                    g_akaoSeState.keyOffMask |= chBit;
                }
                g_akaoOps[op - AKAO_OP_FIRST_CMD](c, chBit);
            }
        }
        c->opCount++;
    } while (op > AKAO_OP_END);
    if (op == AKAO_OP_END) {
        if (c->isSe == 0) {
            g_akaoSeq->keyOffMask |= chBit;
        }
        return;
    }
    vel = akao_peekNextNote(c) & 0xFF;
    if (c->fixedLen != 0) {
        c->noteTicks = c->gateTicks = c->fixedLen;
    }
    if (c->noteTicks != 0) {
        if (vel >= AKAO_NOTE_REST || (vel < AKAO_NOTE_TIE && !(c->legato & (AKAO_LEGATO | AKAO_LEGATO_SE)))) {
            c->gateTicks -= 2;
        }
    } else {
        c->noteTicks = g_akaoNoteLengths[op % AKAO_NOTE_LENGTHS];
        len = c->noteTicks;
        if ((vel < AKAO_NOTE_TIE || vel >= AKAO_NOTE_REST) && !(c->legato & (AKAO_LEGATO | AKAO_LEGATO_SE))) {
            len -= 2;
        }
        c->gateTicks = len;
    }
    if (c->isSe == 0 && (c->flags & AKAO_CH_FULL_GATE)) {
        c->gateTicks = c->noteTicks;
    }
    c->noteLen = c->noteTicks;
    c->voiceDirty |= AKAO_VD_RELEASE_RATE;
    if (op >= AKAO_NOTE_REST) {
        if (c->isSe == 0) {
            g_akaoSeq->playingMask &= ~chBit;
            if (c->voice < 0x18) {
                g_akaoSeq->keyOffMask |= chBit;
            }
        }
        c->portamentoLen = 0;
        c->vibratoOut = 0;
        c->tremoloOut = 0;
        c->legato &= ~AKAO_LEGATO_TIED;
        return;
    }
    if (op < AKAO_NOTE_TIE) {
        op /= AKAO_NOTE_LENGTHS;
        op += c->octave * 12;
        f = c->flags;
        if (f & AKAO_CH_DRUM_KIT) {
            vel = akao_playDrumNote(c, chBit, op);
        } else {
            if (!(c->legato & AKAO_LEGATO_TIED)) {
                if (c->isSe == 0) {
                    if (f & AKAO_CH_KEY_SPLIT) {
                        akao_applyKeySplit(c, op);
                    }
                    g_akaoSeq->keyOnMask |= chBit;
                    if ((g_akaoSeq->playingMask & chBit) && c->voice < 0x18) {
                        g_akaoSeq->keyOffMask |= chBit;
                    }
                    if (c->noteVolumeSlideLen != 0) {
                        c->volumeSlideLen = c->noteVolumeSlideLen;
                        c->volume = c->noteVolume;
                        c->volumeStep = c->noteVolumeStep;
                    }
                } else {
                    g_akaoSeState.keyOnMask |= chBit;
                }
                c->pitchSlideLeft = 0;
            }
            if (c->portamentoLen != 0 && c->prevKey != 0) {
                c->slideLen = c->portamentoLen;
                c->slideKeys = c->transpose + op - c->prevKey - c->prevTranspose;
                c->curKey = c->prevKey - (c->transpose - c->prevTranspose);
                op = c->prevKey + c->prevTranspose;
            } else {
                c->curKey = op;
                op += c->transpose;
            }
            vel = ((s32 (*)())akao_calcPitch)(&g_akaoInstruments[c->instrument], op, c->fineTune, &c->fineTuneOfs);
            if (c->pitchRandom != 0) {
                tmp = (u32)(vel * c->pitchRandom) >> 8;
                tmp *= (u8)g_akaoLfoTableData[g_akaoRandIndex];
                if ((u8)g_akaoLfoTableData[g_akaoRandIndex] & 0x80) {
                    tmp >>= 9;
                    vel -= tmp;
                } else {
                    tmp >>= 7;
                    vel += tmp;
                }
            }
        }
        c->pitch = vel;
        if (c->isSe == 0) {
            g_akaoSeq->playingMask |= chBit;
        } else {
            g_akaoSeState.playingMask |= chBit;
        }
        op = c->flags;
        c->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH;
        if (op & AKAO_CH_VIBRATO) {
            v = (c->vibratoDepthParam & 0x7F00) >> 8;
            if (!(c->vibratoDepthParam & AKAO_VIB_FULL_SCALE)) {
                v = (v * ((u32)(vel * 15) >> 8)) >> 7;
            } else {
                v = (v * vel) >> 7;
            }
            c->vibratoDepth = v;
            if (!(c->legato & AKAO_LEGATO_TIED)) {
                c->vibratoPhase = 0;
                c->vibratoDelayLeft = c->vibratoDelay;
                c->vibratoRate = 0x1000000 / ((u32)c->vibratoPeriod >> 12);
            }
        }
        if ((op & AKAO_CH_TREMOLO) && !(c->legato & AKAO_LEGATO_TIED)) {
            c->tremoloPhase = 0;
            c->tremoloDelayLeft = c->tremoloDelay;
            c->tremoloRate = 0x1000000 / ((u32)c->tremoloPeriod >> 12);
        }
        c->vibratoOut = 0;
        c->tremoloOut = 0;
        c->pitchSlide = 0;
    }
    c->legato = (c->legato & ~AKAO_LEGATO_TIED) | ((c->legato & AKAO_LEGATO) << 1);
    if (c->slideKeys != 0) {
        c->curKey += c->slideKeys;
        vel = ((s32 (*)())akao_calcPitch)(&g_akaoInstruments[c->instrument], c->curKey + c->transpose, c->fineTune, &tmp) << 16;
        c->pitchSlideLeft = c->slideLen;
        c->slideKeys = 0;
        c->pitchSlideStep = (s32)(vel - ((c->pitch << 16) + c->pitchSlide)) / (u16)c->pitchSlideLeft;
    }
    c->prevKey = c->curKey;
    c->prevTranspose = c->transpose;
}

s32 akao_initSlide(s32 *p, s32 target, s32 steps, s32 shift) {
    s32 fracMask = (1 << shift) - 1;
    s32 cur = *p & ~fracMask;
    s32 step;
    target <<= shift;
    step = target - cur;
    step = step / steps;
    *p = cur;
    if (target < cur) {
        *p = cur | fracMask;
        step--;
    } else {
        step++;
    }
    return step;
}

void akao_setChannelInstrument(AkaoChannel *ch, AkaoInstrument *p, s32 startAddr) {
    ch->voiceStartAddr = startAddr;
    ch->voiceLoopAddr = p->loopAddr;
    ch->voiceAdsr1 = p->adsr1;
    ch->voiceAdsr2 = p->adsr2;
    ch->voiceDirty |= AKAO_VD_START_ADDR | AKAO_VD_ADSR1 | AKAO_VD_ADSR2 | AKAO_VD_LOOP_ADDR;
}

void akao_setChannelProgram(AkaoChannel *c, s32 i) {
    c->instrument = i;
    akao_setChannelInstrument(c, &g_akaoInstruments[i], g_akaoInstruments[i].startAddr);
}

void akao_stopSeChannel(AkaoChannel *c, u32 chBit) {
    g_akaoSeState.activeMask &= ~chBit;
    g_akaoSeState.pausedMask &= ~chBit;
    g_akaoSeState.noiseMask &= ~chBit;
    g_akaoSeState.reverbMask &= ~chBit;
    g_akaoSeState.pitchModMask &= ~chBit;
    g_akaoSeState.keyOnMask &= ~chBit;
    g_akaoSeState.playingMask &= ~chBit;
    c->seMask = 0;
    c->seHandle = 0;
}

s32 akao_remapSeInstrument(s32 bank, s32 instr) {
    if (bank != 0) {
        if ((u32)(instr - 0xC0) < 0x20) {
            return instr + bank * 8;
        }
        if ((u32)(instr - 0xE0) < 0x18) {
            return instr + (bank - 4) * 8;
        }
    }
    return instr;
}

void akao_opEnd(AkaoChannel *c, s32 chBit) {
    AkaoSeq *t;

    if (c->isSe == 0) {
        t = g_akaoSeq;
        if ((t->activeMask &= ~chBit) == 0) {
            g_akaoSkipMeasures = 0;
            t->musicId = 0;
            t->flags = 0;
        }
        g_akaoSeq->playingMask &= ~chBit;
        g_akaoSeq->priorityMask &= ~chBit;
        g_akaoSeq->keepVoiceMask &= ~chBit;
        g_akaoSeq->noiseMask &= ~chBit;
        g_akaoSeq->reverbMask &= ~chBit;
        g_akaoSeq->pitchModMask &= ~chBit;
    } else {
        akao_stopSeChannel(c, chBit);
    }
    c->flags = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_opFETempo(AkaoChannel *c) {
    AkaoSeq *a = g_akaoSeq;
    s32 v = c->pc[0] << 16;
    a->tempo.w = v;
    a->tempo.w = v | (c->pc[1] << 24);
    c->pc += 2;
    a->tempoSlideLen = 0;
}

void akao_opFETempoSlide(AkaoChannel *c) {
    s32 x;
    s32 m;

    if ((g_akaoSeq->tempoSlideLen = *c->pc++) == 0) {
        g_akaoSeq->tempoSlideLen = 0x100;
    }
    x = (c->pc[0] << 16) | (c->pc[1] << 24);
    c->pc += 2;
    m = g_akaoSeq->tempo.w & 0xFFFF0000;
    g_akaoSeq->tempoStep = (x - m) / g_akaoSeq->tempoSlideLen;
    g_akaoSeq->tempo.w = m;
}

void akao_opFE02(AkaoChannel *c) {
    AkaoSeq *a = g_akaoSeq;
    u8 *p = c->pc;
    s32 hi = (s8)p[1];
    s32 lo = p[0];
    s32 v;
    c->pc = p + 2;
    v = hi << 20;
    v |= lo << 12;
    a->reverbDepthSlideLen = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_REVERB_DEPTH;
    a->reverbDepth = v;
}

void akao_opFE03(AkaoChannel *c) {
    u8 *p;
    s32 v;

    g_akaoSeq->reverbDepthSlideLen = *c->pc++;
    if (g_akaoSeq->reverbDepthSlideLen == 0) {
        g_akaoSeq->reverbDepthSlideLen = 0x100;
    }
    p = c->pc;
    v = ((s8)p[1] << 20) | (p[0] << 12);
    c->pc = p + 2;
    g_akaoSeq->reverbDepth &= ~0xFFF;
    g_akaoSeq->reverbDepthStep = (v - g_akaoSeq->reverbDepth) / g_akaoSeq->reverbDepthSlideLen;
}

void akao_opFEJump(AkaoChannel *c) {
    u8 *p = c->pc;
    c->pc = p + (s16)(p[0] | (p[1] << 8));
}

void akao_opFEJumpIfGe(AkaoChannel *c) {
    u8 *p = c->pc;
    s32 v = *p++;
    c->pc = p;
    if (g_akaoSeq->condition >= v) {
        c->pc = p + (s16)(p[0] | (p[1] << 8));
    } else {
        c->pc = p + 2;
    }
}

void akao_opFECall(AkaoChannel *c) {
    u8 *p = c->pc;
    s16 off = p[0] | (p[1] << 8);
    c->returnAddr = p + 2;
    c->pc += off;
}

void akao_opFEReturn(AkaoChannel *c) {
    c->pc = c->returnAddr;
}

void akao_opA3(AkaoChannel *c) {
    c->expression = *c->pc++ << 8;
    c->voiceDirty |= AKAO_VD_VOL;
}

void akao_opFE12(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->pc++;
    ch->expressionSlideLen = v;
    if (v == 0) {
        ch->expressionSlideLen = 0x100;
    }
    p = ch->pc;
    base = ch->expression & 0x7F00;
    ch->expressionStep = ((*p << 8) - base) / ch->expressionSlideLen;
    ch->pc = p + 1;
    v = base;
    ch->expression = v;
}

void akao_opA8(AkaoChannel *c) {
    s32 v = (s8)*c->pc++;
    c->volumeSlideLen = 0;
    c->voiceDirty |= AKAO_VD_VOL;
    c->noteVolumeSlideLen = 0;
    c->volume = v << 23;
}

void akao_opA9(AkaoChannel *c) {
    s32 v;

    if ((c->volumeSlideLen = *c->pc++) == 0) {
        c->volumeSlideLen = 0x100;
    }
    v = c->volume & 0xFFFF0000;
    c->volumeStep = (((s8)*c->pc++ << 23) - v) / (u16)c->volumeSlideLen;
    c->volume = v;
    c->noteVolumeSlideLen = 0;
}

void akao_opFE19(AkaoChannel *c) {
    c->noteVolume = (s8)*c->pc++ << 23;
    if ((c->noteVolumeSlideLen = *c->pc++) == 0) {
        c->noteVolumeSlideLen = 0x100;
    }
    c->noteVolumeStep = (((s8)*c->pc++ << 23) - c->noteVolume) / (u16)c->noteVolumeSlideLen;
}

void akao_opFE1A(AkaoChannel *c) {
    c->flags |= AKAO_CH_FULL_GATE;
}

void akao_opFE1B(AkaoChannel *c) {
    c->flags &= ~AKAO_CH_FULL_GATE;
}

void akao_opAA(AkaoChannel *ch) {
    u8 *p = ch->pc;
    s32 v = *p;
    ch->pc = p + 1;
    ch->panSlideLen = 0;
    ch->voiceDirty |= AKAO_VD_VOL;
    ch->pan = ((v + 0x40) & 0xFF) << 8;
}

void akao_opAB(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->pc++;
    ch->panSlideLen = v;
    if (v == 0) {
        ch->panSlideLen = 0x100;
    }
    p = ch->pc;
    base = ch->pan & 0xFF00;
    ch->panStep = ((((*p + 0x40) & 0xFF) << 8) - base) / ch->panSlideLen;
    ch->pc = p + 1;
    v = base;
    ch->pan = v;
}

void akao_opOctave(AkaoChannel *c) {
    c->octave = *c->pc++;
}

void akao_opOctaveUp(AkaoChannel *c) {
    c->octave = (c->octave + 1) & 0xF;
}

void akao_opOctaveDown(AkaoChannel *c) {
    c->octave = (c->octave - 1) & 0xF;
}

void akao_opProgram(AkaoChannel *c) {
    s32 i;
    AkaoInstrument *e;
    s32 b = *c->pc++;

    if (c->isSe == 0) {
        i = akao_remapInstrument(g_akaoSeq->flags, b);
    } else {
        i = akao_remapSeInstrument(c->seBank, b);
    }
    e = &g_akaoInstruments[i];
    akao_setChannelInstrument(c, e, e->startAddr);
    c->instrument = i;
    c->voiceVolScale = 0;
    c->flags &= ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT);
}

void akao_opFE0A(AkaoChannel *c) {
    s32 i = akao_remapInstrument(g_akaoSeq->flags, *c->pc++);
    akao_setChannelInstrument(c, &g_akaoInstruments[i], 0x1010);
    c->instrument = i;
    c->voiceVolScale = 0;
    c->flags &= ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT);
}

void akao_opFE14(AkaoChannel *c) {
    u8 v = *c->pc++;
    u16 *tbl = g_akaoSeq->regionTable;
    if (tbl != NULL) {
        if (tbl[v] > 0x8000) {
            c->voiceVolScale = 0;
            c->flags &= ~AKAO_CH_KEY_SPLIT;
        } else {
            u8 *p = (u8 *)tbl + tbl[v];
            c->curKey = 0xFF;
            c->regions = p + 0x20;
            c->flags = (c->flags & ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT)) | AKAO_CH_KEY_SPLIT;
        }
    }
}

void akao_opB3(AkaoChannel *c) {
    AkaoInstrument *e = &g_akaoInstruments[c->instrument];
    c->voiceAdsr1 = e->adsr1;
    c->voiceAdsr2 = e->adsr2;
    c->voiceDirty |= AKAO_VD_ADSR1 | AKAO_VD_ADSR2;
    c->flags &= ~AKAO_CH_ADSR_SET;
}

void akao_opC0(AkaoChannel *c) {
    c->transpose = (s8)*c->pc++;
}

void akao_opC1(AkaoChannel *c) {
    c->transpose += (s8)*c->pc++;
}

void akao_opA4(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->slideLen = v;
    if (v == 0) {
        c->slideLen = 0x100;
    }
    c->slideKeys = (s8)*c->pc++;
}

void akao_opDA(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->portamentoLen = v;
    if (v == 0) {
        c->portamentoLen = 0x100;
    }
    c->prevTranspose = 0;
    c->prevKey = 0;
    c->legato = AKAO_LEGATO;
}

void akao_opDB(AkaoChannel *c) {
    c->portamentoLen = 0;
}

void akao_opD8(AkaoChannel *c) {
    u8 *p = c->pc;
    u32 k;
    u32 prod;
    u32 r;
    c->fineTune = (s8)*p;
    k = c->pitch;
    r = (u8)c->fineTune;
    prod = k * r;
    c->pc = p + 1;
    if (c->fineTune < 0) {
        r = (prod >> 8) - k;
    } else {
        r = prod >> 7;
    }
    c->fineTuneOfs = r;
    c->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opD9(AkaoChannel *c) {
    u32 r;

    c->fineTune += (s8)*c->pc++;
    r = c->pitch * (u8)c->fineTune;
    if (c->fineTune < 0) {
        r = (r >> 8) - c->pitch;
    } else {
        r >>= 7;
    }
    c->fineTuneOfs = r;
    c->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opB4(AkaoChannel *c) {
    u32 b;
    s16 v;
    u32 n;
    u32 r;
    u32 k;
    u32 t;

    c->flags |= AKAO_CH_VIBRATO;
    if (c->isSe != 0) {
        c->vibratoDelay = 0;
        b = *c->pc++;
        if (b != 0) {
            c->vibratoDepthParam = b << 8;
        }
    } else {
        c->vibratoDelay = *c->pc++;
    }
    c->vibratoPeriod = t = *c->pc++ << 12;
    if (t == 0) {
        c->vibratoPeriod = 0x100000;
    }
    b = *c->pc++;
    v = c->vibratoDepthParam;
    k = (u16)c->pitch;
    n = (v & 0x7F00) >> 8;
    if (!(v & AKAO_VIB_FULL_SCALE)) {
        r = (n * ((s32)(k * 15) >> 8)) >> 7;
    } else {
        r = (n * k) >> 7;
    }
    c->vibratoDepth = r;
    c->vibratoPeriodSlideLen = 0;
    c->vibratoDelayLeft = c->vibratoDelay;
    c->vibratoWave = g_akaoLfoWaves[b & 7];
    c->vibratoRate = 0x1000000 / ((u32)c->vibratoPeriod >> 12);
}

void akao_opB5(AkaoChannel *c) {
    s32 k;
    s16 v;
    u32 n;
    u32 r;

    c->vibratoDepthParam = *c->pc++ << 8;
    k = c->pitch;
    v = c->vibratoDepthParam;
    n = (v & 0x7F00) >> 8;
    if (!(v & AKAO_VIB_FULL_SCALE)) {
        r = n * ((s32)(k * 15) >> 8);
    } else {
        r = n * k;
    }
    c->vibratoDepth = r >> 7;
}

void akao_opDD(AkaoChannel *c) {
    s32 n = *c->pc++;

    if (n == 0) {
        n = 0x100;
    }
    c->vibratoDepthStep = ((*c->pc++ << 8) - c->vibratoDepthParam) / n;
    c->vibratoDepthSlideLen = n;
}

void akao_opE4(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v == 0) {
        v = 0x100;
    }
    c->vibratoPeriodSlideLen = v;
    c->vibratoPeriodStep = akao_initSlide(&c->vibratoPeriod, *c->pc++, v, 0xC);
}

void akao_opB6(AkaoChannel *c) {
    c->vibratoOut = 0;
    c->flags &= ~AKAO_CH_VIBRATO;
    c->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opB8(AkaoChannel *c) {
    s32 v;
    u32 d;

    c->flags |= AKAO_CH_TREMOLO;
    v = *c->pc++;
    if (c->isSe != 0) {
        c->tremoloDelay = 0;
        if (v != 0) {
            c->tremoloDepth = (v & 0x7F) << 8;
        }
    } else {
        c->tremoloDelay = v;
    }
    c->tremoloPeriod = *c->pc++ << 12;
    if (c->tremoloPeriod == 0) {
        c->tremoloPeriod = 0x100000;
    }
    v = *c->pc++;
    d = 0x1000000 / ((u32)c->tremoloPeriod >> 12);
    c->tremoloDelayLeft = c->tremoloDelay;
    c->tremoloPeriodSlideLen = 0;
    c->tremoloWave = g_akaoLfoWaves[v & 7];
    c->tremoloRate = d;
}

void akao_opB9(AkaoChannel *c) {
    c->tremoloDepth = (*c->pc++ & 0x7F) << 8;
}

void akao_opDE(AkaoChannel *c) {
    s32 n = *c->pc++;

    if (n == 0) {
        n = 0x100;
    }
    c->tremoloDepthStep = (((*c->pc++ & 0x7F) << 8) - c->tremoloDepth) / n;
    c->tremoloDepthSlideLen = n;
}

void akao_opE5(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v == 0) {
        v = 0x100;
    }
    c->tremoloPeriodSlideLen = v;
    c->tremoloPeriodStep = akao_initSlide(&c->tremoloPeriod, *c->pc++, v, 0xC);
}

void akao_opBA(AkaoChannel *c) {
    c->tremoloOut = 0;
    c->flags &= ~AKAO_CH_TREMOLO;
    c->voiceDirty |= AKAO_VD_VOL;
}

void akao_opBC(AkaoChannel *c) {
    c->flags |= AKAO_CH_PAN_LFO;
    c->panLfoPeriod = *c->pc++ << 12;
    if (c->panLfoPeriod == 0) {
        c->panLfoPeriod = 0x100000;
    }
    {
        u32 d = 0x1000000 / ((u32)c->panLfoPeriod >> 12);
        u8 b = *c->pc++;
        c->panLfoPeriodSlideLen = 0;
        c->panLfoPhase = 0;
        c->panLfoRate = d;
        c->panLfoWave = g_akaoLfoWaves[b & 7];
    }
}

void akao_opBD(AkaoChannel *c) {
    c->panLfoDepth = *c->pc++ << 7;
}

void akao_opDF(AkaoChannel *c) {
    s32 n = *c->pc++;

    if (n == 0) {
        n = 0x100;
    }
    c->panLfoDepthStep = ((*c->pc++ << 7) - c->panLfoDepth) / n;
    c->panLfoDepthSlideLen = n;
}

void akao_opE6(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v == 0) {
        v = 0x100;
    }
    c->panLfoPeriodSlideLen = v;
    c->panLfoPeriodStep = akao_initSlide(&c->panLfoPeriod, *c->pc++, v, 0xC);
}

void akao_opBE(AkaoChannel *c) {
    c->panLfoOut = 0;
    c->flags &= ~AKAO_CH_PAN_LFO;
    c->voiceDirty |= AKAO_VD_VOL;
}

void akao_opC4(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->noiseMask |= chBit;
    } else {
        g_akaoSeState.noiseMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_opC5(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->noiseMask &= ~chBit;
    } else {
        g_akaoSeState.noiseMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
    c->noiseTimer = 0;
}

void akao_opC6(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->pitchModMask |= chBit;
    } else if (c->flags & AKAO_CH_SE_SECOND) {
        g_akaoSeState.pitchModMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opC7(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->pitchModMask &= ~chBit;
    } else {
        g_akaoSeState.pitchModMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    c->pitchModTimer = 0;
}

void akao_opC2(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->reverbMask |= chBit;
    } else {
        g_akaoSeState.reverbMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opC3(AkaoChannel *c, u32 chBit) {
    if (c->isSe == 0) {
        g_akaoSeq->reverbMask &= ~chBit;
    } else {
        g_akaoSeState.reverbMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opCC(AkaoChannel *c) {
    c->legato = AKAO_LEGATO;
}

void akao_opCD(void) {
}

void akao_opD0(AkaoChannel *c) {
    if (c->isSe != 0) {
        c->legato = AKAO_LEGATO_SE;
    }
}

void akao_opD1(void) {
}

void akao_opAC(AkaoChannel *c) {
    u32 b = *c->pc++;

    if (c->isSe == 0) {
        if (b & 0xC0) {
            g_akaoSeq->noiseClock = (g_akaoSeq->noiseClock + (b & 0x3F)) & 0x3F;
        } else {
            g_akaoSeq->noiseClock = b;
        }
    } else if (b & 0xC0) {
        g_akaoSeState.noiseClock = (g_akaoSeState.noiseClock + (b & 0x3F)) & 0x3F;
    } else {
        g_akaoSeState.noiseClock = b;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK;
}

void akao_opAD(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE;
    c->flags |= AKAO_CH_ATTACK_SET;
    c->voiceAdsr1 = (c->voiceAdsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL)) | (v << 8);
}

void akao_opAE(AkaoChannel *c, s32 unused) {
    u16 t = c->voiceAdsr1 & ~SPU_ADSR1_DECAY_RATE;
    c->voiceAdsr1 = t | (*c->pc++ << 4);
    c->voiceDirty |= AKAO_VD_DECAY_RATE;
}

void akao_opAF(AkaoChannel *c, s32 unused) {
    u16 t = c->voiceAdsr1 & ~SPU_ADSR1_SUSTAIN_LEVEL;
    c->voiceAdsr1 = t | *c->pc++;
    c->voiceDirty |= AKAO_VD_SUSTAIN_LEVEL;
}

void akao_opB1(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= AKAO_VD_SUSTAIN_MODE | AKAO_VD_SUSTAIN_RATE;
    c->flags |= AKAO_CH_SUSTAIN_RATE_SET;
    c->voiceAdsr2 = (c->voiceAdsr2 & ~SPU_ADSR2_SUSTAIN_RATE) | (v << 6);
}

void akao_opB2(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= AKAO_VD_RELEASE_MODE | AKAO_VD_RELEASE_RATE;
    c->flags |= AKAO_CH_RELEASE_RATE_SET;
    c->voiceAdsr2 = (c->voiceAdsr2 & ~SPU_ADSR2_RELEASE_RATE) | v;
}

void akao_opB7(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceAdsr1 &= ~SPU_ADSR1_ATTACK_EXP;
    if (v == AKAO_ENV_EXP_INC) {
        c->voiceAdsr1 |= SPU_ADSR1_ATTACK_EXP;
    }
    c->voiceDirty |= AKAO_VD_ATTACK_MODE;
}

void akao_opBB(AkaoChannel *c) {
    u16 v = *c->pc++;
    u16 x = c->voiceAdsr2 & ~(SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC);
    c->voiceAdsr2 = x;
    switch (v) {
    case AKAO_ENV_LINEAR_DEC:
        c->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        c->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        c->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    c->voiceDirty |= AKAO_VD_SUSTAIN_MODE;
}

void akao_opBF(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_EXP;
    if (v == AKAO_ENV_EXP_DEC) {
        c->voiceAdsr2 |= SPU_ADSR2_RELEASE_EXP;
    }
    c->voiceDirty |= AKAO_VD_RELEASE_MODE;
}

void akao_opFE10(AkaoChannel *c) {
    g_akaoSeq->firstVoice = *c->pc++;
}

void akao_opFE11(void) {
    g_akaoSeq->firstVoice = 0;
}

void akao_opC8(AkaoChannel *c) {
    c->loopDepth = (c->loopDepth + 1) & 3;
    c->loopStart[c->loopDepth] = c->pc;
    c->loopCount[c->loopDepth] = 0;
    c->loopOpCount[c->loopDepth] = c->opCount;
}

void akao_opC9(AkaoChannel *c) {
    u32 n = *c->pc++;

    if (n == 0) {
        n = 0x100;
    }
    if (++c->loopCount[c->loopDepth] != n) {
        c->pc = c->loopStart[c->loopDepth];
        c->opCount = c->loopOpCount[c->loopDepth];
        return;
    }
    c->loopDepth = (c->loopDepth - 1) & 3;
}

void akao_opFE08(AkaoChannel *c) {
    u8 *p = c->pc;
    s32 v = *p++;
    c->pc = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->loopCount[c->loopDepth] + 1 != v) {
        c->pc = p + 2;
    } else {
        c->pc = p + (s16)(p[0] | (p[1] << 8));
    }
}

void akao_opFE09(AkaoChannel *c) {
    u8 *p = c->pc;
    s32 v = *p++;
    c->pc = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->loopCount[c->loopDepth] + 1 != v) {
        c->pc = p + 2;
    } else {
        c->pc = p + (s16)(p[0] | (p[1] << 8));
        c->loopDepth = (c->loopDepth - 1) & 3;
    }
}

void akao_opCA(AkaoChannel *c) {
    c->loopCount[c->loopDepth]++;
    c->pc = c->loopStart[c->loopDepth];
    c->opCount = c->loopOpCount[c->loopDepth];
}

void akao_opA2(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->fixedLen = 0;
    c->gateTicks = v;
    c->noteTicks = v;
    c->noteLen = v;
}

void akao_opDC(AkaoChannel *c) {
    s32 v = (s8)*c->pc++;
    if (v != 0) {
        v += (s16)c->noteLen;
        if (v < 1) {
            v = 1;
        } else if (v >= 0x100) {
            v = 0xFF;
        }
    }
    c->fixedLen = v;
}

void akao_opFE04(AkaoChannel *c) {
    if (g_akaoSeq->drumKit != 0) {
        c->flags = (c->flags & ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT)) | AKAO_CH_DRUM_KIT;
    }
}

void akao_opFE05(AkaoChannel *c) {
    c->voiceVolScale = 0;
    c->flags &= ~AKAO_CH_DRUM_KIT;
}

void akao_opFE15(AkaoChannel *c) {
    AkaoSeq *d = g_akaoSeq;
    d->ticksPerBeat = *c->pc++;
    d->beatsPerMeasure = *c->pc++;
    d->tick = 0;
    d->beat = 0;
}

void akao_opFE16(AkaoChannel *c) {
    AkaoSeq *d = g_akaoSeq;
    d->measure = *c->pc++;
    d->measure |= *c->pc++ << 8;
}

void akao_opB0(AkaoChannel *c, s32 unused) {
    akao_opAE(c, unused);
    akao_opAF(c, unused);
}

void akao_opCE(AkaoChannel *c, u32 chBit) {
    s32 v = *c->pc++;
    c->noiseTimer = v ? v + 1 : 0x101;
    akao_opC4(c, chBit);
}

void akao_opCF(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v != 0) {
        c->noiseTimer = v + 1;
    } else {
        c->noiseTimer = 0x101;
    }
}

void akao_opD2(AkaoChannel *c, u32 chBit) {
    s32 v = *c->pc++;
    c->pitchModTimer = v ? v + 1 : 0x101;
    akao_opC6(c, chBit);
}

void akao_opD3(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v != 0) {
        c->pitchModTimer = v + 1;
    } else {
        c->pitchModTimer = 0x101;
    }
}

void akao_opCB(AkaoChannel *c, u32 chBit) {
    c->flags &= ~(AKAO_CH_VIBRATO | AKAO_CH_TREMOLO | AKAO_CH_PAN_LFO | AKAO_CH_PITCH_FROM_PREV | AKAO_CH_VOL_FROM_PREV);
    akao_opC5(c, chBit);
    akao_opC7(c, chBit);
    akao_opC3(c, chBit);
    c->legato &= ~(AKAO_LEGATO | AKAO_LEGATO_SE);
}

void akao_opD4(AkaoChannel *c) {
    c->flags |= AKAO_CH_PITCH_FROM_PREV;
}

void akao_opD5(AkaoChannel *c) {
    c->flags &= ~AKAO_CH_PITCH_FROM_PREV;
}

void akao_opD6(AkaoChannel *c) {
    c->flags |= AKAO_CH_VOL_FROM_PREV;
}

void akao_opD7(AkaoChannel *c) {
    c->flags &= ~AKAO_CH_VOL_FROM_PREV;
}

static inline u8 *akao_opFE0B_ref(u8 *p) {
    s32 o = (p[1] << 8) | p[0];
    return o != 0 ? p + o + 2 : NULL;
}

void akao_opFE0B(AkaoChannel *c) {
    u8 *p = c->pc;
    u8 *a;
    u8 *b;

    a = akao_opFE0B_ref(p);
    p += 2;
    b = akao_opFE0B_ref(p);
    g_akaoOpSeArgs.arg0 = 0;
    g_akaoOpSeArgs.arg1 = 0;
    g_akaoOpSeArgs.arg2 = c->pan >> 8;
    g_akaoOpSeArgs.arg3 = c->volume >> 23;
    akao_startSe(&g_akaoOpSeArgs, a, b, 0);
    c->pc += 4;
}

void akao_opE0(AkaoChannel *c) {
    c->flags |= AKAO_CH_STOP_AT_LOOP;
}

void akao_opFE1C(AkaoChannel *c) {
    c->pc++;
}

void akao_opFE1D(AkaoChannel *c, s32 b) {
    g_akaoSeq->priorityMask |= b;
}

void akao_opFE1E(AkaoChannel *c, s32 b) {
    g_akaoSeq->priorityMask &= ~b;
}

void akao_opE1(AkaoChannel *c) {
    c->pitchRandom = *c->pc++;
}

void akao_opE2(AkaoChannel *a) {
    a->pitchRandom = 0;
}

void akao_opInvalid(AkaoChannel *c, s32 b) {
    akao_opEnd(c, b);
}

void akao_streamStop(void) {
    u32 m;
    if (g_akaoStream.voiceMask != 0) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        akao_keyOff(g_akaoStream.voiceMask);
        if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
            akao_setVoiceLoopAddr(g_akaoStream.voice - 2, 0x1030);
            akao_setVoiceLoopAddr(g_akaoStream.voice - 1, 0x1030);
        }
        akao_setVoiceLoopAddr(g_akaoStream.voice, 0x1030);
        akao_setVoiceLoopAddr(g_akaoStream.voice + 1, 0x1030);
        m = g_akaoStream.voiceMask;
        g_akaoStream.voiceMask = 0;
        g_akaoSeState.reverbMask &= ~m;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
}

s32 akao_allocStreamVoices(void) {
    u32 busy;
    s32 i;
    u32 bit;

    do {
        bit = 0xC00000;
        busy = g_akaoSeState.activeMask | g_akaoSeState.pausedMask;
        for (i = 15; i != 0; i--, bit >>= 1) {
            if (!(busy & bit)) {
                break;
            }
        }
        if (i != 0) {
            return i + 7;
        }
        akao_stopSeChannels(0, 0x40000000);
    } while (busy != (g_akaoSeState.activeMask | g_akaoSeState.pausedMask));
    return -1;
}

void akao_streamUploadSecond(void) {
    u8 *p = g_akaoStream.readPtr + 0x800;
    SpuSetTransferStartAddr(0x2100);
    SpuSetTransferCallback(akao_streamStartMono);
    SpuWrite(p, 0x800);
}

void akao_streamStart(s32 addr, s32 vol, s32 flag) {
    StreamHeader *h;
    s32 v;
    s32 voiceBits;
    s32 size;
    s32 t;

    v = akao_allocStreamVoices();
    if (v == -1) {
        return;
    }
    g_akaoStream.pan = vol;
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)addr;
    g_akaoStream.readPtr = (u8 *)h + 0x40;
    g_akaoStream.bytesLeft = h->dataSize;
    voiceBits = (1 << v) | (1 << (v + 1));
    t = h->loopLength;
    g_akaoStream.voice = v;
    g_akaoStream.voiceMask = voiceBits;
    g_akaoStream.loopLength = t;
    akao_keyOff(voiceBits);
    g_akaoStream.flags = h->flags;
    g_akaoStream.pitch = h->pitch;
    g_akaoStream.voice = v;
    g_akaoStream.voiceMask = voiceBits;
    akao_keyOff(voiceBits);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    g_akaoStream.loopPtr = (g_akaoStream.flags & AKAO_STREAM_LOOP) ? (s32)(g_akaoStream.readPtr + h->loopStart) : 0;
    if (g_akaoStream.flags & AKAO_STREAM_STEREO) {
        g_akaoStream.loopBytes = (g_akaoStream.flags & AKAO_STREAM_LOOP) ? g_akaoStream.bytesLeft - (h->loopStart >> 1) : 0;
        SpuSetTransferCallback(akao_streamStartStereo);
        size = 0x2000;
    } else {
        g_akaoStream.loopBytes = (g_akaoStream.flags & AKAO_STREAM_LOOP) ? g_akaoStream.bytesLeft - h->loopStart : 0;
        SpuSetTransferCallback(akao_streamUploadSecond);
        size = 0x800;
    }
    SpuWrite(g_akaoStream.readPtr, size);
    if (flag) {
        g_akaoSeState.reverbMask |= g_akaoStream.voiceMask;
    } else {
        g_akaoSeState.reverbMask &= ~g_akaoStream.voiceMask;
    }
    g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
    g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_streamSetVoice(s32 v, s32 mode, s32 startAddr, s32 loopAddr) {
    s16 l;
    s16 r;
    s16 h;
    s32 pan;

    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        l = r = (g_akaoSeVolume * g_akaoMonoGain) >> 16;
    } else if (mode == 1) {
        r = 0;
        l = (u32)g_akaoSeVolume >> 1;
    } else if (mode == 2) {
        l = 0;
        r = (u32)g_akaoSeVolume >> 1;
    } else if (mode == 3) {
        h = g_akaoSeVolume >> 1;
        l = r = (h >> 1) + (h >> 2);
    } else {
        pan = (u8)(g_akaoStream.pan >> 8);
        l = (u32)(g_akaoStream.volume * g_akaoPanCurve[pan]) >> 16;
        r = (u32)(g_akaoStream.volume * g_akaoPanCurve[pan ^ 0xFF]) >> 16;
    }
    akao_setVoiceVolume(v, l, r, 0);
    akao_setVoicePitch(v, g_akaoStream.pitch);
    akao_setVoiceStartAddr(v, startAddr);
    akao_setVoiceLoopAddr(v, loopAddr);
    akao_setVoiceAttack(v, 0, AKAO_ENV_LINEAR_INC);
    akao_setVoiceDecay(v, 0xF);
    akao_setVoiceSustainLevel(v, 0xF);
    akao_setVoiceSustain(v, 0x7F, AKAO_ENV_LINEAR_DEC);
    akao_setVoiceRelease(v, 6, AKAO_ENV_LINEAR_DEC);
}

void akao_streamKeyOn(s32 a, s32 b, void (*cb)(void)) {
    if (g_akaoStream.voiceMask != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.bytesLeft > 0x1000) {
            g_akaoStream.bytesLeft -= 0x1000;
            g_akaoStream.readPtr += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            b = 0x1030;
            g_akaoStream.bytesLeft = 0;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(g_akaoStream.voiceMask);
        SpuSetIRQ(1);
    }
}

void akao_streamStartMono(void) {
    akao_streamSetVoice(g_akaoStream.voice, 0, 0x1100, 0x2100);
    akao_streamSetVoice(g_akaoStream.voice + 1, 0, 0x1100, 0x2100);
    akao_streamKeyOn(0x1000, 0x2100, akao_streamRefillMonoA);
}

void akao_streamStartStereo(void) {
    akao_streamSetVoice(g_akaoStream.voice, 1, 0x1100, 0x2100);
    akao_streamSetVoice(g_akaoStream.voice + 1, 2, 0x1900, 0x2900);
    akao_streamKeyOn(0x2000, 0x2100, akao_streamRefillStereoA);
}

void akao_streamRefill(s32 a, s32 b, s32 n, void (*cb)(void)) {
    if (g_akaoStream.voiceMask == 0 || g_akaoStream.bytesLeft == 0) {
        return;
    }
    SpuSetTransferStartAddr(a);
    akao_beginSpuTransfer();
    SpuWrite(g_akaoStream.readPtr, n);
    SpuSetIRQ(0);
    if (g_akaoStream.bytesLeft > 0x800) {
        SpuSetIRQCallback(cb);
        g_akaoStream.bytesLeft -= 0x800;
        g_akaoStream.readPtr += n;
    } else if (g_akaoStream.loopPtr != NULL) {
        SpuSetIRQCallback(cb);
        g_akaoStream.readPtr = g_akaoStream.loopPtr;
        g_akaoStream.bytesLeft = g_akaoStream.loopBytes;
    } else {
        SpuSetIRQCallback(akao_streamStop);
        b = 0x1030;
        a = b;
        g_akaoStream.bytesLeft = 0;
    }
    akao_setVoiceLoopAddr(g_akaoStream.voice, a);
    akao_setVoiceLoopAddr(g_akaoStream.voice + 1, b);
    SpuSetIRQAddr(a + 8);
    SpuSetIRQ(1);
}

void akao_streamRefillMonoA(void) {
    akao_streamRefill(0x1100, 0x1100, 0x800, akao_streamRefillMonoB);
}

void akao_streamRefillMonoB(void) {
    akao_streamRefill(0x2100, 0x2100, 0x800, akao_streamRefillMonoA);
}

void akao_streamRefillStereoA(void) {
    akao_streamRefill(0x1100, 0x1900, 0x1000, akao_streamRefillStereoB);
}

void akao_streamRefillStereoB(void) {
    akao_streamRefill(0x2100, 0x2900, 0x1000, akao_streamRefillStereoA);
}

void akao_cmdStreamStart(s32 *args) {
    akao_streamStop();
    akao_streamStart(args[0], args[1], args[2]);
    g_akaoSeState.activeMask &= ~g_akaoStream.voiceMask;
}

void akao_cmdStreamStop(void) {
    akao_streamStop();
}

void akao_cmdSetStreamVolume(s32 *args) {
    AkaoStreamState *d = &g_akaoStream;
    s32 v = *args;
    s32 l;
    s32 r;
    s32 pan;

    d->volumeFadeLen = 0;
    d->volume = v;
    if (d->voiceMask == 0) {
        return;
    }
    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        v = v * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(d->voice, v, v, 0);
        akao_setVoiceVolume(d->voice + 1, v, v, 0);
    } else if (d->flags & AKAO_STREAM_STEREO) {
        v <<= 15;
        v >>= 16;
        akao_setVoiceVolume(d->voice, v, 0, 0);
        akao_setVoiceVolume(d->voice + 1, 0, v, 0);
    } else {
        pan = (u8)(d->pan >> 8);
        l = v * g_akaoPanCurve[pan] >> 16;
        r = v * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->voice, l, r, 0);
        akao_setVoiceVolume(d->voice + 1, l, r, 0);
    }
}

void akao_cmdFadeStreamVolume(AkaoArgs *args) {
    s16 n = 1;

    if (args->arg0 != 0) {
        n = args->arg0;
    }
    g_akaoStream.volumeStep = (s16)((s16)((u16)args->arg1 - (u16)g_akaoStream.volume) / n);
    g_akaoStream.volumeFadeLen = n;
}

void akao_cmdSetStreamPan(s32 *args) {
    AkaoStreamState *d = &g_akaoStream;
    s32 p = *args;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    d->pan = p;
    if (d->voiceMask == 0) {
        return;
    }
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
        pan = (p >> 8) & 0xFF;
        l = d->volume * g_akaoPanCurve[pan] >> 16;
        r = d->volume * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->voice, l, r, 0);
        akao_setVoiceVolume(d->voice + 1, l, r, 0);
    }
}

void akao_streamArmStop(void) {
    SpuSetIRQAddr(0x1038);
    SpuSetIRQCallback(akao_streamStop);
}

void akao_streamOneShotStart(void) {
    u32 irq;
    if (g_akaoStream.bytesLeft > 0x2000) {
        SpuSetTransferStartAddr(g_akaoStream.spuAddr + 0x2000);
        akao_spuWrite(g_akaoStream.readPtr, g_akaoStream.bytesLeft - 0x2000);
        irq = g_akaoStream.spuAddr + 0x1FF8;
    } else {
        irq = g_akaoStream.spuAddr + (g_akaoStream.bytesLeft >> 1) + 8;
    }
    akao_streamSetVoice(g_akaoStream.voice, 0, g_akaoStream.spuAddr, 0x1030);
    akao_streamSetVoice(g_akaoStream.voice + 1, 0, g_akaoStream.spuAddr, 0x1030);
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(akao_streamArmStop);
    akao_keyOn(g_akaoStream.voiceMask);
    SpuSetIRQ(1);
}

void akao_streamPlayOneShot(s32 stream, s32 b, s32 addr, s32 loop) {
    AkaoStream *h = (AkaoStream *)stream;
    AkaoStreamState *d;
    s32 v;
    u32 n;
    u32 m;

    v = akao_allocStreamVoices();
    if (v != -1) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        d = &g_akaoStream;
        d->pan = b;
        g_akaoStream.readPtr = h->data;
        d->bytesLeft = h->dataSize;
        d->loopLength = h->loopLength;
        m = (1 << v) | (1 << (v + 1));
        n = d->voiceMask;
        d->voice = v;
        d->voiceMask = m;
        akao_keyOff(m | n);
        d->flags = h->flags;
        d->pitch = h->pitch;
        d->spuAddr = addr;
        SpuSetTransferMode(0);
        SpuSetTransferStartAddr(addr);
        g_akaoSpuBusy = 1;
        SpuSetTransferCallback(akao_streamOneShotStart);
        n = d->bytesLeft;
        if (n > 0x2000) {
            n = 0x2000;
        }
        SpuWrite(g_akaoStream.readPtr, n);
        g_akaoStream.readPtr += n;
        if (loop) {
            g_akaoSeState.reverbMask |= d->voiceMask;
        } else {
            g_akaoSeState.reverbMask &= ~d->voiceMask;
        }
        g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
        g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
}

void akao_streamPlayResident(s32 unused, s32 flag) {
    s32 *h = g_akaoResidentStream;
    s32 *h2;
    s32 v;
    s32 voiceBits;
    s32 irq;
    s32 old;

    if (h[8] == 0) {
        return;
    }
    v = akao_allocStreamVoices();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    voiceBits = (1 << v) | (1 << (v + 1));
    old = g_akaoStream.voiceMask;
    g_akaoStream.voice = v;
    g_akaoStream.voiceMask = voiceBits;
    akao_keyOff(voiceBits | old);
    h2 = h;
    g_akaoStream.flags = h2[6];
    g_akaoStream.pitch = (u16)h2[7];
    if (flag) {
        g_akaoSeState.reverbMask |= g_akaoStream.voiceMask;
    } else {
        g_akaoSeState.reverbMask &= ~g_akaoStream.voiceMask;
    }
    irq = h2[8] + ((u32)h2[4] >> 1) + 8;
    g_akaoSeState.pitchModMask &= ~g_akaoStream.voiceMask;
    g_akaoSeState.noiseMask &= ~g_akaoStream.voiceMask;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(akao_streamArmStop);
    if (h2[6] & 1) {
        if (h2[6] & 2) {
            akao_streamSetVoice(g_akaoStream.voice, 1, h2[8], h2[8] + h2[5]);
            akao_streamSetVoice(g_akaoStream.voice + 1, 2, h2[8] + h2[9], h2[8] + h2[9] + h2[5]);
        } else {
            akao_streamSetVoice(g_akaoStream.voice, 1, h2[8], 0x1030);
            akao_streamSetVoice(g_akaoStream.voice + 1, 2, h2[8] + h2[9], 0x1030);
        }
    } else {
        if (h2[6] & 2) {
            akao_streamSetVoice(g_akaoStream.voice, 3, h2[8], h2[8] + h2[5]);
            akao_streamSetVoice(g_akaoStream.voice + 1, 3, h2[8], h2[8] + h2[5]);
        } else {
            akao_streamSetVoice(g_akaoStream.voice, 3, h2[8], 0x1030);
            akao_streamSetVoice(g_akaoStream.voice + 1, 3, h2[8], 0x1030);
        }
    }
    akao_keyOn(g_akaoStreamVoices);
    SpuSetIRQ(1);
}

void akao_cmdStreamPlayOneShot(s32 *args) {
    akao_streamStop();
    akao_streamPlayOneShot(args[0], args[1], args[2], args[3]);
    g_akaoSeState.activeMask &= ~g_akaoStream.voiceMask;
}

void akao_cmdStreamPlayResident(s32 *args) {
    akao_streamStop();
    akao_streamPlayResident(args[0], args[1]);
    g_akaoSeState.activeMask &= ~g_akaoStream.voiceMask;
}

/* K&R definition: callers pass one or two arguments */
s32 akao_streamNextReadBlock(p)
    s32 *p;
{
    g_akaoStream.blocksRead++;
    (*p)++;
    if (g_akaoStream.ringBlocks - 1 < *p) {
        *p = 0;
    }
    return *p;
}

void akao_streamStartRing(void) {
    StreamHeader *h;
    AkaoStreamState *d;
    s32 v;
    s32 vol;
    s32 voiceBits;

    v = akao_allocStreamVoices();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)(g_akaoStream.spuAddr + 0x80);
    if (h->fadeIn != 0 && g_akaoStream.volumeFadeLen == 0) {
        vol = g_akaoStream.volume;
        g_akaoStream.volume = 0;
        akao_sendCmdE5(h->fadeIn, vol >> 8);
    }
    d = &g_akaoStream;
    g_akaoStream.readPtr = (u8 *)d->spuAddr;
    d->bytesLeft = h->dataSize;
    d->loopLength = h->loopLength;
    voiceBits = (1 << v) | (1 << (v + 1));
    d->voice = v;
    d->voiceMask = voiceBits;
    d->readBlock = 0;
    akao_keyOff(voiceBits);
    d->flags = h->flags;
    d->pitch = h->pitch;
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_streamStartRingVoices);
    if (d->flags & AKAO_STREAM_FOUR_VOICES) {
        SpuWrite(g_akaoStream.readPtr + 0xD0, 0x4000);
        akao_streamNextReadBlock(&d->readBlock);
        akao_streamNextReadBlock(&d->readBlock);
        akao_streamNextReadBlock(&d->readBlock);
        akao_streamNextReadBlock(&d->readBlock);
        d->voiceMask |= (1 << (v - 2)) | (1 << (v - 1));
    } else {
        SpuWrite(g_akaoStream.readPtr + 0xD0, 0x2000);
        akao_streamNextReadBlock(&d->readBlock);
        akao_streamNextReadBlock(&d->readBlock);
    }
    g_akaoSeState.reverbMask &= ~g_akaoStreamVoices;
    g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
    g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_streamRingKeyOn(s32 a, s32 b, void (*cb)(void)) {
    if (g_akaoStream.voiceMask != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.bytesLeft >= 0xE61) {
            g_akaoStream.readPtr += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            b = 0x1030;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(g_akaoStream.voiceMask);
        SpuSetIRQ(1);
    }
}

void akao_streamStartRingVoices(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamSetVoice(g_akaoStream.voice, 1, 0x1100, 0x3100);
        akao_streamSetVoice(g_akaoStream.voice + 1, 2, 0x1900, 0x3900);
        akao_streamSetVoice(g_akaoStream.voice - 2, 1, 0x2100, 0x4100);
        akao_streamSetVoice(g_akaoStream.voice - 1, 2, 0x2900, 0x4900);
        a = 0x4000;
        b = 0x3100;
    } else {
        akao_streamSetVoice(g_akaoStream.voice, 1, 0x1100, 0x2100);
        akao_streamSetVoice(g_akaoStream.voice + 1, 2, 0x1900, 0x2900);
        a = 0x2000;
        b = 0x2100;
    }
    akao_streamRingKeyOn(a, b, akao_streamRefillRingA);
}

s32 akao_streamRefillRing(s32 a, s32 b, s32 n, void (*cb)(void)) {
    AkaoStream *h;

    if (g_akaoStream.voiceMask != 0) {
        h = (AkaoStream *)(g_akaoStream.readPtr + 0x80);
        if (h->magic == AKAO_MAGIC) {
            SpuSetIRQ(0);
            SpuSetTransferStartAddr(a);
            akao_beginSpuTransfer();
            akao_streamNextReadBlock(&g_akaoStream.readBlock);
            SpuWrite(g_akaoStream.readPtr + 0xD0, n - 0xD0);
            g_akaoStream.unk20 = h->unk4;
            g_akaoStream.loopLength = h->loopLength;
            if (h->dataSize > h->loopLength) {
                SpuSetIRQCallback(cb);
                g_akaoStream.readPtr += n;
                if (g_akaoStream.readBlock == 0) {
                    g_akaoStream.readPtr = (u8 *)g_akaoStream.spuAddr;
                }
            } else {
                SpuSetIRQCallback(akao_streamStop);
                b = 0x1030;
                a = b;
            }
            akao_setVoiceLoopAddr(g_akaoStream.voice, a);
            akao_setVoiceLoopAddr(g_akaoStream.voice + 1, b);
            SpuSetIRQAddr(a + 8);
            SpuSetIRQ(1);
        }
    }
    return a;
}

void akao_streamRefillRingA(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamNextReadBlock(&g_akaoStream.readBlock);
        a = akao_streamRefillRing(0x1100, 0x1900, 0x2000, akao_streamRefillRingB);
        b = 0x2900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x2100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.voice - 2, a);
        akao_setVoiceLoopAddr(g_akaoStream.voice - 1, b);
    } else {
        akao_streamRefillRing(0x1100, 0x1900, 0x1000, akao_streamRefillRingB);
    }
}

void akao_streamRefillRingB(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamNextReadBlock(&g_akaoStream.readBlock);
        a = akao_streamRefillRing(0x3100, 0x3900, 0x2000, akao_streamRefillRingA);
        b = 0x4900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x4100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.voice - 2, a);
        akao_setVoiceLoopAddr(g_akaoStream.voice - 1, b);
    } else {
        akao_streamRefillRing(0x2100, 0x2900, 0x1000, akao_streamRefillRingA);
    }
}

void akao_cmdStreamSetupRing(s32 *args) {
    akao_streamStop();
    g_akaoStream.readBlock = -1;
    g_akaoStream.unk20 = 0;
    g_akaoStream.blocksWritten = 0;
    g_akaoStream.blocksRead = 0;
    g_akaoStream.writeBlock = 0;
    g_akaoStream.flags = AKAO_STREAM_RING_PENDING;
    g_akaoStream.ringBlocks = (u32)g_akaoArgs.arg1 >> 12;
    g_akaoStream.spuAddr = args[0];
    g_akaoStream.unk30 = args[1];
}


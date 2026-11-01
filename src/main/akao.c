#include "ff9.h"

void akao_applyCdVolume(void) {
    s32 vol = g_akaoCdVolumeHi;
    SPU_REG16(SPU_CD_VOL_L) = vol;
    SPU_REG16(SPU_CD_VOL_R) = vol;
}

void akao_copy(u32 *src, u32 *dst, u32 size) {
    u32 t0, t1, t2, t3;
    for (size >>= 2; size & ~3; size -= 4) {
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
    for (; size != 0; size--) {
        *dst++ = *src++;
    }
}

void akao_swap(u32 *bufA, u32 *bufB, u32 size) {
    u32 t0, t1, t2, t3;
    for (size >>= 2; size >> 1; size -= 2) {
        t0 = bufA[0];
        t1 = bufB[0];
        t2 = bufA[1];
        t3 = bufB[1];
        bufB[0] = t0;
        bufB[1] = t2;
        bufA[0] = t1;
        bufA[1] = t3;
        bufA += 2;
        bufB += 2;
    }
    if (size != 0) {
        t0 = *bufA;
        t1 = *bufB;
        *bufB = t0;
        *bufA = t1;
    }
}

void akao_tickSe(void) {
    s32 next;
    AkaoChannel *chan;
    u32 pending;
    AkaoStreamState *streamState;
    s32 old;
    s32 k;

    g_akaoRandIndex = (g_akaoRandIndex + 1) & 0xFF;
    if (g_akaoCdVolumeFadeTicks != 0) {
        g_akaoCdVolumeFadeTicks--;
        g_akaoCdVolume += g_akaoCdVolumeStep;
        akao_applyCdVolume();
    }
    streamState = &g_akaoStream;
    if (streamState->voiceMask != 0 && streamState->volumeFadeLen != 0) {
        streamState->volumeFadeLen--;
        old = streamState->volume;
        next = old + streamState->volumeStep;
        if ((next & 0xFF00) != (old & 0xFF00)) {
            if (g_akaoOutputFlags & AKAO_MODE_MONO) {
                old = (old * g_akaoMonoGain) >> 16;
                akao_setVoiceVolume(streamState->voice, old, old, 0);
                akao_setVoiceVolume(streamState->voice + 1, old, old, 0);
            } else {
                old = next << 15;
                old >>= 16;
                akao_setVoiceVolume(streamState->voice, old, 0, 0);
                akao_setVoiceVolume(streamState->voice + 1, 0, old, 0);
            }
        }
        g_akaoSeVolume = next & 0xFFFF;
    }
    if (g_akaoTempoScaleLen != 0) {
        g_akaoTempoScaleLen--;
        g_akaoTempoScale += g_akaoTempoScaleStep;
    }
    if (g_akaoPitchScaleLen != 0) {
        g_akaoPitchScaleLen--;
        next = g_akaoPitchScale + g_akaoPitchScaleStep;
        if ((next & 0xFF0000) != (g_akaoPitchScale & 0xFF0000)) {
            k = 32;
            chan = g_akaoChannels;
            do {
                k--;
                chan->voiceDirty |= AKAO_VD_PITCH;
                chan++;
            } while (k != 0);
        }
        g_akaoPitchScale = next;
    }
    if (g_akaoSeq->activeMask != 0) {
        if (g_akaoSeq->volumeFadeLen != 0) {
            g_akaoSeq->volumeFadeLen--;
            next = g_akaoSeq->volume + g_akaoSeq->volumeStep;
            if ((next & 0x7F0000) != (g_akaoSeq->volume & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->volume = next;
        }
        if (g_akaoSeq->panSlideLen != 0) {
            g_akaoSeq->panSlideLen--;
            next = g_akaoSeq->pan + g_akaoSeq->panStep;
            if ((next & 0x7F0000) != (g_akaoSeq->pan & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->pan = next;
        }
    }
    if (g_akaoSubSeq != NULL && g_akaoSubSeq->activeMask != 0) {
        if (g_akaoSubSeq->volumeFadeLen != 0) {
            g_akaoSubSeq->volumeFadeLen--;
            next = g_akaoSubSeq->volume + g_akaoSubSeq->volumeStep;
            if ((next & 0x7F0000) != (g_akaoSubSeq->volume & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
            }
            g_akaoSubSeq->volume = next;
        }
        if (g_akaoSubSeq->panSlideLen != 0) {
            g_akaoSubSeq->volumeFadeLen--;
            next = g_akaoSubSeq->pan + g_akaoSubSeq->panStep;
            if ((next & 0x7F0000) != (g_akaoSubSeq->pan & 0x7F0000)) {
                akao_markSeqDirty(g_akaoSubSeq, g_akaoSubChannels);
            }
            g_akaoSubSeq->pan = next;
        }
    }
    if (g_akaoSeState.activeMask != 0) {
        pending = g_akaoSeState.activeMask;
        chan = g_akaoSeChannels;
        k = 0x100;
        do {
            if (pending & k) {
                if ((u16)chan->seVolumeSlideLen != 0) {
                    chan->seVolumeSlideLen = (u16)chan->seVolumeSlideLen - 1;
                    next = (s16)chan->seVolume + chan->seVolumeStep;
                    if ((next & 0xFF00) != ((s16)chan->seVolume & 0xFF00)) {
                        chan->voiceDirty |= AKAO_VD_VOL;
                    }
                    chan->seVolume = next;
                }
                if (chan->sePanSlideLen != 0) {
                    chan->sePanSlideLen--;
                    next = chan->sePan + chan->sePanStep;
                    if ((next & 0xFF00) != (chan->sePan & 0xFF00)) {
                        chan->voiceDirty |= AKAO_VD_VOL;
                    }
                    chan->sePan = next;
                }
                if ((u16)chan->sePitchSlideLen != 0) {
                    chan->sePitchSlideLen = (u16)chan->sePitchSlideLen - 1;
                    next = chan->sePitch + chan->sePitchStep;
                    if ((next & 0xFF00) != (chan->sePitch & 0xFF00)) {
                        chan->voiceDirty |= AKAO_VD_PITCH;
                    }
                    chan->sePitch = next;
                }
                /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                if (next) {
                    pending ^= k;
                } else {
                    pending ^= k;
                }
            }
            k <<= 1;
            chan++;
        } while (pending != 0);
    }
}

u32 akao_tickChannels(AkaoChannel *chans, s32 sub) {
    u32 tempo;
    u32 tempoScale;
    s32 acc;
    AkaoChannel *chan;
    u32 bit;
    u32 active;
    s32 flags;

    tempo = g_akaoSeq->tempo.h[1];
    tempoScale = g_akaoTempoScaleHi;
    if (tempoScale != 0) {
        if (tempoScale < 0x80) {
            tempo += (tempo * tempoScale) >> 7;
        } else {
            tempo = (tempo * tempoScale) >> 8;
        }
    }
    acc = g_akaoSeq->tempoAcc + tempo;
    g_akaoSeq->tempoAcc = acc;
    if ((acc & 0xFFFF0000) || (g_akaoPauseFlags & AKAO_PAUSE_FORCE_TICK)) {
        g_akaoSeq->tempoAcc = acc & 0xFFFF;
        do {
            for (;;) {
                chan = chans;
                bit = 1;
                active = g_akaoSeq->activeMask;
                do {
                    if (active & bit) {
                        chan->noteTicks--;
                        chan->gateTicks--;
                        if (chan->noteTicks == 0) {
                            akao_execChannel(chan, bit);
                        } else if (chan->gateTicks == 0) {
                            g_akaoSeq->keyOffMask |= bit;
                        }
                        akao_stepChannelSlides(chan, bit, 0);
                        active &= ~bit;
                    }
                    chan++;
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
    s32 vol;

    if (g_akaoCrossfade.fadeLen != 0) {
        if (--g_akaoCrossfade.fadeLen == 0) {
            s32 mask = seq->activeMask;
            seq->musicId = 0;
            seq->activeMask = 0;
            seq->keyOnMask = 0;
            seq->playingMask = 0;
            seq->keyOffMask = mask;
            return;
        }
        vol = g_akaoCrossfade.volume + g_akaoCrossfade.volumeStep;
        if ((vol & 0xFFFF0000) != (g_akaoCrossfade.volume & 0xFFFF0000)) {
            akao_markSeqDirty(seq, ch, seq);
        }
        g_akaoCrossfade.volume = vol;
    }
}

s32 akao_tick(void) {
    s32 t0;
    s32 ticks;
    u32 now;
    AkaoSeq *sub;
    AkaoChannel *chan;
    u32 elapsed;
    u32 bit;
    u32 acc;
    s32 time1;
    s32 time2;
    s32 time3;

    t0 = GetRCnt(0xF2000002);
    g_akaoVsyncs[1] = g_akaoVsyncs[0];
    now = VSync(-1);
    ticks = 1;
    elapsed = now - g_akaoVsyncs[1];
    g_akaoVsyncs[0] = now;
    if (elapsed != 0) {
        if (elapsed < 9) {
            bit = elapsed * 4;
            if (g_akaoTickPhase < bit) {
                ticks = bit - g_akaoTickPhase + 1;
            }
            g_akaoTickPhase %= bit;
        }
    }
    do {
        ticks--;
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
                bit = AKAO_SE_CH_BIT(0);
                chan = g_akaoSeChannels;
                do {
                    if (elapsed & bit) {
                        if (!(g_akaoPauseFlags & AKAO_PAUSED_SE) || (chan->seMask & AKAO_SE_DEFAULT)) {
                            chan->age++;
                            chan->noteTicks--;
                            chan->gateTicks--;
                            if (chan->noteTicks == 0) {
                                akao_execChannel(chan, bit);
                            } else if (chan->gateTicks == 0) {
                                g_akaoSeState.keyOffMask |= bit;
                                g_akaoSeState.playingMask &= ~bit;
                            }
                            akao_stepChannelSlides(chan, bit, 1);
                        }
                        elapsed ^= bit;
                    }
                    chan++;
                    bit <<= 1;
                } while (elapsed != 0);
            }
        }
        if (!(g_akaoTickCount & 3)) {
            akao_tickSe();
        }
    } while (ticks != 0);
    t0 = GetRCnt(0xF2000002) - t0;
    if (t0 <= 0) {
        t0 += 0x44E8;
    }
    time1 = g_akaoTickTimes[1];
    time2 = g_akaoTickTimes[2];
    time3 = g_akaoTickTimes[3];
    g_akaoTickTimes[3] = t0;
    g_akaoTickTimes[0] = time1;
    g_akaoTickTimes[1] = time2;
    g_akaoTickTimes[2] = time3;
    g_akaoTickTimeSum = time1 + time2 + time3 + t0;
    return t0;
}

/* Looks ahead in the channel's sequence for the next note opcode (0xA0: rest or end). */
s32 akao_peekNextNote(AkaoChannel *chan) {
    u8 *p;
    u32 depth;
    u32 op;
    s32 len;

    p = chan->pc;
    depth = chan->loopDepth;
    for (;;) {
        if (*p < AKAO_NOTE_END) {
            if (*p >= AKAO_NOTE_REST) {
                chan->legato = (u16)chan->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
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
                if (*p == chan->loopCount[depth] + 1) {
                    p++;
                    depth--;
                    depth &= 3;
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
                p = chan->returnAddr;
                continue;
            }
            continue;
        case AKAO_OP_LOOP_END:
            p++;
            if (*p == chan->loopCount[depth] + 1) {
                p++;
                depth--;
                depth &= 3;
                continue;
            }
            p = chan->loopStart[depth];
            continue;
        case AKAO_OP_EFFECTS_OFF:
        case AKAO_OP_NOP_CD:
        case AKAO_OP_NOP_D1:
        case AKAO_OP_PORTAMENTO_OFF:
            chan->legato = (u16)chan->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
            p++;
            continue;
        case AKAO_OP_LOOP_FOREVER:
            if (chan->flags & AKAO_CH_STOP_REQUESTED) {
                goto end;
            }
            p = chan->loopStart[depth];
            continue;
        default:
        end:
            chan->legato = (u16)chan->legato & ~(AKAO_LEGATO | AKAO_LEGATO_SE);
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

void akao_applyKeySplit(AkaoChannel *chan, u32 key) {
    AkaoRegion *region;
    AkaoInstrument *instr;
    u32 chFlags;
    s32 instrId;

    if (chan->curKey < key || chan->curKey == 0xFF) {
        region = (AkaoRegion *)chan->regions;
        while (region[1].sustainMode != 0 && region->highKey < key) {
            region++;
        }
    } else if (key < chan->curKey) {
        region = (AkaoRegion *)chan->regions;
        while (region[1].sustainMode != 0 && !(key < region[1].lowKey)) {
            region++;
        }
    } else {
        return;
    }
    chFlags = chan->flags;
    instrId = akao_remapInstrument(g_akaoSeq->flags, region->sample);
    chan->instrument = instrId;
    instr = &g_akaoInstruments[instrId];
    chan->voiceStartAddr = instr->startAddr;
    chan->voiceLoopAddr = instr->loopAddr;
    if (!(chFlags & AKAO_CH_ATTACK_SET)) {
        chan->voiceAdsr1 = region->attack << 8;
    } else {
        chan->voiceAdsr1 &= SPU_ADSR1_ATTACK_RATE;
    }
    chan->voiceAdsr1 |= instr->adsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL);
    if (!(chFlags & AKAO_CH_SUSTAIN_RATE_SET)) {
        chan->voiceAdsr2 &= 0x201F;
        chan->voiceAdsr2 |= region->sustainRate << 6;
    } else {
        chan->voiceAdsr2 &= 0x3FDF;
    }
    switch (region->sustainMode) {
    case AKAO_ENV_LINEAR_DEC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    if (!(chFlags & AKAO_CH_RELEASE_RATE_SET)) {
        chan->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_RATE;
        chan->voiceAdsr2 |= region->releaseRate;
    }
    chan->voiceAdsr2 |= instr->adsr2 & SPU_ADSR2_RELEASE_EXP;
    chan->voiceVolScale = region->volume;
}

u16 akao_calcPitch(AkaoInstrument *instr, s32 note, u8 fine, s32 *out) {
    s32 tmp = note - instr->baseNote; /* semitone in the octave, then the octave shift (separate locals don't match) */
    s32 freq;
    u32 fineVal;

    while (tmp < 0) {
        tmp += 12;
    }
    tmp %= 12;
    if (instr->fineTune == 0) {
        freq = g_akaoSemitonePitch[tmp] << 8;
    } else if (instr->fineTune < 0) {
        freq = (g_akaoSemitonePitch[tmp] * (u16)instr->fineTune) >> 8;
    } else {
        freq = (g_akaoSemitonePitch[tmp] * instr->fineTune) >> 7;
        freq += g_akaoSemitonePitch[tmp] << 8;
    }
    fineVal = fine;
    if (fineVal != 0) {
        if (fineVal < 0x80) {
            *out = ((u32)freq * fineVal) >> 7;
        } else {
            *out = (((u32)freq * fineVal) >> 8) - freq;
        }
    }
    if (note < instr->baseNote) {
        do {
            *out >>= 1;
            freq >>= 1;
        } while ((note += 12) < instr->baseNote);
    } else {
        tmp = (note - instr->baseNote) / 12;
        if (tmp != 0) {
            freq <<= tmp;
            *out <<= tmp;
        }
    }
    *out >>= 8;
    freq >>= 8;
    return freq;
}

s32 akao_playDrumNote(AkaoChannel *chan, u32 chBit, s32 key) {
    AkaoDrumKey *drum;
    AkaoInstrument *instr;
    u32 chFlags;
    s32 instrId;
    s32 pitch;

    drum = (AkaoDrumKey *)g_akaoSeq->drumKit;
    drum += key;
    g_akaoSeq->keyOnMask |= chBit;
    if (g_akaoSeq->playingMask & chBit) {
        g_akaoSeq->keyOffMask |= chBit;
    }
    chFlags = chan->flags;
    instrId = akao_remapInstrument(g_akaoSeq->flags, drum->sample);
    chan->instrument = instrId;
    instr = &g_akaoInstruments[instrId];
    chan->voiceStartAddr = instr->startAddr;
    chan->voiceLoopAddr = instr->loopAddr;
    if (!(chFlags & AKAO_CH_ATTACK_SET)) {
        chan->voiceAdsr1 = drum->attack << 8;
    } else {
        chan->voiceAdsr1 &= SPU_ADSR1_ATTACK_RATE;
    }
    chan->voiceAdsr1 |= instr->adsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL);
    if (!(chFlags & AKAO_CH_SUSTAIN_RATE_SET)) {
        chan->voiceAdsr2 &= 0x201F;
        chan->voiceAdsr2 |= drum->sustainRate << 6;
    } else {
        chan->voiceAdsr2 &= 0x3FDF;
    }
    switch (drum->sustainMode) {
    case AKAO_ENV_LINEAR_DEC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        chan->voiceAdsr2 |= SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    if (!(chFlags & AKAO_CH_RELEASE_RATE_SET)) {
        chan->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_RATE;
        chan->voiceAdsr2 |= drum->releaseRate;
    }
    chan->voiceAdsr2 |= instr->adsr2 & SPU_ADSR2_RELEASE_EXP;
    pitch = ((s32 (*)())akao_calcPitch)(instr, drum->note, chan->fineTune, &chan->fineTuneOfs);
    chan->voiceVolScale = drum->volume;
    chan->pan = ((drum->pan & 0x7F) + 0x40) << 8;
    if (drum->pan & AKAO_DRUM_REVERB) {
        g_akaoSeq->reverbMask |= chBit;
    } else {
        g_akaoSeq->reverbMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    return pitch;
}

void akao_execChannel(AkaoChannel *chan, u32 chBit) {
    u32 op;
    u32 vel;
    u32 tmp;
    u32 chFlags;
    u16 len;
    u32 vibDepth;

    do {
        op = *chan->pc++;
        if (op >= AKAO_OP_FIRST_CMD) {
            if (op == AKAO_OP_EXT) {
                vel = *chan->pc++;
                /* Not bounded, as in the original: g_akaoOpsFE has 0x20 handlers, but any byte the sequence holds
                 * indexes it. A channel reading garbage (e.g. a replaced overlay, ff9-ds d1_f0310: 0xFE 0x4F) jumps
                 * through whatever word follows the table; on the PS1 that is data (an address error there).
                 * A port may send indices >= 0x20 to akao_opInvalid. */
                g_akaoOpsFE[vel](chan, chBit);
            } else if (op - AKAO_OP_NOTE_LEN < 14) {
                op = (op - AKAO_OP_NOTE_LEN) * AKAO_NOTE_LENGTHS;
                chan->noteTicks = *chan->pc++;
            } else {
                if (op == 0xFF) {
                    op = AKAO_OP_END;
                } else if (op == AKAO_OP_LOOP_FOREVER && (chan->flags & AKAO_CH_STOP_REQUESTED)) {
                    op = AKAO_OP_END;
                    g_akaoSeState.keyOffMask |= chBit;
                }
                g_akaoOps[op - AKAO_OP_FIRST_CMD](chan, chBit);
            }
        }
        chan->opCount++;
    } while (op > AKAO_OP_END);
    if (op == AKAO_OP_END) {
        if (chan->isSe == 0) {
            g_akaoSeq->keyOffMask |= chBit;
        }
        return;
    }
    vel = akao_peekNextNote(chan) & 0xFF;
    if (chan->fixedLen != 0) {
        chan->noteTicks = chan->gateTicks = chan->fixedLen;
    }
    if (chan->noteTicks != 0) {
        if (vel >= AKAO_NOTE_REST || (vel < AKAO_NOTE_TIE && !(chan->legato & (AKAO_LEGATO | AKAO_LEGATO_SE)))) {
            chan->gateTicks -= 2;
        }
    } else {
        chan->noteTicks = g_akaoNoteLengths[op % AKAO_NOTE_LENGTHS];
        len = chan->noteTicks;
        if ((vel < AKAO_NOTE_TIE || vel >= AKAO_NOTE_REST) && !(chan->legato & (AKAO_LEGATO | AKAO_LEGATO_SE))) {
            len -= 2;
        }
        chan->gateTicks = len;
    }
    if (chan->isSe == 0 && (chan->flags & AKAO_CH_FULL_GATE)) {
        chan->gateTicks = chan->noteTicks;
    }
    chan->noteLen = chan->noteTicks;
    chan->voiceDirty |= AKAO_VD_RELEASE_RATE;
    if (op >= AKAO_NOTE_REST) {
        if (chan->isSe == 0) {
            g_akaoSeq->playingMask &= ~chBit;
            if (chan->voice < 0x18) {
                g_akaoSeq->keyOffMask |= chBit;
            }
        }
        chan->portamentoLen = 0;
        chan->vibratoOut = 0;
        chan->tremoloOut = 0;
        chan->legato &= ~AKAO_LEGATO_TIED;
        return;
    }
    if (op < AKAO_NOTE_TIE) {
        op /= AKAO_NOTE_LENGTHS;
        op += chan->octave * 12;
        chFlags = chan->flags;
        if (chFlags & AKAO_CH_DRUM_KIT) {
            vel = akao_playDrumNote(chan, chBit, op);
        } else {
            if (!(chan->legato & AKAO_LEGATO_TIED)) {
                if (chan->isSe == 0) {
                    if (chFlags & AKAO_CH_KEY_SPLIT) {
                        akao_applyKeySplit(chan, op);
                    }
                    g_akaoSeq->keyOnMask |= chBit;
                    if ((g_akaoSeq->playingMask & chBit) && chan->voice < 0x18) {
                        g_akaoSeq->keyOffMask |= chBit;
                    }
                    if (chan->noteVolumeSlideLen != 0) {
                        chan->volumeSlideLen = chan->noteVolumeSlideLen;
                        chan->volume = chan->noteVolume;
                        chan->volumeStep = chan->noteVolumeStep;
                    }
                } else {
                    g_akaoSeState.keyOnMask |= chBit;
                }
                chan->pitchSlideLeft = 0;
            }
            if (chan->portamentoLen != 0 && chan->prevKey != 0) {
                chan->slideLen = chan->portamentoLen;
                chan->slideKeys = chan->transpose + op - chan->prevKey - chan->prevTranspose;
                chan->curKey = chan->prevKey - (chan->transpose - chan->prevTranspose);
                op = chan->prevKey + chan->prevTranspose;
            } else {
                chan->curKey = op;
                op += chan->transpose;
            }
            vel = ((s32 (*)())akao_calcPitch)(&g_akaoInstruments[chan->instrument], op, chan->fineTune, &chan->fineTuneOfs);
            if (chan->pitchRandom != 0) {
                tmp = (u32)(vel * chan->pitchRandom) >> 8;
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
        chan->pitch = vel;
        if (chan->isSe == 0) {
            g_akaoSeq->playingMask |= chBit;
        } else {
            g_akaoSeState.playingMask |= chBit;
        }
        op = chan->flags;
        chan->voiceDirty |= AKAO_VD_VOL | AKAO_VD_PITCH;
        if (op & AKAO_CH_VIBRATO) {
            vibDepth = (chan->vibratoDepthParam & 0x7F00) >> 8;
            if (!(chan->vibratoDepthParam & AKAO_VIB_FULL_SCALE)) {
                vibDepth = (vibDepth * ((u32)(vel * 15) >> 8)) >> 7;
            } else {
                vibDepth = (vibDepth * vel) >> 7;
            }
            chan->vibratoDepth = vibDepth;
            if (!(chan->legato & AKAO_LEGATO_TIED)) {
                chan->vibratoPhase = 0;
                chan->vibratoDelayLeft = chan->vibratoDelay;
                chan->vibratoRate = 0x1000000 / ((u32)chan->vibratoPeriod >> 12);
            }
        }
        if ((op & AKAO_CH_TREMOLO) && !(chan->legato & AKAO_LEGATO_TIED)) {
            chan->tremoloPhase = 0;
            chan->tremoloDelayLeft = chan->tremoloDelay;
            chan->tremoloRate = 0x1000000 / ((u32)chan->tremoloPeriod >> 12);
        }
        chan->vibratoOut = 0;
        chan->tremoloOut = 0;
        chan->pitchSlide = 0;
    }
    chan->legato = (chan->legato & ~AKAO_LEGATO_TIED) | ((chan->legato & AKAO_LEGATO) << 1);
    if (chan->slideKeys != 0) {
        chan->curKey += chan->slideKeys;
        vel = ((s32 (*)())akao_calcPitch)(&g_akaoInstruments[chan->instrument], chan->curKey + chan->transpose, chan->fineTune, &tmp) << 16;
        chan->pitchSlideLeft = chan->slideLen;
        chan->slideKeys = 0;
        chan->pitchSlideStep = (s32)(vel - ((chan->pitch << 16) + chan->pitchSlide)) / (u16)chan->pitchSlideLeft;
    }
    chan->prevKey = chan->curKey;
    chan->prevTranspose = chan->transpose;
}

s32 akao_initSlide(s32 *slideVal, s32 target, s32 steps, s32 shift) {
    s32 fracMask = (1 << shift) - 1;
    s32 cur = *slideVal & ~fracMask;
    s32 step;
    target <<= shift;
    step = target - cur;
    step = step / steps;
    *slideVal = cur;
    if (target < cur) {
        *slideVal = cur | fracMask;
        step--;
    } else {
        step++;
    }
    return step;
}

void akao_setChannelInstrument(AkaoChannel *ch, AkaoInstrument *instr, s32 startAddr) {
    ch->voiceStartAddr = startAddr;
    ch->voiceLoopAddr = instr->loopAddr;
    ch->voiceAdsr1 = instr->adsr1;
    ch->voiceAdsr2 = instr->adsr2;
    ch->voiceDirty |= AKAO_VD_START_ADDR | AKAO_VD_ADSR1 | AKAO_VD_ADSR2 | AKAO_VD_LOOP_ADDR;
}

void akao_setChannelProgram(AkaoChannel *chan, s32 i) {
    chan->instrument = i;
    akao_setChannelInstrument(chan, &g_akaoInstruments[i], g_akaoInstruments[i].startAddr);
}

void akao_stopSeChannel(AkaoChannel *chan, u32 chBit) {
    g_akaoSeState.activeMask &= ~chBit;
    g_akaoSeState.pausedMask &= ~chBit;
    g_akaoSeState.noiseMask &= ~chBit;
    g_akaoSeState.reverbMask &= ~chBit;
    g_akaoSeState.pitchModMask &= ~chBit;
    g_akaoSeState.keyOnMask &= ~chBit;
    g_akaoSeState.playingMask &= ~chBit;
    chan->seMask = 0;
    chan->seHandle = 0;
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

void akao_opEnd(AkaoChannel *chan, s32 chBit) {
    AkaoSeq *seq;

    if (chan->isSe == 0) {
        seq = g_akaoSeq;
        if ((seq->activeMask &= ~chBit) == 0) {
            g_akaoSkipMeasures = 0;
            seq->musicId = 0;
            seq->flags = 0;
        }
        g_akaoSeq->playingMask &= ~chBit;
        g_akaoSeq->priorityMask &= ~chBit;
        g_akaoSeq->keepVoiceMask &= ~chBit;
        g_akaoSeq->noiseMask &= ~chBit;
        g_akaoSeq->reverbMask &= ~chBit;
        g_akaoSeq->pitchModMask &= ~chBit;
    } else {
        akao_stopSeChannel(chan, chBit);
    }
    chan->flags = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_opFETempo(AkaoChannel *chan) {
    AkaoSeq *seq = g_akaoSeq;
    s32 tempoLo = chan->pc[0] << 16;
    seq->tempo.w = tempoLo;
    seq->tempo.w = tempoLo | (chan->pc[1] << 24);
    chan->pc += 2;
    seq->tempoSlideLen = 0;
}

void akao_opFETempoSlide(AkaoChannel *chan) {
    s32 x;
    s32 curTempo;

    if ((g_akaoSeq->tempoSlideLen = *chan->pc++) == 0) {
        g_akaoSeq->tempoSlideLen = 0x100;
    }
    x = (chan->pc[0] << 16) | (chan->pc[1] << 24);
    chan->pc += 2;
    curTempo = g_akaoSeq->tempo.w & 0xFFFF0000;
    g_akaoSeq->tempoStep = (x - curTempo) / g_akaoSeq->tempoSlideLen;
    g_akaoSeq->tempo.w = curTempo;
}

void akao_opFE02(AkaoChannel *chan) {
    AkaoSeq *seq = g_akaoSeq;
    u8 *code = chan->pc;
    s32 hi = (s8)code[1];
    s32 lo = code[0];
    s32 depth;
    chan->pc = code + 2;
    depth = hi << 20;
    depth |= lo << 12;
    seq->reverbDepthSlideLen = 0;
    g_akaoFlags.dirty |= AKAO_DIRTY_REVERB_DEPTH;
    seq->reverbDepth = depth;
}

void akao_opFE03(AkaoChannel *chan) {
    u8 *code;
    s32 depth;

    g_akaoSeq->reverbDepthSlideLen = *chan->pc++;
    if (g_akaoSeq->reverbDepthSlideLen == 0) {
        g_akaoSeq->reverbDepthSlideLen = 0x100;
    }
    code = chan->pc;
    depth = ((s8)code[1] << 20) | (code[0] << 12);
    chan->pc = code + 2;
    g_akaoSeq->reverbDepth &= ~0xFFF;
    g_akaoSeq->reverbDepthStep = (depth - g_akaoSeq->reverbDepth) / g_akaoSeq->reverbDepthSlideLen;
}

void akao_opFEJump(AkaoChannel *chan) {
    u8 *code = chan->pc;
    chan->pc = code + (s16)(code[0] | (code[1] << 8));
}

void akao_opFEJumpIfGe(AkaoChannel *chan) {
    u8 *code = chan->pc;
    s32 threshold = *code++;
    chan->pc = code;
    if (g_akaoSeq->condition >= threshold) {
        chan->pc = code + (s16)(code[0] | (code[1] << 8));
    } else {
        chan->pc = code + 2;
    }
}

void akao_opFECall(AkaoChannel *chan) {
    u8 *code = chan->pc;
    s16 off = code[0] | (code[1] << 8);
    chan->returnAddr = code + 2;
    chan->pc += off;
}

void akao_opFEReturn(AkaoChannel *chan) {
    chan->pc = chan->returnAddr;
}

void akao_opA3(AkaoChannel *chan) {
    chan->expression = *chan->pc++ << 8;
    chan->voiceDirty |= AKAO_VD_VOL;
}

void akao_opFE12(AkaoChannel *ch) {
    s32 slideLen;
    s32 startVal;
    u8 *code;
    u16 base;

    slideLen = *ch->pc++;
    ch->expressionSlideLen = slideLen;
    if (slideLen == 0) {
        ch->expressionSlideLen = 0x100;
    }
    code = ch->pc;
    base = ch->expression & 0x7F00;
    ch->expressionStep = ((*code << 8) - base) / ch->expressionSlideLen;
    ch->pc = code + 1;
    startVal = base;
    ch->expression = startVal;
}

void akao_opA8(AkaoChannel *chan) {
    s32 vol = (s8)*chan->pc++;
    chan->volumeSlideLen = 0;
    chan->voiceDirty |= AKAO_VD_VOL;
    chan->noteVolumeSlideLen = 0;
    chan->volume = vol << 23;
}

void akao_opA9(AkaoChannel *chan) {
    s32 curVol;

    if ((chan->volumeSlideLen = *chan->pc++) == 0) {
        chan->volumeSlideLen = 0x100;
    }
    curVol = chan->volume & 0xFFFF0000;
    chan->volumeStep = (((s8)*chan->pc++ << 23) - curVol) / (u16)chan->volumeSlideLen;
    chan->volume = curVol;
    chan->noteVolumeSlideLen = 0;
}

void akao_opFE19(AkaoChannel *chan) {
    chan->noteVolume = (s8)*chan->pc++ << 23;
    if ((chan->noteVolumeSlideLen = *chan->pc++) == 0) {
        chan->noteVolumeSlideLen = 0x100;
    }
    chan->noteVolumeStep = (((s8)*chan->pc++ << 23) - chan->noteVolume) / (u16)chan->noteVolumeSlideLen;
}

void akao_opFE1A(AkaoChannel *chan) {
    chan->flags |= AKAO_CH_FULL_GATE;
}

void akao_opFE1B(AkaoChannel *chan) {
    chan->flags &= ~AKAO_CH_FULL_GATE;
}

void akao_opAA(AkaoChannel *ch) {
    u8 *code = ch->pc;
    s32 panArg = *code;
    ch->pc = code + 1;
    ch->panSlideLen = 0;
    ch->voiceDirty |= AKAO_VD_VOL;
    ch->pan = ((panArg + 0x40) & 0xFF) << 8;
}

void akao_opAB(AkaoChannel *ch) {
    s32 slideLen;
    s32 startVal;
    u8 *code;
    u16 base;

    slideLen = *ch->pc++;
    ch->panSlideLen = slideLen;
    if (slideLen == 0) {
        ch->panSlideLen = 0x100;
    }
    code = ch->pc;
    base = ch->pan & 0xFF00;
    ch->panStep = ((((*code + 0x40) & 0xFF) << 8) - base) / ch->panSlideLen;
    ch->pc = code + 1;
    startVal = base;
    ch->pan = startVal;
}

void akao_opOctave(AkaoChannel *chan) {
    chan->octave = *chan->pc++;
}

void akao_opOctaveUp(AkaoChannel *chan) {
    chan->octave = (chan->octave + 1) & 0xF;
}

void akao_opOctaveDown(AkaoChannel *chan) {
    chan->octave = (chan->octave - 1) & 0xF;
}

void akao_opProgram(AkaoChannel *chan) {
    s32 i;
    AkaoInstrument *instr;
    s32 program = *chan->pc++;

    if (chan->isSe == 0) {
        i = akao_remapInstrument(g_akaoSeq->flags, program);
    } else {
        i = akao_remapSeInstrument(chan->seBank, program);
    }
    instr = &g_akaoInstruments[i];
    akao_setChannelInstrument(chan, instr, instr->startAddr);
    chan->instrument = i;
    chan->voiceVolScale = 0;
    chan->flags &= ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT);
}

void akao_opFE0A(AkaoChannel *chan) {
    s32 i = akao_remapInstrument(g_akaoSeq->flags, *chan->pc++);
    akao_setChannelInstrument(chan, &g_akaoInstruments[i], 0x1010);
    chan->instrument = i;
    chan->voiceVolScale = 0;
    chan->flags &= ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT);
}

void akao_opFE14(AkaoChannel *chan) {
    u8 regionId = *chan->pc++;
    u16 *tbl = g_akaoSeq->regionTable;
    if (tbl != NULL) {
        if (tbl[regionId] > 0x8000) {
            chan->voiceVolScale = 0;
            chan->flags &= ~AKAO_CH_KEY_SPLIT;
        } else {
            u8 *entry = (u8 *)tbl + tbl[regionId];
            chan->curKey = 0xFF;
            chan->regions = entry + 0x20;
            chan->flags = (chan->flags & ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT)) | AKAO_CH_KEY_SPLIT;
        }
    }
}

void akao_opB3(AkaoChannel *chan) {
    AkaoInstrument *instr = &g_akaoInstruments[chan->instrument];
    chan->voiceAdsr1 = instr->adsr1;
    chan->voiceAdsr2 = instr->adsr2;
    chan->voiceDirty |= AKAO_VD_ADSR1 | AKAO_VD_ADSR2;
    chan->flags &= ~AKAO_CH_ADSR_SET;
}

void akao_opC0(AkaoChannel *chan) {
    chan->transpose = (s8)*chan->pc++;
}

void akao_opC1(AkaoChannel *chan) {
    chan->transpose += (s8)*chan->pc++;
}

void akao_opA4(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    chan->slideLen = len;
    if (len == 0) {
        chan->slideLen = 0x100;
    }
    chan->slideKeys = (s8)*chan->pc++;
}

void akao_opDA(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    chan->portamentoLen = len;
    if (len == 0) {
        chan->portamentoLen = 0x100;
    }
    chan->prevTranspose = 0;
    chan->prevKey = 0;
    chan->legato = AKAO_LEGATO;
}

void akao_opDB(AkaoChannel *chan) {
    chan->portamentoLen = 0;
}

void akao_opD8(AkaoChannel *chan) {
    u8 *code = chan->pc;
    u32 k;
    u32 prod;
    u32 fineOfs;
    chan->fineTune = (s8)*code;
    k = chan->pitch;
    fineOfs = (u8)chan->fineTune;
    prod = k * fineOfs;
    chan->pc = code + 1;
    if (chan->fineTune < 0) {
        fineOfs = (prod >> 8) - k;
    } else {
        fineOfs = prod >> 7;
    }
    chan->fineTuneOfs = fineOfs;
    chan->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opD9(AkaoChannel *chan) {
    u32 fineOfs;

    chan->fineTune += (s8)*chan->pc++;
    fineOfs = chan->pitch * (u8)chan->fineTune;
    if (chan->fineTune < 0) {
        fineOfs = (fineOfs >> 8) - chan->pitch;
    } else {
        fineOfs >>= 7;
    }
    chan->fineTuneOfs = fineOfs;
    chan->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opB4(AkaoChannel *chan) {
    u32 val; /* SE vibrato depth, then the LFO waveform index (separate locals don't match) */
    s16 depthParam;
    u32 amount;
    u32 vibDepth;
    u32 k;
    u32 period;

    chan->flags |= AKAO_CH_VIBRATO;
    if (chan->isSe != 0) {
        chan->vibratoDelay = 0;
        val = *chan->pc++;
        if (val != 0) {
            chan->vibratoDepthParam = val << 8;
        }
    } else {
        chan->vibratoDelay = *chan->pc++;
    }
    chan->vibratoPeriod = period = *chan->pc++ << 12;
    if (period == 0) {
        chan->vibratoPeriod = 0x100000;
    }
    val = *chan->pc++;
    depthParam = chan->vibratoDepthParam;
    k = (u16)chan->pitch;
    amount = (depthParam & 0x7F00) >> 8;
    if (!(depthParam & AKAO_VIB_FULL_SCALE)) {
        vibDepth = (amount * ((s32)(k * 15) >> 8)) >> 7;
    } else {
        vibDepth = (amount * k) >> 7;
    }
    chan->vibratoDepth = vibDepth;
    chan->vibratoPeriodSlideLen = 0;
    chan->vibratoDelayLeft = chan->vibratoDelay;
    chan->vibratoWave = g_akaoLfoWaves[val & 7];
    chan->vibratoRate = 0x1000000 / ((u32)chan->vibratoPeriod >> 12);
}

void akao_opB5(AkaoChannel *chan) {
    s32 k;
    s16 depthParam;
    u32 amount;
    u32 vibDepth;

    chan->vibratoDepthParam = *chan->pc++ << 8;
    k = chan->pitch;
    depthParam = chan->vibratoDepthParam;
    amount = (depthParam & 0x7F00) >> 8;
    if (!(depthParam & AKAO_VIB_FULL_SCALE)) {
        vibDepth = amount * ((s32)(k * 15) >> 8);
    } else {
        vibDepth = amount * k;
    }
    chan->vibratoDepth = vibDepth >> 7;
}

void akao_opDD(AkaoChannel *chan) {
    s32 len = *chan->pc++;

    if (len == 0) {
        len = 0x100;
    }
    chan->vibratoDepthStep = ((*chan->pc++ << 8) - chan->vibratoDepthParam) / len;
    chan->vibratoDepthSlideLen = len;
}

void akao_opE4(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    if (len == 0) {
        len = 0x100;
    }
    chan->vibratoPeriodSlideLen = len;
    chan->vibratoPeriodStep = akao_initSlide(&chan->vibratoPeriod, *chan->pc++, len, 0xC);
}

void akao_opB6(AkaoChannel *chan) {
    chan->vibratoOut = 0;
    chan->flags &= ~AKAO_CH_VIBRATO;
    chan->voiceDirty |= AKAO_VD_PITCH;
}

void akao_opB8(AkaoChannel *chan) {
    s32 val; /* tremolo delay (music) or depth (SE), then the LFO waveform index (separate locals don't match) */
    u32 rate;

    chan->flags |= AKAO_CH_TREMOLO;
    val = *chan->pc++;
    if (chan->isSe != 0) {
        chan->tremoloDelay = 0;
        if (val != 0) {
            chan->tremoloDepth = (val & 0x7F) << 8;
        }
    } else {
        chan->tremoloDelay = val;
    }
    chan->tremoloPeriod = *chan->pc++ << 12;
    if (chan->tremoloPeriod == 0) {
        chan->tremoloPeriod = 0x100000;
    }
    val = *chan->pc++;
    rate = 0x1000000 / ((u32)chan->tremoloPeriod >> 12);
    chan->tremoloDelayLeft = chan->tremoloDelay;
    chan->tremoloPeriodSlideLen = 0;
    chan->tremoloWave = g_akaoLfoWaves[val & 7];
    chan->tremoloRate = rate;
}

void akao_opB9(AkaoChannel *chan) {
    chan->tremoloDepth = (*chan->pc++ & 0x7F) << 8;
}

void akao_opDE(AkaoChannel *chan) {
    s32 len = *chan->pc++;

    if (len == 0) {
        len = 0x100;
    }
    chan->tremoloDepthStep = (((*chan->pc++ & 0x7F) << 8) - chan->tremoloDepth) / len;
    chan->tremoloDepthSlideLen = len;
}

void akao_opE5(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    if (len == 0) {
        len = 0x100;
    }
    chan->tremoloPeriodSlideLen = len;
    chan->tremoloPeriodStep = akao_initSlide(&chan->tremoloPeriod, *chan->pc++, len, 0xC);
}

void akao_opBA(AkaoChannel *chan) {
    chan->tremoloOut = 0;
    chan->flags &= ~AKAO_CH_TREMOLO;
    chan->voiceDirty |= AKAO_VD_VOL;
}

void akao_opBC(AkaoChannel *chan) {
    chan->flags |= AKAO_CH_PAN_LFO;
    chan->panLfoPeriod = *chan->pc++ << 12;
    if (chan->panLfoPeriod == 0) {
        chan->panLfoPeriod = 0x100000;
    }
    {
        u32 rate = 0x1000000 / ((u32)chan->panLfoPeriod >> 12);
        u8 wave = *chan->pc++;
        chan->panLfoPeriodSlideLen = 0;
        chan->panLfoPhase = 0;
        chan->panLfoRate = rate;
        chan->panLfoWave = g_akaoLfoWaves[wave & 7];
    }
}

void akao_opBD(AkaoChannel *chan) {
    chan->panLfoDepth = *chan->pc++ << 7;
}

void akao_opDF(AkaoChannel *chan) {
    s32 len = *chan->pc++;

    if (len == 0) {
        len = 0x100;
    }
    chan->panLfoDepthStep = ((*chan->pc++ << 7) - chan->panLfoDepth) / len;
    chan->panLfoDepthSlideLen = len;
}

void akao_opE6(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    if (len == 0) {
        len = 0x100;
    }
    chan->panLfoPeriodSlideLen = len;
    chan->panLfoPeriodStep = akao_initSlide(&chan->panLfoPeriod, *chan->pc++, len, 0xC);
}

void akao_opBE(AkaoChannel *chan) {
    chan->panLfoOut = 0;
    chan->flags &= ~AKAO_CH_PAN_LFO;
    chan->voiceDirty |= AKAO_VD_VOL;
}

void akao_opC4(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->noiseMask |= chBit;
    } else {
        g_akaoSeState.noiseMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
}

void akao_opC5(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->noiseMask &= ~chBit;
    } else {
        g_akaoSeState.noiseMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
    chan->noiseTimer = 0;
}

void akao_opC6(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->pitchModMask |= chBit;
    } else if (chan->flags & AKAO_CH_SE_SECOND) {
        g_akaoSeState.pitchModMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opC7(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->pitchModMask &= ~chBit;
    } else {
        g_akaoSeState.pitchModMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    chan->pitchModTimer = 0;
}

void akao_opC2(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->reverbMask |= chBit;
    } else {
        g_akaoSeState.reverbMask |= chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opC3(AkaoChannel *chan, u32 chBit) {
    if (chan->isSe == 0) {
        g_akaoSeq->reverbMask &= ~chBit;
    } else {
        g_akaoSeState.reverbMask &= ~chBit;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_opCC(AkaoChannel *chan) {
    chan->legato = AKAO_LEGATO;
}

void akao_opCD(void) {
}

void akao_opD0(AkaoChannel *chan) {
    if (chan->isSe != 0) {
        chan->legato = AKAO_LEGATO_SE;
    }
}

void akao_opD1(void) {
}

void akao_opAC(AkaoChannel *chan) {
    u32 clock = *chan->pc++;

    if (chan->isSe == 0) {
        if (clock & 0xC0) {
            g_akaoSeq->noiseClock = (g_akaoSeq->noiseClock + (clock & 0x3F)) & 0x3F;
        } else {
            g_akaoSeq->noiseClock = clock;
        }
    } else if (clock & 0xC0) {
        g_akaoSeState.noiseClock = (g_akaoSeState.noiseClock + (clock & 0x3F)) & 0x3F;
    } else {
        g_akaoSeState.noiseClock = clock;
    }
    g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK;
}

void akao_opAD(AkaoChannel *chan) {
    s32 rate = *chan->pc++;
    chan->voiceDirty |= AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE;
    chan->flags |= AKAO_CH_ATTACK_SET;
    chan->voiceAdsr1 = (chan->voiceAdsr1 & (SPU_ADSR1_ATTACK_EXP | SPU_ADSR1_DECAY_RATE | SPU_ADSR1_SUSTAIN_LEVEL)) | (rate << 8);
}

void akao_opAE(AkaoChannel *chan, s32 unused) {
    u16 adsr1 = chan->voiceAdsr1 & ~SPU_ADSR1_DECAY_RATE;
    chan->voiceAdsr1 = adsr1 | (*chan->pc++ << 4);
    chan->voiceDirty |= AKAO_VD_DECAY_RATE;
}

void akao_opAF(AkaoChannel *chan, s32 unused) {
    u16 adsr1 = chan->voiceAdsr1 & ~SPU_ADSR1_SUSTAIN_LEVEL;
    chan->voiceAdsr1 = adsr1 | *chan->pc++;
    chan->voiceDirty |= AKAO_VD_SUSTAIN_LEVEL;
}

void akao_opB1(AkaoChannel *chan) {
    s32 rate = *chan->pc++;
    chan->voiceDirty |= AKAO_VD_SUSTAIN_MODE | AKAO_VD_SUSTAIN_RATE;
    chan->flags |= AKAO_CH_SUSTAIN_RATE_SET;
    chan->voiceAdsr2 = (chan->voiceAdsr2 & ~SPU_ADSR2_SUSTAIN_RATE) | (rate << 6);
}

void akao_opB2(AkaoChannel *chan) {
    s32 rate = *chan->pc++;
    chan->voiceDirty |= AKAO_VD_RELEASE_MODE | AKAO_VD_RELEASE_RATE;
    chan->flags |= AKAO_CH_RELEASE_RATE_SET;
    chan->voiceAdsr2 = (chan->voiceAdsr2 & ~SPU_ADSR2_RELEASE_RATE) | rate;
}

void akao_opB7(AkaoChannel *chan) {
    s32 mode = *chan->pc++;
    chan->voiceAdsr1 &= ~SPU_ADSR1_ATTACK_EXP;
    if (mode == AKAO_ENV_EXP_INC) {
        chan->voiceAdsr1 |= SPU_ADSR1_ATTACK_EXP;
    }
    chan->voiceDirty |= AKAO_VD_ATTACK_MODE;
}

void akao_opBB(AkaoChannel *chan) {
    u16 mode = *chan->pc++;
    u16 x = chan->voiceAdsr2 & ~(SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC);
    chan->voiceAdsr2 = x;
    switch (mode) {
    case AKAO_ENV_LINEAR_DEC:
        chan->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_DEC;
        break;
    case AKAO_ENV_EXP_INC:
        chan->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_EXP;
        break;
    case AKAO_ENV_EXP_DEC:
        chan->voiceAdsr2 = x | SPU_ADSR2_SUSTAIN_EXP | SPU_ADSR2_SUSTAIN_DEC;
        break;
    }
    chan->voiceDirty |= AKAO_VD_SUSTAIN_MODE;
}

void akao_opBF(AkaoChannel *chan) {
    s32 mode = *chan->pc++;
    chan->voiceAdsr2 &= ~SPU_ADSR2_RELEASE_EXP;
    if (mode == AKAO_ENV_EXP_DEC) {
        chan->voiceAdsr2 |= SPU_ADSR2_RELEASE_EXP;
    }
    chan->voiceDirty |= AKAO_VD_RELEASE_MODE;
}

void akao_opFE10(AkaoChannel *chan) {
    g_akaoSeq->firstVoice = *chan->pc++;
}

void akao_opFE11(void) {
    g_akaoSeq->firstVoice = 0;
}

void akao_opC8(AkaoChannel *chan) {
    chan->loopDepth = (chan->loopDepth + 1) & 3;
    chan->loopStart[chan->loopDepth] = chan->pc;
    chan->loopCount[chan->loopDepth] = 0;
    chan->loopOpCount[chan->loopDepth] = chan->opCount;
}

void akao_opC9(AkaoChannel *chan) {
    u32 count = *chan->pc++;

    if (count == 0) {
        count = 0x100;
    }
    if (++chan->loopCount[chan->loopDepth] != count) {
        chan->pc = chan->loopStart[chan->loopDepth];
        chan->opCount = chan->loopOpCount[chan->loopDepth];
        return;
    }
    chan->loopDepth = (chan->loopDepth - 1) & 3;
}

void akao_opFE08(AkaoChannel *chan) {
    u8 *code = chan->pc;
    s32 pass = *code++;
    chan->pc = code;
    if (pass == 0) {
        pass = 0x100;
    }
    if (chan->loopCount[chan->loopDepth] + 1 != pass) {
        chan->pc = code + 2;
    } else {
        chan->pc = code + (s16)(code[0] | (code[1] << 8));
    }
}

void akao_opFE09(AkaoChannel *chan) {
    u8 *code = chan->pc;
    s32 pass = *code++;
    chan->pc = code;
    if (pass == 0) {
        pass = 0x100;
    }
    if (chan->loopCount[chan->loopDepth] + 1 != pass) {
        chan->pc = code + 2;
    } else {
        chan->pc = code + (s16)(code[0] | (code[1] << 8));
        chan->loopDepth = (chan->loopDepth - 1) & 3;
    }
}

void akao_opCA(AkaoChannel *chan) {
    chan->loopCount[chan->loopDepth]++;
    chan->pc = chan->loopStart[chan->loopDepth];
    chan->opCount = chan->loopOpCount[chan->loopDepth];
}

void akao_opA2(AkaoChannel *chan) {
    s32 len = *chan->pc++;
    chan->fixedLen = 0;
    chan->gateTicks = len;
    chan->noteTicks = len;
    chan->noteLen = len;
}

void akao_opDC(AkaoChannel *chan) {
    s32 len = (s8)*chan->pc++;
    if (len != 0) {
        len += (s16)chan->noteLen;
        if (len < 1) {
            len = 1;
        } else if (len >= 0x100) {
            len = 0xFF;
        }
    }
    chan->fixedLen = len;
}

void akao_opFE04(AkaoChannel *chan) {
    if (g_akaoSeq->drumKit != 0) {
        chan->flags = (chan->flags & ~(AKAO_CH_ADSR_SET | AKAO_CH_DRUM_KIT | AKAO_CH_KEY_SPLIT)) | AKAO_CH_DRUM_KIT;
    }
}

void akao_opFE05(AkaoChannel *chan) {
    chan->voiceVolScale = 0;
    chan->flags &= ~AKAO_CH_DRUM_KIT;
}

void akao_opFE15(AkaoChannel *chan) {
    AkaoSeq *seq = g_akaoSeq;
    seq->ticksPerBeat = *chan->pc++;
    seq->beatsPerMeasure = *chan->pc++;
    seq->tick = 0;
    seq->beat = 0;
}

void akao_opFE16(AkaoChannel *chan) {
    AkaoSeq *seq = g_akaoSeq;
    seq->measure = *chan->pc++;
    seq->measure |= *chan->pc++ << 8;
}

void akao_opB0(AkaoChannel *chan, s32 unused) {
    akao_opAE(chan, unused);
    akao_opAF(chan, unused);
}

void akao_opCE(AkaoChannel *chan, u32 chBit) {
    s32 timer = *chan->pc++;
    chan->noiseTimer = timer ? timer + 1 : 0x101;
    akao_opC4(chan, chBit);
}

void akao_opCF(AkaoChannel *chan) {
    s32 timer = *chan->pc++;
    if (timer != 0) {
        chan->noiseTimer = timer + 1;
    } else {
        chan->noiseTimer = 0x101;
    }
}

void akao_opD2(AkaoChannel *chan, u32 chBit) {
    s32 timer = *chan->pc++;
    chan->pitchModTimer = timer ? timer + 1 : 0x101;
    akao_opC6(chan, chBit);
}

void akao_opD3(AkaoChannel *chan) {
    s32 timer = *chan->pc++;
    if (timer != 0) {
        chan->pitchModTimer = timer + 1;
    } else {
        chan->pitchModTimer = 0x101;
    }
}

void akao_opCB(AkaoChannel *chan, u32 chBit) {
    chan->flags &= ~(AKAO_CH_VIBRATO | AKAO_CH_TREMOLO | AKAO_CH_PAN_LFO | AKAO_CH_PITCH_FROM_PREV | AKAO_CH_VOL_FROM_PREV);
    akao_opC5(chan, chBit);
    akao_opC7(chan, chBit);
    akao_opC3(chan, chBit);
    chan->legato &= ~(AKAO_LEGATO | AKAO_LEGATO_SE);
}

void akao_opD4(AkaoChannel *chan) {
    chan->flags |= AKAO_CH_PITCH_FROM_PREV;
}

void akao_opD5(AkaoChannel *chan) {
    chan->flags &= ~AKAO_CH_PITCH_FROM_PREV;
}

void akao_opD6(AkaoChannel *chan) {
    chan->flags |= AKAO_CH_VOL_FROM_PREV;
}

void akao_opD7(AkaoChannel *chan) {
    chan->flags &= ~AKAO_CH_VOL_FROM_PREV;
}

static inline u8 *akao_opFE0B_ref(u8 *code) {
    s32 ofs = (code[1] << 8) | code[0];
    return ofs != 0 ? code + ofs + 2 : NULL;
}

void akao_opFE0B(AkaoChannel *chan) {
    u8 *code = chan->pc;
    u8 *track0;
    u8 *track1;

    track0 = akao_opFE0B_ref(code);
    code += 2;
    track1 = akao_opFE0B_ref(code);
    g_akaoOpSeArgs.arg0 = 0;
    g_akaoOpSeArgs.arg1 = 0;
    g_akaoOpSeArgs.arg2 = chan->pan >> 8;
    g_akaoOpSeArgs.arg3 = chan->volume >> 23;
    akao_startSe(&g_akaoOpSeArgs, track0, track1, 0);
    chan->pc += 4;
}

void akao_opE0(AkaoChannel *chan) {
    chan->flags |= AKAO_CH_STOP_AT_LOOP;
}

void akao_opFE1C(AkaoChannel *chan) {
    chan->pc++;
}

void akao_opFE1D(AkaoChannel *chan, s32 chBit) {
    g_akaoSeq->priorityMask |= chBit;
}

void akao_opFE1E(AkaoChannel *chan, s32 chBit) {
    g_akaoSeq->priorityMask &= ~chBit;
}

void akao_opE1(AkaoChannel *chan) {
    chan->pitchRandom = *chan->pc++;
}

void akao_opE2(AkaoChannel *chan) {
    chan->pitchRandom = 0;
}

void akao_opInvalid(AkaoChannel *chan, s32 chBit) {
    akao_opEnd(chan, chBit);
}

void akao_streamStop(void) {
    u32 mask;
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
        mask = g_akaoStream.voiceMask;
        g_akaoStream.voiceMask = 0;
        g_akaoSeState.reverbMask &= ~mask;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
}

s32 akao_allocStreamVoices(void) {
    u32 busy;
    s32 i;
    u32 bit;

    do {
        bit = AKAO_SE_CH_BIT(14) | AKAO_SE_CH_BIT(15);
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
    u8 *src = g_akaoStream.readPtr + 0x800;
    SpuSetTransferStartAddr(0x2100);
    SpuSetTransferCallback(akao_streamStartMono);
    SpuWrite(src, 0x800);
}

void akao_streamStart(s32 addr, s32 vol, s32 flag) {
    StreamHeader *h;
    s32 voiceIdx;
    s32 voiceBits;
    s32 size;
    s32 loopLen;

    voiceIdx = akao_allocStreamVoices();
    if (voiceIdx == -1) {
        return;
    }
    g_akaoStream.pan = vol;
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)addr;
    g_akaoStream.readPtr = (u8 *)h + 0x40;
    g_akaoStream.bytesLeft = h->dataSize;
    voiceBits = (1 << voiceIdx) | (1 << (voiceIdx + 1));
    loopLen = h->loopLength;
    g_akaoStream.voice = voiceIdx;
    g_akaoStream.voiceMask = voiceBits;
    g_akaoStream.loopLength = loopLen;
    akao_keyOff(voiceBits);
    g_akaoStream.flags = h->flags;
    g_akaoStream.pitch = h->pitch;
    g_akaoStream.voice = voiceIdx;
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

void akao_streamSetVoice(s32 voiceIdx, s32 mode, s32 startAddr, s32 loopAddr) {
    s16 volL;
    s16 volR;
    s16 h;
    s32 pan;

    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        volL = volR = (g_akaoSeVolume * g_akaoMonoGain) >> 16;
    } else if (mode == 1) {
        volR = 0;
        volL = (u32)g_akaoSeVolume >> 1;
    } else if (mode == 2) {
        volL = 0;
        volR = (u32)g_akaoSeVolume >> 1;
    } else if (mode == 3) {
        h = g_akaoSeVolume >> 1;
        volL = volR = (h >> 1) + (h >> 2);
    } else {
        pan = (u8)(g_akaoStream.pan >> 8);
        volL = (u32)(g_akaoStream.volume * g_akaoPanCurve[pan]) >> 16;
        volR = (u32)(g_akaoStream.volume * g_akaoPanCurve[pan ^ 0xFF]) >> 16;
    }
    akao_setVoiceVolume(voiceIdx, volL, volR, 0);
    akao_setVoicePitch(voiceIdx, g_akaoStream.pitch);
    akao_setVoiceStartAddr(voiceIdx, startAddr);
    akao_setVoiceLoopAddr(voiceIdx, loopAddr);
    akao_setVoiceAttack(voiceIdx, 0, AKAO_ENV_LINEAR_INC);
    akao_setVoiceDecay(voiceIdx, 0xF);
    akao_setVoiceSustainLevel(voiceIdx, 0xF);
    akao_setVoiceSustain(voiceIdx, 0x7F, AKAO_ENV_LINEAR_DEC);
    akao_setVoiceRelease(voiceIdx, 6, AKAO_ENV_LINEAR_DEC);
}

void akao_streamKeyOn(s32 step, s32 irqBase, void (*cb)(void)) {
    if (g_akaoStream.voiceMask != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.bytesLeft > 0x1000) {
            g_akaoStream.bytesLeft -= 0x1000;
            g_akaoStream.readPtr += step;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            irqBase = 0x1030;
            g_akaoStream.bytesLeft = 0;
        }
        SpuSetIRQAddr(irqBase + 8);
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

void akao_streamRefill(s32 addrL, s32 addrR, s32 size, void (*cb)(void)) {
    if (g_akaoStream.voiceMask == 0 || g_akaoStream.bytesLeft == 0) {
        return;
    }
    SpuSetTransferStartAddr(addrL);
    akao_beginSpuTransfer();
    SpuWrite(g_akaoStream.readPtr, size);
    SpuSetIRQ(0);
    if (g_akaoStream.bytesLeft > 0x800) {
        SpuSetIRQCallback(cb);
        g_akaoStream.bytesLeft -= 0x800;
        g_akaoStream.readPtr += size;
    } else if (g_akaoStream.loopPtr != NULL) {
        SpuSetIRQCallback(cb);
        g_akaoStream.readPtr = g_akaoStream.loopPtr;
        g_akaoStream.bytesLeft = g_akaoStream.loopBytes;
    } else {
        SpuSetIRQCallback(akao_streamStop);
        addrR = 0x1030;
        addrL = addrR;
        g_akaoStream.bytesLeft = 0;
    }
    akao_setVoiceLoopAddr(g_akaoStream.voice, addrL);
    akao_setVoiceLoopAddr(g_akaoStream.voice + 1, addrR);
    SpuSetIRQAddr(addrL + 8);
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
    AkaoStreamState *streamState = &g_akaoStream;
    s32 vol = *args;
    s32 volL;
    s32 volR;
    s32 pan;

    streamState->volumeFadeLen = 0;
    streamState->volume = vol;
    if (streamState->voiceMask == 0) {
        return;
    }
    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        vol = vol * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(streamState->voice, vol, vol, 0);
        akao_setVoiceVolume(streamState->voice + 1, vol, vol, 0);
    } else if (streamState->flags & AKAO_STREAM_STEREO) {
        vol <<= 15;
        vol >>= 16;
        akao_setVoiceVolume(streamState->voice, vol, 0, 0);
        akao_setVoiceVolume(streamState->voice + 1, 0, vol, 0);
    } else {
        pan = (u8)(streamState->pan >> 8);
        volL = vol * g_akaoPanCurve[pan] >> 16;
        volR = vol * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(streamState->voice, volL, volR, 0);
        akao_setVoiceVolume(streamState->voice + 1, volL, volR, 0);
    }
}

void akao_cmdFadeStreamVolume(AkaoArgs *args) {
    s16 len = 1;

    if (args->arg0 != 0) {
        len = args->arg0;
    }
    g_akaoStream.volumeStep = (s16)((s16)((u16)args->arg1 - (u16)g_akaoStream.volume) / len);
    g_akaoStream.volumeFadeLen = len;
}

void akao_cmdSetStreamPan(s32 *args) {
    AkaoStreamState *streamState = &g_akaoStream;
    s32 panArg = *args;
    s32 vol;
    s32 volL;
    s32 volR;
    s32 pan;

    streamState->pan = panArg;
    if (streamState->voiceMask == 0) {
        return;
    }
    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        vol = streamState->volume * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(streamState->voice, vol, vol, 0);
        akao_setVoiceVolume(streamState->voice + 1, vol, vol, 0);
    } else if (streamState->flags & AKAO_STREAM_STEREO) {
        vol = streamState->volume;
        vol <<= 15;
        vol >>= 16;
        akao_setVoiceVolume(streamState->voice, vol, 0, 0);
        akao_setVoiceVolume(streamState->voice + 1, 0, vol, 0);
    } else {
        pan = (panArg >> 8) & 0xFF;
        volL = streamState->volume * g_akaoPanCurve[pan] >> 16;
        volR = streamState->volume * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(streamState->voice, volL, volR, 0);
        akao_setVoiceVolume(streamState->voice + 1, volL, volR, 0);
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

void akao_streamPlayOneShot(s32 stream, s32 panArg, s32 addr, s32 loop) {
    AkaoStream *h = (AkaoStream *)stream;
    AkaoStreamState *streamState;
    s32 voiceIdx;
    u32 val; /* previous stream voice mask, then the upload byte count (separate locals don't match) */
    u32 voiceBits;

    voiceIdx = akao_allocStreamVoices();
    if (voiceIdx != -1) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        streamState = &g_akaoStream;
        streamState->pan = panArg;
        g_akaoStream.readPtr = h->data;
        streamState->bytesLeft = h->dataSize;
        streamState->loopLength = h->loopLength;
        voiceBits = (1 << voiceIdx) | (1 << (voiceIdx + 1));
        val = streamState->voiceMask;
        streamState->voice = voiceIdx;
        streamState->voiceMask = voiceBits;
        akao_keyOff(voiceBits | val);
        streamState->flags = h->flags;
        streamState->pitch = h->pitch;
        streamState->spuAddr = addr;
        SpuSetTransferMode(0);
        SpuSetTransferStartAddr(addr);
        g_akaoSpuBusy = 1;
        SpuSetTransferCallback(akao_streamOneShotStart);
        val = streamState->bytesLeft;
        if (val > 0x2000) {
            val = 0x2000;
        }
        SpuWrite(g_akaoStream.readPtr, val);
        g_akaoStream.readPtr += val;
        if (loop) {
            g_akaoSeState.reverbMask |= streamState->voiceMask;
        } else {
            g_akaoSeState.reverbMask &= ~streamState->voiceMask;
        }
        g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
        g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
    }
}

void akao_streamPlayResident(s32 unused, s32 flag) {
    s32 *h = g_akaoResidentStream;
    s32 *h2;
    s32 voiceIdx;
    s32 voiceBits;
    s32 irq;
    s32 old;

    if (h[8] == 0) {
        return;
    }
    voiceIdx = akao_allocStreamVoices();
    if (voiceIdx == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    voiceBits = (1 << voiceIdx) | (1 << (voiceIdx + 1));
    old = g_akaoStream.voiceMask;
    g_akaoStream.voice = voiceIdx;
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
s32 akao_streamNextReadBlock(block)
    s32 *block;
{
    g_akaoStream.blocksRead++;
    (*block)++;
    if (g_akaoStream.ringBlocks - 1 < *block) {
        *block = 0;
    }
    return *block;
}

void akao_streamStartRing(void) {
    StreamHeader *h;
    AkaoStreamState *streamState;
    s32 voiceIdx;
    s32 vol;
    s32 voiceBits;

    voiceIdx = akao_allocStreamVoices();
    if (voiceIdx == -1) {
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
    streamState = &g_akaoStream;
    g_akaoStream.readPtr = (u8 *)streamState->spuAddr;
    streamState->bytesLeft = h->dataSize;
    streamState->loopLength = h->loopLength;
    voiceBits = (1 << voiceIdx) | (1 << (voiceIdx + 1));
    streamState->voice = voiceIdx;
    streamState->voiceMask = voiceBits;
    streamState->readBlock = 0;
    akao_keyOff(voiceBits);
    streamState->flags = h->flags;
    streamState->pitch = h->pitch;
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(akao_streamStartRingVoices);
    if (streamState->flags & AKAO_STREAM_FOUR_VOICES) {
        SpuWrite(g_akaoStream.readPtr + 0xD0, 0x4000);
        akao_streamNextReadBlock(&streamState->readBlock);
        akao_streamNextReadBlock(&streamState->readBlock);
        akao_streamNextReadBlock(&streamState->readBlock);
        akao_streamNextReadBlock(&streamState->readBlock);
        streamState->voiceMask |= (1 << (voiceIdx - 2)) | (1 << (voiceIdx - 1));
    } else {
        SpuWrite(g_akaoStream.readPtr + 0xD0, 0x2000);
        akao_streamNextReadBlock(&streamState->readBlock);
        akao_streamNextReadBlock(&streamState->readBlock);
    }
    g_akaoSeState.reverbMask &= ~g_akaoStreamVoices;
    g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
    g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

void akao_streamRingKeyOn(s32 step, s32 irqBase, void (*cb)(void)) {
    if (g_akaoStream.voiceMask != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.bytesLeft >= 0xE61) {
            g_akaoStream.readPtr += step;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            irqBase = 0x1030;
        }
        SpuSetIRQAddr(irqBase + 8);
        akao_keyOn(g_akaoStream.voiceMask);
        SpuSetIRQ(1);
    }
}

void akao_streamStartRingVoices(void) {
    s32 step;
    s32 irqBase;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamSetVoice(g_akaoStream.voice, 1, 0x1100, 0x3100);
        akao_streamSetVoice(g_akaoStream.voice + 1, 2, 0x1900, 0x3900);
        akao_streamSetVoice(g_akaoStream.voice - 2, 1, 0x2100, 0x4100);
        akao_streamSetVoice(g_akaoStream.voice - 1, 2, 0x2900, 0x4900);
        step = 0x4000;
        irqBase = 0x3100;
    } else {
        akao_streamSetVoice(g_akaoStream.voice, 1, 0x1100, 0x2100);
        akao_streamSetVoice(g_akaoStream.voice + 1, 2, 0x1900, 0x2900);
        step = 0x2000;
        irqBase = 0x2100;
    }
    akao_streamRingKeyOn(step, irqBase, akao_streamRefillRingA);
}

s32 akao_streamRefillRing(s32 addrL, s32 addrR, s32 size, void (*cb)(void)) {
    AkaoStream *h;

    if (g_akaoStream.voiceMask != 0) {
        h = (AkaoStream *)(g_akaoStream.readPtr + 0x80);
        if (h->magic == AKAO_MAGIC) {
            SpuSetIRQ(0);
            SpuSetTransferStartAddr(addrL);
            akao_beginSpuTransfer();
            akao_streamNextReadBlock(&g_akaoStream.readBlock);
            SpuWrite(g_akaoStream.readPtr + 0xD0, size - 0xD0);
            g_akaoStream.unk20 = h->unk4;
            g_akaoStream.loopLength = h->loopLength;
            if (h->dataSize > h->loopLength) {
                SpuSetIRQCallback(cb);
                g_akaoStream.readPtr += size;
                if (g_akaoStream.readBlock == 0) {
                    g_akaoStream.readPtr = (u8 *)g_akaoStream.spuAddr;
                }
            } else {
                SpuSetIRQCallback(akao_streamStop);
                addrR = 0x1030;
                addrL = addrR;
            }
            akao_setVoiceLoopAddr(g_akaoStream.voice, addrL);
            akao_setVoiceLoopAddr(g_akaoStream.voice + 1, addrR);
            SpuSetIRQAddr(addrL + 8);
            SpuSetIRQ(1);
        }
    }
    return addrL;
}

void akao_streamRefillRingA(void) {
    s32 addrL;
    s32 addrR;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamNextReadBlock(&g_akaoStream.readBlock);
        addrL = akao_streamRefillRing(0x1100, 0x1900, 0x2000, akao_streamRefillRingB);
        addrR = 0x2900;
        if (addrL == 0x1030) {
            addrR = 0x1030;
        } else {
            addrL = 0x2100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.voice - 2, addrL);
        akao_setVoiceLoopAddr(g_akaoStream.voice - 1, addrR);
    } else {
        akao_streamRefillRing(0x1100, 0x1900, 0x1000, akao_streamRefillRingB);
    }
}

void akao_streamRefillRingB(void) {
    s32 addrL;
    s32 addrR;
    if (g_akaoStream.flags & AKAO_STREAM_FOUR_VOICES) {
        akao_streamNextReadBlock(&g_akaoStream.readBlock);
        addrL = akao_streamRefillRing(0x3100, 0x3900, 0x2000, akao_streamRefillRingA);
        addrR = 0x4900;
        if (addrL == 0x1030) {
            addrR = 0x1030;
        } else {
            addrL = 0x4100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.voice - 2, addrL);
        akao_setVoiceLoopAddr(g_akaoStream.voice - 1, addrR);
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


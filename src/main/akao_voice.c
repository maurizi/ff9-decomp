#include "ff9.h"

s32 akao_isqrt(s32 x) {
    s32 digitsLeft = 5;
    u32 xs = x << 8;
    u32 root = 0;
    u32 divisor = 0;
    u32 rem = 0;
    u32 digit;

    do {
        rem |= xs >> 24;
        divisor <<= 4;
        root <<= 4;
        if (rem >= (divisor | 1)) {
            if ((divisor | 12) * 12 < rem) {
                digit = 15;
            } else if ((divisor | 8) * 8 < rem) {
                digit = 12;
            } else if ((divisor | 4) * 4 < rem) {
                digit = 8;
            } else {
                digit = 4;
            }
            while (rem < (divisor | digit) * digit) {
                digit--;
            }
            divisor |= digit;
            rem -= divisor * digit;
            divisor += digit;
            root |= digit;
        }
        xs <<= 8;
        rem <<= 8;
        if (--digitsLeft == 0) {
            goto end;
        }
    } while (rem | xs);
    root <<= digitsLeft * 4;
end:
    return root;
}

s32 akao_lfoSine(u32 x) {
    s32 frac = x & 0xFF;
    s32 amp;

    x >>= 8;
    if (!(x & 1)) {
        frac = 0x100 - frac;
    }
    amp = akao_isqrt(0x10000 - frac * frac);
    if (x & 2) {
        amp = ~amp;
        amp++;
    }
    return amp;
}

static inline s32 akao_lfoSquareScaled_curve(s32 amp, u32 x) {
    if (x < 3) {
        switch (x) {
        case 1:
            amp /= 2;
            break;
        case 2:
            amp = amp * 3 / 4;
            break;
        default:
            amp /= 4;
            break;
        }
    }
    return amp;
}

s32 akao_lfoSineScaled(u32 x) {
    s32 frac = x & 0xFF;
    s32 amp;

    x >>= 8;
    if (!(x & 1)) {
        frac = 0x100 - frac;
    }
    amp = akao_isqrt(0x10000 - frac * frac);
    if (x & 2) {
        amp = ~amp;
        amp++;
    }
    x >>= 2;
    return akao_lfoSquareScaled_curve(amp, x);
}

s32 akao_lfoTriangle(u32 x) {
    s32 amp = (x & 0xFF) << 8;

    switch ((x >> 8) & 3) {
    case 3:
        amp -= 0x10000;
        break;
    case 2:
        amp = -amp;
        break;
    case 1:
        amp = 0x10000 - amp;
        break;
    }
    return amp;
}

s32 akao_lfoTriangleScaled(u32 x) {
    s32 amp = (x & 0xFF) << 8;

    x >>= 8;
    switch (x & 3) {
    case 3:
        amp -= 0x10000;
        break;
    case 2:
        amp = -amp;
        break;
    case 1:
        amp = 0x10000 - amp;
        break;
    }
    x >>= 2;
    return akao_lfoSquareScaled_curve(amp, x);
}

s32 akao_lfoSquare(s32 phase) {
    s32 amp = 0x10000;
    if (phase & 0x100) {
        amp = 0xFFFF0000;
    }
    return amp;
}

s32 akao_lfoSquareScaled(u32 x) {
    s32 amp = (x & 0x100) ? -0x10000 : 0x10000;

    x >>= 9;
    if (x < 3) {
        switch (x) {
        case 1:
            amp /= 2;
            break;
        case 2:
            amp = amp * 3 / 4;
            break;
        default:
            amp /= 4;
            break;
        }
    }
    return amp;
}

static inline s32 akao_lfoTable_inline(s32 x) {
    s32 i = x & 0xFF;
    return g_akaoLfoTableData[i] << 8;
}

s32 akao_lfoTable(s32 x) {
    return akao_lfoTable_inline(x);
}

s32 akao_lfoTableScaled(u32 code) {
    s32 amp;
    u32 sel;

    amp = code & 0xFF;
    amp = g_akaoLfoTableData[amp] << 8;

    sel = code >> 9;
    if (sel < 3) {
        switch (sel) {
        case 1:
            amp /= 2;
            break;
        case 2:
            amp = amp * 3 / 4;
            break;
        default:
            amp /= 4;
            break;
        }
    }

    return amp;
}

void akao_keyOn(u32 x) {
    SPU_REG16(SPU_KEY_ON_LO) = x;
    SPU_REG16(SPU_KEY_ON_HI) = x >> 16;
}

void akao_keyOff(s32 x) {
    SPU_REG16(SPU_KEY_OFF_LO) = x;
    SPU_REG16(SPU_KEY_OFF_HI) = (u32)x >> 16;
}

void akao_setReverbVoices(u32 x) {
    SPU_REG16(SPU_REVERB_ON_LO) = x;
    SPU_REG16(SPU_REVERB_ON_HI) = x >> 16;
}

void akao_setNoiseVoices(u32 x) {
    SPU_REG16(SPU_NOISE_ON_LO) = x;
    SPU_REG16(SPU_NOISE_ON_HI) = x >> 16;
}

void akao_setPitchModVoices(u32 x) {
    SPU_REG16(SPU_PITCH_MOD_LO) = x;
    SPU_REG16(SPU_PITCH_MOD_HI) = x >> 16;
}

void akao_setVoiceVolume(s32 voice, u32 volL, u32 volR, s32 vol) {
    if (vol != 0) {
        volL *= vol;
        volR *= vol;
        volL >>= 7;
        volR >>= 7;
    }
    ((SpuVoiceReg *)SPU_VOICE_BASE)[voice].volL = volL & 0x7FFF;
    ((SpuVoiceReg *)SPU_VOICE_BASE)[voice].volR = volR & 0x7FFF;
}

void akao_setVoicePitch(s32 voice, s32 pitch) {
    SPU_VOICE_REG(voice, SPU_VOICE_PITCH) = pitch;
}

void akao_setVoiceStartAddr(s32 voice, s32 addr) {
    SPU_VOICE_REG(voice, SPU_VOICE_START_ADDR) = (u32)addr >> 3;
}

void akao_setVoiceLoopAddr(s32 voice, s32 addr) {
    SPU_VOICE_REG(voice, SPU_VOICE_LOOP_ADDR) = (u32)addr >> 3;
}

void akao_setVoiceAdsr1(s32 voice, u16 adsr1) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) = adsr1;
}

void akao_setVoiceAdsr2(s32 voice, u16 adsr2) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR2) = adsr2;
}

void akao_setVoiceAttack(s32 voice, s32 rate, s32 mode) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) = SPU_VOICE_REG8(voice, SPU_VOICE_ADSR1) | ((((u32)mode >> 2) << 15) | (rate << 8));
}

void akao_setVoiceDecay(s32 voice, s32 rate) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) = (SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) & ~SPU_ADSR1_DECAY_RATE) | (rate << 4);
}

void akao_setVoiceSustainLevel(s32 voice, s32 level) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) = (SPU_VOICE_REG(voice, SPU_VOICE_ADSR1) & ~SPU_ADSR1_SUSTAIN_LEVEL) | level;
}

void akao_setVoiceSustain(s32 voice, s32 rate, s32 mode) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR2) = (SPU_VOICE_REG(voice, SPU_VOICE_ADSR2) & (SPU_ADSR2_RELEASE_EXP | SPU_ADSR2_RELEASE_RATE)) | ((((u32)mode >> 1) << 14) | (rate << 6));
}

void akao_setVoiceRelease(s32 voice, s32 rate, s32 mode) {
    SPU_VOICE_REG(voice, SPU_VOICE_ADSR2) = (SPU_VOICE_REG(voice, SPU_VOICE_ADSR2) & ~(SPU_ADSR2_RELEASE_EXP | SPU_ADSR2_RELEASE_RATE)) | ((((u32)mode >> 2) << 5) | rate);
}

void akao_writeVoice(s32 voice, AkaoVoiceRegs *regs, s32 vol) {
    u16 *reg;
    s32 outL;
    s32 outR;

    regs->dirty = 0;
    reg = (u16 *)(SPU_VOICE_BASE + voice * 16);
    if (vol == 0) {
        outL = regs->volL;
        outR = regs->volR;
    } else {
        outL = regs->volL * vol;
        outL >>= 7;
        outR = regs->volR * vol;
        outR >>= 7;
    }
    *reg = outL & 0x7FFF;
    *++reg = outR & 0x7FFF;
    *++reg = regs->pitch;
    *++reg = regs->startAddr >> 3;
    *++reg = regs->adsr1;
    *++reg = regs->adsr2;
    reg[2] = regs->loopAddr >> 3;
}

void akao_flushVoice(s32 voice, AkaoVoiceRegs *regs) {
    s32 dirtyBits = regs->dirty;

    if (dirtyBits == 0) {
        return;
    }
    regs->dirty = 0;
    if (dirtyBits & AKAO_VD_PITCH) {
        dirtyBits &= ~AKAO_VD_PITCH;
        akao_setVoicePitch(voice, regs->pitch);
        if (dirtyBits == 0) {
            return;
        }
    }
    if (dirtyBits & AKAO_VD_VOL) {
        dirtyBits &= ~AKAO_VD_VOL;
        akao_setVoiceVolume(voice, regs->volL, regs->volR, regs->volScale);
        if (dirtyBits == 0) {
            return;
        }
    }
    if (dirtyBits & AKAO_VD_START_ADDR) {
        dirtyBits &= ~AKAO_VD_START_ADDR;
        akao_setVoiceStartAddr(voice, regs->startAddr);
        if (dirtyBits == 0) {
            return;
        }
    }
    if (dirtyBits & AKAO_VD_LOOP_ADDR) {
        dirtyBits &= ~AKAO_VD_LOOP_ADDR;
        akao_setVoiceLoopAddr(voice, regs->loopAddr);
        if (dirtyBits == 0) {
            return;
        }
    }
    if (dirtyBits & AKAO_VD_ADSR2) {
        dirtyBits &= ~AKAO_VD_ADSR2;
        akao_setVoiceAdsr2(voice, regs->adsr2);
        if (dirtyBits == 0) {
            return;
        }
    }
    if (dirtyBits & AKAO_VD_ADSR1) {
        akao_setVoiceAdsr1(voice, regs->adsr1);
    }
}

void akao_stepChannelSlides(AkaoChannel *chan, u32 chBit, s32 kind) {
    s32 nw;
    if ((u16)chan->volumeSlideLen != 0) {
        s32 old;
        chan->volumeSlideLen = (u16)chan->volumeSlideLen - 1;
        old = chan->volume;
        nw = old + chan->volumeStep;
        if ((nw & 0xFFE00000) != (old & 0xFFE00000)) {
            chan->voiceDirty |= AKAO_VD_VOL;
        }
        chan->volume = nw;
    }
    if (kind == 0 && chan->expressionSlideLen != 0) {
        u16 old;
        chan->expressionSlideLen--;
        old = chan->expression;
        nw = old + chan->expressionStep;
        if ((nw & 0x7F00) != (old & 0x7F00)) {
            chan->voiceDirty |= AKAO_VD_VOL;
        }
        chan->expression = nw;
    }
    if (chan->panSlideLen != 0) {
        u16 old;
        chan->panSlideLen--;
        old = chan->pan;
        nw = old + chan->panStep;
        if ((nw & 0xFF00) != (old & 0xFF00)) {
            chan->voiceDirty |= AKAO_VD_VOL;
        }
        chan->pan = nw;
    }
    if (chan->vibratoDelayLeft != 0) {
        chan->vibratoDelayLeft--;
    }
    if (chan->tremoloDelayLeft != 0) {
        chan->tremoloDelayLeft--;
    }
    if ((u16)chan->vibratoPeriodSlideLen != 0) {
        chan->vibratoPeriodSlideLen = (u16)chan->vibratoPeriodSlideLen - 1;
        chan->vibratoPeriod += chan->vibratoPeriodStep;
        chan->vibratoRate = 0x1000000 / ((u32)chan->vibratoPeriod >> 12);
    }
    if ((u16)chan->tremoloPeriodSlideLen != 0) {
        chan->tremoloPeriodSlideLen = (u16)chan->tremoloPeriodSlideLen - 1;
        chan->tremoloPeriod += chan->tremoloPeriodStep;
        chan->tremoloRate = 0x1000000 / ((u32)chan->tremoloPeriod >> 12);
    }
    if (chan->panLfoPeriodSlideLen != 0) {
        chan->panLfoPeriodSlideLen--;
        chan->panLfoPeriod += chan->panLfoPeriodStep;
        chan->panLfoRate = 0x1000000 / ((u32)chan->panLfoPeriod >> 12);
    }
    if (chan->noiseTimer != 0) {
        if (--chan->noiseTimer == 0) {
            if (kind == 0) {
                g_akaoSeq->noiseMask ^= chBit;
            } else {
                g_akaoSeState.noiseMask ^= chBit;
            }
            g_akaoFlags.dirty |= AKAO_DIRTY_NOISE_CLOCK | AKAO_DIRTY_VOICE_MODES;
        }
    }
    if (chan->pitchModTimer != 0) {
        if (--chan->pitchModTimer == 0) {
            if (kind == 0) {
                g_akaoSeq->pitchModMask ^= chBit;
            } else {
                g_akaoSeState.pitchModMask ^= chBit;
            }
            g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
        }
    }
    if ((u16)chan->vibratoDepthSlideLen != 0) {
        u32 depthParam;
        u32 depth;
        chan->vibratoDepthSlideLen = (u16)chan->vibratoDepthSlideLen - 1;
        depthParam = chan->vibratoDepthParam + (u16)chan->vibratoDepthStep;
        chan->vibratoDepthParam = depthParam;
        depth = (depthParam & 0x7F00) >> 8;
        if (depthParam & 0x8000) {
            chan->vibratoDepth = (depth * chan->pitch) >> 7;
        } else {
            chan->vibratoDepth = (depth * ((chan->pitch * 15) >> 8)) >> 7;
        }
    }
    if ((u16)chan->tremoloDepthSlideLen != 0) {
        chan->tremoloDepthSlideLen = (u16)chan->tremoloDepthSlideLen - 1;
        chan->tremoloDepth += chan->tremoloDepthStep;
    }
    if ((u16)chan->panLfoDepthSlideLen != 0) {
        chan->panLfoDepthSlideLen = (u16)chan->panLfoDepthSlideLen - 1;
        chan->panLfoDepth += chan->panLfoDepthStep;
    }
    if ((u16)chan->pitchSlideLeft != 0) {
        s32 old;
        chan->pitchSlideLeft = (u16)chan->pitchSlideLeft - 1;
        old = chan->pitchSlide;
        nw = old + chan->pitchSlideStep;
        if ((nw & 0xFFFF0000) != (old & 0xFFFF0000)) {
            chan->voiceDirty |= AKAO_VD_PITCH;
        }
        chan->pitchSlide = nw;
    }
}

void akao_calcChannelVoice(AkaoChannel *chan, u32 bit) {
    u32 chFlags;
    s32 vol;
    s32 lfo;
    s32 x;
    s32 pan;
    s32 pitch;

    chFlags = chan->flags;
    vol = ((chan->volume >> 16) * (chan->expression >> 8)) >> 7;
    if ((chFlags & AKAO_CH_VIBRATO) && chan->vibratoDelayLeft == 0) {
        chan->vibratoPhase += chan->vibratoRate;
        lfo = chan->vibratoWave((u32)chan->vibratoPhase >> 16);
        pan = (s16)((u32)(chan->vibratoDepth * lfo) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        chan->vibratoOut = x;
        chan->voiceDirty |= AKAO_VD_PITCH;
    }
    if ((chFlags & AKAO_CH_TREMOLO) && chan->tremoloDelayLeft == 0) {
        chan->tremoloPhase += chan->tremoloRate;
        lfo = chan->tremoloWave((u32)chan->tremoloPhase >> 16);
        pan = (vol * (chan->tremoloDepth >> 8)) << 9 >> 16;
        chan->voiceDirty |= AKAO_VD_VOL;
        chan->tremoloOut = (pan * lfo) >> 16;
    }
    if (chFlags & AKAO_CH_PAN_LFO) {
        chan->panLfoPhase += chan->panLfoRate;
        lfo = chan->panLfoWave((u32)chan->panLfoPhase >> 16);
        chan->voiceDirty |= AKAO_VD_VOL;
        chan->panLfoOut = ((chan->panLfoDepth >> 8) * lfo) >> 16;
    }
    if (chFlags & AKAO_CH_VOL_FROM_PREV) {
        chan->voiceDirty |= AKAO_VD_VOL;
        vol = ((s16)(chan[-1].voicePitch << 1) * (chan->expression >> 8)) >> 7;
    }
    if (chan->voiceDirty & AKAO_VD_VOL) {
        vol += chan->tremoloOut;
        vol = (vol * (s32)(((u32)g_akaoSeq->volume >> 16) & 0x7F)) >> 7;
        pan = (chan->pan >> 8) + chan->panLfoOut;
        pan = (pan + (g_akaoSeq->pan >> 16) - 0x40) & 0xFF;
        if (g_akaoOutputFlags & AKAO_MODE_MONO) {
            chan->voiceVolL = chan->voiceVolR = (vol * g_akaoMonoGain) >> 15;
        } else {
            chan->voiceVolL = (vol * g_akaoPanCurve[pan]) >> 15;
            chan->voiceVolR = (vol * g_akaoPanCurve[pan ^ 0xFF]) >> 15;
        }
    }
    if (chFlags & AKAO_CH_PITCH_FROM_PREV) {
        pitch = chan[-1].voicePitch + chan->vibratoOut + (chan->pitchSlide >> 16);
        pan = g_akaoPitchScale & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        chan->voicePitch = ((u16)chan->fineTuneOfs + pitch) & 0x3FFF;
        chan->voiceDirty |= AKAO_VD_PITCH;
    } else if (chan->voiceDirty & AKAO_VD_PITCH) {
        pitch = chan->pitch + chan->vibratoOut + (chan->pitchSlide >> 16);
        pan = g_akaoPitchScale & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        chan->voicePitch = ((u16)chan->fineTuneOfs + pitch) & 0x3FFF;
    }
}

void akao_calcSeChannelVoice(AkaoChannel *chan, u32 bit) {
    u32 chFlags;
    s32 vol;
    s32 lfo;
    s32 x;
    s32 pan;
    s32 pitch;
    s32 k;

    chFlags = chan->flags;
    vol = ((chan->volume >> 16) * (chan->expression >> 8)) >> 7;
    if (chFlags & AKAO_CH_VIBRATO) {
        chan->vibratoPhase += chan->vibratoRate;
        lfo = chan->vibratoWave((u32)chan->vibratoPhase >> 16);
        pan = (s16)((u32)(chan->vibratoDepth * lfo) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        chan->vibratoOut = x;
        chan->voiceDirty |= AKAO_VD_PITCH;
    }
    if (chFlags & AKAO_CH_TREMOLO) {
        chan->tremoloPhase += chan->tremoloRate;
        lfo = chan->tremoloWave((u32)chan->tremoloPhase >> 16);
        pan = (vol * (chan->tremoloDepth >> 8)) << 9 >> 16;
        chan->voiceDirty |= AKAO_VD_VOL;
        chan->tremoloOut = (pan * lfo) >> 16;
    }
    if (chFlags & AKAO_CH_PAN_LFO) {
        chan->panLfoPhase += chan->panLfoRate;
        lfo = chan->panLfoWave((u32)chan->panLfoPhase >> 16);
        chan->voiceDirty |= AKAO_VD_VOL;
        chan->panLfoOut = ((chan->panLfoDepth >> 8) * lfo) >> 16;
    }
    if (chFlags & AKAO_CH_VOL_FROM_PREV) {
        chan->voiceDirty |= AKAO_VD_VOL;
        vol = ((s16)(chan[-1].voicePitch << 1) * (chan->expression >> 8)) >> 7;
    }
    if (chan->voiceDirty & AKAO_VD_VOL) {
        vol += chan->tremoloOut;
        if (!(chan->seMask & AKAO_SE_DEFAULT)) {
            vol = (vol * ((s16)chan->seVolume >> 8)) >> 7;
            pan = ((chan->pan + chan->sePan) >> 8) + chan->panLfoOut + 0x80;
            pan &= 0xFF;
        } else {
            pan = 0x80;
        }
        if (g_akaoOutputFlags & AKAO_MODE_MONO) {
            chan->voiceVolL = chan->voiceVolR = (vol * g_akaoMonoGain) >> 15;
        } else {
            chan->voiceVolL = (vol * g_akaoPanCurve[pan]) >> 15;
            chan->voiceVolR = (vol * g_akaoPanCurve[pan ^ 0xFF]) >> 15;
        }
    }
    if (chFlags & AKAO_CH_PITCH_FROM_PREV) {
        pitch = chan[-1].voicePitch + chan->vibratoOut + (chan->pitchSlide >> 16);
        k = chan->sePitch & 0xFF00;
        if (!(chan->seMask & AKAO_SE_DEFAULT) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        chan->voicePitch = ((u16)chan->fineTuneOfs + pitch) & 0x3FFF;
        chan->voiceDirty |= AKAO_VD_PITCH;
    } else if (chan->voiceDirty & AKAO_VD_PITCH) {
        pitch = chan->pitch + chan->vibratoOut + (chan->pitchSlide >> 16);
        k = chan->sePitch & 0xFF00;
        if (!(chan->seMask & AKAO_SE_DEFAULT) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        chan->voicePitch = ((u16)chan->fineTuneOfs + pitch) & 0x3FFF;
    }
}

s32 akao_stealVoice(s32 allVoices) {
    s32 i;
    s32 min;
    s32 best;
    u32 envLevel;
    AkaoVoiceEnv *env;

    if (allVoices != 0) {
        i = 0;
    } else {
        i = g_akaoSeq->firstVoice;
    }
    min = 0x7FFF;
    best = 24;
    env = &g_akaoVoiceEnv[i];
    do {
        envLevel = (u16)env->envelope;
        if (env->envelope < (s16)min) {
            min = envLevel;
            best = i;
        }
        i++;
        env++;
    } while (i < 24);
    if ((s16)min == 0x7FFF) {
        return 24;
    }
    akao_detachVoice(g_akaoChannels, best, min);
    return best;
}

s32 akao_findFreeVoice(s32 allVoices) {
    AkaoVoiceEnv *env;
    if (allVoices != 0) {
        allVoices = 0;
    } else {
        allVoices = g_akaoSeq->firstVoice;
    }
    while ((env = &g_akaoVoiceEnv[allVoices])->envelope != 0) {
        allVoices++;
        if (allVoices >= 0x18) {
            break;
        }
    }
    return allVoices;
}

void akao_assignVoices(AkaoChannel *ch, u32 chMask, u32 keep, u32 *used) {
    u32 bit;
    s32 i;
    u32 on;
    s32 priority;
    s32 voiceIdx;

    bit = 1;
    i = 0;
    on = chMask & g_akaoSeq->keyOnMask;

    do {
        if (chMask & bit) {
            akao_calcChannelVoice(ch, bit);
            if (ch->voiceDirty != 0) {
                if (g_akaoMuteMask & bit) {
                    ch->voiceVolR = 0;
                    ch->voiceVolL = 0;
                }
                if (on & bit) {
                    if (keep & bit) {
                        do {
                            *used |= 1 << i;
                        } while (0);
                        ch->voice = i;
                    } else {
                        priority = (g_akaoSeq->priorityMask & bit) != 0;
                        voiceIdx = akao_findFreeVoice(priority);
                        if (voiceIdx == 24) {
                            g_akaoSeq->flags |= AKAO_SEQ_VOICE_STOLEN;
                            voiceIdx = akao_stealVoice(priority);
                        }
                        if (voiceIdx == 24) {
                            ch->voice = voiceIdx;
                            g_akaoSeq->flags |= AKAO_SEQ_NO_VOICE;
                        } else {
                            *used |= 1 << voiceIdx;
                            ch->voice = voiceIdx;
                            g_akaoVoiceEnv[voiceIdx].envelope = 0x7FFF;
                        }
                    }
                    if (ch->voice < 24) {
                        akao_writeVoice(ch->voice, (AkaoVoiceRegs *)&ch->voice, (u16)ch->voiceVolScale);
                        g_akaoVoiceOwner[ch->voice] = g_akaoSeq;
                        g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
                    }
                } else if (ch->voice < 24) {
                    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                    if (chMask) {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->flags);
                    } else {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->flags);
                    }
                }
            }
            chMask &= ~bit;
        }
        bit <<= 1;
        ch++;
        i++;
    } while (chMask != 0);
}

/* K&R definition: callers pass two or three arguments */
void akao_detachVoice(ch, voice)
    AkaoChannel *ch;
    s32 voice;
{
    u32 i;
    for (i = 0; i < 32; i++) {
        if (voice == ch->voice) {
            ch->voice = 0x18;
        }
        ch++;
    }
    if (g_akaoSubSeq != NULL) {
        ch = g_akaoSubChannels;
        for (i = 0; i < 32; i++) {
            if (voice == ch->voice) {
                ch->voice = 0x18;
            }
            ch++;
        }
    }
}

void akao_readEnvelopes(u32 extra) {
    u32 heldMask = (g_akaoSeq->activeMask & g_akaoSeq->keepVoiceMask) | extra;
    u32 i;
    AkaoVoiceEnv *env;
    if (g_akaoSubSeq != NULL) {
        heldMask |= g_akaoSubSeq->activeMask & g_akaoSubSeq->keepVoiceMask;
    }
    for (i = 0, env = g_akaoVoiceEnv; i < 24; i++, env++) {
        if (heldMask & (1 << i)) {
            env->envelope = 0x7FFF;
        } else {
            SpuGetVoiceEnvelope(i, &env->envelope);
            if (env->envelope == 0) {
                akao_detachVoice(g_akaoChannels, i);
            }
        }
    }
}

/* K&R definition: akao_updateVoices also passes the channel array */
void akao_applyCrossfadeVolume(seq)
    AkaoSeq *seq;
{
    s32 vol;
    g_akaoCrossfade.savedVolume = vol = seq->volume;
    vol >>= 16;
    vol *= g_akaoCrossfade.volume >> 8;
    vol /= 127;
    vol <<= 8;
    seq->volume = vol;
}

/* K&R definition: akao_updateVoices also passes the channel array */
void akao_restoreCrossfadeVolume(seq)
    AkaoSeq *seq;
{
    seq->volume = g_akaoCrossfade.savedVolume;
}

void akao_updateVoices(void) {
    u32 keyon;
    u32 ex;
    u32 m1;
    u32 m2;
    u32 b1;
    u32 b2;
    u32 bit;
    AkaoChannel *ch;
    AkaoSeq *mainSeq;
    AkaoSeq *subSeq;

    ex = g_akaoSeState.activeMask | g_akaoSeState.pausedMask | g_akaoStreamVoices;
    keyon = 0;
    if ((g_akaoSeq->activeMask & g_akaoSeq->keyOnMask) ||
        (g_akaoSubSeq != NULL && (g_akaoSubSeq->activeMask & g_akaoSubSeq->keyOnMask))) {
        akao_readEnvelopes(ex);
    }
    if (g_akaoSubSeq != NULL) {
        if (g_akaoFlags.mode & AKAO_MODE_CROSSFADE) {
            akao_applyCrossfadeVolume(g_akaoSubSeq, g_akaoSubChannels);
        }
        subSeq = g_akaoSubSeq;
        m1 = subSeq->activeMask & subSeq->playingMask & ~(subSeq->keepVoiceMask & ex);
        b1 = m1 & subSeq->keepVoiceMask & ~ex;
        g_akaoSeq = subSeq;
        if (m1 & g_akaoSeq->priorityMask) {
            akao_assignVoices(g_akaoSubChannels, m1 & g_akaoSeq->priorityMask, b1, &keyon);
            g_akaoSeq->keyOnMask &= ~g_akaoSeq->priorityMask;
            m1 &= ~g_akaoSeq->priorityMask;
        }
        mainSeq = &g_akaoMainSeq;
        g_akaoSeq = mainSeq;
        m2 = mainSeq->activeMask & mainSeq->playingMask & ~(mainSeq->keepVoiceMask & (b1 | ex));
        b2 = m2 & mainSeq->keepVoiceMask & ~(b1 | ex);
        if (m2 & g_akaoSeq->priorityMask) {
            akao_assignVoices(g_akaoChannels, m2 & g_akaoSeq->priorityMask, b2, &keyon);
            g_akaoSeq->keyOnMask &= ~g_akaoSeq->priorityMask;
            m2 &= ~g_akaoSeq->priorityMask;
        }
        if (m1) {
            g_akaoSeq = g_akaoSubSeq;
            b1 &= ~b2;
            akao_assignVoices(g_akaoSubChannels, m1, b1, &keyon);
            g_akaoSeq->keyOnMask = 0;
            g_akaoSeq = mainSeq;
        }
        if (m2) {
            akao_assignVoices(g_akaoChannels, m2, b2, &keyon);
            g_akaoSeq->keyOnMask = 0;
        }
        if (g_akaoFlags.mode & AKAO_MODE_CROSSFADE) {
            if (m2) {
                akao_restoreCrossfadeVolume(g_akaoSubSeq, g_akaoSubChannels);
            } else {
                akao_restoreCrossfadeVolume(g_akaoSubSeq, g_akaoSubChannels);
            }
        }
    } else {
        m2 = g_akaoSeq->activeMask & g_akaoSeq->playingMask & ~(g_akaoSeq->keepVoiceMask & ex);
        bit = m2 & g_akaoSeq->keepVoiceMask & ~ex;
        if (m2 & g_akaoSeq->priorityMask) {
            akao_assignVoices(g_akaoChannels, m2 & g_akaoSeq->priorityMask, bit, &keyon);
            g_akaoSeq->keyOnMask &= ~g_akaoSeq->priorityMask;
            m2 &= ~g_akaoSeq->priorityMask;
        }
        if (m2) {
            akao_assignVoices(g_akaoChannels, m2, bit, &keyon);
            g_akaoSeq->keyOnMask = 0;
        }
    }
    m2 = g_akaoSeState.activeMask & g_akaoSeState.playingMask;
    if (m2) {
        keyon |= g_akaoSeState.keyOnMask;
        bit = 0x100;
        ch = g_akaoSeChannels;
        for (; m2 != 0; bit <<= 1, ch++) {
            if (m2 & bit) {
                akao_calcSeChannelVoice(ch, bit);
                if (ch->voiceDirty) {
                    ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->flags);
                }
                m2 &= ~bit;
            }
        }
        g_akaoSeState.keyOnMask = 0;
    }
    m2 = g_akaoFlags.dirty;
    if (m2 & AKAO_DIRTY_REVERB_DEPTH) {
        SpuSetReverbModeDepth(g_akaoSeq->reverbDepth >> 12, g_akaoSeq->reverbDepth >> 12);
        g_akaoFlags.dirty &= ~AKAO_DIRTY_REVERB_DEPTH;
    }
    if (m2 & AKAO_DIRTY_NOISE_CLOCK) {
        SpuSetNoiseClock(g_akaoSeState.activeMask ? g_akaoSeState.noiseClock : g_akaoSeq->noiseClock);
        g_akaoFlags.dirty &= ~AKAO_DIRTY_NOISE_CLOCK;
    }
    if (m2 & AKAO_DIRTY_VOICE_MODES) {
        akao_buildVoiceModeMask((u32 *)&g_akaoVoiceModeMasks[1], g_akaoSubSeq->noiseMask, g_akaoSeq->noiseMask, g_akaoSeState.noiseMask);
        akao_buildVoiceModeMask((u32 *)&g_akaoVoiceModeMasks[0], g_akaoSubSeq->reverbMask, g_akaoSeq->reverbMask, g_akaoSeState.reverbMask);
        akao_buildVoiceModeMask((u32 *)&g_akaoVoiceModeMasks[2], g_akaoSubSeq->pitchModMask, g_akaoSeq->pitchModMask, g_akaoSeState.pitchModMask);
        akao_setReverbVoices(g_akaoVoiceModeMasks[0]);
        akao_setNoiseVoices(g_akaoVoiceModeMasks[1]);
        akao_setPitchModVoices(g_akaoVoiceModeMasks[2]);
        g_akaoFlags.dirty &= ~AKAO_DIRTY_VOICE_MODES;
    }
    if (keyon) {
        akao_keyOn(keyon);
    }
}

void akao_channelsToVoiceMask(AkaoChannel *chan, u32 *out, u32 chMask, u32 keep) {
    u32 bit = 1;
    do {
        if (chMask & bit) {
            if (chan->voice < 0x18) {
                *out |= 1 << chan->voice;
            }
        }
        chMask &= ~bit;
        chan++;
        bit <<= 1;
    } while (chMask != 0);
    *out &= keep;
}

void akao_flushKeyOffs(void) {
    u32 out;
    u32 keep;
    u32 prioMask;
    u32 subOffMask;
    u32 mainOffMask;

    subOffMask = 0;
    out = 0;
    keep = ~((g_akaoSeState.activeMask | g_akaoSeState.pausedMask) | g_akaoStreamVoices);
    if (g_akaoSubSeq != NULL) {
        subOffMask = g_akaoSubSeq->keyOffMask;
        prioMask = subOffMask & g_akaoSubSeq->priorityMask;
        if (prioMask != 0) {
            akao_channelsToVoiceMask(g_akaoSubChannels, &out, prioMask, keep);
            g_akaoSubSeq->keyOffMask &= ~g_akaoSubSeq->priorityMask;
            subOffMask &= ~g_akaoSubSeq->priorityMask;
        }
    }
    mainOffMask = g_akaoSeq->keyOffMask;
    prioMask = mainOffMask & g_akaoSeq->priorityMask;
    if (prioMask != 0) {
        akao_channelsToVoiceMask(g_akaoChannels, &out, prioMask, keep);
        g_akaoSeq->keyOffMask &= ~g_akaoSeq->priorityMask;
        mainOffMask &= ~g_akaoSeq->priorityMask;
    }
    if (g_akaoSubSeq != NULL && subOffMask != 0) {
        akao_channelsToVoiceMask(g_akaoSubChannels, &out, subOffMask, keep);
        g_akaoSubSeq->keyOffMask = 0;
    }
    if (mainOffMask != 0) {
        akao_channelsToVoiceMask(g_akaoChannels, &out, mainOffMask, keep);
        g_akaoSeq->keyOffMask = 0;
    }
    out |= g_akaoSeState.keyOffMask;
    g_akaoSeState.keyOffMask = 0;
    if (out != 0) {
        akao_keyOff(out);
    }
}

void akao_buildVoiceModeMask(u32 *out, u32 m1, u32 m2, u32 extra) {
    u32 subMask;
    u32 voiceMask;
    u32 keep;
    u32 mainMask;
    AkaoSeq *subSeq;

    keep = ~((g_akaoSeState.activeMask | g_akaoSeState.pausedMask) | g_akaoStreamVoices);
    subMask = 0;
    voiceMask = 0;
    subSeq = g_akaoSubSeq;
    if (subSeq != NULL) {
        subMask = subSeq->activeMask & m1;
        if (subMask & subSeq->priorityMask) {
            akao_channelsToVoiceMask(g_akaoSubChannels, &voiceMask, subMask & subSeq->priorityMask, keep);
            subMask &= ~g_akaoSubSeq->priorityMask;
        }
    }
    mainMask = g_akaoSeq->activeMask & m2;
    if (mainMask & g_akaoSeq->priorityMask) {
        akao_channelsToVoiceMask(g_akaoChannels, &voiceMask, mainMask & g_akaoSeq->priorityMask, keep);
        mainMask &= ~g_akaoSeq->priorityMask;
    }
    if (g_akaoSubSeq != NULL && subMask != 0) {
        akao_channelsToVoiceMask(g_akaoSubChannels, &voiceMask, subMask, keep);
    }
    if (mainMask != 0) {
        akao_channelsToVoiceMask(g_akaoChannels, &voiceMask, mainMask, keep);
    }
    *out = voiceMask |= extra;
    g_akaoFlags.dirty |= AKAO_DIRTY_VOICE_MODES;
}

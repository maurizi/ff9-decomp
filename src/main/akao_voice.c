#include "ff9.h"

s32 akao_isqrt(s32 x) {
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

s32 akao_lfoSine(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = akao_isqrt(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    return r;
}

static inline s32 akao_lfoSquareScaled_curve(s32 r, u32 x) {
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

s32 akao_lfoSineScaled(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = akao_isqrt(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    x >>= 2;
    return akao_lfoSquareScaled_curve(r, x);
}

s32 akao_lfoTriangle(u32 x) {
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

s32 akao_lfoTriangleScaled(u32 x) {
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
    return akao_lfoSquareScaled_curve(v, x);
}

s32 akao_lfoSquare(s32 a) {
    s32 r = 0x10000;
    if (a & 0x100) {
        r = 0xFFFF0000;
    }
    return r;
}

s32 akao_lfoSquareScaled(u32 x) {
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

static inline s32 akao_lfoTable_inline(s32 x) {
    s32 i = x & 0xFF;
    return g_akaoLfoTableData[i] << 8;
}

s32 akao_lfoTable(s32 x) {
    return akao_lfoTable_inline(x);
}

s32 akao_lfoTableScaled(u32 code) {
    s32 v;
    u32 sel;

    v = code & 0xFF;
    v = g_akaoLfoTableData[v] << 8;

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

void akao_setVoiceVolume(s32 v, u32 l, u32 r, s32 vol) {
    if (vol != 0) {
        l *= vol;
        r *= vol;
        l >>= 7;
        r >>= 7;
    }
    ((SpuVoiceReg *)SPU_VOICE_BASE)[v].volL = l & 0x7FFF;
    ((SpuVoiceReg *)SPU_VOICE_BASE)[v].volR = r & 0x7FFF;
}

void akao_setVoicePitch(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x4) = x;
}

void akao_setVoiceStartAddr(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x6) = (u32)x >> 3;
}

void akao_setVoiceLoopAddr(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0xE) = (u32)x >> 3;
}

void akao_setVoiceAdsr1(s32 v, u16 x) {
    SPU_VOICE_REG(v, 0x8) = x;
}

void akao_setVoiceAdsr2(s32 v, u16 x) {
    SPU_VOICE_REG(v, 0xA) = x;
}

void akao_setVoiceAttack(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0x8) = SPU_VOICE_REG8(v, 0x8) | ((((u32)b >> 2) << 15) | (a << 8));
}

void akao_setVoiceDecay(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x8) = (SPU_VOICE_REG(v, 0x8) & 0xFF0F) | (x << 4);
}

void akao_setVoiceSustainLevel(s32 v, s32 x) {
    SPU_VOICE_REG(v, 0x8) = (SPU_VOICE_REG(v, 0x8) & 0xFFF0) | x;
}

void akao_setVoiceSustain(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0xA) = (SPU_VOICE_REG(v, 0xA) & 0x3F) | ((((u32)b >> 1) << 14) | (a << 6));
}

void akao_setVoiceRelease(s32 v, s32 a, s32 b) {
    SPU_VOICE_REG(v, 0xA) = (SPU_VOICE_REG(v, 0xA) & 0xFFC0) | ((((u32)b >> 2) << 5) | a);
}

void akao_writeVoice(s32 v, AkaoVoiceRegs *p, s32 vol) {
    u16 *r;
    s32 a;
    s32 b;

    p->dirty = 0;
    r = (u16 *)(SPU_VOICE_BASE + v * 16);
    if (vol == 0) {
        a = p->volL;
        b = p->volR;
    } else {
        a = p->volL * vol;
        a >>= 7;
        b = p->volR * vol;
        b >>= 7;
    }
    *r = a & 0x7FFF;
    *++r = b & 0x7FFF;
    *++r = p->pitch;
    *++r = p->startAddr >> 3;
    *++r = p->adsr1;
    *++r = p->adsr2;
    r[2] = p->loopAddr >> 3;
}

void akao_flushVoice(s32 v, AkaoVoiceRegs *p) {
    s32 f = p->dirty;

    if (f == 0) {
        return;
    }
    p->dirty = 0;
    if (f & 0x10) {
        f &= ~0x10;
        akao_setVoicePitch(v, p->pitch);
        if (f == 0) {
            return;
        }
    }
    if (f & 3) {
        f &= ~3;
        akao_setVoiceVolume(v, p->volL, p->volR, p->volScale);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x80) {
        f &= ~0x80;
        akao_setVoiceStartAddr(v, p->startAddr);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x10000) {
        f &= ~0x10000;
        akao_setVoiceLoopAddr(v, p->loopAddr);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x6600) {
        f &= ~0x6600;
        akao_setVoiceAdsr2(v, p->adsr2);
        if (f == 0) {
            return;
        }
    }
    if (f & 0x9900) {
        akao_setVoiceAdsr1(v, p->adsr1);
    }
}

void akao_stepChannelSlides(AkaoChannel *c, u32 mask, s32 kind) {
    s32 nw;
    if ((u16)c->volumeSlideLen != 0) {
        s32 old;
        c->volumeSlideLen = (u16)c->volumeSlideLen - 1;
        old = c->volume;
        nw = old + c->volumeStep;
        if ((nw & 0xFFE00000) != (old & 0xFFE00000)) {
            c->voiceDirty |= 3;
        }
        c->volume = nw;
    }
    if (kind == 0 && c->expressionSlideLen != 0) {
        u16 old;
        c->expressionSlideLen--;
        old = c->expression;
        nw = old + c->expressionStep;
        if ((nw & 0x7F00) != (old & 0x7F00)) {
            c->voiceDirty |= 3;
        }
        c->expression = nw;
    }
    if (c->panSlideLen != 0) {
        u16 old;
        c->panSlideLen--;
        old = c->pan;
        nw = old + c->panStep;
        if ((nw & 0xFF00) != (old & 0xFF00)) {
            c->voiceDirty |= 3;
        }
        c->pan = nw;
    }
    if (c->vibratoDelayLeft != 0) {
        c->vibratoDelayLeft--;
    }
    if (c->tremoloDelayLeft != 0) {
        c->tremoloDelayLeft--;
    }
    if ((u16)c->vibratoPeriodSlideLen != 0) {
        c->vibratoPeriodSlideLen = (u16)c->vibratoPeriodSlideLen - 1;
        c->vibratoPeriod += c->vibratoPeriodStep;
        c->vibratoRate = 0x1000000 / ((u32)c->vibratoPeriod >> 12);
    }
    if ((u16)c->tremoloPeriodSlideLen != 0) {
        c->tremoloPeriodSlideLen = (u16)c->tremoloPeriodSlideLen - 1;
        c->tremoloPeriod += c->tremoloPeriodStep;
        c->tremoloRate = 0x1000000 / ((u32)c->tremoloPeriod >> 12);
    }
    if (c->panLfoPeriodSlideLen != 0) {
        c->panLfoPeriodSlideLen--;
        c->panLfoPeriod += c->panLfoPeriodStep;
        c->panLfoRate = 0x1000000 / ((u32)c->panLfoPeriod >> 12);
    }
    if (c->noiseTimer != 0) {
        if (--c->noiseTimer == 0) {
            if (kind == 0) {
                g_akaoSeq->unk3C ^= mask;
            } else {
                g_akaoSeState.unk1C ^= mask;
            }
            g_akaoFlags.unk8 |= 0x110;
        }
    }
    if (c->pitchModTimer != 0) {
        if (--c->pitchModTimer == 0) {
            if (kind == 0) {
                g_akaoSeq->unk44 ^= mask;
            } else {
                g_akaoSeState.unk24 ^= mask;
            }
            g_akaoFlags.unk8 |= 0x100;
        }
    }
    if ((u16)c->vibratoDepthSlideLen != 0) {
        u32 v;
        u32 n;
        c->vibratoDepthSlideLen = (u16)c->vibratoDepthSlideLen - 1;
        v = c->vibratoDepthParam + (u16)c->vibratoDepthStep;
        c->vibratoDepthParam = v;
        n = (v & 0x7F00) >> 8;
        if (v & 0x8000) {
            c->vibratoDepth = (n * c->pitch) >> 7;
        } else {
            c->vibratoDepth = (n * ((c->pitch * 15) >> 8)) >> 7;
        }
    }
    if ((u16)c->tremoloDepthSlideLen != 0) {
        c->tremoloDepthSlideLen = (u16)c->tremoloDepthSlideLen - 1;
        c->tremoloDepth += c->tremoloDepthStep;
    }
    if ((u16)c->panLfoDepthSlideLen != 0) {
        c->panLfoDepthSlideLen = (u16)c->panLfoDepthSlideLen - 1;
        c->panLfoDepth += c->panLfoDepthStep;
    }
    if ((u16)c->pitchSlideLeft != 0) {
        s32 old;
        c->pitchSlideLeft = (u16)c->pitchSlideLeft - 1;
        old = c->pitchSlide;
        nw = old + c->pitchSlideStep;
        if ((nw & 0xFFFF0000) != (old & 0xFFFF0000)) {
            c->voiceDirty |= 0x10;
        }
        c->pitchSlide = nw;
    }
}

void akao_calcChannelVoice(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;

    f = c->flags;
    vol = ((c->volume >> 16) * (c->expression >> 8)) >> 7;
    if ((f & 1) && c->vibratoDelayLeft == 0) {
        c->vibratoPhase += c->vibratoRate;
        t = c->vibratoWave((u32)c->vibratoPhase >> 16);
        pan = (s16)((u32)(c->vibratoDepth * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->vibratoOut = x;
        c->voiceDirty |= 0x10;
    }
    if ((f & 2) && c->tremoloDelayLeft == 0) {
        c->tremoloPhase += c->tremoloRate;
        t = c->tremoloWave((u32)c->tremoloPhase >> 16);
        pan = (vol * (c->tremoloDepth >> 8)) << 9 >> 16;
        c->voiceDirty |= 3;
        c->tremoloOut = (pan * t) >> 16;
    }
    if (f & 4) {
        c->panLfoPhase += c->panLfoRate;
        t = c->panLfoWave((u32)c->panLfoPhase >> 16);
        c->voiceDirty |= 3;
        c->panLfoOut = ((c->panLfoDepth >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->voiceDirty |= 3;
        vol = ((s16)(c[-1].voicePitch << 1) * (c->expression >> 8)) >> 7;
    }
    if (c->voiceDirty & 3) {
        vol += c->tremoloOut;
        vol = (vol * (s32)(((u32)g_akaoSeq->unk50 >> 16) & 0x7F)) >> 7;
        pan = (c->pan >> 8) + c->panLfoOut;
        pan = (pan + (g_akaoSeq->unk5C >> 16) - 0x40) & 0xFF;
        if (g_akaoOutputFlags & 2) {
            c->voiceVolL = c->voiceVolR = (vol * g_akaoMonoGain) >> 15;
        } else {
            c->voiceVolL = (vol * g_akaoPanCurve[pan]) >> 15;
            c->voiceVolR = (vol * g_akaoPanCurve[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].voicePitch + c->vibratoOut + (c->pitchSlide >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->voicePitch = ((u16)c->fineTuneOfs + pitch) & 0x3FFF;
        c->voiceDirty |= 0x10;
    } else if (c->voiceDirty & 0x10) {
        pitch = c->pitch + c->vibratoOut + (c->pitchSlide >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->voicePitch = ((u16)c->fineTuneOfs + pitch) & 0x3FFF;
    }
}

void akao_calcSeChannelVoice(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;
    s32 k;

    f = c->flags;
    vol = ((c->volume >> 16) * (c->expression >> 8)) >> 7;
    if (f & 1) {
        c->vibratoPhase += c->vibratoRate;
        t = c->vibratoWave((u32)c->vibratoPhase >> 16);
        pan = (s16)((u32)(c->vibratoDepth * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->vibratoOut = x;
        c->voiceDirty |= 0x10;
    }
    if (f & 2) {
        c->tremoloPhase += c->tremoloRate;
        t = c->tremoloWave((u32)c->tremoloPhase >> 16);
        pan = (vol * (c->tremoloDepth >> 8)) << 9 >> 16;
        c->voiceDirty |= 3;
        c->tremoloOut = (pan * t) >> 16;
    }
    if (f & 4) {
        c->panLfoPhase += c->panLfoRate;
        t = c->panLfoWave((u32)c->panLfoPhase >> 16);
        c->voiceDirty |= 3;
        c->panLfoOut = ((c->panLfoDepth >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->voiceDirty |= 3;
        vol = ((s16)(c[-1].voicePitch << 1) * (c->expression >> 8)) >> 7;
    }
    if (c->voiceDirty & 3) {
        vol += c->tremoloOut;
        if (!(c->seMask & 0x2000000)) {
            vol = (vol * ((s16)c->seVolume >> 8)) >> 7;
            pan = ((c->pan + c->sePan) >> 8) + c->panLfoOut + 0x80;
            pan &= 0xFF;
        } else {
            pan = 0x80;
        }
        if (g_akaoOutputFlags & 2) {
            c->voiceVolL = c->voiceVolR = (vol * g_akaoMonoGain) >> 15;
        } else {
            c->voiceVolL = (vol * g_akaoPanCurve[pan]) >> 15;
            c->voiceVolR = (vol * g_akaoPanCurve[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].voicePitch + c->vibratoOut + (c->pitchSlide >> 16);
        k = c->sePitch & 0xFF00;
        if (!(c->seMask & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->voicePitch = ((u16)c->fineTuneOfs + pitch) & 0x3FFF;
        c->voiceDirty |= 0x10;
    } else if (c->voiceDirty & 0x10) {
        pitch = c->pitch + c->vibratoOut + (c->pitchSlide >> 16);
        k = c->sePitch & 0xFF00;
        if (!(c->seMask & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->voicePitch = ((u16)c->fineTuneOfs + pitch) & 0x3FFF;
    }
}

s32 akao_stealVoice(s32 arg0) {
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
    e = &g_akaoVoiceEnv[i];
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
    akao_detachVoice(g_akaoChannels, best, min);
    return best;
}

s32 akao_findFreeVoice(s32 a) {
    Unk8007F798 *p;
    if (a != 0) {
        a = 0;
    } else {
        a = g_akaoSeq->unk38;
    }
    while ((p = &g_akaoVoiceEnv[a])->unk4 != 0) {
        a++;
        if (a >= 0x18) {
            break;
        }
    }
    return a;
}

void akao_assignVoices(AkaoChannel *ch, u32 mask, u32 keep, u32 *used) {
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
            akao_calcChannelVoice(ch, bit);
            if (ch->voiceDirty != 0) {
                if (D_80080AC4 & bit) {
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
                        f = (g_akaoSeq->unk8 & bit) != 0;
                        v = akao_findFreeVoice(f);
                        if (v == 24) {
                            g_akaoSeq->unk0 |= 2;
                            v = akao_stealVoice(f);
                        }
                        if (v == 24) {
                            ch->voice = v;
                            g_akaoSeq->unk0 |= 1;
                        } else {
                            *used |= 1 << v;
                            ch->voice = v;
                            g_akaoVoiceEnv[v].unk4 = 0x7FFF;
                        }
                    }
                    if (ch->voice < 24) {
                        akao_writeVoice(ch->voice, (AkaoVoiceRegs *)&ch->voice, (u16)ch->voiceVolScale);
                        g_akaoVoiceOwner[ch->voice] = g_akaoSeq;
                        g_akaoFlags.unk8 |= 0x100;
                    }
                } else if (ch->voice < 24) {
                    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                    if (mask) {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->flags);
                    } else {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->flags);
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
void akao_detachVoice(c, voice)
    AkaoChannel *c;
    s32 voice;
{
    u32 i;
    for (i = 0; i < 32; i++) {
        if (voice == c->voice) {
            c->voice = 0x18;
        }
        c++;
    }
    if (g_akaoSubSeq != NULL) {
        c = g_akaoSubChannels;
        for (i = 0; i < 32; i++) {
            if (voice == c->voice) {
                c->voice = 0x18;
            }
            c++;
        }
    }
}

void akao_readEnvelopes(u32 extra) {
    u32 m = (g_akaoSeq->unk4 & g_akaoSeq->unkC) | extra;
    u32 i;
    Unk8007F798 *p;
    if (g_akaoSubSeq != NULL) {
        m |= g_akaoSubSeq->unk4 & g_akaoSubSeq->unkC;
    }
    for (i = 0, p = g_akaoVoiceEnv; i < 24; i++, p++) {
        if (m & (1 << i)) {
            p->unk4 = 0x7FFF;
        } else {
            SpuGetVoiceEnvelope(i, &p->unk4);
            if (p->unk4 == 0) {
                akao_detachVoice(g_akaoChannels, i);
            }
        }
    }
}

void func_80058990(AkaoChannel *c) {
    s32 v;
    D_80080A18.unkC = v = c->tremoloPeriodStep;
    v >>= 16;
    v *= D_80080A18.unk0 >> 8;
    v /= 127;
    v <<= 8;
    c->tremoloPeriodStep = v;
}

void func_800589E0(Unk800589E0 *a) {
    a->unk50 = D_80080A24;
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
    Unk80080A10 *s;
    Unk80080A10 *t;

    ex = g_akaoSeState.unk0 | g_akaoSeState.unk10 | g_akaoStreamVoices;
    keyon = 0;
    if ((g_akaoSeq->unk4 & g_akaoSeq->unk10) ||
        (g_akaoSubSeq != NULL && (g_akaoSubSeq->unk4 & g_akaoSubSeq->unk10))) {
        akao_readEnvelopes(ex);
    }
    if (g_akaoSubSeq != NULL) {
        if (g_akaoFlags.unk4 & 0x100) {
            ((void (*)(Unk80080A10 *, AkaoChannel *))func_80058990)(g_akaoSubSeq, g_akaoSubChannels);
        }
        t = g_akaoSubSeq;
        m1 = t->unk4 & t->unk14 & ~(t->unkC & ex);
        b1 = m1 & t->unkC & ~ex;
        g_akaoSeq = t;
        if (m1 & g_akaoSeq->unk8) {
            akao_assignVoices(g_akaoSubChannels, m1 & g_akaoSeq->unk8, b1, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m1 &= ~g_akaoSeq->unk8;
        }
        s = &g_akaoMainSeq;
        g_akaoSeq = s;
        m2 = s->unk4 & s->unk14 & ~(s->unkC & (b1 | ex));
        b2 = m2 & s->unkC & ~(b1 | ex);
        if (m2 & g_akaoSeq->unk8) {
            akao_assignVoices(g_akaoChannels, m2 & g_akaoSeq->unk8, b2, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m1) {
            g_akaoSeq = g_akaoSubSeq;
            b1 &= ~b2;
            akao_assignVoices(g_akaoSubChannels, m1, b1, &keyon);
            g_akaoSeq->unk10 = 0;
            g_akaoSeq = s;
        }
        if (m2) {
            akao_assignVoices(g_akaoChannels, m2, b2, &keyon);
            g_akaoSeq->unk10 = 0;
        }
        if (g_akaoFlags.unk4 & 0x100) {
            if (m2) {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(g_akaoSubSeq, g_akaoSubChannels);
            } else {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(g_akaoSubSeq, g_akaoSubChannels);
            }
        }
    } else {
        m2 = g_akaoSeq->unk4 & g_akaoSeq->unk14 & ~(g_akaoSeq->unkC & ex);
        bit = m2 & g_akaoSeq->unkC & ~ex;
        if (m2 & g_akaoSeq->unk8) {
            akao_assignVoices(g_akaoChannels, m2 & g_akaoSeq->unk8, bit, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m2) {
            akao_assignVoices(g_akaoChannels, m2, bit, &keyon);
            g_akaoSeq->unk10 = 0;
        }
    }
    m2 = g_akaoSeState.unk0 & g_akaoSeState.unk8;
    if (m2) {
        keyon |= g_akaoSeState.unk4;
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
        D_80080A74 = 0;
    }
    m2 = g_akaoFlags.unk8;
    if (m2 & 0x80) {
        SpuSetReverbModeDepth(g_akaoSeq->unk48 >> 12, g_akaoSeq->unk48 >> 12);
        g_akaoFlags.unk8 &= ~0x80;
    }
    if (m2 & 0x10) {
        SpuSetNoiseClock(g_akaoSeState.unk0 ? g_akaoSeState.unk28 : g_akaoSeq->unk6E);
        g_akaoFlags.unk8 &= ~0x10;
    }
    if (m2 & 0x100) {
        func_80059140((u32 *)&D_80083238[1], g_akaoSubSeq->unk3C, g_akaoSeq->unk3C, g_akaoSeState.unk1C);
        func_80059140((u32 *)&D_80083238[0], g_akaoSubSeq->unk40, g_akaoSeq->unk40, g_akaoSeState.unk20);
        func_80059140((u32 *)&D_80083238[2], g_akaoSubSeq->unk44, g_akaoSeq->unk44, g_akaoSeState.unk24);
        akao_setReverbVoices(D_80083238[0]);
        akao_setNoiseVoices(D_80083238[1]);
        akao_setPitchModVoices(D_80083238[2]);
        g_akaoFlags.unk8 &= ~0x100;
    }
    if (keyon) {
        akao_keyOn(keyon);
    }
}

void func_80058F34(AkaoChannel *c, u32 *out, u32 mask, u32 keep) {
    u32 bit = 1;
    do {
        if (mask & bit) {
            if (c->voice < 0x18) {
                *out |= 1 << c->voice;
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
    keep = ~((g_akaoSeState.unk0 | g_akaoSeState.unk10) | g_akaoStreamVoices);
    if (g_akaoSubSeq != NULL) {
        a = g_akaoSubSeq->unk18;
        m = a & g_akaoSubSeq->unk8;
        if (m != 0) {
            func_80058F34(g_akaoSubChannels, &out, m, keep);
            g_akaoSubSeq->unk18 &= ~g_akaoSubSeq->unk8;
            a &= ~g_akaoSubSeq->unk8;
        }
    }
    b = g_akaoSeq->unk18;
    m = b & g_akaoSeq->unk8;
    if (m != 0) {
        func_80058F34(g_akaoChannels, &out, m, keep);
        g_akaoSeq->unk18 &= ~g_akaoSeq->unk8;
        b &= ~g_akaoSeq->unk8;
    }
    if (g_akaoSubSeq != NULL && a != 0) {
        func_80058F34(g_akaoSubChannels, &out, a, keep);
        g_akaoSubSeq->unk18 = 0;
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &out, b, keep);
        g_akaoSeq->unk18 = 0;
    }
    out |= g_akaoSeState.unkC;
    g_akaoSeState.unkC = 0;
    if (out != 0) {
        akao_keyOff(out);
    }
}

void func_80059140(u32 *out, u32 m1, u32 m2, u32 extra) {
    u32 a;
    u32 r;
    u32 keep;
    u32 b;
    Unk80080A10 *s;

    keep = ~((g_akaoSeState.unk0 | g_akaoSeState.unk10) | g_akaoStreamVoices);
    a = 0;
    r = 0;
    s = g_akaoSubSeq;
    if (s != NULL) {
        a = s->unk4 & m1;
        if (a & s->unk8) {
            func_80058F34(g_akaoSubChannels, &r, a & s->unk8, keep);
            a &= ~g_akaoSubSeq->unk8;
        }
    }
    b = g_akaoSeq->unk4 & m2;
    if (b & g_akaoSeq->unk8) {
        func_80058F34(g_akaoChannels, &r, b & g_akaoSeq->unk8, keep);
        b &= ~g_akaoSeq->unk8;
    }
    if (g_akaoSubSeq != NULL && a != 0) {
        func_80058F34(g_akaoSubChannels, &r, a, keep);
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &r, b, keep);
    }
    *out = r |= extra;
    g_akaoFlags.unk8 |= 0x100;
}

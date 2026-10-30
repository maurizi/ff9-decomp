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
    u32 mask;
    Unk80083168 *s;
    s32 old;
    s32 k;

    D_8008314C = (D_8008314C + 1) & 0xFF;
    if (g_akaoCdVolumeFadeTicks != 0) {
        g_akaoCdVolumeFadeTicks--;
        g_akaoCdVolume += g_akaoCdVolumeStep;
        akao_applyCdVolume();
    }
    s = &g_akaoStream;
    if (s->unkC != 0 && s->unk48 != 0) {
        s->unk48--;
        old = s->unk40;
        v = old + s->unk44;
        if ((v & 0xFF00) != (old & 0xFF00)) {
            if (g_akaoOutputFlags & 2) {
                old = (old * g_akaoMonoGain) >> 16;
                akao_setVoiceVolume(s->unk10, old, old, 0);
                akao_setVoiceVolume(s->unk10 + 1, old, old, 0);
            } else {
                old = v << 15;
                old >>= 16;
                akao_setVoiceVolume(s->unk10, old, 0, 0);
                akao_setVoiceVolume(s->unk10 + 1, 0, old, 0);
            }
        }
        g_akaoSeVolume = v & 0xFFFF;
    }
    if (D_8007F96E != 0) {
        D_8007F96E--;
        D_80083154 += D_8007F8E8;
    }
    if (D_8007F96C != 0) {
        D_8007F96C--;
        v = D_80083150 + D_8007F8E4;
        if ((v & 0xFF0000) != (D_80083150 & 0xFF0000)) {
            k = 32;
            c = g_akaoChannels;
            do {
                k--;
                c->voiceDirty |= 0x10;
                c++;
            } while (k != 0);
        }
        D_80083150 = v;
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
        mask = g_akaoSeState.activeMask;
        c = g_akaoSeChannels;
        k = 0x100;
        do {
            if (mask & k) {
                if ((u16)c->seVolumeSlideLen != 0) {
                    c->seVolumeSlideLen = (u16)c->seVolumeSlideLen - 1;
                    v = (s16)c->seVolume + c->seVolumeStep;
                    if ((v & 0xFF00) != ((s16)c->seVolume & 0xFF00)) {
                        c->voiceDirty |= 3;
                    }
                    c->seVolume = v;
                }
                if (c->sePanSlideLen != 0) {
                    c->sePanSlideLen--;
                    v = c->sePan + c->sePanStep;
                    if ((v & 0xFF00) != (c->sePan & 0xFF00)) {
                        c->voiceDirty |= 3;
                    }
                    c->sePan = v;
                }
                if ((u16)c->sePitchSlideLen != 0) {
                    c->sePitchSlideLen = (u16)c->sePitchSlideLen - 1;
                    v = c->sePitch + c->sePitchStep;
                    if ((v & 0xFF00) != (c->sePitch & 0xFF00)) {
                        c->voiceDirty |= 0x10;
                    }
                    c->sePitch = v;
                }
                /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                if (v) {
                    mask ^= k;
                } else {
                    mask ^= k;
                }
            }
            k <<= 1;
            c++;
        } while (mask != 0);
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
    m = D_80083156;
    if (m != 0) {
        if (m < 0x80) {
            tempo += (tempo * m) >> 7;
        } else {
            tempo = (tempo * m) >> 8;
        }
    }
    acc = g_akaoSeq->tempoAcc + tempo;
    g_akaoSeq->tempoAcc = acc;
    if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
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
                    flags = g_akaoFlags.unk8;
                    flags |= 0x80;
                    if (sub == 0) {
                        g_akaoFlags.unk8 = flags;
                    }
                }
                if (g_akaoSeq->ticksPerBeat != 0) {
                    if (++g_akaoSeq->tick == g_akaoSeq->ticksPerBeat) {
                        g_akaoSeq->tick = 0;
                        if (++g_akaoSeq->beat == g_akaoSeq->beatsPerMeasure) {
                            g_akaoSeq->beat = 0;
                            g_akaoSeq->measure++;
                            if (sub == 0 && D_800809B8 != 0) {
                                D_800809B8--;
                            }
                        }
                    }
                }
                if (sub != 0 || D_800809B8 == 0) {
                    break;
                }
            }
        } while (0);
    }
    return g_akaoSeq->activeMask;
}

void func_8005D628(AkaoSeq *arg0, AkaoChannel *arg1) {
    s32 t;

    if (D_80080A18.unk8 != 0) {
        if (--D_80080A18.unk8 == 0) {
            s32 v = arg0->activeMask;
            arg0->musicId = 0;
            arg0->activeMask = 0;
            arg0->keyOnMask = 0;
            arg0->playingMask = 0;
            arg0->keyOffMask = v;
            return;
        }
        t = D_80080A18.unk0 + D_80080A18.unk4;
        if ((t & 0xFFFF0000) != (D_80080A18.unk0 & 0xFFFF0000)) {
            akao_markSeqDirty(arg0, arg1, arg0);
        }
        D_80080A18.unk0 = t;
    }
}

s32 akao_tick(void) {
    s32 t0;
    s32 n;
    u32 now;
    AkaoSeq *tmp;
    AkaoChannel *c;
    u32 mask;
    u32 bit;
    u32 acc;
    s32 a;
    s32 b;
    s32 e;

    t0 = GetRCnt(0xF2000002);
    D_8007E3C8[1] = D_8007E3C8[0];
    now = VSync(-1);
    n = 1;
    mask = now - D_8007E3C8[1];
    D_8007E3C8[0] = now;
    if (mask != 0) {
        if (mask < 9) {
            bit = mask * 4;
            if (D_8007E3D0 < bit) {
                n = bit - D_8007E3D0 + 1;
            }
            D_8007E3D0 %= bit;
        }
    }
    do {
        n--;
        D_8007E3D0++;
        g_akaoTickCount++;
        if ((g_akaoSeq->keyOffMask | g_akaoSeState.keyOffMask) != 0 || (g_akaoSubSeq != NULL && g_akaoSubSeq->keyOffMask != 0)) {
            func_80058F94();
        }
        if (g_akaoSubSeq != NULL) {
            if (g_akaoSubSeq->activeMask == 0) {
                g_akaoSubSeq = NULL;
            } else if ((g_akaoSeq->activeMask | g_akaoSeq->pausedMask) == 0) {
                akao_copy(g_akaoSubSeq, g_akaoSeq, 0x7C);
                akao_copy(g_akaoSubChannels, g_akaoChannels, 0x2680);
                tmp = g_akaoSubSeq;
                g_akaoSubSeq = NULL;
                tmp->musicId = 0;
                tmp->activeMask = 0;
            }
        }
        if ((g_akaoFlags.unk8 | g_akaoSeq->playingMask | g_akaoSeState.playingMask) != 0 || (g_akaoSubSeq != NULL && g_akaoSubSeq->playingMask != 0)) {
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
            if ((g_akaoFlags.unk4 & 0x100) && !(g_akaoTickCount & 3)) {
                func_8005D628(g_akaoSubSeq, g_akaoSubChannels);
                if (g_akaoSubSeq->activeMask == 0) {
                    g_akaoFlags.unk4 &= ~0x100;
                }
            }
        }
        if (g_akaoSeState.activeMask != 0) {
            mask = g_akaoSeState.activeMask;
            acc = g_akaoSeState.tempoAcc + g_akaoSeState.tempo.h[1];
            g_akaoSeState.tempoAcc = acc;
            if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
                g_akaoSeState.tempoAcc = acc & 0xFFFF;
                bit = 0x100;
                c = g_akaoSeChannels;
                do {
                    if (mask & bit) {
                        if (!(D_800831C4 & 2) || (c->seMask & 0x2000000)) {
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
                        mask ^= bit;
                    }
                    c++;
                    bit <<= 1;
                } while (mask != 0);
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
s32 func_8005DB40(AkaoChannel *c) {
    u8 *p;
    u32 n;
    u32 op;
    s32 len;

    p = c->pc;
    n = c->loopDepth;
    for (;;) {
        if (*p < 0x9A) {
            if (*p >= 0x8F) {
                c->legato = (u16)c->legato & ~5;
            }
            return *p;
        }
        if (*p < 0xA0) {
            return 0xA0;
        }
        op = *p;
        len = D_8006F374[op - 0xA0];
        if (len != 0) {
            p += len;
            continue;
        }
        switch (op) {
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
            return 0x83;
        case 0xFC:
            return 0x84;
        case 0xFD:
            return 0x8F;
        case 0xFE:
            p++;
            op = *p;
            len = D_8006F3D4[op];
            if (len != 0) {
                p += len;
                continue;
            }
            switch (op) {
            case 0x8:
            case 0x9:
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
            case 0x6:
            case 0xE:
                p++;
                op = p[0] + (p[1] << 8);
                p += (s16)op;
                continue;
            case 0x7:
                p++;
                if (*p++ <= g_akaoSeq->condition) {
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 2;
                continue;
            case 0xF:
                p = c->returnAddr;
                continue;
            }
            continue;
        case 0xC9:
            p++;
            if (*p == c->loopCount[n] + 1) {
                p++;
                n--;
                n &= 3;
                continue;
            }
            p = c->loopStart[n];
            continue;
        case 0xCB:
        case 0xCD:
        case 0xD1:
        case 0xDB:
            c->legato = (u16)c->legato & ~5;
            p++;
            continue;
        case 0xCA:
            if (c->flags & 0x200000) {
                goto end;
            }
            p = c->loopStart[n];
            continue;
        default:
        end:
            c->legato = (u16)c->legato & ~5;
            return 0xA0;
        }
    }
}

s32 akao_remapInstrument(s32 a, s32 b) {
    if ((a & 0x20) && (u32)(b - 0x20) < 0x60) {
        return b + 0x30;
    }
    return b;
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
    if (!(f & 0x1000000)) {
        c->voiceAdsr1 = e->attack << 8;
    } else {
        c->voiceAdsr1 &= 0x7F00;
    }
    c->voiceAdsr1 |= p->adsr1 & 0x80FF;
    if (!(f & 0x8000000)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->sustainRate << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->sustainMode) {
    case 3:
        c->voiceAdsr2 |= 0x4000;
        break;
    case 5:
        c->voiceAdsr2 |= 0x8000;
        break;
    case 7:
        c->voiceAdsr2 |= 0xC000;
        break;
    }
    if (!(f & 0x10000000)) {
        c->voiceAdsr2 &= 0xFFE0;
        c->voiceAdsr2 |= e->releaseRate;
    }
    c->voiceAdsr2 |= p->adsr2 & 0x20;
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

s32 func_8005E170(AkaoChannel *c, u32 mask, s32 key) {
    AkaoDrumKey *e;
    AkaoInstrument *p;
    u32 f;
    s32 v;
    s32 r;

    e = (AkaoDrumKey *)g_akaoSeq->drumKit;
    e += key;
    g_akaoSeq->keyOnMask |= mask;
    if (g_akaoSeq->playingMask & mask) {
        g_akaoSeq->keyOffMask |= mask;
    }
    f = c->flags;
    v = akao_remapInstrument(g_akaoSeq->flags, e->sample);
    c->instrument = v;
    p = &g_akaoInstruments[v];
    c->voiceStartAddr = p->startAddr;
    c->voiceLoopAddr = p->loopAddr;
    if (!(f & 0x1000000)) {
        c->voiceAdsr1 = e->attack << 8;
    } else {
        c->voiceAdsr1 &= 0x7F00;
    }
    c->voiceAdsr1 |= p->adsr1 & 0x80FF;
    if (!(f & 0x8000000)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->sustainRate << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->sustainMode) {
    case 3:
        c->voiceAdsr2 |= 0x4000;
        break;
    case 5:
        c->voiceAdsr2 |= 0x8000;
        break;
    case 7:
        c->voiceAdsr2 |= 0xC000;
        break;
    }
    if (!(f & 0x10000000)) {
        c->voiceAdsr2 &= 0xFFE0;
        c->voiceAdsr2 |= e->releaseRate;
    }
    c->voiceAdsr2 |= p->adsr2 & 0x20;
    r = ((s32 (*)())akao_calcPitch)(p, e->note, c->fineTune, &c->fineTuneOfs);
    c->voiceVolScale = e->volume;
    c->pan = ((e->pan & 0x7F) + 0x40) << 8;
    if (e->pan & 0x80) {
        g_akaoSeq->reverbMask |= mask;
    } else {
        g_akaoSeq->reverbMask &= ~mask;
    }
    g_akaoFlags.unk8 |= 0x100;
    return r;
}

void akao_execChannel(AkaoChannel *c, u32 mask) {
    u32 op;
    u32 vel;
    u32 tmp;
    u32 f;
    u16 len;
    u32 v;

    do {
        op = *c->pc++;
        if (op >= 0xA0) {
            if (op == 0xFE) {
                vel = *c->pc++;
                /* Not bounded, as in the original: g_akaoOpsFE has 0x20 handlers, but any byte the sequence holds
                 * indexes it. A channel reading garbage (e.g. a replaced overlay, ff9-ds d1_f0310: 0xFE 0x4F) jumps
                 * through whatever word follows the table; on the PS1 that is data (an address error there).
                 * A port may send indices >= 0x20 to akao_opInvalid. */
                g_akaoOpsFE[vel](c, mask);
            } else if (op - 0xF0 < 14) {
                op = (op - 0xF0) * 11;
                c->noteTicks = *c->pc++;
            } else {
                if (op == 0xFF) {
                    op = 0xA0;
                } else if (op == 0xCA && (c->flags & 0x200000)) {
                    op = 0xA0;
                    g_akaoSeState.keyOffMask |= mask;
                }
                g_akaoOps[op - 0xA0](c, mask);
            }
        }
        c->opCount++;
    } while (op > 0xA0);
    if (op == 0xA0) {
        if (c->isSe == 0) {
            g_akaoSeq->keyOffMask |= mask;
        }
        return;
    }
    vel = func_8005DB40(c) & 0xFF;
    if (c->fixedLen != 0) {
        c->noteTicks = c->gateTicks = c->fixedLen;
    }
    if (c->noteTicks != 0) {
        if (vel >= 0x8F || (vel < 0x84 && !(c->legato & 5))) {
            c->gateTicks -= 2;
        }
    } else {
        c->noteTicks = D_8006F3F4[op % 11];
        len = c->noteTicks;
        if ((vel < 0x84 || vel >= 0x8F) && !(c->legato & 5)) {
            len -= 2;
        }
        c->gateTicks = len;
    }
    if (c->isSe == 0 && (c->flags & 0x40)) {
        c->gateTicks = c->noteTicks;
    }
    c->noteLen = c->noteTicks;
    c->voiceDirty |= 0x4000;
    if (op >= 0x8F) {
        if (c->isSe == 0) {
            g_akaoSeq->playingMask &= ~mask;
            if (c->voice < 0x18) {
                g_akaoSeq->keyOffMask |= mask;
            }
        }
        c->portamentoLen = 0;
        c->vibratoOut = 0;
        c->tremoloOut = 0;
        c->legato &= ~2;
        return;
    }
    if (op < 0x84) {
        op /= 11;
        op += c->octave * 12;
        f = c->flags;
        if (f & 8) {
            vel = func_8005E170(c, mask, op);
        } else {
            if (!(c->legato & 2)) {
                if (c->isSe == 0) {
                    if (f & 0x1000) {
                        akao_applyKeySplit(c, op);
                    }
                    g_akaoSeq->keyOnMask |= mask;
                    if ((g_akaoSeq->playingMask & mask) && c->voice < 0x18) {
                        g_akaoSeq->keyOffMask |= mask;
                    }
                    if (c->noteVolumeSlideLen != 0) {
                        c->volumeSlideLen = c->noteVolumeSlideLen;
                        c->volume = c->noteVolume;
                        c->volumeStep = c->noteVolumeStep;
                    }
                } else {
                    g_akaoSeState.keyOnMask |= mask;
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
                tmp *= (u8)g_akaoLfoTableData[D_8008314C];
                if ((u8)g_akaoLfoTableData[D_8008314C] & 0x80) {
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
            g_akaoSeq->playingMask |= mask;
        } else {
            g_akaoSeState.playingMask |= mask;
        }
        op = c->flags;
        c->voiceDirty |= 0x13;
        if (op & 1) {
            v = (c->vibratoDepthParam & 0x7F00) >> 8;
            if (!(c->vibratoDepthParam & 0x8000)) {
                v = (v * ((u32)(vel * 15) >> 8)) >> 7;
            } else {
                v = (v * vel) >> 7;
            }
            c->vibratoDepth = v;
            if (!(c->legato & 2)) {
                c->vibratoPhase = 0;
                c->vibratoDelayLeft = c->vibratoDelay;
                c->vibratoRate = 0x1000000 / ((u32)c->vibratoPeriod >> 12);
            }
        }
        if ((op & 2) && !(c->legato & 2)) {
            c->tremoloPhase = 0;
            c->tremoloDelayLeft = c->tremoloDelay;
            c->tremoloRate = 0x1000000 / ((u32)c->tremoloPeriod >> 12);
        }
        c->vibratoOut = 0;
        c->tremoloOut = 0;
        c->pitchSlide = 0;
    }
    c->legato = (c->legato & ~2) | ((c->legato & 1) << 1);
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

s32 func_8005EB40(s32 *p, s32 target, s32 steps, s32 shift) {
    s32 mask = (1 << shift) - 1;
    s32 cur = *p & ~mask;
    s32 step;
    target <<= shift;
    step = target - cur;
    step = step / steps;
    *p = cur;
    if (target < cur) {
        *p = cur | mask;
        step--;
    } else {
        step++;
    }
    return step;
}

void akao_setChannelInstrument(AkaoChannel *ch, AkaoInstrument *p, s32 arg2) {
    ch->voiceStartAddr = arg2;
    ch->voiceLoopAddr = p->loopAddr;
    ch->voiceAdsr1 = p->adsr1;
    ch->voiceAdsr2 = p->adsr2;
    ch->voiceDirty |= 0x1FF80;
}

void akao_setChannelProgram(AkaoChannel *c, s32 i) {
    c->instrument = i;
    akao_setChannelInstrument(c, &g_akaoInstruments[i], g_akaoInstruments[i].startAddr);
}

void akao_stopSeChannel(AkaoChannel *c, u32 mask) {
    g_akaoSeState.activeMask &= ~mask;
    g_akaoSeState.pausedMask &= ~mask;
    g_akaoSeState.noiseMask &= ~mask;
    g_akaoSeState.reverbMask &= ~mask;
    g_akaoSeState.pitchModMask &= ~mask;
    g_akaoSeState.keyOnMask &= ~mask;
    g_akaoSeState.playingMask &= ~mask;
    c->seMask = 0;
    c->seHandle = 0;
}

s32 func_8005EC8C(s32 a, s32 b) {
    if (a != 0) {
        if ((u32)(b - 0xC0) < 0x20) {
            return b + a * 8;
        }
        if ((u32)(b - 0xE0) < 0x18) {
            return b + (a - 4) * 8;
        }
    }
    return b;
}

void akao_opEnd(AkaoChannel *c, s32 mask) {
    AkaoSeq *t;

    if (c->isSe == 0) {
        t = g_akaoSeq;
        if ((t->activeMask &= ~mask) == 0) {
            D_800809B8 = 0;
            t->musicId = 0;
            t->flags = 0;
        }
        g_akaoSeq->playingMask &= ~mask;
        g_akaoSeq->priorityMask &= ~mask;
        g_akaoSeq->keepVoiceMask &= ~mask;
        g_akaoSeq->noiseMask &= ~mask;
        g_akaoSeq->reverbMask &= ~mask;
        g_akaoSeq->pitchModMask &= ~mask;
    } else {
        akao_stopSeChannel(c, mask);
    }
    c->flags = 0;
    g_akaoFlags.unk8 |= 0x110;
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
    g_akaoFlags.unk8 |= 0x80;
    a->reverbDepth = v;
}

void akao_opFE03(AkaoChannel *arg0) {
    u8 *p;
    s32 v;

    g_akaoSeq->reverbDepthSlideLen = *arg0->pc++;
    if (g_akaoSeq->reverbDepthSlideLen == 0) {
        g_akaoSeq->reverbDepthSlideLen = 0x100;
    }
    p = arg0->pc;
    v = ((s8)p[1] << 20) | (p[0] << 12);
    arg0->pc = p + 2;
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

void akao_opFEReturn(Unk8005F038 *a) {
    a->unk0 = a->unk14;
}

void akao_opA3(AkaoChannel *c) {
    c->expression = *c->pc++ << 8;
    c->voiceDirty |= 3;
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
    c->voiceDirty |= 3;
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
    c->flags |= 0x40;
}

void akao_opFE1B(AkaoChannel *c) {
    c->flags &= ~0x40;
}

void akao_opAA(AkaoChannel *ch) {
    u8 *p = ch->pc;
    s32 v = *p;
    ch->pc = p + 1;
    ch->panSlideLen = 0;
    ch->voiceDirty |= 3;
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

void akao_opProgram(AkaoChannel *arg0) {
    s32 i;
    AkaoInstrument *e;
    s32 b = *arg0->pc++;

    if (arg0->isSe == 0) {
        i = akao_remapInstrument(g_akaoSeq->flags, b);
    } else {
        i = func_8005EC8C(arg0->seBank, b);
    }
    e = &g_akaoInstruments[i];
    akao_setChannelInstrument(arg0, e, e->startAddr);
    arg0->instrument = i;
    arg0->voiceVolScale = 0;
    arg0->flags &= 0xE6FFEFF7;
}

void akao_opFE0A(AkaoChannel *c) {
    s32 i = akao_remapInstrument(g_akaoSeq->flags, *c->pc++);
    akao_setChannelInstrument(c, &g_akaoInstruments[i], 0x1010);
    c->instrument = i;
    c->voiceVolScale = 0;
    c->flags &= 0xE6FFEFF7;
}

void akao_opFE14(AkaoChannel *c) {
    u8 v = *c->pc++;
    u16 *tbl = g_akaoSeq->regionTable;
    if (tbl != NULL) {
        if (tbl[v] > 0x8000) {
            c->voiceVolScale = 0;
            c->flags &= ~0x1000;
        } else {
            u8 *p = (u8 *)tbl + tbl[v];
            c->curKey = 0xFF;
            c->regions = p + 0x20;
            c->flags = (c->flags & 0xE6FFEFF7) | 0x1000;
        }
    }
}

void akao_opB3(AkaoChannel *c) {
    AkaoInstrument *e = &g_akaoInstruments[c->instrument];
    c->voiceAdsr1 = e->adsr1;
    c->voiceAdsr2 = e->adsr2;
    c->voiceDirty |= 0xFF00;
    c->flags &= 0xE6FFFFFF;
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
    c->legato = 1;
}

void akao_opDB(AkaoChannel *a) {
    a->portamentoLen = 0;
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
    c->voiceDirty |= 0x10;
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
    c->voiceDirty |= 0x10;
}

void akao_opB4(AkaoChannel *c) {
    u32 b;
    s16 v;
    u32 n;
    u32 r;
    u32 k;
    u32 t;

    c->flags |= 1;
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
    if (!(v & 0x8000)) {
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
    if (!(v & 0x8000)) {
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
    c->vibratoPeriodStep = func_8005EB40(&c->vibratoPeriod, *c->pc++, v, 0xC);
}

void akao_opB6(AkaoChannel *c) {
    c->vibratoOut = 0;
    c->flags &= ~1;
    c->voiceDirty |= 0x10;
}

void akao_opB8(AkaoChannel *c) {
    s32 v;
    u32 d;

    c->flags |= 2;
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
    c->tremoloPeriodStep = func_8005EB40(&c->tremoloPeriod, *c->pc++, v, 0xC);
}

void akao_opBA(AkaoChannel *c) {
    c->tremoloOut = 0;
    c->flags &= ~2;
    c->voiceDirty |= 3;
}

void akao_opBC(AkaoChannel *arg0) {
    arg0->flags |= 4;
    arg0->panLfoPeriod = *arg0->pc++ << 12;
    if (arg0->panLfoPeriod == 0) {
        arg0->panLfoPeriod = 0x100000;
    }
    {
        u32 d = 0x1000000 / ((u32)arg0->panLfoPeriod >> 12);
        u8 b = *arg0->pc++;
        arg0->panLfoPeriodSlideLen = 0;
        arg0->panLfoPhase = 0;
        arg0->panLfoRate = d;
        arg0->panLfoWave = g_akaoLfoWaves[b & 7];
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
    c->panLfoPeriodStep = func_8005EB40(&c->panLfoPeriod, *c->pc++, v, 0xC);
}

void akao_opBE(AkaoChannel *c) {
    c->panLfoOut = 0;
    c->flags &= ~4;
    c->voiceDirty |= 3;
}

void akao_opC4(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->noiseMask |= mask;
    } else {
        g_akaoSeState.noiseMask |= mask;
    }
    g_akaoFlags.unk8 |= 0x110;
}

void akao_opC5(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->noiseMask &= ~mask;
    } else {
        g_akaoSeState.noiseMask &= ~mask;
    }
    g_akaoFlags.unk8 |= 0x110;
    c->noiseTimer = 0;
}

void akao_opC6(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->pitchModMask |= mask;
    } else if (c->flags & 0x10000) {
        g_akaoSeState.pitchModMask |= mask;
    }
    g_akaoFlags.unk8 |= 0x100;
}

void akao_opC7(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->pitchModMask &= ~mask;
    } else {
        g_akaoSeState.pitchModMask &= ~mask;
    }
    g_akaoFlags.unk8 |= 0x100;
    c->pitchModTimer = 0;
}

void akao_opC2(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->reverbMask |= mask;
    } else {
        g_akaoSeState.reverbMask |= mask;
    }
    g_akaoFlags.unk8 |= 0x100;
}

void akao_opC3(AkaoChannel *c, u32 mask) {
    if (c->isSe == 0) {
        g_akaoSeq->reverbMask &= ~mask;
    } else {
        g_akaoSeState.reverbMask &= ~mask;
    }
    g_akaoFlags.unk8 |= 0x100;
}

void akao_opCC(AkaoChannel *a) {
    a->legato = 1;
}

void akao_opCD(void) {
}

void akao_opD0(AkaoChannel *c) {
    if (c->isSe != 0) {
        c->legato = 4;
    }
}

void akao_opD1(void) {
}

void akao_opAC(AkaoChannel *arg0) {
    u32 b = *arg0->pc++;

    if (arg0->isSe == 0) {
        if (b & 0xC0) {
            g_akaoSeq->noiseClock = (g_akaoSeq->noiseClock + (b & 0x3F)) & 0x3F;
        } else {
            g_akaoSeq->noiseClock = b;
        }
    } else if (b & 0xC0) {
        g_akaoSeState.noiseClock = (g_akaoSeState.noiseClock + (b & 0x3F)) & 0x3F;
    } else {
        D_80080A98 = b;
    }
    g_akaoFlags.unk8 |= 0x10;
}

void akao_opAD(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= 0x900;
    c->flags |= 0x1000000;
    c->voiceAdsr1 = (c->voiceAdsr1 & 0x80FF) | (v << 8);
}

void akao_opAE(AkaoChannel *c, s32 arg1) {
    u16 t = c->voiceAdsr1 & 0xFF0F;
    c->voiceAdsr1 = t | (*c->pc++ << 4);
    c->voiceDirty |= 0x1000;
}

void akao_opAF(AkaoChannel *c, s32 arg1) {
    u16 t = c->voiceAdsr1 & 0xFFF0;
    c->voiceAdsr1 = t | *c->pc++;
    c->voiceDirty |= 0x8000;
}

void akao_opB1(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= 0x2200;
    c->flags |= 0x8000000;
    c->voiceAdsr2 = (c->voiceAdsr2 & 0xE03F) | (v << 6);
}

void akao_opB2(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceDirty |= 0x4400;
    c->flags |= 0x10000000;
    c->voiceAdsr2 = (c->voiceAdsr2 & 0xFFE0) | v;
}

void akao_opB7(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceAdsr1 &= 0x7FFF;
    if (v == 5) {
        c->voiceAdsr1 |= 0x8000;
    }
    c->voiceDirty |= 0x100;
}

void akao_opBB(AkaoChannel *c) {
    u16 v = *c->pc++;
    u16 x = c->voiceAdsr2 & 0x3FFF;
    c->voiceAdsr2 = x;
    switch (v) {
    case 3:
        c->voiceAdsr2 = x | 0x4000;
        break;
    case 5:
        c->voiceAdsr2 = x | 0x8000;
        break;
    case 7:
        c->voiceAdsr2 = x | 0xC000;
        break;
    }
    c->voiceDirty |= 0x200;
}

void akao_opBF(AkaoChannel *c) {
    s32 v = *c->pc++;
    c->voiceAdsr2 &= 0xFFDF;
    if (v == 7) {
        c->voiceAdsr2 |= 0x20;
    }
    c->voiceDirty |= 0x400;
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

void akao_opC9(AkaoChannel *arg0) {
    u32 n = *arg0->pc++;

    if (n == 0) {
        n = 0x100;
    }
    if (++arg0->loopCount[arg0->loopDepth] != n) {
        arg0->pc = arg0->loopStart[arg0->loopDepth];
        arg0->opCount = arg0->loopOpCount[arg0->loopDepth];
        return;
    }
    arg0->loopDepth = (arg0->loopDepth - 1) & 3;
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
        c->flags = (c->flags & 0xE6FFEFF7) | 8;
    }
}

void akao_opFE05(AkaoChannel *c) {
    c->voiceVolScale = 0;
    c->flags &= ~8;
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

void akao_opB0(AkaoChannel *c, s32 b) {
    akao_opAE(c, b);
    akao_opAF(c, b);
}

void akao_opCE(AkaoChannel *c, u32 mask) {
    s32 v = *c->pc++;
    c->noiseTimer = v ? v + 1 : 0x101;
    akao_opC4(c, mask);
}

void akao_opCF(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v != 0) {
        c->noiseTimer = v + 1;
    } else {
        c->noiseTimer = 0x101;
    }
}

void akao_opD2(AkaoChannel *c, u32 mask) {
    s32 v = *c->pc++;
    c->pitchModTimer = v ? v + 1 : 0x101;
    akao_opC6(c, mask);
}

void akao_opD3(AkaoChannel *c) {
    s32 v = *c->pc++;
    if (v != 0) {
        c->pitchModTimer = v + 1;
    } else {
        c->pitchModTimer = 0x101;
    }
}

void akao_opCB(AkaoChannel *c, u32 mask) {
    c->flags &= ~0x37;
    akao_opC5(c, mask);
    akao_opC7(c, mask);
    akao_opC3(c, mask);
    c->legato &= ~5;
}

void akao_opD4(AkaoChannel *c) {
    c->flags |= 0x10;
}

void akao_opD5(AkaoChannel *c) {
    c->flags &= ~0x10;
}

void akao_opD6(AkaoChannel *c) {
    c->flags |= 0x20;
}

void akao_opD7(AkaoChannel *c) {
    c->flags &= ~0x20;
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
    D_800809F0.unk0 = 0;
    D_800809F0.unk4 = 0;
    D_800809F0.unk8 = c->pan >> 8;
    D_800809F0.unkC = c->volume >> 23;
    akao_startSe(&D_800809F0, a, b, 0);
    c->pc += 4;
}

void akao_opE0(AkaoChannel *c) {
    c->flags |= 0x100000;
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
    if (g_akaoStream.unkC != 0) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        akao_keyOff(g_akaoStream.unkC);
        if (g_akaoStream.unk8 & 0x400) {
            akao_setVoiceLoopAddr(g_akaoStream.unk10 - 2, 0x1030);
            akao_setVoiceLoopAddr(g_akaoStream.unk10 - 1, 0x1030);
        }
        akao_setVoiceLoopAddr(g_akaoStream.unk10, 0x1030);
        akao_setVoiceLoopAddr(g_akaoStream.unk10 + 1, 0x1030);
        m = g_akaoStream.unkC;
        g_akaoStream.unkC = 0;
        g_akaoSeState.reverbMask &= ~m;
        g_akaoFlags.unk8 |= 0x100;
    }
}

s32 akao_allocStreamVoices(void) {
    u32 mask;
    s32 i;
    u32 bit;

    do {
        bit = 0xC00000;
        mask = g_akaoSeState.activeMask | g_akaoSeState.pausedMask;
        for (i = 15; i != 0; i--, bit >>= 1) {
            if (!(mask & bit)) {
                break;
            }
        }
        if (i != 0) {
            return i + 7;
        }
        akao_stopSeChannels(0, 0x40000000);
    } while (mask != (g_akaoSeState.activeMask | g_akaoSeState.pausedMask));
    return -1;
}

void akao_streamUploadSecond(void) {
    u8 *p = g_akaoStream.unk0 + 0x800;
    SpuSetTransferStartAddr(0x2100);
    SpuSetTransferCallback(akao_streamStartMono);
    SpuWrite(p, 0x800);
}

void akao_streamStart(s32 addr, s32 vol, s32 flag) {
    StreamHeader *h;
    s32 v;
    s32 mask;
    s32 size;
    s32 t;

    v = akao_allocStreamVoices();
    if (v == -1) {
        return;
    }
    g_akaoStream.unk4C = vol;
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)addr;
    g_akaoStream.unk0 = (u8 *)h + 0x40;
    g_akaoStream.unk14 = h->unk10;
    mask = (1 << v) | (1 << (v + 1));
    t = h->unk20;
    g_akaoStream.unk10 = v;
    g_akaoStream.unkC = mask;
    g_akaoStream.unk18 = t;
    akao_keyOff(mask);
    g_akaoStream.unk8 = h->unk18;
    g_akaoStream.unk58 = h->unk1C;
    g_akaoStream.unk10 = v;
    g_akaoStream.unkC = mask;
    akao_keyOff(mask);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    g_akaoStream.unk4 = (g_akaoStream.unk8 & 2) ? (s32)(g_akaoStream.unk0 + h->unk14) : 0;
    if (g_akaoStream.unk8 & 1) {
        g_akaoStream.unk1C = (g_akaoStream.unk8 & 2) ? g_akaoStream.unk14 - (h->unk14 >> 1) : 0;
        SpuSetTransferCallback(akao_streamStartStereo);
        size = 0x2000;
    } else {
        g_akaoStream.unk1C = (g_akaoStream.unk8 & 2) ? g_akaoStream.unk14 - h->unk14 : 0;
        SpuSetTransferCallback(akao_streamUploadSecond);
        size = 0x800;
    }
    SpuWrite(g_akaoStream.unk0, size);
    if (flag) {
        g_akaoSeState.reverbMask |= g_akaoStream.unkC;
    } else {
        g_akaoSeState.reverbMask &= ~g_akaoStream.unkC;
    }
    g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
    g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
    g_akaoFlags.unk8 |= 0x100;
}

void akao_streamSetVoice(s32 v, s32 mode, s32 a, s32 b) {
    s16 l;
    s16 r;
    s16 h;
    s32 pan;

    if (g_akaoOutputFlags & 2) {
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
        pan = (u8)(g_akaoStream.unk4C >> 8);
        l = (u32)(g_akaoStream.unk40 * g_akaoPanCurve[pan]) >> 16;
        r = (u32)(g_akaoStream.unk40 * g_akaoPanCurve[pan ^ 0xFF]) >> 16;
    }
    akao_setVoiceVolume(v, l, r, 0);
    akao_setVoicePitch(v, D_800831C0);
    akao_setVoiceStartAddr(v, a);
    akao_setVoiceLoopAddr(v, b);
    akao_setVoiceAttack(v, 0, 1);
    akao_setVoiceDecay(v, 0xF);
    akao_setVoiceSustainLevel(v, 0xF);
    akao_setVoiceSustain(v, 0x7F, 3);
    akao_setVoiceRelease(v, 6, 3);
}

void akao_streamKeyOn(s32 a, s32 b, void (*cb)(void)) {
    if (g_akaoStream.unkC != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.unk14 > 0x1000) {
            g_akaoStream.unk14 -= 0x1000;
            g_akaoStream.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            b = 0x1030;
            g_akaoStream.unk14 = 0;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(g_akaoStream.unkC);
        SpuSetIRQ(1);
    }
}

void akao_streamStartMono(void) {
    akao_streamSetVoice(g_akaoStream.unk10, 0, 0x1100, 0x2100);
    akao_streamSetVoice(g_akaoStream.unk10 + 1, 0, 0x1100, 0x2100);
    akao_streamKeyOn(0x1000, 0x2100, akao_streamRefillMonoA);
}

void akao_streamStartStereo(void) {
    akao_streamSetVoice(g_akaoStream.unk10, 1, 0x1100, 0x2100);
    akao_streamSetVoice(g_akaoStream.unk10 + 1, 2, 0x1900, 0x2900);
    akao_streamKeyOn(0x2000, 0x2100, akao_streamRefillStereoA);
}

void akao_streamRefill(s32 a, s32 b, s32 n, void (*cb)(void)) {
    if (g_akaoStream.unkC == 0 || g_akaoStream.unk14 == 0) {
        return;
    }
    SpuSetTransferStartAddr(a);
    akao_beginSpuTransfer();
    SpuWrite(g_akaoStream.unk0, n);
    SpuSetIRQ(0);
    if (g_akaoStream.unk14 > 0x800) {
        SpuSetIRQCallback(cb);
        g_akaoStream.unk14 -= 0x800;
        g_akaoStream.unk0 += n;
    } else if (g_akaoStream.unk4 != NULL) {
        SpuSetIRQCallback(cb);
        g_akaoStream.unk0 = g_akaoStream.unk4;
        g_akaoStream.unk14 = g_akaoStream.unk1C;
    } else {
        SpuSetIRQCallback(akao_streamStop);
        b = 0x1030;
        a = b;
        g_akaoStream.unk14 = 0;
    }
    akao_setVoiceLoopAddr(g_akaoStream.unk10, a);
    akao_setVoiceLoopAddr(g_akaoStream.unk10 + 1, b);
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

void akao_cmdE0(s32 *a) {
    akao_streamStop();
    akao_streamStart(a[0], a[1], a[2]);
    g_akaoSeState.activeMask &= ~g_akaoStream.unkC;
}

void akao_cmdE2(void) {
    akao_streamStop();
}

void akao_cmdE4(s32 *a) {
    Unk80083168 *d = &g_akaoStream;
    s32 v = *a;
    s32 l;
    s32 r;
    s32 pan;

    d->unk48 = 0;
    d->unk40 = v;
    if (d->unkC == 0) {
        return;
    }
    if (g_akaoOutputFlags & 2) {
        v = v * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(d->unk10, v, v, 0);
        akao_setVoiceVolume(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v <<= 15;
        v >>= 16;
        akao_setVoiceVolume(d->unk10, v, 0, 0);
        akao_setVoiceVolume(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (u8)(d->unk4C >> 8);
        l = v * g_akaoPanCurve[pan] >> 16;
        r = v * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->unk10, l, r, 0);
        akao_setVoiceVolume(d->unk10 + 1, l, r, 0);
    }
}

void akao_cmdE5(Unk80080AA0 *p) {
    s16 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    g_akaoStream.unk44 = (s16)((s16)((u16)p->unk4 - (u16)g_akaoStream.unk40) / n);
    g_akaoStream.unk48 = n;
}

void akao_cmdE6(s32 *a) {
    Unk80083168 *d = &g_akaoStream;
    s32 p = *a;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    d->unk4C = p;
    if (d->unkC == 0) {
        return;
    }
    if (g_akaoOutputFlags & 2) {
        v = d->unk40 * g_akaoMonoGain >> 16;
        akao_setVoiceVolume(d->unk10, v, v, 0);
        akao_setVoiceVolume(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v = d->unk40;
        v <<= 15;
        v >>= 16;
        akao_setVoiceVolume(d->unk10, v, 0, 0);
        akao_setVoiceVolume(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (p >> 8) & 0xFF;
        l = d->unk40 * g_akaoPanCurve[pan] >> 16;
        r = d->unk40 * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->unk10, l, r, 0);
        akao_setVoiceVolume(d->unk10 + 1, l, r, 0);
    }
}

void func_800615F8(void) {
    SpuSetIRQAddr(0x1038);
    SpuSetIRQCallback(akao_streamStop);
}

void func_80061624(void) {
    u32 irq;
    if (g_akaoStream.unk14 > 0x2000) {
        SpuSetTransferStartAddr(g_akaoStream.unk2C + 0x2000);
        akao_spuWrite(g_akaoStream.unk0, g_akaoStream.unk14 - 0x2000);
        irq = g_akaoStream.unk2C + 0x1FF8;
    } else {
        irq = g_akaoStream.unk2C + (g_akaoStream.unk14 >> 1) + 8;
    }
    akao_streamSetVoice(g_akaoStream.unk10, 0, g_akaoStream.unk2C, 0x1030);
    akao_streamSetVoice(g_akaoStream.unk10 + 1, 0, g_akaoStream.unk2C, 0x1030);
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    akao_keyOn(g_akaoStream.unkC);
    SpuSetIRQ(1);
}

void func_800616F8(s32 arg0, s32 b, s32 addr, s32 loop) {
    AkaoStream *h = (AkaoStream *)arg0;
    Unk80083168 *d;
    s32 v;
    u32 n;
    u32 m;

    v = akao_allocStreamVoices();
    if (v != -1) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        d = &g_akaoStream;
        d->unk4C = b;
        g_akaoStream.unk0 = h->unk40;
        d->unk14 = h->unk10;
        d->unk18 = h->unk20;
        m = (1 << v) | (1 << (v + 1));
        n = d->unkC;
        d->unk10 = v;
        d->unkC = m;
        akao_keyOff(m | n);
        d->unk8 = h->unk18;
        d->unk58 = h->unk1C;
        d->unk2C = addr;
        SpuSetTransferMode(0);
        SpuSetTransferStartAddr(addr);
        g_akaoSpuBusy = 1;
        SpuSetTransferCallback(func_80061624);
        n = d->unk14;
        if (n > 0x2000) {
            n = 0x2000;
        }
        SpuWrite(g_akaoStream.unk0, n);
        g_akaoStream.unk0 += n;
        if (loop) {
            g_akaoSeState.reverbMask |= d->unkC;
        } else {
            g_akaoSeState.reverbMask &= ~d->unkC;
        }
        g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
        g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
        g_akaoFlags.unk8 |= 0x100;
    }
}

void func_800618C0(s32 unused, s32 flag) {
    s32 *h = D_8007F720;
    s32 *h2;
    s32 v;
    s32 mask;
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
    mask = (1 << v) | (1 << (v + 1));
    old = g_akaoStream.unkC;
    g_akaoStream.unk10 = v;
    g_akaoStream.unkC = mask;
    akao_keyOff(mask | old);
    h2 = h;
    g_akaoStream.unk8 = h2[6];
    g_akaoStream.unk58 = (u16)h2[7];
    if (flag) {
        g_akaoSeState.reverbMask |= g_akaoStream.unkC;
    } else {
        g_akaoSeState.reverbMask &= ~g_akaoStream.unkC;
    }
    irq = h2[8] + ((u32)h2[4] >> 1) + 8;
    g_akaoSeState.pitchModMask &= ~g_akaoStream.unkC;
    g_akaoSeState.noiseMask &= ~g_akaoStream.unkC;
    g_akaoFlags.unk8 |= 0x100;
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    if (h2[6] & 1) {
        if (h2[6] & 2) {
            akao_streamSetVoice(g_akaoStream.unk10, 1, h2[8], h2[8] + h2[5]);
            akao_streamSetVoice(g_akaoStream.unk10 + 1, 2, h2[8] + h2[9], h2[8] + h2[9] + h2[5]);
        } else {
            akao_streamSetVoice(g_akaoStream.unk10, 1, h2[8], 0x1030);
            akao_streamSetVoice(g_akaoStream.unk10 + 1, 2, h2[8] + h2[9], 0x1030);
        }
    } else {
        if (h2[6] & 2) {
            akao_streamSetVoice(g_akaoStream.unk10, 3, h2[8], h2[8] + h2[5]);
            akao_streamSetVoice(g_akaoStream.unk10 + 1, 3, h2[8], h2[8] + h2[5]);
        } else {
            akao_streamSetVoice(g_akaoStream.unk10, 3, h2[8], 0x1030);
            akao_streamSetVoice(g_akaoStream.unk10 + 1, 3, h2[8], 0x1030);
        }
    }
    akao_keyOn(g_akaoStreamVoices);
    SpuSetIRQ(1);
}

void akao_cmdEC(s32 *a) {
    akao_streamStop();
    func_800616F8(a[0], a[1], a[2], a[3]);
    g_akaoSeState.activeMask &= ~g_akaoStream.unkC;
}

void akao_cmdED(s32 *a) {
    akao_streamStop();
    func_800618C0(a[0], a[1]);
    g_akaoSeState.activeMask &= ~g_akaoStream.unkC;
}

/* K&R definition: callers pass one or two arguments */
s32 func_80061BC8(p)
    s32 *p;
{
    g_akaoStream.unk28++;
    (*p)++;
    if (g_akaoStream.unk3C - 1 < *p) {
        *p = 0;
    }
    return *p;
}

void func_80061C1C(void) {
    StreamHeader *h;
    Unk80083168 *d;
    s32 v;
    s32 vol;
    s32 mask;

    v = akao_allocStreamVoices();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)(g_akaoStream.unk2C + 0x80);
    if (h->unk28 != 0 && g_akaoStream.unk48 == 0) {
        vol = g_akaoStream.unk40;
        g_akaoStream.unk40 = 0;
        akao_sendCmdE5(h->unk28, vol >> 8);
    }
    d = &g_akaoStream;
    g_akaoStream.unk0 = (u8 *)d->unk2C;
    d->unk14 = h->unk10;
    d->unk18 = h->unk20;
    mask = (1 << v) | (1 << (v + 1));
    d->unk10 = v;
    d->unkC = mask;
    d->unk34 = 0;
    akao_keyOff(mask);
    d->unk8 = h->unk18;
    d->unk58 = h->unk1C;
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    SpuSetTransferCallback(func_80061EE4);
    if (d->unk8 & 0x400) {
        SpuWrite(g_akaoStream.unk0 + 0xD0, 0x4000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        d->unkC |= (1 << (v - 2)) | (1 << (v - 1));
    } else {
        SpuWrite(g_akaoStream.unk0 + 0xD0, 0x2000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
    }
    g_akaoSeState.reverbMask &= ~g_akaoStreamVoices;
    g_akaoSeState.pitchModMask &= ~g_akaoStreamVoices;
    g_akaoSeState.noiseMask &= ~g_akaoStreamVoices;
    g_akaoFlags.unk8 |= 0x100;
}

void func_80061E1C(s32 a, s32 b, void (*cb)(void)) {
    if (g_akaoStream.unkC != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (g_akaoStream.unk14 >= 0xE61) {
            g_akaoStream.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(akao_streamStop);
            b = 0x1030;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(g_akaoStream.unkC);
        SpuSetIRQ(1);
    }
}

void func_80061EE4(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.unk8 & 0x400) {
        akao_streamSetVoice(g_akaoStream.unk10, 1, 0x1100, 0x3100);
        akao_streamSetVoice(g_akaoStream.unk10 + 1, 2, 0x1900, 0x3900);
        akao_streamSetVoice(g_akaoStream.unk10 - 2, 1, 0x2100, 0x4100);
        akao_streamSetVoice(g_akaoStream.unk10 - 1, 2, 0x2900, 0x4900);
        a = 0x4000;
        b = 0x3100;
    } else {
        akao_streamSetVoice(g_akaoStream.unk10, 1, 0x1100, 0x2100);
        akao_streamSetVoice(g_akaoStream.unk10 + 1, 2, 0x1900, 0x2900);
        a = 0x2000;
        b = 0x2100;
    }
    func_80061E1C(a, b, func_80062118);
}

s32 func_80061FBC(s32 a, s32 b, s32 n, void (*cb)(void)) {
    AkaoStream *h;

    if (g_akaoStream.unkC != 0) {
        h = (AkaoStream *)(g_akaoStream.unk0 + 0x80);
        if (h->unk0 == AKAO_MAGIC) {
            SpuSetIRQ(0);
            SpuSetTransferStartAddr(a);
            akao_beginSpuTransfer();
            func_80061BC8(&g_akaoStream.unk34);
            SpuWrite(g_akaoStream.unk0 + 0xD0, n - 0xD0);
            g_akaoStream.unk20 = h->unk4;
            g_akaoStream.unk18 = h->unk20;
            if (h->unk10 > h->unk20) {
                SpuSetIRQCallback(cb);
                g_akaoStream.unk0 += n;
                if (g_akaoStream.unk34 == 0) {
                    g_akaoStream.unk0 = (u8 *)g_akaoStream.unk2C;
                }
            } else {
                SpuSetIRQCallback(akao_streamStop);
                b = 0x1030;
                a = b;
            }
            akao_setVoiceLoopAddr(g_akaoStream.unk10, a);
            akao_setVoiceLoopAddr(g_akaoStream.unk10 + 1, b);
            SpuSetIRQAddr(a + 8);
            SpuSetIRQ(1);
        }
    }
    return a;
}

void func_80062118(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.unk8 & 0x400) {
        func_80061BC8(&g_akaoStream.unk34);
        a = func_80061FBC(0x1100, 0x1900, 0x2000, func_800621D4);
        b = 0x2900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x2100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.unk10 - 2, a);
        akao_setVoiceLoopAddr(g_akaoStream.unk10 - 1, b);
    } else {
        func_80061FBC(0x1100, 0x1900, 0x1000, func_800621D4);
    }
}

void func_800621D4(void) {
    s32 a;
    s32 b;
    if (g_akaoStream.unk8 & 0x400) {
        func_80061BC8(&g_akaoStream.unk34);
        a = func_80061FBC(0x3100, 0x3900, 0x2000, func_80062118);
        b = 0x4900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x4100;
        }
        akao_setVoiceLoopAddr(g_akaoStream.unk10 - 2, a);
        akao_setVoiceLoopAddr(g_akaoStream.unk10 - 1, b);
    } else {
        func_80061FBC(0x2100, 0x2900, 0x1000, func_80062118);
    }
}

void akao_cmdE8(s32 *a) {
    akao_streamStop();
    g_akaoStream.unk34 = -1;
    g_akaoStream.unk20 = 0;
    g_akaoStream.unk24 = 0;
    g_akaoStream.unk28 = 0;
    g_akaoStream.unk38 = 0;
    g_akaoStream.unk8 = 0x1000000;
    g_akaoStream.unk3C = (u32)g_akaoArgs.unk4 >> 12;
    g_akaoStream.unk2C = a[0];
    g_akaoStream.unk30 = a[1];
}


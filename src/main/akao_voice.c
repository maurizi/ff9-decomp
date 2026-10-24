#include "ff9.h"

s32 func_8005715C(s32 x) {
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

s32 func_80057234(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = func_8005715C(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    return r;
}

static inline s32 func_800574A0_curve(s32 r, u32 x) {
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

s32 func_8005729C(u32 x) {
    s32 v = x & 0xFF;
    s32 r;

    x >>= 8;
    if (!(x & 1)) {
        v = 0x100 - v;
    }
    r = func_8005715C(0x10000 - v * v);
    if (x & 2) {
        r = ~r;
        r++;
    }
    x >>= 2;
    return func_800574A0_curve(r, x);
}

s32 func_80057364(u32 x) {
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

s32 func_800573CC(u32 x) {
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
    return func_800574A0_curve(v, x);
}

s32 func_80057488(s32 a) {
    s32 r = 0x10000;
    if (a & 0x100) {
        r = 0xFFFF0000;
    }
    return r;
}

s32 func_800574A0(u32 x) {
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

static inline s32 func_80057518_inline(s32 x) {
    s32 i = x & 0xFF;
    return D_8006F440[i] << 8;
}

s32 func_80057518(s32 x) {
    return func_80057518_inline(x);
}

s32 func_80057534(u32 code) {
    s32 v;
    u32 sel;

    v = code & 0xFF;
    v = D_8006F440[v] << 8;

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
    SPU_REG16(0x1F801D88) = x;
    SPU_REG16(0x1F801D8A) = x >> 16;
}

void akao_keyOff(s32 x) {
    SPU_REG16(0x1F801D8C) = x;
    SPU_REG16(0x1F801D8E) = (u32)x >> 16;
}

void akao_setReverbVoices(u32 x) {
    SPU_REG16(0x1F801D98) = x;
    SPU_REG16(0x1F801D9A) = x >> 16;
}

void akao_setNoiseVoices(u32 x) {
    SPU_REG16(0x1F801D94) = x;
    SPU_REG16(0x1F801D96) = x >> 16;
}

void akao_setPitchModVoices(u32 x) {
    SPU_REG16(0x1F801D90) = x;
    SPU_REG16(0x1F801D92) = x >> 16;
}

void akao_setVoiceVolume(s32 v, u32 l, u32 r, s32 vol) {
    if (vol != 0) {
        l *= vol;
        r *= vol;
        l >>= 7;
        r >>= 7;
    }
    ((SpuVoiceReg *)0x1F801C00)[v].volL = l & 0x7FFF;
    ((SpuVoiceReg *)0x1F801C00)[v].volR = r & 0x7FFF;
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
    r = (u16 *)(0x1F801C00 + v * 16);
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

void func_800579B4(AkaoChannel *c, u32 mask, s32 kind) {
    s32 nw;
    if ((u16)c->unkB8 != 0) {
        s32 old;
        c->unkB8 = (u16)c->unkB8 - 1;
        old = c->unk78;
        nw = old + c->unk7C;
        if ((nw & 0xFFE00000) != (old & 0xFFE00000)) {
            c->voiceDirty |= 3;
        }
        c->unk78 = nw;
    }
    if (kind == 0 && c->unkB4 != 0) {
        u16 old;
        c->unkB4--;
        old = c->unkB2;
        nw = old + c->unkFC;
        if ((nw & 0x7F00) != (old & 0x7F00)) {
            c->voiceDirty |= 3;
        }
        c->unkB2 = nw;
    }
    if (c->unkC0 != 0) {
        u16 old;
        c->unkC0--;
        old = c->unkBE;
        nw = old + c->unk104;
        if ((nw & 0xFF00) != (old & 0xFF00)) {
            c->voiceDirty |= 3;
        }
        c->unkBE = nw;
    }
    if (c->unkD0 != 0) {
        c->unkD0--;
    }
    if (c->unkDE != 0) {
        c->unkDE--;
    }
    if ((u16)c->unkD2 != 0) {
        c->unkD2 = (u16)c->unkD2 - 1;
        c->unk3C += c->unk40;
        c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
    }
    if ((u16)c->unkE0 != 0) {
        c->unkE0 = (u16)c->unkE0 - 1;
        c->unk4C += c->unk50;
        c->unk58 = 0x1000000 / ((u32)c->unk4C >> 12);
    }
    if (c->unkE8 != 0) {
        c->unkE8--;
        c->unk5C += c->unk60;
        c->unk68 = 0x1000000 / ((u32)c->unk5C >> 12);
    }
    if (c->unkF0 != 0) {
        if (--c->unkF0 == 0) {
            if (kind == 0) {
                g_akaoSeq->unk3C ^= mask;
            } else {
                D_80080A70.unk1C ^= mask;
            }
            D_80083158.unk8 |= 0x110;
        }
    }
    if (c->unkF2 != 0) {
        if (--c->unkF2 == 0) {
            if (kind == 0) {
                g_akaoSeq->unk44 ^= mask;
            } else {
                D_80080A70.unk24 ^= mask;
            }
            D_80083158.unk8 |= 0x100;
        }
    }
    if ((u16)c->unkD8 != 0) {
        u32 v;
        u32 n;
        c->unkD8 = (u16)c->unkD8 - 1;
        v = c->unkD6 + (u16)c->unkDA;
        c->unkD6 = v;
        n = (v & 0x7F00) >> 8;
        if (v & 0x8000) {
            c->unkD4 = (n * c->unk2C) >> 7;
        } else {
            c->unkD4 = (n * ((c->unk2C * 15) >> 8)) >> 7;
        }
    }
    if ((u16)c->unkE4 != 0) {
        c->unkE4 = (u16)c->unkE4 - 1;
        c->unkE2 += c->unkE6;
    }
    if ((u16)c->unkEC != 0) {
        c->unkEC = (u16)c->unkEC - 1;
        c->unkEA += c->unkEE;
    }
    if ((u16)c->unkC2 != 0) {
        s32 old;
        c->unkC2 = (u16)c->unkC2 - 1;
        old = c->unk30;
        nw = old + c->unk80;
        if ((nw & 0xFFFF0000) != (old & 0xFFFF0000)) {
            c->voiceDirty |= 0x10;
        }
        c->unk30 = nw;
    }
}

void func_80057D80(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;

    f = c->unk34;
    vol = ((c->unk78 >> 16) * (c->unkB2 >> 8)) >> 7;
    if ((f & 1) && c->unkD0 == 0) {
        c->unk44 += c->unk48;
        t = c->unk1C((u32)c->unk44 >> 16);
        pan = (s16)((u32)(c->unkD4 * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->unk110 = x;
        c->voiceDirty |= 0x10;
    }
    if ((f & 2) && c->unkDE == 0) {
        c->unk54 += c->unk58;
        t = c->unk20((u32)c->unk54 >> 16);
        pan = (vol * (c->unkE2 >> 8)) << 9 >> 16;
        c->voiceDirty |= 3;
        c->unk112 = (pan * t) >> 16;
    }
    if (f & 4) {
        c->unk64 += c->unk68;
        t = c->unk24((u32)c->unk64 >> 16);
        c->voiceDirty |= 3;
        c->unk114 = ((c->unkEA >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->voiceDirty |= 3;
        vol = ((s16)(c[-1].voicePitch << 1) * (c->unkB2 >> 8)) >> 7;
    }
    if (c->voiceDirty & 3) {
        vol += c->unk112;
        vol = (vol * (s32)(((u32)g_akaoSeq->unk50 >> 16) & 0x7F)) >> 7;
        pan = (c->unkBE >> 8) + c->unk114;
        pan = (pan + (g_akaoSeq->unk5C >> 16) - 0x40) & 0xFF;
        if (D_8008315C & 2) {
            c->voiceVolL = c->voiceVolR = (vol * D_8006F640) >> 15;
        } else {
            c->voiceVolL = (vol * D_8006F540[pan]) >> 15;
            c->voiceVolR = (vol * D_8006F540[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].voicePitch + c->unk110 + (c->unk30 >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->voicePitch = ((u16)c->unk84 + pitch) & 0x3FFF;
        c->voiceDirty |= 0x10;
    } else if (c->voiceDirty & 0x10) {
        pitch = c->unk2C + c->unk110 + (c->unk30 >> 16);
        pan = D_80083150 & 0xFF0000;
        if (pan != 0) {
            pan >>= 16;
            if (pan < 0x80) {
                pitch += (pitch * pan) >> 7;
            } else {
                pitch = (pitch * pan) >> 8;
            }
        }
        c->voicePitch = ((u16)c->unk84 + pitch) & 0x3FFF;
    }
}

void func_80058118(AkaoChannel *c, u32 bit) {
    u32 f;
    s32 vol;
    s32 t;
    s32 x;
    s32 pan;
    s32 pitch;
    s32 k;

    f = c->unk34;
    vol = ((c->unk78 >> 16) * (c->unkB2 >> 8)) >> 7;
    if (f & 1) {
        c->unk44 += c->unk48;
        t = c->unk1C((u32)c->unk44 >> 16);
        pan = (s16)((u32)(c->unkD4 * t) >> 16);
        x = pan;
        if (pan < 0) {
            x = pan >> 1;
        }
        c->unk110 = x;
        c->voiceDirty |= 0x10;
    }
    if (f & 2) {
        c->unk54 += c->unk58;
        t = c->unk20((u32)c->unk54 >> 16);
        pan = (vol * (c->unkE2 >> 8)) << 9 >> 16;
        c->voiceDirty |= 3;
        c->unk112 = (pan * t) >> 16;
    }
    if (f & 4) {
        c->unk64 += c->unk68;
        t = c->unk24((u32)c->unk64 >> 16);
        c->voiceDirty |= 3;
        c->unk114 = ((c->unkEA >> 8) * t) >> 16;
    }
    if (f & 0x20) {
        c->voiceDirty |= 3;
        vol = ((s16)(c[-1].voicePitch << 1) * (c->unkB2 >> 8)) >> 7;
    }
    if (c->voiceDirty & 3) {
        vol += c->unk112;
        if (!(c->unk28 & 0x2000000)) {
            vol = (vol * ((s16)c->unk100 >> 8)) >> 7;
            pan = ((c->unkBE + c->unk9C) >> 8) + c->unk114 + 0x80;
            pan &= 0xFF;
        } else {
            pan = 0x80;
        }
        if (D_8008315C & 2) {
            c->voiceVolL = c->voiceVolR = (vol * D_8006F640) >> 15;
        } else {
            c->voiceVolL = (vol * D_8006F540[pan]) >> 15;
            c->voiceVolR = (vol * D_8006F540[pan ^ 0xFF]) >> 15;
        }
    }
    if (f & 0x10) {
        pitch = c[-1].voicePitch + c->unk110 + (c->unk30 >> 16);
        k = c->unk70 & 0xFF00;
        if (!(c->unk28 & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->voicePitch = ((u16)c->unk84 + pitch) & 0x3FFF;
        c->voiceDirty |= 0x10;
    } else if (c->voiceDirty & 0x10) {
        pitch = c->unk2C + c->unk110 + (c->unk30 >> 16);
        k = c->unk70 & 0xFF00;
        if (!(c->unk28 & 0x2000000) && k != 0) {
            k >>= 8;
            if (k < 0x80) {
                pitch += (pitch * k) >> 7;
            } else {
                pitch = (pitch * k) >> 8;
            }
        }
        c->voicePitch = ((u16)c->unk84 + pitch) & 0x3FFF;
    }
}

s32 func_800584C8(s32 arg0) {
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
    e = &D_8007F798[i];
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
    func_80058830(g_akaoChannels, best, min);
    return best;
}

s32 func_80058578(s32 a) {
    Unk8007F798 *p;
    if (a != 0) {
        a = 0;
    } else {
        a = g_akaoSeq->unk38;
    }
    while ((p = &D_8007F798[a])->unk4 != 0) {
        a++;
        if (a >= 0x18) {
            break;
        }
    }
    return a;
}

void func_800585E0(AkaoChannel *ch, u32 mask, u32 keep, u32 *used) {
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
            func_80057D80(ch, bit);
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
                        v = func_80058578(f);
                        if (v == 24) {
                            g_akaoSeq->unk0 |= 2;
                            v = func_800584C8(f);
                        }
                        if (v == 24) {
                            ch->voice = v;
                            g_akaoSeq->unk0 |= 1;
                        } else {
                            *used |= 1 << v;
                            ch->voice = v;
                            D_8007F798[v].unk4 = 0x7FFF;
                        }
                    }
                    if (ch->voice < 24) {
                        akao_writeVoice(ch->voice, (AkaoVoiceRegs *)&ch->voice, (u16)ch->voiceVolScale);
                        D_800831C8[ch->voice] = g_akaoSeq;
                        D_80083158.unk8 |= 0x100;
                    }
                } else if (ch->voice < 24) {
                    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
                    if (mask) {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->unk34);
                    } else {
                        ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->unk34);
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
void func_80058830(c, voice)
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
    if (D_8007F790 != NULL) {
        c = D_8007F71C;
        for (i = 0; i < 32; i++) {
            if (voice == c->voice) {
                c->voice = 0x18;
            }
            c++;
        }
    }
}

void func_800588B0(u32 extra) {
    u32 m = (g_akaoSeq->unk4 & g_akaoSeq->unkC) | extra;
    u32 i;
    Unk8007F798 *p;
    if (D_8007F790 != NULL) {
        m |= D_8007F790->unk4 & D_8007F790->unkC;
    }
    for (i = 0, p = D_8007F798; i < 24; i++, p++) {
        if (m & (1 << i)) {
            p->unk4 = 0x7FFF;
        } else {
            SpuGetVoiceEnvelope(i, &p->unk4);
            if (p->unk4 == 0) {
                func_80058830(g_akaoChannels, i);
            }
        }
    }
}

void func_80058990(AkaoChannel *c) {
    s32 v;
    D_80080A18.unkC = v = c->unk50;
    v >>= 16;
    v *= D_80080A18.unk0 >> 8;
    v /= 127;
    v <<= 8;
    c->unk50 = v;
}

void func_800589E0(Unk800589E0 *a) {
    a->unk50 = D_80080A24;
}

void func_800589F0(void) {
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

    ex = D_80080A70.unk0 | D_80080A70.unk10 | D_80083174;
    keyon = 0;
    if ((g_akaoSeq->unk4 & g_akaoSeq->unk10) ||
        (D_8007F790 != NULL && (D_8007F790->unk4 & D_8007F790->unk10))) {
        func_800588B0(ex);
    }
    if (D_8007F790 != NULL) {
        if (D_80083158.unk4 & 0x100) {
            ((void (*)(Unk80080A10 *, AkaoChannel *))func_80058990)(D_8007F790, D_8007F71C);
        }
        t = D_8007F790;
        m1 = t->unk4 & t->unk14 & ~(t->unkC & ex);
        b1 = m1 & t->unkC & ~ex;
        g_akaoSeq = t;
        if (m1 & g_akaoSeq->unk8) {
            func_800585E0(D_8007F71C, m1 & g_akaoSeq->unk8, b1, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m1 &= ~g_akaoSeq->unk8;
        }
        s = &g_akaoMainSeq;
        g_akaoSeq = s;
        m2 = s->unk4 & s->unk14 & ~(s->unkC & (b1 | ex));
        b2 = m2 & s->unkC & ~(b1 | ex);
        if (m2 & g_akaoSeq->unk8) {
            func_800585E0(g_akaoChannels, m2 & g_akaoSeq->unk8, b2, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m1) {
            g_akaoSeq = D_8007F790;
            b1 &= ~b2;
            func_800585E0(D_8007F71C, m1, b1, &keyon);
            g_akaoSeq->unk10 = 0;
            g_akaoSeq = s;
        }
        if (m2) {
            func_800585E0(g_akaoChannels, m2, b2, &keyon);
            g_akaoSeq->unk10 = 0;
        }
        if (D_80083158.unk4 & 0x100) {
            if (m2) {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(D_8007F790, D_8007F71C);
            } else {
                ((void (*)(Unk80080A10 *, AkaoChannel *))func_800589E0)(D_8007F790, D_8007F71C);
            }
        }
    } else {
        m2 = g_akaoSeq->unk4 & g_akaoSeq->unk14 & ~(g_akaoSeq->unkC & ex);
        bit = m2 & g_akaoSeq->unkC & ~ex;
        if (m2 & g_akaoSeq->unk8) {
            func_800585E0(g_akaoChannels, m2 & g_akaoSeq->unk8, bit, &keyon);
            g_akaoSeq->unk10 &= ~g_akaoSeq->unk8;
            m2 &= ~g_akaoSeq->unk8;
        }
        if (m2) {
            func_800585E0(g_akaoChannels, m2, bit, &keyon);
            g_akaoSeq->unk10 = 0;
        }
    }
    m2 = D_80080A70.unk0 & D_80080A70.unk8;
    if (m2) {
        keyon |= D_80080A70.unk4;
        bit = 0x100;
        ch = D_8007E3D8;
        for (; m2 != 0; bit <<= 1, ch++) {
            if (m2 & bit) {
                func_80058118(ch, bit);
                if (ch->voiceDirty) {
                    ((void (*)(s32, AkaoVoiceRegs *, u32))akao_flushVoice)(ch->voice, (AkaoVoiceRegs *)&ch->voice, ch->unk34);
                }
                m2 &= ~bit;
            }
        }
        D_80080A74 = 0;
    }
    m2 = D_80083158.unk8;
    if (m2 & 0x80) {
        SpuSetReverbModeDepth(g_akaoSeq->unk48 >> 12, g_akaoSeq->unk48 >> 12);
        D_80083158.unk8 &= ~0x80;
    }
    if (m2 & 0x10) {
        SpuSetNoiseClock(D_80080A70.unk0 ? D_80080A70.unk28 : g_akaoSeq->unk6E);
        D_80083158.unk8 &= ~0x10;
    }
    if (m2 & 0x100) {
        func_80059140((u32 *)&D_80083238[1], D_8007F790->unk3C, g_akaoSeq->unk3C, D_80080A70.unk1C);
        func_80059140((u32 *)&D_80083238[0], D_8007F790->unk40, g_akaoSeq->unk40, D_80080A70.unk20);
        func_80059140((u32 *)&D_80083238[2], D_8007F790->unk44, g_akaoSeq->unk44, D_80080A70.unk24);
        akao_setReverbVoices(D_80083238[0]);
        akao_setNoiseVoices(D_80083238[1]);
        akao_setPitchModVoices(D_80083238[2]);
        D_80083158.unk8 &= ~0x100;
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
    keep = ~((D_80080A70.unk0 | D_80080A70.unk10) | D_80083174);
    if (D_8007F790 != NULL) {
        a = D_8007F790->unk18;
        m = a & D_8007F790->unk8;
        if (m != 0) {
            func_80058F34(D_8007F71C, &out, m, keep);
            D_8007F790->unk18 &= ~D_8007F790->unk8;
            a &= ~D_8007F790->unk8;
        }
    }
    b = g_akaoSeq->unk18;
    m = b & g_akaoSeq->unk8;
    if (m != 0) {
        func_80058F34(g_akaoChannels, &out, m, keep);
        g_akaoSeq->unk18 &= ~g_akaoSeq->unk8;
        b &= ~g_akaoSeq->unk8;
    }
    if (D_8007F790 != NULL && a != 0) {
        func_80058F34(D_8007F71C, &out, a, keep);
        D_8007F790->unk18 = 0;
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &out, b, keep);
        g_akaoSeq->unk18 = 0;
    }
    out |= D_80080A70.unkC;
    D_80080A70.unkC = 0;
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

    keep = ~((D_80080A70.unk0 | D_80080A70.unk10) | D_80083174);
    a = 0;
    r = 0;
    s = D_8007F790;
    if (s != NULL) {
        a = s->unk4 & m1;
        if (a & s->unk8) {
            func_80058F34(D_8007F71C, &r, a & s->unk8, keep);
            a &= ~D_8007F790->unk8;
        }
    }
    b = g_akaoSeq->unk4 & m2;
    if (b & g_akaoSeq->unk8) {
        func_80058F34(g_akaoChannels, &r, b & g_akaoSeq->unk8, keep);
        b &= ~g_akaoSeq->unk8;
    }
    if (D_8007F790 != NULL && a != 0) {
        func_80058F34(D_8007F71C, &r, a, keep);
    }
    if (b != 0) {
        func_80058F34(g_akaoChannels, &r, b, keep);
    }
    *out = r |= extra;
    D_80083158.unk8 |= 0x100;
}

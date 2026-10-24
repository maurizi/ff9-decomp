#include "ff9.h"

void akao_applyCdVolume(void) {
    s32 v = g_akaoCdVolumeHi;
    SPU_REG16(SPU_CD_VOL_L) = v;
    SPU_REG16(SPU_CD_VOL_R) = v;
}

void func_8005CE04(u32 *src, u32 *dst, u32 n) {
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

void func_8005CE8C(u32 *a, u32 *b, u32 n) {
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

void func_8005CF04(void) {
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
    s = &D_80083168;
    if (s->unkC != 0 && s->unk48 != 0) {
        s->unk48--;
        old = s->unk40;
        v = old + s->unk44;
        if ((v & 0xFF00) != (old & 0xFF00)) {
            if (D_8008315C & 2) {
                old = (old * D_8006F640) >> 16;
                akao_setVoiceVolume(s->unk10, old, old, 0);
                akao_setVoiceVolume(s->unk10 + 1, old, old, 0);
            } else {
                old = v << 15;
                old >>= 16;
                akao_setVoiceVolume(s->unk10, old, 0, 0);
                akao_setVoiceVolume(s->unk10 + 1, 0, old, 0);
            }
        }
        D_800831A8 = v & 0xFFFF;
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
    if (g_akaoSeq->unk4 != 0) {
        if (g_akaoSeq->unk58 != 0) {
            g_akaoSeq->unk58--;
            v = g_akaoSeq->unk50 + g_akaoSeq->unk54;
            if ((v & 0x7F0000) != (g_akaoSeq->unk50 & 0x7F0000)) {
                func_8005A284(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->unk50 = v;
        }
        if (g_akaoSeq->unk64 != 0) {
            g_akaoSeq->unk64--;
            v = g_akaoSeq->unk5C + g_akaoSeq->unk60;
            if ((v & 0x7F0000) != (g_akaoSeq->unk5C & 0x7F0000)) {
                func_8005A284(g_akaoSeq, g_akaoChannels);
            }
            g_akaoSeq->unk5C = v;
        }
    }
    if (D_8007F790 != NULL && D_8007F790->unk4 != 0) {
        if (D_8007F790->unk58 != 0) {
            D_8007F790->unk58--;
            v = D_8007F790->unk50 + D_8007F790->unk54;
            if ((v & 0x7F0000) != (D_8007F790->unk50 & 0x7F0000)) {
                func_8005A284(D_8007F790, D_8007F71C);
            }
            D_8007F790->unk50 = v;
        }
        if (D_8007F790->unk64 != 0) {
            D_8007F790->unk58--;
            v = D_8007F790->unk5C + D_8007F790->unk60;
            if ((v & 0x7F0000) != (D_8007F790->unk5C & 0x7F0000)) {
                func_8005A284(D_8007F790, D_8007F71C);
            }
            D_8007F790->unk5C = v;
        }
    }
    if (D_80080A70.unk0 != 0) {
        mask = D_80080A70.unk0;
        c = D_8007E3D8;
        k = 0x100;
        do {
            if (mask & k) {
                if ((u16)c->unkBC != 0) {
                    c->unkBC = (u16)c->unkBC - 1;
                    v = (s16)c->unk100 + c->unk102;
                    if ((v & 0xFF00) != ((s16)c->unk100 & 0xFF00)) {
                        c->voiceDirty |= 3;
                    }
                    c->unk100 = v;
                }
                if (c->unk9E != 0) {
                    c->unk9E--;
                    v = c->unk9C + c->unkFE;
                    if ((v & 0xFF00) != (c->unk9C & 0xFF00)) {
                        c->voiceDirty |= 3;
                    }
                    c->unk9C = v;
                }
                if ((u16)c->unkB6 != 0) {
                    c->unkB6 = (u16)c->unkB6 - 1;
                    v = c->unk70 + c->unk74;
                    if ((v & 0xFF00) != (c->unk70 & 0xFF00)) {
                        c->voiceDirty |= 0x10;
                    }
                    c->unk70 = v;
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

u32 func_8005D384(AkaoChannel *chans, s32 sub) {
    u32 tempo;
    u32 m;
    s32 acc;
    AkaoChannel *c;
    u32 bit;
    u32 active;
    s32 flags;

    tempo = g_akaoSeq->unk20.h[1];
    m = D_80083156;
    if (m != 0) {
        if (m < 0x80) {
            tempo += (tempo * m) >> 7;
        } else {
            tempo = (tempo * m) >> 8;
        }
    }
    acc = g_akaoSeq->unk28 + tempo;
    g_akaoSeq->unk28 = acc;
    if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
        g_akaoSeq->unk28 = acc & 0xFFFF;
        do {
            for (;;) {
                c = chans;
                bit = 1;
                active = g_akaoSeq->unk4;
                do {
                    if (active & bit) {
                        c->unk96--;
                        c->unk98--;
                        if (c->unk96 == 0) {
                            func_8005E3FC(c, bit);
                        } else if (c->unk98 == 0) {
                            g_akaoSeq->unk18 |= bit;
                        }
                        func_800579B4(c, bit, 0);
                        active &= ~bit;
                    }
                    c++;
                    bit <<= 1;
                } while (active != 0);
                if (g_akaoSeq->unk68 != 0) {
                    g_akaoSeq->unk68--;
                    g_akaoSeq->unk20.w += g_akaoSeq->unk24;
                }
                if (g_akaoSeq->unk66 != 0) {
                    g_akaoSeq->unk66--;
                    g_akaoSeq->unk48 += g_akaoSeq->unk4C;
                    flags = D_80083158.unk8;
                    flags |= 0x80;
                    if (sub == 0) {
                        D_80083158.unk8 = flags;
                    }
                }
                if (g_akaoSeq->unk74 != 0) {
                    if (++g_akaoSeq->unk76 == g_akaoSeq->unk74) {
                        g_akaoSeq->unk76 = 0;
                        if (++g_akaoSeq->unk72 == g_akaoSeq->unk70) {
                            g_akaoSeq->unk72 = 0;
                            g_akaoSeq->unk78++;
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
    return g_akaoSeq->unk4;
}

void func_8005D628(Unk80080A10 *arg0, AkaoChannel *arg1) {
    s32 t;

    if (D_80080A18.unk8 != 0) {
        if (--D_80080A18.unk8 == 0) {
            s32 v = arg0->unk4;
            arg0->unk6A = 0;
            arg0->unk4 = 0;
            arg0->unk10 = 0;
            arg0->unk14 = 0;
            arg0->unk18 = v;
            return;
        }
        t = D_80080A18.unk0 + D_80080A18.unk4;
        if ((t & 0xFFFF0000) != (D_80080A18.unk0 & 0xFFFF0000)) {
            func_8005A284(arg0, arg1, arg0);
        }
        D_80080A18.unk0 = t;
    }
}

s32 akao_tick(void) {
    s32 t0;
    s32 n;
    u32 now;
    Unk80080A10 *tmp;
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
        if ((g_akaoSeq->unk18 | D_80080A70.unkC) != 0 || (D_8007F790 != NULL && D_8007F790->unk18 != 0)) {
            func_80058F94();
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 == 0) {
                D_8007F790 = NULL;
            } else if ((g_akaoSeq->unk4 | g_akaoSeq->unk1C) == 0) {
                func_8005CE04(D_8007F790, g_akaoSeq, 0x7C);
                func_8005CE04(D_8007F71C, g_akaoChannels, 0x2680);
                tmp = D_8007F790;
                D_8007F790 = NULL;
                tmp->unk6A = 0;
                tmp->unk4 = 0;
            }
        }
        if ((D_80083158.unk8 | g_akaoSeq->unk14 | D_80080A70.unk8) != 0 || (D_8007F790 != NULL && D_8007F790->unk14 != 0)) {
            func_800589F0();
        }
        if (g_akaoSeq->unk4 != 0) {
            func_8005D384(g_akaoChannels, 0);
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 != 0) {
                g_akaoSeq = D_8007F790;
                func_8005D384(D_8007F71C, 1);
                g_akaoSeq = &g_akaoMainSeq;
            }
            if ((D_80083158.unk4 & 0x100) && !(g_akaoTickCount & 3)) {
                func_8005D628(D_8007F790, D_8007F71C);
                if (D_8007F790->unk4 == 0) {
                    D_80083158.unk4 &= ~0x100;
                }
            }
        }
        if (D_80080A70.unk0 != 0) {
            mask = D_80080A70.unk0;
            acc = D_80080A70.unk18 + D_80080A70.unk14.h[1];
            D_80080A70.unk18 = acc;
            if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
                D_80080A70.unk18 = acc & 0xFFFF;
                bit = 0x100;
                c = D_8007E3D8;
                do {
                    if (mask & bit) {
                        if (!(D_800831C4 & 2) || (c->unk28 & 0x2000000)) {
                            c->unk88++;
                            c->unk96--;
                            c->unk98--;
                            if (c->unk96 == 0) {
                                func_8005E3FC(c, bit);
                            } else if (c->unk98 == 0) {
                                D_80080A70.unkC |= bit;
                                D_80080A70.unk8 &= ~bit;
                            }
                            func_800579B4(c, bit, 1);
                        }
                        mask ^= bit;
                    }
                    c++;
                    bit <<= 1;
                } while (mask != 0);
            }
        }
        if (!(g_akaoTickCount & 3)) {
            func_8005CF04();
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

    p = c->unk0;
    n = c->unkF4;
    for (;;) {
        if (*p < 0x9A) {
            if (*p >= 0x8F) {
                c->unkCC = (u16)c->unkCC & ~5;
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
                if (*p == c->unkA2[n] + 1) {
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
                if (*p++ <= g_akaoSeq->unk6C) {
                    op = p[0] + (p[1] << 8);
                    p += (s16)op;
                    continue;
                }
                p += 2;
                continue;
            case 0xF:
                p = c->unk14;
                continue;
            }
            continue;
        case 0xC9:
            p++;
            if (*p == c->unkA2[n] + 1) {
                p++;
                n--;
                n &= 3;
                continue;
            }
            p = c->unk4[n];
            continue;
        case 0xCB:
        case 0xCD:
        case 0xD1:
        case 0xDB:
            c->unkCC = (u16)c->unkCC & ~5;
            p++;
            continue;
        case 0xCA:
            if (c->unk34 & 0x200000) {
                goto end;
            }
            p = c->unk4[n];
            continue;
        default:
        end:
            c->unkCC = (u16)c->unkCC & ~5;
            return 0xA0;
        }
    }
}

s32 func_8005DD54(s32 a, s32 b) {
    if ((a & 0x20) && (u32)(b - 0x20) < 0x60) {
        return b + 0x30;
    }
    return b;
}

void func_8005DD7C(AkaoChannel *c, u32 key) {
    AkaoRegion *e;
    Unk8005EBB4 *p;
    u32 f;
    s32 v;

    if (c->unk10A < key || c->unk10A == 0xFF) {
        e = (AkaoRegion *)c->unk18;
        while (e[1].unk5 != 0 && e->unk2 < key) {
            e++;
        }
    } else if (key < c->unk10A) {
        e = (AkaoRegion *)c->unk18;
        while (e[1].unk5 != 0 && !(key < e[1].unk1)) {
            e++;
        }
    } else {
        return;
    }
    f = c->unk34;
    v = func_8005DD54(g_akaoSeq->unk0, e->unk0);
    c->unk9A = v;
    p = &D_8007F970[v];
    c->voiceStartAddr = p->unk0;
    c->voiceLoopAddr = p->unk4;
    if (!(f & 0x1000000)) {
        c->voiceAdsr1 = e->unk3 << 8;
    } else {
        c->voiceAdsr1 &= 0x7F00;
    }
    c->voiceAdsr1 |= p->unkC & 0x80FF;
    if (!(f & 0x8000000)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->unk4 << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->unk5) {
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
        c->voiceAdsr2 |= e->unk6;
    }
    c->voiceAdsr2 |= p->unkE & 0x20;
    c->voiceVolScale = e->unk7;
}

u16 akao_calcPitch(Unk8005EBB4 *p, s32 note, u8 fine, s32 *out) {
    s32 n = note - p->unkA;
    s32 freq;
    u32 f;

    while (n < 0) {
        n += 12;
    }
    n %= 12;
    if (p->unk8 == 0) {
        freq = g_akaoSemitonePitch[n] << 8;
    } else if (p->unk8 < 0) {
        freq = (g_akaoSemitonePitch[n] * (u16)p->unk8) >> 8;
    } else {
        freq = (g_akaoSemitonePitch[n] * p->unk8) >> 7;
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
    if (note < p->unkA) {
        do {
            *out >>= 1;
            freq >>= 1;
        } while ((note += 12) < p->unkA);
    } else {
        n = (note - p->unkA) / 12;
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
    Unk8005EBB4 *p;
    u32 f;
    s32 v;
    s32 r;

    e = (AkaoDrumKey *)g_akaoSeq->unk34;
    e += key;
    g_akaoSeq->unk10 |= mask;
    if (g_akaoSeq->unk14 & mask) {
        g_akaoSeq->unk18 |= mask;
    }
    f = c->unk34;
    v = func_8005DD54(g_akaoSeq->unk0, e->unk0);
    c->unk9A = v;
    p = &D_8007F970[v];
    c->voiceStartAddr = p->unk0;
    c->voiceLoopAddr = p->unk4;
    if (!(f & 0x1000000)) {
        c->voiceAdsr1 = e->unk2 << 8;
    } else {
        c->voiceAdsr1 &= 0x7F00;
    }
    c->voiceAdsr1 |= p->unkC & 0x80FF;
    if (!(f & 0x8000000)) {
        c->voiceAdsr2 &= 0x201F;
        c->voiceAdsr2 |= e->unk3 << 6;
    } else {
        c->voiceAdsr2 &= 0x3FDF;
    }
    switch (e->unk4) {
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
        c->voiceAdsr2 |= e->unk5;
    }
    c->voiceAdsr2 |= p->unkE & 0x20;
    r = ((s32 (*)())akao_calcPitch)(p, e->unk1, c->unk108, &c->unk84);
    c->voiceVolScale = e->unk6;
    c->unkBE = ((e->unk7 & 0x7F) + 0x40) << 8;
    if (e->unk7 & 0x80) {
        g_akaoSeq->unk40 |= mask;
    } else {
        g_akaoSeq->unk40 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
    return r;
}

void func_8005E3FC(AkaoChannel *c, u32 mask) {
    u32 op;
    u32 vel;
    u32 tmp;
    u32 f;
    u16 len;
    u32 v;

    do {
        op = *c->unk0++;
        if (op >= 0xA0) {
            if (op == 0xFE) {
                vel = *c->unk0++;
                D_8006FCE0[vel](c, mask);
            } else if (op - 0xF0 < 14) {
                op = (op - 0xF0) * 11;
                c->unk96 = *c->unk0++;
            } else {
                if (op == 0xFF) {
                    op = 0xA0;
                } else if (op == 0xCA && (c->unk34 & 0x200000)) {
                    op = 0xA0;
                    D_80080A70.unkC |= mask;
                }
                D_8006FB60[op - 0xA0](c, mask);
            }
        }
        c->unkA0++;
    } while (op > 0xA0);
    if (op == 0xA0) {
        if (c->unk94 == 0) {
            g_akaoSeq->unk18 |= mask;
        }
        return;
    }
    vel = func_8005DB40(c) & 0xFF;
    if (c->unkFA != 0) {
        c->unk96 = c->unk98 = c->unkFA;
    }
    if (c->unk96 != 0) {
        if (vel >= 0x8F || (vel < 0x84 && !(c->unkCC & 5))) {
            c->unk98 -= 2;
        }
    } else {
        c->unk96 = D_8006F3F4[op % 11];
        len = c->unk96;
        if ((vel < 0x84 || vel >= 0x8F) && !(c->unkCC & 5)) {
            len -= 2;
        }
        c->unk98 = len;
    }
    if (c->unk94 == 0 && (c->unk34 & 0x40)) {
        c->unk98 = c->unk96;
    }
    c->unkF8 = c->unk96;
    c->voiceDirty |= 0x4000;
    if (op >= 0x8F) {
        if (c->unk94 == 0) {
            g_akaoSeq->unk14 &= ~mask;
            if (c->voice < 0x18) {
                g_akaoSeq->unk18 |= mask;
            }
        }
        c->unkCA = 0;
        c->unk110 = 0;
        c->unk112 = 0;
        c->unkCC &= ~2;
        return;
    }
    if (op < 0x84) {
        op /= 11;
        op += c->unkC4 * 12;
        f = c->unk34;
        if (f & 8) {
            vel = func_8005E170(c, mask, op);
        } else {
            if (!(c->unkCC & 2)) {
                if (c->unk94 == 0) {
                    if (f & 0x1000) {
                        func_8005DD7C(c, op);
                    }
                    g_akaoSeq->unk10 |= mask;
                    if ((g_akaoSeq->unk14 & mask) && c->voice < 0x18) {
                        g_akaoSeq->unk18 |= mask;
                    }
                    if (c->unkBA != 0) {
                        c->unkB8 = c->unkBA;
                        c->unk78 = c->unk8C;
                        c->unk7C = c->unk90;
                    }
                } else {
                    D_80080A70.unk4 |= mask;
                }
                c->unkC2 = 0;
            }
            if (c->unkCA != 0 && c->unkC8 != 0) {
                c->unkC6 = c->unkCA;
                c->unk10C = c->unk106 + op - c->unkC8 - c->unk10E;
                c->unk10A = c->unkC8 - (c->unk106 - c->unk10E);
                op = c->unkC8 + c->unk10E;
            } else {
                c->unk10A = op;
                op += c->unk106;
            }
            vel = ((s32 (*)())akao_calcPitch)(&D_8007F970[c->unk9A], op, c->unk108, &c->unk84);
            if (c->unkF6 != 0) {
                tmp = (u32)(vel * c->unkF6) >> 8;
                tmp *= (u8)D_8006F440[D_8008314C];
                if ((u8)D_8006F440[D_8008314C] & 0x80) {
                    tmp >>= 9;
                    vel -= tmp;
                } else {
                    tmp >>= 7;
                    vel += tmp;
                }
            }
        }
        c->unk2C = vel;
        if (c->unk94 == 0) {
            g_akaoSeq->unk14 |= mask;
        } else {
            D_80080A70.unk8 |= mask;
        }
        op = c->unk34;
        c->voiceDirty |= 0x13;
        if (op & 1) {
            v = (c->unkD6 & 0x7F00) >> 8;
            if (!(c->unkD6 & 0x8000)) {
                v = (v * ((u32)(vel * 15) >> 8)) >> 7;
            } else {
                v = (v * vel) >> 7;
            }
            c->unkD4 = v;
            if (!(c->unkCC & 2)) {
                c->unk44 = 0;
                c->unkD0 = c->unkCE;
                c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
            }
        }
        if ((op & 2) && !(c->unkCC & 2)) {
            c->unk54 = 0;
            c->unkDE = c->unkDC;
            c->unk58 = 0x1000000 / ((u32)c->unk4C >> 12);
        }
        c->unk110 = 0;
        c->unk112 = 0;
        c->unk30 = 0;
    }
    c->unkCC = (c->unkCC & ~2) | ((c->unkCC & 1) << 1);
    if (c->unk10C != 0) {
        c->unk10A += c->unk10C;
        vel = ((s32 (*)())akao_calcPitch)(&D_8007F970[c->unk9A], c->unk10A + c->unk106, c->unk108, &tmp) << 16;
        c->unkC2 = c->unkC6;
        c->unk10C = 0;
        c->unk80 = (s32)(vel - ((c->unk2C << 16) + c->unk30)) / (u16)c->unkC2;
    }
    c->unkC8 = c->unk10A;
    c->unk10E = c->unk106;
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

void func_8005EBB4(AkaoChannel *ch, Unk8005EBB4 *p, s32 arg2) {
    ch->voiceStartAddr = arg2;
    ch->voiceLoopAddr = p->unk4;
    ch->voiceAdsr1 = p->unkC;
    ch->voiceAdsr2 = p->unkE;
    ch->voiceDirty |= 0x1FF80;
}

void func_8005EBE8(AkaoChannel *c, s32 i) {
    c->unk9A = i;
    func_8005EBB4(c, &D_8007F970[i], D_8007F970[i].unk0);
}

void func_8005EC20(AkaoChannel *c, u32 mask) {
    D_80080A70.unk0 &= ~mask;
    D_80080A70.unk10 &= ~mask;
    D_80080A70.unk1C &= ~mask;
    D_80080A70.unk20 &= ~mask;
    D_80080A70.unk24 &= ~mask;
    D_80080A70.unk4 &= ~mask;
    D_80080A70.unk8 &= ~mask;
    c->unk28 = 0;
    c->unk6C = 0;
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
    Unk80080A10 *t;

    if (c->unk94 == 0) {
        t = g_akaoSeq;
        if ((t->unk4 &= ~mask) == 0) {
            D_800809B8 = 0;
            t->unk6A = 0;
            t->unk0 = 0;
        }
        g_akaoSeq->unk14 &= ~mask;
        g_akaoSeq->unk8 &= ~mask;
        g_akaoSeq->unkC &= ~mask;
        g_akaoSeq->unk3C &= ~mask;
        g_akaoSeq->unk40 &= ~mask;
        g_akaoSeq->unk44 &= ~mask;
    } else {
        func_8005EC20(c, mask);
    }
    c->unk34 = 0;
    D_80083158.unk8 |= 0x110;
}

void akao_opFETempo(AkaoChannel *c) {
    Unk80080A10 *a = g_akaoSeq;
    s32 v = c->unk0[0] << 16;
    a->unk20.w = v;
    a->unk20.w = v | (c->unk0[1] << 24);
    c->unk0 += 2;
    a->unk68 = 0;
}

void akao_opFETempoSlide(AkaoChannel *c) {
    s32 x;
    s32 m;

    if ((g_akaoSeq->unk68 = *c->unk0++) == 0) {
        g_akaoSeq->unk68 = 0x100;
    }
    x = (c->unk0[0] << 16) | (c->unk0[1] << 24);
    c->unk0 += 2;
    m = g_akaoSeq->unk20.w & 0xFFFF0000;
    g_akaoSeq->unk24 = (x - m) / g_akaoSeq->unk68;
    g_akaoSeq->unk20.w = m;
}

void akao_opFE02(AkaoChannel *c) {
    Unk80080A10 *a = g_akaoSeq;
    u8 *p = c->unk0;
    s32 hi = (s8)p[1];
    s32 lo = p[0];
    s32 v;
    c->unk0 = p + 2;
    v = hi << 20;
    v |= lo << 12;
    a->unk66 = 0;
    D_80083158.unk8 |= 0x80;
    a->unk48 = v;
}

void akao_opFE03(AkaoChannel *arg0) {
    u8 *p;
    s32 v;

    g_akaoSeq->unk66 = *arg0->unk0++;
    if (g_akaoSeq->unk66 == 0) {
        g_akaoSeq->unk66 = 0x100;
    }
    p = arg0->unk0;
    v = ((s8)p[1] << 20) | (p[0] << 12);
    arg0->unk0 = p + 2;
    g_akaoSeq->unk48 &= ~0xFFF;
    g_akaoSeq->unk4C = (v - g_akaoSeq->unk48) / g_akaoSeq->unk66;
}

void akao_opFEJump(AkaoChannel *c) {
    u8 *p = c->unk0;
    c->unk0 = p + (s16)(p[0] | (p[1] << 8));
}

void akao_opFEJumpIfGe(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (g_akaoSeq->unk6C >= v) {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
    } else {
        c->unk0 = p + 2;
    }
}

void akao_opFECall(AkaoChannel *c) {
    u8 *p = c->unk0;
    s16 off = p[0] | (p[1] << 8);
    c->unk14 = p + 2;
    c->unk0 += off;
}

void akao_opFEReturn(Unk8005F038 *a) {
    a->unk0 = a->unk14;
}

void akao_opA3(AkaoChannel *c) {
    c->unkB2 = *c->unk0++ << 8;
    c->voiceDirty |= 3;
}

void akao_opFE12(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->unk0++;
    ch->unkB4 = v;
    if (v == 0) {
        ch->unkB4 = 0x100;
    }
    p = ch->unk0;
    base = ch->unkB2 & 0x7F00;
    ch->unkFC = ((*p << 8) - base) / ch->unkB4;
    ch->unk0 = p + 1;
    v = base;
    ch->unkB2 = v;
}

void akao_opA8(AkaoChannel *c) {
    s32 v = (s8)*c->unk0++;
    c->unkB8 = 0;
    c->voiceDirty |= 3;
    c->unkBA = 0;
    c->unk78 = v << 23;
}

void akao_opA9(AkaoChannel *c) {
    s32 v;

    if ((c->unkB8 = *c->unk0++) == 0) {
        c->unkB8 = 0x100;
    }
    v = c->unk78 & 0xFFFF0000;
    c->unk7C = (((s8)*c->unk0++ << 23) - v) / (u16)c->unkB8;
    c->unk78 = v;
    c->unkBA = 0;
}

void akao_opFE19(AkaoChannel *c) {
    c->unk8C = (s8)*c->unk0++ << 23;
    if ((c->unkBA = *c->unk0++) == 0) {
        c->unkBA = 0x100;
    }
    c->unk90 = (((s8)*c->unk0++ << 23) - c->unk8C) / (u16)c->unkBA;
}

void akao_opFE1A(AkaoChannel *c) {
    c->unk34 |= 0x40;
}

void akao_opFE1B(AkaoChannel *c) {
    c->unk34 &= ~0x40;
}

void akao_opAA(AkaoChannel *ch) {
    u8 *p = ch->unk0;
    s32 v = *p;
    ch->unk0 = p + 1;
    ch->unkC0 = 0;
    ch->voiceDirty |= 3;
    ch->unkBE = ((v + 0x40) & 0xFF) << 8;
}

void akao_opAB(AkaoChannel *ch) {
    s32 v;
    u8 *p;
    u16 base;

    v = *ch->unk0++;
    ch->unkC0 = v;
    if (v == 0) {
        ch->unkC0 = 0x100;
    }
    p = ch->unk0;
    base = ch->unkBE & 0xFF00;
    ch->unk104 = ((((*p + 0x40) & 0xFF) << 8) - base) / ch->unkC0;
    ch->unk0 = p + 1;
    v = base;
    ch->unkBE = v;
}

void akao_opOctave(AkaoChannel *c) {
    c->unkC4 = *c->unk0++;
}

void akao_opOctaveUp(AkaoChannel *c) {
    c->unkC4 = (c->unkC4 + 1) & 0xF;
}

void akao_opOctaveDown(AkaoChannel *c) {
    c->unkC4 = (c->unkC4 - 1) & 0xF;
}

void akao_opProgram(AkaoChannel *arg0) {
    s32 i;
    Unk8005EBB4 *e;
    s32 b = *arg0->unk0++;

    if (arg0->unk94 == 0) {
        i = func_8005DD54(g_akaoSeq->unk0, b);
    } else {
        i = func_8005EC8C(arg0->unk38, b);
    }
    e = &D_8007F970[i];
    func_8005EBB4(arg0, e, e->unk0);
    arg0->unk9A = i;
    arg0->voiceVolScale = 0;
    arg0->unk34 &= 0xE6FFEFF7;
}

void akao_opFE0A(AkaoChannel *c) {
    s32 i = func_8005DD54(g_akaoSeq->unk0, *c->unk0++);
    func_8005EBB4(c, &D_8007F970[i], 0x1010);
    c->unk9A = i;
    c->voiceVolScale = 0;
    c->unk34 &= 0xE6FFEFF7;
}

void akao_opFE14(AkaoChannel *c) {
    u8 v = *c->unk0++;
    u16 *tbl = g_akaoSeq->unk30;
    if (tbl != NULL) {
        if (tbl[v] > 0x8000) {
            c->voiceVolScale = 0;
            c->unk34 &= ~0x1000;
        } else {
            u8 *p = (u8 *)tbl + tbl[v];
            c->unk10A = 0xFF;
            c->unk18 = p + 0x20;
            c->unk34 = (c->unk34 & 0xE6FFEFF7) | 0x1000;
        }
    }
}

void akao_opB3(AkaoChannel *c) {
    Unk8005EBB4 *e = &D_8007F970[c->unk9A];
    c->voiceAdsr1 = e->unkC;
    c->voiceAdsr2 = e->unkE;
    c->voiceDirty |= 0xFF00;
    c->unk34 &= 0xE6FFFFFF;
}

void akao_opC0(AkaoChannel *c) {
    c->unk106 = (s8)*c->unk0++;
}

void akao_opC1(AkaoChannel *c) {
    c->unk106 += (s8)*c->unk0++;
}

void akao_opA4(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkC6 = v;
    if (v == 0) {
        c->unkC6 = 0x100;
    }
    c->unk10C = (s8)*c->unk0++;
}

void akao_opDA(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkCA = v;
    if (v == 0) {
        c->unkCA = 0x100;
    }
    c->unk10E = 0;
    c->unkC8 = 0;
    c->unkCC = 1;
}

void akao_opDB(AkaoChannel *a) {
    a->unkCA = 0;
}

void akao_opD8(AkaoChannel *c) {
    u8 *p = c->unk0;
    u32 k;
    u32 prod;
    u32 r;
    c->unk108 = (s8)*p;
    k = c->unk2C;
    r = (u8)c->unk108;
    prod = k * r;
    c->unk0 = p + 1;
    if (c->unk108 < 0) {
        r = (prod >> 8) - k;
    } else {
        r = prod >> 7;
    }
    c->unk84 = r;
    c->voiceDirty |= 0x10;
}

void akao_opD9(AkaoChannel *c) {
    u32 r;

    c->unk108 += (s8)*c->unk0++;
    r = c->unk2C * (u8)c->unk108;
    if (c->unk108 < 0) {
        r = (r >> 8) - c->unk2C;
    } else {
        r >>= 7;
    }
    c->unk84 = r;
    c->voiceDirty |= 0x10;
}

void akao_opB4(AkaoChannel *c) {
    u32 b;
    s16 v;
    u32 n;
    u32 r;
    u32 k;
    u32 t;

    c->unk34 |= 1;
    if (c->unk94 != 0) {
        c->unkCE = 0;
        b = *c->unk0++;
        if (b != 0) {
            c->unkD6 = b << 8;
        }
    } else {
        c->unkCE = *c->unk0++;
    }
    c->unk3C = t = *c->unk0++ << 12;
    if (t == 0) {
        c->unk3C = 0x100000;
    }
    b = *c->unk0++;
    v = c->unkD6;
    k = (u16)c->unk2C;
    n = (v & 0x7F00) >> 8;
    if (!(v & 0x8000)) {
        r = (n * ((s32)(k * 15) >> 8)) >> 7;
    } else {
        r = (n * k) >> 7;
    }
    c->unkD4 = r;
    c->unkD2 = 0;
    c->unkD0 = c->unkCE;
    c->unk1C = D_8006FB40[b & 7];
    c->unk48 = 0x1000000 / ((u32)c->unk3C >> 12);
}

void akao_opB5(AkaoChannel *c) {
    s32 k;
    s16 v;
    u32 n;
    u32 r;

    c->unkD6 = *c->unk0++ << 8;
    k = c->unk2C;
    v = c->unkD6;
    n = (v & 0x7F00) >> 8;
    if (!(v & 0x8000)) {
        r = n * ((s32)(k * 15) >> 8);
    } else {
        r = n * k;
    }
    c->unkD4 = r >> 7;
}

void akao_opDD(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkDA = ((*c->unk0++ << 8) - c->unkD6) / n;
    c->unkD8 = n;
}

void akao_opE4(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkD2 = v;
    c->unk40 = func_8005EB40(&c->unk3C, *c->unk0++, v, 0xC);
}

void akao_opB6(AkaoChannel *c) {
    c->unk110 = 0;
    c->unk34 &= ~1;
    c->voiceDirty |= 0x10;
}

void akao_opB8(AkaoChannel *c) {
    s32 v;
    u32 d;

    c->unk34 |= 2;
    v = *c->unk0++;
    if (c->unk94 != 0) {
        c->unkDC = 0;
        if (v != 0) {
            c->unkE2 = (v & 0x7F) << 8;
        }
    } else {
        c->unkDC = v;
    }
    c->unk4C = *c->unk0++ << 12;
    if (c->unk4C == 0) {
        c->unk4C = 0x100000;
    }
    v = *c->unk0++;
    d = 0x1000000 / ((u32)c->unk4C >> 12);
    c->unkDE = c->unkDC;
    c->unkE0 = 0;
    c->unk20 = D_8006FB40[v & 7];
    c->unk58 = d;
}

void akao_opB9(AkaoChannel *c) {
    c->unkE2 = (*c->unk0++ & 0x7F) << 8;
}

void akao_opDE(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkE6 = (((*c->unk0++ & 0x7F) << 8) - c->unkE2) / n;
    c->unkE4 = n;
}

void akao_opE5(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkE0 = v;
    c->unk50 = func_8005EB40(&c->unk4C, *c->unk0++, v, 0xC);
}

void akao_opBA(AkaoChannel *c) {
    c->unk112 = 0;
    c->unk34 &= ~2;
    c->voiceDirty |= 3;
}

void akao_opBC(AkaoChannel *arg0) {
    arg0->unk34 |= 4;
    arg0->unk5C = *arg0->unk0++ << 12;
    if (arg0->unk5C == 0) {
        arg0->unk5C = 0x100000;
    }
    {
        u32 d = 0x1000000 / ((u32)arg0->unk5C >> 12);
        u8 b = *arg0->unk0++;
        arg0->unkE8 = 0;
        arg0->unk64 = 0;
        arg0->unk68 = d;
        arg0->unk24 = D_8006FB40[b & 7];
    }
}

void akao_opBD(AkaoChannel *c) {
    c->unkEA = *c->unk0++ << 7;
}

void akao_opDF(AkaoChannel *c) {
    s32 n = *c->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    c->unkEE = ((*c->unk0++ << 7) - c->unkEA) / n;
    c->unkEC = n;
}

void akao_opE6(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v == 0) {
        v = 0x100;
    }
    c->unkE8 = v;
    c->unk60 = func_8005EB40(&c->unk5C, *c->unk0++, v, 0xC);
}

void akao_opBE(AkaoChannel *c) {
    c->unk114 = 0;
    c->unk34 &= ~4;
    c->voiceDirty |= 3;
}

void akao_opC4(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk3C |= mask;
    } else {
        D_80080A70.unk1C |= mask;
    }
    D_80083158.unk8 |= 0x110;
}

void akao_opC5(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk3C &= ~mask;
    } else {
        D_80080A70.unk1C &= ~mask;
    }
    D_80083158.unk8 |= 0x110;
    c->unkF0 = 0;
}

void akao_opC6(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk44 |= mask;
    } else if (c->unk34 & 0x10000) {
        D_80080A70.unk24 |= mask;
    }
    D_80083158.unk8 |= 0x100;
}

void akao_opC7(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk44 &= ~mask;
    } else {
        D_80080A70.unk24 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
    c->unkF2 = 0;
}

void akao_opC2(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk40 |= mask;
    } else {
        D_80080A70.unk20 |= mask;
    }
    D_80083158.unk8 |= 0x100;
}

void akao_opC3(AkaoChannel *c, u32 mask) {
    if (c->unk94 == 0) {
        g_akaoSeq->unk40 &= ~mask;
    } else {
        D_80080A70.unk20 &= ~mask;
    }
    D_80083158.unk8 |= 0x100;
}

void akao_opCC(AkaoChannel *a) {
    a->unkCC = 1;
}

void akao_opCD(void) {
}

void akao_opD0(AkaoChannel *c) {
    if (c->unk94 != 0) {
        c->unkCC = 4;
    }
}

void akao_opD1(void) {
}

void akao_opAC(AkaoChannel *arg0) {
    u32 b = *arg0->unk0++;

    if (arg0->unk94 == 0) {
        if (b & 0xC0) {
            g_akaoSeq->unk6E = (g_akaoSeq->unk6E + (b & 0x3F)) & 0x3F;
        } else {
            g_akaoSeq->unk6E = b;
        }
    } else if (b & 0xC0) {
        D_80080A70.unk28 = (D_80080A70.unk28 + (b & 0x3F)) & 0x3F;
    } else {
        D_80080A98 = b;
    }
    D_80083158.unk8 |= 0x10;
}

void akao_opAD(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->voiceDirty |= 0x900;
    c->unk34 |= 0x1000000;
    c->voiceAdsr1 = (c->voiceAdsr1 & 0x80FF) | (v << 8);
}

void akao_opAE(AkaoChannel *c, s32 arg1) {
    u16 t = c->voiceAdsr1 & 0xFF0F;
    c->voiceAdsr1 = t | (*c->unk0++ << 4);
    c->voiceDirty |= 0x1000;
}

void akao_opAF(AkaoChannel *c, s32 arg1) {
    u16 t = c->voiceAdsr1 & 0xFFF0;
    c->voiceAdsr1 = t | *c->unk0++;
    c->voiceDirty |= 0x8000;
}

void akao_opB1(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->voiceDirty |= 0x2200;
    c->unk34 |= 0x8000000;
    c->voiceAdsr2 = (c->voiceAdsr2 & 0xE03F) | (v << 6);
}

void akao_opB2(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->voiceDirty |= 0x4400;
    c->unk34 |= 0x10000000;
    c->voiceAdsr2 = (c->voiceAdsr2 & 0xFFE0) | v;
}

void akao_opB7(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->voiceAdsr1 &= 0x7FFF;
    if (v == 5) {
        c->voiceAdsr1 |= 0x8000;
    }
    c->voiceDirty |= 0x100;
}

void akao_opBB(AkaoChannel *c) {
    u16 v = *c->unk0++;
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
    s32 v = *c->unk0++;
    c->voiceAdsr2 &= 0xFFDF;
    if (v == 7) {
        c->voiceAdsr2 |= 0x20;
    }
    c->voiceDirty |= 0x400;
}

void akao_opFE10(AkaoChannel *c) {
    g_akaoSeq->unk38 = *c->unk0++;
}

void akao_opFE11(void) {
    g_akaoSeq->unk38 = 0;
}

void akao_opC8(AkaoChannel *c) {
    c->unkF4 = (c->unkF4 + 1) & 3;
    c->unk4[c->unkF4] = c->unk0;
    c->unkA2[c->unkF4] = 0;
    c->unkAA[c->unkF4] = c->unkA0;
}

void akao_opC9(AkaoChannel *arg0) {
    u32 n = *arg0->unk0++;

    if (n == 0) {
        n = 0x100;
    }
    if (++arg0->unkA2[arg0->unkF4] != n) {
        arg0->unk0 = arg0->unk4[arg0->unkF4];
        arg0->unkA0 = arg0->unkAA[arg0->unkF4];
        return;
    }
    arg0->unkF4 = (arg0->unkF4 - 1) & 3;
}

void akao_opFE08(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->unkA2[c->unkF4] + 1 != v) {
        c->unk0 = p + 2;
    } else {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
    }
}

void akao_opFE09(AkaoChannel *c) {
    u8 *p = c->unk0;
    s32 v = *p++;
    c->unk0 = p;
    if (v == 0) {
        v = 0x100;
    }
    if (c->unkA2[c->unkF4] + 1 != v) {
        c->unk0 = p + 2;
    } else {
        c->unk0 = p + (s16)(p[0] | (p[1] << 8));
        c->unkF4 = (c->unkF4 - 1) & 3;
    }
}

void akao_opCA(AkaoChannel *c) {
    c->unkA2[c->unkF4]++;
    c->unk0 = c->unk4[c->unkF4];
    c->unkA0 = c->unkAA[c->unkF4];
}

void akao_opA2(AkaoChannel *c) {
    s32 v = *c->unk0++;
    c->unkFA = 0;
    c->unk98 = v;
    c->unk96 = v;
    c->unkF8 = v;
}

void akao_opDC(AkaoChannel *c) {
    s32 v = (s8)*c->unk0++;
    if (v != 0) {
        v += (s16)c->unkF8;
        if (v < 1) {
            v = 1;
        } else if (v >= 0x100) {
            v = 0xFF;
        }
    }
    c->unkFA = v;
}

void akao_opFE04(AkaoChannel *c) {
    if (g_akaoSeq->unk34 != 0) {
        c->unk34 = (c->unk34 & 0xE6FFEFF7) | 8;
    }
}

void akao_opFE05(AkaoChannel *c) {
    c->voiceVolScale = 0;
    c->unk34 &= ~8;
}

void akao_opFE15(AkaoChannel *c) {
    Unk80080A10 *d = g_akaoSeq;
    d->unk74 = *c->unk0++;
    d->unk70 = *c->unk0++;
    d->unk76 = 0;
    d->unk72 = 0;
}

void akao_opFE16(AkaoChannel *c) {
    Unk80080A10 *d = g_akaoSeq;
    d->unk78 = *c->unk0++;
    d->unk78 |= *c->unk0++ << 8;
}

void akao_opB0(AkaoChannel *c, s32 b) {
    akao_opAE(c, b);
    akao_opAF(c, b);
}

void akao_opCE(AkaoChannel *c, u32 mask) {
    s32 v = *c->unk0++;
    c->unkF0 = v ? v + 1 : 0x101;
    akao_opC4(c, mask);
}

void akao_opCF(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v != 0) {
        c->unkF0 = v + 1;
    } else {
        c->unkF0 = 0x101;
    }
}

void akao_opD2(AkaoChannel *c, u32 mask) {
    s32 v = *c->unk0++;
    c->unkF2 = v ? v + 1 : 0x101;
    akao_opC6(c, mask);
}

void akao_opD3(AkaoChannel *c) {
    s32 v = *c->unk0++;
    if (v != 0) {
        c->unkF2 = v + 1;
    } else {
        c->unkF2 = 0x101;
    }
}

void akao_opCB(AkaoChannel *c, u32 mask) {
    c->unk34 &= ~0x37;
    akao_opC5(c, mask);
    akao_opC7(c, mask);
    akao_opC3(c, mask);
    c->unkCC &= ~5;
}

void akao_opD4(AkaoChannel *c) {
    c->unk34 |= 0x10;
}

void akao_opD5(AkaoChannel *c) {
    c->unk34 &= ~0x10;
}

void akao_opD6(AkaoChannel *c) {
    c->unk34 |= 0x20;
}

void akao_opD7(AkaoChannel *c) {
    c->unk34 &= ~0x20;
}

static inline u8 *akao_opFE0B_ref(u8 *p) {
    s32 o = (p[1] << 8) | p[0];
    return o != 0 ? p + o + 2 : NULL;
}

void akao_opFE0B(AkaoChannel *c) {
    u8 *p = c->unk0;
    u8 *a;
    u8 *b;

    a = akao_opFE0B_ref(p);
    p += 2;
    b = akao_opFE0B_ref(p);
    D_800809F0.unk0 = 0;
    D_800809F0.unk4 = 0;
    D_800809F0.unk8 = c->unkBE >> 8;
    D_800809F0.unkC = c->unk78 >> 23;
    func_8005A014(&D_800809F0, a, b, 0);
    c->unk0 += 4;
}

void akao_opE0(AkaoChannel *c) {
    c->unk34 |= 0x100000;
}

void akao_opFE1C(AkaoChannel *c) {
    c->unk0++;
}

void akao_opFE1D(AkaoChannel *c, s32 b) {
    g_akaoSeq->unk8 |= b;
}

void akao_opFE1E(AkaoChannel *c, s32 b) {
    g_akaoSeq->unk8 &= ~b;
}

void akao_opE1(AkaoChannel *c) {
    c->unkF6 = *c->unk0++;
}

void akao_opE2(AkaoChannel *a) {
    a->unkF6 = 0;
}

void akao_opInvalid(AkaoChannel *c, s32 b) {
    akao_opEnd(c, b);
}

void func_80060938(void) {
    u32 m;
    if (D_80083168.unkC != 0) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        akao_keyOff(D_80083168.unkC);
        if (D_80083168.unk8 & 0x400) {
            akao_setVoiceLoopAddr(D_80083168.unk10 - 2, 0x1030);
            akao_setVoiceLoopAddr(D_80083168.unk10 - 1, 0x1030);
        }
        akao_setVoiceLoopAddr(D_80083168.unk10, 0x1030);
        akao_setVoiceLoopAddr(D_80083168.unk10 + 1, 0x1030);
        m = D_80083168.unkC;
        D_80083168.unkC = 0;
        D_80080A70.unk20 &= ~m;
        D_80083158.unk8 |= 0x100;
    }
}

s32 func_80060A08(void) {
    u32 mask;
    s32 i;
    u32 bit;

    do {
        bit = 0xC00000;
        mask = D_80080A70.unk0 | D_80080A70.unk10;
        for (i = 15; i != 0; i--, bit >>= 1) {
            if (!(mask & bit)) {
                break;
            }
        }
        if (i != 0) {
            return i + 7;
        }
        akao_stopSeChannels(0, 0x40000000);
    } while (mask != (D_80080A70.unk0 | D_80080A70.unk10));
    return -1;
}

void func_80060A98(void) {
    u8 *p = D_80083168.unk0 + 0x800;
    SpuSetTransferStartAddr(0x2100);
    SpuSetTransferCallback(func_80060FA8);
    SpuWrite(p, 0x800);
}

void func_80060AE0(s32 addr, s32 vol, s32 flag) {
    StreamHeader *h;
    s32 v;
    s32 mask;
    s32 size;
    s32 t;

    v = func_80060A08();
    if (v == -1) {
        return;
    }
    D_80083168.unk4C = vol;
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)addr;
    D_80083168.unk0 = (u8 *)h + 0x40;
    D_80083168.unk14 = h->unk10;
    mask = (1 << v) | (1 << (v + 1));
    t = h->unk20;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    D_80083168.unk18 = t;
    akao_keyOff(mask);
    D_80083168.unk8 = h->unk18;
    D_80083168.unk58 = h->unk1C;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    akao_keyOff(mask);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(0x1100);
    g_akaoSpuBusy = 1;
    D_80083168.unk4 = (D_80083168.unk8 & 2) ? (s32)(D_80083168.unk0 + h->unk14) : 0;
    if (D_80083168.unk8 & 1) {
        D_80083168.unk1C = (D_80083168.unk8 & 2) ? D_80083168.unk14 - (h->unk14 >> 1) : 0;
        SpuSetTransferCallback(func_8006100C);
        size = 0x2000;
    } else {
        D_80083168.unk1C = (D_80083168.unk8 & 2) ? D_80083168.unk14 - h->unk14 : 0;
        SpuSetTransferCallback(func_80060A98);
        size = 0x800;
    }
    SpuWrite(D_80083168.unk0, size);
    if (flag) {
        D_80080A70.unk20 |= D_80083168.unkC;
    } else {
        D_80080A70.unk20 &= ~D_80083168.unkC;
    }
    D_80080A70.unk24 &= ~D_80083174;
    D_80080A70.unk1C &= ~D_80083174;
    D_80083158.unk8 |= 0x100;
}

void func_80060D1C(s32 v, s32 mode, s32 a, s32 b) {
    s16 l;
    s16 r;
    s16 h;
    s32 pan;

    if (D_8008315C & 2) {
        l = r = (D_800831A8 * D_8006F640) >> 16;
    } else if (mode == 1) {
        r = 0;
        l = (u32)D_800831A8 >> 1;
    } else if (mode == 2) {
        l = 0;
        r = (u32)D_800831A8 >> 1;
    } else if (mode == 3) {
        h = D_800831A8 >> 1;
        l = r = (h >> 1) + (h >> 2);
    } else {
        pan = (u8)(D_80083168.unk4C >> 8);
        l = (u32)(D_80083168.unk40 * D_8006F540[pan]) >> 16;
        r = (u32)(D_80083168.unk40 * D_8006F540[pan ^ 0xFF]) >> 16;
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

void func_80060ED8(s32 a, s32 b, void (*cb)(void)) {
    if (D_80083168.unkC != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (D_80083168.unk14 > 0x1000) {
            D_80083168.unk14 -= 0x1000;
            D_80083168.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(func_80060938);
            b = 0x1030;
            D_80083168.unk14 = 0;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(D_80083168.unkC);
        SpuSetIRQ(1);
    }
}

void func_80060FA8(void) {
    func_80060D1C(D_80083168.unk10, 0, 0x1100, 0x2100);
    func_80060D1C(D_80083168.unk10 + 1, 0, 0x1100, 0x2100);
    func_80060ED8(0x1000, 0x2100, func_800611BC);
}

void func_8006100C(void) {
    func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x2100);
    func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x2900);
    func_80060ED8(0x2000, 0x2100, func_8006121C);
}

void func_80061070(s32 a, s32 b, s32 n, void (*cb)(void)) {
    if (D_80083168.unkC == 0 || D_80083168.unk14 == 0) {
        return;
    }
    SpuSetTransferStartAddr(a);
    akao_beginSpuTransfer();
    SpuWrite(D_80083168.unk0, n);
    SpuSetIRQ(0);
    if (D_80083168.unk14 > 0x800) {
        SpuSetIRQCallback(cb);
        D_80083168.unk14 -= 0x800;
        D_80083168.unk0 += n;
    } else if (D_80083168.unk4 != NULL) {
        SpuSetIRQCallback(cb);
        D_80083168.unk0 = D_80083168.unk4;
        D_80083168.unk14 = D_80083168.unk1C;
    } else {
        SpuSetIRQCallback(func_80060938);
        b = 0x1030;
        a = b;
        D_80083168.unk14 = 0;
    }
    akao_setVoiceLoopAddr(D_80083168.unk10, a);
    akao_setVoiceLoopAddr(D_80083168.unk10 + 1, b);
    SpuSetIRQAddr(a + 8);
    SpuSetIRQ(1);
}

void func_800611BC(void) {
    func_80061070(0x1100, 0x1100, 0x800, func_800611EC);
}

void func_800611EC(void) {
    func_80061070(0x2100, 0x2100, 0x800, func_800611BC);
}

void func_8006121C(void) {
    func_80061070(0x1100, 0x1900, 0x1000, func_8006124C);
}

void func_8006124C(void) {
    func_80061070(0x2100, 0x2900, 0x1000, func_8006121C);
}

void akao_cmdE0(s32 *a) {
    func_80060938();
    func_80060AE0(a[0], a[1], a[2]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

void akao_cmdE2(void) {
    func_80060938();
}

void akao_cmdE4(s32 *a) {
    Unk80083168 *d = &D_80083168;
    s32 v = *a;
    s32 l;
    s32 r;
    s32 pan;

    d->unk48 = 0;
    d->unk40 = v;
    if (d->unkC == 0) {
        return;
    }
    if (D_8008315C & 2) {
        v = v * D_8006F640 >> 16;
        akao_setVoiceVolume(d->unk10, v, v, 0);
        akao_setVoiceVolume(d->unk10 + 1, v, v, 0);
    } else if (d->unk8 & 1) {
        v <<= 15;
        v >>= 16;
        akao_setVoiceVolume(d->unk10, v, 0, 0);
        akao_setVoiceVolume(d->unk10 + 1, 0, v, 0);
    } else {
        pan = (u8)(d->unk4C >> 8);
        l = v * D_8006F540[pan] >> 16;
        r = v * D_8006F540[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->unk10, l, r, 0);
        akao_setVoiceVolume(d->unk10 + 1, l, r, 0);
    }
}

void akao_cmdE5(Unk80080AA0 *p) {
    s16 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_80083168.unk44 = (s16)((s16)((u16)p->unk4 - (u16)D_80083168.unk40) / n);
    D_80083168.unk48 = n;
}

void akao_cmdE6(s32 *a) {
    Unk80083168 *d = &D_80083168;
    s32 p = *a;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    d->unk4C = p;
    if (d->unkC == 0) {
        return;
    }
    if (D_8008315C & 2) {
        v = d->unk40 * D_8006F640 >> 16;
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
        l = d->unk40 * D_8006F540[pan] >> 16;
        r = d->unk40 * D_8006F540[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->unk10, l, r, 0);
        akao_setVoiceVolume(d->unk10 + 1, l, r, 0);
    }
}

void func_800615F8(void) {
    SpuSetIRQAddr(0x1038);
    SpuSetIRQCallback(func_80060938);
}

void func_80061624(void) {
    u32 irq;
    if (D_80083168.unk14 > 0x2000) {
        SpuSetTransferStartAddr(D_80083168.unk2C + 0x2000);
        akao_spuWrite(D_80083168.unk0, D_80083168.unk14 - 0x2000);
        irq = D_80083168.unk2C + 0x1FF8;
    } else {
        irq = D_80083168.unk2C + (D_80083168.unk14 >> 1) + 8;
    }
    func_80060D1C(D_80083168.unk10, 0, D_80083168.unk2C, 0x1030);
    func_80060D1C(D_80083168.unk10 + 1, 0, D_80083168.unk2C, 0x1030);
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    akao_keyOn(D_80083168.unkC);
    SpuSetIRQ(1);
}

void func_800616F8(s32 arg0, s32 b, s32 addr, s32 loop) {
    AkaoStream *h = (AkaoStream *)arg0;
    Unk80083168 *d;
    s32 v;
    u32 n;
    u32 m;

    v = func_80060A08();
    if (v != -1) {
        SpuSetIRQ(0);
        SpuSetIRQCallback(NULL);
        d = &D_80083168;
        d->unk4C = b;
        D_80083168.unk0 = h->unk40;
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
        SpuWrite(D_80083168.unk0, n);
        D_80083168.unk0 += n;
        if (loop) {
            D_80080A70.unk20 |= d->unkC;
        } else {
            D_80080A70.unk20 &= ~d->unkC;
        }
        D_80080A70.unk24 &= ~D_80083174;
        D_80080A70.unk1C &= ~D_80083174;
        D_80083158.unk8 |= 0x100;
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
    v = func_80060A08();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    mask = (1 << v) | (1 << (v + 1));
    old = D_80083168.unkC;
    D_80083168.unk10 = v;
    D_80083168.unkC = mask;
    akao_keyOff(mask | old);
    h2 = h;
    D_80083168.unk8 = h2[6];
    D_80083168.unk58 = (u16)h2[7];
    if (flag) {
        D_80080A70.unk20 |= D_80083168.unkC;
    } else {
        D_80080A70.unk20 &= ~D_80083168.unkC;
    }
    irq = h2[8] + ((u32)h2[4] >> 1) + 8;
    D_80080A70.unk24 &= ~D_80083168.unkC;
    D_80080A70.unk1C &= ~D_80083168.unkC;
    D_80083158.unk8 |= 0x100;
    SpuSetIRQAddr(irq);
    SpuSetIRQCallback(func_800615F8);
    if (h2[6] & 1) {
        if (h2[6] & 2) {
            func_80060D1C(D_80083168.unk10, 1, h2[8], h2[8] + h2[5]);
            func_80060D1C(D_80083168.unk10 + 1, 2, h2[8] + h2[9], h2[8] + h2[9] + h2[5]);
        } else {
            func_80060D1C(D_80083168.unk10, 1, h2[8], 0x1030);
            func_80060D1C(D_80083168.unk10 + 1, 2, h2[8] + h2[9], 0x1030);
        }
    } else {
        if (h2[6] & 2) {
            func_80060D1C(D_80083168.unk10, 3, h2[8], h2[8] + h2[5]);
            func_80060D1C(D_80083168.unk10 + 1, 3, h2[8], h2[8] + h2[5]);
        } else {
            func_80060D1C(D_80083168.unk10, 3, h2[8], 0x1030);
            func_80060D1C(D_80083168.unk10 + 1, 3, h2[8], 0x1030);
        }
    }
    akao_keyOn(D_80083174);
    SpuSetIRQ(1);
}

void akao_cmdEC(s32 *a) {
    func_80060938();
    func_800616F8(a[0], a[1], a[2], a[3]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

void akao_cmdED(s32 *a) {
    func_80060938();
    func_800618C0(a[0], a[1]);
    D_80080A70.unk0 &= ~D_80083168.unkC;
}

/* K&R definition: callers pass one or two arguments */
s32 func_80061BC8(p)
    s32 *p;
{
    D_80083168.unk28++;
    (*p)++;
    if (D_80083168.unk3C - 1 < *p) {
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

    v = func_80060A08();
    if (v == -1) {
        return;
    }
    SpuSetIRQ(0);
    SpuSetIRQCallback(NULL);
    h = (StreamHeader *)(D_80083168.unk2C + 0x80);
    if (h->unk28 != 0 && D_80083168.unk48 == 0) {
        vol = D_80083168.unk40;
        D_80083168.unk40 = 0;
        akao_sendCmdE5(h->unk28, vol >> 8);
    }
    d = &D_80083168;
    D_80083168.unk0 = (u8 *)d->unk2C;
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
        SpuWrite(D_80083168.unk0 + 0xD0, 0x4000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
        d->unkC |= (1 << (v - 2)) | (1 << (v - 1));
    } else {
        SpuWrite(D_80083168.unk0 + 0xD0, 0x2000);
        func_80061BC8(&d->unk34);
        func_80061BC8(&d->unk34);
    }
    D_80080A70.unk20 &= ~D_80083174;
    D_80080A70.unk24 &= ~D_80083174;
    D_80080A70.unk1C &= ~D_80083174;
    D_80083158.unk8 |= 0x100;
}

void func_80061E1C(s32 a, s32 b, void (*cb)(void)) {
    if (D_80083168.unkC != 0) {
        SpuSetTransferCallback(NULL);
        g_akaoSpuBusy = 0;
        if (D_80083168.unk14 >= 0xE61) {
            D_80083168.unk0 += a;
            SpuSetIRQCallback(cb);
        } else {
            SpuSetIRQCallback(func_80060938);
            b = 0x1030;
        }
        SpuSetIRQAddr(b + 8);
        akao_keyOn(D_80083168.unkC);
        SpuSetIRQ(1);
    }
}

void func_80061EE4(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x3100);
        func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x3900);
        func_80060D1C(D_80083168.unk10 - 2, 1, 0x2100, 0x4100);
        func_80060D1C(D_80083168.unk10 - 1, 2, 0x2900, 0x4900);
        a = 0x4000;
        b = 0x3100;
    } else {
        func_80060D1C(D_80083168.unk10, 1, 0x1100, 0x2100);
        func_80060D1C(D_80083168.unk10 + 1, 2, 0x1900, 0x2900);
        a = 0x2000;
        b = 0x2100;
    }
    func_80061E1C(a, b, func_80062118);
}

s32 func_80061FBC(s32 a, s32 b, s32 n, void (*cb)(void)) {
    AkaoStream *h;

    if (D_80083168.unkC != 0) {
        h = (AkaoStream *)(D_80083168.unk0 + 0x80);
        if (h->unk0 == 0x4F414B41) {
            SpuSetIRQ(0);
            SpuSetTransferStartAddr(a);
            akao_beginSpuTransfer();
            func_80061BC8(&D_80083168.unk34);
            SpuWrite(D_80083168.unk0 + 0xD0, n - 0xD0);
            D_80083168.unk20 = h->unk4;
            D_80083168.unk18 = h->unk20;
            if (h->unk10 > h->unk20) {
                SpuSetIRQCallback(cb);
                D_80083168.unk0 += n;
                if (D_80083168.unk34 == 0) {
                    D_80083168.unk0 = (u8 *)D_80083168.unk2C;
                }
            } else {
                SpuSetIRQCallback(func_80060938);
                b = 0x1030;
                a = b;
            }
            akao_setVoiceLoopAddr(D_80083168.unk10, a);
            akao_setVoiceLoopAddr(D_80083168.unk10 + 1, b);
            SpuSetIRQAddr(a + 8);
            SpuSetIRQ(1);
        }
    }
    return a;
}

void func_80062118(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80061BC8(&D_80083168.unk34);
        a = func_80061FBC(0x1100, 0x1900, 0x2000, func_800621D4);
        b = 0x2900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x2100;
        }
        akao_setVoiceLoopAddr(D_80083168.unk10 - 2, a);
        akao_setVoiceLoopAddr(D_80083168.unk10 - 1, b);
    } else {
        func_80061FBC(0x1100, 0x1900, 0x1000, func_800621D4);
    }
}

void func_800621D4(void) {
    s32 a;
    s32 b;
    if (D_80083168.unk8 & 0x400) {
        func_80061BC8(&D_80083168.unk34);
        a = func_80061FBC(0x3100, 0x3900, 0x2000, func_80062118);
        b = 0x4900;
        if (a == 0x1030) {
            b = 0x1030;
        } else {
            a = 0x4100;
        }
        akao_setVoiceLoopAddr(D_80083168.unk10 - 2, a);
        akao_setVoiceLoopAddr(D_80083168.unk10 - 1, b);
    } else {
        func_80061FBC(0x2100, 0x2900, 0x1000, func_80062118);
    }
}

void akao_cmdE8(s32 *a) {
    func_80060938();
    D_80083168.unk34 = -1;
    D_80083168.unk20 = 0;
    D_80083168.unk24 = 0;
    D_80083168.unk28 = 0;
    D_80083168.unk38 = 0;
    D_80083168.unk8 = 0x1000000;
    D_80083168.unk3C = (u32)g_akaoArgs.unk4 >> 12;
    D_80083168.unk2C = a[0];
    D_80083168.unk30 = a[1];
}


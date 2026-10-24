#include "ff9.h"

void func_80059548(AkaoChannel *arg0, u8 *arg1) {
    arg0->unkB2 = 0x6E00;
    arg0->unk78 = 0x23000000;
    arg0->unk0 = arg1;
    arg0->unk106 = 0;
    arg0->unk108 = 0;
    arg0->unkCA = 0;
    arg0->unk30 = 0;
    arg0->unk10C = 0;
    arg0->unkC2 = 0;
    arg0->unkFA = 0;
    arg0->unkF8 = 0;
    arg0->unkB8 = 0;
    arg0->unk84 = 0;
    arg0->unkF6 = 0;
    arg0->unkF4 = 0;
    arg0->unk34 = 0;
    arg0->unk114 = 0;
    arg0->unkCC = 0;
    arg0->unkA0 = 0xFFFF;
    arg0->voiceVolScale = 0;
    arg0->unkEA = 0;
    arg0->unkE2 = 0;
    arg0->unkD6 = 0;
    arg0->unkEC = 0;
    arg0->unkE4 = 0;
    arg0->unkD8 = 0;
    arg0->unkE8 = 0;
    arg0->unkE0 = 0;
    arg0->unkD2 = 0;
    arg0->unkF2 = 0;
    arg0->unkF0 = 0;
    func_8005EBE8(arg0, 0);
}

u32 func_800595F0(AkaoChannel *c, u32 mask) {
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

void func_8005963C(s32 addr, s32 mask) {
    AkaoSeqHeader *seq;
    u32 active;
    AkaoChannel *c;
    u16 *ofs;
    u32 bit;
    u32 i;
    u32 x;
    u8 *p;

    g_akaoSeq->unk2C = (u8 *)addr;
    active = ((AkaoSeqHeader *)addr)->unk20;
    seq = (AkaoSeqHeader *)addr;
    if (g_akaoSubSeq != NULL) {
        bit = func_800595F0(g_akaoSubChannels, g_akaoSubSeq->unk4);
    } else {
        bit = 0;
    }
    x = 0xFFFFFF;
    g_akaoSeState.unkC |= ~bit & (~(g_akaoSeState.unk0 | D_80083174) & x);
    g_akaoSeq->unk18 = 0;
    if (D_800831C4 & 1) {
        g_akaoSeq->unk4 = 0;
        g_akaoSeq->unk1C |= active & mask;
    } else {
        g_akaoSeq->unk1C = 0;
        g_akaoSeq->unk4 |= active & mask;
    }
    g_akaoSeq->unk8 = seq->unk24;
    g_akaoSeq->unkC = seq->unk28;
    g_akaoSeq->unk0 &= ~0x33;
    g_akaoSeq->unk0 = seq->unk14 == D_8007F8DC ? g_akaoSeq->unk0 | 0x20 : g_akaoSeq->unk0 | 0x10;
    p = (u8 *)seq + (seq->unk30 + 0x30);
    g_akaoSeq->unk30 = seq->unk30 != 0 ? (u16 *)p : NULL;
    p = (u8 *)seq + (seq->unk34 + 0x34);
    g_akaoSeq->unk34 = seq->unk34 != 0 ? (s32)p : 0;
    g_akaoSeq->unk38 = 0;
    bit = 1;
    i = 0;
    c = g_akaoChannels;
    ofs = (u16 *)(addr + 0x40);
    for (; i < 32; i++) {
        if (active & bit & mask) {
            c->unk0 = (u8 *)ofs + *ofs;
            c->unk96 = 4;
            c->unk98 = 2;
            c->unkB2 = 0x7F00;
            c->unk78 = 0x3FFF0000;
            c->unk100 = 0x4000;
            c->unk108 = 0;
            c->unk106 = 0;
            c->unkCA = 0;
            c->unk30 = 0;
            c->unk10C = 0;
            c->unkC2 = 0;
            c->unkFA = 0;
            c->unkF8 = 0;
            c->unkBE = 0x8000;
            c->unkC0 = 0;
            c->unkCA = 0;
            c->unkBC = 0;
            c->unkB8 = 0;
            c->unk84 = 0;
            c->unkBA = 0;
            c->unkF6 = 0;
            c->unkCC = 0;
            c->unk114 = 0;
            c->unkF4 = 0;
            c->unk34 = !(g_akaoSeq->unkC & bit) << 6;
            ofs++;
            c->unkEA = 0;
            c->unkE2 = 0;
            c->unkD6 = 0;
            c->unkEC = 0;
            c->unkE4 = 0;
            c->unkD8 = 0;
            c->unkE8 = 0;
            c->unkE0 = 0;
            c->unkD2 = 0;
            c->unkF2 = 0;
            c->unkF0 = 0;
            func_8005EBE8(c, 0);
        } else {
            if ((active & bit) && !(bit & mask)) {
                ofs++;
            }
            c->unk96 = 3;
            c->unk98 = 1;
            c->unk0 = D_8006F40C;
            c->voiceDirty |= 0x4400;
            c->voiceAdsr2 = (c->voiceAdsr2 & 0xFFE0) | 5;
        }
        c->voice = 0x18;
        active &= ~bit;
        c++;
        bit <<= 1;
    }
    g_akaoSeq->unk20.w = 0xFFFF0000;
    g_akaoSeq->unk28 = 1;
    g_akaoSeq->unk68 = 0;
    g_akaoSeq->unk48 = 0;
    g_akaoSeq->unk66 = 0;
    g_akaoSeq->unk4C = 0;
    g_akaoFlags.unk8 = 0;
    g_akaoSeq->unk76 = 0;
    g_akaoSeq->unk74 = 0;
    g_akaoSeq->unk72 = 0;
    g_akaoSeq->unk78 = 0;
    g_akaoSeq->unk3C = 0;
    g_akaoSeq->unk40 = 0;
    g_akaoSeq->unk44 = 0;
    g_akaoSeq->unk6C = 0;
    g_akaoSeq->unk14 = 0;
    g_akaoSeq->unk10 = 0;
    g_akaoFlags.unk8 |= 0x100;
}

void akao_stopSeq(Unk80080A10 *a, AkaoChannel *c, s32 id) {
    u32 i;
    if (a->unk4 != 0 && (id == 0 || id == a->unk6A)) {
        a->unk18 = -1;
        for (i = 32; i != 0; i--) {
            c->unk96 = 3;
            c->unk98 = 1;
            c->unk0 = D_8006F40C;
            c++;
        }
        a->unk6A = 0;
        a->unk14 = 0;
        a->unk10 = 0;
        for (i = 0; i < 24; i++) {
            if (D_800831C8[i] == a) {
                D_800831C8[i] = NULL;
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
    active = g_akaoSeState.unk0 | g_akaoSeState.unk10;
    if (mask & 0x0FFFFFFF) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && (c->unk28 & mask)) {
                if (c->unk34 & 0x100000) {
                    c->unk34 |= 0x200000;
                } else {
                    g_akaoSeState.unkC |= bit;
                    func_8005EC20(c, bit);
                    c->unk34 = 0;
                }
            }
        }
    } else if (mask < 0) {
        c = &g_akaoSeChannels[id];
        bit = 0x100 << id;
        if (active & bit) {
            akao_stopSeChannels(c->unk6C, 0);
        }
        bit <<= 1;
        c++;
        if (active & bit) {
            akao_stopSeChannels(c->unk6C, 0);
        }
        return;
    } else if (mask & 0x40000000) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (c->unk28 != 0) {
                active &= ~bit;
            }
        }
        c = g_akaoSeChannels;
        bit = 0x100;
        max = 0;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (max < c->unk88) {
                    max = c->unk88;
                }
            }
        }
        c = g_akaoSeChannels;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((active & bit) && max == c->unk88) {
                if (c->unk34 & 0x100000) {
                    c->unk34 |= 0x200000;
                } else {
                    g_akaoSeState.unkC |= bit;
                    func_8005EC20(c, bit);
                    c->unk34 = 0;
                }
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if (active & bit) {
                if (id == -1) {
                    if (c->unk6C < 0) {
                        if (c->unk34 & 0x100000) {
                            c->unk34 |= 0x200000;
                        } else {
                            g_akaoSeState.unkC |= bit;
                            func_8005EC20(c, bit);
                            c->unk34 = 0;
                        }
                    }
                } else if (c->unk6C == id) {
                    if (c->unk34 & 0x100000) {
                        c->unk34 |= 0x200000;
                    } else {
                        g_akaoSeState.unkC |= bit;
                        func_8005EC20(c, bit);
                        c->unk34 = 0;
                    }
                }
            }
        }
    }
    g_akaoFlags.unk8 |= 0x110;
}

void func_80059E10(AkaoChannel *c, VoiceParams *p, s32 mask, u8 *data) {
    AkaoChannel *ch;
    s32 bit;
    s32 n;

    c->unk6C = p->unk0;
    c->unk28 = p->unk4;
    c->unk9C = p->unk8 << 8;
    c->unk9E = 0;
    c->unkBE = 0x8000;
    c->unkC0 = 0;
    c->unk100 = (p->unkC & 0x7F) << 8;
    c->unkBC = 0;
    c->unk38 = p->unk10;
    c->unk96 = 2;
    c->unk98 = 1;
    c->unk94 = 1;
    c->unk88 = -2;
    c->unk70 = 0;
    c->unkB6 = 0;
    func_80059548(c, data);
    D_800831C8[c->voice] = NULL;
    akao_setVoiceRelease(c->voice, 5, 3);
    g_akaoSeState.unk0 |= mask;
    g_akaoSeState.unkC |= mask;
    mask = ~mask;
    g_akaoSeState.unk4 &= mask;
    g_akaoSeState.unk8 &= mask;
    g_akaoSeState.unk1C &= mask;
    g_akaoSeState.unk20 &= mask;
    g_akaoSeState.unk24 &= mask;
    if (D_800831C4 & 2) {
        mask = 0x100;
        c = g_akaoSeChannels;
        n = 16;
        do {
            if ((g_akaoSeState.unk0 & mask) && !(c->unk28 & 0x2000000)) {
                g_akaoSeState.unk0 &= ~mask;
                g_akaoSeState.unk10 |= mask;
            }
            n--;
            c++;
            mask <<= 1;
        } while (n != 0);
    }
}

void func_80059FB8(AkaoChannel *c, u32 voice) {
    u32 i;
    if (voice < 0x18) {
        for (i = 0; i < 32; i++) {
            if (c->voice == voice) {
                c->voice = 0x18;
                g_akaoSeq->unk14 &= ~(1 << i);
            }
            c++;
        }
    }
}

void func_8005A014(VoiceParams *p, u8 *a, u8 *b, s32 flag) {
    AkaoChannel *c;
    u32 bit;
    s32 n;
    u32 busy;

    if (a == NULL && b == NULL) {
        return;
    }
    if (flag == 0 && p->unk4 != 0) {
        akao_stopSeChannels(0, p->unk4);
    }
    do {
        c = &g_akaoSeChannels[15];
        bit = 0x800000;
        busy = g_akaoSeState.unk0 | g_akaoSeState.unk10 | g_akaoStream.unkC;
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
            if (busy == (g_akaoSeState.unk0 | g_akaoSeState.unk10 | g_akaoStream.unkC)) {
                return;
            }
        }
    } while (n == 0);
    if (a != NULL) {
        func_80059E10(c, p, bit, a);
        func_80059FB8(g_akaoChannels, c->voice);
    }
    if (b != NULL) {
        if (a != NULL) {
            c++;
            bit <<= 1;
        }
        func_80059E10(c, p, bit, b);
        func_80059FB8(g_akaoChannels, c->voice);
        if (a != NULL) {
            c->unk34 |= 0x10000;
        }
    }
    g_akaoFlags.unk8 |= 0x110;
}

static inline u8 *func_8005A20C_get(s32 i) {
    return D_800809DC[i] != 0xFFFF ? D_800809E8 + D_800809DC[i] : NULL;
}

void func_8005A20C(u8 **a, u8 **b, s32 idx) {
    idx &= 0x3FF;
    idx *= 2;
    *a = func_8005A20C_get(idx);
    idx++;
    *b = func_8005A20C_get(idx);
}

/* K&R definition: callers pass two or three arguments */
void func_8005A284(a, c)
    Unk80080A10 *a;
    AkaoChannel *c;
{
    u32 mask;
    u32 bit;
    if (a->unk4 != 0) {
        mask = a->unk4;
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

void func_8005A2CC(void) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (g_akaoSeState.unk0 != 0) {
        mask = g_akaoSeState.unk0;
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

void func_8005A320(s32 addr, s32 restore) {
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
        func_8005CE04(&g_akaoSavedSeq, g_akaoSeq, 0x7C);
        func_8005CE04(g_akaoSavedChannels, g_akaoChannels, 0x2680);
    } else {
        func_8005CE8C(&g_akaoSavedSeq, g_akaoSeq, 0x7C);
        func_8005CE8C(g_akaoSavedChannels, g_akaoChannels, 0x2680);
    }
    flags = g_akaoSeq->unk0 & ~0x30;
    g_akaoSeq->unk0 = flags;
    g_akaoSeq->unk0 = seq->unk14 == D_8007F8DC ? flags | 0x20 : flags | 0x10;
    c = g_akaoChannels;
    n = 32;
    old = g_akaoSeq->unk2C;
    g_akaoSeq->unk2C = (u8 *)seq;
    g_akaoSeq->unk10 = 0;
    bit = 1;
    g_akaoFlags.unk8 |= 0x90;
    d = (u8 *)seq - old;
    active = g_akaoSeq->unk4;
    g_akaoSeq->unk30 = (u16 *)((u8 *)g_akaoSeq->unk30 + d);
    g_akaoSeq->unk34 += d;
    g_akaoSeq->unk10 = g_akaoSeq->unk14;
    do {
        if (active & bit) {
            c->unk0 += d;
            c->unk18 += d;
            c->unk4[0] += d;
            c->unk4[1] += d;
            c->unk4[2] += d;
            c->unk4[3] += d;
            c->unk96 += 2;
            c->unk98 += 2;
            c->voiceDirty |= 0x1FF93;
            akao_shiftInstrumentUp(g_akaoSeq->unk0, c);
        } else {
            c->unk96 = 4;
            c->unk98 = 2;
            c->unk0 = D_8006F40C;
        }
        c->voice = 0x18;
        n--;
        c++;
        bit <<= 1;
    } while (n != 0);
    if (g_akaoSubSeq != NULL) {
        active = func_800595F0(g_akaoSubChannels, g_akaoSubSeq->unk4 & g_akaoSubSeq->unk8);
    } else {
        active = 0;
    }
    g_akaoSeq->unk18 = 0;
    x = 0xFFFFFF;
    g_akaoSavedSeq.unk6A = 0;
    g_akaoSeState.unkC |= ~active & (~(g_akaoSeState.unk0 | g_akaoStream.unkC) & x);
    g_akaoFlags.unk8 |= 0x100;
    if (D_800831C4 & 1) {
        g_akaoSeq->unk1C = g_akaoSeq->unk4;
        g_akaoSeq->unk4 = 0;
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
                if (a != D_800809C0[i - 1]) {
                    goto loop;
                }
                i = i - 1;
            }
        }
    }
    return i;
}

void akao_cmdPlayMusic(Unk80080AA0 *a) {
    if (g_akaoSavedSeq.unk6A != 0 && g_akaoSavedSeq.unk6A == a->unk8) {
        func_8005A320(a->unk0, 0);
    } else {
        func_8005963C(a->unk0, -1);
        g_akaoSeq->unk6A = a->unk8;
    }
}

void akao_cmdPlayMusicFrom(Unk80080AA0 *a) {
    if (g_akaoSavedSeq.unk6A != 0 && g_akaoSavedSeq.unk6A == a->unk8) {
        func_8005A320(a->unk0, 0);
    } else {
        func_8005963C(a->unk0, a->unkC);
        g_akaoSeq->unk6A = a->unk8;
    }
}

void akao_cmdSaveMusic(void) {
    u32 i;
    AkaoChannel *c;

    if (g_akaoSeq->unk4 != 0) {
        func_8005CE04(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        func_8005CE04(g_akaoChannels, g_akaoSavedChannels, 0x2680);
        for (i = 0, c = g_akaoSavedChannels; i < 0x20; i++, c++) {
            akao_shiftInstrumentDown(g_akaoSavedSeq.unk0, c);
        }
    }
}

void akao_cmdPushMusic(Unk80080AA0 *a) {
    Unk80080A10 *t;
    if (g_akaoSeq->unk4 != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->unk6A == 0)) {
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
        func_8005CE04(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
        func_8005CE04(g_akaoChannels, g_akaoSubChannels, 0x2680);
    }
    func_8005963C(a->unk0, -1);
    t = g_akaoSeq;
    t->unk50 = (a->unk10 & 0x7F) << 16;
    t->unk58 = 0;
    t->unk6A = a->unk8;
    func_80059414();
}

void akao_cmdCrossfadeMusic(Unk80080AA0 *a) {
    s32 d;

    if (g_akaoSavedSeq.unk6A != 0 && g_akaoSavedSeq.unk6A == a->unk8) {
        func_8005A320(a->unk0, 1);
        g_akaoSubSeq = &g_akaoSavedSeq;
        g_akaoSubChannels = g_akaoSavedChannels;
    } else {
        if (g_akaoSeq->unk4 != 0 && (g_akaoSubSeq == NULL || g_akaoSubSeq->unk6A == 0)) {
            g_akaoSubSeq = &g_akaoSavedSeq;
            g_akaoSubChannels = g_akaoSavedChannels;
            func_8005CE04(g_akaoSeq, &g_akaoSavedSeq, 0x7C);
            func_8005CE04(g_akaoChannels, g_akaoSubChannels, 0x2680);
        }
        func_8005963C(a->unk0, -1);
        g_akaoSeq->unk6A = a->unk8;
    }
    if (g_akaoSubSeq != NULL) {
        d = a->unk10;
        D_80080A18.unk0 = 0x7F8000;
        D_80080A18.unk8 = d;
        g_akaoFlags.unk4 |= 0x100;
        D_80080A18.unk4 = -0x7F8000 / d;
    }
    g_akaoSeq->unk50 = 0;
    g_akaoSeq->unk58 = a->unk10;
    g_akaoSeq->unk54 = (u32)(((a->unk14 & 0x7F) << 16) | 0x8000) / (u32)a->unk10;
    func_80059414();
}

void akao_cmdPlayMusicCount(Unk80080AA0 *a) {
    s32 r;
    akao_cmdPlayMusic(a);
    r = 0;
    if (a->unkC != 0) {
        r = a->unkC - 1;
    }
    D_800809B8 = r;
}

void akao_cmdPlaySeData(Unk80080AA0 *p) {
    s32 a = p->unk0;
    s32 b = p->unk4;
    p->unk0 = 0x400;
    p->unk4 = 0x1000000;
    p->unk8 = 0x80;
    p->unkC = 0x7F;
    p->unk10 = 0;
    func_8005A014(p, a, b, 0);
}

void akao_cmdPlaySeDefault(Unk80080AA0 *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->unk0];

    p->unk4 = 0x2000000;
    p->unk8 = 0x80;
    p->unkC = 0x7F;
    p->unk10 = func_8005A5FC(D_800809E4[p->unk0]);
    func_8005A20C(&a, &b, p->unk0);
    func_8005A014(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            func_8005A20C(&a, &b, p->unk0 + i);
            n--;
            func_8005A014(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySe(Unk80080AA0 *p) {
    u8 *a;
    u8 *b;
    s32 i;
    s32 n = D_80083148[p->unk0];

    p->unk10 = func_8005A5FC(D_800809E4[p->unk0]);
    func_8005A20C(&a, &b, p->unk0);
    func_8005A014(p, a, b, 0);
    if (n != 0) {
        i = 1;
        do {
            func_8005A20C(&a, &b, p->unk0 + i);
            n--;
            func_8005A014(p, a, b, 1);
            i++;
        } while (n != 0);
    }
}

void akao_cmdPlaySeBank(Unk80080AA0 *s) {
    AkaoSeqHdr *h = (AkaoSeqHdr *)s->unk0;
    u32 *offs = h->unk10;
    s32 i;
    u8 *base;
    u16 *p;
    u8 *a;
    u8 *b;
    s32 n;

    s->unk10 = func_8005A5FC(h->unk8);
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
    func_8005A014(s, a, b, 0);
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
        func_8005A014(s, a, b, 1);
    }
}

void akao_cmdStopSe(s32 *a) {
    akao_stopSeChannels(a[0], a[1]);
}

void akao_cmdSetMusicVolume(Unk80080AA0 *p) {
    Unk80080A10 *s;
    s32 v;

    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        s->unk50 = (p->unk4 & 0x7F) << 16;
        s->unk58 = 0;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && p->unk0 != 0 && p->unk0 == s->unk6A) {
            v = p->unk4;
            s->unk58 = 0;
            s->unk50 = (v & 0x7F) << 16;
            func_8005A284(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdFadeMusicVolume(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    Unk80080A10 *t;
    s32 v;
    s32 cur;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = ((p->unk8 & 0x7F) << 16) | 0x8000;
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        do { } while (0);
        cur = s->unk50 & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        v /= n;
        s->unk58 = n;
        s->unk50 = cur;
        s->unk54 = v;
        func_8005A284(s, g_akaoChannels);
    } else {
        t = g_akaoSubSeq;
        if (t == NULL || p->unk0 == 0 || p->unk0 != t->unk6A) {
            return;
        }
        cur = t->unk50 & 0xFFFF0000; cur |= 0x8000;
        v = v - cur;
        do { } while (0);
        v /= n;
        s = t;
        s->unk58 = n;
        s->unk50 = cur;
        s->unk54 = v;
        func_8005A284(s, g_akaoSubChannels);
    }
}

void akao_cmdFadeMusicVolumeFrom(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    AkaoChannel *c;
    s32 v;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        c = g_akaoChannels;
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || p->unk0 == 0 || p->unk0 != s->unk6A) {
            return;
        }
        c = g_akaoSubChannels;
    }
    v = ((p->unk8 & 0x7F) << 16) | 0x8000;
    s->unk50 = v;
    s->unk54 = ((((p->unkC & 0x7F) << 16) | 0x8000) - v) / n;
    s->unk58 = n;
    func_8005A284(s, c);
}

void akao_cmdC4(Unk80080AA0 *p) {
    Unk80080A10 *s;
    s32 v;

    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        s->unk5C = (p->unk4 & 0x7F) << 16;
        s->unk64 = 0;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s != NULL && p->unk0 != 0 && p->unk0 == s->unk6A) {
            v = p->unk4;
            s->unk64 = 0;
            s->unk5C = (v & 0x7F) << 16;
            func_8005A284(s, g_akaoSubChannels);
        }
    }
}

void akao_cmdC5(Unk80080AA0 *p) {
    s32 n = 1;
    Unk80080A10 *s;
    s32 v;

    if (p->unk4 != 0) {
        n = p->unk4;
    }
    v = (p->unk8 & 0x7F) << 16;
    if (p->unk0 == 0 || p->unk0 == g_akaoSeq->unk6A) {
        s = g_akaoSeq;
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        s->unk64 = n;
        s->unk60 = v;
        func_8005A284(s, g_akaoChannels);
    } else {
        s = g_akaoSubSeq;
        if (s == NULL || p->unk0 == 0 || p->unk0 != s->unk6A) {
            return;
        }
        v = v - s->unk5C;
        do { } while (0);
        v /= n;
        s->unk64 = n;
        s->unk60 = v;
        func_8005A284(s, g_akaoSubChannels);
    }
}

void akao_cmdSetCdVolume(u16 *a) {
    g_akaoCdVolumeFadeTicks = 0;
    g_akaoCdVolume = *a << 16;
    akao_applyCdVolume();
}

void akao_cmdFadeCdVolume(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    g_akaoCdVolumeStep = (((u16)p->unk4 << 16) - g_akaoCdVolume) / n;
    g_akaoCdVolumeFadeTicks = n;
}

void akao_cmdFadeCdVolumeFrom(Unk80080AA0 *p) {
    s32 n = 1;
    s32 v;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    g_akaoCdVolumeStep = (((u16)p->unk8 << 16) - (v = (u16)p->unk4 << 16)) / n;
    g_akaoCdVolumeFadeTicks = n;
    g_akaoCdVolume = v;
}

void akao_cmdSetSeVolume(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk100 = ((u16)a->unk8 & 0x7F) << 8;
                c->unkBC = 0;
                c->voiceDirty |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk100 = ((u16)a->unk8 & 0x7F) << 8;
                c->unkBC = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdFadeSeVolume(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk102 = (s16)((((u16)a->unkC & 0x7F) << 8) - c->unk100) / n;
                c->unkBC = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk102 = (s16)((((u16)a->unkC & 0x7F) << 8) - c->unk100) / n;
                c->unkBC = n;
            }
        }
    }
}

void akao_cmdSetAllSeVolume(u16 *a) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.unk0;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->unk28 & 0x2000000)) {
                c->unk100 = (*a & 0x7F) << 8;
                c->unkBC = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdFadeAllSeVolume(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.unk0;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unk102 = (s16)((((u16)p->unk4 & 0x7F) << 8) - c->unk100) / n;
            c->unkBC = n;
        }
    }
}

void akao_cmdA2(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk9C = (u8)a->unk8 << 8;
                c->unk9E = 0;
                c->voiceDirty |= 3;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk9C = (u8)a->unk8 << 8;
                c->unk9E = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdA3(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unkFE = (s16)(((u8)a->unkC << 8) - c->unk9C) / n;
                c->unk9E = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unkFE = (s16)(((u8)a->unkC << 8) - c->unk9C) / n;
                c->unk9E = n;
            }
        }
    }
}

void akao_cmdAA(u8 *a) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.unk0;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if (mask & bit) {
            if (!(c->unk28 & 0x2000000)) {
                c->unk9C = *a << 8;
                c->unk9E = 0;
                c->voiceDirty |= 3;
            }
        }
    }
}

void akao_cmdAB(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.unk0;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unkFE = (s16)(((u8)p->unk4 << 8) - c->unk9C) / n;
            c->unk9E = n;
        }
    }
}

void akao_cmdA4(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                c->unk70 = (u8)a->unk8 << 8;
                c->unkB6 = 0;
                c->voiceDirty |= 0x10;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                c->unk70 = (u8)a->unk8 << 8;
                c->unkB6 = 0;
                c->voiceDirty |= 0x10;
            }
        }
    }
}

void akao_cmdA5(Unk80080AA0 *a) {
    AkaoChannel *c = g_akaoSeChannels;
    u32 mask = g_akaoSeState.unk0;
    u32 bit = 0x100;
    u32 i;
    s16 n;

    if (a->unk4 != 0) {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && (c->unk28 & a->unk4)) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk74 = (s16)((s16)(((u8)a->unkC << 8) - (u16)c->unk70) / n);
                c->unkB6 = n;
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((mask & bit) && c->unk6C == a->unk0) {
                n = 1;
                if (a->unk8 != 0) {
                    n = a->unk8;
                }
                c->unk74 = (s16)((s16)(((u8)a->unkC << 8) - (u16)c->unk70) / n);
                c->unkB6 = n;
            }
        }
    }
}

void akao_cmdAC(u8 *a) {
    s32 i;
    AkaoChannel *c = g_akaoSeChannels;
    for (i = 16; i != 0; i--) {
        if (!(c->unk28 & 0x2000000)) {
            c->unk70 = *a << 8;
            c->unkB6 = 0;
            c->voiceDirty |= 0x10;
        }
        c++;
    }
}

void akao_cmdAD(Unk80080AA0 *p) {
    u32 bit = 0x100;
    u32 mask = g_akaoSeState.unk0;
    u32 i;
    AkaoChannel *c = g_akaoSeChannels;
    s16 n;

    for (i = 0; i < 16; i++, c++, bit <<= 1) {
        if ((mask & bit) && !(c->unk28 & 0x2000000)) {
            n = 1;
            if (p->unk0 != 0) {
                n = p->unk0;
            }
            c->unk74 = (s16)((s16)(((u8)p->unk4 << 8) - (u16)c->unk70) / n);
            c->unkB6 = n;
        }
    }
}

void akao_cmdD0(s8 *a) {
    s32 v = *a;
    D_8007F96E = 0;
    D_80083154 = v << 16;
}

void akao_cmdD1(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_8007F8E8 = (((s8)p->unk4 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void akao_cmdD2(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk4 != 0) {
        n = p->unk0;
    }
    D_80083154 = (s8)p->unk4 << 16;
    D_8007F8E8 = (((s8)p->unk8 << 16) - D_80083154) / n;
    D_8007F96E = n;
}

void akao_cmdD4(s8 *a) {
    s32 v = *a;
    D_8007F96C = 0;
    D_80083150 = v << 16;
}

void akao_cmdD5(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk0 != 0) {
        n = p->unk0;
    }
    D_8007F8E4 = (((s8)p->unk4 << 16) - D_80083150) / n;
    D_8007F96C = n;
}

void akao_cmdD6(Unk80080AA0 *p) {
    s32 n = 1;

    if (p->unk4 != 0) {
        n = p->unk0;
    }
    D_80083150 = (s8)p->unk4 << 16;
    D_8007F8E4 = (((s8)p->unk8 << 16) - D_80083150) / n;
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
        if ((g_akaoSeState.unk0 & bit) && !(c->unk28 & 0x2000000)) {
            g_akaoSeState.unkC |= bit;
            func_8005EC20(c, bit);
            c->unk34 = 0;
        }
    }
    g_akaoFlags.unk8 |= 0x110;
}

void akao_cmdSetOutputMode1(void) {
    g_akaoFlags.unk4 = 1;
    func_8005A284(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        func_8005A284(g_akaoSubSeq, g_akaoSubChannels);
    }
    func_8005A2CC();
}

void akao_cmdSetOutputMode2(void) {
    g_akaoFlags.unk4 = 2;
    func_8005A284(g_akaoSeq, g_akaoChannels);
    if (g_akaoSubSeq != NULL) {
        func_8005A284(g_akaoSubSeq, g_akaoSubChannels);
    }
    func_8005A2CC();
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
    g_akaoSeq->unk6C = *a;
}

void akao_cmd9B(void) {
    u32 m;
    u32 bit;
    s32 i;
    Unk80080A10 *t;
    s32 v;
    if (g_akaoSeq->unk4 != 0) {
        m = ~(g_akaoSeState.unk0 | g_akaoStream.unkC) & 0xFFFFFF;
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
        v = t->unk4;
        t->unk4 = 0;
        t->unk1C = v;
    }
    D_800831C4 |= 1;
}

void akao_cmd9A(void) {
    AkaoChannel *ch;
    s32 bit;
    s32 mask;
    s32 t;

    t = g_akaoSeq->unk1C;
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
        t = g_akaoSeq->unk4 = g_akaoSeq->unk1C;
        g_akaoSeq->unk1C = 0;
        g_akaoFlags.unk8 |= 0x100;
    }
    D_800831C4 &= ~1;
}

void akao_cmd9D(void) {
    u32 m;
    u32 bit;
    u32 i;
    AkaoChannel *c;
    s32 v;

    if (g_akaoSeState.unk0 != 0) {
        m = g_akaoSeState.unk0;
        c = g_akaoSeChannels;
        bit = 0x100;
        for (i = 0; i < 16; i++, c++, bit <<= 1) {
            if ((m & bit) && (c->unk28 & 0x2000000)) {
                m &= ~bit;
            }
        }
        bit = 0x100;
        v = 8;
        g_akaoSeState.unk10 = m;
        g_akaoSeState.unk0 &= ~m;
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
        m = g_akaoSeState.unk10;
        g_akaoSeState.unk10 = 0;
        g_akaoSeState.unk0 |= m;
        g_akaoFlags.unk8 |= 0x100;
    }
    D_800831C4 &= ~2;
}

void akao_cmd9F(void) {
    if (g_akaoStream.unkC != 0) {
        akao_setVoicePitch(g_akaoStream.unk10, 0);
        akao_setVoicePitch(g_akaoStream.unk10 + 1, 0);
        akao_setVoiceVolume(g_akaoStream.unk10, 0, 0, 0);
        akao_setVoiceVolume(g_akaoStream.unk10 + 1, 0, 0, 0);
    }
}

void akao_cmd9E(void) {
    Unk80083168 *d = &g_akaoStream;
    s32 v;
    s32 l;
    s32 r;
    s32 pan;

    if (d->unkC == 0) {
        return;
    }
    akao_setVoicePitch(d->unk10, d->unk58);
    akao_setVoicePitch(d->unk10 + 1, d->unk58);
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
        pan = (u8)(d->unk4C >> 8);
        l = d->unk40 * g_akaoPanCurve[pan] >> 16;
        r = d->unk40 * g_akaoPanCurve[pan ^ 0xFF] >> 16;
        akao_setVoiceVolume(d->unk10, l, r, 0);
        akao_setVoiceVolume(d->unk10 + 1, l, r, 0);
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
    Unk80080AA0 *blk;

    DisableEvent(g_akaoTickEvent);
    blk = &g_akaoCommandBlock;
    cmd &= 0xFF;
    switch (cmd) {
    case 0x10:
    case 0x12:
    case 0x14:
    case 0x19:
    case 0x1A:
        if (akao_checkHeader((s32 *)g_akaoArgs.unk0) == 0) {
        seq = (AkaoSeqHeader *)g_akaoArgs.unk0;
        if (g_akaoSeq->unk6A != seq->unk4 || (g_akaoSubSeq != NULL && g_akaoSubSeq->unk6A != g_akaoSeq->unk6A)) {
            if (cmd != 0x19) {
                akao_setReverbMode(seq->unk8);
            }
            blk->unk0 = (s32)seq;
            blk->unk8 = seq->unk4;
            switch (cmd) {
            case 0x14:
                blk->unkC = g_akaoArgs.unk4 != 0 ? g_akaoArgs.unk4 | 1 : -1;
                break;
            case 0x12:
                blk->unkC = g_akaoArgs.unk4;
                break;
            case 0x19:
                blk->unk10 = g_akaoArgs.unk4;
                break;
            case 0x1A:
                blk->unk10 = g_akaoArgs.unk4;
                blk->unk14 = g_akaoArgs.unk8;
                break;
            }
            r = seq->unk4;
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
        blk->unk0 = g_akaoArgs.unk0;
        g_akaoCommandHandlers[0xD0](blk);
        cmd = 0xD4;
        break;
    case 0xD9:
        blk->unk0 = g_akaoArgs.unk0;
        blk->unk4 = g_akaoArgs.unk4;
        g_akaoCommandHandlers[0xD1](blk);
        cmd = 0xD5;
        break;
    case 0xDA:
        blk->unk0 = g_akaoArgs.unk0;
        blk->unk4 = g_akaoArgs.unk4;
        blk->unk8 = g_akaoArgs.unk8;
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
        blk->unk0 = g_akaoArgs.unk0;
        blk->unk4 = g_akaoArgs.unk4;
        blk->unk8 = g_akaoArgs.unk8;
        blk->unkC = g_akaoArgs.unkC;
        blk->unk10 = g_akaoArgs.unk10;
        blk->unk14 = g_akaoArgs.unk14;
        break;
    }
    g_akaoCommandHandlers[cmd](blk);
    EnableEvent(g_akaoTickEvent);
    return r;
}

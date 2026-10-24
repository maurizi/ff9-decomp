#include "ff9.h"

u16 akao_shiftInstrumentUp(u32 flags, AkaoChannel *c) {
    if ((flags & 0x20) && (u16)(c->unk9A - 0x20) < 0x60) {
        c->voiceStartAddr += 0x20000;
        c->voiceLoopAddr += 0x20000;
        c->unk9A += 0x30;
    }
    return c->unk9A;
}

u16 akao_shiftInstrumentDown(u32 flags, AkaoChannel *c) {
    if ((flags & 0x20) && (u16)(c->unk9A - 0x20) < 0x60) {
        c->voiceStartAddr -= 0x20000;
        c->voiceLoopAddr -= 0x20000;
        c->unk9A -= 0x30;
    }
    return c->unk9A;
}

void func_80059414(void) {
    Unk80080A10 *a = g_akaoSubSeq;
    Unk80080A10 *seq;
    u32 seq8, a8, seqC, aC;
    u32 seqBoth, aBoth;
    u32 mask;
    u32 found;
    u32 bit;
    u32 lim;
    s32 v;
    s32 n;
    AkaoChannel *ch;

    if (a != NULL) {
        lim = 0xFFFFFF;
        seq = g_akaoSeq;
        found = 0;
        seq8 = seq->unk8;
        a8 = a->unk8;
        seqC = seq->unkC;
        aC = a->unkC;
        seqBoth = seq8 & seqC;
        aBoth = a8 & aC;
        v = ~seqBoth | aBoth;
        mask = ~(aC & v);
        mask &= seqC;
        mask &= lim;
        for (v = 0; mask != 0; v++) {
            if (mask & (1 << v)) {
                n = 0x20;
                bit = 1 << v;
                ch = g_akaoSubChannels;
                for (; n != 0; n--) {
                    if (ch->voice == v) {
                        ch->voice = 0x18;
                        found |= bit;
                    }
                    ch++;
                }
                mask &= ~(1 << v);
            }
        }
        g_akaoSeState.unkC |= found & ~(g_akaoSeState.unk0 | g_akaoStreamVoices);
    }
}

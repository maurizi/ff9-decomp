#include "ff9.h"

u16 akao_shiftInstrumentUp(u32 flags, AkaoChannel *chan) {
    if ((flags & AKAO_SEQ_INSTR_ALT) && (u16)(chan->instrument - 0x20) < 0x60) {
        chan->voiceStartAddr += 0x20000;
        chan->voiceLoopAddr += 0x20000;
        chan->instrument += 0x30;
    }
    return chan->instrument;
}

u16 akao_shiftInstrumentDown(u32 flags, AkaoChannel *chan) {
    if ((flags & AKAO_SEQ_INSTR_ALT) && (u16)(chan->instrument - 0x20) < 0x60) {
        chan->voiceStartAddr -= 0x20000;
        chan->voiceLoopAddr -= 0x20000;
        chan->instrument -= 0x30;
    }
    return chan->instrument;
}

void akao_freeSubSeqVoices(void) {
    AkaoSeq *sub = g_akaoSubSeq;
    AkaoSeq *seq;
    u32 seq8, a8, seqC, aC;
    u32 seqBoth, aBoth;
    u32 freeBits;
    u32 found;
    u32 bit;
    u32 lim;
    s32 val; /* voice mask, then the voice loop counter (separate locals don't match) */
    s32 remaining;
    AkaoChannel *ch;

    if (sub != NULL) {
        lim = 0xFFFFFF;
        seq = g_akaoSeq;
        found = 0;
        seq8 = seq->priorityMask;
        a8 = sub->priorityMask;
        seqC = seq->keepVoiceMask;
        aC = sub->keepVoiceMask;
        seqBoth = seq8 & seqC;
        aBoth = a8 & aC;
        val = ~seqBoth | aBoth;
        freeBits = ~(aC & val);
        freeBits &= seqC;
        freeBits &= lim;
        for (val = 0; freeBits != 0; val++) {
            if (freeBits & (1 << val)) {
                remaining = 0x20;
                bit = 1 << val;
                ch = g_akaoSubChannels;
                for (; remaining != 0; remaining--) {
                    if (ch->voice == val) {
                        ch->voice = 0x18;
                        found |= bit;
                    }
                    ch++;
                }
                freeBits &= ~(1 << val);
            }
        }
        g_akaoSeState.keyOffMask |= found & ~(g_akaoSeState.activeMask | g_akaoStreamVoices);
    }
}

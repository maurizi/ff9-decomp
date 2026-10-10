/* 15 rows differ: registers only. The hi of D_80080970 lives in a3 in the original (not across a call) and r gets s1; for us the hi gets s1 and r gets s3. */
#include "ff9.h"

s32 func_8005C8BC(u32 cmd) {
    s32 r = 0;
    AkaoSeqHeader *seq;
    Unk80080AA0 *blk = &D_80080970;

    func_80056CBC(D_8006F318);
    cmd &= 0xFF;
    switch (cmd) {
    case 0x10:
    case 0x12:
    case 0x14:
    case 0x19:
    case 0x1A:
        if (func_800565DC((s32 *)D_80080AA0.unk0) == 0) {
        seq = (AkaoSeqHeader *)D_80080AA0.unk0;
        if (D_80080A10->unk6A != seq->unk4 || (D_8007F790 != NULL && D_8007F790->unk6A != D_80080A10->unk6A)) {
            if (cmd != 0x19) {
                func_8005C86C(seq->unk8);
            }
            blk->unk0 = (s32)seq;
            blk->unk8 = seq->unk4;
            switch (cmd) {
            case 0x14:
                blk->unkC = D_80080AA0.unk4 != 0 ? D_80080AA0.unk4 | 1 : -1;
                break;
            case 0x12:
                blk->unkC = D_80080AA0.unk4;
                break;
            case 0x19:
                blk->unk10 = D_80080AA0.unk4;
                break;
            case 0x1A:
                blk->unk10 = D_80080AA0.unk4;
                blk->unk14 = D_80080AA0.unk8;
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
        blk->unk0 = D_80080AA0.unk0;
        D_8006F740[0xD0](blk);
        cmd = 0xD4;
        break;
    case 0xD9:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        D_8006F740[0xD1](blk);
        cmd = 0xD5;
        break;
    case 0xDA:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        blk->unk8 = D_80080AA0.unk8;
        D_8006F740[0xD2](blk);
        cmd = 0xD6;
        break;
    case 0x99:
        D_8006F740[0x9B](blk);
        D_8006F740[0x9D](blk);
        cmd = 0x9F;
        break;
    case 0x98:
        D_8006F740[0x9A](blk);
        D_8006F740[0x9C](blk);
        cmd = 0x9E;
        break;
    default:
        blk->unk0 = D_80080AA0.unk0;
        blk->unk4 = D_80080AA0.unk4;
        blk->unk8 = D_80080AA0.unk8;
        blk->unkC = D_80080AA0.unkC;
        blk->unk10 = D_80080AA0.unk10;
        blk->unk14 = D_80080AA0.unk14;
        break;
    }
    D_8006F740[cmd](blk);
    func_80016ED0(D_8006F318);
    return r;
}

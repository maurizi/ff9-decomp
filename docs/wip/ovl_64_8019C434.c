/* STATUS: logic right, 89 rows. ovl_64 func_8019C434 (0x3B0, CdReadyCallback of the FMV stream: header check, ring bookkeeping, next sector read). The two timeout blocks are cross-jumped in the original (keep both copies in C). Remaining differences: the original rematerialises &D_8019DE10 and the constant 1 after the calls instead of keeping them in s-registers, i.e. CSE did not reach across these blocks; the source block structure must differ (gotos/labels?). */
#include "ovl_64.h"
void func_80022F64(s32);
s32 DecDCToutSync(s32);
extern s32 D_801A07F0[];
extern s32 D_801A06F0[];

void func_8019C434(u8 status) {
    u8 hdr[12];
    s32 limit;
    s32 r;
    s32 prev;
    Ovl64RingHdr *h;

    if (D_8019DEA4 != 0) {
        return;
    }
    if (status == 5 || status == 0) {
        limit = 0xB4;
        if (D_8019DE10.unk58 == 1) {
            limit = 0x438;
        }
        if (VSync(-1) - D_8019DE10.unk14 < limit) {
            func_80022F64(D_8019DE10.unk44);
        } else {
            D_8019DE10.unk2 = 2;
            func_80023018();
        }
        return;
    }
    if (D_8019DEA8 == 0 && D_8019DE10.unk58 >= 2) {
        r = DecDCToutSync(1);
        if (r == 1) {
            D_8019DEA8 = r;
            return;
        }
    }
    func_800213C0(hdr, 3);
    if (CdPosToInt((CdlLOC *)hdr) != D_8019DE10.unk44) {
        limit = 0xB4;
        if (D_8019DE10.unk58 == 1) {
            limit = 0x438;
        }
        if (VSync(-1) - D_8019DE10.unk14 < limit) {
            func_80022F64(D_8019DE10.unk44);
        } else {
            D_8019DE10.unk2 = 2;
            func_80023018();
        }
        return;
    }
    if (D_8019DE10.unk18 < 0) {
        if (hdr[5] == hdr[9]) {
            D_8019DE10.unk18 = hdr[5];
        }
    } else if (hdr[5] == hdr[9] && D_8019DE10.unk18 != hdr[5]) {
        D_8019DE10.unk2 = 2;
        func_80023018();
    }
    D_8019DEA4 = 1;
    D_8019DE0C = 0;
    if (D_8019DE10.unk4C < 2) {
        func_800213C0(D_8019DE10.unk90 + (D_8019DE10.unk80 << 11), 0x200);
        D_800761D0.unkC += 0x800;
        func_8019BCE8();
    } else {
        if (D_8019DE10.unk64 == 0) {
            prev = D_8019DE10.unk68 - 1;
            if (D_8019DE10.unk68 == 0) {
                prev = D_8019DE10.unk6C - 1;
            }
            h = &D_8019DE10.unk74[prev];
            if (!(h->unk0 & 0x1000)) {
                D_801A07F0[D_8019DE0C] = D_8019DE10.unk58;
                D_801A06F0[D_8019DE0C] = prev;
                D_8019DE0C++;
                func_800213C0(D_8019DE10.unk78 +
                                  (D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 + 1 - D_8019DE10.unk5C)) *
                                      0x8F4 -
                                  0x20,
                              0x245);
                D_800761D0.unkC += 0x800;
                func_80023018();
                func_8019BA70();
                D_8019DE10.unk1 = 1;
                goto end;
            }
            h->unk0 = (h->unk0 & 0xFFF) | 0x6000;
        }
        func_800213C0(D_8019DE10.unk78 +
                          (D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 + 1 - D_8019DE10.unk5C)) * 0x8F4 -
                          0x20,
                      0x245);
        D_800761D0.unkC += 0x800;
        func_8019BA70();
    }
    D_8019DE10.unk14 = VSync(-1);
end:
    D_8019DEA4 = 0;
}

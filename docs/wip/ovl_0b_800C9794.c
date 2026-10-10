/* WIP func_800C9794: 26 differing rows (build/cmp.py-style aligned diff). New in pass 7. Structure right (second loop walks a copy of src, func_800CA0A0 gets i as an extra third argument). Left: i is set to 0 before the func_800C1184 call (s4) instead of after the n loads (s3), so i/ofs swap s3/s4; flags/src stack slots swapped (0x14/0x10); recs computed as unk1*56+8 then +hdr instead of (hdr+8)+unk1*56. Moving i = 0, separate counters per loop: no change. */
#include "ovl_0b.h"

void func_800C9794(void) {
    Battle *b;
    u32 *flags;
    Battle1330 *grp;
    Battle11C020 *hdr;
    s16 i;
    s16 j;
    s16 k;
    u8 n;
    Unk800C9D1C *recs;
    Unk800C9D1C *rec;
    Battle4880 *en;
    U16Pair *ofs;
    BtlCmd8 *src;
    BtlCmd8 *dst;
    BtlCmd8 *cmd;
    BtlUnit *u;

    b = D_8006794C->unk1C->unk8E4.battle;
    flags = &b->unk132C.unk0;
    grp = b->unk132C.unk4;
    hdr = b->unk11C0->unk20;
    b->unk132C.unk0 = (b->unk132C.unk0 & ~3) | (func_800C1184(flags) & 3);
    i = 0;
    n = hdr->unk2 + hdr->unk3;
    recs = (Unk800C9D1C *)(hdr->unk8 + hdr->unk1);
    rec = recs;
    en = D_8006794C->unk1C->unk8E4.battle->unk4E8;
    ofs = (U16Pair *)(D_8006794C->unk1C->unk8E4.battle->unk11C0->unk14 + 4);
    src = (BtlCmd8 *)&recs[hdr->unk2];
    for (; i < hdr->unk2; i++) {
        func_800C9D1C(rec, en);
        en->unk2C = func_800CA2B0(i + 5);
        en->unk0 = ofs->unk0 + 4;
        ofs++;
        en->unk28 = n;
        n += rec->unk4F;
        en++;
        rec++;
    }
    dst = D_8006794C->unk1C->unk8E4.battle->unk5D8;
    for (i = 0, cmd = src; i < hdr->unk3; i++) {
        func_800CA28C(cmd, dst);
        dst->unkE = ofs->unk0 + 4;
        ofs++;
        cmd++;
        dst++;
    }
    u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    for (j = 0; j < grp->unk1; j++) {
        if (u == NULL) {
            break;
        }
        func_800C9EE0(&grp->unk8[j], u, (s32 *)flags, j);
        ((void (*)(Unk800C9D1C *, BtlUnit *, s32))func_800CA0A0)(&recs[grp->unk8[j].unk0], u, j);
        u = u->unk0;
    }
}

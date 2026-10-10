/*
 * ovl_0a func_800AA4E8 (0x56C bytes): load a field's texture pack into the VRAM cache and
 * upload the 8-bit / 4-bit images (FieldVramPos allocator), filling tpage/clut/uv tables.
 * WIP: same length (347), ~93 aligned diff rows, all scheduling/registers:
 *  - the vp field loads come out as lh x, lh y, lhu x, lhu y; the original has
 *    lhu x, lh x, lh y, lhu y (loop 2: lh y, lhu x, lh x, lhu y). Reordering the x/y reads or
 *    folding the comparison does not change it;
 *  - s-registers of f/c and vp/ent are swapped (s1/s2, s0/s1);
 *  - clut computation scheduled later than in the original.
 */
#include "ovl_0a.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} AA4E8Ent;

s32 func_800AA4E8(s32 arg0) {
    FileEntry *f;
    s32 size;
    Unk800C9CCC *c;
    s32 n;
    FieldVramPos *vp;
    s32 i;
    s32 j;
    s32 pack2;
    s32 pack1;
    u8 *hdr;
    s16 *tp;
    s16 *uv;
    AA4E8Ent *ent;
    s32 img;
    s32 x, y, cx, cy, e;
    s32 e1;

    f = func_8001E218(10, arg0);
    size = (f->unkC - f->unk4) << 11;
    c = func_800AC344(arg0);
    if (c->unk4 == 0) {
        func_8002177C(D_8006794C->unk1C->unk854, f->unk4, size,
                      (void *)(func_800AC3D8(arg0, size) != 0 ? c->unk4 : D_80067948), 0, -1, 0);
        while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
        }
    }
    if (c->unk4 != 0) {
        n = func_8001DAA4(D_80067948, c->unk4, size, NULL);
        DrawSync(0);
        func_800AC67C(D_80067948 + n);
    }
    vp = func_8001C7FC(4, 0x1C0000, 0);
    n = D_80067948;
    pack1 = func_8001C8B0(n, 2, 0x1B0000, 0);
    pack2 = func_8001C8B0(n, 2, 0x1B0001, 0);
    hdr = (u8 *)func_8001C8B0(pack1, 2, 0x120000, 0) + 4;
    tp = (s16 *)(hdr + (*(u32 *)(hdr + 4) & 0xFFFFFF));
    uv = tp + hdr[7] * 2;
    ent = (AA4E8Ent *)(uv + hdr[7] * 2);
    for (i = 0; i < hdr[7]; i++) {
        img = func_8001C8B0(pack2, 2, (i & 0xFFFF) | 0x40000, 0);
        e1 = ent[i].unk0;
        if (e1 & 1) {
            x = vp->unk0;
            y = vp->unk2;
            if (vp->unk2 == 0x100) {
                vp->unk0 += 0x40;
                vp->unk2 = 0x180;
            } else {
                vp->unk2 -= 0x80;
            }
            if (vp->unk0 == vp->unk20) {
                vp->unk0 += 0x40;
            }
            cy = vp->unkA;
            cx = vp->unk8;
            vp->unkA += 2;
            tp[i * 2] = getTPage(1, 0, x, y);
            tp[i * 2 + 1] = getClut(cx, cy);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y % 256;
            ent[i].unk4 = x;
            ent[i].unk6 = y;
            func_8001D9CC(img, cx, cy, x, y);
            func_8001D8A8(img);
        }
    }
    for (i = 0; i < hdr[7]; i++) {
        img = func_8001C8B0(pack2, 2, (i & 0xFFFF) | 0x40000, 0);
        e = ent[i].unk0;
        if (!(e & 1)) {
            y = vp->unk12;
            x = vp->unk10;
            if (vp->unk12 >= 0x1E0) {
                vp->unk10 -= 0x40;
                vp->unk12 = 0x100;
            } else {
                vp->unk12 += 0x20;
            }
            cy = vp->unk1A;
            cx = vp->unk18;
            vp->unk1A += 2;
            tp[i * 2] = getTPage(1, 0, x, y);
            tp[i * 2 + 1] = getClut(cx, cy);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y % 256 - ((e - 2) % 8 * 32 + 0x100) % 128;
            ent[i].unk0 = x;
            ent[i].unk2 = y;
            for (j = hdr[7] - 1; j >= 0; j--) {
                if (ent[j].unk0 == e - 1) {
                    ent[i].unk4 = ent[j].unk4;
                    ent[i].unk6 = ent[j].unk6;
                    break;
                }
            }
            func_8001D9CC(img, cx, cy, x, y);
            func_8001D8A8(img);
        }
    }
    DrawSync(0);
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk28[D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk40] = pack1;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk40++;
    func_8001C750(pack1, 0);
    D_80067948 = pack1 + func_8001CA70(pack1);
    return 0;
}

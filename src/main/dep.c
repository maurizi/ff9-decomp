#include "ff9.h"

s32 shadow_initSlots(s32 prim, s32 cx, s32 cy, s32 u, s32 v, s32 count, s32 flags) {
    Unk11C0Slot *slots;
    Unk11C0Slot *slot;
    POLY_FT4 *poly;
    s32 clut;
    s32 i, j, k;

    clut = cy << 6;
    clut |= (cx >> 4) & 0x3F;
    cx = getTPage(1, 2, u, v);
    u = (u % 64) * 2;
    v = v % 256;
    slots = (Unk11C0Slot *)prim;
    prim += count * (s32)sizeof(Unk11C0Slot);
    for (i = 0; i < count; i++) {
        slot = &slots[i];
        slot->fixedAngle = 0;
        slot->sizeX = 0xE0;
        slot->sizeZ = 0xC0;
        slot->unk0_0 = 0x80;
        slot->ofsX = 0;
        slot->ofsZ = 0;
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 4; k++) {
                if (!(flags & 1)) {
                    slot->prims[j][k] = NULL;
                    continue;
                }
                slot->prims[j][k] = (POLY_FT4 *)prim;
                if (flags & 2) {
                    prim += 0xC;
                }
                poly = (POLY_FT4 *)prim;
                prim += (s32)sizeof(POLY_FT4);
                if (flags & 2) {
                    prim += 0xC;
                }
                setlen(poly, 9);
                setcode(poly, GPU_POLY_FT4 | GPU_ABE);
                switch (k) {
                case 0:
                    poly->u0 = u;
                    poly->v0 = v;
                    poly->u1 = u + 0x1F;
                    poly->v1 = v;
                    poly->u2 = u;
                    poly->v2 = v + 0x1F;
                    poly->u3 = u + 0x1F;
                    poly->v3 = v + 0x1F;
                    break;
                case 1:
                    poly->u0 = u + 0x1F;
                    poly->v0 = v;
                    poly->u1 = u;
                    poly->v1 = v;
                    poly->u2 = u + 0x1F;
                    poly->v2 = v + 0x1F;
                    poly->u3 = u;
                    poly->v3 = v + 0x1F;
                    break;
                case 2:
                    poly->u0 = u;
                    poly->v0 = v + 0x1F;
                    poly->u1 = u + 0x1F;
                    poly->v1 = v + 0x1F;
                    poly->u2 = u;
                    poly->v2 = v;
                    poly->u3 = u + 0x1F;
                    poly->v3 = v;
                    break;
                case 3:
                    poly->u0 = u + 0x1F;
                    poly->v0 = v + 0x1F;
                    poly->u1 = u;
                    poly->v1 = v + 0x1F;
                    poly->u2 = u + 0x1F;
                    poly->v2 = v;
                    poly->u3 = u;
                    poly->v3 = v;
                    break;
                }
                poly->tpage = cx;
                poly->clut = clut;
            }
        }
    }
    return prim;
}

FileEntry *dep_find(FileTable *table, s32 id) {
    FileEntry *entry;
    FileEntry *end;

    id &= 0xFFFFFF;
    entry = table->entries;
    end = entry + table->count;
    for (; entry < end; entry++) {
        if ((entry->id & 0xFFFFFF) == id) {
            return entry;
        }
    }
    return NULL;
}

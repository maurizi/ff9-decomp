#include "ff9.h"

s32 func_80062E3C(s32 prim, s32 cx, s32 cy, s32 u, s32 v, s32 n, s32 flags) {
    Unk11C0Slot *s;
    Unk11C0Slot *q;
    POLY_FT4 *p;
    s32 clut;
    s32 i, j, k;

    clut = cy << 6;
    clut |= (cx >> 4) & 0x3F;
    cx = getTPage(1, 2, u, v);
    u = (u % 64) * 2;
    v = v % 256;
    s = (Unk11C0Slot *)prim;
    prim += n * (s32)sizeof(Unk11C0Slot);
    for (i = 0; i < n; i++) {
        q = &s[i];
        q->unk4_0 = 0;
        q->unk0_8 = 0xE0;
        q->unk0_20 = 0xC0;
        q->unk0_0 = 0x80;
        q->unk4_12 = 0;
        q->unk4_22 = 0;
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 4; k++) {
                if (!(flags & 1)) {
                    q->unk8[j][k] = NULL;
                    continue;
                }
                q->unk8[j][k] = (POLY_FT4 *)prim;
                if (flags & 2) {
                    prim += 0xC;
                }
                p = (POLY_FT4 *)prim;
                prim += (s32)sizeof(POLY_FT4);
                if (flags & 2) {
                    prim += 0xC;
                }
                setlen(p, 9);
                setcode(p, 0x2E);
                switch (k) {
                case 0:
                    p->u0 = u;
                    p->v0 = v;
                    p->u1 = u + 0x1F;
                    p->v1 = v;
                    p->u2 = u;
                    p->v2 = v + 0x1F;
                    p->u3 = u + 0x1F;
                    p->v3 = v + 0x1F;
                    break;
                case 1:
                    p->u0 = u + 0x1F;
                    p->v0 = v;
                    p->u1 = u;
                    p->v1 = v;
                    p->u2 = u + 0x1F;
                    p->v2 = v + 0x1F;
                    p->u3 = u;
                    p->v3 = v + 0x1F;
                    break;
                case 2:
                    p->u0 = u;
                    p->v0 = v + 0x1F;
                    p->u1 = u + 0x1F;
                    p->v1 = v + 0x1F;
                    p->u2 = u;
                    p->v2 = v;
                    p->u3 = u + 0x1F;
                    p->v3 = v;
                    break;
                case 3:
                    p->u0 = u + 0x1F;
                    p->v0 = v + 0x1F;
                    p->u1 = u;
                    p->v1 = v + 0x1F;
                    p->u2 = u + 0x1F;
                    p->v2 = v;
                    p->u3 = u;
                    p->v3 = v;
                    break;
                }
                p->tpage = cx;
                p->clut = clut;
            }
        }
    }
    return prim;
}

FileEntry *dep_find(FileTable *t, s32 id) {
    FileEntry *e;
    FileEntry *end;

    id &= 0xFFFFFF;
    e = t->unk4;
    end = e + t->unk2;
    for (; e < end; e++) {
        if ((e->unk0 & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

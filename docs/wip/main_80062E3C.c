/* First draft, about 100 rows differ: right structure (Unk11C0Slot header bitfields, 2x4 POLY_FT4 grid, switch on the corner) but the header's second word gets its own giv (s+4) here, so its bitfield stores reload; the original addresses it as 4(t3) and reuses the value. Register allocation of the prologue temps also differs (the original keeps n in s0 only). */
#include "ff9.h"

s32 func_80062E3C(s32 arg0, s32 cx, s32 cy, s32 u, s32 v, s32 n, s32 flags) {
    Unk11C0Slot *s;
    u8 *prim;
    POLY_FT4 *p;
    s32 clut;
    s16 tpage;
    u8 u0, v0, u1, v1;
    s32 i, j, k;

    clut = (cy << 6) | ((cx >> 4) & 0x3F);
    tpage = ((v & 0x100) >> 4) | ((u & 0x3FF) >> 6) | 0xC0 | ((v & 0x200) << 2);
    u0 = (u % 64) * 2;
    v0 = v % 256;
    s = (Unk11C0Slot *)arg0;
    prim = (u8 *)(s + n);
    for (i = 0; i < n; i++, s++) {
        s->unk0_8 = 0xE0;
        s->unk0_20 = 0xC0;
        s->unk4_0 = 0;
        s->unk0_0 = 0x80;
        s->unk4_12 = 0;
        s->unk4_22 = 0;
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 4; k++) {
                if (!(flags & 1)) {
                    s->unk8[j][k] = NULL;
                    continue;
                }
                s->unk8[j][k] = (POLY_FT4 *)prim;
                if (flags & 2) {
                    prim += 0xC;
                }
                p = (POLY_FT4 *)prim;
                prim += sizeof(POLY_FT4);
                if (flags & 2) {
                    prim += 0xC;
                }
                setlen(p, 9);
                setcode(p, 0x2E);
                switch (k) {
                case 0:
                    p->u0 = u0;
                    p->v0 = v0;
                    p->u1 = u0 + 0x1F;
                    p->v1 = v0;
                    p->u2 = u0;
                    p->v2 = v0 + 0x1F;
                    p->u3 = u0 + 0x1F;
                    p->v3 = v0 + 0x1F;
                    break;
                case 1:
                    p->u0 = u0 + 0x1F;
                    p->v0 = v0;
                    p->u1 = u0;
                    p->v1 = v0;
                    p->u2 = u0 + 0x1F;
                    p->v2 = v0 + 0x1F;
                    p->u3 = u0;
                    p->v3 = v0 + 0x1F;
                    break;
                case 2:
                    p->u0 = u0;
                    p->v0 = v0 + 0x1F;
                    p->u1 = u0 + 0x1F;
                    p->v1 = v0 + 0x1F;
                    p->u2 = u0;
                    p->v2 = v0;
                    p->u3 = u0 + 0x1F;
                    p->v3 = v0;
                    break;
                case 3:
                    p->u0 = u0 + 0x1F;
                    p->v0 = v0 + 0x1F;
                    p->u1 = u0;
                    p->v1 = v0 + 0x1F;
                    p->u2 = u0 + 0x1F;
                    p->v2 = v0;
                    p->u3 = u0;
                    p->v3 = v0;
                    break;
                }
                p->tpage = tpage;
                p->clut = clut;
            }
        }
    }
    return (s32)prim;
}

/* 27 rows differ: the original keeps best in fp and spills r (sp+0x1C) and initialises bestd with a copy of best (addu s7,fp,zero); we keep r in s8 and spill best. Also a gets a1 (a3 here). Needed: the block-scoped copy p = q after the unk5 test (the original's addu s1,a0 in that branch's delay slot), q->unkF/o->unkF loaded once, the ?: form for a (else gcc turns if (g & 2) a = 1 into sltu), and func_800384D0_find for the script lookup. */
#include "ff9.h"

s32 func_80052EEC(s32);

static inline u8 *func_800384D0_find(s32 idx, s32 key) {
    u8 *h = D_8007ADF8 + *(u16 *)(D_8007ADF8 + idx * 8);
    s32 n = *(u16 *)h >> 8;
    u16 *p = (u16 *)(h + 2);

    while (n != 0) {
        if (*p++ == key) {
            break;
        }
        n--;
        p++;
    }
    return n != 0 ? h + (*p + 2) : NULL;
}

void *func_80038618(Unk8004AB64 *o, s32 flags, s32 *dist) {
    Unk8004AB64 *best = NULL;
    s32 bestd = 0;
    s32 x, y, z;
    s32 back;
    s32 r;
    Unk16D8 *n;
    Unk8004AB64 *q;
    s32 a;
    s32 b;
    s32 dx, dz;
    s32 rr;
    s32 d;
    s32 s;
    u8 f;
    u8 g;
    s32 key;

    x = o->unk14.vx;
    y = o->unk14.vy;
    back = flags & 4;
    z = o->unk14.vz;
    if (back) {
        r = o->unk3D * 4;
    } else {
        r = o->unk3C * 4;
    }
    for (n = D_8007AECC->unk16D8; n != NULL; n = n->unk0) {
        q = n->unk4;
        a = 0;
        if (back) {
            a = 1;
        } else {
            g = o->unkF;
            if (q->unk7 == D_8007AECC->unk16ED ? (g & 2) : (g & 4)) {
                a = 1;
            }
        }
        f = q->unkF;
        if (back) {
            b = f & 8;
        } else if (o->unk7 == D_8007AECC->unk16ED) {
            b = f & 2;
        } else {
            b = f & 4;
        }
        if (q == o || (a && b)) {
            continue;
        }
        if ((flags & 6) && func_800384D0_find(q->unk6, back ? 3 : 2) == NULL) {
            continue;
        }
        if (q->unk5 != 4) {
            continue;
        }
        {
            Unk8004AB64 *p = q;
        if ((u32)(p->unk14.vy - y + 399) >= 799) {
            continue;
        }
        dx = p->unk14.vx - x;
        dz = p->unk14.vz - z;
        if ((dx < 0 ? -dx : dx) >= 0x800) {
            continue;
        }
        if ((dz < 0 ? -dz : dz) >= 0x800) {
            continue;
        }
        if (back) {
            rr = p->unk3D * 4;
        } else {
            rr = p->unk3C * 4;
        }
        if (p->unk2D != 0) {
            s = func_8004BB7C(func_800531D0(p, o, p->unk14.unk22));
            rr = (rr * func_80052EEC(((s * s) >> 4) * p->unk2D + 0x1000000)) >> 12;
        }
        rr += r;
        if (flags & 6) {
            rr += 0x3C + p->unk5C;
        }
        d = func_80052DEC(dx, dz, rr);
        if (back) {
            s = func_800531D0(o, p, o->unk14.unk22);
            if (s < 0) {
                s = -s;
            }
            s -= 640;
            d += s;
            if (s >= 0) {
                continue;
            }
        }
        if (d < bestd) {
            bestd = d;
            best = p;
        }
        }
    }
    if (dist != NULL) {
        *dist = bestd;
    }
    return best;
}

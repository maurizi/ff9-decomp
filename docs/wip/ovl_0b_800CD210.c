/* WIP func_800CD210: 25 differing rows (build/cmp.py-style aligned diff), same length. New in pass 9 (status countdown digits over battle units; sibling of func_800CCDB8, whose lessons apply). Structure, stack and types match; left is register choice: the original keeps the status word in t1, the bone/level variable in t0, the scaled y in v1 (y and dy one pseudo there) and q in a2 with the copy in t0. Fixed: bone reused as the level (that is what keeps the (s16) extension on the bone index), u8 c for the colour, s16 d for unk2E, y via s16 + dy copy. Tried: s8 y temp, separate y/dy, inline bone accessor, bone types: no better. Pass 13: one s32 dy set as `dy = (s8)X` in both arms (the original's sra at the join looks cross-jumped, y and dy are one pseudo) makes combine fold lbu+sll+sra into lb (38 rows); (s16)(s8), (s8)(u8), a u8/s32 y with `dy = (s8)y`: 38-43. Pass 14: `bone = q` after the blink if/else keeps q global in a2 (as in the original) but bone and q are then tied, so the t0 copy is gone (29 rows). In the original the copy sits before the `t <= 0` branch and the else arm still compares q; cse makes a longer-lived or earlier-set register the class head, so a plain copy into bone (set earlier, used later) would replace q there. The level variable probably reaches t0 another way. */
#include "ovl_0b.h"

typedef struct {
    s32 unk0; /* status mask */
    s16 unk4; /* status timer index */
    u16 unk6;
} Unk800F6CA4;

extern Unk800F6CA4 D_800F6CA4[];

void func_800CD210(Unk800CCD00 *arg0) {
    BtlUnit *u;
    Battle4880 *m;
    BtlUnit9C *mdl;
    BtlBone *b;
    Unk800F6CA4 *e;
    SVECTOR v;
    u32 st;
    s32 k;
    s32 bone;
    s16 y;
    s32 dy;
    s16 i;
    s16 t;
    s32 q;
    s16 d;
    u8 c;
    s32 blink;
    u32 n;

    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 22) & 1) || (u->unk9C->unk0 & 0x10)) {
            continue;
        }
        if (D_8006794C->unk1C->unk8E4.battle->unk1358.unk1D4 & u->unkB0) {
            continue;
        }
        st = u->unk3C;
        if ((st & 0x100) || !(st & 0x88000000)) {
            continue;
        }
        if (u->unk68 & 1) {
            bone = D_800F6B48[D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4][5];
            y = (s8)D_800F6BBC[D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4][5];
        } else {
            m = D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0;
            bone = m->unk32[5];
            y = (s8)m->unk38[5];
        }
        dy = y;
        mdl = u->unk9C;
        if (mdl->unk0 & 2) {
            dy = (u32)((s16)dy * mdl->unk56) >> 12;
        }
        b = &mdl->unk20[(s16)bone];
        v.vx = b->unk14;
        v.vy = b->unk18 + dy * 16;
        v.vz = b->unk1C;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&arg0->unk10);
        arg0->unk10.s.y -= 12;
        for (e = D_800F6CA4, i = 1; i != -1; e++, i--) {
            if (st & e->unk0) {
                if (u->unk66 > 0 && (t = u->unk46[e->unk4]) >= 0) {
                    q = t * 10 / u->unk66;
                    d = u->unk2E;
                    bone = q;
                    if (t <= 0) {
                        blink = 2;
                    } else {
                        blink = ((s16)((t - d) * 10 / u->unk66) != (s16)q) * 2;
                    }
                    if (e->unk6 != 0) {
                        c = bone * 16 + 0x20;
                        arg0->unk8 = (c << 16) | (c << 8) | c;
                    } else {
                        arg0->unk8 = 0x1000000;
                    }
                    arg0->unk14 = 0;
                    arg0->unk8 |= blink << 24;
                    t = bone + 1;
                    if (t < 10) {
                        n = t | 0x10000;
                    } else {
                        n = 0x20010;
                    }
                    arg0->unk10.s.x -= (n >> 16) * 4;
                    func_800CCBC0(arg0, n);
                }
                break;
            }
        }
    }
}

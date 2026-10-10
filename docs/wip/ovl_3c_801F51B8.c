/* STATUS: near miss, 13 rows. ovl_3c func_801F51B8 (0x364, buy menu: equip stat comparison per party member). Needs func_801F42A8 returning s32 (header and definition changed, still matches) and func_80028954 declared s32 (no andi). Left: the loop counter i and the x-1 giv swap s2/s3, and i = 0 is scheduled before the func_8002B524 call in ours. Declaration order does not matter; permuter 15 min found nothing. */
#include "ovl_3c.h"
s32 func_80028014(s32);
s32 func_80028954(s32);
s32 func_80033A1C(Character *);
extern s32 D_801FC070[2];
extern u8 D_801FC078[5];
extern u16 D_801FC080[];
typedef struct {
    u8 v[5];
} Equip5;
extern s32 D_801F3FF8;

void func_801F51B8(void) {
    u8 buf[8];
    Equip5 eq;
    RECT *r;
    MenuList *l;
    Character *c;
    u8 id;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    r = &D_801FC2F8.unk18;
    l = &D_801FC2F8.unk28;
    id = D_801FC2F8.unk3C[l->unk5 + l->unk6];
    if (D_801FC2F8.unk4 == 0) {
        return;
    }
    func_8002DEE4(r, 0x23, 1);
    kind = func_80028014(id);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    func_8002B524(r->x + 6, r->y - 3, 0x1D, ((u8 **)&D_800703FC)[D_801FC070[armor]], 0);
    y = 0x9E;
    for (i = 0; i < D_801FC2D0.unk14; i++) {
        x = 0xF + i * 0x24;
        c = &D_8006794C->unk1C->unk3C[(u8)D_801FC2D0.unk18[i]];
        ok = (D_80069018[id].unk6 & D_801FC080[func_80033A1C(c)]) != 0;
        if (ok) {
            if (!armor) {
                new = D_8006E0E8[id].unk5;
                cur = D_8006E0E8[c->unk39[0]].unk5;
            } else {
                eq = *(Equip5 *)c->unk39;
                eq.v[kind] = id;
                cur = func_801F42A8(kind, c->unk39);
                new = func_801F42A8(kind, eq.v);
            }
            if (kind == 4) {
                *(s32 *)buf = D_801F3FF8;
            } else {
                func_8002BAE4(new, buf, 3, 1);
            }
            func_801F44F4(x - 1, y + 0x2E, 0x1E, D_801FC078[kind], 0);
            func_8002B56C(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                func_801F44F4(x - 1, y + 0x27, 0x1E, 5, 0);
            } else if (new < cur) {
                func_801F44F4(x - 1, y + 0x28, 0x1E, 6, 0);
            }
        }
        func_80028204(x, y, 0x1E, func_80028954((u8)D_801FC2D0.unk18[i]), 0x2D, !ok);
    }
}

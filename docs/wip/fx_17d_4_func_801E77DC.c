/* Draft of fx_17d_4 func_801E77DC. Status: first draft, about 170 strict mismatches. Logic follows the original, but
 * the original keeps k in a stack slot, indexes D_801E9724 through a j*4 induction variable and reloads
 * D_801E9724[j][1] in the inner loop. fx_1bf_0 func_801E77C0 is the same code with 8 different words. */
#include "fxmod/fx_17d_4.h"

void func_801E77DC(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 sc, s32 alpha, s32 ot, s32 flag) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);
    SVECTOR pos;
    SVECTOR a;
    SVECTOR b;
    s32 k;
    s32 j;
    s32 i;
    s32 cnt;
    s32 s;
    u8 *data;
    Fx17d4Pts *pts;
    Fx17d4Trail *tr;
    Fx17d4Pt *p;
    SVECTOR *d;

    data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x8A);
    sc = sc * 3 >> 1;
    for (k = 0; k < 3; k++) {
        for (j = 0; j < 18; j++) {
            pts = (Fx17d4Pts *)(data + *(u16 *)&((s32 *)data)[k * 18 + j]);
            tr = (Fx17d4Trail *)(D_801E97B4[j] + D_801E9724[j][2] * k);
            cnt = pts->unk2;
            tr->unk0 = tr->unk2 = cnt - 1;
            tr->unk1 = 0;
            s = sc * (D_801E9724[j][3] + 1);
            p = pts->unk4;
            d = tr->unk4;
            for (i = 0; i < cnt; i++, p++, d++) {
                if (i == 0) {
                    pos.vx = from->vx + p->x * 2;
                    pos.vy = from->vy + p->y * 2;
                    pos.pad = 0;
                    pos.vz = from->vz + p->z * 2;
                }
                if (n >= D_801E9724[j][1] + i) {
                    d->vx = from->vx + p->x * 2;
                    d->vy = from->vy + p->y * 2;
                    d->pad = s;
                    d->vz = from->vz + p->z * 2;
                    ((s32 *)&pos)[0] = ((s32 *)d)[0];
                    ((s32 *)&pos)[1] = ((s32 *)d)[1];
                    if (D_801E9724[j][1] + i == n && flag != 0) {
                        ((s32 *)&a)[0] = ((s32 *)d)[0];
                        ((s32 *)&a)[1] = ((s32 *)d)[1];
                        ((s32 *)&b)[0] = ((s32 *)d)[0];
                        ((s32 *)&b)[1] = ((s32 *)d)[1];
                        b.vy -= 0x1000;
                        func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &b, &a, 0, 0x800, 1, 0x32);
                    }
                } else {
                    ((s32 *)d)[0] = ((s32 *)&pos)[0];
                    ((s32 *)d)[1] = ((s32 *)&pos)[1];
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9724[j][0]), 0, (s32 *)from, 0, 0x1000,
                                alpha, 1, (u8 *)tr, ot);
        }
    }
}

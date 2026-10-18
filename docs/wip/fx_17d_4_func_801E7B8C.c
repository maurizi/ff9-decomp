/* Draft of fx_17d_4 func_801E7B8C. Status: 6 mismatches when registers are ignored. Structure and stack match;
 * ex/p/giv end up in s2/s3/s0 where the original has s3/s0/s2. fx_1bf_0 func_801E7B70 is the same code. */
#include "fxmod/fx_17d_4.h"

void func_801E7B8C(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    s32 k = 0;
    s16 *f = D_801E9814;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);
    Fx17d4Pt *p;
    s32 i;
    s32 a;

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    for (; k < 3; k++, f++) {
        p = (Fx17d4Pt *)((u8 *)ex->btlfx_getFileData(entry->unk8, *f) + 4);
        for (i = 0; i < 20; i++, p++) {
            if (i < n) {
                pos.vx = from->vx + p->x * 2;
                pos.vy = from->vy + p->y * 2;
                pos.vz = from->vz + p->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i & 7]), &pos, &rot, 0x3000,
                                        0, 1, ot, 0, 0);
            }
        }
    }
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 27; i++) {
            if (i <= n * 27 / 20) {
                a = ((i + k * 27) << 12) / 81;
                pos.vx = from->vx + ((ex->math_cos(a) * 1600) >> 12);
                pos.vy = from->vy;
                pos.vz = from->vz + ((ex->math_sin(a) * 1600) >> 12);
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i & 7]), &pos, &rot, 0x1800,
                                        0, 1, ot, 0, 0);
            }
        }
    }
}

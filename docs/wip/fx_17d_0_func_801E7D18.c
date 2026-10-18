/* Draft of func_801E7D18 (module fx_17d_0). Status: 6 aligned mismatches, all in the prologue: the original loads
 * the 6th parameter (scale, 0x7C) before the 5th (morph, 0x78). The table is indexed as s16 pairs through j = i * 4 +
 * 2 / + 3 (keeps (i << 3) | 4 and | 6 unreduced like the original), models is passed as an int and indexed with a
 * precomputed k = i << 1 (the original adds it to the spilled models after the branch). */
#include "fxmod/fx_17d_0.h"

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr);
/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E7D18(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s32 models, s16 *morph, s32 scale) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9470);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    s16 *f;
    s32 n;
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 21; i++) {
        j = i * 4 + 2;
        f = (s16 *)ex->btlfx_getFileData(entry->unk8, ((s16 *)D_801E9370)[j]);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        if (i < 16 || i >= 19 || frame < 0x28) {
            if (i != 16) {
                f = f + 2 + n * 6;
                sc.vx = sc.vy = sc.vz = scale;
            } else {
                f = f + 2 + n * 9;
                sc.vx = (f[6] * scale) >> 12;
                sc.vy = (f[7] * scale) >> 12;
                sc.vz = (f[8] * scale) >> 12;
            }
            pos.vx = ((f[0] * scale) >> 12) + origin->vx;
            pos.vy = ((f[1] * scale) >> 12) + origin->vy;
            pos.vz = ((f[2] * scale) >> 12) + origin->vz;
            rot.vx = f[3];
            rot.vy = f[4];
            rot.vz = -f[5];
            k = i << 1;
            if (*morph < 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, *(s16 *)(k + models));
            } else {
                j = i * 4 + 3;
                f = (s16 *)ex->btlfx_getFileData(entry->unk8, ((s16 *)D_801E9370)[j]);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, *(s16 *)(k + models), *morph, *(f = f + 2 + n * 3));
            }
        }
        morph++;
    }
}

/* Draft of func_801E7D18 (module fx_17d_0). Status: 8/167 aligned instructions with ccprobe; the original does not apply the SetRotMatrix the draft does.
 * Declarations it needs that are not yet in include/fxmod/fx_17d_0.h are repeated below. */
#include "fxmod/fx_17d_0.h"

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr);
/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E7D18(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s16 *models, s16 *morph, s32 scale) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9470);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    s16 *f;
    s16 *k;
    s32 n;
    s32 i;

    for (i = 0; i < 21; i++) {
        f = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801E9370[i].unk4);
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
            if (*morph < 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, models[i]);
            } else {
                f = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801E9370[i].unk6);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, models[i], *morph, f[2 + n * 3]);
            }
        }
        morph++;
    }
}

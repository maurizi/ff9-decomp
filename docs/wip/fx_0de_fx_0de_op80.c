/* Draft of fx_0de_op80 (module fx_0de). Status: 478/568 aligned instructions with ccprobe; structure matches; the r/g/b temporaries and the morph weight get different callee-saved registers.
 * Declarations it needs that are not yet in include/fxmod/fx_0de.h are repeated below. */
#include "fxmod/fx_0de.h"

typedef struct {
    s32 unk0; /* done */
    SVECTOR unk4; /* caster body position */
    SVECTOR unkC; /* caster position */
    s32 unk14[8]; /* models */
    s32 unk34; /* scale from the caster's size */
} Fx0deWork80; /* 0x38 */
typedef struct {
    u16 unk0;
    u16 unk2;
} Fx0deFlash;
extern Fx0deFlash D_801E85F0[];
extern BtlModel D_801E86A8[];
s32 fx_0de_op80(s32 mode, Fx0deWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0de_op80(s32 mode, Fx0deWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR rot;
    VECTOR pos;
    VECTOR sc;
    s32 frame;
    s32 t;
    s32 v;
    s32 r;
    s32 g;
    s32 m;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E8698 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk4.vx);
        ex->btlfx_getUnitPos(0, &work->unkC.vx);
        ex->btlfx_initEffModels(D_801E86A8, 8);
        work->unk14[0] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0x39, 0x3D00, 0, 0x80);
        work->unk14[1] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x39, 0x3D00, 0, 0x80);
        work->unk14[2] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x39, 0x3D00, 0, 0x80);
        work->unk14[3] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x39, 0x3D00, 0, 0x80);
        work->unk14[4] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x39, 0x3D00, 0, 0x80);
        work->unk14[5] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0x3B, 0x3D01, 0, 0x80);
        work->unk14[6] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 6), 0x3B, 0x3D01, 0, 0x80);
        work->unk14[7] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x39, 0x3D00, 0, 0x80);
        v = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (v < 0x400) {
            v = 0x6A4;
        }
        if (v > 0x1800) {
            v = 0x1800;
        }
        D_801E8CE8 = v;
        work->unk34 = v;
    } else {
        frame = entry->unk0;
        if (work->unk0 != 0) {
            return 0;
        }
        if (frame < 0x18) {
            t = (frame << 12) / 24;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            v = (((ex->math_sin(t >> 2) / 10) + 0x199) * work->unk34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            m = (0x1000 - t) << 2;
            rot.vy = -frame << 5;
            sc.vy = (v * 3) >> 2;
            sc.vz = v;
            sc.vx = v;
            if (m > 0x3000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[0], work->unk14[1], m - 0x3000);
            } else if (m > 0x2000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[1], work->unk14[2], m - 0x2000);
            } else if (m > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[2], work->unk14[3], m - 0x1000);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[3], work->unk14[4], m);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[7]);
            t >>= 2;
            r = (ex->math_sin(t) >> 5) - 0x80;
            g = (ex->math_sin(t) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk14[7], r, g, (ex->math_sin(t) >> 5) - 0x80);
        } else {
            m = ((frame - 0x18) << 12) / 26;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 5;
            v = (work->unk34 * 818) >> 12;
            sc.vz = v;
            sc.vx = v;
            sc.vy = (v * 3) >> 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[4]);
            m >>= 2;
            r = (ex->math_cos(m) >> 5) - 0x80;
            g = (ex->math_cos(m) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk14[4], r, g, (ex->math_cos(m) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14[7]);
            r = (ex->math_cos(m) >> 5) - 0x80;
            g = (ex->math_cos(m) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk14[7], r, g, (ex->math_cos(m) >> 5) - 0x80);
        }
        ex->func_800E6624(frame, 0x240, 0x180, 0x10, 0x40, D_801E85F0[frame % 23 + 1].unk0, D_801E85F0[frame % 23 + 1].unk2);
        if (frame >= 0x18) {
            t = ((frame - 0x18) << 12) / 26;
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            sc.vz = sc.vx = (work->unk34 << 11) >> 12;
            sc.vy = ((ex->math_sin(t >> 2) / 4 + 0x2AA) * work->unk34) >> 12;
            rot.vz = 0;
            rot.vx = 0;
            rot.vy = -frame << 6;
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk14[5], work->unk14[6], 0x1000 - t);
            t >>= 1;
            r = (ex->math_sin(t) >> 5) - 0x80;
            g = (ex->math_sin(t) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk14[5], r, g, (ex->math_sin(t) >> 5) - 0x80);
        }
        if (frame == 0x10) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame == 1) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk4);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
            }
        }
        if (frame >= 0x32) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

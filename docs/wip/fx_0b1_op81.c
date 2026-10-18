/* Draft of fx_0b1 fx_0b1_op81. Status: 8 aligned diff lines: the original reloads %hi(D_801EAA54) in each block; ours
 * keeps it in a register across the loop and fills the decrement into a delay slot. */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 D_801E8B90; /* frame */
extern Fx0b1Track D_801E8B9C[16];
extern SVECTOR D_801E8CDC[];
extern s32 D_801EAA54; /* fade timer */
extern s32 D_801EAA5C[4]; /* models */
extern SVECTOR D_801EAA6C; /* model rotation */
extern VECTOR D_801EAA7C; /* model scale */
extern VECTOR D_801EAA8C[4]; /* model positions */
extern SVECTOR D_801EAACC[4]; /* unit body positions */
extern BtlModel D_801EAAEC[];

void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);
void func_801E7788(void);
extern Unk800F78A4 *D_801E8B8C;
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

s32 fx_0b1_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EAA54 = 0;
        D_801E8B8C = env;
        ex->btlfx_initEffModels(D_801EAAEC, 4);
        for (i = 0; i < env->unkC->unk24; i++) {
            D_801EAA5C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EAACC[i]);
            D_801EAA8C[i].vx = D_801EAACC[i].vx;
            D_801EAA8C[i].vy = D_801EAACC[i].vy;
            D_801EAA8C[i].vz = D_801EAACC[i].vz + 0x100;
            D_801EAACC[i].vz += 0x200;
        }
        D_801EAA6C.vx = 0;
        D_801EAA6C.vy = 0x800;
        D_801EAA6C.vz = 0;
        D_801EAA7C.vx = 0x1800;
        D_801EAA7C.vy = 0x1800;
        D_801EAA7C.vz = 0x1800;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            D_801EAA54 = 0x20;
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&D_801EAACC[i]);
                ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&D_801EAACC[i]);
            }
        }
        if (D_801EAA54 != 0) {
            D_801EAA54--;
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->Hi_DrawEffModel(&D_801EAA6C, &D_801EAA8C[i], (s32)&D_801EAA7C, D_801EAA5C[i]);
                if (D_801EAA54 >= 0x19) {
                    c = (0x18 - D_801EAA54) * 0x10;
                    ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                }
                if (D_801EAA54 < 8) {
                    c = (D_801EAA54 - 8) * 0x10;
                    ex->Hi_ModifyEffModelRGB(D_801EAA5C[i], c, c, c);
                }
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x22), frame);
        }
        return frame >= 0x20;
    }
    return 0;
}

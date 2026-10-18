/* Draft of fx_0b3 fx_0b3_op80. Status: 290/299, 6 aligned diff lines: original computes frame<<12 and its sign word
 * before the frame<20 branch (sll v1 / sra a1,v1,31 in the delay slot); ours puts the sll in the delay slot.
 * Multiply/ternary/temporary rewrites tried. */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* enemy positions */
    s32 unk40; /* unit count */
} Fx0b3Work86;

extern s32 D_801EB5D4;
extern SVECTOR D_801EE340[16];

typedef struct {
    u16 unk0; /* file id, track 0 */
    u16 unk2; /* file id, track 1 */
    u16 unk4; /* end frame */
    u16 unk6;
} Fx0b3Track87;

extern Fx0b3Track87 D_801EB5E8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b3Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    Fx0b3Vec3 unk28; /* morph model position */
    Fx0b3Vec3 unk34; /* morph model scale */
    SVECTOR unk40; /* morph model rotation */
    Fx0b3Vec3 unk48; /* model position */
    Fx0b3Vec3 unk54; /* model scale */
    SVECTOR unk60; /* model rotation */
    Pool6CEntry *unk68;
    s32 unk6C; /* model */
    s32 unk70; /* morph frame */
    s32 unk74; /* morph weight */
} Fx0b3Work80; /* 0x78 */

extern BtlModel D_801ED5F0[];

typedef struct {
    u8 pad0[0x184];
    BtlUnit9CC *unk184;
    BtlUnit9CC *unk188;
    BtlUnit9CC *unk18C;
    BtlUnit9CC *unk190;
} Fx0b3Unit;

typedef struct {
    u8 pad0[0x54];
    Fx0b3Unit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0b3Summon;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10; /* morph model position */
    u8 pad18[0x28 - 0x18];
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    Fx0b3Vec3 unk40; /* position */
    Fx0b3Vec3 unk4C; /* scale */
    SVECTOR unk58; /* rotation */
    Fx0b3Vec3 unk60;
    Fx0b3Vec3 unk6C;
    SVECTOR unk78;
    Pool6CEntry *unk80;
    Pool6CEntry *unk84;
    s32 unk88[4]; /* models */
    s32 unk98; /* morph frame */
    s32 unk9C; /* morph weight */
} Fx0b3Work81; /* 0xA0 */

extern s32 D_801EE270; /* model */
extern s32 D_801EB5CC[2]; /* file ids */
extern u8 D_801EB660[];
extern u8 D_801ED0A0[];
extern SVECTOR D_801EE2A0[20];

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[0x28 - 0x8];
    SVECTOR unk28;
    SVECTOR unk30;
    u8 pad38[0x48 - 0x38];
    Fx0b3Vec3 unk48; /* position */
    Fx0b3Vec3 unk54; /* scale */
    SVECTOR unk60; /* rotation */
    Fx0b3Vec3 unk68;
    Fx0b3Vec3 unk74;
    SVECTOR unk80;
    s32 unk88; /* model */
    s32 unk8C; /* model */
} Fx0b3Work84; /* 0x90 */

void func_801E8DF4(SVECTOR *pos, SVECTOR *rot, Fx0b3Vec3 *scale, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9,
                   s32 a10, s32 a11, s32 a12, s32 a13, s32 a14);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[0x18 - 0x8];
    SVECTOR unk18;
    SVECTOR unk20;
    u8 pad28[0x38 - 0x28];
    Fx0b3Vec3 unk38; /* position */
    Fx0b3Vec3 unk44; /* scale */
    SVECTOR unk50; /* rotation */
    Fx0b3Vec3 unk58;
    Fx0b3Vec3 unk64;
    SVECTOR unk70;
    s32 unk78[4]; /* models */
} Fx0b3Work83; /* 0x88 */
extern s32 D_801EB5AC[4]; /* start frames */
extern s32 D_801EB5BC[4]; /* file ids */

extern u16 D_801EB5A0[6]; /* morph model file ids */
extern s32 D_801EE280[6]; /* morph models */


s32 func_801E9DB8(s32 a);
s32 func_801EA6C8(SVECTOR *v);
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0b3Track;
void func_801E7704(Fx0b3Track *track, s32 t, SVECTOR *out);
void func_801E7754(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E77DC(Fx0b3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E7890(Fx0b3Track *track, s32 t);
extern Unk800F78A4 *D_801ED5E0;
void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10; /* midpoint */
    SVECTOR unk18;
    u8 pad20[8];
    Fx0b3Vec3 unk28; /* summon position */
    Fx0b3Vec3 unk34; /* summon scale */
    SVECTOR unk40; /* summon rotation */
    u8 pad48[0x68 - 0x48];
    Pool6CEntry *unk68;
} Fx0b3Work85; /* 0x6C */

extern CVECTOR D_801EB5D8;
void func_801E9DFC(SVECTOR *a, SVECTOR *b, VECTOR *c, s32 r, s32 k, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10,
                   s32 a11, s32 a12, s32 a13, s32 a14);
s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0b3_op80(s32 mode, Fx0b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR ang;
    s32 frame;
    s32 c;
    s32 i;
    s32 n;
    s32 t;
    s32 u;
    s32 x;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work80);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED5E0 = env;
        ex->btlfx_initEffModels(D_801ED5F0, 0x10);
        work->unk6C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 1);
        work->unk34.vx = 0x1800;
        work->unk34.vy = 0x1800;
        work->unk34.vz = 0x1800;
        work->unk54.vx = 0x2800;
        work->unk54.vy = 0x2800;
        work->unk54.vz = 0x2800;
        work->unk40.vx = 0;
        work->unk40.vy = 0;
        work->unk40.vz = 0;
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk28.vz = 0;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk48.vx = 0;
        work->unk48.vy = -0x100;
        work->unk48.vz = 0;
        work->unk68 = NULL;
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = frame << 12;
            if (frame < 20) {
                c = t / 20;
            } else {
                c = 0x1000;
            }
            ex->Hi_DrawEffModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            if (frame == 20) {
                for (i = 0; i < 6; i++) {
                    D_801EE280[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801EB5A0[i]), 0);
                }
                work->unk74 = 0x1000;
                work->unk70 = 0;
                work->unk68 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (work->unk68 != NULL) {
                    work->unk68->unk22 = 0x80;
                }
            }
            if (frame >= 20) {
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), frame, &v);
                work->unk28.vx = v.vx;
                work->unk28.vy = v.vy;
                work->unk28.vz = v.vz;
                BTLFX_COPY_SVEC(&work->unk10, &v);
                work->unk74 -= 0x800;
                if (work->unk74 <= 0) {
                    work->unk74 = 0x1000;
                    if (++work->unk70 >= 6) {
                        work->unk70 = 0;
                    }
                }
                ang.vx = 0;
                ang.vy = 0x800;
                ang.vz = 0;
                if (work->unk68 != NULL) {
                    if (work->unk68->unk30 == -1) {
                        work->unk68 = NULL;
                    }
                    if (work->unk68 != NULL) {
                        work->unk68->unk5C.vx = ang.vx;
                        work->unk68->unk5C.vy = ang.vy;
                        work->unk68->unk5C.vz = ang.vz;
                    }
                }
                n = work->unk70 + 1;
                if (n >= 6) {
                    n = 0;
                }
                ex->Hi_DrawMorphEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34,
                                         D_801EE280[work->unk70], D_801EE280[n], work->unk74);
                if (frame < 8) {
                    x = ((frame << 12) >> 8) - 0x80;
                    ex->Hi_ModifyEffModelRGB(D_801EE280[work->unk70], x, x, x);
                }
            }
            if (frame >= 0x33) {
                u = (frame - 0x33) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, u, u, u);
            }
            if (frame >= 0x3B) {
                ex->Hi_FreeEffModel(work->unk6C);
                ex->func_800E3D54(work->unk68);
                return 1;
            }
        }
        ex->func_800E6624(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        work->unk4++;
    }
    return 0;
}

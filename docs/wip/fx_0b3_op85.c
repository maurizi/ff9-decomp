/* Draft of fx_0b3 fx_0b3_op85. Status: 60 aligned mismatches, logic complete. Open: g + (((i & 1) << 12) + 0x3800)
 * gets g + 0x3800 hoisted out of the loop in ours (the original adds per iteration), the case-0 loop counters are
 * allocated in swapped registers, and some scheduling around the 9DFC calls. Stack layout matches (shared
 * SVECTOR/VECTOR slots at 0x68..0x88). */
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

s32 fx_0b3_op85(s32 mode, Fx0b3Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR q;
    SVECTOR a;
    SVECTOR b;
    VECTOR c;
    SVECTOR p1;
    SVECTOR p2;
    VECTOR scale;
    Pool6CEntry *p;
    SVECTOR a2;
    SVECTOR b2;
    VECTOR c2;
    s32 frame;
    s32 t;
    s32 f;
    s32 d;
    s32 m;
    s32 s;
    s32 u;
    s32 k;
    s32 r;
    s32 g;
    s32 i;
    s32 j;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work85);
    } else if (mode == 1) {
        work->unk40.vy = 0x800;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk40.vx = 0;
        work->unk40.vz = 0;
        work->unk34.vx = 0x1000;
        work->unk34.vy = 0x1000;
        work->unk34.vz = 0x1000;
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk28.vz = 0;
        D_801ED5E0 = env;
        ex->Hi_SetSummonMotion(((Fx0b3Summon *)env->unk4)->unk54->unk18C, ((Fx0b3Summon *)env->unk4)->unk60);
        ex->Hi_SetSummonMotFrame(((Fx0b3Summon *)env->unk4)->unk60, 0x5A);
        return 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 81;
            if (frame == 0x12) {
                work->unk68 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&work->unk10);
                if (work->unk68 != NULL) {
                    work->unk68->unk24->unk0 = -1;
                    ((EffTailSpawn *)work->unk68->unk24)->unk3C = (s32 (*)(SVECTOR *, SVECTOR *, s32, s32))func_801EA6C8;
                    if (work->unk68 != NULL) {
                        work->unk68->unk22 = 0x200;
                    }
                }
            }
            if (frame >= 9) {
                if (frame < 0x3D) {
                    f = ((frame - 9) << 12) / 52;
                } else {
                    f = 0x1000;
                }
                if (frame < 0x14) {
                    m = 1;
                    a.vx = 0;
                } else {
                    a.vx = 0x1000 - (t >> 1);
                    m = 2;
                }
                s = ex->math_sin(t >> 2);
                u = f >> 2;
                a.vy = ((0x1000 - s) >> 2) - 0x400 - ((ex->math_cos(u) * 0x44C) >> 12);
                a.vz = ((0x1000 - ex->math_cos(u)) << 2) - 0x4000;
                b.vx = 0x400;
                b.vy = 0x400;
                b.vz = 0;
                c.vy = 0x1000;
                c.vx = 0x1000;
                c.vz = ex->math_cos(u);
                if (frame < 0x10) {
                    d = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    d = 0x1800;
                }
                k = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    k = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    k = 0x32;
                }
                r = 0x80;
                if (frame >= 0x3D) {
                    r = 0x80 - (((frame - 0x3D) << 5) / 20);
                }
                if (frame < 0x29) {
                    env->unk28 = 0;
                } else {
                    env->unk28 = -0x80;
                }
                func_801E9DFC(&a, &b, &c, r, k, 0x17, 0x3D07, 0, 0x80, 0x7D0, f * m, -0x140, d,
                              ex->math_cos(f >> 2) + 0x400, 0x100);
                g = (t * 3) >> 3;
                env->unk28 = 0x7C;
                for (i = 0, j = frame; i < 18; i++, j += 2) {
                    if (D_801EE340[i].pad != 0) {
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &D_801EE340[i], NULL,
                                                g + (((i & 1) << 12) + 0x3800), j, 1, 0x1000);
                    }
                }
                env->unk28 = 0x37;
                if (frame >= 0x1D) {
                    for (i = 0, j = frame; i < 7; i++, j += 2) {
                        if (D_801EE340[i].pad != 0) {
                            ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &D_801EE340[i], NULL,
                                                    0x2400, j + 4, 1, 0x1000);
                        }
                    }
                }
                env->unk28 = 0;
            }
            if (frame < 0x10) {
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), frame, &q);
                work->unk28.vx = q.vx;
                work->unk28.vy = q.vy;
                work->unk28.vz = q.vz;
                ex->Hi_DrawSummonModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34,
                                       ((Fx0b3Summon *)env->unk4)->unk60, 1);
            }
            if (frame == 0x32) {
                p1.vx = env->unk2C->unk0[0].vx;
                p1.vy = env->unk2C->unk0[0].vy;
                p1.vz = env->unk2C->unk0[0].vz;
                p2.vx = env->unk2C->unk0[1].vx;
                p2.vy = env->unk2C->unk0[1].vy;
                p2.vz = env->unk2C->unk0[1].vz;
                work->unk10.vx = (p1.vx + p2.vx) / 2;
                work->unk10.vy = (p1.vy + p2.vy) / 2;
                work->unk10.vz = (p1.vz + p2.vz) / 2;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame >= 0x2B) {
                s = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(s);
                ex->math_cos(s);
                ex->func_800D0B18((s32)(env->unk20 + 2), -0x20, 0x10, 3, 0x50, 1, &D_801EB5D8, 0x18, frame << 8, 0x186,
                                  0x100);
                if (frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 0, 0x20, 0xAA, 0xFF);
                }
                if (frame == 0x2D || frame == 0x32) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 0, 0, 0x64, 0xFF);
                }
            }
            if (frame >= 0x2B) {
                k = frame - 0x2B;
                p2.vx = 0;
                p2.vy = 0;
                p2.vz = 0;
                scale.vz = 0x1000;
                ((CVECTOR *)&p1)->r = ((CVECTOR *)&p1)->g = ((CVECTOR *)&p1)->b = -0x10 - (k * 0x8D) / 38;
                scale.vx = scale.vy = 0x1400 - (k << 10) / 38;
                ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &p2, &scale, (CVECTOR *)&p1, 0, NULL);
            }
            func_801E78BC(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x51) {
                work->unk0 = 1;
                work->unk18.vx = 0;
                work->unk18.vy = -0x64;
                work->unk18.vz = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&a2);
                if (a2.vy < -0x800) {
                    work->unk18.vz = 0x300;
                    work->unk18.vy -= 0x1F4;
                }
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 6;
            u = t >> 2;
            work->unk18.vy -= (ex->math_cos(u) * 0x18) >> 12;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            a2.vx = 0;
            a2.vy = -0x200 - (ex->math_cos(u) >> 3);
            b2.vx = 0x400;
            b2.vy = 0x400;
            a2.vz = 0;
            b2.vz = 0;
            c2.vy = 0x1000;
            c2.vx = 0x1000;
            c2.vz = 0;
            s = ex->math_cos(u) >> 5;
            func_801E9DFC(&a2, &b2, &c2, s, s, 0x17, 0x3D07, 0, 0x80, (ex->math_cos(u) >> 3) + 0x300, u, 0x100, 0, 0,
                          0);
            func_801E78BC(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

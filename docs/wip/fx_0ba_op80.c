/* Draft of fx_0ba fx_0ba_op80. Status: full draft, 54 aligned diff lines ignoring registers: the frame==0x3C copy of
 * unk48 into unk10 (original keeps both stores and re-reads unk10.vx for D_801ECAB8), the morph key index scheduling,
 * and the c accumulator sequence. The bone buffer shares the mpos stack slot here (block locals gave a worse layout). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1];
} Fx0baStrip;

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* trail index */
    s16 unk16; /* point index */
    u8 *unk18; /* strip point */
} Fx0baPart; /* 0x1C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[17];
} Fx0baTrail; /* 0x8C */

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* rotation */
} Fx0baWork85; /* 0x10 */

extern Fx0baTrail D_801EED20[5];

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* path index */
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15; /* amplitude */
    u8 pad16[2];
    s32 *unk18; /* arc profile */
} Fx0baBolt;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[4];
} Fx0baStrip4; /* 0x24 */

extern s32 D_801EC550;
extern SVECTOR D_801EC554;
extern Fx0baTrail D_801ED420[];
extern Fx0baStrip4 D_801EE5A0[];
extern s32 D_801EC54C;
extern SVECTOR D_801ECAB0;

typedef struct {
    SVECTOR unk0[17];
    s32 unk88;
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *D_801EC5F0;
extern Fx0baPath D_801ED424[32];
extern s32 D_801EEFDC;

typedef struct {
    s32 *unk0; /* arc profile table */
    s32 unk4; /* spawn counter */
    s32 unk8; /* state */
} Fx0baWork83;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad[3];
} Fx0baPt; /* 0xC */

extern s32 D_801EEA20[];
extern s32 D_801EC55C;
extern SVECTOR D_801ECAB8;

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b);
void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad);
void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave);
void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad);
s32 func_801EB1CC(SVECTOR *v);
s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
s32 func_801EAC08(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801EC124(void);
void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to);
s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
    BtlUnit9CC *unk184;
    BtlUnit9CC *unk188;
} Fx0baUnit;

typedef struct {
    u8 pad0[0x54];
    Fx0baUnit *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0baSummon;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0baVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    u8 pad8[8];
    SVECTOR unk10; /* orb position */
    SVECTOR unk18; /* bone position */
    u8 pad20[8];
    Fx0baVec3 unk28; /* model position */
    Fx0baVec3 unk34; /* model scale */
    SVECTOR unk40; /* model rotation */
    Fx0baVec3 unk48; /* summon position */
    Fx0baVec3 unk54; /* summon scale */
    SVECTOR unk60; /* summon rotation */
    SVECTOR unk68; /* model rotation */
    Fx0baVec3 unk70; /* model scale */
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s16 unk94;
    s16 unk96;
    s16 unk98;
    s16 unk9A;
    u8 pad9C[4];
} Fx0baWork80; /* 0xA0 */

extern BtlModel D_801EC600[];
s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR mpos;
    VECTOR scale;
    s32 tint;
    s32 c;
    s32 tint2;
    s32 fade;
    s32 bright;
    s32 k;
    Pool6CEntry *p;
    s16 *keys;
    s32 frame;
    s32 state;
    s32 t;
    s32 n;

    if (mode == 0) {
        *arg4 = sizeof(Fx0baWork80);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC5F0 = env;
        ex->btlfx_initEffModels(D_801EC600, 6);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0);
        work->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0);
        work->unk48.vx = 0;
        work->unk60.vy = 0x800;
        work->unk54.vx = 0x1000;
        work->unk54.vy = 0x1000;
        work->unk54.vz = 0x1000;
        work->unk48.vy = -0x1770;
        work->unk48.vz = -0xA00;
        work->unk60.vx = 0;
        work->unk60.vz = 0;
        D_801ECAB0.vx = work->unk48.vx;
        D_801ECAB0.vy = work->unk48.vy;
        D_801ECAB0.vz = work->unk48.vz;
        work->unk68.vy = 0x600;
        work->unk68.vx = 0;
        work->unk68.vz = 0;
        work->unk70.vx = 0x6000;
        work->unk70.vy = 0x6000;
        work->unk70.vz = 0x6000;
        work->unk18.vx = work->unk48.vx;
        work->unk18.vy = work->unk48.vy;
        work->unk18.vz = work->unk48.vz;
        env->unk2C->unk0[3].vx = work->unk18.vx;
        env->unk2C->unk0[3].vy = work->unk18.vy;
        env->unk2C->unk0[3].vz = work->unk18.vz;
        env->unk2C->unk0[3].vy -= 0x190;
        D_801ECAB8.vz = 0;
        D_801ECAB8.vy = 0;
        D_801ECAB8.vx = 0;
        work->unk96 = 0;
        work->unk94 = 0;
        work->unk9A = 0;
        work->unk98 = 0;
    } else {
        bright = 0;
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            if (frame < 6) {
                bright = (frame << 12) / 6;
            } else {
                bright = 0x1000;
            }
            tint = 0;
            ex->func_800CFC2C(frame, 0xC, 4, -1, 0x18, &tint);
            ex->func_800CFC2C(frame, 0x14, 4, -1, 0x20, &tint);
            if (tint != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x100), 1, tint, tint, tint >> 1);
            }
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk180, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            break;
        case 1:
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            t = (frame << 12) / 60 + 0x23;
            pos.vy += ex->math_sin(t * 2) >> 7;
            D_801ECAB0.vx = pos.vx;
            D_801ECAB0.vy = pos.vy;
            D_801ECAB0.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->unk60, &pos, (s32)&work->unk54, ((Fx0baSummon *)env->unk4)->unk60, 0);
            ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->unk4)->unk60, 0x33, (u16 *)&work->unk18);
            work->unk18.vy += 0x40;
            env->unk2C->unk0[2].vx = work->unk18.vx;
            env->unk2C->unk0[2].vy = work->unk18.vy;
            c = 0;
            env->unk2C->unk0[2].vz = work->unk18.vz;
            ex->func_800CFC2C(frame, 2, 7, -1, 0x555, &c);
            ex->func_800CFC2C(frame, 0xC, 2, 0, 0x1800, &c);
            ex->func_800CFC2C(frame, 0xE, 0xA, -1, 0x1800, &c);
            ex->func_800CFC2C(frame, 0x18, 2, 0, 0x2000, &c);
            ex->func_800CFC2C(frame, 0x1A, 0xF, -1, 0x2000, &c);
            c = c + 0x1000 - ex->math_cos(t >> 2);
            ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->unk4)->unk60, (c >> 5) - 0x80, (c >> 5) - 0x80,
                                        (c >> 5) - 0x80);
            tint2 = 0;
            ex->func_800CFC2C(frame, 0, 6, -1, 0xBE, &tint2);
            ex->func_800CFC2C(frame, 0xC, 6, -1, 0x80, &tint2);
            ex->func_800CFC2C(frame, 0x18, 0xA, -1, 0xF0, &tint2);
            bright = 0x1000;
            if (tint2 != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xC8), 1, tint2, tint2, tint2 / 3);
            }
            if (frame == 0xC) {
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 1);
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 2);
            }
            if (frame == 0x18) {
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 1);
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 2);
            }
            if (frame == 0x3B) {
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk184, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            if (frame >= 0x3C) {
                if (frame == 0x3C) {
                    work->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 1);
                    work->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 1);
                    work->unk10.vy = work->unk48.vy;
                    work->unk10.vx = work->unk48.vx;
                    work->unk10.vz = work->unk48.vz;
                    work->unk10.vz += 0x400;
                    work->unk10.vy += 0x2F3;
                    D_801ECAB8.vx = work->unk10.vx;
                    D_801ECAB8.vy = work->unk10.vy;
                    D_801ECAB8.vz = work->unk10.vz;
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x86), (s32)&work->unk10);
                    if (p != NULL) {
                        p->unk22 = 0x37;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x84), (s32)&work->unk10);
                    if (p != NULL) {
                        p->unk22 = 0x37;
                    }
                    D_801EC550 = 0;
                }
                bright = 0;
                ex->func_800CFC2C(frame, 0x3C, 0x23, 0, 0x1000, &fade);
                keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x85);
                n = frame - 0x3C;
                if (n >= 0x1E) {
                    n = 0x1D;
                }
                scale.vz = 0x1000;
                scale.vy = 0x1000;
                scale.vx = 0x1000;
                mpos.vx = work->unk48.vx;
                mpos.vy = work->unk48.vy;
                mpos.vy += 0x2F3;
                mpos.vz = work->unk48.vz;
                mpos.vz += 0x400;
                ex->Hi_DrawMorphEffModel(&work->unk68, &mpos, (s32)&scale, work->unk84, work->unk88, keys[n * 3 + 2]);
                env->unk28 = 0x100;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x27), &work->unk10, NULL, fade * 3 + 0x1000,
                                  0, 1, ex->math_cos(fade >> 2));
                env->unk28 = 0;
                k = ((fade * 3 / 4 + 0x400) >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->unk4)->unk60, k, k, k);
                ex->func_800E6624(10000 - work->unk4 * 7, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->func_800E6624(10000 - work->unk4 * 7, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            } else {
                ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->func_800E6624(work->unk4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            }
            if (frame >= 0x5F) {
                work->unk0 = 2;
                entry->unk0 = -1;
                D_801ECAB8.vz = 0;
                D_801ECAB8.vy = 0;
                D_801ECAB8.vx = 0;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk188, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
                work->unk18.vx = work->unk48.vx;
                work->unk18.vy = work->unk48.vy;
                work->unk18.vz = work->unk48.vz;
                work->unk18.vy -= 0x127;
                work->unk18.vz += 0x80;
                env->unk2C->unk0[2].vx = work->unk18.vx;
                env->unk2C->unk0[2].vy = work->unk18.vy;
                env->unk2C->unk0[2].vz = work->unk18.vz;
                work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 1);
                work->unk90 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 1);
            }
            break;
        case 2:
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            D_801ECAB0.vx = pos.vx;
            pos.vz = work->unk48.vz;
            D_801ECAB0.vy = pos.vy;
            D_801ECAB0.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->unk60, &pos, (s32)&work->unk54, ((Fx0baSummon *)env->unk4)->unk60, 0);
            work->unk48.vy -= 0x1000;
            ex->Hi_DrawEffModel(&work->unk68, (VECTOR *)&work->unk48, (s32)(VECTOR *)&work->unk70, work->unk80);
            work->unk48.vy += 0x1000;
            if (frame == 0x14) {
                ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->unk4)->unk60, 0x2F, (u16 *)&mpos);
                work->unk28.vx = ((SVECTOR *)&mpos)->vx;
                work->unk28.vy = ((SVECTOR *)&mpos)->vy;
                work->unk40.vx = 0x1DA;
                work->unk40.vy = 0;
                work->unk40.vz = 0;
                work->unk34.vz = 0x1000;
                work->unk34.vy = 0x1000;
                work->unk34.vx = 0x1000;
                work->unk28.vz = ((SVECTOR *)&mpos)->vz;
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            if (frame >= 0x14) {
                work->unk28.vy += 0xAE;
                work->unk28.vz += 0x96;
                ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk8C);
                ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk90);
                k = ((((frame - 0x16) * 0x180) + 0x400) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk90, k, k, k);
                ex->func_800E6624(work->unk4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
            }
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            if (frame >= 0x1E) {
                work->unk0 = state;
                D_801EC54C = 1;
                return 1;
            }
            break;
        }
        if (bright != 0) {
            ex->Hi_DrawEffModel(&work->unk68, (VECTOR *)&work->unk48, (s32)(VECTOR *)&work->unk70, work->unk7C);
            k = (bright >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk7C, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}

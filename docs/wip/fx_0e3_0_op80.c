/* Draft of fx_0e3_0 fx_0e3_0_op80.
 * Status (pass 3, build/w/cmp.py): 16 mismatched rows ignoring registers and stack offsets (was 312), 190 counting
 * registers (was 936); frame size matches (400), same length as the original (2501). ex/work now get s2/s3 like the original:
 * the multi-role variables were split by role (build/w/greedy.py with split2.py; the _bNNN names are those splits).
 * Pass 3 levers: modes 0/1 fall through to one `return 0` (if/else chain; `if (done) return 1;` before
 * work->unk4++); unk70.vz copied right after vx/vy; nbolt = 1 after the summon draw call (cse no longer folds the
 * call's constant 1 into nbolt); the unk10 stores in vx, vy order; (0x1000 - cos) * 3 - 0x2000 in a block local;
 * case-local RGB/tint values in block locals; case 10's own c; the case 0 loop reuses i (header) for j and d for the
 * loop's i (orig s1/s0); `a - K - (call >> n)` as cz/vz temps (fold otherwise reassociates); case 4's t division
 * first and d = t >> 2; amp4/amp5 = 0x37 variables for the ring loops (spilled, rematerialised into t1 like the
 * original); case 1's unk50 = 1 after the unk10.vz store (forces the unk10.vy reload).
 * Layout notes (pass 2): a block variable whose address is taken in a statement directly in its block is moved to
 * the parent temp level (preserve_temp_slots), so it outlives the block; case 4 needs the switch (frame) { default: }
 * wrapper; case 1 col needs 8-byte accounting (union hack; combine_temp_slots adds size, not full_size). */
#include "fxmod/fx_0e3_0.h"

typedef struct {
    s32 vx, vy, vz;
} Fx0e30Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* summon position */
    Fx0e30Vec3 unk10; /* summon model position */
    Fx0e30Vec3 unk1C; /* summon model scale */
    SVECTOR unk28; /* summon model rotation */
    SVECTOR unk30; /* camera */
    SVECTOR unk38; /* bone position */
    SVECTOR unk40; /* bone position */
    s32 unk48;
    Pool6CEntry *unk4C;
    s32 unk50; /* bolts need a reset */
    s32 unk54; /* models */
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    SVECTOR unk68;
    Fx0e30Vec3 unk70;
    Fx0e30Vec3 unk7C;
    SVECTOR unk88;
    Fx0e30Vec3 unk90;
    Fx0e30Vec3 unk9C;
} Fx0e30Work80; /* 0xA8 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
        BtlUnit9CC *unk184;
        BtlUnit9CC *unk188;
        BtlUnit9CC *unk18C;
        BtlUnit9CC *unk190;
        BtlUnit9CC *unk194;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e30Summon;

typedef struct {
    s16 unk0;
    s16 unk2;
} Fx0e30BonePair;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[13];
} Fx0e30Arc; /* 0x6C */

extern SVECTOR D_801EA7E8;
extern s16 D_801EA7F0[];
extern Fx0e30BonePair D_801EA800[8];
extern Fx0e30Seg D_801EA878[4][15];
extern u8 D_801EAC38[4][0x12C];
extern Fx0e30Arc D_801EB0E8[];
extern Fx0e30Bolt D_801EB388[];
extern BtlModel D_801EB6A8[];

#define SUMMON(env) ((Fx0e30Summon *)(env)->unk4)

s32 fx_0e3_0_op80(s32 mode, Fx0e30Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 fade;
    s32 trail;
    s32 nbolt;
    s32 done;
    s32 i;
    s32 i_b526;
    s32 i_b635;
    s32 i_b671;
    s32 i_b648;
    s32 i_b138;
    s32 j;
    s32 k;
    s32 a;
    s32 a_b664;
    s32 b;
    s32 c;
    s32 c_r365;
    s32 c_r136;
    s32 c_r136_b161;
    s32 c_r277;
    s32 d;
    s32 d_b695;
    s32 r;
    s32 x;
    s32 x_b388;
    s32 y;
    s32 y_b158;
    s32 y_b680;
    s32 y_b657;
    s32 n;
    s32 e;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 f4;
    Pool6CEntry *p;
    Fx0e30Puff *q;

    if (mode == 0) {
        init->unk0 = 0xA8;
        init->unk8 = 0x10;
        init->unk14 = (s32)func_801E7F48;
        init->unk4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EA868 = env;
        ex->btlfx_initEffModels(D_801EB6A8, 0x10);
        work->unk60 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 0);
        work->unk64 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk70.vx = D_801EA7E8.vx;
        work->unk70.vy = D_801EA7E8.vy;
        work->unk70.vz = D_801EA7E8.vz;
        work->unk7C.vx = 0x1000;
        work->unk7C.vy = 0x1000;
        work->unk7C.vz = 0x1000;
        work->unk68.vx = 0;
        work->unk68.vy = 0;
        work->unk68.vz = 0;
        work->unk9C.vx = 0x800;
        work->unk9C.vy = 0x800;
        work->unk9C.vz = 0x800;
    } else {
    fade = 0;
    trail = 0;
    nbolt = 0;
    done = 0;
    frame = entry->unk0;
    {
    switch (work->unk0) {
    case 0: {
        SVECTOR rot;
        VECTOR sc;
        VECTOR pos;
        SVECTOR rot2;
        VECTOR sc2;
        VECTOR pos2;

        t = (frame << 12) / 69;
        if (frame < 0xC) {
            s32 v = (ex->math_cos((frame << 10) / 12) * 0xFF) >> 12;

            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, v, v, v);
        }
        if (frame >= 0x18) {
            if (frame == 0x18) {
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180, SUMMON(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            }
            r = frame - 0x18;
            i_b138 = (r << 12) / 45;
            b = (ex->math_sin(((i_b138 >> 1) * 3) / 2) >> 3) + 0x800;
            c_r136_b161 = (((0x1000 - i_b138) * 0x4B0) >> 12) + 0x18;
            if (i_b138 < 0xC00) {
                d = 0x1800;
                y_b158 = ex->math_sin(i_b138 / 3) * 2 - 0x1000;
            } else {
                d = i_b138 - 0xC00;
                y_b158 = ex->math_sin(d) * 3 + 0x1000;
                d = 0x1800 - d * 4;
            }
            work->unk10.vx = work->unk8.vx + ((ex->math_cos(b) * c_r136_b161) >> 12);
            work->unk10.vy = work->unk8.vy + y_b158;
            work->unk10.vz = work->unk8.vz + ((ex->math_sin(b) * c_r136_b161) >> 12);
            work->unk28.vx = 0x400;
            work->unk28.vy = -b * 2 + 0xC00;
            work->unk1C.vx = work->unk1C.vy = work->unk1C.vz = d;
            ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
            nbolt = 1;
            ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, (ex->math_cos(i_b138 >> 2) >> 5) - 0x80,
                                        (ex->math_cos(i_b138 >> 2) >> 5) - 0x80, (ex->math_cos(i_b138 >> 2) >> 5) - 0x80);
            if (frame == 0x2C) {
                work->unk50 = nbolt;
            } else if (frame >= 0x2D) {
                trail = 0x1000;
            }
        }
        x = frame * 0xF5;
        c_r136 = frame << 5;
        pos.vx = work->unk8.vx;
        pos.vz = work->unk8.vz;
        rot.vz = 0;
        rot.vx = 0;
        for (f1 = 0; f1 < 9; f1++, x += 0x1C7, c_r136 += 0x1FF) {
            i_b138 = x;
            n = 2;
            while (i_b138 >= 0x1000 && n > 0) {
                i_b138 -= 0x1000;
                n--;
            }
            if (i_b138 <= 0x1000) {
                d = i_b138 >> 2;
                {
                    s32 kq = (0x1000 - ex->math_cos(d)) * 3 - 0x2000;
                    pos.vy = work->unk8.vy - kq;
                }
                rot.vy = c_r136;
                d = 0x1000 - ex->math_cos(d);
                sc.vx = sc.vy = sc.vz = ((i_b138 >> 1) + 0x800) / 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk64);
                a = ((d + 0x200) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk64, a, a, a);
            }
        }
        if (((frame & 1) && frame < 0x23) || frame >= 0x23) {
            q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (q != NULL) {
                q->unk0.vx = work->unk8.vx + ex->btlfx_randCentered(0x800);
                q->unk0.vz = work->unk8.vz + ex->btlfx_randCentered(0x800);
                q->unk0.vy = work->unk8.vy + 0x2000;
            }
        }
        pos2.vx = work->unk8.vx;
        pos2.vy = work->unk8.vy;
        pos2.vz = work->unk8.vz;
        pos2.vy += 0x6000;
        if (t < 0xC00) {
            e = t + 0x2000;
            sc2.vx = sc2.vy = sc2.vz = e;
        } else {
            e = (t - 0xC00) * 5 + 0x2C00;
            sc2.vx = sc2.vy = sc2.vz = e;
        }
        rot2.vz = 0;
        rot2.vx = t >> 4;
        rot2.vy = frame * 7;
        ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&sc2, work->unk60);
        {
            s32 a2 = (((t >> 1) + 0x800) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk60, a2, a2, a2);
        }
        BTLFX_COPY_SVEC(&work->unk30, &work->unk8);
        work->unk30.vx += (ex->btlfx_randCentered(0xA0) * (0x1000 - t)) >> 12;
        work->unk30.vz += (ex->btlfx_randCentered(0xA0) * (0x1000 - t)) >> 12;
        env->unk2C->unk0[2].vx = work->unk30.vx;
        env->unk2C->unk0[2].vy = work->unk30.vy;
        env->unk2C->unk0[2].vz = work->unk30.vz;
        if (frame < 0x2E) {
            SVECTOR v0;

            {
                s32 a3 = 0x96 - (frame * 0x96) / 46;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x200), 1, a3, a3, a3);
            }
            BTLFX_COPY_SVEC(&v0, &work->unk8);
            v0.vy -= 0x800;
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), &v0, NULL, 0x1800, 0, 1, ex->math_cos(t >> 2));
            env->unk28 = 0;
        }
        if (frame >= 0x45) {
            work->unk0 = 10;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk184, SUMMON(env)->unk60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            work->unk30.vx = 0;
            work->unk30.vy = 0;
            work->unk30.vz = work->unk8.vz + 0x78E;
            env->unk2C->unk0[3].vx = work->unk30.vx;
            env->unk2C->unk0[3].vy = work->unk30.vy;
            env->unk2C->unk0[3].vz = work->unk30.vz;
            work->unk30.vx = work->unk8.vx;
            work->unk30.vy = work->unk8.vy;
            work->unk30.vz = work->unk8.vz;
            work->unk30.vy -= 0x1FF;
            work->unk30.vz -= 0x800;
            env->unk2C->unk0[1].vx = work->unk30.vx;
            env->unk2C->unk0[1].vy = work->unk30.vy;
            env->unk2C->unk0[1].vz = work->unk30.vz;
        }
        break;
    }
    case 10: {
        t = (frame << 12) / 24;
        if (frame == 0) {
            ex->btlfx_poolClear(entry->unk10);
            work->unk50 = 1;
        }
        if (frame < 0x11) {
            c_r277 = (frame << 5) - 0x1800;
        } else {
            c_r277 = (frame - 0x10) * 0x1DB - 0x1600;
        }
        work->unk10.vy = work->unk8.vy + c_r277 - 0x400;
        work->unk10.vx = work->unk8.vx;
        {
            s32 cz = c_r277 / 3 - 0x400;
            work->unk10.vz = work->unk8.vz - cz;
        }
        if (t < 0xC00) {
            work->unk28.vx = 0x400 - (t >> 3);
        } else {
            work->unk28.vx = 0x28C - (((t - 0xC00) * 0x1F4) >> 10);
        }
        work->unk28.vy = 0;
        work->unk1C.vx = work->unk1C.vy = work->unk1C.vz = (c_r277 + 0x1800) * 2;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                    (ex->math_sin(t >> 2) >> 5) - 0x80);
        work->unk30.vx = work->unk10.vx;
        work->unk30.vy = work->unk10.vy;
        work->unk30.vz = work->unk10.vz;
        if (frame < 0x10) {
            d = (frame << 10) >> 4;
            work->unk30.vy = -ex->math_sin(d);
            work->unk30.vx = ex->math_sin(d) >> 2;
        } else {
            work->unk30.vy = -0x1000;
            work->unk30.vx = 0x400;
        }
        env->unk2C->unk0[3].vx = work->unk30.vx;
        env->unk2C->unk0[3].vy = work->unk30.vy;
        env->unk2C->unk0[3].vz = work->unk30.vz;
        work->unk30.vx = work->unk8.vx;
        work->unk30.vy = work->unk8.vy;
        work->unk30.vz = work->unk8.vz;
        work->unk30.vy -= 0x1FF;
        work->unk30.vz -= 0x800;
        env->unk2C->unk0[1].vx = work->unk30.vx;
        nbolt = 2;
        env->unk2C->unk0[1].vy = work->unk30.vy;
        trail = 0x1000;
        env->unk2C->unk0[1].vz = work->unk30.vz;
        if (frame >= 0x18) {
            work->unk0 = 1;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180, SUMMON(env)->unk60);
            work->unk1C.vx = -0x3000;
            work->unk28.vz = 0;
            work->unk28.vy = 0;
            work->unk28.vx = 0;
            work->unk1C.vy = 0x3000;
            work->unk1C.vz = 0x3000;
            work->unk10.vx = D_801EA7E8.vx;
            work->unk10.vy = D_801EA7E8.vy;
            work->unk10.vz = D_801EA7E8.vz;
            work->unk10.vx += 0x800;
            work->unk10.vy -= 0x1000;
            work->unk10.vz = 0x7000;
            work->unk30.vx = D_801EA7E8.vx;
            work->unk30.vy = D_801EA7E8.vy;
            work->unk30.vz = D_801EA7E8.vz;
            trail = 0;
            work->unk30.vy -= 0x400;
            env->unk2C->unk0[2].vx = work->unk30.vx;
            env->unk2C->unk0[2].vy = work->unk30.vy;
            env->unk2C->unk0[2].vz = work->unk30.vz;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
            work->unk50 = 1;
            work->unk54 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 1);
            work->unk58 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
            work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 0);
        }
        break;
    }
    case 1: {
        fade = 0;
        work->unk30.vz -= 0x10;
        trail = 0x1000;
        env->unk2C->unk0[2].vx = work->unk30.vx;
        nbolt = 1;
        env->unk2C->unk0[2].vy = work->unk30.vy;
        env->unk2C->unk0[2].vz = work->unk30.vz;
        if (work->unk10.vz > -0xC000) {
            work->unk10.vz -= 0x1300;
        }
        ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0xA);
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, nbolt);
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, -0x20, -0x20, -0x20);
        if (frame >= 0xE) {
            SVECTOR rot;
            VECTOR sc;
            VECTOR pos;

            d = ((frame - 0xE) << 12) / 10;
            f3 = ex->math_sin(d >> 1);
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vz = 0x2800;
            sc.vx = 0x2800;
            if (d < 0x400) {
                sc.vy = d * 0xC;
            } else {
                sc.vy = 0x3000 - ((d - 0x400) * 2) / 3;
            }
            pos.vx = 0;
            pos.vy = 0;
            pos.vz = ex->math_cos(d >> 2) / 3;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk54);
            {
                s32 c5 = (f3 >> 6) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk54, c5, c5, c5);
            }
        }
        if (frame >= 0xE) {
            x = frame - 0xE;
            if (x < 0x10) {
                union { CVECTOR c_r365; s32 w[2]; } cu;
                SVECTOR rot3;
                VECTOR sc3;

                cu.c_r365.r = cu.c_r365.g = cu.c_r365.b = 0x46;
                rot3.vx = 0;
                rot3.vy = 0;
                rot3.vz = 0;
                sc3.vz = 0x1000;
                c_r365 = (x << 12) / 14;
                sc3.vx = c_r365 + 0x1000;
                sc3.vy = c_r365 / 2 + 0x1000;
                ex->func_800E6764(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot3, &sc3, &cu.c_r365, 1, 0);
            }
        }
        if (frame >= 0x18) {
            work->unk0 = 2;
            entry->unk0 = -1;
            work->unk10.vx = D_801EA7E8.vx;
            work->unk10.vy = D_801EA7E8.vy;
            work->unk10.vz = D_801EA7E8.vz + 0x5000;
            work->unk50 = 1;
            work->unk10.vy -= 0x2000;
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk188, SUMMON(env)->unk60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            work->unk1C.vx = 0x1000;
            work->unk1C.vy = 0x1000;
            work->unk1C.vz = 0x1000;
            env->unk2C->unk0[2].vx = work->unk10.vx;
            env->unk2C->unk0[2].vy = work->unk10.vy;
            trail = 0;
            env->unk2C->unk0[2].vz = work->unk10.vz;
        }
        break;
    }
    case 2: {
        nbolt = 2;
        t = (frame << 12) / 26;
        if (frame == 0) {
            work->unk88.vx = 0x400;
            work->unk9C.vx = 0x1000;
            work->unk9C.vy = 0x1000;
            work->unk88.vz = 0;
            work->unk88.vy = 0;
            work->unk9C.vz = 0x3000;
            work->unk90.vx = work->unk10.vx;
            work->unk90.vy = work->unk10.vy;
            work->unk90.vz = work->unk10.vz;
            work->unk90.vy -= 0x3000;
            work->unk90.vz -= 0x1000;
        }
        work->unk88.vz += 8;
        work->unk90.vy += 0x100;
        work->unk10.vy -= 0x180;
        {
            s32 cz = ex->math_cos(t >> 2) >> 5;
            s32 vz = work->unk10.vz - 0x80;
            work->unk10.vz = vz - cz;
        }
        trail = 0x1000;
        env->unk2C->unk0[2].vx = work->unk10.vx;
        fade = 0x1000;
        env->unk2C->unk0[2].vy = work->unk10.vy;
        env->unk2C->unk0[2].vz = work->unk10.vz;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 0);
        f2 = ((((t * 3) >> 2) + 0x400) >> 5) - 0x80;
        ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, f2, f2, f2);
        if (frame >= 0x1A) {
            work->unk0 = 4;
            entry->unk0 = -1;
            work->unk48 = 0;
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk18C, SUMMON(env)->unk60);
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0xA);
            work->unk30.vy = -0x4966;
            work->unk30.vx = 0;
            work->unk30.vz = 0x3DAF;
            env->unk2C->unk0[3].vx = work->unk30.vx;
            env->unk2C->unk0[3].vy = work->unk30.vy;
            env->unk2C->unk0[3].vz = work->unk30.vz;
        }
        break;
    }
    case 4:
        switch (frame) {
        default: {
        SVECTOR v0;
        SVECTOR v1;
        MATRIX m;
        s32 ang;
        s32 amp4;

        t = (frame << 12) / 30;
        nbolt = 3;
        fade = 0x1000;
        work->unk90.vx = work->unk10.vx;
        work->unk90.vy = work->unk10.vy;
        work->unk90.vz = work->unk10.vz;
        work->unk90.vx += 0x2000;
        d = t >> 2;
        {
            s32 cz = (ex->math_cos(d) * 0x3E) >> 12;
            s16 vz = work->unk88.vz - 8;
            work->unk88.vz = vz - cz;
        }
        work->unk10.vy -= (ex->math_cos(d) << 8) >> 12;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 0);
        ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&work->unk30);
        work->unk30.vy -= 0x80;
        env->unk2C->unk0[3].vx = work->unk30.vx;
        D_801EB370 = 0xA0;
        D_801EB374 = 0x4D;
        env->unk2C->unk0[3].vy = work->unk30.vy;
        env->unk2C->unk0[3].vz = work->unk30.vz;
        D_801EB378 = 0x180;
        ang = work->unk4 * 0x180;
        amp4 = 0x37;
        for (i_b526 = 0; i_b526 < 2; i_b526++) {
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EA7F0[i_b526], (u16 *)&v0);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, D_801EA7F0[i_b526], (BtlBone *)&m);
            ex->func_800CFE88(&m, 0x398, (s32)&v1);
            func_801E7B40(entry, (Fx0e30Bolt *)&D_801EB0E8[i_b526], &v0, &v1, amp4, amp4, ang, 0x200, 0,
                          ex->btlfx_randCentered(0x400) + 0xC00);
        }
        if (frame >= 0x1E) {
            work->unk0 = 5;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk194, SUMMON(env)->unk60);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&work->unk40);
            work->unk4C = NULL;
            work->unk40.vy -= 0x80;
        }
        break;
        }
        }
        break;
    case 5: {
        SVECTOR v0;
        SVECTOR ba;
        SVECTOR bb;
        MATRIX bm;
        SVECTOR ca;
        SVECTOR cb;
        SVECTOR rot2;
        SVECTOR from;
        VECTOR gap;
        VECTOR sc2;
        VECTOR pos2;
        SVECTOR to;
        SVECTOR mid;
        CVECTOR col;
        SVECTOR rot3;
        VECTOR sc3;

        t = (frame << 12) / 65;
        if (frame == 0) {
            work->unk88.vx = 0x400;
            work->unk9C.vx = 0x1000;
            work->unk9C.vy = 0x1000;
            work->unk88.vy = 0;
            work->unk88.vz = 0;
            work->unk9C.vz = 0x2000;
            work->unk90.vy = work->unk10.vy;
            work->unk90.vx = work->unk10.vx;
            work->unk90.vz = work->unk10.vz;
            work->unk90.vy -= 0x1000;
        }
        if (frame < 2) {
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
        }
        if (frame < 0x14) {
            fade = 0x1000;
        } else {
            fade = 0x1000 - ((frame - 0x14) << 11) / 45;
        }
        work->unk88.vz += 2;
        if (frame == 7) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x83), (s32)&work->unk40);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        if (frame == 0xC) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk40);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&ba);
        ex->func_800CFE88((MATRIX *)&ba, 0x18, (s32)&work->unk38);
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&rot2);
        ex->func_800CFF40((MATRIX *)&rot2, (s16 *)&v0);
        if (frame == 0xC) {
            work->unk4C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk38);
            if (work->unk4C != NULL) {
                work->unk4C->unk22 = 0x80;
            }
        }
        if (work->unk4C != NULL) {
            work->unk4C->unk5C.vx = v0.vx;
            work->unk4C->unk5C.vy = v0.vy;
            work->unk4C->unk5C.vz = v0.vz;
        }
        ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0, (u16 *)&work->unk30);
        work->unk30.vy -= 0x200;
        if (frame >= 5) {
            work->unk30.vy += 0x80 + ((ex->math_sin((((frame - 4) << 12) / 61) >> 2) * 0x180) >> 12);
        } else {
            work->unk30.vy += frame << 5;
        }
        BTLFX_COPY_SVEC(&v0, &work->unk30);
        if ((u32)(frame - 0x1C) < 7) {
            v0.vx += (ex->math_cos(((frame - 0x1C) << 12) >> 5) >> 7) * ((frame & 1) * 2 - 1);
        }
        env->unk2C->unk0[2].vx = v0.vx;
        env->unk2C->unk0[2].vy = v0.vy;
        env->unk2C->unk0[2].vz = v0.vz;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 0);
        if (frame < 0xC) {
            s32 ang;
            s32 amp5;

            D_801EB370 = 0xA0;
            D_801EB374 = 0x4D;
            D_801EB378 = 0x180;
            ang = work->unk4 * 0x180;
            amp5 = 0x37;
            for (i_b635 = 0; i_b635 < 2; i_b635++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EA7F0[i_b635], (u16 *)&ba);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, D_801EA7F0[i_b635], (BtlBone *)&bm);
                ex->func_800CFE88(&bm, 0x398, (s32)&bb);
                func_801E7B40(entry, (Fx0e30Bolt *)&D_801EB0E8[i_b635], &ba, &bb, amp5, amp5, ang, 0x200, 0,
                              ex->btlfx_randCentered(0x400) + 0xC00);
            }
        }
        if (frame >= 0xC) {
            s32 ang;
            s32 cnt;
            s32 amp;

            cnt = 4;
            amp = ((t * 0x180) >> 12) + 0x20;
            ang = work->unk4 << 8;
            if (frame >= 0x19) {
                cnt = 6;
            }
            D_801EB370 = 0x180;
            D_801EB374 = 0x80;
            D_801EB378 = 0xA0;
            for (i_b648 = 0; i_b648 < cnt; i_b648++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&ca);
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&cb);
                func_801E7B40(entry, (Fx0e30Bolt *)&D_801EB0E8[i_b648], &ca, &cb, amp, amp, ang, 0x100, 0,
                              ex->btlfx_randCentered(0x400) + 0xC00);
            }
        }
        if (frame == 0x16) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x5C, 0x5C, 0x5C);
        }
        if (frame >= 0x14) {
            a_b664 = (((frame - 0x14) << 12) / 45) >> 2;
            y_b657 = ex->math_cos(a_b664) / 3 + 0x200;
            rot2.vx = 0x800;
            rot2.vy = 0;
            rot2.vz = 0;
            pos2.vx = work->unk10.vx;
            pos2.vy = work->unk10.vy;
            pos2.vz = work->unk10.vz;
            pos2.vy += 0x5A5;
            rot2.vy = (frame << 5) + 0x800;
            sc2.vy = ex->math_cos(a_b664) >> 2;
            b = ex->math_sin(a_b664) * 4 + 0x800;
            d = (y_b657 >> 5) - 0x80;
            for (i_b671 = 0; i_b671 < 9; i_b671++) {
                sc2.vx = sc2.vz = ((ex->math_sin((i_b671 << 11) >> 3) + 0x800) * b) >> 12;
                pos2.vy -= 0x11F;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&sc2, work->unk58);
                ex->Hi_ModifyEffModelRGB(work->unk58, d, d, d);
                rot2.vy += 0x366;
            }
        }
        if (frame >= 0x18) {
            s32 amp;

            a = (((frame - 0x18) << 12) / 41) >> 2;
            y_b680 = ex->math_sin(a) * 2;
            from.vx = work->unk10.vx;
            from.vy = work->unk10.vy;
            from.vz = work->unk10.vz;
            BTLFX_COPY_SVEC(&to, &from);
            BTLFX_COPY_SVEC(&mid, &from);
            y_b680 = y_b680 / 3 + 0x400;
            from.vy -= y_b680;
            to.vy += y_b680;
            amp = (ex->math_sin(a) >> 1) + 0x180;
            for (i = 0, j = -1, k = work->unk4 * 0x9B - 0x400; i < 2; i++, j += 2, k += 0x400) {
                d_b695 = ex->math_cos(a) + ex->btlfx_randCentered(0x400);
                func_801E7D40(entry, (s32)&mid, &D_801EB388[i], &from, &to, amp, k * j,
                              ((ex->math_cos(a) >> 5) + 0x100) >> 1, 0, d_b695);
            }
        }
        if (frame >= 0x1C) {
            s32 xq = frame - 0x1C;
            xq = (xq << 12) / 37;
            y = (ex->math_cos(xq >> 2) * 0x57) >> 12;
            col.r = col.g = col.b = y;
            sc3.vx = sc3.vy = 0x1200 - (xq >> 3);
            sc3.vz = 0x1000;
            rot3.vx = 0;
            rot3.vy = 0;
            rot3.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot3, &sc3, &col, 1, 0);
        }
        if (frame >= 0x41) {
            done = 1;
        }
        break;
    }
    }
    }
    {
    SVECTOR v0;
    SVECTOR v1;
    MATRIX m;
    MATRIX m2;
    MATRIX m3;
    SVECTOR ba;
    SVECTOR bb;

    if (trail != 0) {
        env->unk28 = -0x300;
        trail >>= 1;
        if (work->unk50 != 0) {
            work->unk50 = 0;
            for (j = 0; j < 4; j++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EA800[j].unk0, (u16 *)&v0);
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EA800[j].unk2, (u16 *)&v1);
                func_801E7730(&v0, &v1, 0x3F6, 0, 0, 0, 0, 0, 0, 0, D_801EA878[j], 1, 0);
            }
            for (j = 0; j < 4; j++) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EA7F0[j], (u16 *)&v0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 1), (s32)&v0, 0, -1, 0x1000, trail, 1,
                                    D_801EAC38[j], 0);
            }
        }
        for (j = 0; j < 4; j++) {
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, D_801EA800[j].unk0, (BtlBone *)&m);
            ex->func_800CFE88(&m, (D_801EA800[j + 4].unk0 * work->unk1C.vy) >> 12, (s32)&v0);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, D_801EA800[j].unk2, (BtlBone *)&m2);
            ex->func_800CFE88(&m2, (D_801EA800[j + 4].unk2 * work->unk1C.vy) >> 12, (s32)&v1);
            func_801E7730(&v0, &v1, 0xE, 0x9D, 0x3D40, 0, 0x80, 0x40, 0x80, trail >> 5, D_801EA878[j], 1,
                          10000 - work->unk4);
        }
        for (j = 0; j < 4; j++) {
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, D_801EA7F0[j], (BtlBone *)&m3);
            ex->func_800CFE88(&m3, (D_801EA7F0[j + 4] * work->unk1C.vy) >> 12, (s32)&v0);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 1), (s32)&v0, 0, 0, 0x2000, trail, 1,
                                D_801EAC38[j], 0);
        }
        env->unk28 = 0;
    }
    if (fade != 0) {
        ex->Hi_DrawEffModel(&work->unk88, (VECTOR *)&work->unk90, (s32)&work->unk9C, work->unk5C);
        c = (fade >> 6) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk5C, c, c, c);
    }
    if (nbolt != 0) {
        D_801EB370 = 0x180;
        D_801EB374 = 0x80;
        D_801EB378 = 0xA0;
        d = work->unk4 << 8;
        for (i = 0; i < nbolt; i++) {
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&ba);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&bb);
            func_801E7B40(entry, (Fx0e30Bolt *)&D_801EB0E8[i], &ba, &bb, 0x20, 0x40, d, 0x100, 0,
                          ex->btlfx_randCentered(0x400) + 0x800);
        }
    }
    }
        if (done != 0) {
            return 1;
        }
        work->unk4++;
    }
    return 0;
}

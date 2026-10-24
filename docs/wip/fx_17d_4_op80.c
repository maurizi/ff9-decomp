/* Draft of fx_17d_4 fx_17d_4_op80. Status: 620 aligned mismatches (128 ignoring registers and spill offsets);
 * equivalent by construction. Pass 3 fixes: the ring loops' frame * k stays a multiply when the product goes through
 * a block-local y with a dead `y = 0;` at the end of the body (y is then set twice in the loop, so loop.c does not
 * treat it as a giv; flow deletes the dead store), with sc.vx/sc.vz computed before rot.vy; case 0 calls
 * Hi_SetSummonMotFrame with its two real arguments (the original's a2 is just the divide's partial result, not an
 * argument); case 1's unk28/unk20 copies in the order 28.vx, 28.vy, 28.vz, 20.vy, 20.vx, 20.vz; case 3 computes
 * x = frame << 6 first (the original keeps it in s2); variable roles searched per case (build/w/roles.py).
 * Open: case 3's 0x1000 constant (s6 in the original) and many register choices; statement orders in cases 4-6.
 * Earlier: c and o split per block; case 4 q4 = (frame << 12) / 20; case 5 t >> 1 / t >> 2; case 6
 * t = (frame << 12) / 51 and w13 = (... << 12) / 13; case 2's n * (i + 2) as a for loop. */
#include "fxmod/fx_17d_4.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d4Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR unk10; /* particle position */
    u8 pad18[8];
    SVECTOR unk20; /* camera target */
    Fx17d4Vec3 unk28; /* model position */
    SVECTOR unk34; /* model rotation */
    Fx17d4Vec3 unk3C; /* model scale */
    Fx17d4Vec3 unk48; /* summon position */
    Fx17d4Vec3 unk54; /* summon scale */
    SVECTOR unk60; /* summon rotation */
    s32 unk68; /* models */
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 pad7C[8];
} Fx17d4Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x19C];
        BtlUnit9CC *unk19C;
        BtlUnit9CC *unk1A0;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx17d4Summon;

#define SUMMON(env) ((Fx17d4Summon *)(env)->unk4)

extern SVECTOR D_801E971C;
extern s32 D_801E989C[];
extern s8 D_801EB25C;
extern BtlModel D_801EB65C[];
void func_801E77DC(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 sc, s32 alpha, s32 ot, s32 flag);
void func_801E7B8C(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);
void func_801E7EB4(Unk80100C78Entry *entry, SVECTOR *from, s32 ot);

s32 fx_17d_4_op80(s32 mode, Fx17d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    Fx17d4Vec3 pos;
    SVECTOR rot;
    SVECTOR s;
    SVECTOR b;
    SVECTOR a;
    SVECTOR rot2;
    SVECTOR b2;
    SVECTOR a2;
    Pool6CEntry *p;
    s32 q4;
    s32 w13;
    s32 c_1;
    s32 c_3;
    s32 c_5;
    s32 c_6;
    s32 o_3;
    s32 frame;
    s32 t;
    s32 x;
    s32 r;
    s32 i;
    s32 k;
    s32 n;
    s32 m;
    s32 f;
    s32 q;
    s32 g;
    s32 flag;
    s32 f3;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E9890 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EB65C, 5);
        work->unk68 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        work->unk6C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 1);
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x84), 1);
        work->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 0);
        work->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk19C, SUMMON(env)->unk60);
        work->unk48.vx = 0;
        work->unk48.vy = -0x400;
        work->unk48.vz = 0;
        work->unk3C.vx = work->unk3C.vz = 0x400;
        work->unk28.vy = work->unk48.vy;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk3C.vy = 0x1000;
        work->unk28.vy += 0x800;
        work->unk28.vx = work->unk48.vx;
        work->unk28.vz = work->unk48.vz;
        work->unk20.vx = work->unk48.vx;
        work->unk20.vy = work->unk48.vy;
        work->unk20.vz = work->unk48.vz;
        env->unk2C->unk0[3].vx = work->unk20.vx;
        env->unk2C->unk0[3].vy = work->unk20.vy;
        env->unk2C->unk0[3].vz = work->unk20.vz;
        D_801EB25C = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            x = (frame << 12) / 20;
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            work->unk48.vy += ex->math_cos(x >> 2) >> 4;
            work->unk54.vx = work->unk54.vy = work->unk54.vz = ex->math_cos(x >> 2) + 0x800;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelAbr(work->unk68, 1);
            ex->Hi_ModifyEffModelRGB(work->unk68, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelAbr(work->unk68, 2);
            ex->Hi_ModifyEffModelRGB(work->unk68, 0, 0, 0);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy += 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0x800;
            rot.vy = frame * 24;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            ex->func_800E6624(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame >= 0x14) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3C58();
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1A0, SUMMON(env)->unk60);
                work->unk34.vx = 0;
                work->unk34.vy = 0;
                work->unk34.vz = 0;
                work->unk3C.vx = work->unk3C.vz = 0x800;
                work->unk3C.vy = 0x1000;
                work->unk28.vx = work->unk48.vx;
                work->unk28.vy = work->unk48.vy;
                work->unk28.vz = work->unk48.vz;
                work->unk20.vx = work->unk48.vx;
                work->unk20.vy = work->unk48.vy;
                work->unk20.vz = work->unk48.vz;
                work->unk28.vy -= 0x400;
                work->unk20.vy -= 0x8FC;
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            break;
        case 1:
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            work->unk48.vy -= 0xC;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->func_800E6624(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame >= 0xA) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk10.vy = work->unk48.vy;
                work->unk10.vx = work->unk48.vx;
                work->unk10.vz = work->unk48.vz;
                work->unk10.vy -= 0x400;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E989C;
                    p->unk12 = 0x30;
                    p->unkC = 0x8C;
                    p->unk22 = 0x20;
                }
            }
            break;
        case 2:
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            t = (frame << 12) / 20;
            m = t >> 2;
            work->unk48.vy -= (ex->math_cos(m) * 10) >> 12;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            n = t / 2 + 0x800;
            sc.vy = ex->math_sin(m) / 2 + 0x800;
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            for (r = 0, w13 = 0x20; r < 4; r++, w13 += 0x10) {
                s32 y;

                rot.vx = 0;
                rot.vz = 0;
                sc.vx = sc.vz = (n * (r + 2)) / 6;
                y = frame * w13;
                rot.vy = y;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk78);
                ex->Hi_ModifyEffModelAbr(work->unk78, 1);
                ex->Hi_ModifyEffModelRGB(work->unk78, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                         (ex->math_sin(t >> 2) >> 5) - 0x80);
                y = 0;
            }
            ex->func_800E6624(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame > 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, (frame - 0x10) * 0x3C, (frame - 0x10) * 0x3C,
                                         (frame - 0x10) * 0x3C);
            }
            if (frame >= 0x14) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1A0, SUMMON(env)->unk60);
                ex->func_800E3C58();
                work->unk3C.vx = work->unk3C.vz = 0x1800;
                work->unk34.vx = 0;
                work->unk34.vy = 0;
                work->unk34.vz = 0;
                work->unk3C.vy = 0xAAA;
                work->unk28.vy = work->unk48.vy;
                work->unk28.vx = work->unk48.vx;
                work->unk28.vz = work->unk48.vz;
                work->unk28.vy += 0x800;
            }
            break;
        case 3:
            x = frame << 6;
            if (frame < 0x20) {
                work->unk60.vy = (ex->math_sin((frame << 10) >> 5) - 0x1000) >> 3;
            }
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk70);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            pos.vy -= 0x1000;
            sc.vy = 0x2000;
            rot.vx = 0x800;
            rot.vy = frame * 4;
            rot.vz = 0;
            sc.vx = sc.vz = x / 3 + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk6C);
            x = x >> 2;
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(x) >> 5) - 0x80, (ex->math_cos(x) >> 5) - 0x80,
                                     (ex->math_cos(x) >> 5) - 0x80);
            if (frame < 0x28) {
                q = (frame << 12) / 40;
                sc.vy = q / 3 + 0x1000;
                pos.vx = work->unk48.vx;
                pos.vy = work->unk48.vy;
                o_3 = ((0x1000 - q) >> 5) - 0x80;
                pos.vz = work->unk48.vz;
                for (i = 0, t = 0x20; i < 3; i++, t += 0x10) {
                    s32 y;

                    rot.vx = 0;
                    rot.vz = 0;
                    sc.vx = sc.vz = ((q + 0x1000) * (i + 2)) / 5;
                    sc.vy = sc.vy * 2 / 3;
                    y = frame * t;
                    rot.vy = y;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk78);
                    ex->Hi_ModifyEffModelAbr(work->unk78, 1);
                    ex->Hi_ModifyEffModelRGB(work->unk78, o_3, o_3, o_3);
                    y = 0;
                }
            }
            ex->func_800E6624(0x2710 - frame * 3, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame < 0xC) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xF0 - frame * 0x14, 0xF0 - frame * 0x14,
                                         0xF0 - frame * 0x14);
            }
            work->unk20.vy += 8;
            env->unk2C->unk0[3].vx = work->unk20.vx;
            env->unk2C->unk0[3].vy = work->unk20.vy;
            env->unk2C->unk0[3].vz = work->unk20.vz;
            if (frame >= 0x40) {
                work->unk0 = 4;
                ex->func_800E3C58();
                entry->unk0 = -1;
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk20);
                work->unk20.vx += 0x40;
                work->unk20.vy -= 0x5A;
                work->unk20.vz -= 0x40;
                work->unk10.vx = work->unk20.vx;
                work->unk10.vy = work->unk20.vy;
                work->unk10.vz = work->unk20.vz;
                env->unk2C->unk0[2].vx = work->unk20.vx;
                env->unk2C->unk0[2].vy = work->unk20.vy;
                env->unk2C->unk0[2].vz = work->unk20.vz;
                work->unk28.vy += 0x1000;
            }
            break;
        case 4:
            q4 = (frame << 12) / 20;
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk70);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk20);
            work->unk20.vx += 0x40;
            work->unk20.vy -= 0x5A;
            work->unk20.vz -= 0x40;
            work->unk10.vx = work->unk20.vx;
            work->unk10.vy = work->unk20.vy;
            work->unk10.vz = work->unk20.vz;
            if (frame >= 0xA) {
                f = (0x1000 - ex->math_cos(((frame - 0xA) << 10) / 10)) * 0x258;
                work->unk20.vy += f >> 13;
                work->unk20.vz -= f >> 12;
            }
            env->unk2C->unk0[2].vx = work->unk20.vx;
            env->unk2C->unk0[2].vy = work->unk20.vy;
            env->unk2C->unk0[2].vz = work->unk20.vz;
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x8C;
                }
            }
            env->unk28 = 0x10;
            i = 0x1000;
            if (frame < 8) {
                i = frame << 9;
            }
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unk10, NULL, 0x1000, 0, 1, i);
            c_5 = 0;
            if (frame >= 6) {
                c_5 = ((frame - 6) << 12) / 14;
            }
            BTLFX_COPY_SVEC(&s, &work->unk10);
            s.vy += ex->math_sin(q4 >> 2) >> 3;
            s.vz -= 0x400;
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &s, &work->unk10, frame, c_5, 1, 0x18);
            env->unk28 = 0;
            if (frame >= 0x14) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk8.vx = D_801E971C.vx;
                work->unk8.vy = D_801E971C.vy;
                work->unk8.vz = D_801E971C.vz;
                work->unk10.vy = D_801E971C.vy;
                work->unk10.vx = work->unk8.vx;
                work->unk10.vz = work->unk8.vz;
                env->unk2C->unk0[3].vx = work->unk8.vx;
                env->unk2C->unk0[3].vy = work->unk8.vy;
                env->unk2C->unk0[3].vz = work->unk8.vz;
            }
            break;
        case 5:
            env->unk28 = 0x10;
            t = (frame << 12) / 24;
            c_1 = ex->math_cos(t >> 1) >> 1;
            c_3 = ex->math_cos(t >> 2) >> 1;
            a.vx = b.vx = work->unk8.vx + ((ex->math_cos(c_1) * c_3) >> 12);
            a.vy = work->unk8.vy;
            a.vz = b.vz = work->unk8.vz + ((ex->math_sin(c_1) * c_3) >> 12);
            b.vy = work->unk8.vy - 0x1000;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &a, NULL, 0xAAA, 0, 1, t);
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &a, &b, frame, t, 1, 0x20);
            env->unk28 = 0;
            if (frame >= 0x18) {
                work->unk0 = 6;
                entry->unk0 = -1;
            }
            break;
        case 6:
            a2.vx = b2.vx = work->unk8.vx;
            a2.vy = work->unk8.vy;
            a2.vz = b2.vz = work->unk8.vz;
            b2.vy = work->unk8.vy - 0x1000;
            env->unk28 = 0x10;
            x = (frame << 12) / 51;
            c_6 = 0x1000 - x;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &a2, NULL, 0xAAA, 0, 1, c_6);
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &a2, &b2, frame, c_6, 1,
                          (x >> 7) + 0x20);
            env->unk28 = 0;
            rot2.vx = 0x400;
            rot2.vz = 0;
            rot2.vy = 0;
            rot2.pad = 1;
            if (frame < 0x10) {
                q4 = (frame << 12) >> 6;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &a2, &rot2, ex->math_sin(q4) * 5, 0, 1,
                                  ex->math_cos(q4));
            }
            if (frame >= 0x1E) {
                k = (frame - 0x1E) << 9;
                if (k > 0x1000) {
                    k = 0x1000;
                }
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &a2, &rot2, 0x5000, 0, 1, k);
            }
            flag = 0;
            if (frame < 0x26) {
                o_3 = 0x20;
                if (frame < 0x1E) {
                    w13 = 0x1000;
                    g = 0;
                    q = 0;
                    k = 0;
                    x = frame;
                    flag = 1;
                    f3 = 0;
                } else {
                    q4 = (frame - 0x1E) << 9;
                    o_3 = 0x27;
                    w13 = 0x1000;
                    g = 0;
                    q = 0x1000;
                    x = 0x1D;
                    f3 = (frame - 0x1E) * 3;
                    if (q4 >= 0x800) {
                        k = (q4 - 0x800) * 2;
                    } else {
                        k = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, ((0x1000 - q4) * 0x64) >> 12, 0, 0);
                }
            } else {
                g = 1;
                x = 0x1D;
                r = ((frame - 0x26) << 12) / 13;
                o_3 = (r >> 3) + 0x40;
                f3 = 0x15;
                w13 = 0x1000 - r * 2 / 3;
                k = w13;
                q = k;
            }
            func_801E77DC(entry, &D_801E971C, x, o_3, w13, g, flag);
            if (q != 0) {
                func_801E7B8C(entry, &D_801E971C, f3, q);
                func_801E7EB4(entry, &D_801E971C, k);
            }
            if (frame >= 0x33) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

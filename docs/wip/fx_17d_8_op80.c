/* Draft of fx_17d_8 fx_17d_8_op80. Status: 499 aligned mismatches but only 39 ignoring registers; equivalent by
 * construction. Nearly all register differences are the reload register for the spilled env/entry/frame (t1 here,
 * t3 in the original): the original also uses t1/t2 for case 4's matrix-restore temporaries (its scheduler hoists the
 * restore loads above the save stores), so reload ranks t3 first. Open: case 4's save/restore scheduling, and the
 * frame == 1 copy into unk10/unk18 (the original copies each loaded field into a second register). The matrix
 * copies are word copies through temporaries (a struct assignment uses movstrsi scratch registers instead). */
#include "fxmod/fx_17d_8.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d8Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR unk10; /* particle position */
    SVECTOR unk18; /* particle target */
    SVECTOR unk20; /* camera position */
    Fx17d8Vec3 unk28; /* model position */
    SVECTOR unk34;
    Fx17d8Vec3 unk3C; /* model scale */
    Fx17d8Vec3 unk48; /* spawn position */
    SVECTOR unk54;
    SVECTOR unk5C; /* model rotation */
    s32 unk64; /* models */
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    MATRIX unk7C; /* saved matrices */
    MATRIX unk9C;
} Fx17d8Work80; /* 0xBC */

extern SVECTOR D_801E89F4;
extern Unk800F78A4 *D_801E8A44;
extern BtlModel D_801E8A54[];
s32 func_801E7718(s32 mode, Fx17d8Obj *w, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx17d8Inst *inst);

#define COPY_MAT_WORDS(dst, src)                         \
    do {                                                 \
        ((s32 *)(dst))[0] = ((s32 *)(src))[0];          \
        ((s32 *)(dst))[1] = ((s32 *)(src))[1];          \
        ((s32 *)(dst))[2] = ((s32 *)(src))[2];          \
        ((s32 *)(dst))[3] = ((s32 *)(src))[3];          \
        ((s32 *)(dst))[4] = ((s32 *)(src))[4];          \
        ((s32 *)(dst))[5] = ((s32 *)(src))[5];          \
        ((s32 *)(dst))[6] = ((s32 *)(src))[6];          \
        ((s32 *)(dst))[7] = ((s32 *)(src))[7];          \
    } while (0)

s32 fx_17d_8_op80(s32 mode, Fx17d8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    CVECTOR cv;
    SVECTOR sv;
    VECTOR vv;
    CVECTOR cv2;
    SVECTOR sv2;
    VECTOR vv2;
    VECTOR vv3;
    Fx17d8Obj *o;
    Pool6CEntry *p;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 t;
    s32 h;
    s32 c;
    s32 v;

    if (mode == 0) {
        init->unk0 = 0xBC;
        init->unk8 = 0x14;
        init->unk14 = (s32)func_801E7718;
        init->unk4 = 0x10;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8A44 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801E8A54, 7);
        work->unk64 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 0);
        work->unk68 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk6C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 3), 1);
        work->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 1);
        work->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 1);
        BTLFX_COPY_SVEC(&work->unk8, &D_801E89F4);
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk3C.vx = work->unk3C.vz = work->unk3C.vy = 0x1000;
        work->unk28.vx = work->unk8.vx;
        work->unk28.vy = work->unk8.vy;
        work->unk28.vz = work->unk8.vz;
        work->unk20.vx = D_801E89F4.vx;
        work->unk20.vy = D_801E89F4.vy;
        work->unk20.vz = D_801E89F4.vz;
        env->unk2C->unk0[2].vx = work->unk20.vx;
        env->unk2C->unk0[2].vy = work->unk20.vy;
        env->unk2C->unk0[2].vz = work->unk20.vz;
        work->unk5C.vx = 0;
        work->unk5C.vy = 0x400;
        work->unk5C.vz = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            n1 = (((0x1000 - ex->math_sin(t >> 3)) << 6) >> 12) - 0x2C;
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), n1, 0x78, 0, 0xFF, t);
            work->unk20.vx -= 0x2E;
            env->unk2C->unk0[2].vx = work->unk20.vx;
            env->unk2C->unk0[2].vy = work->unk20.vy;
            env->unk2C->unk0[2].vz = work->unk20.vz;
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800CFD78((s16 *)&work->unk8, (s16 *)&work->unk5C, -0x968, (s16 *)&work->unk54);
                work->unk48.vx = work->unk54.vx;
                work->unk48.vy = work->unk54.vy;
                work->unk48.vz = work->unk54.vz;
            }
            break;
        case 1:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, ((t - 0x800) >> 5) - 0x80, ((t - 0x800) >> 5) - 0x80,
                                     ((t - 0x800) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            work->unk20.vx -= (ex->math_cos(t >> 2) * 0x2E) >> 12;
            env->unk2C->unk0[2].vx = work->unk20.vx;
            env->unk2C->unk0[2].vy = work->unk20.vy;
            env->unk2C->unk0[2].vz = work->unk20.vz;
            D_801E8FCC = 0x400;
            if (frame % 3 == 0) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = 0x400;
                    o->unkC.vy = 0x400;
                    o->unkC.vz = 0;
                    o->unk4 = work->unk48.vx;
                    o->unk6 = work->unk48.vy;
                    o->unk8 = work->unk48.vz;
                    o->unk2 = 0x1000;
                }
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unk54, NULL, t >> 1, 0, 1, t);
            env->unk28 = 0;
            v = (((0x1000 - ex->math_sin((t >> 3) + 0x200)) << 6) >> 12) - 0x2C;
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), v, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x20) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            h = t / 2;
            ex->Hi_ModifyEffModelRGB(work->unk68, ((0x800 - h) >> 5) - 0x80, ((0x800 - h) >> 5) - 0x80,
                                     ((0x800 - h) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            h = ((0x1000 - h) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, h, h, h);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            sc.vx = sc.vy = (0x1000 - ex->math_sin(t >> 2)) / 3 + 0xF5;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk70);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk74);
            D_801E8FCC = (t >> 1) + 0x400;
            if (frame % 3 == 0) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = 0x400;
                    o->unkC.vy = 0x400;
                    o->unkC.vz = 0;
                    o->unk4 = work->unk48.vx;
                    o->unk6 = work->unk48.vy;
                    o->unk8 = work->unk48.vz;
                    o->unk2 = 0x1000;
                }
            }
            if (frame == 1) {
                work->unk18.vx = work->unk10.vx = work->unk54.vx;
                work->unk18.vy = work->unk10.vy = work->unk54.vy;
                work->unk18.vz = work->unk10.vz = work->unk54.vz;
                work->unk18.vx -= 0xC00;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk2C = (s32)&work->unk18;
                    p->unk22 = 0x80;
                }
            }
            cv.r = cv.g = cv.b = 0x84;
            vv.vy = vv.vx = 0x1020;
            sv.vx = 0;
            sv.vy = 0;
            sv.vz = 0;
            vv.vz = 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv, &vv, &cv, 0, NULL);
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unk54, NULL, (t >> 1) + 0x800, 0, 1,
                              ex->math_cos(t >> 2));
            env->unk28 = 0;
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            h = t / 2;
            ex->Hi_ModifyEffModelRGB(work->unk68, (-h >> 5) - 0x80, (-h >> 5) - 0x80, (-h >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            h = ((0x800 - h) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, h, h, h);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            n2 = t >> 2;
            sc.vx = sc.vy = ((0x1000 - ex->math_sin(n2)) * 0xF5) >> 12;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk70);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk74);
            D_801E8FCC = n2 + 0xC00;
            if (!(frame & 3) && frame < 0x18) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = 0x400;
                    o->unkC.vy = 0x400;
                    o->unkC.vz = 0;
                    o->unk4 = work->unk48.vx;
                    o->unk6 = work->unk48.vy;
                    o->unk8 = work->unk48.vz;
                    o->unk2 = 0x1000 - t;
                }
            }
            if (frame == 0x16) {
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->unk14)[0];
                    q1 = ((s32 *)env->unk14)[1];
                    q2 = ((s32 *)env->unk14)[2];
                    q3 = ((s32 *)env->unk14)[3];
                    ((s32 *)&work->unk7C)[0] = q0;
                    ((s32 *)&work->unk7C)[1] = q1;
                    ((s32 *)&work->unk7C)[2] = q2;
                    ((s32 *)&work->unk7C)[3] = q3;
                }
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->unk14)[4];
                    q1 = ((s32 *)env->unk14)[5];
                    q2 = ((s32 *)env->unk14)[6];
                    q3 = ((s32 *)env->unk14)[7];
                    ((s32 *)&work->unk7C)[4] = q0;
                    ((s32 *)&work->unk7C)[5] = q1;
                    ((s32 *)&work->unk7C)[6] = q2;
                    ((s32 *)&work->unk7C)[7] = q3;
                }
            }
            cv2.r = cv2.g = cv2.b = 0x84;
            vv2.vy = vv2.vx = 0x1020;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv2.vz = 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv2, &cv2, 0, NULL);
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk20.vx = work->unk8.vx;
                work->unk20.vy = work->unk8.vy;
                work->unk20.vz = work->unk8.vz;
                work->unk20.vy -= 0x200;
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            break;
        case 4:
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->unk14)[0];
                q1 = ((s32 *)env->unk14)[1];
                q2 = ((s32 *)env->unk14)[2];
                q3 = ((s32 *)env->unk14)[3];
                ((s32 *)&work->unk9C)[0] = q0;
                ((s32 *)&work->unk9C)[1] = q1;
                ((s32 *)&work->unk9C)[2] = q2;
                ((s32 *)&work->unk9C)[3] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->unk14)[4];
                q1 = ((s32 *)env->unk14)[5];
                q2 = ((s32 *)env->unk14)[6];
                q3 = ((s32 *)env->unk14)[7];
                ((s32 *)&work->unk9C)[4] = q0;
                ((s32 *)&work->unk9C)[5] = q1;
                ((s32 *)&work->unk9C)[6] = q2;
                ((s32 *)&work->unk9C)[7] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)&work->unk7C)[0];
                q1 = ((s32 *)&work->unk7C)[1];
                q2 = ((s32 *)&work->unk7C)[2];
                q3 = ((s32 *)&work->unk7C)[3];
                ((s32 *)env->unk14)[0] = q0;
                ((s32 *)env->unk14)[1] = q1;
                ((s32 *)env->unk14)[2] = q2;
                ((s32 *)env->unk14)[3] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)&work->unk7C)[4];
                q1 = ((s32 *)&work->unk7C)[5];
                q2 = ((s32 *)&work->unk7C)[6];
                q3 = ((s32 *)&work->unk7C)[7];
                ((s32 *)env->unk14)[4] = q0;
                ((s32 *)env->unk14)[5] = q1;
                ((s32 *)env->unk14)[6] = q2;
                ((s32 *)env->unk14)[7] = q3;
            }
            t = (frame << 12) / 20;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            ex->Hi_ModifyEffModelAbr(work->unk64, 1);
            c = 0x1000 - t;
            ex->Hi_ModifyEffModelRGB(work->unk64, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)&work->unk9C)[0];
                q1 = ((s32 *)&work->unk9C)[1];
                q2 = ((s32 *)&work->unk9C)[2];
                q3 = ((s32 *)&work->unk9C)[3];
                ((s32 *)env->unk14)[0] = q0;
                ((s32 *)env->unk14)[1] = q1;
                ((s32 *)env->unk14)[2] = q2;
                ((s32 *)env->unk14)[3] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)&work->unk9C)[4];
                q1 = ((s32 *)&work->unk9C)[5];
                q2 = ((s32 *)&work->unk9C)[6];
                q3 = ((s32 *)&work->unk9C)[7];
                ((s32 *)env->unk14)[4] = q0;
                ((s32 *)env->unk14)[5] = q1;
                ((s32 *)env->unk14)[6] = q2;
                ((s32 *)env->unk14)[7] = q3;
            }
            cv2.r = cv2.g = cv2.b = 0x84;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv3.vy = vv3.vx = 0x1020;
            vv3.vz = 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv3, &cv2, 0, NULL);
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), -0x2C, 0x78, 0, 1, c);
            if (frame >= 0x14) {
                ex->btlfx_poolClear(entry->unk10);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->func_800E6624(work->unk4 * 4, 0x2C0, 0x100, 0x20, 0x80, 0x2E0, 0x100);
        ex->func_800E6624(0x2710 - work->unk4 * 4, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), work->unk4);
        work->unk4++;
    }
    return 0;
}

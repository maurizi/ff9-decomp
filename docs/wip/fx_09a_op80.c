/* Draft of fx_09a fx_09a_op80. Status: 88 aligned mismatches. Same summon framework as fx_05e op80 (one summon per
 * target unit). Open: frame lives in s2 in the original, (frame << 12) and its sign are computed before the frame < 6
 * test and shared with frame/22, and the ribbon-decay loops address the second edge point through an integer
 * induction variable plus base (a3 = 8 + 16*i) rather than a pointer. */
#include "btlfx_mod.h"

/* ribbon history: two edge points per step, unk0.pad = live */
typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx09aSeg; /* 0x10 */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* unit */
    s32 unkC; /* units done */
    s32 unk10; /* unit count */
    SVECTOR unk14; /* unit position */
    SVECTOR unk1C; /* unit rotation */
    SVECTOR unk24; /* summon position */
    s32 unk2C; /* ribbon head */
    s32 unk30;
    SVECTOR unk34; /* summon rotation */
    s32 unk3C[3]; /* summon scale */
} Fx09aWork; /* 0x48 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx09aSummon;

extern s32 D_801E87E4[2];
extern SVECTOR D_801E87EC;
extern SVECTOR D_801E87F0;
extern SVECTOR D_801E87F4;
extern SVECTOR D_801E87F8;
extern SVECTOR D_801E8800;
extern SVECTOR D_801E8808;
extern Unk800F78A4 *D_801E8858;
extern Unk80100DC054 D_801E8868;
extern u8 D_801E89F8[];
extern Fx09aSeg D_801E9058[];
extern Fx09aSeg D_801E9168[];

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx09aSeg *buf, s32 abr);
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

s32 fx_09a_op80(s32 mode, Fx09aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    s32 file;
    Pool6CEntry *p;
    s32 frame;
    s32 k;
    s32 t;
    s32 i;
    s32 v;
    s32 rgb;
    SVECTOR *sv;

    if (mode == 0) {
        *arg4 = sizeof(Fx09aWork);
    } else if (mode == 1) {
        work->unk0 = 1;
        work->unk4 = 0;
        D_801E8858 = env;
        file = ex->btlfx_getFileData(entry->unk8, 2);
        ex->func_800E9000((Unk800E9000 *)ex->btlfx_getFileData(entry->unk8, 8), &file, 1, &D_801E8868);
        work->unk8 = -1;
        work->unkC = -1;
        work->unk10 = env->unkC->unk24;
        env->unk0->pad10[0] = 0;
    } else {
        frame = entry->unk0;
        k = 0;
        switch (work->unk0) {
        case 1:
            work->unkC++;
            work->unk8++;
            if (work->unkC >= work->unk10) {
                work->unk0 = 4;
                entry->unk0 = -1;
                break;
            }
            ex->btlfx_getUnitPos(work->unk8, (s16 *)&work->unk14);
            ex->btlfx_getUnitRot(work->unk8, (s16 *)&work->unk1C);
            work->unk1C.vy &= 0xFFF;
            if (work->unk1C.vy - 0x400U > 0x800) {
                work->unk1C.vy = 0;
            } else {
                work->unk1C.vy = 0x800;
            }
            ex->func_800CFCA8(&work->unk1C, ex->btlfx_getUnitRadius(work->unk8) + 0x78, (s32)&a);
            work->unk24.vx = work->unk14.vx + a.vx;
            work->unk24.vy = work->unk14.vy + a.vy;
            work->unk24.vz = work->unk14.vz + a.vz;
            ex->btlfx_getUnitBodyPos(work->unk8, 0, (s16 *)&work->unk14);
            BTLFX_COPY_SVEC(&b, &work->unk24);
            b.vy -= 0x140;
            env->unk2C->unk0[0].vx = b.vx;
            env->unk2C->unk0[0].vy = b.vy;
            env->unk2C->unk0[0].vz = b.vz;
            ex->func_800CFCA8(&work->unk1C, -0xD2, (s32)&a);
            b.vx = work->unk24.vx + a.vx;
            b.vy = work->unk24.vy + a.vy - 0x140;
            b.vz = work->unk24.vz + a.vz;
            env->unk2C->unk0[1].vx = b.vx;
            env->unk2C->unk0[1].vy = b.vy;
            env->unk2C->unk0[1].vz = b.vz;
            ex->func_800CFCA8(&work->unk1C, -ex->btlfx_getUnitRadius(0), (s32)&a);
            b.vx = work->unk14.vx + a.vx;
            b.vy = a.vy - 0x140;
            b.vz = work->unk14.vz + a.vz;
            env->unk2C->unk0[2].vx = b.vx;
            env->unk2C->unk0[2].vy = b.vy;
            env->unk2C->unk0[2].vz = b.vz;
            ex->Hi_SetSummonMotion(((Fx09aSummon *)env->unk4)->unk54->unk180, ((Fx09aSummon *)env->unk4)->unk60);
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->unk4)->unk60, 6);
            work->unk3C[2] = 0x1000;
            work->unk3C[1] = 0x1000;
            work->unk3C[0] = 0x1000;
            work->unk34.vx = 0;
            work->unk34.vz = 0;
            work->unk34.vy = work->unk1C.vy;
            ex->func_800E9930(0xF, entry->unk8, -1);
            ex->func_800E9930(0x10, entry->unk8, -1);
            ex->func_800E9930(0x11, entry->unk8, -1);
            work->unk0 = 2;
            entry->unk0 = -1;
            break;
        case 2:
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            pos.vz = work->unk24.vz;
            ex->Hi_DrawSummonModel(&work->unk34, &pos, (s32)work->unk3C, ((Fx09aSummon *)env->unk4)->unk60, 1);
            t = frame << 12;
            if (frame < 6) {
                rgb = (t / 6 >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->unk4)->unk60, rgb, rgb, rgb);
            }
            if (frame == 0) {
                k = -1;
            } else if (frame < 22) {
                k = t / 22;
            } else {
                k = 0x1000;
            }
            if (frame == 24) {
                ex->func_800E9930(0x12, entry->unk8, -1);
                ex->func_800E9930(0x13, entry->unk8, -1);
                ex->func_800E9930(0x14, entry->unk8, -1);
            }
            if (frame == 26) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 27) {
                p = (Pool6CEntry *)ex->func_800E3CEC(
                    ex->btlfx_getFileData(entry->unk8, D_801E87E4[((work->unk1C.vy & 0xFFF) - 0x400U > 0x800) != ((env->unk2C->unk50 & 0xFFF) - 0x400U > 0x800)]),
                    (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
                ex->func_800E9930(0x15, entry->unk8, -1);
                ex->func_800E9930(0x16, entry->unk8, -1);
                ex->func_800E9930(0x17, entry->unk8, -1);
            }
            if (frame == 28) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xDC, 0, 0x20);
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
            }
            if (frame == 29) {
                ex->btlfx_execCmd(work->unk8);
            }
            if (frame >= 30) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            v = (frame << 12) / 24;
            if (frame == 2) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E89F8;
                    p->unk12 = 0x18;
                    p->unkC = 0x44;
                    p->unk22 = 0x40;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk24);
            }
            work->unk3C[2] = work->unk3C[0] = ((0x1000 - ex->math_sin(v)) << 12) >> 12;
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            pos.vz = work->unk24.vz;
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->unk4)->unk60, 41 - frame);
            ex->Hi_DrawSummonModel(&work->unk34, &pos, (s32)work->unk3C, ((Fx09aSummon *)env->unk4)->unk60, 1);
            k = ex->math_cos(v);
            ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->unk4)->unk60, (k >> 5) - 0x80, (k >> 5) - 0x80,
                                        (k >> 5) - 0x80);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 4:
            if (frame >= 8) {
                env->unk0->pad10[0] = 1;
                return 1;
            }
            break;
        }
        if (k != 0) {
            func_801E7B74(((Fx09aSummon *)env->unk4)->unk60, 9, &D_801E87F8, &a);
            func_801E7B74(((Fx09aSummon *)env->unk4)->unk60, 9, &D_801E8800, &b);
            func_801E7B74(((Fx09aSummon *)env->unk4)->unk60, 9, &D_801E8808, &c);
            if (k < 0) {
                func_801E7704(&a, &b, 0x3F8, (CVECTOR *)&D_801E87EC, (CVECTOR *)&D_801E87F0, 0, D_801E9058, 1);
                func_801E7704(&b, &c, 0x3F8, (CVECTOR *)&D_801E87F0, (CVECTOR *)&D_801E87F4, 0, D_801E9168, 1);
            } else {
                func_801E7704(&a, &b, 0x10, (CVECTOR *)&D_801E87EC, (CVECTOR *)&D_801E87F0, k, D_801E9058, 1);
                func_801E7704(&b, &c, 0x10, (CVECTOR *)&D_801E87F0, (CVECTOR *)&D_801E87F4, k, D_801E9168, 1);
                BTLFX_COPY_SVEC(&work->unk2C, &a);
                sv = (SVECTOR *)D_801E9058;
                for (i = 0; i < 17; i++) {
                    sv[i * 2].vy -= sv[i * 2].vy >> 6;
                    sv[i * 2 + 1].vy -= sv[i * 2 + 1].vy >> 4;
                }
                sv = (SVECTOR *)D_801E9168;
                for (i = 0; i < 17; i++) {
                    sv[i * 2].vy -= sv[i * 2].vy >> 4;
                    sv[i * 2 + 1].vy -= sv[i * 2 + 1].vy >> 3;
                }
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 3), work->unk4);
        work->unk4++;
    }
    return 0;
}

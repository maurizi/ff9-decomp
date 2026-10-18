/* Draft of fx_097 func_801E7CBC. Status: 1 instruction pair out of order: the hoisted &o (fp = sp+0x60) comes one
 * slot before the spilled y init (li t0,-0x180) in the original; everything else matches. The inner loop's
 * q[1].vx/q[3].vx = y statement before the vy pair puts the reload of rx (0x74(sp)) ahead of col << 6. Same body in
 * fx_098 and fx_1cc. */
#include "btlfx_mod.h"

extern s16 D_801E9D50[];
extern Unk800F78A4 *D_801E9DB4;

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx097Vec3;

/* one shard, a pool item */
typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* flying */
    s32 unk4; /* index */
    s32 unk8; /* frame */
    Fx097Vec3 unkC; /* position << 4 */
    SVECTOR unk18; /* velocity */
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* models */
    s32 unk30;
    Pool6CEntry *unk34;
} Fx097Item; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 unk20[22]; /* models */
    s16 unk78;
} Fx097Work;

extern SVECTOR D_801E9CE8[];
s32 func_801E82F4(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work);
typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* models */
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20[14];
    SVECTOR unk58; /* summon position */
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    s16 unk78;
    s16 unk7A; /* shards spawned */
    s32 unk7C;
    s32 unk80;
    Pool6CEntry *unk84;
} Fx097Work80; /* 0x88 */

extern BtlModel D_801E9DC4[];
extern s32 D_801E9D18[14];
void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 p, s32 u, s32 tpage, s32 a6, s32 a7, s32 a8, s32 abr);
s32 fx_097_op80(s32 mode, Fx097Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr) {
    Unk800F78A4 *env = D_801E9DB4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR sc;
    SVECTOR q[4];
    SVECTOR o;
    s32 otz;
    s32 row;
    s32 code;
    s32 rx;
    s32 y;
    POLY_GT4 *prim;
    s32 col;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 k;
    s32 tu;
    s32 tu2;
    s32 tv;
    s32 tv2;

    *(s32 *)&o.vx = 0;
    o.vz = 0;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)env->unk24;
    code = 0x3C;
    env->unk24 = (u8 *)(prim + 14 * 14);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (row = 0, y = -0x180; row < 14; y += 0x40, row++) {
        rx = row << 6;
        for (col = 0; col < 14; col++) {
            q[0].vz = q[1].vz = q[2].vz = q[3].vz = 0;
            q[0].vx = q[2].vx = rx - 0x1C0;
            q[1].vx = q[3].vx = y;
            q[0].vy = q[1].vy = (col << 6) - 0x1C0;
            q[2].vy = q[3].vy = (col << 6) - 0x180;
            d0 = ex->btlfx_distance(&q[0], &o);
            d1 = ex->btlfx_distance(&q[1], &o);
            d2 = ex->btlfx_distance(&q[2], &o);
            d3 = ex->btlfx_distance(&q[3], &o);
            if (d0 > 0x100) {
                q[0].vz = 0;
            } else {
                q[0].vz = -(((ex->math_cos(d0 * 8) + 0x1000) * h) >> 13);
            }
            if (d1 > 0x100) {
                q[1].vz = 0;
            } else {
                q[1].vz = -(((ex->math_cos(d1 * 8) + 0x1000) * h) >> 13);
            }
            if (d2 > 0x100) {
                q[2].vz = 0;
            } else {
                q[2].vz = -(((ex->math_cos(d2 * 8) + 0x1000) * h) >> 13);
            }
            if (d3 > 0x100) {
                q[3].vz = 0;
            } else {
                q[3].vz = -(((ex->math_cos(d3 * 8) + 0x1000) * h) >> 13);
            }
            gte_ldv3c(q);
            gte_rtpt();
            k = c - (-q[0].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d0 > 0x1A0) {
                k = 0;
            }
            prim->code = code;
            prim->r0 = prim->g0 = prim->b0 = k;
            k = c - (-q[1].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d1 > 0x1A0) {
                k = 0;
            }
            prim->r1 = prim->g1 = prim->b1 = k;
            k = c - (-q[2].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d2 > 0x1A0) {
                k = 0;
            }
            prim->r2 = prim->g2 = prim->b2 = k;
            k = c - (-q[3].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d3 > 0x1A0) {
                k = 0;
            }
            prim->r3 = prim->g3 = prim->b3 = k;
            gte_nclip();
            prim->tpage = tpage;
            prim->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&q[3]);
                gte_rtps();
                tu = u + ((row & 3) << 5);
                tu2 = tu + 0x20;
                tv = v + ((col & 3) << 5);
                if ((row & 3) == 3) {
                    tu2 = tu + 0x1F;
                }
                tv2 = tv + 0x20;
                if ((col & 3) == 3) {
                    tv2 = tv + 0x1F;
                }
                prim->u0 = prim->u2 = tu;
                prim->u1 = prim->u3 = tu2;
                prim->v0 = prim->v1 = tv;
                prim->v2 = prim->v3 = tv2;
                ex->btlfx_addPrim(0xC000000, otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
        }
    }
}

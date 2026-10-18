/* Draft of fx_0d3 func_801E8628. Status: 145 aligned mismatches, structure complete (textured ribbon, POLY_GT4).
 * Open: register allocation; the original keeps i in a3 with i + 1 in s5 across btlfx_addPrim, and u/v/w/tpage spill
 * to sp20..2C around the call. */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801ED534;
extern s32 D_801EB084[5];
extern s32 D_801EB098;
extern u16 D_801EB0C4[3]; /* file ids for op87 tracks, end frame */
extern s32 D_801EDF44;
extern s32 D_801EDF48;

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
s32 func_801E9050(SVECTOR *v);
void func_801E90D4(s16 *v);
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
void func_801E8470(Fx0d3Track *track, s32 t, SVECTOR *out);
void func_801E84C0(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E8548(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E85FC(Fx0d3Track *track, s32 t);
/* ribbon history: two edge points per step, unk0.pad = live */
typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx0d3Seg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E8628(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 k);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d3Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* summon position */
    Fx0d3Vec3 unk10;
    Fx0d3Vec3 unk1C; /* summon scale */
    SVECTOR unk28; /* summon rotation */
    Pool6CEntry *unk30;
    Pool6CEntry *unk34;
    u8 pad38[0x60 - 0x38];
    s32 unk60; /* models */
    s32 unk64;
    s32 unk68; /* tint */
} Fx0d3Work80; /* 0x6C */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0d3Summon;

extern SVECTOR D_801EB048;
extern s32 D_801EB050;
extern u8 D_801EB114[];
extern BtlModel D_801ED544[];
void func_801E7CBC(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));
s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

void func_801E8628(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 k) {
    Unk800F78A4 *env = D_801ED534;
    CVECTOR col;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_GT4 *prim;
    Fx0d3Seg *s;
    s32 i;
    s32 c;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[n - 1];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (s32)(prim + n);
    col.cd = abr != 0xFF ? 0x3E : 0x3C;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    for (i = 0; i < n; i++, buf++, prim++, k++) {
        gte_ldv3c(buf);
        c = (i << 12) / n;
        col.r = col.g = col.b = p;
        gte_rtpt_b();
        c = ((0x1000 - c) * p) >> 12;
        col.r = col.g = col.b = c;
        *(s32 *)&prim->r1 = *(s32 *)&col;
        *(s32 *)&prim->r0 = *(s32 *)&col;
        col.r = col.g = col.b = c;
        *(s32 *)&prim->r3 = *(s32 *)&col;
        *(s32 *)&prim->r2 = *(s32 *)&col;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrim(0xC000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->v3 = prim->v2 = v + ((h << (k & 1)) >> 1);
            prim->u2 = prim->u0 = u;
            prim->u3 = prim->u1 = u + w;
            prim->v1 = prim->v0 = v + (((k & 1) * h) >> 1);
        }
    }
}

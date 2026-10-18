/* Draft of fx_07d func_801E7718. Status: 301 aligned mismatches, structure complete (screen ripple: grid of POLY_GT4
 * sampling the frame buffer, DR_MOVE when src is given). Shared body: also fx_07e 7704, fx_0cb 7704, fx_0d3 7CBC,
 * fx_0df 7704, fx_18b 7704. Open: heavy spilling (params live in their home slots, env in fp, ex spilled at sp114),
 * and the init loop writes pts[1][i].vy through an integer giv + base. */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    s32 unk10; /* buffer index */
    s32 *unk14; /* per-point (t offset, amplitude, step) */
} Fx07dArc;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx07dStrip; /* 0x7C */

extern Unk800F78A4 *D_801E9FB0;
extern SVECTOR D_801EA470[24];
extern s32 D_801EA530;
extern Unk80100C78Entry *D_801EA534;
extern s32 D_801EA538;
extern Fx07dStrip D_801EA540[];
extern s32 D_801EA8B0[64][3]; /* random arc profile */

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    SVECTOR unk4; /* position */
    SVECTOR unkC; /* glow rotation */
    s16 unk14; /* spin angle */
    s16 unk16; /* spin speed */
} Fx07dOrb;

typedef struct {
    u8 pad0[0x20];
    s32 unk20; /* glow model */
    s32 unk24;
    s32 unk28; /* model */
    s32 unk2C; /* model */
} Fx07dWork80;

typedef struct {
    SVECTOR unk0; /* caster body position */
    s32 *unk8; /* arc profile table */
    s32 unkC; /* arcs spawned */
    s32 unk10; /* variant */
} Fx07dWork83; /* 0x14 */

void func_801E7ECC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame);
void func_801E7FF4(s32 d, s32 a1, s32 a, s32 a3, s16 *out);
void func_801E80C4(s16 *a, s16 *b);
void func_801E9834(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad);
s32 func_801E9A40(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
s32 func_801E8108(s32 mode, Fx07dOrb *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx07dWork80 *work);
void func_801E7718(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

void func_801E7718(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR a;
    DVECTOR b;
    DVECTOR c;
    DVECTOR d;
    DVECTOR e;
    DVECTOR f;
    DVECTOR g;
    DVECTOR h;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env = D_801E9FB0;
    u32 *move = NULL;
    s32 flag = 0;
    POLY_GT4 *prim;
    BtlfxExports *ex;
    s32 abr;
    s32 tp0;
    s32 tp1;
    s32 tp;
    s32 cx;
    s32 cy;
    s32 step;
    s32 ang;
    s32 r;
    s32 i;
    s32 j;
    s16 sx;
    s16 sy;

    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + n * m);
    abr = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        abr = 1;
    }
    c0.cd = abr != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        sy = src->y;
        sx = src->x;
        tp0 = getTPage(2, 0, sx, sy);
        tp1 = getTPage(2, 0, sx + 0x80, sy);
        src->x = env->unk1C->clip.x + (x - (src->w >> 1));
        src->y = env->unk1C->clip.y + (y - (src->h >> 1));
        move = (u32 *)env->unk24;
        env->unk24 = (u8 *)move + 0x18;
        ex->SetDrawMove(move, src, sx, sy);
        cx = (src->w >> 1) + (sx & 0x3F);
        cy = (src->h >> 1) + (sy & 0xFF);
    } else {
        cx = x;
        cy = y;
        tp0 = getTPage(2, 1, env->unk18->disp.x, env->unk18->disp.y);
        tp1 = getTPage(2, 1, env->unk18->disp.x + 0x80, env->unk18->disp.y);
    }
    tp0 <<= 16;
    tp1 <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        pts[1][i].vx = cx;
        pts[1][i].vy = cy;
    }
    step = 0x1000 / n;
    r = 0;
    for (j = 1; j <= m; j++) {
        ang = 0;
        c0.r = c0.g = c0.b = ex->btlfx_lerpRatio(from, to, j - 1, m);
        c1.r = c1.g = c1.b = ex->btlfx_lerpRatio(from, to, j, m);
        r += dr;
        for (i = 0; i <= n; i++) {
            a = pts[0][i];
            e = pts[1][i];
            if (i < n) {
                b.vx = (ex->math_cos(ang) * r) >> 12;
                b.vy = (ex->math_sin(ang) * r) >> 12;
                f.vx = b.vx + cx;
                f.vy = b.vy + cy;
            } else {
                b = pts[0][0];
                f = pts[1][0];
            }
            if (f.vx < 0) {
                f.vx = 0;
            }
            if (f.vx >= 0x140) {
                f.vx = 0x13F;
            }
            if (f.vy < 0) {
                f.vy = 0;
            }
            if (f.vy >= 0xF0) {
                f.vy = 0xEF;
            }
            pts[1][i] = f;
            if (i != 0) {
                if (e.vx < 0x100 && f.vx < 0x100 && g.vx < 0x100 && h.vx < 0x100) {
                    tp = tp0;
                    flag = 0;
                } else {
                    tp = tp1;
                    flag = 1;
                    e.vx -= 0x80;
                    f.vx -= 0x80;
                    g.vx -= 0x80;
                    h.vx -= 0x80;
                }
            }
            if (i < n && fn != NULL) {
                fn(r, j, ang, i, &b);
            }
            pts[0][i] = b;
            a.vx += x;
            a.vy += y;
            b.vx += x;
            b.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = e.vx | (e.vy << 8);
                *(s32 *)&prim->u1 = f.vx | (f.vy << 8) | tp;
                *(s32 *)&prim->u2 = g.vx | (g.vy << 8);
                *(s32 *)&prim->u3 = h.vx | (h.vy << 8);
                *(CVECTOR *)&prim->r2 = c0;
                *(CVECTOR *)&prim->r0 = c0;
                *(CVECTOR *)&prim->r3 = c1;
                *(CVECTOR *)&prim->r1 = c1;
                *(DVECTOR *)&prim->x0 = a;
                *(DVECTOR *)&prim->x1 = b;
                *(DVECTOR *)&prim->x2 = c;
                *(DVECTOR *)&prim->x3 = d;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->unk20[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (flag) {
                e.vx += 0x80;
                f.vx += 0x80;
                g.vx += 0x80;
                h.vx += 0x80;
            }
            ang += step;
            c = a;
            d = b;
            g = e;
            h = f;
        }
    }
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->unk20[ot], move, 0x5000000);
    }
}

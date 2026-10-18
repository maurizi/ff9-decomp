/* Draft of fx_095 func_801E7CBC. Status: 101/164 with registers, 5 aligned diff lines: register allocation only (case
 * 1 h temp, final or). Note case 2 needs s32 h = (s16)(r->h / 3) and case 1 an s16 temp. */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* spawned */
    DVECTOR unk8; /* caster screen position */
    s32 unkC; /* release */
} Fx095Work80; /* 0x10 */

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    u16 unk8[0x3000]; /* captured screen strip */
} Fx095Work81; /* 0x6008 */

typedef struct {
    s32 unk0; /* angle */
    s32 unk4; /* state */
    s32 unk8; /* index */
} Fx095Part; /* 0xC */

typedef struct {
    u8 pad0[8];
    s16 unk8; /* screen x */
    s16 unkA; /* screen y */
    s32 unkC; /* release */
} Fx095Ctl;

extern Unk800F78A4 *D_801E8904;
extern SVECTOR D_801E890C;
extern s32 D_801E882C[];
extern s32 D_801E885C[];
extern s32 D_801E888C[];

s32 func_801E7704(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx095Ctl *ctl);
void func_801E7CBC(s32 n, s32 x, s32 y, u16 *buf, RECT *r);
void func_801E7F48(s32 ot, s32 x, s32 y, s32 r, s32 c, RECT *rect);

void func_801E7CBC(s32 n, s32 x, s32 y, u16 *buf, RECT *r) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8904);
    RECT a;
    RECT b;
    RECT c;
    s32 k = (n - 1) >> 1;
    s32 h;
    s16 h1;
    s32 i;
    u16 *p;
    u32 v;
    u32 px;
    s32 cr;
    s32 cg;
    s32 cb;
    s32 sx;
    s32 sy;

    if (n != 0) {
        n = ((n - 1) & 1) + 1;
    }
    switch (n) {
    case 0:
        sx = r->x;
        sy = r->y;
        a.x = D_801E8904->unk1C->clip.x + (x - (r->w >> 1));
        a.y = D_801E8904->unk1C->clip.y + (y - (r->h >> 1));
        a.w = r->w;
        a.h = r->h;
        ex->MoveImage(&a, sx, sy);
        break;
    case 1:
        h1 = r->h / 3;
        b.x = r->x;
        b.y = r->y + k * h1;
        b.w = r->w;
        b.h = h1;
        ex->StoreImage(&b, (u32 *)buf);
    case 2:
        h = (s16)(r->h / 3);
        c.x = r->x;
        c.y = r->y + k * h;
        c.w = r->w;
        c.h = h;
        for (i = 0, p = buf; i < r->w * h; i++, p++) {
            px = *p;
            cr = (px & 0x1F) * 4;
            cg = (px >> 3) & 0x7C;
            cb = (px >> 8) & 0x7C;
            v = (cr + cg + cb) / 3 >> 2;
            if (v == 0) {
                v = 1;
            }
            *p = v | 0x8000 | (v << 5) | (v << 10);
        }
        ex->LoadImage(&c, (u32 *)buf);
        break;
    }
}

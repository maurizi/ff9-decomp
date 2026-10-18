/* Draft of fx_095 func_801E7704. Status: 11 aligned diff lines ignoring registers: delay-slot scheduling of the
 * constant 0x1000/abr loads around the math_cos calls. */
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

s32 func_801E7704(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx095Ctl *ctl) {

    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale;
    s32 a;
    s32 glow;
    s32 bright;
    s32 glowFrame;
    s32 abr;
    s32 t;
    s32 r;
    s32 x;
    s32 y;
    s32 *tab;

    if (mode == 1) {
        part->unk4 = 0;
        return 0;
    }
    bright = glow = a = scale = 0;
    glowFrame = 0;
    abr = scale;
    switch (part->unk4) {
    case 0:
        t = frame << 9;
        abr = 1;
        bright = 0x1000;
        scale = ((ex->math_cos(t >> 2) * 3) >> 1) + 0x1000;
        a = part->unk0 + ((0x1000 - t) >> 1);
        if (frame >= 8) {
            part->unk4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        scale = 0x1000;
        a = part->unk0;
        abr = 1;
        bright = scale;
        if (frame < 5) {
            t = frame << 10;
            glow = ex->math_cos(t >> 2);
            glowFrame = frame;
        }
        if (ctl->unkC != 0) {
            part->unk4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        t = frame << 8;
        scale = 0x1000;
        abr = 0xFF;
        bright = scale;
        ex->math_cos(t >> 2);
        a = part->unk0;
        if (frame >= 16) {
            part->unk4 = 3;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 3:
        t = frame << 9;
        scale = 0x1000;
        abr = 1;
        bright = ex->math_cos(t >> 2);
        a = part->unk0;
        if (frame >= 8) {
            return 1;
        }
        break;
    }
    scale = (scale * 62) >> 12;
    a -= 0x400;
    x = ctl->unk8 + ((ex->math_cos(a) * scale) >> 12);
    y = ctl->unkA + ((ex->math_sin(a) * scale) >> 12);
    if (part->unk4 < 2) {
        tab = D_801E882C;
    } else {
        tab = D_801E888C;
    }
    ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, tab[part->unk8]), x, y, 0, abr, bright);
    if (glow != 0) {
        ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E885C[part->unk8]), x, y, 0, 1, glow);
        ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x2C), x, y, glowFrame, 1, 0x1000);
    }
    return 0;
}

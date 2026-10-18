/* Draft of fx_0ba func_801EAC08. Status: 310/369, 26 aligned diff lines ignoring registers in case 2 (k/a/mult
 * scheduling), plus register allocation. */
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

s32 func_801EAC08(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR ang;
    Fx0baTrail *trail;
    Fx0baStrip4 *strip;
    s32 t;
    s32 c;
    s32 k;
    s32 a;
    s32 q;
    s32 amp;
    s32 t2;
    Fx0baTrail *trail2;
    Fx0baStrip4 *strip2;
    s32 c2;

    if (mode != 1) {
        switch (p->unk11) {
        case 0:
        case 1:
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            trail = &D_801ED420[p->unk10];
            strip = &D_801EE5A0[p->unk10];
            if (p->unk11 == 0) {
                p->unk8.vx = D_801EC554.vx;
                p->unk8.vy = D_801EC554.vy;
                p->unk8.vz = D_801EC554.vz;
                func_801EA850(entry, 0x1C, 0, (Fx0baStrip *)trail, 0x8C, p->unk18, &p->unk0, &p->unk8, 0, 0x180,
                              frame * 0x97, 0x140, frame, c, ((0x1000 - t) * p->unk15) >> 12,
                              ((((frame << 10) + t) << 7) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    k = 2;
                } else if (frame < 4) {
                    k = 3;
                } else {
                    k = 0;
                }
                a = ((0x1000 - t) * p->unk15) >> 12;
                if (frame == 0) {
                    a = (a * 3) >> 1;
                }
                func_801EA850(entry, 2, 0, (Fx0baStrip *)trail, 0x8C, p->unk18 + frame * k, &p->unk0, &p->unk8, 0,
                              0x180, frame * 0xC0, 0x168, frame, c, a, 0);
                func_801EAAF0(entry, 3, 0, (Fx0baStrip *)trail, (Fx0baStrip *)strip, 0x24, p->unk18, p->unk12, frame, c,
                              a >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
            break;
        case 2:
            t2 = frame << 9;
            c2 = ex->math_cos(t2 >> 2);
            trail2 = &D_801ED420[p->unk10];
            strip2 = &D_801EE5A0[p->unk10];
            if (frame < 3 && D_801EC550 != 0) {
                q = ((frame + 1) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->unk28 = 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x29), &p->unk0, NULL, (q >> 2) + 0x2000, 0, 1,
                                  0x1000 - q * 2 / 3);
                env->unk28 = 0;
            }
            if ((u32)(frame - 3) < 3 && D_801EC550 != 0) {
                q = ((frame - 2) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->unk28 = 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x29), &p->unk8, NULL, (q >> 2) + 0x2000, 0, 1,
                                  0x1000 - q * 2 / 3);
                env->unk28 = 0;
            }
            if (frame < 2) {
                ex->btlfx_lerpVec((s16 *)&p->unk0, (s16 *)&p->unk8, (frame + 1) << 11, (s16 *)&v);
                amp = p->unk15 >> 1;
            } else {
                BTLFX_COPY_SVEC(&v, &p->unk8);
                amp = p->unk15;
            }
            k = (frame < 3) * 2;
            a = 0x1000;
            a -= t2;
            a *= amp;
            func_801EA850(entry, 2, 0, (Fx0baStrip *)trail2, 0x8C, p->unk18 + frame * k, &p->unk0, &v, 0,
                          0x227, frame * 0xC0, 0x168, frame, c2, a / 1500, 0);
            func_801EAAF0(entry, 3, 0, (Fx0baStrip *)trail2, (Fx0baStrip *)strip2, 0x24, p->unk18, p->unk12, frame, c2,
                          a >> 11);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

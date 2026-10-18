/* Draft of fx_07e fx_07e_op81. Status: full draft, 43 aligned diff lines, all register allocation (ex is $s1 in the
 * original, $s2 here) and the scheduling that follows; case 2 reuses t for the cosine. */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* spawned */
    s32 unk8; /* frame */
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* ring rotation */
    s32 unk1C; /* radius */
    s32 unk20; /* scale */
    s32 unk24; /* shrink done */
    s32 unk28; /* model */
} Fx07eWork80; /* 0x2C */

typedef struct {
    s32 unk0; /* angle offset */
    s32 unk4; /* state */
    s32 unk8; /* index */
} Fx07ePart; /* 0xC */

extern Unk800F78A4 *D_801E8AB8;
extern BtlModel D_801E8AC8[];
extern s32 D_801E8B90;

s32 func_801E7EB8(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx07eWork80 *work);
void func_801E8498(s32 r, s32 a1, s32 a, s32 a3, s16 *out);
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* blend */
    SVECTOR unkC; /* caster body position */
} Fx07eWork81; /* 0x14 */

void func_801E7704(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, s16 *));
s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT r1;
    RECT r2;
    SVECTOR *pos;
    s32 frame;
    s32 t;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = sizeof(Fx07eWork81);
    } else if (mode == 1) {
        D_801E8AB8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk8 = ex->func_800E7CC4(0, 0x20);
        work->unk0 = 0;
    } else {
        frame = entry->unk0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        pos = &work->unkC;
        gte_ldv0(pos);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 12;
            env->unk28 = work->unk8;
            s = ex->math_sin(t >> 2);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, 0x2000, 0, 1, s, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, 0x2000, 0, 2, s);
            env->unk28 = 0;
            if (frame >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            r1.x = 0x1C0;
            r1.y = 0x180;
            r1.w = 0x7A;
            r1.h = 0x7A;
            t = frame << 8;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r1, func_801E8498);
            D_801E8B90 = ex->math_sin(t >> 2) >> 1;
            env->unk28 = work->unk8;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, 0x2000, 0, 1, 0x1000, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, 0x2000, 0, 2, 0x1000);
            env->unk28 = 0;
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            r2.x = 0x1C0;
            r2.y = 0x180;
            r2.w = 0x7A;
            r2.h = 0x7A;
            t = (frame << 12) / 12;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r2, func_801E8498);
            t >>= 2;
            D_801E8B90 = ex->math_cos(t) >> 1;
            env->unk28 = work->unk8;
            s = 0x1000 - ex->math_sin(t);
            t = ex->math_cos(t);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 1), pos, NULL, s * 2, 0, 1, t, 0, 0);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), pos, NULL, s * 2, 0, 2, t);
            env->unk28 = 0;
            if (frame >= 12) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xA), work->unk4);
        work->unk4++;
    }
    return 0;
}

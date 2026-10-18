/* Draft of fx_07e func_801E7EB8. Status: 8 aligned mismatches, only the prologue: the original loads
 * work->unk14.vz into s0 first and part->unk0 into a3 (the SVECTOR word copy in v0/v1); loading vz first
 * (s = vz; c = unk0; copy; s += c) instead moves c to v1 and the copy to a3/v0 (12). Cases 0-2 match (fresh
 * variables for the cos result and the colour, case 0's colour as an expression). */
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

s32 func_801E7EB8(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx07eWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR out;
    SVECTOR rot;
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    s32 s;
    s32 c;
    s32 n1;
    s32 n2;
    s32 (*fn)(SVECTOR *, VECTOR *, s32, s32);

    if (mode == 1) {
        part->unk4 = 0;
        return 0;
    }
    c = part->unk0;
    BTLFX_COPY_SVEC(&ang, &work->unk14);
    s = work->unk14.vz + c;
    ex->func_800D044C(&work->unkC, (s32)&out, 0, (XY32 *)&ang, work->unk1C, s);
    pos.vx = out.vx;
    pos.vy = out.vy;
    pos.vz = out.vz;
    scale.vx = scale.vy = scale.vz = work->unk20;
    BTLFX_COPY_SVEC(&rot, &ang);
    s += 0x400;
    rot.vz = s;
    switch (part->unk4) {
    case 0:
        s = frame << 9;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        ex->Hi_ModifyEffModelRGB(work->unk28, (s >> 5) - 0x80, (s >> 5) - 0x80, (s >> 5) - 0x80);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        if (frame >= 8) {
            part->unk4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        ex->Hi_ModifyEffModelRGB(work->unk28, 0, 0, 0);
        if (work->unk24 != 0) {
            part->unk4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        s = frame << 8;
        n1 = ex->math_cos(s >> 2);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        n2 = (n1 >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk28, n2, n2, n2);
        if (frame >= 16) {
            return 1;
        }
        break;
    }
    return 0;
}

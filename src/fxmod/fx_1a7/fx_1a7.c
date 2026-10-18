#include "fxmod/fx_1a7.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[0], D_801E8990[0], frame, frames);
}

s32 func_801E7748(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[1], D_801E8990[1], frame, frames);
}

s32 func_801E778C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[2], D_801E8990[2], frame, frames);
}

s32 func_801E77D0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[3], D_801E8990[3], frame, frames);
}

s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[4], D_801E8990[4], frame, frames);
}

s32 func_801E7858(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[5], D_801E8990[5], frame, frames);
}

s32 func_801E789C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[6], D_801E8990[6], frame, frames);
}

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[7], D_801E8990[7], frame, frames);
}

s32 func_801E7924(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[8], D_801E8990[8], frame, frames);
}

s32 func_801E7968(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8940);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C0, (s16 *)&D_801EA5D0, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E7AB4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[0], D_801E89B0[0], frame, frames);
}

s32 func_801E7AF8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[1], D_801E89B0[1], frame, frames);
}

s32 func_801E7B3C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[2], D_801E89B0[2], frame, frames);
}

s32 func_801E7B80(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[3], D_801E89B0[3], frame, frames);
}

s32 func_801E7BC4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[4], D_801E89B0[4], frame, frames);
}

s32 func_801E7C08(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[5], D_801E89B0[5], frame, frames);
}

s32 func_801E7C4C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[6], D_801E89B0[6], frame, frames);
}

s32 func_801E7C90(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[7], D_801E89B0[7], frame, frames);
}

s32 func_801E7CD4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[8], D_801E89B0[8], frame, frames);
}

s32 func_801E7D18(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8940);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C8, (s16 *)&D_801EA5D8, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1a7/nonmatchings/fx_1a7", fx_1a7_op80);

INCLUDE_ASM("asm/us/fx_1a7/nonmatchings/fx_1a7", fx_1a7_op81);

INCLUDE_RODATA("asm/us/fx_1a7/nonmatchings/fx_1a7", fx_1a7_headerPtr);

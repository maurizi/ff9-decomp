#include "fxmod/fx_12b.h"

s32 func_801E7704(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E97BC[i];
    Fx12bPath *path = &D_801E9B44[i];
    SVECTOR *ofs = &D_801EF7F4[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    if ((u16)(s->unk20 - 7) < 0xE) {
        return func_801E7B10(p, pos, s, frame, n);
    }
    return 0;
}

s32 func_801E77B0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E9894[i];
    Fx12bPath *path = &D_801E9B74[i];
    SVECTOR *ofs = &D_801EF824[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    if ((u16)(s->unk20 - 7) < 0xE) {
        return func_801E7CA8(p, pos, s, frame, n);
    }
    return 0;
}

s32 func_801E785C(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E996C[i];
    Fx12bPath *path = &D_801E9BA4;
    SVECTOR *ofs = &D_801EF7F4[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    return func_801E7B10(p, pos, s, frame, n);
}

s32 func_801E78F8(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    s32 i = p->unk1F - 1;
    Fx12bShard *s = &D_801E9A20[i];
    Fx12bPath *path = &D_801E9BAC;
    SVECTOR *ofs = &D_801EF824[i];

    if (s->unk20 >= 2 && s->unk20 < 7) {
        return func_801E7994(p, pos, s, path, ofs, frame, n);
    }
    return func_801E7CA8(p, pos, s, frame, n);
}

s32 func_801E7994(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
    SVECTOR v;

    if (frame == 0) {
        ex->func_800E9930(0x39, 0, -1);
        ex->func_800E9930(0x3A, 0, -1);
    }
    func_801E9744(path, s->unk20 - 2, &v);
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    s->unk8.vx = v.vx;
    s->unk8.vy = v.vy;
    s->unk8.vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E7B10(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
    SVECTOR v;
    s32 t = ((s->unk20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&s->unk8, (s16 *)&s->unk0, t, (s16 *)&v);
    t /= 2;
    v.vx += s->unk10 * ex->math_sin(t) / 4096;
    v.vy -= s->unk12 * ex->math_sin(t) / 4096;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 func_801E7CA8(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9B34);
    SVECTOR v;
    s32 t = ((s->unk20 - 6) << 12) / 14;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&s->unk8, (s16 *)&s->unk0, t, (s16 *)&v);
    t /= 2;
    v.vx -= s->unk10 * ex->math_sin(t) / 4096;
    v.vy -= s->unk12 * ex->math_sin(t) / 4096;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    p->unk0.vx = pos->vx << 12;
    p->unk0.vy = pos->vy << 12;
    p->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E7E40);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E7EB0);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E7F20);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E7F90);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E8000);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_op80);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_op81);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_op82);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E9690);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E96D4);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E972C);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E9744);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E97A8);

INCLUDE_RODATA("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_headerPtr);

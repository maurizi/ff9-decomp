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

s32 fx_12b_op82(s32 mode, Fx12bWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E9B34 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk1C);
        work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk14);
        work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk14);
        work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk14);
        work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk14);
        work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk14);
        if (work->unk0[0] != NULL) {
            work->unk0[0]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        if (work->unk0[1] != NULL) {
            work->unk0[1]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        if (work->unk0[2] != NULL) {
            work->unk0[2]->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E9690);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E96D4);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E972C);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E9744);

INCLUDE_ASM("asm/us/fx_12b/nonmatchings/fx_12b", func_801E97A8);

INCLUDE_RODATA("asm/us/fx_12b/nonmatchings/fx_12b", fx_12b_headerPtr);

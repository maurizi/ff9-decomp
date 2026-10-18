#include "fxmod/fx_0d3.h"

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E7720);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E7CBC);

void func_801E8470(Fx0d3Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx0d3Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E84C0(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

void func_801E8548(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

s32 func_801E85FC(Fx0d3Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E8628);

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v) {
    frame += 7;
    v[0] = (v[0] * frame) >> 3;
    v[1] = (v[1] * frame) >> 3;
}

s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 k;
    s32 c;
    s32 m;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d3Work80);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED534 = env;
        ex->btlfx_initEffModels(D_801ED544, 7);
        D_801EB050 = work->unk60 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        m = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x26), 1);
        work->unk28.vy = 0x800;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        work->unk64 = m;
        work->unk28.vx = 0;
        work->unk28.vz = 0;
        work->unk10.vx = D_801EB048.vx;
        work->unk10.vy = D_801EB048.vy;
        work->unk10.vz = D_801EB048.vz;
        work->unk10.vy -= 0x800;
        work->unk8.vx = work->unk10.vx;
        work->unk8.vy = work->unk10.vy;
        work->unk8.vz = work->unk10.vz;
        env->unk2C->unk0[3].vx = work->unk10.vx;
        env->unk2C->unk0[3].vy = work->unk10.vy;
        env->unk2C->unk0[3].vz = work->unk10.vz;
        D_801EB050 = m;
        work->unk30 = NULL;
        work->unk34 = NULL;
        work->unk68 = 0;
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 75;
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x28) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EB114;
                    p->unk12 = 0xC0;
                    p->unkC = 0x1C;
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x32) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
                work->unk30 = p;
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x1E) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                work->unk34 = p;
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x3C) {
                ex->Hi_SetSummonMotion(((Fx0d3Summon *)env->unk4)->unk54->unk180, ((Fx0d3Summon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->unk4)->unk60, 0);
            } else if (frame >= 0x3C) {
                k = frame - 0x3C;
                if (k < 8) {
                    ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->unk4)->unk60, 0);
                }
                work->unk10.vy += 0xC00;
                ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                                       ((Fx0d3Summon *)env->unk4)->unk60, 0);
                work->unk10.vy -= 0xC00;
                if (k < 8) {
                    ex->Hi_ModifySummonModelRGB(((Fx0d3Summon *)env->unk4)->unk60, ((k * 0xA0) >> 3) - 0xA0,
                                                ((k * 0xA0) >> 3) - 0xA0, ((k * 0xA0) >> 3) - 0xA0);
                    ex->Hi_ModifySummonModelAbr(((Fx0d3Summon *)env->unk4)->unk60, 1);
                }
            }
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            rot.vy = 0x800;
            rot.vx = 0;
            rot.vz = frame * 8;
            scale.vz = scale.vx = scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk60);
            c = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk60, c, c, c);
            rot.vz = -frame * 4;
            rot.vx = 0;
            rot.vy = 0x800;
            scale.vz = scale.vx = scale.vy = 0x738;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, c, c, c);
            work->unk68 += 2;
            work->unk68 += t >> 9;
            ex->func_800E6624(work->unk68, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->func_800E6624(work->unk68 * 2 / 3, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
            func_801E7CBC(2, 0xA0, 0x78, 0x10, 4, 0x29, ((ex->math_sin(t >> 2) << 5) >> 12) + 0x40, 0, 0,
                          func_801E8A10);
            if (frame >= 0x4B) {
                ex->func_800E3C58();
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

s32 func_801E9050(SVECTOR *v) {
    s32 i = D_801EB098;
    Unk800F78A4 *env = D_801ED534;
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    v->pad = 1;
    ex->Hi_GetSummonBonePos(((s32 *)env->unk4)[0x60 / 4], D_801EB084[i], (u16 *)v);
    i++;
    if (i >= 5) {
        i = 0;
    }
    D_801EB098 = i;
    return 0;
}

void func_801E90D4(s16 *v) {
    s32 t = (BTLFX_EXPORTS(D_801ED534)->math_sin(D_801EDF48 + (v[1] << 8)) * D_801EDF44) >> 12;
    s32 x = v[0];

    v[0] = x + t;
}

INCLUDE_RODATA("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_headerPtr);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_op81);

s32 fx_0d3_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801ED534 = env;
        env->unk2C->unk52 = 0;
    } else {
        return 1;
    }
    return 0;
}

s32 fx_0d3_op87(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801ED534 = env;
    } else {
        frame = entry->unk0;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[1]), frame, &work[0]);
        env->unk2C->unk0[1].vx = work[0].vx;
        env->unk2C->unk0[1].vy = work[0].vy;
        env->unk2C->unk0[1].vz = work[0].vz;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[0]), frame, &work[1]);
        env->unk2C->unk0[0].vx = work[1].vx;
        env->unk2C->unk0[0].vy = work[1].vy;
        env->unk2C->unk0[0].vz = work[1].vz;
        if (frame >= D_801EB0C4[2]) {
            return 1;
        }
    }
    return 0;
}

/* Draft of fx_0e0 func_801E8628. Status: 9 aligned mismatches, one instruction: in case 4 the original computes
 * frame << 6 (the math_cos argument) one load earlier (after lh 0xE(s1) instead of lh 0x10(s1)). Registers all
 * match; case 2 needed the fresh x1 = t >> 1 (a reused t >>= 1 is hoisted into the draw call's delay slot). */
#include "fxmod/fx_0e0.h"

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* sub-state */
    SVECTOR unk4; /* velocity (vx, vy, vz); pad = model scale index */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* rotation speed */
    s16 unk1E;
    Pool6CEntry *unk20; /* particle */
} Fx0e0Obj;

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* target */
    u8 pad10[0x5C - 0x10];
    s32 unk5C; /* model */
    u8 pad60[0x70 - 0x60];
    s32 unk70; /* model */
} Fx0e0Inst;

extern s32 D_801EA8B8[];
extern s32 D_801EB1D8;
extern s32 D_801EB1DC;

s32 func_801E8628(s32 mode, Fx0e0Obj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5,
                  Fx0e0Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR d;
    SVECTOR old;
    SVECTOR q;
    SVECTOR pad;
    s32 k;
    s32 sub;
    s32 t;
    s32 r;
    s32 g;
    s32 i;
    s32 f;
    s32 x1;
    Pool6CEntry *e;

    if (mode == 1) {
        o->unk2 = 0;
        o->unk14.vx = ex->btlfx_rand();
        o->unk14.vy = ex->btlfx_rand();
        o->unk14.vz = 0;
        o->unk1C = ex->btlfx_rand() % 64;
        o->unk1E = ex->btlfx_rand() % 32;
        o->unk20 = NULL;
    } else {
    switch (o->unk0) {
    case 0:
    case 1:
        sub = o->unk2;
        k = 0;
        switch (sub) {
        case 0:
            o->unk14.vx += o->unk1C;
            o->unk14.vy += o->unk1E;
            old.vx = o->unkC.vx;
            old.vy = o->unkC.vy;
            old.vz = o->unkC.vz;
            o->unkC.vx += o->unk4.vx;
            o->unkC.vy += o->unk4.vy;
            o->unkC.vz += o->unk4.vz;
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            scale.vx = scale.vy = scale.vz = D_801EA8B8[o->unk4.pad];
            ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk5C);
            d.vx = inst->unk8.vx - o->unkC.vx;
            d.vy = inst->unk8.vy - o->unkC.vy;
            d.vz = inst->unk8.vz - o->unkC.vz;
            if (frame < 8) {
                k = (frame << 12) >> 3;
            } else {
                k = 0x1000;
            }
            if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < D_801EB1D8 * D_801EB1D8 * D_801EB1D8) {
                o->unk2 = 1;
                ((s16 *)o)[-1] = -1;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&o->unkC);
                o->unk20 = e;
                if (e != NULL) {
                    e->unk22 = 0x20;
                }
            }
            break;
        case 1:
            scale.vx = scale.vy = scale.vz = D_801EA8B8[o->unk4.pad];
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk5C);
            ex->btlfx_aimAngles((s16 *)&inst->unk8, (s16 *)&o->unkC, (s16 *)&ang);
            ang.pad = 1;
            if (o->unk20 != NULL) {
                o->unk20->unk5C.vx = ang.vx;
                o->unk20->unk5C.vy = ang.vy;
                o->unk20->unk5C.vz = ang.vz;
            }
            if (frame < 4) {
                k = 0x1000 - ((frame << 12) >> 2);
            }
            o->unk4.vx = (o->unk4.vx * 2) / 3;
            o->unk4.vy = (o->unk4.vy * 2) / 3;
            o->unk4.vz = (o->unk4.vz * 2) / 3;
            f = frame < 8;
            env->unk28 = 0x1F4;
            if (f) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &o->unkC, NULL, 0x3000, frame, 1,
                                  ex->math_cos(frame << 7));
            }
            env->unk28 = 0x20;
            if (f) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &o->unkC, &ang, 0x2000, frame, 1,
                                  ex->math_cos(frame << 7));
            }
            env->unk28 = 0;
            if (D_801EB1DC != 0) {
                return 1;
            }
            break;
        }
        if (k != 0) {
            env->unk28 = 0x180;
            BTLFX_COPY_SVEC(&q, &o->unkC);
            q.vx -= o->unk4.vx;
            q.vy -= o->unk4.vy;
            q.vz -= o->unk4.vz;
            for (i = 0; i < 12; i++) {
                q.vx -= o->unk4.vx;
                q.vy -= o->unk4.vy;
                q.vz -= o->unk4.vz;
                func_801E7B08(&q, NULL, 0x600, k, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
            }
            env->unk28 = 0;
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        scale.vx = scale.vy = (ex->math_sin(t >> 2) >> 1) + 0x800;
        scale.vz = o->unk4.pad + 0x200;
        o->unk14.vz -= 0x40;

        o->unkC.vy -= 0x4D;
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk70);
        x1 = t >> 1;
        r = (ex->math_sin(x1) >> 5) - 0x80;
        g = (ex->math_sin(x1) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(inst->unk70, r, g, (ex->math_sin(x1) >> 5) - 0x80);
        if (frame >= 0x18) {
            return 1;
        }
        break;
    case 3:
        o->unk14.vx += o->unk1C * 2;
        o->unk14.vy += o->unk1E * 2;
        o->unkC.vx += o->unk4.vx;
        o->unkC.vy += o->unk4.vy;
        o->unkC.vz += o->unk4.vz;
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        scale.vz = ((o->unk4.pad * ex->math_cos((frame << 8) >> 2)) >> 12) * 2;
        scale.vx = scale.vy = scale.vz / 3;
        ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk5C);
        ex->Hi_ModifyEffModelRGB(inst->unk5C, -0x4B, -0x4B, -0x4B);
        if (frame >= 0x10) {
            return 1;
        }
        break;
    case 4:
        o->unk14.vx += o->unk1C;
        o->unk14.vy += o->unk1E;
        o->unkC.vx += o->unk4.vx;
        o->unkC.vy += o->unk4.vy;
        o->unkC.vz += o->unk4.vz;
        o->unk4.vx = (o->unk4.vx * 31) >> 5;
        o->unk4.vy = (o->unk4.vy * 31) >> 5;
        o->unk4.vz = (o->unk4.vz * 31) >> 5;
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        scale.vz = ((o->unk4.pad * ex->math_cos((frame << 6) >> 2)) >> 12) * 2;
        scale.vx = scale.vy = scale.vz >> 1;
        ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk5C);
        ex->Hi_ModifyEffModelRGB(inst->unk5C, 0, -0x4B, -0x4B);
        if (frame >= 0x40) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

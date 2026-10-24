#include "fxmod/fx_082.h"

s32 fx_082_op80(s32 mode, Fx082Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale[3];
    s32 mpos[3];
    Pool6CEntry *p;
    s32 frame;
    s32 sc;
    s32 morph;
    s32 bright;
    s32 c;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E7E64 = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk18);
        BTLFX_COPY_SVEC(&work->unk8, &work->unk18);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk20);
        ex->btlfx_pointAhead((s16 *)&work->unk18, (s16 *)&work->unk20, 0x226, (s16 *)&work->unk18);
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk18.vy -= 0x145;
        BTLFX_COPY_SVEC(&work->unk28, &work->unk20);
        ex->btlfx_initEffModels(D_801E7E74, 2);
        work->unk30 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 0);
        work->unk34 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 0);
        env->camera->points[0].vx = work->unk18.vx;
        env->camera->points[0].vy = work->unk18.vy;
        env->camera->points[0].vz = work->unk18.vz;
    } else {
        bright = morph = sc = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 7;
            if (frame == 0) {
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vy -= 0x80;
                if (work->unk18.vz > 0) {
                    work->unk10.vz -= 0x40;
                } else {
                    work->unk10.vz += 0x40;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            work->unk28.vx = -0x200;
            sc = (t * 0xC00) >> 12;
            bright = 0x1000;
            if (frame >= 7) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = frame << 10;
            work->unk28.vx = (((0x1000 - ex->math_cos(t >> 2)) * 0x600) >> 12) - 0x200;
            sc = 0xC00;
            bright = 0x1000;
            if (frame == 3) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame >= 4) {
                work->unk0 = 2;
                entry->unk0 = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            break;
        case 2:
            morph = 0x1000;
            sc = 0xC00;
            bright = 0x1000;
            t = (frame << 12) / 6;
            work->unk28.vx = 0x400 - ((frame & 1) * 2 - 1) * (ex->math_cos(t >> 2) >> 7);
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame >= 6) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            sc = 0xC00;
            t = frame << 9;
            bright = ex->math_cos(t >> 2);
            if (frame >= 8) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (bright != 0) {
            mpos[0] = work->unk18.vx;
            mpos[1] = work->unk18.vy;
            mpos[2] = work->unk18.vz;
            scale[0] = sc;
            scale[1] = sc;
            scale[2] = sc;
            ex->Hi_DrawMorphEffModel(&work->unk28, (VECTOR *)mpos, (s32)scale, work->unk30, work->unk34, 0x1000 - morph);
            if (bright < 0x1000) {
                ex->Hi_ModifyEffModelAbr(work->unk30, 1);
                c = (bright >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk30, c, c, c);
            }
        }
    }
    return 0;
}

s32 fx_082_op81(s32 mode, Fx082Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 v[3];
    s32 frame;
    s32 a;
    s32 b;
    s32 k;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode != 1) {
        frame = entry->unk0;
        v[0] = work->unk0[0];
        v[1] = work->unk0[1];
        v[2] = work->unk0[2];
        a = (frame << 8) >> 2;
        k = (ex->math_cos(a) << 11) >> 12;
        b = frame << 9;
        v[1] -= (ex->math_sin(b) * k) >> 12;
        k = (ex->math_cos(a) << 11) >> 12;
        k = (ex->math_sin(b) * k) >> 12;
        v[0] += k;
        v[2] += k;
        ex->btlfx_setUnitScale(0, v);
        if (frame >= 0x10) {
            ex->btlfx_setUnitScale(0, work->unk0);
            return 1;
        }
    } else {
        D_801E7E64 = env;
        ex->btlfx_getUnitScale(0, (VECTOR *)work);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_082/nonmatchings/fx_082", fx_082_headerPtr);

#include "fxmod/fx_1f1.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f1_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *u;
    Pool6CEntry *emitter;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;
    s32 k;
    Fx1f1Model *mm;
    VECTOR *q;
    VECTOR *m;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        fx_1f1_env = env;
        ex->btlfx_clearUnitStatus(0x10);
        ex->btlfx_initEffModels(fx_1f1_effModels, 8);
        for (i = 15; i >= 0; i--) {
            work[i] = 0;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            u = &fx_1f1_units[i];
            u->model18 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
            u->model1C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 4), 1);
            ex->btlfx_getUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
            setVector(&fx_1f1_units[i].rot20, 0, 0, 0);
            setVector(&fx_1f1_units[i].rot28, 0, 0, 0);
            setVector(&fx_1f1_units[i].scale30, 0x1100, 0x1000, 0x1100);
            setVector(&fx_1f1_units[i].scale40, 0x1000, 0x1000, 0x1000);
            setVector(&fx_1f1_units[i].pos50, u->posC.vx, -0x800, u->posC.vz);
            k = fx_1f1_unitDelays[i];
            u->timer4 = 0x20;
            u->unk14 = 0;
            u->delay0 = k;
        }
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work[0] = 0x50;
        }
        if (work[0] != 0) {
            work[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                u = &fx_1f1_units[i];
                if (u->delay0 == 0) {
                    if (u->timer4 != 0) {
                        u->timer4--;
                        if (u->timer4 == 0x1F) {
                            ex->btlfx_getUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
                            u->height8 = u->posC.vy;
                            u->posC.vy = 0;
                            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&fx_1f1_units[i].posC);
                            if (emitter != NULL) {
                                emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x100);
                            }
                            ex->btlfx_playFileSound(6, entry->file, -1);
                            ex->btlfx_playFileSound(7, entry->file, -1);
                            ex->btlfx_playFileSound(8, entry->file, -1);
                        }
                        /* mm holds the base so q and m are computed from it (base + 0x30, + 0x10) while its last
                         * use is the &mm[i] giv: loop.c then hoists the base without dragging + 0x10 along */
                        mm = fx_1f1_unitModels;
                        q = &fx_1f1_unitModels[i].pos30;
                        m = fx_1f1_unitModels[i].scale10;
                        ex->Hi_DrawEffModel(mm[i].rot0, q, (s32)m, u->model18);
                        ex->Hi_DrawEffModel(mm[i].rot0 + 1, q, (s32)(m + 1), u->model1C);
                        if (u->timer4 >= 0x10) {
                            c = (0x10 - u->timer4) * 0x10;
                            ex->Hi_ModifyEffModelRGB(u->model18, c, c, c);
                            ex->Hi_ModifyEffModelRGB(u->model1C, c, c, c);
                            u->pos50.vy = fxm_easeLerp(2, 0x10, u->timer4 - 0x10, -0x800, 0);
                            v.vy = fxm_easeLerp(2, 0x10, u->timer4 - 0x10, u->height8, 0);
                            v.vx = u->posC.vx;
                            v.vz = u->posC.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->timer4 < 0x10) {
                            c = (u->timer4 - 0x10) * 0x10;
                            ex->Hi_ModifyEffModelRGB(u->model18, c, c, c);
                            ex->Hi_ModifyEffModelRGB(u->model1C, c, c, c);
                            ex->btlfx_fadeUnitById(i, u->timer4 * 8);
                            v.vy = u->pos50.vy = fxm_easeLerp(1, 0x10, u->timer4, 0, -0x800);
                            v.vx = u->pos50.vx;
                            v.vz = u->pos50.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->timer4 == 0) {
                            u->posC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
                            ex->btlfx_fadeUnitById(i, 0x80);
                            ex->btlfx_clearUnitStatus(i);
                        }
                        u->rot28.vy += 0x13;
                    }
                } else {
                    u->delay0--;
                }
            }
        }
        ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        ex->btlfx_drawScrollMove(frame * 4, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f1/nonmatchings/fx_1f1", fx_1f1_headerPtr);

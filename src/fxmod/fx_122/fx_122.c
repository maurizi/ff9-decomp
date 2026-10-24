#include "fxmod/fx_122.h"

s32 fx_122_op80(s32 mode, Fx122Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 id;
    s32 r;
    s32 i;
    s32 fade;
    Fx122Offset *o;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        D_801E7D44 = env;
        env->camera->unk52 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk30);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk38);
        work->unk4.vx = 0;
        work->unk4.vz = 0;
        work->unk1C.vx = 0x2000;
        work->unk1C.vy = 0x2000;
        work->unk1C.vz = 0x2000;
        work->unk4.vy = work->unk38.vy + 0x800;
        if (ex->btlfx_isEnemyUnit(0)) {
            id = ex->btlfx_getUnitHeight(0);
            r = ex->btlfx_getUnitRadius(0x100);
            o = D_801E7C94;
            for (i = 0; i < 0x11; i++, o++) {
                if (o->unk0 == id && o->unk2 == r) {
                    if (o->unk4 & 0x8000) {
                        goto forward;
                    }
                    work->unkC.vx = work->unk30.vx;
                    work->unkC.vy = work->unk30.vy - o->unk4;
                    work->unkC.vz = work->unk30.vz;
                    goto found;
                }
            }
            if (id >= 0x258 || r >= 0x2EE) {
                work->unkC.vx = work->unk30.vx;
                work->unkC.vy = work->unk30.vy - id;
                work->unkC.vz = work->unk30.vz;
                goto found;
            }
            goto unit;
        forward:
            work->unkC.vx = work->unk30.vx;
            work->unkC.vy = work->unk30.vy;
            work->unkC.vz = work->unk30.vz;
            work->unkC.vz -= o->unk4 ^ 0x8000;
            goto found;
        }
    unit:
        work->unkC.vx = work->unk30.vx;
        work->unkC.vy = work->unk30.vy;
        work->unkC.vz = work->unk30.vz;
    found:
        work->unk2E = work->unkC.vy;
        env->camera->points[0].vx = work->unkC.vx;
        env->camera->points[0].vy = work->unkC.vy;
        env->camera->points[0].vz = work->unkC.vz;
        work->unk48 = -1;
        work->unk4A = -1;
        ex->btlfx_initEffModels(D_801E7D54, 1);
        work->unk4C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
    } else {
        frame = entry->unk0;
        ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk4C);
        if (frame >= 0x19 && frame < 0x28) {
            if ((frame & 1) && (ex->btlfx_rand() & 1)) {
                work->unkC.vx = work->unk30.vx + (ex->btlfx_rand() % 4 + 4);
            } else {
                work->unkC.vx = work->unk30.vx - (ex->btlfx_rand() % 4 + 4);
            }
            work->unkC.vy = work->unk2E + ex->btlfx_rand() % 4;
        }
        switch (frame) {
        case 0:
            work->unk48 = 8;
            break;
        case 0x40:
            work->unk4A = 0x10;
            break;
        }
        fade = 0;
        if (work->unk48 >= 0) {
            work->unk2C = func_801E7C3C(-0x80, -0x40, 8 - work->unk48, 8);
            fade = 1;
            if (--work->unk48 == 0) {
                work->unk40.vx = work->unkC.vx;
                work->unk40.vy = work->unkC.vy;
                work->unk40.vz = work->unkC.vz;
                work->unk40.vy -= 0xC8;
                work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk40);
                if (work->unk0 != NULL) {
                    work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
        }
        if (work->unk4A >= 0) {
            fade = 1;
            work->unk2C = func_801E7C3C(0x28, -0x80, 0x10 - work->unk4A, 0x10);
            work->unk4A--;
        }
        if (fade) {
            ex->Hi_ModifyEffModelAbr(work->unk4C, 1);
            ex->Hi_ModifyEffModelRGB(work->unk4C, work->unk2C, work->unk2C, work->unk2C);
        }
        if (frame >= 0x4F) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7BF8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7D44)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C3C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_122/nonmatchings/fx_122", fx_122_headerPtr);

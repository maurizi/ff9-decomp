#include "fxmod/fx_0a9.h"

s32 fx_0a9_op80(s32 mode, Fx0a9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_0a9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x18);
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame >= 0x20) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

static s32 fx_0a9_op83PoolTask(s32 mode, Fx0a9Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0a9Work83 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR *pt;
    s32 c;

    if (mode == 1) {
        part->unk12 = ex->btlfx_randCentered(0x20) + 0xE00;
        part->kind17 = (ex->btlfx_rand() % 3) / 2;
    } else {
        pt = part->point18;
        pt->pad = 0x18;
        c = ex->math_cos(frame << 7);
        ((s32 *)&v)[0] = 0;
        ((s32 *)&v)[1] = 0;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)part, 0, &v, pt);
        if (part->fromEnd16 == 0) {
            ((Fx0a9Bolt *)(part->point18 - 2))->unk0 = ((Fx0a9Bolt *)(part->point18 - 2))->unk2 = 0xA;
            ((Fx0a9Bolt *)(part->point18 - 2))->unk1 = 0;
            if (part->kind17 == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 1), 0, NULL, 4, 0x1000, c, 1,
                                  (u8 *)part->point18 - 4, 0);
            } else {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), 0, NULL, 4, 0x1000, c, 1,
                                  (u8 *)part->point18 - 4, 2);
            }
        }
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

static void fxm_resetBolts(void) {
    fx_0a9_nextBolt = 0;
}

static void fxm_spawnBolt(Unk80100C78Entry *entry, SVECTOR *from, SVECTOR *to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0a9_env);
    SVECTOR pos;
    SVECTOR ang;
    s32 r;
    Fx0a9Bolt *bolt;
    SVECTOR *p;
    Fx0a9Part *part;
    s32 i;
    s32 t;
    s32 s;
    s32 z;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    i = 0;
    r = ex->btlfx_rand();
    bolt = &fx_0a9_bolts[fx_0a9_nextBolt];
    p = &bolt->pts4[10];
    bolt->unk0 = bolt->unk2 = 0xA;
    bolt->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 10;
        s = ex->math_sin(t >> 1);
        t += (s * ex->btlfx_randCentered(0x100)) >> 12;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        z = ((t * 3) >> 1) + r + ex->btlfx_randCentered(0x100);
        ex->btlfx_offsetPoint(&pos, (s32)p, 0, (XY32 *)&ang, (s * ((ex->btlfx_rand() & 0x7F) + 0x40)) >> 12, z);
        part = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (part != NULL) {
            part->bolt14 = fx_0a9_nextBolt;
            part->fromEnd16 = 0xA - i;
            part->point18 = p;
            part->pos0[0] = p->vx << 12;
            part->pos0[1] = p->vy << 12;
            part->pos0[2] = p->vz << 12;
            part->velC[0] = ((p->vx - pos.vx) >> 1) + ((ex->btlfx_randCentered(0x18) * s) >> 12);
            part->velC[1] = ((p->vy - pos.vy) >> 1) + ((ex->btlfx_randCentered(0x18) * s) >> 12);
            part->velC[2] = ((p->vz - pos.vz) >> 1) + ((ex->btlfx_randCentered(0x18) * s) >> 12);
        }
        i++;
        p--;
    } while (i < 11);
    fx_0a9_nextBolt++;
    fx_0a9_nextBolt = fx_0a9_nextBolt % 16;
}

s32 fx_0a9_op83(s32 mode, Fx0a9Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 i;

    if (mode == 0) {
        ((EffInit *)arg4)->workSize = 0x10;
        ((EffInit *)arg4)->elemSize = 0x1C;
        ((EffInit *)arg4)->elemFn = (s32)fx_0a9_op83PoolTask;
        ((EffInit *)arg4)->poolCount = 0xB0;
    } else if (mode == 1) {
        fx_0a9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        fxm_resetBolts();
        return 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        if (frame < 0x18) {
            for (i = 0; i < 2; i++) {
                BTLFX_COPY_SVEC(&v, &work->pos0);
                v.vx += ex->btlfx_randCentered(0x180);
                v.vy += ex->btlfx_randCentered(0x180);
                v.vz += ex->btlfx_randCentered(0x180);
                fxm_spawnBolt(entry, &work->pos8, &v);
            }
        }
        env->otZ = 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos8, NULL,
                          ex->math_sin((frame << 11) / 24) * 2 / 3, 0, 1, 0x1000 - ((frame & 1) << 11));
        env->otZ = 0;
        if (frame >= 0x18) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a9/nonmatchings/fx_0a9", fx_0a9_headerPtr);

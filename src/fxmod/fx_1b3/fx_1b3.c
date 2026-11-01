#include "fxmod/fx_1b3.h"

static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot) {
    Unk800F78A4 *env = fx_1b3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bright = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b3_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

/* Hi_RegisterTexListModel called with three arguments (a3 is left as it is) */
#define REGTEX(id) \
    ((s32(*)(BtlModelHeader *, BtlTexInfo *, s16 *))ex->Hi_RegisterTexListModel)( \
        (BtlModelHeader *)ex->btlfx_getFileData(entry->file, id), (BtlTexInfo *)texList, ((BtlTexInfo *)clutList)->pages[0])

#define SET_ROT(p, r)              \
    if ((p) != NULL) {             \
        (p)->rot.vx = (r).vx;    \
        (p)->rot.vy = (r).vy;    \
        (p)->rot.vz = (r).vz;    \
    }

/* Matching notes: the tex/clut lists, trail points and flash locals share three function-scope buffers (texList, clutList, sc),
 * as the original stack offsets show; the trail file buffers are carved with one incremented local (buf); the trail
 * tables are read twice instead of through a selected pointer (cross-jumped calls); the draw loop writes the drawn
 * points through a pointer biv (pt) and bounds the loop with a local (n), with the drawTrail alpha precomputed. */
s32 fx_1b3_op80(s32 mode, Fx1b3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR texList[3]; /* texture list, trail points */
    SVECTOR clutList[3]; /* clut list, trail step, flash colour and rotation */
    VECTOR sc;
    Pool6CEntry *emitter;
    Fx1b3Trail *tr;
    s32 frame;
    s32 i;
    s32 j;
    s32 c;
    s32 trailAlpha;
    s32 link;
    s32 tint;

    if (mode == 0) {
        *args = 0x264;
    } else if (mode == 1) {
        s32 buf;

        fx_1b3_env = env;
        ex->btlfx_initEffModels(fx_1b3_effModels, 0x14);
        for (i = 0; i < 32; i++) {
            work->timers0[i] = 0;
            work->emit80[i] = NULL;
        }
        buf = (s32)ex->btlfx_getFileData(entry->file, 0x93);
        fx_1b3_trails = (Fx1b3Trail *)buf;
        buf += 0xE00;
        fx_1b3_op80Trails0 = buf;
        buf += 0xF30;
        fx_1b3_op80Trails1 = buf;
        buf += 0x2448;
        fx_1b3_splitBuf = (u8 *)buf;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&fx_1b3_emitPos0);
        fx_1b3_emitPos0.vy = -0x514;
        fx_1b3_emitPos0.vz += 0x800;
        env->camera->points[1].vx = fx_1b3_emitPos0.vx;
        env->camera->points[1].vy = 0;
        env->camera->points[1].vz = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos100);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos108);
        ((BtlTexInfo *)texList)->pages[0][0] = 0x8A;
        ((BtlTexInfo *)texList)->pages[1][0] = 0x8C;
        ((BtlTexInfo *)texList)->pages[0][1] = 0x3800;
        ((BtlTexInfo *)texList)->pages[1][1] = 0x3840;
        ((BtlTexInfo *)clutList)->pages[0][0] = 0;
        ((BtlTexInfo *)clutList)->pages[1][0] = 0;
        ((BtlTexInfo *)clutList)->pages[0][1] = 0;
        ((BtlTexInfo *)clutList)->pages[1][1] = 0;
        work->model158 = REGTEX(0x81);
        ((BtlTexInfo *)texList)->pages[0][0] = 0x8E;
        ((BtlTexInfo *)texList)->pages[1][0] = 0x8F;
        ((BtlTexInfo *)texList)->pages[0][1] = 0x3900;
        ((BtlTexInfo *)texList)->pages[1][1] = 0x3980;
        ((BtlTexInfo *)texList)->pages[2][1] = 0x39C0;
        ((BtlTexInfo *)clutList)->pages[2][0] = 0x30;
        ((BtlTexInfo *)texList)->pages[2][0] = 0x8C;
        ((BtlTexInfo *)clutList)->pages[0][0] = 0;
        ((BtlTexInfo *)clutList)->pages[1][0] = 0;
        ((BtlTexInfo *)clutList)->pages[0][1] = 0;
        ((BtlTexInfo *)clutList)->pages[1][1] = 0;
        ((BtlTexInfo *)clutList)->pages[2][1] = 0x80;
        work->pos16C.vx = 0;
        work->pos16C.vy = 0;
        work->pos16C.vz = 0;
        work->rot164.vx = 0;
        work->rot164.vy = 0;
        work->rot164.vz = 0;
        work->scale17C.vx = 0x1000;
        work->scale17C.vy = 0x1000;
        work->scale17C.vz = 0x1000;
        work->rot15C.vx = 0;
        work->rot15C.vy = 0;
        work->rot15C.vz = 0;
        fx_1b3_splits0[0].model0 = REGTEX(0x85);
        fx_1b3_splits0[1].model0 = REGTEX(0x8A);
        fx_1b3_splits0[2].model0 = REGTEX(0x89);
        fx_1b3_splits0[3].model0 = REGTEX(0x8F);
        fx_1b3_splits0[4].model0 = REGTEX(0x86);
        fx_1b3_splits0[5].model0 = REGTEX(0x87);
        fx_1b3_splits0[6].model0 = REGTEX(0x83);
        fx_1b3_splits0[7].model0 = REGTEX(0x90);
        fx_1b3_splits0[8].model0 = REGTEX(0x92);
        fx_1b3_splits0[5].timer4 = 0x40;
        fx_1b3_splits0[4].timer4 = 0x40;
        fx_1b3_splits0[3].timer4 = 0x40;
        fx_1b3_splits0[2].timer4 = 0x40;
        fx_1b3_splits0[1].timer4 = 0x40;
        fx_1b3_splits0[0].timer4 = 0x40;
        fx_1b3_splits0[8].timer4 = 0x64;
        fx_1b3_splits0[7].timer4 = 0x64;
        fx_1b3_splits0[6].timer4 = 0x64;
        ex->btlfx_getCasterTexInfo(&fx_1b3_casterTexInfo, &fx_1b3_casterTexPos, 0x2A0);
        ((BtlTexInfo *)texList)->pages[0][0] = fx_1b3_casterTexInfo.pages[5][0];
        ((BtlTexInfo *)texList)->pages[0][1] = fx_1b3_casterTexInfo.pages[5][1];
        ((BtlTexInfo *)clutList)->pages[0][0] = fx_1b3_casterTexPos.pages[5][0];
        ((BtlTexInfo *)clutList)->pages[0][1] = fx_1b3_casterTexPos.pages[5][1];
        fx_1b3_splits1[0].model0 = REGTEX(0x91);
        fx_1b3_splits1[1].model0 = REGTEX(0x8D);
        fx_1b3_splits1[1].timer4 = 0x80;
        fx_1b3_splits1[0].timer4 = 0x80;
        work->pos19C.vx = 0;
        work->pos19C.vy = 0;
        work->pos19C.vz = 0;
        work->rot18C.vx = 0;
        work->rot18C.vy = 0;
        work->rot18C.vz = 0;
        work->scale1BC.vx = 0x1000;
        work->scale1BC.vy = 0x1000;
        work->scale1BC.vz = 0x1000;
        ((BtlTexInfo *)texList)->pages[0][0] = fx_1b3_casterTexInfo.pages[0][0];
        ((BtlTexInfo *)texList)->pages[0][1] = fx_1b3_casterTexInfo.pages[0][1];
        ((BtlTexInfo *)clutList)->pages[0][0] = fx_1b3_casterTexPos.pages[0][0];
        ((BtlTexInfo *)clutList)->pages[0][1] = fx_1b3_casterTexPos.pages[0][1];
        fx_1b3_splits2.model0 = REGTEX(0x8B);
        fx_1b3_splits2.timer4 = 0x80;
        work->pos1D4.vx = 0;
        work->pos1D4.vy = 0;
        work->pos1D4.vz = 0;
        work->rot1CC.vx = 0;
        work->rot1CC.vy = 0;
        work->rot1CC.vz = 0;
        work->scale1E4.vx = 0x1000;
        work->scale1E4.vy = 0x1000;
        work->scale1E4.vz = 0x1000;
        ((BtlTexInfo *)texList)->pages[0][0] = fx_1b3_casterTexInfo.pages[3][0];
        ((BtlTexInfo *)texList)->pages[0][1] = fx_1b3_casterTexInfo.pages[3][1];
        ((BtlTexInfo *)clutList)->pages[0][0] = fx_1b3_casterTexPos.pages[3][0];
        ((BtlTexInfo *)clutList)->pages[0][1] = fx_1b3_casterTexPos.pages[3][1];
        ((BtlTexInfo *)texList)->pages[1][0] = fx_1b3_casterTexInfo.pages[1][0];
        ((BtlTexInfo *)texList)->pages[1][1] = fx_1b3_casterTexInfo.pages[1][1];
        ((BtlTexInfo *)clutList)->pages[1][0] = fx_1b3_casterTexPos.pages[1][0];
        ((BtlTexInfo *)clutList)->pages[1][1] = fx_1b3_casterTexPos.pages[1][1];
        fx_1b3_splits3[0].model0 = REGTEX(0x8E);
        fx_1b3_splits3[1].model0 = REGTEX(0x8C);
        work->rot1FC.vx = 0;
        work->rot1FC.vy = 0;
        work->rot1FC.vz = 0;
        work->rot204.vx = 0;
        work->rot204.vy = 0;
        work->rot204.vz = 0;
        work->scale22C.vx = 0x1000;
        work->scale22C.vy = 0x1000;
        work->scale22C.vz = 0x1000;
        ((BtlTexInfo *)texList)->pages[0][0] = fx_1b3_casterTexInfo.pages[3][0];
        ((BtlTexInfo *)texList)->pages[0][1] = fx_1b3_casterTexInfo.pages[3][1];
        ((BtlTexInfo *)clutList)->pages[0][0] = fx_1b3_casterTexPos.pages[3][0];
        ((BtlTexInfo *)clutList)->pages[0][1] = fx_1b3_casterTexPos.pages[3][1];
        ((BtlTexInfo *)texList)->pages[1][0] = fx_1b3_casterTexInfo.pages[2][0];
        ((BtlTexInfo *)texList)->pages[1][1] = fx_1b3_casterTexInfo.pages[2][1];
        ((BtlTexInfo *)clutList)->pages[1][0] = fx_1b3_casterTexPos.pages[2][0];
        ((BtlTexInfo *)clutList)->pages[1][1] = fx_1b3_casterTexPos.pages[2][1];
        ((BtlTexInfo *)texList)->pages[2][0] = fx_1b3_casterTexInfo.pages[0][0];
        ((BtlTexInfo *)texList)->pages[2][1] = fx_1b3_casterTexInfo.pages[0][1];
        ((BtlTexInfo *)clutList)->pages[2][0] = fx_1b3_casterTexPos.pages[0][0];
        ((BtlTexInfo *)clutList)->pages[2][1] = fx_1b3_casterTexPos.pages[0][1];
        fx_1b3_splits4[0].model0 = REGTEX(0x88);
        fx_1b3_splits4[1].model0 = REGTEX(0x82);
        work->rot23C.vx = 0;
        work->rot23C.vy = 0;
        work->rot23C.vz = 0;
        work->scale254.vx = 0x1000;
        work->scale254.vy = 0x1000;
        work->scale254.vz = 0x1000;
        work->pos244.vx = work->pos108.vx;
        work->pos244.vy = work->pos108.vy;
        work->pos244.vz = work->pos108.vz;
        ((BtlTexInfo *)texList)->pages[0][0] = fx_1b3_casterTexInfo.pages[4][0];
        ((BtlTexInfo *)texList)->pages[0][1] = fx_1b3_casterTexInfo.pages[4][1];
        ((BtlTexInfo *)clutList)->pages[0][0] = fx_1b3_casterTexPos.pages[4][0];
        ((BtlTexInfo *)clutList)->pages[0][1] = fx_1b3_casterTexPos.pages[4][1];
        ((BtlTexInfo *)texList)->pages[1][0] = fx_1b3_casterTexInfo.pages[0][0];
        ((BtlTexInfo *)texList)->pages[1][1] = fx_1b3_casterTexInfo.pages[0][1];
        ((BtlTexInfo *)clutList)->pages[1][0] = fx_1b3_casterTexPos.pages[0][0];
        ((BtlTexInfo *)clutList)->pages[1][1] = fx_1b3_casterTexPos.pages[0][1];
        fx_1b3_splits4[2].model0 = REGTEX(0x80);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_signalScriptCue();
            work->timers0[0] = 0x10;
            break;
        case 8:
            emitter = work->emit80[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            emitter = work->emit80[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x10:
            work->timers0[1] = 0x90;
            emitter = work->emit80[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x30:
            work->timers0[1] = 0;
            work->timers0[2] = 0x40;
            work->timers0[3] = 0;
            for (i = 0; i < 16; i++) {
                fx_1b3_trails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1b3_trails[i], 0);
            }
            emitter = work->emit80[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->trails = fx_1b3_op80Trails0;
                work->emit80[3]->trailSlots = 0x24;
                work->emit80[3]->trailStride = 0x6C;
                emitter = work->emit80[3];
                if (emitter != NULL) {
                    emitter->otZ = 0x400;
                }
            }
            break;
        case 0x60:
            work->timers0[2] = 0;
            ex->btlfx_stopEmitter(work->emit80[0]);
            ex->btlfx_stopEmitter(work->emit80[1]);
            ex->btlfx_stopEmitter(work->emit80[2]);
            ex->btlfx_stopEmitter(work->emit80[3]);
            work->timers0[13] = 0x20;
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            for (i = 0; i < 3; i++) {
                fx_1b3_splitModel(&fx_1b3_splits0[i]);
            }
            fx_1b3_splitTrigger.mode0 = 0;
            fx_1b3_splitTrigger.angle10 = 0;
            fx_1b3_splitTrigger.spin14 = -0xA;
            work->timers0[4] = 0x3C;
            work->timers0[5] = 0;
            emitter = work->emit80[16] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            emitter = work->emit80[17] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x9C:
            work->timers0[4] = 0;
            ex->btlfx_stopEmitter(work->emit80[16]);
            ex->btlfx_stopEmitter(work->emit80[17]);
            work->timers0[13] = 0x20;
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            for (i = 0; i < 3; i++) {
                fx_1b3_splitModel(&fx_1b3_splits0[3 + i]);
            }
            fx_1b3_splitTrigger.mode0 = 0;
            fx_1b3_splitTrigger.angle10 = -0x800;
            fx_1b3_splitTrigger.spin14 = 0xA;
            work->timers0[4] = 0x3C;
            work->timers0[5] = 3;
            emitter = work->emit80[18] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            emitter = work->emit80[19] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0xD8:
            work->timers0[4] = 0;
            ex->btlfx_stopEmitter(work->emit80[18]);
            ex->btlfx_stopEmitter(work->emit80[19]);
            work->timers0[13] = 0x20;
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            for (i = 0; i < 3; i++) {
                fx_1b3_splitModel(&fx_1b3_splits0[6 + i]);
            }
            fx_1b3_splitTrigger.mode0 = 1;
            fx_1b3_splitTrigger.angle10 = 0;
            fx_1b3_splitTrigger.spin14 = 0xE;
            work->timers0[4] = 0x4B;
            work->timers0[5] = 6;
            emitter = work->emit80[20] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            emitter = work->emit80[21] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos110);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0xFE:
            work->timers0[2] = 0x48;
            work->timers0[3] = 1;
            for (i = 0; i < 16; i++) {
                fx_1b3_trails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1b3_trails[i], 0);
            }
            break;
        case 0x106:
            ex->btlfx_getUnitBonePos(0x10, 0x3A, (u16 *)&work->pos118);
            emitter = work->emit80[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos118);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x11E:
            work->timers0[4] = 0;
            ex->btlfx_stopEmitter(work->emit80[20]);
            ex->btlfx_stopEmitter(work->emit80[21]);
            emitter = work->emit80[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos118);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            emitter = work->emit80[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos118);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x11F:
            ex->btlfx_stopEmitter(work->emit80[4]);
            work->timers0[11] = 0x10;
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            fx_1b3_splitModel(&fx_1b3_splits1[0]);
            work->pos19C.vx = work->pos108.vx;
            work->pos19C.vy = work->pos108.vy;
            work->pos19C.vz = work->pos108.vz;
            fx_1b3_splitTrigger.posY6 = 0x990;
            fx_1b3_splitTrigger.posX4 = 0x3CF;
            fx_1b3_splitTrigger.posZ8 = 0;
            work->timers0[6] = 0x80;
            break;
        case 0x136:
            ex->btlfx_getUnitBonePos(0x10, 0x3A, (u16 *)&work->pos118);
            emitter = work->emit80[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos118);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            work->emit80[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos118);
            emitter = work->emit80[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x3A), (s32)&work->pos118);
            if (emitter != NULL) {
                emitter->trails = fx_1b3_op80Trails1;
                work->emit80[9]->trailSlots = 0x36;
                work->emit80[9]->trailStride = 0xAC;
            }
            work->rot130.vy = 0x800;
            work->rot128.vx = 0;
            work->rot128.vy = 0;
            work->rot128.vz = 0;
            work->rot130.vx = 0;
            work->rot130.vz = 0;
            work->rot138.vx = 0;
            work->rot138.vy = 0x400;
            work->rot138.vz = 0;
            break;
        case 0x14F:
            work->timers0[2] = 0x48;
            work->timers0[3] = 2;
            for (i = 0; i < 16; i++) {
                fx_1b3_trails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1b3_trails[i], 0);
            }
            break;
        case 0x157:
            ex->btlfx_getUnitBonePos(0x10, 0x18, (u16 *)&work->pos120);
            emitter = work->emit80[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x16F:
            work->timers0[6] = 0;
            ex->btlfx_stopEmitter(work->emit80[4]);
            ex->btlfx_stopEmitter(work->emit80[5]);
            ex->btlfx_stopEmitter(work->emit80[6]);
            emitter = work->emit80[11] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            emitter = work->emit80[12] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x170:
            ex->btlfx_stopEmitter(work->emit80[10]);
            work->timers0[11] = 0x10;
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            fx_1b3_splitModel(&fx_1b3_splits1[1]);
            fx_1b3_splitModel(&fx_1b3_splits2);
            work->timers0[7] = 0x80;
            work->pos1AC.vx = work->pos108.vx;
            work->pos1AC.vy = work->pos108.vy;
            work->pos1AC.vz = work->pos108.vz;
            work->pos1D4.vx = work->pos108.vx;
            work->pos1D4.vz = work->pos108.vz;
            work->pos1D4.vy = work->pos108.vy;
            break;
        case 0x187:
            ex->btlfx_stopEmitter(work->emit80[7]);
            ex->btlfx_stopEmitter(work->emit80[8]);
            ex->btlfx_stopEmitter(work->emit80[9]);
            ex->btlfx_getUnitBonePos(0x10, 0x18, (u16 *)&work->pos120);
            emitter = work->emit80[13] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            work->emit80[14] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos120);
            emitter = work->emit80[15] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x39), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->trails = fx_1b3_op80Trails1;
                work->emit80[15]->trailSlots = 0x36;
                work->emit80[15]->trailStride = 0xAC;
            }
            work->rot148.vx = -0x600;
            work->rot148.vy = -0x400;
            work->rot140.vx = 0;
            work->rot140.vy = 0;
            work->rot140.vz = 0;
            work->rot148.vz = 0;
            work->rot150.vx = 0;
            work->rot150.vy = 0xC00;
            work->rot150.vz = 0;
            break;
        case 0x1A8:
            work->timers0[2] = 0x30;
            work->timers0[3] = 3;
            for (i = 0; i < 16; i++) {
                fx_1b3_trails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1b3_trails[i], 0);
            }
            break;
        case 0x1C0:
            work->timers0[7] = 0;
            ex->btlfx_getUnitBonePos(0x10, 0x18, (u16 *)&work->pos120);
            ex->btlfx_stopEmitter(work->emit80[10]);
            ex->btlfx_stopEmitter(work->emit80[11]);
            ex->btlfx_stopEmitter(work->emit80[12]);
            fx_1b3_splitBufPtr = fx_1b3_splitBuf;
            fx_1b3_splitModel(&fx_1b3_splits3[0]);
            fx_1b3_splitModel(&fx_1b3_splits3[1]);
            work->timers0[8] = 0x3C;
            work->pos20C.vx = work->pos108.vx;
            work->pos20C.vy = work->pos108.vy;
            work->pos20C.vz = work->pos108.vz;
            work->pos21C.vx = work->pos108.vx;
            work->pos21C.vy = work->pos108.vy;
            work->pos21C.vz = work->pos108.vz;
            break;
        case 0x1C1:
            ex->btlfx_getUnitBonePos(0x10, 3, (u16 *)&work->pos1F4);
            work->pos1F4.vx += 0x100;
            work->pos1F4.vy -= 0x80;
            emitter = work->emit80[22] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos1F4);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            emitter = work->emit80[23] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos1F4);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0x1C2:
            work->timers0[11] = 0x10;
            work->timers0[9] = 0x22;
            break;
        case 0x1E2:
            for (i = 0; i < 3; i++) {
                fx_1b3_splitModel(&fx_1b3_splits4[i]);
            }
            break;
        case 0x1E3:
            emitter = work->emit80[24] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->trails = fx_1b3_op80Trails0;
                work->emit80[24]->trailSlots = 0x24;
                work->emit80[24]->trailStride = 0x6C;
                emitter = work->emit80[24];
                if (emitter != NULL) {
                    emitter->otZ = 0x400;
                }
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&fx_1b3_emitPos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&fx_1b3_emitPos0);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&fx_1b3_emitPos0);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case 0x1E4:
            work->timers0[8] = 0;
            work->timers0[10] = 0xE0;
            break;
        case 0x1E5:
            ex->btlfx_fadeUnitById(0x10, 0);
            break;
        case 0x264:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&fx_1b3_emitPos0);
            break;
        case 0x27C:
            work->timers0[12] = 0x20;
            break;
        case 0x29C:
            ex->btlfx_signalScriptCue();
            return 1;
        }
        for (i = 0; i < 32; i++) {
            if (work->emit80[i] != NULL && work->emit80[i]->state == BTLFX_EMIT_DONE) {
                work->emit80[i] = NULL;
            }
        }
        if (work->emit80[8] != NULL) {
            work->rot128.vx += 0x80;
            work->rot128.vy += 0x80;
            SET_ROT(work->emit80[8], work->rot128);
        }
        SET_ROT(work->emit80[9], work->rot130);
        SET_ROT(work->emit80[7], work->rot138);
        if (work->emit80[14] != NULL) {
            work->rot140.vx += 0x80;
            work->rot140.vy += 0x80;
            SET_ROT(work->emit80[14], work->rot140);
        }
        SET_ROT(work->emit80[15], work->rot148);
        SET_ROT(work->emit80[13], work->rot150);
        if (work->emit80[9] != NULL || work->emit80[15] != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), frame);
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            tint = ex->btlfx_lerpRatio(0x40, 0, work->timers0[0], 0x10);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, tint, tint, tint);
        }
        if (work->timers0[12] != 0) {
            work->timers0[12]--;
            tint = ex->btlfx_lerpRatio(0xFF, 0, work->timers0[12], 0x20);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, tint, tint, tint);
        }
        if (work->timers0[1] != 0) {
            work->timers0[1]--;
            tint = 0x40;
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, tint, tint, tint);
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            switch (work->timers0[3]) {
            case 0:
                for (i = 0; i < 16; i++) {
                    if (fx_1b3_trails[i].timer4C == 0 && work->timers0[2] >= 0xD) {

                        ex->btlfx_getUnitBonePos(0x10, fx_1b3_trailBones0[ex->btlfx_rand() & 0xF], (u16 *)&texList[0]);
                        fx_1b3_trails[i].timer4C = (ex->btlfx_rand() & 7) + 4;
                        clutList[0].vx = ex->btlfx_randRange(-0x100, 0x100);
                        clutList[0].vy = ex->btlfx_randRange(-0x100, 0x100);
                        clutList[0].vz = -(ex->btlfx_rand() % 255) - 0x200;
                        for (j = 0; j < 9; j++) {
                            texList[0].vx += clutList[0].vx;
                            texList[0].vy += clutList[0].vy;
                            texList[0].vz += clutList[0].vz;
                            fx_1b3_trails[i].pos50[j].vx = texList[0].vx;
                            fx_1b3_trails[i].pos50[j].vy = texList[0].vy;
                            fx_1b3_trails[i].pos50[j].vz = texList[0].vz;
                            fx_1b3_trails[i].vel98[j].vx = ex->btlfx_randRange(-0x60, 0x60);
                            fx_1b3_trails[i].vel98[j].vy = ex->btlfx_randRange(-0x60, 0x60);
                            fx_1b3_trails[i].vel98[j].vz = 0;
                        }
                    }
                }
                break;
            case 1:
            case 2:
                for (i = 0; i < 5; i++) {
                    if (fx_1b3_trails[i].timer4C == 0 && work->timers0[2] >= 0xD) {

                        fx_1b3_trails[i].timer4C = (ex->btlfx_rand() & 3) + 4;
                        for (link = 0; link < 4; link++) {

                            if (work->timers0[3] == 1) {
                                ex->btlfx_getUnitBonePos(0x10, fx_1b3_trailBones1[link], (u16 *)&texList[1]);
                                ex->btlfx_getUnitBonePos(0x10, fx_1b3_trailBones1[link + 1], (u16 *)&texList[2]);
                            } else {
                                ex->btlfx_getUnitBonePos(0x10, fx_1b3_trailBones2[link], (u16 *)&texList[1]);
                                ex->btlfx_getUnitBonePos(0x10, fx_1b3_trailBones2[link + 1], (u16 *)&texList[2]);
                            }
                            texList[2].vx += ex->btlfx_randRange(-0x100, 0x100);
                            texList[2].vy += ex->btlfx_randRange(-0x100, 0x100);
                            texList[2].vz += ex->btlfx_randRange(-0x100, 0x100);
                            texList[2].vx = (s16)(texList[2].vx - texList[1].vx) >> 2;
                            texList[2].vy = (s16)(texList[2].vy - texList[1].vy) >> 2;
                            texList[2].vz = (s16)(texList[2].vz - texList[1].vz) >> 2;
                            for (c = 0; c < 2; c++) {
                                fx_1b3_trails[i].pos50[link * 2 + c].vx = texList[1].vx;
                                fx_1b3_trails[i].pos50[link * 2 + c].vy = texList[1].vy;
                                fx_1b3_trails[i].pos50[link * 2 + c].vz = texList[1].vz;
                                fx_1b3_trails[i].vel98[link * 2 + c].vx = ex->btlfx_randRange(-0x20, 0x20);
                                fx_1b3_trails[i].vel98[link * 2 + c].vz = ex->btlfx_randRange(-0x20, 0x20);
                                fx_1b3_trails[i].vel98[link * 2 + c].vy = ex->btlfx_randRange(-4, 4);
                                texList[1].vx += texList[2].vx;
                                texList[1].vy += texList[2].vy;
                                texList[1].vz += texList[2].vz;
                            }
                            fx_1b3_trails[i].vel98[8].vx = 0;
                            fx_1b3_trails[i].vel98[8].vz = 0;
                            fx_1b3_trails[i].vel98[8].vy = 0;
                            fx_1b3_trails[i].pos50[8].vx = fx_1b3_trails[i].pos50[7].vx;
                            fx_1b3_trails[i].pos50[8].vy = fx_1b3_trails[i].pos50[7].vy;
                            fx_1b3_trails[i].pos50[8].vz = fx_1b3_trails[i].pos50[7].vz;
                        }
                    }
                }
                break;
            case 3:
                for (i = 0; i < 6; i++) {
                    if (fx_1b3_trails[i].timer4C == 0 && work->timers0[2] >= 0xD) {
                        s32 ang;

                        fx_1b3_trails[i].timer4C = (ex->btlfx_rand() & 3) + 4;
                        if (i & 1) {
                            ex->btlfx_getUnitBonePos(0x10, 3, (u16 *)&texList[0]);
                            c = 0x200;
                        } else {
                            ex->btlfx_getUnitBonePos(0x10, 9, (u16 *)&texList[0]);
                            c = 0x180;
                        }
                        ang = ex->btlfx_rand() & 0xFFF;
                        for (link = 0; link < 8; link++) {
                            fx_1b3_trails[i].pos50[link].vx = texList[0].vx + ((ex->math_sin(ang) * (c + (ex->btlfx_rand() & 0x7F))) >> 12);
                            fx_1b3_trails[i].pos50[link].vz = texList[0].vz + ((ex->math_cos(ang) * (c + (ex->btlfx_rand() & 0x7F))) >> 12);
                            fx_1b3_trails[i].pos50[link].vy = texList[0].vy + ex->btlfx_randRange(-0x40, 0x40);
                            fx_1b3_trails[i].vel98[link].vx = ex->btlfx_randRange(-0x20, 0x20);
                            fx_1b3_trails[i].vel98[link].vz = ex->btlfx_randRange(-0x20, 0x20);
                            fx_1b3_trails[i].vel98[link].vy = ex->btlfx_randRange(-0x20, 0x20);
                            ang += 0x100;
                        }
                        fx_1b3_trails[i].vel98[8].vx = 0;
                        fx_1b3_trails[i].vel98[8].vz = 0;
                        fx_1b3_trails[i].vel98[8].vy = 0;
                        fx_1b3_trails[i].pos50[8].vx = fx_1b3_trails[i].pos50[0].vx;
                        fx_1b3_trails[i].pos50[8].vy = fx_1b3_trails[i].pos50[0].vy;
                        fx_1b3_trails[i].pos50[8].vz = fx_1b3_trails[i].pos50[0].vz;
                    }
                }
                break;
            }
            env->otZ = 0x40;
            for (i = 0; i < 16; i++) {
                if (fx_1b3_trails[i].timer4C != 0) {
                    tr = &fx_1b3_trails[i];
                    tr->timer4C--;
                    if (tr->timer4C < 4) {
                        s32 n;
                        SVECTOR *pt;

                        tr->count2 = 8;
                        tr->lastSlot0 = 8;
                        tr->head1 = 0;
                        n = 9;
                        for (pt = tr->points4, j = 0; j < n; j++, pt++) {
                            fx_1b3_trails[i].pos50[j].vx += fx_1b3_trails[i].vel98[j].vx;
                            fx_1b3_trails[i].pos50[j].vy += fx_1b3_trails[i].vel98[j].vy;
                            fx_1b3_trails[i].pos50[j].vz += fx_1b3_trails[i].vel98[j].vz;
                            pt->vx = fx_1b3_trails[i].pos50[j].vx;
                            pt->vy = fx_1b3_trails[i].pos50[j].vy;
                            pt->vz = fx_1b3_trails[i].pos50[j].vz;
                            pt->pad = 0x80;
                        }
                        trailAlpha = fx_1b3_trails[i].timer4C << 10;
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0, NULL, frame, 0x1000,
                                            trailAlpha, 1, (u8 *)&fx_1b3_trails[i], 0);
                    }
                }
            }
            env->otZ = 0;
        }
        if (work->timers0[4] != 0) {
            work->timers0[4]--;
            fx_1b3_splitTrigger.radiusC = 0x80;
            fx_1b3_splitTrigger.posX4 = (u32)(ex->math_sin(fx_1b3_splitTrigger.angle10) * 0x341) >> 9;
            fx_1b3_splitTrigger.posZ8 = -(ex->math_cos(fx_1b3_splitTrigger.angle10) * 0x1A08) >> 12;
            fx_1b3_splitTrigger.posY6 = 0;
            fx_1b3_splitTrigger.angle10 += fx_1b3_splitTrigger.spin14;
            work->pos110.vx = fx_1b3_splitTrigger.posX4;
            work->pos110.vy = fx_1b3_splitTrigger.posY6;
            work->pos110.vz = -fx_1b3_splitTrigger.posZ8;
            for (i = work->timers0[5]; i < work->timers0[5] + 3; i++) {
                if (fx_1b3_splits0[i].timer4 != 0) {
                    fx_1b3_splits0[i].timer4--;
                    fx_1b3_updateSplitPieces(&fx_1b3_splits0[i]);
                    ex->Hi_DrawEffModel(&work->rot164, &work->pos16C, (s32)&work->scale17C, fx_1b3_splits0[i].model0);
                }
            }
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            fx_1b3_splitTrigger.mode0 = 2;
            fx_1b3_splitTrigger.radiusC = 0x80;
            fx_1b3_splitTrigger.posY6 -= 0x80;
            if (fx_1b3_splits1[0].timer4 != 0) {
                fx_1b3_splits1[0].timer4--;
                fx_1b3_updateSplitPieces(&fx_1b3_splits1[0]);
                ex->Hi_DrawEffModel(&work->rot18C, &work->pos19C, (s32)&work->scale1BC, fx_1b3_splits1[0].model0);
            }
        }
        if (work->timers0[7] != 0) {
            work->timers0[7]--;
            fx_1b3_splitTrigger.mode0 = 2;
            fx_1b3_splitTrigger.radiusC = 0x80;
            fx_1b3_splitTrigger.posY6 = 0x990 - (0x80 - work->timers0[7]) * 0x80;
            fx_1b3_splitTrigger.posX4 = -0x3CF;
            fx_1b3_splitTrigger.posZ8 = 0;
            if (fx_1b3_splits1[1].timer4 != 0) {
                fx_1b3_splits1[1].timer4--;
                fx_1b3_updateSplitPieces(&fx_1b3_splits1[1]);
                ex->Hi_DrawEffModel(&work->rot194, &work->pos1AC, (s32)&work->scale1BC, fx_1b3_splits1[1].model0);
            }
            fx_1b3_splitTrigger.mode0 = 3;
            fx_1b3_splitTrigger.radiusC = 0x100;
            fx_1b3_splitTrigger.posX4 = (0x80 - work->timers0[7]) * 0x80 - 0x600;
            fx_1b3_splitTrigger.posY6 = 0;
            fx_1b3_splitTrigger.posZ8 = 0;
            if (fx_1b3_splits2.timer4 != 0) {
                fx_1b3_splits2.timer4--;
                fx_1b3_updateSplitPieces(&fx_1b3_splits2);
                ex->Hi_DrawEffModel(&work->rot1CC, &work->pos1D4, (s32)&work->scale1E4, fx_1b3_splits2.model0);
            }
        }
        if (work->timers0[8] != 0) {
            work->timers0[8]--;
            if (work->timers0[9] != 0) {
                work->timers0[9]--;
                fx_1b3_splitTrigger.mode0 = 4;
                fx_1b3_splitTrigger.radiusC = 0x100;
                fx_1b3_splitTrigger.posY6 = 0x331;
                fx_1b3_splitTrigger.posX4 = 0x2BC - (0x22 - work->timers0[9]) * 0x100;
                fx_1b3_splitTrigger.posZ8 = 0;
                fx_1b3_updateSplitPieces(&fx_1b3_splits3[0]);
                fx_1b3_updateSplitPieces(&fx_1b3_splits3[1]);
            }
            ex->Hi_DrawEffModel(&work->rot1FC, &work->pos20C, (s32)&work->scale22C, fx_1b3_splits3[0].model0);
            ex->Hi_DrawEffModel(&work->rot204, &work->pos21C, (s32)&work->scale22C, fx_1b3_splits3[1].model0);
        }
        if (work->timers0[10] != 0) {
            work->timers0[10]--;
            fx_1b3_splitTrigger.mode0 = 5;
            if (work->timers0[10] < 0x80) {
                fx_1b3_splitTrigger.radiusC = 0;
            } else {
                fx_1b3_splitTrigger.radiusC = (work->timers0[10] - 0x80) * 16;
            }
            fx_1b3_splitTrigger.posY6 = 0x514;
            fx_1b3_splitTrigger.posX4 = 0;
            fx_1b3_splitTrigger.posZ8 = 0x800;
            for (i = 0; i < 3; i++) {
                fx_1b3_updateSplitPieces(&fx_1b3_splits4[i]);
                ex->Hi_DrawEffModel(&work->rot23C, &work->pos244, (s32)&work->scale254, fx_1b3_splits4[i].model0);
            }
        }
        if (work->timers0[13] != 0) {
            work->timers0[13]--;
            switch (work->timers0[13]) {
            case 0x1F:
                ex->btlfx_drawScrollMove(0, 0x380, 0, 0x28, 0x50, 0x380, 0x50);
                break;
            case 0x17:
                ex->btlfx_drawScrollMove(0, 0x380, 0, 0x28, 0x50, 0x380, 0xA0);
                break;
            case 0x13:
                ex->btlfx_drawScrollMove(0, 0x380, 0, 0x28, 0x50, 0x3A8, 0);
                break;
            case 0xF:
                ex->btlfx_drawScrollMove(0, 0x380, 0, 0x28, 0x50, 0x3A8, 0x50);
                break;
            }
        }
        if (work->timers0[11] != 0) {

            work->timers0[11]--;
            ((CVECTOR *)&clutList[0])->r = ((CVECTOR *)&clutList[0])->g = ((CVECTOR *)&clutList[0])->b = work->timers0[11] * 7;
            sc.vz = sc.vy = sc.vx = work->timers0[11] * 10 + 0x1000;
            clutList[1].vx = 0;
            clutList[1].vy = 0;
            clutList[1].vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &clutList[1], &sc, (CVECTOR *)&clutList[0], 1, NULL);
        }
        if (frame >= 0x61) {
            work->rot15C.vy++;
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos16C, (s32)&work->scale17C, work->model158);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_headerPtr);

void fx_1b3_splitModel(Fx1b3Split *sp) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b3_env);
    SVECTOR *vert;
    Fx1b3Piece *piece;
    s32 i;
    s32 j;

    ex->Hi_SplitMdlVertex(sp->model0, -1, (s32)fx_1b3_splitBufPtr);
    for (i = 0; i < 8; i++) {
        sp->count8[i] = ex->Hi_GetMdlVertexPtr(sp->model0, 0, (s32)fx_1b3_splitBufPtr, (s32 *)&sp->verts28[i], i);
    }
    fx_1b3_splitBufPtr += ex->Hi_GetSplitMdlVertex(sp->model0, -1);
    for (i = 0; i < 8; i++) {
        sp->pieces48[i] = (Fx1b3Piece *)fx_1b3_splitBufPtr;
        fx_1b3_splitBufPtr += sp->count8[i] * 0x18;
    }
    for (i = 0; i < 8; i++) {
        piece = sp->pieces48[i];
        vert = sp->verts28[i];
        switch (i) {
        case 0:
        case 2:
        case 4:
        case 6:
            for (j = 0; j < sp->count8[i]; j++) {
                piece->timer6 = -1;
                piece->posX10 = vert[0].vx;
                piece->posX10 += vert[1].vx;
                piece->posX10 += vert[2].vx;
                piece->posX10 = (s16)(piece->posX10 + vert[3].vx) >> 2;
                piece->posY12 = vert[0].vy;
                piece->posY12 += vert[1].vy;
                piece->posY12 += vert[2].vy;
                piece->posY12 = (s16)(piece->posY12 + vert[3].vy) >> 2;
                piece->posZ14 = vert[0].vz;
                piece->posZ14 += vert[1].vz;
                piece->posZ14 += vert[2].vz;
                piece->posZ14 = (s16)(piece->posZ14 + vert[3].vz) >> 2;
                piece->rot8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece++;
                vert += 4;
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            for (j = 0; j < sp->count8[i]; j++) {
                piece->timer6 = -1;
                piece->posX10 = vert[0].vx;
                piece->posX10 += vert[1].vx;
                piece->posX10 = (s16)(piece->posX10 + vert[2].vx) / 3;
                piece->posY12 = vert[0].vy;
                piece->posY12 += vert[1].vy;
                piece->posY12 = (s16)(piece->posY12 + vert[2].vy) / 3;
                piece->posZ14 = vert[0].vz;
                piece->posZ14 += vert[1].vz;
                piece->posZ14 = (s16)(piece->posZ14 + vert[2].vz) / 3;
                piece->rot8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                piece++;
                vert += 3;
            }
            break;
        }
    }
}

void fx_1b3_updateSplitPieces(Fx1b3Split *sp) {
    SVECTOR rel[4];
    VECTOR out;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b3_env);
    Fx1b3Piece *piece;
    SVECTOR *vert;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 dz;

    for (i = 0; i < 8; i++) {
        vert = sp->verts28[i];
        piece = sp->pieces48[i];
        for (j = 0; j < sp->count8[i]; j++) {
            if (piece->timer6 > 0) {
                ex->math_rotMatrixYXZ(&piece->rot8, &m);
                m.t[0] = piece->posX10 + piece->velX0;
                m.t[1] = piece->posY12 + piece->velY2;
                m.t[2] = piece->posZ14 + piece->velZ4;
                scale.vx = scale.vy = scale.vz = piece->scale16;
                piece->scale16 -= piece->scaleStepE;
                ex->ScaleMatrix(&m, &scale);
                gte_SetRotMatrix(&m);
                gte_SetTransMatrix(&m);
                rel[0].vx = vert[0].vx - piece->posX10;
                rel[0].vy = vert[0].vy - piece->posY12;
                rel[0].vz = vert[0].vz - piece->posZ14;
                gte_ldv0(&rel[0]);
                gte_rtv0tr();
                rel[1].vx = vert[1].vx - piece->posX10;
                rel[1].vy = vert[1].vy - piece->posY12;
                rel[1].vz = vert[1].vz - piece->posZ14;
                gte_stlvnl(&out);
                gte_ldv0(&rel[1]);
                gte_rtv0tr();
                vert[0].vx = out.vx;
                vert[0].vy = out.vy;
                vert[0].vz = out.vz;
                rel[2].vx = vert[2].vx - piece->posX10;
                rel[2].vy = vert[2].vy - piece->posY12;
                rel[2].vz = vert[2].vz - piece->posZ14;
                gte_stlvnl(&out);
                gte_ldv0(&rel[2]);
                gte_rtv0tr();
                vert[1].vx = out.vx;
                vert[1].vy = out.vy;
                vert[1].vz = out.vz;
                piece->timer6--;
                gte_stlvnl(&out);
                vert[2].vx = out.vx;
                vert[2].vy = out.vy;
                vert[2].vz = out.vz;
                if (!(i & 1)) {
                    rel[3].vx = vert[3].vx - piece->posX10;
                    rel[3].vy = vert[3].vy - piece->posY12;
                    rel[3].vz = vert[3].vz - piece->posZ14;
                    gte_ldv0(&rel[3]);
                    gte_rtv0tr();
                    gte_stlvnl(&out);
                    vert[3].vx = out.vx;
                    vert[3].vy = out.vy;
                    vert[3].vz = out.vz;
                }
                piece->posX10 += piece->velX0;
                piece->posY12 += piece->velY2;
                piece->posZ14 += piece->velZ4;
            } else {
                dx = fx_1b3_splitTrigger.posX4 - piece->posX10;
                dz = fx_1b3_splitTrigger.posZ8 - piece->posZ14;
                dy = fx_1b3_splitTrigger.posY6 - piece->posY12;
                switch (fx_1b3_splitTrigger.mode0) {
                case 0:
                    if (abs(dx) < fx_1b3_splitTrigger.radiusC && abs(dz) < fx_1b3_splitTrigger.radiusC) {
                        piece->timer6 = 0x40;
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x20;
                        piece->velX0 = (ex->btlfx_rand() & 0x7F) + 0x20;
                        piece->velY2 = (ex->btlfx_rand() & 0x3F) - 0x20;
                        piece->velZ4 = 0;
                    }
                    break;
                case 1:
                    if (abs(dx) < fx_1b3_splitTrigger.radiusC && abs(dz) < fx_1b3_splitTrigger.radiusC) {
                        piece->timer6 = 0x40;
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x20;
                        piece->velX0 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        piece->velY2 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        piece->velZ4 = -0x20 - (ex->btlfx_rand() & 0x7F);
                    }
                    break;
                case 2:
                    if (abs(dy) < fx_1b3_splitTrigger.radiusC) {
                        piece->timer6 = 0x80;
                        piece->velX0 = -(dx >> 3);
                        piece->velY2 = -(dy >> 3);
                        piece->velZ4 = -(dz >> 3);
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x10;
                    }
                    break;
                case 3:
                    if (abs(dx) < fx_1b3_splitTrigger.radiusC && abs(dz) < fx_1b3_splitTrigger.radiusC) {
                        piece->timer6 = 0x80;
                        piece->velX0 = -(dx >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        piece->velY2 = -(ex->btlfx_rand() & 0x7F) + 0x40;
                        piece->velZ4 = -(dz >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x10;
                    }
                    break;
                case 4:
                    if (abs(dx) < fx_1b3_splitTrigger.radiusC) {
                        piece->timer6 = 0x22;
                        piece->velX0 = -(dx >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        piece->velY2 = -(ex->btlfx_rand() & 0x3F);
                        piece->velZ4 = -(dz >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x10;
                    }
                    break;
                case 5:
                    if (fx_1b3_splitTrigger.radiusC < ex->btlfx_sqrt(dy * dy + dx * dx, 0xF)) {
                        piece->timer6 = 0x40;
                        piece->velX0 = dx >> 7;
                        piece->velY2 = dy >> 7;
                        piece->velZ4 = -(dz >> 7);
                        piece->scale16 = 0x1000;
                        piece->scaleStepE = 0x20;
                    }
                    break;
                }
            }
            if (!(i & 1)) {
                vert += 4;
            } else {
                vert += 3;
            }
            piece++;
        }
    }
}

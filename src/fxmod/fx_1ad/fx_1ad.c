#include "fxmod/fx_1ad.h"

static s32 fx_1ad_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    s32 t = (frame << 12) / frames;
    s32 row = (e->slot - 1) % 6;
    s32 v = t * fx_1ad_boneChains[row][0];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ad_env);
    s32 frac = v & 0xFFF;
    s32 i = v / 4096;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;

    ex->btlfx_getUnitBonePos(0x10, fx_1ad_boneChains[row][i + 1], (u16 *)&a);
    ex->btlfx_getUnitBonePos(0x10, fx_1ad_boneChains[row][i + 2], (u16 *)&b);
    ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, frac, (s16 *)&c);
    pos->vx = c.vx;
    pos->vy = c.vy;
    pos->vz = c.vz;
    e->pos.vx = c.vx << 12;
    e->pos.vy = c.vy << 12;
    e->pos.vz = c.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1ad_op80(s32 mode, Fx1adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    MATRIX m;
    Pool6CEntry *emitter;
    s32 frame;
    s32 c;
    s32 t;
    s32 k;
    s32 n;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->done0 = 0;
        work->frame4 = 0;
        fx_1ad_env = env;
        ex->btlfx_initEffModels(fx_1ad_effModels, 1);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->model24 = (s16)ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, 0xF, (u16 *)&work->pos8);
        pos.vx = work->pos8.vx;
        pos.vy = work->pos8.vy;
        pos.vz = work->pos8.vz;
        if (work->done0 == 0) {
            k = 0x40;
            n = work->frame4;
            ex->btlfx_drawScrollMove(n * 4, 0x200, 0x100, 0x20, k, ((n & 3) << 5) + 0x240, ((n * 16) & 0xC0) + 0x100);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), work->frame4);
            if (frame == 0) {
                work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
                if (work->emit20 != NULL) {
                    work->emit20->trails = (s32)fx_1ad_op80Trails0;
                    work->emit20->trailSlots = 0x48;
                    work->emit20->trailStride = 0x3C;
                }
                if (work->emit20 != NULL) {
                    work->emit20->otZ = k;
                }
                if (work->emit20 != NULL) {
                    work->emit20->file->m.module.update = fx_1ad_op80ParticleMotion;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1ad_op80Trails1;
                    emitter->trailSlots = 0x10;
                    emitter->trailStride = 0x6C;
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0xC) {
                t = ((frame - 0xC) << 12) / 57;
                c = ex->math_cos((frame * 0x52 + 0x200) & 0x3FE);
                scale.vz = 0x1800;
                scale.vx = scale.vy = c / 3 + 0x1800;
                ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&m);
                ex->btlfx_matrixAngles(&m, (s16 *)&rot);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                         (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80);
            }
            if (frame >= 0x45) {
                work->done0 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ad/nonmatchings/fx_1ad", fx_1ad_headerPtr);

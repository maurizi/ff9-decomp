#include "fxmod/fx_0ea.h"

s32 fx_0ea_op81ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_arcParticle(pt, pos, &D_801E7E64[0], frame, n);
}

s32 fx_0ea_op81ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_arcParticle(pt, pos, &D_801E7E64[1], frame, n);
}

s32 fx_0ea_op81ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_arcParticle(pt, pos, &D_801E7E64[2], frame, n);
}

s32 fx_0ea_op81ParticleMotion3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_arcParticle(pt, pos, &D_801E7E64[3], frame, n);
}

static s32 fxm_arcParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *to, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ea_env);
    s32 t = (frame << 12) / 18;

    ex->btlfx_lerpVec((s16 *)&D_801E7E5C, (s16 *)to, t, (s16 *)pos);
    pos->vy -= ex->math_sin(t / 2) * 800 / 4096;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0ea_op80(s32 mode, Fx0eaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_0ea_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos8);
        work->unk10.vx = work->pos8.vx;
        work->unk10.vy = work->pos8.vy;
        work->unk10.vz = work->pos8.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        work->unk1A = 0x1000;
        work->unk18 = 0;
    } else {
        frame = entry->frame;
        if (frame == 0x1E) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
        if (frame >= 0x1E && frame < 0x2B) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->unk10, work->unk18, work->unk1A);
            if (frame - 0x1E >= 5) {
                work->unk1A -= 0x200;
            }
            work->unk18++;
            n = work->unk18;
            if (n >= 6) {
                n = 5;
            }
            work->unk18 = n;
        }
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_0ea_op81(s32 mode, Fx0eaWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        n = *arg4;
        work->unk20 = n;
        fx_0ea_env = env;
        if (n >= env->targets->targetCount || ex->btl_getUnit(0x10) == ex->btl_getUnit(work->unk20)) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(0x13, entry->file, -1);
        ex->btlfx_playFileSound(0x14, entry->file, -1);
        ex->btlfx_playFileSound(0x15, entry->file, -1);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        D_801E7E5C.vx = work->pos8.vx;
        D_801E7E5C.vy = work->pos8.vy;
        D_801E7E5C.vz = work->pos8.vz;
        ex->btlfx_getUnitBodyPos(work->unk20, 0, (s16 *)&work->pos10);
        D_801E7E64[work->unk20].vx = work->pos10.vx;
        D_801E7E64[work->unk20].vy = work->pos10.vy;
        D_801E7E64[work->unk20].vz = work->pos10.vz;
        ex->btlfx_getUnitPos(work->unk20, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E7DF4[work->unk20]), (s32)&D_801E7E84);
        if (work->emit0 != NULL) {
            ((Fx0eaEffHdr *)work->emit0->file)->unk8 = fx_0ea_op81ParticleMotions[work->unk20];
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
    } else {
        frame = entry->frame;
        if (frame == 0x14) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(work->unk20, 0x80);
            }
        }
        if (frame == 0xF) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x32;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    env->state->signals[i] = v;
}

INCLUDE_RODATA("asm/us/fx_0ea/nonmatchings/fx_0ea", fx_0ea_headerPtr);

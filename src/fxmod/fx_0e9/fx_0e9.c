#include "fxmod/fx_0e9.h"

static s32 fx_0e9_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0e9_env);
    SVECTOR v;
    SVECTOR rot;
    s32 rad;

    if (frame < 0x10) {
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = fx_0e9_orbit.angle6 + 0x800;
        ex->btlfx_pointAhead((s16 *)&fx_0e9_startPos, (s16 *)&rot, frame * 40, (s16 *)&v);
    } else {
        rad = fx_0e9_orbit.radius2 = fxm_lerpRatio(fx_0e9_orbit.radius0, 0, 0x6E - fx_0e9_orbit.timerA, 0x6E);
        rad += (fx_0e9_orbit.timerA + fx_0e9_orbit.timerA / 2) * ex->math_sin(fx_0e9_orbit.wobble8) / 4096;
        v.vx = rad * ex->math_sin(fx_0e9_orbit.angle6) / 4096;
        v.vy = 0;
        v.vz = rad * ex->math_cos(fx_0e9_orbit.angle6) / 4096;
        switch (fx_0e9_orbit.dir4) {
        case 0:
            fx_0e9_orbit.angle6 -= 0x60;
            break;
        case 1:
            fx_0e9_orbit.angle6 += 0x60;
            break;
        }
        fx_0e9_orbit.wobble8 += 0x100;
        if (fx_0e9_orbit.timerA > 0) {
            fx_0e9_orbit.timerA--;
        }
        v.vx += fx_0e9_orbitCenter.vx;
        v.vy += fx_0e9_orbitCenter.vy;
        v.vz += fx_0e9_orbitCenter.vz;
    }
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0e9_op80(s32 mode, Fx0e9Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR d;
    s32 frame;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        fx_0e9_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot10, 0xC8, (s16 *)&fx_0e9_startPos);
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot10, 0x348, (s16 *)&fx_0e9_orbitStart);
        fx_0e9_orbitCenter.vx = work->pos18.vx;
        fx_0e9_orbitCenter.vy = work->pos18.vy;
        fx_0e9_orbitCenter.vz = work->pos18.vz;
        ex->btlfx_aimAngles((s16 *)&fx_0e9_orbitCenter, (s16 *)&fx_0e9_orbitStart, (s16 *)&rot);
        d.vx = fx_0e9_orbitCenter.vx - fx_0e9_orbitStart.vx;
        d.vy = fx_0e9_orbitCenter.vy - fx_0e9_orbitStart.vy;
        d.vz = fx_0e9_orbitCenter.vz - fx_0e9_orbitStart.vz;
        fx_0e9_orbit.radius0 = fx_0e9_orbit.radius2 = ex->btlfx_sqrt(d.vx * d.vx + d.vz * d.vz, 0x14);
        fx_0e9_orbit.dir4 = ex->btlfx_rand() % 2;
        fx_0e9_orbit.wobble8 = 0;
        fx_0e9_orbit.timerA = 0x6E;
        fx_0e9_orbit.angle6 = rot.vy;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&fx_0e9_emitPos);
        if (work->emit0 != NULL) {
            ((Fx0e9EffHdr *)work->emit0->file)->motion8 = fx_0e9_op80ParticleMotion;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x50) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
        }
        return frame >= 0x8C;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e9_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0e9/nonmatchings/fx_0e9", fx_0e9_headerPtr);

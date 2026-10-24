#include "fxmod/fx_0e9.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7CFC);
    SVECTOR v;
    SVECTOR rot;
    s32 rad;

    if (frame < 0x10) {
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = D_801E7D0C.unk6 + 0x800;
        ex->btlfx_pointAhead((s16 *)&D_801E7D1C, (s16 *)&rot, frame * 40, (s16 *)&v);
    } else {
        rad = D_801E7D0C.unk2 = func_801E7C5C(D_801E7D0C.unk0, 0, 0x6E - D_801E7D0C.unkA, 0x6E);
        rad += (D_801E7D0C.unkA + D_801E7D0C.unkA / 2) * ex->math_sin(D_801E7D0C.unk8) / 4096;
        v.vx = rad * ex->math_sin(D_801E7D0C.unk6) / 4096;
        v.vy = 0;
        v.vz = rad * ex->math_cos(D_801E7D0C.unk6) / 4096;
        switch (D_801E7D0C.unk4) {
        case 0:
            D_801E7D0C.unk6 -= 0x60;
            break;
        case 1:
            D_801E7D0C.unk6 += 0x60;
            break;
        }
        D_801E7D0C.unk8 += 0x100;
        if (D_801E7D0C.unkA > 0) {
            D_801E7D0C.unkA--;
        }
        v.vx += D_801E7D2C.vx;
        v.vy += D_801E7D2C.vy;
        v.vz += D_801E7D2C.vz;
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

s32 fx_0e9_op80(s32 mode, Fx0e9Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR d;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7CFC = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk10, 0xC8, (s16 *)&D_801E7D1C);
        ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk10, 0x348, (s16 *)&D_801E7D24);
        D_801E7D2C.vx = work->unk18.vx;
        D_801E7D2C.vy = work->unk18.vy;
        D_801E7D2C.vz = work->unk18.vz;
        ex->btlfx_aimAngles((s16 *)&D_801E7D2C, (s16 *)&D_801E7D24, (s16 *)&rot);
        d.vx = D_801E7D2C.vx - D_801E7D24.vx;
        d.vy = D_801E7D2C.vy - D_801E7D24.vy;
        d.vz = D_801E7D2C.vz - D_801E7D24.vz;
        D_801E7D0C.unk0 = D_801E7D0C.unk2 = ex->btlfx_sqrt(d.vx * d.vx + d.vz * d.vz, 0x14);
        D_801E7D0C.unk4 = ex->btlfx_rand() % 2;
        D_801E7D0C.unk8 = 0;
        D_801E7D0C.unkA = 0x6E;
        D_801E7D0C.unk6 = rot.vy;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&D_801E7D34);
        if (work->unk0 != NULL) {
            ((Fx0e9EffHdr *)work->unk0->unk0)->unk8 = func_801E7704;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x50) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk18);
        }
        return frame >= 0x8C;
    }
    return 0;
}

void func_801E7C18(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7CFC)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C5C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0e9/nonmatchings/fx_0e9", fx_0e9_headerPtr);

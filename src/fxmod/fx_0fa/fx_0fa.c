#include "fxmod/fx_0fa.h"

void func_801E7704(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 w[4];
    s32 k;
    s32 s;
    s32 r;
    s32 i;
    s32 *wp;

    k = 0x2000000 / (len * len);
    s = 0;
    switch (mode) {
    case 0:
        s = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        s = 0x100 - ((k * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        s = (k * t * t) >> 17;
        break;
    }
    r = 0x100 - s;
    w[0] = (s * s * s) >> 12;
    w[1] = (r * s * s * 6) >> 12;
    w[2] = (r * r * s * 6) >> 12;
    w[3] = (r * r * r) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    wp = w;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *wp) >> 12;
        out->vy += (ctrl[i].vy * *wp) >> 12;
        out->vz += (ctrl[i].vz * *wp) >> 12;
        wp++;
    }
}

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x78;
    } else if (mode == 1) {
        work->unk74 = NULL;
        D_801E8034 = env;
        ex->btlfx_initEffModels(D_801E8044, 2);
        for (i = 9, q = &work->unk0[9]; i >= 0; i--) {
            *q-- = 0;
        }
        work->unk38 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xFA, 0x3DC0, 0x80, 0x80);
        work->unk5C.vx = 0x333;
        work->unk5C.vy = 0x333;
        work->unk5C.vz = 0x333;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, &work->unk44.vx);
        ex->btlfx_getUnitBodyPos(0x10, 0, &work->unk28.vx);
        work->unk4C.vx = work->unk44.vx;
        work->unk4C.vy = work->unk44.vy;
        work->unk4C.vz = work->unk44.vz;
        ex->btlfx_getUnitBodyPos(0, 0, &work->unk30.vx);
        return 0;
    } else {
        frame = entry->unk0;
        if (work->unk74 != NULL && work->unk74->unk30 == -1) {
            work->unk74 = NULL;
        }
        switch (frame) {
        case 0:
            work->unk0[0] = 0x10;
            work->unk0[2] = 0x50;
            break;
        case 0x14:
            work->unk0[4] = 8;
            work->unk0[6] = 1;
            work->unk74 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk44);
            break;
        case 0x20:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0x40:
            work->unk0[1] = 0x10;
            break;
        case 0x50:
            work->unk0[2] = 0;
            break;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            work->unk4C.vx = work->unk44.vx = work->unk28.vx;
            work->unk4C.vy = work->unk44.vy = work->unk28.vy - ((ex->math_sin(work->unk0[0] << 7) * (0x10 - work->unk0[0])) >> 7);
            work->unk4C.vz = work->unk44.vz = work->unk28.vz + ((ex->math_cos(work->unk0[0] << 7) * (0x10 - work->unk0[0])) >> 7);
            ex->btlfx_aimAngles(&work->unk30.vx, &work->unk44.vx, &work->unk3C.vx);
            ex->btlfx_aimAngles(&work->unk44.vx, &work->unk30.vx, &work->unk6C.vx);
            work->unk5C.vx = work->unk5C.vy = work->unk5C.vz = ex->math_cos(work->unk0[0] << 6) / 5;
        }
        if (work->unk0[1] != 0) {
            work->unk0[1]--;
            work->unk4C.vx = work->unk44.vx = work->unk28.vx;
            work->unk4C.vy = work->unk44.vy = work->unk28.vy - ((ex->math_sin((0x10 - work->unk0[1]) << 7) * work->unk0[1]) >> 7);
            work->unk4C.vz = work->unk44.vz = work->unk28.vz + ((ex->math_cos((0x10 - work->unk0[1]) << 7) * work->unk0[1]) >> 7);
            work->unk3C.vx -= work->unk0[1] * work->unk0[1] * 4;
            work->unk5C.vx = work->unk5C.vy = work->unk5C.vz = ex->math_cos((0x10 - work->unk0[1]) << 6) / 5;
        }
        if (work->unk0[6] != 0 && work->unk74 != NULL) {
            work->unk74->unk5C.vx = work->unk6C.vx;
            work->unk74->unk5C.vy = work->unk6C.vy;
            work->unk74->unk5C.vz = work->unk6C.vz;
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->unk4C.vy = work->unk44.vy + (ex->math_sin(frame << 7) >> 8);
            ex->Hi_DrawEffModel(&work->unk3C, &work->unk4C, (s32)&work->unk5C, work->unk38);
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, D_801E7FAC[work->unk0[4] >> 1].unk0,
                              D_801E7FAC[work->unk0[4] >> 1].unk4);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, D_801E7FCC[work->unk0[5]].unk0, D_801E7FCC[work->unk0[5]].unk4);
        } else if (frame > 0x20) {
            if (!(ex->btlfx_rand() & 7)) {
                work->unk0[5] = 4;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fa/nonmatchings/fx_0fa", fx_0fa_headerPtr);

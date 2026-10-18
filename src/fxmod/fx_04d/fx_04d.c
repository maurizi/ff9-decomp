#include "fxmod/fx_04d.h"

s32 func_801E7704(s32 mode, Fx04dItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx04dWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *r;
    Pool6CEntry *p;
    s32 a;
    s32 k;
    s32 c;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->unk4 = 0;
        it->unk8 = 0;
        p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk18);
        it->unk40 = p;
        if (p != NULL) {
            p->unk22 = 0x20;
        }
        return 0;
    }
    fade = 0;
    r = &D_801E8578[it->unk0];
    switch (it->unk4) {
    case 0:
        t = it->unk8 << 10;
        a = t >> 2;
        scale.vx = scale.vy = ((0x1000 - ex->math_cos(a)) * (it->unk3C * 5 / 2)) >> 12;
        scale.vz = (ex->math_sin(a) * it->unk3C) >> 12;
        BTLFX_COPY_SVEC(&rot, r);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->unk40 != NULL) {
            if (it->unk40->unk30 == -1) {
                it->unk40 = NULL;
            }
            if (it->unk40 != NULL) {
                it->unk40->unk5C.vx = r->vx;
                it->unk40->unk5C.vy = r->vy;
                it->unk40->unk5C.vz = r->vz;
            }
        }
        it->unk8++;
        if (it->unk8 >= 4) {
            it->unk4 = 1;
            it->unk8 = 0;
        }
        break;
    case 1:
        scale.vx = scale.vy = it->unk3C * 5 / 2;
        scale.vz = it->unk3C;
        BTLFX_COPY_SVEC(&rot, r);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->unk40 != NULL) {
            if (it->unk40->unk30 == -1) {
                it->unk40 = NULL;
            }
            if (it->unk40 != NULL) {
                it->unk40->unk5C.vx = rot.vx;
                it->unk40->unk5C.vy = rot.vy;
                it->unk40->unk5C.vz = rot.vz;
            }
        }
        if (work->unk28 != 0) {
            it->unk4 = 2;
            it->unk8 = 0;
            it->unk38 += -25 + ex->btlfx_rand() % 50;
            it->unk3A += -16 + (ex->btlfx_rand() & 0x1F);
            BTLFX_COPY_SVEC(&it->unk30, r);
            it->unkC[1].vx = it->unkC[0].vx;
            it->unkC[1].vy = it->unkC[0].vy;
            it->unkC[1].vz = it->unkC[0].vz;
            it->unkC[2].vx = it->unkC[0].vx;
            it->unkC[2].vy = it->unkC[0].vy;
            it->unkC[2].vz = it->unkC[0].vz;
        }
        break;
    case 2:
        t = (it->unk8 << 12) / 20;
        a = t >> 2;
        scale.vx = scale.vy = (ex->math_cos(a) * (it->unk3C * 5 / 2)) >> 12;
        scale.vz = it->unk3C;
        fade = ex->math_cos(a);
        k = ex->math_cos(a) >> 7;
        ex->func_800CFCA8(r, k, (s32)&d);
        it->unkC[0].vx -= d.vx;
        it->unkC[0].vy -= d.vy - (it->unk8 - 6);
        it->unkC[0].vz -= d.vz;
        ex->func_800CFCA8(r, k * 2, (s32)&d);
        it->unkC[1].vx -= d.vx;
        it->unkC[1].vy -= d.vy - (it->unk8 - 6) * 4;
        it->unkC[1].vz -= d.vz;
        ex->func_800CFCA8(r, k * 2 + k, (s32)&d);
        r = &it->unk30;
        it->unkC[2].vx -= d.vx;
        it->unkC[2].vy -= d.vy - (it->unk8 - 6) * 8;
        it->unkC[2].vz -= d.vz;
        it->unk30.vx += it->unk38;
        it->unk30.vy += it->unk3A;
        it->unk8++;
        if (it->unk8 >= 20) {
            return 1;
        }
        break;
    }
    scale.vx >>= 1;
    scale.vy >>= 1;
    if (it->unk4 < 2) {
        ex->Hi_SetEffModelOffset(work->unk4, 2);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[0], (s32)&scale, work->unk4);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->unk4, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->unk4, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk4, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[0], (s32)&scale, work->unk14);
    } else {
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[0], (s32)&scale, work->unk10);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->unk10, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->unk10, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk10, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[1], (s32)&scale, work->unkC);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->unkC, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->unkC, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unkC, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[2], (s32)&scale, work->unk8);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->unk8, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->unk8, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk8, c, c, c);
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_04d/nonmatchings/fx_04d", fx_04d_op80);

INCLUDE_RODATA("asm/us/fx_04d/nonmatchings/fx_04d", fx_04d_headerPtr);

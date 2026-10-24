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

s32 fx_04d_op80(s32 mode, Fx04dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    Fx04dItem *it;
    Pool6CEntry *p;
    Pool6CEntry *q;
    SVECTOR *r;
    s32 frame;
    s32 state;
    s32 i;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x34;
        init->unk8 = 0x44;
        init->unk14 = (s32)func_801E7704;
        init->unk4 = 0xA;
    } else if (mode == 1) {
        D_801E8610 = env;
        ex->btlfx_initEffModels(D_801E8620, 6);
        work->unk4 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x9A, 0x3DC0, 0x80, 0);
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0x9A, 0x3DC0, 0x80, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0x9A, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0x9A, 0x3DC0, 0x80, 0);
        work->unk14 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0xBA, 0x3D40, 0, 0x80);
        work->unk0 = 0;
        work->unk2A = 0;
        work->unk2C = 0;
        work->unk28 = 0;
        work->unk30 = 0;
        work->unk18.vx = env->targets->centerX;
        work->unk18.vy = env->targets->centerY;
        work->unk18.vz = env->targets->centerZ;
        work->unk20.vx = work->unk18.vx;
        work->unk20.vy = work->unk18.vy;
        work->unk20.vz = work->unk18.vz;
        work->unk18.vy -= 0x200;
        q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk18);
        if (q != NULL) {
            q->unk22 = 0x64;
        }
        r = D_801E8578;
        for (i = 0; i < 10; i++, r++) {
            r->vx += 0x5C;
            ((s16 *)r)[1] += 0x200;
        }
        return 0;
    } else {
    state = work->unk0;
    frame = entry->unk0;
    switch (state) {
    case 0:
        it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (it != NULL) {
            it->unk0 = work->unk2A;
            work->unk2A++;
            ex->func_800CFD78((s16 *)&work->unk18, (s16 *)&D_801E8578[it->unk0], -0x10, (s16 *)&d);
            it->unkC[0].vx = d.vx;
            it->unkC[0].vy = d.vy;
            it->unkC[0].vz = d.vz;
            it->unk3C = D_801E8578[it->unk0].pad * 31 >> 3;
        }
        work->unk30 += 0x20;
        if (frame >= 9) {
            work->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    case 1:
        work->unk30 += (ex->math_cos((frame << 10) / 14) << 5) >> 12;
        if (frame >= 14) {
            work->unk0 = 2;
            entry->unk0 = -1;
            work->unk28 = state;
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk18);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        break;
    case 2:
        if (frame < 5) {
            c = (4 - frame) * 250 / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c >> 1, c);
        }
        if (frame == 7) {
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk18);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame >= 21) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    ex->btlfx_drawScrollMove(work->unk30, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
    work->unk2C++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04d/nonmatchings/fx_04d", fx_04d_headerPtr);

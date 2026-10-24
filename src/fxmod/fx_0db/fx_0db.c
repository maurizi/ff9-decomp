#include "fxmod/fx_0db.h"

s32 func_801E7704(s32 mode, Fx0dbItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx0dbWork80 *work) {
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
        p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk14);
        it->unk40 = p;
        if (p != NULL) {
            p->unk22 = 0x40;
        }
        return 0;
    }
    fade = 0;
    r = &D_801E88B4[it->unk0];
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
        if (work->unk24 != 0) {
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
        ex->btlfx_rotateOffset(r, k, (s32)&d);
        it->unkC[0].vx -= d.vx;
        it->unkC[0].vy -= d.vy - (it->unk8 - 6);
        it->unkC[0].vz -= d.vz;
        ex->btlfx_rotateOffset(r, k * 2, (s32)&d);
        it->unkC[1].vx -= d.vx;
        it->unkC[1].vy -= d.vy - (it->unk8 - 6) * 4;
        it->unkC[1].vz -= d.vz;
        ex->btlfx_rotateOffset(r, k * 2 + k, (s32)&d);
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
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->unkC[0], (s32)&scale, work->unk10);
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

s32 fx_0db_op80(s32 mode, Fx0dbWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx0dbItem *part;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 c;
    s16 *a;
    s16 *b;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = 0x34;
        ((EffInit *)arg4)->unk8 = 0x44;
        ((EffInit *)arg4)->unk14 = (s32)func_801E7704;
        ((EffInit *)arg4)->unk4 = 0xA;
    } else if (mode == 1) {
        D_801E894C = env;
        ex->btlfx_initEffModels(D_801E895C, 6);
        work->unk4 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13), 0x9A, 0x3DC0, 0x80, 0);
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0x9A, 0x3DC0, 0x80, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0x9A, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0x9A, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0xBA, 0x3D40, 0, 0x80);
        work->unk0 = 0;
        work->unk26 = 0;
        work->unk28 = 0;
        work->unk24 = 0;
        work->unk2C = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk1C);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x46);
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk14);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x46);
        }
        a = &D_801E88B4[0].vx;
        for (i = 0, b = a + 1; i < 10; i++) {
            *a += 0x5C;
            a += 4;
            *b += 0x200;
            b += 4;
        }
        return 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (part != NULL) {
                part->unk0 = work->unk26;
                work->unk26++;
                ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&D_801E88B4[part->unk0], -0x10, (s16 *)&v);
                part->unkC[0].vx = v.vx;
                part->unkC[0].vy = v.vy;
                part->unkC[0].vz = v.vz;
                part->unk3C = D_801E88B4[part->unk0].pad * 3;
            }
            work->unk2C += 0x20;
            if (frame >= 9) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            work->unk2C += (ex->math_cos((frame << 10) / 14) << 5) >> 12;
            if (frame >= 0xE) {
                work->unk0 = 2;
                entry->unk0 = -1;
                work->unk24 = 1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            break;
        case 2:
            if (frame < 5) {
                c = (4 - frame) * 0xFA / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c >> 1, c);
            }
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x15) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(work->unk2C, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        work->unk28++;
    }
    return 0;
}

s32 fx_0db_op84(s32 mode, Fx0dbWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E894C = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->unk0;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
        ry = 0;
        switch (work->unk0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->unk28 = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0db/nonmatchings/fx_0db", fx_0db_headerPtr);

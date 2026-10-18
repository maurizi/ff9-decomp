#include "fxmod/fx_0db.h"

INCLUDE_ASM("asm/us/fx_0db/nonmatchings/fx_0db", func_801E7704);

s32 fx_0db_op80(s32 mode, Fx0dbWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx0dbPart *part;
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
                ex->func_800CFD78((s16 *)&work->unk14, (s16 *)&D_801E88B4[part->unk0], -0x10, (s16 *)&v);
                part->unkC[0] = v.vx;
                part->unkC[1] = v.vy;
                part->unkC[2] = v.vz;
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
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 2, c >> 1, c);
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
        ex->func_800E6624(work->unk2C, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
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
        ex->func_800E7D3C(0x10, 1, (s16 *)&work->unk8);
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
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0db/nonmatchings/fx_0db", fx_0db_headerPtr);

#include "fxmod/fx_057.h"

void func_801E7704(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n) {
    Unk800F78A4 *env = D_801E8EA4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = D_801EA97C;
    u8 *buf = (u8 *)ex->func_800E0134(pt, e);
    s32 s;
    s32 d;
    SVECTOR v;

    if (t == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), (s32)pos, NULL, -1, 0, 0, 0, buf, 0);
    }
    if (t < n - 4) {
        s = 0x1000;
    } else {
        s = 0x1000 - (((t - (n - 4)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), (s32)pos, (s32 *)e->unk28, t,
                      0x1C00 - (t << 12) / n, s, 1, buf, 1);
    if (pt->unk1F == 1) {
        if (D_801EA704 != NULL) {
            ex->func_800E3D20((s32)D_801EA704, (s32)pos);
        }
        BTLFX_COPY_SVEC(&D_801EA6FC, pos);
        if (t < 0x1D) {
            env->unk2C->unk0[3].vx = pos->vx;
            env->unk2C->unk0[3].vy = pos->vy;
            env->unk2C->unk0[3].vz = pos->vz;
        } else {
            d = t - 0x1D;
            if (d < 0x20) {
                ex->btlfx_lerpVec((s16 *)pos, (s16 *)&D_801EA6F4, 0x1000 - ex->math_cos(d << 5), (s16 *)&v);
            } else {
                BTLFX_COPY_SVEC(&v, &D_801EA6F4);
            }
            env->unk2C->unk0[3].vx = v.vx;
            env->unk2C->unk0[3].vy = v.vy;
            env->unk2C->unk0[3].vz = v.vz;
        }
    }
}

s32 fx_057_op80(s32 mode, Fx057Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    VECTOR sv;
    SVECTOR unused; 
    Pool6CEntry *p;
    s32 frame;
    s32 alpha;
    s32 scale;
    s32 t;
    s32 c;
    s32 k;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E8EA4 = env;
        D_801EA708 = 0;
        D_801EA97C = entry;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&D_801EA6F4, &work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801EA714);
        work->unk2C = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
        if (work->unk2C != NULL) {
            work->unk2C->unk22 = 0x18;
        }
        if (work->unk2C != NULL) {
            work->unk2C->unk14 = (s32)D_801E8EB4;
            work->unk2C->unk12 = 0x10;
            work->unk2C->unkC = 0x184;
        }
        if (work->unk2C != NULL) {
            work->unk2C->unk18->unk24 = (void (*)())func_801E7704;
        }
        D_801EA704 = work->unk30 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unk24);
        if (work->unk30 != NULL) {
            work->unk30->unk22 = 0x18;
        }
        work->unk34 = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk24);
        if (work->unk34 != NULL) {
            work->unk34->unk22 = 0x10;
        }
        ex->btlfx_initEffModels(D_801EA724, 3);
        work->unk8 =
            ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0x137, 0xFFFF, 0, 0x80);
        work->unkC = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x28));
    } else {
        alpha = 0;
        frame = entry->unk0;
        scale = 0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 22;
            if (work->unk2C != NULL && work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->unk30 == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->unk30 == -1) {
                work->unk34 = NULL;
            }
            alpha = 0x1000;
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = alpha;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = alpha;
            }
            scale = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < 4) {
                c = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 1, c, c);
            }
            if (frame >= 22) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 20;
            alpha = 0x1000 - t;
            scale = 0x1000;
            D_801EA708 = t;
            if (work->unk2C != NULL && work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->unk30 == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->unk30 == -1) {
                work->unk34 = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = alpha;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = alpha;
            }
            if (frame >= 20) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk30);
                ex->func_800E3D54(work->unk34);
                D_801EA704 = NULL;
            }
            break;
        case 2:
            D_801EA708 = 0x1000;
            if (work->unk2C != NULL && work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA708;
            }
            if (frame == 19) {
                D_801EA70C = p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk10);
                if (D_801EA70C != NULL) {
                    D_801EA70C->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0x80, 0xFF);
            }
            if (frame >= 22) {
                ex->func_800E3D54(work->unk2C);
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.pad = 0;
            rot.vy = 0;
            rot.vx = 0;
            rot.vz = -work->unk4 * 20;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), &D_801EA6FC, &rot, scale, 0, 1, alpha);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &D_801EA6FC, &rot, 0xE00, 0, 1, alpha);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), &D_801EA6FC, &rot, 0x1000, 0, 1, alpha);
        }
        if (D_801EA708 != 0) {
            sv.vx = 0x1000;
            sv.vy = 0x1000;
            sv.vz = 0x1000;
            ang.vx = 0;
            ang.vy = -0x400;
            ang.vz = 0;
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk18, (s32)&sv, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unk8, (D_801EA708 >> 5) - 0x80, (D_801EA708 >> 5) - 0x80,
                                     (D_801EA708 >> 5) - 0x80);
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk18, (s32)&sv, work->unkC);
            ex->Hi_ModifyEffModelAbr(work->unkC, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unkC, (D_801EA708 >> 5) - 0x80, (D_801EA708 >> 5) - 0x80,
                                     (D_801EA708 >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x18), work->unk4);
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk10, NULL, 0x1600, 0, 1, D_801EA708);
            env->unk28 = 0;
            ang.pad = 1;
            ang.vx = -0x200;
            ang.vy = work->unk4 * 0x18 + 0x580;
            ang.vz = 0;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), &work->unk10, &ang, 0x16000, 0, 1,
                              D_801EA708);
            k = ex->math_cos(((work->unk4 - 22) << 10) / 42);
            ang.vx = -0x180;
            ang.vy = work->unk4 * 32 + 0x180;
            ang.vz = work->unk4 * 0x18;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk10, &ang, k * 4, 0, 1,
                              D_801EA708);
            ang.vx = 0x200;
            ang.vy = 0x200 - work->unk4 * 32;
            ang.vz = -work->unk4 * 0x10;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk10, &ang, (k * 8 + k * 4) >> 1, 0, 1,
                              D_801EA708);
            ang.vx = 0x100;
            ang.vy = -0x180 - work->unk4 * 0x18;
            ang.vz = -work->unk4 * 0x10;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk10, &ang, (k * 16 + k * 4) >> 1, 0, 1,
                              D_801EA708);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x1F), work->unk4);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 3), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_057/nonmatchings/fx_057", fx_057_op81);

INCLUDE_RODATA("asm/us/fx_057/nonmatchings/fx_057", fx_057_headerPtr);

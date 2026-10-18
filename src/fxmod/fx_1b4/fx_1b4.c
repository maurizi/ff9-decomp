#include "fxmod/fx_1b4.h"

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E7704);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E7738);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E776C);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E77A0);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E77D4);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E7808);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E783C);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E7870);

s32 func_801E78A4(Pool30Entry *e, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8558(path, frame, &v);
    v.vx += D_801EF174.vx;
    v.vy += D_801EF174.vy;
    v.vz += D_801EF174.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", fx_1b4_op80);

s32 fx_1b4_op81(s32 mode, Fx1b4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    VECTOR scale2;
    SVECTOR rot2;
    CVECTOR col2;
    DrMode2Words *dm;
    DrMode2Words *dm2;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk10.vx = env->unkC->unk38;
        work->unk10.vy = env->unkC->unk3A;
        D_801E8654 = env;
        work->unk10.vz = env->unkC->unk3C;
        work->unk10.vy -= 0x258;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk10);
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk10);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk22 = 0x100;
            }
            if (work->unk0[1] != NULL) {
                work->unk0[1]->unk22 = 0x80;
            }
            break;
        case 0x1C:
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
            if (work->unk0[2] != NULL) {
                work->unk0[2]->unk14 = (s32)D_801EAA64;
                work->unk0[2]->unk12 = 0xC0;
                work->unk0[2]->unkC = 0x30;
                if (work->unk0[2] != NULL) {
                    work->unk0[2]->unk22 = 0x80;
                }
            }
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk22 = 0x100;
            }
            break;
        case 0x1E:
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        if ((u32)(frame - 0x1F) < 0x3C) {
            dm = (DrMode2Words *)env->unk24;
            env->unk24 += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->unk20[1], dm);
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            col.r = col.g = col.b = 0x28;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        t = frame - 0x5B;
        if ((u32)t < 0xF) {
            dm2 = (DrMode2Words *)env->unk24;
            env->unk24 += sizeof(DrMode2Words);
            setlen(dm2, 2);
            dm2->code[0] = 0xE6000001;
            dm2->code[1] = 0;
            addPrim(&env->unk20[1], dm2);
            scale2.vz = scale2.vy = scale2.vx = (t << 7) + 0x1000;
            col2.r = col2.g = col2.b = 0x28;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->func_800E6764(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot2, &scale2, &col2, 1, 0);
        }
        ex->func_800D3030((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x1C), frame);
        return frame >= 0x8C;
    }
    return 0;
}

s32 fx_1b4_op82(s32 mode, Fx1b4Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        work->unk1C.vx = env->unkC->unk38;
        work->unk1C.vy = env->unkC->unk3A;
        D_801E8654 = env;
        work->unk1C.vz = env->unkC->unk3C;
        work->unk1C.vy -= 0x258;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk1C);
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk1C);
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk1C);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->unk22 = 0x100;
            }
            break;
        case 20:
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk1C);
            work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk1C);
            work->unk0[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk1C);
            work->unk0[6] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2A), (s32)&work->unk1C);
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801ECE64;
                work->unk0[4]->unk12 = 0x21;
                work->unk0[4]->unkC = 0x90;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk14 = (s32)D_801EE0F4;
                work->unk0[5]->unk12 = 0x58;
                work->unk0[5]->unkC = 0x30;
            }
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk22 = 0x80;
            }
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1b4_op83(s32 mode, Fx1b4Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->unkC->unk38;
        work->unk4.vy = env->unkC->unk3A;
        D_801E8654 = env;
        work->unk4.vz = env->unkC->unk3C;
        work->unk4.vy -= 0x258;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2B), (s32)&work->unk4);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E8540);

INCLUDE_ASM("asm/us/fx_1b4/nonmatchings/fx_1b4", func_801E8558);

INCLUDE_RODATA("asm/us/fx_1b4/nonmatchings/fx_1b4", fx_1b4_headerPtr);

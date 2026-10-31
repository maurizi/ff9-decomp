#include "fxmod/fx_0b8.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b8_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_0b8_op80(s32 mode, Fx0b8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    VECTOR sc;
    SVECTOR p2;
    SVECTOR ang;
    SVECTOR rot;
    CVECTOR col;
    SVECTOR ang2;
    VECTOR scale;
    Pool6CEntry *p;
    Fx0b8Bolt *b;
    Fx0b8Unit *u;
    Fx0b8Band *bd;
    u8 *tr;
    SVECTOR *d;
    u8 *q;
    s32 c;
    s32 n;
    s32 y_1;
    s32 y_2;
    s32 y_3;
    s32 k_1;
    s32 k_3;
    s32 frame;
    s32 i;
    s32 j;
    s32 r;
    s32 a;
    s32 x;
    s32 dy;
    s32 lo;
    s32 hi;

    if (mode == 0) {
        *arg4 = 0x1EC;
    } else if (mode == 1) {
        fx_0b8_env = env;
        ex->btlfx_initEffModels(fx_0b8_effModels, 0x20);
        for (i = 0x27, q = &work->unk0[0x27]; i >= 0; i--, q--) {
            *q = 0;
        }
        for (i = 0; i < 8; i++) {
            D_801EC1C0[i].unkDC = -i * 2;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801EC1C0[i], 0);
        }
        work->unk1E4 = 8;
        work->unk1E8 = 8;
        D_801EC1B0 = 2;
        for (i = 0; i < env->targets->targetCount; i++) {
            D_801EC8C0[i].unk1C = ex->btlfx_randRange(0x200, 0x600);
            D_801EC8C0[i].unk1E = ex->btlfx_randRange(0x600, 0xA00);
            D_801EC8C0[i].unk20 = 0;
            D_801EC8C0[i].unk2C = ex->btlfx_randRange(-0x40, 0x40);
            D_801EC8C0[i].unk2E = ex->btlfx_randRange(-0x40, 0x40);
            D_801EC8C0[i].unk30 = ex->btlfx_randRange(-0x40, 0x40);
            ex->btlfx_getUnitScale(i, (VECTOR *)&D_801EC8C0[i].unk8);
            ex->btlfx_getUnitRot(i, &D_801EC8C0[i].unk0.vx);
            ex->btlfx_getUnitRot(i, &D_801EC8C0[i].unk24.vx);
        }
        for (i = 0; i < 0x20; i++) {
            r = ex->btlfx_rand() & 0xFFF;
            a = (ex->btlfx_rand() & 0xDFF) + 0xB00;
            D_801EA7B0[i].vx = (ex->math_sin(a) * r) >> 12;
            D_801EA7B0[i].vz = ((ex->math_cos(a) * r) >> 12) + 0x418;
            D_801EA7B0[i].vy = 0;
        }
        work->pos1A8.vx = 0;
        work->pos1A8.vy = -0xC00;
        work->pos1A8.vz = 0x418;
        work->pos1BC.vx = 0;
        work->pos1BC.vy = -0x2500;
        work->pos1BC.vz = -0x1000;
        work->unk1C4.vx = 0;
        work->unk1C4.vy = -0x2500;
        work->unk1C4.vz = -0xE00;
        work->pos1CC.vx = -0x180;
        work->pos1CC.vy = -0x2C40;
        work->pos1CC.vz = -0xC00;
        work->pos1D4.vx = 0x180;
        work->pos1D4.vy = -0x2C40;
        work->pos1D4.vz = -0xC00;
        work->pos1DC.vx = 0;
        work->pos1DC.vy = -0x2500;
        work->pos1DC.vz = -0xA00;
        work->modelCC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x84), 0xFF);
        work->modelD0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 0xFF);
    } else {
        frame = entry->frame;
        env->camera->points[3].vx = work->pos13C.vx;
        env->camera->points[3].vy = work->pos13C.vy;
        env->camera->points[3].vz = work->pos13C.vz;
        switch (frame) {
        case 0:
            work->model28 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8A), 1);
            work->pos44.vy = -0x900;
            work->pos44.vx = 0;
            work->pos44.vz = 0x1000;
            work->scale34.vx = 0xC00;
            work->scale34.vy = 0x1000;
            work->scale34.vz = 0xC00;
            work->rot2C.vx = 0;
            work->rot2C.vy = 0;
            work->rot2C.vz = 0;
            work->model54 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x89), 0xB6, 0x3D40, 0, 0);
            work->model58 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x89), 0xB6, 0x3D40, 0, 0);
            work->rot5C.vx = -0x400;
            work->rot5C.vy = 0x500;
            work->rot64.vy = 0x300;
            work->pos6C.vy = -0x1100;
            work->pos7C.vy = -0x100;
            work->rot5C.vz = 0;
            work->rot64.vx = -0x400;
            work->rot64.vz = 0;
            work->pos6C.vx = 0;
            work->pos6C.vz = 0x1000;
            work->pos7C.vx = 0;
            work->pos7C.vz = 0x1000;
            work->scale8C.vx = 0xF00;
            work->scale8C.vy = 0xF00;
            work->scale8C.vz = 0xF00;
            work->modelD4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 0xFF);
            work->rotBC.vy = 0x800;
            work->rotBC.vx = 0;
            work->rotBC.vz = 0;
            work->pos9C.vx = 0;
            work->pos9C.vy = -0x2000;
            work->pos9C.vz = -0x1000;
            work->posC4.vx = 0;
            work->posC4.vy = -0x2000;
            work->posC4.vz = -0x1000;
            work->scaleAC.vx = 0x1000;
            work->scaleAC.vy = 0x1000;
            work->scaleAC.vz = 0x1000;
            work->scaleD8.vx = 0x1000;
            work->scaleD8.vy = 0x1000;
            work->scaleD8.vz = 0x1000;
            work->model128 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x10), 0x17C, -1, 0x10, 0x80);
            work->model12C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x8E), 0x17C, -1, 0x10, 0x80);
            work->model130 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x8C), 0x17C, -1, 0x10, 0x80);
            work->model134 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x8D), 0x17C, -1, 0x10, 0x80);
            work->model138 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x8B), 0x17C, -1, 0x10, 0x80);
            work->scale14C.vx = 0;
            work->scale14C.vy = 0;
            work->scale14C.vz = 0;
            work->rot15C.vx = 0;
            work->rot15C.vy = 0;
            work->rot15C.vz = 0;
            work->pos13C.vx = 0;
            work->pos13C.vy = -0x2500;
            work->pos13C.vz = -0x1000;
            work->pos164.vx = 0;
            work->pos164.vy = -0x2500;
            work->pos164.vz = -0x1000;
            work->modelE8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x88), 0xFF);
            work->modelEC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 1);
            work->modelF0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
            work->modelF4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 1);
            work->modelF8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 1);
            work->unk124 = 0x800;
            work->scale10C.vx = 0;
            work->scale10C.vy = 0;
            work->scale10C.vz = 0;
            work->rot11C.vx = 0;
            work->rot11C.vy = 0;
            work->rot11C.vz = 0;
            work->posFC.vx = 0;
            work->posFC.vy = -0x1B00;
            work->posFC.vz = 0x1000;
            break;
        case 0x7C:
            work->pos1A0.vy = -0x246A;
            work->pos1A0.vz = 0x418;
            work->scale14C.vx = 0x555;
            work->scale14C.vy = 0x555;
            work->scale14C.vz = 0x555;
            work->pos1A0.vx = 0;
            work->pos13C.vx = 0;
            work->pos13C.vy = -0x246A;
            work->pos13C.vz = 0x418;
            work->pos164.vx = 0;
            work->pos164.vy = -0x246A;
            work->unk124 = 0x555;
            work->rot11C.vx = 0x800;
            work->posFC.vy = -0x186A;
            work->scale8C.vx = 0xC00;
            work->scale8C.vy = 0xC00;
            work->pos164.vz = 0x418;
            work->scale8C.vz = 0xC00;
            work->pos6C.vy = -0x286A;
            work->rot11C.vy = 0;
            work->rot11C.vz = 0;
            work->posFC.vx = 0;
            work->posFC.vz = 0x418;
            work->pos6C.vx = 0;
            work->pos6C.vz = 0x418;
            work->rot5C.vx = 0x400;
            work->rot5C.vy = 0;
            work->rot5C.vz = 0;
            work->model180 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x85), 0x3A, 0x3D01, 0, 0);
            work->model170 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x11));
            work->model16C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x12), 0x3A, 0x3D01, 0, 0);
            work->model17C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x13), 0xBB, 0x3DC0, 0, 0);
            break;
        }
        switch (frame) {
        case 0:
            work->unk0[0] = 1;
            work->unk0[2] = 0x2C;
            work->unk0[0x11] = 0x4C;
            work->unk0[8] = 1;
            work->unk0[9] = 1;
            work->unk0[0x12] = 0x30;
            break;
        case 4:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->posC4);
            break;
        case 0x20:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos1DC);
            if (p != NULL) {
                p->unk14 = (s32)D_801ECA60;
                p->unk12 = 0x28;
                p->unkC = 0x5C;
            }
            break;
        case 0x28:
            work->unk0[0xA] = 1;
            work->unk0[0xB] = 8;
            break;
        case 0x34:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos1CC);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos1D4);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            work->emit1B8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos1BC);
            break;
        case 0x3F:
            D_801ED8C0 = 4;
            work->unk0[0xD] = 8;
            break;
        case 0x40:
            work->unk0[0xC] = 8;
            work->unk0[0xE] = 0x10;
            break;
        case 0x44:
            work->unk0[1] = 1;
            break;
        case 0x4C:
            work->unk0[0x11] = 0;
            break;
        case 0x50:
            work->unk0[0x10] = 0x10;
            break;
        case 0x64:
            work->unk0[5] = 1;
            work->unk0[4] = 0x10;
            work->unk0[3] = 1;
            break;
        case 0x67:
            work->unk0[0xA] = 0;
            work->unk0[0xF] = 0x10;
            break;
        case 0x74:
            work->unk0[7] = 8;
            break;
        case 0x7C:
            work->unk0[0] = 0;
            work->unk0[8] = 0;
            work->unk0[5] = 0;
            work->unk0[0xA] = 0;
            work->unk0[9] = 0;
            work->unk0[0x1F] = 1;
            work->unk0[6] = 1;
            work->unk0[4] = 0x10;
            work->unk0[3] = 1;
            break;
        case 0x8C:
            work->unk0[0xA] = 1;
            work->unk0[0x22] = 1;
            work->unk0[0x23] = 0x30;
            work->emit19C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos1A0);
            break;
        case 0x90:
            work->unk0[0x1C] = 0x20;
            break;
        case 0xA5:
            work->unk0[0x1F] = 0;
            work->unk0[6] = 0;
            work->unk0[3] = 0;
            D_801ED8C0 = 4;
            work->unk0[0xD] = 8;
            break;
        case 0xAE:
            work->emit184 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos188);
            work->emit190 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos194);
            if (work->emit184 != NULL) {
                work->emit184->otZ = 0x80;
            }
            if (work->emit190 != NULL) {
                work->emit190->otZ = 0x80;
            }
            work->unk0[0x1E] = 0x1E;
            work->unk0[0x13] = 1;
            work->unk0[0x14] = 0x10;
            work->unk0[0x24] = 0x10;
            work->unk0[0x1B] = 0x30;
            break;
        case 0xBC:
            work->emit1B0 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos1A8);
            if (p != NULL) {
                *(s32 *)&p->file->unk8 = (s32)fx_0b8_op80ParticleMotion;
            }
            work->unk0[0x21] = 0x10;
            break;
        case 0xCC:
            work->unk0[0x1D] = 0x10;
            break;
        case 0xE2:
            work->unk0[0x16] = 0x3C;
            work->unk0[0x17] = 0x10;
            work->unk0[6] = 0;
            work->unk0[3] = 0;
            work->unk0[0x1F] = 0;
            work->unk0[0x19] = 0x40;
            ex->btlfx_stopEmitter(work->emit1B0);
            work->scale14C.vx = 0x555;
            work->scale14C.vy = 0x555;
            work->scale14C.vz = 0x555;
            work->pos13C.vx = 0;
            work->pos13C.vy = -0x800;
            work->pos13C.vz = 0x418;
            work->pos164.vx = 0;
            work->pos164.vy = -0x800;
            work->pos164.vz = 0x418;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos164);
            if (p != NULL) {
                p->otZ = 0x400;
            }
            break;
        case 0xF2:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos164);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos164);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos164);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 0xF6:
            work->unk0[0x1A] = 8;
            break;
        case 0xFE:
            work->unk0[0xA] = 0;
            work->unk0[0x13] = 0;
            break;
        case 0x10E:
            work->unk0[0x18] = 0x10;
            break;
        case 0x11E:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitScale(i, (s32 *)&D_801EC8C0[i].unk8);
                ex->btlfx_setUnitRot(i, &D_801EC8C0[i].unk0.vx);
            }
            work->unk0[0x20] = 8;
            break;
        }
        if (work->unk0[8] != 0) {
            func_801EA424(0xFFF);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            work->scaleAC.vy = fxm_easeLerp(0, 0x2C, work->unk0[2], 0x800, 0x1000);
        }
        if (work->unk0[0] != 0) {
            ex->Hi_DrawSummonModel(&work->rotBC, &work->pos9C, (s32)&work->scaleAC, SUMMON(env)->unk60, 1);
            if (work->unk0[1] == 0) {
                ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelCC);
            } else {
                ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelD0);
            }
            ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelD4);
        }
        if (work->unk0[9] != 0) {
            work->rot2C.vy += 2;
            ex->Hi_DrawEffModel(&work->rot2C, &work->pos44, (s32)&work->scale34, work->model28);
            ex->Hi_DrawEffModel(&work->rot5C, &work->pos6C, (s32)&work->scale8C, work->model54);
            ex->Hi_DrawEffModel(&work->rot64, &work->pos7C, (s32)&work->scale8C, work->model58);
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
        }
        if (work->unk0[0x1F] != 0) {
            ex->Hi_DrawEffModel(&work->rot5C, &work->pos6C, (s32)&work->scale8C, work->model54);
        }
        if (work->unk0[0x11] != 0) {
            work->unk0[0x11]--;
            if (work->unk0[0x11] < 0x10) {
                c = fxm_easeLerp(2, 0x10, work->unk0[0x11], 0xFF, 0);
            } else {
                c = 0xFF;
            }
            y_1 = fxm_easeLerp(0, 0x4C, work->unk0[0x11], 0, 0x28);
            func_801EA1E0(0x10, y_1 + 0x8E, 0x62, (frame * 4) & 0xFC, c >> 2, c >> 1, 0x18, 0x3D09, 1);
            func_801EA1E0(0x20, y_1 + 0x74, 0x80, (frame * 2) & 0xFE, c >> 1, c, 0x18, 0x3D09, 1);
        }
        if (work->unk0[0x12] != 0) {
            work->unk0[0x12]--;
            if (work->unk0[0x12] >= 0x29) {
                func_801EA4E0(0, 0xD0, work->unk0[0x12] - 0x28, 8, 1, 0);
            }
            for (i = 0; i < 4; i++) {
                bd = &D_801EA6D8[i];
                bd->unk0 += bd->unk18;
                bd->unk8 = (bd->unk8 + bd->unk14) & 0xFF;
                func_801EA1E0(0, bd->unk0, bd->unk4, bd->unk8, bd->unkC, bd->unk10, 0x18, 0x3D09, 1);
            }
        }
        if (work->unk0[0xE] != 0) {
            work->unk0[0xE]--;
            for (i = 0; i < 8; i++) {
                b = &D_801EC1C0[i];
                if (b->unkDC > 0) {
                    tr = (u8 *)b;
                    b->unkDC--;
                    b->unk0 = b->unk2 = 8;
                    b->unk1 = 0;
                    dy = ((work->pos13C.vz + 0x1000) << 12) / 9;
                    for (d = b->unk4, y_2 = -0x1000 << 12, j = 0; j < 9; d++, j++) {
                        d->vx = b->unk4C[j];
                        d->vy = b->unk70[j];
                        d->vz = y_2 >> 12;
                        y_2 += dy;
                        b->unk4C[j] += b->unk94[j];
                        b->unk70[j] += b->unkB8[j];
                        d->pad = 0x100;
                    }
                    if (b->unkDC < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), 0, NULL, frame, 0x1000,
                                            b->unkDC << 10, 1, tr, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 3), 0, NULL, frame, 0x1000, 0x1000,
                                            1, tr, 0);
                    }
                } else if (b->unkDC < 0) {
                    b->unkDC++;
                } else if (work->unk0[0xE] >= 9) {
                    s32 dx;
                    s32 dz;
                    b->unkDC = ex->btlfx_randRange(4, 8);
                    k_1 = ex->math_sin(ex->btlfx_rand() & 0xFFF);
                    b->unk4C[8] = k_1 >> 5;
                    b->unk4C[0] = k_1 >> 3;
                    k_1 = ex->math_cos(ex->btlfx_rand() & 0xFFF);
                    b->unk70[0] = (k_1 >> 5) - 0x2500;
                    b->unk70[8] = (k_1 >> 3) - 0x2500;
                    b->unk94[0] = 0;
                    b->unkB8[0] = 0;
                    b->unk94[8] = 0;
                    b->unkB8[8] = 0;
                    x = b->unk4C[0] << 12;
                    y_3 = b->unk70[0] << 12;
                    dx = ((b->unk4C[8] - b->unk4C[0]) << 12) / 9;
                    dz = ((b->unk70[8] - b->unk70[0]) << 12) / 9;
                    for (j = 1; j < 8; j++) {
                        x += dx;
                        y_3 += dz;
                        hi = j * 0x20 + 0x20;
                        lo = -j * 0x20 - 0x20;
                        b->unk94[j] = ex->btlfx_randRange(lo, hi);
                        b->unkB8[j] = ex->btlfx_randRange(lo, hi);
                        b->unk4C[j] = (x >> 12) + b->unk94[j];
                        b->unk70[j] = (y_3 >> 12) + b->unkB8[j];
                    }
                }
            }
        }
        work->rot11C.vy -= 0x10;
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            work->scale10C.vx = work->scale10C.vz = fxm_easeLerp(2, 0x10, work->unk0[4], 0, work->unk124);
            work->scale10C.vy = fxm_easeLerp(1, 0x10, work->unk0[4], 0, work->unk124);
        }
        if (work->unk0[3] != 0) {
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF8);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF4);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF0);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelEC);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelE8);
        }
        if (work->unk0[0xB] != 0) {
            work->unk0[0xB]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 8, work->unk0[0xB], 0, 0x800);
        }
        if (work->unk0[0x10] != 0) {
            work->unk0[0x10]--;
            if (work->unk0[0x10] >= 9) {
                work->scale14C.vx = work->scale14C.vy = work->scale14C.vz =
                    fxm_easeLerp(1, 8, work->unk0[0x10] - 8, 0x800, 0xC00);
            } else {
                work->scale14C.vx = work->scale14C.vy = work->scale14C.vz = fxm_easeLerp(2, 8, work->unk0[0x10], 0xC00, 0x1800);
            }
        }
        if (work->unk0[0xC] != 0) {
            work->unk0[0xC]--;
            if (work->unk0[0xC] >= 5) {
                work->scale14C.vz = fxm_easeLerp(2, 4, work->unk0[0xC] - 4, 0x800, 0x2800);
            } else {
                work->scale14C.vz = fxm_easeLerp(2, 4, work->unk0[0xC], 0x2800, 0x800);
            }
            work->pos164.vz = work->pos13C.vz = fxm_easeLerp(2, 8, work->unk0[0xC], -0x1000, 0x1000);
            ex->btlfx_moveEmitterTo((s32)work->emit1B8, (s32)&work->pos164);
        }
        if (work->unk0[0xF] != 0) {
            work->unk0[0xF]--;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1F), frame);
            k_3 = fxm_easeLerp(2, 0x10, work->unk0[0xF], 0x1000, 0);
            work->pos164.vy = work->pos13C.vy = fxm_easeLerp(1, 0x10, work->unk0[0xF], -0x2500, -0x1000);
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 0x10, work->unk0[0xF], 0x1800, 0);
            switch (k_3 >> 10) {
            case 3:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model128, work->model12C,
                                         (k_3 & 0x3FF) << 2);
                break;
            case 2:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model12C, work->model130,
                                         (k_3 & 0x3FF) << 2);
                break;
            case 1:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model130, work->model134,
                                         (k_3 & 0x3FF) << 2);
                break;
            case 0:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model134, work->model138,
                                         (k_3 & 0x3FF) << 2);
                break;
            }
        }
        if (work->unk0[0x23] != 0) {
            work->unk0[0x23]--;
            if (work->unk0[0x23] >= 0x19) {
                work->pos13C.vy = fxm_easeLerp(1, 0x18, work->unk0[0x23] - 0x18, -0x246A, -0x1300);
                work->scale14C.vy = fxm_easeLerp(1, 0x18, work->unk0[0x23] - 0x18, 0x7FE, 0x555);
            } else {
                work->pos13C.vy = fxm_easeLerp(2, 0x18, work->unk0[0x23], -0x1300, -0xC00);
            }
            work->pos164.vy = work->pos13C.vy;
            ex->btlfx_moveEmitterTo((s32)work->emit19C, (s32)&work->pos164);
        }
        if (work->unk0[0x24] != 0) {
            work->unk0[0x24]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(0, 0x10, work->unk0[0x24], 0x555, 0x555);
        }
        if (work->unk0[0x1A] != 0) {
            work->unk0[0x1A]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 8, work->unk0[0x1A], 0x555, 0);
        }
        if (work->unk0[0xA] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1F), frame);
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model128);
        }
        if (work->unk0[0x1C] != 0) {
            work->unk0[0x1C]--;
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model180);
            if (work->unk0[0x1C] < 8) {
                c = (work->unk0[0x1C] - 8) << 5;
                ex->Hi_ModifyEffModelRGB(work->model180, c, c, c);
            }
        }
        if (work->unk0[0x1E] != 0) {
            work->unk0[0x1E]--;
            ex->btlfx_moveEmitterTo((s32)work->emit184, (s32)&work->pos164);
            ex->btlfx_moveEmitterTo((s32)work->emit190, (s32)&work->pos164);
        }
        if (work->unk0[0x13] != 0) {
            pos.vx = work->pos13C.vx;
            pos.vy = work->pos13C.vy;
            pos.vz = work->pos13C.vz;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, &pos.vx, &work->rot174.vx);
            ex->Hi_DrawEffModel(&work->rot174, &work->pos13C, (s32)&work->scale14C, work->model170);
            ex->Hi_DrawEffModel(&work->rot174, &work->pos13C, (s32)&work->scale14C, work->model16C);
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model17C);
        }
        if (work->unk0[0x14] != 0) {
            work->unk0[0x14]--;
            c = -(work->unk0[0x14] * 0x10);
            ex->Hi_ModifyEffModelRGB(work->model170, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->model16C, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->model17C, c, c, c);
        }
        if (work->unk0[0x1B] != 0) {
            work->unk0[0x1B]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                r = fxm_easeLerp(2, 0x30, work->unk0[0x1B], 0x1000, 0x200);
                p2.vy = work->pos164.vy + ((ex->math_sin(D_801EC8C0[i].unk1C) * r) >> 12);
                r = abs((ex->math_cos(D_801EC8C0[i].unk1C) * r) >> 12);
                p2.vx = work->pos164.vx + ((ex->math_sin(D_801EC8C0[i].unk1E) * r) >> 12);
                p2.vz = work->pos164.vz + ((ex->math_cos(D_801EC8C0[i].unk1E) * r) >> 12);
                ex->btlfx_setUnitPos(i, &p2.vx);
                sc.vx = sc.vy = sc.vz = fxm_easeLerp(0, 0x30, work->unk0[0x1B], D_801EC8C0[i].unk8.vy, 0);
                ex->btlfx_setUnitScale(i, (s32 *)&sc);
                D_801EC8C0[i].unk24.vx += D_801EC8C0[i].unk2C;
                D_801EC8C0[i].unk24.vy += D_801EC8C0[i].unk2E;
                D_801EC8C0[i].unk24.vz += D_801EC8C0[i].unk30;
                ex->btlfx_setUnitRot(i, &D_801EC8C0[i].unk24.vx);
            }
        }
        c = 0xFFF;
        if (work->unk0[0x16] != 0) {
            work->unk0[0x16]--;
            if (work->unk0[0x17] != 0) {
                work->unk0[0x17]--;
                c = (0x10 - work->unk0[0x17]) << 8;
            }
            if (work->unk0[0x18] != 0) {
                work->unk0[0x18]--;
                c = work->unk0[0x18] << 8;
            }
            ex->btlfx_aimAngles(&work->pos164.vx, (s16 *)&env->camera->eye, &rot.vx);
            ang.vz = 0xC18;
            ang.vx = 0;
            ang.vy = -0x1200;
            rot.vz = 0;
            rot.vx += 0xA0;
            func_801E9D80(&ang, &rot, c, 0x17, 0x3D09, 1);
            ang.vy = 0x200;
            rot.vz = 0x800;
            rot.vx -= 0x140;
            func_801E9D80(&ang, &rot, c, 0x17, 0x3D09, 1);
        }
        if (work->unk0[0x20] != 0) {
            work->unk0[0x20]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitHomePos(i, &D_801EC8C0[i].unk14.vx);
                D_801EC8C0[i].unk14.vy -= ex->math_sin(work->unk0[0x20] << 7) >> 1;
                ex->btlfx_setUnitPos(i, &D_801EC8C0[i].unk14.vx);
                if (work->unk0[0x20] == 0) {
                    D_801EC8C0[i].unk14.vy = 0;
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&D_801EC8C0[i].unk14);
                    if (p != NULL) {
                        p->otZ = -0x100;
                    }
                }
            }
        }
        if (work->unk0[0x21] != 0) {
            work->unk0[0x21]--;
            if (work->unk0[0x21] != 0) {
                if (work->emit19C != NULL) {
                    work->emit19C->unk20 = work->unk0[0x21] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(work->emit19C);
            }
        }
        if (work->unk0[0x1D] != 0) {
            work->unk0[0x1D]--;
            if (work->unk0[0x1D] != 0) {
                if (work->emit184 != NULL) {
                    work->emit184->unk20 = work->unk0[0x1D] << 8;
                }
                if (work->emit190 != NULL) {
                    work->emit190->unk20 = work->unk0[0x1D] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(work->emit184);
                ex->btlfx_stopEmitter(work->emit190);
            }
        }
        if (work->unk0[0x19] != 0) {
            work->unk0[0x19]--;
            work->unk1E8 = fxm_easeLerp(1, 0x40, work->unk0[0x19], 4, 0x14);
        }
        work->unk1E4 += work->unk1E8;
        if (work->unk0[5] != 0) {
            ex->btlfx_drawScrollMove(frame * 0x10, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        }
        if (work->unk0[6] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - frame * 0x10, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        }
        if (work->unk0[0x22] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - work->unk1E4, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (work->unk0[0x13] != 0) {
            ex->btlfx_drawScrollMove(work->unk1E4, 0x2C0, 0x100, 0x38, 0x70, 0x2C0, 0x170);
        }
        if (work->unk0[0x16] != 0) {
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x18, 0x60, 0x1C0, 0x180);
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            func_801EA4E0(0xFF, 0, work->unk0[7], 8, 1, 0);
        }
        if (work->unk0[0xD] != 0) {
            s32 sh;
            work->unk0[0xD]--;
            sh = D_801ED8C0;
            n = work->unk0[0xD];
            scale.vz = scale.vy = scale.vx = n * 3 + 0x1000;
            col.r = col.g = col.b = n << sh;
            scale.vz = scale.vy = scale.vx = 0x1000;
            ang2.vx = 0;
            ang2.vy = 0;
            ang2.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &ang2, &scale, &col, 1, NULL);
        }
        return frame >= 0x134;
    }
    return 0;
}

void func_801E9D80(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr) {
    SVECTOR v[4];
    SVECTOR t;
    MATRIX m;
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env;
    BtlfxExports *ex;
    POLY_GT4 *p;
    s32 i;
    s32 j;
    s32 k0;
    s32 k1;

    env = fx_0b8_env;
    ex = BTLFX_EXPORTS(env);
    uv0.u = uv2.u = 0;
    uv1.u = uv3.u = 0x5F;
    uv0.v = uv1.v = 0;
    uv2.v = uv3.v = 0x5F;
    v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    tpage |= (abr & 3) << 5;
    p = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(p + 32);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    n /= 8;
    k0 = 0;
    k1 = n;
    for (j = 0; j < 8; j++) {
        t.vx = D_801EA680[j].vx;
        t.vy = D_801EA680[j].vy;
        t.vz = D_801EA680[j].vz;
        c0.r = (t.vx * k0) >> 12;
        c0.g = (t.vy * k0) >> 12;
        c0.b = (t.vz * k0) >> 12;
        t.vx = D_801EA680[j + 1].vx;
        t.vy = D_801EA680[j + 1].vy;
        t.vz = D_801EA680[j + 1].vz;
        c1.r = (t.vx * k1) >> 12;
        c1.g = (t.vy * k1) >> 12;
        c1.b = (t.vz * k1) >> 12;
        k0 += n;
        k1 += n;
        v[0].vz = v[1].vz = j * 0xA00 - 0x2800;
        v[2].vz = v[3].vz = j * 0xA00 - 0x1E00;
        for (i = 0; i < 4; i++) {
            v[0].vx = v[2].vx = i * 0x1400 - 0x2800;
            v[1].vx = v[3].vx = i * 0x1400 - 0x1400;
            gte_ldv3c(v);
            gte_rtpt();
            p->code = (abr != 0xFF) ? 0x3E : 0x3C;
            p->r0 = p->r1 = c0.r;
            p->g0 = p->g1 = c0.g;
            p->b0 = p->b1 = c0.b;
            p->r2 = p->r3 = c1.r;
            p->g2 = p->g3 = c1.g;
            p->b2 = p->b3 = c1.b;
            gte_stsxy3(&p->x0, &p->x1, &p->x2);
            gte_ldv0(&v[3]);
            gte_rtps();
            *(u16 *)&p->u0 = *(u16 *)&uv0;
            *(u16 *)&p->u1 = *(u16 *)&uv1;
            *(u16 *)&p->u2 = *(u16 *)&uv2;
            *(u16 *)&p->u3 = *(u16 *)&uv3;
            p->clut = clut;
            p->tpage = tpage;
            gte_stsxy(&p->x3);
            ex->btlfx_addPrim(0x0C000000, 0, p);
            p++;
        }
    }
}

void func_801EA1E0(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr) {
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    BtlfxExports *ex;
    Unk800F78A4 *env;
    POLY_GT4 *gt4;
    POLY_F4 *f4;
    s32 i;

    env = fx_0b8_env;
    ex = BTLFX_EXPORTS(env);
    uv0.u = uv2.u = 0;
    uv1.u = uv3.u = 0xFF;
    uv0.v = uv1.v = 0x80;
    uv2.v = uv3.v = 0xFF;
    tpage |= (abr & 3) << 5;
    gt4 = (POLY_GT4 *)env->prim;
    f4 = (POLY_F4 *)(gt4 + 3);
    env->prim = (u8 *)(f4 + 3);
    for (i = 0; i < 3; gt4++, i++, f4++) {
        gt4->clut = clut;
        gt4->tpage = tpage;
        gt4->code = (abr != 0xFF) ? 0x3E : 0x3C;
        f4->code = (abr != 0xFF) ? 0x2A : 0x28;
        gt4->r0 = gt4->r1 = gt4->g0 = gt4->g1 = gt4->b0 = gt4->b1 = c0;
        f4->r0 = f4->g0 = f4->b0 = gt4->r2 = gt4->r3 = gt4->g2 = gt4->g3 = gt4->b2 = gt4->b3 = c1;
        *(u16 *)&gt4->u0 = *(u16 *)&uv0;
        *(u16 *)&gt4->u1 = *(u16 *)&uv1;
        *(u16 *)&gt4->u2 = *(u16 *)&uv2;
        *(u16 *)&gt4->u3 = *(u16 *)&uv3;
        f4->x0 = f4->x2 = gt4->x0 = gt4->x2 = (x | ~0xFF) + i * 0x100;
        f4->x1 = f4->x3 = gt4->x1 = gt4->x3 = gt4->x0 + 0x100;
        gt4->y0 = gt4->y1 = y;
        f4->y0 = f4->y1 = gt4->y2 = gt4->y3 = y + h;
        f4->y2 = f4->y3 = gt4->y2 + 0x130;
        ex->btlfx_addPrim(0x0C000000, ot * 4, gt4);
        ex->btlfx_addPrim(0x05000000, ot * 4, f4);
    }
}

void func_801EA424(s32 ot) {
    Unk800F78A4 *env = fx_0b8_env;
    POLY_G4 *p = (POLY_G4 *)env->prim;
    BtlfxExports *ex;

    env->prim = (u8 *)(p + 1);
    ex = BTLFX_EXPORTS(env);
    p->code = GPU_POLY_G4;
    p->r0 = p->r1 = 0xE6;
    p->g0 = p->g1 = 0xCD;
    p->b0 = p->b1 = 0x96;
    p->r2 = p->r3 = 0xB4;
    p->g2 = p->g3 = 0xA0;
    p->b2 = p->b3 = 0x78;
    p->x1 = p->x3 = 320;
    p->x0 = p->x2 = 0;
    p->y0 = p->y1 = 0;
    p->y2 = p->y3 = 240;
    ex->btlfx_addPrim(0x08000000, ot * 4, p);
}

void func_801EA4E0(s32 a, s32 b, s32 t, s32 n, s32 mode, s32 ot) {
    Unk800F78A4 *env = fx_0b8_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 v = ex->btlfx_lerpRatio(a, b, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), mode, v, v, v);
}

static s32 fx_0b8_op80ParticleMotion(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b8_env);
    SVECTOR v;
    s32 r;
    SVECTOR off;

    SVECTOR *a = &D_801EA7B0[(pt->unk1F - 1) & 0x1F];

    off.vx = 0;
    off.vy = -0xC00;
    off.vz = 0x418;
    ex->btlfx_lerpVec((s16 *)a, (s16 *)&off,
                                             ((0x1000000 / n) * frame) >> 12, (s16 *)&v);
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    pt->pos.vx = v.vx << 12;
    pt->pos.vy = v.vy << 12;
    pt->pos.vz = v.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b8/nonmatchings/fx_0b8", fx_0b8_headerPtr);

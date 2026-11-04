#include "fxmod/fx_0b8.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b8_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_0b8_op80(s32 mode, Fx0b8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    VECTOR sc;
    SVECTOR unitPos;
    SVECTOR ang;
    SVECTOR rot;
    CVECTOR col;
    SVECTOR ang2;
    VECTOR scale;
    Pool6CEntry *emitter;
    Fx0b8Bolt *bolt;
    Fx0b8Unit *unit;
    Fx0b8Band *bd;
    u8 *tr;
    SVECTOR *pt;
    u8 *flag;
    s32 bright;
    s32 fade;
    s32 bandY;
    s32 zAcc;
    s32 yAcc;
    s32 rndTrig;
    s32 morphT;
    s32 frame;
    s32 i;
    s32 j;
    s32 radius;
    s32 dirAng;
    s32 xAcc;
    s32 dy;
    s32 lo;
    s32 hi;

    if (mode == 0) {
        *args = 0x1EC;
    } else if (mode == 1) {
        fx_0b8_env = env;
        ex->btlfx_initEffModels(fx_0b8_effModels, 0x20);
        for (i = 0x27, flag = &work->flags0[0x27]; i >= 0; i--, flag--) {
            *flag = 0;
        }
        for (i = 0; i < 8; i++) {
            fx_0b8_bolts[i].timerDC = -i * 2;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_STREAK), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_0b8_bolts[i], 0);
        }
        work->scroll1E4 = 8;
        work->scrollSpeed1E8 = 8;
        fx_0b8_scrollInitWord = 2;
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_0b8_units[i].pitch1C = ex->btlfx_randRange(0x200, 0x600);
            fx_0b8_units[i].yaw1E = ex->btlfx_randRange(0x600, 0xA00);
            fx_0b8_units[i].unk20 = 0;
            fx_0b8_units[i].spinX2C = ex->btlfx_randRange(-0x40, 0x40);
            fx_0b8_units[i].spinY2E = ex->btlfx_randRange(-0x40, 0x40);
            fx_0b8_units[i].spinZ30 = ex->btlfx_randRange(-0x40, 0x40);
            ex->btlfx_getUnitScale(i, (VECTOR *)&fx_0b8_units[i].scale8);
            ex->btlfx_getUnitRot(i, &fx_0b8_units[i].rot0.vx);
            ex->btlfx_getUnitRot(i, &fx_0b8_units[i].rot24.vx);
        }
        for (i = 0; i < 0x20; i++) {
            radius = ex->btlfx_rand() & 0xFFF;
            dirAng = (ex->btlfx_rand() & 0xDFF) + 0xB00;
            fx_0b8_particleFrom[i].vx = (ex->math_sin(dirAng) * radius) >> 12;
            fx_0b8_particleFrom[i].vz = ((ex->math_cos(dirAng) * radius) >> 12) + 0x418;
            fx_0b8_particleFrom[i].vy = 0;
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
        work->modelCC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_PINE_TREES), 0xFF);
        work->modelD0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_PINE_TREES_2), 0xFF);
    } else {
        frame = entry->frame;
        env->camera->points[3].vx = work->pos13C.vx;
        env->camera->points[3].vy = work->pos13C.vy;
        env->camera->points[3].vz = work->pos13C.vz;
        switch (frame) {
        case 0:
            work->model28 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_RUBBLE_BAND), 1);
            work->pos44.vy = -0x900;
            work->pos44.vx = 0;
            work->pos44.vz = 0x1000;
            work->scale34.vx = 0xC00;
            work->scale34.vy = 0x1000;
            work->scale34.vz = 0xC00;
            work->rot2C.vx = 0;
            work->rot2C.vy = 0;
            work->rot2C.vz = 0;
            work->model54 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_CLOUD_DISC), 0xB6, 0x3D40, 0, 0);
            work->model58 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_CLOUD_DISC), 0xB6, 0x3D40, 0, 0);
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
            work->modelD4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_GROUND_CHUNK), 0xFF);
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
            work->model128 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_SPHERE), 0x17C, -1, 0x10, 0x80);
            work->model12C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_WHITE_DROP_4), 0x17C, -1, 0x10, 0x80);
            work->model130 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_WHITE_DROP_2), 0x17C, -1, 0x10, 0x80);
            work->model134 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_WHITE_DROP_3), 0x17C, -1, 0x10, 0x80);
            work->model138 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_WHITE_DROP), 0x17C, -1, 0x10, 0x80);
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
            work->modelE8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_LAVA_CRATER), 0xFF);
            work->modelEC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_GREY_RING), 1);
            work->modelF0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_GOLD_RING), 1);
            work->modelF4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_CRATER_MOUND), 1);
            work->modelF8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_WHITE_PILLAR), 1);
            work->scale124 = 0x800;
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
            work->scale124 = 0x555;
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
            work->model180 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_PURPLE_DOME), 0x3A, 0x3D01, 0, 0);
            work->model170 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_RING_FUNNEL));
            work->model16C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_CLOUD_FUNNEL), 0x3A, 0x3D01, 0, 0);
            work->model17C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_ORANGE_RING), 0xBB, 0x3DC0, 0, 0);
            break;
        }
        switch (frame) {
        case 0:
            work->flags0[0] = 1;
            work->flags0[2] = 0x2C;
            work->flags0[0x11] = 0x4C;
            work->flags0[8] = 1;
            work->flags0[9] = 1;
            work->flags0[0x12] = 0x30;
            break;
        case 4:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_FAINT_GLOW), (s32)&work->posC4);
            break;
        case 0x20:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_SPARKLES), (s32)&work->pos1DC);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0b8_trails;
                emitter->trailSlots = 0x28;
                emitter->trailStride = 0x5C;
            }
            break;
        case 0x28:
            work->flags0[0xA] = 1;
            work->flags0[0xB] = 8;
            break;
        case 0x34:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_BLAST_RINGS), (s32)&work->pos1CC);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_BLAST_RINGS), (s32)&work->pos1D4);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            work->emit1B8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_SPRITES), (s32)&work->pos1BC);
            break;
        case 0x3F:
            fx_0b8_gridShift = 4;
            work->flags0[0xD] = 8;
            break;
        case 0x40:
            work->flags0[0xC] = 8;
            work->flags0[0xE] = 0x10;
            break;
        case 0x44:
            work->flags0[1] = 1;
            break;
        case 0x4C:
            work->flags0[0x11] = 0;
            break;
        case 0x50:
            work->flags0[0x10] = 0x10;
            break;
        case 0x64:
            work->flags0[5] = 1;
            work->flags0[4] = 0x10;
            work->flags0[3] = 1;
            break;
        case 0x67:
            work->flags0[0xA] = 0;
            work->flags0[0xF] = 0x10;
            break;
        case 0x74:
            work->flags0[7] = 8;
            break;
        case 0x7C:
            work->flags0[0] = 0;
            work->flags0[8] = 0;
            work->flags0[5] = 0;
            work->flags0[0xA] = 0;
            work->flags0[9] = 0;
            work->flags0[0x1F] = 1;
            work->flags0[6] = 1;
            work->flags0[4] = 0x10;
            work->flags0[3] = 1;
            break;
        case 0x8C:
            work->flags0[0xA] = 1;
            work->flags0[0x22] = 1;
            work->flags0[0x23] = 0x30;
            work->emit19C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_SPRITES_2), (s32)&work->pos1A0);
            break;
        case 0x90:
            work->flags0[0x1C] = 0x20;
            break;
        case 0xA5:
            work->flags0[0x1F] = 0;
            work->flags0[6] = 0;
            work->flags0[3] = 0;
            fx_0b8_gridShift = 4;
            work->flags0[0xD] = 8;
            break;
        case 0xAE:
            work->emit184 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_ORANGE_STREAKS), (s32)&work->pos188);
            work->emit190 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_VIOLET_RING), (s32)&work->pos194);
            if (work->emit184 != NULL) {
                work->emit184->otZ = 0x80;
            }
            if (work->emit190 != NULL) {
                work->emit190->otZ = 0x80;
            }
            work->flags0[0x1E] = 0x1E;
            work->flags0[0x13] = 1;
            work->flags0[0x14] = 0x10;
            work->flags0[0x24] = 0x10;
            work->flags0[0x1B] = 0x30;
            break;
        case 0xBC:
            work->emit1B0 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_ROCK_DEBRIS), (s32)&work->pos1A8);
            if (emitter != NULL) {
                emitter->file->m.module.update = fx_0b8_op80ParticleMotion;
            }
            work->flags0[0x21] = 0x10;
            break;
        case 0xCC:
            work->flags0[0x1D] = 0x10;
            break;
        case 0xE2:
            work->flags0[0x16] = 0x3C;
            work->flags0[0x17] = 0x10;
            work->flags0[6] = 0;
            work->flags0[3] = 0;
            work->flags0[0x1F] = 0;
            work->flags0[0x19] = 0x40;
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_VIOLET_RINGS), (s32)&work->pos164);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case 0xF2:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_PINK_CLOUD), (s32)&work->pos164);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_PURPLE_RINGS), (s32)&work->pos164);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_SPRITES_3), (s32)&work->pos164);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case 0xF6:
            work->flags0[0x1A] = 8;
            break;
        case 0xFE:
            work->flags0[0xA] = 0;
            work->flags0[0x13] = 0;
            break;
        case 0x10E:
            work->flags0[0x18] = 0x10;
            break;
        case 0x11E:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitScale(i, (s32 *)&fx_0b8_units[i].scale8);
                ex->btlfx_setUnitRot(i, &fx_0b8_units[i].rot0.vx);
            }
            work->flags0[0x20] = 8;
            break;
        }
        if (work->flags0[8] != 0) {
            fx_0b8_drawScreenGradient(0xFFF);
        }
        if (work->flags0[2] != 0) {
            work->flags0[2]--;
            work->scaleAC.vy = fxm_easeLerp(0, 0x2C, work->flags0[2], 0x800, 0x1000);
        }
        if (work->flags0[0] != 0) {
            ex->Hi_DrawSummonModel(&work->rotBC, &work->pos9C, (s32)&work->scaleAC, SUMMON(env)->model60, 1);
            if (work->flags0[1] == 0) {
                ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelCC);
            } else {
                ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelD0);
            }
            ex->Hi_DrawEffModel(&work->rotBC, &work->pos9C, (s32)&work->scaleD8, work->modelD4);
        }
        if (work->flags0[9] != 0) {
            work->rot2C.vy += 2;
            ex->Hi_DrawEffModel(&work->rot2C, &work->pos44, (s32)&work->scale34, work->model28);
            ex->Hi_DrawEffModel(&work->rot5C, &work->pos6C, (s32)&work->scale8C, work->model54);
            ex->Hi_DrawEffModel(&work->rot64, &work->pos7C, (s32)&work->scale8C, work->model58);
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
        }
        if (work->flags0[0x1F] != 0) {
            ex->Hi_DrawEffModel(&work->rot5C, &work->pos6C, (s32)&work->scale8C, work->model54);
        }
        if (work->flags0[0x11] != 0) {
            work->flags0[0x11]--;
            if (work->flags0[0x11] < 0x10) {
                bright = fxm_easeLerp(2, 0x10, work->flags0[0x11], 0xFF, 0);
            } else {
                bright = 0xFF;
            }
            bandY = fxm_easeLerp(0, 0x4C, work->flags0[0x11], 0, 0x28);
            fx_0b8_drawScreenBand(0x10, bandY + 0x8E, 0x62, (frame * 4) & 0xFC, bright >> 2, bright >> 1, 0x18, 0x3D09, 1);
            fx_0b8_drawScreenBand(0x20, bandY + 0x74, 0x80, (frame * 2) & 0xFE, bright >> 1, bright, 0x18, 0x3D09, 1);
        }
        if (work->flags0[0x12] != 0) {
            work->flags0[0x12]--;
            if (work->flags0[0x12] >= 0x29) {
                fxm_tintScreenLerp(0, 0xD0, work->flags0[0x12] - 0x28, 8, 1, 0);
            }
            for (i = 0; i < 4; i++) {
                bd = &fx_0b8_bands[i];
                bd->y0 += bd->dy18;
                bd->x8 = (bd->x8 + bd->dx14) & 0xFF;
                fx_0b8_drawScreenBand(0, bd->y0, bd->h4, bd->x8, bd->colorC, bd->color10, 0x18, 0x3D09, 1);
            }
        }
        if (work->flags0[0xE] != 0) {
            work->flags0[0xE]--;
            for (i = 0; i < 8; i++) {
                bolt = &fx_0b8_bolts[i];
                if (bolt->timerDC > 0) {
                    tr = (u8 *)bolt;
                    bolt->timerDC--;
                    bolt->lastSlot0 = bolt->count2 = 8;
                    bolt->head1 = 0;
                    dy = ((work->pos13C.vz + 0x1000) << 12) / 9;
                    for (pt = bolt->pts4, zAcc = -0x1000 << 12, j = 0; j < 9; pt++, j++) {
                        pt->vx = bolt->x4C[j];
                        pt->vy = bolt->y70[j];
                        pt->vz = zAcc >> 12;
                        zAcc += dy;
                        bolt->x4C[j] += bolt->dx94[j];
                        bolt->y70[j] += bolt->dyB8[j];
                        pt->pad = 0x100;
                    }
                    if (bolt->timerDC < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_STREAK), 0, NULL, frame, 0x1000,
                                            bolt->timerDC << 10, 1, tr, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_STREAK), 0, NULL, frame, 0x1000, 0x1000,
                                            1, tr, 0);
                    }
                } else if (bolt->timerDC < 0) {
                    bolt->timerDC++;
                } else if (work->flags0[0xE] >= 9) {
                    s32 dx;
                    s32 dz;
                    bolt->timerDC = ex->btlfx_randRange(4, 8);
                    rndTrig = ex->math_sin(ex->btlfx_rand() & 0xFFF);
                    bolt->x4C[8] = rndTrig >> 5;
                    bolt->x4C[0] = rndTrig >> 3;
                    rndTrig = ex->math_cos(ex->btlfx_rand() & 0xFFF);
                    bolt->y70[0] = (rndTrig >> 5) - 0x2500;
                    bolt->y70[8] = (rndTrig >> 3) - 0x2500;
                    bolt->dx94[0] = 0;
                    bolt->dyB8[0] = 0;
                    bolt->dx94[8] = 0;
                    bolt->dyB8[8] = 0;
                    xAcc = bolt->x4C[0] << 12;
                    yAcc = bolt->y70[0] << 12;
                    dx = ((bolt->x4C[8] - bolt->x4C[0]) << 12) / 9;
                    dz = ((bolt->y70[8] - bolt->y70[0]) << 12) / 9;
                    for (j = 1; j < 8; j++) {
                        xAcc += dx;
                        yAcc += dz;
                        hi = j * 0x20 + 0x20;
                        lo = -j * 0x20 - 0x20;
                        bolt->dx94[j] = ex->btlfx_randRange(lo, hi);
                        bolt->dyB8[j] = ex->btlfx_randRange(lo, hi);
                        bolt->x4C[j] = (xAcc >> 12) + bolt->dx94[j];
                        bolt->y70[j] = (yAcc >> 12) + bolt->dyB8[j];
                    }
                }
            }
        }
        work->rot11C.vy -= 0x10;
        if (work->flags0[4] != 0) {
            work->flags0[4]--;
            work->scale10C.vx = work->scale10C.vz = fxm_easeLerp(2, 0x10, work->flags0[4], 0, work->scale124);
            work->scale10C.vy = fxm_easeLerp(1, 0x10, work->flags0[4], 0, work->scale124);
        }
        if (work->flags0[3] != 0) {
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF8);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF4);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelF0);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelEC);
            ex->Hi_DrawEffModel(&work->rot11C, &work->posFC, (s32)&work->scale10C, work->modelE8);
        }
        if (work->flags0[0xB] != 0) {
            work->flags0[0xB]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 8, work->flags0[0xB], 0, 0x800);
        }
        if (work->flags0[0x10] != 0) {
            work->flags0[0x10]--;
            if (work->flags0[0x10] >= 9) {
                work->scale14C.vx = work->scale14C.vy = work->scale14C.vz =
                    fxm_easeLerp(1, 8, work->flags0[0x10] - 8, 0x800, 0xC00);
            } else {
                work->scale14C.vx = work->scale14C.vy = work->scale14C.vz = fxm_easeLerp(2, 8, work->flags0[0x10], 0xC00, 0x1800);
            }
        }
        if (work->flags0[0xC] != 0) {
            work->flags0[0xC]--;
            if (work->flags0[0xC] >= 5) {
                work->scale14C.vz = fxm_easeLerp(2, 4, work->flags0[0xC] - 4, 0x800, 0x2800);
            } else {
                work->scale14C.vz = fxm_easeLerp(2, 4, work->flags0[0xC], 0x2800, 0x800);
            }
            work->pos164.vz = work->pos13C.vz = fxm_easeLerp(2, 8, work->flags0[0xC], -0x1000, 0x1000);
            ex->btlfx_moveEmitterTo((s32)work->emit1B8, (s32)&work->pos164);
        }
        if (work->flags0[0xF] != 0) {
            work->flags0[0xF]--;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_BACKDROP), frame);
            morphT = fxm_easeLerp(2, 0x10, work->flags0[0xF], 0x1000, 0);
            work->pos164.vy = work->pos13C.vy = fxm_easeLerp(1, 0x10, work->flags0[0xF], -0x2500, -0x1000);
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 0x10, work->flags0[0xF], 0x1800, 0);
            switch (morphT >> 10) {
            case 3:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model128, work->model12C,
                                         (morphT & 0x3FF) << 2);
                break;
            case 2:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model12C, work->model130,
                                         (morphT & 0x3FF) << 2);
                break;
            case 1:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model130, work->model134,
                                         (morphT & 0x3FF) << 2);
                break;
            case 0:
                ex->Hi_DrawMorphEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model134, work->model138,
                                         (morphT & 0x3FF) << 2);
                break;
            }
        }
        if (work->flags0[0x23] != 0) {
            work->flags0[0x23]--;
            if (work->flags0[0x23] >= 0x19) {
                work->pos13C.vy = fxm_easeLerp(1, 0x18, work->flags0[0x23] - 0x18, -0x246A, -0x1300);
                work->scale14C.vy = fxm_easeLerp(1, 0x18, work->flags0[0x23] - 0x18, 0x7FE, 0x555);
            } else {
                work->pos13C.vy = fxm_easeLerp(2, 0x18, work->flags0[0x23], -0x1300, -0xC00);
            }
            work->pos164.vy = work->pos13C.vy;
            ex->btlfx_moveEmitterTo((s32)work->emit19C, (s32)&work->pos164);
        }
        if (work->flags0[0x24] != 0) {
            work->flags0[0x24]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(0, 0x10, work->flags0[0x24], 0x555, 0x555);
        }
        if (work->flags0[0x1A] != 0) {
            work->flags0[0x1A]--;
            work->scale14C.vx = work->scale14C.vz = work->scale14C.vy = fxm_easeLerp(1, 8, work->flags0[0x1A], 0x555, 0);
        }
        if (work->flags0[0xA] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_BACKDROP), frame);
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model128);
        }
        if (work->flags0[0x1C] != 0) {
            work->flags0[0x1C]--;
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model180);
            if (work->flags0[0x1C] < 8) {
                bright = (work->flags0[0x1C] - 8) << 5;
                ex->Hi_ModifyEffModelRGB(work->model180, bright, bright, bright);
            }
        }
        if (work->flags0[0x1E] != 0) {
            work->flags0[0x1E]--;
            ex->btlfx_moveEmitterTo((s32)work->emit184, (s32)&work->pos164);
            ex->btlfx_moveEmitterTo((s32)work->emit190, (s32)&work->pos164);
        }
        if (work->flags0[0x13] != 0) {
            pos.vx = work->pos13C.vx;
            pos.vy = work->pos13C.vy;
            pos.vz = work->pos13C.vz;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, &pos.vx, &work->rot174.vx);
            ex->Hi_DrawEffModel(&work->rot174, &work->pos13C, (s32)&work->scale14C, work->model170);
            ex->Hi_DrawEffModel(&work->rot174, &work->pos13C, (s32)&work->scale14C, work->model16C);
            ex->Hi_DrawEffModel(&work->rot15C, &work->pos13C, (s32)&work->scale14C, work->model17C);
        }
        if (work->flags0[0x14] != 0) {
            work->flags0[0x14]--;
            bright = -(work->flags0[0x14] * 0x10);
            ex->Hi_ModifyEffModelRGB(work->model170, bright, bright, bright);
            ex->Hi_ModifyEffModelRGB(work->model16C, bright, bright, bright);
            ex->Hi_ModifyEffModelRGB(work->model17C, bright, bright, bright);
        }
        if (work->flags0[0x1B] != 0) {
            work->flags0[0x1B]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                radius = fxm_easeLerp(2, 0x30, work->flags0[0x1B], 0x1000, 0x200);
                unitPos.vy = work->pos164.vy + ((ex->math_sin(fx_0b8_units[i].pitch1C) * radius) >> 12);
                radius = abs((ex->math_cos(fx_0b8_units[i].pitch1C) * radius) >> 12);
                unitPos.vx = work->pos164.vx + ((ex->math_sin(fx_0b8_units[i].yaw1E) * radius) >> 12);
                unitPos.vz = work->pos164.vz + ((ex->math_cos(fx_0b8_units[i].yaw1E) * radius) >> 12);
                ex->btlfx_setUnitPos(i, &unitPos.vx);
                sc.vx = sc.vy = sc.vz = fxm_easeLerp(0, 0x30, work->flags0[0x1B], fx_0b8_units[i].scale8.vy, 0);
                ex->btlfx_setUnitScale(i, (s32 *)&sc);
                fx_0b8_units[i].rot24.vx += fx_0b8_units[i].spinX2C;
                fx_0b8_units[i].rot24.vy += fx_0b8_units[i].spinY2E;
                fx_0b8_units[i].rot24.vz += fx_0b8_units[i].spinZ30;
                ex->btlfx_setUnitRot(i, &fx_0b8_units[i].rot24.vx);
            }
        }
        bright = 0xFFF;
        if (work->flags0[0x16] != 0) {
            work->flags0[0x16]--;
            if (work->flags0[0x17] != 0) {
                work->flags0[0x17]--;
                bright = (0x10 - work->flags0[0x17]) << 8;
            }
            if (work->flags0[0x18] != 0) {
                work->flags0[0x18]--;
                bright = work->flags0[0x18] << 8;
            }
            ex->btlfx_aimAngles(&work->pos164.vx, (s16 *)&env->camera->eye, &rot.vx);
            ang.vz = 0xC18;
            ang.vx = 0;
            ang.vy = -0x1200;
            rot.vz = 0;
            rot.vx += 0xA0;
            fx_0b8_drawGradientPlane(&ang, &rot, bright, 0x17, 0x3D09, 1);
            ang.vy = 0x200;
            rot.vz = 0x800;
            rot.vx -= 0x140;
            fx_0b8_drawGradientPlane(&ang, &rot, bright, 0x17, 0x3D09, 1);
        }
        if (work->flags0[0x20] != 0) {
            work->flags0[0x20]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitHomePos(i, &fx_0b8_units[i].pos14.vx);
                fx_0b8_units[i].pos14.vy -= ex->math_sin(work->flags0[0x20] << 7) >> 1;
                ex->btlfx_setUnitPos(i, &fx_0b8_units[i].pos14.vx);
                if (work->flags0[0x20] == 0) {
                    fx_0b8_units[i].pos14.vy = 0;
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_UNIT_RING), (s32)&fx_0b8_units[i].pos14);
                    if (emitter != NULL) {
                        emitter->otZ = -0x100;
                    }
                }
            }
        }
        if (work->flags0[0x21] != 0) {
            work->flags0[0x21]--;
            if (work->flags0[0x21] != 0) {
                if (work->emit19C != NULL) {
                    work->emit19C->alpha = work->flags0[0x21] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(work->emit19C);
            }
        }
        if (work->flags0[0x1D] != 0) {
            work->flags0[0x1D]--;
            if (work->flags0[0x1D] != 0) {
                if (work->emit184 != NULL) {
                    work->emit184->alpha = work->flags0[0x1D] << 8;
                }
                if (work->emit190 != NULL) {
                    work->emit190->alpha = work->flags0[0x1D] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(work->emit184);
                ex->btlfx_stopEmitter(work->emit190);
            }
        }
        if (work->flags0[0x19] != 0) {
            work->flags0[0x19]--;
            work->scrollSpeed1E8 = fxm_easeLerp(1, 0x40, work->flags0[0x19], 4, 0x14);
        }
        work->scroll1E4 += work->scrollSpeed1E8;
        if (work->flags0[5] != 0) {
            ex->btlfx_drawScrollMove(frame * 0x10, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        }
        if (work->flags0[6] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - frame * 0x10, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        }
        if (work->flags0[0x22] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - work->scroll1E4, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (work->flags0[0x13] != 0) {
            ex->btlfx_drawScrollMove(work->scroll1E4, 0x2C0, 0x100, 0x38, 0x70, 0x2C0, 0x170);
        }
        if (work->flags0[0x16] != 0) {
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x18, 0x60, 0x1C0, 0x180);
        }
        if (work->flags0[7] != 0) {
            work->flags0[7]--;
            fxm_tintScreenLerp(0xFF, 0, work->flags0[7], 8, 1, 0);
        }
        if (work->flags0[0xD] != 0) {
            s32 sh;
            work->flags0[0xD]--;
            sh = fx_0b8_gridShift;
            fade = work->flags0[0xD];
            scale.vz = scale.vy = scale.vx = fade * 3 + 0x1000;
            col.r = col.g = col.b = fade << sh;
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

void fx_0b8_drawGradientPlane(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr) {
    SVECTOR quad[4];
    SVECTOR tint;
    MATRIX m;
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    CVECTOR col0;
    CVECTOR col1;
    Unk800F78A4 *env;
    BtlfxExports *ex;
    POLY_GT4 *poly;
    s32 i;
    s32 j;
    s32 bright0;
    s32 bright1;

    env = fx_0b8_env;
    ex = BTLFX_EXPORTS(env);
    uv0.u = uv2.u = 0;
    uv1.u = uv3.u = 0x5F;
    uv0.v = uv1.v = 0;
    uv2.v = uv3.v = 0x5F;
    quad[0].vy = quad[1].vy = quad[2].vy = quad[3].vy = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    tpage |= TPAGE_ABR(abr);
    poly = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(poly + 32);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    n /= 8;
    bright0 = 0;
    bright1 = n;
    for (j = 0; j < 8; j++) {
        tint.vx = fx_0b8_planeColors[j].vx;
        tint.vy = fx_0b8_planeColors[j].vy;
        tint.vz = fx_0b8_planeColors[j].vz;
        col0.r = (tint.vx * bright0) >> 12;
        col0.g = (tint.vy * bright0) >> 12;
        col0.b = (tint.vz * bright0) >> 12;
        tint.vx = fx_0b8_planeColors[j + 1].vx;
        tint.vy = fx_0b8_planeColors[j + 1].vy;
        tint.vz = fx_0b8_planeColors[j + 1].vz;
        col1.r = (tint.vx * bright1) >> 12;
        col1.g = (tint.vy * bright1) >> 12;
        col1.b = (tint.vz * bright1) >> 12;
        bright0 += n;
        bright1 += n;
        quad[0].vz = quad[1].vz = j * 0xA00 - 0x2800;
        quad[2].vz = quad[3].vz = j * 0xA00 - 0x1E00;
        for (i = 0; i < 4; i++) {
            quad[0].vx = quad[2].vx = i * 0x1400 - 0x2800;
            quad[1].vx = quad[3].vx = i * 0x1400 - 0x1400;
            gte_ldv3c(quad);
            gte_rtpt();
            poly->code = (abr != 0xFF) ? 0x3E : 0x3C;
            poly->r0 = poly->r1 = col0.r;
            poly->g0 = poly->g1 = col0.g;
            poly->b0 = poly->b1 = col0.b;
            poly->r2 = poly->r3 = col1.r;
            poly->g2 = poly->g3 = col1.g;
            poly->b2 = poly->b3 = col1.b;
            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_ldv0(&quad[3]);
            gte_rtps();
            *(u16 *)&poly->u0 = *(u16 *)&uv0;
            *(u16 *)&poly->u1 = *(u16 *)&uv1;
            *(u16 *)&poly->u2 = *(u16 *)&uv2;
            *(u16 *)&poly->u3 = *(u16 *)&uv3;
            poly->clut = clut;
            poly->tpage = tpage;
            gte_stsxy(&poly->x3);
            ex->btlfx_addPrim(0x0C000000, 0, poly);
            poly++;
        }
    }
}

void fx_0b8_drawScreenBand(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr) {
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    BtlfxExports *ex;
    Unk800F78A4 *env;
    POLY_GT4 *gt4;
    POLY_F4 *poly;
    s32 i;

    env = fx_0b8_env;
    ex = BTLFX_EXPORTS(env);
    uv0.u = uv2.u = 0;
    uv1.u = uv3.u = 0xFF;
    uv0.v = uv1.v = 0x80;
    uv2.v = uv3.v = 0xFF;
    tpage |= TPAGE_ABR(abr);
    gt4 = (POLY_GT4 *)env->prim;
    poly = (POLY_F4 *)(gt4 + 3);
    env->prim = (u8 *)(poly + 3);
    for (i = 0; i < 3; gt4++, i++, poly++) {
        gt4->clut = clut;
        gt4->tpage = tpage;
        gt4->code = (abr != 0xFF) ? 0x3E : 0x3C;
        poly->code = (abr != 0xFF) ? 0x2A : 0x28;
        gt4->r0 = gt4->r1 = gt4->g0 = gt4->g1 = gt4->b0 = gt4->b1 = c0;
        poly->r0 = poly->g0 = poly->b0 = gt4->r2 = gt4->r3 = gt4->g2 = gt4->g3 = gt4->b2 = gt4->b3 = c1;
        *(u16 *)&gt4->u0 = *(u16 *)&uv0;
        *(u16 *)&gt4->u1 = *(u16 *)&uv1;
        *(u16 *)&gt4->u2 = *(u16 *)&uv2;
        *(u16 *)&gt4->u3 = *(u16 *)&uv3;
        poly->x0 = poly->x2 = gt4->x0 = gt4->x2 = (x | ~0xFF) + i * 0x100;
        poly->x1 = poly->x3 = gt4->x1 = gt4->x3 = gt4->x0 + 0x100;
        gt4->y0 = gt4->y1 = y;
        poly->y0 = poly->y1 = gt4->y2 = gt4->y3 = y + h;
        poly->y2 = poly->y3 = gt4->y2 + 0x130;
        ex->btlfx_addPrim(0x0C000000, ot * 4, gt4);
        ex->btlfx_addPrim(0x05000000, ot * 4, poly);
    }
}

void fx_0b8_drawScreenGradient(s32 ot) {
    Unk800F78A4 *env = fx_0b8_env;
    POLY_G4 *poly = (POLY_G4 *)env->prim;
    BtlfxExports *ex;

    env->prim = (u8 *)(poly + 1);
    ex = BTLFX_EXPORTS(env);
    poly->code = GPU_POLY_G4;
    poly->r0 = poly->r1 = 0xE6;
    poly->g0 = poly->g1 = 0xCD;
    poly->b0 = poly->b1 = 0x96;
    poly->r2 = poly->r3 = 0xB4;
    poly->g2 = poly->g3 = 0xA0;
    poly->b2 = poly->b3 = 0x78;
    poly->x1 = poly->x3 = 320;
    poly->x0 = poly->x2 = 0;
    poly->y0 = poly->y1 = 0;
    poly->y2 = poly->y3 = 240;
    ex->btlfx_addPrim(0x08000000, ot * 4, poly);
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    Unk800F78A4 *env = fx_0b8_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bright = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), abr, bright, bright, bright);
}

static s32 fx_0b8_op80ParticleMotion(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0b8_env);
    SVECTOR cur;
    s32 r;
    SVECTOR off;

    SVECTOR *from = &fx_0b8_particleFrom[(pt->slot - 1) & 0x1F];

    off.vx = 0;
    off.vy = -0xC00;
    off.vz = 0x418;
    ex->btlfx_lerpVec((s16 *)from, (s16 *)&off,
                                             ((0x1000000 / n) * frame) >> 12, (s16 *)&cur);
    pos->vx = cur.vx;
    pos->vy = cur.vy;
    pos->vz = cur.vz;
    pt->pos.vx = cur.vx << 12;
    pt->pos.vy = cur.vy << 12;
    pt->pos.vz = cur.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b8/nonmatchings/fx_0b8", fx_0b8_headerPtr);

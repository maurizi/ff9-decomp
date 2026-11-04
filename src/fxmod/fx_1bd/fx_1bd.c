#include "fxmod/fx_1bd.h"

static void fxm_buildPathTrail(Fx1bdPath *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR pos;
    SVECTOR *pt;
    s32 count;
    s32 i;
    s32 keyIdx;
    s32 next;

    count = (size - 4) / 8;
    pt = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = count - 1;
    buf[1] = 0;
    if (n < count) {
        for (i = 0; i < count - n; i++) {
            pt->pad = 0;
            pt++;
        }
    } else {
        n = count;
    }
    for (i = 0; i < n; i++) {
        next = i + 1;
        keyIdx = n - next + start;
        if (clip != 0 && keyIdx >= keys->count2) {
            break;
        }
        fxm_posPathGet(keys, keyIdx, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        pt->pad = from + (to - from) / keys->count2 * (n - i);
        pt++;
    }
}

s32 fx_1bd_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_1bd_env = env;
        ex->btlfx_initEffModels(fx_1bd_effModels, 0x20);
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_SHOT0_CAMERA_KEYS_1));
        fxm_camChanAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_SHOT0_CAMERA_KEYS_2));
        return 0;
    } else {
        frame = entry->frame;
        fxm_camChanUpdate(0);
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1bd_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1bd_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_headerPtr);

s32 fx_1bd_op82(s32 mode, Fx1bdWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    DrMode2Words *dm;
    s32 frame;
    s32 i;
    s32 elapsed;

    if (mode == 0) {
        *args = 0x6C;
    } else if (mode == 1) {
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0x320;
        work->scale1C.vx = 0x1000;
        work->scale1C.vy = 0x1000;
        work->scale1C.vz = 0x1000;
        fx_1bd_env = env;
        work->model66 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_IFRIT_WHITE_HAZE), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->model66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_IFRIT_BLOCK_CLUSTER), 0x17, 0x3D00, 0x80, 0xE0);
        fx_1bd_pathTrail.alpha10 = 0x60;
        fx_1bd_pathTrail.start8 = 0;
        fx_1bd_pathTrail.countA = 0;
        fx_1bd_pathTrail.alphaC = 0x1000;
        fx_1bd_pathTrail.scaleE = 0x1000;
        fx_1bd_pathTrail.alpha12 = 0x30;
        work->timer60 = -1;
        work->timer62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x80, 0xA0, fx_1bd_bgOt, 0, 0x168, 0xF6E);
            break;
        case 22:
            work->pos58.vx = -3;
            work->pos58.vy = 0;
            work->pos58.vz = 0x320;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_IFRIT_ORANGE_RING_2), (s32)&work->pos58);
            break;
        case 21:
            work->timer60 = 4;
            break;
        case 15:
            work->timer62 = 6;
            break;
        case 46:
            ex->Hi_FreeEffModel(work->model66);
            break;
        case 33:
            ex->Hi_FreeEffModel(work->model68);
            break;
        }
        if (frame >= 0 && frame < 0x2E) {
            ex->Hi_DrawEffModel(&work->rot4, &work->posC, (s32)&work->scale1C, work->model66);
            ex->Hi_ModifyEffModelRGB(work->model66, 6, 6, 6);
        }
        if (frame >= 21 && frame < 31) {
            fx_1bd_pathTrail.countA = (frame - 21) * 2;
            fx_1bd_pathTrail.countA = fx_1bd_pathTrail.countA >= 10 ? 9 : fx_1bd_pathTrail.countA;
            if (work->timer60 >= 0) {
                fx_1bd_pathTrail.alphaC = fxm_lerpRatio(0, 0x1000, 4 - work->timer60, 4);
                work->timer60--;
            }
            if (work->timer62 >= 0) {
                fx_1bd_pathTrail.alphaC = fxm_lerpRatio(0x1000, 0, 6 - work->timer62, 6);
                work->timer62--;
            }
            for (i = 0; i < 13; i++) {
                fxm_posPathInit(&fx_1bd_pathTrail.path0, fxm_offsetTableEntry((u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_TRAIL_PATHS), i));
                fxm_buildPathTrail(&fx_1bd_pathTrail.path0, fx_1bd_op82Trails[i], 0x50, fx_1bd_pathTrail.start8, fx_1bd_pathTrail.countA, fx_1bd_pathTrail.alpha10,
                              fx_1bd_pathTrail.alpha12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_FIRE_TRAIL), 0, NULL, 0, fx_1bd_pathTrail.scaleE,
                                    fx_1bd_pathTrail.alphaC, 1, fx_1bd_op82Trails[i], 0);
            }
        }
        if (frame >= 26 && frame < 30) {
            elapsed = frame - 26;
            dm = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->ot[1], dm);
            scale.vx = (elapsed << 7) + 0x1000;
            scale.vy = (elapsed << 7) + 0x1000;
            scale.vz = (elapsed << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
            env->camera->projDist = 0x208 - elapsed * elapsed * 4;
        }
        work->unk64++;
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_1bd_op83(s32 mode, Fx1bdWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rockPos;
    SVECTOR trackOfs;
    Fx1bdRock *rock;
    s16 *tex;
    s16 *clut;
    u16 *modelData;
    u16 *trackData;
    s32 frame;
    s32 i;
    s32 j;
    s32 trackFrame;
    s32 dist;

    if (mode == 0) {
        *args = 0x150;
    } else if (mode == 1) {
        work->unkC.vx = 0x400;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unkC.pad = 1;
        work->pos14.vx = 0;
        work->pos14.vy = -0x78;
        work->pos14.vz = 0x320;
        work->unk1C = 0x1800;
        work->xform20.rot0.vx = 0;
        work->xform20.rot0.vy = 0;
        work->xform20.rot0.vz = 0;
        work->xform20.pos8.vx = 0;
        work->xform20.pos8.vy = 0;
        work->xform20.pos8.vz = 0x320;
        work->xform20.scale18.vx = 0x1000;
        work->xform20.scale18.vy = 0x1000;
        work->xform20.scale18.vz = 0x1000;
        fx_1bd_env = env;
        work->model118[0] = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_IFRIT_SPOKED_RING), fx_1bd_texInfo0,
                                                                  fx_1bd_texPos0);
        ex->Hi_SetEffModelOffset(work->model118[0], 0x6E);
        work->xform48.rot0.vx = 0;
        work->xform48.rot0.vy = 0;
        work->xform48.rot0.vz = 0;
        work->xform48.pos8.vx = 0;
        work->xform48.pos8.vy = 0;
        work->xform48.pos8.vz = 0x320;
        work->xform48.scale18.vx = 0x1000;
        work->xform48.scale18.vy = 0x1000;
        work->xform48.scale18.vz = 0x1000;
        for (j = 0; j < 23; j++) {
            if (fx_1bd_op83TexSelect[j]) {
                tex = fx_1bd_texInfo2;
                clut = fx_1bd_texPos2;
            } else {
                tex = fx_1bd_texInfo1;
                clut = fx_1bd_texPos1;
            }
            modelData = fxm_offsetTableEntry((u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_PIECE_MODELS), j);
            trackData = fxm_offsetTableEntry((u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_PIECE_TRACKS), j);
            work->model118[j + 1] = ((s32(*)())ex->Hi_RegisterTexListModel)(modelData, tex, clut);
            ex->Hi_SetEffModelOffset(work->model118[j + 1], 0x6E);
            fxm_rotPosTrackInit(&fx_1bd_rotPosTrack[j], trackData);
        }
        work->xform70.rot0.vx = 0;
        work->xform70.rot0.vy = 0;
        work->xform70.rot0.vz = 0;
        work->xform70.pos8.vx = 0;
        work->xform70.pos8.vy = 0;
        work->xform70.pos8.vz = 0x320;
        work->xform70.scale18.vx = 0x1000;
        work->xform70.scale18.vy = 0x1000;
        work->xform70.scale18.vz = 0x1000;
        fxm_vecTrackInit((Fx1bdTrack12 *)&fx_1bd_vecTrack0, (u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_VEC_TRACK));
        work->model148 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_IFRIT_HEAT_DOME), fx_1bd_texInfo3,
                                                               fx_1bd_texPos3);
        work->xform9C.rot0.vx = 0;
        work->xform9C.rot0.vy = 0;
        work->xform9C.rot0.vz = 0;
        work->xform9C.pos8.vx = 0;
        work->xform9C.pos8.vy = 0;
        work->xform9C.pos8.vz = 0x320;
        work->xform9C.scale18.vx = 0x1000;
        work->xform9C.scale18.vy = 0x1000;
        work->xform9C.scale18.vz = 0x1000;
        work->xformCC.rot0.vx = 0;
        work->xformCC.rot0.vy = 0;
        work->xformCC.rot0.vz = 0;
        work->xformCC.pos8.vx = 0;
        work->xformCC.pos8.vy = 0;
        work->xformCC.pos8.vz = 0x320;
        work->xformCC.scale18.vx = 0x1000;
        work->xformCC.scale18.vy = 0x1000;
        work->xformCC.scale18.vz = 0x1000;
        work->model14A = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_IFRIT_FLAME_DOME), 0x9A, 0x3DC0, 0, 0);
        work->model14C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_IFRIT_FLAME_RING), 0x9A, 0x3DC0, 0, 0);
        fxm_posTrackInit((Fx1bdTrack12 *)&fx_1bd_posTrack, (u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_POS_TRACK));
        fxm_vecTrackInit((Fx1bdTrack12 *)&fx_1bd_vecTrack1, (u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_VEC_TRACK_2));
        fxm_vecTrackInit((Fx1bdTrack12 *)&fx_1bd_vecTrack2, (u16 *)ex->btlfx_getFileData(entry->file, FX_IFRIT_VEC_TRACK_3));
        work->frame10C = 0;
        work->unk104.vx = work->pos14.vx;
        work->unk104.vy = work->pos14.vy;
        work->unk104.vz = work->pos14.vz;
        work->timer10E = -1;
        work->timer110 = -1;
        work->timer112 = -1;
        work->timer114 = -1;
        work->timer116 = -1;
        work->unk104.vy -= 0xF0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->posF4.vx = work->xform20.pos8.vx;
            work->posF4.vy = work->xform20.pos8.vy;
            work->posF4.vz = work->xform20.pos8.vz;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_IFRIT_RED_GLOW), (s32)&work->posF4);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_IFRIT_FIRE_BURST), (s32)&work->posF4);
            break;
        case 3:
            work->xformCC.pos8.vx = work->xform20.pos8.vx;
            work->xformCC.pos8.vy = work->xform20.pos8.vy;
            work->xformCC.pos8.vz = work->xform20.pos8.vz;
            work->posFC.vx = work->xform20.pos8.vx;
            work->posFC.vy = work->xform20.pos8.vy;
            work->posFC.vz = work->xform20.pos8.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_IFRIT_ORANGE_RINGS), (s32)&work->posFC);
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x100;
            }
            break;
        case 4:
            work->timer110 = 3;
            work->timer114 = 6;
            break;
        case 21:
            work->timer112 = 0x10;
            work->timer116 = 0x10;
            work->timer10E = 0x10;
            break;
        }
        if (frame >= 6 && frame < 20) {
            ex->Hi_DrawEffModel(&work->xform20.rot0, &work->xform20.pos8, (s32)&work->xform20.scale18, work->model118[0]);
            ex->Hi_ModifyEffModelRGB(work->model118[0], 6, 6, 6);
            for (i = 0, rock = fx_1bd_op83Pieces; i < 23; i++, rock++) {
                trackFrame = frame - 2;
                if (rock->done2) {
                    trackFrame = 0;
                }
                if (trackFrame == rock->frame0) {
                    rockPos.vx = rock->pos18.vx;
                    rockPos.vy = rock->pos18.vy;
                    rockPos.vz = rock->pos18.vz - 400;
                    ex->btlfx_aimAngles((s16 *)&work->xform48.pos8, (s16 *)&rockPos, (s16 *)&rock->angle8);
                    dist = ex->btlfx_distance(&rockPos, (SVECTOR *)&work->xform48.pos8);
                    rock->vel6 = 200;
                    rock->speed4 = dist / 30;
                    rock->vel6 += ex->btlfx_rand() % 100;
                    rock->speed4 += ex->btlfx_rand() % 50;
                    rock->vel6 /= 2;
                    rock->angle8.vx &= 0xFFF;
                    rock->angle8.vy &= 0xFFF;
                    rock->angle8.vz &= 0xFFF;
                    rock->speed4 /= 2;
                    if (rock->angle8.vy < 0x800) {
                        rock->flag1 |= FX1BD_ROCK_SPIN_X_POS;
                    } else {
                        rock->flag1 |= FX1BD_ROCK_SPIN_X_NEG;
                    }
                    if (rock->angle8.vy < 0x800) {
                        rock->flag1 |= FX1BD_ROCK_SPIN_Z_POS;
                    } else {
                        rock->flag1 |= FX1BD_ROCK_SPIN_Z_NEG;
                    }
                }
                if (trackFrame >= rock->frame0) {
                    rockPos.vx = rock->pos18.vx;
                    rockPos.vy = rock->pos18.vy;
                    rockPos.vz = rock->pos18.vz;
                    ex->btlfx_pointAhead((s16 *)&rockPos, (s16 *)&rock->angle8, rock->speed4, (s16 *)&rockPos);
                    rock->pos18.vx = rockPos.vx;
                    rock->pos18.vy = rockPos.vy;
                    rock->pos18.vz = rockPos.vz;
                    rock->pos18.vy -= rock->vel6;
                    if (trackFrame < rock->frame0 + 2) {
                        if (rock->flag1 & FX1BD_ROCK_SPIN_X_POS) {
                            rock->rot10.vx += 0x40;
                        } else {
                            rock->rot10.vx -= 0x40;
                        }
                        if (rock->flag1 & FX1BD_ROCK_SPIN_Z_POS) {
                            rock->rot10.vz += 0x40;
                        } else {
                            rock->rot10.vz -= 0x40;
                        }
                    }
                    work->xform48.rot0.vx = rock->rot10.vx;
                    work->xform48.rot0.vy = rock->rot10.vy;
                    work->xform48.rot0.vz = rock->rot10.vz;
                    rock->vel6 -= 20;
                    if (rock->vel6 >= 4) {
                        rock->vel6 -= trackFrame * 5;
                    }
                    if (rock->pos18.vy >= 50) {
                        rock->done2 = 1;
                        continue;
                    }
                } else {
                    fxm_rotPosTrackGet(&fx_1bd_rotPosTrack[i], trackFrame, &work->xform48.rot0, &rockPos);
                    rockPos.vx = -rockPos.vx;
                    work->xform48.pos8.vx = 0;
                    work->xform48.pos8.vy = 0;
                    work->xform48.pos8.vz = 0x320;
                    work->xform48.rot0.vz = -work->xform48.rot0.vz;
                    work->xform48.pos8.vx += rockPos.vx;
                    work->xform48.pos8.vy += rockPos.vy;
                    work->xform48.pos8.vz += rockPos.vz;
                    rock->pos18.vx = work->xform48.pos8.vx;
                    rock->pos18.vy = work->xform48.pos8.vy;
                    rock->pos18.vz = work->xform48.pos8.vz;
                    work->xform48.pos8.vx = 0;
                    work->xform48.pos8.vy = 0;
                    work->xform48.pos8.vz = 0x320;
                }
                ex->Hi_DrawEffModel(&work->xform48.rot0, &rock->pos18, (s32)&work->xform48.scale18, work->model118[i + 1]);
                ex->Hi_ModifyEffModelRGB(work->model118[i + 1], 6, 6, 6);
            }
        }
        if (frame >= 4 && frame < 37) {
            fxm_vecTrackGet(&fx_1bd_vecTrack0, frame - 4, &work->xform70.scale18);
            ex->Hi_DrawEffModel(&work->xform70.rot0, &work->xform70.pos8, (s32)&work->xform70.scale18, work->model148);
            ex->Hi_ModifyEffModelAbr(work->model148, 1);
            if (work->timer10E >= 0) {
                work->fade98 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer10E, 0x10);
                ex->Hi_ModifyEffModelRGB(work->model148, work->fade98, work->fade98, work->fade98);
                work->timer10E--;
            }
            fxm_vecTrackGet(&fx_1bd_vecTrack1, frame - 4, &work->xform9C.scale18);
            ex->Hi_DrawEffModel(&work->xform9C.rot0, &work->xform9C.pos8, (s32)&work->xform9C.scale18, work->model14A);
            ex->Hi_ModifyEffModelAbr(work->model14A, 1);
            if (work->timer110 >= 0 || work->timer112 >= 0) {
                if (work->timer110 >= 0) {
                    work->fadeC4 = fxm_lerpRatio(-0x80, 0, 3 - work->timer110, 3);
                    work->timer110--;
                }
                if (work->timer112 >= 0) {
                    work->fadeC4 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer112, 0x10);
                    work->timer112--;
                }
                ex->Hi_ModifyEffModelRGB(work->model14A, work->fadeC4, work->fadeC4, work->fadeC4);
            }
            work->xform9C.rot0.vy += 0x40;
        }
        if (frame >= 4 && frame < 37) {
            work->xformCC.pos8.vx = 0;
            work->xformCC.pos8.vy = 0;
            work->xformCC.pos8.vz = 0x320;
            fxm_posTrackGet(&fx_1bd_posTrack, frame - 4, &trackOfs);
            work->xformCC.pos8.vx += trackOfs.vx;
            work->xformCC.pos8.vy += trackOfs.vy;
            work->xformCC.pos8.vz += trackOfs.vz;
            fxm_vecTrackGet(&fx_1bd_vecTrack2, frame - 4, &work->xformCC.scale18);
            ex->Hi_DrawEffModel(&work->xformCC.rot0, &work->xformCC.pos8, (s32)&work->xformCC.scale18, work->model14C);
            ex->Hi_ModifyEffModelAbr(work->model14C, 1);
            if (work->timer114 >= 0 || work->timer116 >= 0) {
                if (work->timer114 >= 0) {
                    work->fadeC8 = fxm_lerpRatio(-0x80, 0, 6 - work->timer114, 6);
                    work->timer114--;
                }
                if (work->timer116 >= 0) {
                    work->fadeC8 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer116, 0x10);
                    work->timer116--;
                }
                ex->Hi_ModifyEffModelRGB(work->model14C, work->fadeC8, work->fadeC8, work->fadeC8);
            }
        }
        ex->btlfx_drawScrollMove(0x3C0 - work->frame10C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
        work->frame10C++;
        return frame >= 0x3C;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1bd_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posTrackInit(Fx1bdTrack12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx1bdKey12 *)data;
}

static s32 fxm_posTrackGet(Fx1bdTrack6 *track, s32 t, SVECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx1bdKey6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = -key->x;
    out->vy = key->y;
    out->vz = -key->z;
    return done;
}

static void fxm_posPathInit(Fx1bdPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1bdPoint *)data;
}

static s32 fxm_posPathGet(Fx1bdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1bdPoint *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosTrackInit(Fx1bdTrack12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx1bdKey12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx1bdTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx1bdKey12 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6;
        rot->vy = key->rot8;
        rot->vz = key->rotA;
    }
    if (pos != NULL) {
        pos->vx = -key->pos0;
        pos->vy = key->pos2;
        pos->vz = -key->pos4;
    }
    return done;
}

static void fxm_vecTrackInit(Fx1bdTrack12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx1bdKey12 *)data;
}

static s32 fxm_vecTrackGet(Fx1bdTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx1bdKey6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static u16 *fxm_offsetTableEntry(u16 *base, u16 idx) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)idx + 1]);
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1bd_env)->btlfx_drawScreenTint((u32 *)(fx_1bd_env->ot + ot), abr, bright, bright, bright);
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_1BD_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_1bd_camKeys[i].timer6 = freeMark;
    }
    for (i = 3; i >= 0; i--) {
        fx_1bd_camChanFrames[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < 64; i++) {
        if (fx_1bd_camKeys[i].timer6 == FX_1BD_KEY_FREE) {
            fx_1bd_camKeys[i].timer6 = 0;
            fx_1bd_camKeys[i].keys0 = data;
            fx_1bd_camKeys[i].frame4 = start;
            fx_1bd_camKeys[i].dstA = dst;
            fx_1bd_camKeys[i].indexC = 0;
            fx_1bd_camKeys[i].countE = count;
            fx_1bd_camKeys[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_1bd_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < 64; i++) {
        if (fx_1bd_camKeys[i].chan8 == ch) {
            if (fx_1bd_camKeys[i].timer6 == 0 && fx_1bd_camKeys[i].frame4 == fx_1bd_camChanFrames[ch]) {
                fx_1bd_camKeys[i].timer6 = *fx_1bd_camKeys[i].keys0;
                fx_1bd_camKeys[i].keys0++;
            }
            if (fx_1bd_camKeys[i].timer6 > 0) {
                fx_1bd_camKeys[i].timer6--;
                for (k = 0; k < 3; k++) {
                    dstSlot = BTLFX_CAM_SLOT(fx_1bd_camKeys[i].dstA, k);
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_1bd_camKeys[i].keys0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_1bd_camKeys[i].keys0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_1bd_camKeys[i].keys0[2];
                        if (fx_1bd_camKeys[i].indexC < fx_1bd_camKeys[i].countE) {
                            fx_1bd_camKeys[i].keys0 += 3;
                            fx_1bd_camKeys[i].indexC++;
                        }
                    }
                }
            }
        }
    }
    fx_1bd_camChanFrames[ch]++;
}

static void fxm_drawTexelQuadsB(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_1bd_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_bm();
                *(s32 *)&colTL = *(s32 *)&colTR;
                *(s32 *)&colBL = *(s32 *)&colBR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(0x8000000, otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, otz, tpage);
    }
    w--;
    h--;
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_1bd_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

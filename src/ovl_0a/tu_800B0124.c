#include "ovl_0a.h"

/* field music/sound request handler (GameData.unk83C); field version of ovl_0c wm_soundCommand */
s32 fld_soundCommand(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    GameData *g;
    s32 kind;
    s32 id;
    s32 t;
    s32 h;
    s32 ok;
    s32 slot;
    s32 fp;
    s32 playingBit;
    s32 loadedBit;
    s32 sz;
    s32 n;
    s32 m;
    s32 d;

    kind = (cmd >> 8) & 0xF;
    if (kind == SND_CMD_GET_STATE && g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
        goto state;
    }
    switch ((cmd >> 12) & 3) {
    case SND_TYPE_MUSIC:
        t = -1;
        id = (soundId & 0xFFFF) | DB_MUSIC;
        switch (kind) {
        case SND_CMD_RESUME:
            ok = 0;
            if (g_gameState->gameData->savedMusicId != MUSIC_NONE && g_gameState->gameData->savedMusicField == (s16)g_gameState->gameData->fieldId) {
                GameData *p;
                ok = 1;
                g_fieldMusicRequest = kind;
                p = g_gameState->gameData;
                p->musicVolume[0] = p->savedMusicVolume;
                soundId = p->savedMusicId;
                id = p->savedMusicId | DB_MUSIC;
                t = p->savedMusicBank | DB_SOUND_BANK;
                g_gameState->gameData->musicFileSector = g_gameState->gameData->savedMusicFileSector;
                g_gameState->gameData->musicFileSectors = g_gameState->gameData->savedMusicFileSectors;
            }
            /* v2 */
            g_gameState->gameData->savedMusicId = MUSIC_NONE;
            g_gameState->gameData->savedMusicBank = 0xFFFF;
            g_gameState->gameData->savedMusicField = 0xFFFF;
            g_gameState->gameData->savedMusicFileSector = -1;
            g_gameState->gameData->savedMusicFileSectors = 0xFFFF;
            if (!ok) {
                return 0;
            }
        case SND_CMD_PLAY:
        case SND_CMD_LOAD:
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                return -1;
            }
            if (g_fieldMusicRequest == -1) {
                g_fieldMusicRequest = kind;
            }
            if (g_fieldMusicRequest != SND_CMD_RESUME) {
                t = fld_depGetLink(id);
            }
            if (g_gameState->gameData->musicId[0] != soundId || g_gameState->gameData->musicBank[0] != (u16)t) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    g_gameState->gameData->flags |= GD_MUSIC_STOP_PENDING;
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                SCENE->flags &= ~FLD_SCN_BANK_SLOT;
                g_gameState->gameData->musicBank[0] = t;
                g_fieldSoundLoadId = id;
                g_gameState->gameData->musicId[0] = soundId;
                g_gameState->gameData->soundLoadState = SND_LOAD_WAIT_FADE;
                return g_gameState->gameData->soundLoadState;
            }
            if (g_fieldMusicRequest == SND_CMD_LOAD) {
                goto done;
            }
        start:
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            if (!(g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
                akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], 0xA, 0, g_gameState->gameData->musicVolume[0]);
            }
            g_gameState->gameData->flags |= (GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
        done:
            g_fieldMusicRequest = -1;
            return 0;
        case SND_CMD_GET_STATE:
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                goto state;
            }
            return fld_isMusicFading() ? 6 : 0;
        case SND_CMD_SUSPEND:
            akao_saveMusic();
            g_gameState->gameData->savedMusicId = g_gameState->gameData->musicId[0];
            g_gameState->gameData->savedMusicBank = g_gameState->gameData->musicBank[0];
            g_gameState->gameData->savedMusicVolume = g_gameState->gameData->musicVolume[0];
            g_gameState->gameData->savedMusicField = g_gameState->gameData->fieldId;
            g_gameState->gameData->savedMusicFileSector = g_gameState->gameData->musicFileSector;
            g_gameState->gameData->savedMusicFileSectors = g_gameState->gameData->musicFileSectors;
        case SND_CMD_STOP:
            if ((cmd & 0x3F) == SND_PARAM_VOLUME) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
                return 0;
            }
            akao_stopMusic(g_gameState->gameData->musicHandle[0]);
            g_gameState->gameData->musicVolume[0] = 0x7F;
            g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
            return 0;
        case SND_CMD_SET:
            if ((cmd & 0x3F) == 8) {
                return 0;
            }
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME ... SND_PARAM_PITCH:
                switch (cmd & 0x3F) {
                case SND_PARAM_VOLUME:
                    akao_setMusicVolume(g_gameState->gameData->musicHandle[0], param1);
                    g_gameState->gameData->musicVolume[0] = param1;
                    return 0;
                case SND_PARAM_TEMPO:
                    akao_setTempoScale(param1);
                    return 0;
                case SND_PARAM_PITCH:
                    akao_setPitchScale(param1);
                    return 0;
                case SND_PARAM_TEMPO_PITCH:
                    akao_setTempoPitchScale(param1);
                    return 0;
                }
                break;
            case SND_PARAM_PLAY_COUNT:
                g_gameState->gameData->musicHandle[0] = akao_playMusicCount(g_gameState->gameData->musicSeq, param1);
                g_gameState->gameData->flags |= GD_MUSIC_PLAYING;
                return 0;
            }
            break;
        case SND_CMD_FADE:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_fadeMusicVolume(g_gameState->gameData->musicHandle[0], param1, param2);
                fld_setMusicVolumeFade(param2, param1);
                return 0;
            case SND_PARAM_TEMPO:
                akao_fadeTempoScale(param1, param2);
                return 0;
            case SND_PARAM_PITCH:
                akao_fadePitchScale(param1, param2);
                return 0;
            case SND_PARAM_TEMPO_PITCH:
                akao_fadeTempoPitchScale(param1, param2);
                return 0;
            }
            break;
        case SND_CMD_FADE_FROM:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
                fld_setMusicVolumeFade(param3, param1);
                return 0;
            case SND_PARAM_TEMPO:
                akao_fadeTempoScaleFrom(param1, param2, param3);
                return 0;
            case SND_PARAM_PITCH:
                akao_fadePitchScaleFrom(param1, param2, param3);
                return 0;
            case SND_PARAM_TEMPO_PITCH:
                akao_fadeTempoPitchScaleFrom(param1, param2, param3);
                return 0;
            }
            break;
        case SND_CMD_FADE_ALL:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            fld_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, param1, param2, param3);
            fld_setMusicVolumeFade(param3, param1);
            return 0;
        case SND_CMD_SET_CONDITION:
            if ((cmd & 0x3F) != SND_PARAM_CONDITION) {
                break;
            }
            akao_setCondition(soundId);
            return 0;
        }
        return -1;
    case SND_TYPE_SE:
        /* rodata pad between the kind switches' jump tables */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        fp = 0;
        slot = 0;
        playingBit = 0;
        h = 0;
        id = (soundId & 0xFFFF) | DB_SE;
        if (kind != SND_CMD_SET_ALL && kind != SND_CMD_FADE_ALL) {
            fp = (cmd >> 7) & 1;
            if (fp != 0) {
                if (kind == SND_CMD_PLAY || kind == SND_CMD_SUSPEND || kind == SND_CMD_RESUME) {
                    slot = (cmd >> 6) & 1;
                } else {
                    for (slot = 0; slot < 2; slot++) {
                        if (g_gameState->gameData->sfxSlotId[slot] == soundId) {
                            break;
                        }
                    }
                    if (slot >= 2) {
                        return -1;
                    }
                }
                param1 = (0x100000 << slot) & 0x300000;
                playingBit = GD_SFX0_PLAYING << (slot * 2);
                loadedBit = playingBit >> 1;
                if (kind == SND_CMD_PLAY && g_gameState->gameData->sfxSlotId[slot] != soundId) {
                    if (g_gameState->gameData->flags & playingBit) {
                        akao_stopSe(SFXH(slot), param1);
                        g_gameState->gameData->flags ^= playingBit | loadedBit;
                    }
                    memcpy((void *)SFXH(slot), db_lookupRegistered(DB_GET_ADDR, id, 0), (s32)db_lookupRegistered(DB_GET_SIZE, id, 0));
                    g_gameState->gameData->sfxSlotId[slot] = soundId;
                    g_gameState->gameData->sfxSlotVolume[slot] = 0x7F;
                    g_gameState->gameData->sfxSlotPan[slot] = 0x80;
                    g_gameState->gameData->flags |= loadedBit;
                }
                h = SFXH(slot);
            } else {
                h = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
            }
        }
        switch (kind) {
        case SND_CMD_RESUME:
            t = 0;
            if (g_gameState->gameData->sfxSlotId[slot] != SE_NONE && g_gameState->gameData->sfxSlotSuspendField[slot] == (s16)g_gameState->gameData->fieldId) {
                param1 = (0x100000 << slot) & 0x300000;
                t = 1;
                param2 = g_gameState->gameData->sfxSlotPan[slot];
                param3 = g_gameState->gameData->sfxSlotVolume[slot];
            }
            g_gameState->gameData->sfxSlotSuspendField[slot] = 0xFFFF;
            if (!t) {
                return 0;
            }
        case SND_CMD_PLAY:
            if (!fp || !(g_gameState->gameData->flags & playingBit)) {
                akao_playSe(h, param1, param2, param3);
            }
            if (fp) {
                g_gameState->gameData->sfxSlotPan[slot] = param2;
                g_gameState->gameData->sfxSlotVolume[slot] = param3;
                g_gameState->gameData->flags |= playingBit;
            }
            return 0;
        case SND_CMD_SUSPEND:
            if (!(g_gameState->gameData->flags & playingBit)) {
                return 0;
            }
            g_gameState->gameData->sfxSlotSuspendField[slot] = g_gameState->gameData->fieldId;
        case SND_CMD_STOP:
            if ((cmd & 0x3F) == SND_PARAM_ALL_SLOTS && fp) {
                if (g_gameState->gameData->flags & GD_SFX0_PLAYING) {
                    akao_stopSe((s32)g_gameState->gameData->sfxSlotBuf[0], 0x100000);
                }
                if (g_gameState->gameData->flags & GD_SFX1_PLAYING) {
                    akao_stopSe((s32)g_gameState->gameData->sfxSlotBuf[1], 0x200000);
                }
                g_gameState->gameData->flags &= ~(GD_SFX0_PLAYING | GD_SFX1_PLAYING);
            } else {
                akao_stopSe(h, param1);
                if (fp) {
                    g_gameState->gameData->flags &= ~playingBit;
                }
            }
            return 0;
        case SND_CMD_SET:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_setSeVolume(h, param1, param2);
                if (fp) {
                    g_gameState->gameData->sfxSlotVolume[slot] = param2;
                }
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA2(h, param1, param2);
                if (fp) {
                    g_gameState->gameData->sfxSlotPan[slot] = param2;
                }
                return 0;
            case SND_PARAM_PITCH:
                akao_sendCmdA4(h, param1, param2);
                return 0;
            case 8:
                return 0;
            }
            return -1;
        case SND_CMD_FADE:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_fadeSeVolume(h, param1, param2, param3);
                if (fp) {
                    g_gameState->gameData->sfxSlotVolume[slot] = param3;
                }
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA3(h, param1, param2, param3);
                if (fp) {
                    g_gameState->gameData->sfxSlotPan[slot] = param3;
                }
                return 0;
            case SND_PARAM_PITCH:
                akao_sendCmdA5(h, param1, param2, param3);
                return 0;
            }
            return -1;
        case SND_CMD_SET_ALL:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_setAllSeVolume(param1);
                if (fp) {
                    g_gameState->gameData->sfxSlotVolume[0] = g_gameState->gameData->sfxSlotVolume[1] = param1;
                }
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdAA(param1);
                if (fp) {
                    g_gameState->gameData->sfxSlotPan[0] = g_gameState->gameData->sfxSlotPan[1] = param1;
                }
                return 0;
            case SND_PARAM_PITCH:
                akao_sendCmdAC(param1);
                return 0;
            }
            return -1;
        case SND_CMD_FADE_ALL:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_fadeAllSeVolume(param1, param2);
                if (fp) {
                    g_gameState->gameData->sfxSlotVolume[0] = g_gameState->gameData->sfxSlotVolume[1] = param2;
                }
                return -1;
            case SND_PARAM_PAN:
                akao_sendCmdAB(param1, param2);
                if (fp) {
                    g_gameState->gameData->sfxSlotPan[0] = g_gameState->gameData->sfxSlotPan[1] = param2;
                }
                return -1;
            case SND_PARAM_PITCH:
                akao_sendCmdAD(param1, param2);
                return -1;
            }
            return -1;
        }
        return -1;
    case SND_TYPE_STREAM:
        id = (soundId & 0xFFFF) | 0x180000;
        switch (kind) {
        case SND_CMD_PLAY:
            slot = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                return -1;
            }
            akao_sendCmdE4(fld_isMoviePlaying() ? SCENE->movieSndVolume : SCENE->sndVolume);
            akao_sendCmdE0((s32 *)slot, param1, param2);
            SCENE->unk5F = 0;
            return 0;
        case SND_CMD_STOP:
            akao_sendCmdE2();
            return 0;
        case SND_CMD_SET:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                SCENE->sndVolume = param1;
                if (fld_isMoviePlaying() == 0) {
                    akao_sendCmdE4(param1);
                }
                return 0;
            case SND_PARAM_MOVIE_VOLUME:
                SCENE->movieSndVolume = param1;
                if (fld_isMoviePlaying() != 0) {
                    akao_sendCmdE4(param1);
                }
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdE6(param1);
                return 0;
            case 12:
                SCENE->unk5F = param1;
                return 0;
            case 8:
                return 0;
            }
            return -1;
        }
        return -1;
    case SND_TYPE_BANK:
        id = (soundId & 0xFFFF) | DB_SOUND_BANK;
        switch (kind) {
        case SND_CMD_LOAD:
            SCENE->flags &= ~FLD_SCN_BANK_SLOT;
            if (param1 == 4) {
                SCENE->soundBankSlot = param2;
                SCENE->flags |= FLD_SCN_BANK_SLOT;
            }
            g_fieldMusicRequest = kind;
            g_fieldSoundLoadId = id;
            g_gameState->gameData->soundLoadState = SND_LOAD_WAIT_FADE;
            return g_gameState->gameData->soundLoadState;
        case SND_CMD_GET_STATE:
            goto state;
        }
        return -1;
    }
    return -1;

state:
    switch (g_gameState->gameData->soundLoadState) {
    case SND_LOAD_WAIT_FADE:
        if (fld_isMusicFading() == 0) {
            if (g_gameState->gameData->flags & GD_MUSIC_STOP_PENDING) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->flags &= ~GD_MUSIC_STOP_PENDING;
            }
            g_gameState->gameData->soundLoadState = SND_LOAD_START;
        }
        break;
    case SND_LOAD_START:
        if (((g_fieldSoundLoadId >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
            g_gameState->gameData->soundLoadBank = g_gameState->gameData->musicBank[0];
        } else {
            g_gameState->gameData->soundLoadBank = g_fieldSoundLoadId;
        }
        if (g_fieldMusicRequest == SND_CMD_RESUME) {
            g_fieldSoundLoadSector = g_gameState->gameData->musicFileSector;
            sz = g_gameState->gameData->musicFileSectors << 11;
        } else {
            g_fieldSoundLoadSector = fld_depGetFileSector(g_fieldSoundLoadId, &sz);
            if (((g_fieldSoundLoadId >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
                g_gameState->gameData->musicFileSector = g_fieldSoundLoadSector;
                g_gameState->gameData->musicFileSectors = sz >> 11;
            }
        }
        g = g_gameState->gameData;
        if (!(g->ctx.field->area->scene->flags & FLD_SCN_BANK_SLOT) && g->soundLoadBank != 0x84) {
            akao_beginBankUpload();
        }
        g_gameState->gameData->soundLoadDest = SCENE->heapTop ? SCENE->heapTop : g_modeHeap;
        g_gameState->gameData->soundLoadChunk = 0x156F00 - (g_gameState->gameData->soundLoadDest - (s32)g_loadAreaEnd);
        if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
            g_gameState->gameData->soundLoadChunk -= 0x3000;
        }
        m = g_gameState->gameData->soundLoadChunk;
        g_gameState->gameData->soundLoadChunk = m / 2048 * 2048;
        g_gameState->gameData->soundBankSize = 0;
        cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, sz, (void *)g_gameState->gameData->soundLoadDest, 0,
                      g_gameState->gameData->soundLoadChunk, fld_soundBankReadCallback);
        g_gameState->gameData->soundLoadState = SND_LOAD_BANK;
        break;
    case SND_LOAD_BANK:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0 && akao_getSpuTransferState() == 0) {
            fld_invalidateModelCache((s32)g_gameState->gameData->cdRead[0].end);
            preload_discardRange((s32)g_gameState->gameData->cdRead[0].dest, g_gameState->gameData->cdRead[0].chunk);
            if (g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->soundBankSize) {
                g_gameState->gameData->soundLoadDest = (s32)g_gameState->gameData->cdRead[0].dest;
                d = 0x156F00 - ((s32)g_gameState->gameData->cdRead[0].dest - (s32)g_loadAreaEnd);
                if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
                    d -= 0x3000;
                }
                d = d / 2048 * 2048;
                cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                              g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, d, fld_soundBankReadCallback);
            } else if (g_fieldSoundLoadKind == 7) {
                if (g_fieldMusicRequest != SND_CMD_RESUME) {
                    g_gameState->gameData->musicVolume[0] = 0x7F;
                }
                g_gameState->gameData->soundLoadState = SND_LOAD_SEQ_START;
            } else {
                g_fieldMusicRequest = -1;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
            }
        }
        break;
    case SND_LOAD_SEQ_START:
        n = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
        if (n > 0) {
            memcpy(g_gameState->gameData->musicSeq, (u8 *)g_gameState->gameData->soundLoadDest + (g_gameState->gameData->soundBankSize - g_gameState->gameData->cdRead[0].offset), n);
        }
        t = 0x6800 - n;
        g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + n);
        t = t / 2048 * 2048;
        cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                      g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, t, NULL);
        g_gameState->gameData->soundLoadState = SND_LOAD_SEQ;
        break;
    case SND_LOAD_SEQ:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0) {
            if (g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->cdRead[0].size) {
                m = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
                g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + m);
                cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest, m, 0x6800 - m, NULL);
            } else {
                fld_unpackMusicSeq((s32)g_gameState->gameData->musicSeq);
                g_gameState->gameData->flags |= GD_MUSIC_LOADED;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
                if (g_fieldMusicRequest != SND_CMD_LOAD) {
                    goto start;
                }
                g_fieldMusicRequest = -1;
            }
        }
        break;
    }
    return g_gameState->gameData->soundLoadState;
}

void fld_getSoundPanVolume(s16 posX, s16 posY, s16 posZ, u8 *outPan, u8 *outVolume) {
    SVECTOR v;
    DVECTOR sxy;
    s32 z;
    s32 a;
    s32 pan;
    s32 b;

    v.vx = posX;
    v.vy = posY;
    v.vz = posZ;
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_stsz(&z);
    z = z / 4 + g_gameState->gameData->ctx.field->area->scene->otBias;
    if (z < 0) {
        z = 0;
    } else if (z >= 0x1000) {
        z = 0xFFF;
    }
    a = z / 32;
    b = (128 - a) * 2;
    if ((u16)sxy.vx < 320) {
        pan = sxy.vx * b / 320;
        pan += a;
    } else if (sxy.vx < 0) {
        if (sxy.vx < -320) {
            sxy.vx = -320;
        }
        pan = sxy.vx + 320;
        pan = pan * a / 320;
    } else {
        if (sxy.vx >= 640) {
            sxy.vx = 639;
        }
        pan = (sxy.vx - 320) * a / 320;
        pan += 255;
        pan -= a;
    }
    *outPan = pan;
    *outVolume = (0x1000 - z) * 127 / 4096;
}

void fld_suspendResidentSfx(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        fld_soundCommand(((i & 1) << 6) | 0x1880, i, 0, 0, 0);
    }
}

void fld_resumeResidentSfx(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        fld_soundCommand(((i & 1) << 6) | 0x1980, i, 0, 0, 0);
    }
}

void fld_stopResidentSfx(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        if (g_gameState->gameData->sfxSlotId[i] != SE_NONE) {
            fld_soundCommand(SND_SE_STOP | SND_RESIDENT, g_gameState->gameData->sfxSlotId[i], 0, 0, 0);
        }
    }
}

void fld_clearSoundBankCache(void) {
    s32 i;

    for (i = 4; i >= 0; i--) {
        g_fieldSoundBankIds[i] = -1;
        g_fieldSoundBankSlots[i] = -1;
    }
}

/* drop the queued entries except categories 1 and 4 */
void fld_dropSoundBankCache(s32 size) {
    s32 i;
    u8 t;

    if (size >= 0x40000) {
        for (i = 4; i >= 0; i--) {
            t = (u32)g_fieldSoundBankIds[i] >> 24;
            if (t != 4 && t != 1) {
                g_fieldSoundBankIds[i] = -1;
                g_fieldSoundBankSlots[i] = -1;
            }
        }
    }
}

void fld_stopSceneSfx(FieldScene *scene, s16 unused) {
    s32 t = (s32)scene->fieldFile;
    s32 i;

    for (i = db_lookup(t, DB_GET_COUNT, DB_SE, 0) - 1; i >= 0; i--) {
        akao_stopSe(db_lookup(t, DB_GET_AT, (i & 0xFFFF) | DB_SE, 0), 0);
    }
    akao_sendCmdE2();
}

void fld_playAnimSfx(FieldObjSlot **slots, s32 count) {
    s16 a;
    s16 b;
    u16 c;
    u8 vol;
    u8 pan;
    FieldAnimSfx *base;
    u8 n;
    s32 i;
    s32 j;
    FieldObjSlot *ctx;
    FieldAnimSfx *t;
    s32 r;
    s32 ch;
    s32 snd;
    s32 v;

    base = g_gameState->gameData->ctx.field->area->scene->animSfx;
    n = g_gameState->gameData->ctx.field->area->scene->animSfxCount;
    for (i = count - 1; i >= 0; i--) {
        ctx = slots[i];
        if (ctx == NULL || (ctx->flags & FLD_OBJ_NO_ANIM_SFX)) {
            continue;
        }
        for (j = n - 1; j >= 0; j--) {
            t = &base[j];
            if (t->modelSlot == ctx->actor->modelSlot && t->anim == ctx->actor->anim && t->frame == ctx->actor->animFrame) {
                r = 0;
                a = 0;
                b = 0;
                c = 0;
                vol = 0x80;
                pan = 0x7F;
                bgi_getActorTriangle(ctx->actor->modelSlot, (u16 *)&a, (u16 *)&b);
                if (a != -1) {
                    bgi_getTriangleFlags(a, &c);
                    c = (s16)c >> 8;
                }
                ch = t->modelSlot & 3;
                ch = (0x10000 << ch) & 0xF0000;
                snd = t->sound[(c >> 4) & 1] | DB_SE;
                if (t->randomPitch != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                    v = *g_randPtr++;
                    r = (v >> 4) - 8;
                }
                fld_getSoundPanVolume(ctx->actor->pos.vx, ctx->actor->pos.vy, ctx->actor->pos.vz, &vol, &pan);
                fld_soundCommand(SND_SE_PLAY, snd, ch, vol, pan);
                if (r != 0) {
                    fld_soundCommand(SND_SE_PITCH, snd, ch, r, 0);
                }
            }
        }
    }
}

/* find the trigger matching the object, id and value (searching backwards) */
FieldAnimSfx *fld_findAnimSfx(obj, anim, frame)
    FieldObjSlot *obj;
    u16 anim;
    s16 frame;
{
    FieldScene *sc = g_gameState->gameData->ctx.field->area->scene;
    FieldAnimSfx *base = sc->animSfx;
    FieldAnimSfx *e = &base[sc->animSfxCount] - 1;

    do {
        if (e->modelSlot == obj->actor->modelSlot && e->anim == anim && e->frame == frame) {
            return e;
        }
        e--;
    } while (e >= base);
    return NULL;
}

FieldAnimSfx *fld_addAnimSfx(FieldObjSlot *obj, s32 anim, s32 frame) {
    FieldScene *sc = g_gameState->gameData->ctx.field->area->scene;
    u8 i = sc->animSfxCount;
    FieldAnimSfx *e;
    u16 id;
    s32 n;

    n = i + 1;
    sc->animSfxCount = n;
    e = &sc->animSfx[i];
    e->modelSlot = obj->actor->modelSlot;
    id = obj->actor->modelId;
    e->anim = anim;
    do {
        e->frame = frame;
    } while (0);
    e->modelId = id;
    return e;
}

s32 fld_removeAnimSfx(obj, anim, frame)
    FieldObjSlot *obj;
    u16 anim;
    s16 frame;
{
    FieldAnimSfx *base;
    FieldAnimSfx *last;
    FieldAnimSfx *e;

    base = g_gameState->gameData->ctx.field->area->scene->animSfx;
    e = &base[g_gameState->gameData->ctx.field->area->scene->animSfxCount] - 1;
    last = e;
    do {
        if (e->modelSlot == obj->actor->modelSlot && e->anim == anim && e->frame == frame) {
            if (e != last) {
                *e = *last;
            }
            g_gameState->gameData->ctx.field->area->scene->animSfxCount--;
        }
        e--;
    } while (e >= base);
    return -1;
}

s32 fld_depGetLink(s32 fileId) {
    FileTable *t = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *e = t->entries;

    for (i = t->count - 1; i >= 0; i--) {
        if (e[i].id == fileId) {
            return e[i].link;
        }
    }
    return -1;
}

void fld_findBattleMusic(void) {
    FieldScene *o;
    FileEntry *base;
    s32 i;
    s32 j;
    s32 id;
    s32 key;
    s32 v;
    s32 w;

    o = g_gameState->gameData->ctx.field->area->scene;
    o->battleMusic = g_gameState->gameData->musicId[0];
    key = (u16)o->nextId;
    g_gameState->gameData->ctx.field->area->scene->battleMusicBank = g_gameState->gameData->musicBank[0];
    base = g_gameState->gameData->depTable->entries;
    key |= DB_BATTLE;
    w = key;
    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        id = base[i].link;
        if (base[i].id == w && ((id >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
            g_gameState->gameData->ctx.field->area->scene->battleMusic = id;
            for (j = 0; j < g_gameState->gameData->depTable->count; j++) {
                w = base[j].link;
                v = base[j].id;
                if (((base[j].id >> 16) & 0xFF) == (DB_SOUND_BANK >> 16) && w == id) {
                    g_gameState->gameData->ctx.field->area->scene->battleMusicBank = v;
                    return;
                }
            }
            return;
        }
    }
}

/* assign sound slots to the field's sound files and load the missing ones (smallest first) */
s32 fld_loadSoundBanks(FieldScene *unused, s32 unused2) { /* its caller passes these, unused */
    u32 id[5];
    s32 sz[5];
    u8 used[5];
    s8 slot[5];
    s32 n;
    s32 i;
    s32 j;
    s32 cnt;
    s32 found;
    s32 s;
    s32 k;
    s32 done;
    s32 t;
    s32 u;
    s32 m;
    s32 min;
    s32 best;
    u32 x;
    u32 y;
    u32 z;
    FileEntry *e;

    n = 0;
    e = g_gameState->gameData->depTable->entries;
    for (i = 4; i >= 0; i--) {
        id[i] = -1;
        sz[i] = -1;
        used[i] = 0;
        slot[i] = -1;
    }
    for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
        x = e[i].id;
        if (((x >> 16) & 0xFF) == (DB_SOUND_BANK >> 16)) {
            found = 0;
            if (((e[i].link >> 16) & 0xFF) == (DB_SE >> 16)) {
                s = -1;
                for (j = 4; j >= 0; j--) {
                    if (g_fieldSoundBankIds[j] == x) {
                        found = 1;
                        s = g_fieldSoundBankSlots[j];
                        break;
                    }
                }
                id[n] = x;
                sz[n] = e[i].sector;
                used[n] = found;
                slot[n] = s;
                n++;
            }
        }
    }
    for (i = n - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4 && slot[i] == -1) {
            k = 0;
            do {
                done = 1;
                for (j = n - 1; j >= 0; j--) {
                    if (slot[j] == k) {
                        k++;
                        done = 0;
                        break;
                    }
                }
            } while (done == 0);
            slot[i] = k;
        }
    }
    cnt = 0;
    for (i = n - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4) {
            cnt++;
        }
    }
    if (cnt < 2) {
        for (m = 4; m >= 0; m--) {
            y = g_fieldSoundBankIds[m];
            if (y != -1 && (y >> 24) == 4) {
                t = -1;
                for (j = n - 1; j >= 0; j--) {
                    if (id[j] == y) {
                        t = m;
                        break;
                    }
                }
                if (t == -1) {
                    u = -1;
                    j = n - 1;
                    t = g_fieldSoundBankSlots[m];
                    for (; j >= 0; j--) {
                        if (slot[j] == t) {
                            u = j;
                            break;
                        }
                    }
                    if (u == -1) {
                        id[n] = y;
                        sz[n] = 0;
                        used[n] = 1;
                        slot[n] = t;
                        n++;
                        cnt++;
                        if (cnt >= 2) {
                            break;
                        }
                    }
                }
            }
        }
    }
    for (i = 4; i >= 0; i--) {
        g_fieldSoundBankIds[i] = id[i];
        g_fieldSoundBankSlots[i] = slot[i];
    }
    for (;;) {
        min = 0x7FFFFFFF;
        best = -1;
        for (i = n - 1; i >= 0; i--) {
            if (id[i] != -1 && used[i] == 0 && sz[i] < min) {
                min = sz[i];
                best = i;
            }
        }
        if (best == -1) {
            break;
        }
        z = id[best];
        fld_soundCommand(SND_BANK_LOAD, z, z >> 24, slot[best], 0);
        while (fld_soundCommand(SND_GET_STATE, -1, 0, 0, 0) != 0) {
        }
        id[best] = -1;
        sz[best] = -1;
    }
    return 0;
}

s32 fld_depGetFileSector(s32 fileId, s32 *outSize) {
    FileEntry *e = dep_find(g_gameState->gameData->depTable, fileId);

    if (outSize != NULL) {
        *outSize = e->sectors << 11;
    }
    return e->sector;
}

s32 fld_soundBankReadCallback(CdReadCtx *ctx) {
    s32 a;
    s32 id;
    s32 t;
    s32 n;
    s32 b;
    s32 r;
    GameData *gd;
    FieldScene *sc;

    r = 0;
    a = (s32)ctx->dest;
    id = g_gameState->gameData->soundLoadBank | DB_SOUND_BANK;
    if (ctx->offset == 0) {
        b = a;
        t = db_lookup(a, DB_GET_AT, DB_PACK, 0);
        if (t == -1) {
            a = db_lookup(a, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_getSize(b);
        } else {
            a = db_lookup(t, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_lookup(b, DB_GET_AT, (DB_PACK | 1), 0) - g_gameState->gameData->soundLoadDest;
            fld_dropSoundBankCache(g_gameState->gameData->soundBankSize - ctx->offset);
        }
    }
    gd = g_gameState->gameData;
    n = gd->soundBankSize - ctx->offset;
    if (ctx->chunk < n) {
        n = ctx->chunk;
    }
    n = (s32)ctx->dest + n - a;
    if ((id & 0xFFFF) != 0x84) {
        sc = gd->ctx.field->area->scene;
        if (sc->flags & FLD_SCN_BANK_SLOT) {
            r = akao_loadBankInSlot(a, sc->soundBankSlot, 0);
        } else {
            r = akao_uploadBank(a, n, 0);
        }
    }
    ctx->dest = (u8 *)a;
    return r;
}

s32 fld_unpackMusicSeq(s32 db) {
    s32 t = db_lookup(db, DB_GET_KEY_AT, DB_MUSIC, 0);

    memcpy((u8 *)db, (u8 *)db_lookup(db, DB_GET_ADDR, t, 0), db_lookup(db, DB_GET_SIZE, t, 0));
    return 0;
}

void fld_setMusicVolumeFade(s32 volume, s32 frames) {
    GameData *gd = g_gameState->gameData;

    gd->musicVolume[0] = volume;
    if (volume == 0) {
        gd->musicFadeFrames = frames;
        gd->musicFadeStart = VSync(-1);
    }
}

s32 fld_isMusicFading(void) {
    GameData *gd = g_gameState->gameData;

    return (u32)(VSync(-1) - gd->musicFadeStart) < gd->musicFadeFrames;
}

s32 fld_setObjectColor(s32 objId, s32 r, s32 g, s32 b) {
    return fld_setSlotColor((FieldObjSlot *)game_getModel(objId), r, g, b);
}

/* apply the field lights to the object colours */
void fld_applyLights(FieldObjSlot **slots, s32 count) {
    FieldScene *sc;
    FieldLightSet *lt;
    FieldLight *amb;
    FieldLight *dir;
    FieldActorInfo *infos;
    FieldModelLight *def;
    s32 i;
    FieldObjSlot *c;
    s32 chg;
    FieldActorInfo *info;
    FieldModelLight *l;
    u32 shade;
    FieldLight *fl;
    s16 floor;
    u32 a;
    u32 b;
    u32 col;
    s32 dx;
    s32 dy;
    s32 j;
    s32 k;
    FieldActorInfo *inf2;

    sc = g_gameState->gameData->ctx.field->area->scene;
    lt = sc->lights;
    if (lt == NULL || fld_getLightColorTable(lt) == NULL) {
        return;
    }
    amb = NULL;
    dir = NULL;
    infos = sc->actorInfo;
    def = (FieldModelLight *)fld_findLightColor(lt, -1);
    fld_findGlobalLights(lt, &amb, &dir);
    for (i = count - 1; i >= 0; i--) {
        c = slots[i];
        chg = 0;
        if (c == NULL) {
            continue;
        }
        info = &infos[c->actor->modelSlot];
        l = (FieldModelLight *)fld_findLightColor(lt, c->modelId);
        shade = c->flags & FLD_OBJ_DIR_LIGHT;
        fl = fld_findFloorLight(lt, c, &floor);
        if ((u8)info->litFloor != (u8)floor || shade != (info->flags & FLD_INFO_DIR_LIGHT) || !(c->flags & FLD_OBJ_LIT)) {
            chg = 1;
        }
        if (chg) {
            if (!(c->flags & FLD_OBJ_COLOR_SET)) {
                b = 0;
                if (l != NULL) {
                    a = l->color;
                } else {
                    a = def->color;
                }
                if (shade && dir != NULL) {
                    b = dir->color;
                } else if (fl != NULL) {
                    b = fl->color;
                } else if (amb != NULL) {
                    b = amb->color;
                }
                col = ((((a & 0xFF) + (b & 0xFF)) * 8) & 0xFF) | ((((a & 0xFF00) + (b & 0xFF00)) * 8) & 0xFF00)
                    | ((((a & 0xFF0000) + (b & 0xFF0000)) * 8) & 0xFF0000) | 0x01000000;
                info->nextColor.w = col;
                info->color.w = col;
                c->flags |= FLD_OBJ_LIT;
            }
            if (l != NULL) {
                dx = l->shadowShade;
                dy = l->shadowSize;
            } else {
                dx = def->shadowShade;
                dy = def->shadowSize;
            }
            if (fl != NULL) {
                dx += fl->shadowShade;
                dy += fl->shadowSize;
            } else if (amb != NULL) {
                dx += amb->shadowShade;
                dy += amb->shadowSize;
            }
            dx *= 8;
            g_gameState->gameData->ctx.field->area->scene->shadows[c->actor->modelSlot].params.shade = dx;
            g_gameState->gameData->ctx.field->area->scene->shadows[c->actor->modelSlot].params.scaleX = dy * 14;
            g_gameState->gameData->ctx.field->area->scene->shadows[c->actor->modelSlot].params.scaleZ = dy * 12;
        }
        info->litFloor = floor;
        info->flags = (info->flags & ~FLD_INFO_DIR_LIGHT) | shade;
    }
    for (i = count - 1; i >= 0; i--) {
        c = slots[i];
        if (c == NULL) {
            continue;
        }
        inf2 = &infos[c->actor->modelSlot];
        k = inf2->color.w;
        if (k >> 24 != 0) {
            for (j = ((ModelObj *)c->model)->model->groupCount - 1; j >= 0; j--) {
                fldmdl_setGroupRgb(c->model, j & 0xFF, (s16)(k & 0xFF), (s16)((k >> 8) & 0xFF), ((u32)k >> 16) & 0xFF);
            }
            inf2->color.w = inf2->nextColor.w;
            inf2->nextColor.w = 0;
        }
    }
}

/* set the object's light colour */
s32 fld_setSlotColor(FieldObjSlot *obj, s32 r, s32 g, s32 b) {
    u32 f = obj->flags;
    FieldActorInfo *info = &g_gameState->gameData->ctx.field->area->scene->actorInfo[obj->actor->modelSlot];
    u32 col;

    if (!(f & FLD_OBJ_COLOR_WAIT)) {
        col = (r & 0xFF) | ((g << 8) & 0xFF00) | ((b << 16) & 0xFF0000) | 0x01000000;
        info->nextColor.w = col;
        info->color.w = col;
        obj->flags |= FLD_OBJ_COLOR_SET | FLD_OBJ_COLOR_WAIT;
        return 1;
    }
    obj->flags = f & ~FLD_OBJ_COLOR_WAIT;
    return 0;
}

FieldLight *fld_getLightColorTable(FieldLightSet *set) {
    if (set != NULL && set->colorCount != 0) {
        return &set->lights[set->colorOffset];
    }
    return NULL;
}

FieldModelLight *fld_findLightColor(set, modelId)
    FieldLightSet *set;
    u16 modelId;
{
    FieldModelLight *base;
    FieldModelLight *p;
    s32 id;
    u32 v;

    v = modelId;
    if (set->colorCount == 0) {
        return NULL;
    }
    id = v & 0xFFFF;
    base = (FieldModelLight *)&set->lights[set->colorOffset];
    p = &base[set->colorCount] - 1;
    do {
        if (p->modelId == id) {
            return p;
        }
        p--;
    } while (p >= base);
    return NULL;
}

FieldLight *fld_findGlobalLights(FieldLightSet *set, FieldLight **outDefault, FieldLight **outDir) {
    FieldLight *base;
    FieldLight *p;

    *outDefault = NULL;
    *outDir = NULL;
    base = set->lights;
    if (set->lightCount == 0) {
        return NULL;
    }
    p = &set->lights[set->lightCount - 1];
    do {
        if (p->type == FLD_LIGHT_DEFAULT) {
            *outDefault = p;
        } else if (p->type == FLD_LIGHT_DIR) {
            *outDir = p;
        }
        p--;
    } while (p >= base);
    return base;
}

/* find the floor light covering the object's current floor */
FieldLight *fld_findFloorLight(FieldLightSet *set, FieldObjSlot *obj, s16 *outFloor) {
    u16 tri;
    FieldLight *p;
    s32 n;
    s32 v;

    bgi_getActorTriangle(obj->actor->modelSlot, &tri, (u16 *)outFloor);
    if (set->lightCount != 0) {
        if ((u8)*outFloor == 0xFF) {
            return NULL;
        }
        p = &set->lights[set->lightCount - 1];
        do {
            if (p->type == FLD_LIGHT_FLOOR) {
                v = p->floors;
                for (n = 3; n >= 0; n--) {
                    if ((v & 0xFF) == *outFloor) {
                        return p;
                    }
                    v >>= 8;
                }
            }
            p--;
        } while (p >= set->lights);
    }
    return NULL;
}

s32 fld_enableRain(s32 arg0, s32 arg1, s32 enable) {
    s32 start;
    s32 size;
    s32 end;

    if (arg0 == 0 && arg1 == 0) {
        if (enable != 0) {
            if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_RAIN_ALLOC)) {
                start = g_gameState->gameData->ctx.field->area->scene->heapTop;
                if (start == 0) {
                    start = g_modeHeap;
                }
                if (preload_overlaps(start, 0x5000) != 0) {
                    /* preload_waitCurrent returns NULL when no preload is in flight (seen in d4_f2928), so the PS1 reads fileId from low
                     * RAM here. Harmless on the PS1; a native port must check for NULL (ff9-ds F37). */
                    preload_remove(preload_waitCurrent()->fileId);
                }
                if (g_gameState->gameData->ctx.field->area->scene->heapTop != 0) {
                    start = g_gameState->gameData->ctx.field->area->scene->heapTop;
                    g_gameState->gameData->ctx.field->area->scene->heapTop = (s32)rain_init((void *)start);
                    size = g_gameState->gameData->ctx.field->area->scene->heapTop - start;
                } else {
                    start = g_modeHeap;
                    end = (s32)rain_init((void *)start);
                    size = end - start;
                    g_modeHeap = end;
                }
                fld_trimModelCache();
                preload_discardRange(start, size);
                g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_RAIN_ALLOC;
            }
            g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_RAIN_ON;
        } else {
            g_gameState->gameData->ctx.field->area->scene->flags &= ~FLD_SCN_RAIN_ON;
        }
    }
    return 0;
}

void fld_drawRain(void) {
    if (g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_RAIN_ON) {
        rain_draw();
    }
}

s32 fld_preloadRequest(s32 typeArg, s32 fileId) {
    s32 flag;
    s32 id;
    s32 k;
    u8 type;
    s32 x;
    PreloadEntry *e;
    PreloadEntry *p;
    PreloadEntry *q;
    FileEntry *f;

    type = typeArg;
    flag = 0;
    if ((s8)type < 0) {
        type -= 0x80;
        flag = 1;
    }
    switch (type & 0xFF) {
    default:
        return -1;
    case 0x11:
        k = OVL_KEY(OVL_BATTLE);
        if (flag && fld_findPreloadExclude(OVL_KEY(OVL_BATTLE))) {
            return -1;
        }
        if (fld_findPreloadRequest(OVL_KEY(OVL_BATTLE))) {
            return -1;
        }
        if (preload_find(OVL_KEY(OVL_BATTLE))) {
            return -1;
        }
        if (flag) {
            fld_addPreloadExclude(OVL_KEY(OVL_BATTLE));
        } else {
            fld_addPreloadRequest(OVL_KEY(OVL_BATTLE));
        }
        if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
            goto done;
        }
        p = preload_getCurrent();
        if (p != NULL && p->fileId != k) {
            preload_wait(p);
        }
        if (flag || preload_find(k) != NULL) {
            goto set;
        }
        goto clear;
    case 5:
        id = (u16)fileId | DB_FIELD;
        if ((u16)fileId >= 9000) {
            return -1;
        }
        f = dep_find(g_gameState->gameData->depTable, id);
        if (f == NULL) {
            return -1;
        }
        if (f->sectors == 0) {
            return -1;
        }
        if (flag && fld_findPreloadExclude(id)) {
            return -1;
        }
        if (fld_findPreloadRequest(id)) {
            return -1;
        }
        e = preload_find(id);
        if (e != NULL && e->kind == PRELOAD_KIND_READ) {
            if (preload_isBusy(e)) {
                return -1;
            }
            if (!preload_isBusy(e) && (e->readSize >> 2) < e->loaded) {
                return -1;
            }
        }
        if (flag) {
            x = id;
        } else {
            fld_addPreloadRequest(id);
            x = OVL_KEY(OVL_BATTLE);
        }
        fld_addPreloadExclude(x);
        if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
            goto done;
        }
        e = preload_getCurrent();
        if (e != NULL && e->fileId != id && preload_isBusy(e)) {
            preload_wait(e);
        }
        q = preload_find(id);
        if (!flag && (q == NULL || q->kind != PRELOAD_KIND_READ)) {
        clear:
            g_gameState->gameData->ctx.field->area->scene->flags &= ~FLD_SCN_PRELOADS_QUEUED;
        }
    set:
        g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_PRELOAD_RESTART;
    done:
        g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_PRELOAD_REQUESTED;
        return 0;
    }
}

void fld_initPreload(void) {
    preload_init();
    g_gameState->gameData->preloadRequest = fld_preloadRequest;
    preload_setHook((s32)fld_preloadHook);
    fld_clearPreloadLists();
}

void fld_clearPreloadLists(void) {
    s32 i;

    g_fieldPreloadRequestCount = 0;
    for (i = 7; i >= 0; i--) {
        g_fieldPreloadRequests[i] = -1;
    }
    g_fieldPreloadExcludeCount = 0;
    for (i = 7; i >= 0; i--) {
        g_fieldPreloadExcludes[i] = -1;
    }
}

void fld_setPreloadCursor(s32 cursor) {
    if (cursor != 0) {
        g_fieldPreloadCursor = cursor;
    }
}

s32 fld_isEncounterPreloadRequested(void) {
    return fld_findPreloadRequest(OVL_KEY(OVL_BATTLE)) != 0;
}

s32 fld_isEncounterPreloadExcluded(void) {
    return fld_findPreloadExclude(OVL_KEY(OVL_BATTLE)) != 0;
}

/* is any active preload request a field file (DB_FIELD)? */
s32 fld_hasFieldPreloadRequest(void) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (((g_fieldPreloadRequests[i] >> 16) & 0xFF) == (DB_FIELD >> 16)) {
            return 1;
        }
    }
    return 0;
}

s32 *fld_findPreloadRequest(s32 fileId) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (g_fieldPreloadRequests[i] == fileId) {
            return &g_fieldPreloadRequests[i];
        }
    }
    return NULL;
}

void fld_removePreloadRequest(s32 fileId) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (g_fieldPreloadRequests[i] == fileId) {
            if (i < g_fieldPreloadRequestCount - 1) {
                g_fieldPreloadRequests[i] = g_fieldPreloadRequests[g_fieldPreloadRequestCount - 1];
            }
            g_fieldPreloadRequestCount--;
            return;
        }
    }
}

s32 *fld_addPreloadRequest(s32 fileId) {
    s32 *p = fld_findPreloadRequest(fileId);

    if (p != NULL) {
        return p;
    }
    if (g_fieldPreloadRequestCount < 8) {
        p = &g_fieldPreloadRequests[g_fieldPreloadRequestCount];
        g_fieldPreloadRequestCount++;
        *p = fileId;
        return p;
    }
    return NULL;
}

s32 *fld_findPreloadExclude(s32 fileId) {
    s32 i;

    for (i = g_fieldPreloadExcludeCount - 1; i >= 0; i--) {
        if (g_fieldPreloadExcludes[i] == fileId) {
            return &g_fieldPreloadExcludes[i];
        }
    }
    return NULL;
}

void fld_removePreloadExclude(s32 fileId) {
    s32 i;

    for (i = g_fieldPreloadExcludeCount - 1; i >= 0; i--) {
        if (g_fieldPreloadExcludes[i] == fileId) {
            if (i < g_fieldPreloadExcludeCount - 1) {
                g_fieldPreloadExcludes[i] = g_fieldPreloadExcludes[g_fieldPreloadExcludeCount - 1];
            }
            g_fieldPreloadExcludeCount--;
            return;
        }
    }
}

s32 *fld_addPreloadExclude(s32 fileId) {
    s32 *p = fld_findPreloadExclude(fileId);

    if (p != NULL) {
        return p;
    }
    if (g_fieldPreloadExcludeCount < 8) {
        p = &g_fieldPreloadExcludes[g_fieldPreloadExcludeCount];
        g_fieldPreloadExcludeCount++;
        *p = fileId;
        return p;
    }
    return NULL;
}

/* start streaming a file into the free area */
s32 fld_preloadFile(s32 fileId, s32 kind, s32 sector, s32 chunkSize) {
    u32 top;
    s32 size;
    PreloadEntry *p;

    if (fileId == -1 || sector == 0) {
        return 0;
    }
    if (kind == 0 && chunkSize == 0) {
        return 0;
    }
    top = fld_getModelCacheBottom(NULL);
    if ((u32)(g_loadAreaEnd + 0x153F00) < top) {
        top = (u32)(g_loadAreaEnd + 0x153F00);
    }
    size = top - g_fieldPreloadCursor;
    size = size / 2048 * 2048;
    if (kind == 0 && size < 0x8000) {
        return 0;
    }
    p = preload_add();
    if (p == NULL) {
        return 0;
    }
    preload_setEntry(p, kind, fileId, kind == 0 ? g_fieldPreloadCursor : 0, size, chunkSize, sector, chunkSize);
    g_fieldPreloadCursor += chunkSize;
    return (s32)p;
}

static inline s32 fld_queueFieldPreloads_grp(s32 id) {
    return id / 50;
}

void fld_queueFieldPreloads(void) {
    FileEntry *tbl;
    FileEntry *e;
    FileEntry *f;
    FileEntry *g;
    s32 i;
    s32 cnt;
    s32 best;
    s32 sel;
    s32 cur;
    s32 key;
    s32 x;

    tbl = g_gameState->gameData->depTable->entries;
    key = fld_isEncounterPreloadExcluded();
    if (key == 0) {
        cnt = 0;
        key = fld_isEncounterPreloadRequested();
        if (key != 0) {
            cnt = 1;
        } else {
            for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                if (((tbl[i].id >> 16) & 0xFF) == (DB_BATTLE >> 16)) {
                    cnt = 1;
                    break;
                }
            }
        }
        if (cnt != 0) {
            g = file_find(DIR_OVERLAYS, OVL_KEY(OVL_BATTLE));
            fld_preloadFile(OVL_KEY(OVL_BATTLE), 0, g->sector, (g->link - g->sector) << 11);
            key = fld_isEncounterPreloadRequested();
            if (key != 0) {
                fld_removePreloadRequest(OVL_KEY(OVL_BATTLE));
            }
        }
    }
    if (fld_hasFieldPreloadRequest() != 0) {
        for (i = 0; i < g_fieldPreloadRequestCount; i++) {
            key = g_fieldPreloadRequests[i];
            if (((g_fieldPreloadRequests[i] >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                e = dep_find(g_gameState->gameData->depTable, key);
                if (e != NULL) {
                    if ((e->link & 0xFFFF) != (s16)g_gameState->gameData->fieldAreaId) {
                        f = dep_find(g_gameState->gameData->depTable, e->link);
                        if (f != NULL) {
                            fld_preloadFile(e->link & 0xFFFFFF, 0, f->sector, f->sectors << 11);
                        }
                    }
                    fld_preloadFile(key, 0, e->sector, e->sectors << 11);
                    fld_removePreloadRequest(key);
                }
            }
        }
    } else {
        best = 0;
        sel = -1;
        cur = -1;
        for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
            x = tbl[i].id;
            if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
                    fld_queueFieldPreloads_grp(x & 0xFFFF) == fld_queueFieldPreloads_grp(g_gameState->gameData->fieldId) && best < tbl[i].sectors) {
                    best = tbl[i].sectors;
                    sel = i;
                }
            }
        }
        if (sel == -1) {
            for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                x = tbl[i].id;
                if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                    if ((x & 0xFFFF) < 9000 && fld_findPreloadExclude(x) == NULL && (x & 0xFFFF) == g_gameState->gameData->ctx.field->prevFieldId) {
                        best = tbl[i].sectors;
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel == -1) {
            for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                x = tbl[i].id;
                if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                    if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
                        best < tbl[i].sectors) {
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel != -1) {
            if ((tbl[sel].link & 0xFFFF) != (s16)g_gameState->gameData->fieldAreaId) {
                for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                    if (tbl[i].id == tbl[sel].link) {
                        fld_preloadFile(tbl[i].id & 0xFFFFFF, 0, tbl[i].sector, tbl[i].sectors << 11);
                        break;
                    }
                }
            }
            cnt = tbl[sel].id & 0xFFFFFF;
            fld_preloadFile(cnt, 0, tbl[sel].sector, tbl[sel].sectors << 11);
            cur = cnt;
        }
        if (cur != -1) {
            best = 0;
            sel = -1;
            for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                x = tbl[i].id;
                if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                    key = x & 0xFFFF;
                    if (key < 9000 && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
                        fld_findPreloadExclude(x) == NULL && fld_queueFieldPreloads_grp(key) == fld_queueFieldPreloads_grp(g_gameState->gameData->fieldId) &&
                        best < tbl[i].sectors) {
                        best = tbl[i].sectors;
                        sel = i;
                        break;
                    }
                }
            }
            if (sel == -1) {
                for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                    x = tbl[i].id;
                    if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != (cur & 0xFFFF) && fld_findPreloadExclude(x) == NULL &&
                            (x & 0xFFFF) == g_gameState->gameData->ctx.field->prevFieldId) {
                            best = tbl[i].sectors;
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel == -1) {
                for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                    x = tbl[i].id;
                    if (tbl[i].sectors != 0 && ((x >> 16) & 0xFF) == (DB_FIELD >> 16)) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
                            fld_findPreloadExclude(x) == NULL && best < tbl[i].sectors) {
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel != -1) {
                if ((tbl[sel].link & 0xFFFF) != (s16)g_gameState->gameData->fieldAreaId) {
                    for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
                        if (tbl[i].id == tbl[sel].link) {
                            fld_preloadFile(tbl[i].id & 0xFFFFFF, 1, tbl[i].sector, -1);
                            return;
                        }
                    }
                } else {
                    fld_preloadFile(tbl[sel].id & 0xFFFFFF, 1, tbl[sel].sector, -1);
                }
            }
        }
    }
}

s32 fld_preloadHook(void) {
    FieldScene *s;

    if ((u32)(g_gameState->gameData->soundLoadState - 1) < 5) {
        return 1;
    }
    s = g_gameState->gameData->ctx.field->area->scene;
    if (s->flags & FLD_SCN_PRELOAD_RESTART) {
        if (preload_poll() <= 0) {
            preload_reset();
            if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
                fld_setPreloadCursor(g_gameState->gameData->ctx.field->area->scene->heapTop);
                fld_queueFieldPreloads();
                g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_PRELOADS_QUEUED;
            }
            g_gameState->gameData->ctx.field->area->scene->flags &= ~FLD_SCN_PRELOAD_RESTART;
        }
        return 1;
    }
    if (s->flags & FLD_SCN_PRELOADS_QUEUED) {
        return 0;
    }
    if (s->flags & FLD_SCN_ACTORS_READY) {
        fld_setPreloadCursor(s->heapTop);
        fld_queueFieldPreloads();
        g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_PRELOADS_QUEUED;
    }
    return 0;
}

void fld_showSplashImage(void) {
    SPRT *spr[2];
    DR_TPAGE *tp[2];
    RECT *clut;
    RECT *img;
    u8 *p;
    s32 tim;
    s32 i;
    s32 n;
    u32 state;

    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44E20, 0);
    file_getTimRects((void *)tim, &clut, &img);
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    file_loadTim(tim);
    DrawSync(0);
    n = 0;
    state = 0;
    i = 1;
    p = g_gameState->gameData->ctx.field->area->scene->heapTop;
    do {
        {
            /* dead struct initializer: it emits only a CLOBBER of q, which sched2 places in one of
               the loop's load-delay gaps (so p += 20, the tpage constant and &tp[0] land one gap
               later, as in the original), and q's spill slot is the frame's otherwise unused word */
            struct { s32 x; } q = { i };
        }
        spr[i] = (SPRT *)p;
        p += sizeof(SPRT);
        setSprt(spr[i]);
        setRGB0(spr[i], 0x80, 0x80, 0x80);
        spr[i]->x0 = (0x140 - img->w * 4) >> 1;
        spr[i]->y0 = (0xE0 - img->h) / 2;
        spr[i]->u0 = 0;
        spr[i]->v0 = 0;
        spr[i]->w = img->w * 4 - 1;
        spr[i]->h = img->h;
        spr[i]->clut = getClut(clut->x, clut->y);
        tp[i] = (DR_TPAGE *)p;
        p += sizeof(DR_TPAGE);
        setDrawTPage(tp[i], 1, 1, getTPage(0, 0, img->x, img->y));
        i--;
    } while (i >= 0);
    fld_invalidateModelCache((s32)p);
    preload_reset();
    game_setClearColor(0, 0, 0);
    do {
        ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
        switch (state) {
        case 0:
            n = 0x3C;
            i = 0;
            game_startFade(1, 1, 0, 0xFFFFFF);
            game_startFade(1, 1, n, i);
            state = 1;
            break;
        case 1:
            if (i < n) {
                i++;
            } else {
                n = 0x78;
                i = 0;
                state = 2;
            }
            break;
        case 2:
            if (i < n) {
                i++;
            } else {
                n = 0x3C;
                i = 0;
                game_startFade(1, 1, n, 0xFFFFFF);
                state = 3;
            }
            break;
        case 3:
            if (i < n) {
                i++;
            } else {
                state = 4;
            }
            break;
        }
        addPrim((u32 *)g_gameState->gameData->ot[g_gameState->gameData->drawBuf][8], spr[g_gameState->gameData->drawBuf]);
        addPrim((u32 *)g_gameState->gameData->ot[g_gameState->gameData->drawBuf][8], tp[g_gameState->gameData->drawBuf]);
        game_updateFade();
        DrawSync(0);
        g_gameState->vsyncMode = 2;
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
        DrawOTagEnv(g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + OT_LAST, &g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
        g_gameState->gameData->drawBuf ^= 1;
    } while (state < 4);
}

s32 fld_needsSplashImage(void) {
    GameData *gd = g_gameState->gameData;
    FieldScene *sc = gd->ctx.field->area->scene;

    if (sc->exitReason == FLD_EXIT_FIELD && (s16)gd->fieldId == 0x34 && sc->nextId == 100) {
        return 1;
    }
    return 0;
}

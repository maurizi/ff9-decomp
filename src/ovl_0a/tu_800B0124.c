#include "ovl_0a.h"

/* field music/sound request handler (GameData.unk83C); field version of ovl_0c wm_soundCommand */
s32 fld_soundCommand(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    GameData *gd;
    s32 kind;
    s32 id;
    s32 bankId;
    s32 found;
    s32 chunk;
    s32 h;
    s32 ok;
    s32 slot;
    s32 fp;
    s32 playingBit;
    s32 loadedBit;
    s32 sz;
    s32 seqBytes;
    s32 val; /* bank chunk size, then the sequence read offset (separate locals don't match) */
    s32 room;

    kind = SND_CMD(cmd);
    if (kind == SND_CMD_GET_STATE && g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
        goto state;
    }
    switch (SND_TYPE(cmd)) {
    case SND_TYPE_MUSIC:
        bankId = -1;
        id = (soundId & 0xFFFF) | DB_MUSIC;
        switch (kind) {
        case SND_CMD_RESUME:
            ok = 0;
            if (g_gameState->gameData->savedMusicId != MUSIC_NONE && g_gameState->gameData->savedMusicField == (s16)g_gameState->gameData->fieldId) {
                GameData *gd2;
                ok = 1;
                g_fieldMusicRequest = kind;
                gd2 = g_gameState->gameData;
                gd2->musicVolume[0] = gd2->savedMusicVolume;
                soundId = gd2->savedMusicId;
                id = gd2->savedMusicId | DB_MUSIC;
                bankId = gd2->savedMusicBank | DB_SOUND_BANK;
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
                bankId = fld_depGetLink(id);
            }
            if (g_gameState->gameData->musicId[0] != soundId || g_gameState->gameData->musicBank[0] != (u16)bankId) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    g_gameState->gameData->flags |= GD_MUSIC_STOP_PENDING;
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                SCENE->flags &= ~FLD_SCN_BANK_SLOT;
                g_gameState->gameData->musicBank[0] = bankId;
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
            if (SND_PARAM(cmd) == SND_PARAM_VOLUME) {
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
            if (SND_PARAM(cmd) == SND_PARAM_NOP) {
                return 0;
            }
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME ... SND_PARAM_PITCH:
                switch (SND_PARAM(cmd)) {
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
            switch (SND_PARAM(cmd)) {
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
            switch (SND_PARAM(cmd)) {
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
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            fld_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, param1, param2, param3);
            fld_setMusicVolumeFade(param3, param1);
            return 0;
        case SND_CMD_SET_CONDITION:
            if (SND_PARAM(cmd) != SND_PARAM_CONDITION) {
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
                param1 = SND_SE_GROUP_SLOT(slot) & SND_SE_GROUP_SLOTS;
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
            found = 0;
            if (g_gameState->gameData->sfxSlotId[slot] != SE_NONE && g_gameState->gameData->sfxSlotSuspendField[slot] == (s16)g_gameState->gameData->fieldId) {
                param1 = SND_SE_GROUP_SLOT(slot) & SND_SE_GROUP_SLOTS;
                found = 1;
                param2 = g_gameState->gameData->sfxSlotPan[slot];
                param3 = g_gameState->gameData->sfxSlotVolume[slot];
            }
            g_gameState->gameData->sfxSlotSuspendField[slot] = 0xFFFF;
            if (!found) {
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
            if (SND_PARAM(cmd) == SND_PARAM_ALL_SLOTS && fp) {
                if (g_gameState->gameData->flags & GD_SFX0_PLAYING) {
                    akao_stopSe((s32)g_gameState->gameData->sfxSlotBuf[0], SND_SE_GROUP_SLOT(0));
                }
                if (g_gameState->gameData->flags & GD_SFX1_PLAYING) {
                    akao_stopSe((s32)g_gameState->gameData->sfxSlotBuf[1], SND_SE_GROUP_SLOT(1));
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
            switch (SND_PARAM(cmd)) {
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
            case SND_PARAM_NOP:
                return 0;
            }
            return -1;
        case SND_CMD_FADE:
            switch (SND_PARAM(cmd)) {
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
            switch (SND_PARAM(cmd)) {
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
            switch (SND_PARAM(cmd)) {
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
        id = (soundId & 0xFFFF) | DB_SND_STREAM;
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
            switch (SND_PARAM(cmd)) {
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
            case SND_PARAM_NOP:
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
        gd = g_gameState->gameData;
        if (!(gd->ctx.field->area->scene->flags & FLD_SCN_BANK_SLOT) && gd->soundLoadBank != SND_BANK_COMMON) {
            akao_beginBankUpload();
        }
        g_gameState->gameData->soundLoadDest = SCENE->heapTop ? SCENE->heapTop : g_modeHeap;
        g_gameState->gameData->soundLoadChunk = FLD_WORK_SIZE - (g_gameState->gameData->soundLoadDest - (s32)g_loadAreaEnd);
        if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
            g_gameState->gameData->soundLoadChunk -= FLD_WORK_SWIRL_SIZE;
        }
        val = g_gameState->gameData->soundLoadChunk;
        g_gameState->gameData->soundLoadChunk = val / 2048 * 2048;
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
                room = FLD_WORK_SIZE - ((s32)g_gameState->gameData->cdRead[0].dest - (s32)g_loadAreaEnd);
                if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
                    room -= FLD_WORK_SWIRL_SIZE;
                }
                room = room / 2048 * 2048;
                cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                              g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, room, fld_soundBankReadCallback);
            } else if (g_fieldSoundLoadKind == (DB_MUSIC >> 16)) {
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
        seqBytes = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
        if (seqBytes > 0) {
            memcpy(g_gameState->gameData->musicSeq, (u8 *)g_gameState->gameData->soundLoadDest + (g_gameState->gameData->soundBankSize - g_gameState->gameData->cdRead[0].offset), seqBytes);
        }
        chunk = 0x6800 - seqBytes;
        g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + seqBytes);
        chunk = chunk / 2048 * 2048;
        cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                      g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, chunk, NULL);
        g_gameState->gameData->soundLoadState = SND_LOAD_SEQ;
        break;
    case SND_LOAD_SEQ:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0) {
            if (g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->cdRead[0].size) {
                val = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
                g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + val);
                cdRead_init(g_gameState->gameData->cdRead, g_fieldSoundLoadSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest, val, 0x6800 - val, NULL);
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
    SVECTOR pos;
    DVECTOR sxy;
    s32 z;
    s32 edge;
    s32 pan;
    s32 span;

    pos.vx = posX;
    pos.vy = posY;
    pos.vz = posZ;
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(&pos);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_stsz(&z);
    z = z / 4 + g_gameState->gameData->ctx.field->area->scene->otBias;
    if (z < 0) {
        z = 0;
    } else if (z >= OT_LEN) {
        z = OT_LAST;
    }
    edge = z / 32;
    span = (128 - edge) * 2;
    if ((u16)sxy.vx < SCREEN_W) {
        pan = sxy.vx * span / SCREEN_W;
        pan += edge;
    } else if (sxy.vx < 0) {
        if (sxy.vx < -SCREEN_W) {
            sxy.vx = -SCREEN_W;
        }
        pan = sxy.vx + SCREEN_W;
        pan = pan * edge / SCREEN_W;
    } else {
        if (sxy.vx >= (2 * SCREEN_W)) {
            sxy.vx = (2 * SCREEN_W - 1);
        }
        pan = (sxy.vx - SCREEN_W) * edge / SCREEN_W;
        pan += 255;
        pan -= edge;
    }
    *outPan = pan;
    *outVolume = (OT_LEN - z) * 127 / OT_LEN;
}

void fld_suspendResidentSfx(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        fld_soundCommand(((i & 1) << 6) | SND_SE_SUSPEND | SND_RESIDENT, i, 0, 0, 0);
    }
}

void fld_resumeResidentSfx(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        fld_soundCommand(((i & 1) << 6) | SND_SE_RESUME | SND_RESIDENT, i, 0, 0, 0);
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
    u8 category;

    if (size >= 0x40000) {
        for (i = 4; i >= 0; i--) {
            category = (u32)g_fieldSoundBankIds[i] >> 24;
            if (category != 4 && category != 1) {
                g_fieldSoundBankIds[i] = -1;
                g_fieldSoundBankSlots[i] = -1;
            }
        }
    }
}

void fld_stopSceneSfx(FieldScene *scene, s16 unused) {
    s32 fieldDb = (s32)scene->fieldFile;
    s32 i;

    for (i = db_lookup(fieldDb, DB_GET_COUNT, DB_SE, 0) - 1; i >= 0; i--) {
        akao_stopSe(db_lookup(fieldDb, DB_GET_AT, (i & 0xFFFF) | DB_SE, 0), 0);
    }
    akao_sendCmdE2();
}

void fld_playAnimSfx(FieldObjSlot **slots, s32 count) {
    s16 tri;
    s16 triAlt;
    u16 triFlags;
    u8 vol;
    u8 pan;
    FieldAnimSfx *base;
    u8 sfxCount;
    s32 i;
    s32 j;
    FieldObjSlot *ctx;
    FieldAnimSfx *sfx;
    s32 pitch;
    s32 ch;
    s32 snd;
    s32 rnd;

    base = g_gameState->gameData->ctx.field->area->scene->animSfx;
    sfxCount = g_gameState->gameData->ctx.field->area->scene->animSfxCount;
    for (i = count - 1; i >= 0; i--) {
        ctx = slots[i];
        if (ctx == NULL || (ctx->flags & FLD_OBJ_NO_ANIM_SFX)) {
            continue;
        }
        for (j = sfxCount - 1; j >= 0; j--) {
            sfx = &base[j];
            if (sfx->modelSlot == ctx->actor->modelSlot && sfx->anim == ctx->actor->anim && sfx->frame == ctx->actor->animFrame) {
                pitch = 0;
                tri = 0;
                triAlt = 0;
                triFlags = 0;
                vol = 0x80;
                pan = 0x7F;
                bgi_getActorTriangle(ctx->actor->modelSlot, (u16 *)&tri, (u16 *)&triAlt);
                if (tri != -1) {
                    bgi_getTriangleFlags(tri, &triFlags);
                    triFlags = (s16)triFlags >> 8;
                }
                ch = sfx->modelSlot & 3;
                ch = SND_SE_GROUP_ACTOR(ch) & SND_SE_GROUP_ACTORS;
                snd = sfx->sound[(triFlags >> 4) & 1] | DB_SE;
                if (sfx->randomPitch != 0) {
                    rnd = rand8_inline();
                    pitch = (rnd >> 4) - 8;
                }
                fld_getSoundPanVolume(ctx->actor->pos.vx, ctx->actor->pos.vy, ctx->actor->pos.vz, &vol, &pan);
                fld_soundCommand(SND_SE_PLAY, snd, ch, vol, pan);
                if (pitch != 0) {
                    fld_soundCommand(SND_SE_PITCH, snd, ch, pitch, 0);
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
    FieldAnimSfx *sfx = &base[sc->animSfxCount] - 1;

    do {
        if (sfx->modelSlot == obj->actor->modelSlot && sfx->anim == anim && sfx->frame == frame) {
            return sfx;
        }
        sfx--;
    } while (sfx >= base);
    return NULL;
}

FieldAnimSfx *fld_addAnimSfx(FieldObjSlot *obj, s32 anim, s32 frame) {
    FieldScene *sc = g_gameState->gameData->ctx.field->area->scene;
    u8 i = sc->animSfxCount;
    FieldAnimSfx *sfx;
    u16 id;
    s32 newCount;

    newCount = i + 1;
    sc->animSfxCount = newCount;
    sfx = &sc->animSfx[i];
    sfx->modelSlot = obj->actor->modelSlot;
    id = obj->actor->modelId;
    sfx->anim = anim;
    do {
        sfx->frame = frame;
    } while (0);
    sfx->modelId = id;
    return sfx;
}

s32 fld_removeAnimSfx(obj, anim, frame)
    FieldObjSlot *obj;
    u16 anim;
    s16 frame;
{
    FieldAnimSfx *base;
    FieldAnimSfx *last;
    FieldAnimSfx *sfx;

    base = g_gameState->gameData->ctx.field->area->scene->animSfx;
    sfx = &base[g_gameState->gameData->ctx.field->area->scene->animSfxCount] - 1;
    last = sfx;
    do {
        if (sfx->modelSlot == obj->actor->modelSlot && sfx->anim == anim && sfx->frame == frame) {
            if (sfx != last) {
                *sfx = *last;
            }
            g_gameState->gameData->ctx.field->area->scene->animSfxCount--;
        }
        sfx--;
    } while (sfx >= base);
    return -1;
}

s32 fld_depGetLink(s32 fileId) {
    FileTable *table = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *ents = table->entries;

    for (i = table->count - 1; i >= 0; i--) {
        if (ents[i].id == fileId) {
            return ents[i].link;
        }
    }
    return -1;
}

void fld_findBattleMusic(void) {
    FieldScene *scene;
    FileEntry *base;
    s32 i;
    s32 j;
    s32 id;
    s32 key;
    s32 bankId;
    s32 w;

    scene = g_gameState->gameData->ctx.field->area->scene;
    scene->battleMusic = g_gameState->gameData->musicId[0];
    key = (u16)scene->nextId;
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
                bankId = base[j].id;
                if (((base[j].id >> 16) & 0xFF) == (DB_SOUND_BANK >> 16) && w == id) {
                    g_gameState->gameData->ctx.field->area->scene->battleMusicBank = bankId;
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
    s32 bankCount;
    s32 i;
    s32 j;
    s32 cnt;
    s32 found;
    s32 foundSlot;
    s32 k;
    s32 done;
    s32 cached;
    s32 cachedSlot;
    s32 owner;
    s32 cacheIdx;
    s32 min;
    s32 best;
    u32 x;
    u32 y;
    u32 z;
    FileEntry *ents;

    bankCount = 0;
    ents = g_gameState->gameData->depTable->entries;
    for (i = 4; i >= 0; i--) {
        id[i] = -1;
        sz[i] = -1;
        used[i] = 0;
        slot[i] = -1;
    }
    for (i = g_gameState->gameData->depTable->count - 1; i >= 0; i--) {
        x = ents[i].id;
        if (((x >> 16) & 0xFF) == (DB_SOUND_BANK >> 16)) {
            found = 0;
            if (((ents[i].link >> 16) & 0xFF) == (DB_SE >> 16)) {
                foundSlot = -1;
                for (j = 4; j >= 0; j--) {
                    if (g_fieldSoundBankIds[j] == x) {
                        found = 1;
                        foundSlot = g_fieldSoundBankSlots[j];
                        break;
                    }
                }
                id[bankCount] = x;
                sz[bankCount] = ents[i].sector;
                used[bankCount] = found;
                slot[bankCount] = foundSlot;
                bankCount++;
            }
        }
    }
    for (i = bankCount - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4 && slot[i] == -1) {
            k = 0;
            do {
                done = 1;
                for (j = bankCount - 1; j >= 0; j--) {
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
    for (i = bankCount - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4) {
            cnt++;
        }
    }
    if (cnt < 2) {
        for (cacheIdx = 4; cacheIdx >= 0; cacheIdx--) {
            y = g_fieldSoundBankIds[cacheIdx];
            if (y != -1 && (y >> 24) == 4) {
                cached = -1;
                for (j = bankCount - 1; j >= 0; j--) {
                    if (id[j] == y) {
                        cached = cacheIdx;
                        break;
                    }
                }
                if (cached == -1) {
                    owner = -1;
                    j = bankCount - 1;
                    cachedSlot = g_fieldSoundBankSlots[cacheIdx];
                    for (; j >= 0; j--) {
                        if (slot[j] == cachedSlot) {
                            owner = j;
                            break;
                        }
                    }
                    if (owner == -1) {
                        id[bankCount] = y;
                        sz[bankCount] = 0;
                        used[bankCount] = 1;
                        slot[bankCount] = cachedSlot;
                        bankCount++;
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
        for (i = bankCount - 1; i >= 0; i--) {
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
    FileEntry *entry = dep_find(g_gameState->gameData->depTable, fileId);

    if (outSize != NULL) {
        *outSize = entry->sectors << 11;
    }
    return entry->sector;
}

s32 fld_soundBankReadCallback(CdReadCtx *ctx) {
    s32 addr;
    s32 id;
    s32 pack;
    s32 len;
    s32 base;
    s32 ret;
    GameData *gd;
    FieldScene *sc;

    ret = 0;
    addr = (s32)ctx->dest;
    id = g_gameState->gameData->soundLoadBank | DB_SOUND_BANK;
    if (ctx->offset == 0) {
        base = addr;
        pack = db_lookup(addr, DB_GET_AT, DB_PACK, 0);
        if (pack == -1) {
            addr = db_lookup(addr, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_getSize(base);
        } else {
            addr = db_lookup(pack, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_lookup(base, DB_GET_AT, (DB_PACK | 1), 0) - g_gameState->gameData->soundLoadDest;
            fld_dropSoundBankCache(g_gameState->gameData->soundBankSize - ctx->offset);
        }
    }
    gd = g_gameState->gameData;
    len = gd->soundBankSize - ctx->offset;
    if (ctx->chunk < len) {
        len = ctx->chunk;
    }
    len = (s32)ctx->dest + len - addr;
    if ((id & 0xFFFF) != SND_BANK_COMMON) {
        sc = gd->ctx.field->area->scene;
        if (sc->flags & FLD_SCN_BANK_SLOT) {
            ret = akao_loadBankInSlot(addr, sc->soundBankSlot, 0);
        } else {
            ret = akao_uploadBank(addr, len, 0);
        }
    }
    ctx->dest = (u8 *)addr;
    return ret;
}

s32 fld_unpackMusicSeq(s32 db) {
    s32 key = db_lookup(db, DB_GET_KEY_AT, DB_MUSIC, 0);

    memcpy((u8 *)db, (u8 *)db_lookup(db, DB_GET_ADDR, key, 0), db_lookup(db, DB_GET_SIZE, key, 0));
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
    FieldObjSlot *obj;
    s32 chg;
    FieldActorInfo *info;
    FieldModelLight *mdlLight;
    u32 shade;
    FieldLight *fl;
    s16 floor;
    u32 baseCol;
    u32 lightCol;
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
        obj = slots[i];
        chg = 0;
        if (obj == NULL) {
            continue;
        }
        info = &infos[obj->actor->modelSlot];
        mdlLight = (FieldModelLight *)fld_findLightColor(lt, obj->modelId);
        shade = obj->flags & FLD_OBJ_DIR_LIGHT;
        fl = fld_findFloorLight(lt, obj, &floor);
        if ((u8)info->litFloor != (u8)floor || shade != (info->flags & FLD_INFO_DIR_LIGHT) || !(obj->flags & FLD_OBJ_LIT)) {
            chg = 1;
        }
        if (chg) {
            if (!(obj->flags & FLD_OBJ_COLOR_SET)) {
                lightCol = 0;
                if (mdlLight != NULL) {
                    baseCol = mdlLight->color;
                } else {
                    baseCol = def->color;
                }
                if (shade && dir != NULL) {
                    lightCol = dir->color;
                } else if (fl != NULL) {
                    lightCol = fl->color;
                } else if (amb != NULL) {
                    lightCol = amb->color;
                }
                col = ((((baseCol & 0xFF) + (lightCol & 0xFF)) * 8) & 0xFF) | ((((baseCol & 0xFF00) + (lightCol & 0xFF00)) * 8) & 0xFF00)
                    | ((((baseCol & 0xFF0000) + (lightCol & 0xFF0000)) * 8) & 0xFF0000) | FLD_COLOR_PENDING;
                info->nextColor.w = col;
                info->color.w = col;
                obj->flags |= FLD_OBJ_LIT;
            }
            if (mdlLight != NULL) {
                dx = mdlLight->shadowShade;
                dy = mdlLight->shadowSize;
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
            g_gameState->gameData->ctx.field->area->scene->shadows[obj->actor->modelSlot].params.shade = dx;
            g_gameState->gameData->ctx.field->area->scene->shadows[obj->actor->modelSlot].params.scaleX = dy * 14;
            g_gameState->gameData->ctx.field->area->scene->shadows[obj->actor->modelSlot].params.scaleZ = dy * 12;
        }
        info->litFloor = floor;
        info->flags = (info->flags & ~FLD_INFO_DIR_LIGHT) | shade;
    }
    for (i = count - 1; i >= 0; i--) {
        obj = slots[i];
        if (obj == NULL) {
            continue;
        }
        inf2 = &infos[obj->actor->modelSlot];
        k = inf2->color.w;
        if (k >> 24 != 0) {
            for (j = ((ModelObj *)obj->model)->model->groupCount - 1; j >= 0; j--) {
                fldmdl_setGroupRgb(obj->model, j & 0xFF, (s16)(k & 0xFF), (s16)((k >> 8) & 0xFF), ((u32)k >> 16) & 0xFF);
            }
            inf2->color.w = inf2->nextColor.w;
            inf2->nextColor.w = 0;
        }
    }
}

/* set the object's light colour */
s32 fld_setSlotColor(FieldObjSlot *obj, s32 r, s32 g, s32 b) {
    u32 objFlags = obj->flags;
    FieldActorInfo *info = &g_gameState->gameData->ctx.field->area->scene->actorInfo[obj->actor->modelSlot];
    u32 col;

    if (!(objFlags & FLD_OBJ_COLOR_WAIT)) {
        col = (r & 0xFF) | ((g << 8) & 0xFF00) | ((b << 16) & 0xFF0000) | FLD_COLOR_PENDING;
        info->nextColor.w = col;
        info->color.w = col;
        obj->flags |= FLD_OBJ_COLOR_SET | FLD_OBJ_COLOR_WAIT;
        return 1;
    }
    obj->flags = objFlags & ~FLD_OBJ_COLOR_WAIT;
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
    FieldModelLight *entry;
    s32 id;
    u32 key;

    key = modelId;
    if (set->colorCount == 0) {
        return NULL;
    }
    id = key & 0xFFFF;
    base = (FieldModelLight *)&set->lights[set->colorOffset];
    entry = &base[set->colorCount] - 1;
    do {
        if (entry->modelId == id) {
            return entry;
        }
        entry--;
    } while (entry >= base);
    return NULL;
}

FieldLight *fld_findGlobalLights(FieldLightSet *set, FieldLight **outDefault, FieldLight **outDir) {
    FieldLight *base;
    FieldLight *light;

    *outDefault = NULL;
    *outDir = NULL;
    base = set->lights;
    if (set->lightCount == 0) {
        return NULL;
    }
    light = &set->lights[set->lightCount - 1];
    do {
        if (light->type == FLD_LIGHT_DEFAULT) {
            *outDefault = light;
        } else if (light->type == FLD_LIGHT_DIR) {
            *outDir = light;
        }
        light--;
    } while (light >= base);
    return base;
}

/* find the floor light covering the object's current floor */
FieldLight *fld_findFloorLight(FieldLightSet *set, FieldObjSlot *obj, s16 *outFloor) {
    u16 tri;
    FieldLight *light;
    s32 floorIdx;
    s32 floorIds;

    bgi_getActorTriangle(obj->actor->modelSlot, &tri, (u16 *)outFloor);
    if (set->lightCount != 0) {
        if ((u8)*outFloor == 0xFF) {
            return NULL;
        }
        light = &set->lights[set->lightCount - 1];
        do {
            if (light->type == FLD_LIGHT_FLOOR) {
                floorIds = light->floors;
                for (floorIdx = 3; floorIdx >= 0; floorIdx--) {
                    if ((floorIds & 0xFF) == *outFloor) {
                        return light;
                    }
                    floorIds >>= 8;
                }
            }
            light--;
        } while (light >= set->lights);
    }
    return NULL;
}

/* kind/variant: the body only runs for (0, 0), rain, and every caller passes 0, 0; other values look like weather
 * kinds that were never implemented (the JP build is the same). */
s32 fld_enableRain(s32 kind, s32 variant, s32 enable) {
    s32 start;
    s32 size;
    s32 end;

    if (kind == 0 && variant == 0) {
        if (enable != 0) {
            if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_RAIN_ALLOC)) {
                start = g_gameState->gameData->ctx.field->area->scene->heapTop;
                if (start == 0) {
                    start = g_modeHeap;
                }
                if (preload_overlaps(start, 0x5000) != 0) {
                    /* preload_waitCurrent returns NULL when no preload is in flight (seen in d4_f2928), so the PS1 reads fileId from low
                     * RAM here; a port guards it via PSX_NULLABLE (ff9-ds F37). */
                    preload_remove(PSX_NULLABLE(preload_waitCurrent())->fileId);
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
    PreloadEntry *entry;
    PreloadEntry *current;
    PreloadEntry *found;
    FileEntry *file;

    type = typeArg;
    flag = 0;
    if ((s8)type < 0) {
        type -= FLD_PRELOAD_EXCLUDE;
        flag = 1;
    }
    switch (type & 0xFF) {
    default:
        return -1;
    case FLD_PRELOAD_BATTLE:
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
        current = preload_getCurrent();
        if (current != NULL && current->fileId != k) {
            preload_wait(current);
        }
        if (flag || preload_find(k) != NULL) {
            goto set;
        }
        goto clear;
    case FLD_PRELOAD_FIELD:
        id = (u16)fileId | DB_FIELD;
        if ((u16)fileId >= FIELD_ID_WORLD_FIRST) {
            return -1;
        }
        file = dep_find(g_gameState->gameData->depTable, id);
        if (file == NULL) {
            return -1;
        }
        if (file->sectors == 0) {
            return -1;
        }
        if (flag && fld_findPreloadExclude(id)) {
            return -1;
        }
        if (fld_findPreloadRequest(id)) {
            return -1;
        }
        entry = preload_find(id);
        if (entry != NULL && entry->kind == PRELOAD_KIND_READ) {
            if (preload_isBusy(entry)) {
                return -1;
            }
            if (!preload_isBusy(entry) && (entry->readSize >> 2) < entry->loaded) {
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
        entry = preload_getCurrent();
        if (entry != NULL && entry->fileId != id && preload_isBusy(entry)) {
            preload_wait(entry);
        }
        found = preload_find(id);
        if (!flag && (found == NULL || found->kind != PRELOAD_KIND_READ)) {
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
    s32 *slot = fld_findPreloadRequest(fileId);

    if (slot != NULL) {
        return slot;
    }
    if (g_fieldPreloadRequestCount < 8) {
        slot = &g_fieldPreloadRequests[g_fieldPreloadRequestCount];
        g_fieldPreloadRequestCount++;
        *slot = fileId;
        return slot;
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
    s32 *slot = fld_findPreloadExclude(fileId);

    if (slot != NULL) {
        return slot;
    }
    if (g_fieldPreloadExcludeCount < 8) {
        slot = &g_fieldPreloadExcludes[g_fieldPreloadExcludeCount];
        g_fieldPreloadExcludeCount++;
        *slot = fileId;
        return slot;
    }
    return NULL;
}

/* start streaming a file into the free area */
s32 fld_preloadFile(s32 fileId, s32 kind, s32 sector, s32 chunkSize) {
    u32 top;
    s32 size;
    PreloadEntry *entry;

    if (fileId == -1 || sector == 0) {
        return 0;
    }
    if (kind == 0 && chunkSize == 0) {
        return 0;
    }
    top = fld_getModelCacheBottom(NULL);
    if ((u32)(g_loadAreaEnd + FLD_WORK_SWIRL) < top) {
        top = (u32)(g_loadAreaEnd + FLD_WORK_SWIRL);
    }
    size = top - g_fieldPreloadCursor;
    size = size / 2048 * 2048;
    if (kind == 0 && size < 0x8000) {
        return 0;
    }
    entry = preload_add();
    if (entry == NULL) {
        return 0;
    }
    preload_setEntry(entry, kind, fileId, kind == 0 ? g_fieldPreloadCursor : 0, size, chunkSize, sector, chunkSize);
    g_fieldPreloadCursor += chunkSize;
    return (s32)entry;
}

static inline s32 fld_queueFieldPreloads_grp(s32 id) {
    return id / 50;
}

void fld_queueFieldPreloads(void) {
    FileEntry *tbl;
    FileEntry *fieldEntry;
    FileEntry *areaEntry;
    FileEntry *ovlFile;
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
            ovlFile = file_find(DIR_OVERLAYS, OVL_KEY(OVL_BATTLE));
            fld_preloadFile(OVL_KEY(OVL_BATTLE), 0, ovlFile->sector, (ovlFile->link - ovlFile->sector) << 11);
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
                fieldEntry = dep_find(g_gameState->gameData->depTable, key);
                if (fieldEntry != NULL) {
                    if ((fieldEntry->link & 0xFFFF) != (s16)g_gameState->gameData->fieldAreaId) {
                        areaEntry = dep_find(g_gameState->gameData->depTable, fieldEntry->link);
                        if (areaEntry != NULL) {
                            fld_preloadFile(fieldEntry->link & DB_KEY_MASK, 0, areaEntry->sector, areaEntry->sectors << 11);
                        }
                    }
                    fld_preloadFile(key, 0, fieldEntry->sector, fieldEntry->sectors << 11);
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
                if ((x & 0xFFFF) < FIELD_ID_WORLD_FIRST && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
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
                    if ((x & 0xFFFF) < FIELD_ID_WORLD_FIRST && fld_findPreloadExclude(x) == NULL && (x & 0xFFFF) == g_gameState->gameData->ctx.field->prevFieldId) {
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
                    if ((x & 0xFFFF) < FIELD_ID_WORLD_FIRST && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
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
                        fld_preloadFile(tbl[i].id & DB_KEY_MASK, 0, tbl[i].sector, tbl[i].sectors << 11);
                        break;
                    }
                }
            }
            cnt = tbl[sel].id & DB_KEY_MASK;
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
                    if (key < FIELD_ID_WORLD_FIRST && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
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
                        if ((x & 0xFFFF) < FIELD_ID_WORLD_FIRST && (x & 0xFFFF) != (cur & 0xFFFF) && fld_findPreloadExclude(x) == NULL &&
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
                        if ((x & 0xFFFF) < FIELD_ID_WORLD_FIRST && (x & 0xFFFF) != g_gameState->gameData->ctx.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
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
                            fld_preloadFile(tbl[i].id & DB_KEY_MASK, 1, tbl[i].sector, -1);
                            return;
                        }
                    }
                } else {
                    fld_preloadFile(tbl[sel].id & DB_KEY_MASK, 1, tbl[sel].sector, -1);
                }
            }
        }
    }
}

s32 fld_preloadHook(void) {
    FieldScene *scene;

    if ((u32)(g_gameState->gameData->soundLoadState - 1) < 5) {
        return 1;
    }
    scene = g_gameState->gameData->ctx.field->area->scene;
    if (scene->flags & FLD_SCN_PRELOAD_RESTART) {
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
    if (scene->flags & FLD_SCN_PRELOADS_QUEUED) {
        return 0;
    }
    if (scene->flags & FLD_SCN_ACTORS_READY) {
        fld_setPreloadCursor(scene->heapTop);
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
    u8 *pkt;
    s32 tim;
    s32 i;
    s32 duration;
    u32 state;

    tim = (s32)db_lookupRegistered(DB_GET_ADDR, FLD_SPLASH_TIM, 0);
    file_getTimRects((void *)tim, &clut, &img);
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    file_loadTim(tim);
    DrawSync(0);
    duration = 0;
    state = 0;
    i = 1;
    pkt = g_gameState->gameData->ctx.field->area->scene->heapTop;
    do {
        {
            /* dead struct initializer: it emits only a CLOBBER of dead, which sched2 places in one of
               the loop's load-delay gaps (so p += 20, the tpage constant and &tp[0] land one gap
               later, as in the original), and dead's spill slot is the frame's otherwise unused word */
            struct { s32 x; } dead = { i };
        }
        spr[i] = (SPRT *)pkt;
        pkt += sizeof(SPRT);
        setSprt(spr[i]);
        setRGB0(spr[i], 0x80, 0x80, 0x80);
        spr[i]->x0 = (SCREEN_W - img->w * 4) >> 1;
        spr[i]->y0 = (SCREEN_H - img->h) / 2;
        spr[i]->u0 = 0;
        spr[i]->v0 = 0;
        spr[i]->w = img->w * 4 - 1;
        spr[i]->h = img->h;
        spr[i]->clut = getClut(clut->x, clut->y);
        tp[i] = (DR_TPAGE *)pkt;
        pkt += sizeof(DR_TPAGE);
        setDrawTPage(tp[i], 1, 1, getTPage(0, 0, img->x, img->y));
        i--;
    } while (i >= 0);
    fld_invalidateModelCache((s32)pkt);
    preload_reset();
    game_setClearColor(0, 0, 0);
    do {
        ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
        switch (state) {
        case 0:
            duration = 0x3C;
            i = 0;
            game_startFade(FADE_FLAT, FADE_SUB, 0, FADE_COLOR_FULL);
            game_startFade(FADE_FLAT, FADE_SUB, duration, i);
            state = 1;
            break;
        case 1:
            if (i < duration) {
                i++;
            } else {
                duration = 0x78;
                i = 0;
                state = 2;
            }
            break;
        case 2:
            if (i < duration) {
                i++;
            } else {
                duration = 0x3C;
                i = 0;
                game_startFade(FADE_FLAT, FADE_SUB, duration, FADE_COLOR_FULL);
                state = 3;
            }
            break;
        case 3:
            if (i < duration) {
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

    if (sc->exitReason == FLD_EXIT_FIELD && (s16)gd->fieldId == FLD_SPLASH_FROM_FIELD && sc->nextId == FLD_SPLASH_TO_FIELD) {
        return 1;
    }
    return 0;
}

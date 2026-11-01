#include "ff9.h"

s32 snd_init(void) {
    akao_init();
    return 0;
}

s32 snd_quit(void) {
    akao_quit();
    return 0;
}

s32 akao_setSeTables(u8 *data) {
    s32 err;
    u8 *tbl;

    err = akao_checkHeader((s32 *)data);
    if (err == 0) {
        tbl = data + 0x10;
        g_akaoSeTrackOfs = (u16 *)tbl;
        tbl = data + 0x210;
        g_akaoSeBankIds = (u16 *)tbl;
        tbl = data + 0x310;
        g_akaoSeExtraCounts = (u16 *)tbl;
        tbl = data + 0x410;
        g_akaoSeData = tbl;
    }
    return err;
}

s32 akao_playMusic(void *seq) {
    g_akaoArgs.arg0 = (s32)seq;
    return akao_command(AKAO_CMD_PLAY_MUSIC);
}

void akao_stopMusic(s32 musicId) {
    g_akaoArgs.arg0 = musicId;
    akao_command(AKAO_CMD_STOP_MUSIC);
}

void akao_playMusicFrom(s32 seq, s32 chMask) {
    g_akaoArgs.arg0 = seq;
    g_akaoArgs.arg1 = chMask;
    akao_command(AKAO_CMD_PLAY_MUSIC_FROM);
}

void akao_saveMusic(void) {
    akao_command(AKAO_CMD_SAVE_MUSIC);
}

s32 akao_clearSavedMusic(void) {
    if (g_akaoSubSeq == 0) {
        g_akaoSavedSeqMusicId = 0;
    }
    return 0;
}

void akao_pushMusic(s32 seq, s32 volume) {
    g_akaoArgs.arg0 = seq;
    g_akaoArgs.arg1 = volume & 0x7F;
    akao_command(AKAO_CMD_PUSH_MUSIC);
}

void akao_crossfadeMusic(s32 seq, s32 frames, s32 volume) {
    g_akaoArgs.arg0 = seq;
    g_akaoArgs.arg1 = frames;
    g_akaoArgs.arg2 = volume & 0x7F;
    akao_command(AKAO_CMD_CROSSFADE_MUSIC);
}

s32 akao_playMusicCount(void *seq, s32 count) {
    g_akaoArgs.arg0 = (s32)seq;
    g_akaoArgs.arg1 = count;
    return akao_command(AKAO_CMD_PLAY_MUSIC_COUNT);
}

s32 akao_playSe(s32 handle, s32 chMask, s32 pan, s32 volume) {
    s32 err;

    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = pan & 0xFF;
    g_akaoArgs.arg3 = volume & 0x7F;
    if ((u32)handle >= 0x400) {
        err = akao_checkHeader((s32 *)handle);
        if (err != 0) {
            return err;
        }
        akao_command(AKAO_CMD_PLAY_SE_BANK);
        return handle;
    }
    return akao_command(AKAO_CMD_PLAY_SE);
}

void akao_stopSe(s32 handle, s32 chMask) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    akao_command(AKAO_CMD_STOP_SE);
}

void akao_playSeDefault(s32 seId) {
    g_akaoArgs.arg0 = seId & 0x3FF;
    akao_command(AKAO_CMD_PLAY_SE_DEFAULT);
}

u32 akao_getActiveSe(void) {
    u32 active = g_akaoSeState.activeMask;
    AkaoChannel *chan;
    u32 activeSe;
    u32 bit;

    if (active == 0) {
        return 0;
    }
    chan = g_akaoSeChannels;
    activeSe = 0;
    for (bit = AKAO_SE_CH_BIT(0); bit & 0xFFFFFF; bit <<= 1) {
        if (active & bit) {
            activeSe |= chan->seMask;
        }
        chan++;
    }
    activeSe &= 0xFFFFFF;
    return activeSe;
}

s32 akao_isSePlaying(u32 id) {
    u32 active;
    u32 bit;
    AkaoChannel *chan;
    if (id == 0) {
        return 0;
    }
    active = g_akaoSeState.activeMask;
    if (active == 0) {
        return 0;
    }
    chan = g_akaoSeChannels;
    for (bit = AKAO_SE_CH_BIT(0); bit & 0xFFFFFF; bit <<= 1) {
        if ((active & bit) && id == chan->seMask) {
            return 1;
        }
        chan++;
    }
    return 0;
}

void akao_setOutputMode(s32 mode) {
    if (mode == 1) {
        mode = AKAO_CMD_SET_OUTPUT_MODE2;
    } else {
        mode = AKAO_CMD_SET_OUTPUT_MODE1;
    }
    akao_command(mode);
}

void akao_setMuteMask(s32 muteMask) {
    g_akaoArgs.arg0 = muteMask;
    akao_command(AKAO_CMD_SET_MUTE_MASK);
}

void akao_setCondition(s32 condition) {
    g_akaoArgs.arg0 = condition;
    akao_command(AKAO_CMD_SET_CONDITION);
}

void akao_pause(u32 kind) {
    s32 cmd;
    switch (kind) {
    case 1:
        cmd = AKAO_CMD_PAUSE_MUSIC;
        break;
    case 2:
        cmd = AKAO_CMD_PAUSE_SE;
        break;
    case 3:
        cmd = AKAO_CMD_PAUSE_STREAM;
        break;
    default:
        cmd = AKAO_CMD_PAUSE_ALL;
        break;
    }
    akao_command(cmd);
}

void akao_resume(u32 kind) {
    s32 cmd;
    switch (kind) {
    case 1:
        cmd = AKAO_CMD_RESUME_MUSIC;
        break;
    case 2:
        cmd = AKAO_CMD_RESUME_SE;
        break;
    case 3:
        cmd = AKAO_CMD_RESUME_STREAM;
        break;
    default:
        cmd = AKAO_CMD_RESUME_ALL;
        break;
    }
    akao_command(cmd);
}

void akao_setAllSeVolume(s32 volume) {
    g_akaoArgs.arg0 = volume & 0x7F;
    akao_command(AKAO_CMD_SET_ALL_SE_VOLUME);
}

void akao_fadeAllSeVolume(s32 frames, s32 volume) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = volume & 0x7F;
    akao_command(AKAO_CMD_FADE_ALL_SE_VOLUME);
}

void akao_setSeVolume(s32 handle, s32 chMask, s32 volume) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = volume & 0x7F;
    akao_command(AKAO_CMD_SET_SE_VOLUME);
}

void akao_fadeSeVolume(s32 handle, s32 chMask, s32 frames, s32 volume) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = frames;
    g_akaoArgs.arg3 = volume & 0x7F;
    akao_command(AKAO_CMD_FADE_SE_VOLUME);
}

void akao_sendCmdAA(s32 pan) {
    g_akaoArgs.arg0 = pan & 0xFF;
    akao_command(AKAO_CMD_SET_ALL_SE_PAN);
}

void akao_sendCmdAB(s32 frames, s32 pan) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = pan & 0xFF;
    akao_command(AKAO_CMD_FADE_ALL_SE_PAN);
}

void akao_sendCmdA2(s32 handle, s32 chMask, u8 pan) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = pan;
    akao_command(AKAO_CMD_SET_SE_PAN);
}

void akao_sendCmdA3(s32 handle, s32 chMask, s32 frames, u8 pan) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = frames;
    g_akaoArgs.arg3 = pan;
    akao_command(AKAO_CMD_FADE_SE_PAN);
}

void akao_sendCmdAC(s32 pitch) {
    g_akaoArgs.arg0 = pitch & 0xFF;
    akao_command(AKAO_CMD_SET_ALL_SE_PITCH);
}

void akao_sendCmdAD(s32 frames, s32 pitch) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = pitch & 0xFF;
    akao_command(AKAO_CMD_FADE_ALL_SE_PITCH);
}

void akao_sendCmdA4(s32 handle, s32 chMask, u8 pitch) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = pitch;
    akao_command(AKAO_CMD_SET_SE_PITCH);
}

void akao_sendCmdA5(s32 handle, s32 chMask, s32 frames, u8 pitch) {
    g_akaoArgs.arg0 = handle;
    g_akaoArgs.arg1 = chMask & 0xFFFFFF;
    g_akaoArgs.arg2 = frames;
    g_akaoArgs.arg3 = pitch;
    akao_command(AKAO_CMD_FADE_SE_PITCH);
}

void akao_setMusicVolume(s32 musicId, s32 volume) {
    g_akaoArgs.arg0 = musicId;
    g_akaoArgs.arg1 = volume & 0x7F;
    akao_command(AKAO_CMD_SET_MUSIC_VOLUME);
}

void akao_fadeMusicVolume(s32 musicId, s32 frames, s32 volume) {
    g_akaoArgs.arg0 = musicId;
    g_akaoArgs.arg1 = frames;
    g_akaoArgs.arg2 = volume & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_VOLUME);
}

void akao_fadeMusicVolumeFrom(s32 musicId, s32 frames, s32 from, s32 to) {
    g_akaoArgs.arg0 = musicId;
    g_akaoArgs.arg1 = frames;
    g_akaoArgs.arg2 = from & 0x7F;
    g_akaoArgs.arg3 = to & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_VOLUME_FROM);
}

void akao_sendCmdC4(s32 musicId, s32 pan) {
    g_akaoArgs.arg0 = musicId;
    g_akaoArgs.arg1 = pan & 0x7F;
    akao_command(AKAO_CMD_SET_MUSIC_PAN);
}

void akao_sendCmdC5(s32 musicId, s32 frames, s32 pan) {
    g_akaoArgs.arg0 = musicId;
    g_akaoArgs.arg1 = frames;
    g_akaoArgs.arg2 = pan & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_PAN);
}

void akao_setCdVolume(s32 volume) {
    g_akaoArgs.arg0 = volume;
    akao_command(AKAO_CMD_SET_CD_VOLUME);
}

void akao_fadeCdVolume(s32 frames, s32 volume) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = volume;
    akao_command(AKAO_CMD_FADE_CD_VOLUME);
}

void akao_fadeCdVolumeFrom(s32 frames, s32 from, s32 to) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = from;
    g_akaoArgs.arg2 = to;
    akao_command(AKAO_CMD_FADE_CD_VOLUME_FROM);
}

void akao_setTempoScale(s32 scale) {
    g_akaoArgs.arg0 = scale & 0xFF;
    akao_command(AKAO_CMD_SET_TEMPO_SCALE);
}

void akao_fadeTempoScale(s32 frames, s32 scale) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = scale & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_SCALE);
}

void akao_fadeTempoScaleFrom(s32 frames, s32 from, s32 to) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = from & 0xFF;
    g_akaoArgs.arg2 = to & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_SCALE_FROM);
}

void akao_setPitchScale(s32 scale) {
    g_akaoArgs.arg0 = scale & 0xFF;
    akao_command(AKAO_CMD_SET_PITCH_SCALE);
}

void akao_fadePitchScale(s32 frames, s32 scale) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = scale & 0xFF;
    akao_command(AKAO_CMD_FADE_PITCH_SCALE);
}

void akao_fadePitchScaleFrom(s32 frames, s32 from, s32 to) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = from & 0xFF;
    g_akaoArgs.arg2 = to & 0xFF;
    akao_command(AKAO_CMD_FADE_PITCH_SCALE_FROM);
}

void akao_setTempoPitchScale(s32 scale) {
    g_akaoArgs.arg0 = scale & 0xFF;
    akao_command(AKAO_CMD_SET_TEMPO_PITCH_SCALE);
}

void akao_fadeTempoPitchScale(s32 frames, s32 scale) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = scale & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_PITCH_SCALE);
}

void akao_fadeTempoPitchScaleFrom(s32 frames, s32 from, s32 to) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = from & 0xFF;
    g_akaoArgs.arg2 = to & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_PITCH_SCALE_FROM);
}

void akao_stopMusicNow(void) {
    akao_command(AKAO_CMD_STOP_MUSIC_NOW);
}

void akao_stopAllSe(void) {
    akao_command(AKAO_CMD_STOP_ALL_SE);
}

void akao_loadBankRetry(s32 bank, s32 wait) {
    while (akao_loadBank(bank, wait) == 1) {
    }
}

s32 akao_getSpuTransferState(void) {
    s32 reads[2];
    do {
        reads[0] = g_akaoSpuBusy;
        reads[1] = g_akaoSpuBusy;
    } while (reads[0] != reads[1]);
    return reads[0];
}

s32 akao_beginBankUpload(void) {
    g_akaoUpload.spuAddr = 0;
    g_akaoFlags.bankUpload |= 1;
    return 0;
}

s32 akao_uploadBank(s32 *data, u32 size, s32 wait) {
    u32 instrBytes;
    u32 sampleBytes;
    s32 slot;

    if (g_akaoFlags.bankUpload & 1) {
        if (g_akaoUpload.spuAddr == 0) {
            if (data[0] == AKAO_MAGIC) {
                akao_copy(data, (u32 *)&g_akaoBankHeader, sizeof(AkaoBankHeader));
                data += 16;
                size -= sizeof(AkaoBankHeader);
                g_akaoUpload.spuAddr = g_akaoBankHeader.spuAddr;
                g_akaoBankHeader.slot = slot = g_akaoBankHeader.slot;
                g_akaoUpload.sampleLeft = g_akaoBankHeader.sampleSize;
                g_akaoUpload.instrDest = &g_akaoInstruments[slot];
                g_akaoUpload.instrLeft = g_akaoBankHeader.instCount * 16;
            } else {
                size = 0;
                g_akaoUpload.sampleLeft = 0;
                g_akaoUpload.instrLeft = 0;
            }
        }
        if (g_akaoUpload.instrLeft != 0) {
            instrBytes = g_akaoUpload.instrLeft;
            if (size == 0) {
                goto end;
            }
            if (instrBytes >= size) {
                instrBytes = size;
            }
            akao_copy(data, g_akaoUpload.instrDest, instrBytes);
            data += instrBytes >> 2;
            size -= instrBytes;
            g_akaoUpload.instrDest = (AkaoInstrument *)((s32 *)g_akaoUpload.instrDest + (instrBytes >> 2));
            g_akaoUpload.instrLeft -= instrBytes;
            if (g_akaoUpload.instrLeft == 0) {
                akao_relocInstruments((s32 *)&g_akaoInstruments[g_akaoBankHeader.slot], &g_akaoInstruments[g_akaoBankHeader.slot], g_akaoBankHeader.spuAddr, g_akaoBankHeader.instCount);
            }
        }
        if (size != 0) {
            if (g_akaoUpload.sampleLeft == 0) {
                goto clear;
            }
            sampleBytes = g_akaoUpload.sampleLeft;
            if (sampleBytes >= size) {
                sampleBytes = size;
            }
            size = sampleBytes;
            SpuSetTransferStartAddr(g_akaoUpload.spuAddr);
            akao_spuWrite(data, size);
            g_akaoUpload.spuAddr += size;
            g_akaoUpload.sampleLeft -= size;
            if (wait != 0) {
                akao_waitSpuTransfer();
            }
        }
    end:
        if (g_akaoUpload.sampleLeft == 0) {
        clear:
            g_akaoFlags.bankUpload &= ~1;
        }
    }
    return g_akaoUpload.sampleLeft;
}

void akao_loadBankRetryAlias(s32 bank, s32 wait) {
    akao_loadBankRetry(bank, wait);
}

s32 akao_loadBankInSlot(s32 *data, s32 slot, s32 wait) {
    AkaoBankHeader *h = (AkaoBankHeader *)data;
    s32 y;
    s32 id;
    u32 i;

    for (i = 0; i < 7; i++) {
        if (g_akaoLoadedBanks[i] == h->id) {
            g_akaoLoadedBanks[i] = 0;
        }
    }
    i = h->slot + slot * 8;
    y = h->spuAddr;
    if (slot != 0) {
        id = h->id;
        if ((u32)(id - 500) < 100) {
            y += slot << 12;
        } else if ((u32)(id - 200) < 100) {
            y += (slot * 5) << 12;
        } else if ((u32)(id - 300) < 200) {
            y += slot << 14;
            slot += 4;
        }
    }
    g_akaoLoadedBanks[slot] = h->id;
    akao_loadBankAt(data, wait, i, y);
    return 0;
}

s32 akao_setCdMix(s32 vol) {
    u8 monoVol;

    if (g_akaoOutputFlags & AKAO_MODE_MONO) {
        CdlATV *atv = &g_akaoCdAtv;

        monoVol = (u32)(vol * 46448) >> 17;
        atv->val3 = monoVol;
        atv->val1 = monoVol;
        atv->val2 = monoVol;
        g_akaoCdAtv.val0 = monoVol;
    } else {
        g_akaoCdAtv.val2 = vol;
        g_akaoCdAtv.val0 = vol;
        g_akaoCdAtv.val3 = 0;
        g_akaoCdAtv.val1 = 0;
    }
    CdMix(&g_akaoCdAtv);
    return 0;
}

void akao_sendCmdE0(s32 *data, s32 volume, s32 flag) {
    if (akao_checkHeader(data) == 0) {
        g_akaoArgs.arg0 = (s32)data;
        g_akaoArgs.arg1 = (volume & 0xFF) << 8;
        g_akaoArgs.arg2 = flag;
        akao_command(AKAO_CMD_STREAM_START);
    }
}

void akao_sendCmdE2(void) {
    akao_command(AKAO_CMD_STREAM_STOP);
}

void akao_sendCmdE4(s32 volume) {
    g_akaoArgs.arg0 = (volume & 0x7F) << 8;
    akao_command(AKAO_CMD_SET_STREAM_VOLUME);
}

void akao_sendCmdE5(s32 frames, s32 volume) {
    g_akaoArgs.arg0 = frames;
    g_akaoArgs.arg1 = (volume & 0x7F) << 8;
    akao_command(AKAO_CMD_FADE_STREAM_VOLUME);
}

void akao_sendCmdE6(s32 pan) {
    g_akaoArgs.arg0 = (pan & 0xFF) << 8;
    akao_command(AKAO_CMD_SET_STREAM_PAN);
}

s32 akao_uploadSeBank(s32 *data, s32 upper) {
    s32 err = akao_checkHeader(data);
    s32 addr;
    s32 *h;

    if (err == 0) {
        akao_waitSpuTransfer();
        addr = 0x59100;
        if (upper == 0) {
            addr = 0x4F100;
        }
        h = data;
        data += 0x10;
        SpuSetTransferStartAddr(addr);
        akao_spuWrite(data, h[4]);
        h[8] = addr;
        akao_copy(h, g_akaoResidentStream, 0x70);
    } else {
        g_akaoResidentStream[8] = 0;
    }
    return err;
}

void akao_sendCmdED(s32 volume, s32 flag) {
    g_akaoArgs.arg0 = (volume & 0xFF) << 8;
    g_akaoArgs.arg1 = flag;
    akao_command(AKAO_CMD_STREAM_PLAY_RESIDENT);
}

void akao_sendCmdEC(s32 *data, s32 volume, s32 upper, s32 loop) {
    s32 x;
    if (akao_checkHeader(data) == 0) {
        x = 0x59100;
        if (upper == 0) {
            x = 0x4F100;
        }
        g_akaoArgs.arg0 = (s32)data;
        g_akaoArgs.arg1 = (volume & 0xFF) << 8;
        g_akaoArgs.arg2 = x;
        g_akaoArgs.arg3 = loop;
        akao_command(AKAO_CMD_STREAM_PLAY_ONE_SHOT);
    }
}

s32 akao_sendCmdE8(s32 buf, s32 size) {
    if (size == 0) {
        return -1;
    }
    g_akaoArgs.arg0 = buf;
    g_akaoArgs.arg1 = size;
    akao_command(AKAO_CMD_STREAM_SETUP_RING);
    return 0;
}

s32 akao_streamPushBlock(void *unused) { /* mbg_stream passes its stream state */
    g_akaoStream.blocksWritten++;
    g_akaoStream.writeBlock++;
    if (g_akaoStream.ringBlocks - 1 < g_akaoStream.writeBlock) {
        g_akaoStream.writeBlock = 0;
    }
    if ((g_akaoStream.flags & AKAO_STREAM_RING_PENDING) && g_akaoStream.writeBlock >= 2) {
        akao_streamStartRing();
    }
    return g_akaoStream.readBlock;
}

#include "ff9.h"

s32 snd_init(void) {
    akao_init();
    return 0;
}

s32 snd_quit(void) {
    akao_quit();
    return 0;
}

s32 akao_setSeTables(u8 *a) {
    s32 r;
    u8 *p;

    r = akao_checkHeader((s32 *)a);
    if (r == 0) {
        p = a + 0x10;
        g_akaoSeTrackOfs = (u16 *)p;
        p = a + 0x210;
        g_akaoSeBankIds = (u16 *)p;
        p = a + 0x310;
        g_akaoSeExtraCounts = (u16 *)p;
        p = a + 0x410;
        g_akaoSeData = p;
    }
    return r;
}

s32 akao_playMusic(void *a) {
    g_akaoArgs.arg0 = (s32)a;
    return akao_command(AKAO_CMD_PLAY_MUSIC);
}

void akao_stopMusic(s32 a) {
    g_akaoArgs.arg0 = a;
    akao_command(AKAO_CMD_STOP_MUSIC);
}

void akao_playMusicFrom(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
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

void akao_pushMusic(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0x7F;
    akao_command(AKAO_CMD_PUSH_MUSIC);
}

void akao_crossfadeMusic(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    g_akaoArgs.arg2 = c & 0x7F;
    akao_command(AKAO_CMD_CROSSFADE_MUSIC);
}

s32 akao_playMusicCount(void *a, s32 b) {
    g_akaoArgs.arg0 = (s32)a;
    g_akaoArgs.arg1 = b;
    return akao_command(AKAO_CMD_PLAY_MUSIC_COUNT);
}

s32 akao_playSe(s32 a, s32 b, s32 c, s32 d) {
    s32 r;

    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c & 0xFF;
    g_akaoArgs.arg3 = d & 0x7F;
    if ((u32)a >= 0x400) {
        r = akao_checkHeader((s32 *)a);
        if (r != 0) {
            return r;
        }
        akao_command(AKAO_CMD_PLAY_SE_BANK);
        return a;
    }
    return akao_command(AKAO_CMD_PLAY_SE);
}

void akao_stopSe(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    akao_command(AKAO_CMD_STOP_SE);
}

void akao_playSeDefault(s32 a) {
    g_akaoArgs.arg0 = a & 0x3FF;
    akao_command(AKAO_CMD_PLAY_SE_DEFAULT);
}

u32 akao_getActiveSe(void) {
    u32 mask = g_akaoSeState.activeMask;
    AkaoChannel *c;
    u32 r;
    u32 bit;

    if (mask == 0) {
        return 0;
    }
    c = g_akaoSeChannels;
    r = 0;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if (mask & bit) {
            r |= c->seMask;
        }
        c++;
    }
    r &= 0xFFFFFF;
    return r;
}

s32 akao_isSePlaying(u32 id) {
    u32 mask;
    u32 bit;
    AkaoChannel *c;
    if (id == 0) {
        return 0;
    }
    mask = g_akaoSeState.activeMask;
    if (mask == 0) {
        return 0;
    }
    c = g_akaoSeChannels;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if ((mask & bit) && id == c->seMask) {
            return 1;
        }
        c++;
    }
    return 0;
}

void akao_setOutputMode(s32 a) {
    if (a == 1) {
        a = AKAO_CMD_SET_OUTPUT_MODE2;
    } else {
        a = AKAO_CMD_SET_OUTPUT_MODE1;
    }
    akao_command(a);
}

void akao_setMuteMask(s32 a) {
    g_akaoArgs.arg0 = a;
    akao_command(AKAO_CMD_SET_MUTE_MASK);
}

void akao_setCondition(s32 a) {
    g_akaoArgs.arg0 = a;
    akao_command(AKAO_CMD_SET_CONDITION);
}

void akao_pause(u32 x) {
    s32 cmd;
    switch (x) {
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

void akao_resume(u32 x) {
    s32 cmd;
    switch (x) {
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

void akao_setAllSeVolume(s32 a) {
    g_akaoArgs.arg0 = a & 0x7F;
    akao_command(AKAO_CMD_SET_ALL_SE_VOLUME);
}

void akao_fadeAllSeVolume(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0x7F;
    akao_command(AKAO_CMD_FADE_ALL_SE_VOLUME);
}

void akao_setSeVolume(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c & 0x7F;
    akao_command(AKAO_CMD_SET_SE_VOLUME);
}

void akao_fadeSeVolume(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c;
    g_akaoArgs.arg3 = d & 0x7F;
    akao_command(AKAO_CMD_FADE_SE_VOLUME);
}

void akao_sendCmdAA(s32 a) {
    g_akaoArgs.arg0 = a & 0xFF;
    akao_command(AKAO_CMD_SET_ALL_SE_PAN);
}

void akao_sendCmdAB(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    akao_command(AKAO_CMD_FADE_ALL_SE_PAN);
}

void akao_sendCmdA2(s32 a, s32 b, u8 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c;
    akao_command(AKAO_CMD_SET_SE_PAN);
}

void akao_sendCmdA3(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c;
    g_akaoArgs.arg3 = d;
    akao_command(AKAO_CMD_FADE_SE_PAN);
}

void akao_sendCmdAC(s32 a) {
    g_akaoArgs.arg0 = a & 0xFF;
    akao_command(AKAO_CMD_SET_ALL_SE_PITCH);
}

void akao_sendCmdAD(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    akao_command(AKAO_CMD_FADE_ALL_SE_PITCH);
}

void akao_sendCmdA4(s32 a, s32 b, u8 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c;
    akao_command(AKAO_CMD_SET_SE_PITCH);
}

void akao_sendCmdA5(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFFFFFF;
    g_akaoArgs.arg2 = c;
    g_akaoArgs.arg3 = d;
    akao_command(AKAO_CMD_FADE_SE_PITCH);
}

void akao_setMusicVolume(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0x7F;
    akao_command(AKAO_CMD_SET_MUSIC_VOLUME);
}

void akao_fadeMusicVolume(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    g_akaoArgs.arg2 = c & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_VOLUME);
}

void akao_fadeMusicVolumeFrom(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    g_akaoArgs.arg2 = c & 0x7F;
    g_akaoArgs.arg3 = d & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_VOLUME_FROM);
}

void akao_sendCmdC4(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0x7F;
    akao_command(AKAO_CMD_SET_MUSIC_PAN);
}

void akao_sendCmdC5(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    g_akaoArgs.arg2 = c & 0x7F;
    akao_command(AKAO_CMD_FADE_MUSIC_PAN);
}

void akao_setCdVolume(s32 a) {
    g_akaoArgs.arg0 = a;
    akao_command(AKAO_CMD_SET_CD_VOLUME);
}

void akao_fadeCdVolume(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    akao_command(AKAO_CMD_FADE_CD_VOLUME);
}

void akao_fadeCdVolumeFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    g_akaoArgs.arg2 = c;
    akao_command(AKAO_CMD_FADE_CD_VOLUME_FROM);
}

void akao_setTempoScale(s32 a) {
    g_akaoArgs.arg0 = a & 0xFF;
    akao_command(AKAO_CMD_SET_TEMPO_SCALE);
}

void akao_fadeTempoScale(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_SCALE);
}

void akao_fadeTempoScaleFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    g_akaoArgs.arg2 = c & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_SCALE_FROM);
}

void akao_setPitchScale(s32 a) {
    g_akaoArgs.arg0 = a & 0xFF;
    akao_command(AKAO_CMD_SET_PITCH_SCALE);
}

void akao_fadePitchScale(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    akao_command(AKAO_CMD_FADE_PITCH_SCALE);
}

void akao_fadePitchScaleFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    g_akaoArgs.arg2 = c & 0xFF;
    akao_command(AKAO_CMD_FADE_PITCH_SCALE_FROM);
}

void akao_setTempoPitchScale(s32 a) {
    g_akaoArgs.arg0 = a & 0xFF;
    akao_command(AKAO_CMD_SET_TEMPO_PITCH_SCALE);
}

void akao_fadeTempoPitchScale(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_PITCH_SCALE);
}

void akao_fadeTempoPitchScaleFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b & 0xFF;
    g_akaoArgs.arg2 = c & 0xFF;
    akao_command(AKAO_CMD_FADE_TEMPO_PITCH_SCALE_FROM);
}

void akao_stopMusicNow(void) {
    akao_command(AKAO_CMD_STOP_MUSIC_NOW);
}

void akao_stopAllSe(void) {
    akao_command(AKAO_CMD_STOP_ALL_SE);
}

void akao_loadBankRetry(s32 a, s32 b) {
    while (akao_loadBank(a, b) == 1) {
    }
}

s32 akao_getSpuTransferState(void) {
    s32 a[2];
    do {
        a[0] = g_akaoSpuBusy;
        a[1] = g_akaoSpuBusy;
    } while (a[0] != a[1]);
    return a[0];
}

s32 akao_beginBankUpload(void) {
    g_akaoUpload.spuAddr = 0;
    g_akaoFlags.bankUpload |= 1;
    return 0;
}

s32 akao_uploadBank(s32 *buf, u32 size, s32 wait) {
    u32 n;
    u32 m;
    s32 slot;

    if (g_akaoFlags.bankUpload & 1) {
        if (g_akaoUpload.spuAddr == 0) {
            if (buf[0] == AKAO_MAGIC) {
                akao_copy(buf, (u32 *)&g_akaoBankHeader, sizeof(AkaoBankHeader));
                buf += 16;
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
            n = g_akaoUpload.instrLeft;
            if (size == 0) {
                goto end;
            }
            if (n >= size) {
                n = size;
            }
            akao_copy(buf, g_akaoUpload.instrDest, n);
            buf += n >> 2;
            size -= n;
            g_akaoUpload.instrDest = (AkaoInstrument *)((s32 *)g_akaoUpload.instrDest + (n >> 2));
            g_akaoUpload.instrLeft -= n;
            if (g_akaoUpload.instrLeft == 0) {
                akao_relocInstruments((s32 *)&g_akaoInstruments[g_akaoBankHeader.slot], &g_akaoInstruments[g_akaoBankHeader.slot], g_akaoBankHeader.spuAddr, g_akaoBankHeader.instCount);
            }
        }
        if (size != 0) {
            if (g_akaoUpload.sampleLeft == 0) {
                goto clear;
            }
            m = g_akaoUpload.sampleLeft;
            if (m >= size) {
                m = size;
            }
            size = m;
            SpuSetTransferStartAddr(g_akaoUpload.spuAddr);
            akao_spuWrite(buf, size);
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

void func_80056028(s32 a, s32 b) {
    akao_loadBankRetry(a, b);
}

s32 akao_loadBankInSlot(s32 *a, s32 b, s32 c) {
    AkaoBankHeader *h = (AkaoBankHeader *)a;
    s32 y;
    s32 id;
    u32 i;

    for (i = 0; i < 7; i++) {
        if (g_akaoLoadedBanks[i] == h->id) {
            g_akaoLoadedBanks[i] = 0;
        }
    }
    i = h->slot + b * 8;
    y = h->spuAddr;
    if (b != 0) {
        id = h->id;
        if ((u32)(id - 500) < 100) {
            y += b << 12;
        } else if ((u32)(id - 200) < 100) {
            y += (b * 5) << 12;
        } else if ((u32)(id - 300) < 200) {
            y += b << 14;
            b += 4;
        }
    }
    g_akaoLoadedBanks[b] = h->id;
    akao_loadBankAt(a, c, i, y);
    return 0;
}

s32 akao_setCdMix(s32 vol) {
    u8 v;

    if (g_akaoOutputFlags & 2) {
        CdlATV *atv = &g_akaoCdAtv;

        v = (u32)(vol * 46448) >> 17;
        atv->val3 = v;
        atv->val1 = v;
        atv->val2 = v;
        g_akaoCdAtv.val0 = v;
    } else {
        g_akaoCdAtv.val2 = vol;
        g_akaoCdAtv.val0 = vol;
        g_akaoCdAtv.val3 = 0;
        g_akaoCdAtv.val1 = 0;
    }
    CdMix(&g_akaoCdAtv);
    return 0;
}

void akao_sendCmdE0(s32 *a, s32 b, s32 c) {
    if (akao_checkHeader(a) == 0) {
        g_akaoArgs.arg0 = (s32)a;
        g_akaoArgs.arg1 = (b & 0xFF) << 8;
        g_akaoArgs.arg2 = c;
        akao_command(AKAO_CMD_STREAM_START);
    }
}

void akao_sendCmdE2(void) {
    akao_command(AKAO_CMD_STREAM_STOP);
}

void akao_sendCmdE4(s32 a) {
    g_akaoArgs.arg0 = (a & 0x7F) << 8;
    akao_command(AKAO_CMD_SET_STREAM_VOLUME);
}

void akao_sendCmdE5(s32 a, s32 b) {
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = (b & 0x7F) << 8;
    akao_command(AKAO_CMD_FADE_STREAM_VOLUME);
}

void akao_sendCmdE6(s32 a) {
    g_akaoArgs.arg0 = (a & 0xFF) << 8;
    akao_command(AKAO_CMD_SET_STREAM_PAN);
}

s32 akao_uploadSeBank(s32 *a, s32 b) {
    s32 r = akao_checkHeader(a);
    s32 addr;
    s32 *h;

    if (r == 0) {
        akao_waitSpuTransfer();
        addr = 0x59100;
        if (b == 0) {
            addr = 0x4F100;
        }
        h = a;
        a += 0x10;
        SpuSetTransferStartAddr(addr);
        akao_spuWrite(a, h[4]);
        h[8] = addr;
        akao_copy(h, g_akaoResidentStream, 0x70);
    } else {
        g_akaoResidentStream[8] = 0;
    }
    return r;
}

void akao_sendCmdED(s32 a, s32 b) {
    g_akaoArgs.arg0 = (a & 0xFF) << 8;
    g_akaoArgs.arg1 = b;
    akao_command(AKAO_CMD_STREAM_PLAY_RESIDENT);
}

void akao_sendCmdEC(s32 *a, s32 b, s32 c, s32 d) {
    s32 x;
    if (akao_checkHeader(a) == 0) {
        x = 0x59100;
        if (c == 0) {
            x = 0x4F100;
        }
        g_akaoArgs.arg0 = (s32)a;
        g_akaoArgs.arg1 = (b & 0xFF) << 8;
        g_akaoArgs.arg2 = x;
        g_akaoArgs.arg3 = d;
        akao_command(AKAO_CMD_STREAM_PLAY_ONE_SHOT);
    }
}

s32 akao_sendCmdE8(s32 a, s32 b) {
    if (b == 0) {
        return -1;
    }
    g_akaoArgs.arg0 = a;
    g_akaoArgs.arg1 = b;
    akao_command(AKAO_CMD_STREAM_SETUP_RING);
    return 0;
}

s32 akao_streamPushBlock(void *unused) { /* mbg_stream passes its stream state */
    g_akaoStream.blocksWritten++;
    g_akaoStream.writeBlock++;
    if (g_akaoStream.ringBlocks - 1 < g_akaoStream.writeBlock) {
        g_akaoStream.writeBlock = 0;
    }
    if ((g_akaoStream.flags & 0x1000000) && g_akaoStream.writeBlock >= 2) {
        akao_streamStartRing();
    }
    return g_akaoStream.readBlock;
}

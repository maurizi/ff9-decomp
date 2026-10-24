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
        D_800809DC = (u16 *)p;
        p = a + 0x210;
        D_800809E4 = (u16 *)p;
        p = a + 0x310;
        D_80083148 = (u16 *)p;
        p = a + 0x410;
        D_800809E8 = p;
    }
    return r;
}

s32 akao_playMusic(s32 a) {
    g_akaoArgs.unk0 = a;
    return akao_command(0x10);
}

void akao_stopMusic(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x11);
}

void akao_playMusicFrom(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0x14);
}

void akao_saveMusic(void) {
    akao_command(0x40);
}

s32 func_80055330(void) {
    if (g_akaoSubSeq == 0) {
        D_8007F95A = 0;
    }
    return 0;
}

void akao_pushMusic(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0x19);
}

void akao_crossfadeMusic(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0x1A);
}

s32 akao_playMusicCount(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    return akao_command(0x12);
}

s32 akao_playSe(s32 a, s32 b, s32 c, s32 d) {
    s32 r;

    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c & 0xFF;
    g_akaoArgs.unkC = d & 0x7F;
    if ((u32)a >= 0x400) {
        r = akao_checkHeader((s32 *)a);
        if (r != 0) {
            return r;
        }
        akao_command(0x24);
        return a;
    }
    return akao_command(0x20);
}

void akao_stopSe(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    akao_command(0x21);
}

void akao_playSeDefault(s32 a) {
    g_akaoArgs.unk0 = a & 0x3FF;
    akao_command(0x30);
}

u32 akao_getActiveSe(void) {
    u32 mask = g_akaoSeState.unk0;
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
            r |= c->unk28;
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
    mask = g_akaoSeState.unk0;
    if (mask == 0) {
        return 0;
    }
    c = g_akaoSeChannels;
    for (bit = 0x100; bit & 0xFFFFFF; bit <<= 1) {
        if ((mask & bit) && id == c->unk28) {
            return 1;
        }
        c++;
    }
    return 0;
}

void akao_setOutputMode(s32 a) {
    if (a == 1) {
        a = 0x81;
    } else {
        a = 0x80;
    }
    akao_command(a);
}

void akao_sendCmd90(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x90);
}

void akao_sendCmd92(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x92);
}

void akao_sendCmd99(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9B;
        break;
    case 2:
        cmd = 0x9D;
        break;
    case 3:
        cmd = 0x9F;
        break;
    default:
        cmd = 0x99;
        break;
    }
    akao_command(cmd);
}

void akao_sendCmd98(u32 x) {
    s32 cmd;
    switch (x) {
    case 1:
        cmd = 0x9A;
        break;
    case 2:
        cmd = 0x9C;
        break;
    case 3:
        cmd = 0x9E;
        break;
    default:
        cmd = 0x98;
        break;
    }
    akao_command(cmd);
}

void akao_setAllSeVolume(s32 a) {
    g_akaoArgs.unk0 = a & 0x7F;
    akao_command(0xA8);
}

void akao_fadeAllSeVolume(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xA9);
}

void akao_setSeVolume(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xA0);
}

void akao_fadeSeVolume(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d & 0x7F;
    akao_command(0xA1);
}

void akao_sendCmdAA(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xAA);
}

void akao_sendCmdAB(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xAB);
}

void akao_sendCmdA2(s32 a, s32 b, u8 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    akao_command(0xA2);
}

void akao_sendCmdA3(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d;
    akao_command(0xA3);
}

void akao_sendCmdAC(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xAC);
}

void akao_sendCmdAD(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xAD);
}

void akao_sendCmdA4(s32 a, s32 b, u8 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    akao_command(0xA4);
}

void akao_sendCmdA5(s32 a, s32 b, s32 c, u8 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFFFFFF;
    g_akaoArgs.unk8 = c;
    g_akaoArgs.unkC = d;
    akao_command(0xA5);
}

void akao_setMusicVolume(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xC0);
}

void akao_fadeMusicVolume(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xC1);
}

void akao_fadeMusicVolumeFrom(s32 a, s32 b, s32 c, s32 d) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    g_akaoArgs.unkC = d & 0x7F;
    akao_command(0xC2);
}

void akao_sendCmdC4(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0x7F;
    akao_command(0xC4);
}

void akao_sendCmdC5(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c & 0x7F;
    akao_command(0xC5);
}

void akao_setCdVolume(s32 a) {
    g_akaoArgs.unk0 = a;
    akao_command(0x70);
}

void akao_fadeCdVolume(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0x71);
}

void akao_fadeCdVolumeFrom(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    g_akaoArgs.unk8 = c;
    akao_command(0x72);
}

void akao_sendCmdD0(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD0);
}

void akao_sendCmdD1(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD1);
}

void akao_sendCmdD2(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xD2);
}

void akao_sendCmdD4(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD4);
}

void akao_sendCmdD5(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD5);
}

void akao_sendCmdD6(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xD6);
}

void akao_sendCmdD8(s32 a) {
    g_akaoArgs.unk0 = a & 0xFF;
    akao_command(0xD8);
}

void akao_sendCmdD9(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    akao_command(0xD9);
}

void akao_sendCmdDA(s32 a, s32 b, s32 c) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b & 0xFF;
    g_akaoArgs.unk8 = c & 0xFF;
    akao_command(0xDA);
}

void akao_stopMusicNow(void) {
    akao_command(0xF0);
}

void akao_stopAllSe(void) {
    akao_command(0xF1);
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
    D_8008322C = 0;
    g_akaoFlags.unk0 |= 1;
    return 0;
}

s32 akao_uploadBank(s32 *buf, u32 size, s32 wait) {
    u32 n;
    u32 m;
    s32 slot;

    if (g_akaoFlags.unk0 & 1) {
        if (g_akaoUpload.unk4 == 0) {
            if (buf[0] == AKAO_MAGIC) {
                akao_copy(buf, (u32 *)&g_akaoBankHeader, sizeof(AkaoBankHeader));
                buf += 16;
                size -= sizeof(AkaoBankHeader);
                g_akaoUpload.unk4 = g_akaoBankHeader.spuAddr;
                g_akaoBankHeader.slot = slot = g_akaoBankHeader.slot;
                g_akaoUpload.unk8 = g_akaoBankHeader.sampleSize;
                g_akaoUpload.unk0 = &g_akaoInstruments[slot];
                g_akaoUpload.unkC = g_akaoBankHeader.instCount * 16;
            } else {
                size = 0;
                g_akaoUpload.unk8 = 0;
                g_akaoUpload.unkC = 0;
            }
        }
        if (g_akaoUpload.unkC != 0) {
            n = g_akaoUpload.unkC;
            if (size == 0) {
                goto end;
            }
            if (n >= size) {
                n = size;
            }
            akao_copy(buf, g_akaoUpload.unk0, n);
            buf += n >> 2;
            size -= n;
            g_akaoUpload.unk0 = (Unk8005EBB4 *)((s32 *)g_akaoUpload.unk0 + (n >> 2));
            g_akaoUpload.unkC -= n;
            if (g_akaoUpload.unkC == 0) {
                akao_relocInstruments((s32 *)&g_akaoInstruments[g_akaoBankHeader.slot], &g_akaoInstruments[g_akaoBankHeader.slot], g_akaoBankHeader.spuAddr, g_akaoBankHeader.instCount);
            }
        }
        if (size != 0) {
            if (g_akaoUpload.unk8 == 0) {
                goto clear;
            }
            m = g_akaoUpload.unk8;
            if (m >= size) {
                m = size;
            }
            size = m;
            SpuSetTransferStartAddr(g_akaoUpload.unk4);
            akao_spuWrite(buf, size);
            g_akaoUpload.unk4 += size;
            g_akaoUpload.unk8 -= size;
            if (wait != 0) {
                akao_waitSpuTransfer();
            }
        }
    end:
        if (D_80083230 == 0) {
        clear:
            g_akaoFlags.unk0 &= ~1;
        }
    }
    return D_80083230;
}

void func_80056028(s32 a, s32 b) {
    akao_loadBankRetry(a, b);
}

s32 func_80056048(s32 *a, s32 b, s32 c) {
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
        g_akaoArgs.unk0 = (s32)a;
        g_akaoArgs.unk4 = (b & 0xFF) << 8;
        g_akaoArgs.unk8 = c;
        akao_command(0xE0);
    }
}

void akao_sendCmdE2(void) {
    akao_command(0xE2);
}

void akao_sendCmdE4(s32 a) {
    g_akaoArgs.unk0 = (a & 0x7F) << 8;
    akao_command(0xE4);
}

void akao_sendCmdE5(s32 a, s32 b) {
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = (b & 0x7F) << 8;
    akao_command(0xE5);
}

void akao_sendCmdE6(s32 a) {
    g_akaoArgs.unk0 = (a & 0xFF) << 8;
    akao_command(0xE6);
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
        akao_copy(h, D_8007F720, 0x70);
    } else {
        D_8007F740 = 0;
    }
    return r;
}

void akao_sendCmdED(s32 a, s32 b) {
    g_akaoArgs.unk0 = (a & 0xFF) << 8;
    g_akaoArgs.unk4 = b;
    akao_command(0xED);
}

void akao_sendCmdEC(s32 *a, s32 b, s32 c, s32 d) {
    s32 x;
    if (akao_checkHeader(a) == 0) {
        x = 0x59100;
        if (c == 0) {
            x = 0x4F100;
        }
        g_akaoArgs.unk0 = (s32)a;
        g_akaoArgs.unk4 = (b & 0xFF) << 8;
        g_akaoArgs.unk8 = x;
        g_akaoArgs.unkC = d;
        akao_command(0xEC);
    }
}

s32 akao_sendCmdE8(s32 a, s32 b) {
    if (b == 0) {
        return -1;
    }
    g_akaoArgs.unk0 = a;
    g_akaoArgs.unk4 = b;
    akao_command(0xE8);
    return 0;
}

s32 func_80056488(void) {
    g_akaoStream.unk24++;
    g_akaoStream.unk38++;
    if (g_akaoStream.unk3C - 1 < g_akaoStream.unk38) {
        g_akaoStream.unk38 = 0;
    }
    if ((g_akaoStream.unk8 & 0x1000000) && g_akaoStream.unk38 >= 2) {
        func_80061C1C();
    }
    return g_akaoStream.unk34;
}

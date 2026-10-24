#include "ovl_0a.h"

/* field music/sound request handler (GameData.unk83C); field version of ovl_0c wm_soundCommand */
s32 fld_soundCommand(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 id;
    s32 t;
    s32 h;
    s32 ok;
    s32 slot;
    s32 fp;
    s32 mask;
    s32 mask2;
    s32 sz;
    s32 n;
    s32 m;
    s32 d;

    kind = (arg0 >> 8) & 0xF;
    if (kind == 12 && g_gameState->unk1C->unk851 != 0) {
        goto state;
    }
    switch ((arg0 >> 12) & 3) {
    case 0:
        t = -1;
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 9:
            ok = 0;
            if (g_gameState->unk1C->unk7E8 != 0xFFFF && g_gameState->unk1C->unk7EA == (s16)g_gameState->unk1C->fieldId) {
                GameData *p;
                ok = 1;
                g_fieldMusicRequest = kind;
                p = g_gameState->unk1C;
                p->unk826[0] = p->unk850;
                arg1 = p->unk7E8;
                id = p->unk7E8 | 0x70000;
                t = p->unk840 | 0x90000;
                g_gameState->unk1C->unk8A8 = g_gameState->unk1C->unk8AC;
                g_gameState->unk1C->unk84A = g_gameState->unk1C->unk8B0;
            }
            /* v2 */
            g_gameState->unk1C->unk7E8 = 0xFFFF;
            g_gameState->unk1C->unk840 = 0xFFFF;
            g_gameState->unk1C->unk7EA = 0xFFFF;
            g_gameState->unk1C->unk8AC = -1;
            g_gameState->unk1C->unk8B0 = 0xFFFF;
            if (!ok) {
                return 0;
            }
        case 0:
        case 7:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (g_fieldMusicRequest == -1) {
                g_fieldMusicRequest = kind;
            }
            if (g_fieldMusicRequest != 9) {
                t = fld_depGetLink(id);
            }
            if (g_gameState->unk1C->unk81C[0] != arg1 || g_gameState->unk1C->unk820[0] != (u16)t) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    g_gameState->unk1C->unk0 |= 0x200000;
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                SCENE->unk0 &= ~0x10;
                g_gameState->unk1C->unk820[0] = t;
                g_fieldSoundLoadId = id;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk851 = 1;
                return g_gameState->unk1C->unk851;
            }
            if (g_fieldMusicRequest == 7) {
                goto done;
            }
        start:
            g_gameState->unk1C->unk824[0] = akao_playMusic(g_gameState->unk1C->unk814);
            if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                akao_fadeMusicVolumeFrom(g_gameState->unk1C->unk824[0], 0xA, 0, g_gameState->unk1C->unk826[0]);
            }
            g_gameState->unk1C->unk0 |= 0x30000000;
        done:
            g_fieldMusicRequest = -1;
            return 0;
        case 12:
            if (g_gameState->unk1C->unk851 != 0) {
                goto state;
            }
            return fld_isMusicFading() ? 6 : 0;
        case 8:
            akao_saveMusic();
            g_gameState->unk1C->unk7E8 = g_gameState->unk1C->unk81C[0];
            g_gameState->unk1C->unk840 = g_gameState->unk1C->unk820[0];
            g_gameState->unk1C->unk850 = g_gameState->unk1C->unk826[0];
            g_gameState->unk1C->unk7EA = g_gameState->unk1C->fieldId;
            g_gameState->unk1C->unk8AC = g_gameState->unk1C->unk8A8;
            g_gameState->unk1C->unk8B0 = g_gameState->unk1C->unk84A;
        case 1:
            if ((arg0 & 0x3F) == 1) {
                akao_stopMusic(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
                return 0;
            }
            akao_stopMusic(g_gameState->unk1C->unk824[0]);
            g_gameState->unk1C->unk826[0] = 0x7F;
            g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
            return 0;
        case 2:
            if ((arg0 & 0x3F) == 8) {
                return 0;
            }
            switch (arg0 & 0x3F) {
            case 1 ... 3:
                switch (arg0 & 0x3F) {
                case 1:
                    akao_setMusicVolume(g_gameState->unk1C->unk824[0], arg2);
                    g_gameState->unk1C->unk826[0] = arg2;
                    return 0;
                case 2:
                    akao_sendCmdD0(arg2);
                    return 0;
                case 3:
                    akao_sendCmdD4(arg2);
                    return 0;
                case 4:
                    akao_sendCmdD8(arg2);
                    return 0;
                }
                break;
            case 7:
                g_gameState->unk1C->unk824[0] = akao_playMusicCount(g_gameState->unk1C->unk814, arg2);
                g_gameState->unk1C->unk0 |= 0x20000000;
                return 0;
            }
            break;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeMusicVolume(g_gameState->unk1C->unk824[0], arg2, arg3);
                fld_setMusicVolumeFade(arg3, arg2);
                return 0;
            case 2:
                akao_sendCmdD1(arg2, arg3);
                return 0;
            case 3:
                akao_sendCmdD5(arg2, arg3);
                return 0;
            case 4:
                akao_sendCmdD9(arg2, arg3);
                return 0;
            }
            break;
        case 4:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeMusicVolumeFrom(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
                fld_setMusicVolumeFade(arg4, arg2);
                return 0;
            case 2:
                akao_sendCmdD2(arg2, arg3, arg4);
                return 0;
            case 3:
                akao_sendCmdD6(arg2, arg3, arg4);
                return 0;
            case 4:
                akao_sendCmdDA(arg2, arg3, arg4);
                return 0;
            }
            break;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(0, arg2, arg3);
            fld_setMusicVolumeFade(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, arg2, arg3, arg4);
            fld_setMusicVolumeFade(arg4, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            akao_sendCmd92(arg1);
            return 0;
        }
        return -1;
    case 1:
        /* rodata pad between the kind switches' jump tables */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        fp = 0;
        slot = 0;
        mask = 0;
        h = 0;
        id = (arg1 & 0xFFFF) | 0x80000;
        if (kind != 5 && kind != 6) {
            fp = (arg0 >> 7) & 1;
            if (fp != 0) {
                if (kind == 0 || kind == 8 || kind == 9) {
                    slot = (arg0 >> 6) & 1;
                } else {
                    for (slot = 0; slot < 2; slot++) {
                        if (g_gameState->unk1C->unk830[slot] == arg1) {
                            break;
                        }
                    }
                    if (slot >= 2) {
                        return -1;
                    }
                }
                arg2 = (0x100000 << slot) & 0x300000;
                mask = 0x2000000 << (slot * 2);
                mask2 = mask >> 1;
                if (kind == 0 && g_gameState->unk1C->unk830[slot] != arg1) {
                    if (g_gameState->unk1C->unk0 & mask) {
                        akao_stopSe(SFXH(slot), arg2);
                        g_gameState->unk1C->unk0 ^= mask | mask2;
                    }
                    memcpy((void *)SFXH(slot), db_lookupRegistered(4, id, 0), (s32)db_lookupRegistered(5, id, 0));
                    g_gameState->unk1C->unk830[slot] = arg1;
                    g_gameState->unk1C->unk8B8[slot] = 0x7F;
                    g_gameState->unk1C->unk8BA[slot] = 0x80;
                    g_gameState->unk1C->unk0 |= mask2;
                }
                h = SFXH(slot);
            } else {
                h = (s32)db_lookupRegistered(4, id, 0);
            }
        }
        switch (kind) {
        case 9:
            t = 0;
            if (g_gameState->unk1C->unk830[slot] != 0xFFFF && g_gameState->unk1C->unk8B4[slot] == (s16)g_gameState->unk1C->fieldId) {
                arg2 = (0x100000 << slot) & 0x300000;
                t = 1;
                arg3 = g_gameState->unk1C->unk8BA[slot];
                arg4 = g_gameState->unk1C->unk8B8[slot];
            }
            g_gameState->unk1C->unk8B4[slot] = 0xFFFF;
            if (!t) {
                return 0;
            }
        case 0:
            if (!fp || !(g_gameState->unk1C->unk0 & mask)) {
                akao_playSe(h, arg2, arg3, arg4);
            }
            if (fp) {
                g_gameState->unk1C->unk8BA[slot] = arg3;
                g_gameState->unk1C->unk8B8[slot] = arg4;
                g_gameState->unk1C->unk0 |= mask;
            }
            return 0;
        case 8:
            if (!(g_gameState->unk1C->unk0 & mask)) {
                return 0;
            }
            g_gameState->unk1C->unk8B4[slot] = g_gameState->unk1C->fieldId;
        case 1:
            if ((arg0 & 0x3F) == 9 && fp) {
                if (g_gameState->unk1C->unk0 & 0x2000000) {
                    akao_stopSe((s32)g_gameState->unk1C->unk828, 0x100000);
                }
                if (g_gameState->unk1C->unk0 & 0x8000000) {
                    akao_stopSe((s32)g_gameState->unk1C->unk82C, 0x200000);
                }
                g_gameState->unk1C->unk0 &= 0xF5FFFFFF;
            } else {
                akao_stopSe(h, arg2);
                if (fp) {
                    g_gameState->unk1C->unk0 &= ~mask;
                }
            }
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setSeVolume(h, arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8B8[slot] = arg3;
                }
                return 0;
            case 5:
                akao_sendCmdA2(h, arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8BA[slot] = arg3;
                }
                return 0;
            case 3:
                akao_sendCmdA4(h, arg2, arg3);
                return 0;
            case 8:
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeSeVolume((void *)h, arg2, arg3, arg4);
                if (fp) {
                    g_gameState->unk1C->unk8B8[slot] = arg4;
                }
                return 0;
            case 5:
                akao_sendCmdA3(h, arg2, arg3, arg4);
                if (fp) {
                    g_gameState->unk1C->unk8BA[slot] = arg4;
                }
                return 0;
            case 3:
                akao_sendCmdA5(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setAllSeVolume(arg2);
                if (fp) {
                    g_gameState->unk1C->unk8B8[0] = g_gameState->unk1C->unk8B8[1] = arg2;
                }
                return 0;
            case 5:
                akao_sendCmdAA(arg2);
                if (fp) {
                    g_gameState->unk1C->unk8BA[0] = g_gameState->unk1C->unk8BA[1] = arg2;
                }
                return 0;
            case 3:
                akao_sendCmdAC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeAllSeVolume(arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8B8[0] = g_gameState->unk1C->unk8B8[1] = arg3;
                }
                return -1;
            case 5:
                akao_sendCmdAB(arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8BA[0] = g_gameState->unk1C->unk8BA[1] = arg3;
                }
                return -1;
            case 3:
                akao_sendCmdAD(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    case 2:
        id = (arg1 & 0xFFFF) | 0x180000;
        switch (kind) {
        case 0:
            slot = (s32)db_lookupRegistered(4, id, 0);
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            akao_sendCmdE4(fld_isMoviePlaying() ? SCENE->unk5D : SCENE->unk5C);
            akao_sendCmdE0(slot, arg2, arg3);
            SCENE->unk5F = 0;
            return 0;
        case 1:
            akao_sendCmdE2();
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                SCENE->unk5C = arg2;
                if (fld_isMoviePlaying() == 0) {
                    akao_sendCmdE4(arg2);
                }
                return 0;
            case 10:
                SCENE->unk5D = arg2;
                if (fld_isMoviePlaying() != 0) {
                    akao_sendCmdE4(arg2);
                }
                return 0;
            case 5:
                akao_sendCmdE6(arg2);
                return 0;
            case 12:
                SCENE->unk5F = arg2;
                return 0;
            case 8:
                return 0;
            }
            return -1;
        }
        return -1;
    case 3:
        id = (arg1 & 0xFFFF) | 0x90000;
        switch (kind) {
        case 7:
            SCENE->unk0 &= ~0x10;
            if (arg2 == 4) {
                SCENE->soundBankSlot = arg3;
                SCENE->unk0 |= 0x10;
            }
            g_fieldMusicRequest = kind;
            g_fieldSoundLoadId = id;
            g_gameState->unk1C->unk851 = 1;
            return g_gameState->unk1C->unk851;
        case 12:
            goto state;
        }
        return -1;
    }
    return -1;

state:
    switch (g_gameState->unk1C->unk851) {
    case 1:
        if (fld_isMusicFading() == 0) {
            if (g_gameState->unk1C->unk0 & 0x200000) {
                akao_stopMusic(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk0 &= ~0x200000;
            }
            g_gameState->unk1C->unk851 = 2;
        }
        break;
    case 2:
        if (((g_fieldSoundLoadId >> 16) & 0xFF) == 7) {
            g_gameState->unk1C->unk852 = g_gameState->unk1C->unk820[0];
        } else {
            g_gameState->unk1C->unk852 = g_fieldSoundLoadId;
        }
        if (g_fieldMusicRequest == 9) {
            g_fieldSoundLoadSector = g_gameState->unk1C->unk8A8;
            sz = g_gameState->unk1C->unk84A << 11;
        } else {
            g_fieldSoundLoadSector = fld_depGetFileSector(g_fieldSoundLoadId, &sz);
            if (((g_fieldSoundLoadId >> 16) & 0xFF) == 7) {
                g_gameState->unk1C->unk8A8 = g_fieldSoundLoadSector;
                g_gameState->unk1C->unk84A = sz >> 11;
            }
        }
        g = g_gameState->unk1C;
        if (!(g->unk8E4.field->unkC->unk14->unk0 & 0x10) && g->unk852 != 0x84) {
            akao_beginBankUpload();
        }
        g_gameState->unk1C->unk8C0 = SCENE->heapTop ? SCENE->heapTop : D_80067948;
        g_gameState->unk1C->unk878 = 0x156F00 - (g_gameState->unk1C->unk8C0 - (s32)D_80067944);
        if (!(g_gameState->unk1C->unk8E4.field->unk0 & 4)) {
            g_gameState->unk1C->unk878 -= 0x3000;
        }
        m = g_gameState->unk1C->unk878;
        g_gameState->unk1C->unk878 = m / 2048 * 2048;
        g_gameState->unk1C->unk87C = 0;
        cdRead_init(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, sz, (void *)g_gameState->unk1C->unk8C0, 0,
                      g_gameState->unk1C->unk878, fld_soundBankReadCallback);
        g_gameState->unk1C->unk851 = 3;
        break;
    case 3:
        if (cdRead_update(g_gameState->unk1C->unk854) == 0 && akao_getSpuTransferState() == 0) {
            fld_invalidateModelCache((s32)g_gameState->unk1C->unk854[0].unk20);
            preload_discardRange((s32)g_gameState->unk1C->unk854[0].unk8, g_gameState->unk1C->unk854[0].unk10);
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk87C) {
                g_gameState->unk1C->unk8C0 = (s32)g_gameState->unk1C->unk854[0].unk8;
                d = 0x156F00 - ((s32)g_gameState->unk1C->unk854[0].unk8 - (s32)D_80067944);
                if (!(g_gameState->unk1C->unk8E4.field->unk0 & 4)) {
                    d -= 0x3000;
                }
                d = d / 2048 * 2048;
                cdRead_init(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
                              g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10, d, fld_soundBankReadCallback);
            } else if (g_fieldSoundLoadKind == 7) {
                if (g_fieldMusicRequest != 9) {
                    g_gameState->unk1C->unk826[0] = 0x7F;
                }
                g_gameState->unk1C->unk851 = 4;
            } else {
                g_fieldMusicRequest = -1;
                g_gameState->unk1C->unk851 = 0;
            }
        }
        break;
    case 4:
        n = g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 - g_gameState->unk1C->unk87C;
        if (n > 0) {
            memcpy(g_gameState->unk1C->unk814, (u8 *)g_gameState->unk1C->unk8C0 + (g_gameState->unk1C->unk87C - g_gameState->unk1C->unk854[0].unkC), n);
        }
        t = 0x6800 - n;
        g_gameState->unk1C->unk8C0 = (s32)((u8 *)g_gameState->unk1C->unk814 + n);
        t = t / 2048 * 2048;
        cdRead_init(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
                      g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10, t, NULL);
        g_gameState->unk1C->unk851 = 5;
        break;
    case 5:
        if (cdRead_update(g_gameState->unk1C->unk854) == 0) {
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk854[0].unk4) {
                m = g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 - g_gameState->unk1C->unk87C;
                g_gameState->unk1C->unk8C0 = (s32)((u8 *)g_gameState->unk1C->unk814 + m);
                cdRead_init(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0, m, 0x6800 - m, NULL);
            } else {
                fld_unpackMusicSeq((s32)g_gameState->unk1C->unk814);
                g_gameState->unk1C->unk0 |= 0x10000000;
                g_gameState->unk1C->unk851 = 0;
                if (g_fieldMusicRequest != 7) {
                    goto start;
                }
                g_fieldMusicRequest = -1;
            }
        }
        break;
    }
    return g_gameState->unk1C->unk851;
}

void fld_getSoundPanVolume(s16 arg0, s16 arg1, s16 arg2, u8 *arg4, u8 *arg5) {
    SVECTOR v;
    DVECTOR sxy;
    s32 z;
    s32 a;
    s32 pan;
    s32 b;

    v.vx = arg0;
    v.vy = arg1;
    v.vz = arg2;
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_stsz(&z);
    z = z / 4 + g_gameState->unk1C->unk8E4.field->unkC->unk14->otBias;
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
    *arg4 = pan;
    *arg5 = (0x1000 - z) * 127 / 4096;
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
        if (g_gameState->unk1C->unk830[i] != 0xFFFF) {
            fld_soundCommand(0x5180, g_gameState->unk1C->unk830[i], 0, 0, 0);
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
void fld_dropSoundBankCache(s32 arg0) {
    s32 i;
    u8 t;

    if (arg0 >= 0x40000) {
        for (i = 4; i >= 0; i--) {
            t = (u32)g_fieldSoundBankIds[i] >> 24;
            if (t != 4 && t != 1) {
                g_fieldSoundBankIds[i] = -1;
                g_fieldSoundBankSlots[i] = -1;
            }
        }
    }
}

void fld_stopSceneSfx(FieldScene *arg0, s16 arg1) {
    s32 t = (s32)arg0->fieldFile;
    s32 i;

    for (i = db_lookup(t, 1, 0x80000, 0) - 1; i >= 0; i--) {
        akao_stopSe(db_lookup(t, 2, (i & 0xFFFF) | 0x80000, 0), 0);
    }
    akao_sendCmdE2();
}

void fld_playAnimSfx(FieldEvtCtx **arg0, s32 arg1) {
    s16 a;
    s16 b;
    u16 c;
    u8 vol;
    u8 pan;
    FieldAnimSfx *base;
    u8 n;
    s32 i;
    s32 j;
    FieldEvtCtx *ctx;
    FieldAnimSfx *t;
    s32 r;
    s32 ch;
    s32 snd;
    s32 v;

    base = g_gameState->unk1C->unk8E4.field->unkC->unk14->animSfx;
    n = g_gameState->unk1C->unk8E4.field->unkC->unk14->animSfxCount;
    for (i = arg1 - 1; i >= 0; i--) {
        ctx = arg0[i];
        if (ctx == NULL || (ctx->unk0 & 0x40)) {
            continue;
        }
        for (j = n - 1; j >= 0; j--) {
            t = &base[j];
            if (t->unk0 == ctx->unkC->unk3B && t->anim == ctx->unkC->unk36 && t->frame == ctx->unkC->unk3A) {
                r = 0;
                a = 0;
                b = 0;
                c = 0;
                vol = 0x80;
                pan = 0x7F;
                bgi_getActorTriangle(ctx->unkC->unk3B, (u16 *)&a, (u16 *)&b);
                if (a != -1) {
                    bgi_getTriangleFlags(a, &c);
                    c = (s16)c >> 8;
                }
                ch = t->unk0 & 3;
                ch = (0x10000 << ch) & 0xF0000;
                snd = t->sound[(c >> 4) & 1] | 0x80000;
                if (t->randomPitch != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                    v = *g_randPtr++;
                    r = (v >> 4) - 8;
                }
                fld_getSoundPanVolume(ctx->unkC->unk14.vx, ctx->unkC->unk14.vy, ctx->unkC->unk14.vz, &vol, &pan);
                fld_soundCommand(0xD000, snd, ch, vol, pan);
                if (r != 0) {
                    fld_soundCommand(0x9203, snd, ch, r, 0);
                }
            }
        }
    }
}

/* find the trigger matching the object, id and value (searching backwards) */
FieldAnimSfx *fld_findAnimSfx(FieldEvtCtx *arg0, u16 arg1, s16 arg2) {
    FieldScene *sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    FieldAnimSfx *base = sc->animSfx;
    FieldAnimSfx *e = &base[sc->animSfxCount] - 1;

    do {
        if (e->unk0 == arg0->unkC->unk3B && e->anim == arg1 && e->frame == arg2) {
            return e;
        }
        e--;
    } while (e >= base);
    return NULL;
}

FieldAnimSfx *fld_addAnimSfx(FieldEvtCtx *arg0, s32 arg1, s32 arg2) {
    FieldScene *sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    u8 i = sc->animSfxCount;
    FieldAnimSfx *e;
    u16 id;
    s32 n;

    n = i + 1;
    sc->animSfxCount = n;
    e = &sc->animSfx[i];
    e->unk0 = arg0->unkC->unk3B;
    id = arg0->unkC->unk34;
    e->anim = arg1;
    do {
        e->frame = arg2;
    } while (0);
    e->unk2 = id;
    return e;
}

s32 fld_removeAnimSfx(FieldEvtCtx *arg0, u16 arg1, s16 arg2) {
    FieldAnimSfx *base;
    FieldAnimSfx *last;
    FieldAnimSfx *e;

    base = g_gameState->unk1C->unk8E4.field->unkC->unk14->animSfx;
    e = &base[g_gameState->unk1C->unk8E4.field->unkC->unk14->animSfxCount] - 1;
    last = e;
    do {
        if (e->unk0 == arg0->unkC->unk3B && e->anim == arg1 && e->frame == arg2) {
            if (e != last) {
                *e = *last;
            }
            g_gameState->unk1C->unk8E4.field->unkC->unk14->animSfxCount--;
        }
        e--;
    } while (e >= base);
    return -1;
}

s32 fld_depGetLink(s32 arg0) {
    FileTable *t = g_gameState->unk1C->depTable;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unk0 == arg0) {
            return e[i].unkC;
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

    o = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    o->battleMusic = g_gameState->unk1C->unk81C[0];
    key = (u16)o->nextId;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3E = g_gameState->unk1C->unk820[0];
    base = g_gameState->unk1C->depTable->unk4;
    key |= 0x110000;
    w = key;
    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
        id = base[i].unkC;
        if (base[i].unk0 == w && ((id >> 16) & 0xFF) == 7) {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->battleMusic = id;
            for (j = 0; j < g_gameState->unk1C->depTable->unk2; j++) {
                w = base[j].unkC;
                v = base[j].unk0;
                if (((base[j].unk0 >> 16) & 0xFF) == 9 && w == id) {
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3E = v;
                    return;
                }
            }
            return;
        }
    }
}

/* assign sound slots to the field's sound files and load the missing ones (smallest first) */
s32 fld_loadSoundBanks(void) {
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
    e = g_gameState->unk1C->depTable->unk4;
    for (i = 4; i >= 0; i--) {
        id[i] = -1;
        sz[i] = -1;
        used[i] = 0;
        slot[i] = -1;
    }
    for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
        x = e[i].unk0;
        if (((x >> 16) & 0xFF) == 9) {
            found = 0;
            if (((e[i].unkC >> 16) & 0xFF) == 8) {
                s = -1;
                for (j = 4; j >= 0; j--) {
                    if (g_fieldSoundBankIds[j] == x) {
                        found = 1;
                        s = g_fieldSoundBankSlots[j];
                        break;
                    }
                }
                id[n] = x;
                sz[n] = e[i].unk4;
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
        fld_soundCommand(0x7700, z, z >> 24, slot[best], 0);
        while (fld_soundCommand(0xC00, -1, 0, 0, 0) != 0) {
        }
        id[best] = -1;
        sz[best] = -1;
    }
    return 0;
}

s32 fld_depGetFileSector(s32 arg0, s32 *arg1) {
    FileEntry *e = dep_find(g_gameState->unk1C->depTable, arg0);

    if (arg1 != NULL) {
        *arg1 = e->unk8 << 11;
    }
    return e->unk4;
}

s32 fld_soundBankReadCallback(Unk800B29F8 *arg0) {
    s32 a;
    s32 id;
    s32 t;
    s32 n;
    s32 b;
    s32 r;
    GameData *gd;
    FieldScene *sc;

    r = 0;
    a = arg0->unk8;
    id = g_gameState->unk1C->unk852 | 0x90000;
    if (arg0->unkC == 0) {
        b = a;
        t = db_lookup(a, 2, 0x1B0000, 0);
        if (t == -1) {
            a = db_lookup(a, 4, id, 0);
            g_gameState->unk1C->unk87C = db_getSize(b);
        } else {
            a = db_lookup(t, 4, id, 0);
            g_gameState->unk1C->unk87C = db_lookup(b, 2, 0x1B0001, 0) - g_gameState->unk1C->unk8C0;
            fld_dropSoundBankCache(g_gameState->unk1C->unk87C - arg0->unkC);
        }
    }
    gd = g_gameState->unk1C;
    n = gd->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    n = arg0->unk8 + n - a;
    if ((id & 0xFFFF) != 0x84) {
        sc = gd->unk8E4.field->unkC->unk14;
        if (sc->unk0 & 0x10) {
            r = func_80056048(a, sc->soundBankSlot, 0);
        } else {
            r = akao_uploadBank(a, n, 0);
        }
    }
    arg0->unk8 = a;
    return r;
}

s32 fld_unpackMusicSeq(s32 arg0) {
    s32 t = db_lookup(arg0, 3, 0x70000, 0);

    memcpy((u8 *)arg0, (u8 *)db_lookup(arg0, 4, t, 0), db_lookup(arg0, 5, t, 0));
    return 0;
}

void fld_setMusicVolumeFade(s32 arg0, u16 arg1) {
    GameData *gd = g_gameState->unk1C;

    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 fld_isMusicFading(void) {
    GameData *gd = g_gameState->unk1C;

    return (u32)(VSync(-1) - gd->unk8C4) < gd->unk842;
}

void fld_setObjectColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    fld_setSlotColor((FieldEvtCtx *)game_getModel(arg0), arg1, arg2, arg3);
}

/* apply the field lights to the object colours */
void fld_applyLights(FieldEvtCtx **arg0, s32 arg1) {
    FieldScene *sc;
    Unk800B3254 *lt;
    FieldLight *amb;
    FieldLight *dir;
    FieldActorInfo *infos;
    Unk800B3294 *def;
    s32 i;
    FieldEvtCtx *c;
    s32 chg;
    FieldActorInfo *info;
    Unk800B3294 *l;
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

    sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    lt = sc->lights;
    if (lt == NULL || fld_getLightColorTable(lt) == NULL) {
        return;
    }
    amb = NULL;
    dir = NULL;
    infos = sc->actorInfo;
    def = (Unk800B3294 *)fld_findLightColor(lt, -1);
    fld_findGlobalLights(lt, &amb, &dir);
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        chg = 0;
        if (c == NULL) {
            continue;
        }
        info = &infos[c->unkC->unk3B];
        l = (Unk800B3294 *)fld_findLightColor(lt, c->unk4);
        shade = c->unk0 & 4;
        fl = fld_findFloorLight(lt, c, &floor);
        if ((u8)info->unk4 != (u8)floor || shade != (info->unk0 & 4) || !(c->unk0 & 0x100)) {
            chg = 1;
        }
        if (chg) {
            if (!(c->unk0 & 0x400)) {
                b = 0;
                if (l != NULL) {
                    a = l->unk4;
                } else {
                    a = def->unk4;
                }
                if (shade && dir != NULL) {
                    b = dir->unk4;
                } else if (fl != NULL) {
                    b = fl->unk4;
                } else if (amb != NULL) {
                    b = amb->unk4;
                }
                col = ((((a & 0xFF) + (b & 0xFF)) * 8) & 0xFF) | ((((a & 0xFF00) + (b & 0xFF00)) * 8) & 0xFF00)
                    | ((((a & 0xFF0000) + (b & 0xFF0000)) * 8) & 0xFF0000) | 0x01000000;
                info->unkC.w = col;
                info->unk8.w = col;
                c->unk0 |= 0x100;
            }
            if (l != NULL) {
                dx = l->unk2;
                dy = l->unk3;
            } else {
                dx = def->unk2;
                dy = def->unk3;
            }
            if (fl != NULL) {
                dx += fl->unk2;
                dy += fl->unk3;
            } else if (amb != NULL) {
                dx += amb->unk2;
                dy += amb->unk3;
            }
            dx *= 8;
            g_gameState->unk1C->unk8E4.field->unkC->unk14->shadows[c->unkC->unk3B].unk0.unk0 = dx;
            g_gameState->unk1C->unk8E4.field->unkC->unk14->shadows[c->unkC->unk3B].unk0.unk1 = dy * 14;
            g_gameState->unk1C->unk8E4.field->unkC->unk14->shadows[c->unkC->unk3B].unk0.unk2 = dy * 12;
        }
        info->unk4 = floor;
        info->unk0 = (info->unk0 & ~4) | shade;
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        inf2 = &infos[c->unkC->unk3B];
        k = inf2->unk8.w;
        if (k >> 24 != 0) {
            for (j = ((ModelObj *)c->unk8)->model->unk3 - 1; j >= 0; j--) {
                fldmdl_setGroupRgb(c->unk8, j & 0xFF, (s16)(k & 0xFF), (s16)((k >> 8) & 0xFF), ((u32)k >> 16) & 0xFF);
            }
            inf2->unk8.w = inf2->unkC.w;
            inf2->unkC.w = 0;
        }
    }
}

/* set the object's light colour */
s32 fld_setSlotColor(FieldEvtCtx *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 f = arg0->unk0;
    FieldActorInfo *info = &g_gameState->unk1C->unk8E4.field->unkC->unk14->actorInfo[arg0->unkC->unk3B];
    u32 col;

    if (!(f & 0x800)) {
        col = (arg1 & 0xFF) | ((arg2 << 8) & 0xFF00) | ((arg3 << 16) & 0xFF0000) | 0x01000000;
        info->unkC.w = col;
        info->unk8.w = col;
        arg0->unk0 |= 0xC00;
        return 1;
    }
    arg0->unk0 = f & ~0x800;
    return 0;
}

FieldLight *fld_getLightColorTable(Unk800B3254 *arg0) {
    if (arg0 != NULL && arg0->unk9 != 0) {
        return &arg0->unkC[arg0->unk6];
    }
    return NULL;
}

Unk800B3294 *fld_findLightColor(Unk800B3254 *arg0, u16 arg1) {
    Unk800B3294 *base;
    Unk800B3294 *p;
    s32 id;
    u32 v;

    v = arg1;
    if (arg0->unk9 == 0) {
        return NULL;
    }
    id = v & 0xFFFF;
    base = (Unk800B3294 *)&arg0->unkC[arg0->unk6];
    p = &base[arg0->unk9] - 1;
    do {
        if (p->unk0 == id) {
            return p;
        }
        p--;
    } while (p >= base);
    return NULL;
}

FieldLight *fld_findGlobalLights(Unk800B3254 *arg0, FieldLight **arg1, FieldLight **arg2) {
    FieldLight *base;
    FieldLight *p;

    *arg1 = NULL;
    *arg2 = NULL;
    base = arg0->unkC;
    if (arg0->unk7 == 0) {
        return NULL;
    }
    p = &arg0->unkC[arg0->unk7 - 1];
    do {
        if (p->unk0 == 1) {
            *arg1 = p;
        } else if (p->unk0 == 2) {
            *arg2 = p;
        }
        p--;
    } while (p >= base);
    return base;
}

/* find the floor light covering the object's current floor */
FieldLight *fld_findFloorLight(Unk800B3254 *arg0, FieldEvtCtx *arg1, s16 *arg2) {
    u16 tri;
    FieldLight *p;
    s32 n;
    s32 v;

    bgi_getActorTriangle(arg1->unkC->unk3B, &tri, (u16 *)arg2);
    if (arg0->unk7 != 0) {
        if ((u8)*arg2 == 0xFF) {
            return NULL;
        }
        p = &arg0->unkC[arg0->unk7 - 1];
        do {
            if (p->unk0 == 0) {
                v = p->unk8;
                for (n = 3; n >= 0; n--) {
                    if ((v & 0xFF) == *arg2) {
                        return p;
                    }
                    v >>= 8;
                }
            }
            p--;
        } while (p >= arg0->unkC);
    }
    return NULL;
}

s32 func_800B3444(s32 arg0, s32 arg1, s32 arg2) {
    s32 start;
    s32 size;
    s32 end;

    if (arg0 == 0 && arg1 == 0) {
        if (arg2 != 0) {
            if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100)) {
                start = g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop;
                if (start == 0) {
                    start = D_80067948;
                }
                if (preload_overlaps(start, 0x5000) != 0) {
                    preload_remove(preload_waitCurrent()->unk4);
                }
                if (g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop != 0) {
                    start = g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop;
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop = func_80050BE0(start);
                    size = g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop - start;
                } else {
                    start = D_80067948;
                    end = func_80050BE0(start);
                    size = end - start;
                    D_80067948 = end;
                }
                fld_trimModelCache();
                preload_discardRange(start, size);
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100;
            }
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x200;
        } else {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x200;
        }
    }
    return 0;
}

void func_800B3684(void) {
    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x200) {
        func_80050AAC();
    }
}

s32 fld_preloadRequest(s32 arg0, s32 arg1) {
    s32 flag;
    s32 id;
    s32 k;
    u8 type;
    s32 x;
    PreloadEntry *e;
    PreloadEntry *p;
    PreloadEntry *q;
    FileEntry *f;

    type = arg0;
    flag = 0;
    if ((s8)type < 0) {
        type -= 0x80;
        flag = 1;
    }
    switch (type & 0xFF) {
    default:
        return -1;
    case 0x11:
        k = 0x1000B;
        if (flag && fld_findPreloadExclude(0x1000B)) {
            return -1;
        }
        if (fld_findPreloadRequest(0x1000B)) {
            return -1;
        }
        if (preload_find(0x1000B)) {
            return -1;
        }
        if (flag) {
            fld_addPreloadExclude(0x1000B);
        } else {
            fld_addPreloadRequest(0x1000B);
        }
        if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
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
        id = (u16)arg1 | 0x50000;
        if ((u16)arg1 >= 9000) {
            return -1;
        }
        f = dep_find(g_gameState->unk1C->depTable, id);
        if (f == NULL) {
            return -1;
        }
        if (f->unk8 == 0) {
            return -1;
        }
        if (flag && fld_findPreloadExclude(id)) {
            return -1;
        }
        if (fld_findPreloadRequest(id)) {
            return -1;
        }
        e = preload_find(id);
        if (e != NULL && e->unk2 == 0) {
            if (preload_isBusy(e)) {
                return -1;
            }
            if (!preload_isBusy(e) && (e->unk1C >> 2) < e->unkC) {
                return -1;
            }
        }
        if (flag) {
            x = id;
        } else {
            fld_addPreloadRequest(id);
            x = 0x1000B;
        }
        fld_addPreloadExclude(x);
        if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
            goto done;
        }
        e = preload_getCurrent();
        if (e != NULL && e->fileId != id && preload_isBusy(e)) {
            preload_wait(e);
        }
        q = preload_find(id);
        if (!flag && (q == NULL || q->unk2 != 0)) {
        clear:
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x100000;
        }
    set:
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x800000;
    done:
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x200000;
        return 0;
    }
}

void fld_initPreload(void) {
    preload_init();
    g_gameState->unk1C->unk8BC = fld_preloadRequest;
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

void fld_setPreloadCursor(s32 arg0) {
    if (arg0 != 0) {
        g_fieldPreloadCursor = arg0;
    }
}

s32 fld_isEncounterPreloadRequested(void) {
    return fld_findPreloadRequest(0x1000B) != 0;
}

s32 fld_isEncounterPreloadExcluded(void) {
    return fld_findPreloadExclude(0x1000B) != 0;
}

/* is any active entry of category 5? */
s32 fld_hasFieldPreloadRequest(void) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (((g_fieldPreloadRequests[i] >> 16) & 0xFF) == 5) {
            return 1;
        }
    }
    return 0;
}

s32 *fld_findPreloadRequest(s32 arg0) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (g_fieldPreloadRequests[i] == arg0) {
            return &g_fieldPreloadRequests[i];
        }
    }
    return NULL;
}

void fld_removePreloadRequest(s32 arg0) {
    s32 i;

    for (i = g_fieldPreloadRequestCount - 1; i >= 0; i--) {
        if (g_fieldPreloadRequests[i] == arg0) {
            if (i < g_fieldPreloadRequestCount - 1) {
                g_fieldPreloadRequests[i] = g_fieldPreloadRequests[g_fieldPreloadRequestCount - 1];
            }
            g_fieldPreloadRequestCount--;
            return;
        }
    }
}

s32 *fld_addPreloadRequest(s32 arg0) {
    s32 *p = fld_findPreloadRequest(arg0);

    if (p != NULL) {
        return p;
    }
    if (g_fieldPreloadRequestCount < 8) {
        p = &g_fieldPreloadRequests[g_fieldPreloadRequestCount];
        g_fieldPreloadRequestCount++;
        *p = arg0;
        return p;
    }
    return NULL;
}

s32 *fld_findPreloadExclude(s32 arg0) {
    s32 i;

    for (i = g_fieldPreloadExcludeCount - 1; i >= 0; i--) {
        if (g_fieldPreloadExcludes[i] == arg0) {
            return &g_fieldPreloadExcludes[i];
        }
    }
    return NULL;
}

void fld_removePreloadExclude(s32 arg0) {
    s32 i;

    for (i = g_fieldPreloadExcludeCount - 1; i >= 0; i--) {
        if (g_fieldPreloadExcludes[i] == arg0) {
            if (i < g_fieldPreloadExcludeCount - 1) {
                g_fieldPreloadExcludes[i] = g_fieldPreloadExcludes[g_fieldPreloadExcludeCount - 1];
            }
            g_fieldPreloadExcludeCount--;
            return;
        }
    }
}

s32 *fld_addPreloadExclude(s32 arg0) {
    s32 *p = fld_findPreloadExclude(arg0);

    if (p != NULL) {
        return p;
    }
    if (g_fieldPreloadExcludeCount < 8) {
        p = &g_fieldPreloadExcludes[g_fieldPreloadExcludeCount];
        g_fieldPreloadExcludeCount++;
        *p = arg0;
        return p;
    }
    return NULL;
}

/* start streaming a file into the free area */
s32 fld_preloadFile(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 top;
    s32 size;
    PreloadEntry *p;

    if (arg0 == -1 || arg2 == 0) {
        return 0;
    }
    if (arg1 == 0 && arg3 == 0) {
        return 0;
    }
    top = fld_getModelCacheBottom(NULL);
    if ((u32)(D_80067944 + 0x153F00) < top) {
        top = (u32)(D_80067944 + 0x153F00);
    }
    size = top - g_fieldPreloadCursor;
    size = size / 2048 * 2048;
    if (arg1 == 0 && size < 0x8000) {
        return 0;
    }
    p = preload_add();
    if (p == NULL) {
        return 0;
    }
    preload_setEntry(p, arg1, arg0, arg1 == 0 ? g_fieldPreloadCursor : 0, size, arg3, arg2, arg3);
    g_fieldPreloadCursor += arg3;
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

    tbl = g_gameState->unk1C->depTable->unk4;
    key = fld_isEncounterPreloadExcluded();
    if (key == 0) {
        cnt = 0;
        key = fld_isEncounterPreloadRequested();
        if (key != 0) {
            cnt = 1;
        } else {
            for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                if (((tbl[i].unk0 >> 16) & 0xFF) == 0x11) {
                    cnt = 1;
                    break;
                }
            }
        }
        if (cnt != 0) {
            g = file_find(0, 0x1000B);
            fld_preloadFile(0x1000B, 0, g->unk4, (g->unkC - g->unk4) << 11);
            key = fld_isEncounterPreloadRequested();
            if (key != 0) {
                fld_removePreloadRequest(0x1000B);
            }
        }
    }
    if (fld_hasFieldPreloadRequest() != 0) {
        for (i = 0; i < g_fieldPreloadRequestCount; i++) {
            key = g_fieldPreloadRequests[i];
            if (((g_fieldPreloadRequests[i] >> 16) & 0xFF) == 5) {
                e = dep_find(g_gameState->unk1C->depTable, key);
                if (e != NULL) {
                    if ((e->unkC & 0xFFFF) != (s16)g_gameState->unk1C->unk3A) {
                        f = dep_find(g_gameState->unk1C->depTable, e->unkC);
                        if (f != NULL) {
                            fld_preloadFile(e->unkC & 0xFFFFFF, 0, f->unk4, f->unk8 << 11);
                        }
                    }
                    fld_preloadFile(key, 0, e->unk4, e->unk8 << 11);
                    fld_removePreloadRequest(key);
                }
            }
        }
    } else {
        best = 0;
        sel = -1;
        cur = -1;
        for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
            x = tbl[i].unk0;
            if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->unkA && fld_findPreloadExclude(x) == NULL &&
                    fld_queueFieldPreloads_grp(x & 0xFFFF) == fld_queueFieldPreloads_grp(g_gameState->unk1C->fieldId) && best < tbl[i].unk8) {
                    best = tbl[i].unk8;
                    sel = i;
                }
            }
        }
        if (sel == -1) {
            for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    if ((x & 0xFFFF) < 9000 && fld_findPreloadExclude(x) == NULL && (x & 0xFFFF) == g_gameState->unk1C->unk8E4.field->unkA) {
                        best = tbl[i].unk8;
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel == -1) {
            for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->unkA && fld_findPreloadExclude(x) == NULL &&
                        best < tbl[i].unk8) {
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel != -1) {
            if ((tbl[sel].unkC & 0xFFFF) != (s16)g_gameState->unk1C->unk3A) {
                for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                    if (tbl[i].unk0 == tbl[sel].unkC) {
                        fld_preloadFile(tbl[i].unk0 & 0xFFFFFF, 0, tbl[i].unk4, tbl[i].unk8 << 11);
                        break;
                    }
                }
            }
            cnt = tbl[sel].unk0 & 0xFFFFFF;
            fld_preloadFile(cnt, 0, tbl[sel].unk4, tbl[sel].unk8 << 11);
            cur = cnt;
        }
        if (cur != -1) {
            best = 0;
            sel = -1;
            for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    key = x & 0xFFFF;
                    if (key < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->unkA && (x & 0xFFFF) != (cur & 0xFFFF) &&
                        fld_findPreloadExclude(x) == NULL && fld_queueFieldPreloads_grp(key) == fld_queueFieldPreloads_grp(g_gameState->unk1C->fieldId) &&
                        best < tbl[i].unk8) {
                        best = tbl[i].unk8;
                        sel = i;
                        break;
                    }
                }
            }
            if (sel == -1) {
                for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                    x = tbl[i].unk0;
                    if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != (cur & 0xFFFF) && fld_findPreloadExclude(x) == NULL &&
                            (x & 0xFFFF) == g_gameState->unk1C->unk8E4.field->unkA) {
                            best = tbl[i].unk8;
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel == -1) {
                for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                    x = tbl[i].unk0;
                    if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->unkA && (x & 0xFFFF) != (cur & 0xFFFF) &&
                            fld_findPreloadExclude(x) == NULL && best < tbl[i].unk8) {
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel != -1) {
                if ((tbl[sel].unkC & 0xFFFF) != (s16)g_gameState->unk1C->unk3A) {
                    for (i = g_gameState->unk1C->depTable->unk2 - 1; i >= 0; i--) {
                        if (tbl[i].unk0 == tbl[sel].unkC) {
                            fld_preloadFile(tbl[i].unk0 & 0xFFFFFF, 1, tbl[i].unk4, -1);
                            return;
                        }
                    }
                } else {
                    fld_preloadFile(tbl[sel].unk0 & 0xFFFFFF, 1, tbl[sel].unk4, -1);
                }
            }
        }
    }
}

s32 fld_preloadHook(void) {
    FieldScene *s;

    if ((u32)(g_gameState->unk1C->unk851 - 1) < 5) {
        return 1;
    }
    s = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    if (s->unk0 & 0x800000) {
        if (preload_poll() <= 0) {
            preload_reset();
            if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
                fld_setPreloadCursor(g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop);
                fld_queueFieldPreloads();
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100000;
            }
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x800000;
        }
        return 1;
    }
    if (s->unk0 & 0x100000) {
        return 0;
    }
    if (s->unk0 & 0x10000) {
        fld_setPreloadCursor(s->heapTop);
        fld_queueFieldPreloads();
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100000;
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

    tim = (s32)db_lookupRegistered(4, 0x44E20, 0);
    file_getTimRects((void *)tim, &clut, &img);
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    file_loadTim(tim);
    DrawSync(0);
    n = 0;
    state = 0;
    i = 1;
    p = g_gameState->unk1C->unk8E4.field->unkC->unk14->heapTop;
    do {
        {
            /* dead struct initializer: it emits only a CLOBBER of q, which sched2 places in one of
               the loop's load-delay gaps (so p += 20, the tpage constant and &tp[0] land one gap
               later, as in the original), and q's spill slot is the frame's otherwise unused word */
            struct { s32 x; } q = { i };
        }
        spr[i] = (SPRT *)p;
        p += sizeof(SPRT);
        setlen(spr[i], 4);
        setcode(spr[i], 0x64);
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
        setlen(tp[i], 1);
        tp[i]->code[0] = _get_mode(1, 1, getTPage(0, 0, img->x, img->y));
        i--;
    } while (i >= 0);
    fld_invalidateModelCache((s32)p);
    preload_reset();
    game_setClearColor(0, 0, 0);
    do {
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
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
        addPrim((u32 *)g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], spr[g_gameState->unk1C->unk8]);
        addPrim((u32 *)g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], tp[g_gameState->unk1C->unk8]);
        game_updateFade();
        DrawSync(0);
        g_gameState->vsyncMode = 2;
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->dispEnv[g_gameState->unk1C->unk8]);
        DrawOTagEnv(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 0xFFF, &g_gameState->drawEnv[g_gameState->unk1C->unk8]);
        g_gameState->unk1C->unk8 ^= 1;
    } while (state < 4);
}

s32 fld_needsSplashImage(void) {
    GameData *gd = g_gameState->unk1C;
    FieldScene *sc = gd->unk8E4.field->unkC->unk14;

    if (sc->exitReason == FLD_EXIT_FIELD && (s16)gd->fieldId == 0x34 && sc->nextId == 100) {
        return 1;
    }
    return 0;
}

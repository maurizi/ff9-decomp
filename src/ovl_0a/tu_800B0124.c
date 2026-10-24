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
        id = (arg1 & 0xFFFF) | DB_MUSIC;
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
                id = p->unk7E8 | DB_MUSIC;
                t = p->unk840 | DB_SOUND_BANK;
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
                if (g_gameState->unk1C->unk0 & GD_MUSIC_PLAYING) {
                    g_gameState->unk1C->unk0 |= GD_MUSIC_STOP_PENDING;
                }
                g_gameState->unk1C->unk0 &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                SCENE->flags &= ~FLD_SCN_BANK_SLOT;
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
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            if (!(g_gameState->unk1C->unk0 & GD_MUSIC_PLAYING)) {
                func_80055A34(g_gameState->unk1C->unk824[0], 0xA, 0, g_gameState->unk1C->unk826[0]);
            }
            g_gameState->unk1C->unk0 |= (GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
        done:
            g_fieldMusicRequest = -1;
            return 0;
        case 12:
            if (g_gameState->unk1C->unk851 != 0) {
                goto state;
            }
            return fld_isMusicFading() ? 6 : 0;
        case 8:
            func_80055310();
            g_gameState->unk1C->unk7E8 = g_gameState->unk1C->unk81C[0];
            g_gameState->unk1C->unk840 = g_gameState->unk1C->unk820[0];
            g_gameState->unk1C->unk850 = g_gameState->unk1C->unk826[0];
            g_gameState->unk1C->unk7EA = g_gameState->unk1C->fieldId;
            g_gameState->unk1C->unk8AC = g_gameState->unk1C->unk8A8;
            g_gameState->unk1C->unk8B0 = g_gameState->unk1C->unk84A;
        case 1:
            if ((arg0 & 0x3F) == 1) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= ~GD_MUSIC_PLAYING;
                return 0;
            }
            func_800552B8(g_gameState->unk1C->unk824[0]);
            g_gameState->unk1C->unk826[0] = 0x7F;
            g_gameState->unk1C->unk0 &= ~GD_MUSIC_PLAYING;
            return 0;
        case 2:
            if ((arg0 & 0x3F) == 8) {
                return 0;
            }
            switch (arg0 & 0x3F) {
            case 1 ... 3:
                switch (arg0 & 0x3F) {
                case 1:
                    func_800559C8(g_gameState->unk1C->unk824[0], arg2);
                    g_gameState->unk1C->unk826[0] = arg2;
                    return 0;
                case 2:
                    func_80055B6C(arg2);
                    return 0;
                case 3:
                    func_80055C08(arg2);
                    return 0;
                case 4:
                    func_80055CA4(arg2);
                    return 0;
                }
                break;
            case 7:
                g_gameState->unk1C->unk824[0] = func_800553BC(g_gameState->unk1C->unk814, arg2);
                g_gameState->unk1C->unk0 |= GD_MUSIC_PLAYING;
                return 0;
            }
            break;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800559FC(g_gameState->unk1C->unk824[0], arg2, arg3);
                fld_setMusicVolumeFade(arg3, arg2);
                return 0;
            case 2:
                func_80055B98(arg2, arg3);
                return 0;
            case 3:
                func_80055C34(arg2, arg3);
                return 0;
            case 4:
                func_80055CD0(arg2, arg3);
                return 0;
            }
            break;
        case 4:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
                fld_setMusicVolumeFade(arg4, arg2);
                return 0;
            case 2:
                func_80055BCC(arg2, arg3, arg4);
                return 0;
            case 3:
                func_80055C68(arg2, arg3, arg4);
                return 0;
            case 4:
                func_80055D04(arg2, arg3, arg4);
                return 0;
            }
            break;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            fld_setMusicVolumeFade(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            fld_setMusicVolumeFade(arg4, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            func_80055604(arg1);
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
        id = (arg1 & 0xFFFF) | DB_SE;
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
                mask = GD_SFX0_PLAYING << (slot * 2);
                mask2 = mask >> 1;
                if (kind == 0 && g_gameState->unk1C->unk830[slot] != arg1) {
                    if (g_gameState->unk1C->unk0 & mask) {
                        func_8005546C(SFXH(slot), arg2);
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
                func_800553EC(h, arg2, arg3, arg4);
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
                if (g_gameState->unk1C->unk0 & GD_SFX0_PLAYING) {
                    func_8005546C((s32)g_gameState->unk1C->unk828, 0x100000);
                }
                if (g_gameState->unk1C->unk0 & GD_SFX1_PLAYING) {
                    func_8005546C((s32)g_gameState->unk1C->unk82C, 0x200000);
                }
                g_gameState->unk1C->unk0 &= ~(GD_SFX0_PLAYING | GD_SFX1_PLAYING);
            } else {
                func_8005546C(h, arg2);
                if (fp) {
                    g_gameState->unk1C->unk0 &= ~mask;
                }
            }
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8B8[slot] = arg3;
                }
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8BA[slot] = arg3;
                }
                return 0;
            case 3:
                func_8005593C(h, arg2, arg3);
                return 0;
            case 8:
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8((void *)h, arg2, arg3, arg4);
                if (fp) {
                    g_gameState->unk1C->unk8B8[slot] = arg4;
                }
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                if (fp) {
                    g_gameState->unk1C->unk8BA[slot] = arg4;
                }
                return 0;
            case 3:
                func_80055980(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                if (fp) {
                    g_gameState->unk1C->unk8B8[0] = g_gameState->unk1C->unk8B8[1] = arg2;
                }
                return 0;
            case 5:
                func_800557F0(arg2);
                if (fp) {
                    g_gameState->unk1C->unk8BA[0] = g_gameState->unk1C->unk8BA[1] = arg2;
                }
                return 0;
            case 3:
                func_800558DC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8B8[0] = g_gameState->unk1C->unk8B8[1] = arg3;
                }
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                if (fp) {
                    g_gameState->unk1C->unk8BA[0] = g_gameState->unk1C->unk8BA[1] = arg3;
                }
                return -1;
            case 3:
                func_80055908(arg2, arg3);
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
            func_80056248(fld_isMoviePlaying() ? SCENE->movieSndVolume : SCENE->sndVolume);
            func_800561C4(slot, arg2, arg3);
            SCENE->unk5F = 0;
            return 0;
        case 1:
            func_80056228();
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                SCENE->sndVolume = arg2;
                if (fld_isMoviePlaying() == 0) {
                    func_80056248(arg2);
                }
                return 0;
            case 10:
                SCENE->movieSndVolume = arg2;
                if (fld_isMoviePlaying() != 0) {
                    func_80056248(arg2);
                }
                return 0;
            case 5:
                func_800562B0(arg2);
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
        id = (arg1 & 0xFFFF) | DB_SOUND_BANK;
        switch (kind) {
        case 7:
            SCENE->flags &= ~FLD_SCN_BANK_SLOT;
            if (arg2 == 4) {
                SCENE->soundBankSlot = arg3;
                SCENE->flags |= FLD_SCN_BANK_SLOT;
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
            if (g_gameState->unk1C->unk0 & GD_MUSIC_STOP_PENDING) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk0 &= ~GD_MUSIC_STOP_PENDING;
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
        if (!(g->unk8E4.field->area->scene->flags & FLD_SCN_BANK_SLOT) && g->unk852 != 0x84) {
            func_80055DF4();
        }
        g_gameState->unk1C->unk8C0 = SCENE->heapTop ? SCENE->heapTop : D_80067948;
        g_gameState->unk1C->unk878 = 0x156F00 - (g_gameState->unk1C->unk8C0 - (s32)D_80067944);
        if (!(g_gameState->unk1C->unk8E4.field->flags & 4)) {
            g_gameState->unk1C->unk878 -= 0x3000;
        }
        m = g_gameState->unk1C->unk878;
        g_gameState->unk1C->unk878 = m / 2048 * 2048;
        g_gameState->unk1C->unk87C = 0;
        func_8002177C(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, sz, (void *)g_gameState->unk1C->unk8C0, 0,
                      g_gameState->unk1C->unk878, fld_soundBankReadCallback);
        g_gameState->unk1C->unk851 = 3;
        break;
    case 3:
        if (func_800217C4(g_gameState->unk1C->unk854) == 0 && func_80055DCC() == 0) {
            fld_invalidateModelCache((s32)g_gameState->unk1C->unk854[0].unk20);
            func_80022280((s32)g_gameState->unk1C->unk854[0].unk8, g_gameState->unk1C->unk854[0].unk10);
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk87C) {
                g_gameState->unk1C->unk8C0 = (s32)g_gameState->unk1C->unk854[0].unk8;
                d = 0x156F00 - ((s32)g_gameState->unk1C->unk854[0].unk8 - (s32)D_80067944);
                if (!(g_gameState->unk1C->unk8E4.field->flags & 4)) {
                    d -= 0x3000;
                }
                d = d / 2048 * 2048;
                func_8002177C(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
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
        func_8002177C(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
                      g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10, t, NULL);
        g_gameState->unk1C->unk851 = 5;
        break;
    case 5:
        if (func_800217C4(g_gameState->unk1C->unk854) == 0) {
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk854[0].unk4) {
                m = g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 - g_gameState->unk1C->unk87C;
                g_gameState->unk1C->unk8C0 = (s32)((u8 *)g_gameState->unk1C->unk814 + m);
                func_8002177C(g_gameState->unk1C->unk854, g_fieldSoundLoadSector, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0, m, 0x6800 - m, NULL);
            } else {
                fld_unpackMusicSeq((s32)g_gameState->unk1C->unk814);
                g_gameState->unk1C->unk0 |= GD_MUSIC_LOADED;
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
    z = z / 4 + g_gameState->unk1C->unk8E4.field->area->scene->otBias;
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

    for (i = db_lookup(t, 1, DB_SE, 0) - 1; i >= 0; i--) {
        func_8005546C(db_lookup(t, 2, (i & 0xFFFF) | DB_SE, 0), 0);
    }
    func_80056228();
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

    base = g_gameState->unk1C->unk8E4.field->area->scene->animSfx;
    n = g_gameState->unk1C->unk8E4.field->area->scene->animSfxCount;
    for (i = arg1 - 1; i >= 0; i--) {
        ctx = arg0[i];
        if (ctx == NULL || (ctx->flags & FLD_OBJ_NO_ANIM_SFX)) {
            continue;
        }
        for (j = n - 1; j >= 0; j--) {
            t = &base[j];
            if (t->uid == ctx->obj->uid && t->anim == ctx->obj->anim && t->frame == ctx->obj->animFrame) {
                r = 0;
                a = 0;
                b = 0;
                c = 0;
                vol = 0x80;
                pan = 0x7F;
                bgi_getActorTriangle(ctx->obj->uid, (u16 *)&a, (u16 *)&b);
                if (a != -1) {
                    bgi_getTriangleFlags(a, &c);
                    c = (s16)c >> 8;
                }
                ch = t->uid & 3;
                ch = (0x10000 << ch) & 0xF0000;
                snd = t->sound[(c >> 4) & 1] | DB_SE;
                if (t->randomPitch != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                    v = *g_randPtr++;
                    r = (v >> 4) - 8;
                }
                fld_getSoundPanVolume(ctx->obj->pos.vx, ctx->obj->pos.vy, ctx->obj->pos.vz, &vol, &pan);
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
    FieldScene *sc = g_gameState->unk1C->unk8E4.field->area->scene;
    FieldAnimSfx *base = sc->animSfx;
    FieldAnimSfx *e = &base[sc->animSfxCount] - 1;

    do {
        if (e->uid == arg0->obj->uid && e->anim == arg1 && e->frame == arg2) {
            return e;
        }
        e--;
    } while (e >= base);
    return NULL;
}

FieldAnimSfx *fld_addAnimSfx(FieldEvtCtx *arg0, s32 arg1, s32 arg2) {
    FieldScene *sc = g_gameState->unk1C->unk8E4.field->area->scene;
    u8 i = sc->animSfxCount;
    FieldAnimSfx *e;
    u16 id;
    s32 n;

    n = i + 1;
    sc->animSfxCount = n;
    e = &sc->animSfx[i];
    e->uid = arg0->obj->uid;
    id = arg0->obj->unk34;
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

    base = g_gameState->unk1C->unk8E4.field->area->scene->animSfx;
    e = &base[g_gameState->unk1C->unk8E4.field->area->scene->animSfxCount] - 1;
    last = e;
    do {
        if (e->uid == arg0->obj->uid && e->anim == arg1 && e->frame == arg2) {
            if (e != last) {
                *e = *last;
            }
            g_gameState->unk1C->unk8E4.field->area->scene->animSfxCount--;
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

    o = g_gameState->unk1C->unk8E4.field->area->scene;
    o->battleMusic = g_gameState->unk1C->unk81C[0];
    key = (u16)o->nextId;
    g_gameState->unk1C->unk8E4.field->area->scene->battleMusicBank = g_gameState->unk1C->unk820[0];
    base = g_gameState->unk1C->depTable->unk4;
    key |= 0x110000;
    w = key;
    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
        id = base[i].unkC;
        if (base[i].unk0 == w && ((id >> 16) & 0xFF) == 7) {
            g_gameState->unk1C->unk8E4.field->area->scene->battleMusic = id;
            for (j = 0; j < g_gameState->unk1C->depTable->unk2; j++) {
                w = base[j].unkC;
                v = base[j].unk0;
                if (((base[j].unk0 >> 16) & 0xFF) == 9 && w == id) {
                    g_gameState->unk1C->unk8E4.field->area->scene->battleMusicBank = v;
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
    id = g_gameState->unk1C->unk852 | DB_SOUND_BANK;
    if (arg0->unkC == 0) {
        b = a;
        t = db_lookup(a, 2, DB_PACK, 0);
        if (t == -1) {
            a = db_lookup(a, 4, id, 0);
            g_gameState->unk1C->unk87C = func_8001CA70(b);
        } else {
            a = db_lookup(t, 4, id, 0);
            g_gameState->unk1C->unk87C = db_lookup(b, 2, (DB_PACK | 1), 0) - g_gameState->unk1C->unk8C0;
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
        sc = gd->unk8E4.field->area->scene;
        if (sc->flags & 0x10) {
            r = func_80056048(a, sc->soundBankSlot, 0);
        } else {
            r = func_80055E14(a, n, 0);
        }
    }
    arg0->unk8 = a;
    return r;
}

s32 fld_unpackMusicSeq(s32 arg0) {
    s32 t = db_lookup(arg0, 3, DB_MUSIC, 0);

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
    fld_setSlotColor((FieldEvtCtx *)func_8001E350(arg0), arg1, arg2, arg3);
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

    sc = g_gameState->unk1C->unk8E4.field->area->scene;
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
        info = &infos[c->obj->uid];
        l = (Unk800B3294 *)fld_findLightColor(lt, c->modelId);
        shade = c->flags & FLD_OBJ_DIR_LIGHT;
        fl = fld_findFloorLight(lt, c, &floor);
        if ((u8)info->litFloor != (u8)floor || shade != (info->flags & 4) || !(c->flags & FLD_OBJ_LIT)) {
            chg = 1;
        }
        if (chg) {
            if (!(c->flags & FLD_OBJ_COLOR_SET)) {
                b = 0;
                if (l != NULL) {
                    a = l->unk4;
                } else {
                    a = def->unk4;
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
            g_gameState->unk1C->unk8E4.field->area->scene->shadows[c->obj->uid].params.shade = dx;
            g_gameState->unk1C->unk8E4.field->area->scene->shadows[c->obj->uid].params.scaleX = dy * 14;
            g_gameState->unk1C->unk8E4.field->area->scene->shadows[c->obj->uid].params.scaleZ = dy * 12;
        }
        info->litFloor = floor;
        info->flags = (info->flags & ~4) | shade;
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        inf2 = &infos[c->obj->uid];
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
s32 fld_setSlotColor(FieldEvtCtx *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 f = arg0->flags;
    FieldActorInfo *info = &g_gameState->unk1C->unk8E4.field->area->scene->actorInfo[arg0->obj->uid];
    u32 col;

    if (!(f & 0x800)) {
        col = (arg1 & 0xFF) | ((arg2 << 8) & 0xFF00) | ((arg3 << 16) & 0xFF0000) | 0x01000000;
        info->nextColor.w = col;
        info->color.w = col;
        arg0->flags |= 0xC00;
        return 1;
    }
    arg0->flags = f & ~0x800;
    return 0;
}

FieldLight *fld_getLightColorTable(Unk800B3254 *arg0) {
    if (arg0 != NULL && arg0->colorCount != 0) {
        return &arg0->lights[arg0->colorOffset];
    }
    return NULL;
}

Unk800B3294 *fld_findLightColor(Unk800B3254 *arg0, u16 arg1) {
    Unk800B3294 *base;
    Unk800B3294 *p;
    s32 id;
    u32 v;

    v = arg1;
    if (arg0->colorCount == 0) {
        return NULL;
    }
    id = v & 0xFFFF;
    base = (Unk800B3294 *)&arg0->lights[arg0->colorOffset];
    p = &base[arg0->colorCount] - 1;
    do {
        if (p->modelId == id) {
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
    base = arg0->lights;
    if (arg0->lightCount == 0) {
        return NULL;
    }
    p = &arg0->lights[arg0->lightCount - 1];
    do {
        if (p->type == 1) {
            *arg1 = p;
        } else if (p->type == 2) {
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

    bgi_getActorTriangle(arg1->obj->uid, &tri, (u16 *)arg2);
    if (arg0->lightCount != 0) {
        if ((u8)*arg2 == 0xFF) {
            return NULL;
        }
        p = &arg0->lights[arg0->lightCount - 1];
        do {
            if (p->type == 0) {
                v = p->floors;
                for (n = 3; n >= 0; n--) {
                    if ((v & 0xFF) == *arg2) {
                        return p;
                    }
                    v >>= 8;
                }
            }
            p--;
        } while (p >= arg0->lights);
    }
    return NULL;
}

s32 func_800B3444(s32 arg0, s32 arg1, s32 arg2) {
    s32 start;
    s32 size;
    s32 end;

    if (arg0 == 0 && arg1 == 0) {
        if (arg2 != 0) {
            if (!(g_gameState->unk1C->unk8E4.field->area->scene->flags & FLD_SCN_OBJFX_ALLOC)) {
                start = g_gameState->unk1C->unk8E4.field->area->scene->heapTop;
                if (start == 0) {
                    start = D_80067948;
                }
                if (func_800221C0(start, 0x5000) != 0) {
                    func_80022390(func_8002208C()->unk4);
                }
                if (g_gameState->unk1C->unk8E4.field->area->scene->heapTop != 0) {
                    start = g_gameState->unk1C->unk8E4.field->area->scene->heapTop;
                    g_gameState->unk1C->unk8E4.field->area->scene->heapTop = func_80050BE0(start);
                    size = g_gameState->unk1C->unk8E4.field->area->scene->heapTop - start;
                } else {
                    start = D_80067948;
                    end = func_80050BE0(start);
                    size = end - start;
                    D_80067948 = end;
                }
                fld_trimModelCache();
                func_80022280(start, size);
                g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_OBJFX_ALLOC;
            }
            g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_OBJFX_ON;
        } else {
            g_gameState->unk1C->unk8E4.field->area->scene->flags &= ~FLD_SCN_OBJFX_ON;
        }
    }
    return 0;
}

void func_800B3684(void) {
    if (g_gameState->unk1C->unk8E4.field->area->scene->flags & FLD_SCN_OBJFX_ON) {
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
        if (!(g_gameState->unk1C->unk8E4.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
            goto done;
        }
        p = func_80021E20();
        if (p != NULL && p->fileId != k) {
            func_80021C20(p);
        }
        if (flag || preload_find(k) != NULL) {
            goto set;
        }
        goto clear;
    case 5:
        id = (u16)arg1 | DB_FIELD;
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
            if (func_80021A1C(e)) {
                return -1;
            }
            if (!func_80021A1C(e) && (e->unk1C >> 2) < e->unkC) {
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
        if (!(g_gameState->unk1C->unk8E4.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
            goto done;
        }
        e = func_80021E20();
        if (e != NULL && e->fileId != id && func_80021A1C(e)) {
            func_80021C20(e);
        }
        q = preload_find(id);
        if (!flag && (q == NULL || q->unk2 != 0)) {
        clear:
            g_gameState->unk1C->unk8E4.field->area->scene->flags &= ~FLD_SCN_PRELOADS_QUEUED;
        }
    set:
        g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_PRELOAD_RESTART;
    done:
        g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_PRELOAD_REQUESTED;
        return 0;
    }
}

void fld_initPreload(void) {
    preload_init();
    g_gameState->unk1C->unk8BC = fld_preloadRequest;
    func_80021CDC((s32)fld_preloadHook);
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
    func_80021974(p, arg1, arg0, arg1 == 0 ? g_fieldPreloadCursor : 0, size, arg3, arg2, arg3);
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
                if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
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
                    if ((x & 0xFFFF) < 9000 && fld_findPreloadExclude(x) == NULL && (x & 0xFFFF) == g_gameState->unk1C->unk8E4.field->prevFieldId) {
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
                    if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->prevFieldId && fld_findPreloadExclude(x) == NULL &&
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
                    if (key < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
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
                            (x & 0xFFFF) == g_gameState->unk1C->unk8E4.field->prevFieldId) {
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
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != g_gameState->unk1C->unk8E4.field->prevFieldId && (x & 0xFFFF) != (cur & 0xFFFF) &&
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
    s = g_gameState->unk1C->unk8E4.field->area->scene;
    if (s->flags & 0x800000) {
        if (func_80021FB4() <= 0) {
            func_800223CC();
            if (!(g_gameState->unk1C->unk8E4.field->area->scene->flags & FLD_SCN_PRELOADS_QUEUED)) {
                fld_setPreloadCursor(g_gameState->unk1C->unk8E4.field->area->scene->heapTop);
                fld_queueFieldPreloads();
                g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_PRELOADS_QUEUED;
            }
            g_gameState->unk1C->unk8E4.field->area->scene->flags &= ~FLD_SCN_PRELOAD_RESTART;
        }
        return 1;
    }
    if (s->flags & 0x100000) {
        return 0;
    }
    if (s->flags & 0x10000) {
        fld_setPreloadCursor(s->heapTop);
        fld_queueFieldPreloads();
        g_gameState->unk1C->unk8E4.field->area->scene->flags |= FLD_SCN_PRELOADS_QUEUED;
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
    func_8001D910((void *)tim, &clut, &img);
    setRECT(img, 0, 0x100, img->w, img->h);
    setRECT(clut, 0x100, 0xE0, clut->w, clut->h);
    func_8001D8A8(tim);
    DrawSync(0);
    n = 0;
    state = 0;
    i = 1;
    p = g_gameState->unk1C->unk8E4.field->area->scene->heapTop;
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
    func_800223CC();
    func_8001CCB4(0, 0, 0);
    do {
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
        switch (state) {
        case 0:
            n = 0x3C;
            i = 0;
            func_800320B8(1, 1, 0, 0xFFFFFF);
            func_800320B8(1, 1, n, i);
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
                func_800320B8(1, 1, n, 0xFFFFFF);
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
        func_80032098();
        DrawSync(0);
        g_gameState->vsyncMode = 2;
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
        DrawOTagEnv(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 0xFFF, &g_gameState->unk10[g_gameState->unk1C->unk8]);
        g_gameState->unk1C->unk8 ^= 1;
    } while (state < 4);
}

s32 fld_needsSplashImage(void) {
    GameData *gd = g_gameState->unk1C;
    FieldScene *sc = gd->unk8E4.field->area->scene;

    if (sc->exitReason == FLD_EXIT_FIELD && (s16)gd->fieldId == 0x34 && sc->nextId == 100) {
        return 1;
    }
    return 0;
}

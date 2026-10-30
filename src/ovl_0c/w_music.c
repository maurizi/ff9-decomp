#include "ovl_0c.h"

void w_musicUpdate(void) {
    s32 st;

    switch (g_worldMusicState) {
    case 1:
        if (g_worldCdUser == WM_CD_FREE) {
            if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
                printf("w_musicUpdate : Play\n");
            }
            g_worldCdUser = WM_CD_MUSIC;
            if (g_worldMusicSong != -1) {
                wm_musicStop(g_worldMusicSong);
            }
            wm_musicPlay(g_worldMusicRequest);
            g_worldMusicState = WM_MUSIC_WAITING;
            g_worldMusicSong = g_worldMusicRequest;
        }
        break;
    case 2:
        st = wm_soundCommand(SND_GET_STATE, 0, 0, 0, 0);
        if (st == 0) {
            g_worldCdUser = WM_CD_FREE;
            g_worldMusicState = WM_MUSIC_READY;
            if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
                printf("w_musicUpdate : Ready\n");
            }
        } else if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
            printf("w_musicUpdate : Waiting [ State : %d ] \n", st);
        }
        break;
    }
}

/* sound effect ids of world-map sound arg0 */
s32 w_musicGetID(s32 arg0, WmSeIds *arg1) {
    WmSeIds s;

    s.seIds[0] = -1;
    switch (arg0) {
    case 0:
        s.seIds[0] = 0x68;
        break;
    case 1:
        s.seIds[0] = 0x66;
        break;
    case 2:
        s.seIds[0] = 0x6C;
        break;
    case 3:
        s.seIds[0] = 0x27D;
        break;
    case 4:
        s.seIds[0] = 0x27E;
        break;
    case 5:
        s.seIds[0] = 0x2AA;
        break;
    case 39:
        s.seIds[0] = 0x67;
        break;
    }
    if (s.seIds[0] != -1) {
        s.channelMask = 0x800000;
        s.count = 1;
        *arg1 = s;
        return 1;
    } else {
        switch (arg0) {
        case 30:
            s.seIds[0] = 0x44B;
            break;
        case 31:
            s.seIds[0] = 0x4CF;
            break;
        case 32:
            s.seIds[0] = 0x596;
            break;
        case 34:
            s.seIds[0] = 0x44C;
            break;
        case 35:
            s.seIds[0] = 0x37C;
            break;
        case 36:
            s.seIds[0] = 0x44A;
            break;
        case 37:
            s.seIds[0] = 0x37A;
            break;
        case 38:
            s.seIds[0] = 0x5DB;
            break;
        case 40:
            s.seIds[0] = 0x37B;
            break;
        case 41:
            s.seIds[0] = 0xA47;
            break;
        case 42:
            s.seIds[0] = 0xC26;
            break;
        }
        if (s.seIds[0] != -1) {
            s.channelMask = 0;
            s.count = 1;
            *arg1 = s;
            return 1;
        } else {
            switch (arg0) {
            case 6:
                s.seIds[0] = 0x552;
                break;
            case 7:
                s.seIds[0] = 0x553;
                break;
            }
            if (s.seIds[0] != -1) {
                s.channelMask = 0x8000;
                s.count = 1;
                *arg1 = s;
                return 1;
            } else {
                /* the original third jump table is 8-aligned */
                __asm__(".rdata\n\t.word 0\n\t.text");
                switch (arg0) {
                case 33:
                    s.seIds[0] = 0x37A;
                    s.channelMask = 0;
                    s.seIds[1] = 0x37B;
                    break;
                case 20:
                    s.channelMask = 0x8000;
                    s.seIds[0] = 0x374;
                    s.seIds[1] = 0x375;
                    break;
                case 21:
                    s.channelMask = 0x8000;
                    s.seIds[0] = 0x376;
                    s.seIds[1] = 0x377;
                    break;
                case 22:
                    s.channelMask = 0x8000;
                    s.seIds[0] = 0x378;
                    s.seIds[1] = 0x379;
                    break;
                case 23:
                    s.channelMask = 0x8000;
                    s.seIds[0] = 0x554;
                    s.seIds[1] = 0x555;
                    break;
                case 24:
                    s.channelMask = 0x8000;
                    s.seIds[0] = 0x5D9;
                    s.seIds[1] = 0x5DA;
                    break;
                case 25:
                    s.channelMask = 0x800;
                    s.seIds[0] = 0x37D;
                    s.seIds[1] = 0x37E;
                    break;
                case 26:
                    s.channelMask = 0x800;
                    s.seIds[0] = 0x37F;
                    s.seIds[1] = 0x380;
                    break;
                }
                if (s.seIds[0] != -1) {
                    s.count = 2;
                    *arg1 = s;
                    return 1;
                }
                printf("w_musicGetID : Illigal SE Number #%d\n", arg0);
                return 0;
                printf(""); /* unreachable; its empty string is in .rodata */
            }
        }
    }
}

const char D_800A832C[] = "w_musicRequestSend : Normal Song";
const char D_800A8350[] = "w_musicRequestSend : Air Ship Song";
const char D_800A8374[] = "w_musicRequestSend : Chocobo Song";
const char D_800A8398[] = "w_musicRequestSend : Disc4 Song";
const char D_800A83B8[] = "w_musicRequestSend : Field Song 1";
const char D_800A83DC[] = "w_musicRequestSend : Field Song 2";
const char D_800A8400[] = "w_musicRequestSend : Unknown Error";
const char D_800A8424[] = "(Accepted)\n";
const char D_800A8430[] = "(Denied / same request)\n";
/* padded: the jump table that follows is 8-aligned in its object */
const char D_800A844C[0x38] = "w_musicRequestSend : music status is not ready now\n";

void w_musicRequestSend(u32 arg0) {
    if (g_worldMusicState == WM_MUSIC_READY) {
        if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
            switch (arg0) {
            case WM_SONG_NORMAL:
                printf(D_800A832C);
                break;
            case WM_SONG_AIRSHIP:
                printf(D_800A8350);
                break;
            case WM_SONG_CHOCOBO:
                printf(D_800A8374);
                break;
            case WM_SONG_DISC4:
                printf(D_800A8398);
                break;
            case WM_SONG_FIELD1:
                printf(D_800A83B8);
                break;
            case WM_SONG_FIELD2:
                printf(D_800A83DC);
                break;
            default:
                printf(D_800A8400);
                break;
            }
        }
        if (g_worldMusicRequest != arg0) {
            g_worldMusicState = WM_MUSIC_REQUESTED;
            g_worldMusicRequest = arg0;
            if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
                printf(D_800A8424);
            }
        } else if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
            printf(D_800A8430);
        }
    } else if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
        printf(D_800A844C);
    }
}

void wm_stopAllSe(void) {
    WmSeIds s;
    s32 i;
    s32 j0;
    s32 m0;
    s32 j1;
    s32 m1;
    s32 j2;
    s32 m2;

    for (i = 0; i < 7; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m0 = s.channelMask;
            for (j0 = 0; j0 < s.count; j0++) {
                wm_soundCommand(SND_SE_STOP, s.seIds[j0], m0, 0, 0);
                m0 >>= 1;
            }
        }
    }
    for (i = 20; i < 26; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m1 = s.channelMask;
            for (j1 = 0; j1 < s.count; j1++) {
                wm_soundCommand(SND_SE_STOP, s.seIds[j1], m1, 0, 0);
                m1 >>= 1;
            }
        }
    }
    for (i = 30; i < 42; i++) {
        if (w_musicGetID(i, &s) & 0xFF) {
            m2 = s.channelMask;
            for (j2 = 0; j2 < s.count; j2++) {
                wm_soundCommand(SND_SE_STOP, s.seIds[j2], m2, 0, 0);
                m2 >>= 1;
            }
        }
    }
    if ((u8)wm_isMapSongPlaying() && g_worldEventResult != WM_EVT_BATTLE) {
        g_gameState->gameData->unk0 |= 0x400000;
    }
}

s32 func_800C45D8(void) {
    u16 sc = g_evtGlobalVars.h[0];
    u8 v = g_evtGlobalVars.b[0xBE];

    if (sc - 0x2580U < 0x122) {
        return 1;
    }
    if (sc - 0x26A2U < 0x14) {
        return 1;
    }
    return v - 8U < 2;
}

void wm_musicInit(void) {
    g_worldMusicState = WM_MUSIC_READY;
    g_worldMusicId = -1;
    g_worldMusicSong = -1;
    g_worldMusicRequest = -1;
    g_worldMusicStarted = 0;
}

void wm_musicSetup(void) {
}

void wm_musicTerm(void) {
}

void wm_musicPlay(u16 arg0) {
    u8 v = g_evtGlobalVars.b[0x68];

    g_worldMusicId = wm_getSongMusicId(arg0);
    g_worldMusicStarted = 1;
    wm_soundCommand(0, g_worldMusicId, 0, 0, 0);
    if (v == 0) {
        v = 0x7D;
    }
    wm_soundCommand(SND_MUSIC_VOL_INTPL, 0, 0x3C, v, 0);
}

void wm_musicStop(u16 arg0) {
    wm_soundCommand(0x100, wm_getSongMusicId(arg0), 0, 0, 0);
}

void wm_musicFade(void) {
}

void wm_sePlay(s32 arg0, u8 arg1) {
    WmSeIds s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.channelMask;
        for (i = 0; i < s.count; i++) {
            wm_soundCommand(SND_SE_PLAY, s.seIds[i], v, 0x7F, arg1);
            v >>= 1;
        }
    }
}

void wm_seSetVolume(s32 arg0, u8 arg1) {
    WmSeIds s;
    s32 i;
    s32 v;

    if (g_worldMusicState == WM_MUSIC_READY && (w_musicGetID(arg0, &s) & 0xFF)) {
        v = s.channelMask;
        for (i = 0; i < s.count; i++) {
            wm_soundCommand(SND_SE_VOL, s.seIds[i], v, arg1, 0);
            v >>= 1;
        }
    }
}

void wm_seFadeVolume(s32 arg0, u16 arg1, u8 arg2) {
    WmSeIds s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.channelMask;
        for (i = 0; i < s.count; i++) {
            wm_soundCommand(SND_SE_VOL_INTPL, s.seIds[i], v, arg1, arg2);
            v >>= 1;
        }
    }
}

void wm_seStop(s32 arg0) {
    WmSeIds s;
    s32 i;
    s32 v;

    if (w_musicGetID(arg0, &s) & 0xFF) {
        v = s.channelMask;
        for (i = 0; i < s.count; i++) {
            wm_soundCommand(SND_SE_STOP, s.seIds[i], v, 0, 0);
            v >>= 1;
        }
    }
}

s32 wm_getSongMusicId(u16 arg0) {
    u16 sc;

    if (arg0 == WM_SONG_NORMAL) {
        sc = *g_worldStoryCounterPtr;
        if (sc - 0x2580U < 0x122) {
            arg0 = WM_SONG_FIELD1;
        }
        if (sc - 0x26A2U < 0x14) {
            arg0 = WM_SONG_FIELD2;
        }
        if (g_worldDataSet == WM_DATASET_DISC4) {
            arg0 = WM_SONG_DISC4;
        }
    }
    return g_worldSongMusicIds[arg0];
}

s32 wm_isMapSongPlaying(void) {
    u8 r = g_worldMusicId == g_worldSongMusicIds[0];

    if (g_worldMusicId == g_worldSongMusicIds[1]) {
        r = 1;
    }
    if (g_worldMoveMode == 0) {
        if (g_worldMusicId == g_worldSongMusicIds[3]) {
            r = 1;
        }
        if (g_worldMusicId == g_worldSongMusicIds[4]) {
            r = 1;
        }
    }
    return r;
}

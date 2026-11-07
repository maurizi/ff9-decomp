#include "ovl_0c.h"

void w_musicUpdate(void) {
    s32 st;

    switch (g_worldMusicState) {
    case WM_MUSIC_REQUESTED:
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
    case WM_MUSIC_WAITING:
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

/* sound effect ids of world-map sound sound */
s32 w_musicGetID(s32 sound, WmSeIds *out) {
    WmSeIds se;

    se.seIds[0] = -1;
    switch (sound) {
    case 0:
        se.seIds[0] = 0x68;
        break;
    case WM_SE_BUZZER:
        se.seIds[0] = SE_BUZZER;
        break;
    case WM_SE_ITEMS:
        se.seIds[0] = SE_ITEMS;
        break;
    case WM_SE_CHEST_OPEN:
        se.seIds[0] = SE_WM_CHEST_OPEN_1;
        break;
    case WM_SE_CHEST_OPEN_2:
        se.seIds[0] = SE_WM_CHEST_OPEN_2;
        break;
    case WM_SE_HELP_OPEN:
        se.seIds[0] = SE_HELP_OPEN;
        break;
    case WM_SE_CURSOR:
        se.seIds[0] = SE_CURSOR;
        break;
    }
    if (se.seIds[0] != -1) {
        se.channelMask = SND_SE_GROUP_MENU;
        se.count = 1;
        *out = se;
        return 1;
    } else {
        switch (sound) {
        case WM_SE_DEAD_PEPPER:
            se.seIds[0] = SE_WM_DEAD_PEPPER;
            break;
        case WM_SE_TENT_REST:
            se.seIds[0] = SE_WM_TENT_REST;
            break;
        case WM_SE_MOOGLE_CALL:
            se.seIds[0] = SE_WM_MOOGLE_CALL;
            break;
        case 34:
            se.seIds[0] = 0x44C;
            break;
        case WM_SE_CHOCOBO_STEP_2:
            se.seIds[0] = SE_WM_CHOCOBO_STEP_2;
            break;
        case WM_SE_CHOCOBO_CALL:
            se.seIds[0] = SE_CHOCOBO_CALL;
            break;
        case WM_SE_CHOCOBO_PECK:
            se.seIds[0] = SE_WM_CHOCOBO_PECK;
            break;
        case WM_SE_AIRSHIP_WATER:
            se.seIds[0] = SE_WM_AIRSHIP_WATER;
            break;
        case WM_SE_CHOCOBO_STEP:
            se.seIds[0] = SE_WM_CHOCOBO_STEP;
            break;
        case WM_SE_CHEST_RISE:
            se.seIds[0] = SE_WM_CHEST_RISE;
            break;
        case WM_SE_CHEST_LAND:
            se.seIds[0] = SE_WM_CHEST_LAND;
            break;
        }
        if (se.seIds[0] != -1) {
            se.channelMask = 0;
            se.count = 1;
            *out = se;
            return 1;
        } else {
            switch (sound) {
            case WM_SE_MOOGLE_APPEAR:
                se.seIds[0] = SE_WM_MOOGLE_APPEAR;
                break;
            case WM_SE_TENT_START:
                se.seIds[0] = SE_WM_TENT_START;
                break;
            }
            if (se.seIds[0] != -1) {
                se.channelMask = WM_SE_GROUP_VEHICLE;
                se.count = 1;
                *out = se;
                return 1;
            } else {
                /* the original third jump table is 8-aligned */
                __asm__(".rdata\n\t.word 0\n\t.text");
                switch (sound) {
                case WM_SE_CHOCOBO_DIG:
                    se.seIds[0] = SE_WM_CHOCOBO_PECK;
                    se.channelMask = 0;
                    se.seIds[1] = SE_WM_CHOCOBO_STEP;
                    break;
                case WM_SE_FLEET_LOOP:
                    se.channelMask = WM_SE_GROUP_VEHICLE;
                    se.seIds[0] = SE_WM_FLEET_LOOP_1;
                    se.seIds[1] = SE_WM_FLEET_LOOP_2;
                    break;
                case WM_SE_AIRSHIP_CUTSCENE_LOOP:
                    se.channelMask = WM_SE_GROUP_VEHICLE;
                    se.seIds[0] = SE_WM_AIRSHIP_CUTSCENE_LOOP_1;
                    se.seIds[1] = SE_WM_AIRSHIP_CUTSCENE_LOOP_2;
                    break;
                case WM_SE_INVINCIBLE_LOOP:
                    se.channelMask = WM_SE_GROUP_VEHICLE;
                    se.seIds[0] = SE_WM_INVINCIBLE_LOOP_1;
                    se.seIds[1] = SE_WM_INVINCIBLE_LOOP_2;
                    break;
                case WM_SE_HILDA_GARDE_LOOP:
                    se.channelMask = WM_SE_GROUP_VEHICLE;
                    se.seIds[0] = SE_WM_HILDA_GARDE_LOOP_1;
                    se.seIds[1] = SE_WM_HILDA_GARDE_LOOP_2;
                    break;
                case WM_SE_BOAT_LOOP:
                    se.channelMask = WM_SE_GROUP_VEHICLE;
                    se.seIds[0] = SE_WM_BOAT_LOOP_1;
                    se.seIds[1] = SE_WM_BOAT_LOOP_2;
                    break;
                case WM_SE_RUMBLE_LOOP:
                    se.channelMask = WM_SE_GROUP_AMBIENT;
                    se.seIds[0] = SE_WM_RUMBLE_LOOP_1;
                    se.seIds[1] = SE_WM_RUMBLE_LOOP_2;
                    break;
                case WM_SE_WIND_LOOP:
                    se.channelMask = WM_SE_GROUP_AMBIENT;
                    se.seIds[0] = SE_WM_WIND_LOOP_1;
                    se.seIds[1] = SE_WM_WIND_LOOP_2;
                    break;
                }
                if (se.seIds[0] != -1) {
                    se.count = 2;
                    *out = se;
                    return 1;
                }
                printf("w_musicGetID : Illigal SE Number #%d\n", sound);
                return 0;
                printf(""); /* unreachable; its empty string is in .rodata */
            }
        }
    }
}

const char s_wmStrMusicNormalSong[] = "w_musicRequestSend : Normal Song";
const char s_wmStrMusicAirShipSong[] = "w_musicRequestSend : Air Ship Song";
const char s_wmStrMusicChocoboSong[] = "w_musicRequestSend : Chocobo Song";
const char s_wmStrMusicDisc4Song[] = "w_musicRequestSend : Disc4 Song";
const char s_wmStrMusicFieldSong1[] = "w_musicRequestSend : Field Song 1";
const char s_wmStrMusicFieldSong2[] = "w_musicRequestSend : Field Song 2";
const char s_wmStrMusicUnknownError[] = "w_musicRequestSend : Unknown Error";
const char s_wmStrMusicAccepted[] = "(Accepted)\n";
const char s_wmStrMusicDenied[] = "(Denied / same request)\n";
/* padded: the jump table that follows is 8-aligned in its object */
const char s_wmStrMusicNotReady[0x38] = "w_musicRequestSend : music status is not ready now\n";

void w_musicRequestSend(u32 request) {
    if (g_worldMusicState == WM_MUSIC_READY) {
        if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
            switch (request) {
            case WM_SONG_NORMAL:
                printf(s_wmStrMusicNormalSong);
                break;
            case WM_SONG_AIRSHIP:
                printf(s_wmStrMusicAirShipSong);
                break;
            case WM_SONG_CHOCOBO:
                printf(s_wmStrMusicChocoboSong);
                break;
            case WM_SONG_DISC4:
                printf(s_wmStrMusicDisc4Song);
                break;
            case WM_SONG_FIELD1:
                printf(s_wmStrMusicFieldSong1);
                break;
            case WM_SONG_FIELD2:
                printf(s_wmStrMusicFieldSong2);
                break;
            default:
                printf(s_wmStrMusicUnknownError);
                break;
            }
        }
        if (g_worldMusicRequest != request) {
            g_worldMusicState = WM_MUSIC_REQUESTED;
            g_worldMusicRequest = request;
            if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
                printf(s_wmStrMusicAccepted);
            }
        } else if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
            printf(s_wmStrMusicDenied);
        }
    } else if (g_worldMapDebugFlags & WM_DEBUG_MUSIC) {
        printf(s_wmStrMusicNotReady);
    }
}

/* stops the world sounds in three runs of numbers; each run leaves out its last sound (WM_SE_TENT_START,
 * WM_SE_WIND_LOOP, WM_SE_CHEST_LAND) */
void wm_stopAllSe(void) {
    WmSeIds se;
    s32 i;
    s32 j0;
    s32 m0;
    s32 j1;
    s32 m1;
    s32 j2;
    s32 m2;

    for (i = 0; i < WM_SE_TENT_START; i++) {
        if (w_musicGetID(i, &se) & 0xFF) {
            m0 = se.channelMask;
            for (j0 = 0; j0 < se.count; j0++) {
                wm_soundCommand(SND_SE_STOP, se.seIds[j0], m0, 0, 0);
                m0 >>= 1;
            }
        }
    }
    for (i = WM_SE_FLEET_LOOP; i < WM_SE_WIND_LOOP; i++) {
        if (w_musicGetID(i, &se) & 0xFF) {
            m1 = se.channelMask;
            for (j1 = 0; j1 < se.count; j1++) {
                wm_soundCommand(SND_SE_STOP, se.seIds[j1], m1, 0, 0);
                m1 >>= 1;
            }
        }
    }
    for (i = WM_SE_DEAD_PEPPER; i < WM_SE_CHEST_LAND; i++) {
        if (w_musicGetID(i, &se) & 0xFF) {
            m2 = se.channelMask;
            for (j2 = 0; j2 < se.count; j2++) {
                wm_soundCommand(SND_SE_STOP, se.seIds[j2], m2, 0, 0);
                m2 >>= 1;
            }
        }
    }
    if ((u8)wm_isMapSongPlaying() && g_worldEventResult != WM_EVT_BATTLE) {
        g_gameState->gameData->flags |= GD_MUSIC_LEFT_PLAYING;
    }
}

s32 wm_keepEntryMusic(void) {
    u16 sc = g_evtGlobalVars.h[0];
    u8 moveMode = g_evtGlobalVars.b[0xBE];

    if (sc - (u32)WM_STORY_FIELD_SONG1 < WM_STORY_FIELD_SONG2 - WM_STORY_FIELD_SONG1) {
        return 1;
    }
    if (sc - (u32)WM_STORY_FIELD_SONG2 < WM_STORY_FIELD_SONG2_END - WM_STORY_FIELD_SONG2) {
        return 1;
    }
    return (u32)(moveMode - WM_MOVE_HILDA_GARDE) < 2;
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

void wm_musicPlay(u16 song) {
    u8 volume = g_evtGlobalVars.b[0x68];

    g_worldMusicId = wm_getSongMusicId(song);
    g_worldMusicStarted = 1;
    wm_soundCommand(SND_MUSIC_PLAY, g_worldMusicId, 0, 0, 0);
    if (volume == 0) {
        volume = 0x7D;
    }
    wm_soundCommand(SND_MUSIC_VOL_INTPL, 0, 0x3C, volume, 0);
}

void wm_musicStop(u16 song) {
    wm_soundCommand(SND_MUSIC_STOP, wm_getSongMusicId(song), 0, 0, 0);
}

void wm_musicFade(void) {
}

void wm_sePlay(s32 sound, u8 volume) {
    WmSeIds se;
    s32 i;
    s32 chanMask;

    if (w_musicGetID(sound, &se) & 0xFF) {
        chanMask = se.channelMask;
        for (i = 0; i < se.count; i++) {
            wm_soundCommand(SND_SE_PLAY, se.seIds[i], chanMask, 0x7F, volume);
            chanMask >>= 1;
        }
    }
}

void wm_seSetVolume(s32 sound, u8 volume) {
    WmSeIds se;
    s32 i;
    s32 chanMask;

    if (g_worldMusicState == WM_MUSIC_READY && (w_musicGetID(sound, &se) & 0xFF)) {
        chanMask = se.channelMask;
        for (i = 0; i < se.count; i++) {
            wm_soundCommand(SND_SE_VOL, se.seIds[i], chanMask, volume, 0);
            chanMask >>= 1;
        }
    }
}

void wm_seFadeVolume(s32 sound, u16 frames, u8 volume) {
    WmSeIds se;
    s32 i;
    s32 chanMask;

    if (w_musicGetID(sound, &se) & 0xFF) {
        chanMask = se.channelMask;
        for (i = 0; i < se.count; i++) {
            wm_soundCommand(SND_SE_VOL_INTPL, se.seIds[i], chanMask, frames, volume);
            chanMask >>= 1;
        }
    }
}

void wm_seStop(s32 sound) {
    WmSeIds se;
    s32 i;
    s32 chanMask;

    if (w_musicGetID(sound, &se) & 0xFF) {
        chanMask = se.channelMask;
        for (i = 0; i < se.count; i++) {
            wm_soundCommand(SND_SE_STOP, se.seIds[i], chanMask, 0, 0);
            chanMask >>= 1;
        }
    }
}

s32 wm_getSongMusicId(u16 song) {
    u16 sc;

    if (song == WM_SONG_NORMAL) {
        sc = *g_worldStoryCounterPtr;
        if (sc - (u32)WM_STORY_FIELD_SONG1 < WM_STORY_FIELD_SONG2 - WM_STORY_FIELD_SONG1) {
            song = WM_SONG_FIELD1;
        }
        if (sc - (u32)WM_STORY_FIELD_SONG2 < WM_STORY_FIELD_SONG2_END - WM_STORY_FIELD_SONG2) {
            song = WM_SONG_FIELD2;
        }
        if (g_worldDataSet == WM_DATASET_DISC4) {
            song = WM_SONG_DISC4;
        }
    }
    return g_worldSongMusicIds[song];
}

s32 wm_isMapSongPlaying(void) {
    u8 playing = g_worldMusicId == g_worldSongMusicIds[0];

    if (g_worldMusicId == g_worldSongMusicIds[1]) {
        playing = 1;
    }
    if (g_worldMoveMode == WM_MOVE_WALK) {
        if (g_worldMusicId == g_worldSongMusicIds[3]) {
            playing = 1;
        }
        if (g_worldMusicId == g_worldSongMusicIds[4]) {
            playing = 1;
        }
    }
    return playing;
}

#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* w_frameSetParameter request ids (event opcode 0xC4). Names follow Square's debug strings
 * "*[ Request from script:... ]" printed for each id (spelling corrected), [code] where silent. */
#define WM_REQ_WEATHER 0 /* "Weather": wm_setWeather(arg, WM_FOG_KEEP, param 0); WM_WEATHER_AUTO turns auto weather on */
#define WM_REQ_ACTOR 1 /* "Actor": w_movementChange */
#define WM_REQ_NAVI_ACTIVE 2 /* "Navi Active" */
#define WM_REQ_NAVI_MODE 4 /* "Navi mode" */
#define WM_REQ_CAMERA_LOOKUP_FIX 5 /* "Camera Lookup Fix mode" */
#define WM_REQ_PARAM0 6 /* silent: g_worldReqParams[0..3] (6-9) */
#define WM_REQ_PARAM1 7
#define WM_REQ_PARAM2 8
#define WM_REQ_PARAM3 9
#define WM_REQ_SPAWN_EFFECT 10 /* silent: wm_spawnEffect(param 0-2, arg, param 3) */
#define WM_REQ_TITLE_DRAW_TIME 11 /* "Title Draw Time" */
#define WM_REQ_WEATHER_SPEED 14 /* "Whather Speed": param 0 */
#define WM_REQ_WEATHER_AUTO 15 /* "Whather Auto" */
#define WM_REQ_SE_PARAM0 16 /* "SE Param0".."SE Param3" (16-19): g_worldReqParams[0..3] */
#define WM_REQ_SE_PARAM1 17
#define WM_REQ_SE_PARAM2 18
#define WM_REQ_SE_PARAM3 19
#define WM_REQ_SE_PLAY 20 /* "SE Play" */
#define WM_REQ_SE_VOL 21 /* "SE Vol" */
#define WM_REQ_SE_VOL_INTPL 22 /* "SE Vol Intpl" */
#define WM_REQ_SE_STOP 23 /* "SE Stop" */
#define WM_REQ_MUSIC_VOL_INTPL 24 /* "Music Vol Intpl" */
#define WM_REQ_CAMERA_SMOOTH 25 /* "Camera Smooth" */
#define WM_REQ_EVENT_BATTLE_PROB 26 /* "EventBattleProbability" */
#define WM_REQ_NAVI_CURSOR 27 /* "Navi Cursol" */
#define WM_REQ_GET_GETOFF_POS 28 /* "Get Getoffpos": wm_findGetoffPos into params 0-2 (10000 each when none) */
#define WM_REQ_CALL_CHOCOBO 29 /* "Call Chocobo" */
#define WM_REQ_GETOFF_SP1 30 /* "Get Getoff SP1" */
#define WM_REQ_GETOFF_SP2 31 /* "Get Getoff SP2" */
#define WM_REQ_MUSIC_FADE 32 /* "Music Fade" */
#define WM_REQ_MUSIC_FADE_FORCE 33 /* "Music Fade Force" */
#define WM_REQ_AUTO_PILOT_ON 34 /* "Auto Pilot ON" */
#define WM_REQ_MOG_CHOICE 35 /* "Mog choice" */
#define WM_REQ_CURSOR_DRAW 36 /* "Cursol Draw" */
#define WM_REQ_ALPHA0 37 /* "Alpha 0" (clears the smoothed speed/climb inputs) */
#define WM_REQ_AUTO_PILOT_OFF 38 /* "Auto Pilot OFF" */
#define WM_REQ_CD_USE_ON 39 /* "CD Use ON" */
#define WM_REQ_CD_USE_OFF 40 /* "CD Use OFF" */
#define WM_REQ_MAP_JUMP 41 /* "Map Jump" */

/* w_frameGetParameter ids: event-script expression variables >= 0xC0 on the world map
 * (main evt_getSysVar's default branch) */
#define WM_PARAM_AREA 0xC0 /* area of the cell under the player ("Now Area" in wm_getEncounterEntry) */
#define WM_PARAM_TOPOGRAPHY 0xC1 /* topography of that cell ("Topograph") */
#define WM_PARAM_RIDE_Y 0xC2 /* the player's floorY + rideOffset (floorY + 400 in WM_MOVE_CHOCOBO_FLY); get-off height check */
#define WM_PARAM_REQ0 0xC3 /* g_worldReqParams[0..2], e.g. the WM_REQ_GET_GETOFF_POS result */
#define WM_PARAM_REQ1 0xC4
#define WM_PARAM_REQ2 0xC5
#define WM_PARAM_CHOCOGRAPH 0xC6 /* GameData.chocograph */
#define WM_PARAM_CAMERA_YAW 0xC7 /* g_worldCamera->yaw >> 4 */
#define WM_PARAM_CD_BUSY 0xC8 /* "Event:CDAccessWait": g_worldCdUser | music request pending */
#define WM_PARAM_PLAYER_X 0xC9
#define WM_PARAM_PLAYER_Y 0xCA
#define WM_PARAM_PLAYER_Z 0xCB
#define WM_PARAM_WEATHER_POINT_DIST 0xCC /* g_worldWeatherPointDist */
#define WM_PARAM_EVENT_BATTLE 0xCD /* 1 in g_worldEventBattleRate + 1 while walking on topography 0x24-0x26 */
#define WM_PARAM_PLAYER_DIR 0xCE /* player rotY >> 4 */
#define WM_PARAM_ZONE 0xCF /* wm_getAreaZone(area) */
#define WM_PARAM_TYPE11_RIDE_OFFSET 0xD0 /* g_worldActorTypeInfo[11].rideOffset */
#define WM_PARAM_RUNNING 0xD1 /* g_worldState == WM_STATE_RUNNING */
#define WM_PARAM_TITLE_PHASE 0xD2 /* g_worldTitlePhase */
#define WM_PARAM_NAVI_VISIBLE 0xD3 /* g_worldNaviVisible */

/* cell attribute (WmActorType.unk8, wm_getBlockHeight): area in bits 8-13, topography in bits 2-7 (docs/encounters.md) */
#define WM_ATTR_AREA(a) (((a) & 0x3F00) >> 8)
/* areas (WM_ATTR_AREA), named after g_worldAreaNames (WM_PACK_AREA_NAMES: entry 2 * area is the name the game shows, read
 * from the world data pack on disc 3; R41b's runtime area numbers match it). Evidence: [string] */
#define WM_AREA_GUNITAS_BASIN          0 /* "Gunitas Basin" */
#define WM_AREA_MELDA_PLAINS           1 /* "Melda Plains" */
#define WM_AREA_NOLRICH_HEIGHTS        2 /* "Nolrich Heights" */
#define WM_AREA_BYAN_HEIGHTS           3 /* "Byan Heights" */
#define WM_AREA_TUHANDA_SHORES         4 /* "Tuhanda Shores" */
#define WM_AREA_NOMARANIA_BEACH        5 /* "Nomarania Beach" */
#define WM_AREA_LACHENTA_WETLANDS      6 /* "Lachenta Wetlands" */
#define WM_AREA_EUNORUS_PLAINS         7 /* "Eunorus Plains" */
#define WM_AREA_ELEANOR_COAST          8 /* "Eleanor Coast" */
#define WM_AREA_DAINES_HORSE_BASIN     9 /* "Daines-horse Basin" */
#define WM_AREA_BENTINI_HEIGHTS        10 /* "Bentini Heights" */
#define WM_AREA_BOHDEN_HEIGHTS         11 /* "Bohden Heights" */
#define WM_AREA_VUBE_DESERT            12 /* "Vube Desert" */
#define WM_AREA_EESISTERN_COAST        13 /* "Eesistern Coast" */
#define WM_AREA_LINDBLUM_PLATEAU       14 /* "Lindblum Plateau" */
#define WM_AREA_KING_ED_PLAINS         15 /* "King Ed Plains" */
#define WM_AREA_METALARK_SHORES        16 /* "Metalark Shores" */
#define WM_AREA_DEREK_BEACH            17 /* "Derek Beach" */
#define WM_AREA_MAKIKI_BEACH           18 /* "Makiki Beach" */
#define WM_AREA_LANSHAKE_ISLAND        19 /* "Lanshake Island" */
#define WM_AREA_SUMA_ARCHIPELAGO       20 /* "Suma Archipelago" */
#define WM_AREA_DEADLY_CAPE            21 /* "Deadly Cape" */
#define WM_AREA_ALEXANDRIA_PLATEAU     22 /* "Alexandria Plateau" */
#define WM_AREA_ZAMO_BASIN             23 /* "Zamo Basin" */
#define WM_AREA_EDGECRY_COAST          24 /* "Edgecry Coast" */
#define WM_AREA_TOGULL_BEACH           25 /* "Togull Beach" */
#define WM_AREA_POPOS_HEIGHTS          26 /* "Popos Heights" */
#define WM_AREA_DONNA_PLAINS           27 /* "Donna Plains" */
#define WM_AREA_MAGDALENE_FOREST       28 /* "Magdalene Forest" */
#define WM_AREA_DYSHNOSE_SHORES        29 /* "Dyshnose Shores" */
#define WM_AREA_GEGALRICH_SHORES       30 /* "Gegalrich Shores" */
#define WM_AREA_KOGNISH_SHORES         31 /* "Kognish Shores" */
#define WM_AREA_LUCID_PLAINS           32 /* "Lucid Plains" */
#define WM_AREA_BANYANO_SHORES         33 /* "Banyano Shores" */
#define WM_AREA_PUALEI_PLAINS          34 /* "Pualei Plains" */
#define WM_AREA_MOTEORE_PENINSULA      35 /* "Moteore Peninsula" */
#define WM_AREA_KORUA_ISLAND           36 /* "Korua Island" */
#define WM_AREA_FALSTEAD_COAST         37 /* "Falstead Coast" */
#define WM_AREA_SHENGALASTA_SHORES     38 /* "Shengalasta Shores" */
#define WM_AREA_KIERA_DESERT           39 /* "Kiera Desert" */
#define WM_AREA_PILKIRAS_ISLAND        40 /* "Pilkiras Island" */
#define WM_AREA_SEAWAYS_CANYON         41 /* "Seaways Canyon" */
#define WM_AREA_BLAIRSURPASS_PLAINS    42 /* "Blairsurpass Plains" */
#define WM_AREA_YUANSHAO_PENINSULA     43 /* "Yuanshao Peninsula" */
#define WM_AREA_HADRIS_BASIN           44 /* "Hadris Basin" */
#define WM_AREA_GUSTY_VALLEY           45 /* "Gusty Valley" */
#define WM_AREA_EVERLANG_ISLAND        46 /* "Everlang Island" */
#define WM_AREA_LANAR_ISLAND           47 /* "Lanar Island" */
#define WM_AREA_SACROBLESS_ISLAND      48 /* "Sacrobless Island" */
#define WM_AREA_PALMNELL_ISLAND        49 /* "Palmnell Island" */
#define WM_AREA_SALVAGE_ARCHIPELAGO    50 /* "Salvage Archipelago" */
#define WM_AREA_RYUKI_SHORES           51 /* "Ryuki Shores" */
#define WM_AREA_QUELMIERA_SHORES       52 /* "Quelmiera Shores" */
#define WM_AREA_MITMAKIS_ICE_FIELD     53 /* "Mitmakis Ice Field" */
#define WM_AREA_GULUG_MOUNTAINS        54 /* "Gulug Mountains" */
#define WM_AREA_CANARAMIST_CAPE        55 /* "Canaramist Cape" */
#define WM_AREA_SEATENCE_ICE_FIELD     56 /* "Seatence Ice Field" */
#define WM_AREA_CAZEDIL_PLAINS         57 /* "Cazedil Plains" */
#define WM_AREA_COMMURE_SHORES         58 /* "Commure Shores" */
#define WM_AREA_LIKANMACK_ISLAND       59 /* "Likanmack Island" */
#define WM_AREA_TRIQUAI_PLAINS         60 /* "Triquai Plains" */
#define WM_AREA_KUENTIS_PENINSULA      61 /* "Kuentis Peninsula" */
#define WM_AREA_VILE_ISLAND            62 /* "Vile Island" */
#define WM_AREA_UAHO_ISLAND            63 /* "Uaho Island" */
#define WM_ATTR_TRIGGER_MASK 0xC000 /* cell event trigger kind (evt_callWorldCellTrigger's third argument), 0 none */
#define WM_ATTR_TRIGGER(a) (((a) & WM_ATTR_TRIGGER_MASK) >> 14)
#define WM_ATTR_TOPO_MASK 0xFC /* bits 2-7 (callers that shift it as s32 use the mask directly) */
#define WM_ATTR_TOPOGRAPHY(a) (((a) & WM_ATTR_TOPO_MASK) >> 2)
/* topography values (WM_ATTR_TOPOGRAPHY) the code tests by number. Which move modes may enter a topography is data
 * (WmMoveMode.allowedCells: the Reef chocobo adds 0x33 0x35-0x37 0x3D, Mountain 0x31 0x32, Ocean 0x30 0x38 0x39, the
 * flying ones 0x3A-0x3C 0x3E; the boat only 0x35 0x36 0x39). Evidence: R37 in docs/runtime-findings.md; the chocobo
 * track cells, 0x17, 0x2D/0x2E, 0x30 and 0x33 from R41b (airship sweep of the topography grid, walker teleported onto
 * each). 0x1E/0x23 (beach splash like WM_TOPO_BEACH) stay numbers. */
#define WM_TOPO_BEACH        0x22 /* runtime: sand at the water's edge; walking spawns WM_FX_BEACH_SPLASH */
#define WM_TOPO_FOREST       0x24 /* runtime: the walker sinks into the trees (wm_getTerrainHeightOffset class 0), */
#define WM_TOPO_FOREST_2     0x25 /* walking spawns WM_FX_FOREST_LEAVES; 0x24-0x26 are always handled together and */
#define WM_TOPO_FOREST_3     0x26 /* raise WM_PARAM_EVENT_BATTLE */
#define WM_TOPO_SAND         0x29 /* runtime: sandy plain; WM_FX_SAND_DUST walking, WM_FX_AIRSHIP_SAND / WM_FX_CHOCOBO_SAND flying low */
#define WM_TOPO_MOUNTAIN     0x31 /* runtime (airship climbs over it) and data (Mountain chocobo); the camera eye rises faster */
#define WM_TOPO_NO_ENCOUNTER 0x34 /* code: script battles are dropped while the player stands on it, no getting off onto it */
/* runtime (R41b, medium): neither can be walked on, the walker sinks into them like the other waters */
#define WM_TOPO_WATERFALL    0x30 /* the waterfalls and their basin (polygons falling from y -6736 to -684), a flat pool */
#define WM_TOPO_RIVER        0x33 /* thin strips running down into WM_TOPO_SHALLOWS / WM_TOPO_WATER (a river mouth, below the falls) */
#define WM_TOPO_SHALLOWS     0x35 /* data: Reef chocobo and boat water, shallowest sinking; the boat can only be left here */
/* deeper water. Runtime (R41): the boat's cell went 0x35 -> 0x36 -> 0x39 sailing out to open sea; 0x36 / 0x37 form the
 * lake in the Hadris crater, 0x38 / 0x39 the coast (0x39 the most common topography). The walker sinks by
 * g_worldTerrainHeightOffsets[1]: 150 (0x35), 250, 200, 450, 450 (0x39) */
#define WM_TOPO_WATER        0x36 /* inland lake / water beyond the shallows; boat water */
#define WM_TOPO_WATER_2      0x37
#define WM_TOPO_SEA_2        0x38 /* deepest sinking, like WM_TOPO_SEA; Ocean chocobo only */
#define WM_TOPO_SEA          0x39 /* open sea; boat and Ocean chocobo */
/* chocobo tracks: wm_canCallChocobo lets a walker call the chocobo only on these five [code]. Runtime (R41b): every
 * one found shows chocobo footprints; they differ by the ground (0x03 grass, 0x12 / 0x15 brown earth, 0x1C snow),
 * not by region. 0x15 was seen only on the Forgotten Continent side (Blairsurpass Plains). */
#define WM_TOPO_CHOCOBO_TRACKS_GRASS   0x03
#define WM_TOPO_CHOCOBO_TRACKS_EARTH   0x12
#define WM_TOPO_CHOCOBO_TRACKS_EARTH_2 0x15
#define WM_TOPO_CHOCOBO_TRACKS_5       0x16 /* (runtime: pending R41; not present in the disc 3 world, its ground is unknown) */
#define WM_TOPO_CHOCOBO_TRACKS_SNOW    0x1C
#define WM_TOPO_GUSTY_VALLEY 0x17 /* wm_updateMap: within 0x22 of ambient sound source 1 a walker or chocobo on it is pushed */
                                  /* towards +z; wm_findGetoffPos refuses within 0x16 of it. Runtime (R41b): only in the */
                                  /* cells next to source 1, area 45 "Gusty Valley"; a walker there drifts toward +z */
/* wm_findGetoffPos: the airships can't set down on 0x2D, 0x2E (and WM_TOPO_NO_ENCOUNTER) [code]. Runtime (R41b,
 * medium): rocky plateau tops of the Forgotten Continent; the refusal itself was not isolated (Circle over them landed
 * on a nearby cell). */
#define WM_TOPO_NO_LANDING   0x2D /* plateaus around Ipsen's Castle (areas 43 Yuanshao Peninsula, 42) */
#define WM_TOPO_NO_LANDING_2 0x2E /* Seaways Canyon / Gusty Valley plateaus (areas 41, 45) */

/* g_worldMapDebugFlags bits: each gates one group of debug prints */
#define WM_DEBUG_MUSIC 0x4 /* w_music */
#define WM_DEBUG_ENCOUNTER 0x8 /* "- Encount Information -" */
#define WM_DEBUG_FORM 0x10 /* "BGI Form", "Scene:%d / Status:%d" */
#define WM_DEBUG_MOVEMENT 0x20 /* w_movementChange */
#define WM_DEBUG_MEMORY 0x40 /* w_memoryman, destructors */
#define WM_DEBUG_REQUEST 0x80 /* "Request from script", "Event:CDAccessWait" */
#define WM_DEBUG_PACK 0x100 /* w_frameExtruct, w_framePackGetPtr */
#define WM_DEBUG_TEXTURE 0x200 /* "Texture ReadDone and Extruct", "Texture Transfer Area" */

/* g_worldState */
#define WM_STATE_START 0 /* only the event script runs */
#define WM_STATE_SETUP 1 /* set by wm_frameMain on frame 4 */
#define WM_STATE_RUNNING 2 /* set by wm_chunksLoaded after the first 16 chunks: draw and pad input */

/* g_worldEventResult: event-script frame result returned by wm_frameMain */
#define WM_EVT_BATTLE EVT_UPD_BATTLE /* -> WM_EXIT_BATTLE */
#define WM_EVT_FIELD EVT_UPD_FIELD /* "MAP JUMP" -> WM_EXIT_FIELD */
#define WM_EVT_WORLD EVT_UPD_WORLD /* next world map */

/* WorldMapObj.exitReason, switched on by wm_exit */
#define WM_EXIT_FIELD 1
#define WM_EXIT_BATTLE 2
#define WM_EXIT_WORLD 3

/* g_worldCdUser: who holds the CD */
#define WM_CD_FREE 0
#define WM_CD_CHUNKS 1 /* wm_readChunks */
#define WM_CD_MUSIC 2 /* w_musicUpdate */
#define WM_CD_SCRIPT 3 /* "CD Use ON" */

/* g_worldDataSet ("Read CD Image [Disc-1]" / "[Disc-4]") */
#define WM_DATASET_DISC1 1
#define WM_DATASET_DISC4 4

/* g_evtGlobalVars.b[EVT_GB_WM_FILE]: world file index (worldId - FIELD_ID_WORLD_FIRST), written by the world script.
 * Runtime (R41b): 0 = disc 1 Mist-continent world, 3 = boat era, 7 = disc 3 free world, 8 = disc 4 (which story spans
 * each covers: medium). The code tests only these two (wm_updateMap's rain spot, wm_weatherAuto's area check). */
#define WM_FILE_DISC1 0
#define WM_FILE_DISC4 8

/* story counter (g_evtGlobalVars.h[0], *g_worldStoryCounterPtr) values the world map tests. Evidence: [code] what
 * changes at each; [runtime] R41b where noted. */
/* entering at exactly these four shows continent title g_worldTitleId 0-3 (WM_ITEM_NAVI_TIMS images 5-12); runtime
 * (R41b): the title shown at each */
#define WM_STORY_TITLE_MIST      0x960  /* "Mist Continent" */
#define WM_STORY_TITLE_OUTER     0x1766 /* "Outer Continent" */
#define WM_STORY_TITLE_FORGOTTEN 0x2585 /* "Forgotten Continent" */
#define WM_STORY_TITLE_LOST      0x26A2 /* "Lost Continent" */
/* wm_getStoryFog: the fog is the Mist. Runtime (R41b, medium): 0x1765 shows the Mist Continent under haze, 0x1766
 * clear (the data set and the navi map change at the same value); disc 4 (0x2B5C) has it again. 0x2B53 itself was
 * not entered (the world init stalled on disc 1). */
#define WM_STORY_MIST_GONE 0x1766 /* no fog from here, the navi map's second page (g_worldNaviPage 1) and WM_NAVI_KNOWN_LATE */
#define WM_STORY_MIST_BACK 0x2B53 /* ... until here */
#define WM_STORY_FIELD_SONG1 0x2580 /* wm_getSongMusicId: WM_SONG_NORMAL plays WM_SONG_FIELD1 in [0x2580, 0x26A2), */
#define WM_STORY_FIELD_SONG2 0x26A2 /* WM_SONG_FIELD2 in [0x26A2, 0x26B6); wm_keepEntryMusic keeps the music there */
#define WM_STORY_FIELD_SONG2_END 0x26B6
#define WM_STORY_CLEYRA_FALLEN 0x137E /* set on the Red Rose (field 1153 "Red Rose/Bridge", disc 2) after Cleyra is   */
                                    /* destroyed; also g_worldFormStoryThresholds[1], where form WM_FORM_CLAY starts. */
                                    /* Below it the camera view is forced to normal and can't be toggled while the   */
                                    /* player is in WM_AREA_VUBE_DESERT (wm_updateCamera, wm_toggleCameraView). R43   */
#define WM_STORY_BLOCKFX3_ON 0x24EA /* WM_BLOCKFX_KIERA_DESERT emits in [0x24EA, 0x26A3) */
#define WM_STORY_BLOCKFX3_OFF 0x26A3

/* g_worldFog / wm_setWeather fog argument */
#define WM_FOG_OFF 0
#define WM_FOG_ON 1
#define WM_FOG_KEEP 2
/* wm_setWeather mode */
/* runtime (R37, forced through wm_setWeather at the Hadris basin): the sky/light/fog preset pairs */
#define WM_WEATHER_CLEAR 0
#define WM_WEATHER_SUNSET 1      /* orange sky and light; wm_getWeatherByPos: around two points */
#define WM_WEATHER_DARK 2        /* dark blue, night-like; inside the first point's circle; wm_isDarkWeather */
#define WM_WEATHER_DARK_PURPLE 3 /* purple light, dark sea; one point, world data set 4 only; wm_isDarkWeather */
#define WM_WEATHER_AUTO 4 /* WM_REQ_WEATHER: turn auto weather on */
#define WM_WEATHER_KEEP 5

/* WmMoveMode.controlType (wm_controlPlayer: wm_controlWalk / wm_controlFly / wm_controlSteer) */
#define WM_CTRL_WALK 0
#define WM_CTRL_FLY 1
#define WM_CTRL_STEER 2
#define WM_CTRL_WALK_FAST 3 /* WM_MOVE_FAST_WALK; handled like WM_CTRL_WALK by the camera */

/* songs (w_musicRequestSend strings) */
#define WM_SONG_NORMAL 0 /* "Normal Song" */
#define WM_SONG_CHOCOBO 1 /* "Chocobo Song" */
#define WM_SONG_AIRSHIP 2 /* "Air Ship Song" */
#define WM_SONG_DISC4 3 /* "Disc4 Song" */
#define WM_SONG_FIELD1 4 /* "Field Song 1" */
#define WM_SONG_FIELD2 5 /* "Field Song 2" */
/* g_worldMusicState (w_musicUpdate strings) */
#define WM_MUSIC_READY 0
#define WM_MUSIC_REQUESTED 1
#define WM_MUSIC_WAITING 2

/* palette animation entry (wm_setActorFog) */
typedef struct WmPalAnim {
    u16 modelId; /* id */
    u8 pad2[2];
    s16 clutX; /* clut x */
    s16 clutY; /* clut y */
    u16 srcPal[16]; /* source palette */
    u16 workPal[2][16]; /* per draw buffer */
} WmPalAnim; /* 0x68 */

/* per-object entry in WorldMapObj.unk1C (0x28 bytes) */
typedef struct {
    unsigned shade : 8;
    unsigned scaleX : 12; /* scale x */
    unsigned scaleZ : 12; /* scale z */
    unsigned unk4 : 12; /* angle; not read by C */
    signed offsetX : 10; /* offset x */
    signed offsetZ : 10; /* offset z */
    POLY_FT4 *prims[2][4]; /* shadow quads, per draw buffer */
} WmObjEnt;

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    s32 flags;
    void *heapEnd;
    u8 unk8[4]; /* write-only (cleared) */
    s16 swirlX;
    s16 swirlY;
    void *mapScript; /* file 0x5xxxx */
    void *areaData; /* file 0x6xxxx */
    s8 battleStep;
    s8 swirlFrames;
    u8 palAnimCount;
    u8 exitReason;
    WmObjEnt *shadows;
    s32 primCursor;
    s16 sceneId; /* map id */
    u16 battleMusic;
    struct WmPalAnim *palAnims; /* count: unk1A */
    u8 *loadBase;
    u8 *loadPtr;
    void *files[2]; /* [1] freed by wm_freeFile */
    u8 unk3C[16]; /* write-only (cleared) */
} WorldMapObj;
/* WorldMapObj.flags */
#define WM_OBJ_SWIRL_DONE 0x1 /* wm_updateBattleTransition: swirl_update finished; the battle starts once loaded */
/* WorldMapObj.battleStep: counts down from 5 (wm_startBattleTransition), at 0 starts loading, then */
#define WM_BTLSTEP_MUSIC -1 /* waiting for the battle music load */
#define WM_BTLSTEP_SWIRL -2 /* waiting for WM_OBJ_SWIRL_DONE */

/* world map context, GameData.unk8E4.world */
typedef struct WorldMap {
    u32 flags; /* WM_MAP_* */
    u8 *mapHeapBase; /* end of the load area */
    WorldMapObj *obj; /* = &unk48 */
    void *worldData; /* allocation, freed by wm_freeWorldData */
    s32 parkedSoundSize;
    u8 unk14[0x34]; /* write-only (cleared) */
    WorldMapObj objData;
} WorldMap; /* 0x94 */
/* WorldMap.flags */
#define WM_MAP_LOADING 0x1 /* set at the start of wm_init, cleared at the end of wm_load */
#define WM_MAP_NO_FRAME 0x100 /* wm_main skips wm_frameMain (wm_startBattleTransition) */
#define WM_MAP_NO_FADE 0x200 /* wm_main skips game_updateFade (also set by wm_startBattleTransition) */
#define WM_MAP_NO_TRANSITION 0x400 /* set by wm_load; while clear wm_main runs wm_updateBattleTransition */
#define WM_MAP_NO_TIMER 0x800 /* wm_main skips game_updateTimer */
#define WM_MAP_NAVI_SCREEN 0x1000 /* w_frame: set while g_worldNaviMode == WM_NAVI_FULL */

typedef struct {
    u8 pad0[0x11];
    u8 autoTurnOff;
} WmCameraFlags;

/* doubly linked list node; g_worldEffectFree is the list head */
typedef struct WmEffect {
    s32 sprite;
    struct WmEffect *prev;
    struct WmEffect *next;
    s8 abr; /* WM_FX_ABR_* */
    s8 type;
    s16 scale;
    s32 x;
    s32 y;
    s32 z;
    s16 frame;
    s16 pad1E;
    s32 phase;
    s32 unk24; /* write-only (cleared) */
} WmEffect; /* 0x28 */
/* WmEffect.type, wm_spawnEffect's fourth argument (sprite = data pack entry type + 7). Only kinds whose use the code or
 * a runtime check shows (R37 in docs/runtime-findings.md). */
#define WM_FX_BOAT_WAKE 15 /* wm_controlSteer: behind the boat at the water surface while it moves fast; flat, drawn with g_worldEffectSpeed */
#define WM_FX_AIRSHIP_SPRAY 18 /* airship low over water cells, drawn at g_worldSprayLevel (14 and 19 are drawn the same way) */
#define WM_FX_WATER_RIPPLE 22 /* under the moving player on a water cell, at the surface; flat, drawn with g_worldEffectSpeed */
#define WM_FX_CHOCOBO_SPRAY 6 /* code: the flying gold chocobo low over water cells (as WM_FX_AIRSHIP_SPRAY) */
#define WM_FX_BEACH_SPLASH 7 /* runtime: small light-blue specks at the feet, walking on WM_TOPO_BEACH (and 0x1E, 0x23) */
#define WM_FX_BOAT_SIDE_WAKE 10 /* code: spawned in pairs beside WM_FX_BOAT_WAKE on odd frames */
#define WM_FX_CHOCOBO_SAND 14 /* code: the flying gold chocobo low over WM_TOPO_SAND; drawn like WM_FX_AIRSHIP_SPRAY */
#define WM_FX_FOREST_LEAVES 16 /* runtime: green leaves around the walker in a forest, two every other frame */
#define WM_FX_AIRSHIP_SAND 19 /* code: an airship low over WM_TOPO_SAND; drawn like WM_FX_AIRSHIP_SPRAY */
#define WM_FX_WATERFALL_MIST 20 /* runtime: white spray at the foot of the waterfalls (WM_BLOCKFX_WATERFALLS) */
#define WM_FX_SAND_DUST 27 /* runtime: faint pale puff behind the walker on WM_TOPO_SAND */
/* by behaviour (code); runtime: pending R41 */
#define WM_FX_RISING_PUFF 2 /* rises 60-83 a frame with sideways drift, grows (scale frame * 400 + 6000), */
#define WM_FX_RISING_PUFF_2 3 /* subtractive blend; 2 lives 77 frames, 3 46 (3 is emitted by WM_BLOCKFX_GULUG_SUMMIT) */
#define WM_FX_CLAY_SWIRL 23 /* circles WM_BLOCKFX_CLAY's centre (g_worldBlockFx4Pos) while rising, then drifts like 24 */
#define WM_FX_MEMORIA_STREAM 25 /* code: 25 and 26 are drawn turned to face (WM_FX_MEMORIA_AIM_X, _Z), the */
#define WM_FX_MEMORIA_STREAM_2 26 /* WM_BLOCKFX_MEMORIA_ORB sphere. Never spawned (R43): no block-effect emitter */
                                  /* and no world script requests them; 0 wm_spawnEffect calls at the orb */
#define WM_FX_MEMORIA_AIM_X 0x30031 /* 196657, -81774: 240 units from the orb's centre g_worldBlockFx6Pos */
#define WM_FX_MEMORIA_AIM_Z -0x13F6E
/* WmEffect.abr: wm_spawnEffect's blend per type, passed to wm_drawSprite3D (abr & 3 into the tpage, 0xFF = no
 * semi-transparency code). Evidence: [code] */
#define WM_FX_ABR_ADD 1 /* TPAGE_ABR(1): back + front (most types) */
#define WM_FX_ABR_SUB 2 /* back - front (WM_FX_RISING_PUFF, _2) */
#define WM_FX_ABR_OPAQUE 0xFF /* WM_FX_FOREST_LEAVES (drawn twice, with 1 and 2, by wm_drawEffects itself) */
/* spawn heights (wm_spawnEffect's y; y grows downward, the sea surface is y 0) */
#define WM_FX_WATER_Y -100 /* WM_FX_WATER_RIPPLE, WM_FX_BOAT_WAKE and _SIDE_WAKE: just above the water surface */
#define WM_FX_SPRAY_Y -200 /* WM_FX_AIRSHIP_SPRAY and WM_FX_CHOCOBO_SPRAY over water cells */
/* g_worldEffectPos (wm_initEffects; nothing reads it): the anchors of three block effects. [0] = g_worldBlockFx7Pos
 * (WM_BLOCKFX_DALI_WINDMILL), [1] (copied to [2]-[5]) within 700 units of g_worldBlockFx4Pos (WM_BLOCKFX_CLAY, Cleyra),
 * [6] (copied to [7], [8]) = g_worldBlockFx6Pos (WM_BLOCKFX_MEMORIA_ORB). Evidence: [code] the matching positions */
#define WM_FXPOS_DALI_WINDMILL_X 0x4464C
#define WM_FXPOS_DALI_WINDMILL_Y -0x1E68
#define WM_FXPOS_DALI_WINDMILL_Z -0x32681
#define WM_FXPOS_CLEYRA_X 0x3805B
#define WM_FXPOS_CLEYRA_Y 0x1F4
#define WM_FXPOS_CLEYRA_Z -0x30847
#define WM_FXPOS_MEMORIA_ORB_X 0x2FFFA
#define WM_FXPOS_MEMORIA_ORB_Y -0x2A82
#define WM_FXPOS_MEMORIA_ORB_Z -0x14052
/* wm_updateBlockFx / wm_drawBlockFx block ids (the emitter table is data pack entry id + 0x28) */
#define WM_BLOCKFX_WATERFALLS 5 /* runtime: always on, emits WM_FX_WATERFALL_MIST at three waterfalls */
#define WM_BLOCKFX_WATER_SHRINE 10 /* code: on while WM_FORM_HOKORA_MIZU ("HOKORA(MIZU)") is set */
/* by behaviour (code, positions from the overlay's data); places from R41b where noted */
#define WM_BLOCKFX_GULUG_SUMMIT 2 /* R37: its block is Mount Gulug's summit; always emits, and turns the CLUT animations */
                                  /* 12-17 on (g_worldClutAnimFx2) (runtime: pending R41, the effect was never seen) */
#define WM_BLOCKFX_KIERA_DESERT 3 /* emits effect type 4 only while the story counter is in [WM_STORY_BLOCKFX3_ON, */
                                  /* WM_STORY_BLOCKFX3_OFF); runtime (R41b, medium): its three emitters are in area 39 */
                                  /* Kiera Desert (seen spawning; the effect itself was not visible) */
#define WM_BLOCKFX_CLAY 4 /* five spinning models at (229007, 0, -198927), drawn on data set 1 until form WM_FORM_CLAY; */
                          /* ambient sound 0 (WM_SE_WIND_LOOP) sits there too (runtime: pending R41, not reached in R41b) */
#define WM_BLOCKFX_MEMORIA_ORB 6 /* data set 4 only: a sprite, a core model, two counter-rotating shells, 10 random */
                                 /* flashes and 10 expanding rings at (196602, -10882, -82002); runtime (R41b): the */
                                 /* huge purple glowing sphere over the Iifa Tree mound ("???", Pualei Plains) */
#define WM_BLOCKFX_DALI_WINDMILL 7 /* one model at (280140, -7784, -206465), turning except in [STORY_DISC3, */
                                   /* STORY_WORLD_LATE); runtime (R41b): windmill sails on a stone hut in Nolrich */
                                   /* Heights; navi place 13 "Dali" is at (280253, -206433), 120 units away */
/* wm_cameraFollow: which way the camera target / eye height has to move (y grows downward) */
#define WM_CAM_HOLD 0
#define WM_CAM_LOWER 1 /* the wanted height is more than the dead zone below the current one */
#define WM_CAM_RAISE 2 /* ... above it */
/* camera heights (y grows downward, sea level 0). Evidence: [code] wm_cameraFollow */
#define WM_CAM_EYE_CEILING -0x47CF /* the eye never goes higher */
#define WM_CAM_FLY_EYE_LOWEST -0x1194 /* while flying (WM_CTRL_FLY) the eye stays at least this high */
#define WM_CAM_GROUND_TOP_FLY -0x2134 /* the ground under the eye counts as no higher than this when flying, */
#define WM_CAM_GROUND_TOP -0x251C /* ... and than this otherwise (the peaks don't push the eye up further) */
/* Two balls at the Iifa Tree (navi place 36 "Iifa Tree" (200097, -82107); on data set WM_DATASET_DISC4 the
 * WM_BLOCKFX_MEMORIA_ORB sphere, g_worldBlockFx6Pos (196602, -10882, -82002), sits there). Evidence: [code]
 * wm_cameraFollow, wm_movePlayer */
#define WM_IIFA_X 0x2FF77 /* 196471 */
#define WM_IIFA_Z -0x14031 /* -82001 */
/* the camera eye stays outside a dome over the whole Iifa Tree (runtime R43): */
#define WM_IIFA_CAM_R 0x36B0 /* the camera eye is pushed up onto a ball of radius 14000 ... */
#define WM_IIFA_CAM_Y 0x898 /* ... centred this far below sea level */
/* the Iifa Tree's crown (canopy), which the airship slides over (or passes under; wm_updateActorHeight undoes the */
/* snap below the centre). Runtime R43: the ship rests on the tree's green top; disc 4 raises the crown by 700 */
#define WM_IIFA_CROWN_R 0x9C4 /* a flyer within 2500 of the centre slides over (or under) a ball of radius 2500, */
#define WM_IIFA_CROWN_Y_DISC1 0x1964 /* centred at y = -6500 (data set WM_DATASET_DISC1) */
#define WM_IIFA_CROWN_Y_DISC4 0x1C20 /* or -7200 (data set WM_DATASET_DISC4) */
#define WM_IIFA_CROWN_NOCACHE_DISC1 0x1E78 /* above y = -7800 / -8500 there the ground poly cache is off */
#define WM_IIFA_CROWN_NOCACHE_DISC4 0x2134
/* flight ceiling (wm_movePlayer): climbing slows down within WM_FLY_CEILING_SOFT of y = WM_FLY_CEILING */
#define WM_FLY_CEILING -0x2A30
#define WM_FLY_CEILING_SOFT 0x7E8
/* Ipsen's Castle: the airships can't land within 1440 of (49120, -121408) (wm_findGetoffPos; navi place 44 "Ipsen's
 * Castle" is at (49042, -121464); R41b saw the upside-down castle in a crater there) */
#define WM_IPSEN_CASTLE_X 0xBFE0
#define WM_IPSEN_CASTLE_Z -0x1DA40
#define WM_IPSEN_CASTLE_R 0x5A0
/* low-flight effects (wm_controlFly; y grows downward): spray and dust spawn only below these heights, and their
 * strength (g_worldSprayTarget) falls off with the height */
#define WM_AIRSHIP_SPRAY_ALT -0x800 /* WM_FX_AIRSHIP_SPRAY over water */
#define WM_SAND_DUST_ALT -0x708 /* WM_FX_AIRSHIP_SAND / WM_FX_CHOCOBO_SAND over WM_TOPO_SAND */
#define WM_CHOCOBO_SPRAY_ALT -100 /* WM_FX_CHOCOBO_SPRAY over water: the gold chocobo skimming the surface */
/* wm_drawActors: actors are drawn only inside this box (view space, and world distance from the camera target) */
#define WM_ACTOR_VIEW_NEAR 2001
#define WM_ACTOR_VIEW_FAR 29000
#define WM_ACTOR_VIEW_SIDE 20000
/* world-map sound numbers (w_musicGetID; wm_sePlay & co., script requests WM_REQ_SE_*, WmMoveMode.loopSe,
 * g_worldAmbientSe). 0-5 and 39 play on SND_SE_GROUP_MENU, 6-7 and 20-24 on WM_SE_GROUP_VEHICLE, 25-26 on
 * WM_SE_GROUP_AMBIENT; 20-26 and 33 are two-voice sounds. Named from the code's tables, or for the script-only ones from runtime (R43). */
/* SE group masks (WmSeIds.channelMask): wm_sePlay shifts the mask right by one per sound, so a two-sound entry's
 * second sound plays on mask >> 1. Runtime (R41): sailing, wm_soundCommand got (0xD301, 0x5D9, 0x8000) and
 * (0xD301, 0x5DA, 0x4000) for WM_SE_BOAT_LOOP. WM_SE_GROUP_AMBIENT is code only. */
#define WM_SE_GROUP_VEHICLE 0x8000 /* vehicle loops 20-24 (second sound 0x4000), and 6-7 */
#define WM_SE_GROUP_AMBIENT 0x800  /* the ambient sources 25-26 (second sound 0x400) */
#define WM_SE_BUZZER 1 /* SE_BUZZER */
#define WM_SE_HELP_OPEN 5 /* SE_HELP_OPEN */
#define WM_SE_CURSOR 39 /* SE_CURSOR */
#define WM_SE_ITEMS 2 /* SE 0x6C, the sound ovl_65 plays for items received (SE_ITEMS) and ovl_15 for a Blackjack */
                      /* win; runtime (R43): each "Found N <item>!" line of a chocobo dig treasure */
/* Sounds only the world script requests (op C4: WM_REQ_SE_PLAY with the number). Runtime (R43): log of wm_sePlay /
 * w_musicGetID during each event (docs/runtime-findings.md "R43 runtime pass: world map", row 3). Unknown: 0 (SE
 * 0x68, no world script requests it) and 34 (0x44C, only in an unreached chocobo script tag after 30). */
#define WM_SE_CHEST_OPEN 3 /* 0x27D: with 4, the dug-up treasure chest opens */
#define WM_SE_CHEST_OPEN_2 4 /* 0x27E */
#define WM_SE_MOOGLE_APPEAR 6 /* 0x552: the moogle arrives (its "Can I help you, kupo?" window opens) */
#define WM_SE_TENT_START 7 /* 0x553: "Rest" chosen at the moogle's tent question, the screen starts to darken */
#define WM_SE_FLEET_LOOP 20 /* 0x374 + 0x375: world file 9001 (disc 1 cutscene with the fleet of airships) (low) */
#define WM_SE_AIRSHIP_CUTSCENE_LOOP 21 /* 0x376 + 0x377: world files 9004-9006 (one airship over the mist) (low) */
#define WM_SE_DEAD_PEPPER 30 /* 0x44B: the chocobo's reaction to "Feed him a Dead Pepper" */
#define WM_SE_TENT_REST 31 /* 0x4CF: the screen is black, before the night scene with the tent */
#define WM_SE_MOOGLE_CALL 32 /* 0x596: Square on the world map, Zidane calls the moogle */
#define WM_SE_CHOCOBO_DIG 33 /* 0x37A + 0x37B: Square on the chocobo at a dig spot (with WM_SE_CHOCOBO_CALL) */
#define WM_SE_CHOCOBO_STEP_2 35 /* 0x37C: chocobo step on topographies 0x31 / 0x33 / 0x34 / 0x36-0x39 (low) */
#define WM_SE_CHOCOBO_PECK 37 /* 0x37A: every 9 frames while the chocobo pecks at the glowing ground (Dead Pepper) */
#define WM_SE_CHOCOBO_STEP 40 /* 0x37B: chocobo step on topographies 0x30 / 0x32 / 0x35 (low) */
#define WM_SE_CHEST_RISE 41 /* 0xA47: the dug-up treasure chest shoots out of the ground */
#define WM_SE_CHEST_LAND 42 /* 0xC26: the chest lands next to the chocobo */
#define WM_SE_CHOCOBO_CALL 36 /* SE_CHOCOBO_CALL; runtime (R43): also with WM_SE_CHOCOBO_DIG */
#define WM_SE_INVINCIBLE_LOOP 22 /* data: WmMoveMode.loopSe of WM_MOVE_INVINCIBLE (0x378 + 0x379) */
#define WM_SE_HILDA_GARDE_LOOP 23 /* data: loopSe of WM_MOVE_HILDA_GARDE (0x554 + 0x555) */
#define WM_SE_BOAT_LOOP 24 /* data: loopSe of WM_MOVE_BOAT, volume follows its speed (wm_controlSteer; 0x5D9 + 0x5DA) */
#define WM_SE_RUMBLE_LOOP 25 /* data: g_worldAmbientSe[WM_AMBIENT_EARTH_SHRINE], which also shakes the camera */
                             /* (wm_updateMap; 0x37D + 0x37E); runtime (R41b): played there (Triquai Plains) */
#define WM_SE_WIND_LOOP 26 /* data: g_worldAmbientSe[0] (at WM_BLOCKFX_CLAY, silenced by WM_FORM_CLAY) and [1] (the */
                           /* WM_TOPO_GUSTY_VALLEY source) (0x37F + 0x380); runtime (R41b): played at source 1, */
                           /* recorded as low-frequency wind and hiss */
#define WM_SE_AIRSHIP_WATER 38 /* the airships' loop over water: started with the Hilda Garde / Invincible move modes, volume follows speed and height over water (w_movementChange, wm_controlFly) */

/* frame pack slot list: g_worldReadtxHead (head) -> g_worldReadtxSlots[6] -> g_worldReadtxTail (tail) */
typedef struct WmReadtxSlot {
    struct WmReadtxSlot *prev;
    struct WmReadtxSlot *next;
    s32 *pack; /* frame pack */
    s16 texId; /* WM_TEXID_MASK: id; -1: none */
    u8 extracted;
    u8 padF;
} WmReadtxSlot; /* 0x10 */

/* WmCameraState is in ff9.h (GameData.worldCamera) */
typedef struct {
    u8 terrainClass;
    u8 shadowWidth;
    u8 shadowDepth;
    u8 flying;
    u8 rideMode; /* the g_worldMoveMode of riding this type (data, runtime pass: 3-7 -> 1-5, 8-10 -> 7-9) */
    u8 polyCache;
    u8 shadowJoint;
    u8 shadowOtBias;
    u32 cellAttr;
    s16 rideOffset;
    s16 floorY;
} WmActorType; /* 0x10 */
/* EvtObject.actorType on the world map, index into g_worldActorTypeInfo (wm_findVehicles; world scripts set it
 * with op 0x3B before the model, docs/runtime-findings.md R30b) */
#define WM_ACTOR_NONE 0 /* not a world actor: every code-only (kind 2) object (runtime); wm_canCallChocobo excludes it */
#define WM_ACTOR_WALKER 1 /* party leader on foot (model 310), g_worldWalkerActor */
#define WM_ACTOR_WALKER_ALT 2 /* also g_worldWalkerActor (model 309, only in world file 2) */
#define WM_ACTOR_CHOCOBO0 3 /* 3-7: g_worldMountActor; wm_setMountPalette picks palette 0-4 by type; */
#define WM_ACTOR_CHOCOBO1 4 /* move modes 1-6 play WM_SONG_CHOCOBO, and only walkers can call one */
#define WM_ACTOR_CHOCOBO2 5 /* (wm_canCallChocobo) */
#define WM_ACTOR_CHOCOBO3 6
#define WM_ACTOR_CHOCOBO4 7 /* WmActorType.flying while g_worldMoveMode is 6 (wm_updateActors); the gold chocobo (runtime) */
/* 8-10: g_worldVehicleActor (wm_findVehicles); boarding is decided by the world script, not by the type */
#define WM_ACTOR_BOAT 8 /* the Blue Narciss (from data: not flying, floorY forced 0, no shadow; rideMode WM_MOVE_BOAT) */
#define WM_ACTOR_AIRSHIP 9 /* the Hilda Garde 3 (runtime, disc 3; rideMode WM_MOVE_HILDA_GARDE) */
#define WM_ACTOR_AIRSHIP2 10 /* the Invincible (from data: flying, rideMode WM_MOVE_INVINCIBLE) */
#define WM_ACTOR_MOOGLE 11 /* g_worldMoogleActor (model 307), comes when called with Square */
#define WM_ACTOR_NO_MODEL 14 /* wm_drawActorModel and wm_drawActorShadow draw nothing for it */

/* camera preset, blended by wm_updateCameraPreset / wm_calcCameraPreset */
typedef struct {
    s16 projDist; /* GameData.unkA (SetGeomScreen) */
    s16 distance; /* eye distance from the target (x/z, scaled by g_worldCameraDistScale) */
    s16 eyeHeight; /* eye y offset */
    s16 groundClearance; /* the eye stays at least this far above the ground under it (y up is negative) */
    s16 targetHeight; /* g_worldCameraTargetHeight */
} WmCameraParams; /* 10 bytes */

/* movement mode table entry (g_worldMoveModes[], current one at g_worldMoveModeInfo) */
typedef struct {
    u8 controlType; /* mode */
    u8 checkTransitions;
    s16 speed; /* speed */
    u8 turnSpeed; /* turn speed */
    u8 climbSpeed;
    s8 maxRoll;
    u8 pad7;
    u8 airborne;
    u8 canToggleView;
    u8 lookAhead;
    u8 music;
    u8 loopSe;
    u8 countsSteps;
    s16 getoffDist;
    u8 cameraInFront;
    u8 cameraRow;
    u8 pad12[2];
    u32 allowedCells[2]; /* allowed cell types (bit mask) */
} WmMoveMode; /* 0x1C */
/* g_worldMoveMode (event var b[0xBE], w_movementChange): index into g_worldMoveModes, whose entries give
 * control type, music and loop SE */
#define WM_MOVE_WALK 0 /* WM_CTRL_WALK, counts steps */
/* 1-6: WM_SONG_CHOCOBO, riding g_worldMountActor. 1-5 = the chocobo's ability count (event byte b[0xBF], runtime),
 * the allowed cells grow with it */
#define WM_MOVE_CHOCOBO_FIELD 1 /* same cells as walking */
#define WM_MOVE_CHOCOBO_REEF 2
#define WM_MOVE_CHOCOBO_MOUNTAIN 3
#define WM_MOVE_CHOCOBO_OCEAN 4
#define WM_MOVE_CHOCOBO_SKY 5 /* the gold chocobo on the ground */
#define WM_MOVE_CHOCOBO_FLY 6 /* last chocobo mode: WM_CTRL_FLY, airborne; WM_ACTOR_CHOCOBO4 flies */
#define WM_MOVE_BOAT 7 /* WM_CTRL_STEER, loop SE 24: the Blue Narciss (from data) */
#define WM_MOVE_HILDA_GARDE 8 /* 8, 9: WM_CTRL_FLY with WM_SONG_AIRSHIP and a loop SE; 8 = Hilda Garde 3 (runtime) */
#define WM_MOVE_INVINCIBLE 9 /* loop SE 22, longer get-off distance (from data) */
#define WM_MOVE_FAST_WALK 10 /* wm_movePlayer while g_worldFastWalk: 4x speed */

/* doubly linked node (wm_frmbufInit) */
typedef struct WmFrmbufSlot {
    struct WmFrmbufSlot *prev;
    struct WmFrmbufSlot *next;
    s16 texId;
    s8 vramArea;
    u8 transferred;
} WmFrmbufSlot; /* 0xC */

/* model object (same layout in ovl_0a, ovl_0b and ovl_0c) */

/* primitive sources of a model group; index fields select vertices (unk0..),
 * uv pairs (WmModelGroup.unk20) and colours (WmModelGroup.unk24) */
typedef struct {
    u16 vertices[4];
    u16 uvs[4]; /* uv */
    u8 rgb[3]; /* rgb */
    u8 texture;    /* texture */
    u8 unk14; /* not read by C */
    u8 flags; /* WMSRC_* */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

typedef struct {
    u16 vertices[3];
    u8 texture; /* texture */
    u8 pad7;
    u8 rgb[3]; /* rgb */
    u8 padB;
    u16 uvs[3]; /* uv */
    u8 flags;    /* flags */
    u8 pad13;
} WmSrcFT3; /* 0x14 */

typedef struct {
    u16 vertices[4];
    u16 uvs[4];  /* uv */
    u16 colors[4]; /* colours */
    u8 pad18[4];
    u8 flags; /* flags */
    u8 texture; /* texture */
    u8 pad1E[2];
} WmSrcGT4; /* 0x20 */

typedef struct {
    u16 vertices[3];
    u16 uvs[3]; /* uv */
    u8 padC[3];
    u8 flags;      /* flags */
    u16 colors[3]; /* colours */
    u8 texture;     /* texture */
    u8 pad17;
} WmSrcGT3; /* 0x18 */

typedef struct {
    u16 vertices[4];
    u16 colors[4]; /* colours */
    u8 pad10[4];
    u8 flags; /* flags */
    u8 pad15[3];
} WmSrcG4; /* 0x18 */

typedef struct {
    u16 vertices[3];
    u8 pad6[2];
    u16 colors[3]; /* colours */
    u8 padE[5];
    u8 flags; /* flags */
} WmSrcG3; /* 0x14 */

typedef struct {
    u16 vertices[4];
    u8 rgb[3]; /* rgb */
    u8 padB[2];
    u8 flags; /* flags */
    u8 padE[2];
} WmSrcF4; /* 0x10 */

typedef struct {
    u16 vertices[3];
    u8 pad6[2];
    u8 rgb[3]; /* rgb */
    u8 flags; /* flags */
} WmSrcF3; /* 0xC */
/* WmSrc*.flags (same as ovl_0a) */
#define WMSRC_SEMITRANS 0x1 /* the packet builder sets the semi-transparency bit */
#define WMSRC_DOUBLE_SIDED 0x2 /* the mesh draw functions skip the nclip backface test */
#define WMSRC_TPAGE_BITS 0xE0 /* with WMSRC_SEMITRANS: ORed into the tpage (abr bits 5-6, and bit 7) */

typedef struct {
    u16 packetSize;
    u16 ft4Count;  /* FT4 count */
    u16 ft3Count;  /* FT3 */
    u16 gt4Count;  /* GT4 */
    u16 gt3Count;  /* GT3 */
    u16 g4Count;  /* G4 */
    u16 g3Count;  /* G3 */
    u16 f4Count;  /* F4 */
    u16 f3Count; /* F3 */
    u8 pad12;
    s8 otBias; /* ordering table bias */
    void *boneVertexCounts;
    void *vertices;
    u8 *prims;      /* primitive sources, in the order above */
    u8 (*uvs)[2]; /* uv */
    u8 (*colors)[4]; /* colours */
} WmModelGroup; /* 0x28 */

typedef struct {
    u16 flags; /* WMMODEL_* */
    u8 boneCount;
    u8 groupCount; /* group count */
    u16 packetSize; /* sum of the groups' packetSize (ovl_0a WmModel, docs/field-models.md) */
    u8 lookAtBone; /* ovl_0a fldmdl_calcBoneMatricesLookAt; the world map has no look-at variant */
    u8 pad7[0xC - 7];
    s32 bones;
    WmModelGroup *groups;
} WmModel;
/* WmModel.flags */
#define WMMODEL_RELOCATED 0x1 /* the model init (wm_model.c) turned the file offsets into pointers */

typedef struct {
    s16 unk0; /* write-only (cleared) */
    u8 pad2[2];
    void *prims; /* first primitive */
    s32 unk8; /* write-only (cleared) */
} WmModelOut; /* 0xC */

typedef struct {
    u16 tpage; /* tpage */
    u16 clut; /* clut */
} WmTexInfo;

/* skeleton bone (WmModel.unkC, 4 bytes) */
typedef struct {
    s16 length; /* length: z offset from the parent */
    u8 unk2; /* not read by C */
    u8 parent; /* parent bone */
} WmBone;

/* animation track: three 16-bit offsets/constants (x, y in xy, z in zFlags)
 * and their flags in the high half of zFlags (see gte_ldrotkey) */
typedef struct {
    u32 xy; /* x (low half), y (high half): offset of the axis's per-frame bytes, or the constant; read only by the gte_ldrotkey asm */
    u32 zFlags; /* z (low half) the same; high half: 1 x, 2 y, 4 z is a constant (gte_ldrotkey asm only) */
} WmAnimKey;

/* animation file header: offsets relocated by wm_loadActorModel */
typedef struct WmAnimHdr {
    u8 pad0[4];
    s16 rootPos[3]; /* root position: constant, or offset of s16[frame] */
    u16 constMask;    /* WMANIM_ROOT_CONST(i): rootPos[i] is a constant */
    s32 rotKeys;    /* -> WmAnimKey[], rotation tracks, one per bone */
    s32 rotKeysLo;   /* -> WmAnimKey[], low rotation bits, two frames per byte */
} WmAnimHdr;
/* WmAnimHdr.constMask: axis i of the root position is rootPos[i] itself instead of an offset of a per-frame s16
 * track (wm_calcBoneMatrices). Evidence: [code] */
#define WMANIM_ROOT_CONST(i) (1 << (i))

typedef struct ModelObj {
    u16 flags; /* flags; 0x10: behind the camera */
    u8 pad2[2];
    u8 parentBone; /* bone in the parent */
    u8 pad5[3];
    WmModel *model;
    struct WmAnimHdr *anim;
    u8 *packets; /* primitive buffer */
    s32 hiddenGroups;
    WmTexInfo *texInfo;
    struct ModelObj *parent; /* parent */
    u8 *boneMatrices;
    MATRIX worldMatrix; /* t: position */
    WmModelOut *groupOut;
    u8 pad48[0x50 - 0x48];
    s32 texAnims;
    u16 scaleX; /* scale */
    u16 scaleY;
    u16 scaleZ;
    u16 clipY;
    s32 unk5C; /* write-only (cleared) */
    s32 mirrorSrc;
    u8 pad64[0x74 - 0x64];
    u16 attachX;
    u16 attachY;
    u16 attachZ;
} ModelObj;
/* ModelObj.flags: MDL_* (ff9.h; wm_applyScale, wm_cullBehindCamera use them as ovl_0a does) */

/* object model slot (game_getModel): ovl_0c's view of ModelSlot (ff9.h), with the world map's model type */
typedef struct {
    s32 flags; /* WM_SLOT_* */
    s32 modelId; /* model item */
    ModelObj *model;
    EvtObject *actor;
} WmObjSlot;
/* WmObjSlot.flags (ModelSlot.flags) */
#define WM_SLOT_LOADED MODEL_SLOT_LOADED /* wm_loadActorModel built the model */
#define WM_SLOT_NO_SHADOW MODEL_SLOT_NO_SHADOW /* wm_drawShadow skips the object (FLD_OBJ_NO_SHADOW in the field) */
s32 evt_getObjectChrIndex(EvtObject *);
void wm_drawMesh(ModelObj *, u8);
void wm_projectMesh(ModelObj *, u8);
void wm_applyScale(ModelObj *);
void wm_calcBoneMatrices(ModelObj *, u16);
void wm_drawShadow(WmObjEnt *, s32, s32, s32); /* draw a shadow */
void wm_projectMeshClipY(ModelObj *, u8);
void wm_drawMeshClipY(ModelObj *, u8);
u8 *db_getModelTexPages(s32);
u8 *db_getModelUvOffsets(s32);

/* relocatable block: offsets are turned into pointers by wm_relocatePack */
typedef struct {
    s32 unk0; /* not read by C */
    s32 verts;
    s32 polys;
} WmPackEntry;

typedef struct {
    s8 count; /* count */
    u8 pad1[3];
    s32 unk4; /* relocated by wm_relocatePack, not read otherwise */
    WmPackEntry *entries[1];
} WmPack;

/* text windows (menuMain_setCursor .. menuMain_updatePanels) */
typedef struct {
    u8 cursor;
    u8 state; /* index into s_menuMainInput */
    u8 orderCursor; /* party slots being swapped */
    u8 orderTarget;
    u8 orderPicked;
    u8 pad5[3];
    WinDef panels[6];
} MenuMain;
/* MenuMain.state (menuMain_selectCommand) */
#define MENUMAIN_COMMAND 0 /* picking a command */
#define MENUMAIN_MEMBER 1 /* picking the party member for it (menuScreen_needsMember) */
#define MENUMAIN_ORDER 2 /* Order: swapping party members */

/* map cell (4 bytes) */
typedef struct {
    u16 sector;
    u8 packSectors;
    u8 texSectors;
} WmChunkEntry;

typedef struct {
    s16 width; /* width */
    s16 height; /* height */
    WmChunkEntry entries[1];
} WmChunkTable;

/* filled by w_musicGetID */
typedef struct {
    s32 channelMask; /* third argument of wm_soundCommand, shifted right by one per sound (wm_sePlay) */
    s32 count; /* count */
    s32 seIds[3]; /* sound ids */
} WmSeIds;

typedef struct {
    s32 pos[4];
} WmBlockPos; /* 0x10 */

/* list nodes (wm_blockListCompact, wm_chunkListCompact): g_worldBlockHead (head) ->
 * g_worldBlockNodes[40] -> g_worldBlockTail (tail) */
typedef struct WmBlockNode {
    struct WmBlockNode *prev;
    struct WmBlockNode *next;
    struct WmBlock *block;
    WmBlockPos *pos;
    s16 cell;
    u8 state; /* WM_BLOCK_* */
    s8 slot;
} WmBlockNode; /* 0x14 */
#define WM_BLOCK_NONE 0 /* no block yet (wm_blockListRequest, wm_blockListResolve) */
#define WM_BLOCK_SET 2 /* wm_blockListResolve set block (the loaded chunk's, or the default block); wm_drawBlocks draws it */

/* list: g_worldChunkHead (head) -> g_worldChunkSlots[32] -> g_worldChunkTail (tail) */
typedef struct {
    u8 v0; /* vertex indices */
    u8 v1;
    u8 v2;
    u8 pad3[0xE - 3];
    u16 attr; /* attribute */
} WmPoly; /* 0x10 */

/* terrain mesh block (4 per map chunk): wm_getBlockHeight walks its triangles, wm_drawBlock draws it */
typedef struct WmBlock {
    u8 unk0; /* not read by C */
    u8 texArea;
    u16 polyCount; /* polygon count */
    SVECTOR *verts; /* vertices */
    WmPoly *polys;
    u8 outline[0x14]; /* vertex indices of the on-screen test (wm_updateVisibleBlocks) */
    u8 outlineCount; /* vertex count */
    u8 fxId; /* block effect id (wm_drawBlockFx, wm_updateBlockFx) */
} WmBlock;

/* visible cells sorted by distance (wm_updateVisibleBlocks) */
typedef struct {
    s32 cell; /* cell */
    WmBlock *block;
    s32 dist; /* distance */
} WmNearCell;

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} WmScrRect;

/* chunk pack read by wm_readChunks: the 4 terrain blocks of one map chunk (wm_getCellBlock) */
typedef struct WmChunkPack {
    u8 unk0; /* not read by C */
    u8 unk1; /* not read by C */
    u8 texArea;
    u8 unk3; /* not read by C */
    s32 unk4; /* not read by C */
    WmBlock *blocks[4];
} WmChunkPack;

typedef struct WmChunkSlot {
    struct WmChunkSlot *prev;
    struct WmChunkSlot *next;
    struct WmChunkPack *pack;
    s16 chunkId;
    u8 state;
    u8 generation;
    u8 hasData;
    u8 pad11[3];
} WmChunkSlot; /* 0x14 */
/* WmChunkSlot.state */
#define WM_CHUNK_WANTED 0 /* wm_chunkRequest took the slot; wm_findChunkRun picks runs of these */
#define WM_CHUNK_READING 1 /* in the run wm_readChunks is reading */
#define WM_CHUNK_READY 2 /* wm_chunksLoaded relocated the pack; blocks can use it */

/* colour + 2 values (8 bytes) */
typedef struct {
    CVECTOR color;
    u16 fogOffset;
    u16 fogOffset2;
} WmFogParam;

typedef struct {
    u16 fogOffset;
    s16 fogOffset2;
    s16 color[3]; /* rgb */
    s16 pad;
} WmFog; /* 0xC */

typedef union {
    SVECTOR v;
    u8 c[8];
} WmSkyCol;

/* sky / lighting parameter block (0x5E bytes); wm_blendWeather blends them */
typedef struct {
    SVECTOR lightColor[3]; /* light colours: columns of g_worldMapColorMatrix (wm_updateColorMatrices) */
    SVECTOR backdropLightColor;
    WmSkyCol backColor; /* back colour 1 */
    WmSkyCol backdropBackColor; /* back colour 2 */
    WmFog fog[3];
    s16 fxBackColor[3];
    u8 pad5A[2];
    u16 shadowShade; /* scales the actor shadows' shade (4096 = 1.0, wm_drawActorShadow) */
} WmSky; /* 0x5E */

typedef struct {
    WmSky presets[16]; /* presets: [mode * 4 + flag * 2 + 0/1] */
    WmSky work[7]; /* [0] current, [1]/[2] low/high-altitude sky, [3]/[5] blend sources, [4]/[6] blend targets (wm_weatherUpdate, wm_setWeather) */
} WmWeatherData;
void wm_blendWeather(WmSky *, WmSky *, WmSky *, s32, s32);
void wm_weatherAuto(void);
void wm_updateFogParams(void);
void wm_updateColorMatrices(void);

typedef struct {
    s16 tpage;
    s16 clut;
    u8 u;
    u8 v;
    u8 pad6[2];
    s16 frmTpage; /* tpage */
    s16 frmClut; /* clut */
} WmTexArea; /* 0xC */

typedef struct {
    s16 mapX;
    s16 mapY;
    s32 worldX; /* x */
    s32 worldZ; /* z */
} WmNaviPlace; /* 0xC */

/* file 0x12xxxx lookup table (wm_findDefaultAnim) */
typedef struct {
    u16 modelId;
    u16 animId;
} WmAnimTableEntry;

typedef struct {
    u8 unk0; /* not read by C */
    u8 count; /* count */
    u8 pad2[2];
    WmAnimTableEntry entries[1];
} WmAnimTable;

/* streamed load request (wm_streamSoundChunk) */
typedef struct {
    u8 pad0[8];
    s32 addr; /* address */
    s32 offset; /* offset */
    s32 size; /* size */
} WmStreamReq;

/* POLY_F4 with the screen coordinates as packed words */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s32 xy0;
    s32 xy1;
    s32 xy2;
    s32 xy3;
} WmPolyF4;

/* ---- overlay functions ---- */
s32 wm_getLeftStickX(s32 *);
s32 wm_getLeftStickY(s32 *);
s32 wm_getRightStickY(s32 *);
void wm_spawnEffect(s32, s32, s32, s32, s32);
s32 wm_freeFile(WorldMapObj *, s16);
s32 wm_depFindBattle(s16, s32 *, s32 *);
s32 wm_depFindBackground(s32, s32 *, s32 *);
s32 wm_depFindField(s16, s32 *, s32 *);
s32 wm_depFindArea(s32, s32 *, s32 *);
void w_frameMapDestructor(void);
void menuMain_swapPortraits(s32, s32);
void wm_blockVertsFog(void *);
s32 wm_blockFacesFog(void *, s32, s32);
void wm_blockVertsFar(void *);
s32 wm_blockFacesFar(void *, s32, s32);
void wm_blockVertsNear(void *);
s32 wm_blockFacesNear(void *, s32, s32);
s32 wm_getWeatherByPos(void);
s32 wm_getStoryFog(void);
void w_musicRequestSend(u32);
void wm_seFadeVolume(s32, u16, u8);
void wm_blendCameraPreset(s16, s16);
void wm_autoPilotOff(void);
void menuMain_selectCommand(void);
void wm_seStop(s32);
void wm_sePlay(s32, u8);
void wm_seSetVolume(s32, u8);
void wm_updateActors(void);
s32 w_memorymanHeapAlloc(s32);
void wm_setWeather(s32, s32, s32);
s32 wm_getTerrainHeightOffset(s32, u32, u8 *);
void wm_updateActorHeight(EvtObject *);
void wm_modelVerts(s32);
void wm_modelVertsFlat(s32);
s32 wm_modelFaces(s32, s32, s32);
s32 wm_modelFacesFog(s32, s32, s32);
void menuMain_drawMember(s32);
void menuMain_drawPortraitShade(RECT *, s32);
void wm_controlWalk(void);
void wm_controlSteer(void);
void wm_controlFly(void);
void menuMain_initPanels(void);
void menuMain_updateHelp(void);
void wm_frameTerm(void);
void w_memorymanDestructor(void);
void wm_mapSystemTerm(void);
void wm_musicTerm(void);
void wm_termEffects(void);
void w_movementChange(void);
void w_musicUpdate(void);
/* TIM image blocks */
typedef struct {
    u32 length;
    RECT rect;
    u16 data[1];
} WmTimBlk;

typedef struct {
    u32 id;
    u32 flags; /* 8 = has CLUT */
    WmTimBlk block;
} WmTim;

void wm_loadTim(u32 *, s32, s32, s32, s32, s32, s8);
void menuMain_setExpHelp(s32);
void menuMain_setEquipHelp(u8);
s32 wm_getPadHeld(void);
s32 wm_getPadPressed(void);
s32 w_musicGetID(s32, WmSeIds *);
s32 w_frameExtruct(s32 *);
void wm_backdropVerts(s32);
s32 wm_backdropFaces(s32, s32, s32);
void wm_copyViaVram(s32, s32, s32);
void wm_mapTerm(void);
void wm_stopAllSe(void);
void wm_applyEncounterEvents(void);
void wm_frameSetup(void);
void wm_initClutAnims(void);
void wm_setMountPalette(void);
void wm_findVehicles(void);
void wm_weatherStart(void);
void wm_musicSetup(void);
void wm_main(s32);
void wm_init(void);
void wm_shutdown(void);
void wm_allocContext(void);
void wm_resetContext(void);
s32 wm_loadSoundBanks(void);
s32 wm_loadResidentMusic(void);
s32 wm_loadShadowTexture(void);
s32 wm_loadWorldData(void);
s32 wm_afterLoadSoundBanks(void);
s32 wm_afterLoadWorldData(void);
s32 wm_afterLoadResidentMusic(void);
s32 wm_freeWorldData(void);
s32 wm_afterLoadShadowTexture(void);
void wm_load();
void wm_exit(void);
u32 wm_loadActorModel(s32, EvtObject *);
void wm_clearOT(void);
void wm_endFrame(void);
void wm_startBattleTransition(void);
void wm_updateBattleTransition(void);
void wm_togglePause(void);
s32 wm_preloadRequest(void);
s32 wm_frameMain(void);
void wm_systemInit(void);
void w_frameSystemDestructor(void);
s32 wm_keepEntryMusic(void);
s32 wm_soundCommand(s32, s32, s32, s32, s32);
void wm_cameraFollow(void);
void wm_updateCameraPreset(void);
void wm_snapActorsToFloor(void);
void wm_scrollTextures(void);
void wm_animTextures(void);
void wm_animCluts(void);
void wm_initTexPages(void);
void wm_readtxInit(void);
void wm_readtxUpdate(void);
void wm_frmbufInit(void);
void wm_frmbufUpdate(void);
s32 wm_getSongMusicId(u16);
void menuMain_drawTimeGil(void);
void menuMain_drawLocation(void);
void menuMain_drawCursor(void);
void menuMain_input(void);
void menuMain_draw(void);
void menuMain_updatePanels(void);
void menuMain_drawParty(void);

/* ---- overlay data ---- */
extern RECT *s_menuMainTimeGilRect;
extern u8 s_menuMainGilSuffix[];
extern u8 g_worldFog;
extern u8 g_worldCellTriggersOn;
extern WmTexArea *g_worldCurTexArea;
extern u8 *s_menuMainEquipLabels[5];
extern char s_wmStrFrameExtructExtract[];
extern char s_wmStrFrameExtructAlready[];
extern RECT *s_menuMainLocationRect;
extern s16 g_worldCameraBlendSpeed;
extern s16 g_worldCameraBlend;
extern s16 g_worldCameraBlendRow;
extern s8 g_worldCameraGroupOverride;
extern s32 g_worldStickDiv;
extern s32 g_worldSpeedSmoothDiv;
extern s32 g_worldSpeedScaleDiv;
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */
extern s32 g_worldAutoTurnBoost;
extern s32 g_worldAutoTurnDiv;
extern s32 g_worldAutoTurnDivMin;
extern s32 g_worldAutoTurnDivMax;
extern s32 g_worldAutoTurnDeadZone;
extern u8 g_worldAreaCameraGroup[];
extern u8 g_worldAreaZone[];
extern u8 g_worldEncounterZoneStart[];
extern char s_wmStrEncounterNoScene[];
extern WmCameraParams g_worldCameraParams;
extern WmCameraParams g_worldCameraParamsFrom;
extern u8 g_worldBackdropOn;
extern s32 g_worldReadyFrameCount;
extern MATRIX g_worldBlockFxLightMatrix;
extern MATRIX g_worldBackdropLightMatrix;
extern MATRIX g_worldMapLightMatrix;
extern u8 g_worldMapLoaded;
extern u8 s_menuMainText[]; /* text */
/* s_menuMainText entries (text_get; decoded with tools/ff9text.py text_table) */
#define MENU_MAIN_TEXT_NO_LOCATION 1 /* "Honolulu conversion code": a leftover placeholder, shown when GameData.locationName is empty */
#define MENU_MAIN_TEXT_EQUIP_HELP 4 /* "Select character." + the five equipment lines (string args 0-9) */
#define MENU_MAIN_TEXT_STATUS_HELP 5 /* "Select character." + Exp / Next lines (string args 2-7) */
#define MENU_MAIN_TEXT_EMPTY 0xD /* "": an empty equipment slot */
extern WmActorType g_worldActorTypeInfo[];
extern EvtObject *g_worldPlayer; /* player actor */
extern WmMoveMode *g_worldMoveModeInfo;
extern s32 g_worldWidth; /* map width (wraps) */
extern s32 g_worldDepth; /* map height (wraps) */
/* the world's size as the code wraps distances with constants (g_worldWidth / g_worldDepth hold the same from the
 * cell map: 24 x 20 chunks of 0x4000, WM_CELL); a distance beyond half of it is shorter the other way round.
 * Evidence: [code] wm_worldToView, wm_updateCamera, wm_controlFly's autopilot */
#define WM_MAP_SIZE_X 0x60000
#define WM_MAP_SIZE_Z 0x50000
extern RECT g_worldVramParkRect;
extern char s_wmStrFrameSystemDestructor[]; /* "w_frameSystemDestructor:\n" */
extern WmNaviPlace g_worldNaviPlaces[][64];
extern s32 *g_worldTexBuffers[3];
extern s32 g_worldDataArea; /* 0x80100800, end of the world heap: loaded actor meshes, world script, images */
extern u8 g_worldPointFxBuf[];
extern u8 *g_worldPointFxBufPtr;
extern s32 g_worldPrimBuffers[];
extern MATRIX g_worldBackdropColorMatrix;
extern MATRIX g_worldMapColorMatrix;
extern MATRIX g_worldBlockFxColorMatrix;
extern u16 g_worldClutBuf[2][0x800]; /* palettes, [1] = shifted copy */
/* texture animation entries (wm_stepClutAnims) */
typedef struct {
    s16 x;
    s16 y;
    s16 start;
    s16 count; /* frame count */
    s16 speed; /* speed */
} WmClutAnim; /* 0xA */
extern WmClutAnim g_worldClutAnims[];
extern s16 g_worldClutAnimAcc[];
extern u8 g_worldClutAnimFrame[];
extern u16 g_worldClutAnimOffset[];
extern WmTexArea g_worldTexAreas[17];
extern s8 g_worldNaviPage;
extern char s_wmStrReadtxDone[];
extern s32 g_worldPrimPtr;
extern WmChunkTable *g_worldChunkTable;
extern WmChunkTable *g_worldChunkTableT;
extern u8 *g_worldBufIndex;
extern s32 g_worldOt[];
extern s32 g_worldEventResult;
extern s32 g_worldTimeEvents;
extern s32 g_worldTimeLogic; /* frame timing */
extern s32 g_swirlFrame;
extern s32 g_swirlBuf;
extern s32 g_worldTimeDraw;
extern s32 g_worldPrimUsed;
extern s32 g_worldPrimUsedPercent;
extern s32 g_worldTimeTotal;
extern WmFogParam g_worldBackdropFog;
extern WmFogParam g_worldFogParams[2];
extern s32 g_worldChunksH;
extern WmBlockNode g_worldBlockHead; /* list head */
extern s32 g_worldBlockListDirty;
extern WmBlockNode g_worldBlockNodes[40];
extern WmBlockPos g_worldBlockPositions[20];
extern WmBlockNode g_worldBlockTail; /* list tail */
extern WmChunkSlot g_worldChunkTail;
extern WmChunkSlot g_worldChunkHead;
extern WmChunkSlot g_worldChunkSlots[32];
extern s32 g_worldChunkCenter;
extern s32 g_worldChunksLoadedCount;
extern s32 g_worldChunkListDirty;
extern WmWeatherData g_worldWeatherData;
extern char s_wmStrFrameMapDestructor[]; /* "w_frameMapDestructor:\n" */
extern char s_wmStrFramePackGetPtrRange[]; /* "w_framePackGetPtr : attempt to get %d element with %d max\n" */
extern void (*s_menuMainInput[])(void);
extern u8 g_worldSongMusicIds[]; /* [WM_SONG_*] -> music id: MUSIC_WORLD_MAP, MUSIC_CHOCOBO, MUSIC_AIRSHIP,
                                   * MUSIC_WORLD_DISC4, 0x5F, 0x60 (the two field songs, not heard). Data, the same on
                                   * every disc ([runtime] R41b) */
extern WmCameraState *g_worldCamera;
extern s8 g_worldMoveMode;
extern s32 g_worldHeapUsed;
extern s32 g_worldHeapDepth;
extern s32 g_worldHeapTop;
extern s32 g_worldHeapLastSize;
extern u16 *g_worldStoryCounterPtr;
extern u8 g_worldDataSet;
extern s32 g_worldCellsW;
extern WmChunkPack *g_worldDefaultBlocks;
extern s32 g_worldChunksW;
extern MATRIX g_worldBlockMatrix;
extern s32 g_worldMapDebugFlags; /* debug flags */
extern u8 g_worldPlayerStepped;
extern u16 g_worldEventBattleRate;
extern s32 g_worldReqParams[4]; /* script request parameters */
/* texture transfer requests (wm_frmbufTransfer) */
typedef struct {
    s32 pack;
    u8 area;
    u8 vramArea;
    u8 pad6[2];
} WmTexTransfer;
extern WmTexTransfer g_worldFrmbufTransfers[];
extern s16 g_worldFrmbufAreaX[2];
extern s16 g_worldFrmbufAreaY[2];
extern s16 g_worldFrmbufClutX[2];
extern s16 g_worldFrmbufClutY[2];
extern char s_wmStrFrmbufTransfer[];
extern WmCameraFlags *g_worldCameraFlags;
extern u8 g_worldAutoPilot;
extern u8 g_worldAutoPilotSavedAutoTurn;
extern s32 g_worldAutoPilotDist;
extern s32 g_worldAutoPilotDest;
extern WmReadtxSlot g_worldReadtxTail;
extern WmReadtxSlot g_worldReadtxHead;
extern WmReadtxSlot g_worldReadtxSlots[6];
extern s32 g_worldReadtxReqCount;
extern s32 g_worldReadtxDirty;
extern s8 g_worldMapError;
extern char g_worldMapErrorMsg[];
extern WmEffect g_worldEffectFree;
extern WmEffect g_worldEffectActive;
extern WmEffect g_worldEffectNodes[60]; /* free list nodes */
extern WmEffect g_worldEffectFreeTail; /* list tail */
extern WmEffect g_worldEffectActiveTail; /* list tail */
extern s32 g_worldEffectSpeed;
extern s32 g_worldSprayLevel;
extern s32 g_worldSprayTarget;
extern s32 g_worldEffectSpeedTarget;
extern VECTOR g_worldBlockFx4Pos;
extern SVECTOR g_worldBlockFx6RingRot[10];
extern s16 g_worldBlockFx6RingDist[10];
extern s16 g_worldBlockFx6FlashLen[10];
extern s8 g_worldBlockFx6FlashTimer[10];
extern SVECTOR g_worldBlockFx6FlashRot[10];
extern VECTOR g_worldEffectPos[9];
extern SVECTOR g_worldEffectRot[9];
extern SVECTOR g_worldBlockFx7Rot;
extern SVECTOR g_worldBlockFx4Rot;
extern SVECTOR g_worldBlockFx6RotA;
extern SVECTOR g_worldBlockFx6RotB;
extern SVECTOR g_worldBlockFx4PartRots[3];
extern void **g_worldDataPack;
extern MATRIX *g_worldViewMatrix;
void wm_drawBackdrop(s32, s32);
extern u8 g_worldParkedSoundRectIdx;
extern RECT g_worldParkedSoundRects[];
extern u8 g_worldAreaZone[];
extern u8 g_worldCameraSmoothX;
extern u8 g_worldCameraSmoothY;
extern u8 g_worldCameraSmoothZ;
extern s32 g_worldCameraTarget[];
extern s32 g_worldCameraEye[];
extern s32 g_worldCameraDir[];
extern s32 g_worldChunkLookPos[];
extern s32 g_worldCameraRecenter;
extern s32 g_worldChunkBuffer;
extern u8 g_worldGetoffSearching;
extern u8 g_worldLoadedFrames;
extern u8 g_worldUsePolyCache;
extern u8 g_worldChunkGeneration;
extern u8 g_worldChunkFlags[]; /* map cells, g_worldChunksW per row */
#define WM_CELL(x, y) g_worldChunkFlags[(y) * g_worldChunksW + (x)]
#define WM_CELL_FORM 0x80 /* load the cell's story-form chunk (wm_applyStoryForms sets it; wm_chunkRequest clears it when the chunk has none) */
#define WM_CELL_TEX_MASK 0x3F /* texture area: the texture pack read with the chunk (w_readtxRequest), shared by */
                              /* all cells of the area (wm_readtxFind). Runtime (R43): story form f's textures are */
                              /* texture area f + 1, so a form flags every cell of that area plus the cells whose */
                              /* geometry changes */
#define WM_CELL_TEX_AREA(form) ((form) + 1) /* texture area of story form WM_FORM_* (R43, data) */
/* Map cells (16384 x 16384 world units: column = x >> 14, row = -z >> 14) that wm_applyStoryForms gives their
 * story-form chunk, as "column, row" pairs for WM_CELL_AT. Each is named for the navi map place whose world position
 * (g_worldNaviPlaces, name = world text bank 0 entry place + 1, docs/r32/3ds-requests-2.md) lies in it. Cleyra,
 * Lindblum and Alexandria sit on a cell corner, so their forms change the cells around it. Cells without a navi place
 * of their own are named for the nearest one. Runtime (R43, data comparison of base and form chunks): a form's list is
 * its geometry cells plus every cell of its texture area (WM_CELL_TEX_AREA), so some cells are in a form only for
 * the shared texture and the form changes nothing there. */
#define WM_CELL_AT(cell) WM_CELL(cell)
#define WM_CELL_SOUTH_GATE 18, 14 /* places 6-10 "South Gate" (300090, -230918); SGATE changes only texture area 1 */
                                /* (the gate's round crest becomes a dark opening; R43) */
#define WM_CELL_DALI 17, 12 /* places 13 "Dali" (280253, -206433; WM_BLOCKFX_DALI_WINDMILL) and 12 "Observatory Mountain"; */
                          /* in SGATE through texture area 1 (R43) */
#define WM_CELL_CLEYRA_W 13, 12 /* place 18 "Cleyra" (229548, -198479) is 170 units into the cell east of it; */
#define WM_CELL_CLEYRA_E 14, 12 /* WM_BLOCKFX_CLAY's centre (229007, -198927) is in the west one */
#define WM_CELL_LINDBLUM_NW 13, 16 /* place 24 "Lindblum" (229342, -278220) is 310 units from the corner of these four */
#define WM_CELL_LINDBLUM_NE 14, 16
#define WM_CELL_LINDBLUM_SW 13, 17 /* also place 25 "Lindblum Harbor" */
#define WM_CELL_LINDBLUM_SE 14, 17 /* also place 22 "Pinnacle Rocks" */
/* KUROMA (texture area 9) changes nothing in the data at its three cells: same geometry, same textures (R43) */
#define WM_CELL_BLACK_MAGE_VILLAGE 14, 6 /* places 30 "Black Mage Village" and 31 "Fossil Roo" */
#define WM_CELL_ALEXANDRIA_HARBOR 21, 10 /* place 0 "Alexandria Harbor"; in KUROMA through texture area 9 */
#define WM_CELL_QUANS_DWELLING_E 22, 14 /* no place; east of place 4 "Quan's Dwelling" (21, 14); */
                                   /* in KUROMA only through texture area 9, and KUROMA changes nothing (R43) */
#define WM_CELL_ALEXANDRIA_NW 19, 10 /* place 1 "Alexandria" (328680, -178910) is 1600 units from the corner of these four */
#define WM_CELL_ALEXANDRIA_SW 19, 11 /* also place 2 "Evil Forest" */
#define WM_CELL_ALEXANDRIA_NE 20, 10
#define WM_CELL_ALEXANDRIA_SE 20, 11
#define WM_CELL_WATER_SHRINE 3, 9 /* place 43 "Water Shrine" (57147, -155460; WM_BLOCKFX_WATER_SHRINE); */
                                /* HOKORA_MIZU rebuilds its geometry, down below sea level (R43) */
#define WM_CELL_FIRE_SHRINE_E2 9, 1 /* no place; two east of WM_CELL_FIRE_SHRINE (Seatence Ice Field); in */
                                   /* HOKORA_MIZU only through texture area 11, nothing changes (R43) */
#define WM_CELL_FIRE_SHRINE 7, 1 /* place 48 "Fire Shrine" (129014, -29732), Mount Gulug (WM_BLOCKFX_GULUG_SUMMIT); */
                               /* HOKORA_HI cools its lava (texture area 4, polygons to WM_TOPO_MOUNTAIN; R43) */
#define WM_CELL_FIRE_SHRINE_E 8, 1 /* no place; east half of the volcano: HOKORA_HI cools its lava as in (7, 1) */
                                  /* (dark-crust texture, topography 0x3B -> WM_TOPO_MOUNTAIN; R43) */
#define WM_CELL_DRAGONS_GATE 14, 15 /* place 23 "Lindblum Dragon's Gate" (234496, -260825); in HOKORA_HI only */
                                  /* through texture area 4, its own textures are unchanged (R43) */
#define WM_CELL_MOGNET_CENTRAL 16, 1 /* place 28 "Mognet Central"; MOG_NET adds 40 triangles there (R43) */
#define WM_CELL_CONDE_PETIE_PATH 13, 4 /* place 35 "Conde Petie Mountain Path"; in MOG_NET only through texture */
                                     /* area 13, nothing changes (R43) */
#define WM_CELL_CONDE_PETIE_E 14, 5 /* no place; place 32 "Conde Petie" (229037, -90135) is 340 units west of it; */
                                  /* in MOG_NET only through texture area 13, nothing changes (R43) */
#define WM_CELL_CHOCOBOS_PARADISE 0, 0 /* no navi position (place 49 "Chocobo's Paradise" has none); the map corner. */
                                    /* Runtime (R43): area 63 Uaho Island with its cell trigger; CHOCOBO_HEAVEN */
                                    /* opens a cave mouth in its cliff (geometry and texture area 8) */
#define WM_CELL_CHOCOBOS_FOREST 16, 14 /* place 19 "Chocobo's Forest" (278526, -243814); with the lagoon, in */
#define WM_CELL_CHOCOBOS_LAGOON 9, 17 /* place 37 "Chocobo's Lagoon"; CHOCOBO_HEAVEN through texture area 8 */
extern u16 g_worldFormStoryThresholds[11]; /* story progress thresholds (wm_applyStoryForms) */
extern u8 g_worldFormStatusTable[][11];
extern u32 g_worldLandCellTypes[2]; /* cell type masks */
extern u32 g_worldWaterCellTypes[2];
u8 wm_testCellType(u32 *, u32);
extern s32 g_worldChunkRunGeneration;
s32 wm_chunkRequest(s16);
void wm_depthCueClut(u16 *, u16 *, s32, s16 *);
extern s32 g_worldWeatherAutoMode;
extern s32 g_worldWeatherSpeed;
extern s32 g_worldWeatherBlend;
extern s32 g_worldWeatherMode;
extern u8 g_worldWeatherAuto;
extern s8 g_worldMusicState;
extern s32 g_worldMusicSong;
extern s32 g_worldMusicStarted;
extern s32 g_worldMusicId;
extern s32 g_worldMusicRequest;
extern MenuMain s_menuMain;
extern u8 g_worldState;
extern char s_wmStrMemorymanDone[];
extern s16 g_worldTerrainHeightOffsets[][9];
extern WmMoveMode g_worldMoveModes[];
extern s16 g_worldClimbSmooth;
extern s16 g_worldSpeedSmooth;
extern s32 g_worldHeapStack[];
extern u8 g_worldCameraFourProbes;
extern s32 g_worldNaviMode; /* WM_NAVI_* */
/* g_worldNaviMode (WM_REQ_NAVI_MODE, default event var 0x64; wm_naviDraw) */
#define WM_NAVI_MINI 1 /* mini map */
#define WM_NAVI_FULL 2 /* full-screen map */
#define WM_NAVI_MAP_W 240 /* full map size in pixels (WmNaviPlace.mapX/mapY); the cursor wraps at it (wm_naviUpdate) */
#define WM_NAVI_MAP_H 200
extern u8 g_worldNaviVisible;
extern s32 g_worldTitleDelay;
extern POLY_G3 *g_worldNaviConePrim;
extern POLY_FT4 *g_worldTitlePrim;
extern s8 g_worldTitleId;
extern s32 g_worldTitleWide;
extern u8 g_worldTitleFade;
extern u8 g_worldNaviCursorDraw;
extern u8 g_worldNaviCursorOn;
extern u8 g_worldNaviPlaceCycled;
extern u8 g_worldNaviActive;
extern s32 g_worldNaviPrevMode;
extern s8 g_worldTitlePhase;
extern u8 g_worldNaviPlace;
extern s32 g_worldFrmbufReqCount;
extern WmFrmbufSlot g_worldFrmbufTail;
extern WmFrmbufSlot g_worldFrmbufHead;
extern s32 g_worldFrmbufDirty;
extern WmFrmbufSlot g_worldFrmbufSlots[4];

/* scrolling texture strips (wm_scrollTextures) */
typedef struct {
    u16 x; /* x */
    u16 y; /* y */
    u16 w; /* w */
    u16 h; /* h */
    u16 speed; /* speed (8.8) */
} WmTexScroll; /* 0xA */

/* random-encounter table entry (g_worldEncounterTable, docs/encounters.md) */
typedef struct {
    u16 battle[4];
    u8 topoPattern; /* topography << 2 | slot pattern */
    u8 zoneFog; /* zone << 1 | fog */
} WmEncounter; /* 0xA */

extern u16 g_worldTexScrollAcc[8];
extern WmTexScroll g_worldTexScrolls[8];
extern u16 g_worldTexAnim0Interval;
extern u16 g_worldTexAnim1Interval;
extern void *g_worldTexAnimPack;
extern s8 g_worldTexAnimStep[2];
extern s8 g_worldTexAnimSeq[];
extern WmEncounter *g_worldEncounterTable;
extern u16 (*g_worldEncounterEventRows)[12];
extern s32 g_worldAmbientSeDist[3];
extern u8 g_worldAmbientSeOn[3];
extern s32 g_worldAmbientSeX[3]; /* sound source x */
extern s32 g_worldAmbientSeZ[3]; /* sound source z */
extern u8 g_worldAmbientSe[3]; /* sound ids */
/* the three sources, named for the navi map place each sits at (g_worldNaviPlaces, world text bank 0). Distances are
 * in units of 256. Evidence: [data] the source positions, [code] wm_updateMap */
#define WM_AMBIENT_CLEYRA 0 /* (229279, -198701): place 18 "Cleyra"; WM_SE_WIND_LOOP, silent once WM_FORM_CLAY is set */
                            /* and on data set WM_DATASET_DISC4 */
#define WM_AMBIENT_WIND_SHRINE 1 /* (130781, -227819): place 38 "Wind Shrine" (131348, -231368), Gusty Valley; */
                                 /* WM_SE_WIND_LOOP, and the WM_TOPO_GUSTY_VALLEY push */
#define WM_AMBIENT_EARTH_SHRINE 2 /* (299201, -93859): place 26 "Earth Shrine" (299041, -94130); WM_SE_RUMBLE_LOOP and */
                                  /* the camera shake */
#define WM_AMBIENT_RADIUS 0x3F /* a source plays within this, louder the nearer */
#define WM_AMBIENT_SHAKE_RADIUS 0x28 /* WM_AMBIENT_EARTH_SHRINE shakes the camera target within this */
#define WM_AMBIENT_PUSH_RADIUS 0x22 /* WM_AMBIENT_WIND_SHRINE pushes a walker or chocobo toward +z within this */
#define WM_AMBIENT_BLOCK_RADIUS 0x16 /* ... and within this nothing can get off onto WM_TOPO_GUSTY_VALLEY (wm_findGetoffPos) */
extern u8 g_worldFormFlags[]; /* flags (wm_applyStoryForms), indexed by WM_FORM_* */
/* g_worldFormFlags indices: the map forms wm_applyStoryForms switches to. Names from its "BGI Form : %s" debug
 * prints, whose cases are these indices; the status table holds index + 1 (0 = no change), see WM_FORM_STATUS. */
#define WM_FORM_SGATE 0 /* "SGATE" */
#define WM_FORM_ALEX 2 /* "ALEX" */
#define WM_FORM_HOKORA_HI 3 /* "HOKORA(HI)": also picks other CLUT animation rows (w_memoryman) */
#define WM_FORM_LIND 4 /* "LIND" */
#define WM_FORM_CLAY 5 /* "CLAY": block effect 4 is no longer drawn, ambient sound 0 goes quiet */
#define WM_FORM_CHOCOBO_HEAVEN 7 /* "- Replace Chocobo Heaven -" (EVT_GB_GENERAL_CHOCOBO_FORM) */
#define WM_FORM_KUROMA 8 /* "KUROMA" */
#define WM_FORM_HOKORA_MIZU 10 /* "HOKORA(MIZU)": turns block effect 10 on */
#define WM_FORM_MOG_NET 12 /* "- Replace Mog Net -" (EVT_GB_GENERAL_MOGNET_FORM) */
#define WM_FORM_STATUS(f) ((f) + 1) /* g_worldFormStatusTable value that applies form f */
extern u8 g_worldBlockFx7Spin;
extern u8 g_worldClutAnimFx2;
extern u8 g_worldCdUser;
void wm_stepClutAnims(s32, s32, s32);
void wm_initTextureLists(void);
void wm_blockListInit(void);
void menuMain_reorder(s32, s32);
void wm_musicPlay(u16);
void wm_musicStop(u16);

/* image pack loaded by wm_loadImagePack; offsets are from the header start */
typedef struct {
    s32 clutOfs; /* offset: WmImagePackClut */
    s32 imageOfs; /* offset: images (RECT + pixels) */
    s32 imageCount; /* image count */
} WmImagePack;

typedef struct {
    s32 dataOfs; /* offset: pixel data */
    s32 count; /* count */
    RECT rects[1];
} WmImagePackClut;

extern RECT s_menuMainPanelRects[6];
extern TweenDef s_menuMainSlideIn[4];
extern u8 s_menuMainExpText[16];
extern u8 s_menuMainNextText[16];
extern u8 s_menuMainExpLabel[];
extern u8 s_menuMainNextLabel[];
extern u8 s_menuMainUntilLabel[];
extern u8 s_menuMainStatusLabel[];
extern u8 s_menuMainStatusNormal[];
extern u8 s_menuMainStatusAbnormal[];

/* wm_initMovement init state */
typedef struct {
    u8 lastSlot; /* last written slot */
    u8 pad1[3];
    u16 polys[6]; /* polygon */
    u16 cells[6]; /* cell */
} WmPolyCache; /* 0x1C: recently hit walkmesh polygons */
s32 wm_getGroundHeight(VECTOR *, u32 *, s32 *, WmPolyCache *, u16 *);
u8 wm_tryStep(VECTOR *, s32, s32, s32, u32 *, s32 *);
u8 wm_findGetoffPos(ActorPos *);
extern WmPolyCache g_worldPolyCache[11];
extern EvtObject g_worldDummyActor;
extern u8 g_worldFlyTurning;
extern u8 g_worldCameraTurning;
extern u8 g_worldPlayerPosChanged;
extern s32 g_worldCeilingGap;
extern EvtObject *g_worldMountActor;
extern u8 g_worldCameraFastRise;
extern EvtObject *g_worldMoogleActor;
extern u8 g_worldFastFly;
extern s32 g_worldPlayerClimbSpeed;
extern s32 g_worldMountPaletteOverride;
extern s32 g_worldMoveInitWord;
extern u8 g_worldCameraTurnedByHand;
extern s32 g_worldPlayerSpeed;
extern u16 g_worldPlayerHeading;
extern EvtObject *g_worldVehicleActor;
extern s32 g_worldCameraTurnGain;
extern EvtObject *g_worldWalkerActor;
extern s32 g_worldPlayerTurn;

extern u8 g_worldActorHideLevel;
extern s32 g_worldDebugStep; /* debug: last update step */
extern s32 g_worldDrawActors;
extern u8 g_worldPointFxOn;
extern u8 g_worldStartup;
extern TILE g_worldPauseTile;
extern DR_TPAGE g_worldPauseTpage;
extern u32 g_worldPauseOT;
extern u8 g_worldPauseSprite[];
void game_resumeClock(s32);
void akao_pause(s32);
void akao_resume(s32);
extern s32 g_worldFrameCount;
extern u8 g_worldMogChoice; /* a world-script choice window is open (WM_REQ_MOG_CHOICE): moogle, chocobo menu, airship Go to, navigation method */
extern s16 g_worldOtBias;
extern u8 g_worldSysInitFlag;
extern u8 g_worldSuspended;
extern u8 g_worldMapJump;
extern u8 g_worldVehiclesFound;
u8 *wrain_init(u8 *);
void wrain_set(s32, u32);
extern s32 g_worldTimeActorDraw;
extern s32 g_worldTimeMapDraw;
void wm_drawActors(void);
void wm_drawMap(void);
void wm_naviDraw(void);
void wm_drawEffects(void);
s32 wm_getCellBlock(s16, u8 *);
s32 wm_getBlockHeight(s32, WmBlock *, VECTOR *, u32 *, s32 *, WmPolyCache *);
void wm_wrapPos(s32 *, s32 *);
void wm_posToCell(s32, s32, s32 *);
void wm_cellToPos(s32, s32 *, s32 *);
void wm_readtxReset(void);
void wm_readtxDone(s32 *);
void wm_relocatePack(WmPack *);
extern u8 g_worldCameraHold;
extern u8 g_worldCameraLookupFix;
extern s32 g_worldChunkRunSlot;
extern s32 g_worldChunkRunCount[];
extern WmChunkSlot *g_worldChunkRun[];
extern WmPack *g_worldChunkRunPacks[];
extern s32 *g_worldChunkRunTex[];
extern s32 g_worldImageNames[5];
extern s32 g_worldImageSectors[5]; /* CD sectors */
extern s32 g_worldImageNamesDisc1[5];
extern s32 g_worldImageNamesDisc4[5];
extern char s_wmStrImageTopSector[];
/* sprite table (wm_naviDrawFrame) */
typedef struct {
    s16 x; /* x */
    s16 y; /* y */
    u8 u; /* u */
    u8 pad5;
    u8 v; /* v */
    u8 pad7;
    s16 w; /* w */
    s16 h; /* h */
} WmNaviFramePart; /* 0xC */
extern WmNaviFramePart g_worldNaviFrameParts[4];
void wm_naviDrawFrame();
extern s32 g_worldWeatherPointDist;
void wm_frmbufReset(void);

extern WmCameraParams g_worldCameraPresets[];
extern u8 g_worldCameraPresetIndex[][3][3];
extern VECTOR g_worldCameraUp;
extern SVECTOR g_worldCameraLookAt; /* look-at point passed to wm_lookAt, never written (zero: the scene is drawn relative to g_worldCameraTarget) */
extern SVECTOR g_worldCameraEyeRel;
extern SVECTOR g_worldCameraEyeOffset;
extern s32 g_worldCameraTargetHeight;
extern s32 g_worldCameraDistScale;
extern s16 g_worldCameraRoll;
extern u8 g_worldCameraInitFlag;
extern u8 g_worldCameraStartPhase;

/* main exe: candidates for ff9.h */
void math_rotMatrixZXY(SVECTOR *, MATRIX *);
void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
void pen_locate(s32, s32);
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */
void math_rotMatrixYXZ(SVECTOR *, MATRIX *); /* RotMatrix */
void math_rotMatrixXYZ(SVECTOR *, MATRIX *);
s32 math_cos(s32); /* cos */
s32 math_sin(s32); /* sin */
void SetGeomScreen(u16);
s32 wrain_draw(s32);
extern u32 g_portraitSavedVram[]; /* VRAM save buffer */
extern u8 *g_menuHelpText;
s32 text_measureStyle(u8 *, s32);
extern s32 g_menuScreenFrame; /* overlaps MenuScreen.unk4 in ff9.h */
EvtObject *evt_getPlayer(void);
void evt_resetIdleTimer(s32);

/* run on the scratchpad stack (top of the 1 KB scratchpad) */
#define SetSpadStack(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -4\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack() __asm__ volatile("addiu $29, $29, 4\n\tlw $29, 0($29)" : : : "memory")

/* world effect emitter table entry (wm_updateBlockFx) */
typedef struct {
    s32 x; /* position */
    s32 y;
    s32 z;
    s32 rangeX; /* random range */
    s32 rangeY;
    s32 rangeZ;
    u8 pad18[0x24 - 0x18];
    s16 effect; /* effect, -1 = end */
    s16 scale;
    s16 chance; /* chance (of 4096) */
    u8 pad2A[0x40 - 0x2A];
} WmEmitter; /* 0x40 */

/* map-marker colour (wm_naviDrawPlaces) */
extern s32 g_worldNaviCursorX; /* navi cursor */
extern s32 g_worldNaviSnapDist; /* marker snap distance */
extern s32 g_worldNaviCursorY;
typedef struct { SVECTOR v[4]; } WmQuad; /* shadow quad template */
extern WmQuad g_worldBlockBackQuad;
extern WmQuad g_worldNaviMiniMapArrow; /* radar arrow */
extern WmQuad g_worldNaviFullMapArrow; /* big-map arrow */
extern WmScrRect g_worldBlockCullRects[]; /* on-screen test rects */
extern char s_wmStrIllegalWeatherMode[]; /* "Illegal Weather Mode" */
extern VECTOR g_worldBlockFx7Pos;
extern VECTOR g_worldBlockFx6Pos;
extern SVECTOR g_worldBlockFx6CoreRot;
extern s32 g_worldBlockFx6CoreOtOfs;
extern s32 g_worldBlockFx6SpriteSize;
extern MATRIX g_worldIdentityMatrix;
extern VECTOR g_worldBlockFx4Scale;
extern VECTOR g_worldBlockFx6RingScale;

/* first entry of data pack `id` */
#define WM_PACK0(id) ((s32)((WmPack *)w_framePackGetPtr(g_worldDataPack, id))->entries[0])

/* inline CompMatrix: c = a * b, c->t = a * b->t + a->t */
#define gte_CompMatrix(a, b, c)    \
    gte_MulMatrix0(a, b, c);    \
    gte_SetTransMatrix(a);      \
    gte_ldlv0((b)->t);          \
    gte_rtv0tr();               \
    gte_stir((c)->t);

extern WmNearCell g_worldNearCells[];
s32 wm_getCameraViewState();
void wm_frmbufCompact(void);
void wm_frmbufTransfer(void);
void wm_blockListCompact(void);
void wm_blockListResolve(void);
s32 wm_blockListRequest(s16);
s32 w_frmbufRequest(s16);
void wm_updateBlockFx(s32);
extern u8 g_worldNaviConeR; /* big-map cone colour */
extern u8 g_worldNaviConeG;
extern u8 g_worldNaviConeB;
extern u8 g_worldNaviTileR; /* big-map tile colour */
extern u8 g_worldNaviTileG;
extern u8 g_worldNaviTileB;
extern RECT g_worldVramSaveRect;
extern char s_wmStrEncounterNoScene[]; /* encounter fallback message */ /* palette area (wm_systemInit) */
extern CVECTOR g_worldNaviVehicleColorA; /* marker colours */
extern CVECTOR g_worldNaviVehicleColorB;
extern CVECTOR g_worldNaviMountColorA;
extern CVECTOR g_worldNaviMountColorB;
extern u8 g_worldNaviPlaceR;
extern u8 g_worldNaviPlaceG;
extern u8 g_worldNaviPlaceB;
void wm_naviDrawMarker(s32, s32, s32, s32, CVECTOR, CVECTOR, s32, s32, s32, s32);
void wm_naviWorldToMap(s32, s32, s32 *, s32 *, u8);
void wm_naviUpdate(void);
s32 wm_naviIsPlaceKnown(u32);

static inline CVECTOR wm_naviDrawPlaces_color(void) {
    CVECTOR c;

    memset(&c, 0, 4);
    c.r = g_worldNaviPlaceR;
    c.g = g_worldNaviPlaceG;
    c.b = g_worldNaviPlaceB;
    return c;
}

/* Inlined copy of wm_wrapPos (wrap map coordinates). */
static inline void wm_wrapPos_inline(s32 *x, s32 *z) {
    if (*x < 0) {
        *x += g_worldWidth;
    }
    if (*x >= g_worldWidth) {
        *x -= g_worldWidth;
    }
    if (*z > 0) {
        *z -= g_worldDepth;
    }
    if (*z <= -g_worldDepth) {
        *z += g_worldDepth;
    }
}

/* pad state, only read in mode 2 (wm_debugAdjustScreen) */
static inline u32 wm_debugAdjustScreen_pad(void) {
    return g_worldState == WM_STATE_RUNNING ? g_padPortHeld[0] : 0;
}

static inline u32 wm_debugAdjustScreen_trg(void) {
    if (g_worldState != WM_STATE_RUNNING) {
        return 0;
    }
    return g_padPortPressed;
}

void wm_freeEffect(WmEffect *);
s32 wm_getSpriteFrameCount(u16 *);

/* Inlined copy of wm_testCellType, taking the topography index already extracted (WM_ATTR_TOPOGRAPHY). */
static inline u8 wm_testCellType_inline(u32 *a, u32 c) {
    if (c >= 32) {
        if ((a[0] >> (c - 32)) & 1) {
            return 1;
        }
    } else if ((a[1] >> c) & 1) {
        return 1;
    }
    return 0;
}

typedef struct {
    u16 frameCount; /* frame count (WMSPR_COUNT_MASK) */
    u16 tpage;
    u16 clut;
    u8 height;
    u8 width;
    u16 frameOffsets[1]; /* per-frame data offsets, then a counted u16 table */
} WmSprHdr; /* sprite animation header (wm_drawSprite3D) */
#define WMSPR_COUNT_MASK 0x7FFF /* WmSprHdr.frameCount: the count; bit 15 is never tested. Evidence: [code] */

typedef struct {
    u16 *uvs;
    u16 *colors;
    s32 halfW;
    s32 halfH;
    s32 tpage;
    s32 clut;
    s32 fade;
    s32 quadCount;
    s32 primCode;
    s32 sizeShift;
    s32 otBias;
} WmSprCtx; /* draw context passed to wm_emitSprite3D */

s32 wm_emitSprite3D(u8 *, WmSprCtx *, s32, s32);
s32 wm_drawSprite3D(s32, WmSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    s32 size; /* size */
    void *dest; /* destination */
} WmLoadReq; /* CD read request (wm_readChunks, cd_readChunks) */

extern WmLoadReq g_worldChunkReadReqs[];
extern WmLoadReq g_worldChunkReadList[];
extern s32 g_worldChunkRunDir[];
extern s32 g_worldChunkReadCount;
extern char s_wmStrBlockNoForm[];
extern SVECTOR g_worldEffectFlatRot; /* rotation used by wm_drawEffects */
s32 wm_readtxFind(s32, s32);
u8 wm_findChunkRun(WmChunkSlot **, s32 *, s32 *);
void wm_getChunkEntry(s32, s32, WmChunkEntry *);
void wm_getChunkEntryT(s32, s32, WmChunkEntry *);
void wm_chunksLoaded(void);

typedef struct {
    u8 b[0x1E0];
} WmCellMap; /* g_worldChunkFlags as one block */
typedef struct {
    WmTexScroll e[30];
} WmTexScrollTbl; /* g_worldTexScrolls and the entries after it */
extern u8 g_worldZoneEncounterCount[];
extern s32 g_worldCellsH;
void wm_applyStoryForms(void);
void wm_chunkListInit(void);
void wm_initBlockMatrix(void);
void wm_loadImagePack(WmImagePack *);

#define gte_stir(r0) __asm__ volatile( \
    "swc2\t$9, 0(%0);" \
    "swc2\t$10, 4(%0);" \
    "swc2\t$11, 8(%0)" \
    : : "r"(r0) : "memory")

void wm_calcShadowMatrix(WmObjEnt *, WmObjSlot *, MATRIX *, s32, s32);

/* wm_drawShadow: project one corner of the 3x3 shadow grid */
#define SHADOW_PT(x, z) \
    corner.vx = (x); \
    corner.vy = 0; \
    corner.vz = (z); \
    gte_ldv0(&corner); \
    gte_rtps()

/* wm_drawShadow: link shadow quad q at its deepest corner */
#define SHADOW_ADD(k, q) \
    k >>= 3; \
    if (-g_worldOtBias < k && k < 0x1000 - g_worldOtBias) { \
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k], shadow->prims[g_gameState->gameData->drawBuf][q]); \
    }

/* main exe: candidates for ff9.h */
u8 cd_readChunks(s32, void *, s32, s32);
s32 evt_getWorldCamEyeObj(void);
s32 evt_getWorldCamTargetObj(void);
void fade_inAdditiveDefault(void);
void OuterProduct0(VECTOR *v0, VECTOR *v1, VECTOR *v2);

/* wm_getBlockHeight (walkmesh height lookup) */
extern s16 g_worldHitHeights[];
extern u16 g_worldHitPolys[];
extern char s_wmStrMemorymanError[];

/* wm_movePlayer (walkmesh movement) */
void wm_controlPlayer(void);
extern u8 g_worldPlayerControlOff;
extern u8 g_worldStepFlagOn;
extern u8 g_worldFastWalk;
extern u8 g_worldPlayerFloorPoly;

/* wm_cameraFollow (camera follow) */
extern u8 g_worldCameraSmoothing;
extern WmPolyCache g_worldCameraPolyCache[4];
extern s32 g_worldCameraEaseDiv;
extern s32 g_worldCameraEaseShift;
extern s32 g_worldCameraEaseDivMin;
extern s32 g_worldCameraEaseDivMax;
extern s32 g_worldCameraEaseScale;
extern s32 g_worldCameraEaseBoost;

/* wm_soundCommand (sound/music request handler) */
extern s32 g_worldMusicReqId; /* requested music id */
extern u8 g_worldMusicReqIdKind;  /* byte 2 of g_worldMusicReqId: id kind */
extern s8 g_worldMusicReqKind;  /* request kind, -1 = none */
extern s32 g_worldMusicSector; /* CD sector of the music file */
s32 wm_isMusicFadingOut(void);
s32 wm_findSoundFile(); /* old-style: one caller passes a third argument */
s32 wm_depGetLink(s32 fileId);
s32 wm_unpackMusic(s32 db);
void wm_streamSoundChunk(WmStreamReq *req);
s32 wm_getModelCacheBottom(void);

/* resources wm_load* takes from the registered world db. Evidence: [code] what each loader does with it */
#define WM_SOUND_BANK 0x139 /* DB_SOUND_BANK id uploaded to the SPU (wm_loadSoundBanks) */
#define WM_SOUND_BANK_PARKED 0x17B /* DB_SOUND_BANK id parked in VRAM beside the draw buffer (wm_uploadParkedSound) */
/* wm_loadResidentMusic copies DB_MUSIC | MUSIC_CHOCOBO to GameData.musicSeq2: the resident song is the chocobo's */

/* main exe: candidates for ff9.h (sound) */
s32 akao_playMusic(void *);
void akao_stopMusic(s32);
void akao_saveMusic(void);
s32 akao_playSe(s32, s32, s32, s32);
void akao_setCondition(s32);
void akao_setAllSeVolume(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_setSeVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_sendCmdAA(s32);
void akao_sendCmdAB(s32, s32);
void akao_sendCmdA2(s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_sendCmdAC(s32);
void akao_sendCmdAD(s32, s32);
void akao_sendCmdA4(s32, s32, s32);
void akao_sendCmdA5(s32, s32, s32, s32);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);
s32 akao_getSpuTransferState(void);

s32 wm_initShadowPrims(s32, s32); /* returns shadow_initSlots's result (the next free address) */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void evt_createModels(void); /* main */
s32 evt_isControlEnabled(void); /* main */
u8 *evt_loadScripts(u8 *p); /* main */
s32 evt_countNpcActors(s32 unused); /* main */
void evt_callWorldCellTrigger(s32 a, s32 b, s32 c); /* main */
void wm_mountPalettePre(void);
s32 wm_isDarkWeather(void);
void wm_naviDrawChocograph(void);
void game_updateWindows(void); /* main */
void menu_update(void); /* main */
void msg_setOtz(s32 otz); /* main */
s32 swirl_update(s16 startX, s16 startY); /* main */
void *w_framePackGetPtr(void **pack, s32 idx);
void w_movementChangeSP1(void);
void w_movementChangeSP2(void);
s32 w_readtxRequest(); /* (s16); unprototyped: callers pass the argument unconverted */
void wm_autoPilotOn(void);
void wm_buildPackets(ModelObj *mdl, WmTexInfo *texInfo, s16 (*uvOffsets)[2]);
void wm_calcCameraPreset(s32 altitude, s32 row, s32 col, s32 viewBlend, WmCameraParams *out);
void wm_callChocobo(void);
s32 wm_cameraAltitudeBlend(s32 altitude);
void wm_cameraAutoTurnVehicle(void);
void wm_cameraAutoTurnWalk(void);
void wm_cameraSmooth(void);
void wm_chunkListCompact(void);
s32 wm_drawActorModel(EvtObject *act, SVECTOR *pos, s32 unused);
void wm_drawActorShadow(EvtObject *act, SVECTOR *pos, s32 depth);
void wm_drawBlock(u8 *block, s32 parity, s32 far);
void wm_drawBlockFx(u8 fxId);
void wm_drawBlocks(void);
void wm_drawModel(s32 model, s32 otz, u32 mode);
void wm_drawTitle(void);
void wm_fetchParkedSound(void);
s32 wm_findAreaData(); /* (WorldMapObj *, u16); unprototyped: callers pass the argument unconverted */
void wm_findBattleMusic(void);
s32 wm_findDefaultAnim(s32 modelId);
s32 wm_findMapScript(); /* (WorldMapObj *, u16); unprototyped: callers pass the argument unconverted */
void wm_finishLoad(void);
s32 wm_getAreaZone(s32 area);
s32 wm_getChunkBuffer(void);
s32 wm_getWorldDataFileId(void);
void wm_heapInit(void);
void wm_initCamera(void);
void wm_initDiscImage(void);
void wm_initEffects(void);
void wm_initLighting(void);
void wm_initMapState(void);
void wm_initModel(ModelObj *mdl, u8 *work);
void wm_initMovement(void);
s32 wm_isMapSongPlaying(void);
s32 wm_lerp(s32 from, s32 to, s32 t);
s32 wm_lerpShift(s32 from, s16 to, s32 t, s32 headroom);
s32 wm_loadFile(WorldMapObj *obj, s32 unused);
void wm_loadTimPack(s32 *pack, s32 imgDx, s32 imgDy, s32 clutDx, s32 clutDy);
void wm_lookAt(MATRIX *out, SVECTOR *eye, SVECTOR *target, VECTOR *up);
void wm_movePlayer(WmActorType *cc);
void wm_musicFade(); /* (void); unprototyped: w_frame.c passes an argument it ignores (splat writes the empty body itself) */
void wm_musicInit(void);
void wm_naviDrawFullMap(void);
void wm_naviDrawMiniMap(void);
void wm_naviDrawPlaces(void);
void wm_naviInit(void);
void wm_postDraw(void);
void wm_readChunks(void);
void wm_readtxCompact(void);
void wm_requestChunks(s32 chunk);
void wm_resetMapObj(void);
void wm_runEvents(void);
void wm_saveDepTable(void);
void wm_setActorFog(EvtObject *act, s32 depth);
void wm_setMusicVolumeFade(s32 volume, s32 frames);
void wm_setupMapObj(void);
void wm_setupWorldData(void);
void wm_updateCamera(void);
void wm_updateCameraNoView(void);
void wm_updateChunks(void);
void wm_updateDraw(void);
void wm_updateEffects(void);
void wm_updateLogic(void);
void wm_updateMap(void);
void wm_updateTextureLists(void);
void wm_updateVisibleBlocks(void);
void wm_updateVramAnims(void);
void wm_uploadParkedSound(void);
void wm_weatherInit(void);
void wm_weatherUpdate(void);
s32 wm_worldToView(VECTOR *pos, VECTOR *outRel, VECTOR *outView, s32 *outScale);

/* Packs (w_frameExtruct, w_framePackGetPtr, wm_loadTimPack): word 0 = entry count | WM_PACK_EXTRACTED, then one
 * offset per entry, relative to the pack until w_frameExtruct relocates them. Evidence: [code] */
#define WM_PACK_COUNT_MASK 0x7FFFFFFF
#define WM_PACK_EXTRACTED 0x80000000 /* the offsets are pointers now; w_frameExtruct prints "already" and skips it */
/* items of the registered world data file (FILE_WM_DATA / FILE_WM_DATA_LATE), db_lookupRegistered(DB_GET_ADDR, id, 0)
 * in wm_setupWorldData. Six come in per-data-set versions, id = base + g_worldDataSet (WM_DATASET_DISC1 / _DISC4).
 * Evidence: [code] what wm_setupWorldData does with each */
#define WM_ITEM_TEX_ANIM(set) (0x410 + (set)) /* g_worldTexAnimPack: TIMs for the texture animations (wm_animTextures) */
#define WM_ITEM_DATA_PACK(set) (0x41A + (set)) /* g_worldDataPack, entries WM_PACK_* */
#define WM_ITEM_CHUNKS(set) (0x424 + (set)) /* g_worldChunkTable */
#define WM_ITEM_CHUNKS_T(set) (0x42E + (set)) /* g_worldChunkTableT */
#define WM_ITEM_TIMS(set) (0x438 + (set)) /* a TIM pack loaded whole (wm_loadTimPack) */
#define WM_ITEM_FORM_TIMS(set) (0x442 + (set)) /* story form textures: entry form + 16 * g_worldFormFlags[form] */
/* DIR_WORLD_IMAGES file ids (wm_initDiscImage), id = base + g_worldDataSet; the g_worldImageNames slot names them
 * ("\\DISCn.IMG", "...S", "...T", "...H", "...V"). Evidence: [string] g_worldImageNamesDisc1/Disc4, [code] */
#define WM_IMAGE_FILE(set) (0x3F2 + (set)) /* slot 0 "DISCn.IMG", "Image Top Sector" */
#define WM_IMAGE_FILE_H(set) (0x3FC + (set)) /* slot 3 "DISCnH.IMG": chunk runs of direction 1 (wm_getChunkEntry) */
#define WM_IMAGE_FILE_V(set) (0x406 + (set)) /* slot 4 "DISCnV.IMG": the other runs (wm_getChunkEntryT) */
#define WM_ITEM_CELL_MAP 0x5DC /* the WmCellMap copied to g_worldChunkFlags */
#define WM_ITEM_COMMON_TIMS 0x5DD /* a TIM pack loaded whole (wm_loadTimPack) */
#define WM_ITEM_NAVI_TIMS 0x5DE /* navi map halves (page * 2, | 1), 4, then the area titles (title * 2 + 5, + 6) */
#define WM_ITEM_CHOCOGRAPH_TIMS 0x5DF /* one picture per chocograph (GameData.chocograph - 1) */
#define WM_ITEM_IMAGES 0x5E0 /* wm_loadImagePack */
/* g_worldDataPack entries (w_framePackGetPtr / WM_PACK0). Evidence: [code] the user of each */
#define WM_PACK_AREA_NAMES 0 /* g_worldAreaNames */
#define WM_PACK_MOUNT_PALETTES 1 /* wm_setMountPalette */
#define WM_PACK_CLUT_ANIM 2 /* colours copied into g_worldClutBuf by the CLUT animations 15-17 (wm_initClutAnims) */
#define WM_PACK_ENCOUNTERS 3 /* g_worldEncounterTable (docs/encounters.md) */
#define WM_PACK_ENCOUNTER_EVENTS 4 /* g_worldEncounterEventRows */
#define WM_PACK_WEATHER 5 /* g_worldWeatherData */
#define WM_PACK_TEX_SCROLLS 6 /* g_worldTexScrolls */
#define WM_PACK_FX_SPRITE(type) ((type) + 7) /* WmEffect sprite of effect type (wm_spawnEffect) */
#define WM_PACK_BLOCKFX_EMITTERS(id) ((id) + 0x28) /* WmEmitter list of block effect id (wm_updateBlockFx) */
#define WM_PACK_MODELS_FIRST 0x35 /* 0x35-0x42 are relocated by wm_relocatePack (wm_setupWorldData) */
#define WM_PACK_MODELS_END 0x43
#define WM_PACK_CLAY_MODEL 0x35 /* WM_BLOCKFX_CLAY: 0x35, then the three spinning parts 0x36-0x38 and 0x3A */
#define WM_PACK_CLAY_PARTS 0x36
#define WM_PACK_CLAY_OUTER 0x3A
#define WM_PACK_SPINNER_MODEL 0x39 /* WM_BLOCKFX_WINDMILL */
#define WM_PACK_CORE_SPRITE 0x25 /* WM_BLOCKFX_MEMORIA_ORB: its sprite (= WM_PACK_FX_SPRITE(30)) and models */
#define WM_PACK_CORE_SHELL_A 0x3C
#define WM_PACK_CORE_SHELL_B 0x3D
#define WM_PACK_CORE_RING 0x3E
#define WM_PACK_CORE_MODEL 0x3F
#define WM_PACK_CORE_FLASH_A 0x40
#define WM_PACK_CORE_FLASH_B 0x41
#define WM_PACK_BACKDROP 0x3B /* model drawn behind the map while g_worldBackdropOn (wm_drawMap) */
#define WM_PACK_DEFAULT_BLOCKS 0x42 /* g_worldDefaultBlocks */
/* frame-buffer texture slots (WmFrmbufSlot.texId, w_frmbufRequest): the low 6 bits are the texture id, 0 = none */
#define WM_TEXID_MASK 0x3F
/* navigation map: which places are known, one bit per place (wm_naviIsPlaceKnown), places 0-15 in event global
 * u16 var WM_NAVI_KNOWN_VAR, 16-31 in the next, up to 63. Evidence: [code] */
#define WM_NAVI_KNOWN_VAR 0x2E /* g_evtGlobalVars.h index */
#define WM_NAVI_KNOWN_LATE 0x7C0 /* places 6-10: wm_naviInit marks them known once the story counter is >= WM_STORY_MIST_GONE */
#define WM_NAVI_KNOWN_PAIR 0xC000 /* places 14 and 15: then knowing either marks both */

#endif

#ifndef OVL_14_H
#define OVL_14_H

/* Declarations local to overlay 0x14 (loaded at 0x800A7000). Addresses in the
 * overlay range mean different things in different overlays, so they live here,
 * not in ff9.h. */
#include "ff9.h"

/* A title screen image: an 8-byte item drawn by func_80027710 (opening_titleLoadTims fills them). */
typedef struct {
    union {
        u32 w; /* uv + clut, copied as one word */
        struct {
            u8 u;
            u8 v;
            u16 clut;
        } s;
    } uvClut;
    u8 w;
    u8 h;
    u16 tpage;
} TitleImage;

/* 8-byte entry of the directory lists at D_80067944 + 0x14C700 (opening_findSceneSectors) */
typedef struct {
    u16 id;     /* field / world id, or the id the first lookup gives for the second list */
    u16 link;   /* id to look up in the second list */
    s32 sector; /* start; the next entry's start ends it */
} SceneDirEntry;

/* main exe: candidates for ff9.h */
/* Menu sounds for snd_playSe: main's menuList_up/down play SE_CURSOR on a move, menus play it on a pick too,
 * SE_CANCEL on Circle backing out, SE_BUZZER when an action is refused. */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67
#define SE_BUZZER 0x66
#define SE_CANCEL 0x65
#endif
#define SE_GAME_START 0xC18 /* New Game on the title, and the ending's secret code */
/* g_gameState->unkB: one bit per disc of the game found in the drive (bit 0 = disc 1), 0xFF = no disc read.
 * The opening plays its movie only with disc 1 in (bit 0), the ending only with disc 4 (bit 3), and New Game
 * sets it to disc 1 before scene_checkDisc asks for the disc that was in before. */
#define DISC_NONE 0xFF
#define DISC_1    1
#define DISC_4    8
#define MENU_SAVE 4 /* menu_open id of the save / load menu (ovl_39); argument 1 = load */
#define MENU_DISC 8 /* menu_open id given scene_checkDisc's disc, (disc << 14) | 15000 */
void func_8001D910(void *, RECT **, RECT **); /* TIM: clut rect, image rect */
typedef struct {
    u32 unk0;
    u32 unk4; /* flags: bpp in low bits */
} TmTimLike; /* TIM header */
s32 cd_findFile(char *);
extern s32 g_mbgFrame;
u8 func_80022ED4(s32);
void func_80032100(void);
void func_80023018(void);
s32 func_80055290(void *);
void func_800552B8(s32);
void func_800553EC(s32, s32, s32, s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800558DC(s32);
void func_80055908(s32, s32);
void func_8005593C(s32, s32, s32);
void func_80055980(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);

/* ovl_64 (FMV player) */
void mbg_streamSetBaseSector(s32);
void MBG_init(s32, u8 **);
void mbg_play(s32, s32);
s32 mbg_update(s32);
void mbg_setRgb24(s32);
void mbg_setEndCallback(void (*)(void));
s32 mbg_getLastFrame(void);
/* movie table (ovl_64's g_mbgMovieLists): list 1 is the disc 1 movies, its entry 0 the opening movie */
typedef struct {
    char *path; /* movie file name */
    s32 pad4[4];
} MbgMovie; /* 0x14 bytes */
typedef struct {
    s16 count;
    s16 pad2;
    MbgMovie *movies;
} MbgMovieList;
extern MbgMovieList g_mbgMovieLists[];

void mbg_shutdown(void);
void mbg_stop(void);

/* ovl_39 (save / load menu, 0x801EDF00): nonzero when the load menu loaded a game */
u8 func_801F5330(void);

/* ---- this overlay ---- */

/* g_openingState: the states of opening_main */
enum {
    OPENING_MOVIE,    /* opening movie (disc 1 only), then the title */
    OPENING_ATTRACT1, /* attract image set 0, then the title */
    OPENING_PAUSE1,   /* OPENING_PAUSE_FRAMES before the movie comes back (disc 1) */
    OPENING_MOVIE2,
    OPENING_ATTRACT2, /* attract image set 1, then the title */
    OPENING_PAUSE2,
    OPENING_BOOT,     /* boot screen, first boot only (D_80067950 == 0) */
    OPENING_DONE      /* the title started a game */
};
#define OPENING_PAUSE_FRAMES 180

/* db ids (db_lookupRegistered) of the music the title plays through opening_soundCmd, and of its sound bank
 * (opening_loadMusic); opening_soundCmd records the bank's low half (0x122) with the music */
#define TITLE_MUSIC      0x7003A
#define TITLE_MUSIC_BANK 0x90122

/* Module W ("scene" table, the same source as ovl_15's): one SceneMode per scene id. */
typedef struct {
    void (*init)(void);   /* scene_init calls all of them */
    void (*enter)(void);
    void (*update)(void); /* scene_update, every frame */
    void (*leave)(void);
} SceneMode;

/* scene ids of the opening, index into g_sceneModes */
enum {
    SCENE_NONE,
    SCENE_TITLE, /* New Game / Continue */
    SCENE_LOAD,  /* the load menu is open */
    SCENE_DISC,  /* the disc prompt (MENU_DISC) is open */
    SCENE_START, /* the game is about to start */
    SCENE_END,   /* the loops of opening_movieThenTitle / opening_attract return 1 */
    SCENE_COUNT
};
extern SceneMode g_sceneModes[SCENE_COUNT];

typedef struct {
    u8 cur; /* scene id */
    u8 unk1;
    u8 unk2;
    u8 fadeScreen; /* opening_titleStartFadeIn: 0 fades the choices in, 1 shows them and fades the screen in */
} SceneState;
extern SceneState g_scene;

extern s32 g_titleCardEvents[4];
extern u32 g_startFrames;
extern u8 g_loadMenuClosed;

typedef struct {
    s16 x;
    s16 y;
} Ovl14Pos;
extern Ovl14Pos g_titleImage10Pos;
extern Ovl14Pos g_titleImage13Pos;
extern Ovl14Pos g_titleNewGamePos;
extern Ovl14Pos g_titleContinuePos;
extern Ovl14Pos g_titleCursorPos[3];

/* g_titleMenu.cursor */
enum { TITLE_NEW_GAME, TITLE_CONTINUE };

/* g_titleMenu.images */
#define TITLE_BG_STRIPS    10 /* images 0-9: 32-pixel strips drawn across the top */
#define TITLE_IMG_10       10 /* drawn semi-transparent (opening_titleLoadTims sets the tpage's blend bit) */
#define TITLE_IMG_NEW_GAME 11
#define TITLE_IMG_CONTINUE 12
#define TITLE_IMG_13       13
#define TITLE_IMG_COUNT    14
#define TITLE_FADE_FRAMES  0x30 /* fade-in of the choices */

/* Title menu state (g_titleMenu). */
typedef struct {
    u8 unk0;
    u8 shown;    /* choices and cursor drawn */
    u8 fadingIn; /* choices fading in (opening_titleDrawFading), no input */
    u8 cursor;   /* TITLE_NEW_GAME / TITLE_CONTINUE */
    u8 cardIn[2]; /* per port: a memory card answered */
    u8 cardPort;  /* port being polled */
    u8 cardPolling;
    u8 decided;   /* set when the fade-out after a choice ends */
    u8 pad9[3];
    s32 fadeFrames;
    s32 unk10;
    s32 cardEvents[4]; /* OpenEvent handles of g_titleCardEvents */
    TitleImage images[TITLE_IMG_COUNT];
} TitleMenu;
extern TitleMenu g_titleMenu;

/* g_movieFlags (opening_movieThenTitle). The movie sets 7 through g_movieFlagQueue while it plays. */
#define MOVIE_NO_SCENES 0x1    /* don't run the scenes */
#define MOVIE_NO_FADE   0x2    /* don't run the screen fade (func_80032098) */
#define MOVIE_NO_OT     0x4    /* opening_movieSwap only puts the draw env, the decoder draws */
#define MOVIE_PLAYING   0x10
#define MOVIE_SKIPPING  0x20   /* Start/Cross pressed */
#define MOVIE_ENDED     0x40
#define MOVIE_DONE      0x80   /* go to the title (opening_movieOnEnd, or no movie) */
#define MOVIE_MUSIC     0x100  /* title music started */
#define MOVIE_IDLE_FADE 0x1000 /* the title's idle timeout started its fade-out */
/* g_movieState: opening_movieThenTitle */
enum {
    MOVIE_ST_START,   /* plays the movie with disc 1 in, else straight to MOVIE_ST_END */
    MOVIE_ST_OPEN,    /* MBG_init, mbg_play */
    MOVIE_ST_PLAY,
    MOVIE_ST_END,     /* waits for MOVIE_DONE and the CD, then enters the title */
    MOVIE_ST_TITLE,   /* runs the scenes until the idle timeout or SCENE_END */
    MOVIE_ST_TIMEOUT, /* returns 0 */
    MOVIE_ST_STARTED  /* returns 1 */
};
/* g_attractState: opening_attract */
enum {
    ATTRACT_ST_START,
    ATTRACT_ST_IMAGES, /* the ATTRACT_IMAGES images of the set */
    ATTRACT_ST_TITLE,
    ATTRACT_ST_TIMEOUT, /* returns 0 */
    ATTRACT_ST_STARTED  /* returns 1 */
};
#define TITLE_IDLE_FRAMES 0x4EC /* frames on the title before the attract loop goes on */
#define TITLE_IDLE_FADE   0x5A  /* fade-out at the end of those */

extern u16 g_movieFlags;
extern s32 g_movieFrame;
extern s16 g_movieFlagQueue[2][4];
extern void *g_attractData;
extern void *g_bootScreenData;
extern s32 g_movieState;

extern void *g_openingData;
extern u32 g_openingState; /* OPENING_* */
extern u16 g_attractImages[];
extern SPRT g_attractSprites[2][2];
extern DR_TPAGE g_attractTpages[2][2];
extern u8 g_attractSkipped;
#define ATTRACT_IMAGES 4     /* images per attract set */
#define ATTRACT_FRAMES 0x3A2 /* frames per image */
s32 opening_attractInit(s32);
s32 opening_attractFree(s32);
void opening_attractClearOt(void);
void opening_attractSwap(void);
extern SPRT g_bootScreenSprite[2];
extern DR_TPAGE g_bootScreenTpage[2];
void opening_bootScreenLoad(void);
void opening_bootScreenFree(void);
void opening_bootClearOt(void);
void opening_bootSwap(void);
extern u8 g_movieSkipped; /* 1: movie skipped or not played */
extern u16 g_movieLastFrame; /* g_mbgFrame seen last loop; the second half of g_movieSkipped's word */
void opening_movieOnEnd(void);
s32 scene_isTitleIdle(void);
void scene_update(void);
s32 opening_movieInit(void);
void opening_movieClearOt(void);
void opening_movieSwap(void);
extern s8 g_openingSoundPending; /* sound request kind, -1 = none */
void opening_setMusicVolume(s32, s32);
s32 opening_movieThenTitle(void);
s32 opening_attract(s32);
void opening_bootScreen(void);
void opening_findSceneSectors(void);
void opening_init(void);
void scene_init(void);
void scene_exit(void);
void opening_exit(void);
extern u16 g_attractFiles[];
extern s32 g_bootScreenImage;
extern u32 g_attractState;
/* g_attractFlags */
#define ATTRACT_FADING    0x1   /* this image's fade-out started */
#define ATTRACT_IDLE_FADE 0x10  /* the title's idle timeout started its fade-out */
#define ATTRACT_SKIP      0x100 /* Start/Cross: skip the remaining images */
typedef struct {
    u16 bits; /* ATTRACT_* */
} Ovl14Flags; /* a struct member in the original: loads of it are not moved above OT stores */
extern Ovl14Flags g_attractFlags;

void opening_initNewGame(void);
void opening_resetGameData(void);
void opening_resetFieldState(void);
void opening_resetMusicSlots(void);
void opening_loadMusic(void);
void opening_loadData(void);
void opening_freeData(void);
s32 opening_soundCmd(s32, s32, s32, s32, s32);
s32 scene_get(void);
void scene_set(s32);
s32 scene_checkDisc(void);
s32 scene_discIndex(s32);
u8 scene_isDiscGone(s32, s32);
void scene_setFadeScreen(s32);
void opening_discOnClose(void);
void opening_loadOnClose(void);
void opening_loadDone(void);
void opening_titleRefresh(void);
void opening_titleInput(void);
void opening_titleDraw(void);
void opening_titleTick(void);
void opening_titleLoadTims(s32 *);
void opening_titleDecide(void);
void func_800A8A8C(void);
void opening_titleDrawBackground(void);
void opening_titleDrawChoices(void);
void opening_titleDrawCursor(void);
void opening_titleStepFadeIn(void);
void opening_titleCloseCardEvents(void);
void opening_titlePollCard(void);
u8 opening_titleHasCard(void);
void opening_titleOnFadeOut(void);
void opening_titleConfirm(void);
void opening_titleStartCardInfo(void);
s32 opening_titleTestCardEvents(void);
void opening_titleDrawFading(s32, s32, s32, TitleImage *, s32);
u8 scene_getFadeScreen(void);
void opening_titleLoadImages(void);
void opening_titleOpenCardEvents(void);
s32 opening_titleGetCursor(void);
void opening_titleStartFadeIn(void);
void func_800AB1E4(void);
void func_800AB200(void);

#endif

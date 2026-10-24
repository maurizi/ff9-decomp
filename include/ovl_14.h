#ifndef OVL_14_H
#define OVL_14_H

/* Declarations local to overlay 0x14 (loaded at 0x800A7000). Addresses in the
 * overlay range mean different things in different overlays, so they live here,
 * not in ff9.h. */
#include "ff9.h"

typedef struct {
    union {
        u32 w; /* uv + clut, copied as one word */
        struct {
            u8 u;
            u8 v;
            u16 clut;
        } s;
    } unk0;
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Ovl14Text; /* 8-byte item drawn by func_80027710 */

/* 8-byte directory entry in the area at D_80067944 + 0x14C700 */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4; /* start; the next entry's start ends it */
} Ovl14Dir;

/* main exe: candidates for ff9.h */
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
void mbg_stop(void);
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
extern char **D_8019DC68;

/* other overlay (0x801F....) */
u8 func_801F5330(void);
void mbg_shutdown(void);

/* ---- this overlay ---- */

typedef struct {
    void (*unk0)(void); /* init */
    void (*unk4)(void); /* enter */
    void (*unk8)(void); /* update */
    void (*unkC)(void); /* leave */
} Ovl14Mode;
extern Ovl14Mode g_sceneModes[6];

typedef struct {
    u8 unk0; /* current mode, index into g_sceneModes */
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Ovl14ModeState;
extern Ovl14ModeState g_scene;
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
extern u16 g_movieFlags;
extern s32 g_movieFrame;
extern s16 g_movieFlagQueue[2][4];
extern void *g_attractData;
extern void *g_bootScreenData;
extern s32 g_movieState;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4[2];
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
    s32 unkC;
    s32 unk10;
    s32 unk14[4]; /* sound/voice handles */
    Ovl14Text unk24[14];
} Ovl14Unk320;
extern Ovl14Unk320 g_titleMenu;

extern void *g_openingData;
extern u32 g_openingState; /* main state */
extern u16 g_attractImages[];
extern SPRT g_attractSprites[2][2];
extern DR_TPAGE g_attractTpages[2][2];
extern u8 g_attractSkipped;
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
extern u8 g_movieSkipped; /* 1: FMV skipped */
extern u16 g_movieLastFrame; /* FMV frame seen last loop; no splat symbol, see ovl_14.c */
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
typedef struct {
    u16 unk0; /* flags */
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
void opening_titleDrawFading(s32, s32, s32, Ovl14Text *, s32);
u8 scene_getFadeScreen(void);
void opening_titleLoadImages(void);
void opening_titleOpenCardEvents(void);
s32 opening_titleGetCursor(void);
void opening_titleStartFadeIn(void);
void func_800AB1E4(void);
void func_800AB200(void);

#endif

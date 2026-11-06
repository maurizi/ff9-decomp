#ifndef OVL_3E_H
#define OVL_3E_H

/* Declarations local to overlay 0x3E: the "insert disc" / disc swap screen.
 * Checks the CD lid and disc type, shows a two-part 160x224 picture and waits
 * for the right disc. Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- overlay ---- */

/* DiscChange.state: index into s_discChange{Input,Draw,Poll}Funcs */
enum {
    DISC_ST_BUSY,        /* fading / checking the disc: no input or poll handler */
    DISC_ST_LID_CLOSED,  /* picture shown, lid closed: Cross checks */
    DISC_ST_LID_OPEN,    /* picture shown, lid open: closing it (or Cross) checks */
    DISC_ST_ERROR,       /* check failed: error image until Cross or DISC_ERROR_FRAMES */
    DISC_ST_ERROR_CLOSE  /* fading the error image out */
};
#define DISC_ERROR_FRAMES 180
#define DISC_FADE_FRAMES 0x18 /* fade_out / fade_in around every check and the error panel */
#define DISC_SPINUP_FRAMES 600 /* discChange_checkDisc: give up (DISC_CHECK_NO_DISC) if not in standby by then */
#define DISC_IMG_INIT_TRIES 10 /* file_initImg attempts before the disc counts as unreadable */
#define DISC_SEEK_SECTOR 5 /* test seek target (CdIntToPos) */

/* discChange_checkDisc results */
enum {
    DISC_CHECK_SHELL_OPEN = 1, /* CdlStatShellOpen seen */
    DISC_CHECK_NO_DISC,        /* motor didn't spin up in 600 frames, or CdGetDiskType says no disc */
    DISC_CHECK_OTHER_FORMAT,   /* CdDiskReady error or not a CD-ROM */
    DISC_CHECK_ERROR,
    DISC_CHECK_WRONG_DISC = 6, /* g_gameState->disc doesn't have the wanted disc's bit */
    DISC_CHECK_OK              /* the wanted disc */
};

/* g_menu.arg when the menu opens: wanted disc in bits 14-15, next location in bits 0-13 */
#define DISC_REQ_DISC(x)     (((x) >> 14) & 3)
#define DISC_REQ_LOCATION(x) ((u16)(x) & 0x3FFF)
#define DISC_PICTURE_W 160 /* the 320x224 16-bit picture is read and drawn as two halves */
#define DISC_PICTURE_HALF_BYTES 0x11800 /* 160 * 224 * 2 */
#define DISC_PICTURE_HALF_SECTORS 0x23
/* screen layout: the picture fills the screen in two DISC_PICTURE_W x SCREEN_H sprites; in DISC_ST_ERROR /
 * DISC_ST_ERROR_CLOSE the error panel (its own image in VRAM, loaded with the picture) replaces it */
#define DISC_OTZ 6 /* OT slot of the picture and the panel */
#define DISC_PANEL_X 0x4E
#define DISC_PANEL_Y 0x1C
#define DISC_PANEL_W 0xC2
#define DISC_PANEL_H 0xA8
#define DISC_PANEL_VRAM_X 0x140 /* panel pixels (tpage) */
#define DISC_PANEL_VRAM_Y 0x100
#define DISC_PANEL_CLUT_Y 0x1FF /* panel CLUT row (x 0) */
#define DISC_PICTURE_BUF PSX_RAM(u32 *, 0x801CDF00) /* CD read buffer for one half */

/* Disc change state (s_discChange). */
typedef struct {
    u8 shellOpen;   /* last cd_checkShellOpen */
    u8 state;       /* DISC_ST_* */
    u8 unk2;        /* cleared, never read */
    u8 checkResult; /* last discChange_checkDisc (DISC_CHECK_*) */
    u8 disc;        /* wanted disc (0-3) */
    u8 pad5;
    s16 nextLocation; /* DISC_REQ_LOCATION, or DISC_NO_LOCATION */
    s32 savedFlags[2]; /* g_padPortMask[0..1], restored on exit */
    u32 timer;      /* frames in DISC_ST_ERROR */
} DiscChange;
extern DiscChange s_discChange;

extern void (*s_discChangeInputFuncs[])(void);
extern void (*s_discChangeDrawFuncs[])(void);
extern void (*s_discChangeUpdateFuncs[])(void);
extern RECT s_discChangeClutRect;
extern RECT s_discChangePanelRect;
extern s32 s_discChangePictureFiles[];
extern RECT s_discChangePictureRect;
extern u32 s_discChangeClut[];

void discChange_drawBackground(void);
void discChange_drawPanel(void);
u8 discChange_checkDisc(void);
void discChange_input(void);
void discChange_draw(void);
void discChange_updateState(void);
void discChange_loadPicture(void);
void discChange_loadPictureHalf(u32 *, s32);
void discChange_startCheck(void);
void discChange_doCheck(void);
void discChange_closeError(void);
void discChange_resetState(void);

#endif

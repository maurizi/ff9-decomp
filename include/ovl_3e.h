#ifndef OVL_3E_H
#define OVL_3E_H

/* Declarations local to overlay 0x3E: the "insert disc" / disc swap screen.
 * Checks the CD lid and disc type, shows a two-part 160x224 picture and waits
 * for the right disc. Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
#ifndef SE_CURSOR
#define SE_CURSOR 0x67  /* cursor move / pick */
#define SE_BUZZER 0x66  /* refused action */
#define SE_CANCEL 0x65  /* Circle backs out */
#endif
/* libcd (PsyQ libcd.h names) */
#define CdlNop           0x01
#define CdlStandby       0x07
#define CdlSetmode       0x0E
#define CdlSeekL         0x15
#define CdlModeSpeed     0x80
#define CdlStatError     0x01
#define CdlStatStandby   0x02
#define CdlStatShellOpen 0x10
#define CdlComplete      0x02
#define CdlDiskError     0x05
#define CdlStatNoDisk    0
#define CdlOtherFormat   1
#define CdlCdromFormat   2

/* ---- overlay ---- */

/* DiscSwap.state: index into g_menuDisc{Input,Draw,Poll}Funcs */
enum {
    DISC_ST_BUSY,        /* fading / checking the disc: no input or poll handler */
    DISC_ST_LID_CLOSED,  /* picture shown, lid closed: Cross checks */
    DISC_ST_LID_OPEN,    /* picture shown, lid open: closing it (or Cross) checks */
    DISC_ST_ERROR,       /* check failed: error image until Cross or DISC_ERROR_FRAMES */
    DISC_ST_ERROR_CLOSE  /* fading the error image out */
};
#define DISC_ERROR_FRAMES 180

/* menu_disc_check results */
enum {
    DISC_CHECK_SHELL_OPEN = 1, /* CdlStatShellOpen seen */
    DISC_CHECK_NO_DISC,        /* motor didn't spin up in 600 frames, or CdGetDiskType says no disc */
    DISC_CHECK_OTHER_FORMAT,   /* CdDiskReady error or not a CD-ROM */
    DISC_CHECK_ERROR,
    DISC_CHECK_WRONG_DISC = 6, /* g_gameState->unkB doesn't have the wanted disc's bit */
    DISC_CHECK_OK              /* the wanted disc */
};

/* g_menu.unkC when the menu opens: wanted disc in bits 14-15, next location in bits 0-13 */
#define DISC_REQ_DISC(x)     (((x) >> 14) & 3)
#define DISC_REQ_LOCATION(x) ((u16)(x) & 0x3FFF)
#define DISC_NO_LOCATION     15000 /* don't call evt_setNextLocation on exit */

#define DISC_PICTURE_W 160 /* the 320x224 16-bit picture is read and drawn as two halves */
#define DISC_PICTURE_HALF_BYTES 0x11800 /* 160 * 224 * 2 */
#define DISC_PICTURE_HALF_SECTORS 0x23

/* Disc swap state (g_menuDisc). */
typedef struct {
    u8 shellOpen;   /* last cd_checkShellOpen */
    u8 state;       /* DISC_ST_* */
    u8 unk2;        /* cleared, never read */
    u8 checkResult; /* last menu_disc_check (DISC_CHECK_*) */
    u8 disc;        /* wanted disc (0-3) */
    u8 pad5;
    s16 nextLocation; /* DISC_REQ_LOCATION, or DISC_NO_LOCATION */
    s32 savedFlags[2]; /* D_80073998[0..1], restored on exit */
    u32 timer;      /* frames in DISC_ST_ERROR */
} DiscSwap;
extern DiscSwap g_menuDisc;

extern void (*g_menuDiscInputFuncs[])(void);
extern void (*g_menuDiscDrawFuncs[])(void);
extern void (*g_menuDiscPollFuncs[])(void);
extern RECT g_menuDiscClutRect;
extern RECT g_menuDiscErrorRect;
extern s32 g_menuDiscPictureFiles[];
extern RECT g_menuDiscPictureRect;
extern u32 g_menuDiscClut[];

void menu_disc_drawPicture(void);
void menu_disc_drawError(void);
u8 menu_disc_check(void);
void menu_disc_input(void);
void menu_disc_draw(void);
void menu_disc_poll(void);
void menu_disc_loadImages(void);
void menu_disc_loadPictureHalf(u32 *, s32);
void menu_disc_startCheck(void);
void menu_disc_doCheck(void);
void menu_disc_closeError(void);
void menu_disc_resetState(void);

#endif

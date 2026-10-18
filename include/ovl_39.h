#ifndef OVL_39_H
#define OVL_39_H

/* Declarations local to overlay 0x39 (save / load menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

typedef struct {
    u8 v[20];
} SaveName20; /* memory card file name (DIRENTRY.name) */

typedef struct {
    SaveName20 unk0;
    u8 unk14[2]; /* terminator */
} __attribute__((aligned(2))) SaveFileName; /* copied as a halfword-aligned block */

/* PsyQ libapi struct DIRENTRY */
typedef struct {
    SaveName20 unk0; /* name */
    s32 unk14; /* attr */
    s32 unk18; /* size */
    void *unk1C; /* next */
    s32 unk20; /* head */
    u8 unk24[4]; /* system */
} DirEnt;

typedef struct {
    u8 v[10];
} __attribute__((aligned(2))) SaveName; /* 10 bytes, copied as a halfword-aligned block */

typedef struct {
    u8 v[10];
} Name10; /* character name, byte-aligned copy */

typedef struct {
    s32 v[7];
} SaveBlk1C;

typedef struct {
    u8 unk0; /* present */
    u8 unk1;
    u8 unk2[0xA]; /* name */
    u8 unkC; /* level */
    u8 padD[3];
} SaveMember;

/* Per-slot entry of the save list (0x98 bytes; the first byte is the state). */
typedef struct {
    s32 unk0; /* version tag, 0x30005 */
    SaveFileName unk4;
    u8 unk1A; /* state: 1 = empty, 3 = foreign, 4 = used */
    u8 unk1B; /* slot index */
    u8 unk1C;
    u8 unk1D;
    SaveName unk1E;
    SaveBlk1C unk28;
    u8 unk44;
    u8 pad45[0x48 - 0x45];
    s32 unk48; /* config bit */
    u32 unk4C; /* save counter */
    s32 unk50;
    s32 unk54;
    SaveMember unk58[4]; /* party */
} SaveSlot;

/* Save file header block read into D_801FBEF8.unk2D0 + 0x100 (func_801F7190). */
typedef struct {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    SaveName unk6;
    SaveBlk1C unk10;
    s32 unk2C;
    s32 unk30;
    SaveMember unk34[4];
    s32 unk74;
    s32 unk78;
    u8 unk7C;
} SaveHeader;

/* Save menu state (D_801FB1A8). */
typedef struct {
    u8 unk0;
    u8 unk1; /* state */
    u8 unk2; /* 1 = busy, 2 = done */
    u8 unk3; /* selected slot */
    u8 unk4[2]; /* card status per slot */
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
    u32 unkC;  /* highest save counter */
    s32 unk10; /* slot holding it */
    s32 unk14; /* saved func_8002F8E4 mode */
    s32 unk18[2]; /* saved D_80073998[] */
    u8 *unk20; /* help text */
    MenuList unk24; /* slot list */
    SaveSlot unk34[15];
} SaveMenu;
extern SaveMenu D_801FB1A8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk801FBAC8;
extern Unk801FBAC8 D_801FBAC8;

typedef struct Unk801FBEE0 {
    u8 unk0;
    u8 unk1;
    u16 unk2; /* text size: w | rows << 12 */
    s32 unk4;
    Window *unk8;
    void (*unkC)(struct Unk801FBEE0 *);
    u16 unk10; /* text ids */
    u16 unk12;
} Unk801FBEE0;
extern Unk801FBEE0 D_801FBEE0;
s32 text_getInfo(s32); /* main exe */

typedef struct {
    u8 unk0;
    u8 unk1; /* state, index into D_801FB014 */
    u8 unk2; /* slot */
    u8 unk3; /* attempts */
    u8 unk4[4]; /* attempts per result */
    s32 unk8;
    s32 unkC;  /* used slots */
    s32 unk10; /* file count */
    s32 unk14;
    s32 unk18; /* file descriptor */
    Window *unk1C;
    DirEnt unk20[15];
    u8 pad278[0x2B4 - 0x278];
    u8 unk2B4[15];
    u8 pad2C3[0x2D0 - 0x2C3];
    u8 unk2D0[0x180]; /* read buffer */
} Unk801FBEF8;
extern Unk801FBEF8 D_801FBEF8;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    u32 unk4; /* frame counter */
    Window *unk8;
} Unk801FC348;
extern Unk801FC348 D_801FC348;

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2; /* text size: w | rows << 12 */
    Window *unk4;
    u8 *unk8; /* text */
} Unk801FC358;
extern Unk801FC358 D_801FC358;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    SaveFileName unk6; /* file name */
    Window *unk1C;
    s32 unk20; /* card channel */
    s32 unk24;
} Unk801FC388;
extern Unk801FC388 D_801FC388;

/* memory card status */
extern s32 D_801FB074;
extern s32 D_801FB078;
extern s32 D_801FB07C;
extern s32 D_801FB080;
extern s32 D_801FB084;
extern s32 D_801FB088;
extern s32 D_801FB08C;
extern s32 D_801FC604;
extern void (*D_801FB098)(void); /* memory card completion callback */
extern s32 D_801FB09C;
extern s32 D_801FB0A0;
extern u8 D_801EE4D4[];
extern u16 D_801FC3E0[256]; /* CRC-16 table */
/* CRC accumulator: the union initializer emits a clobber, so gcc cannot prove
   the upper bits are zero and keeps both andi 0xFFFF (func_801F973C). */
typedef union { s32 w; u16 h; } Crc16;
extern s32 D_801FC5E0;
extern s32 D_801FC5E4;
extern s32 D_801FC5E8;
extern s32 D_801FC5EC;
extern s32 D_801FC5F4;
extern s32 D_801FC5F8;
extern s32 D_801FC5FC;
extern s32 D_801FC600;
extern s32 D_801FC608;
extern s32 D_801FC60C;
extern s32 D_801FC610;
extern s32 D_801FC614;

typedef struct {
    s32 unk0;
    Unk880 unk4;
    u8 unk14;
} Unk801FA078;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2[0x20 - 2];
    u16 unk20;
    u16 unk22;
    u16 unk24;
    s16 unk26;
    u16 unk28;
    u16 unk2A;
    u8 unk2C[0x14];
} Unk801F9BEC;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    Unk6 unk6[0x69];
    u16 unk27C;
    u16 unk27E;
} CardData; /* Tetra Master cards (func_80062824) */

/* other overlay */
u16 func_800B5084(void);
u16 func_800B5094(void);

typedef struct {
    EventVars unk0;
    s32 unk800; /* D_8007AEC4 */
} SaveEvents;

typedef struct {
    Character unk0[9];
    u8 unk510[4]; /* party: character index, 9 = none */
    u16 unk514;
    u16 unk516;
    u32 unk518; /* gil */
    u16 unk51C;
    u16 unk51E;
    u16 unk520;
} SaveParty; /* 0x524 */

/* inventory part of a save block (func_801F9EA8 / func_801F9F5C) */
typedef struct {
    ItemInventory unk0;
    ItemFlags unk200;
    GameStateUnk24 unk240; /* GameState.unk24/unk28 */
    u8 pad248[0x258 - 0x248];
} SaveItems;

/* game data part of a save block */
typedef struct {
    Unk801F9BEC unk0;
    SaveEvents unk40;
    u8 pad844[0x850 - 0x844];
    SaveParty unk850;
    u8 padD74[0xD88 - 0xD74];
    Unk801FA078 unkD88;
    SaveItems unkDA0;
    u8 unkFF8[0x127C - 0xFF8];
} SaveData; /* 0x127C */

/* save block buffer at 0x801FCB00 */
typedef struct {
    u8 pad0[4];
    u8 unk4;   /* GameState.unkB */
    u8 pad5[0x2C - 5];
    s32 unk2C; /* game_setPlayTime */
    u8 pad30[0x80 - 0x30];
} SaveHdr;

typedef struct {
    u8 unk0[0x100]; /* header */
    SaveHdr unk100;
    SaveData unk180;
    u8 pad13FC[2];
    u16 unk13FE; /* CRC-16 of everything before it */
} SaveBuf;

void func_801F963C(SaveData *);
void func_801FA01C(Unk801FA078 *);
typedef struct {
    u8 pad0[0x12];
    u8 unk12; /* ASCII digits */
    u8 unk13;
} Unk801F6D24;

/* main exe: candidates for ff9.h */
Unk8004AB64 *func_8004AE00(void);
u8 func_800B5074(void); /* other overlay */
void func_801F9120(u8 *, s32);
s32 func_801F9688(void);
void func_8002397C(void);
extern RECT D_801EDF14;
extern s32 D_801FB050[]; /* text index per mode */
extern RECT D_801FB058;
void func_801F5E40(void);
void func_801F5E90(s32, s32, s32);
void func_801F95E4(); /* also used as a Unk801FBEE0 callback */
void func_801F62F0(void (*)(Unk801FBEE0 *));
void func_801F6BD4(void);
void func_801F770C(void);
void func_801F734C(void);
void func_801F61B0(void);
void func_801F830C(void);
void func_801F7928(void);
void func_801F5A60(void);
void func_801F5BE0(void);
void func_801F500C(void);
u8 func_801F8524(void);
void func_801F6EDC(Window *);
void func_801F6F3C(Window *);
void func_801F6F74(Window *);
u8 func_801F5E54(u8);
extern RECT D_801FB00C;
extern s32 D_80070420;
void func_801F6368(Window *);
void func_801F6378(Window *);
void func_801F5FE4(Window *);
void func_801F60DC(Window *);
void func_801F49C4(RECT *, s32, s32);
void func_801F5688(void);
void func_801F573C(void);
void func_801F8BD4(MenuList *, RECT *, RECT *);
extern s32 D_80070444;
extern u8 D_801FB0A8[]; /* help text buffer */
void func_8002BE5C(s32, s32); /* main exe */
extern u8 *D_80077DF0; /* = D_80077DE8.unk8, referenced by its own name */
extern void (*D_801FAFE8[])(RECT *, s32, s32); /* slot drawer per state */
typedef struct {
    u8 unk0; /* u */
    u8 unk1; /* v */
    u8 unk2; /* w */
    u8 unk3; /* h */
} Ovl39Sprite;
extern Ovl39Sprite D_801FAFFC[];
u8 func_801F7E68(void);
extern RECT D_801FAF7C;
extern RECT D_801FAF84;
void func_801F77F4(Window *);
void func_801F7830(Window *);
void func_801F7840(Window *);
void func_801F5114(void);
u8 *func_801F6320(void);
void func_801F7CD0(Window *);
s32 func_801FA2B8();
s32 func_801FA2C8();
void func_801F7FD4(s32, s32, s32, s32, s32);
void func_801F8408(Window *);
void func_801F8424(Window *);
s32 firstfile(char *, s32); /* PsyQ libapi: struct DIRENTRY *firstfile(char *, struct DIRENTRY *) */
s32 func_80028954(s32); /* defined u8 in main; used unmasked */

extern RECT D_801FAF74; /* a RECT in .data (splat starts .data late) */
extern u8 g_saveMenuText[];
extern u8 D_801FB040[]; /* save icon per character model */
extern u16 D_801FDEFE;
void func_801F96CC(void);
extern RECT D_801FAFE0;
extern u8 D_801FBACC[]; /* CLUT + image buffer */
extern u8 D_801F2854[]; /* icon images, 0x210 each */
s32 func_801FA1C4(s32, s32);
extern u8 D_801FC3B8[0x26]; /* name buffer */
u8 func_800B5064(void); /* other overlay */
void func_801F9024(u8 *);
extern u8 D_801FB02C[]; /* retries allowed per result */
void func_801F6E68(s32);
void func_801F6DAC(void);
void func_801F6E1C(void);
void func_801F70E4(void);
char *strcpy(char *, const char *); /* C library (builtin) */
s32 open(char *, s32); /* main exe: open */
s32 func_801FA3D8(s32, char *);
u8 func_801FA6AC(s32, char *);
void game_setPlayTime(s32); /* main exe */
void func_801F9BEC(Unk801F9BEC *);
void func_801F9F5C(SaveItems *);
void func_801F9D18(void *);
void func_801FA078(Unk801FA078 *);
void func_801F8DA4(u8 *);
u16 func_801F973C(u8 *, s32);
void func_801F9B00(Unk801F9BEC *);
void func_801F9D48(SaveEvents *);
void func_801F9DF8(SaveEvents *);
void func_801F99FC(SaveParty *);
void func_801F9904(SaveParty *);
extern s32 D_8007AEC4;
void func_801F9EA8(SaveItems *);
void func_801F9CE8(void *);
void func_801F6938(void);


void func_801F4538(void);
void func_801F53E0(void);
void func_801F5460(void);
void func_801F54C8(void);
void func_801F5550(void);
void func_801F5A24(void);
void func_801F5A68(s32, s32, s32, s32);
void func_801F75C8(s32, s32);
void func_801F7084(void);
void func_801F5E68(s32, s32, s32);
void func_801F6A84(void);
void func_801F6B48(void);
void func_801F72B4(s32);
u8 func_801F77E4(void);
void func_801F9788(s32, s32);
void func_801F94D0(s32, s32);
s32 func_801FA53C(void);
void func_801FAE74(void);
s32 func_801FA2D8(void);
s32 func_801FA350(s32, void (*)(void));
s32 func_801FA754(s32);
void func_801F65C4(void);
void func_801F7300(s32);
void func_801F7364(SaveMenu *);
void func_801F77A8(s32);
void func_801F7918(void);
void func_801F889C(s32);
s32 func_801FAB08(void);
void func_801FAE1C(void);
void func_801F7780(s32);
u8 func_801F79F4(s32);
void func_801F9C58(CardData *, CardData *);
s32 func_801FA37C(s32);
s32 func_801FA4E4(s32, s32, s32);
s32 func_801FA5D0(s32, s32, s32);
void func_801FAAF4(void);
void func_801F4E00(void);
void func_801F5D00(void);
u8 func_801F6694(void);
s32 func_801F6CD0(s32);
void func_801F7108(s32);
void func_801F7190(SaveSlot *, SaveHeader *);
void func_801F7950(void);
void func_801F89A0(Window *);
s32 func_801FA478(s32);
s32 func_801FA184(s32);
s32 func_801FABB8(void);
void func_801FAECC(void);
u8 func_801F5340(s32, u8);
s32 func_801FA0F4(s32, s32);
void func_801F8904(void);
extern u8 D_801FC368[13];
extern u8 D_801F43A8[13]; /* "BASLUS-01251" */
s32 func_801FA2E8(s32);
s32 func_801FA644(void);
s32 func_801FA21C(s32);
void func_801FA794(void);
void func_801F7718(void);
void func_801F7AF4(void);
void func_801F7C20(void);
void func_801F98D0(void);
void func_801F63D8(void);
void func_801F59B8(void);
void func_801F5DE0(void);
extern RECT D_801FB030;
extern u8 D_801FB038[];
extern s32 D_801FBF18;
s32 func_801FA240(s32);
s32 func_801FAAE4(); /* unprototyped: callers pass the channel or nothing */
void func_801F8610(void);
void func_801F86C0(s32);
void func_801F78CC(void);
void func_801F74F4(Window *);
void func_801F7510(Window *);
void func_801F7560(Window *);
u8 func_801F8478(void);
void func_801F8588(void);
MenuView *func_801F89B8(MenuList *, MenuView *);
void func_801F8704(s32, SaveHeader *);
u8 func_801F9804(void);
void func_801F5384(void);
void func_801F53B0(void);
void func_801F5960(void);
void func_801F8C34(MenuList *, s32);
void func_801F8CE8(MenuList *, s32);
MenuView *func_801F8B10(MenuList *, MenuView *);
void func_801F49C4(RECT *, s32, s32);
extern void (*D_801FAF8C[])(void);
extern void (*D_801FAFC4[])(void);
extern void (*D_801FAFA8[])(void);
extern void (*D_801FB014[])(void);



/* Inlined copy of func_801F5340 (slot usable?). */
static inline u8 func_801F5340_inline(s32 slot, u8 any) {
    s32 st = D_801FB1A8.unk4[slot];
    s32 r;

    if (any) {
        r = 0;
        if (st == 0 || st == 3) {
            r = 1;
        }
        return r;
    }
    return st == 0;
}

extern s32 D_801FB090[2]; /* per card port: retries; D_801FB088 holds the status */
/* main exe: candidates for ff9.h */
void lseek(s32, s32, s32);
/* card status helpers inlined into func_801FA794 */
static inline void func_801FA794_set(s32 chan, s32 r) {
    s32 *p = &D_801FB088;
    p[chan / 16] = r;
    D_801FB090[chan / 16] = 0;
}

static inline void func_801FA794_retry(s32 chan, s32 r) {
    s32 *p = D_801FB090;
    if (p[chan / 16] > 0) {
        (&D_801FB088)[chan / 16] = r;
    } else {
        p[chan / 16]++;
    }
}

s32 func_801F6FE8(void);
u8 *func_801F83B8(void);
s32 func_801F6D24(Unk801F6D24 *);
s32 func_801F6C74(void);
s32 func_801F6F7C(void);

typedef struct {
    u8 v[0x23];
} TitleTmpl; /* memory card title template (SJIS) */
typedef struct {
    u8 v[0xA0];
} IconTmpl; /* memory card icon CLUT + bitmap */
extern TitleTmpl D_801F43B8;
extern IconTmpl D_801F4104;
extern IconTmpl D_801F41A4;
extern IconTmpl D_801F4244;
extern IconTmpl D_801F42E4;

/* main exe: candidates for ff9.h */
/* slider gauge pieces drawn by func_80027710: left cap, middle (repeated via DR_TWIN), right cap */
typedef struct {
    u8 unk0; /* texture window x */
    u8 unk1; /* texture window y */
    u16 unk2; /* uv */
    u8 unk4; /* width */
    u8 unk5; /* height */
    u8 pad6[2];
} GaugeIcon;
extern GaugeIcon D_80068988[3];
void SetTexWindow(void *p, RECT *r);

/* LINE_F2 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineWords;
extern u32 D_801FB068[3]; /* gauge line colours */
extern RECT D_801FB060; /* gauge texture window */


#endif

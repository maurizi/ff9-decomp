#ifndef PSYQ_H
#define PSYQ_H

/* Minimal PsyQ SDK declarations, added as the game code needs them. */
#include "common.h"

typedef struct {
    s16 x, y;
    s16 w, h;
} RECT;

/* libgpu primitive tag (24-bit OT link + 8-bit word count) and its macros. */
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
    u8 r0, g0, b0, code;
} P_TAG;

#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u32)(_addr))
#define getaddr(p) (u32)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

#define setRECT(r, _x, _y, _w, _h) ((r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h))
#define setXY0(p, _x0, _y0) ((p)->x0 = (_x0), (p)->y0 = (_y0))
#define setUV0(p, _u0, _v0) ((p)->u0 = (_u0), (p)->v0 = (_v0))
#define setWH(p, _w, _h) ((p)->w = (_w), (p)->h = (_h))
#define setRGB0(p, _r0, _g0, _b0) ((p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0))

typedef struct {
    u32 tag;
    u32 code[15];
} DR_ENV;

typedef struct {
    RECT clip;
    s16 ofs[2];
    RECT tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV; /* 0x5C */

typedef struct {
    RECT disp;
    RECT screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0, pad1;
} DISPENV; /* 0x14 */

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 w, h;
} SPRT;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u32 tag;
    u32 code[1];
} DR_TPAGE;

typedef struct {
    u8 minute, second, sector, track;
} CdlLOC;

typedef struct {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdlATV;
int CdMix(CdlATV *vol);

typedef struct {
    CdlLOC pos;
    u32 size;
    char name[16];
} CdlFILE;

/* libgte */
typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    u8 r, g, b, cd;
} CVECTOR;

MATRIX *TransMatrix(MATRIX *m, VECTOR *v);

long VSync(long mode);
/* C library: gcc 2.8 expands small constant-size memcpy inline (lwl/lwr) */
void *memcpy(void *, const void *, u32);

int DrawSync(int mode);
int LoadImage(RECT *rect, u32 *p);
int StoreImage(RECT *rect, u32 *p);
int MoveImage(RECT *rect, int x, int y);
void ClearImage(RECT *rect, s32 r, s32 g, s32 b); /* real lib: u_char r, g, b */
int ResetGraph(int mode);
void SetDispMask(int mask);
u32 *ClearOTag(u32 *ot, int n);
u32 *ClearOTagR(u32 *ot, int n);
void DrawOTag(u32 *p);
void AddPrim(void *ot, void *p);
void SetSprt(SPRT *p);
void DrawOTagEnv(u32 *p, DRAWENV *env);
DRAWENV *PutDrawEnv(DRAWENV *env);
DISPENV *PutDispEnv(DISPENV *env);
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
void SetDrawTPage(void *p, int dfe, int dtd, int tpage);
void SetSemiTrans(void *p, int abe);
void SetTile(TILE *p);
u16 GetTPage(int tp, int abr, int x, int y);
u16 GetClut(int x, int y);

void *memmove(void *dst, const void *src, int n);
int sprintf(char *buf, const char *fmt, ...);

int CdControlF(u8 com, u8 *param);
int CD_getsector(void *madr, int size);
int CD_getsector2(void *madr, int size);
void *SpuSetTransferCallback(void (*func)(void));
u32 SpuSetTransferStartAddr(u32 addr);
void SpuSetReverb(int on_off);
long GetRCnt(unsigned long spec);
long SpuSetReverbModeType(long mode);
void SpuSetIRQAddr(u32 addr);
int SpuSetIRQ(int on_off);
void SpuGetVoiceEnvelope(int vNum, short *envx);
void *SpuSetIRQCallback(void (*func)(void));
int CdControlB(u8 com, u8 *param, u8 *result);
int CdReset(int mode);
int CdDiskReady(int mode);
int CdGetDiskType(void);
CdlLOC *CdIntToPos(int i, CdlLOC *p);
int CdPosToInt(CdlLOC *p);
CdlFILE *CdSearchFile(CdlFILE *fp, char *name);

long _card_format(long chan);
long _card_clear(long chan);

void SpuStart(void);
long SpuInitMalloc(long num, char *top);
long SpuSetTransferMode(long mode);
long SetRCnt(unsigned long spec, unsigned short target, long mode);
long StartRCnt(unsigned long spec);

#endif

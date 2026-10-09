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

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u32)(_addr))
#define getaddr(p) (u32)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setcode(p, _code) (((P_TAG *)(p))->code = (u8)(_code))
#define getcode(p) (u8)(((P_TAG *)(p))->code)
#define _get_mode(dfe, dtd, tpage) ((0xe1000000) | ((dtd) ? 0x0200 : 0) | ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9ff))
#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))
#define setSemiTrans(p, abe) ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define setPolyG4(p) setlen(p, 8), setcode(p, 0x38)
/* libgpu.h primitive-word helpers */
#define get_mode(dfe, dtd, tpage) \
    ((0xE1000000) | ((dtd) ? 0x0200 : 0) | ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9FF))
#define get_tw(p) \
    (0xE2000000 | ((p)->y & 0xFF) >> 3 << 15 | ((p)->x & 0xFF) >> 3 << 10 | \
     (~((p)->h - 1) & 0xFF) >> 3 << 5 | (~((p)->w - 1) & 0xFF) >> 3)

#define setRECT(r, _x, _y, _w, _h) ((r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h))
#define setXY0(p, _x0, _y0) ((p)->x0 = (_x0), (p)->y0 = (_y0))
#define setUV0(p, _u0, _v0) ((p)->u0 = (_u0), (p)->v0 = (_v0))
#define setWH(p, _w, _h) ((p)->w = (_w), (p)->h = (_h))
#define setRGB0(p, _r0, _g0, _b0) ((p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0))
#define setRGB1(p, _r1, _g1, _b1) ((p)->r1 = (_r1), (p)->g1 = (_g1), (p)->b1 = (_b1))
#define setRGB2(p, _r2, _g2, _b2) ((p)->r2 = (_r2), (p)->g2 = (_g2), (p)->b2 = (_b2))
#define setRGB3(p, _r3, _g3, _b3) ((p)->r3 = (_r3), (p)->g3 = (_g3), (p)->b3 = (_b3))

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
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} POLY_FT4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
} POLY_FT3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
} POLY_GT3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad3;
} POLY_GT4;

typedef struct {
    u32 tag;
    u32 code[1];
} DR_TPAGE;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_STP;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} POLY_F3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} POLY_F4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} POLY_G3;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} POLY_G4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
} LINE_F2;

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
u16 LoadTPage(u32 *pix, s32 tp, s32 abr, s32 x, s32 y, s32 w, s32 h);
u16 LoadClut(u32 *clut, s32 x, s32 y);
void DrawOTagEnv(u32 *p, DRAWENV *env);
DRAWENV *PutDrawEnv(DRAWENV *env);
DISPENV *PutDispEnv(DISPENV *env);
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
int DrawSyncCallback(void (*func)(void));
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
int CdInit(void);
typedef struct {
    short left;
    short right;
} SpuVolume;

typedef struct {
    unsigned long mask;
    long mode;
    SpuVolume depth;
    long delay;
    long feedback;
} SpuReverbAttr;

long SpuSetReverbModeParam(SpuReverbAttr *attr);
void SpuSetReverbDepth(SpuReverbAttr *attr);
long SpuClearReverbWorkArea(long mode);
void VSyncCallback(void (*f)(void));
void ResetCallback(void);
int SetGraphDebug(int level);
void InitGeom(void);
void SpuInit(void);
void InitCARD(long val);
long StartCARD(void);
void PadInitDirect(u8 *pad1, u8 *pad2);
int PadStartCom(void);

long _card_format(long chan);
long _card_clear(long chan);

long ratan2(long y, long x);
void SpuStart(void);
long SpuInitMalloc(long num, char *top);
long SpuSetTransferMode(long mode);
long SetRCnt(unsigned long spec, unsigned short target, long mode);
long StartRCnt(unsigned long spec);

/* merged from field/world-map agent */
#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))
#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3) \
    ((p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x1), (p)->y1 = (_y1), (p)->x2 = (_x2), (p)->y2 = (_y2), \
     (p)->x3 = (_x3), (p)->y3 = (_y3))
#define setXYWH(p, _x0, _y0, _w, _h) \
    ((p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), (p)->x2 = (_x0), \
     (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h))
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) \
    ((p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u1), (p)->v1 = (_v1), (p)->u2 = (_u2), (p)->v2 = (_v2), \
     (p)->u3 = (_u3), (p)->v3 = (_v3))
#define setUVWH(p, _u0, _v0, _w, _h) \
    ((p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), (p)->u2 = (_u0), \
     (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h))
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
} SPRT_16;
long SquareRoot0(long a);
void SetGeomOffset(long ofx, long ofy);
typedef struct {
    s16 vx, vy;
} DVECTOR;
#define setVector(v, _x, _y, _z) (v)->vx = _x, (v)->vy = _y, (v)->vz = _z
int VectorNormal(VECTOR *v0, VECTOR *v1);
#define gte_SetRotMatrix(r0) __asm__ volatile( \
    "lw\t$12, 0(%0);" \
    "lw\t$13, 4(%0);" \
    "ctc2\t$12, $0;" \
    "ctc2\t$13, $1;" \
    "lw\t$12, 8(%0);" \
    "lw\t$13, 12(%0);" \
    "lw\t$14, 16(%0);" \
    "ctc2\t$12, $2;" \
    "ctc2\t$13, $3;" \
    "ctc2\t$14, $4" \
    : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile( \
    "lw\t$12, 20(%0);" \
    "lw\t$13, 24(%0);" \
    "ctc2\t$12, $5;" \
    "lw\t$14, 28(%0);" \
    "ctc2\t$13, $6;" \
    "ctc2\t$14, $7" \
    : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetGeomScreen(r0) __asm__ volatile( \
    "ctc2\t%0, $26" \
    : : "r"(r0))
#define gte_SetGeomOffset(r0, r1) __asm__ volatile( \
    "sll\t$12, %0, 16;" \
    "sll\t$13, %1, 16;" \
    "ctc2\t$12, $24;" \
    "ctc2\t$13, $25" \
    : : "r"(r0), "r"(r1) : "$12", "$13")
#define gte_ldv0(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$1, 4(%0)" \
    : : "r"(r0))
#define gte_ldv1(r0) __asm__ volatile( \
    "lwc2\t$2, 0(%0);" \
    "lwc2\t$3, 4(%0)" \
    : : "r"(r0))
#define gte_ldv2(r0) __asm__ volatile( \
    "lwc2\t$4, 0(%0);" \
    "lwc2\t$5, 4(%0)" \
    : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$1, 4(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$3, 4(%1);" \
    "lwc2\t$4, 0(%2);" \
    "lwc2\t$5, 4(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldclmv(r0) __asm__ volatile( \
    "lhu\t$12, 0(%0);" \
    "lhu\t$13, 6(%0);" \
    "lhu\t$14, 12(%0);" \
    "mtc2\t$12, $9;" \
    "mtc2\t$13, $10;" \
    "mtc2\t$14, $11" \
    : : "r"(r0) : "$12", "$13", "$14")
#define gte_stclmv(r0) __asm__ volatile( \
    "mfc2\t$12, $9;" \
    "mfc2\t$13, $10;" \
    "mfc2\t$14, $11;" \
    "sh\t$12, 0(%0);" \
    "sh\t$13, 6(%0);" \
    "sh\t$14, 12(%0)" \
    : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_stsxy(r0) __asm__ volatile( \
    "swc2\t$14, 0(%0)" \
    : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile( \
    "swc2\t$12, 0(%0);" \
    "swc2\t$13, 0(%1);" \
    "swc2\t$14, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz(r0) __asm__ volatile( \
    "swc2\t$19, 0(%0)" \
    : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile( \
    "swc2\t$7, 0(%0)" \
    : : "r"(r0) : "memory")
#define gte_stopz(r0) __asm__ volatile( \
    "swc2\t$24, 0(%0)" \
    : : "r"(r0) : "memory")
#define gte_stflg(r0) __asm__ volatile( \
    "cfc2\t$12, $31;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
#define GTE_NOPS ".word\t0x00000000;.word\t0x00000000;"
#define gte_rtps() __asm__ volatile(GTE_NOPS ".word\t0x4A180001")
#define gte_rtpt() __asm__ volatile(GTE_NOPS ".word\t0x4A280030")
#define gte_nclip() __asm__ volatile(GTE_NOPS ".word\t0x4B400006")
#define gte_avsz3() __asm__ volatile(GTE_NOPS ".word\t0x4B58002D")
#define gte_ldopv1(r0) __asm__ volatile( \
    "lw\t$12, 0(%0);" \
    "lw\t$13, 4(%0);" \
    "ctc2\t$12, $0;" \
    "lw\t$14, 8(%0);" \
    "ctc2\t$13, $2;" \
    "ctc2\t$14, $4" \
    : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldopv2(r0) __asm__ volatile( \
    "lwc2\t$11, 8(%0);" \
    "lwc2\t$9, 0(%0);" \
    "lwc2\t$10, 4(%0)" \
    : : "r"(r0))
#define gte_stlvnl(r0) __asm__ volatile( \
    "swc2\t$25, 0(%0);" \
    "swc2\t$26, 4(%0);" \
    "swc2\t$27, 8(%0)" \
    : : "r"(r0) : "memory")
#define gte_rtir() __asm__ volatile(GTE_NOPS ".word\t0x4A49E012")
#define gte_rtir_sf0() __asm__ volatile(GTE_NOPS ".word\t0x4A41E012")
#define gte_rtv0() __asm__ volatile(GTE_NOPS ".word\t0x4A486012")
#define gte_rtv0tr() __asm__ volatile(GTE_NOPS ".word\t0x4A480012")
#define gte_op0() __asm__ volatile(GTE_NOPS ".word\t0x4B70000C")
#define gte_op12() __asm__ volatile(GTE_NOPS ".word\t0x4B78000C")
#define gte_gpf0() __asm__ volatile(GTE_NOPS ".word\t0x4B90003D")
#define gte_gpf12() __asm__ volatile(GTE_NOPS ".word\t0x4B98003D")
#define gte_MulMatrix0(r1, r2, r3) \
    { \
        gte_SetRotMatrix(r1); \
        gte_ldclmv(r2); \
        gte_rtir(); \
        gte_stclmv(r3); \
        gte_ldclmv((s16 *)(r2) + 1); \
        gte_rtir(); \
        gte_stclmv((s16 *)(r3) + 1); \
        gte_ldclmv((s16 *)(r2) + 2); \
        gte_rtir(); \
        gte_stclmv((s16 *)(r3) + 2); \
    }
long RotTransPers4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *flag);
int MargePrim(void *p0, void *p1);
void SetLightMatrix(MATRIX *m);
void SetColorMatrix(MATRIX *m);
MATRIX *CompMatrix(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void SetRotMatrix(MATRIX *m);
VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *v0, VECTOR *v1);
VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v0, VECTOR *v1);
void SetTransMatrix(MATRIX *m);
void SetPolyFT4(POLY_FT4 *p);
void SetPolyF4(POLY_F4 *p);
void SetPolyF3(POLY_F3 *p);
void SetPolyG3(POLY_G3 *p);
void SetPolyG4(POLY_G4 *p);

#endif

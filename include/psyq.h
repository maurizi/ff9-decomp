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

/* libgte inline macros (PsyQ inline_c.h); they expand to the same asm text */
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
/* GTE commands; the two leading nops are emitted as .word 0 so that the
 * assembler (reorder mode) keeps them */
#define GTE_NOPS ".word\t0x00000000;.word\t0x00000000;"
#define gte_rtps() __asm__ volatile(GTE_NOPS ".word\t0x4A180001")
#define gte_rtpt() __asm__ volatile(GTE_NOPS ".word\t0x4A280030")
#define gte_nclip() __asm__ volatile(GTE_NOPS ".word\t0x4B400006")
#define gte_avsz3() __asm__ volatile(GTE_NOPS ".word\t0x4B58002D")
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
u16 GetTPage(int tp, int abr, int x, int y);
u16 GetClut(int x, int y);

void *memmove(void *dst, const void *src, int n);
int sprintf(char *buf, const char *fmt, ...);

int CdControlF(u8 com, u8 *param);
int CD_getsector(void *madr, int size);
int CD_getsector2(void *madr, int size);
void *SpuSetTransferCallback(void (*func)(void));
void SpuSetIRQAddr(u32 addr);
void *SpuSetIRQCallback(void (*func)(void));
int CdControlB(u8 com, u8 *param, u8 *result);
int CdReset(int mode);
int CdDiskReady(int mode);
int CdGetDiskType(void);
CdlLOC *CdIntToPos(int i, CdlLOC *p);

long _card_format(long chan);
long _card_clear(long chan);

#endif

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

long VSync(long mode);
int DrawSync(int mode);
int LoadImage(RECT *rect, u32 *p);
int StoreImage(RECT *rect, u32 *p);

int CdControlF(u8 com, u8 *param);
int CD_getsector(void *madr, int size);
int CD_getsector2(void *madr, int size);
void *SpuSetTransferCallback(void (*func)(void));
void SpuSetIRQAddr(u32 addr);
void *SpuSetIRQCallback(void (*func)(void));

#endif

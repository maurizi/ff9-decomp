#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    s32 unk0;
    void *unk4;
    u8 unk8[4];
    s16 unkC;
    s16 unkE;
    void *unk10; /* file 0x5xxxx */
    void *unk14; /* file 0x6xxxx */
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    void *unk1C;
    s32 unk20;
    s16 unk24; /* map id */
    u16 unk26;
    s32 unk28;
    u8 *unk2C;
    u8 *unk30;
    void *unk34[2]; /* [1] freed by func_800A9DE4 */
    u8 unk3C[16];
} WorldMapObj;

/* world map context, GameData.unk8E4.world */
typedef struct WorldMap {
    u32 unk0; /* flags: 0x100 0x200 0x400 0x800 skip per-frame updates */
    u8 *unk4; /* end of the load area */
    WorldMapObj *unk8; /* = &unk48 */
    void *unkC; /* allocation, freed by func_800A91B4 */
    s32 unk10;
    u8 unk14[0x34];
    WorldMapObj unk48;
} WorldMap; /* 0x94 */

typedef struct {
    u8 pad0[0x11];
    u8 unk11;
} Unk800CE0E8;

/* doubly linked list node; D_800D4848 is the list head */
typedef struct WmNode {
    s32 unk0;
    struct WmNode *unk4;
    struct WmNode *unk8;
} WmNode;

/* frame pack slot list: D_800D3EA8 (head) -> D_800D3EC0[6] -> D_800D3E98 (tail) */
typedef struct Unk800D3EAC {
    struct Unk800D3EAC *unk0; /* prev */
    struct Unk800D3EAC *unk4; /* next */
    s32 *unk8; /* frame pack */
    s16 unkC; /* low 6 bits: id; -1: none */
    u8 unkE;
    u8 padF;
} Unk800D3EAC; /* 0x10 */

typedef struct {
    u8 pad0[2];
    s16 unk2;
    s32 unk4;
    s32 unk8;
} Unk800CE0F0;

typedef struct {
    u8 unk0;
    u8 pad1[7];
    u32 unk8;
    s16 unkC;
    u16 unkE;
} Unk800CCCC0; /* 0x10 */

typedef struct {
    s16 unk0[5];
} Unk800CE1E8; /* 10 bytes */

/* movement mode table entry (D_800CCB70[], current one at D_800CE3C8) */
typedef struct {
    u8 unk0; /* mode */
    u8 pad1[7];
    u8 unk8;
    u8 unk9;
    u8 padA;
    u8 unkB;
    u8 unkC;
    u8 padD[4];
    u8 unk11;
    u8 pad12[0x1C - 0x12];
} Unk800CE3C8; /* 0x1C */

/* doubly linked node (func_800C2134) */
typedef struct WmLNode {
    struct WmLNode *unk0; /* prev */
    struct WmLNode *unk4; /* next */
    s16 unk8;
    s8 unkA;
    u8 unkB;
} WmLNode; /* 0xC */

/* object tested by func_800C95C8 (same code in ovl_0a/ovl_0b) */
typedef struct {
    u16 unk0; /* flags; 0x10: behind the camera */
    u8 pad2[0x1C - 2];
    u16 *unk1C; /* parent flags */
    u8 pad20[0x24 - 0x20];
    MATRIX unk24; /* t: position */
    u8 pad44[0x54 - 0x44];
    u16 unk54; /* scale */
    u16 unk56;
    u16 unk58;
} Unk800C95C8;

/* relocatable block: offsets are turned into pointers by func_800BA528 */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} WmPackEntry;

typedef struct {
    s8 unk0; /* count */
    u8 pad1[3];
    s32 unk4;
    WmPackEntry *unk8[1];
} WmPack;

/* text windows (func_800CC820 .. func_800CC8C8) */
typedef struct {
    u8 unk0;
    u8 unk1; /* index into D_800CDFC0 */
    u8 unk2; /* party slots being swapped */
    u8 unk3;
    u8 unk4;
    u8 pad5[3];
    WinDef unk8[6];
} Unk800D5438;

/* map cell (4 bytes) */
typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
} WmCell;

typedef struct {
    s32 unk0;
    WmCell unk4[1];
} WmCellTable;

/* filled by func_800C3FE4 */
typedef struct {
    s32 unk0;
    s32 unk4; /* count */
    s32 unk8[4];
} Unk800C3FE4;

typedef struct {
    s32 unk0[4];
} Unk800D3AB8; /* 0x10 */

/* list nodes (func_800BE6B4, func_800BD5D0): D_800D3758 (head) ->
 * D_800D3798[40] -> D_800D3BF8 (tail) */
typedef struct Unk800D375C {
    struct Unk800D375C *unk0; /* prev */
    struct Unk800D375C *unk4; /* next */
    s32 unk8;
    Unk800D3AB8 *unkC;
    s16 unk10;
    u8 unk12;
    s8 unk13;
} Unk800D375C; /* 0x14 */

typedef struct Unk800D335C {
    s32 unk0;
    struct Unk800D335C *unk4; /* next */
    s32 unk8;
    s16 unkC;
    u8 unkE;
} Unk800D335C;

/* colour + 2 values (8 bytes) */
typedef struct {
    CVECTOR unk0;
    u16 unk4;
    u16 unk6;
} Unk800D1FD8;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4[3]; /* rgb */
    s16 pad;
} Unk800D3FA0Sub; /* 0xC */

typedef struct {
    u8 pad0[0x14A];
    s16 unk14A;
    s16 unk14C;
    u8 pad14E[0x5E0 - 0x14E];
    SVECTOR unk5E0[3];
    SVECTOR unk5F8;
    u8 pad600[0x610 - 0x600];
    Unk800D3FA0Sub unk610[3];
} Unk800D3FA0;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s16 unk8;
    u8 padA[2];
} Unk800D2FF8; /* 0xC */

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0xC - 4];
} Unk800CD10C; /* 0xC */

/* file 0x12xxxx lookup table (func_800AA39C) */
typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800AA39CEntry;

typedef struct {
    u8 unk0;
    u8 unk1; /* count */
    u8 pad2[2];
    Unk800AA39CEntry unk4[1];
} Unk800AA39C;

/* ---- overlay functions ---- */
void func_800CB9D8(s32, s32);
void func_800B4270(void *);
s32 func_800B4FD4(void *, s32, s32);
void func_800B53FC(void *);
s32 func_800B5880(void *, s32, s32);
void func_800B6338(void *);
s32 func_800B6598(void *, s32, s32);
s32 func_800C291C(void);
s32 func_800C3E88(void);
void func_800C42B0(u32);
void func_800C4850(s32, u16, u8);
void func_800AE784(s16, s16);
void func_800B3638(void);
void func_800CC54C(void);
void func_800C48F4(s32);
void func_800AFCE0(void);
s32 func_800B88C0(s32);
void func_800C2A80(s32, s32, s32);
s16 func_800B36B4(s32, u32, u8 *);
void func_800B01F0(Unk8004AB64 *);
void func_800BE748(VECTOR *, u32 *, s32 *, s32, u16 *);
void func_800B4720(s32);
void func_800B4BDC(s32);
s32 func_800B5CD0(s32, s32, s32);
s32 func_800B5FD0(s32, s32, s32);
void func_800CBB4C(s32);
void func_800CBDF8(RECT *, s32);
void func_800B17B4(void);
void func_800B1CD0(void);
void func_800B20B8(void);
void func_800CB434(void);
void func_800CC324(void);
void func_800B69AC(void);
void func_800B8ABC(void);
void func_800BC0A0(void);
void func_800C4654(void);
void func_800C6EF8(void);
void func_800B153C(void);
void func_800C3EAC(void);
void func_800B8DC0(void *, s32, s32, s32, s32, s32, s32);
void func_800CC3E0(s32);
void func_800CC5D8(u8);
s32 func_800B8498(void);
s32 func_800B84C0(void);
s32 func_800C3FE4(s32, Unk800C3FE4 *);
s32 func_800B828C(s32 *);
void func_800B3A80(void);
s32 func_800B3FEC(s32, s32, s32);
void func_800AD114(s32, s32, s32);
void func_800BC098(void);
void func_800C43F0(void);
void func_800BAD98(void);
void func_800B8724(void);
void func_800B8B10(void);
void func_800AEC74(void);
void func_800AEF10(void);
void func_800C3E28(void);
void func_800C464C(void);
void func_800A88C4(s16);
void func_800A8B14(void);
void func_800A8CA4(void);
void func_800A8CF0(void);
void func_800A8D28(void);
s32 func_800A8D78(void);
s32 func_800A8F18(void);
s32 func_800A8F8C(void);
void func_800A8FF0(void);
s32 func_800A919C(void);
s32 func_800A91A4(void);
s32 func_800A91AC(void);
s32 func_800A91B4(void);
s32 func_800A9220(void);
void func_800A924C(void);
void func_800A95A0(void);
void func_800AA15C(s32, void *);
void func_800AC580(void);
void func_800AC5C8(void);
void func_800AC7D4(void);
void func_800ACAFC(void);
void func_800ACEEC(void);
s32 func_800AD10C(void);
s32 func_800B6BC4(void);
void func_800B69B4(void);
void func_800B8068(void);
s32 func_800C45D8(void);
s32 func_800AB0C0(s32, s32, s32, s32, s32);
void func_800AD738(void);
void func_800AE280(void);
void func_800B30C0(void);
void func_800B905C(void);
void func_800B91A0(void);
void func_800B93B8(void);
void func_800BA1E4(void);
void func_800C1C98(void);
void func_800C2048(void);
void func_800C2134(void);
void func_800C26E4(void);
s32 func_800C497C(u16);
void func_800CBF18(void);
void func_800CC050(void);
void func_800CC13C(void);
void func_800CC82C(void);
void func_800CC890(void);
void func_800CC8C8(void);
void func_800CC940(void);

/* ---- overlay data ---- */
extern RECT *D_800CE04C;
extern u8 D_800CE050[];
extern u8 D_800CCE4B;
extern Unk800D2FF8 *D_800D27F0;
extern u8 *D_800CE058[5];
extern char D_800A79D0[];
extern char D_800A7A0C[];
extern RECT *D_800CE054;
extern s16 D_800CE188;
extern s16 D_800CE1A8;
extern s16 D_800CE1C0;
extern Unk800CE1E8 D_800CE1E8;
extern Unk800CE1E8 D_800CE1F8;
extern u8 D_800CF46D;
extern s32 D_800CF4AC;
extern MATRIX D_800CF4C0;
extern MATRIX D_800CF520;
extern MATRIX D_800CF540;
extern u8 D_800D33F4;
extern u8 D_800A8678[]; /* text */
extern Unk800CCCC0 D_800CCCC0[];
extern Unk800CE0E8 *D_800CE3DC;
extern Unk800CE3C8 *D_800CE3C8;
extern s32 D_800D32D8; /* map width (wraps) */
extern s32 D_800D32DC; /* map height (wraps) */
extern RECT D_800A71E0;
extern char D_800A720C[]; /* "w_frameSystemDestructor:\n" */
extern Unk800CD10C D_800CD10C[][64];
extern s32 *D_800CE3E8[3];
extern s32 D_800CE3F8;
extern u8 D_800CE400[];
extern u8 *D_800CF400;
extern s32 D_800CF420[];
extern MATRIX D_800CF4E0;
extern MATRIX D_800CF500;
extern MATRIX D_800CF560;
extern u8 D_800CFFD0[];
extern Unk800D2FF8 D_800D2FF8[17];
extern s8 D_800D3E74;
extern char D_800A8098[];
extern s32 D_800CF404;
extern WmCellTable *D_800CF40C;
extern WmCellTable *D_800CF410;
extern u8 *D_800CF45C;
extern s32 D_800CF460[];
extern s32 D_800CF49C;
extern s32 D_800CF4B4;
extern Unk800D1FD8 D_800D1FD8;
extern Unk800D1FD8 D_800D1FE0[2];
extern s32 D_800D3308;
extern Unk800D375C D_800D3758; /* list head */
extern s32 D_800D3790;
extern Unk800D375C D_800D3798[40];
extern Unk800D3AB8 D_800D3AB8[20];
extern Unk800D375C D_800D3BF8; /* list tail */
extern Unk800D335C D_800D3340;
extern Unk800D335C *D_800D335C;
extern s32 D_800D33F0;
extern Unk800D3FA0 D_800D3FA0;
extern char D_800A71F4[]; /* "w_frameMapDestructor:\n" */
extern char D_800A7A40[]; /* "w_framePackGetPtr : attempt to get %d element with %d max\n" */
extern void (*D_800CDFC0[])(void);
extern u8 D_800CDF30[];
extern Unk800CE0F0 *D_800CE0F0;
extern s8 D_800CE3C4;
extern s32 D_800CFE10;
extern s32 D_800CFE14;
extern s32 D_800CFE18;
extern s32 D_800CFE1C;
extern u16 *D_800CF430;
extern u8 D_800CF444;
extern s32 D_800D32F0;
extern s32 D_800D3304;
extern MATRIX D_800D3770;
extern s32 D_800CCE50; /* debug flags */
extern Unk800CE0E8 *D_800CE0E8;
extern u8 D_800CE25C;
extern u8 D_800CE26C;
extern s32 D_800CE3E4;
extern s32 D_800CF480;
extern Unk800D3EAC D_800D3E98;
extern Unk800D3EAC D_800D3EA8;
extern Unk800D3EAC D_800D3EC0[6];
extern s32 D_800D3E90;
extern s32 D_800D3EB8;
extern WmNode D_800D4848;
extern u8 D_800CCA1C;
extern RECT D_800CE0B8[];
extern u8 D_800CCFB4[];
extern u8 D_800CE0EC;
extern u8 D_800CE0ED;
extern u8 D_800CE0EE;
extern s32 D_800CE108[];
extern s32 D_800CE198[];
extern s32 D_800CE27C;
extern s32 D_800CF408;
extern u8 D_800D30C4;
extern u8 D_800D30C5;
extern u8 D_800D30C6;
extern u8 D_800D3338;
extern s32 D_800D3F8C;
extern s32 D_800D3F90;
extern s32 D_800D3F94;
extern s32 D_800D3F98;
extern u8 D_800D3F9C;
extern s8 D_800D4818;
extern s32 D_800D481C;
extern s32 D_800D4820;
extern s32 D_800D4824;
extern s32 D_800D4828;
extern Unk800D5438 D_800D5438;
extern u8 D_800CF46C;
extern char D_800A7CCC[];
extern s16 D_800CCB28[][9];
extern Unk800CE3C8 D_800CCB70[];
extern s16 D_800CE276;
extern s16 D_800CE3BC;
extern s32 D_800CFDB0[];
extern s8 D_800CE1F2;
extern s32 D_800D3E60; /* navi mode */
extern s8 D_800D3E64;
extern s32 D_800D3E6C;
extern s8 D_800D3E76;
extern s8 D_800D3E77;
extern s8 D_800D3E78;
extern s8 D_800D3E79;
extern s8 D_800D3E80;
extern s32 D_800D3E84;
extern s8 D_800D3E88;
extern s8 D_800D3E89;
extern s32 D_800D3F20;
extern WmLNode D_800D3F28;
extern WmLNode D_800D3F38;
extern s32 D_800D3F44;
extern WmLNode D_800D3F48[4];

/* scrolling texture strips (func_800B905C) */
typedef struct {
    u16 unk0; /* x */
    u16 unk2; /* y */
    u16 unk4; /* w */
    u16 unk6; /* h */
    u16 unk8; /* speed (8.8) */
} Unk800CFEA0; /* 0xA */

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
    u8 pad8[2];
} Unk800CF450; /* 0xA */

extern u16 D_800CFE88[8];
extern Unk800CFEA0 D_800CFEA0[8];
extern Unk800CF450 *D_800CF450;
extern u16 (*D_800D32E4)[12];
extern s32 D_800D32F8[3];
extern u8 D_800D330C[3];
extern u8 D_800D5410;
extern u8 D_800D1FD0;
extern u8 D_800D30CB;
extern u8 D_800CF454;
void func_800B94F8(s32, s32, s32);
void func_800BD57C(void);
void func_800BD674(void);
void func_800C465C(u16);
void func_800C46D8(u16);

/* main exe: candidates for ff9.h */
Unk800CE0E8 *func_8004AE00(void);
void func_8004B20C(s32);

/* Inlined copy of func_800BC0A8 (wrap map coordinates). */
static inline void func_800BC0A8_inline(s32 *x, s32 *z) {
    if (*x < 0) {
        *x += D_800D32D8;
    }
    if (*x >= D_800D32D8) {
        *x -= D_800D32D8;
    }
    if (*z > 0) {
        *z -= D_800D32DC;
    }
    if (*z <= -D_800D32DC) {
        *z += D_800D32DC;
    }
}

#endif

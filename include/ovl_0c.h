#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    u8 pad0[4];
    void *unk4;
    u8 pad8[0x10 - 8];
    void *unk10; /* file 0x5xxxx */
    void *unk14; /* file 0x6xxxx */
    u8 pad18[0x1B - 0x18];
    u8 unk1B;
    void *unk1C;
    s32 unk20;
    s16 unk24;
    u8 pad26[0x2C - 0x26];
    u8 *unk2C;
    s32 unk30;
    u8 pad34[0x38 - 0x34];
    void *unk38;
    u8 pad3C[0x4C - 0x3C];
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

typedef struct Unk800D3EAC {
    s32 unk0;
    struct Unk800D3EAC *unk4; /* next */
    s32 unk8;
    u16 unkC; /* low 6 bits: id */
    u8 unkE;
} Unk800D3EAC;

typedef struct {
    u8 pad0[2];
    s16 unk2;
} Unk800CE0F0;

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
    u8 pad2[6];
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

/* list nodes (func_800BE6B4, func_800BD5D0) */
typedef struct Unk800D375C {
    s32 unk0;
    struct Unk800D375C *unk4; /* next */
    s32 unk8;
    s32 unkC;
    s16 unk10;
    u8 unk12;
} Unk800D375C;

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
    u8 pad0[0x610];
    Unk800D3FA0Sub unk610[3];
} Unk800D3FA0;

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
s32 func_800C3FE4(s32, Unk800C3FE4 *);
void func_800B828C(s32);
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
void func_800AB0C0(s32, s32, s32, s32, s32);
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
extern s32 D_800D32D8;
extern s32 D_800D32DC;
extern s32 D_800D3308;
extern Unk800D375C *D_800D375C;
extern s32 D_800D3790;
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
extern Unk800D3EAC *D_800D3EAC;
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
extern u8 D_800D4818;
extern s32 D_800D481C;
extern s32 D_800D4820;
extern s32 D_800D4824;
extern s32 D_800D4828;
extern Unk800D5438 D_800D5438;
extern u8 D_800CF46C;

#endif

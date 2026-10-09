#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* palette animation entry (func_800AF414) */
typedef struct WmPalAnim {
    u16 unk0; /* id */
    u8 pad2[2];
    s16 unk4; /* clut x */
    s16 unk6; /* clut y */
    u16 unk8[16]; /* source palette */
    u16 unk28[2][16]; /* per draw buffer */
} WmPalAnim; /* 0x68 */

/* per-object entry in WorldMapObj.unk1C (0x28 bytes) */
typedef struct {
    unsigned unk0 : 8;
    unsigned unk1 : 12; /* scale x */
    unsigned unk2 : 12; /* scale z */
    unsigned unk4 : 12; /* angle */
    signed unk4_12 : 10; /* offset x */
    signed unk4_22 : 10; /* offset z */
    u8 pad8[0x28 - 8];
} WmObjEnt;

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    s32 unk0;
    void *unk4;
    u8 unk8[4];
    s16 unkC;
    s16 unkE;
    void *unk10; /* file 0x5xxxx */
    void *unk14; /* file 0x6xxxx */
    s8 unk18;
    s8 unk19;
    u8 unk1A;
    u8 unk1B;
    WmObjEnt *unk1C;
    s32 unk20;
    s16 unk24; /* map id */
    u16 unk26;
    struct WmPalAnim *unk28; /* count: unk1A */
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
    s8 unkC;
    s8 unkD;
    s16 unkE;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s16 unk1C;
    s16 pad1E;
    s32 unk20;
    s32 unk24;
} WmNode; /* 0x28 */

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
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    u16 unkC; /* camera angle */
    u16 unkE;
} Unk800CE0F0;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u32 unk8;
    s16 unkC;
    s16 unkE;
} Unk800CCCC0; /* 0x10 */

typedef struct {
    s16 unk0[5];
} Unk800CE1E8; /* 10 bytes */

/* movement mode table entry (D_800CCB70[], current one at D_800CE3C8) */
typedef struct {
    u8 unk0; /* mode */
    u8 unk1;
    s16 unk2; /* speed */
    u8 pad4[4];
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[3];
    u8 unk10;
    u8 unk11;
    u8 pad12[2];
    u8 unk14[8]; /* allowed cell types */
} Unk800CE3C8; /* 0x1C */

/* doubly linked node (func_800C2134) */
typedef struct WmLNode {
    struct WmLNode *unk0; /* prev */
    struct WmLNode *unk4; /* next */
    s16 unk8;
    s8 unkA;
    u8 unkB;
} WmLNode; /* 0xC */

/* model object (same layout in ovl_0a, ovl_0b and ovl_0c) */

/* primitive sources of a model group; index fields select vertices (unk0..),
 * uv pairs (WmModelGroup.unk20) and colours (WmModelGroup.unk24) */
typedef struct {
    u16 unk0[4];
    u16 unk8[4]; /* uv */
    u8 unk10[3]; /* rgb */
    u8 unk13;    /* texture */
    u8 unk14;
    u8 unk15; /* flags: 1 = semi transparent, 0xE0 = mode */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

typedef struct {
    u16 unk0[3];
    u8 unk6; /* texture */
    u8 pad7;
    u8 unk8[3]; /* rgb */
    u8 padB;
    u16 unkC[3]; /* uv */
    u8 unk12;    /* flags */
    u8 pad13;
} WmSrcFT3; /* 0x14 */

typedef struct {
    u16 unk0[4];
    u16 unk8[4];  /* uv */
    u16 unk10[4]; /* colours */
    u8 pad18[4];
    u8 unk1C; /* flags */
    u8 unk1D; /* texture */
    u8 pad1E[2];
} WmSrcGT4; /* 0x20 */

typedef struct {
    u16 unk0[3];
    u16 unk6[3]; /* uv */
    u8 padC[3];
    u8 unkF;      /* flags */
    u16 unk10[3]; /* colours */
    u8 unk16;     /* texture */
    u8 pad17;
} WmSrcGT3; /* 0x18 */

typedef struct {
    u16 unk0[4];
    u16 unk8[4]; /* colours */
    u8 pad10[8];
} WmSrcG4; /* 0x18 */

typedef struct {
    u16 unk0[3];
    u8 pad6[2];
    u16 unk8[3]; /* colours */
    u8 padE[6];
} WmSrcG3; /* 0x14 */

typedef struct {
    u16 unk0[4];
    u8 unk8[3]; /* rgb */
    u8 padB[5];
} WmSrcF4; /* 0x10 */

typedef struct {
    u16 unk0[3];
    u8 pad6[2];
    u8 unk8[3]; /* rgb */
    u8 padB;
} WmSrcF3; /* 0xC */

typedef struct {
    u16 unk0;
    u16 unk2;  /* FT4 count */
    u16 unk4;  /* FT3 */
    u16 unk6;  /* GT4 */
    u16 unk8;  /* GT3 */
    u16 unkA;  /* G4 */
    u16 unkC;  /* G3 */
    u16 unkE;  /* F4 */
    u16 unk10; /* F3 */
    u8 pad12[2];
    void *unk14;
    void *unk18;
    u8 *unk1C;      /* primitive sources, in the order above */
    u8 (*unk20)[2]; /* uv */
    u8 (*unk24)[4]; /* colours */
} WmModelGroup; /* 0x28 */

typedef struct {
    u16 unk0; /* flags: 1 = relocated */
    u8 unk2;
    u8 unk3; /* group count */
    u8 pad4[0xC - 4];
    s32 unkC;
    WmModelGroup *unk10;
} WmModel;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    void *unk4; /* first primitive */
    s32 unk8;
} WmModelOut; /* 0xC */

typedef struct {
    u16 unk0; /* tpage */
    u16 unk2; /* clut */
} WmTexInfo;

/* animation file header: offsets relocated by func_800AA15C */
typedef struct WmAnimHdr {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
} WmAnimHdr;

typedef struct Unk800C95C8 {
    u16 unk0; /* flags; 0x10: behind the camera */
    u8 pad2[6];
    WmModel *unk8;
    struct WmAnimHdr *unkC;
    u8 *unk10; /* primitive buffer */
    s32 unk14;
    WmTexInfo *unk18;
    struct Unk800C95C8 *unk1C; /* parent */
    u8 *unk20;
    MATRIX unk24; /* t: position */
    WmModelOut *unk44;
    u8 pad48[0x50 - 0x48];
    s32 unk50;
    u16 unk54; /* scale */
    u16 unk56;
    u16 unk58;
    u16 unk5A;
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x74 - 0x64];
    u16 unk74;
    u16 unk76;
    u16 unk78;
} Unk800C95C8;

/* model slot (func_8001E350) */
typedef struct {
    s32 unk0; /* flags: 1 = loaded */
    s32 unk4; /* file id */
    Unk800C95C8 *unk8;
    Unk8004AB64 *unkC;
} WmObjSlot;
s32 func_8004AB64(Unk8004AB64 *);
void func_800C709C(Unk800C95C8 *, u8);
void func_800C7914(Unk800C95C8 *, u8);
void func_800C9484(Unk800C95C8 *);
void func_800C96AC(Unk800C95C8 *, u8);
void func_800C9FCC(Unk800C95C8 *, u8);
void func_800CA1D8(Unk800C95C8 *, u8);
u8 *func_800634A8(s32);
u8 *func_800634D8(s32);

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
    struct WmTexEnt *unk8;
    Unk800D3AB8 *unkC;
    s16 unk10;
    u8 unk12;
    s8 unk13;
} Unk800D375C; /* 0x14 */

/* list: D_800D3358 (head) -> D_800D34A8[32] -> D_800D3340 (tail) */
typedef struct WmTexEnt {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x21 - 2];
    u8 unk21; /* sound id */
} WmTexEnt;

typedef struct WmTexPack {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    WmTexEnt *unk8[4];
} WmTexPack;

typedef struct Unk800D335C {
    struct Unk800D335C *unk0; /* prev */
    struct Unk800D335C *unk4; /* next */
    struct WmTexPack *unk8;
    s16 unkC;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 pad11[3];
} Unk800D335C; /* 0x14 */

/* colour + 2 values (8 bytes) */
typedef struct {
    CVECTOR unk0;
    u16 unk4;
    u16 unk6;
} Unk800D1FD8;

typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4[3]; /* rgb */
    s16 pad;
} Unk800D3FA0Sub; /* 0xC */

/* sky / lighting parameter block (0x5E bytes); func_800C3954 blends them */
typedef struct {
    SVECTOR unk0[3]; /* light directions */
    SVECTOR unk18;
    u8 unk20; /* back colour 1 */
    u8 pad21;
    u8 unk22;
    u8 pad23;
    u8 unk24;
    u8 pad25[3];
    u8 unk28; /* back colour 2 */
    u8 pad29;
    u8 unk2A;
    u8 pad2B;
    u8 unk2C;
    u8 pad2D[3];
    Unk800D3FA0Sub unk30[3];
    s16 unk54[3];
    u8 pad5A[2];
    u16 unk5C;
} WmSky; /* 0x5E */

typedef struct {
    u8 pad0[0x14A];
    s16 unk14A;
    s16 unk14C;
    u8 pad14E[0x5E0 - 0x14E];
    WmSky unk5E0[7]; /* [0] current, [1]/[2] blended from [3]..[6] */
} Unk800D3FA0;
void func_800C3954(WmSky *, WmSky *, WmSky *, s32, s32);
void func_800C2830(void);
void func_800B9774(void);
void func_800B8664(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s16 unk8; /* tpage */
    s16 unkA; /* clut */
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

/* streamed load request (func_800AC424) */
typedef struct {
    u8 pad0[8];
    s32 unk8; /* address */
    s32 unkC; /* offset */
    s32 unk10; /* size */
} Unk800AC424;

/* ---- overlay functions ---- */
s32 func_800A9DE4(WorldMapObj *, s16);
s32 func_800A9E24(s16, s32 *, s32 *);
s32 func_800A9EC4(s32, s32 *, s32 *);
s32 func_800A9FC0(s16, s32 *, s32 *);
s32 func_800AA060(s32, s32 *, s32 *);
void func_800B8024(void);
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
void func_800C4718(s32, u8);
void func_800C47B0(s32, u8);
void func_800AFCE0(void);
s32 func_800B88C0(s32);
void func_800C2A80(s32, s32, s32);
s32 func_800B36B4(s32, u32, u8 *);
void func_800B01F0(Unk8004AB64 *);
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
/* TIM image blocks */
typedef struct {
    u32 unk0; /* length */
    RECT unk4;
    u16 unk8[1];
} WmTimBlk;

typedef struct {
    u32 unk0; /* id */
    u32 unk4; /* flags: 8 = has CLUT */
    WmTimBlk unk8;
} WmTim;

void func_800B8DC0(u32 *, s32, s32, s32, s32, s32, s8);
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
s32 func_800A8FF0(void);
s32 func_800A919C(void);
s32 func_800A91A4(void);
s32 func_800A91AC(void);
s32 func_800A91B4(void);
s32 func_800A9220(void);
void func_800A924C();
void func_800A95A0(void);
u32 func_800AA15C(s32, Unk8004AB64 *);
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
extern u8 D_800CCE4D;
extern Unk800D2FF8 *D_800D27F0;
extern u8 *D_800CE058[5];
extern char D_800A79D0[];
extern char D_800A7A0C[];
extern RECT *D_800CE054;
extern s16 D_800CE188;
extern s16 D_800CE1A8;
extern s16 D_800CE1C0;
extern s8 D_800CCA20;
extern s32 D_800CCE2C;
extern s32 D_800CCE30;
extern s32 D_800CCE34;
extern s32 D_800CCE38;
extern s32 D_800CCE3C;
extern u8 D_800CCA28[];
extern u8 D_800CCFB4[];
extern u8 D_800D30D8[];
extern char D_800A7F48[];
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
extern Unk8004AB64 *D_800CE3DC; /* player actor */
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
extern u16 D_800CFFD0[2][0x800]; /* palettes, [1] = shifted copy */
/* texture animation entries (func_800B94F8) */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6; /* frame count */
    s16 unk8; /* speed */
} Unk800CFF18; /* 0xA */
extern Unk800CFF18 D_800CFF18[];
extern s16 D_800CFE48[];
extern u8 D_800CFE70[];
extern u16 D_800CFE20[];
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
extern s32 D_800CF43C; /* frame timing */
extern s32 D_8007BD28;
extern s32 D_8007BD30;
extern s32 D_800CF458;
extern s32 D_800CF470;
extern s32 D_800CCE5C;
extern s32 D_800CF4B8;
extern Unk800D1FD8 D_800D1FD8;
extern Unk800D1FD8 D_800D1FE0[2];
extern s32 D_800D3308;
extern Unk800D375C D_800D3758; /* list head */
extern s32 D_800D3790;
extern Unk800D375C D_800D3798[40];
extern Unk800D3AB8 D_800D3AB8[20];
extern Unk800D375C D_800D3BF8; /* list tail */
extern Unk800D335C D_800D3340;
extern Unk800D335C D_800D3358;
extern Unk800D335C D_800D34A8[32];
extern s32 D_800D3408;
extern s32 D_800D3750;
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
extern WmTexPack *D_800D32EC;
extern s32 D_800D3304;
extern MATRIX D_800D3770;
extern s32 D_800CCE50; /* debug flags */
extern s16 D_800CCD7C;
extern u8 D_800CCE54;
extern u16 D_800CF474;
extern s32 D_800CF488[4]; /* script request parameters */
extern s32 D_800CF48C;
extern s32 D_800CF490;
/* texture transfer requests (func_800C24D8) */
typedef struct {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
} Unk800D3F78;
extern Unk800D3F78 D_800D3F78[];
extern s16 D_800CDF14[2];
extern s16 D_800CDF18[2];
extern s16 D_800CDF1C[2];
extern s16 D_800CDF20[2];
extern char D_800A8118[];
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
extern s8 D_800CCE69;
extern char D_800CCE6C[];
extern WmNode D_800D4848;
extern WmNode D_800D53A0;
extern WmNode D_800D4A40[60]; /* free list nodes */
extern WmNode D_800D53E8; /* list tail */
extern WmNode D_800D48C0; /* list tail */
extern s32 D_800D4A38;
extern s32 D_800D53CC;
extern s32 D_800D53C8;
extern VECTOR D_800CDF50;
extern SVECTOR D_800D4870[10];
extern s16 D_800D4830[10];
extern s16 D_800D48F8[10];
extern u8 D_800D48E8[10];
extern SVECTOR D_800D4910[10];
extern VECTOR D_800D4960[9];
extern SVECTOR D_800D49F0[9];
extern SVECTOR D_800CDF48;
extern SVECTOR D_800CDF80;
extern SVECTOR D_800CDF88;
extern SVECTOR D_800CDF90;
extern SVECTOR D_800CDFA0[3];
extern void **D_800CF428;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk800B8388;
extern MATRIX *D_800CF448;
extern s32 D_800CE19C;
void func_800BA3F4(s32, s32);
extern u8 D_800CCA1C;
extern RECT D_800CE0B8[];
extern u8 D_800CCFB4[];
extern u8 D_800CE0EC;
extern u8 D_800CE0ED;
extern u8 D_800CE0EE;
extern s32 D_800CE108[];
extern s32 D_800CE198[];
extern s32 D_800CE1D0[];
extern s32 D_800D3730[];
extern s32 D_800CE27C;
extern s32 D_800CF408;
extern u8 D_800D30C4;
extern u8 D_800D30C5;
extern u8 D_800D30C6;
extern u8 D_800D3338;
extern u8 D_800D30F8[];
extern u8 D_800CCB18[];
extern u8 D_800CCB20[];
u8 func_800B3340(u32 *, s32);
extern s32 D_800CD098;
s32 func_800BCA6C(s16);
extern s16 D_800D45D4[3];
void func_800B3998(u16 *, u16 *, s32, s16 *);
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
extern u8 D_800D3E64;
extern s32 D_800D3E6C;
extern POLY_G3 *D_800D3E7C;
extern POLY_FT4 *D_800D3E68;
extern s8 D_800D3E75;
extern s32 D_800CCE40;
extern u8 D_800D3E76;
extern u8 D_800D3E77;
extern s8 D_800D3E78;
extern s8 D_800D3E79;
extern u8 D_800D3E80;
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
    u8 unk8; /* topograph << 2 */
    u8 unk9; /* bit 0: fog */
} Unk800CF450; /* 0xA */

extern u16 D_800CFE88[8];
extern Unk800CFEA0 D_800CFEA0[8];
extern u16 D_800CFEF8;
extern u16 D_800CFF02;
extern void *D_800CE3F4;
extern s8 D_800CFE98[2];
extern s8 D_800CCFAC[];
extern Unk800CF450 *D_800CF450;
extern u16 (*D_800D32E4)[12];
extern s32 D_800D32F8[3];
extern u8 D_800D330C[3];
extern s32 D_800CD078[3]; /* sound source x */
extern s32 D_800CD084[3]; /* sound source z */
extern u8 D_800CD090[3]; /* sound ids */
extern u8 D_800D30CD;
extern s32 D_800D3300;
extern u8 D_800D5410;
extern u8 D_800D1FD0;
extern u8 D_800D30D2;
extern u8 D_800D30CB;
extern u8 D_800CF454;
void func_800B94F8(s32, s32, s32);
void func_800BD57C(void);
void func_800BD674(void);
void func_800CB8C8(s32, s32);
void func_800C465C(u16);
void func_800C46D8(u16);

/* image pack loaded by func_800CB09C; offsets are from the header start */
typedef struct {
    s32 unk0; /* offset: Unk800CB09CClut */
    s32 unk4; /* offset: images (RECT + pixels) */
    s32 unk8; /* image count */
} Unk800CB09C;

typedef struct {
    s32 unk0; /* offset: pixel data */
    s32 unk4; /* count */
    RECT unk8[1];
} Unk800CB09CClut;

/* text window contents passed to func_80063118 (as in ovl_38) */
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 *unk4;
    s32 unk8;
    void (*unkC)(void);
} TextWin;

extern RECT D_800CDFCC[6];
extern TextWin D_800CE00C[4];
extern u8 D_800D5418[16];
extern u8 D_800D5428[16];
extern u8 D_800CE06C[];
extern u8 D_800CE070[];
extern u8 D_800CE078[];
extern u8 D_800CE080[];
extern u8 D_800CE088[];
extern u8 D_800CE090[];

/* func_800AEADC init state */
typedef struct {
    s8 unk0;
    u8 pad1[3];
    s16 unk4[6];
    u16 unk10[6];
} Unk800CE288; /* 0x1C */
s32 func_800BE748(VECTOR *, u32 *, s32 *, Unk800CE288 *, u16 *);
extern Unk800CE288 D_800CE288[11];
extern Unk8004AB64 D_800CE210;
extern u8 D_800CE209;
extern s8 D_800CE25D;
extern u8 D_800CE25E;
extern s32 D_800CE260;
extern Unk8004AB64 *D_800CE268;
extern s8 D_800CE26D;
extern Unk8004AB64 *D_800CE270;
extern s8 D_800CE274;
extern s32 D_800CE278;
extern s32 D_800CE280;
extern s32 D_800CE284;
extern u8 D_800CE3BE;
extern s32 D_800CE3C0;
extern u16 D_800CE3CC;
extern Unk8004AB64 *D_800CE3D0;
extern s32 D_800CE3D4;
extern Unk8004AB64 *D_800CE3D8;
extern s32 D_800CE3E0;

extern u8 D_800CCE49;
extern s32 D_800CCF70; /* debug: last update step */
extern s32 D_800CE264;
extern u8 D_800CF44C;
extern u8 D_800CF47D;
extern TILE D_800CE0C8;
extern DR_TPAGE D_800CE0D8;
extern u32 D_800CE0E0;
extern u8 D_800CE098[];
void func_80023F60(s32);
void func_8005562C(s32);
void func_80055698(s32);
extern s32 D_800CF4B0;
extern u8 D_800CF440;
extern s16 D_800CF442;
extern u16 D_800D45DC;
extern u16 D_800D4754;
extern u8 D_800CF47E;
extern u8 D_800CF434;
extern u8 D_800CF4A8;
extern u8 D_800CF47C;
void func_80050E34(u8 *);
void func_80050EFC(s32, s32);
extern s32 D_800CF468;
extern s32 D_800CF4A0;
void func_800AF08C(void);
void func_800BAEC0(void);
void func_800BE984(void);
void func_800C4DD8(void);
s32 func_800BE4A8(s16, u8 *);
s32 func_800B9808(s32, s32, VECTOR *, u32 *, s32 *, s32);
void func_800BC0A8(s32 *, s32 *);
void func_800BC130(s32, s32, s32 *);
void func_800BC2C8(s32, s32 *, s32 *);
void func_800C2040(void);
void func_800C20B0(s32 *);
void func_800BA528(WmPack *);
extern u8 D_800CE202;
extern u8 D_800CE0EF;
extern s32 D_800CD094;
extern s32 D_800D3320[];
extern Unk800D335C *D_800D33F8[];
extern WmPack *D_800D3740[];
extern s32 *D_800D3310[];
extern s32 D_800CF580[5];
extern s32 D_800CF598[5]; /* CD sectors */
extern s32 D_800CCF84[5];
extern s32 D_800CCF98[5];
extern char D_800A7BB8[];
/* sprite table (func_800C1824) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* y */
    u8 unk4; /* u */
    u8 pad5;
    u8 unk6; /* v */
    u8 pad7;
    s16 unk8; /* w */
    s16 unkA; /* h */
} Unk800CD744; /* 0xC */
extern Unk800CD744 D_800CD744[4];
void func_800C1824();
extern s32 D_800D3F88;
void func_800C26DC(void);

extern Unk800CE1E8 D_800CCA68[];
extern u8 D_800CCACC[][3][3];
extern VECTOR D_800CE1B0;
extern SVECTOR D_800CE0F8; /* camera up vector */
extern SVECTOR D_800CE190;
extern SVECTOR D_800CE1C8;
extern s32 D_800CE1C4;
extern s32 D_800CCA24;
extern s16 D_800CE100;
extern u8 D_800CE1E1;
extern u8 D_800CE1F3;

/* main exe: candidates for ff9.h */
void func_800316D8(SVECTOR *, MATRIX *); /* RotMatrix */
void func_80030AF4(SVECTOR *, MATRIX *);
s32 func_8004BB7C(s32); /* cos */
s32 func_8004BB80(s32); /* sin */
void func_800639D4(u16);
s32 func_80050D38(s32);
extern u32 D_80076F80[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *D_80077DF0;
s32 func_8002B980(u8 *, s32);
extern s32 D_80077E54; /* overlaps Unk80077E50.unk4 in ff9.h */
Unk8004AB64 *func_8004AE00(void);
void func_8004B20C(s32);

/* run on the scratchpad stack (top of the 1 KB scratchpad) */
#define SetSpadStack(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -4\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack() __asm__ volatile("addiu $29, $29, 4\n\tlw $29, 0($29)" : : : "memory")

/* world effect emitter table entry (func_800C69F4) */
typedef struct {
    s32 unk0; /* position */
    s32 unk4;
    s32 unk8;
    s32 unkC; /* random range */
    s32 unk10;
    s32 unk14;
    u8 pad18[0x24 - 0x18];
    s16 unk24; /* effect, -1 = end */
    s16 unk26;
    s16 unk28; /* chance (of 4096) */
    u8 pad2A[0x40 - 0x2A];
} WmEmitter; /* 0x40 */

/* map-marker colour (func_800C1428) */
extern s32 D_800CD104; /* navi cursor */
extern s32 D_800CD70C; /* marker snap distance */
extern s32 D_800CD108;
typedef struct { SVECTOR v[4]; } Unk800A7FB8; /* shadow quad template */
extern Unk800A7FB8 D_800A7FB8;
extern Unk800A7FB8 D_800A8020; /* radar arrow */
extern RECT D_800A71D8;
extern char D_800A7F48[]; /* encounter fallback message */ /* palette area (func_800B69B4) */
extern CVECTOR D_800A7FF0; /* marker colours */
extern CVECTOR D_800A7FF4;
extern CVECTOR D_800A7FF8;
extern CVECTOR D_800A7FFC;
extern u8 D_800CD710;
extern u8 D_800CD714;
extern u8 D_800CD718;
void func_800BF544(s32, s32, s32, s32, CVECTOR, CVECTOR, s32, s32, s32, s32);
void func_800C1B00(s32, s32, s32 *, s32 *, u8);

static inline CVECTOR func_800C1428_color(void) {
    CVECTOR c;

    func_8001D898(&c, 0, 4);
    c.r = D_800CD710;
    c.g = D_800CD714;
    c.b = D_800CD718;
    return c;
}

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

/* pad state, only read in mode 2 (func_800B7DAC) */
static inline u32 func_800B7DAC_pad(void) {
    return D_800CF46C == 2 ? D_800739A0[0] : 0;
}

static inline u32 func_800B7DAC_trg(void) {
    if (D_800CF46C != 2) {
        return 0;
    }
    return D_80073990;
}

#endif

#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* field context, GameData.unk8E4.field */
typedef struct {
    u8 unk0; /* flags */
    u8 pad1[0x14 - 1];
} Unk800AB0B0Entry; /* 0x14 */

typedef struct Unk800AB0B0 {
    u16 unk0; /* count */
    u8 pad2[2];
    Unk800AB0B0Entry unk4[1];
} Unk800AB0B0;

/* 0x330-byte block, FieldMapObj.unk14 (allocated by func_800A9350) */
typedef struct {
    u32 unk0; /* flags */
    u8 pad4[0xA - 4];
    s16 unkA;
    void *unkC;  /* file 0x5xxxx */
    u8 pad10[0x14 - 0x10];
    struct Unk800C9DB4 *unk14;
    void *unk18;
    void *unk1C; /* file 0x1Fxxxx */
    s32 unk20;
    void *unk24;
    u8 *unk28; /* = unk6C */
    u8 pad2C[0x41 - 0x2C];
    u8 unk41;
    s8 unk42;
    u8 pad43[0x4A - 0x43];
    u8 unk4A;
    u8 pad4B;
    void *unk4C; /* file 0x1Exxxx */
    struct Unk800AB0B0 *unk50;
    u8 *unk54; /* = unk90 */
    u8 *unk58; /* = unk270 */
    u8 pad5C[0x6C - 0x5C];
    u8 unk6C[0x90 - 0x6C];
    u8 unk90[0x270 - 0x90];
    u8 unk270[0x330 - 0x270];
} FieldScene;

/* 0x38-byte object hanging off FieldMap.unkC */
typedef struct {
    s32 unk0;
    s32 unk4;    /* end of the load area */
    void *unk8;  /* file 0x6xxxx */
    void *unkC;
    u16 unk10;
    u8 unk12[2];
    FieldScene *unk14;
    u8 unk18[0x20];
} FieldMapObj;

/* field context, GameData.unk8E4.field (0x50 bytes) */
typedef struct FieldMap {
    u32 unk0; /* flags */
    s32 unk4;
    s8 unk8;
    s8 unk9;
    s16 unkA;
    FieldMapObj *unkC;
    void *unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x20 - 0x1C];
    s32 unk20;
    u8 unk24[0x2C];
} FieldMap;

/* 0x28-byte entries at D_800C9DF4 */
typedef struct Unk800C9DF4 {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    s16 unk6;
    u16 unk8;
    u16 unkA;
    u8 padC[0x1C - 0xC];
    s32 unk1C;
    s32 unk20;
    Unk8004AB64 *unk24;
} Unk800C9DF4;

/* 0xC-byte entries at D_800C9DCC */
typedef struct {
    u8 unk0;
    u8 pad1[0xB];
} Unk800C9DCC;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
} Unk800CA058;

typedef struct {
    void (*unk0)(void); /* enter */
    void (*unk4)(u8);   /* update */
    void (*unk8)(void); /* leave */
} Unk800C9D5C; /* 0xC */

typedef struct {
    u16 unk0;
    u16 unk2;
} __attribute__((aligned(4))) Unk800CA068Pair;

/* field state (D_800CA068) */
typedef struct {
    s32 unk0; /* flags */
    Unk800CA068Pair unk4;
    u8 pad8[0xC - 8];
    s16 unkC; /* geometry offset adjust */
    s16 unkE;
    Unk800CA068Pair unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 pad1C[0x24 - 0x1C];
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u8 pad2C[0x75 - 0x2C];
    u8 unk75;
    s16 unk76;
    u8 pad78[0xA8 - 0x78];
    s16 unkA8[4][2];
    s16 unkB8[4][2];
    s16 unkC8[4];
    s16 unkD0[4];
} Unk800CA068;

/* walkmesh-like data block at D_800C9DB4: the header holds offsets (from the
 * block start) of its tables */
typedef struct Unk800C9DB4 {
    u8 pad0[4];
    u16 unk4; /* entries in the unkC table */
    u8 pad6[0xC - 6];
    s32 unkC;  /* offset: Unk800C9DB4Path[] */
    s32 unk10; /* offset: Unk800C9DB4Obj[] */
    u8 pad14[0x18 - 0x14];
    s32 unk18; /* offset: Unk800C9DB4Cam[] */
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
} Unk800C9DB4;

typedef struct {
    u8 unk0;
    u8 unk1;
} Unk800C9DB4Step;

typedef struct {
    unsigned unk0 : 8; /* flags */
    unsigned unk1 : 24; /* entries at unkC */
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC; /* offset: Unk800C9DB4Step[] */
} Unk800C9DB4Path; /* 0x10 */

typedef struct {
    u8 unk0; /* flags */
    u8 pad1[7];
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u8 pad10[0x1C - 0x10];
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    u32 unk24;
    u8 pad28[0x38 - 0x28];
} Unk800C9DB4Obj; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
    u16 unk22;
    u8 pad24[0x28 - 0x24];
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u8 pad30[0x34 - 0x30];
} Unk800C9DB4Cam; /* 0x34 */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 *unk18;
    u8 *unk1C; /* 3 bytes per entry */
    u8 *unk20;
    u8 unk24;
    u8 pad25[3];
    void *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} Unk800C9DA4;

typedef struct {
    u8 pad0[6];
    u8 unk6; /* count */
    u8 pad7[2];
    u8 unk9;
    u8 padA[2];
    u8 unkC[1][12];
} Unk800B3254;

/* header of the block D_800C9DF0.unk8 points to */
typedef struct {
    u8 pad0[0x26];
    u16 unk26; /* offset: Unk800C9DF8Node[] */
    u8 pad28[2];
    u16 unk2A; /* offset: Unk800C9DF8Link[] */
    u16 unk2C; /* entry count for D_800C9DF0.unk20 */
    u8 pad2E[0x32 - 0x2E];
    u16 unk32; /* offset: Unk800C9DF8Tri[] */
} Unk800C9DF8;

typedef struct {
    u16 unk0; /* low byte: flags, high byte: mask */
    u8 pad2[2];
    u16 unk4;
    u8 pad6[0x28 - 6];
} Unk800C9DF8Node; /* 0x28 */

typedef struct {
    u16 unk0;
    u8 pad2[2];
} Unk800C9DF8Link; /* 4 bytes */

typedef struct {
    u8 pad0[0xA];
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u8 pad10[0x20 - 0x10];
} Unk800C9DF8Tri; /* 0x20 */

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[4];
} Unk800C9DF8Pt; /* 8 bytes */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk800C9E04; /* 0x10 */

typedef struct {
    u16 unk0; /* flags */
    u16 unk2; /* count */
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC; /* offset into D_800C9DF0.unk8 */
} Unk800C9E10; /* 0x10 */

typedef struct {
    u8 pad0[6];
    s16 unk6;
    u16 unk8;
    s16 unkA;
    u8 padC[0x28 - 0xC];
} Unk800C9E00; /* 0x28 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2; /* count of unk4 entries */
    struct Unk800C9DF4 *unk4;
    Unk800C9DF8 *unk8;
    u8 padC[0x10 - 0xC];
    Unk800C9E00 *unk10;
    Unk800C9E04 *unk14;
    u8 pad18[0x20 - 0x18];
    Unk800C9E10 *unk20;
} Unk800C9DF0;

typedef struct {
    u16 unk0; /* flags */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
} Unk800CAE98; /* 0x18 */

/* file 0x1C0000 header */
typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u16 unk32;
} Unk800AA01C;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    s16 unk4;
    u8 pad6[0x14 - 6];
    s16 unk14;
    u8 pad16[0x1E - 0x16];
    s16 unk1E;
} Unk800C9EF0;

/* text windows (same layout as ovl_0c's D_800D5438) */
typedef struct {
    u8 unk0;
    u8 unk1; /* index into D_800C9E14 */
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5[3];
    WinDef unk8[6];
} Unk800CB188;

/* file 0x12xxxx lookup table (func_800AAF5C) */
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

/* object tested by func_800B7E30 (same code in ovl_0b/ovl_0c) */
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

/* ---- overlay functions ---- */
void func_800AD944(s32, s32, s32);
s32 func_800A8588(void);
void func_800C6EEC();
void func_80054A28(void *);
void func_800B0124(s32, s32, s32, s32, s32);
void func_800B36E0(void);
void func_800B4894(void);
void func_800B3ACC(void);
s32 func_8001E350(s32);
void func_800B31B8(s32, s32, s32, s32);
void func_800B5938(void);
void func_800AC67C(s32);
void func_800C0038(s32, s32);
void func_800A8198(void);
void func_800A82F8(void);
void func_800A7AAC(s32);
void func_800A7B10(s16);
void func_800A85F4(s32);
void func_800A8698(void);
void func_800B5114(void);
void func_800B51D0(void);
void func_800B5290(void);
void func_800B52DC(void);
void func_800B532C(void);
s32 func_800C44F4(s16);
void func_800B57C8(void);
void func_800B1CE4(void);
s32 func_800AA0C8(FieldScene *);
s32 func_800A8990(FieldMapObj *, s16);
void func_800B5430(s32);
s32 *func_800B3BD8(s32);
s32 *func_800B3D10(s32);
void func_800C951C(void);
void func_800C95B8(void);
void func_800C9580(void);
void func_800B559C(void);
void func_800B5660(void);
void func_800B58B8(void);
void func_800C8C08(void);
void func_800C8D40(void);
void func_800C8E2C(void);
void func_800C9630(void);
/* party/menu windows shared with ovl_0c (func_800CB434 .. func_800CC940) */
void func_800C8124(void);
void func_800C86C8(s32, s32);
void func_800C9014(void);
void func_800C90D0(s32);
void func_800C923C(void);
void func_800C92C8(u8);

/* ---- overlay data ---- */
extern u8 D_800A717C[]; /* text */
extern s32 D_800C9D58;
extern Unk800C9D5C D_800C9D5C[];
extern Unk800C9DA4 D_800C9DA4;
extern Unk800C9DB4 *D_800C9DB4;
extern s32 D_800C9DD0;
extern u16 D_800C9DDE;
extern u16 D_800C9DE0;
extern Unk800C9DF0 D_800C9DF0;
extern Unk800C9DCC *D_800C9DCC;
extern s32 D_800C9F20[16];
extern s8 D_800C9FF0[5];
extern s32 D_800C9FF8[5];
extern s32 D_800CA0E0;
extern s32 D_800CA010[8];
extern s32 D_800CA030[8];
extern u8 D_800CA050; /* D_800CA010 count */
extern u8 D_800CA051; /* D_800CA030 count */
extern Unk800CA058 D_800CA058;
extern Unk800CA068 D_800CA068;
extern u8 D_800CAA98[];
extern u8 D_800CAAB8[];
extern Unk800CAE98 D_800CAE98[];
extern Unk800CB188 D_800CB188;
extern void (*D_800C9E14[])(void);
extern Unk800C9EF0 D_800C9EF0;
extern u16 D_800C9D44;
extern RECT *D_800C9EA0;
extern u8 D_800C9EA4[];
extern RECT *D_800C9EA8;
extern u8 *D_800C9EAC[5];
extern u8 D_800A7860[]; /* text */

/* text window contents passed to func_80063118 (as in ovl_38) */
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 *unk4;
    s32 unk8;
    void (*unkC)(void);
} TextWin;

extern RECT D_800C9E20[6];
extern TextWin D_800C9E60[4];
extern u8 D_800CB168[16];
extern u8 D_800CB178[16];
extern u8 D_800C9EC0[];
extern u8 D_800C9EC4[];
extern u8 D_800C9ECC[];
extern u8 D_800C9ED4[];
extern u8 D_800C9EDC[];
extern u8 D_800C9EE4[];

/* main exe: candidates for ff9.h */
extern u32 D_80076F80[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *D_80077DF0;
s32 func_8002B980(u8 *, s32);
extern s32 D_80077E54; /* overlaps Unk80077E50.unk4 in ff9.h */
void func_800C85B8(s32, s32);
extern s32 D_80071E34;

/* Inlined copy of func_800C8108. */
static inline Unk800CAE98 *func_800C8108_inline(s32 i) {
    return &D_800CAE98[i];
}

/* tables inside the D_800C9DB4 block (stored as offsets from its start) */
static inline Unk800C9DB4Path *func_800C9DB4_path(s32 i) {
    return &((Unk800C9DB4Path *)((u8 *)D_800C9DB4 + D_800C9DB4->unkC))[i];
}

static inline Unk800C9DB4Obj *func_800C9DB4_obj(s32 i) {
    return &((Unk800C9DB4Obj *)((u8 *)D_800C9DB4 + D_800C9DB4->unk10))[i];
}

static inline Unk800C9DB4Cam *func_800C9DB4_cam(s32 i) {
    return &((Unk800C9DB4Cam *)((u8 *)D_800C9DB4 + D_800C9DB4->unk18))[i];
}

static inline Unk800C9DB4Step *func_800C9DB4_steps(Unk800C9DB4Path *p) {
    return (Unk800C9DB4Step *)((u8 *)D_800C9DB4 + p->unkC);
}

/* tables inside the D_800C9DF0.unk8 block (offsets from its start) */
static inline Unk800C9DF8Node *func_800C9DF8_node(s32 i) {
    return &((Unk800C9DF8Node *)((u8 *)D_800C9DF0.unk8 + D_800C9DF0.unk8->unk26))[i];
}

static inline Unk800C9DF8Link *func_800C9DF8_link(s32 i) {
    return &((Unk800C9DF8Link *)((u8 *)D_800C9DF0.unk8 + D_800C9DF0.unk8->unk2A))[i];
}

static inline Unk800C9DF8Tri *func_800C9DF8_tri(s32 i) {
    return &((Unk800C9DF8Tri *)((u8 *)D_800C9DF0.unk8 + D_800C9DF0.unk8->unk32))[i];
}

#endif

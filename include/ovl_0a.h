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

/* 0x24-byte entries at Unk800AFCBC.unk8 (func_800AFAC0 sets them) */
typedef struct {
    u32 unk0_0 : 4; /* flags */
    u32 unk0_4 : 4;
    s32 unk1 : 8;
    s32 unk2 : 8;
    u32 unk3 : 8;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s16 unk8[3]; /* position */
    s16 unkE;
    s16 unk10[3]; /* rotation */
    s16 unk16;
    void *unk18;
    s16 unk1C[3]; /* offset */
    s16 unk22;
} FieldEnt; /* 0x24 */

/* initialised by func_800AFCBC */
typedef struct Unk800AFCBC {
    u16 unk0;
    u8 unk2;
    u8 unk3; /* count of 0x24-byte entries at unk8 */
    u16 *unk4; /* 16 entries, follows this header */
    FieldEnt *unk8;
    u8 *unkC[2]; /* primitive buffers, per draw buffer */
} Unk800AFCBC; /* 0x14, followed by the buffers it points to */

/* trigger table entry (func_800B2218) */
typedef struct FieldTrig {
    u8 unk0; /* object uid; low 2 bits also select a channel */
    u8 unk1;
    u16 unk2;
    u16 unk4;
    s16 unk6;
    u16 unk8[2];
} FieldTrig; /* 0xC */

/* script object (func_800B2218) */
typedef struct {
    u8 pad0[0x14];
    VECTOR unk14; /* position */
    u8 pad24[0x34 - 0x24];
    u16 unk34;
    u16 unk36;
    u8 pad38[2];
    u8 unk3A;
    u8 unk3B; /* uid */
} FieldEvtObj;

typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* light id */
    void *unk8; /* model (Unk800C95C8) */
    FieldEvtObj *unkC;
} FieldEvtCtx;

/* 0x330-byte block, FieldMapObj.unk14 (allocated by func_800A9350) */
typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* end of the load area */
    u8 pad8[0xA - 8];
    s16 unkA;
    void *unkC;  /* file 0x5xxxx */
    struct Unk800AFCBC *unk10;
    struct Unk800C9DB4 *unk14;
    void *unk18;
    void *unk1C; /* file 0x1Fxxxx */
    s32 unk20;
    void *unk24;
    s32 *unk28; /* = unk6C, unk40 entries */
    struct FieldObjPlace *unk2C; /* object placements */
    s16 unk30; /* sound depth bias */
    u8 pad32[0x3C - 0x32];
    s16 unk3C;
    s16 unk3E;
    u8 unk40;
    u8 unk41;
    s8 unk42;
    s8 unk43;
    u8 pad44[4];
    s8 unk48;
    s8 unk49;
    u8 unk4A;
    u8 unk4B; /* unk58 entry count */
    void *unk4C; /* file 0x1Exxxx */
    s16 unk50;
    s16 unk52;
    struct FieldActorInfo *unk54; /* = unk90 */
    struct FieldTrig *unk58; /* = unk270 */
    u8 unk5C;
    u8 pad5D[3];
    u8 unk60; /* D_800C9F20 count */
    u8 pad61[0x6C - 0x61];
    s32 unk6C[9];
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
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
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
    s32 unk8[2]; /* saved D_80073998 */
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
    Unk800CA068Pair unk8;
    s16 unkC; /* geometry offset adjust */
    s16 unkE;
    Unk800CA068Pair unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    MATRIX *unk1C;
    u16 *unk20;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u8 pad2C[0x72 - 0x2C];
    u8 unk72;
    u8 unk73;
    u8 pad74;
    u8 unk75;
    s16 unk76;
    u8 pad78[0xA4 - 0x78];
    s16 unkA4;
    s16 unkA6;
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
    u16 unk6; /* entries in the unk10 table */
    u8 pad8[0xC - 8];
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
    u8 pad28[2];
    u16 unk2A;
    u8 pad2C[2];
    u16 unk2E;
    u16 unk30; /* screen origin x */
    u16 unk32; /* screen origin y */
} Unk800C9DB4;

typedef struct {
    u8 unk0;
    s8 unk1;
} Unk800C9DB4Step;

typedef struct {
    unsigned unk0 : 8; /* flags */
    unsigned unk1 : 24; /* entries at unkC */
    unsigned unk4 : 8;
    signed unk5 : 24; /* position (8.8 step index) */
    s16 unk8;
    s16 unkA;
    s32 unkC; /* offset: Unk800C9DB4Step[] */
} Unk800C9DB4Path; /* 0x10 */

/* background sprite: DR_TPAGE followed by a SPRT_16 (0x18 bytes) */
typedef struct {
    DR_TPAGE unk0;
    SPRT_16 unk8;
} BgSprt;

/* packed tile position: x in the top 10 bits, y below it */
typedef struct {
    unsigned pad : 12;
    unsigned y : 10;
    unsigned x : 10;
} BgPos;

/* packed tile texture info (8 bytes) */
typedef struct {
    unsigned clutY : 9;
    unsigned clutX : 6;
    unsigned tpY : 1;
    unsigned tpX : 4;
    unsigned tp : 2;
    unsigned abr : 2;
    unsigned v : 8;
    unsigned u : 8;
    unsigned pad : 20;
    unsigned trans : 1;
} BgTile;

typedef struct {
    unsigned unk0 : 8; /* flags */
    unsigned unk1 : 12; /* depth */
    unsigned unk2 : 12; /* depth */
    u16 unk4; /* scroll wrap x */
    u16 unk6; /* scroll wrap y */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u16 unk18;
    u16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    unsigned unk24 : 8;
    unsigned unk25 : 1;
    unsigned unk25_1 : 7;
    u16 unk26;  /* tile count */
    s32 unk28;  /* offset: BgPos[] */
    s32 unk2C;  /* offset: BgTile[] */
    BgSprt *unk30;
    u8 pad34[4];
} Unk800C9DB4Obj; /* 0x38 */

typedef struct {
    u16 unk0; /* projection distance */
    s16 unk2[3][3];
    s32 unk14[3];
    u16 unk20;
    u16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    u16 unk30;
    u8 pad32[0x34 - 0x32];
} Unk800C9DB4Cam; /* 0x34 */

/* sequence header (func_800BD3A0): offsets from the header start */
typedef struct {
    u8 pad0[6];
    u16 unk6; /* track count */
    u8 pad8[4];
    s32 unkC; /* -> u32 table, 16-byte stride */
    s32 unk10; /* -> track table, 0x38-byte stride */
    u8 pad14[4];
    s32 unk18; /* -> Unk800C9DB4Cam[] */
    u8 pad1C[0x24 - 0x1C];
    u16 unk24;
    u16 unk26;
    u8 pad28[0x30 - 0x28];
    u16 unk30;
    u16 unk32;
} Unk800C9DA4Hdr;

typedef struct {
    u8 unk0;
    u8 pad1[0x38 - 1];
} Unk800C9DA4Trk; /* 0x38 */

typedef struct {
    u32 unk0;
    u8 pad4[0xC];
} Unk800C9DA4Tbl; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
} Unk800C9DA4Ent; /* 0xC */

typedef struct {
    u8 *unk0; /* -> draw buffer index */
    s32 unk4;
    u32 *unk8[2]; /* ordering tables, per draw buffer */
    Unk800C9DA4Hdr *unk10;
    s32 unk14;
    u8 *unk18;
    u8 *unk1C; /* 3 bytes per entry */
    u8 *unk20;
    u8 unk24;
    u8 pad25[3];
    Unk800C9DA4Ent *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} Unk800C9DA4;

/* field light (12 bytes) */
typedef struct {
    s8 unk0; /* type: 1, 2 */
    u8 pad1;
    s8 unk2; /* sprite colour offsets */
    s8 unk3;
    u32 unk4; /* colour */
    s32 unk8; /* 4 floor ids (type 0) */
} FieldLight;

/* 8-byte entries following Unk800B3254.unkC[unk6] */
typedef struct {
    u16 unk0; /* id */
    s8 unk2;
    s8 unk3;
    u32 unk4; /* colour */
} Unk800B3294;

typedef struct {
    u8 pad0[6];
    u8 unk6; /* count */
    u8 unk7; /* count */
    u8 pad8;
    u8 unk9;
    u8 padA[2];
    FieldLight unkC[1];
} Unk800B3254;

/* 6-byte vertex entries at D_800C9DF0.unkC */
typedef struct Unk800C9DFC {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Unk800C9DFC;

/* header of the block D_800C9DF0.unk8 points to */
typedef struct {
    u16 unk0; /* data size */
    s16 unk2; /* original position */
    s16 unk4;
    s16 unk6;
    s16 unk8; /* origin (current position) */
    s16 unkA;
    s16 unkC;
    s16 unkE; /* minimum */
    s16 unk10;
    s16 unk12;
    s16 unk14; /* maximum */
    s16 unk16;
    s16 unk18;
    s16 unk1A; /* default character position */
    s16 unk1C;
    s16 unk1E;
    s16 unk20; /* active floor */
    s16 unk22; /* active triangle */
    u16 unk24; /* triangle count */
    u16 unk26; /* offset: Unk800C9E00[] */
    u16 unk28; /* edge count */
    u16 unk2A; /* offset: Unk800C9DF8Link[] */
    u16 unk2C; /* keyframe animation count (D_800C9DF0.unk20) */
    u16 unk2E; /* offset: D_800C9DF0.unk20 */
    u16 unk30; /* floor count */
    u16 unk32; /* offset: Unk800C9DF8Tri[] */
    u16 unk34; /* normal count */
    u16 unk36; /* offset: D_800C9DF0.unk14 */
    u16 unk38; /* vertex count */
    u16 unk3A; /* offset: D_800C9DF0.unkC */
} Unk800C9DF8; /* walkmesh ("BGI") header */

typedef struct {
    u16 unk0; /* low byte: flags, high byte: mask */
    u8 pad2[2];
    u16 unk4;
    u8 pad6[0x28 - 6];
} Unk800C9DF8Node; /* 0x28 */

typedef struct {
    u16 unk0; /* flags */
    s16 unk2; /* clone */
} Unk800C9DF8Link; /* edge, 4 bytes */

typedef struct {
    u16 unk0; /* flags */
    u16 unk2; /* floor index */
    s16 unk4[3]; /* base position */
    u16 unkA[3]; /* current position */
    s16 unk10[3]; /* minimum */
    s16 unk16[3]; /* maximum */
    u16 unk1C; /* count of unk1E entries */
    u16 unk1E; /* offset: s32[] (Unk800C9E00 indices) */
} Unk800C9DF8Tri; /* 0x20 */

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u16 unk4; /* count of unk6 entries */
    u16 unk6; /* offset: s32[] (Unk800C9E00 indices) */
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

/* keyframe of a walkmesh animation (Unk800C9E10.unkC) */
typedef struct {
    u16 unk0; /* flags */
    s16 unk2; /* value */
    u16 unk4; /* triangle count */
    u16 unk6; /* offset: s32 triangle indices */
} Unk800C9E10Frame;

typedef struct {
    u16 unk0; /* flags */
    u16 unk2; /* data */
    s16 unk4; /* Unk800C9DF8Tri index */
    s16 unk6;
    u16 unk8;
    s16 unkA;
    s16 unkC[3]; /* Unk800C9DFC (vertex) indices */
    s16 unk12[3]; /* edge indices */
    s16 unk18[3]; /* neighbour node ids, -1 = none */
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s32 unk24; /* plane distance (16.16) */
} Unk800C9E00; /* 0x28 */

/* walkmesh edge */
typedef struct {
    s16 unk0;
    s16 unk2; /* index in the neighbour's edge list, -1 = none */
} Unk800C9E08;

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2; /* count of unk4 entries */
    struct Unk800C9DF4 *unk4;
    Unk800C9DF8 *unk8;
    struct Unk800C9DFC *unkC;
    Unk800C9E00 *unk10;
    Unk800C9E04 *unk14;
    Unk800C9E08 *unk18;
    Unk800C9DF8Tri *unk1C;
    Unk800C9E10 *unk20;
} Unk800C9DF0;

typedef struct {
    u16 unk0; /* flags */
    s16 unk2;
    s16 unk4;
    u16 unk6; /* curve: 0 linear, 1 cosine */
    s16 unk8; /* min */
    s16 unkA; /* max */
    s16 unkC; /* current */
    s16 unkE; /* previous */
    u16 unk10;
    s16 unk12; /* speed (8.8) */
    s32 unk14;
} Unk800CAE98; /* 0x18 */

/* file 0x1C0000 header */
typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u16 unk32;
} Unk800AA01C;

/* deferred flag updates: [0] = set, [1] = clear, 4-frame queues for
 * GameData.unk0, FieldMap.unk0 and D_800C9D44 */
typedef struct {
    u16 unk0[2][4];
    u16 unk10[2][4];
    u16 unk20[2][4];
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
    u16 unk2;          /* file 0x3xxxx */
    unsigned unk4 : 24; /* offset from the entry: 3 tables of unk7 words */
    unsigned unk7 : 8;
    u16 unk8;          /* file 0x19xxxx */
    u8 padA[2];
} Unk800AA39CEntry; /* 0xC */

typedef struct {
    u8 unk0;
    u8 unk1; /* count */
    u8 pad2[2];
    Unk800AA39CEntry unk4[1];
} Unk800AA39C;

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

typedef struct {
    u16 unk0; /* flags; 0x10: behind the camera */
    u8 pad2[6];
    WmModel *unk8;
    WmModel *unkC; /* file 0x3xxxx */
    u8 *unk10; /* primitive buffer */
    s32 unk14;
    WmTexInfo *unk18;
    u16 *unk1C; /* parent flags */
    u8 *unk20;
    MATRIX unk24; /* t: position */
    WmModelOut *unk44;
    u8 pad48[0x50 - 0x48];
    struct Unk800AB0B0 *unk50; /* file 0x19xxxx */
    u16 unk54; /* scale */
    u16 unk56;
    u16 unk58;
    u8 pad5A[2];
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x74 - 0x64];
    u16 unk74;
    u16 unk76;
    u16 unk78;
} Unk800C95C8;

/* walkmesh swap tables (func_800C6C04) */
typedef struct {
    u8 pad0[0xC];
    s32 unkC; /* offset of 8-byte link entries */
} Unk800C6C04A;

typedef struct {
    u8 pad0[6];
    u16 unk6; /* offset of the replacement id table */
} Unk800C6C04B;

typedef struct {
    u8 pad0[4];
    u16 unk4; /* count */
    u16 unk6; /* offset of the id list */
} Unk800C6C04E;

/* per-actor extension (FieldActorInfo.unk10) */
typedef struct FieldActorExt {
    u32 unk0; /* flags */
    u8 pad4[4];
    Unk800C95C8 *unk8;
    u8 padC[0xF8 - 0xC];
    FieldEvtCtx *unkF8;
    u8 padFC[0x10C - 0xFC];
    u8 unk10C; /* rgb */
    u8 unk10D;
    u8 unk10E;
    u8 unk10F; /* frames left */
} FieldActorExt;

/* 0x14-byte per-actor record at FieldScene.unk54, indexed by uid */
typedef struct FieldActorInfo {
    u32 unk0; /* flags */
    u8 unk4; /* light index */
    u8 unk5;
    u8 pad6[2];
    u32 unk8; /* colour (bit 24: pending) */
    u32 unkC; /* next colour */
    FieldActorExt *unk10;
} FieldActorInfo;

/* background VRAM animation (func_800BB090) */
typedef struct {
    RECT unk0;      /* destination area */
    DVECTOR *unk8;  /* per-frame source position */
} BgAnimRect; /* 0xC */

typedef struct {
    u8 unk0; /* flags: 1 active, 2 one-shot, 4 scroll */
    u8 unk1; /* frame count */
    s16 unk2; /* speed (20.12) */
    u16 unk4; /* random delay base */
    u16 unk6; /* random delay range */
    s32 unk8; /* position (20.12) */
    u8 unkC; /* rect count */
    u8 padD;
    u16 unkE; /* last frame */
    BgAnimRect *unk10;
} BgAnim; /* 0x14 */

typedef struct {
    u16 unk0; /* count */
    u8 pad2[2];
    BgAnim unk4[1];
} BgAnimHdr;

/* screen fade quads (D_800CA140, func_800C02D0) */
typedef struct {
    s8 unk0; /* flags */
    u8 unk1; /* rgb */
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 padA[2];
    POLY_F4 unkC[2][4]; /* per draw buffer */
    DR_TPAGE unkCC[2];
} FieldFade; /* 0xDC */

extern FieldFade D_800CA140[];
extern u16 D_800C9DA0; /* FieldFade count */

/* palette fade packet (func_800C07D8), in the D_800C9DA4.unk2C buffer */
typedef struct {
    u8 unk0;
    u8 unk1;  /* colour count */
    u8 unk2;  /* first */
    u8 unk3;  /* last */
    s16 unk4; /* mode */
    u8 pad6[2];
    u32 unk8; /* frames */
    s32 unkC;
    s32 unk10; /* r step (16.16) */
    s32 unk14;
    s32 unk18;
    s32 unk1C; /* offset: saved colours */
    s32 unk20; /* offset: work colours */
    RECT unk24;
} FieldPalAnim;

/* pause screen (func_800AF740) */
typedef struct {
    SPRT unk0;
    DR_TPAGE unk14;
    u8 pad1C[0x40 - 0x1C];
} FieldPausePkt;
extern DR_TPAGE D_800C9F90;
extern u32 D_800C9F98;
extern FieldPausePkt D_800C9FA0;
extern TILE D_800C9FE0;

/* 3D sprite data (FieldEnt.unk18) */
typedef struct {
    u16 unk0; /* frame count | 0x8000 */
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u8 unk6;  /* width */
    u8 unk7;  /* height */
    u16 unk8[1]; /* frame offsets, then the u16-counted table */
} FieldSprHdr;

/* parameter block for func_800C9AB8 */
typedef struct {
    u16 *unk0;
    u8 *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} FieldSprArgs;

u8 *func_800C970C(u8 *, FieldSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, u32 *);
u8 *func_800C9AB8(u8 *, FieldSprArgs *, u8 *, u32 *);
void func_800316D8(SVECTOR *, MATRIX *);

/* object placement (func_800AD488) */
typedef struct {
    unsigned unk0 : 8;
    unsigned unk1 : 12; /* scale x */
    unsigned unk2 : 12; /* scale z */
    unsigned unk4 : 12; /* angle */
    signed unk4_12 : 10; /* offset x */
    signed unk4_22 : 10; /* offset z */
} FieldObjEnt;

/* object placement entries (FieldScene.unk2C) */
typedef struct FieldObjPlace {
    FieldObjEnt unk0;
    u8 pad8[0x28 - 8];
} FieldObjPlace; /* 0x28 */

FieldLight *func_800B3254(Unk800B3254 *);
Unk800B3294 *func_800B3294(Unk800B3254 *, s32);
s32 func_800C76E8(s32, u16 *, u16 *);
s32 func_800C6DA4(Unk800C9DF8 *, Unk800C9E10 *, Unk800C9DF8Pt *);
FieldLight *func_800B3300(Unk800B3254 *, FieldLight **, FieldLight **);
FieldLight *func_800B3378(Unk800B3254 *, FieldEvtCtx *, s16 *);

/* object model slot (func_8001E350) */
typedef struct {
    s32 unk0; /* flags: 1 = loaded */
    s32 unk4; /* file id */
    Unk800C95C8 *unk8;
    void *unkC; /* actor */
} FieldObjSlot;

/* ---- overlay functions ---- */
s32 func_800AD944(s32, s32, s32);
s32 func_800A8588(void);
s32 func_800C6EEC();
void func_80054A28(void *);
s32 func_800B0124(s32, s32, s32, s32, s32);
void func_800B1A20(s16, s16, s16, u8 *, u8 *);
s32 func_800B36E0(s32, s32);
s32 func_800B4894(void);
void func_800B3ACC(void);
s32 func_8001E350(s32);
s32 func_800B31B8(FieldEvtCtx *, s32, s32, s32);
void func_800B5938(void);
void func_800AC67C(s32);
s32 func_800C0038(s32, s32);
void func_800A8198(void);
void func_800A82F8(void);
void func_800A7AAC(s32);
void func_800A7B10(s32);
void func_800A85F4(s32);
void func_800A8698(void);
void func_800B5114(void);
void func_800B51D0(void);
void func_800B5290(void);
void func_800B52DC(void);
void func_800B532C(void);
void func_800B57C8(void);
void func_800B1CE4(void);
/* func_800AA0C8: some callers pass two arguments; no prototype */
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
extern u8 D_800CA108; /* background sprite x extent */
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
extern s16 D_800C9DDC;
extern char D_800A7814[]; /* "          1/ny = %lx\n" (walkmesh dump) */
extern s16 D_800CA9D8[][3]; /* collision push vectors */
extern s16 D_800CAA38[];
extern s32 D_800CAA58[];
/* mask-bit primitives, one per draw buffer (func_800AE788) */
typedef struct {
    u32 tag;
    u32 code[5];
} Unk800C9F60; /* 0x18 */
extern Unk800C9F60 D_800C9F60[2];
extern Unk800C9DF4 D_800CAAD8[24];
Unk800C9E00 *func_800C1F98(Unk800C9DF4 *, Unk800C9E00 *, s32);
u8 func_800C4B30(Unk800C9DF8 *, Unk800C9DF8Tri *, Unk800C9E00 *, s32);
s32 func_800C4EC8(Unk800C9DF8 *, Unk800C9DF8Tri *, Unk800C9E00 *, SVECTOR *);
typedef struct {
    s32 unk0;
    s32 unk4;
} Unk8002208C;
Unk8002208C *func_8002208C(void);
s32 func_800221C0(s32, s32);
void func_80022280(s32, s32);
s32 func_80050BE0(s32);

extern RECT *D_800C9EA0;
extern u8 D_800C9EA4[];
extern RECT *D_800C9EA8;
extern u8 *D_800C9EAC[5];
extern u8 D_800A7860[]; /* text */
extern RECT D_800C9D98;

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

/* 8-byte entries following Unk800B3254.unkC[unk6] */


/* 10 VRAM cache slots at 0x800C9CCC. splat puts the first word at the end of
 * .text (as "func_800C9CCC"), so the array is reached through that label. */
typedef struct {
    s16 unk0;
    s16 unk2; /* id */
    u32 unk4; /* address (0 = free) */
    s32 unk8; /* size */
} Unk800C9CCC; /* 0xC */
extern Unk800C9CCC func_800C9CCC[10];
Unk800C9CCC *func_800AC344(s32);
u32 func_800AC380(Unk800C9CCC **);
void func_800A86FC(void);
void func_800A8724(void);
/* func_800A879C: called before its definition without a prototype */
s32 func_800C3EC0(s32, s16, s16, s16);
s32 func_800C3608(Unk800C9DF4 *);
s32 func_800C79B0(u32);
s32 func_800C6E94(void);
void func_800A87DC(FieldMapObj *, s32);

/* main exe: candidates for ff9.h */
extern u32 D_80076F80[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *D_80077DF0; /* = D_80077DE8.unk8, referenced by its own name */
extern s32 D_80077DEC; /* = D_80077DE8.unk4, referenced by its own name */
s32 func_8002B980(u8 *, s32);
extern s32 D_80077E54; /* overlaps Unk80077E50.unk4 in ff9.h */
void func_800C85B8(s32, s32);
void func_800B5794(void);
s32 func_800AAB30(s32, void *);
void func_800C883C(s32);
void func_800C8AE8(RECT *, s32);
extern s32 D_80071E34;
void func_801F3488(void *); /* overlay 0x65 (loaded at 0x801F0000) */
void func_8019A5B0(s32); /* overlay loaded at 0x80198000 */
s32 func_8019AB70(void);
void func_80032100(void);
Unk8004AB64 *func_8004AE00(void);
void func_80023A0C(void);
void func_800223CC(void);

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

static inline Unk800C9DF8Pt *func_800C9DF8_pts(Unk800C9E10 *e) {
    return (Unk800C9DF8Pt *)((u8 *)D_800C9DF0.unk8 + e->unkC);
}

static inline Unk800C9DF8Tri *func_800C9DF8_tri(s32 i) {
    return &((Unk800C9DF8Tri *)((u8 *)D_800C9DF0.unk8 + D_800C9DF0.unk8->unk32))[i];
}

#endif

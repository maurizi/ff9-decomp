#ifndef FX_1A2_H
#define FX_1A2_H

/* Effect module fx_1a2 (dir 13 file 0x1a2): Moonlight Slash (Blank, enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_1a2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MOONLIGHT_SLASH_LIGHT_BAND       0x1 /* model: cylinder band, blue light-streak texture */
#define FX_MOONLIGHT_SLASH_LIGHT_BAND_2     0x2 /* model: cylinder band, blue light-streak texture */
#define FX_MOONLIGHT_SLASH_LIGHT_RING       0x3 /* model: flat ring of blue light streaks */
#define FX_MOONLIGHT_SLASH_MOON             0x5 /* emitter: blue full-moon particles, 30 frames + blue gouraud rings */
#define FX_MOONLIGHT_SLASH_SPIKED_SHAPE 0x6 /* model: 20-poly spiked shape (Hi_RegisterTexListModel, texture not drawn) (look pending) */
#define FX_MOONLIGHT_SLASH_CYAN_RINGS       0x7 /* emitter: thin cyan streak fan + white-cyan gouraud rings */
#define FX_MOONLIGHT_SLASH_SPRITES          0x8 /* emitter: three sprite elements, 24 frames, render black (look pending) */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL   0x9 /* emitter: spiralling 3D trail, blue crescent strip */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2 0xA /* emitter: 3D trail, blue crescent strip */
#define FX_MOONLIGHT_SLASH_BLADE            0xB /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_BLADE_2          0xC /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_BLADE_3          0xD /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_ROT_KEYS         0xE /* table: per-frame rotation keys */
#define FX_MOONLIGHT_SLASH_CYAN_STREAKS     0xF /* emitter: three fans, thin cyan light streaks */
#define FX_MOONLIGHT_SLASH_ARMOR            0x10 /* model: grey-brown armour plating (129 polys) */
#define FX_MOONLIGHT_SLASH_ARMOR_2          0x11 /* model: grey-brown armour plating (24 polys) */
#define FX_MOONLIGHT_SLASH_WHITE_RINGS      0x12 /* rings: white gouraud rings, 10 frames */
#define FX_MOONLIGHT_SLASH_WHITE_RINGS_2    0x13 /* rings: white gouraud rings, 10 frames */
#define FX_MOONLIGHT_SLASH_CYAN_SPARKLES    0x14 /* emitter: cyan sparkle dots, 12 frames */
#define FX_MOONLIGHT_SLASH_CYAN_SPARKLES_2  0x15 /* emitter: cyan sparkle dots + faint green-grey particles */
#define FX_MOONLIGHT_SLASH_SPRITES_2        0x17 /* emitter: three sprite elements, 24 frames, render black (look pending) */

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
} Fx1a2Seg; /* 0x10 */

typedef struct {
    SVECTOR verts0[3];
} Fx1a2Tri; /* 0x18 */

typedef struct {
    s32 pos0; /* position, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12; /* write-only (0x1000) */
    s16 rot14;
    s16 rot16;
    s16 frame18;
    s16 delay1A;
} Fx1a2Piece; /* 0x1C */

extern Fx1a2Tri fx_1a2_shatterTris[130];
extern Fx1a2Piece fx_1a2_shatterPieces[130];
typedef struct {
    s16 count0; /* bounce count */
    s16 height2; /* sink speed */
    s16 height4; /* bounce height */
    s16 wait6; /* wait frames */
    s32 state8; /* state */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s32 scale1C; /* scale */
} Fx1a2Rock;

typedef struct {
    u8 pad0[0x48];
    s32 model48; /* model */
} Fx1a2Inst;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* unit position */
    SVECTOR pos10; /* body position */
    SVECTOR pos18; /* particle position */
    s16 pos20; /* bouncing rock position */
    s16 pos22;
    s16 pos24;
    s16 unk26; /* unused */
    s32 vel28; /* bouncing rock velocity */
    s32 scale2C; /* scale */
    s32 model30[3]; /* models */
    s32 model3C;
    s32 model40;
    s32 model44;
    s32 model48;
    s32 model4C;
    s32 model50;
} Fx1a2Work80; /* 0x54 */

extern BtlModelBuf fx_1a2_effModels[];
extern CVECTOR fx_1a2_colors0;
extern CVECTOR fx_1a2_colors1;
extern CVECTOR fx_1a2_colors2;
extern Fx1a2Seg fx_1a2_ribbonEdges0[];
extern Fx1a2Seg fx_1a2_ribbonEdges1[];
extern Fx1a2Seg fx_1a2_ribbonEdges2[];
extern s32 fx_1a2_op80Trails0[];
extern s32 fx_1a2_op80Trails1[];

/* copied */
extern Unk800F78A4 *fx_1a2_env;
/* end copied */

#endif

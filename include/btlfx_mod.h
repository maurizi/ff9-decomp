#ifndef BTLFX_MOD_H
#define BTLFX_MOD_H

/* Battle effect modules (FF9.IMG dir 13, loaded at 0x801E7700 / 0x801EC700; docs/ff9img.md).
 * A module is its own binary and calls nothing directly. It reaches ovl_0b through the export table
 * g_btlfxModuleExports (ovl_0b 0x800F7510), whose address the effect environment carries
 * (g_btlfxEnv->exports; env is an argument of every op handler). BtlfxExports is that table: one member per
 * entry, named after the ovl_0b / main function it points to, with the parameter types of its definition.
 *
 * The modules' own view of the table differs from ovl_0b's definitions in two ways, both needed to match:
 * every entry returns a value (fx_001_op80 only matches when the call to btlfx_getUnitBodyPos, void in
 * ovl_0b, is a call_value), and narrow integer parameters are words (fx_001 passes Hi_ModifyEffModelRGB
 * unmasked values). So void returns and s16/u16/s8/u8 parameters are widened to s32 here.
 *
 * ovl_0b.h is included for the battle types. Its functions must not be called directly from a module
 * (nothing links a module to ovl_0b): call through BTLFX_EXPORTS(env)->name(...). */
#include "ovl_0b.h"

typedef struct BtlfxExports {
    int (*LoadImage)(RECT *, u32 *); /* 0x000 */
    int (*StoreImage)(RECT *, u32 *); /* 0x004 */
    s32 (*btlfx_stepModuleParticle)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *); /* 0x008 (asm; DS port F12: particle, bias, out) */
    s32 (*btlfx_drawTrailScreen)(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32); /* 0x00C */
    s32 (*btl_setVsyncMode)(s32); /* 0x010 */
    s32 (*func_800316D8)(void *, void *); /* 0x014 */
    s32 (*Hi_RegisterGouEffModel)(BtlUnit9C8 *); /* 0x018 */
    s32 (*SetDrawMove)(void *, RECT *, int, int); /* 0x01C */
    MATRIX * (*ScaleMatrix)(MATRIX *, VECTOR *); /* 0x020 */
    MATRIX * (*CompMatrix)(MATRIX *, MATRIX *, MATRIX *); /* 0x024 */
    s32 (*SetRotMatrix)(MATRIX *); /* 0x028 */
    s32 (*Hi_StopSummonTexAnim)(s32, s32); /* 0x02C */
    s32 (*Hi_StartSummonTexAnim)(s32, s32, s32); /* 0x030 */
    s32 (*SetTransMatrix)(MATRIX *); /* 0x034 */
    s32 (*math_cos)(s32); /* 0x038 */
    s32 (*math_sin)(s32); /* 0x03C */
    long (*ratan2)(long, long); /* 0x040 */
    s32 (*SquareRoot0)(void); /* 0x044 (main exe) */
    s32 (*LoadAverageShort12)(SVECTOR *, SVECTOR *, s32, s32, SVECTOR *); /* 0x048 */
    s32 (*Hi_RegisterTexListModel)(BtlUnit9C8 *, BtlTexInfo *, s16 *, s32); /* 0x04C */
    s32 (*printf)(const char *, ...); /* 0x050 */
    s32 (*Hi_RegisterSolidEffModel)(BtlUnit9C8 *); /* 0x054 */
    s32 (*Hi_RegisterTexEffModel)(BtlUnit9C8 *, s32, s32, s32, s32); /* 0x058 */
    s32 (*Hi_RegisterSummonModel)(struct Unk80100DC054 *, s16 *); /* 0x05C */
    s32 (*Hi_DrawEffModel)(SVECTOR *, VECTOR *, s32, s32); /* 0x060 */
    s32 (*Hi_DrawSummonModel)(SVECTOR *, VECTOR *, s32, s32, s32); /* 0x064 */
    s32 (*Hi_SetSummonMotion)(BtlUnit9CC *, s32); /* 0x068 */
    s32 (*btl_forEachModelVertex)(BtlUnit9C8 *, void (*)(SVECTOR *, s32)); /* 0x06C */
    s32 (*btlfx_initEffModels)(BtlModel *, s32); /* 0x070 */
    s32 (*btlfx_initSummonModels)(BtlModel *, s32); /* 0x074 */
    s32 (*btlfx_clearEffModels)(void); /* 0x078 */
    s32 (*btlfx_clearSummonModels)(void); /* 0x07C */
    s32 (*func_800DC818)(s32, s32, u16 *); /* 0x080 */
    s32 (*func_800DC858)(s32, s32, u16 *); /* 0x084 */
    s32 (*btlfx_setUnitMotion)(s32, s32); /* 0x088 */
    s32 (*btlfx_getUnitMotion)(s32, s32); /* 0x08C */
    s32 (*func_800D8C9C)(void); /* 0x090 */
    s32 (*func_800D8CA4)(void); /* 0x094 */
    s32 (*btlfx_setMotionFrame)(s32, s32); /* 0x098 */
    s32 (*btlfx_getMotionFrame)(s32); /* 0x09C */
    s32 (*btlfx_playUnitMotion)(s32, s32); /* 0x0A0 */
    s32 (*func_800D8CAC)(void); /* 0x0A4 */
    s32 (*btlfx_resetUnitMotion)(s32, s32); /* 0x0A8 */
    s32 (*btlfx_align4)(s32); /* 0x0AC */
    s32 (*btlfx_sqrt)(s32, s32); /* 0x0B0 */
    s32 (*btlfx_lerp)(s32, s32, s32); /* 0x0B4 */
    s32 (*btlfx_lerpRatio)(s32, s32, s32, s32); /* 0x0B8 */
    s32 (*btlfx_lerpVec)(s16 *, s16 *, s32, s16 *); /* 0x0BC */
    s32 (*btlfx_rand)(void); /* 0x0C0 */
    s32 (*btlfx_randRange)(s32, s32); /* 0x0C4 */
    s32 (*btlfx_randCentered)(s32); /* 0x0C8 */
    s32 (*btlfx_distance)(SVECTOR *, SVECTOR *); /* 0x0CC */
    s32 (*btlfx_randRotation)(SVECTOR *); /* 0x0D0 */
    s32 (*btlfx_aimAngles)(s16 *, s16 *, s16 *); /* 0x0D4 */
    s32 (*func_800CFCA8)(SVECTOR *, s32, s32); /* 0x0D8 */
    s32 (*func_800CFD78)(s16 *, s16 *, s32, s16 *); /* 0x0DC */
    s32 (*btlfx_angleDiff)(s32, s32, s32 *); /* 0x0E0 */
    s32 (*btlfx_turnToward)(s16 *, s16 *, s32); /* 0x0E4 */
    s32 (*func_800D0130)(SVECTOR *, MATRIX *); /* 0x0E8 */
    s32 (*func_800D01D4)(SVECTOR *, VECTOR *, MATRIX *, s32); /* 0x0EC */
    s32 (*func_800D044C)(SVECTOR *, s32, s32, XY32 *, s32, s32); /* 0x0F0 */
    s32 (*func_800D0580)(s16 *, XY32 *, s32); /* 0x0F4 */
    s32 (*func_800D0674)(SVECTOR *, SVECTOR *, SVECTOR *, s32, s32, s32 *); /* 0x0F8 */
    DR_TPAGE * (*btlfx_allocDrMode)(s32, s32); /* 0x0FC */
    s32 (*btlfx_drawScreenTint)(u32 *, s32, s32, s32, s32); /* 0x100 */
    s32 (*Hi_ModifySummonModelRGB)(s32, s32, s32, s32); /* 0x104 */
    s32 (*btlfx_drawTrail3D)(s32, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32); /* 0x108 */
    s32 (*btlfx_drawTrailFlat)(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32); /* 0x10C */
    s32 (*btlfx_drawTrail)(u8 *, s32, s32 *, s32, s32, s32, s32, u8 *, s32); /* 0x110 */
    s32 (*btlfx_drawParticles)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, s32, s32); /* 0x114 */
    s32 (*func_800D198C)(u8 *, SVECTOR *, s32, s32, s32); /* 0x118 */
    s32 (*func_800D1BC4)(u32, u8 *, s32, s32, s32, s32, s32); /* 0x11C */
    s32 (*btlfx_drawAnimQuads)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32); /* 0x120 */
    s32 (*btlfx_drawSprites)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32); /* 0x124 */
    s32 (*func_800D23D8)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32); /* 0x128 */
    s32 (*func_800D2938)(Unk800D2938 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32); /* 0x12C */
    s32 (*func_800D2DBC)(u8 *, s32, s32, s32, s32, s32); /* 0x130 */
    s32 (*btlfx_drawBackdrop)(BtlBgLayers *, s32); /* 0x134 */
    s32 (*func_800D3B6C)(u8 *, SVECTOR *, s32, s32); /* 0x138 */
    s32 (*func_800E00DC)(s32, Pool30Entry *, Pool30 *); /* 0x13C */
    s32 (*func_800E0134)(Pool30Entry *, Pool6CEntry *); /* 0x140 */
    s32 (*func_800E0168)(Pool30 *); /* 0x144 */
    s32 (*func_800E14D8)(u32, Pool6C *); /* 0x148 */
    Pool6CEntry * (*func_800E1534)(Pool30 *, Pool6C *, u8 *, s32); /* 0x14C */
    s32 (*func_800E1708)(Pool30 *, s32, s32); /* 0x150 */
    s32 (*func_800E1810)(Pool30 *, Pool6CEntry *); /* 0x154 */
    s32 (*func_800E1874)(Pool30 *, Pool6C *); /* 0x158 */
    Pool30Entry * (*func_800E1F90)(Pool30 *); /* 0x15C */
    s32 (*func_800E2000)(Pool30 *, Pool6C *); /* 0x160 */
    s32 (*func_800E2F84)(Pool30 *, Pool30Entry *, SVECTOR *, SVECTOR *); /* 0x164 */
    s32 (*btlfx_initTasks)(void); /* 0x168 */
    s32 (*btlfx_allocTaskMem)(Unk80100C78Entry *, s32); /* 0x16C */
    s32 (*btlfx_spawnTask)(s32, s32, s32, s32); /* 0x170 */
    s32 (*btlfx_updateTasks)(void); /* 0x174 */
    s32 (*btlfx_poolSize)(s32, s32); /* 0x178 */
    s32 (*btlfx_poolInit)(struct Unk800DA1B8 *, Unk80100C78Entry *, s32, s32, s32); /* 0x17C */
    s32 (*btlfx_poolClear)(Unk800DA1B8 *); /* 0x180 */
    void * (*btlfx_poolSpawn)(s16 *); /* 0x184 */
    s32 (*btlfx_poolUpdate)(Unk800DA1B8 *); /* 0x188 */
    s32 (*btlfx_findFileSlot)(s32); /* 0x18C */
    s32 (*Hi_SetSummonMotFrame)(s32, s32); /* 0x190 */
    s32 (*func_800E6D58)(void); /* 0x194 */
    s32 (*btlfx_getFileData)(s32, s32); /* 0x198 */
    s32 (*btlfx_getFileDataSize)(s32, s32); /* 0x19C */
    s32 (*func_800DA4E8)(void); /* 0x1A0 */
    s32 (*btlfx_loadFile)(s32); /* 0x1A4 */
    s32 (*func_800DA5C4)(void); /* 0x1A8 */
    s32 (*btlfx_updateLoader)(void); /* 0x1AC */
    s32 (*btlfx_reset)(void); /* 0x1B0 */
    s32 (*btlfx_start)(s32); /* 0x1B4 */
    s32 (*btlfx_runScript)(void); /* 0x1B8 */
    s32 (*btlfx_heapInit)(Heap *, s32); /* 0x1BC */
    s32 (*btlfx_heapAlloc)(Heap *, s32); /* 0x1C0 */
    s32 (*btlfx_heapWrapIfFull)(Heap *, s32); /* 0x1C4 */
    s32 (*btlfx_heapFree)(void *); /* 0x1C8 */
    s32 (*func_800E3C58)(void); /* 0x1CC */
    s32 (*func_800E3C98)(void); /* 0x1D0 */
    Pool6CEntry * (*func_800E3CEC)(s32, s32); /* 0x1D4 */
    s32 (*func_800E3D20)(s32, s32); /* 0x1D8 */
    s32 (*func_800E3D54)(Pool6CEntry *); /* 0x1DC */
    s32 (*btlfx_setTargets)(BtlUnitList *); /* 0x1E0 */
    s32 (*func_800E6DF4)(void); /* 0x1E4 */
    s32 (*func_800E6DB8)(s32); /* 0x1E8 */
    s32 (*btlfx_clearTargets)(void); /* 0x1EC */
    s32 (*btlfx_getUnitRadius)(s32); /* 0x1F0 */
    s32 (*btlfx_getUnitHeight)(s32); /* 0x1F4 */
    s32 (*btlfx_getUnitHomePos)(s32, s16 *); /* 0x1F8 */
    s32 (*btlfx_getUnitPos)(s32, s16 *); /* 0x1FC */
    s32 (*btlfx_getUnitBodyPos)(s32, s32, s16 *); /* 0x200 */
    s32 (*btlfx_setUnitPos)(s32, s16 *); /* 0x204 */
    s32 (*btlfx_nextUnitPos)(s32, s16 *, BtlUnit **, s32 *); /* 0x208 */
    s32 (*btlfx_getUnitRot)(s32, s16 *); /* 0x20C */
    s32 (*btlfx_isEnemyUnit)(s32); /* 0x210 */
    s32 (*btlfx_turnUnitToward)(s32, s16 *, s16 *, s32, s32, s32, s32); /* 0x214 */
    s32 (*btlfx_getStrikePos)(s32, s32, s16 *); /* 0x218 */
    s32 (*btlfx_compareDepth)(s32, s32); /* 0x21C */
    s32 (*func_800E7CC4)(s32, s32); /* 0x220 */
    s32 (*btlfx_fadeUnitGroup)(u32, s32); /* 0x224 */
    s32 (*btlfx_execCmd)(s32); /* 0x228 */
    s32 (*btlfx_addPrimAbr)(s32, s32, void *, s32); /* 0x22C */
    s32 (*btlfx_showPopups)(s32); /* 0x230 */
    s32 (*btlfx_addPrim)(u32, s32, void *); /* 0x234 */
    s32 (*btlfx_sampleGradient)(u8 *, CVECTOR *, s32); /* 0x238 */
    s32 (*btlfx_linkPrimAbr)(u32, u32 *, void *, s32); /* 0x23C */
    s32 (*func_800E6624)(s32, s32, s32, s32, s32, s32, s32); /* 0x240 */
    s32 (*Hi_DrawMorphModelByBone)(s32, s32, s32, s32, s32, s32); /* 0x244 */
    void (*func_800E6764)(u32, s32, s32, s32, s32, s32, s32, s32, s32, SVECTOR *, VECTOR *, CVECTOR *, s32,
                          void (*)(SVECTOR *)); /* 0x248 (arg 13: per-vertex callback in the module) */
    s32 (*Hi_ModifySummonModelAbr)(s32, s32); /* 0x24C */
    s32 (*func_800E6C58)(SVECTOR *); /* 0x250 */
    s32 (*Hi_GetSummonBonePos)(s32, s32, u16 *); /* 0x254 */
    s32 (*func_800DA4C4)(s32); /* 0x258 */
    s32 (*Hi_FreeEffModel)(s32); /* 0x25C */
    s32 (*btlfx_setWeaponVisible)(s32, s32); /* 0x260 */
    s32 (*func_800D0B18)(s32, s32, s32, s32, s32, s32, CVECTOR *, s32, s32, s32, s32); /* 0x264 */
    s32 (*Hi_ModifyEffModelAbr)(s32, s32); /* 0x268 */
    s32 (*Hi_ModifyEffModelRGB)(s32, s32, s32, s32); /* 0x26C */
    s32 (*btlfx_turnUnit)(s32, s32); /* 0x270 */
    s32 (*Hi_ShowSummonModelMesh)(s32, s32); /* 0x274 */
    s32 (*Hi_HideSummonModelMesh)(s32, s32); /* 0x278 */
    MATRIX * (*MulRotMatrix)(MATRIX *); /* 0x27C */
    s32 (*func_800D8A20)(s32, s32); /* 0x280 */
    u32 (*func_800D89E8)(s32); /* 0x284 */
    s32 (*Hi_DrawEffModelByBone)(s32, s32, s32, s32); /* 0x288 */
    s32 (*Hi_DrawMorphEffModel)(SVECTOR *, VECTOR *, s32, s32, s32, s32); /* 0x28C */
    s32 (*Hi_GetSummonBoneMatrix)(s32, s32, BtlBone *); /* 0x290 */
    s32 (*ClearImage)(void); /* 0x294 (main exe) */
    int (*MoveImage)(RECT *, int, int); /* 0x298 */
    s32 (*func_800DF938)(void); /* 0x29C */
    s32 (*btlfx_getUnitScale)(s32, VECTOR *); /* 0x2A0 */
    s32 (*btlfx_setUnitScale)(s32, s32 *); /* 0x2A4 */
    s32 (*btlfx_setUnitRot)(s32, s16 *); /* 0x2A8 */
    s32 (*Hi_RegisterTexPtrModel)(BtlUnit9C8 *, s16 *, s16 *); /* 0x2AC */
    s32 (*func_800CFE88)(MATRIX *, s32, s32); /* 0x2B0 */
    s32 (*func_800CFF40)(MATRIX *, s16 *); /* 0x2B4 */
    s32 (*btlfx_getWeaponPos)(s32, s32, s16 *); /* 0x2B8 */
    s32 (*btlfx_fadeUnitById)(s32, s32); /* 0x2BC */
    s32 (*func_800E8B28)(s32, s32, s32); /* 0x2C0 */
    s32 (*func_800DC8AC)(s32, s32, BtlBone *); /* 0x2C4 */
    s32 (*func_800DC8EC)(s32, s32, BtlBone *); /* 0x2C8 */
    s32 (*btlfx_startScreenFade)(s32, s32); /* 0x2CC */
    s32 (*func_800E98C8)(s32, s32); /* 0x2D0 */
    s32 (*func_800E98F8)(s32); /* 0x2D4 */
    s32 (*btlfx_playFileSound)(s32, s32, s32); /* 0x2D8 */
    s32 (*func_800E8554)(s32, s32); /* 0x2DC */
    s32 * (*btlfx_swapModelData)(s32, s32 *); /* 0x2E0 */
    s32 (*Hi_SetEffModelOffset)(s32, s32); /* 0x2E4 */
    s32 (*btlfx_setUnitLight)(s32, s32); /* 0x2E8 */
    s32 (*btlfx_setUnitVisible)(s32, s32); /* 0x2EC */
    s32 (*func_800E8980)(s32); /* 0x2F0 */
    s32 (*func_800E6E1C)(s32, s32, s32, s32, u32 *, s32, s32, s32); /* 0x2F4 */
    BtlUnit * (*btl_getUnit)(s32); /* 0x2F8 */
    s32 (*Hi_GetSplitMdlVertex)(s32, s32); /* 0x2FC */
    s32 (*func_800DF928)(void); /* 0x300 */
    s32 (*Hi_SplitMdlVertex)(s32, s32, s32); /* 0x304 */
    s32 (*func_800DF920)(void); /* 0x308 */
    s32 (*func_800DF930)(void); /* 0x30C */
    s32 (*Hi_GetMdlVertexPtr)(s32, s32, s32, s32 *, s32); /* 0x310 */
    s32 (*func_800DD0B0)(s32); /* 0x314 */
    s32 (*func_800DD0DC)(s32); /* 0x318 */
    s32 (*Hi_DrawSliceEffModel)(SVECTOR *, VECTOR *, s32, s32); /* 0x31C */
    s32 (*Hi_SetEffModelSlice)(s32, s32); /* 0x320 */
    s32 (*btlfx_callScript)(s32); /* 0x324 */
    s32 (*btlfx_getMotionLength)(s32); /* 0x328 */
    s32 (*btlfx_isUnitActive)(s32); /* 0x32C */
    s32 (*func_800CFC2C)(s32, s32, s32, s32, s32, s32 *); /* 0x330 */
    s32 (*btlfx_startFadeBackground)(s32, s32); /* 0x334 */
    s32 (*btlfx_registerEffectModel)(Unk800E8F50 *, s32); /* 0x338 */
    s32 (*btlfx_isUnitKind)(s32, s32); /* 0x33C */
    s32 (*btlfx_registerSummonModel)(Unk800E9000 *, s32 *, s32, Unk80100DC054 *); /* 0x340 */
    s32 (*func_800DA898)(void); /* 0x344 */
    s32 (*btlfx_replaceSound)(s32, s32, s32, s32); /* 0x348 */
    s32 (*func_800E83EC)(void); /* 0x34C */
    s32 (*btlfx_stopSound)(s32); /* 0x350 */
    s32 (*btlfx_clearUnitStatus)(s32); /* 0x354 */
    s32 (*func_800E87C4)(s32); /* 0x358 */
    s32 (*func_800DD120)(BtlTexInfo *, BtlTexInfo *, s32); /* 0x35C */
} BtlfxExports; /* followed in ovl_0b by the g_btlfxEnv block (D_800F7870) */

#define BTLFX_EXPORTS(env) ((BtlfxExports *)(env)->exports)

/* Modules copy SVECTORs as two words (lw/sw pairs, not a halfword struct copy). */
#define BTLFX_COPY_SVEC(dst, src) \
    (((s32 *)(dst))[0] = ((s32 *)(src))[0], ((s32 *)(dst))[1] = ((s32 *)(src))[1])

/* Module header, pointed to by the module's first word (its first .rodata word).
 * btlfx_updateLoader installs &ops[0] as the slot's op table (D_80100C70) and calls init when non-NULL
 * (NULL in every module on the discs); script op 0x80 + i runs ops[i] (btlfx_spawnTask). */
typedef struct BtlfxModHeader {
    s32 unk0; /* 0 in every module */
    void (*init)(s32 data, Unk800F78A4 *env, s32 exports);
    EffFn ops[1]; /* as many as the module uses (up to 8 on the discs) */
} BtlfxModHeader;

#endif

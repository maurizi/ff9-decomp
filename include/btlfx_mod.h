#ifndef BTLFX_MOD_H
#define BTLFX_MOD_H

/* Battle effect modules (FF9.IMG dir 13, loaded at 0x801E7700 / 0x801EC700; docs/ff9img.md).
 * A module is its own binary and calls nothing directly. It reaches ovl_0b through the export table
 * g_btlfxModuleExports (ovl_0b 0x800F7510), whose address the effect environment carries
 * (g_btlfxEnv->unk10; env is an argument of every op handler). BtlfxExports is that table: one member per
 * entry, named after the ovl_0b / main function it points to. __typeof__ keeps the ones declared in
 * ovl_0b.h / ff9.h / psyq.h in sync; the others are copied from ovl_0b's definitions.
 *
 * ovl_0b.h is included for the battle types. Its functions must not be called directly from a module
 * (nothing links a module to ovl_0b): call through BTLFX_EXPORTS(env)->name(...). */
#include "ovl_0b.h"

typedef struct BtlfxExports {
    __typeof__(LoadImage) *LoadImage; /* 0x000 */
    __typeof__(StoreImage) *StoreImage; /* 0x004 */
    void (*func_800E0014)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *); /* 0x008 (asm; DS port F12: particle, bias, out) */
    __typeof__(func_800D12C4) *func_800D12C4; /* 0x00C */
    __typeof__(btl_setVsyncMode) *btl_setVsyncMode; /* 0x010 */
    __typeof__(func_800316D8) *func_800316D8; /* 0x014 */
    __typeof__(Hi_RegisterGouEffModel) *Hi_RegisterGouEffModel; /* 0x018 */
    __typeof__(SetDrawMove) *SetDrawMove; /* 0x01C */
    __typeof__(ScaleMatrix) *ScaleMatrix; /* 0x020 */
    __typeof__(CompMatrix) *CompMatrix; /* 0x024 */
    __typeof__(SetRotMatrix) *SetRotMatrix; /* 0x028 */
    void (*Hi_StopSummonTexAnim)(s32, s32); /* 0x02C */
    void (*Hi_StartSummonTexAnim)(s32, s32, u8); /* 0x030 */
    __typeof__(SetTransMatrix) *SetTransMatrix; /* 0x034 */
    __typeof__(math_cos) *math_cos; /* 0x038 */
    __typeof__(math_sin) *math_sin; /* 0x03C */
    __typeof__(ratan2) *ratan2; /* 0x040 */
    s32 (*func_80030874)(); /* 0x044 (main exe, no symbol yet) */
    __typeof__(LoadAverageShort12) *LoadAverageShort12; /* 0x048 */
    __typeof__(Hi_RegisterTexListModel) *Hi_RegisterTexListModel; /* 0x04C */
    __typeof__(printf) *printf; /* 0x050 */
    u16 (*Hi_RegisterSolidEffModel)(BtlUnit9C8 *); /* 0x054 */
    u16 (*Hi_RegisterTexEffModel)(BtlUnit9C8 *, s16, s16, s32, s32); /* 0x058 */
    __typeof__(Hi_RegisterSummonModel) *Hi_RegisterSummonModel; /* 0x05C */
    void (*Hi_DrawEffModel)(SVECTOR *, VECTOR *, s32, s32); /* 0x060 */
    s32 (*Hi_DrawSummonModel)(SVECTOR *, VECTOR *, s32, s32, s32); /* 0x064 */
    void (*Hi_SetSummonMotion)(BtlUnit9CC *, s32); /* 0x068 */
    void (*func_800E8854)(BtlUnit9C8 *, void (*)(SVECTOR *, s32)); /* 0x06C */
    void (*btlfx_initEffModels)(BtlModel *, s32); /* 0x070 */
    __typeof__(btlfx_initSummonModels) *btlfx_initSummonModels; /* 0x074 */
    __typeof__(btlfx_clearEffModels) *btlfx_clearEffModels; /* 0x078 */
    __typeof__(btlfx_clearSummonModels) *btlfx_clearSummonModels; /* 0x07C */
    void (*func_800DC818)(s32, s32, u16 *); /* 0x080 */
    void (*func_800DC858)(s32, s32, u16 *); /* 0x084 */
    __typeof__(btlfx_setUnitMotion) *btlfx_setUnitMotion; /* 0x088 */
    __typeof__(btlfx_getUnitMotion) *btlfx_getUnitMotion; /* 0x08C */
    s32 (*func_800D8C9C)(void); /* 0x090 */
    s32 (*func_800D8CA4)(void); /* 0x094 */
    void (*btlfx_setMotionFrame)(s32, s32); /* 0x098 */
    u8 (*btlfx_getMotionFrame)(s32); /* 0x09C */
    __typeof__(btlfx_playUnitMotion) *btlfx_playUnitMotion; /* 0x0A0 */
    void (*func_800D8CAC)(void); /* 0x0A4 */
    __typeof__(btlfx_resetUnitMotion) *btlfx_resetUnitMotion; /* 0x0A8 */
    __typeof__(btlfx_align4) *btlfx_align4; /* 0x0AC */
    __typeof__(btlfx_sqrt) *btlfx_sqrt; /* 0x0B0 */
    __typeof__(btlfx_lerp) *btlfx_lerp; /* 0x0B4 */
    __typeof__(btlfx_lerpRatio) *btlfx_lerpRatio; /* 0x0B8 */
    __typeof__(btlfx_lerpVec) *btlfx_lerpVec; /* 0x0BC */
    __typeof__(btlfx_rand) *btlfx_rand; /* 0x0C0 */
    __typeof__(btlfx_randRange) *btlfx_randRange; /* 0x0C4 */
    __typeof__(btlfx_randCentered) *btlfx_randCentered; /* 0x0C8 */
    __typeof__(btlfx_distance) *btlfx_distance; /* 0x0CC */
    __typeof__(btlfx_randRotation) *btlfx_randRotation; /* 0x0D0 */
    __typeof__(btlfx_aimAngles) *btlfx_aimAngles; /* 0x0D4 */
    void (*func_800CFCA8)(SVECTOR *, s32, s32); /* 0x0D8 */
    __typeof__(func_800CFD78) *func_800CFD78; /* 0x0DC */
    __typeof__(btlfx_angleDiff) *btlfx_angleDiff; /* 0x0E0 */
    void (*btlfx_turnToward)(s16 *, s16 *, s32); /* 0x0E4 */
    void (*func_800D0130)(SVECTOR *, MATRIX *); /* 0x0E8 */
    void (*func_800D01D4)(SVECTOR *, VECTOR *, MATRIX *, s32); /* 0x0EC */
    void (*func_800D044C)(SVECTOR *, s32, s32, XY32 *, s32, s32); /* 0x0F0 */
    void (*func_800D0580)(s16 *, XY32 *, s32); /* 0x0F4 */
    void (*func_800D0674)(SVECTOR *, SVECTOR *, SVECTOR *, s32, s32, s32 *); /* 0x0F8 */
    __typeof__(btlfx_allocDrMode) *btlfx_allocDrMode; /* 0x0FC */
    __typeof__(btlfx_drawScreenTint) *btlfx_drawScreenTint; /* 0x100 */
    void (*Hi_ModifySummonModelRGB)(s32, s16, s16, s16); /* 0x104 */
    __typeof__(func_800D0E14) *func_800D0E14; /* 0x108 */
    __typeof__(func_800D0FFC) *func_800D0FFC; /* 0x10C */
    void (*func_800D1494)(u8 *, s32, s32 *, s32, s32, s32, s32, u8 *, s32); /* 0x110 */
    void (*func_800D1680)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, s32, s32); /* 0x114 */
    __typeof__(func_800D198C) *func_800D198C; /* 0x118 */
    void (*func_800D1BC4)(u32, u8 *, s32, s32, s32, s32, s32); /* 0x11C */
    __typeof__(func_800D1D74) *func_800D1D74; /* 0x120 */
    __typeof__(func_800D1FFC) *func_800D1FFC; /* 0x124 */
    void (*func_800D23D8)(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32); /* 0x128 */
    __typeof__(func_800D2938) *func_800D2938; /* 0x12C */
    __typeof__(func_800D2DBC) *func_800D2DBC; /* 0x130 */
    void (*func_800D3030)(BtlBgLayers *, s32); /* 0x134 */
    void (*func_800D3B6C)(u8 *, SVECTOR *, s32, s32); /* 0x138 */
    __typeof__(func_800E00DC) *func_800E00DC; /* 0x13C */
    s32 (*func_800E0134)(Pool30Entry *, Pool6CEntry *); /* 0x140 */
    __typeof__(func_800E0168) *func_800E0168; /* 0x144 */
    __typeof__(func_800E14D8) *func_800E14D8; /* 0x148 */
    __typeof__(func_800E1534) *func_800E1534; /* 0x14C */
    __typeof__(func_800E1708) *func_800E1708; /* 0x150 */
    __typeof__(func_800E1810) *func_800E1810; /* 0x154 */
    __typeof__(func_800E1874) *func_800E1874; /* 0x158 */
    Pool30Entry * (*func_800E1F90)(Pool30 *); /* 0x15C */
    __typeof__(func_800E2000) *func_800E2000; /* 0x160 */
    __typeof__(func_800E2F84) *func_800E2F84; /* 0x164 */
    __typeof__(btlfx_initTasks) *btlfx_initTasks; /* 0x168 */
    __typeof__(btlfx_allocTaskMem) *btlfx_allocTaskMem; /* 0x16C */
    __typeof__(btlfx_spawnTask) *btlfx_spawnTask; /* 0x170 */
    s32 (*btlfx_updateTasks)(void); /* 0x174 */
    __typeof__(btlfx_poolSize) *btlfx_poolSize; /* 0x178 */
    __typeof__(btlfx_poolInit) *btlfx_poolInit; /* 0x17C */
    void (*btlfx_poolClear)(Unk800DA1B8 *); /* 0x180 */
    void * (*btlfx_poolSpawn)(s16 *); /* 0x184 */
    __typeof__(btlfx_poolUpdate) *btlfx_poolUpdate; /* 0x188 */
    __typeof__(btlfx_findFileSlot) *btlfx_findFileSlot; /* 0x18C */
    void (*Hi_SetSummonMotFrame)(s32, s32); /* 0x190 */
    void (*func_800E6D58)(void); /* 0x194 */
    __typeof__(btlfx_getFileData) *btlfx_getFileData; /* 0x198 */
    s32 (*btlfx_getFileDataSize)(s32, s32); /* 0x19C */
    __typeof__(func_800DA4E8) *func_800DA4E8; /* 0x1A0 */
    __typeof__(btlfx_loadFile) *btlfx_loadFile; /* 0x1A4 */
    __typeof__(func_800DA5C4) *func_800DA5C4; /* 0x1A8 */
    void (*btlfx_updateLoader)(void); /* 0x1AC */
    __typeof__(btlfx_reset) *btlfx_reset; /* 0x1B0 */
    s32 (*btlfx_start)(s32); /* 0x1B4 */
    void (*btlfx_runScript)(void); /* 0x1B8 */
    __typeof__(btlfx_heapInit) *btlfx_heapInit; /* 0x1BC */
    __typeof__(btlfx_heapAlloc) *btlfx_heapAlloc; /* 0x1C0 */
    s32 (*btlfx_heapWrapIfFull)(Heap *, s32); /* 0x1C4 */
    __typeof__(btlfx_heapFree) *btlfx_heapFree; /* 0x1C8 */
    __typeof__(func_800E3C58) *func_800E3C58; /* 0x1CC */
    void (*func_800E3C98)(void); /* 0x1D0 */
    __typeof__(func_800E3CEC) *func_800E3CEC; /* 0x1D4 */
    void (*func_800E3D20)(s32, s32); /* 0x1D8 */
    void (*func_800E3D54)(Pool6CEntry *); /* 0x1DC */
    void (*btlfx_setTargets)(BtlUnitList *); /* 0x1E0 */
    void (*func_800E6DF4)(void); /* 0x1E4 */
    void (*func_800E6DB8)(s32); /* 0x1E8 */
    __typeof__(btlfx_clearTargets) *btlfx_clearTargets; /* 0x1EC */
    s32 (*btlfx_getUnitRadius)(s32); /* 0x1F0 */
    void (*func_800E738C)(s32); /* 0x1F4 */
    __typeof__(btlfx_getUnitHomePos) *btlfx_getUnitHomePos; /* 0x1F8 */
    __typeof__(btlfx_getUnitPos) *btlfx_getUnitPos; /* 0x1FC */
    __typeof__(btlfx_getUnitBodyPos) *btlfx_getUnitBodyPos; /* 0x200 */
    __typeof__(btlfx_setUnitPos) *btlfx_setUnitPos; /* 0x204 */
    s32 (*func_800E82CC)(s32, s16 *, BtlUnit **, s32 *); /* 0x208 */
    __typeof__(btlfx_getUnitRot) *btlfx_getUnitRot; /* 0x20C */
    __typeof__(btlfx_isEnemyUnit) *btlfx_isEnemyUnit; /* 0x210 */
    __typeof__(func_800E78B4) *func_800E78B4; /* 0x214 */
    void (*btlfx_getStrikePos)(s32, s32, s16 *); /* 0x218 */
    __typeof__(func_800E7C54) *func_800E7C54; /* 0x21C */
    __typeof__(func_800E7CC4) *func_800E7CC4; /* 0x220 */
    __typeof__(btlfx_fadeUnitGroup) *btlfx_fadeUnitGroup; /* 0x224 */
    __typeof__(btlfx_execCmd) *btlfx_execCmd; /* 0x228 */
    void (*btlfx_addPrimAbr)(s32, s32, void *, s32); /* 0x22C */
    __typeof__(btlfx_showPopups) *btlfx_showPopups; /* 0x230 */
    __typeof__(btlfx_addPrim) *btlfx_addPrim; /* 0x234 */
    __typeof__(func_800E651C) *func_800E651C; /* 0x238 */
    __typeof__(btlfx_linkPrimAbr) *btlfx_linkPrimAbr; /* 0x23C */
    void (*func_800E6624)(s32, s32, s32, s32, s32, s32, s32); /* 0x240 */
    void (*Hi_DrawMorphModelByBone)(s32, s32, s32, s32, s32, s32); /* 0x244 */
    void (*func_800E6764)(u32, s32, s32, s32, s32, s32, s32, s32, s32, SVECTOR *, VECTOR *, CVECTOR *, s32,
                          void (*)(SVECTOR *)); /* 0x248 (arg 13: per-vertex callback in the module) */
    void (*Hi_ModifySummonModelAbr)(s32, s32); /* 0x24C */
    void (*func_800E6C58)(SVECTOR *); /* 0x250 */
    void (*Hi_GetSummonBonePos)(s32, s32, u16 *); /* 0x254 */
    s32 (*func_800DA4C4)(s32); /* 0x258 */
    void (*Hi_FreeEffModel)(s32); /* 0x25C */
    void (*btlfx_setWeaponVisible)(s32, s32); /* 0x260 */
    void (*func_800D0B18)(s32, s32, s32, s32, s32, s32, CVECTOR *, s32, s32, s32, s32); /* 0x264 */
    void (*Hi_ModifyEffModelAbr)(s32, s32); /* 0x268 */
    void (*Hi_ModifyEffModelRGB)(s32, s16, s16, s16); /* 0x26C */
    __typeof__(btlfx_turnUnit) *btlfx_turnUnit; /* 0x270 */
    void (*Hi_ShowSummonModelMesh)(s32, s32); /* 0x274 */
    void (*Hi_HideSummonModelMesh)(s32, s32); /* 0x278 */
    __typeof__(MulRotMatrix) *MulRotMatrix; /* 0x27C */
    __typeof__(func_800D8A20) *func_800D8A20; /* 0x280 */
    __typeof__(func_800D89E8) *func_800D89E8; /* 0x284 */
    void (*Hi_DrawEffModelByBone)(s32, s32, s32, s32); /* 0x288 */
    void (*Hi_DrawMorphEffModel)(SVECTOR *, VECTOR *, s32, s32, s32, s32); /* 0x28C */
    __typeof__(Hi_GetSummonBoneMatrix) *Hi_GetSummonBoneMatrix; /* 0x290 */
    s32 (*func_80013228)(); /* 0x294 (main exe, no symbol yet) */
    __typeof__(MoveImage) *MoveImage; /* 0x298 */
    void (*func_800DF938)(void); /* 0x29C */
    __typeof__(btlfx_getUnitScale) *btlfx_getUnitScale; /* 0x2A0 */
    void (*func_800E7ACC)(s32, s32 *); /* 0x2A4 */
    __typeof__(btlfx_setUnitRot) *btlfx_setUnitRot; /* 0x2A8 */
    u16 (*Hi_RegisterTexPtrModel)(BtlUnit9C8 *, s16 *, s16 *); /* 0x2AC */
    void (*func_800CFE88)(MATRIX *, s32, s32); /* 0x2B0 */
    void (*func_800CFF40)(MATRIX *, s16 *); /* 0x2B4 */
    __typeof__(func_800E7D3C) *func_800E7D3C; /* 0x2B8 */
    void (*func_800E806C)(s32, s32); /* 0x2BC */
    __typeof__(func_800E8B28) *func_800E8B28; /* 0x2C0 */
    void (*func_800DC8AC)(s32, s32, BtlBone *); /* 0x2C4 */
    void (*func_800DC8EC)(s32, s32, BtlBone *); /* 0x2C8 */
    void (*func_800E63E4)(s32, s32); /* 0x2CC */
    void (*func_800E98C8)(s32, s32); /* 0x2D0 */
    void (*func_800E98F8)(s32); /* 0x2D4 */
    __typeof__(func_800E9930) *func_800E9930; /* 0x2D8 */
    s32 (*func_800E8554)(s32, s32); /* 0x2DC */
    s32 * (*func_800E871C)(s32, s32 *); /* 0x2E0 */
    void (*Hi_SetEffModelOffset)(s32, s16); /* 0x2E4 */
    void (*func_800E80CC)(s32, s32); /* 0x2E8 */
    void (*btlfx_setUnitVisible)(s32, s32); /* 0x2EC */
    void (*func_800E8980)(s32); /* 0x2F0 */
    void (*func_800E6E1C)(s32, s32, s32, s32, u32 *, s32, s32, s32); /* 0x2F4 */
    __typeof__(btl_getUnit) *btl_getUnit; /* 0x2F8 */
    s32 (*Hi_GetSplitMdlVertex)(s32, s32); /* 0x2FC */
    s32 (*func_800DF928)(void); /* 0x300 */
    void (*Hi_SplitMdlVertex)(s32, s32, s32); /* 0x304 */
    void (*func_800DF920)(void); /* 0x308 */
    s32 (*func_800DF930)(void); /* 0x30C */
    void (*Hi_GetMdlVertexPtr)(s32, u16, s32, s32 *, s32); /* 0x310 */
    void (*func_800DD0B0)(s32); /* 0x314 */
    void (*func_800DD0DC)(s32); /* 0x318 */
    void (*Hi_DrawSliceEffModel)(SVECTOR *, VECTOR *, s32, s32); /* 0x31C */
    void (*Hi_SetEffModelSlice)(s32, s16); /* 0x320 */
    __typeof__(btlfx_callScript) *btlfx_callScript; /* 0x324 */
    __typeof__(btlfx_getMotionLength) *btlfx_getMotionLength; /* 0x328 */
    s32 (*btlfx_isUnitActive)(s32); /* 0x32C */
    s32 (*func_800CFC2C)(s32, s32, s32, s32, s32, s32 *); /* 0x330 */
    __typeof__(func_800E60A4) *func_800E60A4; /* 0x334 */
    void (*func_800E8F50)(Unk800E8F50 *, s32); /* 0x338 */
    __typeof__(func_800E865C) *func_800E865C; /* 0x33C */
    void (*func_800E9000)(Unk800E9000 *, s32 *, s32, Unk80100DC054 *); /* 0x340 */
    __typeof__(func_800DA898) *func_800DA898; /* 0x344 */
    void (*func_800E99B0)(s32, s32, s32, s32); /* 0x348 */
    void (*func_800E83EC)(void); /* 0x34C */
    void (*func_800E9884)(s32); /* 0x350 */
    void (*func_800E8274)(s32); /* 0x354 */
    void (*func_800E87C4)(s32); /* 0x358 */
    void (*func_800DD120)(BtlTexInfo *, BtlTexInfo *, s32); /* 0x35C */
} BtlfxExports; /* followed in ovl_0b by the g_btlfxEnv block (D_800F7870) */

#define BTLFX_EXPORTS(env) ((BtlfxExports *)(env)->unk10)

/* Module header, pointed to by the module's first word (its first .rodata word).
 * btlfx_updateLoader installs &ops[0] as the slot's op table (D_80100C70) and calls init when non-NULL
 * (NULL in every module on the discs); script op 0x80 + i runs ops[i] (btlfx_spawnTask). */
typedef struct BtlfxModHeader {
    s32 unk0; /* 0 in every module */
    void (*init)(s32 data, Unk800F78A4 *env, s32 exports);
    EffFn ops[1]; /* as many as the module uses (up to 8 on the discs) */
} BtlfxModHeader;

#endif

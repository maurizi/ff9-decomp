#ifndef FF9_H
#define FF9_H

#include "common.h"
#include "psyq.h"
#include "ff9_ids.h"
#include "snd_ids.h"
#include "res_ids.h"

/* An argument the original code never passed: the callee reads an argument register that still holds this
 * value (usually the caller's own parameter), which only works on MIPS. Empty here, so the call compiles as
 * in the original (passing it explicitly changes register allocation); a port defines it as `x`.
 * tools/arg_check.py finds these. */
#ifndef REG_PASSTHROUGH
#define REG_PASSTHROUGH(x)
#endif

/* A buffer at a fixed PS1 RAM address. Square's code loads these addresses as constants (lui/ori), which a
 * linker-placed symbol can't reproduce (it assembles to lui/addiu %hi/%lo), so every one goes through this
 * macro instead of a raw cast: a port redefines it to map them all to its own memory. tools/raw_addr.py
 * lists any address literal that bypasses it. */
#ifndef PSX_RAM
#define PSX_RAM(type, addr) ((type)(addr))
#endif

/* Pins a local to a MIPS register (`register T *v REG_ASM("$8") = ...;`) where register allocation alone keeps a
 * function from matching. No effect on behaviour; a port defines it empty. */
#ifndef REG_ASM
#define REG_ASM(r) asm(r)
#endif

/* Is this offset-or-pointer field still a file offset, i.e. not yet relocated in place? Relocated values are
 * KSEG0 pointers (0x80xxxxxx, negative as s32). A port whose pointers are positive defines its own. */
#ifndef IS_OFFSET
#define IS_OFFSET(x) ((s32)(x) >= 0)
#endif

/* Layouts are partial: only offsets seen in decompiled code are named.
 * Names are placeholders (unkXX = offset) until their purpose is known.
 * Split into per-subsystem headers once subsystems are identified. */

/* ---- game state (g_gameState) ---- */

typedef struct {
    u16 flags; /* PRELOAD_* */
    u16 kind; /* PRELOAD_KIND_* */
    s32 fileId; /* id, -1 = none */
    u32 dest; /* RAM address the file is read to */
    s32 loaded; /* bytes read so far */
    s32 bufSize; /* room at dest (sector multiple) */
    s32 chunkSize; /* most bytes per read */
    s32 sector; /* CD sector of the next read (relative to GameState.imgSector) */
    s32 readSize; /* bytes of the current read */
    s16 vramX; /* VRAM rect the data was moved to (preload_setVramRect; game_loadModeOverlay StoreImage) */
    s16 vramY;
    s16 vramW;
    s16 vramH;
} PreloadEntry; /* element passed to preload_clearEntry */

/* PreloadEntry.flags */
#define PRELOAD_BUSY 0x1 /* a read or seek was issued (preload_startRead); preload_clearBusy clears it */
#define PRELOAD_READ_OK 0x2 /* a read completed (preload_onReadDone) */
#define PRELOAD_IN_VRAM 0x20 /* the data was moved to the VRAM rect (preload_setVramRect) */
#define PRELOAD_STARTED 0x40 /* picked by preload_next */
#define PRELOAD_READ_FAILED 0x80 /* preload_onReadDone got an error status */
/* PreloadEntry.kind (preload_setEntry's second argument) */
#define PRELOAD_KIND_READ 0 /* read into dest */
#define PRELOAD_KIND_SEEK 1 /* only seek to the sector */
/* PreloadQueue.state (preload_update) */
#define PRELOAD_Q_IDLE 0 /* nothing left to read */
#define PRELOAD_Q_NEXT_ENTRY 4 /* pick the next entry (preload_next) */
#define PRELOAD_Q_NEXT_CHUNK 5 /* entry done? else build the next chunk in entries[6] */
#define PRELOAD_Q_WAIT_CD 8 /* wait until cd_getState is idle */
#define PRELOAD_Q_START_READ 9 /* preload_startRead */
#define PRELOAD_Q_READING 10 /* preload_poll until the chunk is done */

typedef struct PreloadQueue {
    u8 unk0; /* write-only (preload_init clears it) */
    u8 state; /* state */
    s8 count; /* count */
    s8 cur;
    PreloadEntry entries[7]; /* [6] is the read in progress (preload_startRead) */
    s32 (*hook)(struct PreloadQueue *); /* hook, nonzero = handled (preload_setHook; fld_preloadHook) */
} PreloadQueue;
PreloadEntry *preload_makeChunk(PreloadEntry *, PreloadEntry *);
s32 preload_startRead(void);
void preload_setVramRect(PreloadEntry *, s16, s16, s16, s16);

typedef struct {
    u16 codeBase; /* (second table) high byte: first two-byte code; subtracted from a code to index the glyphs (text_buildPlain/text_measure, kanji table) */
    u8 pad2[2];
    u16 extCode; /* high byte: first text code that takes an extra byte (text_draw: (c << 8) + next byte - extCode + extBase) */
    u16 extBase; /* glyph index of the first two-byte code */
    s16 unk8; /* copied to g_msgFontUnusedParam by msg_init, not read otherwise */
    u8 padA[2];
    u32 glyphs[1]; /* glyph info, width in the top 4 bits */
} Font;

/* Character.info: packed ids, also read as a whole word */
typedef union {
    u32 w;
    struct {
        u32 id : 4;    /* character id (index into GameData.unk3C) */
        u32 kind : 5;  /* model/appearance kind (chr_calcAppearance; btlfx_getUnitKind, battle model and motion tables; 2 = Sword Magic user); index into g_portraitByKind */
        u32 battleRow : 1; /* = the battle unit's row (flags >> 8 & 3, btl_toggleRow); toggled by the menu's Order; event op 0x62 */
        u32 victoryPose : 1; /* runtime: with it the character strikes its victory pose at the end of a won battle (btl_update), without it stays in its idle stance; event op 0xDB sets it */
        u32 recruited : 1; /* chr_setRecruited / chr_clearRecruited; members counted by chr_join's level average, shops, config */
        u32 row : 5;   /* ability table row (g_abilityRows) */
        u32 unk11 : 15; /* not used by C */
    } b;
} CharBits;
/* CharBits.w masks, for the code that reads the word */
#define CHR_INFO_ID 0xF
#define CHR_INFO_KIND_SHIFT 4
#define CHR_INFO_KIND 0x1F0
#define CHR_INFO_BATTLE_ROW_BIT 9
#define CHR_INFO_BATTLE_ROW (1 << CHR_INFO_BATTLE_ROW_BIT)
#define CHR_INFO_VICTORY_POSE_BIT 10
#define CHR_INFO_VICTORY_POSE (1 << CHR_INFO_VICTORY_POSE_BIT)
#define CHR_INFO_RECRUITED_BIT 11
#define CHR_INFO_ROW_SHIFT 12
#define CHR_INFO_ROW 0x1F000

/* Character record: GameData.characters[9]; GameData.party[] points at the party members. Field names checked
 * against the main menu at runtime (docs/runtime-findings.md). */
typedef struct {
    u8 name[0xA]; /* 0xFF-terminated text, read up to 7 chars */
    u8 classFlags; /* chr_getClassFlags(row): 0x2 checked by Protect Girls (btl_findCover) and ability 0x61, 0x8 by ability 0x62, 0x10 picks the alternate default set / appearance */
    u8 level;
    u32 exp;      /* capped at 9999999 */
    u16 hp;
    s16 mp;
    u8 pad14[0x16 - 0x14];
    u8 unk16; /* write-only (chr_setLevel sets 10) */
    u8 stones;    /* magic stones left */
    u16 maxHp;
    s16 maxMp;
    s16 maxAtb;   /* runtime: where the battle ATB gauge stops */
    u8 pad1E;
    u8 maxStones;
    u8 trance;    /* gauge 0-255; runtime: 121 with the status bar about half full */
    u8 weaponBone; /* bone the weapon model hangs from (btl_initWeapon, btlfx_getWeaponPos) */
    u8 pad22[0x24 - 0x22];
    /* as the Status screen shows them (runtime, Zidane Lv12: 24 26 22 25 / 9 10 10 7) */
    u8 speed;
    u8 strength;
    u8 magic;
    u8 spirit;
    u8 defense;
    u8 evade;
    u8 magicDef;
    u8 magicEva;
    u16 baseMaxHp; /* chr_calcMaxHp, before equipment (chr_setLevel; chr_updateStats input) */
    u16 baseMaxMp; /* chr_calcMaxMp */
    /* before equipment bonuses (runtime: 24 25 22 24 vs 24 26 22 25 shown) */
    u8 baseSpeed;
    u8 baseStrength;
    u8 baseMagic;
    u8 baseSpirit;
    CharBits info;
    u8 status;   /* persistent status bits shown by ui_drawStatusIcons (runtime); 0x45 = dead/stone/... (can't gain exp) */
    u8 equip[5]; /* item ids: weapon, head, arm, body, accessory (0xFF = none); runtime: Zidane 3 115 90 152 255 with no accessory */
    u16 equipBonus[5]; /* stat bonuses from equipment (chr_sumEquipBonus), >> 5 added to speed/strength/magic/spirit/stones */
    u8 pad48[0x58 - 0x48];
    u8 ap[0x30]; /* AP per ability slot of the character's g_abilityRows row (chr_getAp, chr_clearAp) */
    u32 supportAbilities[2]; /* equipped: bit (id - 0xC0) (runtime, chr_setSupportAbility) */
} Character; /* 0x90 */
/* Character.classFlags (chr_getClassFlags: g_chrClassFlags, main data 0x8006CACC = 09 05 06 05 06 05 06 05 15 15 15 16 by
 * stat class (CHR_STATS_*, from the ability row); bits 0x4/0x8 left unnamed: 0x8 only for class 0,
 * ability formula 0x62 misses it, 0x4 on every other class;
 * formula 0x62 is in no ability or enemy record, so neither was observed) */
#define CHR_CLASS_MALE 0x1 /* data only, medium: set on every class without CHR_CLASS_FEMALE (Quina included); no code
                            * tests it (battle AI scripts can read classFlags through btl_attr 0x43) */
#define CHR_CLASS_FEMALE 0x2 /* Protect Girls covers it (btl_findCover); ability 0x61 misses it; classes 2, 4, 6 and 11 (Beatrix) */
#define CHR_CLASS_GUEST 0x10 /* classes 8 and up (CHR_STATS_*): no ability learning (chr_canLearnAbilities), no trance (btl_initPartyUnit),
                             * the alternate default set and appearance (chr_getDefaultSet, chr_calcAppearance) */

/* GameData.npcModels entry */
typedef struct ModelSlot {
    s32 flags; /* MODEL_SLOT_*, others from event op args (the field's FLD_OBJ_* bits) */
    s32 modelId; /* model item, 0x20000 | model id (DB_MODEL), -1 after game_clearModelSlot (runtime) */
    struct BtlModel *model; /* the model object (GameData.chrModelObjs / npcModelObjs entry; battle BtlModel) */
    struct BtlUnitA0 *actor; /* the EvtObject using the slot (FieldMap.playerObj's actor) */
    struct BtlUnit *unit; /* battle unit (btl_resetUnit) */
} ModelSlot;
/* ModelSlot.flags. ModelSlot is the same record as ovl_0a's FieldObjSlot, ovl_0c's WmObjSlot and ovl_0b's
 * BtlObj / Unk8E0Ld (runtime: one read breakpoint on a slot's flags hits fld_drawModels, fld_drawShadow,
 * fld_applyLights...). Those views keep the field names (flags, modelId, model, actor) and only differ in the
 * pointer types (each mode has its own model object type); FLD_OBJ_LOADED / FLD_OBJ_NO_SHADOW and
 * WM_SLOT_LOADED / WM_SLOT_NO_SHADOW are defined as these. */
#define MODEL_SLOT_LOADED    0x1  /* the slot has a model (set when it is built, cleared by fld_exit / btl_cleanup / wm exit) */
#define MODEL_SLOT_NO_SHADOW 0x10 /* no shadow drawn (EVT_OP_JUMP sets it for the jump) */
#define MODEL_SLOT_SHADOW_FIXED 0x20 /* the shadow uses the fixed placement (EVT_OP_SHADOW_FIXED / EVT_OP_SHADOW_UNFIX; ovl_0a FLD_OBJ_SHADOW_FIXED) */

/* main's and ovl_65's view of ovl_0b BtlFiles (Battle.files) */
typedef struct {
    u8 pad0[4];
    s32 data1; /* BtlFiles.data[1]: the battle-end db ovl_65 registers at g_modeHeap (battleEnd_loadDb / battleEnd_unloadDb) */
} Unk1354;

/* view of ovl_0b Battle11C0 (Battle.work), the fields main and ovl_65 read */
typedef struct {
    u8 pad0[4];
    s16 returnId; /* Battle11C0.returnId; game_setNextScene sets it in battle */
    u8 returnMode; /* Battle11C0.returnMode: GameState.mode battleEnd_main goes back to (1 = field) */
    u8 pad7[0x10 - 7];
    s32 eventScript; /* event script battleEnd_init runs (evt_init) when g_btlRewards.hasPostEvent; btl_initMode clears it */
    s32 texts; /* Battle11C0.texts: message table in battle (msg_init); battleEnd_init calls msg_init when it is set */
    u8 pad18[4];
    u8 *bgData; /* Battle11C0.bgData: battleEnd_preloadScene reads the next mode's overlay here */
    u8 pad20[0x28 - 0x20];
    struct Unk11C0Slot *shadows; /* Battle11C0.shadows, indexed by EvtObject.modelSlot (EVT_OP_SHADOW_*) */
} Unk11C0;

/* main's view of ovl_0c WorldMapObj (WorldMap.obj) */
typedef struct {
    u8 pad0[0x14];
    struct Unk8007B954 *areaData; /* WorldMapObj.areaData, the message table on the world map (msg_init) */
    u8 pad18[0x1C - 0x18];
    struct Unk11C0Slot *shadows; /* WorldMapObj.shadows (WmObjEnt, the same 0x28-byte record), by EvtObject.modelSlot */
    u8 pad20[0x24 - 0x20];
    s16 sceneId; /* WorldMapObj.sceneId; game_setNextScene sets it on the world map */
} WmObjView;

/* per-object shadow (0x28 bytes: Battle11C0.shadows, WorldMapObj.shadows, FieldScene.shadows), built by
 * shadow_initSlots, set by the EVT_OP_SHADOW_* ops in evt_modelOp* */
typedef struct Unk11C0Slot {
    u32 unk0_0 : 8;
    u32 sizeX : 12; /* EVT_OP_SHADOW_SIZE x * 14 (0xE0 from shadow_initSlots) */
    u32 sizeZ : 12; /* EVT_OP_SHADOW_SIZE z * 12 (0xC0) */
    u32 fixedAngle : 12; /* EVT_OP_SHADOW_FIXED angle * 16, used with MODEL_SLOT_SHADOW_FIXED */
    u32 ofsX : 10; /* EVT_OP_SHADOW_OFFSET x */
    u32 ofsZ : 10; /* EVT_OP_SHADOW_OFFSET z */
    POLY_FT4 *prims[2][4]; /* shadow quads per draw buffer, filled by shadow_initSlots (ovl_0c WmObjEnt.prims) */
} Unk11C0Slot;

/* main's view of ovl_0a FieldScene (FieldMapObj.scene) */
typedef struct {
    u8 pad0[0xA];
    s16 nextId; /* FieldScene.nextId; game_setNextScene sets it in a field */
    u8 padC[0x2C - 0xC];
    struct Unk11C0Slot *shadows; /* FieldScene.shadows, by EvtObject.modelSlot */
} Unk8E4C14;

/* main's view of ovl_0a FieldMapObj (FieldMap.area) */
typedef struct {
    u8 pad0[8];
    struct Unk8007B954 *msgTable; /* FieldMapObj.msgTable (msg_init) */
    u8 padC[0x14 - 0xC];
    Unk8E4C14 *scene; /* FieldMapObj.scene */
} Unk8E4C;

/* GameData.ctx as main and ovl_65 see it: the offsets of ovl_0a FieldMap, ovl_0c WorldMap and ovl_0b Battle
 * that are read through it */
typedef struct {
    u32 flags; /* the mode's flags word: FieldMap.flags (FLD_MAP_NO_PRELOAD from menu_doOpen; ovl_64 sets
                * FLD_MAP_NO_WINDOWS | FLD_MAP_NO_BGI_ANIM), WorldMap.flags, Battle.flags (ovl_0b btl_endFrame tests BTL_FLAG_NO_DRAW) */
    u8 pad4[4];
    WmObjView *worldObj; /* WorldMap.obj */
    Unk8E4C *fieldArea; /* FieldMap.area */
    u8 pad10[0xD80 - 0x10];
    u8 phase; /* ovl_0b Battle.phase (EVT_SYSVAR_BATTLE_PHASE) */
    u8 padD81[0x11C0 - 0xD81];
    Unk11C0 *work; /* ovl_0b Battle.work */
    u8 pad11C4[0x1340 - 0x11C4];
    u8 groupNo; /* ovl_0b Battle.groupNo (EVT_SYSVAR_BATTLE_GROUP) */
    u8 pad1341[0x1354 - 0x1341];
    Unk1354 *files; /* ovl_0b Battle.files */
} Unk8E4;
/* Unk8E4.flags in field mode (FieldMap.flags; the other FLD_MAP_* bits are in ovl_0a.h) */
#define FLD_MAP_NO_WINDOWS 0x1 /* fld_main skips game_updateWindows (win_updateAll/win_drawAll) */
#define FLD_MAP_NO_BGI_ANIM 0x8 /* fld_main skips bgi_updateAnims/bgi_updateFloorMoves */
#define FLD_MAP_NO_PRELOAD 0x1000 /* fld_main skips preload_update (fld_movieStart, main menu_doOpen) */

/* GameData.worldCamera, the world map camera (ovl_0c g_worldCamera); ovl_39 saves it as a block */
typedef struct {
    s16 stickSide;
    s16 viewBlend;
    s32 viewBlendSpeed;
    s32 viewForceNormal;
    u16 yaw; /* camera angle */
    u16 yawOffset;
} WmCameraState;

/* GameData.ctx: per-module context; its layout depends on the loaded
 * overlay. */
struct WorldMap; /* ovl_0c, include/ovl_0c.h */
struct FieldMap; /* ovl_0a, include/ovl_0a.h */
struct Battle;   /* ovl_0b, include/ovl_0b.h */
typedef union {
    Unk8E4 *ovl65;
    struct WorldMap *world;
    struct FieldMap *field;
    struct Battle *battle;
} Unk8E4Ptr;


/* 0x98-byte per-object record (GameData.chrModelObjs / unk8DC) */
typedef struct Unk8D4 {
    u8 pad0[0x98];
} Unk8D4;

typedef struct CdReadCtx {
    s32 sector; /* start sector of the file (relative to GameState.imgSector) */
    s32 size; /* total bytes of the file */
    u8 *dest; /* destination of this chunk */
    s32 offset; /* byte offset in the file this chunk starts at */
    s32 chunk; /* chunk size, -1 = all in, bytes read out (cdRead_update) */
    void (*onDone)(struct CdReadCtx *); /* done callback */
    s32 unk18; /* write-only: cdRead_init stores the total size here too */
    s32 state; /* CDREAD_* (cdRead_update) */
    u8 *end; /* dest + bytes read */
} CdReadCtx; /* GameData.cdRead */
#define CDREAD_DONE 0
#define CDREAD_WAIT_CD 1 /* until cd_getState is idle */
#define CDREAD_START 2 /* cd_readAsync the next chunk */
#define CDREAD_READING 3 /* until cd_getState is idle, then onDone */

/* runtime: menu_items shows Potion 46, Hi-Potion 10, Ether 12, Phoenix Down 14, ... = {236,46} {237,10} {238,12} {240,14} */
typedef struct {
    u8 id;
    u8 count;
} ItemSlot;
typedef ItemSlot UnkPair; /* old name, still used by ovl_0b */

/* GameData.items: item inventory, copied as a block by the save code (ovl_39) */
typedef struct {
    ItemSlot slots[0x100];
} __attribute__((aligned(4))) ItemInventory; /* block copies are word-aligned */

/* GameData.keyItems: 2 bits per item id */
typedef struct {
    u8 bits[0x40]; /* bit 2n: item n held (keyItem_add/keyItem_has), bit 2n+1: seen (keyItem_setSeen/keyItem_isSeen) */
} __attribute__((aligned(4))) ItemFlags;

/* g_gameState->gameData: looks like the save-data block */
typedef struct {
    u32 flags; /* flags: GD_* (menu open/disabled, hide all, paused, music/SE slot state) */
    u32 allocTop; /* allocator top (g_allocTop) after setup */
    u8 drawBuf; /* draw buffer index (0/1): index into GameState drawEnv/dispEnv and ot[] */
    u8 unk9; /* write-only (cleared by every mode's setup) */
    u16 projDist; /* projection distance (SetFogNearFar); GTE geometry screen distance (SetGeomScreen), the battle camera's projDist */
    u16 fieldId; /* current field (runtime: Dali inn 351 -> street 350) */
    u16 battleId; /* next battle (encounter bundle id), set by evt_setNextBattle / the field and world exits */
    u32 *ot[2]; /* ordering tables, 0x1000 entries */
    MATRIX viewMatrix; /* camera view matrix (gte_SetRotMatrix/SetTransMatrix before projecting; btlcam_lookAt, mbg_getStartCamera) */
    u8 npcCount; /* kind-4 objects below the character scripts (evt_countNpcActors in fld_load) */
    u8 npcModelSlots; /* NPC model slots used, NPC slot = 9 + this */
    u16 fieldAreaId; /* field area to load next (fld_runArea, fld_unloadArea from FieldMapObj.areaId, wm_exit from wm_depFindArea) */
    Character characters[9];
    Character *party[4];
    u16 escapeCount; /* +1 per successful Escape (btl_checkCmdUsable, BTL_END_ESCAPE); saved */
    u16 firstUseFxSeen;
    u32 gil; /* capped at 9999999 (runtime: menu shows this value) */
    u8 pad564[0x568 - 0x564];
    s16 frogCount; /* event op 0xE0 adds 1 (max 0x7FFF), system variable 16 (EVT_SYSVAR_FROG_COUNT); ability formula 0x42 = it * level (Frog Drop) */
    s16 stealCount; /* +1 per successful steal (btl_calcSteal, max 9999); formula 0x43 = it * speed / 2 (Thievery) */
    s16 dragonKills; /* +1 per defeated enemy with enemy flag 8 (btl_updateKo) or BTL_SETVAR 39 (max 9999); formula 0x44 = it squared (Dragon's Crest) */
    u8 battleResult; /* how the battle ended, 6 = game over (ovl_0b, ovl_65 battle_end; event system variable 27, EVT_SYSVAR_BATTLE_RESULT) */
    u8 battleFlags; /* GD_BTL_*, cleared by btl_initBattleState */
    u8 rainCount; /* rain streaks (event opcode 0xD8, ovl_0b) */
    u8 darkBattle; /* set from wm_isDarkWeather when a world-map battle starts in the dark weather zone - night battle, Steiner model kind 8 (runtime) */
    u8 pad572[2];
    ItemInventory items;
    ItemFlags keyItems;
    u8 pad7B4[0x7D4 - 0x7B4];
    Font *font; /* text font (game_loadCommonImages), text_* glyph widths */
    void *commonDb; /* the common db registered by game_loadCommonDb (sound bank lookup) */
    s16 worldId; /* world map to run (wm_main), set by field/world exits; the battle saves and restores it (returnId) */
    u16 worldAreaId; /* area data of that world map (wm_findAreaData), from wm_depFindArea / fld_exit */
    s32 packetBuf; /* two 0x4000-byte primitive buffers for gfx_init (game_initGfx); btlui heap */
    struct FileTable *depTable; /* the loaded field/world file's dependency table (dep_find), -1 = none */
    u16 savedMusicId; /* music saved by sound command kind 8 (akao_saveMusic), restored by kind 9; 0xFFFF = none */
    u16 savedMusicField; /* field (or world) id where it was saved; kind 9 restores only there */
    s32 returnSector[2]; /* sceneSector/sceneSectors saved by fld_exit, to return to after a battle */
    s32 returnSectors[2];
    s32 sceneSector[2]; /* CD sectors of the next scene's files: field/world file + area, or battle bundle + background */
    s32 sceneSectors[2]; /* their sector counts */
    u8 *locationName; /* menu LOCATION box text (g_evtLocationName / g_evtLocationNameBuf) */
    u8 *debugLocationName; /* Shift-JIS developer name of the current location (runtime) */
    void *musicSeq; /* music sequence buffer (0x6800 bytes) of slot 0, akao_playMusic(musicSeq) */
    s32 musicSeq2; /* second music buffer (0x2000 bytes): slot-1 music (wm_loadResidentMusic), the battle copies it to musicSeq */
    u16 musicId[2]; /* current music per slot, 0xFFFF = none */
    u16 musicBank[2]; /* its sound bank (dep link of the music file) */
    u8 musicHandle[2]; /* akao_playMusic handle per slot */
    u8 musicVolume[2]; /* per slot, 0x7F = full */
    void *sfxSlotBuf[2]; /* resident sound effect slot data (0x800 bytes each), the akao_playSe handle; channel mask 0x100000 << slot */
    u16 sfxSlotId[2]; /* sound effect loaded in each slot, 0xFFFF = none */
    u16 worldBankReq; /* last sound bank requested by the world map (wm_soundCommand class 3 kind 7 skips a repeat) */
    u16 unk836; /* write-only (fld_init sets 0xFFFF) */
    s32 (*modelHook)(s32, void *); /* per-mode model setup (field fld_setupActorModel) */
    s32 (*soundHook)(s32, s32, s32, s32, s32); /* per-mode sound command handler (fld_soundCommand, wm_soundCommand, btl_soundCmd), snd_playSe calls it */
    u16 savedMusicBank; /* musicBank saved with savedMusicId */
    u16 musicFadeFrames; /* length of the running music fade (fld_setMusicVolumeFade / fld_isMusicFading) */
    s32 chocograph; /* selected chocograph 1-24, 0 = none (runtime field/menu campaign) */
    s8 enemyGroup; /* forced enemy group of the next battle (event op 0x8C), -1 = random (btl_pickEnemyGroup) */
    u8 unk849[1]; /* write-only (cleared) */
    u16 musicFileSectors; /* sector count of the current music file (fld_refindSectors, fld_soundCommand) */
    Font *kanjiFont; /* second (kanji) font table */
    u8 savedMusicVolume; /* musicVolume saved with savedMusicId */
    u8 soundLoadState; /* SND_LOAD_*: music/bank load state machine of the mode's sound command (fld_soundCommand, btl_soundCmd, wm_soundCommand) */
    u16 soundLoadBank; /* id of the sound bank being loaded (fld_soundBankReadCallback looks it up) */
    CdReadCtx cdRead[1]; /* CD read context (cdRead_init) */
    s32 soundLoadChunk; /* bytes read per chunk while loading a sound bank (free heap rounded to sectors) */
    s32 soundBankSize; /* bytes of the bank part of the sound file being loaded (the music sequence follows) */
    WmCameraState worldCamera; /* world map camera state (g_worldCamera points here, cleared with the next 0x18 bytes); saved */
    u8 worldCamKeep; /* opening_setWorldCamKeepNew / opening_setWorldCamKeepLoad, world-map camera reset (runtime) */
    u8 pad891;
    u8 inWorldFrame; /* 1 while a world-map frame runs once the vehicles are found (wm_frameMain sets it at the start, clears it at the end); a field entered with it 0 resets the world camera yaw on return (w_movement). Saved (ovl_39). Runtime: 1 at the vsync pause, 0 with the menu open; it only stays 1 when a frame does not finish (e.g. a save written during one). Name medium (the vehicle-to-field case was not tried) */
    u8 pad893[0x8A8 - 0x893];
    s32 musicFileSector; /* CD sector of the current music file (fld_refindSectors, fld_soundCommand) */
    s32 savedMusicFileSector; /* musicFileSector saved with savedMusicId */
    u16 savedMusicFileSectors; /* musicFileSectors saved with savedMusicId */
    s16 cardOpponent; /* Tetra Master opponent (evt_setCardOpponent, read by ovl_0d tm_game) */
    u16 sfxSlotSuspendField[2]; /* field id where the slot's sound was suspended (sound kind 8), kind 9 resumes it there (fld_suspendResidentSfx) */
    u8 sfxSlotVolume[2]; /* per slot, 0x7F = full (akao_setSeVolume) */
    u8 sfxSlotPan[2]; /* per slot, 0x80 = centre (akao_sendCmdA2 sets the SE channels' pan, akao_voice adds it to the voice pan) */
    void *preloadRequest; /* callback, signature varies by module; event op 0xFD (fld_preloadRequest, wm_preloadRequest) */
    s32 soundLoadDest; /* where the next sound-file chunk is read to */
    s32 musicFadeStart; /* VSync(-1) when the music fade started */
    PreloadQueue *preloadQueue;
    u8 unk8CC[8]; /* write-only (cleared) */
    struct Unk8D4 *chrModelObjs; /* [9] model objects of the characters, bound to chrModels[i].model */
    struct ModelSlot *chrModels; /* [9] per character (game_getModel ids 0-8) */
    struct Unk8D4 *npcModelObjs; /* model objects of the NPCs (npcCount), bound to npcModels[i].model */
    struct ModelSlot *npcModels; /* per NPC (game_getModel ids 9+), indexed by FieldMap.playerObj */
    Unk8E4Ptr ctx; /* the running mode's context (field, world, battle, ...) */
} GameData;
/* GameData.soundLoadState. Evidence: [code] the SND_CMD_GET_STATE step of fld_soundCommand / btl_soundCmd /
 * wm_soundCommand, which all run the same machine (the battle one starts at SND_LOAD_START). */
#define SND_LOAD_IDLE 0
#define SND_LOAD_WAIT_FADE 1 /* waits for the music fade-out, stops the music if GD_MUSIC_STOP_PENDING */
#define SND_LOAD_START 2     /* finds the sound file, akao_beginBankUpload, starts the CD read */
#define SND_LOAD_BANK 3      /* reads the bank part in chunks (uploaded to SPU); then the sequence or idle */
#define SND_LOAD_SEQ_START 4 /* copies what the last bank chunk read of the sequence, reads on into musicSeq */
#define SND_LOAD_SEQ 5       /* reads the rest of the sequence; then unpack it, GD_MUSIC_LOADED, idle */


typedef struct {
    u32 flags; /* flags: GS_* */
    u32 allocTop; /* allocator top after boot (g_allocTop, game_initState) */
    u8 mode; /* GS_MODE_* */
    u8 vsyncMode; /* VSync mode */
    u8 prevMode; /* GS_MODE_* being left (field, world, Tetra Master; set by the exits) */
    u8 disc;          /* DISC_*: disc bit (1 << disc - 1) of the SLUS file file_initImg found */
    DISPENV *dispEnv;    /* [2] */
    DRAWENV *drawEnv;   /* [2] */
    struct DirInfo *dirs; /* [15] FF9.IMG directory records (file_initImg) */
    void **dirTables; /* file tables per directory (file_loadDirTables) */
    GameData *gameData; /* the game data block (g_gameState->gameData) */
    s32 imgSector; /* FF9.IMG's first sector, added to file start sectors (file_initImg) */
    u32 config; /* Config screen settings, viewed bit by bit as GameConfig (with padMap) */
    u32 padMap; /* custom pad mapping, 4 bits per action */
    u32 playTime; /* frames at 60/s, shown as h:mm:ss in the menu (runtime) */
    s32 dirCount; /* FF9.IMG directories (file_initImg) */
    u8 unk34[15]; /* write-only (game_resetState clears it) */
} GameState;

extern GameState *g_gameState;
/* GameState.disc: one bit per disc of the game found in the drive (bit 0 = disc 1). The opening plays its movie
 * only with disc 1 in, the ending only with disc 4, and New Game sets disc 1 before scene_checkDisc asks for the disc
 * that was in before. */
#define DISC_NONE 0xFF /* no disc read (file_initImg, db.c reset) */
#define DISC_1    1
#define DISC_4    8
void file_loadDirTables(); /* game_initState calls it without arguments */

/* Inlined copy of chr_testSupportBit (support ability bit test). */
static inline u8 chr_testSupportBit_inline(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

/* Inlined copy of item_findSlot (item search); the original source inlines it. */
static inline ItemSlot *item_findSlot_inline(s32 id) {
    s32 i;
    ItemSlot *e = g_gameState->gameData->items.slots;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->count != 0 && e->id == id) {
            return e;
        }
    }
    return NULL;
}

/* ---- event countdown timer and play-time clock (VSync based) ---- */

/* script countdown timer (event opcodes: timer_setSeconds/timer_enable/timer_setRunning, timer_getSeconds) */
typedef struct {
    u8 enabled; /* drawn by game_updateTimer */
    u8 running; /* counting down */
    u8 pad2[2];
    s32 startVsync; /* VSync(-1) when enabled */
    s32 initial; /* frames */
    s32 remaining; /* frames */
} EvtTimer;

typedef struct {
    s32 lastVsync; /* VSync(-1) at the last play-time update */
    u8 pkt[0x400]; /* timer window packets, 0x200 per buffer */
    u8 *pktPtr; /* write pointer into pkt */
    EvtTimer timer;
} ClockState;
extern ClockState g_clock;
extern EvtTimer g_timer; /* the same timer as g_clock.timer, under its own symbol in the original */
extern RECT g_timerRect; /* countdown timer window */

/* ---- CD / streaming state near libcd ---- */

/* CD request, queued by cd_submit (0x24 bytes) */
/* one contiguous read: byte count and destination */
typedef struct {
    u32 size; /* runtime: ovl_39's single chunk {0xE800, 0x801EDF00} */
    u8 *dest;
} CdChunk;

typedef struct {
    u8 flags; /* CDREQ_* */
    u8 pad1[3];
    s32 sector; /* start sector; runtime: 0x5FA for ovl_39 */
    s32 size;   /* bytes; runtime: 0xE800 (29 sectors) for ovl_39 */
    CdChunk *chunks; /* chunk list; -1 = use unk1C: chunk list (cd_readChunks), -1 = the single chunk, NULL = seek only */
    void (*syncCallback)(u8, u8 *);
    void (*readyCallback)(u8, u8 *); /* data-ready handler */
    void (*doneCallback)(); /* callback */
    CdChunk chunk; /* single chunk */
} CdRequest;
/* CdRequest.flags (cd_startRequest, cd_startTransfer) */
#define CDREQ_SEEK 0x1 /* CdlSetloc to sector first; without it the request only pauses the drive */
#define CDREQ_READ 0x2 /* read after the seek (CdlReadN/ReadS); without it only seek (CdlSeekL/SeekP) */
#define CDREQ_PAUSE 0x4 /* pause/sync request: when the drive is idle, cd_submit only calls doneCallback */
#define CDREQ_STREAM 0x8 /* streaming: CdlReadS with cd_onStreamReady, CdlSeekP; a queued request doesn't pause it */

typedef struct {
    u8 state; /* CD_* */
    u8 errorCount; /* read errors of the request (cd_onDataReady / cd_onDiskError; reset by cd_submit) */
    u8 hasQueued;
    u8 error; /* 1 after a read error (wrong sector or disk error) */
    s32 sectorsLeft; /* runtime: 29 -> 0 while ovl_39 loads */
    CdlLOC pos; /* current position: CdlSetloc target (cd_startRequest) */
    s32 bytesRead;   /* runtime: +0x800 per sector */
    u32 chunkOfs;    /* offset in the current chunk */
    s32 chunkIndex;
    s32 lastSector; /* last sector read (CdGetSector), 0 before the first */
    u8 pad1C[0x944 - 0x1C];
    CdRequest cur; /* current request */
    CdRequest queued; /* passed to cd_startRequest */
} CdLoadState;
extern CdLoadState g_cdLoad;
/* CdLoadState.state: index into g_cdStateHandlers, the handler run on CdlComplete */
#define CD_IDLE 0
#define CD_SETLOC 1 /* CdlSetloc sent (cd_onSetloc) */
#define CD_SETMODE 2 /* CdlSetmode sent (cd_startTransfer) */
#define CD_READING 3
#define CD_PAUSING 4 /* CdlPause sent (cd_onPaused: done, or retry after an error) */
#define CD_RESTART 5 /* CdlPause sent by cd_restartAt (cd_onRestart reissues the request) */

/* ---- drawing ---- */

extern u32 *g_ot; /* ordering table */
extern u8 *g_primPtr;  /* next free primitive in the packet buffer */
extern s16 g_statusIconSprites[];
extern u8 g_portraitByKind[];

typedef struct {
    u16 name; /* name offset into g_itemNames (msg_emitItemName, btl_getItemName) */
    u16 help; /* name offset into the shop's text block: the screens' item help text (menuItem_updateHelp, shopBuy_updateHelp) */
    u16 price; /* price; runtime: the shop charges it, sells for half */
    u16 equipMask; /* who can equip (mask): bit per character set (shopBuy_drawParty, menuEquip_buildItemList) */
    u8 icon; /* kind (icon): sprite 0x5C + it (text_buildBlock), chr_calcAppearance weapon kind */
    u8 iconPalette; /* icon palette */
    u8 rank; /* equip rank: menuEquip_optimize picks the highest */
    u8 statBonus; /* stat bonus row (g_equipStatBonus) */
    u8 abilities[3]; /* abilities taught */
    u8 slots; /* mask tested against g_equipSlotMasks[]: equip slot bits (item_getEquipSlot), 1 = usable from the Item menu (menuItem_selectItem) */
    u8 sortKey; /* sort key: menuItem_sortAuto */
    u8 pad11;
} ItemInfo; /* item table, 0x12-byte entries */
/* ItemInfo.slots: g_equipSlotMasks (main data 0x8006BB84) = 80 20 40 10 08, in Character.equip order */
#define ITEM_SLOT_USABLE    0x01 /* usable from the Item menu (menuItem_selectItem, ovl_37) */
#define ITEM_SLOT_ACCESSORY 0x08
#define ITEM_SLOT_BODY      0x10
#define ITEM_SLOT_HEAD      0x20
#define ITEM_SLOT_ARM       0x40
#define ITEM_SLOT_WEAPON    0x80
#define ITEM_SLOT_ARMOR (ITEM_SLOT_WEAPON | ITEM_SLOT_HEAD | ITEM_SLOT_ARM | ITEM_SLOT_BODY) /* 0xF0: shop_isEquipmentShop (ovl_3c) */
#define ITEM_SLOT_EQUIP (ITEM_SLOT_ARMOR | ITEM_SLOT_ACCESSORY) /* 0xF8: equipment (ovl_3c shop screens) */
extern ItemInfo g_itemTable[];
extern u8 g_chrHpMpSupports[4]; /* max hp/mp support abilities */
extern u8 g_chrStatCaps[4]; /* stat caps */
extern u8 g_armorDefs[][4]; /* armor bonuses, indexed by item id (0x58..0xDF) */
/* weapon table entry (0xC bytes), indexed by item id < 0x58 */
typedef struct {
    u8 flags; /* BTL_WEAPON_* (ovl_0b.h) */
    u8 pad1;
    u16 modelId; /* weapon model: FF9.IMG dir 8 file id (| 0x20000: db key; ovl_0b btl_initWeapon) */
    u8 pad4;
    u8 attack; /* attack power (chr_calcEquipStats; Throw: power x2) */
    u8 pad6[0xC - 6];
} WeaponEntry;
extern WeaponEntry g_weaponTable[];
extern u8 g_equipSlotMasks[];
extern u8 g_newGameItems[];

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 wh;
} TileWords; /* TILE written as whole words */

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
    u32 xy2;
    u32 xy3;
} PolyF4Words; /* POLY_F4 written as whole words */

typedef struct {
    u32 tag;
    u32 code;
} DrTpageWords;

extern u16 g_textPalettes[];
/* g_textPalettes indexes: the palette argument of text_drawBlock / text_buildBlock / text_draw / text_drawChar /
 * text_buildGlyph / text_buildSmall / ui_drawSmallDigits, g_textColor and the TEXT_COLOR + n text codes
 * (colours from the runtime pass: fill, outline) */
#define TEXT_PAL_WHITE    0 /* white, dark outline: the default */
#define TEXT_PAL_PLAIN    1 /* white without the dark outline (text_buildGlyph draws it semi-transparent) */
#define TEXT_PAL_GREY     2 /* grey: not available / not selectable */
#define TEXT_PAL_RED      3
#define TEXT_PAL_YELLOW   4
#define TEXT_PAL_CYAN     5
#define TEXT_PAL_PURPLE   6
#define TEXT_PAL_GREEN    7
#define TEXT_PAL_HELP     8 /* pale steel blue: help text (help_draw, item descriptions) */
#define TEXT_PAL_HELP_RED 9 /* orange-red with a lavender outline: the help window's red */
extern u32 g_numberClutRows[]; /* per-style clut row offset for text_buildDigits */
/* ui_drawDigits / text_buildDigits style (g_numberClutRows: 0,0,0,0x40,0x80,0,0,0xC0; 1, 5 and 6 are white too) */
#define DIGITS_WHITE  0
#define DIGITS_DIM    2 /* white, drawn dim and semi-transparent */
#define DIGITS_RED    3 /* KO (hp 0) */
#define DIGITS_YELLOW 4 /* low HP / MP */
#define DIGITS_GREEN  7

/* sprite descriptor: uv/clut word, size, tpage (g_uiSprites) */
typedef struct {
    u32 uvclut; /* uv0 | clut << 16 */
    u8 w; /* w */
    u8 h; /* h */
    u16 tpage; /* tpage */
} SpriteDef;
extern SpriteDef g_uiSprites[];

/* member header slide (ui_*MemberSwitch, ui_drawMemberHeader) */
typedef struct {
    u8 active; /* active: the header is sliding to the next member (ui_startMemberSwitch) */
    u8 dir; /* slide direction (ui_drawMemberHeader: 1 = from the other side) */
    u8 member; /* party slot shown */
    u8 nextMember; /* party slot sliding in */
    s32 frame; /* ticks, 12 frames per switch */
    void (*onDone)(void); /* (ui_initMemberSwitch) */
} MemberSwitch;

/* ---- misc globals ---- */

/* per port: the DualShock actuator bytes PadSetAct sends (pad_update), set by vib_update from the vibration tracks */
typedef struct {
    u8 smallMotor; /* actuator 0: on/off (vib_update clamps the track sum to 1) */
    u8 largeMotor; /* actuator 1: strength 0-255 */
} PadActuator;
extern PadActuator g_padActuator[2];

typedef struct {
    u8 state; /* MENU_STATE_* (g_menuStateFuncs index) */
    u8 type; /* menu_open type (g_menuTypes) */
    u8 cardSlot; /* memory card slot (ovl_39 save screen) */
    u8 unk3; /* write-only (menu_reset clears it) */
    u8 unk4; /* write-only (menu_reset clears it) */
    u8 open; /* set by menu_doOpen, cleared by menu_finish */
    u8 reading; /* overlay/portrait read running (g_menuReading) */
    u8 fadingOut;
    u8 pad8;
    u8 drawBackground; /* menu_updateRun draws ui_drawBackground */
    u8 fadeInOnClose;
    u8 fadeOutOnClose;
    u32 arg; /* menu_open argument; bits 0-13: id, bits 14-15: disc */
    s32 frame; /* frame counter */
    s32 savedVsyncMode;
    s32 savedPadMask;
    s32 unk1C; /* cleared on open/reset; only read by menu_getUnusedWord1C, which nothing calls */
    u8 savedR; /* saved background colour */
    u8 savedG;
    u8 savedB;
    u8 pad23;
    s32 onClose; /* run by menu_finish */
} MenuState;
/* MenuState.state (g_menu) */
#define MENU_STATE_CLOSED 0
#define MENU_STATE_LOADING 1 /* set by menu_doOpen; menu_updateLoad reads the menu overlay / portraits */
#define MENU_STATE_WAITING 2 /* read issued (menu_readOverlay / menu_readPortraits); menu_updateWait */
#define MENU_STATE_RUNNING 3 /* menu_updateRun (menu_updateWait sets it when the loads are done) */
extern MenuState g_menu;

extern s32 g_nameGlyphs;
extern s32 g_nameImage;
extern u8 g_nameGlyphSpans[]; /* font glyph start/width nibbles, text_renderNameGlyph */
extern void (*g_winShadowFuncs[])(RECT *, s32);
extern s32 (*g_textMeasureFuncs[])(u8 *); /* text width of a value, per format */
s32 text_getCodeWidth(s32);
s32 text_measureNumber(s32, s32);
#if !defined(OVL_0A_H) && !defined(OVL_0C_H)
s32 text_measureStyle(u8 *, s32);
#endif
extern s32 g_numberDigitWidths[];
extern s32 g_buttonSprites[];
extern u8 g_textNumberBuf[];
extern u8 *g_textArgPtr;
extern s32 g_textColor;
extern u8 *g_textStringArgs[]; /* menu strings (text_setStringArg) */
extern s32 g_textNumberArgs[];
extern u8 g_textIconArgs[];
extern s32 g_winLastVsync;

typedef struct {
    u8 cur; /* buffer being built */
    u8 uiMode; /* 2 for the battle UI (gfx_setUiMode): windows close without the close animation */
    u8 unk2; /* write-only (cleared) */
    u8 pad3;
    s32 freeBytes; /* free bytes in the packet buffer */
    s32 lastVsync; /* VSync(-1) of the last window update (g_winLastVsync) */
    u8 *prims[2];   /* packet buffers */
    u32 *ots[2]; /* ordering tables */
    DVECTOR dispPos[2]; /* display x/y per buffer */
} DrawBufs;
extern DrawBufs g_drawBufs;

/* window frame insets per style (rodata, win_setClip) */
typedef struct {
    s8 insets[5][4]; /* per window style: x, y, w, h added to the clip rect (win_setClip) */
} Unk80010CE0;
extern Unk80010CE0 g_winClipInsets;

/* ---- functions (main exe) ---- */

s32 db_unregister(void *);
void memset(void *, s32, s32);
void (*CdReadyCallback(void (*)()))(); /* returns the previous callback */
void preload_clearEntry(PreloadEntry *);
void cd_controlF(u8, u8 *);
void cd_onStreamReady(u8, u8 *);
void ui_drawCharStatus(s32, s32, s32, Character *);
u8 *win_buildFill(RECT *, u32 *, u8, u8, s32, u8 *);
u8 *win_buildBorder(RECT *, u32 *, u8, u8, s32, s32, u8 *);
void win_drawHelpBorder(RECT *, s32, u8, u8, s32);
void win_drawHelpTailEdge(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void win_drawMsgTailEdge(); /* (s16 x, s16 y, s32 otz, s32 n, u8 flag), K&R */
void win_drawTailFill(s32, s32, s32, s32, s32, s32);
void win_drawHelpFill(RECT *, s32);
void win_drawHelpTailFill(s32, s32, s32, s32);
void win_drawTailEdge(s16, s16, s32, s32, u8, u8);
u8 *ui_buildItemIcon(s32, s32, u32 *, s32, u8, u8 *);
void ui_drawMemberStats(s32, s32, s32, s32);
void ui_drawCharStatuses(s32, s32, s32, s32);
u8 *ui_buildApGauge(s32, s32, u32 *, s32, s32, u8, u8, u8 *);
s32 ui_drawSprite(s32, s32, s32, s32, u8);
void ui_drawPartyPortrait(s32, s32, s32, s32, u8);

void text_formatNumber(s32, u8 *, s32, u8);
/* text_formatNumber / text_formatInt number formats (last argument) and width */
#define TEXT_NUM_CODES 0 /* game text digits 0-9 ('-' 0x0B, fill 0x0F), TEXT_END-terminated: for text_drawBlock */
#define TEXT_NUM_ASCII 1 /* '0'-'9' ('-', fill ' '), NUL-terminated: for ui_drawSmallDigits / ui_drawLabel / ui_drawDigits */
#define TEXT_NUM_NO_PAD (-1) /* text_formatNumber width: as many digits as needed (text_formatInt) */
void item_clearAll(void);
void keyItem_clearAll(void);
s32 item_add(s32, s32);
void ui_drawPortraitId(s32, s32, s32, s32, s32, u8);
extern s32 g_portraitCount; /* number of portraits (ui_drawPortraitId) */
void ui_drawPortrait(s32, s32, s32, s32, s32, u8);
void ui_getPartyPortraitRects(s32, RECT *, RECT *, RECT *);
void ui_loadPartyPortrait(u8 *, s32);
s32 text_renderNameGlyph(s32, s32, s32);
u8 *ui_buildLabel(s32, s32, u32 *, s32, s32, u8 *);
u8 *text_buildSmall(s32, s32, u32 *, u8 *, s32, u8 *);
u8 *text_buildDigits(s32, s32, u32 *, u8 *, s32, void *);
void ui_drawDigits(s32, s32, s32, u8 *, s32);
u8 *text_buildBlock(s32, s32, u32 *, u8 *, s32, u8 *);
typedef struct {
    s16 w;
    s16 h;
} TextSize;
TextSize text_measure(u8 *); /* measure a text string */
s32 config_remapButton(); /* (s32 index); unprototyped, as the original's callers pass ints unconverted */
void win_drawAll(void);
void win_updateAll(void);

/* Text encoding (font codes, 0xFF-terminated), shared by the menu text (text_buildBlock / text_measure) and the
 * message windows (msg_renderText / msg_advanceText / msg_getLayout). docs/text-codes.md lists every code.
 * Plain bytes from 0xF7 up are control codes; TEXT_ESC is followed by one escape byte (below), some of which
 * take arguments. Escape bytes the menu renderer doesn't handle go to text_buildCode / text_getCodeWidth. */
#define TEXT_NEWLINE 0xF7 /* x back to the start, y += 15 (message windows: += 14, the next line) */
#define TEXT_SPACE_1PX 0xF8 /* message windows: x += 1 */
#define TEXT_ESC 0xF9
#define TEXT_WIDE_FIRST 0xFA /* message windows: 0xFA-0xFD lead a two-byte glyph */
#define TEXT_WIDE_LAST 0xFD
#define TEXT_PAGE 0xFE /* message windows: wait for Cross, then a new page */
#define TEXT_END 0xFF /* message windows: wait for Cross, then close */
/* escape bytes after TEXT_ESC. Message timing and flow (msg_advanceText, msg_stepControl): */
#define TEXT_WAIT_PAGE 0x00 /* + frames: pause (Cross can't skip, MSGW_NO_SKIP), then a new page */
#define TEXT_WAIT_CLOSE 0x01 /* + frames: the same, then close the window */
#define TEXT_SPEED 0x02 /* + typing speed (MsgWindow.speed) */
#define TEXT_CHOICE_START 0x08 /* the choice lines start at the next line (MSGW_CHOICE) */
#define TEXT_HOLD 0x09 /* wait with Cross ignored (MSGW_WAIT_INPUT, MSGW_NO_INPUT): the script closes the window */
#define TEXT_NO_SKIP 0x0A /* Cross doesn't skip the rest of the page (MSGW_NO_SKIP) */
#define TEXT_SKIP 0x0C /* the rest of the page shows at once (MSGW_SKIP) */
#define TEXT_SYNC_TAG 0x0F /* + tag: g_msgSyncTag (EVT_SYSVAR_MSG_SYNC_TAG) */
#define TEXT_WAIT 0x41 /* + frames: pause (skipped while MSGW_SKIP), then go on */
#define TEXT_SPEED_DEFAULT 0x42 /* typing speed back to the config's (g_msgTextSpeeds) */
#define TEXT_NO_INPUT 0x45 /* Cross is ignored (MSGW_NO_INPUT); in the layout header: WINLAYOUT_NO_FOCUS */
#define TEXT_SYNC_TAG_NEXT 0x46 /* g_msgSyncTag++ */
/* message header codes, read by msg_getLayout from the start of the message (msg_advanceText skips them) */
#define TEXT_WIN_POS 0x03 /* + u16 x, u8 y: window position (anchor WINLAYOUT_ANCHOR_POS) */
#define TEXT_BANK 0x04 /* the message is a text bank (g_msgTextBanks, msg_resetChoices), not shown */
#define TEXT_CHOICES 0x07 /* + count, cancel choice + 1 (0: none): the message ends in choices */
#define TEXT_NO_OPEN_ANIM 0x0B /* the window opens without its animation (g_msgLayoutAltOpen, WIN_ANIM_NONE) */
#define TEXT_LINE_WIDTHS 0x0D /* {u16 choice << 12 | width, text up to TEXT_END}..., 0xFFFF: the window width is the
                               * widest line, counting only enabled choices (msg_measureLine adds the embedded values) */
#define TEXT_CHOICES_MASKED 0x4F /* + lines, cancel line: choices, some hidden by g_msgChoiceMaskReq (EVT_OP_MSG_CHOICE_MASK) */
#define TEXT_BALLOON_OFS 0x5F /* + 3 x s16: g_evtBalloonOfsX/Y/Z, the speaker point the balloon points at */
/* window anchors (WinLayout.anchor, evt_getMsgLayout). As a balloon the low 2 bits are its side of the speaker
 * (MSG_TAIL_DIR inverted: 0 = below right); without one, the fixed screen position given */
#define TEXT_WIN_BOTTOM_RIGHT 0x50
#define TEXT_WIN_BOTTOM_LEFT 0x51
#define TEXT_WIN_TOP_RIGHT 0x52
#define TEXT_WIN_TOP_LEFT 0x53
#define TEXT_WIN_BOTTOM 0x54 /* centred; a balloon is placed automatically */
#define TEXT_WIN_TOP 0x55 /* centred; a balloon is placed automatically */
#define TEXT_WIN_FIXED_BOTTOM_RIGHT 0x58 /* balloon only: that side even near the screen edge (evt_placeBalloon fixed) */
#define TEXT_WIN_FIXED_BOTTOM_LEFT 0x59
#define TEXT_WIN_FIXED_TOP_RIGHT 0x5A
#define TEXT_WIN_FIXED_TOP_LEFT 0x5B
#define TEXT_WIN_CENTER 0x5C /* x centred, y centred on 74 (no anchor: on 110); a balloon is placed automatically */
/* embedded values */
#define TEXT_BANK_TEXT 0x06 /* + bank, number slot: text g_msgNumbers[slot] of text bank g_msgTextBanks[bank] */
#define TEXT_MSG_ITEM_NAME 0x0E /* + number slot: name of item / key item / card g_msgNumbers[slot], in yellow */
#define TEXT_CHAR_NAME 0x10 /* + character 0-7 */
#define TEXT_PARTY_NAME 0x18 /* + party slot 0-3 */
#define TEXT_COLOR 0x20 /* + palette 0-9 (g_textColor; message windows: 0-8, pen_setPalette) */
#define TEXT_MSG_NUMBER 0x40 /* + index: number from the message system (g_msgNumbers) */
#define TEXT_CHAR_NAME_CAPS 0x80 /* + character 0-7: the name in capitals (msg_emitCharNameCaps) */
/* layout */
#define TEXT_X_ADD 0x43 /* + s8 */
#define TEXT_X_SET 0x44 /* + x from the line start */
#define TEXT_CENTER 0x48 /* + u16 width, codes up to TEXT_END whose values add to it (msg_measureLine): message
                         * windows: the line is centred */
#define TEXT_Y_DOWN 0x49 /* + pixels */
#define TEXT_Y_UP 0x4A /* + pixels */
#define TEXT_TAB 0x71 /* message windows: x += 18 */
#define TEXT_SPRITE 0x4B /* + sprite id */
/* button glyph escape bytes (text_buildCode / text_getCodeWidth draw these UI sprites; runtime pass) */
#define TEXT_DPAD 0x2F /* UI_SPRITE_DPAD (message windows only) */
#define TEXT_BTN_CIRCLE   0x30 /* UI_SPRITE_CIRCLE */
#define TEXT_BTN_CROSS    0x31 /* UI_SPRITE_CROSS */
#define TEXT_BTN_TRIANGLE 0x32 /* UI_SPRITE_TRIANGLE */
#define TEXT_BTN_SQUARE   0x33 /* UI_SPRITE_SQUARE */
#define TEXT_BTN_L1       0x34 /* UI_SPRITE_L1 */
#define TEXT_BTN_R1       0x35 /* UI_SPRITE_R1 */
#define TEXT_BTN_L2       0x36 /* UI_SPRITE_L2 */
#define TEXT_BTN_R2       0x37 /* UI_SPRITE_R2 */
#define TEXT_BTN_START    0x38 /* UI_SPRITE_START */
#define TEXT_BTN_SELECT   0x39 /* UI_SPRITE_SELECT */
/* the d-pad with arrows (text_buildDpad mask, g_dpadArrowSprites order: up, down, left, right) */
#define TEXT_DPAD_UP 0x3A
#define TEXT_DPAD_DOWN 0x3B
#define TEXT_DPAD_LEFT 0x3C
#define TEXT_DPAD_RIGHT 0x3D
#define TEXT_DPAD_UP_DOWN 0x3E /* menu text only */
#define TEXT_DPAD_LEFT_RIGHT 0x3F /* menu text only */
#define TEXT_PAD_BTN 0x60 /* + 0-7 in TEXT_BTN_* order (circle .. R2): that button as remapped by the controller
                           * config (text_getButtonSprite / GameState.padMap) */
#define TEXT_FLAG_ICON 0x70 /* + bit: message windows: a blinking two-sprite icon (0xB5, 0xB6) when g_msgNumbers[0] has the bit */
#define TEXT_ITEM_ICON 0x4C /* + item id */
#define TEXT_ARG_SPRITE 0x4D /* + arg index (g_textIconArgs) */
#define TEXT_ARG_ITEM_ICON 0x4E /* + arg index (g_textIconArgs) */
#define TEXT_ARG_NUMBER 0x68 /* + arg index (g_textNumberArgs), font */
#define TEXT_ARG_STRING 0x69 /* + arg index (g_textStringArgs), font */
#define TEXT_ARG_NUMBER_SMALL 0x6A
#define TEXT_ARG_STRING_SMALL 0x6B
#define TEXT_ARG_NUMBER_DIGITS 0x6C
#define TEXT_ARG_STRING_DIGITS 0x6D
#define TEXT_ARG_NUMBER_LABEL 0x6E
#define TEXT_ARG_STRING_LABEL 0x6F

/* ---- 0x54-byte objects, 10 at g_windows (win_reapClosed..8002E3F0) ---- */

typedef struct {
    s16 count; /* items */
    u8 rows; /* count / cols, rounded up */
    u8 visibleRows;
    u8 cols;
    u8 top; /* first visible row */
    u8 cursor; /* row on screen; runtime: battle spell list 0..3 */
    u8 col;
    u8 scroll; /* MENULIST_SCROLL_* animation in progress, 0 = none (menuList_update) */
    u8 scrollPos; /* pixels scrolled / page divider position */
    u8 fillU; /* fill texture u offset (win_drawFill), follows the open animation x */
    u8 fillV; /* fill texture v offset, moves with the scroll */
    u8 pageTop; /* top row after a page scroll */
    u8 height; /* pixels */
    u8 pageStep;
    u8 padF;
} MenuList;
typedef MenuList Unk16; /* older name */
/* MenuList.scroll */
#define MENULIST_SCROLL_DOWN 0x1 /* one row down (menuList_down), or a page down with MENULIST_SCROLL_PAGE */
#define MENULIST_SCROLL_UP 0x2 /* one row up (menuList_up); with MENULIST_SCROLL_PAGE a page up */
#define MENULIST_SCROLL_PAGE 0x4 /* a page (menuList_pageUp sets 7, menuList_pageDown 5) */
#define MENULIST_SCROLL_WRAP_COL 0x8 /* the cursor moved past the last/first column: col wraps when the row scroll ends */

typedef struct {
    u8 b[4];
} Unk4; /* 4 bytes, byte-aligned (copied with lwl/lwr) */

/* Menu window object (win_openFree returns one of these). */
typedef struct Window {
    s16 x; /* final rect (the open animation grows cur towards it) */
    s16 y; /* y */
    s16 w; /* w */
    s16 h; /* h */
    RECT cur; /* rect drawn this frame (frame, clip, shadow, content), animated towards x/y/w/h */
    s16 openFrames;
    s16 closeFrames;
    s16 stepW; /* per-frame width step of the open/close animation (win_setOpenAnim, win_stepOpen) */
    s16 stepH; /* per-frame height step */
    s16 otz; /* OT layer (win_init otz), passed to every draw call */
    u8 style; /* WIN_STYLE_* (runtime: the "Sure is dark..." balloon is WIN_STYLE_BALLOON) */
    u8 slot; /* index in g_windows */
    u8 state; /* WIN_STATE_* */
    u8 translucent;
    u8 active; /* nonzero: draw the content, bright title (0 dims it: the battle command window while a list is open) */
    u8 shadow;
    s16 titleX;
    s8 titleY;
    u8 visible;
    s32 title;
    u8 animMode; /* open/close animation, WIN_ANIM_* */
    u8 tailMode; /* feeds the open steps */
    s16 tailX; /* tail x (runtime: poking it moves the tail) */
    s16 stepLeft; /* per-frame step of the left edge when the window opens around its tail (win_setTail) */
    s16 stepRight; /* per-frame step of the right edge */
    MenuList list;
    void (*drawContent)(struct Window *);
    void (*drawOverlay)(struct Window *);
    void (*onOpened)(struct Window *);
    void (*onClosed)(struct Window *);
    s32 userData; /* user data: msg_open stores its MsgWindow, battle_end the learn-window slot */
} Window; /* 0x54 */
extern Window g_windows[10];
/* Window styles: Window.style, the style argument of win_open/win_openFree/win_drawBox/win_drawBoxEx, and the
 * index of g_winClipInsets (win_setClip), g_winShadowFuncs (win_drawShadow) and g_winStyleOpenAnim */
#define WIN_STYLE_LINE 0 /* line border (win_drawLineBorder); rect shadow; opens with WIN_ANIM_UNROLL */
#define WIN_STYLE_LINE_RULE 1 /* line border under a rule along the top edge (ui_drawRule); rect shadow; WIN_ANIM_UNROLL */
#define WIN_STYLE_BEVEL 2 /* bevelled border (win_drawBorder); slanted shadow; WIN_ANIM_ZOOM */
#define WIN_STYLE_BALLOON 3 /* speech balloon border with a tail (win_drawBalloonBorder); shadow on by default; WIN_ANIM_ZOOM */
#define WIN_STYLE_FRAMELESS 4 /* no frame, no shadow, no tail (win_drawAll, win_setTail); WIN_ANIM_NONE */
/* Window.state (win_updateAll) */
#define WIN_STATE_CLOSED 0 /* free slot */
#define WIN_STATE_OPENING 1 /* win_stepOpen each frame */
#define WIN_STATE_CLOSING 2 /* win_stepClose each frame; win_reapClosed frees it */
#define WIN_STATE_OPEN 3
/* Window.animMode: win_setOpenAnim / win_setCloseAnim modes */
#define WIN_ANIM_ZOOM 0 /* grow from the centre in both directions */
#define WIN_ANIM_UNROLL 1 /* grow downwards from the top edge */
#define WIN_ANIM_TAIL 2 /* grow around the balloon tail (win_setTail) */
#define WIN_ANIM_NONE 3 /* open (or close) at once */
#define WIN_ANIM_ZOOM_X 4 /* grow from the centre horizontally only */

void win_setOpenAnim(Window *, s32);
void win_setCloseAnim(Window *, s32);
void win_setTail(Window *, s32, s32);

/* ---- misc globals (batch 1) ---- */

extern u8 D_80068268[];
extern s8 g_mbgState;
extern s32 g_mbgFrame; /* movie frame counter (mbg_getStatus reports 3 once it is > 0) */
extern s8 g_mbgPaused;
extern s8 g_mbgRgb24;
extern s8 g_mbgIsBackground; /* mbg_getStatus reports bit 8 when it is 1 */
extern s32 g_menuOnClose;
extern u8 g_menuFadeInOnClose;
extern u8 g_menuFadeOutOnClose;
extern s32 g_labelWarning;
extern s32 g_labelCard;
extern u8 g_partyChanged;
extern struct MsgWindow *g_msgFocus;
extern s32 g_cardGameResult;
extern s32 D_8006798C;
extern u16 g_padFlags;
extern u8 D_8006825C;
extern u8 D_8006826C;

typedef struct {
    u8 pad0[0x24];
    u8 cmdArg; /* argument byte of the command */
    u8 pad25[0x2C - 0x25];
    s32 cmdArgs; /* &cmdArg when the command takes one, else NULL */
    u8 pad30[6];
    u8 cmdHasArg; /* 1 with cmdArgs */
    u8 cmd; /* controller command byte to send (0x45, 0x46, 0x47, 0x4B, 0x4C; func_8001A084 moves it to prevCmd) */
    u8 prevCmd;
} Unk8001A084; /* pad state? (near libpad) */


/* Sound sequencer channel (AKAO) */
typedef struct {
    u8 *pc; /* byte-code read pointer (akao_execChannel) */
    u8 *loopStart[4]; /* loop start addresses (op C8, C9/CA jump back) */
    u8 *returnAddr; /* return address of op FE call (akao_opFECall / akao_opFEReturn) */
    u8 *regions; /* key-split regions (AkaoRegion[], op FE14; akao_applyKeySplit) */
    s32 (*vibratoWave)(u32); /* LFO wave functions (pitch, volume, pan): pitch LFO wave (op B4, g_akaoLfoWaves) */
    s32 (*tremoloWave)(u32); /* volume LFO wave (op B8) */
    s32 (*panLfoWave)(u32); /* pan LFO wave (op BC) */
    u32 seMask; /* sound-effect channel group bits (VoiceParams +4; akao_stopSeChannels, akao_getActiveSe), AKAO_SE_DEFAULT */
    u32 pitch; /* pitch of the current note (akao_calcPitch), base of voicePitch and of the vibrato depth */
    s32 pitchSlide; /* pitch offset of a running slide (<< 16), added to voicePitch */
    u32 flags; /* AKAO_CH_* */
    s32 seBank; /* instrument bank offset of a sound effect (VoiceParams +0x10; akao_remapSeInstrument in op Program) */
    s32 vibratoPeriod; /* LFO period << 12 (op B4) */
    s32 vibratoPeriodStep; /* (op E4) */
    s32 vibratoPhase;
    s32 vibratoRate; /* phase step, 0x1000000 / period */
    s32 tremoloPeriod; /* (op B8) */
    s32 tremoloPeriodStep; /* (op E5) */
    s32 tremoloPhase;
    s32 tremoloRate;
    s32 panLfoPeriod; /* (op BC) */
    s32 panLfoPeriodStep; /* (op E6) */
    s32 panLfoPhase;
    u32 panLfoRate;
    s32 seHandle; /* sound effect this channel plays (akao_playSe handle; SE commands match it) */
    s32 sePitch; /* SE pitch scale (<< 8, 0 = none; akao commands A4/A5/AC/AD), applied in akao_calcSeChannelVoice */
    s32 sePitchStep;
    s32 volume; /* channel volume << 16 (op A8), times expression */
    s32 volumeStep; /* (op A9 slide) */
    s32 pitchSlideStep;
    u32 fineTuneOfs; /* pitch offset of fineTune (akao_calcPitch out), added to voicePitch */
    s32 age; /* ticks since the sound effect started (akao_tick); akao_stopSeChannels drops the oldest */
    s32 noteVolume; /* volume a slide restarts from on every key-on (op FE19) */
    s32 noteVolumeStep;
    u16 isSe; /* nonzero: sound-effect channel (akao_startSeChannel), 0: music */
    u16 noteTicks; /* ticks to the next event (akao_tickChannels) */
    u16 gateTicks; /* ticks to key-off */
    u16 instrument; /* index into g_akaoInstruments (op Program, key split) */
    u16 sePan; /* SE pan << 8 (akao commands A2/A3/AA/AB), added to pan */
    u16 sePanSlideLen;
    u16 opCount; /* commands executed (+1 per command; loops restore it) */
    u16 loopCount[4]; /* loop counters */
    u16 loopOpCount[4]; /* opCount at each loop start */
    u16 expression; /* second volume factor << 8 (op A3), multiplies volume */
    u16 expressionSlideLen; /* (op FE12) */
    s16 sePitchSlideLen;
    s16 volumeSlideLen; /* (op A9) */
    u16 noteVolumeSlideLen; /* (op FE19), nonzero: restart the volume slide on every key-on */
    s16 seVolumeSlideLen;
    u16 pan; /* << 8, 0x40 = centre in the byte code (op AA) */
    u16 panSlideLen; /* (op AB) */
    s16 pitchSlideLeft; /* frames left of the pitch slide */
    u16 octave; /* (ops Octave/OctaveUp/OctaveDown) */
    u16 slideLen; /* pitch-slide length set by op A4 */
    u16 prevKey; /* key of the previous note (portamento start) */
    u16 portamentoLen; /* op DA (0 = off, op DB) */
    s16 legato; /* AKAO_LEGATO_* */
    u16 vibratoDelay; /* frames before the vibrato starts (op B4) */
    u16 vibratoDelayLeft;
    s16 vibratoPeriodSlideLen;
    u16 vibratoDepth; /* depth in pitch units, from vibratoDepthParam and pitch */
    u16 vibratoDepthParam; /* op B5 (<< 8): bits 8-14 depth, AKAO_VIB_FULL_SCALE */
    s16 vibratoDepthSlideLen; /* (op DD) */
    s16 vibratoDepthStep;
    u16 tremoloDelay; /* (op B8) */
    u16 tremoloDelayLeft;
    s16 tremoloPeriodSlideLen;
    u16 tremoloDepth; /* (op B9) */
    s16 tremoloDepthSlideLen; /* (op DE) */
    s16 tremoloDepthStep;
    u16 panLfoPeriodSlideLen;
    u16 panLfoDepth; /* (op BD) */
    s16 panLfoDepthSlideLen; /* (op DF) */
    s16 panLfoDepthStep;
    u16 noiseTimer; /* frames until the noise bit toggles (ops CE/CF) */
    u16 pitchModTimer; /* frames until the pitch-modulation bit toggles (ops D2/D3) */
    u16 loopDepth; /* loop depth */
    u16 pitchRandom; /* op E1: random pitch variation per note (g_akaoLfoTableData[g_akaoRandIndex]) */
    u16 noteLen; /* length of the last note */
    s16 fixedLen; /* op DC: every note gets this length (0 = off) */
    s16 expressionStep;
    s16 sePanStep;
    u16 seVolume; /* SE volume << 8 (akao_cmdSetSeVolume), multiplies the volume */
    s16 seVolumeStep;
    s16 panStep;
    s16 transpose; /* (ops C0/C1) */
    s16 fineTune; /* (ops D8/D9) */
    s16 curKey; /* key of the current note (key split, portamento) */
    s16 slideKeys; /* op A4: semitones the next note slides by */
    s16 prevTranspose; /* transpose of the previous note */
    s16 vibratoOut; /* current vibrato pitch offset */
    s16 tremoloOut; /* current tremolo volume offset */
    s16 panLfoOut; /* current pan LFO offset */
    u8 pad116[0x118 - 0x116];
    u32 voice; /* voice number */
    u32 voiceDirty; /* AKAO_VD_*: voice registers akao_flushVoice still has to write */
    s32 voiceStartAddr;
    s32 voiceLoopAddr;
    u16 voicePitch; /* pitch */
    u16 voiceAdsr1;
    u16 voiceAdsr2;
    s16 voiceVolScale;
    s16 voiceVolL; /* volume left */
    s16 voiceVolR; /* volume right */
} AkaoChannel; /* 0x134 */
#define AKAO_VIB_FULL_SCALE 0x8000 /* AkaoChannel.vibratoDepthParam: the depth scales the full pitch instead of 15/256 of it (akao_opB5) */
/* AkaoChannel.legato (runtime: op D0 only runs on SE channels) */
#define AKAO_LEGATO      1 /* legato on (ops CC, DA); copied to AKAO_LEGATO_TIED after each note */
#define AKAO_LEGATO_TIED 2 /* the previous note is tied into this one: no vibrato/tremolo restart */
#define AKAO_LEGATO_SE   4 /* op D0 on a sound-effect channel; like AKAO_LEGATO it suppresses the gate cut */
/* AkaoChannel.voiceDirty / AkaoVoiceRegs.dirty: which SPU voice registers akao_flushVoice writes. The bits are
 * libspu's SpuVoiceAttr mask bits (SPU_VOICE_VOLL ... SPU_VOICE_LSAX); the AKAO ADSR ops set the ones they change. */
/* AkaoChannel.flags */
#define AKAO_CH_VIBRATO 0x1 /* ops B4 on / B6 off (akao_calcChannelVoice) */
#define AKAO_CH_TREMOLO 0x2 /* ops B8 / BA */
#define AKAO_CH_PAN_LFO 0x4 /* ops BC / BE */
#define AKAO_CH_DRUM_KIT 0x8 /* ops FE04 / FE05: notes play akao_playDrumNote */
#define AKAO_CH_PITCH_FROM_PREV 0x10 /* ops D4 / D5: the voice pitch follows the previous channel's */
#define AKAO_CH_VOL_FROM_PREV 0x20 /* ops D6 / D7: the volume follows the previous channel's pitch */
#define AKAO_CH_FULL_GATE 0x40 /* ops FE1A / FE1B: music notes keep the key on for their whole length */
#define AKAO_CH_KEY_SPLIT 0x1000 /* op FE14: the instrument has key-split regions (akao_applyKeySplit) */
#define AKAO_CH_SE_SECOND 0x10000 /* second channel of a two-channel sound effect (akao_startSe); op C6 may pitch-modulate it */
#define AKAO_CH_STOP_AT_LOOP 0x100000 /* op E0: a stop request only marks AKAO_CH_STOP_REQUESTED (akao_stopSeChannels) */
#define AKAO_CH_STOP_REQUESTED 0x200000 /* the next loop-back op CA ends the channel instead */
#define AKAO_CH_ATTACK_SET 0x1000000 /* op AD set the attack rate: notes keep it instead of the instrument's */
#define AKAO_CH_SUSTAIN_RATE_SET 0x8000000 /* op B1 */
#define AKAO_CH_RELEASE_RATE_SET 0x10000000 /* op B2 */
#define AKAO_CH_ADSR_SET (AKAO_CH_ATTACK_SET | AKAO_CH_SUSTAIN_RATE_SET | AKAO_CH_RELEASE_RATE_SET) /* cleared by op B3 */

#define AKAO_VD_VOLL 0x1
#define AKAO_VD_VOLR 0x2
#define AKAO_VD_VOL (AKAO_VD_VOLL | AKAO_VD_VOLR) /* akao_setVoiceVolume */
#define AKAO_VD_PITCH 0x10 /* akao_setVoicePitch */
#define AKAO_VD_START_ADDR 0x80 /* akao_setVoiceStartAddr */
#define AKAO_VD_ATTACK_MODE 0x100 /* op B7 */
#define AKAO_VD_SUSTAIN_MODE 0x200 /* op BB */
#define AKAO_VD_RELEASE_MODE 0x400 /* op BF */
#define AKAO_VD_ATTACK_RATE 0x800 /* op AD */
#define AKAO_VD_DECAY_RATE 0x1000 /* op AE */
#define AKAO_VD_SUSTAIN_RATE 0x2000 /* op B1 */
#define AKAO_VD_RELEASE_RATE 0x4000 /* op B2 */
#define AKAO_VD_SUSTAIN_LEVEL 0x8000 /* op AF */
#define AKAO_VD_LOOP_ADDR 0x10000 /* akao_setVoiceLoopAddr */
#define AKAO_VD_ADSR1 (AKAO_VD_ATTACK_MODE | AKAO_VD_ATTACK_RATE | AKAO_VD_DECAY_RATE | AKAO_VD_SUSTAIN_LEVEL) /* 0x9900: akao_setVoiceAdsr1 */
#define AKAO_VD_ADSR2 (AKAO_VD_SUSTAIN_MODE | AKAO_VD_RELEASE_MODE | AKAO_VD_SUSTAIN_RATE | AKAO_VD_RELEASE_RATE) /* 0x6600: akao_setVoiceAdsr2 */
void akao_calcChannelVoice(AkaoChannel *, u32); /* channel LFO / pitch update (akao_assignVoices passes the channel bit) */

/* 8-byte key-split region of an AKAO instrument (AkaoChannel.unk18) */
typedef struct {
    u8 sample; /* sample: instrument index (akao_remapInstrument) */
    u8 lowKey; /* low key */
    u8 highKey; /* high key */
    u8 attack; /* ADSR1 attack bits (<< 8) */
    u8 sustainRate; /* ADSR2 sustain rate (<< 6) */
    u8 sustainMode; /* 0 = end; 3/5/7 set ADSR2 bits 14-15 */
    u8 releaseRate; /* ADSR2 release (low 5 bits) */
    u8 volume; /* voiceVolScale */
} AkaoRegion;

/* voice setup parameters for akao_startSeChannel */
typedef struct {
    s32 handle; /* AkaoChannel.seHandle */
    s32 mask; /* AkaoChannel.seMask */
    u8 pan; /* AkaoChannel.sePan */
    u8 pad9[0xC - 0x9];
    u16 volume; /* AkaoChannel.seVolume */
    u8 padE[0x10 - 0xE];
    s32 bank; /* AkaoChannel.seBank */
} VoiceParams;

/* frame animation (sprAnim_update) */
typedef struct {
    u16 cellList; /* cell list index */
    u8 ticks; /* ticks */
    u8 pad3;
    s16 x; /* x offset */
    s16 y; /* y offset */
} SprAnimFrame;

typedef struct {
    s16 frameCount; /* frame count */
    u8 pad2[2];
    SprAnimFrame frames[1];
} SprAnimData;

typedef struct SprAnim {
    u8 playing; /* active (sprAnim_setPlaying) */
    u8 frame; /* frame */
    u8 tick; /* tick */
    u8 frameCount; /* frame count */
    SprAnimData *data; /* animation data (sprAnim_init) */
    SprAnimFrame *frames; /* frame table (data->frames) */
    SprAnimFrame *cellLists; /* cell lists, right after the frames */
    SprAnimData *base; /* cell offsets are relative to it (= data) */
    void (*onDone)(); /* called with the object; at the last frame (sprAnim_setOnDone) */
} SprAnim;


/* screen fade (g_fade, fade_*): runtime field/menu campaign, docs/runtime-findings.md */
typedef struct {
    u8 type; /* fade_start type, picks the g_fadeTypes handlers */
    u8 subtract; /* 1: subtractive (darkening) blend */
    u8 active;
    u8 covered; /* end colour non-zero: the screen stays covered after the fade (fade_setEndColor) */
    u8 eased; /* fadeFlat follows fade_ease's curve */
    u8 prevType;
    u8 pad6[0x8 - 6];
    s16 frames; /* length */
    s16 angle; /* z rotation of the wipe (fade_setAngle) */
    s32 frame; /* counts up to frames, then onDone runs */
    s32 onDone;
    u8 pad14[0x18 - 0x14];
    u8 *pkt; /* packet buffer cursor */
    s32 otz;
    Unk4 startColor;
    u8 endR;
    u8 endG;
    u8 endB;
    u8 pad27;
    Unk4 color; /* current colour */
} FadeState;
extern FadeState g_fade;
u32 *fade_getOt(s32);
#if !defined(OVL_0C_H) /* ovl_0c declares its own typed view */
void math_rotMatrixXYZ(void *, void *); /* rotation -> matrix */
#endif
s32 math_cos(s32); /* cos */
extern SVECTOR g_fadeWipeRot;
extern SVECTOR g_fadeWipeQuads[2][4]; /* two screen-filling quads */

/* Select help window (g_menuHelp, help_* functions; layout merged from main + ovl_14 + ovl_32) */
typedef struct {
    s16 x; /* hand cursor position; the help box is placed beside it */
    s16 y;
    s32 otz; /* hand OT index */
    u8 *text; /* help text, set by the handler */
    void (*callback)(void); /* per-menu function run every frame while help is open: sets text */
    RECT bounds; /* help_setBounds: the window stays inside */
    u8 isOpen;
    u8 opening; /* open animation running (help_toggle .. help_onIconAnimDone) */
    u8 textShown;
    u8 locked; /* Select doesn't toggle help */
    u8 follow; /* 0: help box at x/y itself, else beside the hand */
    u8 cursorOnTop; /* hand drawn above the help box */
    u8 pad1E[2];
    SprAnim iconAnim; /* moogle icon (sprAnim_*) */
} HelpState;
extern HelpState g_menuHelp;

/* sound sequencer state (AKAO), g_akaoSeq */
typedef struct {
    s32 flags; /* AKAO_SEQ_* */
    s32 activeMask; /* channels running (akao_tickChannels) */
    s32 priorityMask; /* channels that take voices first, from voice 0 (header +0x24, ops FE1D/FE1E; akao_findFreeVoice) */
    s32 keepVoiceMask; /* channels that keep their voice between notes (header +0x28); the others get flag 0x40 */
    s32 keyOnMask; /* channels with a key-on this tick */
    s32 playingMask; /* channels with a note sounding */
    s32 keyOffMask; /* channels to key off */
    s32 pausedMask; /* activeMask saved while paused (akao_cmdPauseMusic / akao_cmdResumeMusic) */
    union {
        s32 w;
        u16 h[2];
    } tempo; /* tempo, 16.16 */
    s32 tempoStep; /* op FE tempo slide */
    s32 tempoAcc; /* tempo accumulator */
    u8 *data; /* sequence data (akao_startSeq) */
    u16 *regionTable; /* key-split region table (header +0x30; op FE14) */
    s32 drumKit; /* drum kit table (AkaoDrumKey[], header +0x34; op FE04) */
    s32 firstVoice; /* first voice the channels without priority may use (ops FE10/FE11; akao_findFreeVoice, akao_stealVoice) */
    u32 noiseMask; /* channels with SPU noise (ops C4/C5/CE, akao_setNoiseVoices) */
    u32 reverbMask; /* channels with reverb (ops C2/C3, drum keys; akao_setReverbVoices) */
    u32 pitchModMask; /* channels with pitch modulation (ops C6/C7/D2, akao_setPitchModVoices) */
    s32 reverbDepth; /* << 12 (op FE02, SpuSetReverbModeDepth) */
    s32 reverbDepthStep; /* (op FE03) */
    s32 volume; /* music volume << 16 (0x7F), scales every channel (akao_calcChannelVoice) */
    s32 volumeStep; /* (akao_cmdFadeMusicVolume) */
    s16 volumeFadeLen;
    u8 pad5A[0x5C - 0x5A];
    s32 pan; /* music pan << 16, 0x40 = centre (akao_cmdSetMusicPan), added to the channel pans */
    s32 panStep; /* (akao_cmdFadeMusicPan) */
    s16 panSlideLen;
    s16 reverbDepthSlideLen;
    u16 tempoSlideLen;
    u16 musicId; /* id of the music playing (akao_cmdPlayMusic, akao_stopSeq) */
    u16 condition; /* value tested by op FE JumpIfGe, set by akao_cmdSetCondition */
    u16 noiseClock; /* SpuSetNoiseClock value (op AC) */
    u16 beatsPerMeasure; /* op FE15 */
    u16 beat;
    u16 ticksPerBeat; /* op FE15, 0 = no counting */
    u16 tick;
    u16 measure; /* measure counter (op FE16 sets it) */
} AkaoSeq;
/* AkaoSeq.flags */
#define AKAO_SEQ_NO_VOICE 0x1 /* akao_assignVoices found no voice for a channel */
#define AKAO_SEQ_VOICE_STOLEN 0x2 /* akao_assignVoices had to steal a voice (akao_stealVoice) */
#define AKAO_SEQ_INSTR_MAIN 0x10 /* the sequence's instrument set isn't g_akaoAltInstrumentSet (akao_startSeq) */
#define AKAO_SEQ_INSTR_ALT 0x20 /* it is: programs 0x20-0x7F use the instruments 0x30 up (akao_remapInstrument, akao_shiftInstrumentUp/Down) */
extern AkaoSeq *g_akaoSeq;
extern AkaoSeq g_akaoMainSeq; /* main sequencer state */
extern s32 g_akaoTickTimeSum;
extern u16 g_akaoTickCount; /* tick counter */
extern s32 g_akaoTickTimes[4]; /* recent tick timings */
void akao_flushKeyOffs(void);
void akao_updateVoices(void);
void akao_tickCrossfade(AkaoSeq *, AkaoChannel *);
extern AkaoChannel *g_akaoChannelsPtr;
extern s32 g_akaoInstrumentSets[2];
extern s32 g_akaoRandIndex;
extern s32 g_akaoInitDone;
extern s32 g_akaoVoiceModeMasks[3];
extern s32 g_akaoUnusedWord;
extern s32 g_akaoVsyncs[2];
extern s32 g_akaoTickPhase;

typedef Character Unk800539FC;


extern u8 g_menuScreenCur;
extern struct EvtObject *g_evtWorldCamEyeObj; /* object with EVT_MOVE_WM_CAMERA_EYE */
extern struct EvtObject *g_evtWorldCamTargetObj; /* object with EVT_MOVE_WM_CAMERA_TARGET */
extern s32 g_msgKeepWindows;
extern s32 *D_80066764;


/* actor position (VECTOR layout, the pad word holds two rotation angles) */
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s16 rotX; /* rotation x */
    s16 rotY; /* facing; runtime: 0 toward the camera, 3072 walking right */
} ActorPos;

/* field / world object (actor) */
typedef struct EvtObject {
    u8 *ip; /* script instruction pointer (runtime: walks the logged opcodes) */
    u8 level; /* running priority, 7 idle (runtime: 1 while the Moogle's talk function runs) */
    u8 kind; /* EVT_KIND_* */
    u8 scriptIdx; /* script entry (= uid except copies made by op 0x09 with a uid >= 0x80) */
    u8 uid; /* object id */
    u8 varsOfs; /* header size in words, 5/14/43 for kinds 2/3/4 (g_evtObjVars = o + varsOfs * 4) */
    u8 stackOfs; /* varsOfs + local-variable words */
    u8 sp; /* call stack depth in words (2 per call) */
    u8 battleTurnState; /* enemy turn: 2 requested (evt_battleTrigger BTL_CMD_ENEMY_TURN), 1 once the turn function runs, 0 after op 0x38 level 3 */
    u8 stackWords; /* call-stack words allocated after the local variables (evt_createObject: 0x10 for code, region and actor objects, 0 for threads); not read by C */
    u8 wait; /* frames left of op 0x22, or EVT_WAIT_WINDOW / EVT_WAIT_CALL */
    u8 state; /* EVT_STATE_* */
    u8 flags; /* EVT_OBJ_* (op 0x93 sets bits 0-5) */
    u8 windowId; /* message window it opened (ops 0x1F/0x20/0x54), 0xFF none */
    u8 actorType; /* op 0x3B; the world map indexes g_worldActorTypeInfo with it (wm_getEncounterEntry, w_frameGetParameter) */
    u8 savedState; /* state saved while suspended (evt_returnFromFunc copies it back) */
    u8 pad13;
    ActorPos pos; /* runtime: x rises walking right, z falls walking toward the camera */
    s16 rotZ; /* rotation z (op EVT_OP_SET_ROT_XZ second byte << 4) */
    u8 savedWalkRadius; /* walkmesh radius / 4 saved when the field exits to a battle (evt_createModels restores) */
    u8 pad27;
    u16 savedWalkTri; /* walkmesh triangle saved for a battle */
    u16 savedWalkFlags; /* walkmesh flags saved for a battle */
    u8 savedFlags; /* flags saved by op 0xD5, bit 0 restored by op 0xD6 */
    u8 collisionEllipse; /* op 0xDF, stretches the radius along the facing */
    s8 savedWalkFloor; /* walkmesh floor saved for a battle */
    u8 scale; /* 64 = 1.0 (op 0x9F), places the HERE icon / bubble anchor */
    s16 pitch; /* rotation x while flying to a point (op 0xA2) */
    s16 height; /* -(op 0x2F height << 2), places the HERE icon / bubble anchor */
    u16 modelId; /* op 0x2F (runtime: 98 Zidane, 220 the Moogle), EVT_MODEL_NONE */
    u16 anim; /* current animation (runtime: 200 idle, 38 running, 25 walking) */
    u8 animFrames; /* frame count of anim */
    u8 animPrevFrame; /* animFrame of the previous step */
    u8 animFrame; /* 0 .. animFrames-1 */
    u8 modelSlot; /* 0-8 the characters, 9 + GameData.npcModelSlots for NPCs */
    u8 collisionRadius; /* x4 world units (op 0x4B) */
    u8 talkRadius; /* x4 world units (op 0x4B, runtime: 0 stops Cross from starting the dialog) */
    u8 balloonWindow; /* window anchored on the actor (evt_followBalloon), EVT_BALLOON_NONE */
    u8 pad3F;
    ActorPos prevPos; /* position one game frame earlier, unk20/rotY double as turn-left/right anims (ops 0x7A/0x7B) */
    u16 moveFlags; /* EVT_MOVE_* */
    s16 turnTarget; /* target direction of a turn */
    s16 standAnim; /* op 0x33 */
    s16 walkAnim; /* op 0x34 */
    s16 runAnim; /* op 0x35 */
    u16 turnPrevAnim; /* animation to restore after turning */
    u8 speed; /* pad / script speed (runtime: 60 running, 30 walking) */
    u8 animSpeed; /* 16 moving, 18 idle (Zidane) */
    u8 runSpeed; /* speed >= runSpeed shows runAnim (op 0xA6) */
    u8 moveTurnRate; /* per-frame turn step x8 while walking to a point (op 0x55) */
    u8 animStart; /* op 0x3D */
    u8 animEnd; /* op 0x3D */
    u8 animFlags; /* EVT_ANIM_* */
    u8 animLoops; /* loop count (ops 0x3F/0x98), EVT_ANIM_FOREVER */
    u8 animSubFrame; /* 4-bit fraction of the 12.4 frame position */
    u8 pad65[0x68 - 0x65];
    u8 jumpFrame; /* 0 .. jumpFrames */
    u8 defaultAnimSpeed; /* op 0x3E, copied to animSpeed on each anim change */
    u8 lookParam0; /* op 0x8B */
    u8 lookParam1; /* op 0x8B */
    u16 turnRate; /* rotation y step per frame, or EVT_TURN_DIRECT_DOWN/UP (turn without a turn animation) */
    s16 talkTurnScript; /* EVT_OP_TURN_TO_ANGLE: turn toward the talker before the talk function (op 0x91), EVT_OP_RETURN: don't; the script "A7 00 angle 50 04" (EVT_OP_TURN_TO_ANGLE, EVT_OP_WAIT_TURN, EVT_OP_RETURN) starts here */
    u8 talkTurnAngle; /* its angle byte (evt_callTurnScript) */
    u8 talkTurnWaitOp; /* EVT_OP_WAIT_TURN, set at object creation */
    u8 talkTurnReturnOp; /* EVT_OP_RETURN */
    u8 turnSpeed; /* op 0x99, default 16 */
    u16 idleAnim; /* op 0x52 (runtime: 55) */
    u16 jumpAnim; /* op 0x94 */
    s32 moveDistSq; /* squared distance to the walk target, 0x7FFFFFFF when done */
    union { u32 w; u8 b[4]; } idleAnimSpeeds; /* op 0x86 (runtime: Zidane 14/16/18/20) */
    u16 turnStartDir; /* rotY when the turn started */
    u16 turnStep; /* per-frame turn step */
    s16 lookAtX; /* smoothed look-at point (evt_relaxLookAt) */
    s16 lookAtY;
    s16 lookAtZ;
    s16 balloonOfsX; /* speech bubble anchor offset (copied from g_evtBalloonOfsX..64) */
    s16 balloonOfsY;
    s16 balloonOfsZ;
    u8 jumpAnimStart; /* op 0x94 */
    u8 jumpAnimEnd; /* op 0x94 */
    u8 jumpFrames; /* op 0xE2 (runtime: 12) */
    u8 talkTarget; /* uid of the object the player last talked to */
    s16 jumpTargetX; /* op 0xE2 */
    s16 jumpTargetZ; /* op 0xE2 */
    struct EvtObject *collidedObject; /* object collided with (evt_movePlayer) */
    s32 collisionDepth; /* negative penetration depth */
    struct EvtObject *animSyncSource; /* actor whose anim it copies (op 0xB5) */
    u16 jumpStartX; /* position before the jump */
    u16 jumpStartY;
    u16 jumpStartZ;
    s16 jumpTargetY; /* op 0xE2 */
} EvtObject;
#define EVT_TURN_DIRECT_DOWN 0x7FFE /* EvtObject.turnRate: turning to a lower angle without a turn animation, turnStep per frame (evt_execInstr turn, evt_updateObjects) */
#define EVT_TURN_DIRECT_UP   0x7FFF /* EvtObject.turnRate: the same towards a higher angle */
/* Object property numbers of the script expression's object variables (evt_popValue -> evt_getObjectProp,
 * evt_popAssign -> evt_setObjectProp). The two number them differently; any other property goes to the battle unit
 * of a battle object (btl_getUnitAttr / btl_setUnitAttr, BTL_ATTR_*). Evidence: [code] the two functions. */
#define EVT_GETPROP_X 0
#define EVT_GETPROP_Y 1
#define EVT_GETPROP_Z 2
#define EVT_GETPROP_ROT 3        /* rotY >> 4, as a byte */
#define EVT_GETPROP_FLAGS 4
#define EVT_GETPROP_UID 5
#define EVT_GETPROP_LEVEL 6
#define EVT_GETPROP_ANIM_FRAME 7
#define EVT_SETPROP_ROT 2        /* rotY = value * 16 */
#define EVT_SETPROP_INSERT 0x20  /* BTL_ATTR_INSERT: the unit isn't listed yet, so it's found with btl_findUnitByBit */
/* EvtObject.kind: what created the object (evt_createObject sets 0, the creating op sets the kind) */
#define EVT_KIND_THREAD 1 /* EVT_OP_RUN_THREAD: a function of another object running on its own */
#define EVT_KIND_CODE 2 /* EVT_OP_NEW_CODE, 0x14-byte header */
#define EVT_KIND_REGION 3 /* EVT_OP_NEW_REGION, 0x38-byte header, a walkmesh region (evt_isInRegion) */
#define EVT_KIND_ACTOR 4 /* EVT_OP_NEW_ACTOR, 0xAC-byte header, has a model and moves */

/* EvtObject.state (evt_runScripts, evt_returnFromFunc) */
/* EvtObject.wait special values (evt_runScripts) */
#define EVT_WAIT_WINDOW 0xFE /* until its message window closes (msg_isOpen; ops 0x1F/0x54) */
#define EVT_WAIT_CALL 0xFF /* until a function it called on another object returns (evt_callObjectFunc, evt_returnFromFunc) */

#define EVT_STATE_NEW 0 /* created this frame (evt_createObject); evt_runScripts makes it EVT_STATE_INIT without running it */
#define EVT_STATE_RUNNING 1 /* running; when its call stack empties it idles (ip NULL) */
#define EVT_STATE_INIT 2 /* running the init function (entry 0); when it returns, entry 1 starts at level 7 and the state becomes EVT_STATE_RUNNING */
#define EVT_STATE_SUSPENDED 3 /* skipped by evt_runScripts; savedState holds the state to restore (evt_init after a battle, evt_returnFromFunc) */

/* EvtObject.flags */
#define EVT_OBJ_ENABLED 0x01 /* set with the model (EVT_OP_SET_MODEL) and on new regions; actors without it are skipped by the field/world overlays and the navi map; op 0xD5 clears it, op 0xD6 restores it */
#define EVT_OBJ_NO_PLAYER_COLLISION 0x02 /* passes through the player (evt_findNearestObject: both objects need the bit) */
#define EVT_OBJ_NO_NPC_COLLISION 0x04 /* passes through objects other than the player; new actors have it */
#define EVT_OBJ_NO_TALK 0x08 /* evt_findNearestObject's talk search skips it */
#define EVT_OBJ_NOT_PUSHABLE 0x10 /* bumping into it doesn't count as pushing (g_evtPushable, g_evtPushFrames) */
#define EVT_OBJ_KEEP_ENABLED 0x20 /* op 0xD5 leaves EVT_OBJ_ENABLED set */
#define EVT_OBJ_TURNING 0x80 /* a turn started by evt_turnObject is in progress (evt_finishTurn clears it) */
#define EVT_OBJ_SCRIPT_BITS 0x3F /* the bits EVT_OP_SET_OBJ_FLAGS sets */

/* EvtObject.moveFlags */
#define EVT_MOVE_LOOK_AT_ACTORS 0x0001 /* turns its head toward nearby EVT_MOVE_LOOK_TARGET actors whose lookParam0 matches its lookParam1 (evt_updateLookAt) */
#define EVT_MOVE_LOOK_TARGET 0x0002 /* EVT_MOVE_LOOK_AT_ACTORS actors may look at it */
#define EVT_MOVE_LOOK_AT_SPEAKER 0x0004 /* looks at g_evtLookTarget, the actor whose balloon window is open */
#define EVT_MOVE_LOOK_BITS 0x0007 /* the bits EVT_OP_SET_MOVE_FLAGS sets (EVT_OP_SET_LOOK sets the first two) */
#define EVT_MOVE_JUMPING 0x0008 /* EVT_OP_JUMP; cleared at the end of the jump animation (evt_onAnimEnd) */
#define EVT_MOVE_APPROACHING 0x0010 /* the distance to the walk target fell; the walk ends once it grows again */
#define EVT_MOVE_FACING_SET 0x0020 /* the walk turned to face the target; the heading stays fixed */
#define EVT_MOVE_WM_CAMERA_EYE 0x0040 /* world map: the camera eye follows the actor (op 0xB7) */
#define EVT_MOVE_WM_CAMERA_TARGET 0x0080 /* world map: the camera target follows the actor (op 0xB8) */
#define EVT_MOVE_LOOKING 0x0100 /* the look-at point is active (lookAtX-Z drive the model's head) */
#define EVT_MOVE_LOOKED_AT_SPEAKER 0x0200 /* looked at g_evtLookTarget */
#define EVT_MOVE_LOOK_RELEASED 0x0400 /* the speaker's window closed: no head turn script, the look-at point relaxes (evt_relaxLookAt clears 0x700) */

/* EvtObject.animFlags (evt_stepAnim, evt_onAnimLimit, evt_onAnimEnd) */
#define EVT_ANIM_IDLE 0x02 /* playing idleAnim (evt_updateIdleTimer) */
#define EVT_ANIM_HELD 0x04 /* stopped on its last frame (EVT_ANIM_HOLD); evt_stepAnim does nothing */
#define EVT_ANIM_HOLD 0x08 /* at the end keep the last frame instead of going back to standAnim */
#define EVT_MODEL_NONE 0xFFFF /* EvtObject.modelId before op 0x2F (evt_initActor) */
#define EVT_BALLOON_NONE 0xFF /* EvtObject.balloonWindow: no window follows the actor */
#define EVT_ANIM_LOOP 0x10 /* at the end restart from animStart (animLoops times, EVT_ANIM_FOREVER) */
#define EVT_ANIM_FOREVER 0xFF /* EvtObject.animLoops: loop without end */
#define EVT_ANIM_BOUNCE 0x20 /* at the end reverse direction */
#define EVT_ANIM_REVERSE 0x40 /* playing backwards */
#define EVT_ANIM_SCRIPTED 0x80 /* a script animation is playing (EVT_OP_WAIT_ANIM waits for it to end) */
#define EVT_ANIM_LOOP_MODE (EVT_ANIM_HOLD | EVT_ANIM_LOOP | EVT_ANIM_BOUNCE) /* EVT_OP_SET_ANIM_LOOP's argument << 3 */

/* Model object flags: ovl_0a ModelObj.flags, main's view Unk8004AFF0.flags */
#define MDL_LOOK_AT 0x1 /* fld_drawModels uses fldmdl_calcBoneMatricesLookAt (evt_updateLookAt sets it) */
#define MDL_SCALED 0x2 /* fldmdl_applyScale and the look-at variant apply the scale (EVT_OP_MODEL_SCALE) */
#define MDL_BEHIND_CAMERA 0x10 /* fldmdl_cullBehindCamera: origin SZ 0 or parent behind; colour/abr helpers skip it */

struct Unk8004AFF0; /* main's view of the model object, below */
void evt_onAnimLimit(EvtObject *);
s32 evt_callTurnScript(EvtObject *, s32);
void evt_relaxLookAt(EvtObject *, struct Unk8004AFF0 *);
s32 math_distSq2(s32, s32, s32); /* distance */
s32 math_isqrt(s32);
s32 math_dist2(s32, s32, s32);

typedef struct EvtAllocNode {
    EvtObject *start; /* key = start of the block in unk50 */
    u32 *end; /* end of the block */
    struct EvtAllocNode *next; /* next */
} EvtAllocNode;

/* node of the script object lists (EvtState.objects, listFree, battleObjectsTail) */
typedef struct EvtListNode {
    struct EvtListNode *next;
    EvtObject *obj;
} EvtListNode;

typedef struct {
    u8 fieldVars[0x50]; /* variable bank 1 (evt_getVarBank 1), cleared by evt_init */
    u32 objPool[0x500]; /* object pool (evt_allocObject) */
    EvtAllocNode allocNodes[32];
    EvtAllocNode *allocUsed; /* active list */
    EvtAllocNode *allocFree; /* free list */
    EvtListNode listNodes[32];
    EvtListNode *objects; /* object list the scheduler runs */
    EvtListNode *objectsTail; /* tail */
    EvtListNode *listFree; /* free list */
    u8 partyUids[4]; /* party member object uids (uid - 19 = character in the Dali party) */
    s16 analogDirOfs; /* added to the stick direction (op 0x67) */
    s16 padDirOfs; /* added to the d-pad direction (op 0x67) */
    u8 controlEnabled; /* runtime: 1 while the player walks, 0 during a dialog (field/menu campaign) */
    u8 playerUid; /* runtime: uid of the object the pad moves (Zidane, 19, in the Dali inn) */
    u8 mode; /* game mode the state belongs to */
    u8 encounterRate; /* op 0x57, added to g_evtEncounterChance per roll */
    u8 *scriptBase; /* event data, call frames store ip - scriptBase */
    u16 locationId; /* field / world id the state belongs to (evt_init restores g_evtSavedState when mode and id match) */
    u8 pad16F6[0x16F8 - 0x16F6];
    u8 walkOnly; /* op 0x6A sets, 0xF0 clears (runtime: walk anim only) */
    u8 pad16F9[0x16FC - 0x16F9];
    EvtListNode *battleObjectsTail; /* evt_init in battle, returned by evt_getBattleObjects */
    s16 idleTimer; /* frames until the player's idle anim, -1 disabled (op 0x90), 0 off (op 0xEE) */
    u8 pad1702[0x17C8 - 0x1702];
} EvtState; /* 0x17C8 */
extern EvtState *g_evtState;
extern EvtState g_evtStateData; /* the script system state g_evtState points at */
extern EvtState g_evtSavedState; /* saved copy, restored when returning to the same field */
extern Unk4 g_evtDefaultAnalogThresholds; /* rodata */
extern s32 g_evtResumeNode;
extern s32 g_evtCamera;
extern s32 g_evtEncounterPending;
void msg_resetPlayerBubble(void);

/* Inlined copy of evt_getObject (find object by id). */
static inline EvtObject *evt_getObject_inline(s32 id) {
    EvtListNode *n;
    for (n = g_evtState->objects; n != NULL; n = n->next) {
        if (n->obj->uid == id) {
            break;
        }
    }
    return n != NULL ? n->obj : NULL;
}


typedef struct {
    s32 type; /* 0 none, 1-2 passed to the focus window chain (2 adds bit 31), set by msg_readPad */
    s32 buttons;
} Unk8007B988;
extern Unk8007B988 g_msgPadEvent;
extern s32 g_msgPadCopySrc;
extern s32 g_msgPadCopyDst;

extern s32 g_textBank;
extern s32 g_evtScriptCount;
extern s32 g_evtCharScriptMask; /* party mask, set by evt_loadScripts */
extern u32 g_padPortHeld[];
extern u32 g_msgPadPressed;
extern u32 g_msgPadReleased;
extern s32 g_bubbleRequest;
extern s32 g_bubbleVariant;
extern u8 g_menuReading;
extern s16 g_evtBattleVars[8];
extern s32 g_msgPadPrev;
extern s8 g_akaoLfoTableData[];
extern s16 g_akaoTempoScaleLen;
extern s32 g_akaoTempoScale;
extern s16 g_akaoPitchScaleLen;
extern s32 g_akaoPitchScale;

extern Unk4 g_padAnalogThresholds;

extern u8 D_800739B8[];
extern AkaoSeq *g_akaoSubSeq;
extern AkaoSeq g_akaoSavedSeq;
/* AKAO sequence file header (akao_resumeSeq) */
typedef struct {
    u8 pad0[4];
    u16 id; /* sequence id (akao_command compares it with AkaoSeq.musicId) */
    u8 pad6[2];
    u16 reverbMode; /* passed to akao_setReverbMode when the music starts */
    u8 padA[0x14 - 0xA];
    s32 instrumentSet; /* compared with g_akaoAltInstrumentSet: equal selects AkaoSeq.flags 0x20 (programs 0x20-0x7F remapped), else 0x10 */
    u8 pad18[0x20 - 0x18];
    u32 channelMask; /* channel mask */
    s32 priorityMask; /* AkaoSeq.priorityMask */
    s32 keepVoiceMask; /* AkaoSeq.keepVoiceMask */
    u8 pad2C[0x30 - 0x2C];
    s32 regionTableOfs; /* offset from &unk30 */
    s32 drumKitOfs; /* offset from &unk34 */
    u8 pad38[0x40 - 0x38];
    u16 channelOfs[1]; /* per-channel offsets from the entry itself */
} AkaoSeqHeader;
void akao_resumeSeq(s32, s32);
void akao_startSeq(s32, s32);
extern s16 g_akaoSavedSeqMusicId;
extern s32 g_akaoAltInstrumentSet;
typedef struct {
    s32 bankUpload; /* 1 while a sound bank upload runs (akao_beginBankUpload / akao_uploadBank) */
    s32 mode; /* AKAO_MODE_STEREO / AKAO_MODE_MONO (akao_cmdSetOutputMode1/2), | AKAO_MODE_CROSSFADE */
    s32 dirty; /* AKAO_DIRTY_*: SPU state akao_updateVoices still has to write */
} AkaoFlags;
extern AkaoFlags g_akaoFlags;
#define AKAO_MODE_STEREO 1 /* AkaoFlags.mode output mode (Config Sound row, runtime): akao_cmdSetOutputMode1, the reset value */
#define AKAO_MODE_MONO 2 /* akao_cmdSetOutputMode2 */
#define AKAO_MODE_CROSSFADE 0x100 /* AkaoFlags.mode: akao_cmdCrossfadeMusic started one, akao_tickCrossfade runs until the sub sequence ends */
#define AKAO_DIRTY_NOISE_CLOCK 0x10 /* AkaoFlags.dirty: SpuSetNoiseClock */
#define AKAO_DIRTY_REVERB_DEPTH 0x80 /* SpuSetReverbModeDepth */
#define AKAO_DIRTY_VOICE_MODES 0x100 /* the noise / reverb / pitch-modulation voice masks */
/* menu state */
typedef struct {
    u8 active; /* 1 while the main menu runs (menuScreen_init / menuScreen_exit) */
    u8 slideIn; /* the main menu panels are sliding in (menuMain_onSlideInDone clears it) */
    u8 screen; /* current screen (0 main, 1 items, 4 status, 6 cards at runtime) */
    u8 member; /* selected party slot */
    s32 frame; /* frame counter, reset when a screen opens */
    s32 savedOtz; /* fade_getOtz() when the menu opened */
} MenuScreen;
extern MenuScreen g_menuScreen;
extern u8 g_menuScreenMember;
extern s32 g_evtLastEncounter;
extern u8 g_evtModelsPending;
extern s32 g_msgNumbers[8];
extern u8 *g_bubbleScript; /* help-arrow animation script */
extern s32 g_bubbleWait;
extern s32 g_bubbleRequestPrev;
extern volatile s32 g_akaoSpuBusy;

void db_clear(void);
s32 game_unloadCommonDb(void);
s32 strncmp(char *, char *, s32);
void game_setNextScene(u16);
void config_reset(void);
void win_runFrame(void);
void timer_resetAll(void);
void timer_update(void);
void fade_in(s32);
void fade_out(s32);
void fade_inAdditive(s32);
void fade_outAdditive(s32);
void fadeWipe_getColors(u8 *);
s32 fade_reverse(void (*)(void));
MATRIX *math_buildRotMatrixXYZ(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixZXY(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixZYX(SVECTOR *, MATRIX *, s32);
MATRIX *math_buildRotMatrixYXZ(SVECTOR *, MATRIX *, s32);

/* sine table, 0x400 entries for a quarter wave (4096 = 360 degrees) */
extern s16 g_sinTable[];

/* out = sin(a) from g_sinTable. a is the angle (bits above 0xFFF ignored), mk the same angle & 0xFFF. */
#define ISIN(out, a, mk)                              \
    if ((a) & 0x400) {                                \
        out = g_sinTable[0x400 - ((a) & 0x3FF)];      \
    } else {                                          \
        out = g_sinTable[(a) & 0x3FF];                \
    }                                                 \
    if ((mk) & 0x800) {                               \
        out = -out;                                   \
    }
void fade_init(void);
void fade_update(void);
void fade_start(s32, s32, s32, s32);
void menu_dispatch(void);
void menu_doOpen(s32, s32);
void akao_init(void);
void akao_quit(void);
s32 akao_command(u32);
void akao_loadBankRetry(s32, s32);
void akao_opEnd(AkaoChannel *, s32);
void akao_streamStop(void);
void help_setCallback(void (*)(void));

typedef struct {
    u8 pad0[4];
    u16 trackCount; /* row count */
    u16 tracksOfs; /* offset of a u16[][6] flag table: per track {flags[2], length[2], data offset[2]} (u16[6]; bit 0 of flags = selected, vib_selectTrack) */
} VibData;

typedef struct {
    u16 flags; /* VIB_* */
    s16 start; /* first frame (pos is reset to it) */
    s16 end; /* last frame (longest track - 1, vib_init) */
    s16 rate; /* pos step per frame << 8 (vib_setRate; negated when bouncing) */
    s32 pos; /* current frame << 8 */
    VibData *data; /* vibration data (vib_init) */
} VibState;
extern VibState g_vib;
/* VibState.flags (vib_update) */
#define VIB_INIT 0x1 /* vib_init ran; vib_update returns 3 without it */
#define VIB_PLAYING 0x2 /* vib_play / vib_setPlaying(1) set it with VIB_RUNNING */
#define VIB_RUNNING 0x4 /* cleared at the end of the range; vib_update runs while VIB_RUNNING or VIB_LOOP is set */
#define VIB_LOOP 0x8 /* at the end, keep playing (VIB_PLAYING stays set) */
#define VIB_BOUNCE 0x10 /* at the end, reverse the rate instead of jumping back to the start */
#define VIB_LOOP_MODE (VIB_LOOP | VIB_BOUNCE) /* the bits vib_setLoopMode sets (event op) */
u8 btlui_isSecondPortUsed(void); /* ovl_0b */


extern u8 g_chrAppearanceTypes[][2];
extern u8 g_menuScreenStrings[];
extern u8 g_cardNames[];
extern s32 g_atePromptMode; /* ATE_PROMPT_* (EVT_OP_ATE_PROMPT, msg_setAtePrompt) */
#define ATE_PROMPT_OFF 0            /* nothing drawn */
#define ATE_PROMPT_SELECT 1         /* "ACTIVE TIME EVENT" alternating with "PRESS [SELECT] TO SEE ATE" every 44 frames */
#define ATE_PROMPT_BLINK 2          /* grey "ACTIVE TIME EVENT", blinking 15 frames on, 15 off */
#define ATE_PROMPT_STYLE_MASK 3     /* style bits: ATE_PROMPT_BLINK, any other value draws the ATE_PROMPT_SELECT style */
#define ATE_PROMPT_ALWAYS 4         /* bit: draw even while the player has no control (scripts use modes 5 and 6) */
extern s32 g_atePromptTimer;
/* event / script variables; h[0] is the story progress counter */
typedef union {
    s16 h[0x400];
    u8 b[0x800];
} EventVars;
extern EventVars g_evtGlobalVars;
/* story flags in g_evtGlobalVars.b[] read by game code (docs/runtime-findings.md R30) */
#define EVT_GB_TRANCE_ENABLED 0x10      /* Trance gauge shown and filled (menuStatus_drawExp, btl_cmd) */
#define EVT_GB_DAGGER_FALTERS 0x11      /* Dagger may fail to act after Alexandria (btl_cmd) */
#define EVT_GB_SUMMON_MP_X4 0x12        /* summons marked flags & 4 cost 4x MP (Ramuh 22 -> 88) */
#define EVT_GB_BATTLE_NO_HUD 0xC1       /* battle runs without the HUD (btl_update) */
#define EVT_GB_NO_ACTION_ABILITIES 0xE3 /* action abilities unusable from the menu (menu_ability) */
#define EVT_GB_BATTLE_AI_FLAGS 0xC7     /* battle AI script flags (encounters 337, 891) */
#define EVT_GB_AI_EFFECT_CUE 0x10       /* EVT_GB_BATTLE_AI_FLAGS bit set by btlfx_signalScriptCue, the AI waits for it to end the battle */
/* script-only globals, set and read by field scripts (docs/r32/3ds-requests.md); byte offsets into g_evtGlobalVars like
 * the flags above, wider values stored little-endian at that offset (the scripts' s16/u16 variable types) */
#define EVT_GB_ARRIVAL_POINT 0x02       /* s16 (h[1]): entrance the next field's init places the player at; exits set it before GOTO_FIELD (also the entry code the destination init branches on, e.g. 1 = play the end-of-disc-1 FMV in field 70; R36) */
#define EVT_GB_ATE_BITS 0xEC            /* u16 (h[0x76]): available-ATE bits, copied to EVT_GB_ATE_CHOICE_MASK */
#define EVT_GB_ATE_AVAILABLE 0xEE       /* s8: 1 = an ATE can be viewed (the field script opens the ATE window on Select) */
#define EVT_GB_ATE_CHOICE_MASK 0xF1     /* s16 (unaligned): enabled choices of the ATE window (EVT_OP_MSG_CHOICE_MASK) */
#define EVT_GB_ATE_RETURN_FIELD 0xEF  /* s16: field to return to after an ATE (medium, docs/r32/3ds-requests-6.md) */
#define EVT_GB_ATE_ANNOUNCED 0xFB      /* u16: ATEs already announced (medium) */
#define EVT_GB_NAVI_PLACE 0x67  /* u8: navi map place under the cursor + 1, 0 = none (wm_naviUpdate; the world script names it, wm autopilot reads it) */
#define EVT_GB_GENERAL_FLAGS 0x65 /* u8: printed as "General Global Flag" by wm_applyStoryForms (ovl_0c): */
#define EVT_GB_GENERAL_MOGNET_FORM 0x80   /* the world map uses the replacement Mog Net form ("- Replace Mog Net -") */
#define EVT_GB_GENERAL_CHOCOBO_FORM 0x40  /* the world map uses the replacement Chocobo Heaven form */
extern u8 g_evtGeneralFlags; /* g_evtGlobalVars.b[EVT_GB_GENERAL_FLAGS] under its own symbol (ovl_0c reads it that way) */
#define EVT_GB_NAVI_KNOWN 0x5C  /* u16[4] (h[0x2E..0x31]): navi map places known, bit i&15 of h[0x2E + i/16] (wm_naviDraw) */
/* Mognet state, read only by the field scripts (docs/r32/3ds-requests-5-mognet.md) */
#define EVT_GB_MOGNET_VERSION 0x400     /* u8: Mognet data version (1) */
#define EVT_GB_MOGNET_GIVE_COUNT 0x408  /* u8: +1 on each "Give a letter" (medium) */
#define EVT_GB_MOGNET_SLOTS 0x40A       /* carried letters, 3 x {used, letter, from, to} */
#define EVT_GB_MOGNET_TAKEN 0x417       /* u8[8]: letter L taken from its sender, byte L/8 mask 0x80 >> L%8 */
#define EVT_GB_MOGNET_RECEIVED 0x41F    /* u8[8]: letter L received by its recipient, same layout */
#define EVT_GB_MOGNET_INTRO 0x427       /* bit 7: Mognet intro heard */
#define EVT_GB_MOGNET_LIST_LETTER 0x428 /* u8[15]: letter id of read-mail list row k */
#define EVT_GB_MOGNET_LIST_SENDER 0x437 /* u8[15]: sender (Moogle id) of list row k */

void config_applySound(void);
void config_applyPadMapping(void);
void menu_reset(void);
void menu_clearCardSlot(void);
s32 text_getFrom(u8 *, s32);
s32 text_measureWidth(s32);
void akao_setOutputMode(s32);
void chr_clearAp(s32);
void chr_clearSupport(s32);
void bzero(void *, s32);

/* argument block for sound command akao_command */
typedef struct {
    s32 arg0; /* first argument of the sound command (akao_api.c senders store their arguments in order, then akao_command) */
    s32 arg1;
    s32 arg2;
    s32 arg3;
    s32 arg4;
    s32 arg5;
} AkaoArgs;
void akao_startSe(); /* (VoiceParams *, u8 *, u8 *, s32); callers pass AkaoArgs * */
extern AkaoArgs g_akaoArgs;
extern AkaoArgs g_akaoCommandBlock; /* command block passed to the handlers */
extern void (*g_akaoCommandHandlers[])(AkaoArgs *); /* sound command handlers */

typedef struct {
    u8 id; /* card id, 0xFF = none */
    u8 arrows; /* arrow bits (card_rollStats; tm_game copies it to the board card) */
    u8 power; /* (tm_game power, card_rollStats) */
    u8 type; /* (tm_game type) */
    u8 physDef; /* (tm_game pdef) */
    u8 magicDef; /* (tm_game mdef) */
} CardEntry; /* card inventory entry */

/* Tetra Master card inventory (runtime field/menu campaign) */
typedef struct {
    s16 wins;
    s16 losses;
    s16 draws;
    CardEntry cards[105]; /* id 0xFF = empty */
} CardInventory;
extern CardInventory g_cards;

/* message window (g_msgWindows[10]; runtime field/menu campaign) */
typedef struct MsgWindow {
    struct MsgWindow *parent; /* next in the focus chain (msg_update) */
    s16 x;
    s16 y;
    s16 open;
    s16 id; /* index in g_msgWindows (msg_open) */
    s32 (*onInput)(struct MsgWindow *, s32); /* pad handler, msg_update */
    s32 unk10; /* write-only (cleared) */
    Window *window;
    s16 w;
    s16 tailOffset; /* from the layout, passed to win_setTail */
    s16 textWidth; /* width the text is centred with */
    s16 msgId; /* message index */
    s16 textPos; /* offset of the next character */
    s16 pageStart;
    s16 typeAccum; /* text timing in 1/8 characters (+speed per frame) */
    u8 waitFrames; /* frames of a text-code pause, or MSGW_WAIT_INPUT */
    u8 h;
    u8 flags; /* MSGW_* */
    u8 speed; /* text speed */
    u8 pendingChars;
    u8 tailMode; /* from the layout, passed to win_setTail */
} MsgWindow;
/* MsgWindow / WinLayout tailMode: bits 0-1 are win_setTail's mode (evt_placeBalloon: below << 1 | right),
 * MSG_FLAG_FRAMELESS / MSG_FLAG_NO_FOLLOW come from the message flags (evt_getMsgLayout); 0x20 has no reader there */
#define MSG_TAIL_RIGHT 0x1
#define MSG_TAIL_BELOW 0x2
#define MSG_TAIL_DIR (MSG_TAIL_RIGHT | MSG_TAIL_BELOW) /* passed to win_setTail */
#define MSG_TAIL_FRAMELESS 0x10 /* msg_open opens the window WIN_STYLE_FRAMELESS (no balloon or bevel) */
/* message flags: the flags argument of EVT_OP_MSG* (evt_getMsgLayout, msg_open, evt_execInstr_window) */
#define MSG_FLAG_NO_TAIL 0x04 /* balloon frame without its tail (also without a tail position), at least 0x1D wide */
#define MSG_FLAG_MOGNET 0x08 /* the Mognet title (g_msgMognetTitle) */
#define MSG_FLAG_FRAMELESS 0x10 /* copied into tailMode: MSG_TAIL_FRAMELESS */
#define MSG_FLAG_NO_FOLLOW 0x20 /* copied into tailMode; a balloon isn't attached to the actor (EvtObject.balloonWindow) */
#define MSG_FLAG_ATE 0x40 /* the ATE title (g_msgAteTitle), at least 0x72 wide */
#define MSG_FLAG_BALLOON 0x80 /* a speech balloon placed at the actor (evt_placeBalloon), at least 0x1D wide */
extern MsgWindow g_msgWindows[];
/* MsgWindow.flags */
#define MSGW_WAIT_INPUT 0xFF /* MsgWindow.waitFrames: waiting for Cross (msg_input); TEXT_PAGE / TEXT_END / TEXT_HOLD */
#define MSGW_CHOICE 0x02 /* has a choice (TEXT_CHOICE_START, or msg_allocWindow's flag); msg_getLastModal finds it */
#define MSGW_SKIP 0x04 /* Cross pressed: the rest of the page shows at once (TEXT_SKIP sets it too); cleared at the next wait for Cross */
#define MSGW_NO_SKIP 0x08 /* Cross doesn't skip the text (TEXT_WAIT_PAGE / TEXT_WAIT_CLOSE / TEXT_NO_SKIP); cleared at the next page */
#define MSGW_MESSAGE 0x10 /* opened by msg_open */
#define MSGW_NO_INPUT 0x40 /* Cross is ignored (TEXT_HOLD / TEXT_NO_INPUT) */


void menuScreen_loadOverlay(void);
void akao_stopSeChannels(s32, s32);
s32 preload_poll(void);
void preload_clearBusy(void);
void help_init(void);
void config_init(void);
void game_initFade(void);
void akao_streamRefill(s32, s32, s32, void (*)(void));
void akao_streamRefillMonoA(void);
void akao_streamRefillMonoB(void);
void akao_streamRefillStereoA(void);
void akao_streamRefillStereoB(void);
void akao_onSpuTransferDone(void);

typedef struct {
    s16 unk0; /* write-only (db_clear clears it) */
    s16 count; /* count */
    void *dbs[16]; /* registered dbs (db_register / db_unregister) */
} DbList;
extern DbList g_dbList;

typedef struct {
    u32 hdr; /* first word of the PadBuf: status, id byte (controller type << 4 | size), so (hdr >> 12) & 0xF = PAD_ID_* */
    u8 pad4[0x24 - 4];
} Unk80073940; /* 0x24-byte entries */
extern Unk80073940 g_padBuf[];


typedef struct {
    u8 pad0[0x12];
    s16 mp;
} Unk8006573C;

extern s16 g_resetCount;
extern u32 g_menuFlagsWord;
extern u8 g_chrDefaultSets[][2];

u16 *db_findModelInfo(s32);
void item_initNew(void);
void chr_newGameStub(void);
void party_initNewGame(void);
s32 snd_quit(); /* unprototyped: main calls it with no args, ovl_65 passes a pointer */
void db_quit(void);
void *db_lookupRegistered(s32, s32, s32); /* mode 4: address, 5: size */
void win_drawBoxEx(RECT *, s32, s32, s32);
void menu_start(void);
EvtObject *evt_createObject(s32, s32, s32, s32);
s32 CdSyncCallback(void (*)(u8, u8 *));
void CdFlush(void);

typedef struct {
    s32 startAddr; /* SPU sample start (voiceStartAddr) */
    s32 loopAddr; /* SPU loop address (voiceLoopAddr) */
    s16 fineTune; /* fine tune */
    s16 baseNote; /* base note */
    u16 adsr1; /* default ADSR1 (op B3) */
    u16 adsr2; /* default ADSR2 */
} AkaoInstrument;
extern u32 g_akaoSemitonePitch[12]; /* semitone pitch table */
extern AkaoInstrument g_akaoInstruments[];
u16 akao_calcPitch(AkaoInstrument *p, s32 note, u8 fine, s32 *out);
extern u8 g_akaoOpLengths[]; /* operand length of opcodes 0xA0-0xFF, 0 = special */
extern u8 g_akaoFeOpLengths[]; /* operand length of 0xFE xx opcodes */
s32 akao_peekNextNote(AkaoChannel *c);
extern void (*g_akaoOps[])(AkaoChannel *, u32); /* opcode 0xA0-0xFF handlers */
extern void (*g_akaoOpsFE[])(AkaoChannel *, u32); /* 0xFE xx opcode handlers */
extern u16 g_akaoNoteLengths[]; /* note lengths */

/* drum kit key entry (akao_playDrumNote) */
typedef struct {
    u8 sample; /* sample: instrument index */
    u8 note; /* note */
    u8 attack; /* ADSR1 attack bits (<< 8) */
    u8 sustainRate; /* ADSR2 sustain rate (<< 6) */
    u8 sustainMode; /* 3/5/7 set ADSR2 bits 14-15 */
    u8 releaseRate; /* ADSR2 release (low 5 bits) */
    u8 volume; /* voiceVolScale */
    u8 pan; /* bits 0-6 pan, AKAO_DRUM_REVERB */
} AkaoDrumKey;
#define AKAO_DRUM_REVERB 0x80 /* AkaoDrumKey.pan: akao_playDrumNote adds the voice to the reverb mask */
void akao_setChannelInstrument(AkaoChannel *, AkaoInstrument *, s32);

typedef struct {
    void (*init)(void); /* screen init (menuScreen_updateOpen, menuScreen_onSwitchFaded) */
    void (*update)(void); /* per-frame update (menuScreen_update); NULL = no screen (menuScreen_open ignores it) */
    void (*exit)(void);
    u8 needsMember; /* 1 for the screens that pick a party member first (menuScreen_needsMember) */
    u8 padD[3];
} MenuScreenDef;

typedef struct {
    u8 busy; /* a screen open/switch is in progress (menuScreen_updateOpen) */
    u8 titleResizing; /* the command window is resizing to the title box (menuScreen_stepTitleResize) */
    u8 titleScrolling; /* (menuScreen_stepTitleScroll) */
    u8 overlayRetry; /* the overlay read was refused, retried every frame (menuScreen_loadOverlay) */
    u8 overlayLoading; /* the overlay read is running */
    u8 fadingOut; /* menuScreen_open's fade-out runs (menuScreen_onOpenFaded clears it) */
    u8 switching; /* menuScreen_switch (no overlay load) */
    u8 target; /* screen being opened */
    s16 titleScroll; /* y offset of the command names in the title box */
    s16 titleScrollEnd;
    s16 titleScrollStep;
    s16 titleScrollFrame;
    s32 loadedOverlay; /* file id of the screen overlay at 0x801EDF00, 0xFF none */
    RECT cmdRect; /* command window rect (menuScreen_drawCommands) */
    MenuScreenDef *screens; /* g_menuScreensField / g_menuScreensWorld */
    void (*onFaded)(void); /* previous screen's exit, called when the fade-out ends */
} MenuScreenSwitch;
extern MenuScreenSwitch g_menuScreenSwitch;

extern s16 g_akaoCdVolumeFadeTicks;
extern s32 g_akaoCdVolume;
extern u32 *g_penOt;

void akao_applyCdVolume(void);
void menu_close(void);
void game_initTimer(void);
s32 fade_setOtz(s32);

/* one auto-repeat channel: pad_updateRepeat ORs its buttons into g_padRepeat after delay, then every interval frames */
typedef struct {
    u8 enabled;
    u8 repeating; /* past the first delay */
    u8 armed; /* buttons were released since the reset */
    u8 pad3;
    s32 prevHeld;
    s32 lastVsync; /* VSync(-1) of the last repeat */
    s32 mask; /* buttons this channel repeats */
    u16 delay;
    u16 interval;
} PadRepeat; /* 20 bytes */
extern PadRepeat g_padRepeats[][23];

void msg_beginDraw(void);
void pen_setColor(s32);
void preload_setEntry(PreloadEntry *, s32, s32, s32, s32, s32, s32, s32);
PreloadEntry *preload_find(s32);
s32 preload_wait(PreloadEntry *);
void gfx_init(u32 *, u32 *, s32, s32);

typedef struct {
    u8 pad0[0x34];
    u16 unk34; /* not used by C */
} Unk80021698;

extern s32 g_msgChoiceEnabled;
extern s32 g_msgChoice;
extern s32 g_msgChoiceCount;
extern s32 g_msgChoiceCancel;
extern s32 g_msgChoiceMaskReq;
extern s32 g_msgChoiceDefault;

s32 SpuRead(s32, s32);
void akao_beginSpuTransfer(void);
void akao_opAE(AkaoChannel *, s32);
void akao_opAF(AkaoChannel *, s32);
u8 cd_syncAsync(void (*)(u8 *));



extern u8 g_chrDefaultEquip[][5];
extern s32 g_akaoSkipMeasures;
extern u8 g_akaoTempoScaleHi; /* tempo scale */
void akao_execChannel(AkaoChannel *, u32);
void akao_stepChannelSlides(AkaoChannel *, u32, s32);

void akao_cmdPlayMusic(AkaoArgs *);

/* ---- hoisted from overlay headers ---- */

/* file table entry: a dependency-table record (dep_find, FileTable.entries). file_find returns its 8-byte
 * FileIdEntry viewed through this type: there `sectors` is the next entry's id and `link` the next entry's start
 * sector, i.e. the file's end sector (callers read link - sector as its length). */
typedef struct {
    s32 id; /* file id (24-bit key in a dependency table, dep_find) */
    s32 sector; /* start sector */
    s32 sectors; /* sector count (fld/wm/btl dep lookups pass it on as the size) */
    s32 link; /* key of the linked file (dep_find on it: the field's area, a music file's sound bank) */
} FileEntry;

/* g_gameState->dirTables[i]: 8-byte {id, sector} entries searched by file_find */
typedef struct {
    u16 id; /* id */
    u16 unk2; /* not read by C */
    s32 sector; /* start sector (the next entry's sector ends the file) */
} FileIdEntry;

/* file id table (GameData.depTable) */
typedef struct FileTable {
    u16 unk0; /* not read by C */
    u16 count; /* count */
    FileEntry entries[1]; /* {key, sector, sectors, link} records (dep_find) */
} FileTable;

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
    u32 wh;
} SprtWords; /* SPRT written as whole words */

/* menu list/cursor state passed to menuList_input etc. */


/* visible window of a MenuList (menuList_getPageView) */
typedef struct {
    u8 rows; /* visible rows */
    u8 first; /* first index */
    s16 y; /* y */
    u8 pad4[4];
} MenuView;

typedef struct {
    s32 labels[1]; /* window title / label texts, indexed past its end (win_setTitle, ui_drawLabel) */
    s32 abilityList; /* title of ability lists (menuAbility_drawList, btlui_openAbilityList, status ability pages) */
    u8 pad8[0x30 - 8];
    s32 command; /* title of the battle command window (btlui_drawCmdOverlay) */
    u8 pad34[0x3C - 0x34];
    s32 labelEnemy; /* "ENEMY" */
    u8 pad40[0x68 - 0x40];
    s32 itemList; /* title of item lists (menuItem_drawList, btlui_openItemList, shopBuy_drawList) */
    u8 pad6C[0x7C - 0x6C];
    s32 abilityCost; /* second title of ability lists (cost column) */
    s32 labelName; /* "NAME" (= g_labelName) */
    s32 labelNew; /* "NEW" */
    s32 labelNo; /* "NO" */
    s32 labelNormal; /* "NORMAL" */
    s32 labelNotice; /* "NOTICE" (= g_labelNotice) */
    s32 itemCount; /* right-aligned count-column title of item lists (btlui item/throw lists, menuEquip_drawList) */
    u8 pad98[0xA0 - 0x98];
    s32 labelPlayer; /* "PLAYER" (second copy) */
    s32 shopPrice; /* right-aligned price-column title (shopBuy_drawList, shopSynth_drawList) */
    u8 padA8[0xC0 - 0xA8];
    s32 throwList; /* title of the Throw list (btlui_openThrowList) */
} Unk800703FC; /* pointers to the window label strings ("ABILITY" .. "WRIST", +4 onwards); the fields are named by role or by their text */
extern Unk800703FC g_winLabelStrings;

extern u8 *g_loadAreaEnd; /* end of load area */
extern s32 g_modeHeap;
extern u32 g_levelExp[]; /* exp needed for next level, by level */
extern s32 g_labelChocoboAbility;
extern s32 g_labelChocograph;
extern s32 g_labelHelp;
extern s32 g_labelHint;
extern s32 g_labelInfo;
extern s32 g_labelSelected;
extern s32 g_padPortMask[];
/* Pad bits in g_padHeld/g_padPressed/g_padRepeat and g_padPort*: the low 16 are the controller's own
 * button bits, the high ones the game's buttons, named after their default mapping (checked at runtime by
 * holding each button, docs/runtime-findings.md). Triangle also sets 0x1000000, Select 0x2000000 | 0x8000000. */
#define PAD_SELECT   0x0001
#define PAD_START    0x0008
#define PAD_UP       0x0010
#define PAD_RIGHT    0x0020
#define PAD_DOWN     0x0040
#define PAD_LEFT     0x0080
#define PAD_L2       0x0100
#define PAD_R2       0x0200
#define PAD_L1       0x0400
#define PAD_R1       0x0800
#define PAD_TRIANGLE 0x1000
#define PAD_CIRCLE   0x2000
#define PAD_CROSS    0x4000
#define PAD_SQUARE   0x8000
#define BTN_CIRCLE   0x10000
#define BTN_CROSS    0x20000
#define BTN_TRIANGLE 0x40000
#define BTN_SQUARE   0x80000
#define BTN_L1       0x100000
#define BTN_R1       0x200000
#define BTN_L2       0x400000
#define BTN_R2       0x800000
#define BTN_START    0x4000000  /* raw Start (pad_setMapping in config_applyPadMapping) */
#define BTN_SELECT   0x8000000  /* raw Select; toggles the help window (help_update) */
#define PAD_SOFT_RESET (PAD_SELECT | PAD_START | PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1) /* 0xF09: fld/wm_endFrame set GS_RESET when all are held */
#define PAD_ID_DIGITAL 4 /* controller-type nibble ((PadBuf.unk0 >> 12) & 0xF) pad_update accepts */
#define PAD_ID_ANALOG 7 /* pad_update reads the stick bytes only for this type */

/* GameState.flags. Each mode loop runs until its exit bits are set. */
#define GS_RESET 0x1 /* game_main's inner loop ends: soft reset (PAD_SOFT_RESET) or restart */
#define GS_FLD_LEAVE 0x2 /* fld_entry loop ends */
#define GS_FLD_NEW_AREA 0x4 /* fld_runArea loop ends: next field in another area */
#define GS_FLD_NEW_FIELD 0x8 /* fld_main loop ends: next field in the same area */
#define GS_BTL_LEAVE 0x100 /* btl_entry loop ends (set with GS_MODE_BATTLE_END when the battle is over) */
#define GS_BTL_NEXT 0x200 /* btl_main loop ends without leaving btl_entry (nothing decompiled sets it) */
#define GS_EXIT_BITS (GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA | GS_FLD_NEW_FIELD | GS_BTL_LEAVE | GS_BTL_NEXT) /* 0x30F, cleared by fld_main/btl_load */
#define GS_WM_LEAVE 0x1000 /* wm_entry loop ends */
#define GS_WM_NEW_MAP 0x2000 /* wm_main loop ends without leaving wm_entry */
#define GS_RELOAD_FILE_TABLES 0x100000 /* set at boot (game_resetState) and after a disc change; file_loadDirTables reloads, then clears it */

/* GameState.mode / prevMode: the mode game_main runs next (its switch, game_loadModeOverlay's overlay table) */
#define GS_MODE_OPENING 0 /* ovl_14 opening_main (logos, title) */
#define GS_MODE_FIELD 1 /* ovl_0a fld_entry */
#define GS_MODE_BATTLE 2 /* ovl_0b btl_entry */
#define GS_MODE_WORLD 3 /* ovl_0c wm_entry */
#define GS_MODE_ENDING 4 /* ovl_15 ending_main */
/* 5: no overlay and no case in game_main; vib_update and ovl_64/ovl_0a treat it like the field. 6: no overlay either;
 * msg_beginDraw treats it like the battle (docs/wip/constants_runtime_main.md) */
#define GS_MODE_GAME_OVER 7 /* ovl_65 fld_gameOver (field exit FLD_EXIT_GAME_OVER) */
#define GS_MODE_BATTLE_END 8 /* ovl_65 battleEnd_main, set by ovl_0b when a battle ends */
#define GS_MODE_TETRA_MASTER 9 /* ovl_0d tm_main; as prevMode it makes evt_init/evt_createModels rebuild the field models */

/* g_evtMode / EvtState.mode: the event system's mode, from GameState.mode in evt_init */
#define EVT_MODE_FIELD 1
#define EVT_MODE_BATTLE 2
#define EVT_MODE_WORLD 3
#define EVT_MODE_BATTLE_END 4 /* GS_MODE_BATTLE_END */

/* GameData.flags flags */
#define GD_MENU_DISABLED 0x1 /* menu_doOpen refuses to open */
#define GD_MENU_OPEN 0x2 /* set by menu_doOpen, cleared by menu_finish; the field/world event update is skipped */
#define GD_HIDE_MODELS 0x4
#define GD_HIDE_BG 0x8
#define GD_HIDE_SPRITES 0x10
#define GD_HIDE_ALL (GD_HIDE_MODELS | GD_HIDE_BG | GD_HIDE_SPRITES) /* 0x1C: menu_onFadedOut sets it, menu_finish clears it */
#define GD_NO_FLIP 0x20 /* fld/wm_endFrame keep the draw buffer (pause screens) */
#define GD_PAUSED 0x100
#define GD_SOUND_REINIT 0x100000 /* battleEnd_exit shut the sound down (snd_quit) before going back to the title; opening_setup runs snd_init and clears it */
#define GD_MUSIC_STOP_PENDING 0x200000
#define GD_MUSIC_LEFT_PLAYING 0x400000 /* the mode just left kept its music playing (wm_main, w_music, battleEnd_exit); the next one fades it (SND_MUSIC_VOL_INTPL) while loading and fld_startAudio stops it */
#define GD_SFX0_LOADED 0x01000000 /* resident SE slot 0 copied (fld_soundCommand with SND_RESIDENT) */
#define GD_SFX0_PLAYING 0x02000000
#define GD_SFX1_LOADED 0x04000000
#define GD_SFX1_PLAYING 0x08000000
#define GD_MUSIC_LOADED 0x10000000
#define GD_MUSIC_PLAYING 0x20000000
#define GD_MUSIC2_LOADED 0x40000000
#define GD_MUSIC2_PLAYING 0x80000000

/* GameData.battleFlags (per battle) */
#define GD_BTL_NEXT_LOCATION 0x01 /* the battle set the next location (BTL_SETVAR_NEXT_LOCATION); battleEnd_main loads it */
#define GD_BTL_VICTORY_MUSIC_LOADED 0x02 /* btl_playVictoryMusic steps */
#define GD_BTL_FLED 0x04 /* escaped with the Flee command: the escape costs gil */
#define GD_BTL_KEEP_MUSIC 0x08 /* the battle music goes on after the battle (battleEnd_exit doesn't set GD_MUSIC_LEFT_PLAYING); event op 0x1B */
#define GD_BTL_VICTORY_MUSIC_FADE 0x10 /* the music fade before the victory music started */
#define GD_BTL_VICTORY_MUSIC_PLAYING 0x20
#define GD_BTL_EXIT_PRELOAD 0x40 /* btl_main starts the exit preload (btl_preloadExit) */

/* inventory and character limits */
#define LEVEL_MAX 99
#define EXP_MAX 9999999
#define GIL_MAX 9999999

/* main-menu screens (g_menuScreen.screen, menuScreen_open/switch; runtime field/menu campaign) */
#define MENU_SCREEN_MAIN 0
#define MENU_SCREEN_ITEM 1
#define MENU_SCREEN_ABILITY 2
#define MENU_SCREEN_EQUIP 3
#define MENU_SCREEN_STATUS 4
#define MENU_SCREEN_ORDER 5
#define MENU_SCREEN_CARD 6
#define MENU_SCREEN_CONFIG 7

/* w_frameGetParameter ids: event-script expression variables >= 0xC0 on the world map
 * (main evt_getSysVar's default branch) */
#define WM_PARAM_AREA 0xC0 /* area of the cell under the player ("Now Area" in wm_getEncounterEntry) */
#define WM_PARAM_TOPOGRAPHY 0xC1 /* topography of that cell ("Topograph") */
#define WM_PARAM_REQ0 0xC3 /* g_worldReqParams[0..2], e.g. the WM_REQ_GET_GETOFF_POS result */
#define WM_PARAM_REQ1 0xC4
#define WM_PARAM_REQ2 0xC5
#define WM_PARAM_CAMERA_YAW 0xC7 /* g_worldCamera->yaw >> 4 */
#define WM_PARAM_CD_BUSY 0xC8 /* "Event:CDAccessWait": g_worldCdUser | music request pending */
#define WM_PARAM_PLAYER_X 0xC9
#define WM_PARAM_PLAYER_Y 0xCA
#define WM_PARAM_PLAYER_Z 0xCB
#define WM_PARAM_WEATHER_POINT_DIST 0xCC /* g_worldWeatherPointDist */
#define WM_PARAM_EVENT_BATTLE 0xCD /* 1 in g_worldEventBattleRate + 1 while walking on topography 0x24-0x26 */
#define WM_PARAM_PLAYER_DIR 0xCE /* player rotY >> 4 */
#define WM_PARAM_ZONE 0xCF /* wm_getAreaZone(area) */
#define WM_PARAM_RUNNING 0xD1 /* g_worldState == WM_STATE_RUNNING */
#define WM_PARAM_TITLE_PHASE 0xD2 /* g_worldTitlePhase */
#define WM_PARAM_NAVI_VISIBLE 0xD3 /* g_worldNaviVisible */

extern u32 g_padRepeat; /* press, then auto-repeat after 15 frames, every 5 */
extern s32 g_menuFrame; /* frame counter? */

s32 mbg_getStatus(void);
s32 EnterCriticalSection(void);
void ExitCriticalSection(void);
s32 OpenEvent(s32, s32, s32, void (*)(void)); /* BIOS OpenEvent */
s32 EnableEvent(s32); /* BIOS EnableEvent */
s32 db_register(s32, s32);
s32 db_lookup(s32, s32, s32, s32);
s32 db_getSize(s32); /* file size */
void game_setClearColor(s32, s32, s32);
void game_freezeScreen(void);
void file_loadTim(s32);
void file_setTimPos(s32, s32, s32, s32, s32);
s32 file_copyViaVram(s32, s32, s32, RECT *); /* copy through VRAM, returns bytes done */
extern u8 g_vramCopyRectIdx;      /* next slot in g_vramCopyRects */
extern RECT g_vramCopyRects[6]; /* transfer rects (file_copyViaVram) */
void file_initImg(void);
FileEntry *file_find(s32, s32);
void evt_setNextLocation(s32);
void timer_clear(void);
void pad_resetRepeat(s32, s32);
void pad_clearRepeat(s32, s32);
void pad_setRepeatEnabled(s32, s32, s32);
typedef struct {
    s32 r;
    s32 g;
    s32 b;
} Rgb32;
typedef struct {
    s32 startR; /* start colour (fadeFlat_start) */
    s32 startG;
    s32 startB;
    s32 endR;
    s32 endG;
    s32 endB;
    s32 stepR; /* per-frame step (delta / frames) */
    s32 stepG;
    s32 stepB;
    Rgb32 color; /* current: linear colour */
    Rgb32 out; /* output: eased colour (fade_ease) or color */
    s32 deltaR; /* end - start */
    s32 deltaG;
    s32 deltaB;
} FadeFlat; /* screen fade state */
extern FadeFlat g_fadeFlat;
s32 fade_divRoundOut(s32, s32);
void chr_init(s32);
void pad_setRepeatMask(s32, s32, s32);
typedef struct {
    s32 mask;
    u8 enabled;
    u8 delay;
    u8 interval;
    u8 pad7;
} PadRepeatDef;
extern PadRepeatDef g_padRepeatDefs[23]; /* pad_initRepeat defaults */
typedef struct {
    u8 x0, y0, x1, y1, x2, y2, x3, y3;
} Unk80068F48;
extern Unk80068F48 g_winTailShapes[];
void akao_setVoiceAttack(s32, s32, s32);
void akao_setVoiceSustain(s32, s32, s32);
s32 chr_getClass(u32);
s32 chr_sumEquipBonus(u8 *, s32);
typedef struct {
    u8 speed; /* base speed (chr_calcBaseSpeed) */
    u8 strength; /* (chr_calcBaseStrength) */
    u8 magic; /* (chr_calcBaseMagic) */
    u8 spirit; /* (chr_calcBaseSpirit) */
    u8 stones; /* magic stones (chr_calcMaxStones) */
    u8 unk5; /* not read by C */
} ClassStats; /* base stats per character class */
extern ClassStats g_charClassStats[];
void pad_initRepeat(void);
void win_initAll(void);
void preload_onReadDone(u8 *);
u8 cd_seekAsync(s32, void (*)(u8 *));
void akao_stopSeChannel(AkaoChannel *, u32);
typedef struct {
    u8 stats[4]; /* stat bonuses: speed, strength, magic, spirit (chr_calcEquipStats) */
    u8 elemGuard[4]; /* OR'd into BtlUnit.elemGuard (btl_initEquipElements) */
    u8 elemBoost; /* OR'd into BtlUnit.elemBoost */
    u8 pad9[0xC - 9];
} EquipBonus;
extern EquipBonus g_equipStatBonus[];
void chr_setRecruited(u32);
void msg_setFocus(MsgWindow *);
MsgWindow *msg_getFocus(void);
extern s32 g_msgChoiceLine;
extern s32 g_msgChoiceWindow;
extern Unk4 g_msgSavedAnalogThresholds;
extern Unk4 g_msgChoiceAnalogThresholds; /* rodata constant, used by msg_advanceText */


void akao_streamArmStop(void);
extern u8 g_akaoEmptyTrack[];
extern AkaoSeq *g_akaoVoiceOwner[24];
void akao_setVoiceRelease(s32, s32, s32);
void help_draw(void);
void help_toggle(void);
void sprAnim_update(SprAnim *);
void menu_onPortraitsRead(u8 *);
void akao_relocInstruments(s32 *, AkaoInstrument *, u32, s32);
s32 akao_loadBankAt(s32 *, s32, s32, u32);
typedef struct {
    s32 w[4]; /* per window style (win_isVisible) */
} Unk80010CD0;
extern Unk80010CD0 g_winMinWidths; /* min widths per window style */
void akao_freeSubSeqVoices(void);
extern s32 g_akaoResidentStream[]; /* AKAO header copy (0x70 bytes) */
extern u8 g_chrClassFlags[];
void menu_onOverlayRead(u8 *);
extern AkaoChannel g_akaoSavedChannels[];
void akao_copy(); /* (u32 *src, u32 *dst, u32 size): word copy */
u16 akao_shiftInstrumentDown(u32, AkaoChannel *);
s32 text_initNameImage(s32);
void text_renderPartyName(s32);
extern RECT g_partyNameRect;
void akao_streamStartRing(void);
void akao_setVoicePitch(s32, s32);
void akao_setVoiceVolume(s32, u32, u32, s32);
u8 fade_isActive(void);
void menu_onFadedOut(void);
u16 pad_getFlags(void);
void pad_setFlags(u16, s32);
void pad_getAnalogThresholds(Unk4 *);
void pad_setAnalogThresholds(Unk4 *);
extern u8 g_padSavedStickFlag;
extern Unk4 g_padSavedThresholds;
extern Unk4 g_padStickDpadThresholds;
void menuScreen_drawCommands(void);
void menuScreen_updateOpen(void);
void akao_streamSetVoice(s32, s32, s32, s32);
void akao_streamKeyOn(s32, s32, void (*)(void));
s32 bgi_updateActor(u32); /* field overlay (ovl_0a) */
s32 fldbg_applyGeomOffset(void); /* field overlay (ovl_0a) */
s32 fldbg_resetGeomOffset(void); /* field overlay (ovl_0a) */
extern CdRequest g_cdReadRequest;
s32 akao_initSlide(s32 *, s32, s32, s32);
s32 CdMode(void);
s32 cd_startTransfer(); /* called with a u8 it ignores; callers see an int return (no prototype in the original) */
u8 timer_isEnabled(void);
void timer_draw(void);
void game_tickClock(void);
extern SprAnimData g_helpIconAnimData;
void sprAnim_init(SprAnim *, SprAnimData *);
s32 akao_checkHeader(s32 *);
extern u16 *g_akaoSeTrackOfs;
extern u16 *g_akaoSeBankIds;
extern u16 *g_akaoSeExtraCounts;
s32 akao_findBankSlot(s32);
void akao_getSeData(u8 **, u8 **, s32);
void akao_keyOn(u32);
extern u8 *g_akaoSeData;
extern s32 g_akaoLoadedBanks[];
void akao_streamPlayOneShot(s32, s32, s32, s32);
s32 preload_isBusy(PreloadEntry *);
u8 fade_clearEase(); /* called with an argument it ignores */
extern void (*g_menuStateFuncs[])(void);
extern s32 g_menuTypeOvl[];
void menu_readOverlay(void);
void menu_readPortraits(void);
typedef struct {
    u16 hp;
    u16 mp;
} Unk8006DDD0;
extern Unk8006DDD0 g_levelHpMp[];
typedef struct RainNode {
    struct RainNode *next; /* next */
    u8 pad4[0xC - 4];
} RainNode;
typedef struct {
    RainNode *freeList; /* free list */
    s32 active; /* active drop list (rain_drawDrops) */
    s32 count; /* drops spawned per frame, max 0x82 (rain_set) */
    s32 speed; /* fall speed * 16 (rain_set); 0xFF0 spawns higher */
    DR_TPAGE tpage; /*  */
    DR_TPAGE tpage2; /* copy of tpage (second draw buffer) */
    u8 pad20[0x24 - 0x20];
    s32 stopping; /* rain_set(0, speed): stop spawning and let the drops fall out */
} RainState;
extern u8 *g_fieldRainPrims;
extern u8 *g_worldRainPrims;
extern RainState g_fieldRain;
extern RainState g_worldRain;
void rain_spawn(s32);  /* hand-written asm module */
s32 rain_drawDrops(s32, u8, u32 *, s32);
void wrain_spawn(s32);
s32 wrain_drawDrops(s32, u8, u32 *, s32);
void chr_setLevel(s32, s32, u32 *, u8);
extern u32 g_chrInitInfo[];
extern u32 g_fadeBattleOt[];
s32 akao_loadBank(s32, s32);
void pen_init(void);
void msg_resetWindows(void);
void msg_resetPad(void);
void rand_init(s32);
void evt_reset(void);
extern u16 g_evtCurUnitMask;
void menu_finish(void);
void menu_loadOverlay(void);
void menuScreen_onSwitchFaded(void);
CdReadCtx *cdRead_init(void *, s32, s32, void *, s32, s32, void (*)(CdReadCtx *));
s32 cdRead_update(void *);
void preload_init(); /* takes no args, but ovl_0b passes one */
void preload_setHook(s32);
PreloadEntry *preload_add(void);
s32 preload_isReady(s32);
void preload_remove(s32);
s32 cd_cancel(void);
void cd_stopMotor(void);
s32 cd_readSync(); /* TODO: (sector, size, dst); ovl_14 passes a 4th argument */
s32 cd_isShellOpen(void);
u8 cd_checkShellOpen(void);
void game_initGfx(void);
void ui_drawBackground(void);
void win_drawShadow(RECT *, s32, s32);
s32 ui_drawSpriteDef(s32, s32, s32, void *, u8);
u8 *text_append(u8 *, u8 *);
s32 ui_measureLabel(u8 *);
s32 text_measureSmall(u8 *s);
s32 text_measureDigits(u8 *s);
typedef struct {
    u8 pad0[4];
    u8 advance; /* advance width */
    u8 pad5[3];
} Glyph8;
/* g_uiSprites ids of the label font (ui_measureLabel, text_buildLabel): 'A'-'Z' at c - 8, 'a'-'z' share them
 * (c - 0x28), '0'-'9' at c - 2, plus these three */
#define UI_SPRITE_AMPERSAND 0x38
#define UI_SPRITE_PERIOD 0xC2
#define UI_SPRITE_UNDERSCORE 0xDF
#define UI_SPRITE_AP_GAUGE 3 /* gauge frame ui_buildApGauge sizes the bar by */
#define UI_SPRITE_SMALL_DIGITS 0x22 /* '0'-'9', then '%' (10) and '/' (11) (ui_buildDigits) */
/* button glyphs (runtime pass: menu sprites rewritten to each id; g_buttonSprites = CROSS, CIRCLE, TRIANGLE, SQUARE) */
#define UI_SPRITE_CROSS    4
#define UI_SPRITE_CIRCLE   5
#define UI_SPRITE_SQUARE   6
#define UI_SPRITE_TRIANGLE 7
#define UI_SPRITE_L1       8
#define UI_SPRITE_L2       9
#define UI_SPRITE_R1       10
#define UI_SPRITE_R2       11
#define UI_SPRITE_DPAD     12 /* the d-pad; 13-16 are its small direction arrows (g_dpadArrowSprites) */
#define UI_SPRITE_START    17
#define UI_SPRITE_SELECT   18
/* ui_drawLabel / ui_buildLabel mode */
#define UI_LABEL_NORMAL 0 /* full brightness */
#define UI_LABEL_MASK   1 /* glyphs black and semi-transparent, CLUT 0x40 rows down: only the gradient behind shows
                           * through the letters (btlui_drawTranceLabel's TRANCE title) */
#define UI_LABEL_DIM    2 /* half brightness (any other value): win_drawTitle of an inactive window */
void ui_drawLabel(s32, s32, s32, s32, s32);
void ui_drawSmallDigits(s32, s32, s32, u8 *, s32);
void text_drawBlock(s32, s32, s32, u8 *, s32);
u8 *text_copy(u8 *, u8 *);
void gfx_setUiMode(s32);
void gfx_setBuffer(s32);
void pad_updateRepeat(s32);
void pad_setStickAsDpad(u8);
void win_setClip(RECT *, s32, s32);
Window *win_openFree(RECT *, s32, s32);
Window *win_init(RECT *, s32, s32, Window *);

/* Inlined copy of win_close (close window). */
static inline void win_close_inline(Window *w) {
    if (w != NULL && w->state != WIN_STATE_CLOSED && w->state != WIN_STATE_CLOSING) {
        w->state = WIN_STATE_CLOSING;
        if (g_drawBufs.uiMode != 2) {
            win_setCloseAnim(w, w->animMode);
        }
    }
}

void win_close(Window *);
void win_closeNow(Window *);
void win_closeAllNow(void);
void win_drawBox(RECT *, s32, s32);
void win_drawListBox(RECT *, s32, s32, MenuList *);
void win_setOnOpened(Window *, void (*)(Window *));
void win_setDrawContent(Window *, void (*)(Window *));
void win_setDrawOverlay(Window *, void (*)(Window *));
void win_setOnClosed(Window *, void (*)(Window *));
void win_setTitle(Window *, s32);
void win_setShadow(Window *, u8);
void win_setOpenFrames(Window *, s16);
void SystemError(s32, s32);
void menuList_update(MenuList *);
void win_stepOpen(Window *);
void win_stepClose(Window *);
MenuView *menuList_getView(MenuList *, MenuView *);
void menuList_init(MenuList *, s32, s32, s32, s32);
void menuList_input(MenuList *, s32);
void menuList_left(MenuList *, s32);
void menuList_right(MenuList *, s32);
MenuView *menuList_getPageView(MenuList *, MenuView *);
void menuList_getRowsRect(MenuList *, RECT *, RECT *);
void snd_playSe(s32);
void snd_startSe(s32);
void snd_stopSe(s32);
u8 fade_isBusy(void);
s32 fade_setOnDone(void (*)(void));
u16 fade_setAngle(u16);
void fade_inDefault(void);
void game_startFade(s32, s32, s32, s32);
/* menu_open(type, arg): MENU event op (0x75) type values (docs/r32/3ds-requests-3.md) */
#define MENU_MAIN 0       /* main menu */
#define MENU_NAME 1       /* name entry */
#define MENU_SHOP 2       /* shop, arg = shop id (ovl_3c; >= 0x20 synthesis) */
#define MENU_PARTY 3      /* party select (event op 0xB2, arg = request) */
#define MENU_SAVE 4       /* save / load (ovl_39); argument 1 = title Continue */
#define MENU_CHOCOGRAPH 5 /* chocograph */
#define MENU_DISC 8       /* "Change to DISC" (ovl_3e); argument (disc & 3) << 14 | field id */
void menu_open(s32, s32);
void menu_initNewGame(void);
void menu_initForMode(void);
void menu_setOnClose(void (*)(void));
void menu_setFadeInOnClose(u8);
void menu_setFadeOutOnClose(u8);
s32 chr_levelUp(s32, s32);
void party_setChanged(u8);
u8 chr_hasSupportAbility(s32, s32);
u8 chr_canLearnAbilities(Character *);
u8 chr_testSupportBit(u32 *, s32);
void help_setBounds(RECT *);
void help_drawCursor(); /* unprototyped: the argument is unused and most callers pass none */
void help_setCursorOnTop(u8);
void text_setBank(const u8 *);
u8 *text_get(s32); /* text lookup */
void evt_init(u8 *); /* start the field script system on a script data block */
void evt_initSystem(void);
void msg_init(void);
void evt_clearGlobalVars(void);
void gamedata_resetEventFlags(void);
s32 snd_init(void);
void akao_stopMusicNow(void);
void akao_stopAllSe(void);
s32 akao_beginBankUpload(void);
s32 akao_uploadBank(s32 *, u32, s32);
s32 CloseEvent(s32);
s32 DisableEvent(s32);
void card_initAll(void);
void card_resetGameResult(void);
s32 card_addWithStats(u8, u8, u8, u8, u8, u8);
s32 card_add(s32);
s32 card_countAll(void);
s32 card_countKinds(void);
s32 rand(void);
s32 card_getName(s32); /* number to string */
void card_addWin(void);
void card_addLoss(void);
void card_addDraw(void);
void card_setGameResult(s32);
FileEntry *dep_find(FileTable *, s32);
void strcat(u8 *, u8 *);
/* per-frame offset animation of a window (tween_start / tween_step) */
typedef struct TweenDef {
    u16 delay; /* start delay */
    s16 frames; /* frame count */
    s8 *dx; /* x deltas */
    s8 *dy; /* y deltas */
    void (*onDone)(struct TweenDef *); /* called when done */
} TweenDef;

/* text window (tween_start / tween_step) */
typedef struct {
    s16 delay; /* start delay */
    s16 frame; /* frame */
    RECT rect; /* moved by the tween's deltas */
    TweenDef *anim; /* running animation, NULL when idle */
} WinDef; /* 0x10 */
void tween_start(void *, void *, s32);
void gfx_initTpageSprt(DR_TPAGE *, s32);
void gfx_initTpageTile(DR_TPAGE *, s32);
void game_newGame(void);
s32 TestEvent(s32);
void _card_info(s32);
void _card_wait(s32);

/* ---- main batch 1 ---- */

typedef struct {
    u8 b[8];
} Unk8; /* 8 bytes, byte-aligned (copied with lwl/lwr) */
extern Unk8 g_cdResult; /* last CD status/result bytes */

s32 evt_callObjectFunc(EvtObject *, s32, s32, s32);
extern s32 g_evtBattleTriggerArg;
extern CdlATV g_akaoCdAtv;
void akao_markSeqDirty(); /* (AkaoSeq *, AkaoChannel *) */
void card_rollStats(CardEntry *, u8);
extern s32 g_akaoLfoWaves[];

typedef struct {
    s32 volume; /* fade-out volume of the crossfaded music (<< 16 of 0x7F), akao_cmdCrossfadeMusic */
    s32 volumeStep;
    s32 fadeLen; /* frames left; at 0 the old music stops (akao_tickCrossfade) */
    s32 savedVolume; /* volume of the fading-out sequence before akao_applyCrossfadeVolume scaled it (akao_restoreCrossfadeVolume) */
} AkaoCrossfade;
extern AkaoCrossfade g_akaoCrossfade;


extern AkaoChannel g_akaoChannels[];
extern s32 g_akaoMuteMask;
extern AkaoChannel g_akaoSeChannels[];
extern s32 g_akaoPauseFlags;

typedef struct {
    s32 activeMask; /* sound-effect channels running (bits 0x100 << i) */
    u32 keyOnMask;
    u32 playingMask;
    u32 keyOffMask;
    s32 pausedMask; /* active channels set aside while g_akaoPauseFlags & 2 */
    union {
        u32 w;
        u16 h[2];
    } tempo; /* tempo, 16.16 */
    u32 tempoAcc; /* tempo accumulator */
    u32 noiseMask; /* (SE ops C4/C5) */
    u32 reverbMask; /* (SE ops C2/C3) */
    u32 pitchModMask; /* (SE ops C6/C7) */
    u16 noiseClock; /* SpuSetNoiseClock while sound effects play (op AC) */
} AkaoSeState;
extern AkaoSeState g_akaoSeState;
void akao_opC4(AkaoChannel *, u32);
void akao_opC6(AkaoChannel *, u32);


typedef struct {
    void (*init)(u8); /* menu_start */
    void (*update)(u8); /* menu_updateRun */
    s32 exit; /* menu_finish */
} Unk8006C9D4; /* g_menuTypes: per menu_open type */
extern Unk8006C9D4 g_menuTypes[];
s32 win_resetClock(void);
void help_update(void);

extern s32 g_evtArgFlags;
extern s32 g_evtArgRead;
extern struct EvtObject *g_evtCurObj; /* current script object */
void evt_evalExpr(void);
s32 evt_popResult(void);
void evt_assignResult(s32);
extern s32 g_evtHalted;
void msg_update(void);
void evt_updateIdleTimer(void);
s32 evt_updateObjects(void); /* per-frame field object update */
extern s32 g_evtPlayerBlocked;
extern s32 g_evtPlayerMoved; /* the player moved this frame */
extern s32 g_evtPushFrames;
extern s32 g_evtPushable;
extern u8 g_worldPlayerStepped; /* world map (ovl_0c) data */
void evt_movePlayer(EvtObject *);
void evt_updateLookAt(EvtObject *);
void evt_checkPlayerAction(EvtObject *);
void evt_updateBattleObjects(void);
void evt_stepAnim(EvtObject *);
void msg_drawHereIcon(EvtObject *);
s32 evt_runScripts(EvtListNode *);
s32 math_length3(s32, s32, s32);
s32 evt_updateActorWalkmesh(EvtObject *);
s32 evt_rollEncounter(EvtObject *);
#if !defined(OVL_0A_H) /* field overlay functions; ovl_0a has the definitions */
s32 fld_enableRain(s32, s32, s32);
/* field walkmesh actor entry returned by ovl_0a bgi_getActor (ovl_0a calls it BgiActor) */
typedef struct {
    s16 walkFlags; /* EvtObject.savedWalkFlags (evt_updateObjects) */
    u8 pad2[6];
    u8 walkFloor; /* EvtObject.savedWalkFloor */
    u8 pad9;
    u16 walkTri; /* EvtObject.savedWalkTri */
} Unk800C748C;
void *bgi_getActor(s32);
s32 bgi_getActorRadius(s32, s32 *);
s32 bgi_revertActor(s32);
#endif
Window *msg_getWindow(s32);
void evt_followBalloon(EvtObject *, Window *);
s32 evt_angleToObject(EvtObject *, EvtObject *, s32);
void math_rotateSVector(SVECTOR *, s32, s32);
void msg_updatePlayerBubble(void);
void msg_drawAtePrompt(void);

u16 btl_getUnitMask(u32); /* ovl_0b code */
void menuScreen_startTitleAnim(void);
void menuScreen_requestOverlay(void);
void menuScreen_onOpenFaded(void);
void fade_outDefault(void);

void cd_startRequest(CdRequest *);
u8 cd_submit(CdRequest *);
void cd_onSync(u8, u8 *);
void cd_onDataReady(u8, u8 *);
s32 akao_remapInstrument(s32, s32);
s32 akao_remapSeInstrument(s32, s32);
void akao_setChannelProgram(AkaoChannel *, s32);
void akao_spuWrite(void *, s32);
void akao_waitSpuTransfer(void);
void akao_initState(void);
s32 akao_tick(void); /* root counter 2 callback; returns the ticks this tick took */
extern u8 g_akaoSpuMallocTable[];
void UnDeliverEvent(s32, s32);
void akao_keyOff(s32);
s32 StopRCnt(s32);
void SpuQuit(void);
extern s32 g_akaoTickEvent;
extern u8 g_akaoSilenceSample[];
extern s32 g_evtMode;
/* offset-table blob: count, then per-entry u16 offsets relative to unk4 */
typedef struct Unk8007B954 {
    s32 count; /* messages */
    u16 offsets[1]; /* per message, from &offsets (msg_getText) */
} Unk8007B954;
extern Unk8007B954 *g_msgTable;
extern u8 *g_msgTextBanks[];

typedef struct {
    u8 pad0[2];
    s16 unk2; /* not used by C */
    u8 pad4[9 - 4];
    u8 unk9; /* not used by C */
} Unk8004EFB0;

/* ---- main batch 2 ---- */

extern CdRequest g_cdRetryRequest;
extern CdRequest g_cdRestartRequest;
extern EvtObject *g_evtActor;

/* Same as evt_getArg8_inline, but written as `>>= (g_evtArgRead = 1)` (srav). */
static inline s32 evt_getArg8_inline2(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->ip++;
    }
    g_evtArgFlags >>= (g_evtArgRead = 1);
    return r;
}

/* Inlined copy of evt_resolveObjectId (resolve a script object reference). */
static inline EvtObject *evt_resolveObjectId_inline(s32 id) {
    if (id == 0xFF) {
        return g_evtActor;
    }
    if (id == 0xFA) {
        id = g_evtState->playerUid;
    } else if ((u32)(id - 0xFB) < 4) {
        id = g_evtState->partyUids[id - 0xFB];
    }
    return evt_getObject_inline(id);
}
extern s32 g_evtPlayerAnimCount;
extern s32 g_evtMsgCount;
extern s32 g_hereIconActive;
extern s32 g_hereIconTimer;
extern AkaoChannel *g_akaoSubChannels;

typedef struct {
    u8 pad0[4];
    s16 envelope; /* SpuGetVoiceEnvelope level; 0 = voice free (akao_findFreeVoice), 0x7FFF = reserved */
    u8 pad6[2];
} AkaoVoiceEnv;
extern AkaoVoiceEnv g_akaoVoiceEnv[24];
void akao_detachVoice(); /* (AkaoChannel *, s32 voice) */

typedef struct {
    s32 startR; /* start colour (FadeState.color, fadeWipe_start) */
    s32 startG;
    s32 startB;
    s32 endR; /* end colour (FadeState.endR-B) */
    s32 endG;
    s32 endB;
    s32 stepR; /* per-frame step of the first phase (delta / frames) */
    s32 stepG;
    s32 stepB;
    s32 step2R; /* per-frame step of the second phase (delta / frames2) */
    s32 step2G;
    s32 step2B;
    Rgb32 color; /* colour of the first phase (fadeWipe_update) */
    Rgb32 color2; /* colour of the second phase */
    Rgb32 out; /* output: copied to FadeState.color */
    s32 deltaR; /* end - start */
    s32 deltaG;
    s32 deltaB;
    u32 frames; /* first-phase frames */
    s32 frames2; /* second-phase frames */
} FadeWipe;
extern FadeWipe g_fadeWipe;

typedef struct {
    s32 unk0; /* not used by C */
    s32 unk4; /* not used by C */
} Unk8005AE14;

/* ---- main batch 3 ---- */
typedef struct {
    u8 pad0;
    u8 stones;
    u16 name; /* name offset */
    u8 pad4[4];
} Unk8006D128; /* support abilities, indexed by id - 0xC0 */
extern Unk8006D128 g_supportAbilities[];
void chr_setSupportAbility(s32, s32, u8);
extern s32 g_menuScreenOvl[];
extern RECT g_menuCmdRectInit;
extern MenuScreenDef g_menuScreensField[];
extern MenuScreenDef g_menuScreensWorld[];
u8 cd_readAsync(s32, s32, void *, void (*)(u8 *));
void menuScreen_onOverlayRead(u8 *);
extern void *g_evtUnitObjects[8];
#ifndef OVL_0B_H /* battle overlay code; ovl_0b.h has the real prototypes */
struct BtlUnit *btl_findUnitByBit(u16);
struct BtlUnit *btl_getUnitById(u16);
u32 btl_getUnitAttr(struct BtlUnit *, s32);
void btl_setUnitAttr(struct BtlUnit *, s32, u32);
#endif
void cd_onDiskError(s32);
void cd_onSyncQueued(u8);
extern void (*g_cdStateHandlers[])(s32);

/* ---- main batch 4 ---- */
u8 sprAnim_setPlaying(SprAnim *, u8);
void sprAnim_setOnDone(SprAnim *, void (*)());
void sprAnim_reset(SprAnim *);
void help_onIconAnimDone(void);
extern s32 g_msgDrawBufMode;
extern u8 *g_msgDrawPktStart;
extern u8 *g_msgBattlePktPtr;
extern u32 g_msgBattlePktBufs[];
extern u8 **g_penPktPtr;
void pen_putGlyph(s32);
extern u8 g_msgNameBuf[8]; /* name text being converted */
u32 pen_getGlyph(s32); /* glyph info of a text code, width in the top 4 bits */
s32 evt_getPartyChrIndex(s32);
void msg_measureLine(u8 **, s32 *); /* add the width of a text control code */

typedef struct {
    u8 *readPtr; /* stream read pointer */
    u8 *loopPtr; /* loop start (header flags & 2) */
    s32 flags; /* header +0x18 flags: AKAO_STREAM_* (snd_ids.h) */
    u32 voiceMask; /* voices the stream plays on (akao_streamStart), 0 = stopped */
    s32 voice; /* first of its two voices (left, right = voice + 1) */
    u32 bytesLeft; /* bytes left */
    u32 loopLength; /* loop length (initial), header +0x20 */
    u32 loopBytes; /* bytesLeft reloaded at the loop point (akao_streamRefill) */
    s32 unk20; /* write-only: header word +4 on a ring refill (akao_streamRefillRing), cleared by the ring setup */
    s32 blocksWritten; /* ring stream: blocks the producer reported (akao_streamPushBlock) */
    s32 blocksRead; /* ring stream: blocks consumed (akao_streamNextReadBlock) */
    s32 spuAddr; /* SPU address of the stream buffer (SpuSetTransferStartAddr, voice start/loop); the ring in RAM for the ring stream */
    s32 unk30; /* write-only: second word of the ring setup argument */
    s32 readBlock; /* ring stream: block the next upload reads (-1 before the start) */
    u32 writeBlock; /* ring stream: block the producer fills next, akao_streamStartRing runs once it reaches 2 */
    u32 ringBlocks; /* ring stream: blocks in the ring (sound command 0xE8 size >> 12) */
    s32 volume; /* volume (akao_cmdSetStreamVolume/E5, akao_streamSetVoice) */
    s32 volumeStep; /* (akao_cmdFadeStreamVolume fade) */
    s32 volumeFadeLen;
    s32 pan; /* pan in bits 8-15 (akao_cmdSetStreamPan) */
    u8 pad50[0x58 - 0x50];
    s32 pitch; /* header +0x1C, akao_setVoicePitch on both voices */
} AkaoStreamState;
u32 SpuWrite(u8 *, u32);
void akao_streamStartMono(void);
void akao_stopSeq(AkaoSeq *, AkaoChannel *, s32);
void SpuGetReverbModeType(s32 *); /* libspu SpuGetReverbModeType */
void akao_streamPlayResident(s32, s32);
void akao_streamStart(s32, s32, s32);
extern AkaoStreamState g_akaoStream;
s32 akao_streamNextReadBlock(); /* K&R: callers pass one or two arguments */
s32 akao_streamRefillRing(s32, s32, s32, void (*)(void));
void akao_setVoiceLoopAddr(s32, s32);
void akao_setReverbMode(s32 mode);
void akao_streamRefillRingA(void);
void akao_streamRingKeyOn(s32, s32, void (*)(void));
void akao_streamRefillRingB(void);

/* ---- main batch 5 ---- */
s32 preload_isUsed(PreloadEntry *);
PreloadEntry *preload_getCurrent(void);
void ui_loadPartyPortraits(u8 *);
u8 party_hasChanged(void);
void FlushCache(void);
extern u8 *g_randPtr;
extern u8 g_randTable[]; /* random byte table */

extern u8 g_randTableEnd[];
void rand_refill(void);

/* ---- hoisted for menu overlays ---- */
void ui_initMemberSwitch(MemberSwitch *, s32, void (*)(void));
void ui_startMemberSwitch(MemberSwitch *, s32, s32, s32);
void ui_updateMemberSwitch(MemberSwitch *);
void text_setStringArg(s32, u8 *);
void text_setIconArg(s32, s32);
u8 help_getFollow(void);
void help_setFollow(u8);
void menuScreen_open(s32);
s32 party_getPrevMember(s32);
s32 party_getNextMember(s32);
void ui_drawEtchedLine(s32, s32, s32, s32);
typedef struct {
    u8 id; /* ability id, 0 = none */
    u8 ap; /* AP needed to learn it (chr_learnAbility, chr_getApNeeded) */
} AbilityEntry;
extern AbilityEntry g_abilityRows[16][0x30]; /* rows indexed by Character.info.b.row */

/* Inlined copy of chr_findAbilitySlot (ability slot of a character). */
static inline s32 chr_findAbilitySlot_inline(s32 ch, s32 id) {
    s32 row;
    s32 i;
    if (ch >= 9) {
        return -1;
    }
    row = ((g_gameState->gameData->characters + ch)->info.w >> 12) & 0x1F;
    if (row >= 16) {
        return -1;
    }
    for (i = 0; i < 0x30; i++) {
        if (g_abilityRows[row][i].id == id) {
            return i;
        }
    }
    return -1;
}

void ui_drawGauge(s32, s32, s32, s32, s32, s32, u8);
/* gauge colour set per style (ui_drawGauge): c[2]/c[3] are the bar's left/right
   colours, c[0]/c[1] the blink targets; c[0] is also the frame colour */
typedef struct {
    CVECTOR c[4];
} GaugeColors;
extern GaugeColors g_gaugeColors[];
/* o = f + (t - f) * n / d per channel (o is a CVECTOR, f/t point to CVECTORs) */
#define CVECTOR_LERP(o, f, t, n, d)                         \
    (o).r = (f)->r + ((t)->r - (f)->r) * (n) / (d);         \
    (o).g = (f)->g + ((t)->g - (f)->g) * (n) / (d);         \
    (o).b = (f)->b + ((t)->b - (f)->b) * (n) / (d)
void ui_drawMemberHeader(s32, s32, s32, MemberSwitch *);
void ui_drawStatusIcons(s32, s32, s32, u8, s32); /* arg4 unused, callers pass 0 */
void ui_drawStones(s32, s32, s32, s32, s32);
void chr_setDefaultName(u8 *, s32); /* default name by id */
s32 text_getLength(u8 *); /* strlen */
extern u32 g_menuArg; /* = g_menu.arg, read on its own by ovl_3a */
extern u32 g_padPressed; /* buttons that went down this frame */
void ui_restorePortraits(void);
void ui_getPortraitRects(s32, RECT *, RECT *);
typedef struct {
    u32 *images; /* portrait image data, 0x6C0 bytes each */
    s32 count;  /* count */
} Unk80076F78;
extern Unk80076F78 g_portraits;
extern u32 g_portraitSavedVram[]; /* first CLUT, right after g_portraits */
s32 item_remove(s32, s32);
typedef struct {
    u8 pad0[4];
    u8 w; /* width */
    u8 pad5[3];
} Icon; /* sprite drawn by ui_drawSpriteDef */
extern Icon g_venomStatusSprite;
extern Icon g_koStatusSprite;
extern Icon g_petrifyStatusSprite;
extern s32 g_labelParty;
void ui_drawHand(s32, s32, s32);
void pad_setRepeatDelay(s32, s32, s32);
void pad_setRepeatInterval(s32, s32, s32);
s32 cd_getState(void);
s32 card_countOf(); /* defined K&R-style: u8 param, callers pass ints unmasked */
CardEntry *card_find(s32, s32);
s32 card_remove(s32, s32);
typedef struct {
    u8 power; /* power (x2) */
    u8 type; /* type */
    u8 physDef; /* physical defence (x2) */
    u8 magicDef; /* magic defence (x2) */
    u8 points; /* collector points */
} CardInfo; /* 5 bytes */
extern CardInfo g_cardInfo[]; /* card table */
CardInfo *card_getInfoTable(void);
s32 card_getRarity(s32);
extern s32 g_labelTrash;
extern s32 g_labelCardInfo;
void help_setLocked(u8);
s32 keyItem_has(s32);
s32 tween_sumDeltas(s32, s8 *);
extern s32 g_padPortState[];
u8 *ui_buildSprite(s32, s32, u32 *, s32, u8, s32 *, void *);
u8 *text_buildPlain(s32, s32, u32 *, Character *, s32 *, u8 *);
u8 *text_buildDpad(s32, s32, u32 *, u32, u8, s32 *, u8 *);
/* text_buildDpad mask: the arrows drawn (bit i = g_dpadArrowSprites[i]) */
#define DPAD_ARROW_UP 0x1
#define DPAD_ARROW_DOWN 0x2
#define DPAD_ARROW_LEFT 0x4
#define DPAD_ARROW_RIGHT 0x8
u8 *text_buildCode(s32, s32, u32 *, s32, s32 *, u8 *); /* draw a text control icon */
#if !defined(OVL_3A_H)
u8 *text_buildFontTpage(u32 *, DrTpageWords *);
#endif
typedef struct {
    u8 x; /* x offset of the d-pad part sprite (text_buildDpad) */
    u8 y;
    u8 sprite; /* g_uiSprites index */
} Unk8006BC0C;
extern Unk8006BC0C g_dpadArrowSprites[4];
u8 help_isOpen(void);
void ui_drawRule(s32, s32, s32, s32, s32, s32);
void menuList_down(MenuList *, u32);
void menuList_pageUp(MenuList *, s32);
void menuList_pageDown(MenuList *, s32);
void menuList_up(MenuList *, u32);
extern u32 g_padHeld; /* buttons held */
extern s32 g_labelConfig;
void chr_updateStats(Character *);
s32 chr_calcAppearance(s32, s32, u8 *);
void ui_loadPortraits(void *, s32);
void pad_repeatStub(void);
s32 nextfile(s32);
s32 close(s32);
s32 _card_load(s32);
s32 write(s32, s32, s32);
s32 read(s32, s32, s32);
void *card_getInventory(void);
extern s32 g_labelError;
extern s32 g_labelNotice;
void party_setMember(s32, s32);


/* ---- hoisted for the world map (ovl_0c) ---- */
s32 shadow_initSlots(s32, s32, s32, s32, s32, s32, s32);
#if defined(OVL_0A_H) || defined(OVL_0C_H) /* the field code only reads port 1 */
extern s32 g_padPortPressed;
#else
extern s32 g_padPortPressed[2]; /* went down this frame, per port */
#endif
extern s32 g_padPortReleased[2]; /* buttons released this frame */
extern s32 g_padPortPrevHeld[2]; /* previous g_padPortHeld */
extern s32 g_padHoldFrames[2][32]; /* frames each button has been held */
extern Unk4 g_padAnalog[2]; /* analog stick bytes */
extern u8 g_padActAlign[]; /* actuator alignment */
/* byte view of a g_padBuf pad buffer */
typedef struct {
    u8 status; /* status, 0 = ok */
    u8 unk1; /* not read by C */
    u16 buttons; /* buttons, active low */
    u8 analog[4]; /* analog sticks */
} PadBuf;
#define PADBUF(i) ((PadBuf *)&g_padBuf[i])
void pad_update(void);
void game_updateFade(void);
void game_updateTimer(void);
void game_setScreenHeight(s32);
s32 evt_update(); /* defined K&R-style: u8 param (unused); some callers pass none */
/* evt_execInstr results that end evt_runScripts' pass and come back from evt_updateObjects / evt_update (0 = none;
 * evt_execInstr's 1 only suspends the running object, 2 is a deleted object). Evidence: [code] evt_execInstr returns,
 * evt_runScripts (stops on 3-8), evt_updateObjects (6 -> 0), fld_main and wm_frameMain (WM_EVT_*) switch on them. */
#define EVT_UPD_BATTLE 3    /* EVT_OP_BATTLE / EVT_OP_BATTLE_EX: fld_main -> FLD_EXIT_BATTLE */
#define EVT_UPD_FIELD 4     /* EVT_OP_GOTO_FIELD: next field (or FLD_NEXT_ENDING) */
#define EVT_UPD_WORLD 5     /* EVT_OP_GOTO_WORLD: fld_main -> FLD_EXIT_WORLD */
#define EVT_UPD_END_FRAME 6 /* EVT_OP_END_FRAME: the pass resumes at g_evtResumeNode next frame; evt_update returns 0 */
#define EVT_UPD_CARD_GAME 7 /* EVT_OP_CARD_GAME: fld_main -> FLD_EXIT_TETRA_MASTER */
#define EVT_UPD_GAME_OVER 8 /* EVT_OP_GAME_OVER: fld_main -> FLD_EXIT_GAME_OVER */
u8 menuScreen_needsMember(s32);
s32 menuScreen_getCommandHelp(s32);
void menuScreen_close(void);
s32 vib_clear(void);
s32 rain_draw(void);
void akao_stopSe(s32, s32);
void akao_sendCmdE2(void);
s32 game_getPlayTime(void);
s32 text_formatTime(s32, u8 *, s32);
extern s32 g_labelTimeAndGil;
extern u8 g_itemNames[]; /* item names */
extern u8 g_keyItemNames[]; /* key item names */
typedef struct {
    s16 name; /* name offset into g_keyItemNames */
    s16 help; /* help offset: Key item help text (menuItem_updateHelp) */
    u8 pad4[2];
    s16 desc; /* description offset (menuItem_drawKeyItem) */
} KeyItemInfo;
extern KeyItemInfo g_keyItemTable[];
EvtListNode *evt_getObjectList(void);
extern s32 g_labelLocation;
void SetBackColor(s32, s32, s32);
void SetFarColor(s32, s32, s32);
void SetFogNearFar(s32, s32, s32);
s32 party_getFirstMember(void);
void printf(const char *, ...);

/* Inlined body of msg_closeAll for one entry (close and unlink). */
static inline void msg_closeAll_inline(MsgWindow *p) {
    MsgWindow *q;
    if (p != NULL && p->open != 0) {
        p->open = 0;
        if (msg_getFocus() == p) {
            msg_setFocus(p->parent);
        }
        for (q = g_msgWindows; q < g_msgWindows + 10; q++) {
            if (q->open != 0 && q->parent == p) {
                q->parent = NULL;
            }
        }
    }
}

/* Inlined copy of msg_closeWindow (close an entry, its window and its children). */
static inline void msg_closeWindow_inline(MsgWindow *p) {
    if (p != NULL && p->open != 0) {
        if (p->window != NULL) {
            win_close(p->window);
        }
        if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == p->id) {
            pad_setAnalogThresholds(&g_msgSavedAnalogThresholds);
        }
        msg_closeAll_inline(p);
    }
}
extern s32 g_msgPadRepeatTimer;

s32 evt_getAnimFrameCount();
s32 game_getModel(s32);

/* Inlined copy of evt_finishTurn. */
static inline void evt_finishTurn_inline(EvtObject *p) {
    s32 v;
    p->pos.rotY = (u16)p->turnTarget & 0xFFF;
    v = p->turnPrevAnim;
    if (v != p->anim) {
        p->anim = v;
        p->animFrame = 0;
        p->animSubFrame = 0;
        p->animFrames = evt_getAnimFrameCount(p->modelId, v);
        p->animSpeed = p->defaultAnimSpeed;
    }
    p->flags &= ~EVT_OBJ_TURNING;
}

s32 evt_getSysVar(s32);
extern u8 *g_worldAreaNames;
/* help-window rect plus frame style and x offset (help_layout) */
typedef struct {
    RECT rect; /*  */
    s16 style; /* HELP_TAIL_* (win_drawHelpFrame's style argument) */
    s16 tailX; /* x offset (win_drawHelpFrame) */
} HelpLayout;
/* help window tail (HelpLayout.style, win_drawHelpFrame style; help_layout picks it) */
#define HELP_TAIL_UP 0 /* window below the point, tail on its top edge */
#define HELP_TAIL_RIGHT 1 /* flag: the tail sits right of the window centre (window shifted left) */
#define HELP_TAIL_DOWN 2 /* window above the point, tail on its bottom edge */
#define HELP_TAIL_NONE 4 /* no tail (help_layout without a point to follow) */
void help_layout(HelpLayout *);
void sprAnim_draw(s32, s32, s32, SprAnim *);
s32 fade_stepClamp(s32, s32, s32);
s32 fade_ease(s32, s32, s32);
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineF2Words; /* LINE_F2 */
extern u32 g_winLineColors[][4]; /* line colours */
extern s8 g_winShadowLineOffsets[4][4]; /* shadow line offsets (win_drawLineBorder) */
typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} LineXY;
u8 *ui_buildSpriteDef(s32 x, s32 y, u32 *ot, SpriteDef *d, u8 flag, s32 *outWidth, SprtWords *s);
u8 *ui_buildDigits(s32 x, s32 y, u32 *ot, u8 *s, s32 unused, SprtWords *p);
/* Inlined copy of ui_buildSpriteDef (SPRT + DR_TPAGE from a sprite descriptor). */
static inline u8 *ui_buildSpriteDef_inline(s32 x, s32 y, u32 *ot, SpriteDef *d, u8 flag, s32 *out, SprtWords *s) {
    DrTpageWords *t;
    setlen(s, 4);
    if (flag) {
        s->rgbc = 0x66606060;
    } else {
        s->rgbc = 0x64808080;
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->uvclut;
    s->wh = d->w | (d->h << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = (d->tpage & 0x9FF) | 0xE1000600;
    addPrim(ot, t);
    *out = d->w;
    s = (SprtWords *)(t + 1);
    return (u8 *)s;
}
/* Inlined copy of evt_freeAllocNode (move an object's node to the free list). */
static inline void evt_freeAllocNode_inline(EvtObject *key) {
    EvtAllocNode *n;
    EvtAllocNode *prev = NULL;
    for (n = g_evtState->allocUsed; n != NULL; n = n->next) {
        if (n->start == key) {
            break;
        }
        prev = n;
    }
    if (prev != NULL) {
        prev->next = n->next;
    } else {
        g_evtState->allocUsed = n->next;
    }
    n->next = g_evtState->allocFree;
    g_evtState->allocFree = n;
}
void win_drawScrollArrows(RECT *, s32, MenuList *);
void win_drawSplitFill(RECT *, s32, MenuList *, s32);
void win_drawDivider(RECT *, s32, MenuList *, s32);
void win_drawLineBorder(RECT *, s32, u8, s32);
void win_drawFill(RECT *, s32, u8, u8, s32);
extern s32 g_akaoOutputFlags;
extern s32 g_akaoSeVolume; /* sound effect volume */
extern s16 g_akaoMonoGain;
extern s16 g_akaoPanCurve[]; /* pan curve, 256 entries */
/* item icon sprites (g_itemIconSprites) */
typedef struct {
    u8 u; /* u */
    u8 v; /* v */
    u16 clut; /* clut */
    u8 w; /* w */
    u8 h; /* h */
    u16 tpage; /* tpage */
} UiSprite;
extern UiSprite g_itemIconSprites[];
extern UiSprite g_winTailSprites[];
u8 win_isVisible(Window *);
void win_drawFrame(Window *);
void win_drawTitle(Window *);
/* sprite cell of a frame animation (sprAnim_drawCell) */
typedef struct {
    u8 u; /* u */
    u8 v; /* v */
    u8 x; /* x + 0x80 */
    u8 y; /* y + 0x80 */
    u32 clutTpage; /* clut | tpage bits << 16; bit 26 = scaled (sprAnim_draw) */
    u16 w; /* w */
    u16 h; /* h */
} SprCell;
/* debug/system text output (hand-written asm, 0x80051xxx) */
void pen_begin(void);
void pen_setPos(s32, s32);
void pen_move(s32, s32);
void pen_drawSprite(s32);
void pen_end(void);
extern s32 *g_evtExprSp; /* script value stack pointer */
extern void *g_evtExprObj; /* script current object */

/* Inlined copy of evt_getBattleVar (read script variable, refreshing 2-4). */
static inline u16 evt_getBattleVar_inline(s32 arg0) {
    s32 i = arg0 & 7;

    switch (i) {
    case 2:
        g_evtBattleVars[2] = btl_getUnitMask(0);
        break;
    case 3:
        g_evtBattleVars[3] = btl_getUnitMask(1);
        break;
    case 4:
        g_evtBattleVars[4] = btl_getUnitMask(2);
        break;
    }
    return g_evtBattleVars[i];
}
typedef struct {
    s32 lead; /* pending lead byte (>= 0xFA) of a two-byte code (pen_getGlyph) */
    s32 extRange; /* Font.extCode | extBase << 16 (msg_init) */
    s32 kanjiCode; /* first code of the second table (-1 = none) */
} Unk8007BB98;
extern Unk8007BB98 g_penGlyphDecode;
extern s32 g_msgChoiceDrawMask;
void msg_renderText(MsgWindow *, u8 *); /* draw a window's text */
extern DVECTOR *g_penStackPtr; /* pen position stack (pen_pushPos) */
typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } s;
} PenPos;
extern PenPos g_penPos; /* current pen position */
/* "!" balloon sprite (msg_drawHereIcon) */
typedef struct {
    u8 u; /* u */
    u8 v; /* v */
    u16 clut; /* clut */
    u8 w; /* w */
    u8 h; /* h */
} Unk80068790;
extern Unk80068790 g_hereIconSprite;
extern s32 g_hereIconFlipped; /* balloon flipped (pen x < 24) */
extern u8 g_msgIconSprites[];
void pen_setX(s32);
void pen_setLastSemiTrans(void);
s32 msg_emitNumber(s32, s32);
s32 msg_emitString(u8 *, s32);
s32 msg_emitItemName(s32, s32);
s32 msg_emitCharName(s32, s32);
s32 msg_emitCharNameCaps(s32, s32);
void msg_advanceText(MsgWindow *, u8 *);
void msg_stepControl(MsgWindow *, u8 *);
void pen_pushPos(void);
void pen_popPos(void);
void pen_setPalette(s32);

/* Inlined lookup of message i in the current offset table (g_msgTable). */
static inline u8 *msg_drawText_msg(s32 i) {
    u8 *e = NULL;
    u16 *t;

    if (i >= 0 && i < g_msgTable->count) {
        t = g_msgTable->offsets;
        e = (u8 *)t;
        e += t[(g_evtMode == 2 || g_evtMode == 4) ? i * 2 : i * 4 + 2];
    }
    return e;
}
/* AP learned by a character for ability id, -1 if not learnable (inlined helper). */
static inline s32 chr_findAbilitySlot_ap_inline(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->gameData->characters[ch].ap[i];
}

/* AP needed to master ability id (inlined form of chr_getApNeeded). */
static inline s32 chr_getApNeeded_inline(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *c = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    return g_abilityRows[c->info.b.row][i].ap;
}
void akao_setVoiceStartAddr(s32, s32);
void akao_setVoiceDecay(s32, s32);
void akao_setVoiceSustainLevel(s32, s32);
/* streamed AKAO sound header (akao_streamPlayOneShot) */
typedef struct {
    s32 magic; /* "AKAO" */
    s32 unk4; /* only copied to g_akaoStream.unk20 */
    u8 pad8[0x10 - 0x8];
    u32 dataSize; /* data size */
    u32 loopStart; /* loop start offset */
    s32 flags; /* flags: AkaoStreamState.flags */
    u16 pitch; /* voice pitch (AkaoStreamState.pitch) */
    u8 pad1E[0x20 - 0x1E];
    u32 loopLength; /* loop length */
    u8 pad24[0x40 - 0x24];
    u8 data[1]; /* sample data */
} AkaoStream;
s32 akao_allocStreamVoices(void);
void akao_streamOneShotStart(void);
void akao_streamStartStereo(void);
void akao_streamUploadSecond(void);
u8 *text_buildGlyph(s32, s32, u32 *, u32, s32, s32 *, SprtWords *);
void win_drawTail(Window *);
s32 chr_calcBaseSpeed(s32, s32, u8);
s32 chr_calcBaseStrength(s32, s32, u8);
s32 chr_calcBaseMagic(s32, s32, u8);
s32 chr_calcBaseSpirit(s32, s32, u8);
s32 chr_calcMaxHp(s32, s32);
s32 chr_calcMaxMp(s32, s32);
s32 chr_calcMaxStones(s32, s32, u8);
void chr_fitSupportAbilities(Character *);
POLY_FT4 *pen_addPolyFT4(void);
typedef struct {
    u8 u; /* u */
    u8 v; /* v */
    u16 clut; /* clut */
} Unk80068788;
extern Unk80068788 g_bubbleSprite;
extern s32 g_bubbleColor; /* colour */
extern s32 g_bubbleHalfWidth; /* half width */
extern s32 g_bubbleHeight; /* height */
extern s32 g_bubbleLift;
extern s32 g_evtCameraChanged;
extern u8 g_bubbleScriptOpenAlt[];
extern u8 g_bubbleScriptOpen[];
extern u8 g_bubbleScriptClose[];
EvtObject *evt_getObject(s32);
void evt_setDrawPosAtObject(EvtObject *, s32, s32);
void msg_drawPlayerBubble(void);
/* zero the first three bytes (r, g, b) of a 4-byte colour */
static inline void fade_init_clear3(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
}
void pen_endFont(void);
void msg_drawText(MsgWindow *);
/* 8-byte message-table entry (g_msgTable in modes other than 4) */
typedef struct {
    u32 unk0; /* not read by C */
    u16 unk4; /* not read by C */
    u8 lines; /* text lines: window height = lines * 14 + 6 (msg_getEntry) */
    u8 unk7; /* not read by C */
} MsgEntry8;

/* SPU hardware voice registers (0x1F801C00 + voice * 16) */
typedef struct {
    u16 volL; /* volume left */
    u16 volR; /* volume right */
    u16 pitch; /* pitch */
    u16 startAddr; /* start address */
    u16 adsr1; /* adsr 1 */
    u16 adsr2; /* adsr 2 */
    u16 adsrVol;
    u16 loopAddr; /* loop address */
} SpuVoiceReg;
extern s16 g_akaoCdVolumeHi;
extern s32 g_akaoCdVolumeStep;
s32 akao_isqrt(s32);
extern s32 g_akaoTempoScaleStep;
extern s32 g_akaoPitchScaleStep;
extern AkaoArgs g_akaoOpSeArgs;
/* vibration track (g_vib.data + tracksOfs), [0] = actuator 1 (largeMotor), [1] = actuator 0 (smallMotor) as vib_update sums them */
typedef struct {
    u16 flags[2]; /* bit 0: enabled (vib_selectTrack, vib_setTrackEnabled) */
    u16 length[2]; /* last frame with data */
    u16 dataOfs[2]; /* per-frame strengths, from g_vib.data */
} VibTrack;
extern u32 g_akaoStreamVoices;
/* voice parameter block written to the SPU by akao_writeVoice */
typedef struct {
    u8 pad0[4];
    s32 dirty;
    u32 startAddr;  /* start address */
    u32 loopAddr;  /* loop address */
    u16 pitch; /* pitch */
    u16 adsr1; /* adsr 1 */
    u16 adsr2; /* adsr 2 */
    u16 volScale; /* volume scale */
    s16 volL; /* volume left */
    s16 volR; /* volume right */
} AkaoVoiceRegs;
typedef struct {
    void (*start)(FadeState *);
    s32 update;
    s32 draw;
} Unk8006BD04; /* g_fadeTypes */
extern Unk8006BD04 g_fadeTypes[];
extern u8 g_winStyleOpenAnim[]; /* WIN_STYLE_* -> WIN_ANIM_* (win_init) */
typedef struct {
    u8 b[8];
} Bytes8;
extern Bytes8 g_cardArrowBits; /* card arrow bits (card_rollStats) */
typedef struct {
    u8 b[5];
} Bytes5;
typedef struct {
    u32 w[2];
} Words2;
/* stat calculation input (chr_calcEquipStats) */
typedef struct {
    Bytes8 base; /* = Character baseMaxHp, baseMaxMp, baseSpeed..baseSpirit */
    Words2 support; /* Character.supportAbilities (Auto HP/MP+ bits 0xC5-0xC8) */
    u16 hp;
    u16 mp;
    Bytes5 equip; /* equipment */
} EquipStatsIn;
/* stat calculation output (chr_calcEquipStats) */
typedef struct {
    u16 hp; /* hp */
    u16 mp; /* mp */
    u16 maxHp; /* max hp */
    u16 maxMp; /* max mp */
    u8 stats[4]; /* stats: speed, strength, magic, spirit (capped by g_chrStatCaps) */
    u16 defs[5]; /* attack (weapon), defense, evade, magicDef, magicEva (capped at 999) */
} EquipStatsOut;
EquipStatsOut *chr_calcEquipStats(EquipStatsIn *, EquipStatsOut *);

/* Inlined copy of chr_updateStats (recompute a character's stats). */
static inline void chr_updateStats_inline(Character *c) {
    EquipStatsIn in;
    EquipStatsOut out;

    in.base = *(Bytes8 *)&c->baseMaxHp;
    in.hp = c->hp;
    in.mp = c->mp;
    in.support = *(Words2 *)c->supportAbilities;
    in.equip = *(Bytes5 *)c->equip;
    chr_calcEquipStats(&in, &out);
    c->speed = out.stats[0];
    c->strength = out.stats[1];
    c->magic = out.stats[2];
    c->spirit = out.stats[3];
    c->defense = out.defs[1];
    c->evade = out.defs[2];
    c->magicDef = out.defs[3];
    c->magicEva = out.defs[4];
    c->hp = out.hp;
    c->mp = out.mp;
    c->maxHp = out.maxHp;
    c->maxMp = out.maxMp;
}
extern void (*g_fadeSeekFuncs[])(FadeState *, s32);
extern s32 g_msgReady;
extern u32 *g_penGlyphs;
extern s32 g_msgFontUnusedParam;
void win_closeAll(void);
void msg_resetChoices(void);
extern u8 *g_evtScripts; /* script data */
/* 8-byte entry of the field script directory at g_evtScripts */
typedef struct {
    u16 offset; /* offset from g_evtScripts */
    u16 size; /* size */
    u8 varBytes; /* local-variable bytes of the object (evt_createObject: (varBytes + 3) >> 2 words) */
    u8 flags; /* flags; bit 0 clear: evt_loadScripts compacts it */
    u8 pad6[2];
} ScriptEntry;

u8 chr_getClassFlags(u32);
void chr_clearAbilities(s32);
s32 chr_getDefaultSet(Character *);
void chr_setDefaultEquip(u8 *, s32);
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4Half; /* POLY_F4 with the colour/code written as one word */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineF2W; /* LINE_F2 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc0;
    u32 xy0;
    u32 rgb1;
    u32 xy1;
    u32 rgb2;
    u32 xy2;
    u32 rgb3;
    u32 xy3;
} PolyG4Words; /* POLY_G4 written as whole words */
typedef struct {
    u32 tag;
    u32 code[2];
} DrMode2Words; /* DR_MODE / DR_TWIN */
extern UiSprite g_uiBackgroundSprite;
extern UiSprite g_winFillSprite;
/* window frame tables (win_buildBorder) */
extern u32 g_winEdgeRgbc[2]; /* edge rgbc, by flag */
extern u32 g_winCornerRgbc[2]; /* corner rgbc, by flag */
extern u8 g_winCornerSprites[][4]; /* corner sprites (g_uiSprites index) per style */
extern u8 g_winHEdgeSprites[][2]; /* top/bottom edge sprites per style */
extern u8 g_winVEdgeSprites[][2]; /* left/right edge sprites per style */
extern u8 g_winRuleSprites[][3]; /* title bar sprites (left, right, middle) per style */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
} Sprt8Words; /* SPRT_8 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u16 clut;
} Sprt8Fields; /* SPRT_8 with rgb+code as one word */
typedef struct {
    u16 uv;
    u16 clut;
    u8 w;
    u8 h;
    u16 tpage;
} SpriteDesc16; /* SpriteDef with uv and clut as halves */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 uv0clut;
    u32 xy1;
    u32 uv1tpage;
    u32 xy2;
    u32 uv2;
    u32 xy3;
    u32 uv3;
} PolyFT4Words; /* POLY_FT4 written as whole words */

u16 win_getClut(s32, s32);
extern RECT g_noTexWindow;
void akao_channelsToVoiceMask(AkaoChannel *, u32 *, u32, u32);
/* ---- main: boot, system setup, TIM helpers ---- */

/* GameState.config bits (the Config screen rows, ovl_34; runtime field/menu campaign) */
#define CFG_BIT_SOUND         0  /* 1 = mono */
#define CFG_BIT_CONTROLLER    1  /* 1 = custom button mapping (GameState.unk28) */
#define CFG_BIT_CURSOR        2  /* 1 = memory */
#define CFG_BIT_ATB           3  /* 1 = wait */
#define CFG_BIT_CAMERA        4  /* 1 = fixed */
#define CFG_BIT_BTL_CONTROL   5  /* 1 = custom: controller port per character (CFG_SHIFT_CHAR_PORT bits) */
#define CFG_BIT_MOVEMENT      6  /* 1 = run */
#define CFG_BIT_VIBRATION     7  /* 1 = off */
#define CFG_SHIFT_BTL_SPEED   8  /* 3 bits, 0..CFG_BTL_SPEED_MAX */
#define CFG_SHIFT_BTL_MSG_SPEED 11 /* 3 bits: battle message speed (g_btlBannerDurations index). Set only by config_reset (3);
                                   * the NTSC-U Config screen has no row for it (runtime) */
#define CFG_SHIFT_MSG_SPEED   14 /* 3 bits, 0..CFG_MSG_SPEED_MAX */
#define CFG_BIT_HERE_ICON     17 /* 1 = off */
#define CFG_BIT_WINDOW_COLOR  18 /* 1 = classic */
#define CFG_BIT_TARGET_WINDOW 19 /* 1 = always */
#define CFG_SHIFT_CHAR_PORT   20 /* one bit per character: 1 = controller port 2 */
#define CFG_BTL_SPEED_MAX     2
#define CFG_MSG_SPEED_MAX     6
#define CONFIG_WINDOW_COLOR() ((g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1) /* CFG_BIT_WINDOW_COLOR: the window palette (pal/abr) of all window drawing */

/* GameState.config/padMap viewed as a pair: the config menu's settings (saved by ovl_39) */
typedef struct {
    u32 soundMono : 1; /* Config rows (runtime field/menu campaign, ovl_34): Sound */
    u32 padCustom : 1; /* Controller: Custom (buttonMap is used, config_remapButton) */
    u32 cursorMemory : 1;
    u32 atbWait : 1;
    u32 battleCameraFixed : 1;
    u32 battleControlCustom : 1;
    u32 movementRun : 1;
    u32 vibration : 1;
    u32 battleSpeed : 3;
    u32 battleMsgSpeed : 3; /* bits 11-13 (CFG_SHIFT_BTL_MSG_SPEED): ovl_0b's battle message speed, no Config row */
    u32 messageSpeed : 3;
    u32 unk0_17 : 15; /* CFG_BIT_HERE_ICON, CFG_BIT_WINDOW_COLOR, CFG_BIT_TARGET_WINDOW, CFG_SHIFT_CHAR_PORT.. */
    u32 buttonMap; /* 8 x 4 bits: game button i -> pad button (GameState.padMap) */
} GameConfig;

/* GameState.dirs[15]: FF9.IMG directory records, copied from the header (file_initImg) */
typedef struct DirInfo {
    s32 type; /* kind; 3 = table of u16 ids (docs/ff9img.md) */
    s32 count; /* entry count */
    s32 tableSector; /* sector of the directory's file table in FF9.IMG (file_loadDirTables reads the tables from sector 0 and indexes them by (tableSector - dirs[0].tableSector) << 11) */
    s32 firstSector; /* first sector of the directory's files (dir 13: the battle data base, btl_initMode) */
} DirInfo;

/* file_loadDirTables: 8-byte entries of directory lists 8 and 11 */
typedef struct {
    u16 id; /*  */
    u16 link; /* list-8 entries: id of a list-11 entry; file_loadDirTables reuses it in list 11 as a mark (1 needed, 2 copied) */
    u16 unk4; /* not read by C */
    u16 unk6; /* not read by C */
} Unk8001DE50;

/* file_loadDirTables: 16-byte records copied out of directory list 11 */
typedef struct {
    u8 pad0[8];
    u16 unk8; /* write-only (file_loadDirTables sets 0xFFFF) */
    u8 padA[6];
} Unk8001DE50b;

/* PreloadEntry.fileId split into its parts */
typedef struct {
    u16 unk0; /* not used by C */
    u8 unk2; /* not used by C */
    u8 unk3; /* not used by C */
} FileIdParts;

/* TIM image: header, optional CLUT block (flag bit 3), then the pixel block */
typedef struct {
    u32 bnum; /* block size in bytes, header included */
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} TimBlock;

typedef struct {
    u32 id;
    u32 flag;
    TimBlock clut;
} TimHeader;

extern u32 g_allocTop; /* bump allocator for the boot-time state block */
void game_allocState(void);
void game_resetState(void);
s32 cd_init(void);
void CdSetDebug(s32);
void _bu_init(void);
void pad_init(void);

s32 game_loadCommonAkao(void);
void game_allocData(void);
void game_resetData(void);
void game_clearModelSlot(); /* (ModelSlot *): resets a slot; ovl_0b declares it with void * */
s32 game_loadCommonImages(void);
s32 game_loadCommonDb(void);

s32 CdGetSector(void *, s32);
s32 preload_isLoaded(PreloadEntry *);
void cd_onSectorReady(void);

extern SpuReverbAttr g_reverbAttr;
extern s32 g_reverbMode; /* reverb mode */
extern DRAWENV g_freezeDrawEnv;
void pad_resetState(void);
void cd_resetState(void);
void db_init(void);
void mbg_resetState(void);
void menu_init(void);
void game_resetStub(void);

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

/* first sector of FF9.IMG: directory records copied to GameState.unk14 */
typedef struct {
    u8 pad0[8];
    s32 dirCount; /* directory count */
    u8 padC[4];
    DirInfo dirs[15];
} Ff9ImgHeader;


extern u32 g_configButtons[8];
extern s32 g_configPadButtons[];
extern u32 g_configExtraButtons[1]; /* extra button masks (parallel to g_configExtraSlots) */
extern s32 g_configExtraSlots[1]; /* their action indices */
void pad_setMapping(s32, u32, s32);
extern s32 g_padMapping[2][32]; /* indexed by leading-zero count (pad_setMapping) */
extern s32 g_padHoldFrames[2][32];
extern s32 g_padPortPrevHeld[];
extern s32 g_padPortReleased[];

void __main(void);
void game_loadModeOverlay(s32);
void FlushCache(void); /* BIOS FlushCache */
s32 EnterCriticalSection(void); /* BIOS EnterCriticalSection */
void ExitCriticalSection(void); /* BIOS ExitCriticalSection */
void game_quitData(void);
void game_quitState(void);
void game_shutdown(void);
void game_freezeScreen(void);

/* overlay load addresses by game mode (game_loadModeOverlay). The rodata block
 * g_modeOverlayAddrs also holds the next word and libgpu's version string. */
typedef struct {
    u32 *addr[10]; /* per GameState.mode */
} LoadAddrs;
extern LoadAddrs g_modeOverlayAddrs;

/* battle calculation context (fieldCalc_checkAlive ... chr_addMp) */
typedef struct {
    u32 info; /* as ovl_0b BtlCmd8's first word: bits 0-3 target type (fieldCalc_multiTargetHalf), 8-16 effect id */
    u8 pad4;
    u8 power; /* power */
    u8 pad6[2];
    u8 statusMask; /* checked against the target by chr_cureStatusMask: status bits cured (fieldCalc_cureStatus) */
    u8 statusSet; /* index into fieldCalc_cureStatusType's status mask table */
} MainCalcCmd;

typedef struct {
    Character *caster; /* caster */
    Character *target; /* target */
    MainCalcCmd *cmd; /* ability/item data */
    s32 all; /* nonzero: the item/ability hits the whole party (item_useOnMember last argument; fieldCalc_multiTargetHalf) */
    u8 flags; /* MAINCALC_MISS */
    u8 result; /* MAINCALC_RESULT_*; write-only */
    s16 power; /* base power (cmd power) */
    s16 defense; /* target magicDef (magic heals), 0 for items */
    s16 multiplier; /* level-based (fieldCalc_setMagicHeal) or 10 for items, halved/boosted */
    s16 hpChange; /* chr_addHp amount */
    s16 mpChange; /* chr_addMp amount */
} MainCalc;
/* BtlCmd8.targetType (bits 0-3 of MainCalc.cmd->info) / Battle16CC.targetType values. Evidence: [code] g_btluiDefaultTargets (side 0xFF = the
 * ability's defaultSide with side switching, 1 party, 0 enemies; targetAll) and g_btluiTargetAllByType, rows 0-13;
 * btlui_startTargetSelect (canSwitchSide for 0/3/6/9, canToggleAll for 3-5). 9-11 are 6-8 again, but the command's
 * targeting is 2 instead of the target count (btlui_getTargeting, btlui_confirmDoubleCast; Charge!, Spear). */
#define BTL_TGT_SINGLE_ANY  0x0 /* one unit, either side (Life, Esuna, Scan, Haste) */
#define BTL_TGT_SINGLE_PARTY 0x1 /* one party member (Vanish, Auto-Life, Chakra) */
#define BTL_TGT_SINGLE_ENEMY 0x2 /* one enemy (Fire, Comet, Goblin Punch) */
#define BTL_TGT_MULTI_ANY   0x3 /* one or all of a side, spread halves it (Cure, Fire, Sleep) */
#define BTL_TGT_MULTI_PARTY 0x4
#define BTL_TGT_MULTI_ENEMY 0x5
#define BTL_TGT_ALL_ANY     0x6 /* a whole side, either */
#define BTL_TGT_ALL_PARTY   0x7 /* the party (Carbuncle, Mighty Guard, White Wind) */
#define BTL_TGT_ALL_ENEMY   0x8 /* every enemy (the summons, LV5 Death) */
#define BTL_TGT_EVERYONE 0xC /* every targetable unit is marked (btlui_setDefaultTargets) */
#define BTL_TGT_SELF     0xD /* the user only: no cursor (btlui_setDefaultTargets, btlui_cycleTarget) */
#define MAINCALC_RESULT_MISS 0x20 /* MainCalc.result: set by fieldCalc_apply on MAINCALC_MISS (the battle's BTL_POPUP_MISS bit) */
#define MAINCALC_MISS 0x1 /* MainCalc.flags: nothing happens (full hp, dead target, cure failed); fieldCalc_apply then changes nothing */

u8 chr_hasStatus(Character *, s32);
s32 chr_cureStatusMask(Character *, s32);
u8 fieldCalc_apply(MainCalc *);
u8 fieldCalc_checkAlive(MainCalc *);
u8 fieldCalc_checkDead(MainCalc *);
s32 menuItem_callChocobo(void); /* ovl_37 */
void exit(s32); /* BIOS B0 call */

void chr_addHp(Character *, s32);
void chr_addMp(Character *, s32);

/* 12-byte entry of a type-0x12 resource table (db_findModelInfo) */
typedef struct {
    u16 id; /* id */
    u16 unk2; /* not read by C */
    u32 dataOfs : 24; /* data offset from the entry */
    u32 texCount : 8; /* texture count (db_getModelTexCount, db_getModelTexPos) */
    u32 unk8; /* not read by C */
} DbEntry12;

extern TimHeader g_timBuffer;
extern s32 g_penLineStart;

void text_formatInt(s32, u8 *, s32);

/* Inlined copy of text_formatNumber (number to fixed-width text). */
static inline void text_formatNumber_inline(s32 n, u8 *buf, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 q;
    s32 d;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        text_formatInt(n, buf, ascii);
        return;
    }
    memset(buf, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        buf[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = buf + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}
extern s32 g_hourHundredsGlyphs[];

extern u8 g_fadePktBufs[2][0x200]; /* fade packet buffers, one per draw buffer */

typedef void (*FadeFn)(FadeState *);

/* frame animation cells (sprAnim_draw) */
typedef struct {
    s16 count; /* cell count */
    s16 offset; /* byte offset of the first cell in unk10 */
} AnimCellList;

typedef struct {
    SprCell cell;
    u8 padC[0x10 - 0xC];
    u16 scaleX; /* x scale, 4096 = 1.0 (sprAnim_drawScaledCell) */
    u16 scaleY; /* y scale */
} AnimCell;

void sprAnim_drawScaledCell(s32, s32, s32, SprAnim *, AnimCell *);


/* dialog window layout (msg_getLayout / evt_getMsgLayout) */
typedef struct {
    s16 x; /* x */
    s16 w; /* w */
    s16 tailOffset; /* MsgWindow.tailOffset (msg_open), the balloon tail x (evt_placeBalloon) */
    u16 tailMode; /* flags: MsgWindow.tailMode, 0x10/0x20 from the message flags */
    s8 y; /* y */
    u8 h; /* h */
    u8 anchor; /* anchor: evt_getMsgLayout placement, & 3 = balloon side */
    u8 flags; /* WINLAYOUT_* */
} WinLayout;
#define WINLAYOUT_NO_FOCUS 0x1 /* msg_open doesn't make the window g_msgFocus (TEXT_NO_INPUT in the message header) */
/* WinLayout.anchor: TEXT_WIN_* or */
#define WINLAYOUT_ANCHOR_NONE 0x00 /* no header code: centred on the screen, or a balloon placed automatically */
#define WINLAYOUT_ANCHOR_POS 0xFF /* TEXT_WIN_POS */
s32 msg_overlapsRect(WinLayout *r);
extern s32 g_msgLayoutAltOpen;
s32 msg_input(MsgWindow *, s32);
void msg_drawWindowContent(Window *);
void msg_drawChoiceCursor(Window *);

WinLayout *msg_getLayout(s32, s32 *, s32 *, s32 *);
extern WinLayout g_msgLayout;
void evt_placeBalloon(EvtObject *, WinLayout *, s32, s32);
extern s32 g_evtBalloonOfsX;
extern s32 g_evtBalloonOfsY;
extern s32 g_evtBalloonOfsZ;


s32 tween_step(WinDef *);

/* AKAO sequence header (akao_cmdPlaySeBank) */
typedef struct {
    u8 pad0[4];
    s32 trackCount; /* track count */
    s32 id; /* id */
    u8 padC[4];
    u32 trackOfs[1]; /* track offsets */
} AkaoSeqHdr;



/* ---- event script interpreter (evt_execInstr) ---- */
/* Event script opcodes (first byte of an instruction run by evt_runScripts / evt_execInstr; 0xFFxx = FF xx).
 * Opcodes seen running in the field fixture campaign (docs/runtime-findings.md, opcode table) plus the
 * scheduler ops evt_runScripts handles itself. Comment: arguments: effect. */
#define EVT_OP_JMP 0x01 /* s16 ofs: scheduler: ip += ofs */
#define EVT_OP_JMP_IFNOT 0x02 /* s16 ofs: scheduler: pop expression result, jump when 0 */
#define EVT_OP_JMP_IF 0x03 /* s16 ofs: scheduler: jump when non-zero */
#define EVT_OP_RETURN 0x04 /* scheduler: evt_returnFromFunc */
#define EVT_OP_EXPR 0x05 /* expr: scheduler: evt_runExpr (assignments, variable writes) */
#define EVT_OP_SWITCH 0x06 /* n, default, {value, ofs}[n]: scheduler: switch on the popped value */
#define EVT_OP_NEW_CODE 0x07 /* entry, uid: evt_createObject kind 2 (0x14-byte header) */
#define EVT_OP_NEW_REGION 0x08 /* entry, uid: evt_createObject kind 3 (0x38), flags 1 */
#define EVT_OP_NEW_ACTOR 0x09 /* entry, uid: evt_createObject kind 4 (0xAC) with actor defaults */
#define EVT_OP_SWITCH_TABLE 0x0B /* u8 n, base, default, ofs[n]: scheduler: dense jump table indexed by value - base */
#define EVT_OP_SWITCH_TABLE16 0x0D /* u16 n, base, default, ofs[n]: scheduler: the same with a 16-bit count */
#define EVT_OP_CALL 0x10 /* level, obj, func: evt_callObjectFunc(obj, level, func, 0) */
#define EVT_OP_CALL_WAIT_FREE 0x12 /* level, obj, func: retries until obj.level > level, then calls */
#define EVT_OP_CALL_SYNC 0x14 /* level, obj, func: calls with save=1 (caller waits, wait=0xFF) */
#define EVT_OP_CALL_CALLER 0x16 /* level, func: EVT_OP_CALL aimed at the object that called the current function */
#define EVT_OP_CALL_CALLER_WAIT_FREE 0x18 /* level, func: EVT_OP_CALL_WAIT_FREE on the caller */
#define EVT_OP_CALL_CALLER_SYNC 0x1A /* level, func: EVT_OP_CALL_SYNC on the caller */
#define EVT_OP_DELETE_OBJ 0x1C /* obj (0xFF self): scheduler: evt_deleteObject */
#define EVT_OP_SET_POS_XZ 0x1D /* x, z: pos/prevPos = (x, -0x8000, z): placed on the floor */
#define EVT_OP_BG_SCROLL_LIMITS 0x1E /* cam, 4 x s16: fldbg_setScrollLimits */
#define EVT_OP_MSG_WAIT 0x1F /* win, flags, msg: msg_open + wait (obj.wait = 0xFE) */
#define EVT_OP_MSG 0x20 /* win, flags, msg: msg_open, no wait */
#define EVT_OP_MSG_CLOSE 0x21 /* win: msg_close */
#define EVT_OP_WAIT 0x22 /* u8 frames: scheduler: obj.wait = n-1 (max 0xFD) */
#define EVT_OP_MOVE_XZ 0x23 /* x, z: evt_moveObjectTo(x, 0, z, 0), repeats until there */
#define EVT_OP_MOVE_TO_OBJ 0x24 /* obj: evt_moveObjectTo the object */
#define EVT_OP_MOVE_RESET 0x25 /* loop count 0xFF, stops the walk state */
#define EVT_OP_SET_WALK_SPEED 0x26 /* u8: actor.speed */
#define EVT_OP_BG_MASK_FILTER 0x27 /* u8: bgi_setMaskFilter */
#define EVT_OP_MOVIE 0x28 /* cmd, u16, u8: fld_movieCommand */
#define EVT_OP_SET_REGION 0x29 /* n, {x,z}[n]: EvtRegion polygon (n <= 8) tested by evt_isInRegion; 0xA4 uses pts[0..1] as a line */
#define EVT_OP_BATTLE 0x2A /* swirl, u16 battle: evt_setNextBattle, returns 3 */
#define EVT_OP_GOTO_FIELD 0x2B /* u16 field: evt_setNextLocation, returns 4 */
#define EVT_OP_SET_PLAYER 0x2C /* playerUid = current object */
#define EVT_OP_DISABLE_CONTROL 0x2D /* controlEnabled = 0, HERE icon off, msg/anim counters cleared */
#define EVT_OP_ENABLE_CONTROL 0x2E /* controlEnabled = 1, msg_startHereIcon(1), idle timer re-rolled */
#define EVT_OP_SET_MODEL 0x2F /* u16 model, u8 height: actor.modelId/modelSlot/height */
#define EVT_OP_PEN_CELL 0x32 /* a, b: scheduler: pen_setCell(a, b) */
#define EVT_OP_SET_STAND_ANIM 0x33 /* u16: actor.standAnim */
#define EVT_OP_SET_WALK_ANIM 0x34 /* u16: actor.walkAnim */
#define EVT_OP_SET_RUN_ANIM 0x35 /* u16: actor.runAnim */
#define EVT_OP_SET_DIR 0x36 /* u8 dir: rotY = dir << 4 */
#define EVT_OP_SET_ROT_XZ 0x37 /* u8, u8: rotX, rotZ */
#define EVT_OP_MESH_SHOW 0x39 /* obj, group: hiddenGroups &= ~bit (fld_drawModels draws groups whose bit is clear) */
#define EVT_OP_MESH_HIDE 0x3A /* obj, group: hiddenGroups |= bit */
#define EVT_OP_ENCOUNTER_TABLE 0x3C /* u8, 4 x u16: g_evtEncounterTable */
#define EVT_OP_SET_ANIM_RANGE 0x3D /* start, end: actor anim frame range */
#define EVT_OP_SET_ANIM_SPEED 0x3E /* u8: actor.defaultAnimSpeed */
#define EVT_OP_SET_ANIM_LOOP 0x3F /* mode, count: anim flags (loop/bounce) and loop count */
#define EVT_OP_ANIM 0x40 /* u16 anim: evt_execInstr_anim (play once) */
#define EVT_OP_WAIT_ANIM 0x41 /* retries while the anim plays (flags 0x80) */
#define EVT_OP_ANIM_END 0x42 /* evt_onAnimEnd */
#define EVT_OP_RUN_THREAD 0x43 /* func: kind-1 object uid+0x40 running a function of this object */
#define EVT_OP_WAIT_THREAD 0x44 /* retries while the uid+0x40 thread exists */
#define EVT_OP_KILL_THREAD 0x45 /* deletes it */
#define EVT_OP_SET_MOVE_FLAGS 0x47 /* u8: actor flags 0-2 */
#define EVT_OP_ADD_ITEM 0x48 /* u16 id, u8 n: item / key item / card */
#define EVT_OP_SET_RADIUS 0x4B /* walkmesh r, collision r, talk r: bgi radius, actor.collisionRadius, actor.talkRadius */
#define EVT_OP_ATTACH 0x4C /* obj, parent, bone: model attach */
#define EVT_OP_DETACH 0x4D /* obj */
#define EVT_OP_WAIT_TURN 0x50 /* retries while turning (flags 0x80) */
#define EVT_OP_TURN_TO_OBJ 0x51 /* obj, speed: evt_turnObject toward obj */
#define EVT_OP_SET_IDLE_ANIM 0x52 /* u16: actor.idleAnim */
#define EVT_OP_MSG_WAIT_WIN 0x54 /* win: waits for window */
#define EVT_OP_SET_MOVE_TURN 0x55 /* u8: actor turn rate while walking (0 = 0xFF) */
#define EVT_OP_TURN 0x56 /* dir, speed: evt_turnObject */
#define EVT_OP_ENCOUNTER_RATE 0x57 /* u8: state.encounterRate (0 clears the chance) */
#define EVT_OP_BG_LAYER_COLOR 0x59 /* layer, r, g, b: fldbg_setOverlayColor */
#define EVT_OP_BG_LAYER_MOVE 0x5A /* fldbg_moveOverlay */
#define EVT_OP_BG_LAYER_SHOW 0x5B /* layer, on: fldbg_showOverlay */
#define EVT_OP_BG_LAYER_LOOP 0x5C /* fldbg_setOverlayLoop */
#define EVT_OP_BG_LAYER_PARALLAX 0x5D /* fldbg_setOverlayParallax */
#define EVT_OP_BG_LAYER_POS 0x5E /* fldbg_setOverlayPos */
#define EVT_OP_BG_ANIM_FRAME 0x5F /* fldbg_setAnimFrame */
#define EVT_OP_BG_ANIM_PLAY 0x60 /* fldbg_playAnim */
#define EVT_OP_BG_ANIM_SPEED 0x61 /* fldbg_setAnimSpeed */
#define EVT_OP_BG_ANIM_DELAY 0x63 /* fldbg_setAnimStepDelay */
#define EVT_OP_BG_ANIM_FLAGS 0x64 /* fldbg_setAnimFlags */
#define EVT_OP_BG_ANIM_RANGE 0x65 /* fldbg_playAnimRange */
#define EVT_OP_MSG_NUMBER 0x66 /* slot, u16: msg_setNumber (value printed by a text code) */
#define EVT_OP_SET_DIR_OFFSETS 0x67 /* analog, pad: state.analogDirOfs/padDirOfs = arg << 4 */
#define EVT_OP_PLAYER_BUBBLE 0x68 /* variant: msg_showPlayerBubble: 0 "?", 1 "!" */
#define EVT_OP_WALK_ONLY 0x6A /* state.walkOnly = 1 */
#define EVT_OP_CLEAR_COLOR 0x6B /* r, g, b: game_setClearColor */
#define EVT_OP_BG_SCROLL_TO 0x6F /* x, y, frames, mode: fldbg_scrollTo */
#define EVT_OP_BG_SCROLL_TO_PLAYER 0x70 /* fldbg_scrollToPlayer */
#define EVT_OP_BG_FOLLOW_HEIGHT 0x72 /* s16: fldbg_setFollowHeight */
#define EVT_OP_BG_LIMITS_OFF 0x74 /* fldbg_disableScrollLimits */
#define EVT_OP_MENU 0x75 /* type, arg: menu_open + evt_onMenuOpen, returns 1 (runtime: Moogle "Save" -> menu_open(4, 0), ovl_39 loads) */
#define EVT_OP_REGION_START 0x76 /* x, z: EvtRegion = 2 points, both (x, z) */
#define EVT_OP_REGION_MOVE_LAST 0x77 /* x, z: overwrite the last point (pts[0] when count is 0) */
#define EVT_OP_REGION_DUP_LAST 0x78 /* append a copy of the last point (when count is 1-7) */
#define EVT_OP_SET_TURN_LEFT_ANIM 0x7A /* u16: actor prevPos.rotX field reused as turn-left anim */
#define EVT_OP_SET_TURN_RIGHT_ANIM 0x7B /* u16 */
#define EVT_OP_BG_CAMERA 0x7E /* u8 cam: fldbg_setCamera */
#define EVT_OP_SHADOW_ON 0x7F /* clears MODEL_SLOT_NO_SHADOW (evt_modelOp*) */
#define EVT_OP_SHADOW_OFF 0x80 /* sets MODEL_SLOT_NO_SHADOW */
#define EVT_OP_SHADOW_SIZE 0x81 /* u8, u8: evt_modelOpField */
#define EVT_OP_SHADOW_FIXED 0x83 /* u8: MODEL_SLOT_SHADOW_FIXED + placement */
#define EVT_OP_SHADOW_OFFSET 0x82 /* s16 x, s16 z: the shadow offset (Unk11C0Slot.ofsX/ofsZ) */
#define EVT_OP_SHADOW_UNFIX 0x84
#define EVT_OP_SET_IDLE_SPEEDS 0x86 /* 4 x u8: actor.idleAnimSpeeds */
#define EVT_OP_SET_DIR_OBJ 0x87 /* obj, dir: 0x36 on another object */
#define EVT_OP_MODEL_CMD 0x88 /* cmd, 3 x s16: fld_modelCommand */
#define EVT_OP_SOUND_POS 0x89 /* x, y, z, u8: fld_getSoundPanVolume -> sysvars 22/23 (EVT_SYSVAR_SOUND_VOLUME/PAN) */
#define EVT_OP_SOUND_POS_OBJ 0x8A /* obj, u8 */
#define EVT_OP_SET_LOOK 0x8B /* u8, u8: look-at params, flags |= 3 */
#define EVT_OP_OBJ_COLOR 0x8F /* obj, r, g, b: fld_setObjectColor (retries while fading) */
#define EVT_OP_IDLE_OFF 0x90 /* state.idleTimer = -1 */
#define EVT_OP_SET_TALK_TURN 0x91 /* u8: actor turns toward the talker (0xA7) or not */
#define EVT_OP_BG_FOLLOW_LAYER 0x92 /* 7 args: fldbg_addFollowOverlay */
#define EVT_OP_SET_OBJ_FLAGS 0x93 /* u8: flags 0-5 of +0xF */
#define EVT_OP_SET_JUMP_ANIM 0x94 /* u16 anim, start, end: actor.jumpAnim / jumpAnimStart / jumpAnimEnd */
#define EVT_OP_MSG_OBJ_WAIT 0x95 /* obj, win, flags, msg: message anchored on obj */
#define EVT_OP_MSG_OBJ 0x96 /* obj, win, flags, msg */
#define EVT_OP_SET_MOVE_COUNT 0x98 /* u8: actor loop/move count */
#define EVT_OP_SET_TURN_SPEED 0x99 /* u8: actor.turnSpeed */
#define EVT_OP_BGI_TRI_ACTIVE 0x9A /* u16 tri, on: bgi_setTriangleActive */
#define EVT_OP_TURN_TO_POINT 0x9B /* x, z */
#define EVT_OP_ANIM_FWD 0x9C /* jump anim forward */
#define EVT_OP_ANIM_BACK 0x9D
#define EVT_OP_MODEL_SCALE 0x9F /* obj, x, y, z: model scale (x64), actor.scale */
#define EVT_OP_SET_POS 0xA1 /* x, y, z: pos/prevPos */
#define EVT_OP_MOVE_XYZ 0xA2 /* x, y, z: evt_moveObjectTo flags 2 (flying) */
#define EVT_OP_MOVE_XZ_NOTURN 0xA5 /* x, z: evt_moveObjectTo flags 1 */
#define EVT_OP_BGI_ACTOR_ACTIVE 0xA8 /* u8: bgi_setActorActive */
#define EVT_OP_OBJ_SCREEN_POS 0xA9 /* obj: evt_projectToScreen -> sysvars 12/13 (EVT_SYSVAR_SCREEN_X/Y) */
#define EVT_OP_PAD_UNLOCK_MENU 0xAA /* g_padPortMask[0] &= ~0x40000 */
#define EVT_OP_PAD_LOCK_MENU 0xAB /* |= 0x40000 */
#define EVT_OP_SET_POS_OBJ 0xAD /* obj, x, y, z */
#define EVT_OP_SPRITE_CMD 0xB3 /* 5 args: fld_spriteCommand */
#define EVT_OP_SET_RECRUITED 0xB4 /* u16 mask: chr_setRecruited per bit */
#define EVT_OP_GOTO_WORLD 0xB6 /* u16 world: evt_setNextLocation, returns 5 (runtime: field_ice_300 walking down: 0xB6(9000) -> mode 3, ovl_0c, wm_main(0x2328)) */
#define EVT_OP_ANIM_OBJ 0xBD /* obj, anim */
#define EVT_OP_WAIT_ANIM_OBJ 0xBE /* obj */
#define EVT_OP_SET_POS_XZ_OBJ 0xBF /* obj, x, z */
#define EVT_OP_TEX_ANIM_PLAY 0xC0 /* obj, n: model texture anim n on */
#define EVT_OP_BG_LAYER_VIEWPORT 0xC3 /* fldbg_setOverlayViewport */
#define EVT_OP_SOUND 0xC5 /* u16 cmd, u16 id: GameData.soundHook(cmd, id) */
#define EVT_OP_BG_VIEWPORT 0xC9 /* fldbg_setViewport */
#define EVT_OP_BG_ANIM_RESET 0xCA /* fldbg_resetAnim */
#define EVT_OP_BGI_FLOOR_ACTIVE 0xCB /* floor, on: bgi_setFloorActive */
#define EVT_OP_MODEL_FLAGS_SET 0xCC /* u16 */
#define EVT_OP_MODEL_FLAGS_CLEAR 0xCD /* u16 */
#define EVT_OP_ATE_PROMPT 0xD7 /* mode: msg_setAtePrompt */
#define EVT_OP_RAIN 0xD8 /* count, speed: fld_enableRain(0,0,1) + rain_set, GameData.rainCount */
#define EVT_OP_CURE_STATUS 0xD9 /* chr, bits: chr_cureStatusBit */
#define EVT_OP_JUMP_STEP 0xDC /* one jump frame, retries until done */
#define EVT_OP_PARTY_REMOVE 0xDD /* chr */
#define EVT_OP_JUMP 0xE2 /* x, y, z, frames: starts a jump (jumpTarget, jumpFrames) */
#define EVT_OP_MSG_SYNC_TAG 0xE3 /* u8: g_msgSyncTag (EVT_SYSVAR_MSG_SYNC_TAG) */
#define EVT_OP_BG_LAYER_SKEW 0xE4
#define EVT_OP_BG_ANIM_SHOW_FRAME 0xE7 /* fldbg_showAnimFrame */
#define EVT_OP_GET_SCROLL 0xEA /* fldbg_getScroll -> sysvars 12/13 (EVT_SYSVAR_SCREEN_X/Y) */
#define EVT_OP_FADE 0xEC /* 6 args: game_startFade */
#define EVT_OP_BG_VIEWPORT_RATE 0xED
#define EVT_OP_HERE_ICON 0xEF /* u8: msg_startHereIcon */
#define EVT_OP_SET_HP 0xF1 /* character, value: set HP (capped at maxHp); runtime: the Moogle's Tent */
#define EVT_OP_SET_MP 0xF2 /* character, value: set MP (capped at maxMp); runtime: the Moogle's Tent */
#define EVT_OP_GAME_OVER 0xF5 /* field game over: returns 8 -> field exit 7 -> mode 7 (runtime: patched into the Dali Moogle script) */
#define EVT_OP_VIB_SET_PLAYING 0xF7 /* u8: vib_setPlaying */
#define EVT_OP_VIB_TRACK 0xF8 /* 3 x u8: vib_selectTrack */
#define EVT_OP_PARTY_JOIN 0xFE /* chr_join */
/* more opcodes, named from their evt_execInstr handlers (code) */
#define EVT_OP_BTL_KEEP_MUSIC 0x1B /* u8: GD_BTL_KEEP_MUSIC on/off */
#define EVT_OP_SET_ACTOR_TYPE 0x3B /* u8: EvtObject.actorType */
#define EVT_OP_REMOVE_ITEM 0x49 /* u16 id, u8 n: item_remove / keyItem_remove */
#define EVT_OP_SET_BATTLE_VAR 0x4A /* u8 var, u16 value: btl_setBattleVar */
#define EVT_OP_MSG_KEEP_WINDOWS 0x53 /* msg_keepWindowsOnInit */
#define EVT_OP_BG_MOVE_ORIGIN 0x58 /* 3 x u16: fldbg_moveOrigin */
#define EVT_OP_TIMER_SET 0x69 /* u16 seconds: timer_setSeconds */
#define EVT_OP_BG_FOLLOW 0x71 /* 3 x u8: fldbg_setFollow */
#define EVT_OP_BG_LIMITS_ON 0x73 /* fldbg_enableScrollLimits */
#define EVT_OP_MSG_CHOICE_MASK 0x7C /* u16, u8: msg_setChoiceMask */
#define EVT_OP_TIMER_RUN 0x7D /* u8: timer_setRunning */
#define EVT_OP_BATTLE_EX 0x8C /* u8 swirl, u8 enemy group, u16 battle | 0x8000 dark: GameData.darkBattle, evt_setNextBattle; result 3 */
#define EVT_OP_TIMER_ENABLE 0x8D /* u8: timer_enable */
#define EVT_OP_MSG_CLEAR_OTZ 0x8E /* msg_clearWindowOtz */
#define EVT_OP_RESTART_OBJ 0x97 /* obj: empty its call stack and start its entry 1 again (EVT_STATE_INIT, evt_returnFromFunc) */
#define EVT_OP_SET_RUN_SPEED 0xA6 /* u8: EvtObject.runSpeed */
#define EVT_OP_TURN_TO_ANGLE 0xA7 /* u8 angle >> 4: evt_turnObject at turnSpeed (the talk turn script runs it) */
#define EVT_OP_DISC_CHANGE 0xAC /* u16 disc << 14 | location: fld_requestDiscChange */
#define EVT_OP_CARD_GAME 0xAE /* u16 opponent: evt_setCardOpponent; result 7 (Tetra Master) */
#define EVT_OP_SET_LOCATION_NAME 0xB0 /* u16 text: copied to g_evtLocationNameBuf, GameData.locationName */
#define EVT_OP_RESET_LOCATION_NAME 0xB1 /* GameData.locationName back to the event data one (g_evtLocationName) */
#define EVT_OP_ANIM_SYNC 0xB5 /* obj: copy that actor's stand/walk/run animation (itself: stop) */
#define EVT_OP_WM_CAMERA_EYE_ACTOR 0xB7 /* world map: the actor moves to the camera eye and drives it (EVT_MOVE_WM_CAMERA_EYE) */
#define EVT_OP_WM_CAMERA_TARGET_ACTOR 0xB8 /* world map: the same for the camera target (EVT_MOVE_WM_CAMERA_TARGET) */
#define EVT_OP_PAD_MASK_SET 0xB9 /* u8 port, u16 bits: g_padPortMask |= */
#define EVT_OP_PAD_MASK_CLEAR 0xBA /* u8 port, u16 bits: g_padPortMask &= ~ */
#define EVT_OP_TEX_ANIM_PLAY_ONCE 0xC1 /* obj, u8 n: TEXANIM_ACTIVE | TEXANIM_ONESHOT */
#define EVT_OP_TEX_ANIM_STOP 0xC2 /* obj, u8 n: clears TEXANIM_ACTIVE | TEXANIM_ONESHOT */
#define EVT_OP_WM_SET_PARAM 0xC4 /* u8 param, u16 value: w_frameSetParameter */
#define EVT_OP_SOUND_ARG 0xC6 /* u16 cmd, u16 id, u24 arg: GameData.soundHook(cmd, id, arg) */
#define EVT_OP_SOUND_ARG_PAN 0xC7 /* u16 cmd, u16 id, u24 arg, u8 pan: GameData.soundHook */
#define EVT_OP_SOUND_ARG_PAN_VOL 0xC8 /* u16 cmd, u16 id, u24 arg, u8 pan, u8 volume: GameData.soundHook */
#define EVT_OP_ADD_GIL 0xCE /* u24: GameData.gil, at most GIL_MAX */
#define EVT_OP_REMOVE_GIL 0xCF /* u24: GameData.gil, at least 0 */
#define EVT_OP_BTL_BANNER 0xD0 /* u16 text: btl_setBanner */
#define EVT_OP_CLEAR_GLOBALS 0xD1 /* evt_clearGlobalVars */
#define EVT_OP_ATTACH_OFFSET 0xD4 /* 3 x u16: the model's offset from its parent bone (EVT_OP_ATTACH) */
#define EVT_OP_DISABLE_ACTORS 0xD5 /* saves every actor's flags, clears EVT_OBJ_ENABLED unless EVT_OBJ_KEEP_ENABLED */
#define EVT_OP_RESTORE_ACTORS 0xD6 /* EVT_OBJ_ENABLED back from the flags EVT_OP_DISABLE_ACTORS saved */
#define EVT_OP_SET_CHR_NAME 0xDE /* u8 character, u16 text: Character.name */
#define EVT_OP_SET_COLLISION_ELLIPSE 0xDF /* u8: EvtObject.collisionEllipse */
#define EVT_OP_ADD_FROG 0xE0 /* GameData.frogCount++ (at most 0x7FFF) */
#define EVT_OP_REWARDS_CLOSE 0xE1 /* rewards_requestClose, g_evtHalted */
#define EVT_OP_BTLSEQ_START 0xE5 /* u8: btlseq_startDirect(g_evtBattleVars[1], g_evtBattleVars[0], arg) */
#define EVT_OP_BG_OVERLAY_SCREEN 0xE6 /* u8, u8: fldbg_setOverlayScreen */
#define EVT_OP_MSG_CLOSE_ALL 0xEB /* msg_resetWindows, win_closeAll */
#define EVT_OP_IDLE_TIMER_RESET 0xEE /* EvtState.idleTimer = 0 */
#define EVT_OP_WALK_ONLY_OFF 0xF0 /* state.walkOnly = 0 */
#define EVT_OP_RESET_AP 0xF3 /* u8 character, u8: chr_resetAp */
#define EVT_OP_LEARN_ABILITY 0xF4 /* u8 character, u8 ability: chr_learnAbility */
#define EVT_OP_VIB_PLAY 0xF6 /* u8 frame: vib_play */
#define EVT_OP_VIB_TRACK_ENABLE 0xF9 /* 3 x u8: vib_setTrackEnabled */
#define EVT_OP_VIB_RATE 0xFA /* u16: vib_setRate */
#define EVT_OP_VIB_LOOP_MODE 0xFB /* u8: vib_setLoopMode (VIB_LOOP_MODE bits) */
#define EVT_OP_VIB_RANGE 0xFC /* u8 start, u8 end: vib_setRange */
#define EVT_OP_PRELOAD 0xFD /* u8, u16: GameData.preloadRequest */
#define EVT_OP_NOP_46 0x46 /* no arguments, does nothing (as 0x79, 0xD2, 0xD3) */
#define EVT_OP_NOP_79 0x79
#define EVT_OP_NOP_D2 0xD2
#define EVT_OP_NOP_D3 0xD3
#define EVT_OP_NOP_A3 0xA3 /* 4 x u8, read and ignored */
#define EVT_OP_NOP_STRING 0x4E /* NUL-terminated string + variable reference, skipped */
#define EVT_OP_END_FRAME 0x4F /* u8 ignored: returns 6, which ends this frame's evt_runScripts pass (resumed from g_evtResumeNode) */
#define EVT_OP_BTL_ENEMY_CMD 0x38 /* u8: btl_setEnemyCmd for the battle vars' unit, by the function level: 0 BTL_CMD_ENEMY_DEATH,
                                   * 1 BTL_CMD_ENEMY_COUNTER, 3 BTL_CMD_ENEMY_TURN */
#define EVT_OP_SET_ROW 0x62 /* u8 character, u8: CHR_INFO_BATTLE_ROW */
#define EVT_OP_PLAYER_TO_LINE_POINT 0x9E /* the player runs g_evtPlayerToLinePointScript (EVT_OP_MOVE_RESET, EVT_OP_MOVE_TO_LINE_POINT, EVT_OP_RETURN)
                                          * as a pushed call; control off, every object's collisions off */
#define EVT_OP_MOVE_TO_LINE_POINT 0xA0 /* evt_moveObjectTo(g_evtLinePointX, 0, g_evtLinePointZ), repeats until there */
#define EVT_OP_LINE_POINT 0xA4 /* g_evtLinePointX/Z = the point of the region's line pts[0]-pts[1] nearest the player */
#define EVT_OP_PARTY_MENU 0xB2 /* u8 size, u16 fixed-character mask: the party menu (menu_open(MENU_PARTY, &g_evtPartyRequest)) */
#define EVT_OP_TURN_OBJ 0xBB /* obj, dir, speed: EVT_OP_TURN on another object */
#define EVT_OP_WAIT_TURN_OBJ 0xBC /* obj: EVT_OP_WAIT_TURN on another object */
#define EVT_OP_SPRITE_CMD_U8 0xDA /* u8, u8, u8, u16, u16: fld_spriteCommand (EVT_OP_SPRITE_CMD with a u8 third argument) */
#define EVT_OP_SET_VICTORY_POSE 0xDB /* u8 character, u8: CHR_INFO_VICTORY_POSE */
#define EVT_OP_MOVE_XYZ_NOTURN 0xE8 /* x, y, z: evt_moveObjectTo EVT_MOVETO_NO_TURN | EVT_MOVETO_3D */
#define EVT_OP_PARTY_UPDATE_UIDS 0xE9 /* EvtState.partyUids rebuilt from the party (evt_execInstr_party) */
#define EVT_OP_EXT 0xFF /* u8 op: the 0xFFxx opcodes below */
#define EVT_OP_BGI_FLOOR_MOVE_START 0xFF00 /* u8, u8: bgi_startFloorMove */
#define EVT_OP_BGI_FLOOR_MOVE_STOP 0xFF01 /* u8, u8: bgi_stopFloorMove */
#define EVT_OP_BGI_FLOOR_MOVE_PLAYING 0xFF02 /* u8, u8: bgi_setFloorMovePlaying */
#define EVT_OP_BGI_FLOOR_MOVE_MODE 0xFF03 /* u8, u8: bgi_setFloorMoveMode */
#define EVT_OP_BGI_FLOOR_MOVE_FLOOR 0xFF04 /* u8, u8: bgi_setFloorMoveFloor */
#define EVT_OP_BGI_FLOOR_MOVE_SPEED 0xFF05 /* u8, u16: bgi_setFloorMoveSpeed */
#define EVT_OP_BGI_FLOOR_MOVE_CURVE 0xFF06 /* u8, u8: bgi_setFloorMoveCurve */
#define EVT_OP_BGI_FLOOR_MOVE_RANGE 0xFF07 /* u8, u16, u16: bgi_setFloorMoveRange */
#define EVT_OP_BGI_FLOOR_MOVE_AXIS 0xFF08 /* u8, u8: bgi_setFloorMoveAxis */
#define EVT_OP_BGI_ANIM_FRAME 0xFF09 /* u8, u8: bgi_setAnimFrame */
#define EVT_OP_BGI_ANIM_SHOW_FRAME 0xFF0A /* u8, u8: bgi_showAnimFrame */
#define EVT_OP_BGI_ANIM_PLAYING 0xFF0B /* u8, u8: bgi_setAnimPlaying */
#define EVT_OP_BGI_ANIM_MODE 0xFF0C /* u8, u8: bgi_setAnimMode */
#define EVT_OP_BGI_ANIM_RATE 0xFF0D /* u8, u16: bgi_setAnimRate */
#define EVT_OP_BGI_ANIM_FRAME_VALUES 0xFF0E /* u8, u8: bgi_setAnimFrameValues */
#define EVT_OP_BGI_ANIM_FRAME_VALUE 0xFF0F /* u8, u8, u8: bgi_setAnimFrameValue */
#define EVT_OP_BGI_ANIM_RANGE 0xFF10 /* u8, u8, u8: bgi_playAnimRange */
#define EVT_OP_BGI_ANIM_RESET 0xFF11 /* u8, u8: bgi_resetAnim */
/* evt_moveObjectTo flags (a negative value is instead an object to stop at) */
#define EVT_MOVETO_NO_TURN 0x1 /* keep the facing */
#define EVT_MOVETO_3D 0x2 /* move in y too (pitch) */
/* function tags in a script entry's {u16 tag, u16 ofs} table (evt_callObjectFunc key) */
#define EVT_FUNC_INIT 0   /* runs when the object is created */
#define EVT_FUNC_LOOP 1   /* main loop, started when init returns */
#define EVT_FUNC_RANGE 2  /* evt_checkPlayerAction: player inside the region / touching the actor (while control is enabled) */
#define EVT_FUNC_TALK 3   /* evt_checkPlayerAction: Cross (or Square without an EVT_FUNC_SQUARE) on the actor / region */
#define EVT_FUNC_SQUARE 8 /* evt_checkPlayerAction: Square, tried before EVT_FUNC_TALK */
/* expression bytes (evt_runExpr, jump table 0x8006E718; C equivalent in src_equiv/main/script_text.c); bytes >= 0x80 are
 * variable references */
#define EVT_EXPR_PROP_REF 0x29     /* u8: property reference */
#define EVT_EXPR_PAD_PRESSED 0x4F  /* (pop & msg_getPadPressed()) != 0 */
#define EVT_EXPR_PAD_RELEASED 0x58 /* (pop & msg_getPadReleased()) != 0 */
#define EVT_EXPR_PAD_HELD 0x59     /* (pop & msg_getPadHeld()) != 0 */
#define EVT_EXPR_OBJECT 0x5F       /* u8 id: evt_resolveObjectId(id) (operand of 0x60 / 0x61) */
#define EVT_EXPR_OBJ_VAR_REF 0x78  /* u8 uid, u8 index: another object's variable */
#define EVT_EXPR_SYSVAR_REF 0x79   /* u8: system variable reference */
#define EVT_EXPR_SYSVAR 0x7A       /* u8: evt_getSysVar value */
/* Expression ops evt_runExpr passes to evt_exprSpecial (its jump table 0x8006E718). The EVT_EXPR_UNIT_* ops work on
 * every battle unit in the mask below the operands (g_evtUnitObjects); ops not listed here give 0. The plain
 * versions are handled in evt_runExpr itself: ++/-- 0x04-0x07, = 0x2C, *= .. >>= 0x2F-0x35, &= ^= |= 0x3D-0x3F,
 * < > <= >= 0x18-0x1B, == != 0x20/0x21. */
#define EVT_EXPR_UNIT_POST_INC 0x08 /* var++ per unit */
#define EVT_EXPR_UNIT_POST_DEC 0x09
#define EVT_EXPR_UNIT_PRE_INC 0x0A
#define EVT_EXPR_UNIT_PRE_DEC 0x0B
#define EVT_EXPR_UNIT_NOT 0x0F /* mask of the units whose value is 0 */
#define EVT_EXPR_UNIT_LT 0x1C /* mask of the units whose value < operand */
#define EVT_EXPR_UNIT_GT 0x1D
#define EVT_EXPR_UNIT_LE 0x1E
#define EVT_EXPR_UNIT_GE 0x1F
#define EVT_EXPR_UNIT_EQ 0x22
#define EVT_EXPR_UNIT_NE 0x23
#define EVT_EXPR_UNIT_ANY_BITS 0x54 /* (value & operand) != 0 */
#define EVT_EXPR_UNIT_NO_BITS 0x55 /* (value & operand) == 0 */
#define EVT_EXPR_UNIT_XOR_NZ 0x56 /* (value ^ operand) != 0 */
#define EVT_EXPR_UNIT_OR_NZ 0x57 /* (value | operand) != 0 */
#define EVT_EXPR_UNIT_ASSIGN 0x2D /* value = operand per unit (evt_exprUnitAssign) */
#define EVT_EXPR_UNIT_MUL_ASSIGN 0x36 /* *= */
#define EVT_EXPR_UNIT_DIV_ASSIGN 0x37 /* /= (not by 0) */
#define EVT_EXPR_UNIT_MOD_ASSIGN 0x38 /* %= (not by 0) */
#define EVT_EXPR_UNIT_ADD_ASSIGN 0x39 /* += */
#define EVT_EXPR_UNIT_SUB_ASSIGN 0x3A /* -= */
#define EVT_EXPR_UNIT_SHL_ASSIGN 0x3B /* <<= */
#define EVT_EXPR_UNIT_SHR_ASSIGN 0x3C /* >>= */
#define EVT_EXPR_UNIT_AND_ASSIGN 0x40 /* &= */
#define EVT_EXPR_UNIT_XOR_ASSIGN 0x41 /* ^= */
#define EVT_EXPR_UNIT_OR_ASSIGN 0x42 /* |= */
#define EVT_EXPR_UNIT_MASK 0x2E /* the mask of the units whose value is non-zero, assigned (evt_exprUnitLogic) */
#define EVT_EXPR_UNIT_MASK_AND 0x43 /* the same mask & the next operand */
#define EVT_EXPR_UNIT_MASK_XOR 0x44
#define EVT_EXPR_UNIT_MASK_OR 0x45
#define EVT_EXPR_UNIT_MAX 0x4B /* bit of the unit with the largest value */
#define EVT_EXPR_UNIT_MIN 0x4C /* bit of the unit with the smallest value */
#define EVT_EXPR_CHR_HP 0x52 /* character (evt_exprSpecial_chr): Character.hp */
#define EVT_EXPR_CHR_MAX_HP 0x53
#define EVT_EXPR_CHR_MP 0x6E
#define EVT_EXPR_CHR_MAX_MP 0x6F
#define EVT_EXPR_UNUSED_5A 0x5A /* pops one operand, gives 0 (as 0x5B, 0x5C) */
#define EVT_EXPR_UNUSED_5B 0x5B
#define EVT_EXPR_UNUSED_5C 0x5C
#define EVT_EXPR_ITEM_COUNT 0x64 /* item id: item_getCount / keyItem_has / card_countOf */
#define EVT_EXPR_BGI_ANIM_FRAME 0x65 /* bgi anim: bgi_getAnimFrame */
#define EVT_EXPR_UNUSED_6C 0x6C /* pops two operands, gives 0 */
#define EVT_EXPR_ACTOR_TRIANGLE 0x70 /* object: its walkmesh triangle (bgi_getActorTriangle), -1 unless an actor */
#define EVT_EXPR_ACTOR_FLOOR 0x71 /* object: its walkmesh floor */
/* evt_getSysVar ids (operand of EVT_EXPR_SYSVAR), named from what each case returns. Evidence: [code] main
 * evt_getSysVar; ids 32-0xBF go to ovl_0b btl_getBattleVar (BTL_GETVAR_*), ids >= 0xC0 to ovl_0c
 * w_frameGetParameter (WM_PARAM_*) */
#define EVT_SYSVAR_RAND 0              /* next byte of the random table (0-255) */
#define EVT_SYSVAR_CAMERA 1            /* current field camera index (fldbg_getCamera) */
#define EVT_SYSVAR_CONTROL_ENABLED 2   /* EvtState.controlEnabled */
#define EVT_SYSVAR_SOUND_STATE 3       /* fld_soundCommand(SND_GET_STATE): non-zero while loading */
#define EVT_SYSVAR_TALK_ANGLE 4        /* g_evtTalkAngle */
#define EVT_SYSVAR_OBJ_CHAR 5          /* character index of the object id on the expression stack (its script among
                                        * the last 9, the character scripts), -1 if there is no such object */
#define EVT_SYSVAR_GIL 6               /* GameData.gil */
#define EVT_SYSVAR_STEP_COUNT 7        /* g_evtStepCount */
#define EVT_SYSVAR_MSG_SYNC_TAG 8      /* g_msgSyncTag (EVT_OP_MSG_SYNC_TAG, text codes) */
#define EVT_SYSVAR_MSG_CHOICE 9        /* msg_getChoiceIndex(): the chosen message choice */
#define EVT_SYSVAR_LINE_POINT_X 10     /* g_evtLinePointX */
#define EVT_SYSVAR_LINE_POINT_Z 11     /* g_evtLinePointZ */
#define EVT_SYSVAR_SCREEN_X 12         /* g_evtScreenX (EVT_OP_GET_SCROLL, EVT_OP_OBJ_SCREEN_POS) */
#define EVT_SYSVAR_SCREEN_Y 13         /* g_evtScreenY */
#define EVT_SYSVAR_MOVIE_FRAME 14      /* fld_movieCommand(4): mbg_getFrame */
#define EVT_SYSVAR_MOVIE_STATE 15      /* fld_movieCommand(3): field movie state */
#define EVT_SYSVAR_FROG_COUNT 16       /* GameData.frogCount */
#define EVT_SYSVAR_TIMER_SECONDS 17    /* timer_getSeconds(): countdown seconds left */
#define EVT_SYSVAR_CARD_RESULT 18      /* card_getGameResult(): last Tetra Master result */
#define EVT_SYSVAR_CARD_COUNT 19       /* card_countAll(): cards owned */
#define EVT_SYSVAR_PLAY_TIME 20        /* game_getPlayTimeSeconds(), at most 0x7FFFFF */
#define EVT_SYSVAR_TIMER_ENABLED 21    /* timer_isEnabled() */
#define EVT_SYSVAR_SOUND_VOLUME 22     /* g_evtSoundVolume (EVT_OP_SOUND_POS / EVT_OP_SOUND_POS_OBJ) */
#define EVT_SYSVAR_SOUND_PAN 23        /* g_evtSoundPan (same ops) */
#define EVT_SYSVAR_BATTLE_ID 24        /* GameData.battleId (s16) */
#define EVT_SYSVAR_BTLSEQ_BUSY 25      /* btlseq_isBusy() */
#define EVT_SYSVAR_BATTLE_END 26       /* g_evtMode == EVT_MODE_BATTLE_END */
#define EVT_SYSVAR_BATTLE_RESULT 27    /* GameData.battleResult */
/* GameData.battleResult values Evidence: [code] btl_victory,
 * btl_setBattleVar, btl_saveParty. 2 (BTL_SETVAR_END's 1 without the scene's victory sequence; ovl_65 gameover turns
 * it back into 1) and 5 (no fade-out on exit, btl_update) stay unnamed. */
#define BATTLE_END_VICTORY   1 /* BTL_END_VICTORY / BTL_END_SCRIPT_VICTORY */
#define BATTLE_END_DEFEAT    3 /* BTL_END_DEFEAT, BTL_SETVAR_DEFEAT */
#define BATTLE_END_ESCAPE    4 /* BTL_END_ESCAPE: enemies removed now give no rewards (btl_updateKo) */
#define BATTLE_END_GAME_OVER 6 /* btl_saveParty: a defeat without BTL_SCENE_NO_GAMEOVER */
#define EVT_SYSVAR_BATTLE_ARG_HI 28    /* g_evtBattleTriggerArg bits 8-15 */
#define EVT_SYSVAR_BATTLE_ARG_LO 29    /* g_evtBattleTriggerArg bits 0-7 */
#define EVT_SYSVAR_BATTLE_PHASE 30     /* ovl_0b Battle.phase (BTL_PHASE_*) */
#define EVT_SYSVAR_BATTLE_GROUP 31     /* ovl_0b Battle.groupNo */
#define EVT_SYSVAR_WORLD_FIRST 0xC0    /* from here: w_frameGetParameter; below (from 32): btl_getBattleVar */
#define EVT_EXPR_CONST16 0x7D      /* s16 constant */
#define EVT_EXPR_CONST32 0x7E      /* 4 bytes: 26-bit signed constant */
#define EVT_EXPR_END 0x7F
/* overlay entry points as called from the main exe (the overlays have their own prototypes) */
#if !defined(OVL_0A_H) && !defined(OVL_0B_H) && !defined(OVL_0C_H) && !defined(OVL_0D_H) && !defined(OVL_14_H) && !defined(OVL_15_H) && !defined(OVL_32_H) && !defined(OVL_33_H) && !defined(OVL_34_H) && !defined(OVL_36_H) && !defined(OVL_37_H) && !defined(OVL_38_H) && !defined(OVL_39_H) && !defined(OVL_3A_H) && !defined(OVL_3B_H) && !defined(OVL_3C_H) && !defined(OVL_3E_H) && !defined(OVL_64_H) && !defined(OVL_65_H)
/* mode entry points in overlays, linked through config/xrefs.us.txt */
void btl_entry(void);
void battleEnd_main(void);
void fld_spriteCommand(s32, s32, s32, s32, s32);
void fld_getSoundPanVolume(s16, s16, s16, u8 *arg4, u8 *arg5);
s32 fld_setObjectColor(s32, s32, s32, s32);
void fld_requestDiscChange(s32, s16);
void w_frameSetParameter(u32, s32); /* ovl_0c */
void fld_gameOver(void); /* ovl_0a */
s32 fldbg_addFollowOverlay(s32, s16, s16, s8, u8, u8, u8);
s32 fldbg_setCamera(s32);
s32 fldbg_moveOrigin(s16, s16, s16);
s32 fldbg_scrollTo(s16, s16, u16, s32);
s32 fldbg_getScroll(u16 *outX, u16 *outY);
s32 fldbg_setFollow(s32, s32, s32);
s32 fldbg_setFollowHeight(s16);
s32 fldbg_disableScrollLimits(void);
s32 fldbg_enableScrollLimits(void);
s32 fldbg_setViewport(s32, s16, s16, s16, s16);
s32 fldbg_setViewportScrollRate(s32, s32, s32);
s32 fldbg_moveOverlay(s32, s16, s16, s16);
s32 fldbg_setOverlayColor(s32, u8, u8, u8);
s32 fldbg_showOverlay(s32, s32);
s32 fldbg_setOverlayLoop(s32, s32, s32, s32);
s32 fldbg_setOverlayScreen(s32, s32);
s32 fldbg_setOverlaySkewLoop(s32, s32, s32, s32, s32);
s32 fldbg_setOverlayParallax(s32, s32, s32, s32);
s32 fldbg_setOverlayPos(s32, s32, s32);
s32 fldbg_setOverlayViewport(s32, s32);
s32 fldbg_setAnimFrame(s32, s32);
s32 fldbg_showAnimFrame(s32, s32);
s32 fldbg_playAnim(s32, s32);
s32 fldbg_setAnimSpeed(s32, s32);
s32 fldbg_setAnimStepDelay(s32, s32, s32);
s32 fldbg_setAnimFlags(s32, s32);
s32 fldbg_playAnimRange(s32, u32, u32);
s32 fldbg_resetAnim(s32, s32);
s32 fldbg_setScrollLimits(s32, s16, s16, s16, s16);
s32 bgi_setFloorActive(s32, s32);
s32 bgi_showAnimFrame(u32, u32);
s32 bgi_setMaskFilter(u8);
s32 bgi_setTriangleActive(s32, s32);
s32 bgi_setAnimFrame(u32, u32);
s32 bgi_setAnimPlaying(u32, s32);
s32 bgi_setAnimRate(u32, s16);
s32 bgi_setAnimFrameValues(u32, s32);
s32 bgi_setAnimFrameValue(u32, u32, s32);
s32 bgi_setAnimMode(u32, s32);
s32 bgi_resetAnim(u32, s32);
s32 bgi_startFloorMove(s32, s32);
s32 bgi_stopFloorMove(s32, s32);
s32 bgi_setFloorMovePlaying(s32, s32);
s32 bgi_setFloorMoveMode(s32, s32);
s32 bgi_setFloorMoveFloor(s32, s32);
s32 bgi_setFloorMoveSpeed(s32, s16);
s32 bgi_setFloorMoveCurve(s32, s32);
s32 bgi_setFloorMoveRange(s32, s16, s16);
s32 bgi_setFloorMoveAxis(s32, s32);
void btl_setBanner(s32, s32);
void rewards_requestClose(void);
s32 bgi_setActorActive(s32, s32);
s32 bgi_setActorRadius(s32, u16);
void btl_setEnemyCmd(s32, s32, s32, s32);
void btlseq_startDirect(s32, s32, u16);
void btl_setBattleVar(s32, s32);
s32 fld_modelCommand(s32, s32, s32, s32, s32);
s32 fld_movieCommand(s32, s32, s32);
VECTOR *wm_getCameraEye(void);
VECTOR *wm_getCameraTarget(void);
s32 bgi_playAnimRange(u32, u32, u32);
#endif
s32 fld_queueModelBundle(s32); /* uid; queues the actor's model */

extern u8 *g_evtInstrStart;
extern s32 g_evtUnitObjectCount;
EvtObject *evt_getObjectArg(void);
void evt_returnFromFunc(EvtObject *);
s32 evt_modelOpField(EvtObject *, s32);
s32 evt_modelOpBattle(EvtObject *, s32);
s32 evt_modelOpWorld(EvtObject *, s32);

extern s32 g_evtPosSetThisFrame;
extern EvtObject *g_evtLookTarget;
extern s32 g_evtLookReleaseTimer;
extern s32 g_msgSyncTag;
extern s32 g_msgLine; /* message counter */
extern u8 g_msgTextSpeeds[]; /* text speed per config setting */
void evt_turnObject(EvtObject *, s32, s32, s32);
s32 math_atan2(s32, s32);
void msg_setNumber(u32, s32);
void msg_setChoiceMask(s32, s32);
void msg_keepWindowsOnInit(void);
WinLayout *evt_getMsgLayout(EvtObject *, s32, s32);
void msg_open(s32, s32, WinLayout *, s32);

static inline s32 evt_getArg16_inline(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        EvtObject *pp = g_evtCurObj;
        r = *pp->ip++;
        r |= *pp->ip++ << 8;
        r = (s16)r;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

/* uid of the object the current script runs for */
#define CUR_UID() (((u8 *)((s32 *)g_evtCurObj + g_evtCurObj->stackOfs) + g_evtCurObj->sp * 4)[-1])

static inline s32 evt_getArg8_inline(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->ip++;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

extern s32 g_evtPlayerAnimCount;
extern s32 g_evtLinePointX;
extern s32 g_evtLinePointZ;
void msg_close(s32);
void msg_resetWindows(void);
void win_closeAll(void);
void msg_clearWindowOtz(void);
void *msg_getText(s32);
s32 evt_moveObjectTo(s32, s32, s32, s32);
void evt_finishTurn(EvtObject *);

/* Inlined helper: start the animation set up in animStart/animEnd/animFlags. */
static inline void evt_execInstr_anim(EvtObject *p, s32 anim) {
    if ((p->animFlags & EVT_ANIM_SCRIPTED) && (p->flags & EVT_OBJ_TURNING)) {
        evt_finishTurn(p);
    }
    p->anim = anim;
    p->animFrame = p->animStart;
    p->animFlags &= ~(EVT_ANIM_REVERSE | EVT_ANIM_HELD | EVT_ANIM_IDLE);
    p->animFlags |= (p->animStart > p->animEnd) ? (EVT_ANIM_SCRIPTED | EVT_ANIM_REVERSE) : EVT_ANIM_SCRIPTED;
    p->animSubFrame = 0;
    p->animFrames = evt_getAnimFrameCount(p->modelId, anim);
    p->animSpeed = p->defaultAnimSpeed;
}

/* event region object (EVT_KIND_REGION, EVT_OP_NEW_REGION), same header as EvtObject: a convex floor polygon set
 * by EVT_OP_SET_REGION; evt_checkPlayerAction runs its EVT_FUNC_RANGE function while the player is inside */
typedef struct {
    s16 x; /* x */
    s16 z; /* z */
} EvtRegionPt;
typedef struct {
    u8 pad0[0x14];
    s32 count; /* vertex count */
    EvtRegionPt pts[8];
} EvtRegion;

/* texture animation n seen from the set's start (Unk8004AFF0.texAnims->unk0[n]): the set has a 4-byte count before
 * its 0x14-byte entries (ovl_0a TexAnimSet / TexAnim), so this record's +4 is entry n's +0 */
typedef struct {
    u8 pad0[4];
    u8 flags; /* TexAnim.flags: TEXANIM_* */
    u8 pad5[0xC - 5];
    s32 pos; /* TexAnim.pos: frame position (20.12) */
    u8 pad10[0x12 - 0x10];
    s16 lastFrame; /* TexAnim.lastFrame: 0x1000 forces the next frame's upload */
} Unk8004AFF0Sub; /* 0x14 */
/* TexAnim.flags (ovl_0a fldmdl_updateTexAnims; set by EVT_OP_TEX_ANIM_PLAY and ops C1/C2) */
#define TEXANIM_ACTIVE 0x1
#define TEXANIM_ONESHOT 0x2 /* cleared with TEXANIM_ACTIVE at the end */
#define TEXANIM_SCROLL 0x4 /* vertical scroll mode, frameCount 0 */

/* model object (ModelSlot.unk8): main's view of ovl_0a ModelObj / ovl_0b BtlModel (docs/field-models.md) */
typedef struct Unk8004AFF0 {
    u16 flags; /* MDL_* */
    u8 pad2[4 - 2];
    u8 parentBone; /* op 0x4C: bone of the parent model it follows */
    u8 pad5[0x14 - 5];
    u32 hiddenGroups; /* mesh-group bits not drawn (ops 0x39 clear / 0x3A set) */
    u8 pad18[0x1C - 0x18];
    struct Unk8004AFF0 *parent; /* op 0x4C / 0x4D: model it is attached to */
    u8 pad20[0x48 - 0x20];
    s16 lookAtX; /* look-at point (evt_relaxLookAt; ModelObj.lookAtTarget) */
    s16 lookAtY;
    s16 lookAtZ;
    u8 pad4E[0x50 - 0x4E];
    struct { Unk8004AFF0Sub unk0[1]; } *texAnims; /* texture animations (item 0x19xxxx), ops 0xC0-0xC2 */
    s16 scaleX; /* op 0x9F: (s8)arg << 6, flags 2 */
    s16 scaleY;
    s16 scaleZ;
    u8 pad5A[0x74 - 0x5A];
    s16 attachX; /* op 0xD4: offset from the parent bone */
    s16 attachY;
    s16 attachZ;
} Unk8004AFF0;

/* Inlined copy of evt_getObjectModelData. */
static inline Unk8004AFF0 *evt_getObjectModelData_inline(EvtObject *p) {
    ModelSlot *q;

    if (p != NULL) {
        if (p->modelId == EVT_MODEL_NONE) {
            return NULL;
        }
        q = (ModelSlot *)game_getModel(p->modelSlot);
        if (q != NULL) {
            return (Unk8004AFF0 *)q->model;
        }
    }
    return NULL;
}

extern u8 g_swirlVariant;
extern s32 g_evtEncounterChance;
typedef struct {
    s16 battle0; /* encounter battle ids by slot (op EVT_OP_ENCOUNTER_TABLE; evt_rollEncounter_pick picks one by g_encounterSlotThresholds) */
    s16 battle1;
    s16 battle2;
    s16 battle3;
    u8 pattern; /* & 3 selects the slot thresholds */
} EncounterTable;
extern EncounterTable g_evtEncounterTable;
extern u8 g_evtPlayerToLinePointScript[];
void evt_setNextBattle(u16);
void evt_setCardOpponent();
void msg_startHereIcon(s32);
void evt_skipVarRef(u8 **);
EvtListNode *evt_deleteObject(EvtObject *);
EvtObject *evt_allocObject(s32);
void evt_onAnimEnd(EvtObject *);

extern s32 g_evtScreenX;
extern s32 g_evtScreenY;
extern s32 g_evtSoundVolume;
extern s32 g_evtSoundPan;
void msg_showPlayerBubble(s32);
void msg_setAtePrompt(s32);
void evt_onMenuOpen(void);
void keyItem_add(s32);
void keyItem_remove(s32);
s32 card_add(s32);
void chr_clearRecruited(u32);

/* Inlined copy of evt_getArg24 (24-bit script argument). */
static inline s32 evt_getArg24_inline(void) {
    s32 v;
    EvtObject *pp;

    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        v = evt_popResult();
    } else {
        pp = g_evtCurObj;
        v = *pp->ip++;
        v |= *pp->ip++ << 8;
        v |= *pp->ip++ << 16;
    }
    g_evtArgFlags >>= (g_evtArgRead = 1);
    return v;
}

extern u8 g_evtPartyOrderChars[];
extern u8 g_evtCharPartyOrder[];
s32 msg_getPartyMemberId(s32);
void chr_join(s32, u8, s32);
s32 chr_resetAp(s32, s32);
s32 chr_learnAbility(s32, s32);
s32 chr_cureStatusBit(Unk800539FC *, s32);
void timer_setSeconds(s32);
void timer_enable(u8);
void timer_setRunning(u8);
void evt_projectToScreen(SVECTOR *, s32 *, s32 *);
s32 vib_play(s32);
s32 vib_setPlaying(s32);
s32 vib_selectTrack(s32, s32, s32);
s32 vib_setTrackEnabled(s32, s32, s32);
s32 vib_setRate(s16);
s32 vib_setLoopMode(s32);
s32 vib_setRange(s32, s32);
extern u8 g_evtLocationNameBuf[];
extern u8 *g_evtLocationName;

typedef struct {
    s32 size; /* members to pick (EVT_OP_PARTY_MENU; = ovl_3b PartyRequest.size) */
    u8 party[4]; /* party character ids */
    u8 reserve[8]; /* recruited characters not in the party, 0xFF-terminated */
    u8 fixed[9]; /* per character: can't leave the party (EVT_OP_PARTY_MENU bit mask) */
} Unk8007AE70;
extern Unk8007AE70 g_evtPartyRequest;

/* Inlined helper: rebuild g_evtState->unk16E4 from the current party. */
static inline void evt_execInstr_party(void) {
    s32 mask = 0;
    s32 i = 0;
    u8 *p = g_evtState->partyUids;
    s32 r;

    *(s32 *)g_evtState->partyUids = -1;
    for (; i < 4; i++) {
        r = msg_getPartyMemberId(i);
        if (r >= 0) {
            mask |= 1 << g_evtCharPartyOrder[r];
        }
    }
    for (i = 0; mask != 0; i++) {
        if (mask & 1) {
            *p++ = g_evtScriptCount - 9 + g_evtPartyOrderChars[i];
        }
        mask >>= 1;
    }
}

static inline void func_8004B3A8_inline(s32 a, s32 b) {
    evt_createObject(a, b, 0x14, 0x10)->kind = EVT_KIND_CODE;
}

static inline void evt_createRegion_inline(s32 a, s32 b) {
    EvtObject *p = evt_createObject(a, b, 0x38, 0x10);
    p->kind = EVT_KIND_REGION;
    p->flags = EVT_OBJ_ENABLED;
}

static inline void evt_execInstr_newactor(s32 uid, s32 b) {
    EvtObject *p;

    p = (EvtObject *)evt_createObject(uid, b, 0xAC, 0x10);
    p->collisionRadius = 0x10;
    p->talkRadius = 0x10;
    p->modelId = EVT_MODEL_NONE;
    p->balloonWindow = EVT_BALLOON_NONE;
    p->scale = 0x40;
    p->kind = EVT_KIND_ACTOR;
    p->defaultAnimSpeed = 0x10;
    p->speed = 0x1E;
    p->animSpeed = 0x10;
    p->animEnd = 0xFF;
    p->runSpeed = 0x1F;
    p->moveTurnRate = 0x10;
    p->talkTurnScript = EVT_OP_RETURN;
    p->talkTurnWaitOp = EVT_OP_WAIT_TURN;
    p->talkTurnReturnOp = EVT_OP_RETURN;
    p->turnSpeed = 0x10;
    p->talkTarget = 0xFF;
    p->idleAnimSpeeds.w = 0x10101010;
    p->flags |= EVT_OBJ_NO_NPC_COLLISION;
    switch (g_evtMode) {
    case EVT_MODE_FIELD:
        fld_queueModelBundle(uid);
        break;
    case EVT_MODE_BATTLE:
        g_evtUnitObjects[g_evtUnitObjectCount++] = p;
        break;
    }
}

/* Inlined helper: give the window opened by obj the focus (b = window flags). */
static inline void evt_execInstr_window(EvtObject *obj, s32 b, s32 c) {
    EvtListNode *n;
    EvtObject *o;

    if ((b & (MSG_FLAG_BALLOON | MSG_FLAG_NO_FOLLOW)) == MSG_FLAG_BALLOON && obj->kind == EVT_KIND_ACTOR) {
        for (n = g_evtState->objects; n != NULL; n = n->next) {
            o = n->obj;
            if (o->kind == EVT_KIND_ACTOR && o->balloonWindow == c) {
                o->balloonWindow = EVT_BALLOON_NONE;
            }
        }
        obj->balloonWindow = c;
        g_evtLookTarget = obj;
        g_evtLookReleaseTimer = 0;
    }
}

/* Inlined helper: script character id -> GameData.unk3C index. */
static inline s32 evt_execInstr_chr(s32 id) {
    if (id >= 9) {
        id -= 4;
    }
    return id;
}



/* Inlined helpers for the pad binding setup (config_applyPadMapping): GameState.unk24 bit 1
 * enables the custom button map in unk28 (4 bits per action). */
static inline s32 config_applyPadMapping_custom(u32 *c) {
    return (c[0] >> 1) & 1;
}
static inline s32 config_applyPadMapping_remap(s32 k) {
    u32 *c = &g_gameState->config;
    if (config_applyPadMapping_custom(c) == 1) {
        return (c[1] >> (k * 4)) & 0xF;
    }
    return k;
}


/* Inlined max helper of vib_init (keeps the running maximum in memory). */
static inline void vib_init_max(u32 *m, u32 v) {
    if (*m < v) {
        *m = v;
    }
}

/* primitive buffer / ordering table used by swirl_drawFlash */
extern u32 *g_swirlOt;
extern u32 *g_swirlPkt;
extern u16 g_swirlFlash;
/* battle-entry warp grid (swirl_buildMesh fills it, swirl_drawMesh draws it) */
typedef struct {
    s16 x, y;
    s16 pad[2];
} WarpVert; /* 9x9 screen vertices */
extern DVECTOR *g_swirlColumns; /* 9 texture columns/rows */
extern WarpVert *g_swirlMesh;
/* battle-entry swirl (swirl_update) */
typedef struct {
    s16 angle; /* rotation: start angle (g_swirlAngle) */
    s16 angleStep; /* rotation speed */
    s16 scale; /* scale */
    s16 zoom; /* start g_swirlZoom */
    s16 zoomStep; /* added to g_swirlZoom per frame */
    s16 fade; /* start g_swirlFade */
    s16 fadeStart; /* frame the fade starts (g_swirlFrame) */
    s16 fadeStep; /* subtracted from g_swirlFade per frame */
    s16 flashStart; /* frame the flash starts */
    s16 flashStep; /* added to g_swirlFlash per frame */
} SwirlDef; /* 0x14 */
extern SwirlDef g_swirlParamSets[2];
extern SwirlDef *g_swirlParams;
extern u16 g_swirlFade;
extern s16 g_swirlAngle;
extern s16 g_swirlAngleStep __asm__("g_swirlAngle+2"); /* per-frame g_swirlAngle change; no symbol of its own */
extern s16 g_swirlZoom;
extern s32 g_swirlFrame; /* frame */
#if !defined(OVL_0C_H) /* ovl_0c declares it as s32 */
#if !defined(OVL_0A_H) && !defined(OVL_0C_H) /* the overlays type it themselves */
extern u32 *g_swirlBuf;
#endif
#endif
void fade_clear(void);
s32 math_sin(s32); /* sin */
void swirl_drawFlash(s32, s32);
u32 *swirl_buildMesh(u32 *, s32, s32);
void swirl_drawMesh(void);
#if !defined(OVL_0B_H) /* ovl_0b declares it with its DR_MOVE */
void SetDrawMove(void *p, RECT *rect, int x, int y);
#endif
extern u8 g_swirlFlashX[];
extern u8 g_swirlFlashY[];

/* sound stream header (g_akaoStream.unk2C + 0x80), akao_streamStartRing */
typedef struct {
    u8 pad0[0x10];
    s32 dataSize; /* AkaoStreamState.bytesLeft */
    u32 loopStart; /* loop offset (AkaoStreamState.loopPtr) */
    s32 flags; /* AkaoStreamState.flags */
    u16 pitch; /* AkaoStreamState.pitch */
    u8 pad1E[0x20 - 0x1E];
    s32 loopLength; /* AkaoStreamState.loopLength */
    u8 pad24[0x28 - 0x24];
    s32 fadeIn; /* nonzero: akao_sendCmdE5 fades the volume in over it (akao_streamStartRing) */
} StreamHeader;

void akao_sendCmdE5(s32 a, s32 b);
void akao_streamStartRingVoices(void);

/* AKAO sample stream state, akao_uploadBank */
typedef struct {
    AkaoInstrument *instrDest; /* destination of the next sample-info copy */
    u32 spuAddr; /* SPU transfer address */
    u32 sampleLeft; /* sample bytes left */
    u32 instrLeft; /* sample-info bytes left */
} AkaoBankUpload;
extern AkaoBankUpload g_akaoUpload;
/* header of an AKAO instrument bank (akao_loadBankAt, akao_uploadBank); instrument entries follow, then the samples */
#define AKAO_MAGIC 0x4F414B41 /* "AKAO" */
typedef struct {
    s32 magic; /* AKAO_MAGIC */
    s32 id; /* bank id (g_akaoLoadedBanks keeps the loaded ones) */
    s32 unk8; /* not read by C */
    s32 unkC; /* not read by C */
    s32 spuAddr; /* where the samples go in SPU RAM */
    s32 sampleSize;
    s32 slot; /* first g_akaoInstruments entry */
    s32 instCount; /* 16-byte instrument entries after the header */
    s32 unk20[8]; /* not read by C */
} AkaoBankHeader; /* 0x40 */
extern AkaoBankHeader g_akaoBankHeader; /* AKAO header copy */

/* ovl_0a functions called from the main script interpreter */
s32 bgi_getActorTriangle(s32, u16 *, u16 *);
s32 bgi_getAnimFrame(u32, s32 *);

/* party member index -> character slot (9.. map to 5..) */
static inline s32 evt_exprSpecial_chr(s32 i) {
    if (i >= 9) {
        i -= 4;
    }
    return i;
}

/* evt_getSysVar (script system variables) */
extern s32 g_evtTalkAngle;
void *evt_findNearestObject(EvtObject *, s32, s32 *); /* nearest object of a kind */
extern s32 g_evtStepCount;
s32 fldbg_getCamera(u8 *);
s32 btlseq_isBusy(void);
s32 w_frameGetParameter(s32); /* ovl_0c */
s32 btl_getBattleVar(s32);
u32 game_getPlayTimeSeconds(void);
s32 timer_getSeconds(void);
s32 card_getGameResult(void);
s32 msg_getChoiceIndex(void);

/* random encounter check (evt_rollEncounter). ovl_0c declares wm_getEncounterEntry with its own
 * table type, so these are hidden there. */
#ifndef OVL_0C_H
extern s32 g_evtEncounterDist;
extern u8 g_encounterSlotThresholds[][4];
EncounterTable *wm_getEncounterEntry();

static inline s32 evt_rollEncounter_rand(void) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return *g_randPtr++;
}

static inline u16 evt_rollEncounter_pick(void) {
    EncounterTable *t;
    s32 r;
    s32 i;
    u8 *tbl;

    if (g_evtMode != 1) {
        t = wm_getEncounterEntry();
    } else {
        t = &g_evtEncounterTable;
    }
    r = evt_rollEncounter_rand();
    tbl = g_encounterSlotThresholds[t->pattern & 3];
    if (r >= tbl[0]) {
        if (r >= tbl[1]) {
            i = 6;
            if (r < tbl[2]) {
                i = 4;
            }
        } else {
            i = 2;
        }
    } else {
        i = 0;
    }
    return *(u16 *)((u8 *)t + i); /* i is a byte offset into battle0..battle3 */
}
#endif

/* start an object's idle animation (evt_updateIdleTimer) */
static inline void evt_updateIdleTimer_idle(EvtObject *q) {
    q->animStart = 0;
    q->animEnd = 0xFF;
    evt_execInstr_anim(q, q->idleAnim);
}

/* party slot key for evt_isInParty: slots 9+ map down by 4 */
static inline s32 evt_isInParty_key(s32 slot) {
    s32 key = slot;
    if (slot >= 9) {
        key = slot - 4;
    }
    return key;
}

/* is the party member with this key present, kind < 14 for slot < 8, kind >= 14 otherwise */
static inline s32 evt_isInParty_find(s32 key, s32 slot) {
    Character **p;
    Character *c;
    s32 kind;
    s32 ge;
    s32 lt;

    for (p = g_gameState->gameData->party; p < &g_gameState->gameData->party[4]; p++) {
        c = *p;
        if (c == NULL) {
            continue;
        }
        if (c->info.b.id != key) {
            continue;
        }
        kind = c->info.b.kind;
        lt = kind < 14;
        if (kind) {
            ge = lt ^ 1;
        } else {
            ge = lt ^ 1;
        }
        if (slot >= 8) {
            if (lt) {
                continue;
            }
        } else {
            if (ge) {
                continue;
            }
        }
        break;
    }
    return p < &g_gameState->gameData->party[4];
}

extern const u8 g_chrDefaultNames[]; /* default character names: count, then {offset, length} entries */
extern const u8 g_msgAteTitle[]; /* "ACTIVE TIME EVENT" window title */
extern const u8 g_msgMognetTitle[]; /* "MOGNET" window title */

/* sound driver (AKAO) routines used by akao_updateVoices */
void akao_setReverbVoices(u32 x);
void akao_setNoiseVoices(u32 x);
void akao_setPitchModVoices(u32 x);
void akao_flushVoice(s32 v, AkaoVoiceRegs *p);
void akao_calcSeChannelVoice(AkaoChannel *c, u32 bit);
void akao_assignVoices(AkaoChannel *ch, u32 mask, u32 keep, u32 *used);
void akao_readEnvelopes(u32 extra);
void akao_applyCrossfadeVolume(); /* (AkaoSeq *): saves the sequence volume in g_akaoCrossfade.savedVolume and scales it by the crossfade volume */
void akao_restoreCrossfadeVolume(); /* (AkaoSeq *) */
void akao_buildVoiceModeMask(u32 *out, u32 m1, u32 m2, u32 extra);

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
/* hand-written asm (no C definition) */
void math_dirVector(s16 *, s32, s32, s32);
s32 evt_isInRegion(EvtObject *, s32, s32); /* is (x, z) inside the object's region */
void pen_endSmall(void);
void pen_setColorPreset(s32);
/* ovl_0a / ovl_0c menuMain_setCursor(u8); unprototyped: the menu screen passes an int unconverted */
void menuMainField_setCursor();
void menuMainWorld_setCursor();
u16 akao_shiftInstrumentUp(u32 flags, AkaoChannel *c);
void akao_swap(u32 *a, u32 *b, u32 n);
s32 cd_findFile(char *name);
void config_apply(void);
void ending_main(void); /* ovl_15 */
s32 evt_createModel(EvtObject *a);
s32 evt_getArg16(void);
s32 evt_getArg8(void);
s32 evt_isControlEnabled(void);
s32 fade_getOtz(void);
s32 fade_isSubtractive(void);
void fieldCalc_calcHpGain(MainCalc *a);
void fieldCalc_calcMpGain(MainCalc *a);
void fieldCalc_calcReviveHp(MainCalc *c);
void fieldCalc_casterStatusHalf(MainCalc *c);
void fieldCalc_cureStatus(MainCalc *c);
void fieldCalc_cureStatusType(MainCalc *c);
void fieldCalc_multiTargetHalf(MainCalc *c);
void fieldCalc_restoreAll(MainCalc *c);
void fieldCalc_restoreHalf(MainCalc *c);
void fieldCalc_reviveRandom(MainCalc *c);
void fieldCalc_setItemHeal(MainCalc *c);
void fieldCalc_setMagicHeal(MainCalc *c);
void fieldCalc_supportBoost(MainCalc *a);
void fld_entry(void); /* ovl_0a */
s32 fld_soundCommand(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3); /* ovl_0a */
s32 fldbg_scrollToPlayer(s32 frames, s32 scrollMode); /* ovl_0a */
void help_drawStub(void);
void rain_set(s32 a, u32 b);
void wrain_set(s32 a, u32 b);
void game_initData(void);
void game_initLibraries(void);
void game_initState(void);
void game_reset(void);
s32 item_getCount(s32 id);
void menuScreen_stepTitleResize(void);
void menuScreen_stepTitleScroll(void);
void menu_startFadeOut(void);
void msg_closeAll(void);
u32 msg_getPadHeld(void);
u32 msg_getPadPressed(void);
void opening_main(void); /* ovl_14 */
void pad_disableRepeat(s32 a);
void pad_resetRepeats(s32 a);
s32 preload_isInVram(PreloadEntry *p);
s32 text_getButtonSprite(s32 button);
void tm_main(void); /* ovl_0d */
void win_drawBalloonBorder(RECT *rect, s32 otz, u8 flag, u8 abe, s32 pal);
void win_drawBorder(RECT *rect, s32 otz, u8 flag, u8 abe, s32 pal);
void win_drawHelpFrame(RECT *r, s32 otz, s32 style, s32 xoff);
Window *win_open(RECT *r, s32 otz, s32 style, u32 slot);
void wm_entry(void); /* ovl_0c */

#endif

#ifndef OVL_0B_H
#define OVL_0B_H

/* Declarations local to overlay 0x0B (battle, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ---- battle units ---- */

typedef struct BtlUnitA0 {
    u8 pad0[0x14];
    s32 x; /* start position, 16.16 (also the home of party units) */
    s32 y;
    s32 z;
    s16 rotX; /* start rotation */
    s16 rotY;
    s16 rotZ;
    u8 pad26[0x3A - 0x26];
    u8 frame; /* motion frame (btl_updateUnitModel advances it) */
    u8 modelSlot; /* EvtObject.modelSlot: the chrModels index, high byte of the model DB key (btlfx_getCasterTexInfo, as btl_main's party model keys) */
} BtlUnitA0;

/* model mesh group (ovl_0a WmModelGroup): primitive counts per type. The five arrays follow each other in the file in
 * this order; uvs and colours are indexed per polygon corner (not shared with the vertices). docs/battle-models.md */
typedef struct {
    u16 packetSize; /* bytes of GPU packets if every polygon were drawn: 0x28 FT4, 0x20 FT3, 0x34 GT4, 0x28 GT3, 0x24 G4, 0x1C G3, 0x18 F4, 0x14 F3 (ovl_0a sizes packet buffers with it) */
    u16 nFT4; /* primitive counts by type, in the order of the packed sources (btlfx_getMeshPrims) */
    u16 nFT3;
    u16 nGT4;
    u16 nGT3;
    u16 nG4;
    u16 nG3;
    u16 nF4;
    u16 nF3;
    u8 pad12;
    s8 otBias; /* ordering table entries the group's polygons are moved toward the front (btl_drawMesh) */
    u16 *vertexCounts; /* per bone (BtlModelHeader.boneCount): vertices bone b moves, consecutive from bone 0 (btl_projectMesh) */
    SVECTOR *vertices; /* sum of vertexCounts; polygon vertex indices index this (and g_vtxScreenXY/Z) */
    u32 *prims; /* primitive sources (WmSrc*), the eight lists back to back in the order of the counts */
    u8 (*uvs)[2]; /* indexed by the textured sources' uv indices */
    u8 (*colors)[4]; /* indexed by the gouraud sources' colour indices */
} BtlMesh; /* 0x28 */

/* primitive sources of a model mesh (same layout as WorldMap's in ovl_0a/0c) */
typedef struct {
    u16 vtx[4]; /* vertex indices (BtlMesh.vertices) */
    u16 uv[4]; /* uv indices (BtlMesh.uvs) */
    u8 rgb[3]; /* colour (with the code byte: the packet's rgb word) */
    u8 tex; /* texture: BtlModel.texPages index, model info uv offsets */
    u8 unk14; /* not read by C (as in ovl_0a/0c WmSrcFT4) */
    u8 flags; /* WMSRC_* */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

/* WmSrc*.flags (btl_drawMesh and its asm twin; the same bits as ovl_0a/0c's WMSRC_*). Evidence: [code] */
#define WMSRC_SEMITRANS    0x01 /* semi-transparent: the packet gets its abr from WMSRC_TPAGE_BITS */
#define WMSRC_DOUBLE_SIDED 0x02 /* no nclip backface test */
#define WMSRC_TPAGE_BITS   0xE0 /* with WMSRC_SEMITRANS: ORed into the tpage (abr bits 5-6, and bit 7) */

typedef struct {
    u16 vtx[3]; /* vertex indices */
    u8 tex; /* texture */
    u8 pad7;
    u8 rgb[3];
    u8 padB;
    u16 uv[3]; /* uv indices */
    u8 flags; /* as WmSrcFT4 */
    u8 pad13;
} WmSrcFT3; /* 0x14 */

typedef struct {
    u16 vtx[4]; /* vertex indices */
    u16 uv[4]; /* uv indices */
    u16 col[4]; /* colour indices (BtlMesh.colors) */
    u8 pad18[4];
    u8 flags; /* as WmSrcFT4 */
    u8 tex; /* texture */
    u8 pad1E[2];
} WmSrcGT4; /* 0x20 */

typedef struct {
    u16 vtx[3]; /* vertex indices */
    u16 uv[3]; /* uv indices */
    u8 padC[3];
    u8 flags; /* as WmSrcFT4 */
    u16 col[3]; /* colour indices */
    u8 tex; /* texture */
    u8 pad17;
} WmSrcGT3; /* 0x18 */

typedef struct {
    u16 vtx[4]; /* vertex indices */
    u16 col[4]; /* colour indices */
    u8 pad10[4];
    u8 flags; /* as WmSrcFT4 */
    u8 pad15[3];
} WmSrcG4; /* 0x18 */

typedef struct {
    u16 vtx[3]; /* vertex indices */
    u8 pad6[2];
    u16 col[3]; /* colour indices */
    u8 padE[5];
    u8 flags; /* as WmSrcFT4 */
} WmSrcG3; /* 0x14 */

typedef struct {
    u16 vtx[4]; /* vertex indices */
    u8 rgb[3];
    u8 padB[2];
    u8 flags; /* as WmSrcFT4 */
    u8 padE[2];
} WmSrcF4; /* 0x10 */

typedef struct {
    u16 vtx[3]; /* vertex indices */
    u8 pad6[2];
    u8 rgb[3];
    u8 flags; /* as WmSrcFT4 */
} WmSrcF3; /* 0xC */

/* skeleton bone of a model (BtlModelHeader.bones, 4 bytes; ovl_0a WmBone): btl_calcBoneMatrices composes bone i's
 * matrix with its parent's (already built: parent < i) and a z offset of `length`; bone 0 is the root */
typedef struct {
    s16 length; /* offset from the parent's origin along the parent's z axis */
    u8 unk2; /* not read by C (as in ovl_0a WmBone) */
    u8 parent; /* parent bone */
} BtlBoneDef;

/* model header (DB item 0x02xxxx: battle model files in FF9.IMG dir index 7, file id = model id; weapon and effect
 * models use the same format; ovl_0a WmModel). Layout and users: docs/battle-models.md */
typedef struct BtlModelHeader {
    u16 flags; /* BTL_MODELHDR_* */
    u8 boneCount; /* bones: BtlMesh.vertexCounts entries, motion keys per frame (btl_projectMesh, btl_calcBoneMatrices) */
    u8 meshCount; /* mesh groups (meshes); bit i of BtlModel.hiddenMeshes hides group i */
    u16 packetSize; /* sum of the groups' BtlMesh.packetSize */
    u8 lookAtBone; /* not read by ovl_0b; ovl_0a reads the same header byte as WmModel.lookAtBone */
    u8 pad7;
    u16 height; /* btl_getUnitHeight */
    u16 radius; /* btlfx_getUnitRadius (party units) */
    s32 bones; /* BtlBoneDef[boneCount]: offset from the header, relocated on load (btl_applyModelTextures) */
    BtlMesh *meshes; /* meshCount groups: offset from the header, relocated on load */
} BtlModelHeader;

/* BtlModelHeader.flags. Evidence: [code] */
#define BTL_MODELHDR_RELOCATED 0x01 /* the offsets were turned into pointers (btl_applyModelTextures) */

/* motion (DB item 0x03xxxx, in the model file next to the model header; btl_calcBoneMatrices; ovl_0a WmAnim).
 * One key per bone of the model it plays on (BtlModelHeader.boneCount), in bone order */
typedef struct BtlMotion {
    u8 pad0[2];
    u16 frameCount; /* frames (btl_updateUnitModel wraps the unit's frame at it) */
    u16 rootX; /* root x: the value (constMask bit 0) or the offset of a per-frame s16 table */
    u16 rootY; /* root y (bit 1) */
    u16 rootZ; /* root z (bit 2) */
    u16 constMask; /* bits 0-2: rootX/Y/Z is the value itself */
    s32 rotKeys; /* offset, relocated on load: per-bone keys {u16 x, y, z, constMask}: bits 4-11 of the angles (4096 = 360 deg), a constant or the offset of an s8 per-frame table */
    s32 rotKeysLo; /* offset, relocated on load: the same for bits 0-3 (per-frame tables: two frames per byte, even frame in the low nibble), or 0 */
} BtlMotion;

/* bone matrix (BtlModel.bones, built by btl_calcBoneMatrices), laid out as a MATRIX: model/world space of the bone
 * (BtlModel.matrix applied), 4096 = 1.0. docs/battle-models.md "Motions" */
typedef struct {
    s32 rot[5]; /* rotation, s16 m[3][3] (row-major) + pad: parent rotation . Rz Ry Rx of the motion key */
    s32 tx; /* translation: parent's + parent rotation . (0, 0, BtlBoneDef.length); bone 0: the root translation */
    s32 ty;
    s32 tz;
} BtlBone; /* 0x20 */

typedef struct BtlUnit9C50 {
    u8 pad0[4];
    u8 flags; /* TexAnim.flags (the entries seen 4 bytes early): BTL_TEXANIM_* */
    u8 pad5[0xC - 5];
    s32 pos; /* TexAnim.pos */
    u8 pad10[0x12 - 0x10];
    s16 lastFrame; /* TexAnim.lastFrame (0x1000 forces a redraw) */
} BtlUnit9C50; /* 0x14 */

/* TexAnim.flags (btl_updateTexAnims). Evidence: [code] */
#define BTL_TEXANIM_PLAY    0x01 /* playing */
#define BTL_TEXANIM_ONCE    0x02 /* stop at the end (else loop) */
#define BTL_TEXANIM_VSCROLL 0x04 /* vertical scroll (frameCount 0) */

/* texture word of a model, one per texture index (WmSrc*.tex): the model info entry's first table (ovl_0a WmTexInfo).
 * btl_drawMesh puts clut in the first uv word and tpage in the second; btl_setStoneClut moves clut */
typedef struct {
    s16 tpage;
    s16 clut;
} BtlTexWord;

/* FileEntry viewed with its first word split (file_find type 0xD) */
typedef struct {
    u16 id; /* low half of the 32-bit file id word; in dir 13's type-3 table the u16 entry itself: the sector offset (btlfx_loadFile) */
    u16 idHi; /* high half of the file id word: character models: sectors read into the party slot buffer (btl_loadPartyModelsStep, btl_reloadPartyStep); weapon models: their sound bank id (| DB_SOUND_BANK) */
    s32 sector; /* start sector (FileEntry.sector) */
    s32 nextId; /* the next entry's id word (not read) */
    s32 endSector; /* the next entry's start: size in sectors = endSector - sector (btl_reloadPartyStep) */
} FileEntry16;

/* double buffer used by the standalone display loop btlfx_halt */
typedef struct {
    DRAWENV draw;
    DISPENV disp;
    s16 index;
    u8 pad72[0x74 - 0x72];
} Unk80100E30Buf; /* 0x74 */

typedef struct {
    Unk80100E30Buf bufs[2];
    Unk80100E30Buf *cur; /* current */
} Unk80100E30;
extern Unk80100E30 g_btlfxHaltBufs;

/* model animation event list (db_lookup section 2, tag 0x14) */
typedef struct {
    union {
        s32 w; /* sound id */
        struct {
            u8 pad0[2];
            u8 kind; /* kind: 9 = the sound bank of a weapon model (btl_loadWeaponSoundStep loads data.w) */
        } b;
    } data;
    u8 pad4[0x10 - 4];
} BtlAnimEvt; /* 0x10 */

typedef struct {
    u16 unk0; /* not read */
    u16 count; /* count */
    BtlAnimEvt events[1];
} BtlAnimEvtList;

/* battle task with a step counter (btl_loadWeaponSoundStep) */
typedef struct {
    u8 pad0[6];
    s16 step; /* step */
} BtlStepTask;

/* work area of the effect step btlfx_opReturnHome */
typedef struct {
    u8 unk0; /* unused */
    u8 phase; /* phase */
    u8 unit; /* unit id */
    u8 stopped; /* the unit has Stop: moves home without its motion (btlfx_moveUnitHome) */
    u8 pad4[0xC - 4];
    s16 startPos[8]; /* the unit's position when the walk home starts (btlfx_moveUnitHome) */
    s16 startRot[6]; /* its rotation when the final turn starts (btlfx_turnUnitTowardFast) */
} Unk800E5538; /* 0x28 */

void btlfx_turnUnitTowardFast(s32, s16 *, s32, s32, s32);
s32 btlfx_moveUnitHome(s32, s32, s16 *, s32);
s32 btlfx_isCmdPendingOnAutoPotion(s32);

extern u8 g_btlSwdMagSourceAbilities[]; /* character 1 ability ids per Sword Magic entry */

/* main exe: candidates for ff9.h */
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */
/* draw environment pair built in the primitive buffer by btlfx_drawBgOffscreen */
typedef struct {
    DRAWENV env;
    DRAWENV prev; /* previous */
} DrawEnvPair; /* 0xB8 */

/* one primitive in a model's packed GPU packet buffer (BtlModel.prims): per mesh, polygons and DR_TPAGEs, closed by a
 * P_TAG with code BTL_PRIM_MESH_END (btl_buildMeshPrims). Evidence: [code] */
#define BTL_PRIM_MESH_END 0xFF
typedef union {
    P_TAG tag;
    u32 w[13]; /* w[1], w[3]/w[4], ...: rgb + code words */
} BtlPrim;

typedef struct BtlModel {
    u16 flags; /* BTL_MODEL_* */
    u8 pad2[4 - 2];
    u8 parentBone; /* weapon models: the bone of the parent model they follow (Character.unk21, btl_initWeapon) */
    u8 abr; /* semi-transparency mode (btl_setModelAbr) */
    u8 pad6[8 - 6];
    BtlModelHeader *header;
    BtlMotion *motion; /* playing motion (BtlUnit.motions entries) */
    s32 prims; /* built primitive packets (btl_setModelAbr walks them) */
    u32 hiddenMeshes; /* mesh-group bits not drawn (btl_hideMeshes) */
    BtlTexWord *texPages;
    struct BtlModel *parent; /* parent model (weapon models: the unit model) */
    BtlBone *bones; /* bone matrices (btl_calcBoneMatrices) */
    MATRIX matrix; /* model matrix */
    s32 unk44; /* write-only (cleared by btl_applyModelTextures) */
    u8 pad48[0x50 - 0x48];
    BtlUnit9C50 *texAnims; /* texture animations (btl_updateTexAnims) */
    s16 scaleX; /* with flag 2 (btl_applyModelScale) */
    s16 scaleY;
    s16 scaleZ;
    s16 sliceY; /* y limit of the slice effect (Hi_DrawSliceEffModel, btlfx_projectSliceMesh) */
    BtlModelHeader *morph; /* morph target */
    void *unk60; /* write-only (cleared by btl_applyModelTextures) */
    u8 pad64[0x74 - 0x64];
    s16 attachX; /* offset from the parent bone (btl_calcBoneMatrices; event op 0xD4 writes it through main's Unk8004AFF0) */
    s16 attachY;
    s16 attachZ;
} BtlModel;

/* BtlModel.flags (unit and weapon models). Evidence: [code] */
#define BTL_MODEL_SCALED    0x02 /* scaleX-Z apply (btl_applyModelScale; Mini, btlseq_opScale) */
#define BTL_MODEL_OFFSCREEN 0x10 /* behind the camera (btl_updateModelVisibility; a child takes its parent's): not drawn, no popups */
#define BTL_MODEL_DRAWN     0x20 /* a mesh group was drawn this frame (btl_updateUnitModel) */

/* GameData.unk8E0 entry as btl_registerModel fills it (ff9.h's ModelSlot has pad4) */
typedef struct {
    s32 flags; /* MODEL_SLOT_* */
    s32 modelId; /* model id */
    BtlModel *model;
    s32 obj;
} Unk8E0Ld;

typedef struct BtlUnit98 {
    s32 pos[3]; /* home position, 16.16 */
} BtlUnit98;

/* the party member's Character.maxHp/maxMp/maxAtb (runtime: BtlUnit.max == &party[i]->maxHp) */
typedef struct {
    u16 maxHp;
    s16 maxMp;
    s16 maxAtb;
} BtlUnit24;

/* current stats at BtlUnit.hp, parallel to BtlUnit24 */
typedef struct {
    u16 hp; /* hp */
    s16 mp; /* mp */
    s16 atb; /* atb */
    s8 atbSpeed;  /* atb speed */
} BtlUnit28;

/* battle unit; 8 of them at Battle.units, linked through next */

/* Battle.phase, as btl_update switches on it */
#define BTL_PHASE_SCRIPT   1 /* code: waits for the event script (btl_setBattleVar 33-36 act only now) */
#define BTL_PHASE_INTRO    2 /* code: btl_startBattle; btl_updateIntro loads and fades the units in */
#define BTL_PHASE_ENTRANCE 3 /* code: waits for the entrance motions, then combat and the ambush messages */
#define BTL_PHASE_COMBAT   4 /* runtime */
#define BTL_PHASE_END      5 /* runtime: btl_victory; Battle.subPhase says how (victory, defeat, escape) */
#define BTL_PHASE_EXIT     6 /* runtime: after a victory, fade out, then btl_saveParty */
#define BTL_PHASE_EXIT_DEFEAT 7 /* code: from a defeat (GameData.unk56E = 3) */
#define BTL_PHASE_EXIT_OTHER  8 /* code: from an escape or a script-ended battle */

/* Battle.subPhase in BTL_PHASE_END: how the battle ended (btl_victory switches on it) */
#define BTL_END_VICTORY        0 /* the enemies are down; rewards (BTL_PHASE_EXIT) */
#define BTL_END_DEFEAT         1 /* btl_checkSideDefeated: party down (BTL_PHASE_EXIT_DEFEAT) */
#define BTL_END_STOPPED        2 /* BTL_SETVAR_STOP_COMBAT: back to BTL_PHASE_SCRIPT */
#define BTL_END_ESCAPE         3 /* BTL_CMD_ESCAPE succeeded (BTL_PHASE_EXIT_OTHER) */
#define BTL_END_SCRIPT_VICTORY 4 /* BTL_SETVAR_END with result 1: handled like a victory */

/* event-script battle variables: btl_getBattleVar ids */
#define BTL_GETVAR_AMBUSH         32 /* scene.unk0 & 3: 0 back attack, 1 preemptive, 2 normal */
#define BTL_GETVAR_GIL            33
#define BTL_GETVAR_PHASE          34 /* Battle.phase (BTL_PHASE_*) */
#define BTL_GETVAR_SUB_PHASE      35 /* Battle.subPhase */
#define BTL_GETVAR_ATTACKER       36 /* unit bit of curCmd's caster */
#define BTL_GETVAR_TARGETS        37 /* curCmd's target mask */
#define BTL_GETVAR_WEAPON_ELEMENT 38 /* element of curCmd's caster's weapon */
#define BTL_GETVAR_ABILITY_ELEMENT 39 /* element of curCmd's ability */
#define BTL_GETVAR_LOAD_FLAGS     40 /* Battle.loadFlags */
#define BTL_GETVAR_EXP            41 /* g_btlRewards.exp */
#define BTL_GETVAR_EIKO_REBIRTH   42 /* the first CHR_KIND_EIKO* unit has BTL_CMD_REBIRTH_FLAME queued */
/* btl_setBattleVar ids */
#define BTL_SETVAR_STOP_COMBAT    32 /* phase END with subPhase 2: the units settle, then BTL_PHASE_SCRIPT */
#define BTL_SETVAR_END            33 /* in BTL_PHASE_SCRIPT: ends the battle with result n (GameData.unk56E) */
#define BTL_SETVAR_DEFEAT         34 /* in BTL_PHASE_SCRIPT: BTL_PHASE_EXIT_DEFEAT, result 3 */
#define BTL_SETVAR_RESUME         35 /* in BTL_PHASE_SCRIPT: back to BTL_PHASE_ENTRANCE, then combat */
#define BTL_SETVAR_PLAY_CAMERA    36 /* in BTL_PHASE_SCRIPT: btlcam_playScene(btl_getSceneData(n)) */
#define BTL_SETVAR_NEXT_LOCATION  37 /* evt_setNextLocation(n) */
#define BTL_SETVAR_ADD_GIL        38 /* gil += n, at most 9999999 */
#define BTL_SETVAR_ADD_DRAGON_KILL 39 /* GameData.dragonKills + 1, at most 9999 */
#define BTL_SETVAR_FADE_MUSIC     40 /* SND_MUSIC_VOL_INTPL fade, sets GD_BTL_VICTORY_MUSIC_FADE */

/* BtlEnemyType.category bits (btl_hasEnemyFlag). Names: bit j is Scan's text g_btlScanTexts[j] (btl_updateScan
 * prints one line per set bit); the Killer supports pair with them in btl_applyKillerBonus (SA_*_KILLER). */
#define BTL_ENEMY_HUMANOID 0x01 /* [text btl scan] "Humanoid"; SA_MAN_EATER; Eat fails on it */
#define BTL_ENEMY_BEAST    0x02 /* [text btl scan] "Beast"; SA_BEAST_KILLER */
#define BTL_ENEMY_DEMON    0x04 /* [text btl scan] "Demon"; SA_DEVIL_KILLER */
#define BTL_ENEMY_DRAGON   0x08 /* [text btl scan] "Dragon"; SA_DRAGON_KILLER; kills count GameData.dragonKills */
#define BTL_ENEMY_UNDEAD   0x10 /* [text btl scan] "Undead"; SA_UNDEAD_KILLER; code: Regen and healing hurt it like
                                 * Zombie (btl_tickRegen, btl_calcHeal) */
#define BTL_ENEMY_STONE    0x20 /* [text btl scan] "Stone"; SA_STONE_KILLER; Soft kills it ("too soft") */
#define BTL_ENEMY_INSECT   0x40 /* [text btl scan] "Insect"; SA_BUG_KILLER */
#define BTL_ENEMY_AERIAL   0x80 /* [text btl scan] "Aerial"; SA_BIRD_KILLER; code: Earth attacks miss it
                                 * (btl_missIfAirborne) */

/* btl_showMessage ids: entry id + 8 of the message table g_btlMessages (its first 8 entries are the element names);
 * the names are the US texts */
#define BTL_MSG_PREEMPTIVE         0x01 /* "Preemptive strike!" */
#define BTL_MSG_BACK_ATTACK        0x02 /* "Back attack!" */
#define BTL_MSG_CANT_ESCAPE        0x04 /* "Can't escape!" */
#define BTL_MSG_ANNIHILATED        0x05 /* "Annihilated..." */
#define BTL_MSG_TRANCE             0x06 /* "Trance!" */
#define BTL_MSG_COUNTER            0x07 /* "Counterattack!" */
#define BTL_MSG_RETURN_MAGIC       0x08 /* "Return magic!" */
#define BTL_MSG_AUTO_POTION        0x09 /* "Auto-Potion!" */
#define BTL_MSG_STEAL_FAILED       0x0A /* "Couldn't steal anything." */
#define BTL_MSG_NOTHING_TO_STEAL   0x0B /* "Doesn't have anything." */
#define BTL_MSG_STOLE              0x0C /* "Stole <item>!" */
#define BTL_MSG_SNAKE_BITE         0x0D /* "Bitten by Silence/Darkness/Poison Snake!" */
#define BTL_MSG_WEAK_AGAINST       0x0E /* "Became weak against <element>!" */
#define BTL_MSG_COMBINATION_FAILED 0x0F /* "Combination failed!" */
#define BTL_MSG_CANT_EAT_YET       0x10 /* "I no can eat until it weaker!" */
#define BTL_MSG_LEARNED            0x11 /* "Learned <ability>!" */
#define BTL_MSG_TASTE_BAD          0x12 /* "Taste bad!" */
#define BTL_MSG_IMPACT_CRUSHES     0x13 /* "Impact crushes the enemy!" */
#define BTL_MSG_NOT_ENOUGH_GIL     0x14 /* "Not enough Gil!" */
#define BTL_MSG_CANT_CAST          0x15 /* "Can't cast!" */
#define BTL_MSG_NOT_ENOUGH_MP      0x16 /* "Not enough MP!" */
#define BTL_MSG_CANT_REACH         0x17 /* "Can't reach!" */
#define BTL_MSG_NOT_ENOUGH_ITEMS   0x18 /* "Not enough items!" */
#define BTL_MSG_TOO_SOFT           0x19 /* "Became too soft to live..." */
#define BTL_MSG_CHARGE_FAILED      0x1A /* "Charge failed!" */
#define BTL_MSG_CANT_CONCENTRATE   0x1B /* "Command failed! <name> can't concentrate!" */
#define BTL_MSG_CANT_EAT           0x1D /* "I no can eat!" */
#define BTL_MSG_DROPPED_GIL        0x1E /* "Dropped <n> Gil!" */
#define BTL_MSG_WAS_STOLEN         0x1F /* "<item> was stolen!" */

/* g_btlUi->menuState: index of the per-state handler tables g_btluiMenuInput (input), g_btluiMenuUpdate, g_btluiMenuHelp */
#define BTLUI_MENU_NONE    0 /* runtime: no menu */
#define BTLUI_MENU_CMD     1 /* runtime: command menu */
#define BTLUI_MENU_LIST    2 /* code: ability/item list (btlui_listInput) */
#define BTLUI_MENU_TARGET  3 /* runtime: target select (btlui_targetInput) */
#define BTLUI_MENU_CLOSING 4 /* runtime: command issued, menu closing; no input */
#define BTLUI_MENU_KEEP    5 /* btlui_setMenuState ignores it */

/* btl_getUnitAttr / btl_setUnitAttr ids (event-script unit variables) */
#define BTL_ATTR_INSERT        0x20 /* set: btl_insertUnit */
#define BTL_ATTR_REMOVE        0x21 /* set: btl_cancelUnitCmds + btl_unlinkUnit */
#define BTL_ATTR_CANCEL_CMDS   0x22
#define BTL_ATTR_MAX_HP        0x23 /* runtime */
#define BTL_ATTR_HP            0x24 /* runtime */
#define BTL_ATTR_MAX_MP        0x25 /* runtime */
#define BTL_ATTR_MP            0x26 /* runtime */
#define BTL_ATTR_MAX_ATB       0x27 /* runtime */
#define BTL_ATTR_ATB           0x28
#define BTL_ATTR_LEVEL         0x29 /* runtime */
#define BTL_ATTR_IMMUNE_HI     0x2A /* statusImmune bits 24-31 */
#define BTL_ATTR_IMMUNE_LO     0x2B /* statusImmune bits 0-23 */
#define BTL_ATTR_LO_MASK       0xFFFFFF /* the *_LO attributes: bits 0-23 of the word (the *_HI ones read byte 3) */
#define BTL_ATTR_AUTO_STATUS_HI 0x2C
#define BTL_ATTR_AUTO_STATUS_LO 0x2D
#define BTL_ATTR_STATUS_HI     0x2E
#define BTL_ATTR_STATUS_LO     0x2F
#define BTL_ATTR_ELEM_GUARD    0x30 /* runtime: element bytes unk70-unk73 */
#define BTL_ATTR_ELEM_ABSORB   0x31
#define BTL_ATTR_ELEM_HALF     0x32
#define BTL_ATTR_ELEM_WEAK     0x33
#define BTL_ATTR_TARGETABLE    0x34 /* code: BtlUnit.flags bit 28 (btl_getUnitAttr), set through targetableReq */
#define BTL_ATTR_HIDDEN        0x35 /* code: BtlUnit.flags bit 22 (btl_setUnitAttr) */
#define BTL_ATTR_SHADOW        0x36 /* code: BtlUnit.flags bit 24, the shadow btl_updateUnitModel draws */
#define BTL_ATTR_SCALE         0x37 /* code: btlseq_opScale */
#define BTL_ATTR_RESET_SCALE   0x38
#define BTL_ATTR_MODEL         0x39
#define BTL_ATTR_HIDDEN_MESHES 0x3A /* get: BtlUnit.hiddenMeshes */
#define BTL_ATTR_HIDE_MESHES   0x3B
#define BTL_ATTR_SHOW_MESHES   0x3C
#define BTL_ATTR_TEXANIM_PLAY  0x3D /* set: plays model texture animation n (loops) */
#define BTL_ATTR_TEXANIM_STOP  0x3E /* set: stops texture animation n */
#define BTL_ATTR_TEXANIM_ONCE  0x3F /* set: plays texture animation n once */
#define BTL_ATTR_ROW           0x40 /* get: BTL_UNIT_ROW */
#define BTL_ATTR_SLOT          0x41 /* get: unit slot (BtlUnit.flags bits 5-7) */
#define BTL_ATTR_KIND          0x42 /* get: the character's CHR_KIND_* (Character.info.b.kind) */
#define BTL_ATTR_CLASS_FLAGS   0x43 /* get: Character.classFlags (CHR_CLASS_*) */
#define BTL_ATTR_CATEGORY      0x44 /* get: the enemy's BtlEnemyType.category (BTL_ENEMY_*) */
#define BTL_ATTR_WEAK_STANCE   0x45 /* BtlUnit.flags BTL_UNIT_WEAK_STANCE */
#define BTL_ATTR_INDEX         0x46 /* get: BTL_UNIT_INDEX (character id or enemy index) */
#define BTL_ATTR_ADD_REWARDS   0x47 /* set: btl_addEnemyRewards */
#define BTL_ATTR_STRENGTH      0x48
#define BTL_ATTR_MAGIC         0x49
#define BTL_ATTR_DEFENCE       0x4A
#define BTL_ATTR_EVADE         0x4B
#define BTL_ATTR_MAGIC_DEFENCE 0x4C
#define BTL_ATTR_MAGIC_EVADE   0x4D
#define BTL_ATTR_TERRA_HOMING  0x4E /* set: fills the ATB and queues BTL_CMD_SUMMON2 ABIL_TERRA_HOMING at targets n */

typedef struct BtlUnit {
    struct BtlUnit *next; /* active-unit list (Battle.unitList) */
    struct BtlCmd *cmd; /* six command slots (btl_initUnitCmds; enemies have the first three): chosen command */
    struct BtlCmd *autoCmd; /* btl_queueAutoCmd: counter, Return Magic, Auto-Potion, Spear, enemy reactions */
    struct BtlCmd *koCmd; /* BTL_CMD_KO / BTL_CMD_PETRIFY */
    struct BtlCmd *delayedCmd; /* Jump while in the air, trance Eidolon */
    struct BtlCmd *tranceCmd; /* BTL_CMD_TRANCE */
    s32 reviveCmd; /* BtlCmd *: BTL_CMD_AUTO_LIFE */
    u16 bounceMasks[4]; /* Reflect: units each reflected copy hits (btl_applyReflect), counted by btl_applyCalcResult */
    BtlUnit24 *max;
    u16 hp;
    s16 mp;
    s16 atb; /* fills over time, stops when the command menu is ready (runtime) */
    s8 atbSpeed; /* added to atb per tick (btl_updateAtb); btl_resetAtbSpeed */
    u8 pad2F;
    u8 speed; /* Character.speed (btl_initPartyUnit) */
    u8 strength;
    u8 magic;
    u8 spirit;
    s32 statusImmune; /* add-status (btl_addStatus) refuses these bits; runtime: Iifa monster 0x4400E004 */
    u32 autoStatus; /* permanent BTL_STATUS_* bits: support abilities set them at battle start (e.g. BTL_STATUS_REGEN); add-status (btl_addStatus) does nothing for a bit already set here */
    u32 status; /* current BTL_STATUS_* bits; runtime: Sleep 0x20000, KO 0x100 (added by btl_addStatus) */
    s16 dotTimers[3]; /* Venom/Poison/Regen tick timers (btl_initDotTimer) */
    s16 statusTimers[16]; /* per timed status bit 16-31 (btl_tickStatusTimers) */
    s16 countdownStep; /* timer units per number of the Doom/petrify countdown (btl_initDoomCount, btl_drawStatusCountdown) */
    u32 flags; /* BTL_UNIT_* below */
    u8 defense; /* Character.defense (btl_initPartyUnit) */
    u8 evade;
    u8 magicDef;
    u8 magicEva;
    u8 elemGuard; /* element bits taking no damage ("Guard", btl_applyWeaponElement) */
    u8 elemAbsorb; /* absorbed: the damage heals */
    u8 elemHalf;
    u8 elemWeak; /* x1.5 */
    u8 *weapon; /* weapon stats (g_weaponTable entry, btl_initWeapon): [1] status set, [5] power, [6] element */
    u8 trance; /* gauge 0-0xFF (Character.trance; btl_gainTrance) */
    u8 elemBoost; /* element bits dealt x1.5 (btl_applyWeaponElementBoost) */
    u8 level; /* Character.level, enemy record level */
    u8 escapeStance; /* Battle.escapeHeld when the idle motion was last chosen (btl_updateUnits re-picks it on a change) */
    u8 baseBone; /* bone used as the unit position (btl_getBaseBonePos) */
    u8 koState; /* BTL_KO_*: KO state machine step (btl_updateKo) */
    u16 popupFlags; /* BTL_POPUP_*: what btl_spawnPopups shows (BtlCalc.targetResult/casterResult or-ed in) */
    s16 hpChange; /* damage or heal shown by btl_spawnPopups */
    s16 mpChange;
    u16 modelId;
    s16 rot[3]; /* rot[1] = facing */
    s32 pos[3]; /* position, 16.16 */
    struct BtlUnit98 *home; /* home position block */
    BtlModel *model;
    BtlUnitA0 *obj; /* model object entry (GameData.unk8E0): start position/rotation, motion frame */
    s32 *motions; /* motion table (BtlMotion * per slot); model->motion is the playing one */
    BtlModel *weaponModel;
    u16 hiddenMeshes; /* BTL_ATTR_HIDE_MESHES / BTL_ATTR_SHOW_MESHES */
    u16 vanishMeshes; /* mesh groups Vanish hides */
    u16 unitBit; /* bit in target masks (BtlCmd.targets): party 0-3, enemies from 4 */
    u8 targetableReq; /* BTL_TGTREQ_*: BTL_ATTR_TARGETABLE, Jump, landing */
    u8 cmdIssued; /* set when the menu issues its command, cleared when it ends */
    struct Unk80104F88Entry *menu; /* party member menu data */
    s16 screenX;
    s16 screenY;
    u8 onScreen; /* screenX/Y valid (btl_projectUnitPos) */
    u8 shadowBone; /* bone the shadow sits under (btl_calcShadowMatrix) */
    u8 shadowDirBone; /* bone the shadow faces toward from shadowBone; = shadowBone: that bone's own axis */
    u8 shadowW; /* shadow width / 14 (btl_setShadowSize) */
    u8 shadowD; /* shadow depth / 12 */
    u8 tint[3]; /* status colour (Zombie, Berserk, Heat, Freeze) btl_updateUnitTint fills the models with */
    u32 *support; /* support-ability bits (Character.unk88) */
    s16 tickHeal; /* btl_tickRegen */
    s16 tickDamage; /* btl_tickPoison */
    s16 tickMpDamage; /* btl_tickVenomMp */
    u8 tickFlags; /* BTL_TICK_*: popups btl_spawnTickPopups shows */
    u8 inMenuQueue; /* waiting for its command menu (btlui_queueMenuUnit) */
} BtlUnit; /* 0xD0 */

/* BtlUnit.motions slots (party: 0x22, btl_loadPartyMotions; enemies: BtlEnemyType.motionIds). Runtime: Zidane's model
 * poked to slots 2 and 3; 0x17 / 0x19 seen in Attack; 8, 0x12, 0x13, 0x1A-0x20 [runtime] (R41: Zidane's model poked
 * to each slot, and black magic / item traces, docs/runtime-findings.md). The rest [code]: what btl_setIdleMotion,
 * btl_endPartyHitMotion, btl_setCmd and the command code switch between (docs/battle-models.md). */
#define BTL_MOTION_IDLE      0
#define BTL_MOTION_IDLE_WEAK 1 /* idle in weak stance (BTL_UNIT_WEAK_STANCE): Venom, Low HP, Poison, Sleep */
#define BTL_MOTION_HIT       2 /* short flinch, head back */
#define BTL_MOTION_HIT_HEAVY 3 /* party: knocked off the feet backwards (abilities with flags2 0x81); enemies: the death
                                * motion, or the hit in weak stance */
#define BTL_MOTION_KO        4 /* btl_setKoMotion */
#define BTL_MOTION_WEAK_TO_IDLE 5 /* the weak statuses went away (from IDLE_WEAK or GET_UP) */
#define BTL_MOTION_GET_UP    6 /* after HIT_HEAVY, or when revived from KO; ends in weak stance */
#define BTL_MOTION_IDLE_TO_WEAK 7 /* a weak status came while in IDLE */
#define BTL_MOTION_KO_FALL   8 /* kneel, slump and end lying down (btl_updateKo), then KO */
#define BTL_MOTION_READY     9 /* holds a command (BTL_UNIT_CMD_WAITING) or acts */
#define BTL_MOTION_IDLE_TO_READY 10 /* + 1 from IDLE_WEAK (WEAK_TO_READY): a command was set (btl_setCmd) */
#define BTL_MOTION_WEAK_TO_READY 11
#define BTL_MOTION_DEFEND    12 /* the Defend command */
#define BTL_MOTION_DEFEND_IDLE 13 /* held while BTL_STATUS_DEFEND */
#define BTL_MOTION_DEFEND_TO_READY 14 /* from DEFEND_IDLE when a menu command is set */
#define BTL_MOTION_COVER     15 /* stepped in front of the target (btl_findCover) */
#define BTL_MOTION_DODGE     16 /* party target dodged (BTL_CALC_DODGED) */
#define BTL_MOTION_FLEE      17 /* Flee command, or escape held */
#define BTL_MOTION_VICTORY   0x12 /* crouch, leap, weapon raised: at the victory when Character.info.b.victoryPose */
#define BTL_MOTION_VICTORY_LOOP 0x13 /* short loop, weapon overhead: each time the motion ends in BTL_PHASE_EXIT */
#define BTL_MOTION_ATTACK    0x17 /* run in and strike (btlfx_opPlayAttackMotion) */
#define BTL_MOTION_CAST_START 0x1A /* btlfx_opCast, BTLFX_CAST_PARTY: after stepping forward */
#define BTL_MOTION_CAST_LOOP  0x1B /* when CAST_START ends, while the cast emitter runs */
#define BTL_MOTION_CAST_END   0x1C /* once the effect signals (signal 2), then READY */
#define BTL_MOTION_STEP_FORWARD 0x1D /* step towards the enemies before casting (btlfx_stepForward) */
#define BTL_MOTION_STEP_HOME  0x1E /* walk back home (btlfx_moveUnitHome) */
#define BTL_MOTION_ITEM       0x1F /* btlfx_opCast, BTLFX_CAST_ITEM */
#define BTL_MOTION_READY_TO_IDLE 0x20 /* lower the guard back to idle: home after acting (btlfx_opReturnHome), or a READY
                                       * unit's command dropped (btl_cmd), or before KO_FALL (btl_updateKo) */
#define BTL_MOTION_ATTACK_RETURN 0x19 /* jump back after attacking an enemy (btlfx_opPlayAttackReturnMotion) */
#define BTL_MOTION_CHANGE_ROW BTL_MOTION_STEP_FORWARD /* + BTL_UNIT_ROW: the Change command reuses the step motions */

/* BtlUnit.flags (bitfields in Square's source: single bits are read as (flags >> NAME_BIT) & 1). Evidence: [code] */
#define BTL_UNIT_PARTY             (1 << 0) /* party member, else enemy: the side (btl_initPartyUnit sets it, btl_initEnemies clears it) */
#define BTL_UNIT_INDEX_SHIFT       1 /* bits 1-4: character id (CHR_*, GameData.characters) or Battle.enemies index */
#define BTL_UNIT_INDEX_MASK        (0xF << BTL_UNIT_INDEX_SHIFT)
#define BTL_UNIT_INDEX(f)          (((f) >> BTL_UNIT_INDEX_SHIFT) & 0xF)
#define BTL_UNIT_SLOT_SHIFT        5 /* bits 5-7: unit slot, party 0-3 and enemies 4-7 (btl_getUnitBySlot, expMask bit, command slots) */
#define BTL_UNIT_SLOT_MASK         (7 << BTL_UNIT_SLOT_SHIFT)
#define BTL_UNIT_SLOT(f)           (((f) >> BTL_UNIT_SLOT_SHIFT) & 7)
#define BTL_UNIT_ROW_SHIFT         8 /* bits 8-9: row, Character battleRow for the party (btl_toggleRow, btl_applyRowMod), 2 for enemies */
#define BTL_UNIT_ROW_MASK          (3 << BTL_UNIT_ROW_SHIFT)
#define BTL_UNIT_ROW(f)            (((f) >> BTL_UNIT_ROW_SHIFT) & 3)
#define BTL_UNIT_ATB_ACTIVE_BIT    16 /* the ATB gauge fills and status timers tick (btl_updateAtb, btl_updateUnitStatus); cleared by KO, Petrify, Stop, Jump and cancelled commands */
#define BTL_UNIT_ATB_ACTIVE        (1 << BTL_UNIT_ATB_ACTIVE_BIT)
#define BTL_UNIT_HIT_MOTION_BIT    17 /* hit reaction playing (btl_startHitMotion .. btl_end*HitMotion); btl_setIdleMotion leaves the unit alone */
#define BTL_UNIT_HIT_MOTION        (1 << BTL_UNIT_HIT_MOTION_BIT)
#define BTL_UNIT_WEAK_STANCE_BIT   18 /* idle motion 1 instead of 0: Venom, Low HP, Poison or Sleep (btl_updateHpStatus, btlseq_opSetStance) */
#define BTL_UNIT_WEAK_STANCE       (1 << BTL_UNIT_WEAK_STANCE_BIT)
#define BTL_UNIT_CMD_WAITING_BIT   19 /* a command is queued and the unit holds its ready motion (btl_setCmd, btl_setEnemyCmd; cleared when it leaves the queue) */
#define BTL_UNIT_CMD_WAITING       (1 << BTL_UNIT_CMD_WAITING_BIT)
#define BTL_UNIT_KILLED_BIT        20 /* hp set to 0 outright (btl_killUnit, btl_setTargetHp, btl_checkFrozenShatter): the KO skips the dying steps */
#define BTL_UNIT_KILLED            (1 << BTL_UNIT_KILLED_BIT)
#define BTL_UNIT_MOTION_HELD_BIT   21 /* motion frame not advanced (btl_updateUnitModel; btlfx_setMotionHeld, btlfx_holdMotion) */
#define BTL_UNIT_MOTION_HELD       (1 << BTL_UNIT_MOTION_HELD_BIT)
#define BTL_UNIT_HIDDEN_BIT        22 /* not drawn, no popups or status icons (BTL_ATTR_HIDDEN, btlfx_setUnitVisible, btl_setUnitFade) */
#define BTL_UNIT_HIDDEN            (1 << BTL_UNIT_HIDDEN_BIT)
#define BTL_UNIT_MODEL_PART_BIT    23 /* enemy without a model of its own, a part of another unit's (BtlEnemy flag 0x200; btl_isModelPart, btl_findBodyUnit) */
#define BTL_UNIT_MODEL_PART        (1 << BTL_UNIT_MODEL_PART_BIT)
#define BTL_UNIT_SHADOW_BIT        24 /* shadow drawn (BTL_ATTR_SHADOW, btlseq_opShowShadow, btl_updateUnitModel) */
#define BTL_UNIT_SHADOW            (1 << BTL_UNIT_SHADOW_BIT)
#define BTL_UNIT_COVERING_BIT      25 /* stepped in front of the target with Cover / Protect Girls (btl_findCover); the attack can't be dodged (btl_rollHit) */
#define BTL_UNIT_COVERING          (1 << BTL_UNIT_COVERING_BIT)
#define BTL_UNIT_DODGED_BIT        26 /* dodged the current command and stepped back (btl_applyCalcResult; btl_finishCmd moves it home) */
#define BTL_UNIT_DODGED            (1 << BTL_UNIT_DODGED_BIT)
#define BTL_UNIT_DEATH_CRY_BIT     27 /* death cry played once (btl_playDeathCry) */
#define BTL_UNIT_DEATH_CRY         (1 << BTL_UNIT_DEATH_CRY_BIT)
#define BTL_UNIT_TARGETABLE_BIT    28 /* can be targeted (BTL_ATTR_TARGETABLE, btlui_applyTargetable) */
#define BTL_UNIT_TARGETABLE        (1 << BTL_UNIT_TARGETABLE_BIT)
#define BTL_UNIT_TARGETED_BIT      29 /* under the target cursor (btlui_setDefaultTargets, btlui_cycleTarget, btlui_clearTargeted) */
#define BTL_UNIT_TARGETED          (1 << BTL_UNIT_TARGETED_BIT)
#define BTL_UNIT_TRANCE_ENABLED_BIT 30 /* the trance gauge fills (btl_gainTrance) and shows (btlui_updatePanel); party only, off before the story allows it */
#define BTL_UNIT_TRANCE_ENABLED    (1 << BTL_UNIT_TRANCE_ENABLED_BIT)

/* BtlUnit.koState (btl_updateKo; enemies and party units step differently). Evidence: [code] */
#define BTL_KO_NONE           0
#define BTL_KO_START          1 /* BTL_CMD_KO ran (btl_checkCmdUsable) or the hit motion ended KO */
#define BTL_KO_ENEMY_DYING    2 /* the death motion plays, then is held on its last frame */
#define BTL_KO_ENEMY_SOUND    3 /* death sounds (also the start for BTL_UNIT_KILLED enemies) */
#define BTL_KO_ENEMY_FADE     4 /* fades out by the BtlEnemy fade level; no shadow from here on (btl_updateUnitModel) */
#define BTL_KO_ENEMY_REMOVE   5 /* rewards, btl_checkSideDefeated, btl_unlinkUnit */
#define BTL_KO_PARTY_FALL     2 /* picks the collapse motion (7, or 8 straight away in the weak stance) */
#define BTL_KO_PARTY_COLLAPSE 3 /* motion 7 plays, then motion 8 */
#define BTL_KO_PARTY_DOWN     4 /* motion 8 plays */
#define BTL_KO_PARTY_SET      5 /* btl_setKoMotion: the KO motion (4), then DONE or REVIVING */
#define BTL_KO_DONE           6 /* down for good (btl_checkSideDefeated waits for it on the whole side) */
#define BTL_KO_REVIVING       7 /* Auto-Life queued its revive command (btl_setKoMotion) */

/* BtlUnit.targetableReq: BTL_UNIT_TARGETABLE change requested for btlui_applyTargetable (next frame) */
#define BTL_TGTREQ_OFF     0 /* untargetable (applied) */
#define BTL_TGTREQ_ON      1 /* targetable (applied) */
#define BTL_TGTREQ_SET_OFF 2 /* make it untargetable */
#define BTL_TGTREQ_SET_ON  3 /* make it targetable */

/* BtlUnit.tickFlags: status tick popups (btl_spawnTickPopups). Evidence: [code] */
#define BTL_TICK_REGEN       0x01 /* btl_tickRegen: tickHeal */
#define BTL_TICK_POISON      0x02 /* btl_tickPoison: tickDamage */
#define BTL_TICK_VENOM_MP    0x04 /* btl_tickVenomMp: tickMpDamage */
#define BTL_TICK_REGEN_HURTS 0x08 /* Regen damaged a Zombie or undead unit: the heal number is shown as damage */

/* BtlUnit.popupFlags, BtlCalc.targetResult / casterResult (bits 0-4): what btl_spawnPopups shows. Text popups index
 * g_btlTextPopups: 0 miss, 1 death, 2 guard, 3 critical. Evidence: [code] */
#define BTL_POPUP_HP       0x01 /* hp number (hpChange; btl_damageHp / btl_healHp in btl_applyCalcResult) */
#define BTL_POPUP_HEAL     0x02 /* the hp number heals (clut 0xC0; also absorbed damage, btl_finishMagicDamage) */
#define BTL_POPUP_CRITICAL 0x04 /* "Critical" text (btl_rollCritical); btl_startHitMotion knocks the unit back */
#define BTL_POPUP_MP       0x08 /* mp number (mpChange) */
#define BTL_POPUP_MP_HEAL  0x10 /* the mp number heals */
#define BTL_POPUP_MISS     0x20 /* "Miss" text: BTL_CALC_MISS, or the target is KO (btl_damageHp); no hit sound (btl_playHitSound) */
#define BTL_POPUP_DEATH    0x40 /* "Death" text: KO added to a unit with hp left (btl_addStatus) */
#define BTL_POPUP_GUARD    0x80 /* "Guard" text: BTL_CALC_GUARD; shown alone */
#define BTL_POPUP_TROUBLE  0x100 /* the target has Trouble (btl_markTrouble): btl_spawnPopups spreads half the damage to its side (btl_spreadTroubleDamage) */

/* ability/command calculation context */
/* BtlCmd seen by the formulas (btl_calcCmd's sub = the BtlCmd; flags = BtlCmd.info.w) */
typedef struct BtlCalc8 {
    u8 pad0[8];
    struct BtlCmd8 *ability;
    u16 targets;
    u8 cmdNo;
    u8 subNo;
    u32 flags;
} BtlCalc8;

typedef struct BtlCalc {
    BtlUnit *caster; /* caster */
    BtlUnit *target; /* target */
    BtlCalc8 *cmd;
    s16 statusRate; /* weapon add-status chance (weapon[7]) */
    s16 power; /* base power (weapon or ability) */
    s16 defense; /* target defense */
    s16 evade; /* target evade % */
    s16 multiplier;
    s16 hitRate; /* hit % */
    u16 flags; /* BTL_CALC_* */
    u8 casterResult; /* caster popup flags (BTL_POPUP_*) */
    u8 targetResult; /* target popup flags (BTL_POPUP_*) */
    s16 casterHpChange; /* caster hp change */
    s16 targetHpChange; /* target hp change */
    s16 casterMpChange; /* caster mp change */
    s16 targetMpChange; /* target mp change */
} BtlCalc;

/* BtlCalc.flags: outcome of the formulas, applied by btl_applyCalcResult. Evidence: [code] */
#define BTL_CALC_MISS          0x01 /* missed (hit/evade rolls, formula checks such as btl_checkTargetAlive): BTL_POPUP_MISS */
#define BTL_CALC_DODGED        0x02 /* with MISS: evaded, the target plays its dodge motion (btl_rollHit; not when covered) */
#define BTL_CALC_MP_ATTACK     0x04 /* MP Attack raised the damage: btl_payMpAttack takes the caster's mp */
#define BTL_CALC_ABSORB        0x08 /* the target absorbs the element: the damage heals (btl_apply*Element) */
#define BTL_CALC_GUARD         0x40 /* nullified: element guard (btl_apply*Element) or immunity (btl_guardIfDeathImmune): BTL_POPUP_GUARD */
#define BTL_CALC_NO_DAMAGE     0x80 /* hit without damage numbers: hit motion and trance only (btl_applyCalcResult) */
#define BTL_CALC_WEAPON_STATUS 0x100 /* add the caster's weapon status set (weapon[1]) on a hit */

/* btl_calcCmd balance bands and odds: what each threshold decides (the values are Square's data). Rolls are RAND8()
 * (0-255) unless noted; RAND16() % 100 rolls are percentages. Evidence: [code] */
/* Steal (BTL_FORMULA_STEAL): Enemy steal slots 3 (rarest) .. 0 are tried in that order, each slot taking the roll
 * RAND8() < odds; slot 0 takes what is left. Master Thief raises the odds and skips empty slots. */
#define BTL_STEAL_ODDS_SLOT3     1  /* 1/256 */
#define BTL_STEAL_ODDS_SLOT2     16 /* 16/256 */
#define BTL_STEAL_ODDS_SLOT1     64 /* 64/256 */
#define BTL_STEAL_MT_ODDS_SLOT3  32 /* Master Thief, slot 3 filled: 32/256 */
#define BTL_STEAL_MT_ODDS_SLOT2  32 /* Master Thief, slot 2 filled */
#define BTL_STEAL_MT_ODDS_SLOT1  64 /* Master Thief, slot 1 filled */
/* Six Dragons (BTL_FORMULA_SIX_DRAGONS): one RAND16() % 100 roll, cumulative upper bounds of each outcome band */
#define BTL_SIX_DRAGONS_FULL_RESTORE 10 /* < 10: HP and MP to max (10%) */
#define BTL_SIX_DRAGONS_HP_RESTORE   30 /* < 30: HP to max (20%) */
#define BTL_SIX_DRAGONS_MP_RESTORE   50 /* < 50: MP to max (20%) */
#define BTL_SIX_DRAGONS_HP_TO_1      65 /* < 65: HP to 1 (15%) */
#define BTL_SIX_DRAGONS_MP_TO_1      80 /* < 80: MP to 1 (15%); the other 20%: HP and MP to 1 */
/* other odds */
#define BTL_COMET_HIT_ODDS          0xAB /* BTL_FORMULA_RANDOM_MAGIC: an effect-BTLFX_EFFECT_COMET ability hits when RAND8() < 171 (2/3) */
#define BTL_SHORT_SUMMON_ODDS_RICH  0xE6 /* btl_rollShortSummon: caster's mp > 2 x the cost, RAND8() < 230 casts the short version */
#define BTL_SHORT_SUMMON_ODDS       0xAA /* btl_rollShortSummon otherwise: RAND8() < 170 */
/* healing items (BTL_FORMULA_ITEM_HEAL / ITEM_MP_HEAL): multiplier on the item's power */
#define BTL_ITEM_HEAL_MULT          15 /* HP/MP = item power x 15 */
#define BTL_ITEM_HEAL_MULT_CHEMIST  30 /* with Chemist: doubled */
/* random low HP values */
#define BTL_REVIVE_HP_RANGE         10 /* Phoenix Down revives with RAND8() % 10 + 1 HP; on undead/zombie sets 0-9 */
#define BTL_LOW_HP_RANGE            9  /* BTL_FORMULA_LOW_HP_STATUS, and the Low HP status of BTL_FORMULA_RANDOM_STATUS: hp = RAND8() % 9 + 1 */
#define BTL_SPEAR_TRANCE_MIN_MULT   4  /* BTL_FORMULA_SPEAR_TRANCE: multiplier raised to at least 4 before the split over the targets */
#define BTL_GRUDGE_CAP_EXP 14 /* BTL_FORMULA_GRUDGE: from exponent 14 (1 << 13 = 8192; the 7th Tonberry kill) the damage
                              * is DAMAGE_MAX */

/* BtlCmd as the sequence interpreter reads it (BtlScript.cmd) */
typedef struct {
    u8 pad0[4];
    struct BtlUnit *caster;
    struct BtlCmd8 *ability;
    u16 targets;
} BtlScript4;

typedef struct BtlScript {
    u32 flags; /* BTLSEQ_F_* */
    BtlScript4 *cmd; /* command it runs for, NULL = free slot */
    u8 *pc; /* script pointer */
    u8 *prevPc; /* an op's enter handler runs when pc moved */
    s16 opFrame; /* frames in the current op */
    s16 waitLeft; /* btlseq_opWait counter */
    s16 frame; /* frames since the start */
    u8 seqFlags; /* Battle.seq.seqFlags of the sequence */
    u8 soundDelay; /* btlseq_opDelayedSound */
    s16 sound;
    u8 pad1A;
    u8 soundVol;
    u16 turnFrom; /* btlseq_opTurn: start angle */
    s16 turnBy;
    u8 turnFrame;
    u8 turnFrames; /* 0 = not turning */
    s16 fxId; /* btlseq_opDelayedEffect */
    u8 fxArg;
    u8 fxDelay;
    u8 fadeFrames; /* btlseq_opFadeOut */
    u8 fadeLeft;
    s16 work[8]; /* per-op work area */
} BtlScript;

/* BtlScript.flags (Battle122C.flags). Evidence: [code] */
/* btlseq_opTurn angle word. Evidence: [code] */
#define BTLSEQ_TURN_SPECIAL     0x8000 /* the low bits pick a direction (BTLSEQ_TURN_*), else they are the angle */
#define BTLSEQ_TURN_MODE        0x7FFF
#define BTLSEQ_TURN_MODEL       0      /* the model's own rotation */
#define BTLSEQ_TURN_TARGETS     1      /* ratan2 of the unit minus the targets' centre (btlseq_getTargetCenter) */
/* btlseq_opShowName argument. Evidence: [code] */
#define BTLSEQ_NAME_CMD         0x80   /* the command's ability name (else the enemy's ability idx) */
#define BTLSEQ_F_WAIT_HIT       0x01 /* btlseq_opEffect started an effect; the effect's seqSync turns it into HIT */
#define BTLSEQ_F_HIT_BIT        1
#define BTLSEQ_F_HIT            (1 << BTLSEQ_F_HIT_BIT) /* the effect hit (btlseq_opWaitEffectHit goes on) */
#define BTLSEQ_F_END_MOTION_BIT 2
#define BTLSEQ_F_END_MOTION     (1 << BTLSEQ_F_END_MOTION_BIT) /* btlseq_opEnd put the idle motion back */
#define BTLSEQ_F_DIRECT_BIT     3
#define BTLSEQ_F_DIRECT         (1 << BTLSEQ_F_DIRECT_BIT) /* started without a command (btlseq_startDirect): no btl_onCmdEffectDone at the end */
typedef struct {
    void (*enter)(BtlScript *, BtlUnit *); /* on entry (pc moved) */
    s32 (*step)(BtlScript *, BtlUnit *); /* per frame; 0 = next op */
} BtlScriptOp; /* script opcode handlers: on entry, per frame */
extern BtlScriptOp g_btlseqOps[];
void btl_setUnitFade(BtlUnit *, s32);


/* effect unit list (Battle.fxTargets, g_btlfxEnv->targets); effect scripts name units by index: 0-7 targets, 8.. bounced, 0x10 caster, 0x11 partner */
typedef struct {
    BtlUnit *caster; /* id 0x10 */
    BtlUnit *targets[8]; /* ids 0..7 */
    u8 targetCount; /* count of targets */
    u8 fxArgs[2]; /* btl_playCmdEffect fxArgs[0-1] - unit ids (btlfx_setPartnerByArg) or bone numbers (btlfx_getUnitArgBone*) */
    u8 bounceCount; /* count of bounced */
    BtlUnit *bounced[4]; /* ids 8..: units a reflected spell bounces to (btl_playCmdEffect) */
    s16 centerX; /* average target position (btl_playCmdEffect) */
    s16 centerY;
    s16 centerZ;
    u8 pad3E[0x40 - 0x3E];
    u16 kind; /* BTL_FXK_* (btl_playCmdEffect: from the command, or fxArgs[3]) */
    u16 fxParam; /* btl_playCmdEffect fxArgs[2] (fx_031: distance ahead of the fxArgs bones) */
    BtlUnit *partner; /* id 0x11: the Swd Mag partner (character kind 2) */
    u8 pad48[0x4C - 0x48];
} BtlUnitList; /* 0x4C */
/* BtlUnitList ids beyond the targets (btl_getUnit). Evidence: [code] */
#define BTLFX_UNIT_BOUNCED 8    /* + i: BtlUnitList.bounced[i]; ids below are BtlUnitList.targets[id] */
#define BTLFX_UNIT_CASTER  0x10 /* BtlUnitList.caster */
#define BTLFX_UNIT_PARTNER 0x11 /* BtlUnitList.partner */
#define BTLFX_UNIT_NONE    0xFF /* no unit: the effect ops skip it or use the targets' centre */
#define BTLFX_UNIT_RANDOM_TARGET 0xFE /* btlfx_opApplyResult: a random one of the targets */
#define BTLFX_UNIT_MODEL_RADIUS 0x100 /* btlfx_getUnitRadius: | id: the model's radius even for an enemy (else the enemy type's) */
/* btlfx_fadeUnitGroup groups (op 0x24 btlfx_opFadeUnits: arg 1 & BTLFX_FADE_GROUP; cast task). Evidence: [code] */
#define BTLFX_FADE_PARTY       0 /* the party units */
#define BTLFX_FADE_ENEMIES     1
#define BTLFX_FADE_ALL         2
#define BTLFX_FADE_OUT         4 /* btlfx_opFadeUnits: from 0x80 down to 0 (else 0 up to 0x80) */
#define BTLFX_FADE_CASTER_SIDE 8 /* the enemies if the caster is one, else the other party units */
#define BTLFX_FADE_CASTER      9
#define BTLFX_FADE_GROUP       0xB /* btlfx_opFadeUnits: the group bits of its argument */
/* effect script table entry (resident module, btlfx_findScript): u8 id, u8 BTLFX_SCRENT_*, then 3-byte ops. Evidence: [code] */
#define BTLFX_SCRENT_ID_HI 0x80 /* id + 0x100 */
#define BTLFX_SCRENT_OPS   0x7F /* op count */
/* btlfx_opSetSpriteClut argument 1. Evidence: [code] */
#define BTLFX_SPRCLUT_INDEX 0x1F /* g_btlfxCommonCluts index */
#define BTLFX_SPRCLUT_SLOT  0xE0 /* 0-1: the sprite's first clut word, 2-3: the second */
#define BTLFX_SPRCLUT_SLOT_SHIFT 5
#define BTLFX_ABR_OPAQUE 0xFF /* abr / tpage argument of the btlfx draw helpers: no semi-transparency (code byte without GPU_ABE, no DR_TPAGE) */
#define BTLFX_ABR_HALF     0    /* abr argument of the btlfx draw helpers: semi-transparency rate 0 (back/2 + front/2) */
#define BTLFX_ABR_ADD      1    /* rate 1: back + front (additive glow) */
#define BTLFX_ABR_SUB      2    /* rate 2: back - front (subtractive, darkens) */
#define BTLFX_ABR_QUARTER  3    /* rate 3: back + front/4 */
/* effect ids (BtlfxState.effectId) tested by ovl_0b, identities from docs/effect-modules.md */
#define BTLFX_EFFECT_ROULETTE  0x0ED /* fx_0ed Roulette: btlfx_op does not turn the caster towards its target */
#define BTLFX_EFFECT_ARK_SHORT 0x1BF /* fx_1bf Ark (short) / Eternal Darkness: the caster returns to its idle motion afterwards */
/* fixed effect ids of btl_startCmdEffect (docs/runtime-findings.md, round 31 static sweep and BTLFX_HDR_RESIDENT:
 * the Attack and Steal ids are resident physical-hit scripts without a dir-13 file) */
#define BTLFX_EFFECT_COMET           0x038 /* fx_038 Comet: BTL_FORMULA_RANDOM_MAGIC only deals damage when RAND8() < 0xAB */
#define BTLFX_EFFECT_ATTACK          0x064 /* + CHR_KIND: the party Attacks 0x64-0x76 */
#define BTLFX_EFFECT_ATTACK_ENEMY    0x077 /* enemy Attack, also Marcus's Steal */
#define BTLFX_EFFECT_STEAL_CINNA     0x013
#define BTLFX_EFFECT_STEAL           0x014 /* Steal by any other kind */
#define BTLFX_EFFECT_STEAL_ZIDANE_DAGGER 0x0C8 /* CHR_KIND_ZIDANE_DAGGER */
#define BTLFX_EFFECT_STEAL_ZIDANE    0x111 /* CHR_KIND_ZIDANE */
#define BTLFX_EFFECT_TRANCE          0x101 /* fx_101 the Trance transformation (Trance command while in Trance) */
#define BTLFX_EFFECT_TRANCE_CMD      0x1E9 /* fx_1e9 the Trance command when not in Trance */
/* Throw, by the thrown item's icon (fx_109 .. fx_115) */
#define BTLFX_EFFECT_THROW_RACKET    0x109
#define BTLFX_EFFECT_THROW_THIEF_SWORD 0x10A
#define BTLFX_EFFECT_THROW_SWORD     0x10B /* also knight swords */
#define BTLFX_EFFECT_THROW_SPEAR     0x10C
#define BTLFX_EFFECT_THROW_CLAW      0x10D
#define BTLFX_EFFECT_THROW_ROD       0x10E /* rods, flutes, staffs */
#define BTLFX_EFFECT_THROW_FORK      0x10F
#define BTLFX_EFFECT_THROW_DAGGER    0x110
#define BTLFX_EFFECT_THROW_THROWING  0x115 /* pinwheel, rising sun */

/* BtlUnitList.kind. Evidence: [code] */
#define BTL_FXK_NO_CAST 0x01 /* the caster doesn't step out and cast (btlfx caster task), no command camera: BTL_CMD_TRANCE_EIDOLON, BTL_CMD_REBIRTH_FLAME, bounces, btlseq op 0x1A */
#define BTL_FXK_ITEM    0x02 /* BTL_CMD_ITEM / BTL_CMD_AUTO_POTION: the item motion, no command camera */
#define BTL_FXK_SWD_MAG 0x04 /* BTL_CMD_SWD_MAG: the partner (unit 0x11) is the effect's caster */
#define BTL_FXK_PARTY_UNLOAD 0x08 /* set by btlfx_runScript for effects whose caster task kind (BTLFX_HDR_CASTER) is 1: the loader waits until no party member is visible, unloads the party models and loads summon / common texture blocks into their buffers (btlfx_updateLoader) */
#define BTL_FXK_BOUNCE  0x10 /* a Reflect bounce (with NO_CAST); control op 8 doesn't wait for the enemy sequence */

typedef struct Unk800F78A40 {
    s16 phase; /* BTLFX_PHASE_* (btlfx_runScript) */
    s16 effectId; /* effect being played (btlfx_start, after btlfx_resolveFirstUse) */
    u8 pad4[0x8 - 4];
    s16 file; /* effect file slot of the script (btlfx_getFileData) */
    u8 padA[0xF - 0xA];
    u8 seqRelease; /* set by btlseq op 0A; control op 8 waits for it with an enemy caster */
    u8 signals[8]; /* script signals (BTLFX_SIG_*): Unk800FEC40.signals.b[0-3], then motionDone, moveDone, turnDone,
                    * waitDone */
    u8 pad18[0x1C - 0x18];
    u32 flags; /* BTLFX_HDR_* (as Unk800FEC40.flags) */
    u8 retry; /* btlfx_retryOp: run the same op again next frame */
    u8 ending; /* Unk800FEC40.ending */
    u8 partyReload; /* Unk800FEC40.partyReload: control op 11, btl_reloadParty running */
    u8 seqSync; /* Unk800FEC40.seqSync */
} Unk800F78A40;

/* Unk800F78A40.phase / Unk800FEC40.phase (g_btlfxState): btlfx_runScript state */
#define BTLFX_PHASE_IDLE (-1) /* no effect playing */
#define BTLFX_PHASE_SAVE_VRAM 0 /* btlfx_start: save the VRAM strip at (0x300, 0x140) */
#define BTLFX_PHASE_START     1 /* read the effect's header word (BTLFX_HDR_*), start the file load */
#define BTLFX_PHASE_RESIDENT  2 /* BTLFX_HDR_RESIDENT: the script is in the loaded module (btlfx_findScript) */
#define BTLFX_PHASE_LOADING   3 /* wait for the effect file's module (g_btlfxLoader.stage 3) */
#define BTLFX_PHASE_RUN       4 /* run the script ops */

typedef struct {
    u8 pad0[0xE];
    s16 stage; /* Unk80100DC0.stage */
    u8 pad10[0x30 - 0x10];
    s32 *data[8]; /* offset tables, entries relative to their start */
    s16 summonState;
    u8 summonDataState;
    u8 commonTexLoaded;
    u8 pad54[0x68 - 0x54];
    u8 bufState;
    u8 commonTexCount;
} Unk800F78A44;

typedef struct {
    Unk800F78A40 *state; /* &g_btlfxState */
    Unk800F78A44 *loader; /* &g_btlfxLoader */
    u8 pad8[0xC - 8]; /* &g_btlfxTasks */
    BtlUnitList *targets; /* effect target list (btlfx_setTargets) */
    s32 exports; /* g_btlfxModuleExports, the table modules call through (btlfx_mod.h) */
    MATRIX *viewMatrix; /* camera view matrix */
    DISPENV *dispEnv;
    DRAWENV *drawEnv;
    s32 *ot; /* effect ordering table */
    u8 *prim; /* next free primitive */
    s32 otZ; /* z subtracted before indexing the effect OT (>> 2); reset to 0 before each task, modules raise it to draw in front */
    struct Unk800FE3B8 *camera; /* g_btlCamera (btlcam_init) */
    s32 workBuf; /* battle load buffer (Battle11C0.loadBuf; btlfx_start) */
} Unk800F78A4;
extern Unk800F78A4 *g_btlfxEnv;
/* u16 table inside .text (0x800CFA80): 0x1000 * sqrt(64 / i) for i = 0x40-0xFF, the trail ribbons' normalization */
extern s16 g_btlfxRsqrtTable[];

typedef struct {
    s32 cmdRow; /* command menu cursor */
    s8 listCursor[4]; /* per command row: list cursor */
    s8 listTop[4]; /* per command row: list top row */
} Unk800EB518;

/* animated hp/mp readout */
typedef struct {
    s16 step; /* per frame */
    s16 shown; /* shown */
    s16 target; /* target */
    u8 frames; /* frames left */
    u8 pad7;
} Unk80104F88Gauge;

typedef struct Unk80104F88Entry {
    Unk80104F88Gauge gauges[2]; /* hp, mp readouts (btlui_updateGauges) */
    struct BtlUnit *unit;
    Character *chr;
    u8 slot; /* unit slot id (flags bits 5-7) */
    u8 learns; /* the character learns abilities (chr_canLearnAbilities); 0: btlui_addLearnedAbilities skips it */
    u8 defaultSet; /* chr_getAppearanceType of the character */
    u8 port; /* controller port in the two-controller option */
    u8 *apCounters[ABILITY_SUPPORT_FIRST]; /* -> Character.unk58[], AP gathered per ability id */
    u8 apNeeded[ABILITY_SUPPORT_FIRST];
    u8 fromEquip[ABILITY_SUPPORT_FIRST]; /* abilities available from equipment */
    Unk800EB518 cursor; /* remembered command and list cursors (btlui_saveCursor) */
} Unk80104F88Entry; /* 0x4A8, one per party member */

typedef struct {
    Unk80104F88Entry entries[4];
    struct BtlUnit *cmdUnit; /* unit whose command menu is up (runtime) */
    Window *windows[4]; /* 0 target names, 1 battle list, 2 ability/item list, 3 commands */
    u8 menuState; /* BTLUI_MENU_* */
    u8 partyCount;
    u8 frozenSide; /* side whose ATB stays still (btlui_getFrozenSide) */
    u8 swdMagPartner; /* character 1 is in the party (btlui_checkSwdMagPartner) */
    union {
        u8 b[2]; /* set separately by btlui_checkSwdMagPartner */
        u16 h;   /* tested together */
    } swdMagBlocked;
    u8 pad12BA[0x12BC - 0x12BA];
    s32 ambush; /* scene.unk0 & 3 at battle start */
    u32 tick; /* btlui_updateAtbFreeze calls */
    u8 queueCount; /* units waiting for their command menu */
    u8 queueHead;
    u8 unk12C6; /* write-only (cleared with the queue) */
    u8 pad12C7;
    struct BtlUnit *queue[4];
} Unk80104F88;
extern Unk80104F88 *g_btlUi;

typedef struct {
    s16 itemCount; /* count of items */
    u16 throwCount; /* count of throwItems */
    UnkPair *items[31]; /* inventory slots with item ids >= 0xE0 (btlui_buildItemList) */
    UnkPair *throwItems[WEAPON_COUNT]; /* inventory slots of throwable weapons (WEAPON_THROWABLE, btlui_buildThrowList) */
    u8 reserved[0x100]; /* per item id: count reserved by queued commands (btlui_reserveItem) */
} Unk80104FA8;
extern Unk80104FA8 *g_btluiItems;

/* battle menu / frame state */
typedef struct {
    u8 enabled; /* UI running (btlui_applyEnable, btlui_enable(0) clears it) */
    u8 inVsync; /* inside btlui_vsync */
    u8 atbOn; /* ATB and menus run (btlui_setAtbEnabled, btl_isAtbRunning) */
    s8 targetWindow; /* target select opens the target-name window (btlui_updateTargetFlags; read as g_btluiTargetWindow by btlui_startTargetSelect) */
    u8 multiTarget; /* two or more units are targeted (btlui_updateTargetFlags) */
    u8 unk5; /* unused */
    u8 started; /* btlui_init ran */
    u8 pad7;
    u8 hideWindows; /* Select (pad bit 19) held: btlui_drawWindows skips the windows */
    u8 firstValueSet; /* set by the first btlui_latchFirstValue call (no callers), which also stores firstValue */
    u8 padA;
    s8 pauseReq; /* pause requested (Start, btlui_readPad; btlui_isPauseRequested) */
    u8 skipFrames; /* frames the vsync callback skips drawing (btlui_resumeFromPause sets 2) */
    u8 prevAtbOn; /* atbOn at the start of the frame (btlui_frameUpdate) */
    u8 secondPort; /* a party member is on controller port 2 (btlui_anyOnSecondPort, stored through g_btluiSecondPort) */
    u8 frameReady; /* set by btlui_waitFrame: the next vsync runs btlui_frameUpdate */
    u8 drawing; /* inside DrawOTag/DrawOTagEnv (btlui_drawOTag): the vsync draw doesn't break in */
    u8 pad11;
    u8 enableReq; /* btlui_enable(1): btlui_applyEnable enables the UI next frame */
    u8 escapeFrames; /* frames L1+R1 held (btlui_pollEscape: btl_tryEscape(1) every 0x3E) */
    s8 firstValue; /* argument of the first btlui_latchFirstValue call; nothing reads it */
    u8 port; /* controller port of the last command menu (btlui_onCmdMenuOpened, pad_updateRepeat) */
    u8 pad16[0x18 - 0x16];
    s32 vsyncTick; /* btlui_vsync calls (blinks the port icon, btlui_drawPortIcons) */
    s32 vsyncCount; /* vsyncs since btlui_resetVsyncCount (btlui_waitFrame) */
    s32 unk20; /* unused */
    s32 breakMisses; /* BreakDraw found nothing to break (btlui_frame) */
} Unk80104E68;
extern volatile Unk80104E68 g_btluiState;

typedef struct {
    s16 phase; /* BTLFX_PHASE_* (btlfx_runScript) */
    s16 effectId; /* effect being played (btlfx_start, after btlfx_resolveFirstUse) */
    s16 wait; /* frames left of control op 1 (a = 0) */
    s16 fileId; /* effect file id of control op 5 (file = its slot) */
    s16 file; /* effect file slot of the script (btlfx_getFileData) */
    union {
        struct {
            u8 ids[3]; /* camera choices added by btlfx_opAddCameraChoice */
            u8 count; /* count of ids */
        } b;
        u16 h[2]; /* cleared as two halfwords (btlfx_opAddCameraChoice) */
    } cameras; /* camera choices of the effect (btlfx_opAddCameraChoice) */
    u8 cameraPick; /* camera choice picked (btlfx_opPlayCamera) */
    u8 seqRelease; /* set by btlseq op 0A; control op 8 waits for it with an enemy caster */
    union {
        s32 w; /* cleared as a word (only signals 0-3) */
        u8 b[4]; /* b[2]: done */
    } signals; /* control op 2 sets signal a, op 1 waits for signal b (b[2]: the caster task finished). Signals 4-7
                * (BTLFX_SIG_*) are the four done flags below: op 1 indexes b[] up to 7 (runtime: `01 01 07`
                * polls waitDone); the modules' view (Unk800F78A40.signals) is one u8[8] */
    s8 motionDone; /* signal 4: the motion op waited for has ended (btlfx_stepPlayMotionWait) */
    s8 moveDone; /* signal 5: the move op has arrived (btlfx_opRunHome, btlfx_opJumpForward, ...) */
    s8 turnDone; /* signal 6: the turn op has finished (btlfx_opFaceTarget, btlfx_opFaceHome) */
    s8 waitDone; /* signal 7: unit ops 0x79-0x7B (side/timing wait); modules set it too (fxm_setSignal) */
    u8 *pc; /* script pointer, 3-byte commands */
    s32 flags; /* BTLFX_HDR_* */
    u8 retry; /* btlfx_retryOp: run the same op again next frame */
    u8 ending; /* end op: cleanup ran, waiting for btlfx_reloadCommonData */
    u8 partyReload; /* control op 11: btl_reloadParty running */
    u8 seqSync; /* control op 8 ran with an enemy caster (btlseq waits for it) */
    u8 modelSwap; /* control op 13: unit slot + 1 whose model btl_swapPartyModel swaps */
    u8 castEnded; /* the cast op (btlfx_opCast) finished its caster motion; the loader holds common texture blocks (kind 9) until then */
} Unk800FEC40;

/* Script signals (Unk800FEC40.signals.b[n] / Unk800F78A40.signals[n]): control op 2 sets one, op 1 waits for one.
 * 0-3 are free for the script (2: the caster task finished); 4-7 are set by the unit ops and polled with op 1. */
#define BTLFX_SIG_MOTION 4 /* Unk800FEC40.motionDone */
#define BTLFX_SIG_MOVE   5 /* Unk800FEC40.moveDone */
#define BTLFX_SIG_TURN   6 /* Unk800FEC40.turnDone */
#define BTLFX_SIG_WAIT   7 /* Unk800FEC40.waitDone; runtime: Mighty Guard's module sets it, the script's `01 01 07` waits */

/* Effect script ops (3 bytes: op, a, b; btlfx_runScript phase BTLFX_PHASE_RUN). Below BTLFX_OP_TASK_FIRST they are
 * control ops run inline; 0x20-0x7F spawn task kind op - 0x20, 0x80 and up spawn the module's op handler op.
 * Evidence: [code] btlfx_runScript. */
#define BTLFX_CTL_END             0  /* stays on the op until no task runs, then ends the effect (reloading common VRAM first) */
#define BTLFX_CTL_WAIT            1  /* a = 0: wait b frames; else wait for signal b (BTLFX_SIG_*) and clear it */
#define BTLFX_CTL_SIGNAL          2  /* signals[a] = b */
#define BTLFX_CTL_LOAD_NEXT       3  /* btlfx_loadNextBlock */
#define BTLFX_CTL_WAIT_LOADER     4  /* wait until the loader stage reaches a */
#define BTLFX_CTL_SET_FILE        5  /* fileId = a, file = its slot */
#define BTLFX_CTL_WAIT_LOAD       6  /* wait for the load picked by a (BTLFX_WAITLD_*) */
#define BTLFX_CTL_VRAM            7  /* a: BTLFX_VRAM_* */
#define BTLFX_CTL_SEQ_SYNC        8  /* enemy caster: sync with the enemy sequence (waits for seqRelease unless a bounce) */
#define BTLFX_CTL_WAIT_SOUND_BANK 9  /* btlfx_waitSoundBank(a, b) */
#define BTLFX_CTL_LOAD_SOUND_BANK 10 /* btlfx_loadSoundBank */
#define BTLFX_CTL_RELOAD_PARTY    11 /* a = 0: start btl_reloadParty; else wait for it */
#define BTLFX_CTL_WAIT_BUF        12 /* wait for the loader's load buffer (bufState 2) */
#define BTLFX_CTL_SWAP_MODEL      13 /* a = 0: btl_swapPartyModel on unit b's slot; else wait for it */
#define BTLFX_CTL_RETURN          14 /* return from btlfx_callScript */
#define BTLFX_CTL_VIBRATE         15 /* b = 0: start the vibration data at file offset a; else stop */
#define BTLFX_CTL_NOP             20
#define BTLFX_OP_TASK_FIRST       0x20 /* ops from here spawn a task */
#define BTLFX_OP_UNIT_FIRST       0x50 /* g_btlfxUnitOps covers 0x50-0x7F, g_btlfxResidentOps 0x20-0x2F */
#define BTLFX_OP_MODULE_FIRST     0x80 /* ops from here are the module's op handlers */
/* resident task ops: g_btlfxResidentOps index (script op BTLFX_OP_TASK_FIRST + n, btlfx_spawnTask), named after the handler */
#define BTLFX_RES_SET_ARGS_A 0
#define BTLFX_RES_DRAW_ON_UNIT 1
#define BTLFX_RES_SET_ARGS_C 2
#define BTLFX_RES_ADD_CAMERA_CHOICE 3
#define BTLFX_RES_FADE_UNITS 4
#define BTLFX_RES_APPLY_RESULT 5
#define BTLFX_RES_SET_ARGS_B 6
#define BTLFX_RES_CAST 7
#define BTLFX_RES_RETURN_HOME 8
#define BTLFX_RES_PLAY_CAMERA 9
#define BTLFX_RES_FADE_BACKGROUND 10
#define BTLFX_RES_BOUNCE 11
#define BTLFX_RES_SET_FRAME_RATE 12
#define BTLFX_RES_PLAY_SOUND 13
#define BTLFX_RES_SCREEN_FADE 14
#define BTLFX_RES_SET_SPRITE_CLUT 15
/* BTLFX_CTL_WAIT_LOAD's a */
#define BTLFX_WAITLD_SUMMON       0 /* loader summonState 2 */
#define BTLFX_WAITLD_SUMMON_DATA  1 /* loader summonDataState 2 */
#define BTLFX_WAITLD_COMMON_TEX   2 /* loader commonTexLoaded */
/* BTLFX_CTL_VRAM's a */
#define BTLFX_VRAM_SWAP_BACK       0 /* btlfx_swapVramPages(1) with the header's BTLFX_HDR_VRAM_SWAP mode */
#define BTLFX_VRAM_SWAP_FILE       1 /* btlfx_swapVramPages(2) from file data b */
#define BTLFX_VRAM_WAIT_COMMON_TEX 2 /* wait until b common texture blocks are loaded */

/* Unk800FEC40.flags (g_btlfxState): the effect's header word (RAM_BTLFX_MODULE u16 table, btlfx_runScript phase 1)
 * without bit 0. Evidence: [code] */
#define BTLFX_HDR_COMMON_VRAM  0x0001 /* runtime: a block overwrote the common textures (btlfx_markCommonVram); the end op reloads them */
#define BTLFX_HDR_PARTNER      0x0004 /* the camera also takes the partner's anchors (btlcam_beginEffect) */
#define BTLFX_HDR_CAMERA       0x0008 /* the effect camera runs whatever the camera option (btlfx_updateCameraEnable); the cast op hides the target markers */
#define BTLFX_HDR_CASTER       0x00F0 /* caster task kind (btlfx_startCasterTask) */
#define BTLFX_HDR_CASTER_SHIFT 4
#define BTLFX_HDR_BG_FADE      0x0300 /* background fade of the cast op (btlfx_startFadeBackground), 3 = none */
#define BTLFX_HDR_BG_FADE_SHIFT 8
#define BTLFX_HDR_VRAM_SWAP    0x0C00 /* btlfx_swapVramPages mode before and after the effect, 0 = none */
#define BTLFX_HDR_VRAM_SWAP_SHIFT 10
#define BTLFX_HDR_CAST_MODE    0x3000 /* cast op castMode */
#define BTLFX_HDR_CAST_MODE_SHIFT 12
#define BTLFX_HDR_RESIDENT     0x8000 /* the script is in the resident module: no file to load (BTLFX_PHASE_RESIDENT).
                                       * Runtime (header table dumped at btlfx_loadFile): 118 ids, all with header 0x83FF
                                       * and no dir-13 file, the weapon and physical-hit scripts: party Attacks
                                       * 0x64-0x76, enemy/Marcus Attack 0x77, Steal 0xC8 / 0x111 / 0x13 / 0x14, Jump
                                       * 0x189, enemy melee 0x139-0x178 and 0x1CD-0x1E5 (Tail, Claws, Fang, ...),
                                       * 0x10 / 0x12 / 0x25 / 0x27 */
extern Unk800FEC40 g_btlfxState;

typedef struct {
    s32 sector; /* CD sector of the next read */
    s32 effectId; /* effect file being loaded (btlfx_loadFile) */
    s32 slot; /* buffer slot 0/1 of the current block */
    s16 state; /* BTLFX_LD_* (btlfx_updateLoader) */
    s16 stage; /* BTLFX_STAGE_* (control op 4 waits for it) */
    s16 blockType; /* block header word 0: 1 loads into the second module buffer in slot 1 */
    s16 cmdsLeft; /* commands left in the current block */
    u8 pad14[0x16 - 0x14];
    u8 cmdKind; /* BTLFX_LDCMD_* */
    u8 cmdArg; /* command argument (kind 2: ring size follows; kind 9: texture page mask) */
    u16 blockCount; /* block count */
    u16 block; /* block being loaded */
    u16 *cmd; /* command pointer */
    u8 *ringBase; /* data ring at 0x801F1700 (kind 2 with an argument) */
    s32 ringSize;
    u8 *ringCursor;
    union {
        s16 h[2];
        s32 w; /* set to -1 as a word */
    } slotBlock; /* per slot: block loaded there, -1 = none (btlfx_findFileSlot) */
    u8 *data[2]; /* per slot: file data base (btlfx_getFileData) */
    s32 modArg; /* data handed to the module's init (file data 0x40) */
    struct Heap *heaps[2]; /* per slot: the file's heap (btlfx_allocTaskMem) */
    RECT *texRects;
    s16 texIndex;
    u8 pad4A[0x4C - 0x4A];
    u32 *texData;
    s16 summonState; /* BTLFX_BLOCK_*: kind 4 reading, kind 5 done (summon model registered) */
    u8 summonDataState; /* BTLFX_BLOCK_* of kind 6 */
    u8 commonTexLoaded; /* kind 9 loaded (control op 6 a=2 waits for it) */
    struct Unk80100DC054 *summon; /* summon file in Battle.partyBufs[0] */
    s32 workCursor; /* summon work area (btlfx_takeWork) */
    s32 workLeft;
    s32 summonModel; /* Hi_RegisterSummonModel result */
    u8 soundState; /* BTLFX_BLOCK_* of kind 7 */
    u8 soundBlocks; /* kind 7 blocks read */
    u8 soundUploading; /* sound bank upload running (btlfx_waitSoundBank) */
    u8 soundUploaded; /* btlfx_loadSoundBank ran: the loader may continue */
    u8 bufState; /* BTLFX_BLOCK_* of kind 8 (control op 12 waits for it) */
    u8 commonTexCount; /* kind 9 blocks loaded (control op 7 a=2 waits for b) */
} Unk80100DC0;
extern Unk80100DC0 g_btlfxLoader;

/* Unk80100DC0.stage, set as the commands start (btlfx_updateLoader). Evidence: [code] */
#define BTLFX_STAGE_NONE  0
#define BTLFX_STAGE_DATA  1 /* a BTLFX_LDCMD_DATA block is being read */
#define BTLFX_STAGE_CODE  2 /* the BTLFX_LDCMD_CODE module is being read */
#define BTLFX_STAGE_READY 3 /* the module is in and its init ran: BTLFX_PHASE_LOADING ends */
/* Unk80100DC0.summonState / summonDataState / soundState / bufState */
#define BTLFX_BLOCK_NONE    0
#define BTLFX_BLOCK_READING 1 /* its command started */
#define BTLFX_BLOCK_DONE    2 /* its command finished (summonState: the summon model is registered) */

/* Unk80100DC0.state (btlfx_updateLoader switches on state - 1). Evidence: [code] */
#define BTLFX_LD_IDLE        0
#define BTLFX_LD_READ_HEADER 1 /* read the file's first sector (btlfx_loadFile) */
#define BTLFX_LD_WAIT_HEADER 2
#define BTLFX_LD_NEXT_BLOCK  3 /* start the next block, or stop after blockCount (btlfx_loadNextBlock) */
#define BTLFX_LD_NEXT_CMD    4 /* start reading the next command's data */
#define BTLFX_LD_WAIT_CMD    5 /* wait for the read, then process it by cmdKind */
#define BTLFX_LD_WAIT_SPU    6 /* sound bank: wait for the SPU transfer */
#define BTLFX_LD_WAIT_SOUND  7 /* sound bank: wait for btlfx_loadSoundBank (soundUploaded) */

/* Unk80100DC0.cmdKind: block command kinds (btlfx_updateLoader). Evidence: [code] */
#define BTLFX_LDCMD_TEX          0 /* textures with their header (images, then the texture rects) */
#define BTLFX_LDCMD_TEX_MORE     1 /* more of the texture data */
#define BTLFX_LDCMD_DATA         2 /* file data into the data ring (its heap: file data 0) */
#define BTLFX_LDCMD_CODE         3 /* effect module code: its init runs, stage 3 */
#define BTLFX_LDCMD_SUMMON_TEX   4 /* summon file into the party buffers (party models unloaded), its textures uploaded */
#define BTLFX_LDCMD_SUMMON_MODEL 5 /* summon model data, registered (Hi_RegisterSummonModel) */
#define BTLFX_LDCMD_SUMMON_DATA  6 /* summon work data */
#define BTLFX_LDCMD_SOUND        7 /* sound bank */
#define BTLFX_LDCMD_LOADBUF      8 /* data into the battle load buffer */
#define BTLFX_LDCMD_COMMON_TEX   9 /* common texture pages (cmdArg: page mask) */
#define BTLFX_LDCMD_RING         10 /* more data appended in the data ring */


typedef struct {
    s32 cmdNo;
    struct Unk80104FD4 *cmd; /* command table entry (g_btlCmdTable) of the list */
    s32 label; /* list window title label */
    s32 label2; /* second title label (right-aligned) */
    s16 doubleCast; /* Dbl Wht / Dbl Blk picks made (btlui_nextDoubleCast) */
    s16 label2Width;
    struct BtlUnit *unit;
    u8 halfMp; /* support ability 0xE2 (btl_getMpCost halves the cost) */
    u8 trance; /* the list was opened in trance */
} Unk80104FD0;
extern Unk80104FD0 g_btluiList;

extern s32 g_btlbgScrollPaused;
extern s32 g_btlbgRainCount;
extern MATRIX g_btlfxBgTopView;
void btlbg_setLayerMask(s32);
extern s32 g_btlbgId;
extern s32 g_btlbgSkySpeed;
extern struct BtlBgHeader *g_btlbgHeader;
extern s32 g_btlbgLayerMask; /* BTLBG_SHOW_* */
/* BtlBgRec.layer: where btlbg_drawRecord sorts the record, and g_btlbgLayerMask: which ones btlbg_draw draws.
 * Evidence: [code] btlbg_draw, btlbg_drawRecord, btlfx_drawBgOffscreen (draws only BTLBG_SHOW_FRONT) */
#define BTLBG_LAYER_SCENE 0 /* in the scene OT, depth sorted with the units */
#define BTLBG_LAYER_FRONT 2 /* background OT, front (index 0) */
#define BTLBG_LAYER_MID   4 /* background OT, depth sorted */
#define BTLBG_LAYER_SKY   8 /* background OT, back (0x3FC); turns with g_btlbgSkyAngle */
#define BTLBG_SHOW_SCENE  0x1
#define BTLBG_SHOW_FRONT  0x2
#define BTLBG_SHOW_MID    0x4
#define BTLBG_SHOW_SKY    0x8
#define BTLBG_SHOW_ALL    (BTLBG_SHOW_SCENE | BTLBG_SHOW_FRONT | BTLBG_SHOW_MID | BTLBG_SHOW_SKY)
extern s32 g_btlbgSkyAngle;
extern struct BtlUnit9C50 *g_btlbgTexAnims;
extern s32 g_btlbgSkyOwnProj;
/* battle background: texture scroll entry (g_btlbgScrolls) */
typedef struct {
    s16 uvOfs; /* the uv pair it moves, offset from the first group (Battle11C0.bgData) */
    u8 mode;   /* BTLBG_SCROLL_* */
    u8 du;
    u8 dv;
    u8 step;   /* frames since the last wrap/bounce (written at run time) */
    u8 period; /* frames per cycle */
    u8 pad7;
} BtlBgScroll;
/* BtlBgScroll.mode: what happens after period frames (btlbg_draw). Evidence: [code] */
#define BTLBG_SCROLL_WRAP   0 /* step back period x (du, dv) */
#define BTLBG_SCROLL_BOUNCE 1 /* negate du, dv */

/* battle background: group header (0x18 bytes), followed by its records; offsets are from the group, groups
 * follow each other (docs/battle-backgrounds.md). Members marked "data" are not read by the code. */
typedef struct {
    u16 size; /* bytes to the next group */
    u16 cornerCount; /* data: 4 x quads + 3 x triangles of all records (index and uv entries) */
    u16 recCount;
    u16 recOfs; /* BtlBgRec[recCount] */
    u16 texCount; /* data: entries of the texture word table */
    u16 texOfs; /* texture word table (clut << 16 | tpage, indexed by color bits 24-28) */
    u16 vertexCount; /* data: sum of the records' vertexCount */
    u16 vertexOfs; /* the packed 6-byte vertices (s16 x, y, z) */
    u16 polyCount; /* data: quads + triangles of all records (color words) */
    u16 colorOfs; /* data: first record's colors */
    u16 indexOfs; /* data: first record's vertex indices */
    u16 uvOfs; /* data: first record's uvs */
} BtlBgGroup;

/* battle background: record (draw batch) of a group, 0x10 bytes; offsets are from the record itself
 * (docs/battle-backgrounds.md) */
typedef struct {
    u16 layer; /* BTLBG_LAYER_*: 8 sky (turns with g_btlbgSkyAngle), 4, 2, 0 (btlbg_draw, g_btlbgLayerMask BTLBG_SHOW_*);
                * btlbg_drawRecord: 0 = scene OT, index from the depth sum << g_btlbgOtShift; else the
                * background OT: bit 3 = back (0x3FC), bit 1 = front (0), otherwise sum >> 6 (quads) /
                * >> 4 (triangles) << 2 */
    u16 vertexCount; /* vertices projected (a non-zero multiple of 6, at most 126: the scratchpad holds 128) */
    u16 firstVertex; /* index of the first one in the group's vertex array */
    u16 colorOfs; /* the colors (a word per primitive: bgr, texture index << 24, bit 29 = semi-transparent,
                   * bits 30-31 = blend mode) */
    u16 indexOfs; /* the vertex indices (scratchpad byte offsets, i.e. 4 x index; 4 per quad then 3 per triangle) */
    u16 uvOfs; /* the uvs (u8 u, v per corner, in the same order) */
    u16 quadCount;
    u16 triCount;
} BtlBgRec;

extern BtlBgScroll *g_btlbgScrolls;
extern s32 g_btlbgFrame; /* frame counter */
extern s32 g_btlbgJitterBase1;
extern s32 g_btlbgJitterBase2;
u8 *btlbg_drawRain(u8 *, u32 *, MATRIX *);
u8 *btlbg_drawRecord(BtlBgRec *, u8 *, u32 *, BtlBgGroup *);
void btlbg_projectVertices(BtlBgRec *, u8 *, u32 *, s32);
extern s32 g_btlbgTexAnimCount;
extern s32 g_btlbgScrollCount;
/* Kind-0 db items of FILE_BTL_COMMON (btl_loadCommonData db_register's it; its kind-0 block holds exactly these 5
 * keys, and its DB_MUSIC block MUSIC_VICTORY). Evidence: [code] the lookups, [data] the file's db header. */
#define BTL_COMMON_NAME_GLYPHS 0x14  /* text_uploadPartyNames work (glyph sheet, name image at +0x4000; btlui_uploadPartyNames) */
#define BTL_COMMON_PORTRAITS   0x1E  /* party portraits, 0x6C0 bytes each (btlui_loadPortraits); g_modeHeap starts here afterwards */
#define BTL_COMMON_STATUS_INFO 0x1F4 /* copied to Battle.statusInfo (0x180 bytes) */
#define BTL_COMMON_ABILITIES   0x1F5 /* copied to Battle.abilities (0xC00 bytes) */
#define BTL_COMMON_STATUS_SETS 0x1F6 /* copied to Battle.statusSets (0x100 bytes) */
/* Battle.cmdSlots per unit (btl_initUnitCmds: party slots first, then the enemies'). Evidence: [code] */
#define BTL_PARTY_CMD_SLOTS 6
#define BTL_ENEMY_CMD_SLOTS 3
#define BTL_SOUND_SE_BANK 0x201 /* the battle's SE instrument bank (instruments 0xC0-0xDF, which akao_remapSeInstrument
                                 * maps; the field SE banks load at the same SPU address): DIR_SOUND_BANKS file btl_loadSoundsStep
                                 * always reads first and loads with akao_loadBankRetry (no slot), before the slot banks.
                                 * Evidence: [code]; [runtime] R41b (one load per battle; instrument 192 keyed by Zidane's attack SE) */
/* VRAM area of the common effect textures: FILE_BTLFX_COMMON's four 64x128 pages (btlfx_reloadCommonData); an upload
 * inside it sets BTLFX_HDR_COMMON_VRAM (btlfx_markCommonVram). Evidence: [code] */
#define VRAM_BTLFX_COMMON_X 0x300
#define VRAM_BTLFX_COMMON_Y 0x100
#define VRAM_BTLFX_COMMON_W 0x80
#define VRAM_BTLFX_COMMON_H 0x100
extern s32 *g_btlfxCommonTable; /* offset table, entries relative to its start */
/* btlfx_getCommonData entries (g_btlfxCommonData, 32 entries). Evidence: [code] the callers below, and the static
 * table in ovl_0b's data: 0-1, 3-5 are emitter definitions (0x13C / 0x17C bytes), 2, 6, 7, 8, 0x14 particle
 * textures, 9-0x12 battle cameras (g_btlfxCmdCameras, g_btlfxBounceCamera*), 0x13 and 0x15-0x1F AKAO sounds
 * (g_btlfxSoundSets). */
#define BTLFX_COMMON_EMIT_SUMMON_CAST 0x01 /* btlfx_startCastEmitter, BTLFX_CASTK_SUMMON / _SUMMON_SHORT */
#define BTLFX_COMMON_EMIT_BOUNCE      0x03 /* one per reflected target (btlfx_opBounce) */
#define BTLFX_COMMON_EMIT_ENEMY_CAST  0x04 /* btlfx_startCastEmitter, BTLFX_CASTK_ENEMY */
#define BTLFX_COMMON_EMIT_CAST        0x05 /* btlfx_startCastEmitter, BTLFX_CASTK_WHITE / _BLACK */
#define BTLFX_COMMON_TEX_SUMMON_C     0x02 /* summon cast emitter's particle 2 */
#define BTLFX_COMMON_TEX_CAST_B       0x06 /* particle 1 of every cast emitter and of the bounce emitter */
#define BTLFX_COMMON_TEX_CAST_A       0x07 /* particle 0 of the WHITE, ENEMY and bounce emitters */
#define BTLFX_COMMON_TEX_BLACK_CAST   0x08 /* particle 0 of the BLACK cast emitter */
#define BTLFX_COMMON_TEX_SUMMON_A     0x14 /* summon cast emitter's particle 0 */
/* g_btlfxSoundSets rows (btlfx_playSoundSet): 3 common sounds each. Evidence: [code] */
#define BTLFX_SNDSET_CAST        0 /* enemy cast emitter, and WHITE / BLACK cast emitters of every other caster kind */
#define BTLFX_SNDSET_CAST_EIKO   1 /* WHITE / BLACK cast emitter when the caster's kind is g_btlfxUnitKinds[0]: CHR_KIND_EIKO
                                  * only, not CHR_KIND_EIKO_RACKET (that kind gets set 0). [runtime] R41b */
#define BTLFX_SNDSET_SUMMON_CAST 2 /* summon cast emitter */
#define BTLFX_SNDSET_BOUNCE      3 /* each reflected target (btlfx_opBounce) */
/* Cast kind: the effect header's BTLFX_HDR_CASTER field, passed by btlfx_startCasterTask as the cast op's argument
 * high nibble (Unk800E4C88.kind) and used as btlfx_startCastEmitter's kind. Evidence: [code] btlfx_runScript,
 * btlfx_opCast, btlfx_startCastEmitter, btlfx_playCmdCamera; [data] the effect header table (FILE_BTLFX_COMMON
 * + 0x2000, the VRAM copy at (0x300, 0x140) that btlfx_runScript reads; matches the R31 runtime dump), per kind with
 * the effects of docs/effect-modules.md. */
#define BTLFX_CASTK_WHITE    0   /* 104 effects: Cure, Esuna, Life, Mini, Float, Regen, Potion, ... */
#define BTLFX_CASTK_SUMMON   1   /* 24: the full summons (Shiva, Ifrit, Ramuh, Bahamut, Ark, ...): BTL_FXK_PARTY_UNLOAD,
                                  * summon camera (Dagger's or the other), summon cast emitter, castMode screen tint */
#define BTLFX_CASTK_BLACK    2   /* 54: Fire, Thunder, Blizzard, Sleep, Comet, Osmose, ... */
#define BTLFX_CASTK_ENEMY    4   /* 103: enemy abilities and Blue Magic (White Wind, Aerial Slash, Snort, ...) */
#define BTLFX_CASTK_SUMMON_SHORT 7 /* 15: the short summons (Shiva (short), Carbuncle (short), ...): summon camera and
                                    * emitter without the party unload */
#define BTLFX_CASTK_NONE     0xF /* 212: no cast (Elixir, Luna, Armor Break, the resident 0x83FF scripts, ...) */
/* g_btlfxCastBgBrightness index (BTLFX_HDR_BG_FADE field) */
#define BTLFX_BG_FADE_NONE 3 /* btlfx_opCast doesn't fade the background */
typedef struct {
    s16 count; /* count of pos */
    s16 partyCount; /* count of party */
    struct BtlUnit *party[4]; /* party units among them */
    s16 pos[8][2]; /* cursor positions of the targetable units on screen (btlui_collectCursorPositions) */
} Unk80105020;
extern Unk80105020 g_btluiCursors;
typedef struct {
    u8 open; /* battle list window open (btlui_openListWindow) */
    u8 kind; /* g_btluiListWindowDrawFns content handler */
} Unk80105058;
extern Unk80105058 g_btluiListWindow;
extern s32 g_btlBannerOt[];

typedef struct {
    u16 nameOfs; /* name text offset (btl_setupEnemies) */
    u8 category; /* btl_hasEnemyFlag bits (BTL_ENEMY_*) */
    u8 level;
    BtlUnit24 max; /* max stats */
    u8 padA[0xC - 0xA];
    BtlMotion *motions[6]; /* motion table (BtlUnit.motions) */
    u16 radius; /* btlfx_getUnitRadius */
    u8 confuseAbility; /* enemy ability used under Confuse/Berserk (btl_setEnemyCmd) */
    u8 eatAbility; /* Blue Magic learned by eating it (Eat formula 0x41) */
    u8 firstAbility; /* first of its abilities in Battle.enemyAbilities (btl_setupEnemies) */
    u8 bones[3]; /* bones for camera anchors; [2] the head (btl_getUnitBodyPos) */
    s32 camera; /* cast camera data (btl_getSceneData(5 + type), btlfx_playCmdCamera) */
    u16 deathCry; /* sound (btl_playDeathCry) */
    u8 iconBones[6]; /* status icon attach points: bone (btl_drawStatusIcons, as g_btlPartyIconBones for players) */
    u8 iconY[6]; /* y offset (<< 4) */
    u8 iconZ[6]; /* z offset (<< 4) */
    u16 gil; /* gil (g_btlRewards.gil; an escape drops a tenth of the remaining enemies' gil, Flee-Gil gains it: btl_victory). runtime: Malboro 2572 = "Received Gil 2572G" */
    u16 exp; /* exp (g_btlRewards.exp). runtime: Malboro 30579, the rewards screen shows EXP 7644 = 30579 / 4 */
    union {
        s32 w;
        u8 b[4]; /* item drops, rarest last */
    } drops;
    s32 card; /* card */
} BtlEnemyType; /* 0x50 */

typedef struct {
    BtlEnemyType *type;
    u32 flags; /* BTL_ENEMYF_* */
    union {
        s32 w;
        u8 b[4]; /* items to steal, 0xFF = none (btl_calcSteal) */
    } steal;
    u8 home[0x18 - 0xC]; /* home position block (BtlUnit.home) */
} BtlEnemy; /* 0x18 */

/* BtlEnemy.flags (set from the group record, Unk800C9EE0 / Unk800C9D1C, by btl_initEnemyUnit). Evidence: [code] */
#define BTL_ENEMYF_FADE              0x3F /* bits 0-5: fade level for btl_setUnitFade, 0 gone */
#define BTL_ENEMYF_FADE_OPAQUE       0x20 /* start level (btl_initEnemies); the KO fade steps it down by 2 */
#define BTL_ENEMYF_SCRIPT_DEATH_BIT  6
#define BTL_ENEMYF_SCRIPT_DEATH      (1 << BTL_ENEMYF_SCRIPT_DEATH_BIT) /* its death is an event-script trigger (0x36) instead of the normal KO (btl_endEnemyHitMotion) */
#define BTL_ENEMYF_DEATH_MOTION_BIT  7
#define BTL_ENEMYF_DEATH_MOTION      (1 << BTL_ENEMYF_DEATH_MOTION_BIT) /* a killing hit plays motion 3 and the death cry (btl_startHitMotion) */
#define BTL_ENEMYF_SHARED_MODEL_BIT  8
#define BTL_ENEMYF_SHARED_MODEL      (1 << BTL_ENEMYF_SHARED_MODEL_BIT) /* the group's enemies share the first one's model; its owner is the body (btl_findBodyUnit) */
#define BTL_ENEMYF_NO_MODEL_BIT      9
#define BTL_ENEMYF_NO_MODEL          (1 << BTL_ENEMYF_NO_MODEL_BIT) /* no model of its own (btl_getEnemyModelId -1): BTL_UNIT_MODEL_PART */

typedef struct {
    void *data[4]; /* registered battle files: [0] common data, [2] encounter, [3] background */
    void *enemyModels[4]; /* enemy model files (btl_loadEnemyModelsStep) */
    void *weaponFiles[4]; /* party weapon model files by slot (btl_loadWeaponModelsStep) */
    void *partyModels[4]; /* party model files by slot (btl_loadPartyModelsStep) */
} BtlFiles;

typedef struct {
    u8 pad0;
    u8 tickScale; /* Venom/Poison/Regen tick period factor (btl_initDotTimer) */
    u16 timerScale; /* duration factor of the timed status (btl_addStatus, btl_initDoomCount) */
    s32 removes; /* statuses adding this one removes (btl_addStatus) */
    u32 prevents; /* statuses this one keeps from being added (btl_addStatus) */
} BtlStatusInfo; /* 0xC, one per status bit */

typedef struct Battle1538 {
    u8 pad0[0x20];
    s32 base; /* buffer start (btl_initReturnInfo) */
    s32 cursor; /* next free byte; the event-script loader allocates from it (btl_getDepSectors kind 7) */
} Battle1538;

typedef struct {
    s32 flags; /* BtlScript.flags */
    struct BtlCmd *cmd; /* BtlCmd it runs for, NULL = free slot */
    s32 pc; /* script pointer */
    s32 prevPc;
    s16 opFrame;
    s16 waitLeft;
    s16 frame;
    u8 seqFlags;
    u8 soundDelay;
    u8 pad18[0x21 - 0x18];
    u8 turnFrames;
    u8 pad22[0x25 - 0x22];
    u8 fxDelay;
    u8 fadeFrames;
    u8 pad27[0x38 - 0x27];
} Battle122C; /* 0x38 */

typedef struct Battle11C4 {
    BtlMotion **anims; /* animations the sequences play (btlseq_init) */
    u8 *seqFlags; /* one byte per sequence, copied to the slot (btlseq_start) */
    s32 seqs[24]; /* sequence code by enemy ability id, 0 = none (btlseq_init) */
    Battle122C slots[4]; /* the four running sequences (btlseq_update) */
    struct BtlUnit *camCaster; /* unit the sequence camera follows (btlseq_opPlayCamera) */
    s32 camTarget; /* BtlUnit *: first target for the sequence camera */
    u16 camCenterX; /* targets' centre for the sequence camera (btlseq_getTargetCenter) */
    s16 camCenterY;
    u16 camCenterZ;
    u8 pad156[0x158 - 0x156];
} Battle11C4; /* 0x158 */

typedef struct {
    u8 type; /* enemy type (Battle.enemyTypes index) */
    u8 flags; /* 1 targetable (BtlUnit.flags bit 28), 2 shares the first enemy's model (btl_getEnemyModelId: -1; BtlEnemy.flags bit 8) */
    u8 pad2[2];
    s16 x; /* position (btl_initEnemyUnit) */
    s16 y;
    s16 z;
    s16 rotY; /* facing, plus g_btlEnemyRotByAmbush[ambush] */
} Unk800C9EE0;

typedef struct {
    u8 weight; /* chance out of 100 (btl_pickEnemyGroup) */
    u8 enemyCount; /* count of enemies */
    u8 cameraNo; /* Battle.cameraNo; 3 or more: random 0-2 (btl_initScene) */
    u8 pad3;
    u16 ap; /* g_btlRewards.ap (btl_initSceneFlags) */
    u8 pad6[8 - 6];
    Unk800C9EE0 enemies[4]; /* enemy placements */
} Battle1330; /* 0x38 */

/* enemy type record as the scene sees it (Unk800C9D1C is the same record) */
typedef struct {
    u8 pad0[0x1E];
    u16 modelId; /* model (Unk800C9D1C.modelId) */
    u8 pad20[0x62 - 0x20];
    u16 entranceSound; /* btl_getEntranceSound (first enemy of the group) */
    u8 pad64[0x74 - 0x64];
} Battle1334; /* 0x74 */

typedef struct Battle11C020 {
    u8 unk0; /* not read */
    u8 groupCount; /* count of groups */
    u8 typeCount; /* count of enemy type records following the groups */
    u8 abilityCount; /* count of enemy abilities (BtlCmd8) following the enemy records */
    u16 flags; /* Battle.scene.flags bits 2 and up (btl_initSceneFlags): BTL_ENC_* */
    u8 pad6[8 - 6];
    Battle1330 groups[1]; /* enemy groups (btl_pickEnemyGroup) */
} Battle11C020;

/* battle command (queued action) */
typedef struct BtlCmd8 {
    u32 targetType : 4; /* target type; 6-12 make enemy commands multi-target (btl_setEnemyCmd), 3-5 spread casts (btl_halveForSpread) */
    u32 defaultSide : 1; /* target select starts on this side (Battle16CC.defaultSide) */
    u32 listKind : 3; /* nonzero: targets are picked from a list window (Battle16CC.listKind) */
    u32 effect : 9; /* effect id (btl_startCmdEffect -> btl_playCmdEffect) */
    u32 unk0_17 : 13; /* not read in ovl_0b */
    u32 forceCamera : 1; /* the enemy's camera move (btlseq_opPlayCamera) plays even with the fixed-camera option, a Vanish/Mini caster or target, and past the 50% roll.
                          * Set on 227 enemy abilities in 102 encounters, all scripted or story actions (friendly monsters'
                          * "Happy"/"More", Ragtime Mouse quiz, death and appearance sequences, Beatrix, Kuja Transform),
                          * runtime round 33 (static scan of every encounter) */
    u32 unk0_31 : 1; /* not read in ovl_0b */
    u8 formula; /* btl_calcCmd formula */
    u8 power; /* power, or the formula's parameter (an item id, a divisor) */
    u8 element; /* element bits */
    u8 rate; /* hit/status rate */
    u8 flags; /* BTL_ABIL_* */
    u8 statusSet; /* index into Battle.statusSets */
    u8 mpCost; /* runtime: Fire 6, Fira 12, Sleep 10 as the spell list shows */
    u8 flags2; /* BTL_ABIL2_* */
    u16 altEffect; /* effect played for short summons, Seiken and single-target casts (btl_startCmdEffect) */
    u16 nameOfs; /* name text offset (runtime: Fire 161, Fira 166, Firaga 171, Sleep 178 - steps = name lengths + 1) */
} BtlCmd8;

/* BtlCmd8.flags (ability flags). Evidence: [code] */
#define BTL_ABIL_REFLECTABLE    0x01 /* Reflect bounces it (btl_applyReflect) */
#define BTL_ABIL_VOICE          0x02 /* needs a voice: Silence blocks it */
#define BTL_ABIL_PHYSICAL       0x08 /* answered by Counter (btl_tryCounter), Cover steps in (BTL_CMD_ENEMY_TURN), a hit wakes Sleep and Confuse */
#define BTL_ABIL_DISPELS_VANISH 0x10 /* a hit removes Vanish (btl_applyCalcResult) */
#define BTL_ABIL_NO_HIT_MOTION  0x40 /* the target plays no hit motion (btl_canAddHitStatus) */
#define BTL_ABIL_RETURNABLE     0x80 /* answered by Return Magic */

/* BtlCmd8.flags2. Evidence: [code] */
#define BTL_ABIL2_SUMMON 0x04 /* costs 4x MP while EVT_GB_SUMMON_MP_X4 is set (btl_payMpCost, btl_getMpCost) */
#define BTL_ABIL2_HEAVY_HIT 0x81 /* both bits set: party targets play BTL_MOTION_HIT_HEAVY (btl_startHitMotion); the bits alone are not tested */


typedef struct BtlCmd {
    struct BtlCmd *next; /* queue link */
    BtlUnit *caster;
    BtlCmd8 *ability;
    s16 targets; /* unit bitmask: party bits 0-3, enemies from bit 4 */
    u8 cmdNo;    /* menu command: Attack 1, Focus 13, ... */
    u8 subNo;    /* ability id (Attack 176, Focus 188), or item id for item commands */
    union {
        struct {
            u32 targeting : 2; /* btl_setCmd's last argument; 1 = several targets (btl_halveForSpread, btl_setEnemyCmd) */
            u32 unk0_2 : 2; /* write-only: 1 while linked into a queue (btl_linkCmd, BTL_CMDINFO_LINKED); cleared by btl_clearCmd, btl_unlinkCmd */
            u32 automatic : 1; /* set for the ids above BTL_CMD_TABLE_COUNT (btl_setCmd, btl_setEnemyCmd) */
            u32 covered : 1; /* retargeted to a Cover unit (btl_findCover) */
            u32 dodged : 1; /* the target dodged; its step aside is undone at the end (btl_applyCalcResult) */
            u32 reflect : 2; /* 2 = bounce pending, 1 = bounced (btl_playCmdEffect, btl_onCmdEffectDone) */
            u32 magicFailed : 1; /* btl_rollMagicFail: the spell misses (btl_calcCmd formula 0x12) */
            u32 shortSummon : 1; /* plays the ability's altEffect (btl_rollShortSummon) */
            u32 enemyBounce : 1; /* an enemy's reflected spell: btl_processCmdQueue steps it like a party command (btl_updatePartyCmd) */
        } b;
        u32 w; /* cleared as a word; BTL_CMDINFO_* */
    } info;
} BtlCmd;

/* BtlCmd.targets / BtlUnit.unitBit masks. Evidence: [code] (unit bit = 1 << slot) */
#define BTL_TARGETS_PARTY       0x000F /* the four party slots (the escape command targets them) */
#define BTL_TARGETS_FIRST_ENEMY 0x0010 /* unit bit of enemy slot 4: targets >= it hit an enemy */

/* BtlCmd.info.b as a word (info.w; BtlCalc8.flags) */
#define BTL_CMDINFO_TARGETING         0x003 /* info.b.targeting */
#define BTL_CMDINFO_LINK_STATE        0x00C /* info.b.unk0_2 */
#define BTL_CMDINFO_LINKED            0x004 /* unk0_2 = 1: btl_linkCmd put it in a queue */
#define BTL_CMDINFO_AUTOMATIC         0x010 /* info.b.automatic */
#define BTL_CMDINFO_COVERED_BIT       5
#define BTL_CMDINFO_COVERED           (1 << BTL_CMDINFO_COVERED_BIT) /* info.b.covered */
#define BTL_CMDINFO_DODGED            0x040 /* info.b.dodged */
#define BTL_CMDINFO_REFLECT_SHIFT     7
#define BTL_CMDINFO_REFLECT           (3 << BTL_CMDINFO_REFLECT_SHIFT) /* info.b.reflect */
#define BTL_CMDINFO_MAGIC_FAILED      0x200 /* info.b.magicFailed */
#define BTL_CMDINFO_SHORT_SUMMON_BIT  10
#define BTL_CMDINFO_SHORT_SUMMON      (1 << BTL_CMDINFO_SHORT_SUMMON_BIT) /* info.b.shortSummon */
#define BTL_CMDINFO_ENEMY_BOUNCE      0x800 /* info.b.enemyBounce */

typedef struct BtlShadow {
    u32 shade : 8; /* grey level of the shadow quads (btl_drawShadow) */
    u32 width : 12; /* btl_setShadowSize */
    u32 depth : 12;
    u32 unk4_0 : 12; /* unused (Unk800AD4E4.pad4_0) */
    u32 offsetX : 10; /* x offset of the shadow (Unk800AD4E4, btl_calcShadowMatrix) */
    u32 offsetZ : 10; /* z offset */
    POLY_FT4 *quads[2][4]; /* shadow quads, per draw buffer */
} BtlShadow; /* 0x28 */

typedef struct Battle11C0 {
    s32 unk0; /* write-only (cleared by btl_initReturnInfo) */
    s16 returnId; /* field id or world position to return to (btl_initReturnInfo) */
    u8 returnMode; /* GameState mode before the battle (btl_cleanup) */
    u8 unk7[1]; /* write-only (cleared by btl_initReturnInfo) */
    s32 freeHeap; /* g_modeHeap once the shadows are allocated (btl_load, btl_initReturnInfo); never read */
    s32 *loadBuf; /* loader work buffer (CD requests and step states at 0x9FA0/0x9FD8) */
    u8 *script; /* battle event script (dependency 0x05xxxx; btl_initEncounter, evt_init) */
    u8 *texts; /* enemy and enemy-ability names (BtlEnemyType.nameOfs, btl_showCmdName); the bundle's DB_AREA item */
    struct BtlBgHeader *bgHeader; /* background header (btlbg_init) */
    s32 bgData; /* background groups (btlbg_draw) */
    Battle11C020 *encounter; /* encounter header: groups, enemy records, abilities (btl_initScene) */
    u16 *bundle; /* encounter bundle buffer (0x2800), starts with the sequence and scene data table offsets */
    struct BtlShadow *shadows; /* per unit model slot (btl_drawShadow) */
    s32 ot[2]; /* small ordering table per draw buffer (btl_beginFrame) */
    s32 prims[2]; /* primitive buffer per draw buffer */
    s32 *weaponBufs; /* per party slot: weapon model file buffer (btl_loadWeaponModelsStep) */
    BtlUnit9C50 *bgTexAnims; /* background texture animations (btlbg_init) */
    u8 *heapStart; /* work heap start */
    u8 *heap; /* work heap cursor */
} Battle11C0;

typedef struct {
    u32 targetType : 4; /* as BtlCmd8.targetType */
    u32 defaultSide : 1; /* target select starts on this side (g_btlTarget.side) */
    u32 listKind : 3; /* nonzero: targets are picked from a list window of this kind (btlui_openListWindow) */
    u32 unk0_8 : 21; /* not read in ovl_0b */
    u32 allowKo : 1; /* KO units can be targeted */
    u32 unk0_30 : 1; /* not read in ovl_0b */
    u32 koFirst : 1; /* the cursor starts on a KO unit (btlui_findKoUnit) */
    u8 pad4[8 - 4];
    u8 flags; /* as BtlCmd8.flags (2 needs a voice) */
    u8 pad9;
    u8 mpCost; /* runtime: Fire 6, Fira 12, Sleep 10 as the spell list shows */
    u8 padB[0xE - 0xB];
    u16 nameOfs; /* as BtlCmd8.nameOfs */
} Battle16CC; /* 0x10 */

/* Battle.popups ring entry (btl_newPopup hands out the next one) */
typedef struct {
    u8 *anim; /* animation, 0xFF-terminated */
    u16 glyph; /* glyph index into g_uiSprites */
    u8 pad6[8 - 6];
} Unk800F6A54;

typedef struct BtlPopup {
    struct BtlUnit *unit;
    u8 pad4[8 - 4];
    s16 x; /* base bone position (btl_newPopup) */
    s16 y;
    s16 z;
    u16 frame; /* animation frame */
    union {
        s32 w; /* cleared as a word */
        struct {
            u8 delay; /* frames before it shows (btl_drawPopups) */
            s8 yOfs; /* y offset that keeps it on screen, fixed in its first frame (btl_clampPopup) */
            u8 kind; /* BTL_POPKIND_*: g_btlPopupDrawFns draw handler */
            u8 noClamp; /* status-tick popups (btl_spawnTickPopups): keep yOfs, no clamping */
        } b;
    } info;
    union {
        u32 w; /* packed digits (btl_packDigits) */
        Unk800F6A54 *p; /* text popup: animation and glyph */
    } data;
    u16 clut; /* palette offset added to the glyphs' clut (0 damage, BTL_POPCLUT_HEAL) */
    u8 pad1A[0x1C - 0x1A];
} BtlPopup; /* 0x1C */
#define BTL_POPCLUT_HEAL 0xC0 /* BtlPopup.clut of heal numbers (BTL_POPUP_HEAL, BTL_POPUP_MP_HEAL, regen); btl_drawNumberPopup bounces them with g_btlPopupBounceHeal */

/* BtlPopup.info.b.kind: g_btlPopupDrawFns index */
#define BTL_POPKIND_HP   0 /* btl_popupHp */
#define BTL_POPKIND_MP   1 /* btl_popupMp */
#define BTL_POPKIND_TEXT 2 /* btl_popupText */
#define BTL_POPKIND_DONE 3 /* and up: the popup is removed */

#define BTL_POPUP_RING_SIZE 16 /* BtlPopupRing.entries: popups shown at once */
typedef struct {
    BtlPopup entries[BTL_POPUP_RING_SIZE];
    s16 last; /* last entry handed out (counts down, wraps to 15) */
    s16 tick; /* btl_drawPopups calls */
    s32 scanUnit; /* BtlUnit *: unit being scanned (btl_startScan); btlseq_opEnd waits while set */
    union {
        s32 w; /* cleared as a word */
        u8 b[4]; /* b[0]: delay, b[1]: item index, b[2]: state */
    } scan; /* Scan banner sequence (btl_updateScan) */
    BtlEnemy *detectEnemy; /* enemy whose steal items Detect lists, NULL = idle (btl_startDetect, btl_updateDetect) */
    u8 detectDelay; /* frames until the next item banner */
    u8 detectSlot; /* next steal slot to show (0-3, 4 = done) */
    u8 pad1D2[0x1D4 - 0x1D2];
    u8 hiddenMask; /* units hidden from status effects (mask) */
    u8 pad1D5[0x1D8 - 0x1D5];
} BtlPopupRing; /* Battle.popups (0x1D8) */
/* damage / text popup placement (btl_drawNumberPopup, btl_drawTextPopup, btl_clampPopup): screen limits a popup is kept
 * inside, and the number font's pitch. Evidence: [code] */
#define BTL_POPUP_LEFT    8   /* x and hard top limit */
#define BTL_POPUP_RIGHT   312 /* right edge (a popup ending past it moves to 312 - w) */
#define BTL_POPUP_TOP     21  /* first frame above this: the popup is shifted down for its whole life (info.b.yOfs) */
#define BTL_POPUP_BOTTOM  211 /* bottom limit (moves to 210 - h) */
#define BTL_POPUP_DIGIT_W 8   /* digit pitch of the number popups */
#define BTL_POPUP_NUM_H   10  /* height btl_drawNumberPopup clamps a number by */

/* Inlined copy of btl_clearCmd (clear a command); the original source inlines it. */
static inline void btl_clearCmd_inline(BtlCmd *cmd) {
    cmd->next = 0;
    cmd->ability = 0;
    cmd->targets = 0;
    cmd->cmdNo = 0;
    cmd->subNo = 0;
    cmd->info.b.targeting = 0;
    cmd->info.b.unk0_2 = 0;
    cmd->info.b.automatic = 0;
    cmd->info.b.covered = 0;
    cmd->info.b.dodged = 0;
    cmd->info.b.reflect = 0;
    cmd->info.b.magicFailed = 0;
    cmd->info.b.shortSummon = 0;
    cmd->info.b.enemyBounce = 0;
}

typedef struct Battle108C {
    DR_TPAGE tpage;
    POLY_F3 poly; /* cursor face */
} Battle108C; /* 0x1C */

/* target cursor: four faces per draw buffer */
typedef struct Battle108CSet {
    Battle108C faces[4];
} Battle108CSet;

/* battle scene data (Battle.scene) */
typedef struct {
    u32 flags; /* BTL_SCENE_*: bits 0-1 ambush (BTL_GETVAR_AMBUSH: 0 back attack, 1 preemptive, 2 normal, btl_rollAmbush); bits 2 and up
                * are the encounter's Battle11C020.flags ^ 0x30, shifted left 2 (btl_initSceneFlags) */
    Battle1330 *group; /* enemy group chosen by btl_pickEnemyGroup */
    Battle1334 *enemyTypes; /* the encounter's enemy type records */
    u16 *seqTable; /* enemy battle sequences: counts, offsets, animations (btlseq_init) */
    s16 *sceneData; /* offset table, entries relative to its start (btl_getSceneData) */
} Battle132C;

/* Battle132C (Battle.scene).flags: bits 0-1 the ambush, bits 2 and up the encounter flags (btl_initSceneFlags).
 * Evidence: [code] */
#define BTL_SCENE_AMBUSH        0x03 /* BTL_AMBUSH_* (btl_rollAmbush, BTL_GETVAR_AMBUSH) */
#define BTL_AMBUSH_BACK         0 /* back attack: the party starts with empty ATB, "Back attack!" */
#define BTL_AMBUSH_PREEMPTIVE   1 /* preemptive strike: the enemies start with empty ATB, the party's are full */
#define BTL_AMBUSH_NORMAL       2
/* Battle11C020.flags -> Battle.scene.flags (btl_initSceneFlags). Evidence: [code] */
#define BTL_ENC_TO_SCENE        4    /* scene.flags = (flags ^ BTL_ENC_INVERTED) * 4: bit n becomes scene bit n + 2 */
#define BTL_ENC_INVERTED        0x30 /* stored inverted: they become BTL_SCENE_VICTORY and BTL_SCENE_CAN_ESCAPE */
#define BTL_SCENE_SCRIPT_START_BIT 2 /* the intro hands over to the event script (BTL_PHASE_SCRIPT, btlcam_step) */
#define BTL_SCENE_SCRIPT_START  (1 << BTL_SCENE_SCRIPT_START_BIT)
#define BTL_SCENE_BACK_ATTACK_BIT 3 /* btl_rollAmbush: a back attack for sure (unless Alert); with BTL_SCENE_SCRIPT_START a
                                    * back attack instead of a normal start, kept by BTL_SETVAR_RESUME */
#define BTL_SCENE_BACK_ATTACK   (1 << BTL_SCENE_BACK_ATTACK_BIT)
#define BTL_SCENE_NO_GAMEOVER_BIT 4 /* a defeat plays no defeat music and keeps battleResult 3 (else 6; btl_saveParty) */
#define BTL_SCENE_NO_GAMEOVER   (1 << BTL_SCENE_NO_GAMEOVER_BIT)
#define BTL_SCENE_VICTORY_BIT   6 /* victory music, poses and rewards (btl_victory); BTL_SETVAR_END 1 goes through it */
#define BTL_SCENE_VICTORY       (1 << BTL_SCENE_VICTORY_BIT)
#define BTL_SCENE_CAN_ESCAPE_BIT 7 /* clear: "Can't escape!" (btl_tryEscape, btl_canFlee) */
#define BTL_SCENE_CAN_ESCAPE    (1 << BTL_SCENE_CAN_ESCAPE_BIT)
#define BTL_SCENE_OUT_OF_REACH_BIT 8 /* the enemies are out of reach of weapons with weapon[0] bit 0: "Can't reach!" (btl_checkCmdUsable) */
#define BTL_SCENE_OUT_OF_REACH  (1 << BTL_SCENE_OUT_OF_REACH_BIT)
#define BTL_SCENE_NO_VOICE_BIT  9 /* abilities needing a voice (BTL_ABIL_VOICE) are greyed out as under Silence (btlui_getAbilityState) */
#define BTL_SCENE_NO_VOICE      (1 << BTL_SCENE_NO_VOICE_BIT)
#define BTL_SCENE_INVERT_POWER_BIT 10 /* btl_applyInvertedPower: power = 60 - power */
#define BTL_SCENE_INVERT_POWER  (1 << BTL_SCENE_INVERT_POWER_BIT)
#define BTL_SCENE_NO_PARTY_FX_CAMERA_BIT 11 /* no effect camera when the first target is a party unit (btlfx_updateCameraEnable) */
#define BTL_SCENE_NO_PARTY_FX_CAMERA (1 << BTL_SCENE_NO_PARTY_FX_CAMERA_BIT)
#define BTL_SCENE_NO_ENEMY_FX_CAMERA_BIT 12 /* the same for enemy targets */
#define BTL_SCENE_NO_ENEMY_FX_CAMERA (1 << BTL_SCENE_NO_ENEMY_FX_CAMERA_BIT)
#define BTL_SCENE_POST_EVENT_BIT 13 /* g_btlRewards.hasPostEvent (btl_startBattle) */
#define BTL_SCENE_POST_EVENT    (1 << BTL_SCENE_POST_EVENT_BIT)
#define BTL_SCENE_KEEP_MUSIC_BIT 14 /* the battle music goes on after the battle (GD_BTL_KEEP_MUSIC; not after the victory music) (btl_update) */
#define BTL_SCENE_KEEP_MUSIC    (1 << BTL_SCENE_KEEP_MUSIC_BIT)
/* bit 14: without VICTORY, a victory, stop or escape sets GameData.battleFlags 8 */

/* a model object padded to the 0x98 stride of the weapon and effect model buffers */
typedef struct {
    BtlModel model;
    u8 pad7C[0x98 - sizeof(BtlModel)];
} BtlModelBuf; /* 0x98 */

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u16 flags; /* BTL_FLAG_* */
    u8 unk2[2]; /* write-only (cleared by btl_clearBattle; a write watch in round 31 saw no other writes) */
    s32 savedHeap; /* g_modeHeap after btl_initMode; btl_load allocates from here again */
    s32 partyMotions[4][0x22]; /* per party member: motion data addresses; BtlUnit.motions points here (btl_loadPartyMotions) */
    BtlModelBuf weaponModels[4]; /* per party member: weapon model (BtlUnit.weaponModel, btl_initPartyUnit) */
    BtlEnemy enemies[4];
    BtlEnemyType enemyTypes[3]; /* enemy type records (BtlEnemy.unk0 points here) */
    struct BtlCmd8 enemyAbilities[18]; /* enemy attacks: BtlCmd.ability of enemy commands (btl_setEnemyCmd) and Return Magic */
    BtlUnit *unitList; /* active units, linked through BtlUnit.next */
    BtlUnit units[8]; /* runtime: party at [0..3] (Zidane 0x80108EFC, Vivi +0xD0, ...) */
    s32 tick; /* btl_update count; low 4 bits animate (bobbing, cursor) */
    u8 phase; /* BTL_PHASE_* */
    u8 subPhase; /* in BTL_PHASE_END how the battle ended (BTL_END_*); a step counter in the exit phases (in
                  * BTL_PHASE_EXIT the victory camera's BTLCAM_OP_CUE takes it 0 -> 1 to start the fade-out).
                  * A camera script's cue increments it in any phase, and btl_checkSideDefeated reads a nonzero
                  * value left over from combat as BTL_END_DEFEAT (only a victory can't overwrite it). */
    u8 exitTimer; /* exit phases: frames of the fade before btl_saveParty */
    u8 escapeHeld; /* runtime: 1 while L1+R1 are held (btlui_pollEscape), units take the run-away stance */
    u8 fleeBrightness; /* escaping units' brightness, -2 per tick (btl_victory) */
    u8 loadFlags; /* BTL_LOAD_*: battle start steps */
    u8 partyBrightness; /* intro fade-in, +4 per tick to 0x20 (btl_updateIntro) */
    u8 enemyBrightness; /* intro fade-in, +4 per tick to 0x20 */
    u8 padD88[0xD90 - 0xD88];
    BtlCmd cmdSlots[36]; /* per unit: 6 (players) or 3 (enemies); BtlUnit.cmd.. point here */
    BtlCmd escapeCmd; /* the party's escape (BTL_CMD_ESCAPE) command */
    BtlCmd cmdQueue; /* list head (only unk0 used); runtime: commands are queued here when issued */
    BtlCmd *curCmd; /* runtime: Vivi's command while her Attack executes */
    struct Battle108CSet cursorPrims[2]; /* target cursor, per draw buffer */
    BtlUnitList fxTargets; /* effect target list (btl_playCmdEffect -> btlfx_setTargets) */
    u16 cmdFlags; /* BTL_CMDF_* */
    u8 cmdPhase; /* curCmd's step in btl_processCmdQueue (0 check, 1 start effect, 2 wait, 3 finish) */
    u8 eidolonAbility; /* ability of the trance Eidolon follow-up (btl_startCmdEffect, BTL_CMD_EIDOLON) */
    s16 eidolonTimer; /* counts down by atbSpeed until BTL_CMD_TRANCE_EIDOLON */
    u16 deathCryWait; /* frames until the next death cry may play: btl_playDeathCry sets 8, btl_drawScene counts down */
    struct Battle11C0 *work; /* battle work area: buffers, background, encounter, shadows (btl_allocBuffers) */
    struct Battle11C4 seq; /* enemy battle sequences (btlseq_*) */
    u8 cameraNo; /* camera variant (0-2) the camera scripts pick (btlcam: g_btlCamera.variant); btl_initScene from the group, btlseq_opSetCamera / btlseq_opPlayCamera set it */
    u8 pad131D[0x132C - 0x131D];
    Battle132C scene;
    u8 groupNo; /* enemy group index btl_pickEnemyGroup chose */
    u8 pad1341[0x1354 - 0x1341];
    BtlFiles *files;
    BtlPopupRing popups;
    s32 unk1530; /* write-only (cleared by btl_clearBattle) */
    s32 dataSector; /* battle data file sector */
    struct Battle1538 **partyBufEnds; /* per party slot: allocation header in the last 0x28 bytes of partyBufs[i] */
    s32 partyBufs[4]; /* per party slot: 0x13000-byte model file buffer (btl_allocBuffers); the effect loader reuses [0] for summons */
    BtlStatusInfo statusInfo[32];
    Battle16CC abilities[0xC0]; /* runtime: BtlCmd.ability = &abilities[subNo] (Attack 176, Focus 188) */
    s32 statusSets[64]; /* status masks indexed by ability/weapon status-set bytes (btl_applyAbilityStatus) */
} Battle; /* 0x23CC */

/* Battle.flags. Evidence: [code] */
#define BTL_FLAG_BG_LOADED       0x01 /* background loaded: btl_updateIntro draws it (btl_loadBackgroundStep) */
#define BTL_FLAG_ENEMIES_LOADED  0x02 /* enemy models loaded (btl_loadEnemyModelsStep); btl_updateIntro initialises the enemies */
#define BTL_FLAG_PARTY_LOADED    0x04 /* party models loaded (btl_loadModelsStep, btl_reloadPartyStep); btl_updateIntro initialises the party */
#define BTL_FLAG_UI_STARTED      0x10 /* btlui_init ran (btl_main, once loadFlags is BTL_LOAD_READY); the disc cover check runs from here on */
#define BTL_FLAG_DISC_COVER_OPEN 0x20 /* btl_checkDiscCover: shell open, the pause screen shows "Disc Cover Open" */
#define BTL_FLAG_EXIT_PRELOADED  0x40 /* btl_preloadExit ran (btl_main) */
#define BTL_FLAG_NO_DISPENV      0x100 /* btl_putDispEnvNoUi and btlui_putDispEnvIfReady skip PutDispEnv; never set by game C */
#define BTL_FLAG_NO_DRAW         0x200 /* btl_endFrame skips the background and main OT draws; never set by game C */
#define BTL_FLAG_OVER            0x1000 /* btl_update returned 1: btl_main leaves (mode 8) */

/* Battle.loadFlags: battle start steps (btl_updateIntro). Evidence: [code] */
#define BTL_LOAD_STARTED       0x01 /* btl_startBattle ran */
#define BTL_LOAD_ENEMIES_SHOWN 0x02 /* the enemies have faded in; btlseq_update runs from here on */
#define BTL_LOAD_PARTY_SHOWN   0x04 /* the party has faded in */
#define BTL_LOAD_READY         (BTL_LOAD_STARTED | BTL_LOAD_ENEMIES_SHOWN | BTL_LOAD_PARTY_SHOWN) /* btl_main starts the UI, btlcam_step goes on */
#define BTL_LOAD_ENEMIES_INIT  0x08 /* btl_initEnemies ran */
#define BTL_LOAD_PARTY_INIT    0x10 /* btl_initParty ran */
#define BTL_LOAD_ENEMY_GREY    0x20 /* btl_makeEnemyGreyCluts ran: the enemy fade-in starts */
#define BTL_LOAD_PARTY_GREY    0x40 /* btl_makePartyGreyCluts ran: the party fade-in starts */

/* Battle.cmdFlags. Evidence: [code] */
#define BTL_CMDF_ESCAPE         0x01 /* an escape (BTL_CMD_ESCAPE) is queued (btl_setCmd): no other command is accepted, units take the flee stance */
#define BTL_CMDF_MARKERS_BIT    1
#define BTL_CMDF_MARKERS        (1 << BTL_CMDF_MARKERS_BIT) /* active-unit marker and target projection on (btl_setMarkersVisible; set by btl_initCmdQueue) */
#define BTL_CMDF_EIDOLON_TIMER  0x04 /* trance Dagger's Eidolon follow-up timer runs (btl_finishCmd after BTL_CMD_EIDOLON) */
#define BTL_CMDF_EIDOLON_QUEUED 0x08 /* the timer ran out and BTL_CMD_TRANCE_EIDOLON is queued (btl_updateUnitStatus) */
#define BTL_CMDF_SPEAR          0x10 /* a Spear (BTL_CMD_SPEAR*) is queued: jumping units' timers stop (btl_updateUnitStatus) */

/* battle work area, allocated at g_loadAreaEnd + 0x61800 */
typedef struct {
    Battle battle;
    u8 filesBuf[0x240C - 0x23CC]; /* Battle.files */
    u8 partyBufEndsBuf[0x241C - 0x240C]; /* Battle.partyBufEnds */
    u8 heap[1]; /* becomes g_modeHeap */
} BattleMem;

/* g_btlfxOpArgs: parameter bytes for the unit op that follows; what they mean depends on that op (dist, frames,
 * turnAway, partyMode, ...). Evidence: [code] */
typedef struct {
    u8 paramA[2]; /* unit op 0x50 (btlfx_opSetParamsA): its two arguments */
    u8 paramB[2]; /* unit op 0x51 (btlfx_opSetParamsB): its two arguments */
} Unk80100D88;
extern Unk80100D88 g_btlfxOpArgs;


typedef struct {
    u16 hp; /* BtlUnit.hp / Character.hp (btl_copyStats) */
    u16 mp;
    u16 atb; /* BtlUnit.atb / Character +0x14 */
    u8 pad6;
    u8 stones; /* Character.stones (BtlUnit +0x2F) */
} Unk800BD8C0;

typedef struct {
    u16 text; /* text offset or id: enemy and enemy-ability names (Battle11C0.texts), g_btlAbilityHelp offsets, list-window title labels */
    u16 unk2; /* no access in any binary. g_btlAbilityHelpOfs: lines << 12 | text width, as TextTblEnt.layout (all 192 entries); g_btluiListTitles: 0 */
} U16Pair;


typedef struct {
    s16 pos[4]; /* x, y, z, and the fourth value (distance / roll) */
    u8 anchor; /* bits 0-4 anchor kind (btlcam_resolveAnchor: 1 caster, 5 target, 0xA focusUnit, 0xF partner, 0x15+ points), 0x20 use the unit's rotation */
    u8 anchorFlags; /* bits 0-4 second anchor averaged in (btlcam_resolveAnchorPair), BTLCAM_AF_* */
    u8 padA[0xC - 0xA];
} Unk800C412C0; /* 0xC */

/* Camera anchor byte (Unk800C412C0.anchor; anchorFlags bits 0-5 name a second anchor in the same layout, averaged in
 * by btlcam_resolveAnchorPair with the first anchor's height bits). Evidence: [code] btlcam_resolveAnchor,
 * btlcam_getAnchorUnit, btlcam_getUnitAnchors */
#define BTLCAM_ANCHOR_KIND        0x1F /* kind, BTLCAM_ANCHOR_* below */
#define BTLCAM_ANCHOR_HOME        0x20 /* take the unit's model rotation and home position (else its current rot/pos) */
#define BTLCAM_ANCHOR_HEIGHT      0xC0 /* which height of the unit (btlcam_getUnitAnchors) the anchor sits at: */
#define BTLCAM_ANCHOR_H_MID       0x00 /*   anchors[1]: half the model height (party) / enemy bone 1 */
#define BTLCAM_ANCHOR_H_HIGH      0x40 /*   anchors[2]: three quarters up (party) / enemy bone 2 */
#define BTLCAM_ANCHOR_H_GROUND    0x80 /*   y 0 */
#define BTLCAM_ANCHOR_H_LOW       0xC0 /*   anchors[0]: a quarter up (party) / enemy bone 0 */
/* anchor kinds (BTLCAM_ANCHOR_KIND) */
#define BTLCAM_ANCHOR_NONE        0x00 /* origin */
#define BTLCAM_ANCHOR_CASTER      0x01 /* g_btlCamTargets.caster */
#define BTLCAM_ANCHOR_TARGET      0x05 /* g_btlCamTargets.target */
#define BTLCAM_ANCHOR_TARGET_CENTER 0x09 /* the target, at g_btlCamTargets.centerX/Z */
#define BTLCAM_ANCHOR_FOCUS       0x0A /* g_btlCamera.focusUnit (nothing when NULL) */
#define BTLCAM_ANCHOR_ENEMY_GROUP 0x0B /* the enemy group's first placement x/z, y 0 */
#define BTLCAM_ANCHOR_SEQ_CASTER  0x0C /* Battle.seq.camCaster */
#define BTLCAM_ANCHOR_SEQ_TARGET  0x0D /* Battle.seq.camTarget */
#define BTLCAM_ANCHOR_SEQ_CENTER  0x0E /* Battle.seq.camTarget, at seq.camCenterX/Z */
#define BTLCAM_ANCHOR_PARTNER     0x0F /* g_btlCamTargets.partner */
#define BTLCAM_ANCHOR_POINT0      0x15 /* 0x15 + i: g_btlCamera.points[i] (and pointRots[i] for i < 4) */
#define BTLCAM_ANCHOR_SAVED_EYE   0x19 /* points[4]: eye kept by BTLCAM_OP_SET_HOME_VIEW (btlcam_evalPath counts viewState) */
#define BTLCAM_ANCHOR_SAVED_AT    0x1A /* points[5]: target kept by BTLCAM_OP_SET_HOME_VIEW */

typedef struct {
    Unk800C412C0 keys[2]; /* from [0] and to [1] (btlcam_setPathKey shifts them) */
    u8 mode; /* BTLCAM_PATH_* */
    u8 wrapModes; /* per axis, 2 bits: how angles wrap while lerping (BTLCAM_WRAP_*) */
    u8 easeModes; /* 2 bits per weight (BTLCAM_EASE_*; g_btlCamScript.easeSpeed) */
    u8 locked; /* keys set by camera flag 0x8000: the next command doesn't replace the mode or timing */
    u8 frames;
    u8 framesLeft; /* counts down in btlcam_step */
    u8 pad1E[0x20 - 0x1E];
} Unk800C412C; /* 0x20 */

/* Unk800C412C.easeModes / wrapModes: 2-bit fields, one per weight / axis (btlcam_evalPath). Evidence: [code] */
#define BTLCAM_MODE2_BITS  2
#define BTLCAM_MODE2_MASK  3
#define BTLCAM_EASE_LINEAR 0 /* weight falls evenly with framesLeft */
#define BTLCAM_EASE_IN     1 /* starts at g_btlCamScript.easeSpeed (at rest when 0) and speeds up (runtime) */
#define BTLCAM_EASE_OUT    2 /* slows down to g_btlCamScript.easeSpeed (to rest when 0) (runtime) */
#define BTLCAM_WRAP_SHORT  0 /* turn the short way round */
#define BTLCAM_WRAP_UP     1 /* the to-angle gets + 0x1000 when it is not above the from-angle */
#define BTLCAM_WRAP_DOWN   2 /* the from-angle gets + 0x1000 when it is not above the to-angle */

/* Unk800C412C.mode (g_btlCamera.paths[]; set from camera script op bits 5-6) */
#define BTLCAM_PATH_OFF      0
#define BTLCAM_PATH_HOLD     1 /* hold the key */
#define BTLCAM_PATH_LERP     2
#define BTLCAM_PATH_ANCHORED 3 /* lerp between anchored points */
#define BTLCAM_PATH_CUBIC    4 /* cubic over g_btlCamera.points */

/* Camera script commands (btlcam_step): u16 cmd[0] = frame (low 10 bits) | flag bits 16-21 (top 6 bits),
 * u16 cmd[1] = flag bits 0-15, then the arguments (bytes, each group 2-byte aligned) of each set flag in the order below. Bits 0-2 / 3-5 are the
 * eye / target path modes (BTLCAM_PATH_*). Evidence: [code] */
#define BTLCAM_CMD_FRAME_MASK  0x3FF
#define BTLCAM_CMD_PATH_BITS   3        /* path i's mode is (flags >> (i * 3)) & 7 */
#define BTLCAM_OP_EASE_SPEED   0x40     /* u8 g_btlCamScript.easeSpeed, pad */
#define BTLCAM_OP_SET_HOME_VIEW 0x80    /* copy the eye/target (before bob and shake) to points[4]/[5], where the idle camera
                                         * (scene data 1) and the target-select view sit: every intro sets it with its
                                         * END_INTRO cue, so the battle's home view is where the intro ended (runtime) */
#define BTLCAM_OP_PAUSE_CMD    0x100    /* stop counting script frames until the action resumes them: used by the command
                                         * cameras (scene data 3/4), released when the cast ends (code) */
#define BTLCAM_OP_BOB          0x200    /* u8 g_btlCamScript.bob (BTLCAM_BOB_*), pad: vertical bob of eye and target (runtime) */
#define BTLCAM_OP_IDLE_BOB     0x400    /* u8 g_btlCamera.idleBob, pad: the bob used when a script sets none; outlives the script */
#define BTLCAM_OP_ZOOM         0x800    /* u8 frames, u8 target, u16 projection distance (0: the default) */
#define BTLCAM_OP_SHAKE        0x1000   /* u8 from, to, frames, axes (bits 0-1) */
#define BTLCAM_OP_PICK_VARIANT 0x2000   /* effectPick = the playing variant */
#define BTLCAM_OP_CUE          0x4000   /* u8 BTLCAM_CUE_END_INTRO, else Battle.subPhase++ (see there); pad */
#define BTLCAM_OP_RETURN       0x8000   /* u8 path mode (bits 5-6, both paths), frames, easeModes: ends the script, back to the idle scene camera (scene data 1) */
#define BTLCAM_OP_PAUSE_SEQ    0x100000 /* same effect as BTLCAM_OP_PAUSE_CMD (tested together); used by the enemy-type
                                         * cameras (scene data 5+), released by btlseq_opWaitAnim (runtime) */
#define BTLCAM_OP_KEEP_CAMERA  0x80000  /* btlcam_endEffect keeps this camera instead of reloading the idle one */
/* BTLCAM_OP_CUE argument */
#define BTLCAM_CUE_END_INTRO   1 /* set g_btlCamera.endIntro: the intro camera may end; any other value is a subPhase cue */
/* On the discs (tools/btlcam_scan.py) every intro camera (scene data 0) cues 1 and every victory camera (scene data 2,
 * all but battle 938's) cues 2; no idle, command, enemy, AI-played or effect camera has a cue. */

typedef struct Unk800FE3B8 {
    SVECTOR points[6]; /* camera path control points (cubic, btlcam_step); [4]/[5] keep the eye/target for BTLCAM_OP_SET_HOME_VIEW */
    SVECTOR pointRots[4]; /* rotations kept with points[0-3] (zeroed by btlcam_loadScript; btlcam_resolveAnchor anchors 0x15+) */
    s16 yaw; /* from eye to target (btlcam_step) */
    u8 paused; /* script frame stops counting (BTLCAM_OP_PAUSE_CMD/_SEQ); the action resumes it */
    u8 effectPick; /* camera choice btlfx_opPlayCamera mode 3 plays: random 0/1 per effect, or the variant (BTLCAM_OP_PICK_VARIANT) */
    SVECTOR eye; /* eye position given to btlcam_lookAt */
    u8 flags; /* BTLCAM_F_* */
    u8 stepped; /* the effect frame already stepped the camera: btlcam_update skips this frame */
    u16 projDist; /* projection distance (SetGeomScreen through GameData.unkA) */
    u16 baseProjDist; /* default projection distance (0x200) */
    u8 pad62[0x65 - 0x62];
    u8 prevViewState; /* viewState of the previous frame */
    u8 projFixed; /* the target view set the projection (btlcam_step leaves GameData.unkA) */
    u8 endIntro; /* BTLCAM_OP_CUE with BTLCAM_CUE_END_INTRO: ends the intro once loadFlags has all three setup bits */
    Unk800C412C paths[2]; /* eye path [0], target path [1] (btlcam_evalPath) */
    s32 variant; /* script variant 0-2 the camera plays (btlcam_loadScript: from the script header, Battle.cameraNo, the unit size, ...) */
    u32 idleBob; /* bob used when the script sets none (BTLCAM_OP_IDLE_BOB): BTLCAM_BOB_* */
    s32 idleBobPhase;
    s32 viewBlend; /* frames left of the blend into the target view (btlcam_applyTargetView) */
    u8 nextVariant; /* variant chosen by a header with BTLCAM_HDR_NEXT_VARIANT for the next script (BTLCAM_SEL_CHAINED) */
    u8 mode; /* BTLCAM_MODE_* */
    u8 targetView; /* 1 while the target-select view is on */
    u8 viewState; /* 2: at the target view; btlcam_evalPath counts anchors 0x19/0x1A up to it */
    SVECTOR viewFromEye; /* eye when the target view started */
    SVECTOR viewFromAt; /* target when the target view started */
    BtlUnit *focusUnit; /* unit the scene camera follows (anchor 0xA; btlcam_loadScript variant from its state) */
} Unk800FE3B8;

/* Camera script header (btlcam_loadScript): u16 flags, then a u16 offset per set bit 0-3 (bits 0-2: the pcs of
 * variants 0-2, bit 3: the variant selector word), the offset of the point data, then 3 s16 per set bit 4-7
 * (g_btlCamera.points[0-3]). Evidence: [code] */
#define BTLCAM_HDR_SLOTS        4      /* offsets (bits 0-3) and points (bits 4-7) */
#define BTLCAM_HDR_SELECTOR     3      /* slot of the variant selector word */
#define BTLCAM_HDR_NEXT_VARIANT 0x200  /* store the next script's variant: bits 10 + 2 * variant (2 bits) */
#define BTLCAM_HDR_NEXT_SHIFT   10
/* variant selector word: kind (bits 14-15), argument (bits 12-13), bias (bits 10-11) */
#define BTLCAM_SEL_KIND_SHIFT   14
#define BTLCAM_SEL_FIXED        0      /* variant = the argument */
#define BTLCAM_SEL_RANDOM       1      /* argument 0: 0 or 1, (5 + bias) in 10 for 0; else 0, 1 or 2 ((3 + bias), 3, rest) */
#define BTLCAM_SEL_CONTEXT      2      /* chosen by BTLCAM_SELCTX_* (the argument) */
#define BTLCAM_SEL_CHAINED      3      /* g_btlCamera.nextVariant (3 means 2, 2 means 0) */
#define BTLCAM_SEL_ARG_SHIFT    12
#define BTLCAM_SEL_ARG          0x3000
#define BTLCAM_SEL_BIAS_SHIFT   10
#define BTLCAM_SELCTX_AMBUSH    0      /* normal battle 0, else random 0/1; kept as savedVariant */
#define BTLCAM_SELCTX_CAMERA_NO 1      /* Battle.cameraNo */
#define BTLCAM_SELCTX_TARGET_SIZE 2    /* the target's radius/height: 0 small, 1 medium, 2 large */
#define BTLCAM_SELCTX_FOCUS     3      /* 0 when focusUnit can't act, else by the ambush and the party's live count */
/* BTLCAM_SELCTX_TARGET_SIZE: the target model header's radius and height (btlcam_loadScript). Evidence: [code] */
#define BTLCAM_SIZE_SMALL  0 /* radius below BTLCAM_RADIUS_SMALL, or medium radius and height below BTLCAM_HEIGHT_SMALL */
#define BTLCAM_SIZE_MEDIUM 1
#define BTLCAM_SIZE_LARGE  2 /* radius BTLCAM_RADIUS_LARGE and up or BTLCAM_RADIUS_WORM_HYDRA, or height BTLCAM_HEIGHT_LARGE and up */
#define BTLCAM_RADIUS_SMALL       101
#define BTLCAM_RADIUS_LARGE       3000
#define BTLCAM_RADIUS_WORM_HYDRA  1239 /* counted as large: [data] the only enemy model with this radius is 340, Worm Hydra (tools/battle_models.py; height 559 would make it medium) */
#define BTLCAM_HEIGHT_SMALL       345
#define BTLCAM_HEIGHT_LARGE       1001

/* bob byte (g_btlCamScript.bob, g_btlCamera.idleBob; btlcam_applyBob). Evidence: [code], runtime (docs/runtime-findings.md BTLCAM_OP runtime check) */
#define BTLCAM_BOB_AMP         0xF0   /* amplitude (scaled by the eye-target distance) */
#define BTLCAM_BOB_SPEED       0x0F   /* phase step per frame */

/* Unk800FE3B8 (g_btlCamera).mode: what the camera plays. Evidence: [code] */
#define BTLCAM_MODE_SCENE  0 /* btlcam_playScene */
#define BTLCAM_MODE_EFFECT 1 /* btlcam_play */
#define BTLCAM_MODE_SEQ    2 /* enemy sequence (btlcam_playSeq) */
/* Unk800FE3B8.flags. Evidence: [code] */
#define BTLCAM_F_SCRIPT_STARTED 0x01 /* a script command ran: the projection distance was reset to 0x200 (btlcam_step) */
#define BTLCAM_F_EFFECT_RUNNING 0x02 /* the effect script runs its ops (btlfx_runScript phase 4); btlcam_play clears the flags then */
extern Unk800FE3B8 g_btlCamera;

#define BTL_REWARD_ITEM_SLOTS 16 /* g_btlRewards.items */
typedef struct {
    s32 gil; /* gil (BtlEnemyType.gil of the defeated enemies; ovl_65 BattleResult.gil) */
    s32 exp; /* exp (BtlEnemyType.exp; ovl_65 splits it among expMask) */
    s16 ap; /* ap of the enemy group (Battle1330.ap, btl_initSceneFlags). runtime: Malboro group 3 = "AP 3" */
    u8 expMask; /* bit per party slot in the battle (btl_initPartyUnit), cleared when a unit leaves (btl_calcCmd 0x6A); ovl_65: receives exp */
    u8 hasPostEvent; /* encounter flag 0x800, story battles (BattleResult in ovl_65, runtime) */
    u8 items[BTL_REWARD_ITEM_SLOTS]; /* dropped item ids, 0xFF = none (btl_addEnemyRewards) */
    u8 card; /* card won, 0xFF = none */
    u8 fleeGil; /* set when Flee-Gil (support 0x4000) turned the escape into gil (btl_victory) */
} Unk800FDF30;
extern Unk800FDF30 g_btlRewards;



typedef struct {
    u8 pad0[0xC];
    s32 work; /* Unk80100C78Entry.work */
} Unk800DA1B88;

typedef struct Unk800DA1B8 {
    s16 stride; /* entry stride in bytes */
    s16 count; /* entry count */
    s32 (*elemFn)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32); /* per-entry step function (btlfx_poolUpdate) */
    Unk800DA1B88 *task; /* task owning the pool */
    s32 elems[1]; /* entries: s16 in use, s16, data */
} Unk800DA1B8;

extern s32 g_btlfxInCall;
extern u8 *g_btlfxReturnPc;
extern s16 g_btlfxWeaponScriptId;
extern u16 g_btlfxWeaponScripts[];
/* g_btlfxWeaponScripts: the hit script per weapon item id (btlfx_getUnitWeapon); ids from the count on use entry 0.
 * Evidence: [code] btlfx_getWeaponScript, [data] the table */
#define BTLFX_WEAPON_SCRIPT_COUNT 0x55
#define BTLFX_EFFECT_BLOOD_SWORD 0x16D /* g_btlfxWeaponScripts[0x13], [text items] "Blood Sword": btlfx_opApplyResult still shows the
                                   * caster's popups for it in Steiner's Attack (the HP drain) */

typedef struct LinkNode {
    struct LinkNode *next;
    u8 pad4[0x10 - 4];
    u32 flags; /* BtlCmd.info.w */
} LinkNode;
void btl_linkCmd(LinkNode *, LinkNode **);
EvtListNode *evt_getBattleObjects(void);
s16 btl_getEnemyModelId(s16);
void btl_resetUnit(struct BtlUnit *, struct ModelSlot *);
void btl_setupEnemies(void);
void btlseq_init(void);
extern u8 g_btlEnemySupport[];

typedef struct {
    u8 pad0[0x1C];
    s16 bounceMasks[4]; /* BtlUnit.bounceMasks */
} Unk800B44A0;


typedef struct {
    u8 flags; /* weapon table flags: BTL_WEAPON_REACH_X */
    u8 pad1[7];
    u16 reach; /* tip offset along y (or x) in the weapon bone's space */
    u16 offsetZ; /* tip offset along z */
} Unk800BC1E0;
/* Unk800BC1E0.flags. Evidence: [code] */
#define BTL_WEAPON_REACH_X 0x08 /* the reach is along the weapon bone's x (else y) (btl_getWeaponOffset) */

typedef struct {
    u8 pad0[0x33];
    u8 spirit; /* BtlUnit.spirit */
    u8 pad34[0x38 - 0x34];
    u8 chrStatus; /* Character.status (btl_saveCharStatus copies the unit's status & 0x7F there) */
    u8 pad39[0x3C - 0x39];
    u8 status; /* BtlUnit.status */
    u8 pad3D[0x66 - 0x3D];
    s16 countdownStep; /* BtlUnit.countdownStep (btl_initDoomCount) */
} Unk800C09D8;

typedef struct {
    u8 pad0[0x2E];
    u8 atbSpeed; /* BtlUnit.atbSpeed (btl_resetAtbSpeed) */
} Unk800BD8F0;


typedef struct Heap {
    u8 *cursor; /* next free byte */
    s32 free; /* bytes left before wrapping (btlfx_heapWrapIfFull) */
    s32 size;
    u8 data[1];
} Heap;

/* effect task (g_btlfxTasks, btlfx_spawnTask): an op handler called with mode 0 size query, 1 start, 2 step */
typedef struct {
    s32 frame; /* steps run */
    union {
        s32 w; /* cleared as a word */
        s16 h;
    } state; /* 0 free */
    s16 file; /* effect file slot the op came from */
    s16 sharedHeap; /* allocate from the effect file's heap (btlfx_allocTaskMem: else g_btlfxTaskHeap) */
    void *work; /* work data (size from mode 0) */
    struct Unk800DA1B8 *pool; /* element pool (btlfx_poolInit) */
    s32 (*handler)(s32, void *, Unk800F78A4 *, void *, void *);
} Unk80100C78Entry; /* 0x18 */

/* effect handler: (mode, work, camera, entry, args) */
typedef s32 (*EffFn)(s32, void *, Unk800F78A4 *, void *, void *);
extern EffFn *g_btlfxModuleOps[];
extern EffFn g_btlfxResidentOps[];
extern EffFn g_btlfxUnitOps[];

/* filled by an EffFn in mode 0 */
typedef struct {
    s32 workSize; /* work size */
    s32 poolCount; /* element pool entries (btlfx_poolInit) */
    s32 elemSize; /* element size */
    s32 padC[2];
    s32 elemFn; /* element step function */
} EffInit;

s32 btlfx_allocTaskMem(Unk80100C78Entry *, s32);
s32 btlfx_poolSize(s32, s32);
void btlfx_poolInit(struct Unk800DA1B8 *, Unk80100C78Entry *, s32, s32, s32);

typedef struct {
    u16 kind; /* BtlBgLayer.kind: every SprInfo is a BtlBgLayer (btlfx_drawLayerTexture's casts); btlfx_addSpriteQuad doesn't read it */
    u16 tpage; /* tpage (abr bits added) */
    u16 clut; /* clut */
    u16 pad6;
    u16 abr; /* abr, 0xFF = opaque */
} SprInfo; /* sprite texture info (btlfx_addSpriteQuad) */

void btlfx_addPrim(u32, s32, void *);

/* one layer of a layer table (BtlBgLayers): a scrolling / waving / fading 2D layer that btlfx_drawLayerTexture
 * draws into the table's VRAM rect (an off-screen texture, not the screen: the "Bg" names are older) */
typedef struct {
    u16 kind; /* kind: 0 scroll, 1 horizontal wave, 2 fade, 3 vertical wave */
    u16 tpage; /* tpage (SprInfo view) */
    u16 clut; /* clut */
    u8 u;  /* u */
    u8 v;  /* v */
    u16 abr; /* abr */
    u16 gradFrames; /* colour gradient period (btlfx_sampleGradient) */
    s32 gradOfs; /* colour gradient offset from the layer block, 0 = grey 0x80 */
    union {
        struct BtlBgScrollP {
            u8 sineX; /* x: sine motion */
            u8 sineY; /* y: sine motion */
            u8 pad2[2];
            s16 speedX; /* x speed */
            s16 speedY; /* y speed */
            u16 ampX; /* x amplitude */
            u16 ampY; /* y amplitude */
        } scroll;
        struct BtlBgWaveP {
            u16 stripSize; /* strip size */
            u16 stripCount; /* strip count */
            u16 phaseStep; /* phase step per strip */
            u16 phaseSpeed; /* phase step per frame */
            u16 amplitude; /* amplitude */
            s16 scrollSpeed; /* scroll speed */
        } wave;
        struct BtlBgFadeP {
            s32 dataOfs; /* sprite set offset from the layer block (btlfx_drawLayerSprite) */
            u8 pad4;
            u8 delay;  /* delay */
            u16 duration; /* fade-in frames */
            s16 x; /* sprite position */
            s16 y;
        } fade;
    } params; /* by kind */
} BtlBgLayer;

typedef struct {
    u16 count; /* layer count */
    u8 w;  /* width */
    u8 h;  /* height */
    u16 x; /* x */
    u16 y; /* y */
    s32 layers[1]; /* layer offsets */
} BtlBgLayers; /* layer table (effect data): btlfx_drawLayerTexture clears the VRAM rect (x, y, w, h) and draws the layers
                * into it; the effect's sprites / models then sample it as a texture. All 89 tables the modules pass
                * target x >= 0x180 (off screen). Evidence: [code] DR_AREA / DR_OFFSET from the header, [runtime]
                * docs/runtime-findings.md effect-data pass */


/* positive modulo: wt holds the dividend */
#define BG_WRAP(a, n)          \
    {                       \
        (a) = (a) / (n);    \
        if (wt < 0) {       \
            (a)--;          \
        }                   \
        (a) *= (n);         \
        (a) = wt - (a);     \
    }
/* colour gradient header (btlfx_sampleGradient), followed by 8-byte keys
   {CVECTOR color (cd = key length); u16 len; u16 start} terminated by cd == 0 */
typedef struct {
    s32 total; /* 0 until the keys' start/len are filled in */
    s32 count;
} Grad;
void btlfx_sampleGradient(u8 *, CVECTOR *, s32);
/* first u16 of a sprite set (the headers below, btlfx_getCommonSprite): bits 0-14 the frame count; bit 15 is masked off and
 * never tested in ovl_0b. Evidence: [code] */
#define BTLFX_SPRITE_FRAMES 0x7FFF
/* sprite set header (btlfx_drawAnimQuads, btlfx_drawLayerSprite): u16 frame offsets, then at
   frameOfs[count] a palette (u16 count, u16 entries[]) and per frame a byte count of
   3-byte sprites */
typedef struct {
    u16 frameCount; /* frame count (& BTLFX_SPRITE_FRAMES) */
    u16 tpage; /* tpage */
    u16 clut; /* clut */
    u8 h; /* height */
    u8 w; /* width */
    u16 frameOfs[1]; /* frame offsets */
} Unk800D1D74Hdr;
/* sprite set draw context (btlfx_drawAnimQuads, btlfx_drawSprites and their per-sprite workers) */
typedef struct {
    u16 *uvTab; /* palette / uv table */
    u16 *colTab; /* colour table */
    s32 halfW; /* from the width byte */
    s32 halfH; /* from the height byte */
    s32 tpage; /* tpage (abr bits added) */
    s32 clut; /* clut */
    s32 fade; /* fade */
    s32 count; /* sprite count */
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 code; /* primitive code */
} Unk800D3ED4Ctx;
void btlfx_emitAnimQuads(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitSprites(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitFixedSprites(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitDots(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
DR_TPAGE *btlfx_allocDrMode(s32, s32);
void btlfx_drawLayerSprite(u8 *, s32, s32, s32, s32, s32);


typedef struct {
    s32 count; /* running tasks */
    Unk80100C78Entry tasks[11];
} Unk80100C78;
extern Unk80100C78 g_btlfxTasks;
extern Heap g_btlfxTaskHeap; /* 0x1800 bytes */

typedef struct {
    s16 pages[6][2]; /* tpage/clut pairs (btl_applyModelTextures) */
} BtlTexInfo; /* 0x18, passed to btl_applyModelTextures */

typedef struct {
    BtlModel *model;
    BtlTexInfo tex; /* texture tpage/clut pairs (btl_applyModelTextures) */
    u8 used; /* in use */
    u8 slice; /* drawn sliced (Hi_SetEffModelSlice) */
    s16 id; /* index */
    s16 kind; /* HI_EFFMODEL_* */
    s16 otOffset; /* OT offset when drawn (Hi_SetEffModelOffset, btl_drawMeshOffset) */
    s16 sliceY; /* slice height (Hi_SetEffModelSlice) */
    u8 pad26[0x28 - 0x26];
} BtlObjSlot; /* 0x28 */

/* BtlObjSlot.kind, by the registering function's name. Evidence: [string] */
#define HI_EFFMODEL_SOLID 0 /* Hi_RegisterSolidEffModel */
#define HI_EFFMODEL_GOU   1 /* Hi_RegisterGouEffModel: drawn with btl_drawMeshOffset */
#define HI_EFFMODEL_TEX   2 /* Hi_RegisterTex*EffModel */
extern BtlObjSlot g_btlfxEffModels[32];


typedef struct {
    BtlModel *model;
    s16 tex[6][2]; /* texture tpage/clut pairs */
    s16 texPos[6][4]; /* texture positions (btl_relocTexAnims) */
    u8 used; /* in use */
    u8 texAnimOn; /* texture animations run (btl_updateTexAnims) */
    s16 id; /* index */
    u16 frame; /* motion frame */
    u8 pad52[0x54 - 0x52];
} Unk8010141C; /* 0x54 */
extern Unk8010141C g_btlfxSummonModels[1];


extern s32 g_btlfxDrawSyncState;
extern u16 g_btlfxKindTimings[][CHR_KIND_COUNT]; /* [timing][btlfx_getUnitKind] */
extern u8 g_btlMessages[];
extern char s_hiraishiErrorFmt[];
extern char s_hiFreeEffModelErr[];
extern char s_hiSplitMdlVertexErr[];
/* effect element (0x28) in an effect file, after the header: what a particle draws (btlfx_updateParticles) and how it
 * starts (btlfx_runEmitters, btlfx_runSubEmitter). Evidence: [code] */
typedef struct EffElem {
    u8 kind; /* draw kind: 0 btlfx_drawParticles, 1 btlfx_drawSprite, 2 btlfx_drawAnimQuads, 3 btlfx_drawSprites, 4/5/8 btlfx_drawTrail, 6 btlfx_drawRings, 7 btlfx_drawFan, 9 the draw callback (no texture) */
    u8 kind2; /* second draw kind with tex2, 0xFF = none */
    u8 blend; /* semi-transparency mode (abr = blend & 3), 0xFF = opaque; 0 and 0xFF become 1 (additive) once alpha <= 0x800 */
    u8 flags; /* BTLFX_ELEM_* */
    s16 frameMode; /* 1: the element frame advances every frame and the particle lives frameCount frames, 2: it loops over frameCount, else it stays */
    u8 fadeCurve; /* alpha over the particle's life unless BTLFX_ELEM_FIXED_ALPHA: 1 cos (fades out), 2 sin (fades in), 3 sin over half a turn (in and out) */
    u8 alignRot; /* nonzero: the launch direction is added to the start rotation */
    s16 frameCount; /* element frames (btlfx_opDrawOnUnit copies a sprite set's frame count here) */
    s16 rotStepX; /* element rotation added every frame */
    s16 rotStepY;
    s16 rotStepZ;
    u8 spinRange; /* random spin per particle (+-spinRange, x4 per frame) on the BTLFX_ELEM_SPIN_* axes */
    s8 baseRot[3]; /* start rotation (x16) */
    s8 spawnRotStep[3]; /* start rotation added per emitter frame at spawn (x8) */
    u8 rotSpread[3]; /* random start rotation (+-, x16) */
    u8 scaleStart; /* scale over the particle's life (x128, times the emitter's scale), unless BTLFX_ELEM_FIXED_SCALE */
    u8 scaleEnd;
    s32 tex; /* texture data offset, relocated on load */
    s32 tex2; /* second texture data, relocated on load */
    void (*draw)(); /* kind 9: set by the effect module; (particle, emitter, pos, rot, &scale, &alpha, &blend, frame, frames) */
} EffElem; /* 0x28 */
#define BTLFX_TEX_ADDR 0x80000000 /* EffElem.tex/tex2 already hold an address (btlfx_getCommonData, sprites): not relocated (btlfx_spawnEmitter). Evidence: [code] */

/* EffElem.flags (btlfx_updateParticles). Evidence: [code] */
#define BTLFX_ELEM_NO_ROT      0x01 /* no element rotation */
#define BTLFX_ELEM_FIXED_SCALE 0x02 /* scale = the emitter's scale (no scaleStart/scaleEnd) */
#define BTLFX_ELEM_SCREEN_ROT  0x04 /* the rotation is not combined with the view rotation (SVECTOR pad 0 for the draw functions) */
#define BTLFX_ELEM_FIXED_ALPHA 0x08 /* alpha = the emitter's alpha (no fadeCurve) */
#define BTLFX_ELEM_SPIN_X      0x10 /* the particle's spin turns the x rotation */
#define BTLFX_ELEM_SPIN_Y      0x20
#define BTLFX_ELEM_SPIN_Z      0x40

/* EffElem.kind / kind2 (btlfx_updateParticles). Evidence: [code] */
#define BTLFX_ELEMK_PARTICLES    0 /* btlfx_drawParticles */
#define BTLFX_ELEMK_SPRITE       1 /* btlfx_drawSprite */
#define BTLFX_ELEMK_ANIM_QUADS   2 /* btlfx_drawAnimQuads */
#define BTLFX_ELEMK_SPRITES      3 /* btlfx_drawSprites */
#define BTLFX_ELEMK_TRAIL_3D     4 /* btlfx_drawTrail mode 1 (btlfx_drawTrail3D, from the emitter origin) */
#define BTLFX_ELEMK_TRAIL_FLAT   5 /* btlfx_drawTrail mode 2 (btlfx_drawTrailFlat) */
#define BTLFX_ELEMK_RINGS        6 /* btlfx_drawRings */
#define BTLFX_ELEMK_FAN          7 /* btlfx_drawFan */
#define BTLFX_ELEMK_TRAIL_SCREEN 8 /* btlfx_drawTrail mode 0 (btlfx_drawTrailScreen) */
#define BTLFX_ELEMK_CALLBACK     9 /* EffElem.draw, no texture */
#define BTLFX_ELEMK_NONE         0xFF /* kind2 only: no second draw */

/* element table header, just before the EffElems (btlfx_spawnEmitter, btlfx_updateEmitters) */
typedef struct {
    s32 weightCount; /* count of weights */
    s16 elemCount; /* count of EffElem following */
    u16 scaleStart; /* emitter scale over its EffAnim duration (Pool6CEntry.scale, 0x1000 = 1.0) */
    u16 scaleEnd;
    u16 alphaStart; /* emitter alpha over its duration (Pool6CEntry.alpha, 0x1000 = opaque) */
    u16 alphaEnd;
    s16 weightAnim; /* 1 = blend weights animated */
    union {
        s32 w;
        u8 b[4];
    } weights;
} EffElemHdr; /* 0x14 */


/* emitter animation parameters (btlfx_updateEmitters lerps the *Start/*End pairs over duration into the
 * Pool6CEntry). Evidence: [code] */
typedef struct {
    s16 rateCurve; /* spawn rate (8.8 particles per frame) over the duration: 0 lerp rateStart..rateEnd, 1 sin * rateStart, 2 EffTail.rateTable, 3 one per frame */
    s16 duration; /* emitter frames */
    s16 rateStart;
    s16 rateEnd;
    s16 maxParticles; /* live particle limit */
    s16 stepSpread; /* random time step per particle (0x40 +-stepSpread, Pool30Entry.timeStep) */
    s16 accelStart[3]; /* particle acceleration (Pool6CEntry.accel) */
    s16 gravityStart; /* (Pool6CEntry.gravity) */
    s16 accelEnd[3];
    s16 gravityEnd;
    s16 fieldMode; /* BTLFX_FIELD_*: force field (btlfx_updateParticles) */
    s16 fieldCurve; /* BTLFX_FALLOFF_*: strength by distance */
    s16 fieldStart[2]; /* {range, strength} (Pool6CEntry.field) */
    s16 fieldEnd[2];
    SVECTOR rot; /* emitter rotation at frame 0 (Pool6CEntry.rot); pad = launch direction (BTLFX_LAUNCH_*, btlfx_runEmitters) */
    SVECTOR rotStep; /* added to the emitter rotation every frame; pad = launch direction jitter (+-) */
} EffAnim; /* 0x38 */

/* EffTail.shape: where an emitter spawns its particles and which way it launches them (btlfx_runEmitters); the
 * parameters are EffTail.s. Evidence: [code] */
#define BTLFX_SHAPE_MODULE   (-1) /* EffTail.s.module.spawnFn */
#define BTLFX_SHAPE_SPHERE   0 /* around the origin, launched outward (s.sphere) */
#define BTLFX_SHAPE_CYLINDER 1 /* vertical cylinder around the origin (s.cylinder, span = height) */
#define BTLFX_SHAPE_AXIS     2 /* cylinder around the emitter rotation axis (s.cylinder, span = distance along it) */
#define BTLFX_SHAPE_TUBE     3 /* cylinder around the line origin..target (s.cylinder, span = position on it, 0x1000 = target) */
#define BTLFX_SHAPE_CONE     4 /* cone along the emitter rotation axis (s.cone) */

/* emitter spawn shape and animation, after the elements (Pool6CEntry.tail). Evidence: [code] */
typedef struct {
    s32 shape; /* BTLFX_SHAPE_* */
    EffAnim anim;
    union {
        struct {
            s16 radiusMinStart; /* spawn distance range (Pool6CEntry.radiusMin / radiusMax), lerped over the duration */
            s16 radiusMinEnd;
            s16 radiusMaxStart;
            s16 radiusMaxEnd;
            CVECTOR axisScaleStart; /* x/y/z scale of the sphere (128 = 1.0), lerped to axisScaleEnd; cd 1/2: x angle limited to 0..0x800 / 0x800..0x1000 (one half of the sphere) */
            CVECTOR axisScaleEnd;
        } sphere; /* BTLFX_SHAPE_SPHERE */
        struct {
            s16 radiusStart[2]; /* {min, max} distance from the axis (Pool6CEntry.radiusMin / radiusMax) */
            s16 radiusEnd[2];
            s16 spanStart[2]; /* {min, max} (Pool6CEntry.shape.span) */
            s16 spanEnd[2];
            s16 dirMode; /* launch direction: 1 along the axis (shape 1: x angle 0x400), 2 the reverse, else outward */
        } cylinder; /* BTLFX_SHAPE_CYLINDER, BTLFX_SHAPE_AXIS, BTLFX_SHAPE_TUBE */
        struct {
            s16 radiusMinStart; /* radius at distance 0 along the axis (Pool6CEntry.radiusMin; both radii are scaled by one random 0..1, filling the cone) */
            s16 radiusMinEnd;
            s16 radiusMaxStart; /* radius at distance length (Pool6CEntry.radiusMax) */
            s16 radiusMaxEnd;
            s16 lengthStart; /* (Pool6CEntry.shape.cone.length) */
            s16 lengthEnd;
            s16 dirMode; /* launch direction: 1 along the axis, 2 the reverse, else along the cone's side */
        } cone; /* BTLFX_SHAPE_CONE */
        struct {
            s32 (*spawnFn)(SVECTOR *, SVECTOR *, s32, s32); /* (offset, direction, frame, duration), set by the effect module (NULL on spawn); nonzero ends the emitter; offset pad nonzero = absolute position */
            s16 rateTable[8]; /* as EffTail.rateTable */
        } module; /* BTLFX_SHAPE_MODULE */
    } s;
    s16 rateTable[8]; /* EffAnim.rateCurve 2: rate by sixteenth of the duration (BTLFX_SHAPE_MODULE: s.module.rateTable) */
} EffTail;
void LoadAverageCol(u8 *, u8 *, s32, s32, u8 *);

/* sub-emitter parameters, the 0x38-byte block in front of Pool6CEntry.sub: particles whose spawnFlags allow it spawn
 * sub-particles around themselves (btlfx_runSubEmitter); lerps over the parent particle's life. Evidence: [code] */
typedef struct {
    s16 rateStart; /* sub-particles per frame, 8.8 */
    s16 rateEnd;
    u8 elems[4]; /* element indices */
    u8 weights[4]; /* element weights */
    u8 rotMode;    /* BTLFX_LAUNCH_*: launch direction */
    u8 elemCount;    /* element count */
    s16 rotJitter; /* launch direction jitter (+-) */
    u8 maxParticles;   /* live sub-particle limit (Pool6CEntry.live.s.y) */
    u8 startFrame;   /* first spawn frame of the parent + 1 */
    s16 stepSpread; /* as EffAnim.stepSpread */
    s16 accelStart[3]; /* as EffAnim (Pool6CEntry.subAccel, subGravity, subField) */
    s16 gravityStart;
    s16 accelEnd[3];
    s16 gravityEnd;
    s16 fieldMode;
    s16 fieldCurve;
    s16 fieldStart[2];
    s16 fieldEnd[2];
    s16 radiusStart[2]; /* {min, max} spawn distance from the parent */
    s16 radiusEnd[2];
} EffSub; /* 0x38 */

/* EffEmit.kind: particle motion (btlfx_runEmitters / btlfx_runSubEmitter set a particle up, btlfx_updateParticles
 * moves it; the parameters from EffEmit+8 and the particle's Pool30Entry.motA / motB depend on it). Evidence: [code] */
#define BTLFX_MOTION_BALLISTIC 0 /* velocity, drag, gravity, optional floor (EffEmit.m.ballistic) */
#define BTLFX_MOTION_ORBIT     1 /* drawn at a radius from a moving centre, in a turning direction (m.orbit) */
#define BTLFX_MOTION_SPIRAL    2 /* spins around an axis with growing radius and axial distance (m.spiral) */
#define BTLFX_MOTION_WOBBLE    3 /* drawn at a radius in a sine-swinging direction (m.wobble) */
#define BTLFX_MOTION_HELIX     4 /* spins around an axis while moving along it, radius by curve (m.helix); the swirl around the caster while casting */
#define BTLFX_MOTION_HOMING    5 /* ballistic, steered toward Pool6CEntry.target (m.homing) */
#define BTLFX_MOTION_MODULE    0xFF /* EffEmit.m.module.update, set by the effect module; a 0x10-byte EffEmit */

/* fieldMode of EffAnim / EffSub: force field around Pool6CEntry.fieldPos, applied within range (fieldStart[0]) with
 * strength by fieldCurve (btlfx_updateParticles). Modes below BTLFX_FIELD_FADE move the particle, the others offset its
 * alpha. Evidence: [code] btlfx_updateParticles */
#define BTLFX_FIELD_NONE        0
#define BTLFX_FIELD_REPEL       1 /* pushed away from fieldPos */
#define BTLFX_FIELD_ATTRACT     2 /* pulled toward fieldPos */
#define BTLFX_FIELD_UP          3 /* pushed up (-y), horizontal distance */
#define BTLFX_FIELD_UP_2        4 /* the same code as BTLFX_FIELD_UP */
#define BTLFX_FIELD_ALONG_AXIS  5 /* pushed along fieldRot (btlfx_toLocalPolar distance) */
#define BTLFX_FIELD_AROUND_AXIS 6 /* pushed in the plane around the fieldRot axis, at the particle's polar angle */
#define BTLFX_FIELD_FADE        7 /* alpha + strength by 3D distance (strength from fieldStart[1], not lerped) */
#define BTLFX_FIELD_FADE_FLAT   8 /* the same by horizontal distance */
#define BTLFX_FIELD_FADE_POLAR  9 /* the same by btlfx_toLocalPolar distance */

/* fieldCurve: field strength by the distance fraction d (0 at fieldPos, a quarter turn at the range) */
#define BTLFX_FALLOFF_CONST   0
#define BTLFX_FALLOFF_COS     1 /* cos d: full at the centre, none at the range */
#define BTLFX_FALLOFF_SIN     2 /* sin d: none at the centre */
#define BTLFX_FALLOFF_INV_SIN 3 /* 1 - sin d */
#define BTLFX_FALLOFF_INV_COS 4 /* 1 - cos d */
#define BTLFX_FALLOFF_SIN2    5 /* sin 2d: strongest half-way */

/* launch direction of a new particle: EffAnim.rot.pad (btlfx_runEmitters, 0-3) and EffSub.rotMode
 * (btlfx_runSubEmitter, 0-5); every mode but BTLFX_LAUNCH_RANDOM adds +-rotJitter. Evidence: [code] the two switches */
#define BTLFX_LAUNCH_SHAPE           0 /* the spawn shape's direction (the sub-emitter's random spawn offset) */
#define BTLFX_LAUNCH_SHAPE_FLIPPED   1 /* the same with x turned half a turn */
#define BTLFX_LAUNCH_RANDOM          2 /* btlfx_randRotation */
#define BTLFX_LAUNCH_EMITTER         3 /* the emitter's rotation */
#define BTLFX_LAUNCH_PARENT          4 /* sub-particles: the parent particle's direction of travel this frame */
#define BTLFX_LAUNCH_PARENT_REVERSED 5 /* the same with x turned half a turn */

/* radiusCurve of EffEmit.m.orbit / wobble / helix: the particle's radius over its life t */
#define BTLFX_RADIUS_SIN    0 /* radius * sin(lerp(sinStart, sinEnd, t)) */
#define BTLFX_RADIUS_GROW   1 /* radius * t */
#define BTLFX_RADIUS_SHRINK 2 /* radius * (1 - t) */

/* particle motion parameters of an emitter (Pool6CEntry.file: the effect file starts with it) or sub-emitter
 * (Pool6CEntry.sub). Start/End pairs are lerped over the emitter's life (a sub-emitter's over the parent particle's;
 * an emitter's partly through Pool6CEntry.cur), *Spread is a random +- per particle. 0x28 bytes, 0x10 for
 * BTLFX_MOTION_MODULE. Angles: 0x1000 = 360 degrees. Evidence: [code] */
typedef struct {
    u8 kind; /* BTLFX_MOTION_* (runtime round 33: 4 = the party cast emitter, the sparkle swirl winding up around the
              * caster while casting) */
    u8 emitsSub; /* nonzero: a sub-emitter (EffSub, then its EffEmit) follows (btlfx_spawnEmitter) and the particles spawn sub-particles (Pool30Entry.spawnFlags) */
    s16 lifeStart; /* particle lifetime in frames, +-lifeSpread */
    s16 lifeEnd;
    s16 lifeSpread;
    union {
        struct {
            s16 speedStart; /* launch speed along the launch direction (Pool30Entry.motA.vel) */
            s16 speedEnd;
            s16 speedSpread;
            s16 drag; /* velocity factor per frame, 0x1000 = none (Pool30Entry.motB.ballistic.drag) */
            s16 dragSpread;
            s16 floorMode; /* 0 none, 1 bounce off the floor (Pool6CEntry.rot.pad), else slow down above it, stop on it */
            s16 floorMargin; /* height above the floor where floorMode acts */
            s16 bounce; /* floorMode 1: vertical speed kept, 0x100 = 1.0 */
            s16 jitter; /* nonzero: velocity += jitterX/Y/Z random +- every frame */
            s16 jitterX;
            s16 jitterY;
            s16 jitterZ;
        } ballistic; /* BTLFX_MOTION_BALLISTIC */
        struct {
            s16 turnX; /* direction x angle step per frame (Pool30Entry.motB.orbit.turnX) */
            s16 turnY;
            s16 turnXSpread;
            s16 turnYSpread;
            s16 drag; /* of the centre, as ballistic (Pool30Entry.motB.orbit.drag) */
            s16 dragSpread;
            s16 radiusCurve; /* BTLFX_RADIUS_* */
            s16 radiusSpread;
            s16 radiusStart; /* distance from the centre (Pool6CEntry.cur.orbit.radius) */
            s16 radiusEnd;
            s16 sinStart; /* BTLFX_RADIUS_SIN: angle over the particle's life */
            s16 sinEnd;
            s16 speedStart; /* launch speed of the centre (Pool6CEntry.cur.orbit.speed) */
            s16 speedEnd;
            s16 speedSpread;
        } orbit; /* BTLFX_MOTION_ORBIT */
        struct {
            s16 axialSpeedStart; /* distance along the axis per frame (Pool6CEntry.cur.spiral.axialSpeed) */
            s16 axialSpeedEnd;
            s16 axialSpeedSpread;
            s16 spinSpeedSpread;
            s16 radiusSpeedStart; /* radius growth per frame (btlfx_lerpVec pairs it with radiusSpeedEnd) */
            s16 spinSpeedStart; /* angle around the axis per frame (paired with spinSpeedEnd) */
            s16 radiusSpeedEnd;
            s16 spinSpeedEnd;
            s16 radiusSpeedSpread;
        } spiral; /* BTLFX_MOTION_SPIRAL */
        struct {
            s16 swingXSpeed; /* sine phase step of the x angle swing per frame */
            s16 swingYSpeed;
            s16 swingXSpread;
            s16 swingYSpread;
            s16 dirXSpread; /* random +- on the swing centre (the launch direction) */
            s16 dirYSpread;
            s16 radiusCurve; /* BTLFX_RADIUS_* */
            s16 radiusSpread;
            s16 radiusStart; /* (Pool6CEntry.cur.wobble.radius) */
            s16 radiusEnd;
            s16 sinStart;
            s16 sinEnd;
        } wobble; /* BTLFX_MOTION_WOBBLE */
        struct {
            s16 axialSpeedStart; /* (Pool6CEntry.cur.spiral.axialSpeed) */
            s16 axialSpeedEnd;
            s16 axialSpeedSpread;
            s16 radiusCurve; /* BTLFX_RADIUS_* */
            s16 radiusStart; /* (btlfx_lerpVec pairs it with radiusEnd) */
            s16 spinSpeedStart; /* (paired with spinSpeedEnd) */
            s16 radiusEnd;
            s16 spinSpeedEnd;
            s16 radiusSpread;
            s16 spinSpeedSpread;
            s16 sinStart;
            s16 sinEnd;
        } helix; /* BTLFX_MOTION_HELIX */
        struct {
            s16 speedStart; /* as ballistic */
            s16 speedEnd;
            s16 speedSpread;
            s16 drag;
            s16 dragSpread;
            s16 steerStart; /* speed added toward the target every frame */
            s16 steerEnd;
            s16 pad16; /* unused */
            s16 jitter; /* as ballistic */
            s16 jitterX;
            s16 jitterY;
            s16 jitterZ;
            s16 aimX; /* added to the angles toward the target */
            s16 aimY;
        } homing; /* BTLFX_MOTION_HOMING */
        struct {
            s32 (*update)(); /* (particle, emitter, &pos, trail, frame, frames), nonzero frees the particle */
        } module; /* BTLFX_MOTION_MODULE */
    } m;
} EffEmit;

typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } s;
} XY32;

typedef struct Pool6CEntry {
    EffEmit *file; /* effect file: starts with the particle motion parameters */
    EffSub *sub; /* the sub-emitter's EffEmit (motion parameters), NULL = none; its EffSub block is just before it */
    union {
        struct {
            s16 speed;
        } ballistic; /* BTLFX_MOTION_BALLISTIC, BTLFX_MOTION_HOMING */
        struct {
            s16 radius;
            s16 speed;
        } orbit;
        struct {
            s16 axialSpeed;
        } spiral; /* BTLFX_MOTION_SPIRAL, BTLFX_MOTION_HELIX */
        struct {
            s16 radius;
        } wobble;
    } cur; /* the file's EffEmit Start..End pairs at the current frame (btlfx_updateEmitters), by file->kind */
    s16 trailStride; /* bytes per particle in trails */
    s16 spawnCount; /* particles spawned (modulo trailSlots when trails is set): the next one's slot - 1 */
    s16 subSpawnCount; /* the same for sub-particles */
    s16 trailSlots; /* particles in trails */
    s32 trails; /* trail point buffer set by the effect module, trailSlots * trailStride bytes, 0 = none (btlfx_getParticleTrail) */
    EffElem *elems; /* elements (btlfx_spawnEmitter relocates their textures) */
    s32 scale; /* particle scale, 0x1000 = 1.0 (EffElemHdr.scaleStart..End; effect modules set it) */
    s16 alpha; /* particle alpha, 0x1000 = opaque (EffElemHdr.alphaStart..End) */
    s16 otZ; /* otZ of its particles (btlfx_updateParticles); btlfx_opDrawOnUnit / btlfx_startCastEmitter set it */
    EffTail *tail;
    SVECTOR *origin; /* emitter position (btlfx_moveEmitter shifts its particles with it) */
    s32 target; /* SVECTOR *: end point of shape 3 and the aim of motion kind 5 (starts as origin) */
    s16 state; /* BTLFX_EMIT_* */
    s16 spawnNow; /* particles to spawn this frame (btlfx_updateEmitters, from the spawn rate) */
    XY32 live; /* live particle counts: s.x particles, s.y sub-particles */
    s16 frame; /* frame */
    s16 spawnFrac; /* fraction of the spawn rate carried to the next frame (/0x100) */
    s16 radiusMin; /* spawn distance range of the shape (EffTail.s, lerped by btlfx_updateEmitters) */
    s16 radiusMax;
    union {
        struct {
            s16 min, max;
        } span; /* BTLFX_SHAPE_CYLINDER: height, AXIS: distance along the axis, TUBE: position on origin..target */
        struct {
            s16 length;
        } cone; /* BTLFX_SHAPE_CONE: length along the axis */
    } shape; /* by EffTail.shape */
    s16 accel[3]; /* particle acceleration (EffAnim.accelStart..End) */
    s16 gravity; /* added to the particles' y velocity (EffAnim.gravityStart..End) */
    XY32 field; /* force field {range, strength} (EffAnim.fieldStart..End) */
    s16 subAccel[3]; /* the same for sub-particles (EffSub) */
    s16 subGravity;
    XY32 subField;
    SVECTOR rot; /* emitter rotation (from its EffAnim, or set every frame by the ops: btlfx_opBounce, btlfx_opDrawOnUnit); pad = floor height of the particles */
    SVECTOR *fieldPos; /* centre of the force-field modes 1-9 (btlfx_updateParticles) */
    SVECTOR *fieldRot; /* angles of the force field (modes 5, 6, 9: btlfx_toLocalPolar) */
} Pool6CEntry; /* 0x6C */

/* Pool6CEntry.state (btlfx_spawnEmitter, the emitter updates). Evidence: [code] */
#define BTLFX_EMIT_FREE 0
#define BTLFX_EMIT_LIVE 1
#define BTLFX_EMIT_DONE (-1) /* finished: freed by the next update; effect code polls it to drop its pointer */

typedef struct {
    s32 capacity; /* entries */
    s32 live; /* live emitters (btlfx_updateEmitters) */
    s32 maxUsed; /* highest index in use */
    Pool6CEntry *entries;
    Pool6CEntry buf[1];
} Pool6C;

typedef struct {
    struct {
        s32 vx, vy, vz;
    } pos; /* position, 20.12 */
    union {
        struct {
            s16 x, y, z;
        } vel; /* BTLFX_MOTION_BALLISTIC, ORBIT (the centre's), HOMING: velocity (btlfx_stepParticle*: pos += vel * 0x40) */
        struct {
            s16 angle; /* around the axis (motB.spiral.axis / helix.axis) */
            s16 speed; /* added to angle every frame */
            s16 fall; /* the centre's vertical speed, from gravity (btlfx_stepParticleNoDrag) */
        } spin; /* BTLFX_MOTION_SPIRAL, BTLFX_MOTION_HELIX */
        struct {
            s16 phaseX; /* sine phases of the direction swing (x, y angle) */
            s16 phaseY;
            s16 radius;
        } wobble; /* BTLFX_MOTION_WOBBLE */
    } motA; /* motion state by the emitter's EffEmit.kind; the effect modules' motions use it as they like */
    u8 spawnFlags; /* BTLFX_PART_SUB, bits 0-6: sub-particle spawn fraction (nonzero = spawns sub-particles) */
    s8 spin; /* random spin (EffElem.spinRange) */
    XY32 elemRot; /* element rotation x, y (z in elemRotZ), passed to the draw functions */
    s16 elemRotZ;
    u8 frame; /* frame */
    u8 frames; /* lifetime in frames */
    u8 elem; /* element index, 0xFF = none */
    u8 elemFrame; /* element frame */
    s8 timeStep; /* time step of the motion (btlfx_stepParticle*) */
    u8 slot; /* 0 = free; else the emitter's spawn count % 255 + 1: its trail slot + 1 (btlfx_getParticleTrail), effect modules index their tables with slot - 1 */
    union {
        struct {
            s16 drag; /* velocity factor per frame, 0x1000 = none */
        } ballistic; /* BTLFX_MOTION_BALLISTIC, BTLFX_MOTION_HOMING */
        struct {
            s16 turnX; /* added to dir every frame */
            s16 turnY;
            XY32 dir; /* direction x, y angles of the particle from the centre */
            s16 radius;
            s16 drag;
        } orbit; /* BTLFX_MOTION_ORBIT */
        struct {
            s16 axial; /* distance along the axis, 4 = 1 */
            s16 axialSpeed;
            s16 radius; /* 4 = 1 */
            s16 radiusSpeed;
            XY32 axis; /* x, y angles of the axis (the launch direction) */
        } spiral; /* BTLFX_MOTION_SPIRAL */
        struct {
            s16 swingXSpeed; /* added to motA.wobble.phaseX every frame */
            s16 swingYSpeed;
            s16 fall; /* the centre's vertical speed, from gravity (btlfx_stepParticleNoDragAlt) */
            s16 dirX; /* swing centre: direction x, y angles */
            s16 dirY;
        } wobble; /* BTLFX_MOTION_WOBBLE */
        struct {
            s16 radius;
            s16 axialSpeed;
            s32 axial; /* distance along the axis, 0x40 = 1 */
            XY32 axis;
        } helix; /* BTLFX_MOTION_HELIX */
    } motB; /* motion state by EffEmit.kind, continued */
    Pool6CEntry *owner; /* emitter (btlfx_killEmitter, btlfx_moveEmitter) */
} Pool30Entry; /* 0x30 */

/* Pool30Entry.spawnFlags. Evidence: [code] */
#define BTLFX_PART_SUB      0x80 /* spawned by the sub-emitter (counts in live.s.y, moves with the EffSub parameters) */
#define BTLFX_PART_SUB_FRAC 0x7F

typedef struct {
    s32 capacity; /* entries in the buffer */
    s32 live; /* live particles (btlfx_updateParticles) */
    s32 maxUsed; /* highest index in use */
    Pool30Entry *entries;
} Pool30;

extern Pool30Entry g_btlfxParticleBuf[0x100];
extern Pool30 g_btlfxParticles;
extern Pool6C g_btlfxEmitters; /* 0x920 bytes */
/* g_btlfxResArgs: argument bytes of resident ops 0x20 (mode, data) and 0x26 (offX, offY) for btlfx_opDrawOnUnit.
 * Evidence: [code] */
typedef struct {
    u8 mode; /* bits 6-7: kind (btlfx_opDrawOnUnit); bits 0-3: g_btlfxDrawScales index (kind 2: frames) */
    u8 data; /* kinds 0, 1: common sprite id; kind 2: emitter id; kind 3: frames */
    u8 offX; /* kind 0: screen offset / 4 (s8) */
    u8 offY;
} BtlfxDrawArgs;
extern BtlfxDrawArgs g_btlfxResArgs;
/* g_btlfxResArgs2: argument bytes of resident op 0x22 for btlfx_opDrawOnUnit. Evidence: [code] */
typedef struct {
    u8 unit; /* unit drawn on, 0xFF = the target centre */
    u8 otBias; /* / 8 */
    u8 pad2[2];
} BtlfxDrawArgs2;
extern BtlfxDrawArgs2 g_btlfxResArgs2;
extern s32 g_btlfxSavedGeomScreen;
extern MATRIX g_btlfxSavedView;
extern u16 g_btlfxCastBgBrightness[];
extern Unk800F6A54 g_btlTextPopups[];
extern char s_hiSetSummonMotFrameErr[];

typedef struct {
    u8 pad0[0xE];
    s16 power; /* BtlCalc.power (btl_applyInvertedPower: 60 - power with scene flag bit 10) */
} Unk800BA938;

typedef struct {
    u8 pad0[0x8C];
    s32 pos[3]; /* BtlUnit.pos */
    s32 *home; /* BtlUnit.home */
} Unk800CB090;


extern RECT g_btlClutRect;
extern u16 g_btlClutBuf[][0x100]; /* CLUT rows */
extern u32 *g_btlSavedCluts;


extern u8 g_btlfxSoundSets[][4];
extern char s_hiGetMdlVertexPtrErr[];

typedef struct Unk800D8EA4 {
    u8 unit; /* unit id */
    u8 motion; /* motion slot */
    u8 trimFrames; /* the hold variants end this many frames before the motion's end (op argument) */
    u8 pad3;
    s32 lastFrame; /* frame the step ends at (motion length - 1, or minus trimFrames) */
} Unk800D8EA4;

typedef struct Unk800D7E14 {
    u8 unit; /* unit id moved */
    u8 point; /* destination kind (btlfx_stepMoveToPoint) */
    u8 pad2[2];
    s32 frames;
    s16 from[4]; /* start position */
} Unk800D7E14;

/* battle background header: DB item 0x1Axxxx of the background file (docs/battle-backgrounds.md) */
typedef struct BtlBgHeader {
    s16 id; /* scene id (g_btlbgId), not the file id; btlbg_draw moves the groups after the first for 7, 0x44,
             * 0x6E, 0x70, 0xA8, 0xAB */
    s16 texAnimCount; /* count of g_btlbgTexAnims entries (DB item 0x19xxxx) */
    s16 skySpeed; /* added to g_btlbgSkyAngle every frame (layer 8 records turn by it >> 3) */
    s16 skyOwnProj; /* layer 8 records are drawn with their own projection (screen 0x200, fixed distance) */
    s16 lastGroup; /* last group index */
    s16 scrollCount; /* BtlBgScroll entries after the header */
    u8 lightR; /* light colour the units take (btl_applyBgLight, btl_setUnitFade) */
    u8 lightG;
    u8 lightB;
    u8 shadowShade; /* shadow grey level (BtlShadow.shade) */
    BtlBgScroll scrolls[1]; /* [scrollCount] (g_btlbgScrolls; their step bytes change at run time) */
} BtlBgHeader;
/* BtlBgHeader.id scene ids whose extra groups btlbg_draw moves by code. Named by the battle and place: [data]
 * tools/encounter_catalog.py / battle_models.py give the background file, the encounters and the field that starts
 * them (field names from the field scripts); [runtime] R41b read g_btlbgId in each of those battles. The motion in
 * each comment is from the code (btlbg_draw); which on-screen part moves was not isolated at runtime. */
#define BTLBG_SCENE_NOVA_DRAGON      7    /* sky, no floor (file 160; only encounter 931 Nova Dragon, field 2756 "Red Rose/Bridge"): groups 1-2 jump to random y angles */
#define BTLBG_SCENE_FOSSIL_ROO       0x44 /* stone tunnel (file 32: Armodullahan, Abomination, Griffin; field 1423 "Fossil Roo/Passage"): group 1 swings about z */
#define BTLBG_SCENE_MEMORIA_STAIRS   0x6E /* stone colonnade (file 157: Ash, Veteran; field 2902 "Memoria/Stairs of Time"): group 1 sways about z, tilted */
#define BTLBG_SCENE_MEMORIA_OBLIVION 0x70 /* tall hall (file 156: Ash, Veteran; field 2906 "Memoria/Oblivion"): groups 1-3 turn and bob */
#define BTLBG_SCENE_MEMORIA_ORIGIN   0xA8 /* void with a red crystal mass (file 172: Hades, Stilva, Behemoth; fields 2914 "Memoria/Birth",
                                           * 2921 "Memoria/To the Origin"): groups turn slowly about y */
#define BTLBG_SCENE_CRYSTAL_WORLD    0xAB /* platform in space (file 162: Trance Kuja, field 2927 "Crystal World/Origin"): group 1 rolls
                                           * about z, group 2 spins tilted */


typedef struct {
    u16 sound; /* sound id, 0 = free */
    s16 age; /* age */
    s32 unit; /* unit id (pan source, btl_soundPan) */
} Unk80104E30Entry;

typedef struct {
    s32 count; /* playing sounds */
    Unk80104E30Entry slots[6];
} Unk80104E30;
extern Unk80104E30 g_btlfxSounds;

void btlbg_init(struct BtlBgHeader *, BtlUnit9C50 *);
void btl_initCmdQueue(struct Battle *);
void btl_initTargetCursor(struct Battle *);
void btl_initSceneFlags(void);
s16 btl_loadModelsStep(s32);

extern char g_btlfxErrorMsg[];

/* summon model header ('so' magic) */
typedef struct {
    u16 tpage;
    u16 clut; /* clut (position table: y) */
} Unk800E9000Ent;

typedef struct {
    u16 magic; /* BTLFX_SUMMON_MAGIC */
    u16 unk2; /* not read */
    u16 modelOfs; /* model header offset; (modelOfs - 8) / 8 texture entries */
    u16 texPosOfs; /* texture position table offset */
    Unk800E9000Ent tex[1]; /* texture entries */
} Unk800E9000;
#define BTLFX_SUMMON_MAGIC 0x6F73 /* 'so' (btlfx_registerSummonModel and its caller check it) */

typedef struct Unk80100DC054 {
    s16 dataOfs; /* offset of the model data */
    s16 motionCount; /* count of motions */
    s16 texCount; /* count of the texture tables */
    u16 clutRows; /* CLUT rows loaded at y 0xE6 */
    s32 clutOfs; /* CLUT data offset from the model data */
    u8 padC[0x10 - 0xC];
    s32 workOfs; /* work area offset (work area: 0x50000 - workOfs bytes) */
    s32 texAnimOfs; /* texture animation offset */
    u16 texTpage[6]; /* per texture: tpage (TPAGE_X_MASK / TPAGE_Y_BIT) */
    u16 texClut[6]; /* per texture: clut */
    u16 texY[6]; /* per texture: y in its page */
    u8 *modelHdr; /* model header */
    BtlUnit9C50 *texAnims;
    u8 *work; /* work area (g_btlfxLoader.workCursor) */
    s32 workSize;
    s32 unk4C; /* write-only (cleared when the summon textures are loaded, btlfx_updateLoader) */
    s32 *data[16]; /* file data tables (btlfx_getFileData 0x80-0xBF) */
    BtlModelBuf model;
    u8 pad128[0x180 - 0x128];
    s32 motions[1];
} Unk80100DC054;

typedef struct {
    u16 sound; /* sound id (btlfx_findSound) */
    u8 pad2[2];
    s32 unit; /* unit id (pan source) */
} Unk80104E34; /* 8 */
extern Unk80104E34 g_btlfxSoundSlots[6];

extern u16 g_btlSoundBankIds[];
extern u8 g_btlfxUnitKinds[];
extern s32 g_btlfxCameraOn;
extern u8 g_btlfxCmdCameras[5]; /* battle message ids */
BtlUnit *btlfx_getFirstTarget(void);
extern u8 g_btlfxBounceCameraParty;
extern u8 g_btlfxBounceCameraEnemy;
extern char g_battleText[];
/* g_battleText entries (text_get; decoded with tools/ff9text.py text_table) */
#define BTL_TEXT_ESCAPE      4    /* "Escape": help text while escape is held */
#define BTL_TEXT_TARGET      5    /* + targetType: "Single\n" .. "Self\n" */
#define BTL_TEXT_TARGET_SM   0x13 /* + k: single/multi switch lines, S highlighted (0x16 + k: M highlighted) */
#define BTL_TEXT_TARGET_MS   0x16
#define BTL_TEXT_HELP_CMD    0x19 /* command menu keys: Change, Defend, Skip */
#define BTL_TEXT_HELP_TARGET 0x1A /* "Target Window" key line */

extern u8 g_btlAbilityNames[];
extern void (*g_btluiMenuUpdate[])(void);


typedef struct {
    u8 open; /* help window shown (btlui_toggleHelp) */
    u8 opening; /* sliding in (unkC frames) */
    u8 unk2; /* write-only (cleared by btlui_initHelp) */
    u8 closeReq; /* btlui_checkHelpClose closes it next frame */
    s32 text; /* help text (btlui_updateHelpText) */
    u8 pad8[0xC - 8];
    s32 frame; /* frames since it opened (slide-in: 7) */
} Unk80104F90;
extern volatile Unk80104F90 g_btluiHelp;
extern u8 g_btluiPrevAtbOn;

struct Unk800F412C;
typedef struct Unk800F3F10 {
    s16 dx; /* per frame */
    s16 dy; /* per frame */
    s32 frames; /* frames */
    void (*onDone)(struct Unk800F412C *); /* called when the slide ends */
} Unk800F3F10;

/* moving menu sprite */
typedef struct Unk800F412C {
    s16 text; /* label index into g_winLabelStrings */
    s16 align; /* 1 = right-aligned (btlui_alignPanelLabel) */
    u8 visible;
    u8 unk5; /* unused */
    u16 frame; /* slide frame */
    u16 x; /* x */
    u16 y; /* y */
    Unk800F3F10 *slide; /* running slide, NULL when still */
} Unk800F412C; /* 0x10 */

typedef struct {
    u8 shown; /* panel drawn (btlui_showPanel) */
    u8 unk1; /* write-only (cleared by btlui_updatePanel) */
    u8 highlightRow; /* row of the menu unit, 0xFF none (btlui_findHighlightRow) */
    u8 pad3;
    Unk800F412C labels[5]; /* header labels with their slides */
    u32 pulse; /* highlight pulse 0..0x1F (btlui_pulsePanelHighlight) */
    s32 returnState; /* menu state btlui_hideListHeader restores */
    u32 rowCount; /* party rows */
    s32 y; /* panel top */
    BtlUnit *menuUnit; /* unit whose row is highlighted */
    BtlUnit *units[4]; /* unit per row */
    s8 rowColor[4]; /* ui_drawDigits style per row, also g_btluiRowCluts index (btlui_updatePanelRowColor: DIGITS_RED KO, DIGITS_YELLOW low HP) */
    u8 cantAct[4]; /* per row: the unit can't act (gauge greyed) */
    u8 slot[4]; /* per row: unit slot id */
    u8 atbKind[4]; /* per row: 0 normal, 2 slow/stop, 3 haste (gauge colour) */
    u8 tranceKind[4]; /* per row: 0 no trance gauge, 1 gauge, 2 in trance */
} Unk80105078;
extern Unk80105078 *g_btluiPanel;
/* battle party panel layout (btlui_drawPanel*, the HP/MP/status list windows): one row per party member, name | HP |
 * MP | ATB gauge (Trance gauge under it), the header labels above (g_btluiPanelLabels). Evidence: [code] */
#define BTLUI_PANEL_ROW_H    12   /* row pitch: panel rows, the list windows' rows (h = rows * 12 + 8) */
#define BTLUI_PANEL_X        0x8E /* left edge of the header rule and its clip */
#define BTLUI_PANEL_HP_X     0xD0 /* HP digits (btlui_drawPanelHp) */
#define BTLUI_PANEL_MP_X     0xEF /* MP digits (btlui_drawPanelMp) */
#define BTLUI_PANEL_ATB_X    0x108 /* ATB gauge */
#define BTLUI_PANEL_TRANCE_X 0x10C /* Trance gauge, 4 px under the ATB gauge */
#define BTLUI_STATUS_ICON_W  12   /* btlui_drawStatusList: icon pitch */
#define BTLUI_STATUS_ICONS_MAX 9  /* btlui_drawStatusList: icons shown per member */
/* Unk80105078.tranceKind ([runtime] R44: a gauge for every member, 2 for Eiko in Trance) */
#define BTLUI_TRANCE_NONE    0 /* no Trance gauge: ATB gauge centred in the row */
#define BTLUI_TRANCE_GAUGE   1 /* Trance gauge under the ATB gauge */
#define BTLUI_TRANCE_ACTIVE  2 /* in Trance: the Trance gauge drawn first, in gauge style 1 */
extern s32 g_btluiPanelY[];
extern Unk800F3F10 g_btluiListHideSlide;
extern Unk800F3F10 g_btluiListHideTitleSlide;

extern void *g_btluiHelpText;
extern s32 (*g_btluiMenuHelp[])(void);

typedef struct {
    u8 ready; /* the slide finished: confirm runs it */
    u8 side; /* 0 none, 1 right (Defend, command 4), 2 left (Change, command 7) */
    u8 slide; /* 1 sliding out (btlui_slideLeft/Right), 2 sliding back (btlui_slideBack) */
    u8 slideFrame; /* 0-5 */
} Unk80104FC0; /* g_btlCmdMenu.sideCmd */
/* Unk80104FC0.side / .slide */
#define BTLUI_SIDE_NONE  0
#define BTLUI_SIDE_RIGHT 1 /* Defend (Right on the command window) */
#define BTLUI_SIDE_LEFT  2 /* Change (Left) */
#define BTLUI_SLIDE_OUT  1
#define BTLUI_SLIDE_BACK 2
#define BTLUI_SIDE_SLIDE_FRAMES 5 /* frames of the slide out (3 px a frame), then ready */
#define BTLUI_SIDE_BACK_FRAMES  3 /* btlui_slideBack: the box is gone after frame 3 (5 px a frame) */
/* battle command window and the ability / item lists (btlui_drawCmdList, btlui_drawCmdOverlay, btlui_drawSideCmd,
 * the list windows) */
#define BTLUI_LIST_ROW_H 15   /* row pitch: command rows, the hand cursor, ability / item rows */
#define BTLUI_CMD_TEXT_X 0xA  /* command names: x inside the window */
#define BTLUI_CMD_TEXT_Y 6    /* first command name's y inside the window */
#define BTLUI_SIDE_W     0x3E /* Defend / Change box beside the cursor row */
#define BTLUI_SIDE_H     0x11
/* target window (btlui_drawTargetList, btlui_drawTargetListOverlay): enemy column left, party column right of an
 * etched divider, rows BTLUI_LIST_ROW_H apart, a hand left of each targeted name */
#define BTLUI_TGT_DIVIDER_X  0x6E /* the etched line between the columns */
#define BTLUI_TGT_ENEMY_X    0xC  /* enemy names */
#define BTLUI_TGT_PARTY_X    0x7A /* party names */
#define BTLUI_TGT_ENEMY_LABEL_X 8    /* "Enemy" label on the top edge */
#define BTLUI_TGT_PARTY_LABEL_X 0x76 /* "Player" label */
#define BTLUI_TGT_ENEMY_HAND_X (-0xC) /* hand cursor, from the open rect's x */
#define BTLUI_TGT_PARTY_HAND_X 0x62

/* battle command menu (runtime: Vivi's menu -> Eiko's, docs/runtime-findings.md) */
typedef struct {
    u8 cursor;    /* row: 0 Attack, 1, 2, 3 Item */
    u8 trance; /* the unit is in trance (trance commands; btlui_buildCmdList) */
    u8 charIndex; /* GameData.characters index of the commanding member */
    u8 pad3;
    u8 cmdIds[4]; /* command per row: Attack 1, Blk Mag 22, Focus 13, Wht Mag 19, Summon 20, Item 14 */
    u8 enabled[4]; /* per row: selectable (else drawn grey, btlui_confirmCmd refuses) */
    u8 tranceRow[4]; /* per row: the trance command replacing the normal one (g_btlCharTranceCmds[2]) */
    Unk80104FC0 sideCmd; /* Defend (right) / Change (left) slid out of the menu */
    u8 pad14[0x18 - 0x14];
    BtlUnit *unit; /* the unit choosing a command */
} Unk80104FB0;
extern Unk80104FB0 g_btlCmdMenu;
extern Unk80104FC0 g_btlSideCmd; /* g_btlCmdMenu.sideCmd, also addressed by its own label */
extern void (*g_btluiMenuInput[])(void);



typedef struct Unk80104FD4 {
    u8 pad0[6];
    u8 kind; /* Unk800FD56C.kind */
    u8 count; /* count */
    s32 first;
} Unk80104FD4;
extern s32 (*g_btluiListHelpFns[])(void);
extern void (*g_btluiListRowFns[])(s32, s32, s32, MenuView *);

typedef struct {
    u8 returnState; /* menu state btlui_cancelTarget returns to */
    u8 cmdNo;
    u8 targetType; /* target type of the ability (g_btluiTargetAllByType / g_btluiDefaultTargets by type; 0xC all, 0xD self) */
    u8 defaultSide; /* side the cursor starts on (ability) */
    u8 koFirst; /* the cursor starts on a KO unit (btlui_setDefaultTargets) */
    u8 unk5; /* unused */
    u8 targetAll; /* the whole side is marked */
    u8 unk7; /* write-only: target types 6-8 (btlui_startTargetSelect) */
    u8 canSwitchSide; /* target types 0, 3, 6, 9: left/right moves to the other side (btlui_switchTargetSide) */
    u8 canToggleAll; /* target types 3-5: single/all toggles (btlui_toggleTargetAll) */
    u8 side; /* side the cursor is on (unit flag bit 0) */
    u8 allowKo; /* KO units can be targeted */
    u8 windowOpen; /* target-name window open (btlui_openTargetWindow) */
    u8 halfMp; /* the caster has support ability 0xE2 (MP costs halved, btlui_isCmdUsable) */
    u8 trance; /* the menu was opened in trance (btlui_isCmdUsable: else a trance unit can't use it) */
    u8 listKind; /* nonzero: targets are picked from a list window of this kind (btlui_openListWindow) */
    u8 doubleCast; /* Dbl Wht / Dbl Blk: the first pick is stored (btlui_confirmDoubleCast) */
    u8 openState; /* menu state once the target window has opened */
    s8 closeState; /* menu state once the target window has closed */
    u8 firstCmdNo; /* double cast: first pick */
    u8 firstTargeting;
    u8 pad15[0x18 - 0x15];
    s16 firstSubNo;
    s16 subNo; /* ability or item id */
    s32 firstTargets;
    BtlUnit *unit; /* the unit choosing targets */
    BtlUnit *prevTarget; /* cursor unit before the whole side was marked (btlui_toggleTargetAll restores it) */
} Unk80104FF0;
extern Unk80104FF0 g_btlTarget;
extern u8 g_btluiRetargetOnAdd[];
extern void (*g_btluiListWindowDrawFns[])(Window *);

typedef struct {
    s32 base; /* start (GameData.unk7E0) */
    s32 cursor; /* next free byte (btlui_alloc) */
    s32 size; /* 0x8000 */
    s32 used; /* used */
    s32 free; /* free */
} Unk80105060;
extern volatile Unk80105060 g_btluiHeap;



typedef struct {
    u8 text; /* text buffer (0xFF = none) */
    u8 pad1[0x80 - 1];
    s32 timer; /* frames shown */
    s32 duration; /* frames to show (btl_initBanner: by the message speed option) */
    TextSize size; /* text width/height (text_measure) */
    u8 active;
    u8 useRect; /* 1: drawn in rect (btl_setBannerAt), 0: the default top box */
    u8 pad8E[2];
    s32 priority; /* a lower-priority text does not replace it (btl_setBanner) */
    u8 pad94[0x98 - 0x94];
    u32 *ot; /* ordering table (btl_getBannerOt) */
    u8 pad9C[0xA4 - 0x9C];
    RECT rect;
} Unk801051F0;
extern Unk801051F0 g_btlBanner;
extern s32 g_btlBannerDurations[];
extern s16 g_btlEnemyRotByAmbush[];
/* enemy type record (btl_initEnemyType; 0x74 bytes, the encounter header's records after its groups; Battle1334 is the
 * same record) */
typedef struct {
    s32 statusImmune; /* BtlUnit.statusImmune (btl_setEnemyStats) */
    s32 autoStatus; /* BtlUnit.autoStatus */
    s32 status; /* BtlUnit.status at the start */
    u16 maxHp; /* max hp (BtlEnemyType.max, BtlUnit.hp) */
    u16 maxMp;
    u16 gil; /* BtlEnemyType.gil */
    u16 exp; /* BtlEnemyType.exp */
    s32 drops; /* BtlEnemyType.drops */
    s32 steal; /* BtlEnemy.steal */
    u16 radius; /* BtlEnemyType.radius */
    u16 modelId; /* model (key 0x02xxxx; FF9.IMG dir index 7, file id = model id): btl_getEnemyModelId via Battle1334 */
    u16 motionIds[6]; /* motions (keys 0x03xxxx, in the model file): BtlEnemyType.motions */
    s32 meshMasks; /* BtlUnit.hiddenMeshes | vanishMeshes << 16: mesh-group bits (btl_setEnemyStats) */
    u16 flags; /* 1: its death triggers the event script (BtlEnemy.flags bit 6), 2: BtlEnemy.flags bit 7 (death motion and cry, btl_startHitMotion) */
    u8 pad32[0x34 - 0x32];
    u8 stats[4]; /* speed, strength, magic, spirit (btl_setEnemyStats) */
    u8 pad38[0x3C - 0x38];
    s32 elements; /* BtlUnit.elemGuard, elemAbsorb, elemHalf, elemWeak as one word */
    u8 level; /* BtlEnemyType.level */
    u8 category; /* BtlEnemyType.category (BTL_ENEMY_*) */
    u8 pad42;
    u8 defence[4]; /* defense, evade, magicDef, magicEva */
    u8 eatAbility; /* BtlEnemyType.eatAbility */
    u8 bones[3]; /* BtlEnemyType.bones: camera/effect anchor bones */
    u8 baseBone; /* BtlUnit.baseBone */
    u16 deathCry; /* BtlEnemyType.deathCry */
    u8 confuseAbility; /* BtlEnemyType.confuseAbility */
    u8 abilityCount; /* abilities of this type in Battle.enemyAbilities (btl_setupEnemies sums them into firstAbility) */
    u8 iconBones[6]; /* BtlEnemyType.iconBones */
    u8 iconY[6];
    u8 iconZ[6];
    u8 pad62[0x64 - 0x62];
    u16 shadowW; /* btl_setShadowSize */
    u16 shadowD;
    u8 shadowBone; /* BtlUnit.shadowBone */
    u8 card; /* BtlEnemyType.card */
    s16 shadowX; /* BtlShadow.offsetX */
    s16 shadowZ; /* BtlShadow.offsetZ */
    u8 shadowDirBone; /* BtlUnit.shadowDirBone */
    u8 pad6F[0x74 - 0x6F];
} Unk800C9D1C;
extern u32 g_btluiVsyncTick;
typedef struct {
    SPRT cmdIcons[4][2]; /* command icon sprites (btlui_initCachedCmdIcons) */
    DR_TPAGE cmdIconsTpage[2];
    SPRT headerA[2]; /* panel header sprites (btlui_initCachedHeader) */
    SPRT headerB[2];
    DR_TPAGE headerTpage[2];
    SPRT names[4][2]; /* party name sprites (btlui_initNamePrims) */
    SPRT rowsA[4][2]; /* cached row sprites (btlui_initCachedRows) */
    SPRT rowsB[4][2];
    DR_TPAGE namesTpage[2];
    DR_TPAGE rowsTpage[2];
    POLY_G4 highlight[2]; /* highlight bar (btlui_initHighlightPrim) */
    DR_TPAGE highlightTpage[2];
} Unk801050D8; /* 0x368, double-buffered menu primitives */
extern Unk801050D8 *g_btluiPrims;
extern u8 g_btluiBuf;
extern u8 g_btluiSavedRepeat[2][0x18];

typedef struct {
    s32 frames; /* frames */
    s16 from; /* start brightness */
    s16 delta; /* brightness change */
    s32 group; /* unit group (btlfx_fadeUnitGroup) */
} Unk800E6270;
s32 btlfx_fadeUnitGroup(u32, s32);

/* main exe: candidates for ff9.h */
extern UiSprite g_sideFrameSprites[3];
s32 win_getPalClut(s32);
void SetTexWindow(void *, RECT *); /* PsyQ */
void win_reapClosed(void);
u8 chr_getAppearanceType(s32, u8);
void ui_drawItemIcon(s32, s32, s32, s32, u8);
extern u32 g_targetCursorSprite;
void math_rotMatrixYXZ(void *, void *);
void gfx_selectBuffer(s32);
void pad_setRepeatEnabled(s32, s32, s32);
void win_setVisible(Window *, s32);
void win_setCloseFrames(Window *, s32);
void pad_resetRepeats(s32);
void win_close(Window *);
Window *win_get(s32);
ItemSlot *item_findSlot(s32);
void VSyncCallback(void (*)(void));
void text_uploadPartyNames(s32);
s32 akao_playSe(s32, s32, s32, s32);
s32 item_getCount(s32); /* main.c defines it returning u8 */
typedef struct {
    u32 unk0_0 : 8; /* not read in ovl_0b */
    u32 effect : 9; /* effect played (btl_playCmdEffect), as BtlCmd8.effect */
    u32 unk0_17 : 15; /* not read in ovl_0b */
    u8 formula; /* btl_calcCmd formula, as BtlCmd8.formula */
    u8 power; /* as BtlCmd8.power (heal amount per item count + 1 for some formulas) */
    u8 pad6; /* 0 in all 32 g_usableItemTable entries of the exe, no access in any binary */
    u8 rate; /* status hit rate in percent */
    s32 statuses; /* statuses added or removed (btl_addStatuses, btl_removeStatuses) */
} Unk8006B1F4; /* 0xC */
extern Unk8006B1F4 g_usableItemTable[];
void SetGeomScreen(s32);
s32 ReadGeomScreen(void);
s32 akao_getSpuTransferState(void);
void akao_sendCmdED(s32, s32);
s32 evt_createModel(EvtObject *);

/* ovl_0b functions */
s32 btlfx_moveUnitArc(s32, s16 *, s32, s32, s32, s16 *);
void btlfx_setUnitRot(s32, s16 *);
u16 btl_orTargets(u16 *);
void btlui_queueMenuUnit(BtlUnit *);
void btlui_initCachedCmdIcons(s32);
s32 btlui_cycleTarget(s32);
s32 btlui_switchTargetSide(u8);
void btlui_setDefaultTargets(s32);
void btlui_onListClosed(Window *);
void btlui_cancelTarget(void);
s32 btlui_isCmdUsable(void);
s32 btlui_refreshTargets(void);
void btlui_updateTargetWindow(void);
void btlui_onTargetWindowClosed(Window *);
void btlui_initCachedHeader(s32);
void btlui_initHighlightPrim(s32);
void btlui_initNamePrims(s32);
void btlui_initCachedRows(s32);
void btlui_updateGauges(void);
void btlui_hideListHeader(s32);
void btlui_initPanelPrims(s32);
void btlui_slidePanelLabels(void);
void btlui_pulsePanelHighlight(void);
void btlui_findHighlightRow(void);
void btlui_clearOtLinked(u32 *, u32);
void btl_setBannerText(s32);
void btlui_saveCursor(Unk800EB518 *, s32, s32, s32);
void btlui_onPassClosed(Window *);
s32 btlfx_stepHitSound(u8 *, s32, s32);
void btl_saveCharacter(BtlUnit *, s32);
void btl_tryEscape(u8);
void btl_spawnPopups(BtlUnit *);
s32 btlfx_isResultDeferred(void);
void btlui_initPartyEntries(void);
BtlUnit *btlui_popMenuUnit(void);
void btlui_pruneMenuQueue(void);
void btlui_clearAbilityLists(void);
s32 btlui_canOpenMenu(BtlUnit *);
void btlui_openCmdMenu(void);
void btlui_slideLeft(void);
void btlui_slideRight(void);
void btlui_slideBack(void);
s32 btl_setIdleMotion(BtlUnit *);
u32 btl_addStatuses(BtlUnit *, u32);
void btl_fillModelRgb(BtlModel *, s32, s32, s32);
void btl_setModelAbr(BtlModel *, s32);
void btlfx_setUnitBrightness(BtlUnit *, s32);
void btlui_copyTranceAbilities(Unk80104F88Entry *);
void btlui_linkSwdMagAbilities(Unk80104F88Entry *);
void btlui_addLearnedAbilities(Unk80104F88Entry *);
void btlui_addEquipAbilities(Unk80104F88Entry *);
void btlui_toggleHelp(u8);
void btlui_buildItemList(void);
void btlui_buildThrowList(void);
void btlui_addItemListEntry(s32);
void btlui_addThrowListEntry(s32);
s32 btlui_findItemListEntry(s32);
s32 btlui_findThrowListEntry(s32);
void btlui_confirmCmd(void);
void btlui_cmdCursorUp(void);
void btlui_cmdCursorDown(void);
void btlui_cmdSideInput(void);
void btlui_passTurn(void);
void *btlui_alloc(s32);
Unk80105078 *btlui_getPanel(void);
void btlui_showPanel(u8);
void btlfx_pollDrawSync(void);
void btl_updateAtb(void);
void btlui_checkSwdMagPartner(void);
void btlui_menuInput(void);
void btlui_updateMenu(void);
void btlui_clearMenuQueue(void);
void btlui_flushMenuQueue(void);
void btlui_closeMenu(void);
void btlui_closeWindows(void);
void btlui_applyTargetable(void);
void btlui_pollEscape(void);
s32 btlui_getFrozenSide(void);
void btlui_drawHelpWindow(void);
void btlui_helpInput(void);
void btlui_updateHelpSlide(void);
void btlui_closeHelp(void);
void btlui_removeItem(s32);
void btlui_buildItemLists(void);
s32 btlui_getItemReserved(s32);
void btlui_cacheCmdIcons(void);
void btlui_drawCachedCmdIcons(void);
void btlui_drawCached(void);
void btlui_drawTargetCursors(void);
s32 btlui_countTargeted(void);
void btlui_clearTargeted(void);
void btlui_cacheHeader(void);
void btlui_drawCachedHeader(void);
void btlui_updatePanel(void);
void btlui_drawPartyPanel(void);
void btlui_cacheRows(void);
void btlui_drawCachedRows(void);
void btlui_updatePanelAnims(void);
void btlui_drawPortIcons(void);
void btl_unloadEnemyModels(void);
u32 btl_addStatus(BtlUnit *, u32);
u32 btl_countActiveUnits(u32);
void btlcam_play(s32);
s32 btlfx_getCommonData(s32);
void btl_getBaseBonePos(BtlUnit *, u16 *);
void btlfx_copyWords(s32 *, s32 *, u32);
s32 btlfx_allocSound(s32);
void btlui_vsync(void);
void btlui_enable(u8);
void btlui_initMenu(void);
void btlui_initHelp(void);
void btlui_initItemLists(void);
void btlui_initHeap(void);
void btlui_initPanel(void);
void btlui_saveKeyRepeat(void);
void btlui_restoreKeyRepeat(void);
void btlui_initPrims(void);
void btlui_onReset(void);
void btlui_initDrawBufs(void);
void btlui_clearDfe(void);
void btl_execCmd(BtlUnit *);
void btl_flushCmdQueue(Battle *);
void btl_unlinkCmd(BtlCmd **, u8);
void btl_releaseItem(BtlCmd *);
void btl_calcCmd(BtlUnit *, BtlUnit *, BtlCmd *, s32);
u16 btl_getUnitsWithStatus(u32, s32);
void btl_useReservedItem(s32);
s32 btl_consumeMp(BtlUnit *, s16);
void btl_killUnit(BtlUnit *);
s32 btl_removeStatus(BtlUnit *, u32);
s32 btl_hasEnemyFlag(BtlUnit *, s32);
void btl_initState(void);
void btlcam_init(void);
void btl_clearPopups(void);
void btl_showMessage(u32, ...);
/* psyq-style varargs (stdarg.h) */
typedef char *va_list;
#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, LASTARG) (AP = ((char *)&(LASTARG) + __va_rounded_size(LASTARG)))
#define va_arg(AP, TYPE) (AP += __va_rounded_size(TYPE), *((TYPE *)(AP - __va_rounded_size(TYPE))))

typedef struct {
    s16 ofs; /* offset from the entry table */
    s16 layout; /* lines << 12 | text width, the packing of main's message-table word (MSG_ENTRY_W / MSG_ENTRY_LINES, msg_getEntry); the line count matches the 0xF7 newlines + 1 in all 40 g_btlMessages entries; no access in any binary */
} TextTblEnt;

typedef struct {
    u16 count; /* count */
    u16 pad2;
    TextTblEnt entries[1];
} TextTbl; /* message table (g_btlMessages) */

void text_formatInt(s32, u8 *, s32);

u8 *btl_getItemName(u8);
typedef struct {
    u8 pad0[8];
    u8 steal[4]; /* BtlEnemy.steal: steal items, 0xFF = none */
} Unk800B7F20; /* enemy steal slots */

void btl_addItem(s32);

u32 btlfx_holdMotion(s32);
s32 btlfx_stepTurnUnit(u8 *, s32, s32);
void btlfx_error(char *);
void btlfx_initAll(void);
u8 btl_isAtbRunning(void);
void btl_useItem(s32);
void btlui_setAtbEnabled(u8);
void btlui_drawListRow(s32, s32, s32, MenuView *);
void btl_initBanner(void);
s32 btl_unregisterFiles(void **, s32);
void btl_unloadPartyModels(void);
void btlui_exit(void);
void btl_updateStatusColor(BtlUnit *);
void btl_setStoneClut(BtlUnit *, s32);
void btl_applyBgLight(BtlModel *);
void btl_setMeshRgb(BtlModel *, u8, s16, s16, s16);
Battle122C *btlseq_start(BtlCmd *);
struct BtlBgHeader *btlbg_getHeader(void);
s32 btlfx_stepPlayMotionWait(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepPlayMotionHold(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepPlayAttackMotion(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepPlayAttackMotionHold(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepPlayAttackReturnMotion(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepFileMotionWait(struct Unk800D8EA4 *, s32, s32 *);
typedef struct {
    u8 unit; /* unit id moved */
    u8 target;
    u8 pad2[2];
    s32 frames;
    s16 from[4]; /* start position */
} Unk800D9260;
s32 btlfx_stepRunHome(Unk800D9260 *, s32, s32 *);
s32 btlfx_stepMoveToPoint(struct Unk800D7E14 *, s32, s32);
s32 btl_soundCmd(s32, s32, s32, s32, s32);
void btl_onCmdEffectDone(void);
void btl_enqueueCmd(struct BtlCmd *);
void btl_reapplyStatusLook(BtlUnit *);
void btl_clearRewards(void);
void btl_setModelRgb(BtlModel *, s32, s32, s32);
void btl_showCmdName(BtlCmd *);
s32 btlfx_stepUnitSound(u8 *, s32, s32);
s32 btlfx_stepPlayMotion(u8 *, s32, s32);
s32 btlfx_stepFileMotion(u8 *, s32, struct Unk800F78A40 *);
void btlfx_reapplyMini(BtlUnit *);
void btlfx_reapplyVanish(BtlUnit *);
s32 btl_soundPan(s32); /* unit id, or an SVECTOR pointer */
void btl_setCmd(); /* (BtlCmd *, u32, s32, s32, s32); callers pass the command as s32 */
u8 btl_isCmdPending(BtlCmd *);
void btl_hideMeshes(BtlUnit *, u16);
void btl_showMeshes(BtlUnit *, u16);
s32 btl_hasStatus(BtlUnit *, u32);
void btl_clearUnitStatus(BtlUnit *);
s32 btl_removeStatuses(BtlUnit *, u32);
s32 btlfx_getMotionLength(s32);
u16 btlfx_getMeshPrims(BtlModel *, u16, s32, s32 *, s32);
s32 btl_isModelPart(BtlUnit *);
s32 btl_getUnitHeight(BtlUnit *);
void btl_getUnitPos(BtlUnit *, s16 *);
void btl_getUnitBodyPos(BtlUnit *, s32, s16 *);
typedef union {
    s32 w[2];
    s16 h[4];
} SVec4; /* SVECTOR accessed as words */
void btlfx_turnUnitToward(s32, s16 *, s16 *, s32, s32, s32, s32);
void btlfx_getUnitScale(s32, VECTOR *);
void btlfx_fadeUnit(BtlUnit *, s32);
s32 btlfx_playCommonSound(s32, s32);
BtlUnit *btl_getUnitById(u16);
void btl_main(s32);
void btl_initMode(void);
void btl_exitMode(void);
s32 btl_getDepSectors(s32, s32 *, s32 *);
s32 btlfx_stepResetMotion(u8 *, s32, s32);
s32 btlfx_stepReleaseMotion(u8 *, s32, s32);
s32 btlfx_stepCallWeaponScript(u8 *, s32, s32);
void btl_playHitSound(BtlUnit *, BtlUnit *);
Unk8010141C *btlfx_getSummonModel(s32, char *);
void btlfx_clearEffModels(void);
void btlfx_clearSummonModels(void);
void btlfx_reset(void);
void btlfx_initTasks(void);
void btlfx_resetLoader(void);
void btlfx_initParticleSystem(void);
void btlfx_clearTargets(void);
void btl_setVsyncMode(s32);
void btlfx_initSounds(void);
s32 btlfx_playFileSound(s32, s32, s32);
s32 btl_cancelUnitCmds(BtlUnit *);
void btlcam_loadScript(s32);
void btlcam_finishPaths(void);
s32 btlseq_stepMove(BtlScript *, BtlUnit *);
BtlPopup *btl_newPopup(struct BtlUnit *);
s32 btl_packDigits(u32);
s32 btl_getWeaponSound(s32, u16);
void btl_playUnitSound(BtlUnit *, u16, u8);
s32 btlcam_step(void);
void btlfx_spawnTask(s32, s32, s32, s32);
void btlfx_setUnitMotion(s32, s32);
s32 btlfx_getUnitMotion(s32, s32);
s32 btlfx_getUnitKind(s32);
s32 btlfx_getKindTiming(s32, s32);
s32 btlfx_getFileData(s32, s32);
void btlfx_halt(char *);
void btl_getBonePos(BtlModel *, s32, u16 *);
void btl_getBoneMatrix(BtlModel *, s32, BtlBone *);
BtlObjSlot *btlfx_getEffModel(s32, char *);
u8 btl_getMeshCount(BtlModel *);
void btlfx_initParticles(s32, Pool30Entry *, Pool30 *);
void btlfx_initEmitters(u32, Pool6C *);
Pool6CEntry *btlfx_spawnEmitter(Pool30 *, Pool6C *, u8 *, s32);
Pool6CEntry *btlfx_startEmitter(s32, s32);
void btlfx_playBounceCamera(void);
void btlfx_playSoundSet(s32, s32);
s32 btlfx_getBouncePos(s32, s16 *, s16 *);

/* btlfx_opBounce work area */
typedef struct {
    s32 point; /* next point, -1 = done */
    s32 timer; /* timer, 0xFFFF = skipped */
    SVECTOR points[4]; /* bounce positions */
    s16 rot[3];
    u8 pad2E[2];
    Pool6CEntry *emitters[4];
} Unk800E4634;

typedef struct {
    u8 pad0[0x27];
    u8 bounceCount; /* BtlUnitList.bounceCount */
} Unk800E4634Arg2C;

typedef struct {
    u8 pad0[0xC];
    Unk800E4634Arg2C *targets; /* g_btlfxEnv->targets */
} Unk800E4634Arg2;

void btlfx_moveEmitter(Pool30 *, s32, s32);
void btlfx_killEmitter(Pool30 *, Pool6CEntry *);
s32 btlfx_updateParticles(Pool30 *);
/* particle helpers used by btlfx_updateParticles */
s32 btlfx_distance(SVECTOR *, SVECTOR *);
s32 btlfx_randCentered(s32);
void btlfx_randRotation(SVECTOR *);
void btlfx_stepParticle(Pool30Entry *, s32, s16 *, SVECTOR *);
void btlfx_stepParticleAltDrag(Pool30Entry *, s32, s16 *, SVECTOR *);
void btlfx_stepParticleNoDrag(Pool30Entry *, s32, s16 *, SVECTOR *);
void btlfx_stepParticleNoDragAlt(Pool30Entry *, s32, s16 *, SVECTOR *);
/* Effect modules only (export slot 0x008): the modules' own particle records. */
void btlfx_stepModuleParticle(void *, s32, SVECTOR *, SVECTOR *);
extern u16 g_btlfxRotPhase;
void btlfx_drawAnimQuads(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32);
void btlfx_drawSprites(u8 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32);
/* effect frame table (btlfx_drawFan) */
typedef struct {
    u16 frameCount; /* frame count */
    s16 otZ; /* ot z (x4) */
    u16 gouraud; /* gouraud flag */
    u16 pointCount; /* points per frame */
} Unk800D2938;

typedef struct {
    s32 code; /* primitive code */
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 fade; /* fade */
    s32 z; /* ot base z */
    u16 *uv; /* uv table of the frame */
    s32 count;
    s32 abr; /* abr << 5 (tpage bits) */
    u32 lastXY; /* set by btlfx_emitFrameQuad: the quad's last corner (x word) */
    u32 lastZ; /* last corner z */
    u8 pad24[0x28 - 0x24];
} Unk800D57D0Ctx;

typedef struct {
    s32 code; /* primitive code */
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 fade; /* fade */
    s32 tpage; /* abr << 5 */
    s32 z; /* ot base z */
    s32 count;
} Unk800D59D0Ctx;

void btlfx_emitFrameQuad(u8 *, Unk800D57D0Ctx *, u8 *, s32 *);
void btlfx_emitFrameFan(u8 *, Unk800D59D0Ctx *, u8 *, s32 *);
/* asm convention: packet $a2, OT $a3, SZ $t2, z base $t4 (src_equiv/ovl_0b/frames.c) */
void btlfx_linkFrameTri(u8 *, s32 *, u32, s32);
void btlfx_drawFan(Unk800D2938 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32);
void btlfx_runSubEmitter(Pool30 *, Pool30Entry *, SVECTOR *, SVECTOR *);
extern s32 g_btlfxSavedOtZ; /* saved g_btlfxEnv->otZ */
/* main exe: candidates for ff9.h */
MATRIX *MulMatrix2(MATRIX *, MATRIX *);
s32 btlfx_updateEmitters(Pool30 *, Pool6C *);
void btlfx_runEmitters(Pool30 *, Pool6C *);
void btlfx_heapInit(Heap *, s32);
void btlfx_resetAll(void);
void btlfx_initCommonData(void);
s32 btlfx_isEnemyUnit(s32);
void btlfx_setUnitTexAnim(s32, s32, s32);
s32 btl_unloadCommonData(void);
void btl_hideWeapon(BtlUnit *);
void btl_showWeapon(BtlUnit *);
s32 btlfx_align4(s32);
void btlfx_linkPrimAbr(); /* (u32 code, u32 *ot, void *prim, s32 tpage) */
u8 *btlfx_findScript(s32);
void btlfx_callScript(s32);
void btlfx_playUnitMotion(s32, s32);
void btlfx_setFileMotion(struct Unk800F78A40 *, s32, s32);
void btlfx_turnUnit(s32, s32);
void btlfx_playHitSound(s32, s32, s32);
s32 btlfx_isPartyUnit(s32);
s32 btlfx_getUnitWeapon(s32);
s32 btlfx_getWeaponScript(s32);
void btl_unreserveItem(s32);
/* flags of the battle load steps (btl_loadSoundsStep, btl_loadEnemyModelsStep, btl_load{Weapon,Party}ModelsStep,
 * btl_reloadPartyStep, btl_swapPartyModelStep) and btl_getPartyModelId. Evidence: [code] */
/* 0x01 and 0x02 are only passed through: no step (nor btl_getPartyModelId) tests them, in the US or the JP build
 * (docs/jp-build.md; the callers pass the same 0x207/0x22 words there). Named after the callers that pass them. */
#define BTL_STEPF_IN_BATTLE   0x01   /* only the mid-battle reloads pass it (btl_reloadParty, btl_swapPartyModel) */
#define BTL_STEPF_PARTY       0x02   /* every party-side load passes it: those two and btl_loadModelsStep's weapon and
                                      * party model polls, never its sound or enemy model steps */
#define BTL_STEPF_RELOAD_UNITS 0x04  /* btl_reloadPartyStep: then reapply the models, textures and motions to the party units */
#define BTL_STEPF_FLIP_TRANCE 0x10   /* btl_getPartyModelId: the form the unit isn't showing (trance <-> normal); every step clears it */
#define BTL_STEPF_BASE_MODEL  0x20   /* btl_getPartyModelId: the normal form, without looking at the unit (none exist yet) */
#define BTL_STEPF_DEP_SOUNDS  0x100  /* btl_loadSoundsStep: also the dependency table's sound banks (type DB_SOUND_BANK, kind 2) */
#define BTL_STEPF_SOUNDS      0x200  /* btl_reloadPartyStep: load the sound banks first */
#define BTL_STEPF_BEGIN       0x8000 /* start over: clear the loaded flag, state 1 (or 2) */
/* Step states of the battle loaders (the task's state, which the step also returns; 0 = done). A loader collects
 * the file ids it needs, then reads them nearest sector first: READ_NEXT starts one CD read and parks in CD_WAIT,
 * which goes on at task->nextState once the read is over. Each loader numbers its steps itself. Evidence: [code] */
#define BTL_LOAD_DONE 0
/* btl_loadBackgroundStep */
#define BTL_BGLOAD_CD_WAIT 1
#define BTL_BGLOAD_READ 2 /* read the scene's background file (sceneSector[1]) */
#define BTL_BGLOAD_REGISTER 4 /* register it, BTL_FLAG_BG_LOADED */
/* btl_loadSoundsStep */
#define BTL_SNDLOAD_CD_WAIT 1
#define BTL_SNDLOAD_COLLECT 2 /* BTL_SOUND_SE_BANK, and with BTL_STEPF_DEP_SOUNDS the dependency table's banks */
#define BTL_SNDLOAD_READ_NEXT 5
#define BTL_SNDLOAD_UPLOAD 6 /* once the SPU is free: akao_loadBankRetry / akao_loadBankInSlot, then READ_NEXT */
#define BTL_SNDLOAD_FINISH 7 /* wait for the last SPU transfer */
/* btl_loadEnemyModelsStep */
#define BTL_ENMLOAD_CD_WAIT 1
#define BTL_ENMLOAD_COLLECT 2 /* the dependency table's model files */
#define BTL_ENMLOAD_READ_NEXT 4
#define BTL_ENMLOAD_REGISTER 6 /* textures to VRAM, register the pack, then READ_NEXT */
#define BTL_ENMLOAD_FINISH 7 /* BTL_FLAG_ENEMIES_LOADED */
/* btl_loadWeaponModelsStep and btl_loadPartyModelsStep (which swaps CD_WAIT and COLLECT) */
#define BTL_MDLLOAD_WPN_CD_WAIT 1
#define BTL_MDLLOAD_WPN_COLLECT 2 /* the party's weapon model ids (g_weaponTable[equip[0]].modelId) */
#define BTL_MDLLOAD_PTY_COLLECT 1 /* the party's model ids (btl_getPartyModelId) */
#define BTL_MDLLOAD_PTY_CD_WAIT 2
#define BTL_MDLLOAD_READ_NEXT 5
#define BTL_MDLLOAD_REGISTER 7 /* btl_uploadModelTextures, register the pack, then READ_NEXT */
#define BTL_MDLLOAD_FINISH 8
/* btl_loadModelsStep (battle start): one step per loader, each polled until it returns BTL_LOAD_DONE */
#define BTL_MODELS_SOUNDS 1
#define BTL_MODELS_ENEMIES 2
#define BTL_MODELS_WEAPONS 3
#define BTL_MODELS_PARTY 4 /* then BTL_FLAG_PARTY_LOADED */
/* btl_reloadPartyStep (mid battle): BEGIN steps start a loader, the next one polls it */
#define BTL_RELOAD_START 1 /* BTL_STEPF_SOUNDS: SOUNDS, else straight to WEAPONS_BEGIN */
#define BTL_RELOAD_SOUNDS 2
#define BTL_RELOAD_WEAPONS_BEGIN 3
#define BTL_RELOAD_WEAPONS 4
#define BTL_RELOAD_PARTY_BEGIN 5
#define BTL_RELOAD_PARTY 6 /* then UNITS with BTL_STEPF_RELOAD_UNITS, else done */
#define BTL_RELOAD_UNITS 7 /* reapply models, textures, motions and weapons to the party units */
/* btl_swapPartyModelStep (one slot) */
#define BTL_SWAP_START 1 /* unregister the slot's old model */
#define BTL_SWAP_CD_WAIT 2
#define BTL_SWAP_READ 3
#define BTL_SWAP_REGISTER 5 /* textures to the slot's VRAM, register the model */
#define BTL_SWAP_APPLY 6 /* as BTL_RELOAD_UNITS for this slot */
s32 btl_reloadPartyStep(s32);
/* the statuses a character keeps after the battle (btl_saveCharStatus: Character.status). Evidence: [code] */
#define BTL_STATUS_KEPT (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_VIRUS | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | \
                         BTL_STATUS_TROUBLE | BTL_STATUS_ZOMBIE)
s32 btl_swapPartyModelStep(s32, s32);
u32 btlfx_getUnitStatus(s32, u32);
void btlfx_resetUnitMotion(s32, u8);
void btlfx_playSwingSound(s32);
void btlfx_setMotionHeld(s32, s32);
BtlUnit *btl_getUnit(s32);
void btlui_uploadPartyNames(void);
void btlui_addItem(s32);
void btlui_reserveItem(s32);
void btlui_unreserveItem(s32);
void btlui_buildCmdList(void);
extern RECT g_btluiCmdWindowRect; /* command window rect */
extern RECT g_btluiTitleTexWindow; /* title bar texture window (btlui_drawSideFrame) */
void btlui_drawCmdList(Window *);
void btlui_drawCmdOverlay(Window *);
void btlui_onCmdMenuOpened(); /* used as a Window callback */
s32 btlui_setMenuState(s32);
u8 btlui_isTargetWindowOpen(void);

typedef struct {
    u8 unit; /* unit id moved */
    u8 target;
    u8 skipParty; /* op argument 2: 1 with a party-unit target ends the op at once (btlfx_opJumpHome). 1 in abilities
                   * that can target allies (items, the Break skills, Reis's Wind, White Draw, Eat, Cook, Dragon
                   * Breath, ...), so the caster stays home instead of jumping at a party member; 0 in Luna and Focus */
    u8 playMotion; /* btlfx_stepJumpHome plays the unit's motion 0x1E */
    s32 frames; /* move frames (the motion's length) */
    s16 from[4]; /* start position */
    s16 to[4]; /* destination (home) */
} Unk800D955C;

typedef struct {
    u16 tpage;
    u16 clut;
} Unk800E8F50Entry;

typedef struct {
    u16 magic; /* 'so' */
    u16 textured; /* nonzero: textured (Hi_RegisterTexListModel), else gouraud */
    u16 modelOfs; /* offset */
    u16 texPosOfs; /* offset */
    Unk800E8F50Entry tex[1];
} Unk800E8F50;

/* btl_loadBackgroundStep's step state (a local of btl_load). Same layout as the other load steps' states. */
typedef struct {
    s16 state; /* state */
    u16 nextState; /* next state */
    u8 pad4[0x24 - 4];
    s32 sceneBuf; /* file buffer being read (the background, Battle11C0.bgData) */
} Unk800AA08C;

typedef struct {
    s16 state; /* state */
    u16 nextState; /* state after the CD read */
    s16 unk4; /* unk4-unkE, bufStart: initialised only (btl_reloadPartyStep) */
    s16 unk6; /* write-only (btl_reloadPartyStep) */
    s16 unk8; /* write-only (btl_reloadPartyStep) */
    s16 slot; /* party slot of the file being loaded */
    u8 padC[0xE - 0xC];
    s16 unkE; /* write-only (btl_reloadPartyStep) */
    union {
        s32 w[4];
        u16 h[8];
    } fileIds; /* file ids still to load, -1 = done */
    s32 bufStart; /* write-only: btl_reloadPartyStep sets it to the party buffer, the same value as buf */
    s32 buf; /* destination buffer */
} Unk800AA958; /* party model load state */

typedef struct {
    s32 size0; /* CD read request (cd_readChunks): first chunk size */
    s32 dst0; /* first chunk destination */
    s32 size1; /* second chunk size */
    s32 dst1; /* second chunk destination */
} Unk800AA958Req; /* CD read request */

/* btl_swapPartyModelStep: model texture table (db_lookup section 2, tag 0x12) */
typedef struct {
    u16 modelId; /* model id */
    u8 pad2[4 - 2];
    union {
        s32 w; /* low 24 bits: offset of the texture lists from this entry */
        u8 b[4]; /* b[3]: texture count */
    } tex;
    u8 pad8[0xC - 8];
} Unk800ABBC4Ent; /* 0xC */

typedef struct {
    u8 pad0;
    u8 count; /* entry count */
    u8 pad2[4 - 2];
    Unk800ABBC4Ent entries[1];
} Unk800ABBC4Tbl;

typedef struct {
    s16 tpage; /* tpage */
    s16 clut; /* clut */
} Unk800ABBC4Tex;

typedef struct {
    s16 x; /* x */
    s16 y; /* y (CLUT row; becomes h) */
    s16 w;
    s16 h;
} Unk800ABBC4Rect;

u8 cd_readChunks(s32, void *, s32, s32);
typedef struct {
    u8 pad0[8];
    s32 addr; /* file address */
    s32 done; /* bytes done */
    s32 chunk; /* chunk size */
} Unk800AE844; /* streamed file read */

extern s16 g_btlSoundLoadSlot;
s32 db_getSize(s32);
s32 akao_loadBankInSlot(s32 *, s32, s32);

/* btl_soundCmd: sound/streamed music command handler */
s32 btl_getSoundFileSectors(s32, s32 *);
s32 btl_getDepLink(s32);
s32 btl_unpackMusic(s32);
void btl_onSoundRead(Unk800AE844 *);
extern s32 g_btlSoundLoadId;
extern u8 g_btlSoundLoadType; /* byte 2 of g_btlSoundLoadId */
extern s32 g_btlSoundLoadSector;
extern s8 g_btlSoundLoadKind;
/* main exe: candidates for ff9.h */
void akao_stopMusic(s32);
s32 akao_playMusic(void *);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);
void akao_setCondition(s32);
void akao_setSeVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_setAllSeVolume(s32);
void akao_sendCmdAA(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_sendCmdAB(s32, s32);

s32 btl_uploadModelTextures(Unk800AA958 *, Unk800AA958Req *, s32, s32);
s32 btl_getPartyModelId(s32, s32);


s32 btlfx_stepJumpHome(Unk800D955C *work, s32 env, s32 *entry);
s32 btlfx_moveUnitLinear(s32 unitId, s16 *dest, s32 frame, s32 frames, s16 *from, s32 fromMode);
void btlfx_lerpVec(s16 *, s16 *, s32, s16 *);
u16 Hi_RegisterTexListModel(BtlModelHeader *, BtlTexInfo *, s16 *, s32);
u16 Hi_RegisterGouEffModel(BtlModelHeader *);
void btlfx_aimAngles(s16 *, s16 *, s16 *);
void btlfx_pointAhead(s16 *, s16 *, s32, s16 *);

typedef struct {
    u16 nameOfs; /* text offset into g_btlCmdNames */
    u16 helpOfs; /* text offset into g_btlCmdHelp */
    u8 pad4[6 - 4];
    u8 kind; /* confirm handler (g_btluiCmdConfirmFns): 1 ability list, 2/3 item lists, ... */
    u8 count; /* abilities of the command */
    s32 first; /* first ability: index into g_btlCmdAbilities (single-ability commands: btl_setCmd's ability) */
} Unk800FD56C; /* 0xC */
extern Unk800FD56C g_btlCmdTable[];
extern u8 g_btlCmdHelp[];
extern u16 g_btlItemHelpOfs[];
extern u8 g_btlItemHelp[];
extern s32 g_btluiSideCmdIds[];
extern RECT g_btluiTargetWindowRect;
extern RECT g_btluiListWindowRect;
Window *win_open(RECT *, s32, s32, u32);
void btlui_onTargetWindowOpened(Window *);
void btlui_drawListWindow(Window *);
void btlui_drawTargetList(Window *);
void btlui_drawTargetListOverlay(Window *);
void btlui_onSideCmdClosed(Window *);

typedef struct {
    s32 unk0; /* not read (DbEntry12.id, unk2 at db_findModelInfo) */
    u32 tablesOfs : 24; /* offset of the tables from this header */
    u32 count : 8; /* table length */
} Unk800AB758;

/* rotation part of a MATRIX, used where no translation is stored */
typedef struct {
    s16 m[3][3];
} RotMatrix33;

/* packed screen coordinate */

typedef struct {
    u32 *prim; /* next free primitive */
    u32 *ot; /* ordering table entry */
    s32 rgb; /* colour word: rgb | GPU_CODE(GPU_TGE [| GPU_ABE]), or'ed with the SPRT code (popups: BTL_POPANIM_SEMI) */
    u8 padC[0x10 - 0xC];
    XY32 xy; /* screen position (projected from the popup, then offset) */
    u16 clut; /* clut (BtlPopup.clut), shifted into the uv word */
} Unk800CCD00;
/* btl_drawPopupDigits' word: digit count << 16 | BCD digits, the last digit in the low nibble. Evidence: [code] */
#define BTL_POPDIGITS_COUNT_SHIFT 16
#define BTL_POPDIGITS(count, bcd) (((count) << BTL_POPDIGITS_COUNT_SHIFT) | (bcd))
#define BTL_POPDIGITS_DIGIT 0xF
#define BTL_POPDIGITS_DIGIT_BITS 4
/* popup bounce animation bytes (g_btlPopupBounce*, BtlPopupText.anim), one per frame. Evidence: [code] btl_drawNumberPopup, btl_drawTextPopup */
#define BTL_POPANIM_SEMI 0x80 /* draw semi-transparent this frame */
#define BTL_POPANIM_Y    0x7F /* height above the anchor (number popups; text popups read bits 0-4) */
#define BTL_POPANIM_END  0xFF /* the popup ends */

void btl_drawPopupGlyph(Unk800CCD00 *, SpriteDef *);
void btl_drawPopupDigits(Unk800CCD00 *, u32);
void btl_clampPopup(struct BtlPopup *, Unk800CCD00 *, s32, s32);
extern void (*g_btlPopupDrawFns[])(struct BtlPopup *, Unk800CCD00 *);
void btl_drawStatusCountdown(Unk800CCD00 *);
void btl_drawStatusIcons(Unk800CCD00 *);

/* status-effect sprites drawn on battle units (btl_drawStatusIcons) */
typedef struct {
    s32 showMask; /* status mask that shows it */
    s32 hideMask; /* status mask that hides it */
    u8 *sprite; /* sprite data */
    u8 attach; /* attach point index (g_btlPartyIconBones rows for players, BtlEnemyType.iconBones/iconY/iconZ for enemies) */
    u8 abr; /* semi-trans mode */
    u8 single; /* nonzero: one animated sprite (btlfx_drawSprite), else a sprite set (btlfx_drawSprites) */
    u8 turnsWithUnit; /* turns with the unit */
} Unk800F6A88;

extern Unk800F6A88 g_btlStatusIcons[];
/* per player model (Character.unk34.b.unk4): attach points (bone, y, z) */
extern u8 g_btlPartyIconBones[][6];
extern u8 g_btlPartyIconY[][6];
extern u8 g_btlPartyIconZ[][6];

/* status countdowns drawn as digits over battle units (btl_drawStatusCountdown) */
typedef struct {
    s32 status; /* status mask */
    s16 timer; /* status timer index (BtlUnit.statusTimers) */
    u16 colorByLevel; /* nonzero: grey level from the count (btl_drawStatusCountdown) */
} Unk800F6CA4;

extern Unk800F6CA4 g_btlStatusCountdowns[];

void btlfx_beginPrims(u8 *, s32 *);
u8 *btlfx_endPrims(void);
void btlfx_drawSprite(u8 *, SVECTOR *, s32, s32, s32);
void btl_updateScan(void);
void btl_updateDetect(void);
extern u8 g_btlPopupBounce[];
extern u8 g_btlPopupBounceHeal[];
/* main exe: candidates for ff9.h */
extern SpriteDef g_popupMpSprite;
s32 evt_battleTrigger(s32, s32, s32, s32);
void math_rotMatrixXYZ(void *, void *); /* rotation -> matrix */
s32 btlfx_transformPolar(SVECTOR *, s32, s32);
void btlfx_transformPoint(s32, s32, s32);
extern SpriteDef g_popupDigitSprites[]; /* digits */

typedef struct {
    u8 *pc; /* camera script pointer (btlcam_step reads its commands) */
    u32 frame; /* script frame, compared with each command's 10-bit frame */
    s32 easeSpeed; /* linear speed term of eased paths, weight per frame / 4096: the start speed of BTLCAM_EASE_IN, the end speed of BTLCAM_EASE_OUT (BTLCAM_OP_EASE_SPEED; runtime) */
    s32 viewHold; /* frames the target view is kept after target select (btlcam_applyTargetView) */
    u32 bob; /* vertical bob of eye and target together: BTLCAM_BOB_* (btlcam_applyBob; runtime) */
    s32 bobPhase;
    s32 savedVariant; /* variant restored for the scene camera after an effect (btlcam_endEffect) */
    s32 shakeAxes; /* BTLCAM_SHAKE_* (btlcam_applyShake) */
    s32 shakeFrames;
    s32 shakeLeft;
    s32 shakeFrom;
    s32 shakeTo;
    s32 zoomFrames; /* projection-distance tween (btlcam_step) */
    s32 zoomLeft;
    s32 zoomTarget; /* BTLCAM_ZOOM_* */
    s32 zoomFrom;
    s32 zoomTo;
    u8 setHomeView; /* BTLCAM_OP_SET_HOME_VIEW: 1 requested, 2 eye/target saved in points[4]/[5] */
    u8 keepCamera; /* BTLCAM_OP_KEEP_CAMERA: btlcam_endEffect doesn't reload the scene camera */
    s16 casterAnchors[5]; /* btlcam_getUnitAnchors of the acting unit */
    s16 partnerAnchors[5]; /* btlcam_getUnitAnchors of the partner (effect flag 4) */
    s16 targetAnchors[5]; /* btlcam_getUnitAnchors of the target */
} Unk800FE488; /* 0x64 */
/* Unk800FE488.shakeAxes (BTLCAM_OP_SHAKE). Evidence: [code] btlcam_applyShake */
#define BTLCAM_SHAKE_NO_VERTICAL   0x01 /* leave y alone */
#define BTLCAM_SHAKE_NO_HORIZONTAL 0x02 /* leave x/z alone */
#define BTLCAM_SHAKE_AXES          (BTLCAM_SHAKE_NO_VERTICAL | BTLCAM_SHAKE_NO_HORIZONTAL)
/* Unk800FE488.zoomTarget (BTLCAM_OP_ZOOM): which distances the tween writes. Evidence: [code] btlcam_step */
#define BTLCAM_ZOOM_BASE    0x01 /* g_btlCamera.baseProjDist */
#define BTLCAM_ZOOM_CURRENT 0x02 /* g_btlCamera.projDist */
extern Unk800FE488 g_btlCamScript;
extern u32 g_btluiTargetMovePads[][4];
extern void (*g_btluiTargetMoveFns[])(void);
extern RECT g_btlBannerDefaultRect;
u8 *btl_buildBannerFrame(u8 *, RECT *);
void btl_applyModelScale(BtlModel *);
void btl_scaleModelMatrix(BtlModel *, VECTOR *);
void btlcam_getUnitAnchors(BtlUnit *, s16 *);
s32 btlfx_rand(void);
s32 btlfx_opJumpHome(s32, Unk800D955C *, s32, s32 *, s32 *);
s32 btlfx_opWaitSameSide(s32, u8 *, s32, s32 *, s32 *);


extern u8 g_btluiAtbOn;
extern u8 g_btluiTargetWindow;
extern u8 g_btluiTargetAllByType[];
extern u8 g_btlCharTranceCmds[][3];
extern u8 g_btlCharCmds[][2];
extern RECT s_btluiCmdIconsSrc;
void btl_initUnitCmds(BtlUnit *);
extern u16 g_btlCharModelIds[]; /* [0x27]: battle body model (FF9.IMG dir 10 file id) per appearance kind (Character.info.b.kind, 19 kinds), trance model at kind + 0x13 (0 for the guests 14-18); a non-party unit reads [0x13] (581) leaving and [0x26] (125) entering Trance (btl_checkCmdUsable BTL_CMD_TRANCE) */
void btl_initPartyUnit(Character *, BtlUnit *, u16, u16);
void btl_insertUnit(BtlUnit *);
void btl_placeParty(void);
void btl_initDotTimer(BtlUnit *, s32);
void btl_createObjModel(EvtObject *, s16);
extern RECT s_btluiRowsSrcA;
extern RECT s_btluiRowsSrcB;
void btl_updateUnitTint(BtlUnit *);
void btl_updateKo(BtlUnit *);
s32 btl_playVictoryMusic(GameData *, u32, s32);
void btlcam_playScene(s32);
void btl_clearCmdQueue(Battle *);
extern s16 g_btluiDispOrigin[][2];
void btl_projectUnitPos(BtlUnit *);
void btlui_drawTranceLabel(s32, s32, s32, char *);
s32 btlui_getMenuState(void);
s32 btlui_getTargetReturnState(void);
extern u8 g_btlCmdNames[];
void btlui_drawSideCmd(); /* (Window *); btlui_drawCmdList passes its own */
s32 btlfx_isUnitKind(s32, s32);
void btlfx_projectSliceMesh(BtlModel *, u8);
u8 *btlfx_drawSliceMesh(BtlModel *, u8, u8 *);
void btlfx_projectMorphMesh(BtlModel *, u8, u16);
u8 *btl_drawMeshOffset(BtlModel *, u8, u8 *, s32);
void *btl_getUnitName(BtlUnit *);
extern u8 g_btlCmdNameMap[];
extern u8 g_btlCmdNameTexts[];
void btlseq_getTargetCenter(u16, u16 *, u16 *);
void btlcam_playSeq(s32);
void btl_copyAbility(BtlCmd8 *, BtlCmd8 *);
s32 btl_getMpCost(BtlCmd8 *);
void btl_initEnemyUnit(Unk800C9EE0 *, BtlUnit *, s32 *, s32);
void btl_setEnemyStats(Unk800C9D1C *, BtlUnit *);
void btl_initEnemyType(Unk800C9D1C *, BtlEnemyType *);
s32 btl_rollAmbush(u32 *);
void btlbg_setBrightness(s32);
s32 btlbg_getBrightness(void);
/* Data words inside .text (hand-written asm), stored by btlbg_setBrightness / btlbg_setOtShift. */
extern s32 g_btlbgBrightness;
extern s32 g_btlbgOtShift;
UnkPair *btlui_getItemListEntry(s32);
Battle1330 *btl_pickEnemyGroup(Battle11C020 *);
u32 btl_findNearestFile(u32, u16 *, u16 *, u16 **, s32 *);
s32 Hi_RegisterSummonModel(struct Unk80100DC054 *, s16 *);
typedef struct {
    u32 uvclut; /* uv/clut */
    u8 w; /* w */
    u8 h; /* h */
    u16 tpage; /* tpage */
} Unk80068EE8; /* sprite descriptor */
extern Unk80068EE8 g_listCursorSprites[];
extern Unk80068EE8 g_portIconSprite1;
extern Unk80068EE8 g_portIconSprite2;
extern Unk80068EE8 g_portBlinkSprite1;
extern Unk80068EE8 g_portBlinkSprite2;
extern u8 g_btluiPort;
extern SprtWords g_btluiPortIconSprt;
extern SprtWords g_btluiPortBlinkSprt;
extern DR_TPAGE g_btluiPortIconTpage;
extern DR_STP g_btluiPortIconStp;
extern u32 g_btluiPortIconOt;
s32 btl_findBackgroundId(void);
Unk8010141C *btlfx_allocSummonModel(void);
u8 *db_getModelTexPages(s32);
u8 *db_getModelUvOffsets(s32);
typedef struct {
    u32 targetType : 4; /* Battle16CC layout (btlui_selectItem copies them into the target select) */
    u32 defaultSide : 1;
    u32 listKind : 3;
    u32 unk0_8 : 21; /* not read in ovl_0b */
    u32 allowKo : 1;
    u32 unk0_30 : 1; /* not read in ovl_0b */
    u32 koFirst : 1;
    u8 pad4[0xC - 4];
} Unk8006A774;
extern Unk8006A774 g_usableItemTableById[];
u8 *btl_calcBoneMatrices(BtlModel *, s32, u8 *);
void btl_projectMesh(BtlModel *, u8);
u8 *btl_drawMesh(BtlModel *, u8, u8 *);
void btl_setModelTransform(BtlModel *, SVECTOR *, VECTOR *, VECTOR *);
void Hi_GetSummonBoneMatrix(s32, s32, BtlBone *);
/* texture (VRAM) animations */
typedef struct {
    s16 x; /* dest x */
    s16 y; /* dest y */
    s16 w; /* w */
    s16 h; /* h */
    s16 (*src)[2]; /* source x/y per frame */
} TexAnimRect; /* 0xC */

typedef struct {
    u8 flags; /* BTL_TEXANIM_* */
    u8 frameCount; /* frame count */
    s16 speed; /* speed (20.12) */
    u16 waitMin; /* frames to wait before looping again: waitMin + random(waitRand + 1) */
    u16 waitRand;
    s32 pos; /* position (20.12) */
    u8 rectCount; /* rect count */
    u8 relocIndex; /* index into the offset table (btl_relocTexAnims) */
    u16 lastFrame; /* last frame shown */
    TexAnimRect *rects;
} TexAnim; /* 0x14 */

typedef struct {
    u16 count; /* count */
    u16 pad2;
    TexAnim anims[1];
} TexAnimSet;

void btl_updateTexAnims(TexAnimSet *);
BtlUnit *btl_findBodyUnit(void);
u16 btl_findCover(s32);
s32 btl_setKoMotion(BtlUnit *);
void btl_checkSideDefeated(BtlUnit *);
void btl_addEnemyRewards(BtlEnemyType *);
void btl_unlinkUnit(BtlUnit *);
s32 btl_getModelSlot(BtlUnit *);
void btl_makeGreyCluts(BtlUnit *);
void btl_gainTrance(BtlCalc *, u8);
s32 btl_canAddHitStatus(BtlCalc *);
/* ability effect helpers called by btl_calcCmd */
void btl_applyCalcResult(BtlCalc *);
void btl_calcSteal(BtlCalc *, Unk800B7F20 *, s32);
u8 btl_rollHit(BtlCalc *);
u8 btl_rollStatusHit(BtlCalc *);
void btl_setPhysParams(BtlCalc *, u8);
void btl_applyKillerBonus(BtlCalc *);
void btl_rollCritical(BtlCalc *);
u8 btl_applyAbilityElement(BtlCalc *);
void btl_finishPhysDamage(BtlCalc *);
void btl_setTargetHp(BtlCalc *);
void btl_rollFlee(BtlCalc *);
u8 btl_checkFrozenShatter(BtlCalc *);
void btl_setDrainFlags(BtlCalc *);
void btl_initHitRates(BtlCalc *);
void btl_initStatusHitRate(BtlCalc *);
u8 btl_initUndeadHitRate(BtlCalc *);
u32 btl_hasSureHit(BtlCalc *);
void btl_halveHitForShell(BtlCalc *);
void btl_halveHitForSpread(BtlCalc *);
u8 btl_checkLevelMultiple(BtlCalc *);
u8 btl_missIfDeathImmune(BtlCalc *);
u8 btl_checkTargetIsPlayer(BtlCalc *);
u8 btl_checkTargetAlive(BtlCalc *);
u8 btl_checkRevivable(BtlCalc *);
u8 btl_checkNotSelf(BtlCalc *);
u8 btl_guardIfDeathImmune(BtlCalc *);
void btl_setAbilityPhysParams(BtlCalc *);
void btl_setMagicParams(BtlCalc *);
void btl_setMultiplierFromPower(BtlCalc *);
void btl_setWeaponAbilityParams(BtlCalc *);
void btl_setEnemyAttackParams(BtlCalc *);
void btl_applyCasterStatusMod(BtlCalc *);
void btl_randomizeDefense(BtlCalc *);
void btl_applyTargetStatusMod(BtlCalc *);
void btl_applyCasterMiniMagic(BtlCalc *);
void btl_applyTargetShell(BtlCalc *);
void btl_applyHealBoost(BtlCalc *);
void btl_applyRowMod(BtlCalc *);
void btl_applyWeaponElementBoost(BtlCalc *);
u8 btl_applyWeaponElement(BtlCalc *);
u8 btl_missIfAirborne(BtlCalc *);
void btl_halveForSpread(BtlCalc *);
void btl_applyAbilityElementBoost(BtlCalc *);
void btl_calcHeal(BtlCalc *);
void btl_finishMagicDamage(BtlCalc *);
void btl_calcPercentDamage(BtlCalc *);
void btl_rollAbilityStatus(BtlCalc *);
void btl_applyAbilityStatus(BtlCalc *);
void btl_cureAbilityStatus(BtlCalc *);
void btl_cureItemStatus(BtlCalc *);
void btl_payMpAttack(BtlCalc *);
u8 btl_canFlee(BtlCalc *);
void btl_markTrouble(BtlCalc *);
void btl_calcEnemySteal(BtlCalc *);
void btl_toggleRow(BtlUnit *);
s32 btl_getElementName(u8);
void btl_startScan(s32);
void btl_startDetect(BtlUnit *);
u8 *btl_getAbilityName(s32);

/* main exe: candidates for ff9.h */
u8 chr_isAbilityLearned(s32, s32);
void btl_damageHp(BtlUnit *, s32, u8);
void btl_healHp(BtlUnit *, s32);
void btl_damageMp(BtlUnit *, s32);
void btl_healMp(BtlUnit *, s32);
void btl_updateModelVisibility(BtlModel *);
u8 *btl_drawShadow(BtlShadow *, s32, s32, s32, u8 *);
void btl_checkEnemiesKO(); /* defined as (void); callers pass the unit */
void btl_startEnemySeq(BtlCmd *);
void btlbg_setOtShift(s32);
void gfx_initTpagePolyF3(DR_TPAGE *, u16);
void btl_relocTexAnims(TexAnimSet *, u16 (*)[4]);
void btl_applyHitSupports(BtlUnit *, u8 *);
void btlui_nextDoubleCast(void);
void btl_playDeathCry(BtlUnit *, u16);
void btlui_applyEnable(void);
void btlui_updateAtbFreeze(void);
void btlui_checkHelpClose(void);
void btlui_refreshPanel(void);
void btlui_resizeListWindow(void);
void btlui_collectCursorPositions(void);
s32 btl_findWeaponModel(s32, s32);
void btl_getWeaponTexInfo(s32, s32, s32 **, s32 **, s32 **);
BtlUnit *btlui_getActiveUnit(void);
void btlseq_update(void);
u8 *btlbg_draw(MATRIX *, s32, s32, u32 *, s32);
u8 *btlfx_runFrame(u8 *);
u8 *btl_drawPopups(u8 *);
void btl_updateHpStatus(BtlUnit *);
void btl_updateUnitStatus(BtlUnit *);
void btlui_drawSideFrame(RECT *, s32);
extern s32 g_btluiSideCmdTexts[];
Window *btlui_getTargetWindow(void);
void btlui_getHelpRect(RECT *, RECT *);
void btlui_updateHelpText(void);
s32 text_getColor(void);
void text_setColor(s32);
void gfx_setBufferIndex(u8);
void win_drawHelpFrame(RECT *, s32, s32, s32);
extern RECT g_btluiHelpRect;
void btl_updateUnitModel(BtlUnit *, s32 *);
void btl_drawActiveMarker(GameData *, Battle *, BtlUnit *);
void btl_processCmdQueue(Battle *);
s32 btl_checkCmdUsable(Battle *, BtlCmd *);
s32 btl_validateTargets(Battle *, BtlCmd *);
s32 btl_payMpCost(BtlCmd *);
void btl_removeCmd(BtlCmd *);
s32 btl_waitHitMotions(BtlCmd *);
void btl_updatePartyCmd(BtlCmd *);
void btl_startCmdEffect(BtlCmd *);
void btl_finishCmd(Battle *);
void btl_drawScene(GameData *, Battle *, s32);

s32 akao_isSePlaying(s32);
void akao_sendCmdA2(s32, s32, s32);
s32 chr_findAbilitySlot(s32, s32);
void btlui_showListHeader(s32);
void btlui_drawListCursors(Window *);
s32 btlfx_angleDiff(s32, s32, s32 *);
s32 btlcam_resolveAnchor(s32, s16 *, s16 *, s32 *);
void btlcam_resolveAnchorPair(s32, s32, s16 *, s16 *, s32 *);
s32 btlcam_getAnchorUnit(s32);

typedef struct {
    u8 unit; /* unit id moved (0x10 caster) */
    u8 target; /* unit id moved toward */
    union {
        u8 skipParty; /* btlfx_opJumpForward (op argument 2): 1 with a party-unit target ends the op at once
                       * (set by items, the Break skills, Reis's Wind, Eat ...: see the jump-home view) */
        u8 reach; /* btlfx_opMoveTowardTarget (g_btlfxOpArgs.paramB[0]): tenths of the way to the strike point */
    } arg;
    u8 playMotion; /* btlfx_stepJumpForward plays the caster's BTL_MOTION_STEP_FORWARD */
    s16 dist; /* distance ahead of the target (btlfx_pointAhead) */
    s16 dist2; /* second distance (btlfx_pointAhead) */
    s32 frames; /* move frames */
    s16 from[4]; /* start position (btlfx_moveUnitArc/Linear) */
    s16 to[4]; /* destination */
} Unk800D7C9C;

typedef struct {
    u8 unit; /* unit id turned */
    u8 target; /* unit id faced, 0xFF = its home */
    u8 turnAway; /* 1: face away from the target (+0x800) */
    u8 snapToFront; /* op 0x71 third argument (runtime) */
    s32 frames; /* turn frames */
    s32 unk8; /* write-only (cleared by btlfx_opFaceTarget) */
    s16 delta[3]; /* rotation delta (btlfx_stepFaceTarget) */
} Unk800D805C;

typedef struct {
    u8 unit; /* unit id turned */
    u8 target;
    u8 partyMode; /* BTLFX_FACE_*: what a party-member target does (unit op 0x50 b, g_btlfxOpArgs.paramA[1]) */
    u8 unk3; /* unused */
    s32 frames; /* turn frames */
    s16 delta[3]; /* rotation delta back to its start rotation (btlfx_stepFaceHome) */
} Unk800D8124;

/* Unk800D8124.partyMode (unit op 0x72, btlfx_opFaceHome / btlfx_stepFaceHome). Runtime: Throw dagger at Vivi snaps
 * in one frame, at the Malboro turns over 4 frames; scripts use 0 in items and some abilities, 1 in Throw, Sword
 * Magic, Sword Arts, Flair and Seiken, 2 only in the resident racket attacks (0x68, 0x6A, 0x6F) and 0x27. */
#define BTLFX_FACE_ALWAYS     0 /* turn back over the frames whatever the target */
#define BTLFX_FACE_SNAP_PARTY 1 /* party target: snap back to the start rotation at once */
#define BTLFX_FACE_SKIP_PARTY 2 /* party target: the op ends at once without turning */

s32 btlfx_stepJumpForward(Unk800D7C9C *, s32, s32 *);
s32 btlfx_stepFaceTarget(Unk800D805C *, s32, s32 *);
s32 btlfx_stepFaceHome(Unk800D8124 *, s32, s32 *);
s32 btlfx_isSameUnit(s32, s32);

typedef struct {
    BtlUnit *caster;
    BtlUnit *target; /* first target */
    u8 pad8[0x38 - 8];
    s32 centerX; /* targets' centre (read as words) */
    s32 centerZ;
    u8 pad40[0x44 - 0x40];
    BtlUnit *partner;
    u8 pad48[0x4C - 0x48];
} Unk800FE368;
extern Unk800FE368 g_btlCamTargets;
extern u8 g_btluiSecondPort;
void bcopy(void *, void *, s32);
struct Battle11C020 *btl_initScene(void);
u8 pad_isRepeatEnabled(s32, s32);
void pad_setRepeatTiming(s32, s32, s32, s32);
void pad_clearRepeats(s32);
u8 btlui_anyOnSecondPort(void);
void btlfx_getUnitRot(s32, s16 *);

extern u16 g_btlPartyMotionIds[][0x22];
BtlPopup *btl_popupHp(s32, s16, s16, u8);
BtlPopup *btl_popupMp(s32, s16, s16, u8);
void btlui_setPauseRequest(s8);
s32 btlfx_stepRunToTarget(Unk800D7C9C *, s32, s32 *);
s32 btlfx_stepMoveTowardTarget(Unk800D7C9C *, s32, s32 *);

typedef struct {
    u32 tag;
    u32 code[5];
} DR_MOVE;

extern s32 g_btlfxSoundPaused;
void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y);
void akao_pause(s32);
void btlfx_resetMaskBit(void);
/* the frame update btlfx_runFrame runs (src_equiv/ovl_0b/effect_frame.c) */
void btlfx_initEnv(void);
void btlfx_runScript(void);
void btlfx_updateLoader(void);
s32 btlfx_updateTasks(void);
void btlfx_updateParticleSystem(void);
void btlfx_copyDisplayToDraw(void);
void btlfx_updateSounds(void);
extern s32 g_btlfxRandSeed;
extern s32 g_btlfxSavedSp;
/* main exe: candidates for ff9.h */
s32 vib_update(void);
extern u8 g_btlScanTexts[];
/* g_btlScanTexts entries (tools/ff9text.py text_table): 0-7 the enemy categories "Humanoid".."Aerial" (bit k of
 * BtlEnemyType.category), then */
#define BTL_SCAN_TEXT_CARRIES 8 /* "Carries " (Detect) */
#define BTL_SCAN_TEXT_NOTHING 9 /* "Isn't carrying anything." */
#define BTL_SCAN_TEXT_LV      10 /* TEXT_X_ADD, then "LV: " */
#define BTL_SCAN_TEXT_HP      11 /* "HP: " */
#define BTL_SCAN_TEXT_MP      12 /* TEXT_X_ADD, then "MP: " */
#define BTL_SCAN_TEXT_SLASH   13 /* "/" */
#define BTL_SCAN_TEXT_WEAK    14 /* + element bit: "Weak against Fire." .. "Weak against Shadow." */
extern u8 s_btlDetectItemSuffix[]; /* "/\xFF" suffix string */
void btl_setBanner(s32, s32);
void btl_setBannerAt(s32, s32);
s32 btl_getBannerDuration(void);
extern u8 g_btluiFrameReady;
void btlui_onUnitUntargetable(BtlUnit *);
void btlui_onUnitTargetable(BtlUnit *);
u32 btl_registerModel(s32, s32);
void btl_load(); /* takes the battle id (s32); unprototyped, the caller passes an s16 unconverted */
s32 btl_allocShadows(s32, s32);
s32 evt_countNpcActors(s32);
void game_clearModelSlot(void *);
void btl_makeEnemyGreyCluts(void);
void btl_execCmd(struct BtlUnit *);
void btl_onCmdEffectDone(void);
void btl_enqueueCmd(struct BtlCmd *);
void btl_makePartyGreyCluts(void);
void btl_saveEnemyCluts(u32 *);
void btl_savePartyCluts(u32 *);
void btl_setUnitBrightness(struct BtlUnit *, u32);
void btl_rollMagicFail(struct BtlCmd *);
s32 btl_checkSwdMagUsable(struct BtlUnit *, s16);
void btl_rollShortSummon(struct BtlCmd *);
void btl_interruptUnit(struct BtlUnit *, s32);
void btl_checkPartyDisabled(struct BtlUnit *);
void btl_initParty(struct Battle *);
u32 *btl_initEnemyState(struct Battle *, u32 *);
void btl_resetAtbSpeed(Unk800BD8F0 *);
void btl_initDoomCount(Unk800C09D8 *);
void math_rotMatrixZYX(SVECTOR *, MATRIX *);

s32 btl_setupBackground(Battle11C0 *, s32);
s32 btl_loadEncounterFile(Battle11C0 *, s32);
void btl_beginFrame(void);
void btl_checkDiscCover(void);
u8 btlui_isPauseRequested(void);
void btl_updatePause(void);
s32 btl_update(void);
void btl_updateIntro(GameData *, Battle *);
void btl_updateCombat(GameData *, Battle *);
void btl_victory(GameData *, Battle *);
void btlui_init(void);
void btl_preloadExit(void);
s32 preload_update(void);
void btl_endFrame(void);
void btlui_waitFrame(void);
void btl_putDispEnvNoUi(void);
void btlui_drawOTagEnv(u32 *, DRAWENV *);
void btl_cleanup(void);
s32 btl_getModelTexInfo(u32, s32 **, s32 **, s32 **);
u32 evt_getObjectChrIndex(void *);
/* object slot returned by game_getModel in battle */
typedef struct {
    u32 flags; /* MODEL_SLOT_* (0x10: btl_drawShadow skips it) */
    s32 modelId; /* model id */
    BtlModel *model;
    s32 unkC; /* not read */
} BtlObj;
/* placement data passed to btl_calcShadowMatrix */
typedef struct {
    u32 pad0_0 : 8;
    u32 width : 12; /* x scale (BtlShadow.width) */
    u32 depth : 12; /* z scale (BtlShadow.depth) */
    s32 pad4_0 : 12;
    s32 offsetX : 10; /* x offset */
    s32 offsetZ : 10; /* z offset */
} Unk800AD4E4;

/* copies a MATRIX as eight words (the original doesn't use a struct assignment) */
#define COPY_MATRIX(d, s)                    \
    {                                        \
        s32 *_d = (s32 *)(d);                \
        s32 *_s = (s32 *)(s);                \
        _d[0] = _s[0];                       \
        _d[1] = _s[1];                       \
        _d[2] = _s[2];                       \
        _d[3] = _s[3];                       \
        _d[4] = _s[4];                       \
        _d[5] = _s[5];                       \
        _d[6] = _s[6];                       \
        _d[7] = _s[7];                       \
    }

void btl_allocBattle(void);
void btl_clearBattle(void);
s32 btl_loadKanjiFont(void);
s32 btl_loadResidentMusic(void);
s32 btl_loadSoundBanks(void);
s32 btl_loadShadowTexture(void);
s32 btl_initPartyNames(void);
s32 btl_loadCommonData(void);
s32 btl_preloadRequest(void);
extern void (*g_btluiCmdConfirmFns[])(Unk800FD56C *);
extern void (*g_btluiSideCmdConfirmFn)(Unk800FD56C *);
extern void (*g_btluiPanelColumnFns[])(BtlUnit *, s32);
void btlui_drawPanelHeader(void);
u8 btl_isCmdQueued(BtlUnit *, u8);
void btlui_targetFirst(u8, u8);
typedef struct {
    u32 status; /* status mask */
    s16 icon; /* g_uiSprites index */
    s16 width; /* x advance (btlui_drawStatusList2) */
} Unk800F8F98;
extern Unk800F8F98 g_btluiStatusIcons2[];
extern Unk800F8F98 g_btluiStatusIcons[];
void btl_removeQueuedCmd(BtlUnit *, u8);
void btlui_frameUpdate(void);
s32 btlui_isEnabled(void);
void btlui_putDispEnvIfReady(void);
void btlui_onVsync(void);
void btlui_readPad(void);
void btlui_perfMark(s32);
void btlui_beginFrame(void);
void btlui_cacheFrame(void);
void btlui_input(void);
void btlui_drawWindows(void);
void btlui_update(void);
void btlui_submitOt(void);
s32 btl_partyHasSupport(s32, u32);
u32 btl_addRgbClamped(u32, s32, s32, s32);
void menuList_pageUp(MenuList *, s32);
void menuList_pageDown(MenuList *, s32);
extern s16 g_btluiRowCluts[];
void btlfx_heapFree(void *);
s32 btlfx_poolUpdate(Unk800DA1B8 *);
void btlui_targetDefault(u8, u8);
BtlUnit *btlui_findKoUnit(u8);

extern u8 g_btluiDefaultTargets[][2];
void btlui_closeTargetWindow(s32);
s32 clut_getY(s16);
s32 db_getModelTexCount(s32);
void btl_startHitMotion(BtlUnit *);
s32 btlfx_heapAlloc(Heap *, s32);
void btl_playCmdEffect(BtlCmd *, s32, s16 *);

s32 btlfx_stepArcAtTarget(Unk800D7C9C *, s32, s32 *);
s32 math_sin(s32);

typedef struct {
    u8 returnState; /* g_btlTarget.returnState */
    u8 cmdNo;
    u8 targetType;
    u8 defaultSide;
    u8 koFirst;
    u8 allowKo;
    u8 subNo;
    u8 listKind;
    u8 doubleCast;
    u8 trance;
} Unk800EF80C;

typedef struct {
    u8 pad0[8];
    s32 ability; /* ability id (Battle.abilities index) */
} Unk800EDE6C;

typedef struct {
    u32 tag; /* tag */
    u32 stp; /* DR_STP words: GPU_STP(0, 0) clears the mask bit settings */
    u32 nop;
    u32 ot; /* ordering table */
} Unk800D0940;

extern s32 g_btlfxMaskResetBusy;
void btlui_startTargetSelect(Unk800EF80C *);

void btl_endPartyHitMotion(BtlUnit *);
void btl_endEnemyHitMotion(BtlUnit *);
s32 btlfx_stepWaitKindTime(u8 *, s32, s32 *);

extern Unk800F412C g_btluiPanelLabels[5];
s32 math_cos(s32);
void btlfx_execCmd(s32);
void btlfx_showPopups(s32);
void btlui_confirmTarget(void);
void btlui_toggleTargetAll(void);
void btlui_alignPanelLabel(Unk800F412C *);
void btlui_resetGauges(s32);

BtlObjSlot *btlfx_allocEffModel(void);
void btl_applyModelTextures(BtlModel *, s16 *, s16 *);

void GeoSplitVertices(BtlModel *, s32, s32);
s32 btlfx_getSplitVertexSize(BtlModel *, s32);

/* argument block of btlui_openList */
typedef struct {
    s16 cmdNo;
    u8 trance;
    u8 pad3;
    s32 cmd; /* command table entry */
    s32 label; /* list window title label */
    s32 label2; /* second title label */
} Unk800EE37C;
void btlui_openList(Unk800EE37C *);
extern RECT g_btluiSelectListRect;
void win_setList(Window *, MenuList *);
void btlui_drawListContent(Window *);
void btlui_drawListOverlay(Window *);
void btlui_onListOpened(void);
s32 btlfx_isSameSide(s32, s32);
s32 btlfx_stepSyncOpposed(u8 *, s32, s32);
extern u8 g_btluiHideWindows;
extern u8 g_btlCmdAbilities[];
extern U16Pair g_btlAbilityHelpOfs[];
extern char g_btlAbilityHelp[];
s32 btlui_getAbilityState(s32);
s32 *btl_getBannerOt(s32);
u8 *btl_buildBanner(u8 *);
u8 *btl_drawBanner(u8 *);
typedef struct Unk800D9134 {
    u8 unit; /* unit id moved */
    u8 target; /* unit id moved to */
    u8 pad2[2];
    s32 frames;
    s16 from[4]; /* start position */
    s16 to[4]; /* destination */
} Unk800D9134;
s32 btlfx_stepMoveToUnit(Unk800D9134 *, s32, s32 *);
extern s32 g_btlfxCommonInit;
extern s32 g_btlfxCommonData[];
extern u16 g_btlfxCommonCluts[];
extern s32 (*g_btluiListConfirmFns[])(void);
s32 btlfx_reloadCommonData(s32);
u16 *btlfx_getCommonSprite(s32);
s32 btlfx_resolveFirstUse(s32);
extern u8 g_btlfxLargeTargetFx[][2];  /* summon id upgrades */
extern u16 g_btlfxFirstUsePairs[16][2];
void btlcam_focusCaster(void);
u32 btlfx_splitMeshVertices(BtlModel *, u16, s32);
extern Unk800F3F10 g_btluiListShowSlide;
extern Unk800F3F10 g_btluiListShowTitleSlide;
extern U16Pair g_btluiListTitles[];
s32 btl_pickRandomUnit(s32);
s32 btlfx_stepSwingSound(u8 *, s32, s32);
void btlui_closeList(void);
extern u16 g_btlPowersOf10[];
s32 btlfx_stepWaitOtherSide(u8 *, s32, s32 *);
u8 btlui_isListWindowOpen(void);
void btlui_closeListWindow(s32);
void btlui_onCmdWindowClosed(Window *);

/* main exe: candidates for ff9.h */
s32 akao_uploadSeBank(s32 *, s32);
void text_draw(s32, s32, s32, u8 *, s32);
void win_closeAll(void);
s32 chr_getDefaultSet(Character *);

/* battle menu: unit under the target cursor, the first one flagged both TARGETABLE and TARGETED (inlined by its
 * callers) */
static inline BtlUnit *BtlFindTargetCursor(void) {
    BtlUnit *p;
    for (p = g_gameState->gameData->ctx.battle->unitList; p != NULL; p = p->next) {
        if ((p->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            return p;
        }
    }
    return NULL;
}

/* inlined copy of btlui_getTargeting */
static inline s32 btlui_getTargeting_inline(void) {
    BtlUnit *unit;
    s32 count;
    if (g_btlTarget.targetType >= 9 && g_btlTarget.targetType <= 11) {
        return 2;
    }
    count = 0;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if ((unit->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            count++;
        }
    }
    return count >= 2;
}

/* Queue-entry validity check inlined into btlui_pruneMenuQueue. */
static inline u8 BtlUnitCanAct(BtlUnit *u) {
    if (u == NULL) {
        return 0;
    }
    if (u->hp == 0 || (u8)btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) != 0) {
        return 0;
    }
    if ((g_btlRewards.expMask >> ((u8)u->flags >> BTL_UNIT_SLOT_SHIFT)) & 1) {
        return 1;
    }
    return 0;
}

/* -O1 unit (ovl_0b_o1.c) */
s32 btl_getSceneData(s32);
/* Unk800C412C0.anchorFlags bits (btlcam_orbitAnchor). Evidence: [code] */
#define BTLCAM_AF_ABS_ANGLES 0x40 /* the key's angles are absolute: the anchor's rotation is not added (no party flip) */
#define BTLCAM_AF_ABS_DIST   0x80 /* the key's distance is absolute: the anchor's distance is not added */
void btlcam_evalPath(s32, Unk800C412C *, s16 *);
s32 btlfx_lerp(s32, s32, s32);

typedef struct {
    u8 anchor; /* Unk800C412C0.anchor */
    u8 anchorFlags;
    u16 x : 7; /* x >> 5 */
    u16 y : 7;
    u16 preset : 2; /* nonzero: per-character preset row g_btlCamPartyPresets[(preset - 1) * 19 + model] for caster/target anchors */
    u16 z : 7;
    u16 w : 7; /* fourth value >> 7 */
} Unk800C3F78;
typedef struct {
    u8 w; /* fourth value >> 6 */
    u8 x; /* x >> 4 */
    u8 y;
    u8 z;
} Unk800F5758;
extern Unk800F5758 g_btlCamPartyPresets[];
void btlcam_setPathKey(Unk800C412C *, Unk800C3F78 *);
void btlcam_setPathTiming(Unk800C412C *, u8 *);
void btlcam_lookAt(MATRIX *, SVECTOR *, SVECTOR *);
void btlcam_applyTargetView(SVECTOR *, SVECTOR *);
void btlcam_applyBob(s16 *, s16 *);
void btlcam_applyShake(s16 *);
s32 btlfx_lerpRatio(s32, s32, s32, s32);
s32 btlfx_randRange(s32, s32);
s32 btlfx_sqrt(s32, s32);
s32 btlui_isTargetSelect(void);
s32 btl_isTargetWindowOpen(void);

typedef struct {
    u8 pad0[0x17];
    u8 cmdArg; /* Unk80100DC0.cmdArg */
    u8 pad18[0x44 - 0x18];
    RECT *texRects;
    s16 texIndex;
} Unk800DA790;

typedef struct {
    s32 frames; /* frames */
    s32 shade; /* shade */
} Unk800E6410;

void btlfx_drawScreenTint(u32 *, s32, s32, s32, s32);

void btlfx_getUnitHomePos(s32, s16 *);
void btlfx_setUnitPos(s32, s16 *);
void btlfx_applyFloatHeight(s32, s16 *);

/* main exe: candidates for ff9.h */
void LoadAverageShort12(SVECTOR *, SVECTOR *, s32, s32, SVECTOR *);

#define gte_DpqColorFar(rgb, p) __asm__ volatile( \
    "ctc2\t$0, $21;" \
    "ctc2\t$0, $22;" \
    "ctc2\t$0, $23;" \
    "lwc2\t$6, 0(%0);" \
    "addiu\t$14, $0, 0x1000;" \
    "subu\t$14, $14, %1;" \
    "mtc2\t$14, $8;" \
    GTE_NOPS ".word\t0x4A780010;" \
    "swc2\t$22, 0(%0)" \
    : : "r"(rgb), "r"(p) : "$14", "memory")

/* gte_DpqColorFar without the store: the result is fetched later by gte_strgb */
#define gte_DpqColorFar_nst(rgb, p) __asm__ volatile( \
    "ctc2\t$0, $21;" \
    "ctc2\t$0, $22;" \
    "ctc2\t$0, $23;" \
    "lwc2\t$6, 0(%0);" \
    "addiu\t$14, $0, 0x1000;" \
    "subu\t$14, $14, %1;" \
    "mtc2\t$14, $8;" \
    GTE_NOPS ".word\t0x4A780010" \
    : : "r"(rgb), "r"(p) : "$14")
#define gte_strgb(r0) __asm__ volatile("swc2\t$22, 0(%0)" : : "r"(r0) : "memory")

/* btlfx_drawSpriteLine: sprites placed along a path */
typedef struct {
    s32 spriteOfs; /* offset of the sprite data */
    s16 unk4; /* not read */
    s16 along; /* position along the path (x2, btlfx_lerpVec) */
    u16 start; /* start time */
    u16 fadeIn; /* fade-in time */
} Unk800D3B6CEntry;

typedef struct {
    u16 duration; /* duration */
    u16 count; /* count */
    u16 polar; /* 0: sprites along from..to; else around the point at angle/dist */
    u8 angleEase; /* btlfx_ease mode of the angle */
    u8 distEase; /* btlfx_ease mode of the distance */
    s16 from[2];
    s16 to[2];
    s16 angle0;
    s16 angle1;
    s16 dist0;
    s16 dist1;
    Unk800D3B6CEntry entries[1];
} Unk800D3B6C;

/* sprite-strip context passed to btlfx_emitTrailRibbonScreen / btlfx_emitTrailRibbon3D */
typedef struct {
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 ring; /* trail point ring */
    s32 fade; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } color; /* rgb + primitive code (b[3]) */
    u32 farColor; /* rgb at the far end */
    s16 v0; /* v of the strip */
    s16 v1;
    s32 tpage; /* tpage << 16 */
    s32 clut; /* clut << 16 */
    s32 fadeStep; /* 0x1000 / count */
    s16 u0; /* u << 8 */
    s16 u1; /* (u + width) << 8 */
    u8 pad28[0x70 - 0x28];
} Unk800D5C30Ctx;
void btlfx_emitTrailRibbonScreen(s32, Unk800D5C30Ctx *, u8 *, s32 *);

/* same as Unk800D5C30Ctx plus two words (btlfx_emitTrailRibbon3D) */
typedef struct {
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 ring; /* trail point ring */
    s32 fade; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } color; /* rgb + primitive code (b[3]) */
    u32 farColor; /* rgb at the far end */
    s16 v0; /* v of the strip */
    s16 v1;
    s32 tpage; /* tpage << 16 */
    s32 clut; /* clut << 16 */
    s32 fadeStep; /* 0x1000 / count */
    s16 u0; /* u << 8 */
    s16 u1; /* (u + width) << 8 */
    s32 anchorXY; /* btlfx_drawTrail arg 2 (an SVECTOR as two words): the emitter origin for element kind 4 */
    s32 anchorZ;
    u8 pad30[0x60 - 0x30];
} Unk800D50A0Ctx;
void btlfx_emitTrailRibbon3D(s32, Unk800D50A0Ctx *, u8 *, s32 *);

/* same fields as Unk800D50A0Ctx, without the padding (btlfx_emitTrailRibbonFlat / btlfx_emitTrailLines) */
typedef struct {
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 ring; /* trail point ring */
    s32 fade; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } color; /* rgb + primitive code (b[3]) */
    u32 farColor; /* rgb at the far end */
    s16 v0; /* v of the strip */
    s16 v1;
    s32 tpage; /* tpage << 16 */
    s32 clut; /* clut << 16 */
    s32 fadeStep; /* 0x1000 / count */
    s16 u0; /* u << 8 */
    s16 u1; /* (u + width) << 8 */
    union {
        s32 sinCos; /* btlfx_emitTrailRibbonFlat: sin (low half) and cos (high half) of 0x400 - camera yaw */
        s32 abr; /* btlfx_emitTrailLines: semi-transparency mode << 5, or'ed into its draw-mode (0xE1) word */
    } mode;
    s32 anchorZ; /* as Unk800D50A0Ctx.anchorZ; btlfx_emitTrailRibbonFlat / btlfx_emitTrailLines don't read it */
} Unk800D4D6CCtx;
void btlfx_emitTrailRibbonFlat(s32, Unk800D4D6CCtx *, u8 *, s32 *);
void btlfx_emitTrailLines(s32, Unk800D4D6CCtx *, u8 *, s32 *);

/* sprite animation header (btlfx_drawTrail); followed by u32 frame entries and colors */
typedef struct {
    u16 frameCount; /* frame count */
    u16 length; /* trail points kept (frame -1 resets the trail to this many) */
    s16 tpage;
    u16 clut;
    u8 h; /* texture height (v extent = h - 1) */
    u8 w; /* texture width (u extent = w - 1) */
    u16 frameOfs; /* first frame entry, in 8-byte units */
} Unk800D1494;

s32 btlfx_pushTrailPoint(u8 *, s32, s32, s32);
void btlfx_drawTrailScreen(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);
void btlfx_drawTrail3D(s32, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);
void btlfx_drawTrailFlat(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);

typedef struct {
    u16 frameCount; /* frame count (& BTLFX_SPRITE_FRAMES) */
    u16 tpage; /* tpage */
    u16 unk4; /* not read */
    u16 frameOfs[1]; /* frame offsets */
} Unk800D1BC4;

typedef struct {
    u32 color; /* rgb */
    s32 x; /* screen x */
    s32 y; /* screen y */
    s32 abr; /* abr << 5 */
    s32 tpage; /* tpage */
    s32 frame; /* frame word: 0x8000 colour from the data, 0x4000 skips a word */
    u8 *prim;
} Unk800D1BC4Ctx;

/* sprite frame word (the first u16 of a frame in the sprite data: btlfx_drawParticles, btlfx_drawSprite,
 * btlfx_drawSpriteAtOt). Evidence: [code] */
#define BTLFX_SPRF_COLOR 0x8000 /* an rgb word follows (else grey from the alpha) */
#define BTLFX_SPRF_SCALE 0x4000 /* a u16 x, y scale word follows (btlfx_drawParticles; the screen sprites skip it) */
#define BTLFX_SPRF_COUNT 0x3FFF /* sprites in the frame */

u8 *btlfx_emitScreenSprites(u8 *, Unk800D1BC4Ctx *, u8 *, s32 *);


#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

/* rtps without the leading GTE_NOPS (original scheduled code into the slots) */
#define gte_rtps_nn() __asm__ volatile(".word\t0x4A180001")

extern DR_OFFSET g_btluiDrOffset[2];
extern DR_AREA g_btluiDrArea[2];
extern DR_MODE g_btluiDrMode[2];
extern DR_STP g_btluiDrStp[2];
extern DR_ENV g_btluiSavedEnv[2];
extern RECT g_btluiTexWindowRect;

#define gte_lddp(r0) __asm__ volatile("mtc2\t%0, $8" : : "r"(r0))
#define gte_ldir(r0) __asm__ volatile( \
    "lwc2\t$9, 0(%0);" \
    "lwc2\t$10, 4(%0);" \
    "lwc2\t$11, 8(%0)" \
    : : "r"(r0))
#define gte_stir(r0) __asm__ volatile( \
    "swc2\t$9, 0(%0);" \
    "swc2\t$10, 4(%0);" \
    "swc2\t$11, 8(%0)" \
    : : "r"(r0) : "memory")
#define gte_rtv0tr_nn() __asm__ volatile(".word\t0x4A480012")

/* btl_drawShadow: project one corner of the 3x3 shadow grid */
#define SHADOW_PT(x, z) \
    corner.vx = (x); \
    corner.vy = 0; \
    corner.vz = (z); \
    gte_ldv0(&corner); \
    gte_rtps()

/* btl_drawShadow: shadow quad q of `shadow` in the current buffer */
#define SHADOW_QUAD(q) shadow->quads[g_gameState->gameData->drawBuf][q]

typedef struct {
    u32 color; /* rgb */
    s32 x; /* x offset (<< 2) */
    s32 y; /* y offset (<< 2) */
    s32 abr; /* abr << 5 */
    s32 tpage; /* tpage */
    s32 frame; /* frame word: 0x8000 colour from the data */
    s32 otZ; /* g_btlfxEnv->otZ */
} Unk800D1680Ctx;

void btlfx_emitParticles(u8 *, Unk800D1680Ctx *, u8 *, s32 *);

/* addPrim written as inline asm (len word in a register it clobbers) */
#define addPrimLen(ot, p, len) __asm__ volatile( \
    "lw\t$14, 0(%2);" \
    "nop;" \
    "sll\t$15, $14, 8;" \
    "srl\t$15, $15, 8;" \
    "or\t$15, $15, %0;" \
    "sw\t$15, 0(%1);" \
    "sll\t%0, %1, 8;" \
    "srl\t%0, %0, 8;" \
    "srl\t$15, $14, 24;" \
    "sll\t$15, $15, 24;" \
    "or\t$15, $15, %0;" \
    "sw\t$15, 0(%2)" \
    : "=r"(len) : "r"(p), "r"(ot), "0"(len) : "$14", "$15", "memory")

/* the same, but len is a plain input that the asm overwrites (only safe when len is dead afterwards);
   lets a constant len get its own register (btlfx_linkPrimAbr) */
#define addPrimLenIn(ot, p, len) __asm__ volatile( \
    "lw\t$14, 0(%2);" \
    "nop;" \
    "sll\t$15, $14, 8;" \
    "srl\t$15, $15, 8;" \
    "or\t$15, $15, %0;" \
    "sw\t$15, 0(%1);" \
    "sll\t%0, %1, 8;" \
    "srl\t%0, %0, 8;" \
    "srl\t$15, $14, 24;" \
    "sll\t$15, $15, 24;" \
    "or\t$15, $15, %0;" \
    "sw\t$15, 0(%2)" \
    : : "r"(len), "r"(p), "r"(ot) : "$14", "$15", "memory")

typedef struct {
    DR_STP mainStp; /* in the main OT (GameData.ot) */
    DR_STP bgStp; /* in the battle background OT (work->ot) */
    u8 pad18[0x30 - 0x18];
} BtlStpPair;
extern BtlStpPair g_btlFrameEndStp[2];
extern BtlStpPair g_btlFrameStartStp[2];
void btlui_resetVsyncCount(void);

/* main exe: candidates for ff9.h */
u32 *fade_getOt(s32);

void btl_spawnTickPopups(BtlUnit *);
void btl_tickStatusTimers(BtlUnit *);

/* BtlUnit.statusImmune viewed as one block (status timers at unkC..) */
typedef struct {
    s32 statusImmune;
    u32 autoStatus;
    u32 status;
    s16 venomTimer; /* dot timers (BtlUnit.dotTimers) */
    s16 poisonTimer;
    s16 regenTimer;
    s16 statusTimers[14]; /* BtlUnit.statusTimers */
    s16 jumpTimer; /* statusTimers[14]: Jump (bit 30) lands when it runs out */
} BtlUnit34;

/* status bits set in either status word */
static inline u8 BtlUnitHasStatus(BtlUnit *u, u32 m) {
    return (u->autoStatus & m) || (u->status & m);
}

/* disc cover open screen (btl_updatePause) */
extern u32 g_btlPauseOt[1];
extern DR_STP g_btlPauseStpOff;
extern TILE g_btlPauseDimTile;
extern DR_TPAGE g_btlPauseDrawMode;
extern DR_STP g_btlPauseStpOn;
extern u8 g_btlPauseSprite[];
void game_resumeClock(s32);
void akao_resume(s32);
void btlui_resumeFromPause(void);

s32 btl_loadWeaponModelsStep(s32, Unk800AA958 *);
s32 btl_loadPartyModelsStep(s32, Unk800AA958 *);
s32 btl_getWeaponModelHeader(s32);
BtlUnit *btl_getUnitBySlot(s32);
void btl_loadPartyMotions(BtlUnit *, s32, s32);
u8 *db_getModelTexPos(s32);

typedef struct {
    s16 state; /* state */
    u16 nextState; /* next state */
    s16 bank; /* sound bank index found (btl_findNearestFile), -1 = none */
    u8 pad6[8 - 6];
    union {
        s32 w[4];
        s16 h[8];
    } fileIds;
    u8 pad18[0x24 - 0x18];
    s32 buf;
} Unk800AA24C; /* model file load state (btl_loadSoundsStep) */

typedef union {
    u32 w;
    struct {
        u16 id;
        u8 type;
        u8 kind;
    } b;
} FileEntryId;

typedef struct {
    FileEntryId id;
    s32 sector; /* FileEntry.sector (the entries of a FileTable) */
    s32 sectors; /* FileEntry.sectors */
    s32 link; /* FileEntry.link */
} FileEntryX;

s32 btl_loadSoundsStep(s16, s32, Unk800AA24C *);
/* battle scene model load state (btl_loadEnemyModelsStep) */
typedef struct {
    s16 state; /* state */
    u16 nextState; /* next state */
    u8 pad4[8 - 4];
    s16 fileId; /* file id being loaded */
    s16 loaded; /* files loaded */
    s32 dst; /* load address */
    union {
        s32 w[2];
        s16 h[4];
    } fileIds; /* file ids still to load, -1 = done */
} Unk800AA504;

typedef struct {
    u16 modelId; /* id */
    u8 pad2[4 - 2];
    union {
        s32 w; /* low 24 bits: offset of the texture lists from this entry */
        u8 b[4]; /* b[3]: texture count */
    } tex;
    u8 pad8[0xC - 8];
} Unk800AA504Ent; /* 0xC */

typedef struct {
    u8 pad0;
    u8 count; /* entry count */
    u8 pad2[4 - 2];
    Unk800AA504Ent entries[1];
} Unk800AA504Tbl;

typedef struct {
    s16 tpage; /* tpage */
    s16 clut; /* clut */
} ModelTexWord;

typedef struct {
    s16 x; /* x */
    s16 y; /* y (CLUT row; becomes h) */
    s16 w;
    s16 h;
} ModelTexPos;

s32 btl_loadEnemyModelsStep(s32, s32, Unk800AA504 *);

/* inlined into btl_loadSoundsStep: append the ids of file-table entries of the
 * given type/kind to dst, skipping duplicates */
static inline void btl_loadSoundsStep_collect(s16 *dst, u32 type, u32 kind) {
    FileTable *t = g_gameState->gameData->depTable;
    FileEntryX *first = (FileEntryX *)t->entries;
    FileEntryX *e = &first[t->count] - 1;
    s16 *q = dst;
    s16 *s;
    u32 w;

    do {
        w = e->id.w;
        if (e->id.b.type == type && (w >> 24) == kind) {
            w &= 0xFFFF;
            s = q - 1;
            do {
                if (*s == w) {
                    goto next;
                }
                s--;
            } while (s >= dst);
            *q++ = w;
        }
    next:
        e--;
    } while (e >= first);
}

/* main exe: candidates for ff9.h */
void file_advanceLoadPtr(void *, s32);
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */

/* btlfx_drawRings: multi-frame sprite sheets */
#define gte_zero_vz012() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5" : : )

typedef struct {
    u16 frameCount; /* frame count */
    u8 unk2; /* not read */
    u8 unk3; /* not read */
    u8 flags; /* BTLFX_RINGS_SCALED */

    u8 type; /* ring type */
    u8 count; /* rings */
    u8 w;
} Unk800D23D8Hdr;

/* Unk800D23D8Hdr.flags (btlfx_drawRings). Evidence: [code] */
#define BTLFX_RINGS_SCALED 0x02 /* a per-frame x, y scale table (Unk800D23D8Scale) follows the header */


typedef struct {
    u16 x; /* per-frame x / y scale (with Unk800D23D8Hdr.flags BTLFX_RINGS_SCALED) */
    u16 y;
} Unk800D23D8Scale;

typedef struct {
    u32 color; /* rgb + primitive code */
    s32 clut; /* clut << 16 */
    u16 *scale; /* scale table */
    u16 *sizes; /* size table */
    u16 *dirs; /* direction table */
    u16 *frame; /* frame entry */
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 last; /* last ring index */
    s32 uvAdd; /* uv offset of the frame */
    s32 count;
    s32 tpage; /* tpage << 16 (abr bits added) */
    u8 pad2C[0x40 - 0x2C];
} Unk800D4768Ctx;

typedef struct {
    u32 code; /* primitive code */
    s32 fade; /* fade */
    u32 *scale; /* scale table */
    u32 *sizes; /* size table */
    u16 *dirs; /* direction table */
    u32 *colors; /* colour table (inner ring) */
    s32 otZ; /* g_btlfxEnv->otZ */
    s32 last; /* last ring index */
    u32 *colors2; /* colour table of the frame */
    s32 count;
    s32 abr; /* abr << 5 */
    u8 pad2C[0x40 - 0x2C];
} Unk800D4A30Ctx;

void btlfx_emitRingQuads(s32, Unk800D4768Ctx *, u8 *, s32 *);
void btlfx_emitRingGouraud(s32, Unk800D4A30Ctx *, u8 *, s32 *);
/* asm convention: color in $s1 (result in $s1), fade in $t1 (src_equiv/ovl_0b/rings.c) */
u32 btlfx_fadeColor(u32, s32);

/* GTE: load 3 halfwords into IR1-3; general purpose interpolation with base */
#define gte_ldsv(r0) __asm__ volatile( \
    "lhu\t$12, 0(%0);" \
    "lhu\t$13, 2(%0);" \
    "lhu\t$14, 4(%0);" \
    "mtc2\t$12, $9;" \
    "mtc2\t$13, $10;" \
    "mtc2\t$14, $11" \
    : : "r"(r0) : "$12", "$13", "$14")
#define gte_gpl12() __asm__ volatile(GTE_NOPS ".word\t0x4BA8003E")
#define gte_gpl0() __asm__ volatile(GTE_NOPS ".word\t0x4BA0003E")

/* work area of the effect step btlfx_opCast */
typedef struct {
    u8 kind; /* cast kind (op argument high nibble, 0xF = none): btlfx_startCastEmitter emitter kind */
    u8 phase; /* phase */
    u8 unit; /* unit id (0x10 + n) */
    u8 target; /* unit id of the target (+8 when bounced, 0xFF none) */
    u8 castMode; /* effect flags bits 12-13 */
    u8 motionMode; /* BTLFX_CAST_*: cast motion handling */
    u16 frame; /* frames run */
    s32 loopFrames; /* cast motion length */
    s16 pos[4]; /* emitter position */
    s16 pos2[4];
    s16 startRot[4];
    Pool6CEntry *emitter; /* cast emitter */
} Unk800E4C88; /* 0x28 */
/* btlfx_opCast motionMode [code] */
#define BTLFX_CAST_PARTY 0 /* party caster: CAST_START, CAST_LOOP, CAST_END, then READY */
#define BTLFX_CAST_ENEMY 1 /* enemy caster: no motions, a cast emitter on the body */
#define BTLFX_CAST_NONE  2 /* party command without a cast (BTL_FXK_NO_CAST or kind 0xF) */
#define BTLFX_CAST_ITEM  3 /* party item use (BTL_FXK_ITEM): BTL_MOTION_ITEM, then READY */

/* btlfx_opDrawOnUnit effect state (sprite sheet on a unit) */
typedef struct {
    u8 kind; /* BTLFX_DRAWON_* (resident op argument >> 6) */
    u8 unit; /* unit id, 0xFF = none (targets' centre) */
    s16 otBias; /* otZ for the unit (btlfx_addRadiusSixth) */
    s16 scale; /* sprite scale, -1 = none (btlfx_drawAnimQuads/btlfx_drawParticles arg 3) */
    s16 roll; /* z rotation of the sprites (op argument b << 8) */
    s16 dataId; /* common data id of the sprite set (btlfx_getCommonSprite) */
    s16 emitterId; /* common data id of the emitter (BTLFX_DRAWON_EMITTER) */
    s16 colorIdx; /* g_btlfxCommonCluts palette index */
    s16 frames; /* frame count (BTLFX_DRAWON_EMITTER: frame the emitter starts) */
    void *data; /* sprite set */
    s32 frame; /* frame */
    s16 offX; /* x offset (x4) */
    s16 offY; /* y offset (x4) */
    SVECTOR pos; /* position */
    s16 rot[3]; /* rotation (btlfx_aimAngles) */
} Unk800E57B0; /* 0x2C */
/* Unk800E57B0.kind (bits 6-7 of the op's mode byte): what btlfx_opDrawOnUnit draws. Evidence: [code] its two switches */
#define BTLFX_DRAWON_PARTICLES 0 /* common sprite set dataId, btlfx_drawParticles with offX/offY */
#define BTLFX_DRAWON_QUADS     1 /* common sprite set dataId, btlfx_drawAnimQuads */
#define BTLFX_DRAWON_EMITTER   2 /* starts common emitter emitterId on frame `frames` with sprite set dataId (btlfx_spawnDrawOnUnit) */
#define BTLFX_DRAWON_RINGS     3 /* btlfx_drawRings, fading out with cos */

extern u16 g_btlfxDrawScales[];
s32 btlfx_compareDepth(s32, s32);
s32 btlfx_addRadiusSixth(s32, s32);
void btlfx_getWeaponPos(s32, s32, s16 *);

void btlfx_getUnitPos(s32, s16 *);
void btlfx_getUnitBodyPos(s32, s32, s16 *);
void btlfx_startFadeBackground(s32, s32);
Pool6CEntry *btlfx_startCastEmitter(s32, s32, s16 *, s16 *);
/* The definition takes its 3rd-5th argument slots as one 12-byte struct by value (a2, a3, stack 0x30):
   it overwrites slots 4 and 5 with the 3rd (b = c = a) and reads them back from their stack homes, and the
   incoming 4th argument is never used. Callers pass two pointers through the prototype above, so the
   definition gets its own C name with the same assembler name. Native ports: callers' 4th argument is dead,
   and only a (slot 3) and the b = a copy are read. */
typedef struct {
    s16 *a;
    s16 *b;
    s16 *c;
} Unk800E4A3CArgs;
Pool6CEntry *btlfx_startCastEmitter_def(s32, s32, Unk800E4A3CArgs) __asm__("btlfx_startCastEmitter");
void btl_setMarkersVisible(s32);
void btlfx_showCasterCmdName(void);


/* btlfx_runScript: battle event script */
s32 btl_swapPartyModel(s32, s32);
s32 btl_reloadParty(s32);
void btlcam_beginEffect(void);
void btlcam_endEffect(void);
void btlfx_swapVramPages(s32, u32 *, s32);
s32 btlfx_findFileSlot(s32);
s32 btlfx_loadFile(s32);
s32 btlfx_loadNextBlock(void);
s32 btlfx_loadSoundBank(s32);
s32 btlfx_waitSoundBank(s32, s32);
void btlfx_startCasterTask(s32);
u32 btlfx_getUnitSlot(s32);
void btlfx_restorePartyLooks(void);
void btlfx_restoreUnitLook(s32);
s32 btl_areMarkersVisible(void);
void btlfx_updateCameraEnable(void);
void btlfx_playCmdCamera(s32);
void btlfx_stopAllSounds(void);
void btlfx_retryOp(void);
extern s32 g_btlfxVibData[];
extern u8 g_btlfxFileScript[];
extern const u8 s_btluiTranceLabel[]; /* "TRANCE" label */
/* BtlModel.texAnims seen as an array member: indexing it as an ARRAY_REF keeps `c ? 1 : 0` from folding into the address (btlfx_setUnitTexAnim) */
typedef struct {
    BtlUnit9C50 e[3];
} BtlUnit9C50Arr;
/* btl_placeParty: per-character battle setup row (5 bytes, indexed by Character.unk34.b.unk4) */
typedef struct {
    u8 shadowBone; /* BtlUnit.shadowBone (btl_placeParty) */
    u8 shadowDirBone;
    u8 shadowW; /* btl_setShadowSize */
    u8 shadowD;
    u8 shadowZ; /* BtlShadow.offsetZ */
} Unk800F57F0;
extern Unk800F57F0 g_btlPartyShadows[];
void btl_setShadowSize(BtlUnit *, s32, u32);
/* btlui_drawTranceLabel */
extern u8 g_btluiHighlightLabel[]; /* text measured for the highlight bar width */
extern CVECTOR g_btluiHighlightColors[4]; /* highlight bar gradient colours */
void btlui_drawPulseGradient(s32, s32, s32, s32, s32, CVECTOR *);
/* btlfx_updateLoader */
extern u16 g_btlfxFileHeader[];
extern s32 g_btlfxSummonTexPos[][2];
s32 btlfx_countVisibleParty(void);
void btlfx_initSummonModels(BtlModelBuf *, s32);
typedef void (*Unk800DA92CFn)(s32, Unk800F78A4 *, s32);
/* main exe: candidates for ff9.h */
s32 vib_init(void *);


/* GTE loads used by btlfx_drawScreenGrid */
#define gte_ldvz012_0() __asm__ volatile("mtc2\t$0, $1;" "mtc2\t$0, $3;" "mtc2\t$0, $5")
#define gte_ldxy3(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldxy0(r0) __asm__ volatile("lwc2\t$0, 0(%0)" : : "r"(r0))

/* btl_checkCmdUsable */
/* main exe: candidates for ff9.h */

/* the command's caster can't act (status 8 with flag 2): prints message 0x15 */
static inline u8 btl_checkCmdUsable_can(BtlCmd *cmd) {
    if ((cmd->caster->status & BTL_STATUS_SILENCE) && (cmd->ability->flags & BTL_ABIL_VOICE)) {
        btl_showMessage(0x15);
        return 0;
    }
    return 1;
}

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void btl_allocBuffers(void);
void btl_applyAutoAbilities(BtlUnit *unit);
s32 btl_applyReflect(BtlCmd *cmd);
void btl_autoAttack(BtlUnit *unit);
void btl_calcShadowMatrix(Unk800AD4E4 *shadow, BtlObj *obj, MATRIX *out, s32 groundY, s32 bone, s32 dirBone);
void btl_copyStats(Unk800BD8C0 *dst, Unk800BD8C0 *src);
s32 btl_getAnimFileId(s32 modelId);
u16 btl_getEntranceSound(void);
void btl_initBattleState(void);
s32 btl_initEncounter(Battle11C0 *work, s32 battleId);
void btl_initEnemies(Battle *battle);
void btl_initEquipElements(Character *c, BtlUnit *u);
void btl_initReturnInfo(void);
void btl_initWeapon(Character *chr, BtlUnit *unit);
s32 btl_loadBackgroundStep(Battle11C0 *work, s32 battleId, Unk800AA08C *task);
BtlPopup *btl_popupText(); /* (s32, u8, s16, s8); unprototyped: callers pass the argument unconverted */
void btl_queueAutoCmd(BtlUnit *unit, s32 cmdNo, s32 subNo, s32 targets);
void btl_reserveItem(s32 itemId);
s32 btl_saveParty(Battle *battle);
void btl_setUnitAttr(BtlUnit *unit, s32 attr, u32 value);
void btl_spreadTroubleDamage(BtlUnit *source);
void btl_startBattle(void);
void btl_tickPoison(BtlUnit *unit);
void btl_tickRegen(BtlUnit *unit);
void btl_tickVenomMp(BtlUnit *unit);
s32 btl_updateUnits(GameData *gd, Battle *battle);
void btlcam_update(void);
void btlfx_drawParticles(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha, s32 offX, s32 offY);
void btlfx_drawRings(u8 *data, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 alpha);
void btlfx_drawTrail(u8 *p, s32 newPoint, s32 *anchor, s32 frame, s32 scale, s32 alpha, s32 tp, u8 *out, s32 mode);
s32 btlfx_getScaledRadius(s32 unitId);
void btlfx_getStrikePos(s32 strikerId, s32 targetId, s16 *out);
void btlfx_getUnitBoneMatrix(s32 unitId, s32 bone, BtlBone *out);
void btlfx_getUnitBonePos(s32 unitId, s32 bone, u16 *out);
s32 btlfx_heapWrapIfFull(Heap *heap, s32 bytes);
void btlfx_rotateOffset(SVECTOR *rot, s32 dist, s32 out);
void btlfx_setTargets(BtlUnitList *list);
s32 btlfx_start(s32 effectId);
s32 btlfx_stepWaitSameSide(u8 *work, s32 env, s32 *entry);
void btlfx_stopEmitter(Pool6CEntry *emitter);
void btlfx_turnToward(s16 *target, s16 *rot, s32 rate);
void btlui_buildAbilityList(Unk80104F88Entry *entry);
void btlui_clearCursor(Unk800EB518 *saved);
s32 btlui_confirmDoubleCast(s32 targets);
void btlui_drawListScroll(Window *win);
void btlui_drawPanelHighlight(void);
void btlui_listConfirm(void);
void btlui_openListWindow(s32 kind);
void btlui_openTargetWindow(s8 openState);
void btlui_reset(void);
s32 btlui_resolveAbility(s32 ability);
void btlui_startLabelSlide(Unk800F412C *label, Unk800F3F10 *slide);
void btlui_targetMove(void);
void btlui_updatePanelRowColor(s32 row);
s32 clut_getX(s32 clut); /* main */
void btl_setMusicVolumeFade(s32 volume, s32 frames);
void btl_cancelTranceEidolon(BtlUnit *unit);
void btl_applyInvertedPower(Unk800BA938 *calc);
void btl_getWeaponOffset(Unk800BC1E0 *weapon, s16 *out);
void btl_orElements(u8 *dst, u8 *src);
s32 btlcam_getNextVariant(void);
void btlcam_orbitAnchor(s32 anchor, s32 anchorFlags, s16 *offset, s16 *out);
void btlfx_offsetPoint(SVECTOR *origin, s32 out, s32 axial, XY32 *axis, s32 radius, s32 spin);
void btlfx_forwardAngles(s16 *outRot, XY32 *axis, s32 spin);
void btlfx_toLocalPolar(SVECTOR *pos, SVECTOR *center, SVECTOR *axis, s32 outAxial, s32 outRadius, s32 *outAngle);
void btlui_updateTargetFlags(void);


/* Inlined copy of btl_killUnit (hp 0, KILLED flag, death cry, back to the idle motion); btl_setTargetHp and
 * btl_checkFrozenShatter expand it. */
static inline void btl_killUnit_inline(BtlUnit *unit) {
    BtlModel *model;
    unit->hp = 0;
    unit->flags |= BTL_UNIT_KILLED;
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        btl_playDeathCry(unit, g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->deathCry);
    }
    model = unit->model;
    if ((s32)model->motion != unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && (!(unit->flags & BTL_UNIT_PARTY) || (s32)model->motion != unit->motions[BTL_MOTION_READY]) && unit != PSX_NULLABLE(g_gameState->gameData->ctx.battle->curCmd)->caster) {
        model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        unit->obj->frame = 0;
    }
}

/* Inlined copy of btl_initStatusHitRate (status hit rate from level, ability rate and magic); btl_initUndeadHitRate
 * expands it. */
static inline void btl_initStatusHitRate_inline(BtlCalc *calc) {
    BtlUnit *caster = calc->caster;
    calc->hitRate = caster->level + (calc->cmd->ability->rate + (caster->magic >> 2)) - calc->target->level;
    if (calc->hitRate > 100) {
        calc->hitRate = 100;
    } else if (calc->hitRate <= 0) {
        calc->hitRate = 1;
    }
    calc->evade = calc->target->magicEva;
}

/* Forget a pending Dagger trance eidolon (btl_clearCmdQueue, btl_cancelTranceEidolon). */
static inline void btl_resetEidolon_inline(Battle *b) {
    b->eidolonTimer = 0;
    b->eidolonAbility = 0;
    b->cmdFlags &= ~(BTL_CMDF_EIDOLON_TIMER | BTL_CMDF_EIDOLON_QUEUED);
}

/* Inlined copy of btl_damageHp (subtract damage from a unit's hp, turn it to face front, hit motion or death);
 * btl_spreadTroubleDamage expands it per ally. */
static inline void btl_damageHp_inline(BtlUnit *unit, s32 damage, u8 hitMotion) {
    s32 facing;
    if (unit->status & BTL_STATUS_KO) {
        unit->popupFlags = BTL_POPUP_MISS;
        return;
    }
    if (unit->status & BTL_STATUS_PETRIFY) {
        unit->hpChange = 0;
        return;
    }
    if (unit != PSX_NULLABLE(g_gameState->gameData->ctx.battle->curCmd)->caster) {
        facing = (unit->flags & BTL_UNIT_PARTY) << 11;
        unit->obj->rotY = facing;
        unit->rot[1] = facing;
    }
    if (damage < unit->hp) {
        unit->hp -= damage;
    } else {
        unit->hp = 0;
    }
    unit->hpChange = damage;
    if (hitMotion) {
        btl_startHitMotion(unit);
        return;
    }
    if (unit->hp == 0) {
        btl_killUnit(unit);
    }
}

/* Inlined copy of btlui_clearMenuQueue (empty the battle menu's unit queue). */
static inline void btlui_clearMenuQueue_inline(void) {
    Unk80104F88 *ui = g_btlUi;
    ui->unk12C6 = 0;
    ui->queueHead = 0;
    ui->queueCount = 0;
}

/* Inlined copy of btlui_queueMenuUnit (append a unit to the battle menu's queue). */
static inline void btlui_queueMenuUnit_inline(BtlUnit *unit) {
    Unk80104F88 *ui = g_btlUi;
    unit->inMenuQueue = 1;
    g_btlUi->queue[(ui->queueHead + ui->queueCount) & 3] = unit;
    ui->queueCount++;
}

/* Inlined copy of btlui_openTargetWindow (open the target list window). */
static inline void btlui_openTargetWindow_inline(s8 openState) {
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    g_btlUi->windows[0] = win_open(&g_btluiTargetWindowRect, 5, WIN_STYLE_BEVEL, 0);
    win_setOpenAnim(g_btlUi->windows[0], WIN_ANIM_ZOOM_X);
    win_setOnOpened(g_btlUi->windows[0], btlui_onTargetWindowOpened);
    win_setDrawContent(g_btlUi->windows[0], btlui_drawTargetList);
    win_setDrawOverlay(g_btlUi->windows[0], btlui_drawTargetListOverlay);
    g_btlTarget.openState = openState;
    g_btlTarget.windowOpen = 1;
}

/* Inlined copy of btlui_closeTargetWindow (close the target list window if it is open). */
static inline void btlui_closeTargetWindow_inline(s32 closeState) {
    if (g_btlTarget.windowOpen != 0) {
        g_btlTarget.closeState = closeState;
        btlui_setMenuState(BTLUI_MENU_CLOSING);
        win_setOnClosed(g_btlUi->windows[0], btlui_onTargetWindowClosed);
        win_close(g_btlUi->windows[0]);
    }
}

#endif

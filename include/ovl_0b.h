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
    u8 unk3B;
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
    u16 *vertexCounts; /* per bone (BtlUnit9C8.boneCount): vertices bone b moves, consecutive from bone 0 (btl_projectMesh) */
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
    u8 tex; /* texture: BtlUnit9C.texPages index, model info uv offsets */
    u8 unk14;
    u8 flags; /* flags: 1 semi-transparent (abr = flags & 0xE0), 2 double-sided (no NCLIP) */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

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

/* skeleton bone of a model (BtlUnit9C8.bones, 4 bytes; ovl_0a WmBone): btl_calcBoneMatrices composes bone i's
 * matrix with its parent's (already built: parent < i) and a z offset of `length`; bone 0 is the root */
typedef struct {
    s16 length; /* offset from the parent's origin along the parent's z axis */
    u8 unk2;
    u8 parent; /* parent bone */
} BtlBoneDef;

/* model header (DB item 0x02xxxx: battle model files in FF9.IMG dir index 7, file id = model id; weapon and effect
 * models use the same format; ovl_0a WmModel). Layout and users: docs/battle-models.md */
typedef struct BtlUnit9C8 {
    u16 flags; /* 1 = relocated (btl_applyModelTextures turns the offsets below into pointers once) */
    u8 boneCount; /* bones: BtlMesh.vertexCounts entries, motion keys per frame (btl_projectMesh, btl_calcBoneMatrices) */
    u8 meshCount; /* mesh groups (meshes); bit i of BtlUnit9C.hiddenMeshes hides group i */
    u16 packetSize; /* sum of the groups' BtlMesh.packetSize */
    u8 unk6; /* ovl_0a: look-at bone (WmModel.lookAtBone) */
    u8 pad7;
    u16 height; /* btl_getUnitHeight */
    u16 radius; /* btlfx_getUnitRadius (party units) */
    s32 bones; /* BtlBoneDef[boneCount]: offset from the header, relocated on load (btl_applyModelTextures) */
    BtlMesh *meshes; /* meshCount groups: offset from the header, relocated on load */
} BtlUnit9C8;

/* motion (DB item 0x03xxxx, in the model file next to the model header; btl_calcBoneMatrices; ovl_0a WmAnim).
 * One key per bone of the model it plays on (BtlUnit9C8.boneCount), in bone order */
typedef struct BtlUnit9CC {
    u8 pad0[2];
    u16 frameCount; /* frames (btl_updateUnitModel wraps the unit's frame at it) */
    u16 rootX; /* root x: the value (constMask bit 0) or the offset of a per-frame s16 table */
    u16 rootY; /* root y (bit 1) */
    u16 rootZ; /* root z (bit 2) */
    u16 constMask; /* bits 0-2: rootX/Y/Z is the value itself */
    s32 rotKeys; /* offset, relocated on load: per-bone keys {u16 x, y, z, constMask}: bits 4-11 of the angles (4096 = 360 deg), a constant or the offset of an s8 per-frame table */
    s32 rotKeysLo; /* offset, relocated on load: the same for bits 0-3 (per-frame tables: two frames per byte, even frame in the low nibble), or 0 */
} BtlUnit9CC;

/* bone matrix (BtlUnit9C.bones, built by btl_calcBoneMatrices), laid out as a MATRIX: model/world space of the bone
 * (BtlUnit9C.matrix applied), 4096 = 1.0. docs/battle-models.md "Motions" */
typedef struct {
    s32 rot[5]; /* rotation, s16 m[3][3] (row-major) + pad: parent rotation . Rz Ry Rx of the motion key */
    s32 tx; /* translation: parent's + parent rotation . (0, 0, BtlBoneDef.length); bone 0: the root translation */
    s32 ty;
    s32 tz;
} BtlBone; /* 0x20 */

typedef struct BtlUnit9C50 {
    u8 pad0[4];
    u8 flags; /* TexAnim.flags (the entries seen 4 bytes early): 1 playing, 2 stop at the end */
    u8 pad5[0xC - 5];
    s32 pos; /* TexAnim.pos */
    u8 pad10[0x12 - 0x10];
    s16 lastFrame; /* TexAnim.lastFrame (0x1000 forces a redraw) */
} BtlUnit9C50; /* 0x14 */

/* texture word of a model, one per texture index (WmSrc*.tex): the model info entry's first table (ovl_0a WmTexInfo).
 * btl_drawMesh puts clut in the first uv word and tpage in the second; btl_setStoneClut moves clut */
typedef struct {
    s16 tpage;
    s16 clut;
} BtlUnit9C18;

/* FileEntry viewed with its first word split (file_find type 0xD) */
typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
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
            u8 unk2; /* kind */
        } b;
    } data;
    u8 pad4[0x10 - 4];
} BtlAnimEvt; /* 0x10 */

typedef struct {
    u16 unk0;
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
    u8 unk0;
    u8 phase; /* phase */
    u8 unit; /* unit id */
    u8 stopped; /* the unit has Stop: moves home without its motion (btlfx_moveUnitHome) */
    u8 pad4[0xC - 4];
    s16 unkC[8];
    s16 unk1C[6];
} Unk800E5538; /* 0x28 */

void btlfx_turnUnitTowardFast(s32, s16 *, s32, s32, s32);
s32 btlfx_moveUnitHome(s32, s32, s16 *, s32);
s32 btlfx_isCmdPendingOnAutoPotion(s32);

extern u8 g_btlSwdMagSourceAbilities[]; /* character 1 ability ids per Sword Magic entry */

/* GPU mask-bit setting word (GP0 E6, what libgpu's SetDrawStp writes): set = draw with the mask bit set,
 * check = don't draw over masked pixels. Candidate for psyq.h. */
#define GPU_STP(set, check) (0xE6000000 | ((check) << 1) | (set))

/* main exe: candidates for ff9.h */
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */
typedef struct {
    u8 unk0;
    u8 apNeeded; /* ap needed */
} Unk8006CB88;
extern Unk8006CB88 D_8006CB88[]; /* abilities */

/* draw environment pair built in the primitive buffer by btlfx_drawBgOffscreen */
typedef struct {
    DRAWENV env;
    DRAWENV prev; /* previous */
} DrawEnvPair; /* 0xB8 */

/* same as a GNU statement expression: the operand order around it differs
 * from the inline function (btl_applyReflect) */
#define RAND8() ({ if (g_randPtr >= g_randTableEnd) { rand_refill(); } (s32)*g_randPtr++; })

/* next byte of the random table, refilling it when exhausted (inlined) */
static inline s32 rand8_inline(void) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return *g_randPtr++;
}

/* one primitive in a model's packed GPU packet buffer (BtlUnit9C.prims) */
typedef union {
    P_TAG tag;
    u32 w[13]; /* w[1], w[3]/w[4], ...: rgb + code words */
} BtlPrim;

typedef struct BtlUnit9C {
    u16 flags; /* 2 scaled (scaleX-Z, btl_applyModelScale), 0x10 off screen (btl_updateModelVisibility) */
    u8 pad2[4 - 2];
    u8 parentBone; /* weapon models: the bone of the parent model they follow (Character.unk21, btl_initWeapon) */
    u8 abr; /* semi-transparency mode (btl_setModelAbr) */
    u8 pad6[8 - 6];
    BtlUnit9C8 *header;
    BtlUnit9CC *motion; /* playing motion (BtlUnit.motions entries) */
    s32 prims; /* built primitive packets (btl_setModelAbr walks them) */
    u32 hiddenMeshes; /* mesh-group bits not drawn (btl_hideMeshes) */
    BtlUnit9C18 *texPages;
    struct BtlUnit9C *parent; /* parent model (weapon models: the unit model) */
    BtlBone *bones; /* bone matrices (btl_calcBoneMatrices) */
    MATRIX matrix; /* model matrix */
    s32 unk44;
    u8 pad48[0x50 - 0x48];
    BtlUnit9C50 *texAnims; /* texture animations (btl_updateTexAnims) */
    s16 scaleX; /* with flag 2 (btl_applyModelScale) */
    s16 scaleY;
    s16 scaleZ;
    s16 sliceY; /* y limit of the slice effect (Hi_DrawSliceEffModel, btlfx_projectSliceMesh) */
    BtlUnit9C8 *morph; /* morph target */
    void *unk60;
    u8 pad64[0x74 - 0x64];
    s16 attachX; /* offset from the parent bone (btl_calcBoneMatrices; event op 0xD4 writes it through main's Unk8004AFF0) */
    s16 attachY;
    s16 attachZ;
} BtlUnit9C;

/* GameData.unk8E0 entry as btl_registerModel fills it (ff9.h's ModelSlot has pad4) */
typedef struct {
    s32 flags; /* flags (1 loaded) */
    s32 modelId; /* model id */
    BtlUnit9C *model;
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
/* BtlUnit.status bits (docs/runtime-findings.md "Battle STATUS bits"; 0x4 unidentified). BtlUnit.autoStatus uses the same
 * bits (btl_hasStatus tests both words). */
#define BTL_STATUS_PETRIFY         0x00000001 /* runtime: Break; model CLUT turns to stone (btl_setStoneClut) */
#define BTL_STATUS_VENOM           0x00000002 /* code: the tick drains HP and MP (btl_tickPoison, btl_tickVenomMp) */
#define BTL_STATUS_SILENCE         0x00000008 /* runtime: Silence */
#define BTL_STATUS_BLIND           0x00000010 /* runtime: Blind */
#define BTL_STATUS_TROUBLE         0x00000020 /* code: btl_spreadTroubleDamage splashes half the damage on allies */
#define BTL_STATUS_ZOMBIE          0x00000040 /* code: healing hurts, drain reversed */
#define BTL_STATUS_DEATH_IMMUNE    0x00000080 /* code: instant death/gravity formulas fail on it */
#define BTL_STATUS_KO              0x00000100 /* runtime: set when the monster died */
#define BTL_STATUS_LOW_HP          0x00000200 /* added when hp * 6 < maxHp */
#define BTL_STATUS_CONFUSE         0x00000400 /* runtime: Confuse */
#define BTL_STATUS_BERSERK         0x00000800 /* runtime: Berserk */
#define BTL_STATUS_STOP            0x00001000 /* runtime: Stop */
#define BTL_STATUS_AUTO_LIFE       0x00002000 /* runtime: Auto-Life; KO queues the revive command 0x3D */
#define BTL_STATUS_TRANCE          0x00004000 /* runtime */
#define BTL_STATUS_DEFEND          0x00008000 /* runtime: removed by btl_setCmd at every action */
#define BTL_STATUS_POISON          0x00010000 /* runtime: Poison */
#define BTL_STATUS_SLEEP           0x00020000 /* runtime: Sleep; timer at +0x48 (unk46[1]) */
#define BTL_STATUS_REGEN           0x00040000 /* runtime: the Regen spell adds it (btl_addStatus); timer at +0x44 (unk40[2]) */
#define BTL_STATUS_HASTE           0x00080000 /* runtime: Haste */
#define BTL_STATUS_SLOW            0x00100000 /* runtime: Slow */
#define BTL_STATUS_FLOAT           0x00200000 /* runtime: Float */
#define BTL_STATUS_SHELL           0x00400000 /* runtime: Shell */
#define BTL_STATUS_PROTECT         0x00800000 /* runtime: Protect */
#define BTL_STATUS_HEAT            0x01000000 /* code: acting while it is set adds KO (btl_processCmdQueue) */
#define BTL_STATUS_FREEZE          0x02000000 /* runtime */
#define BTL_STATUS_VANISH          0x04000000 /* runtime: Vanish; hides the meshes (btl_hideMeshes) */
#define BTL_STATUS_DOOM            0x08000000 /* code: countdown number (btl_drawStatusCountdown) */
#define BTL_STATUS_MINI            0x10000000 /* runtime: Mini */
#define BTL_STATUS_REFLECT         0x20000000 /* runtime: Reflect */
#define BTL_STATUS_JUMP            0x40000000 /* code: in the air (btl_checkCmdUsable) */
#define BTL_STATUS_GRADUAL_PETRIFY 0x80000000 /* code: countdown number (btl_drawStatusCountdown) */

/* BtlCmd.cmdNo. 0x00-0x2F index the command table g_btlCmdTable; the names are its menu texts (g_btlCmdNames; the table's
 * own name for a repeated text gets a number). Ids from 0x30 up are queued by the battle code itself. */
#define BTL_CMD_NONE          0x00
#define BTL_CMD_ATTACK        0x01 /* runtime: Attack (ability 0xB0) */
#define BTL_CMD_STEAL         0x02 /* ability 0xB1 Steal (0xB5 Mug with a support bit, btl_setCmd) */
#define BTL_CMD_JUMP          0x03 /* runtime: Jump; ability 0xB2 */
#define BTL_CMD_DEFEND        0x04 /* runtime: Defend */
#define BTL_CMD_FLEE          0x05 /* runtime: Flee */
#define BTL_CMD_CHANGE        0x07 /* runtime: Change (row) */
#define BTL_CMD_EAT           0x08 /* runtime: Eat */
#define BTL_CMD_COOK          0x09
#define BTL_CMD_SPEAR         0x0A /* code: the landing of Jump (cmd 3) queues it with ability 0xB9 Spear */
#define BTL_CMD_SPEAR_TRANCE  0x0B /* code: the landing of cmd 0xC queues it with ability 0xBA Spear */
#define BTL_CMD_JUMP_TRANCE   0x0C /* table text Jump, ability 0xAC */
#define BTL_CMD_FOCUS         0x0D /* runtime: Focus */
#define BTL_CMD_ITEM          0x0E
#define BTL_CMD_THROW         0x0F /* runtime: Throw */
#define BTL_CMD_SUMMON        0x10 /* code: btl_rollShortSummon handles 0x10/0x12/0x14 */
#define BTL_CMD_WHT_MAG       0x11 /* runtime: Wht Mag */
#define BTL_CMD_EIDOLON       0x12
#define BTL_CMD_WHT_MAG2      0x13
#define BTL_CMD_SUMMON2       0x14
#define BTL_CMD_DBL_WHT       0x15
#define BTL_CMD_BLK_MAG       0x16 /* runtime: Blk Mag */
#define BTL_CMD_DBL_BLK       0x17
#define BTL_CMD_BLU_MAG       0x18 /* runtime: Blu Mag */
#define BTL_CMD_SKILL         0x19 /* runtime: Skill */
#define BTL_CMD_DYNE          0x1A
#define BTL_CMD_DRAGON        0x1B
#define BTL_CMD_FLAIR         0x1C
#define BTL_CMD_ELAN          0x1D
#define BTL_CMD_SWD_ART       0x1E
#define BTL_CMD_SWD_MAG       0x1F
#define BTL_CMD_SEIKEN        0x20
#define BTL_CMD_SEIKEN2       0x21
#define BTL_CMD_WHT_MAG3      0x22
#define BTL_CMD_ENEMY_TURN    0x2F /* runtime: the enemy AI's command (event-script trigger 0x2F, btl_setEnemyCmd) */
#define BTL_CMD_TABLE_COUNT   0x30 /* entries in g_btlCmdTable; lower ids are chosen commands, higher ones automatic */
#define BTL_CMD_COUNTER       0x31 /* runtime: Counter queues it with Attack against the attacker (btl_tryCounter) */
#define BTL_CMD_RETURN_MAGIC  0x32 /* code: Return Magic queues it with the spell that hit (btl_tryCounter) */
#define BTL_CMD_AUTO_POTION   0x33 /* code: btl_tryAutoPotion queues it with the item; handled like Item */
#define BTL_CMD_CHARGE_ATTACK 0x34 /* code: formula 0x3D (ability Charge!) queues an Attack by each low-HP party unit */
#define BTL_CMD_ENEMY_COUNTER 0x35 /* code: enemy command from the event script (trigger 0x35 = hit) */
#define BTL_CMD_ENEMY_DEATH   0x36 /* code: enemy command from the event script (trigger 0x36 = killed) */
#define BTL_CMD_ENEMY_REACT   0x37 /* code: btl_startCmdEffect plays a sequence for it like 0x2F/0x35/0x36 */
#define BTL_CMD_ESCAPE        0x38 /* runtime: queued by btl_rollFlee / btl_tryEscape; ability 0xB4 Flee */
#define BTL_CMD_TRANCE_EIDOLON 0x39 /* code: trance Eidolon timer (btl_updateUnitStatus) casts Battle.eidolonAbility */
#define BTL_CMD_REBIRTH_FLAME 0x3A /* code: the item 0xF9 chance in btl_checkSideDefeated, ability 0x49 Rebirth Flame */
#define BTL_CMD_TRANCE        0x3B /* code: adding/removing BTL_STATUS_TRANCE queues it; effect 0x101 in trance */
#define BTL_CMD_KO            0x3C /* code: adding BTL_STATUS_KO queues it (btl_addStatus) */
#define BTL_CMD_AUTO_LIFE     0x3D /* code: KO with BTL_STATUS_AUTO_LIFE queues it */
#define BTL_CMD_PETRIFY       0x3E /* code: adding BTL_STATUS_PETRIFY during combat queues it */

/* BtlCmd.subNo / Battle.abilities indices with a fixed use in the code (names from the ability text g_btlAbilityNames) */
#define BTL_ABIL_REBIRTH_FLAME 0x49
#define BTL_ABIL_ATTACK        0xB0
#define BTL_ABIL_FLEE          0xB4
#define BTL_ABIL_MUG           0xB5
#define BTL_ABIL_SPEAR         0xB9
#define BTL_ABIL_SPEAR_TRANCE  0xBA
#define BTL_ABIL_TERRA_HOMING  0xBB
#define BTL_ABIL_THROW         0xBE

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
/* btl_setBattleVar ids */
#define BTL_SETVAR_STOP_COMBAT    32 /* phase END with subPhase 2: the units settle, then BTL_PHASE_SCRIPT */
#define BTL_SETVAR_END            33 /* in BTL_PHASE_SCRIPT: ends the battle with result n (GameData.unk56E) */
#define BTL_SETVAR_DEFEAT         34 /* in BTL_PHASE_SCRIPT: BTL_PHASE_EXIT_DEFEAT, result 3 */
#define BTL_SETVAR_RESUME         35 /* in BTL_PHASE_SCRIPT: back to BTL_PHASE_ENTRANCE, then combat */
#define BTL_SETVAR_PLAY_CAMERA    36 /* in BTL_PHASE_SCRIPT: btlcam_playScene(btl_getSceneData(n)) */
#define BTL_SETVAR_NEXT_LOCATION  37 /* evt_setNextLocation(n) */
#define BTL_SETVAR_ADD_GIL        38 /* gil += n, at most 9999999 */

/* element bits (BtlUnit.elem*, BtlCmd8.element, weapon[6]); names from the element texts btl_getElementName returns */
#define BTL_ELEM_FIRE    0x01
#define BTL_ELEM_ICE     0x02
#define BTL_ELEM_THUNDER 0x04
#define BTL_ELEM_EARTH   0x08 /* misses Float and flying enemies (btl_missIfAirborne) */
#define BTL_ELEM_WATER   0x10
#define BTL_ELEM_WIND    0x20
#define BTL_ELEM_HOLY    0x40
#define BTL_ELEM_SHADOW  0x80

/* btl_hasEnemyFlag categories (enemy type record unk2) */
#define BTL_ENEMY_UNDEAD 0x10 /* code: Regen and healing hurt it like Zombie (btl_tickRegen, btl_calcHeal) */
#define BTL_ENEMY_FLYING 0x80 /* code: Earth attacks miss it (btl_missIfAirborne) */

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
#define BTL_ATTR_HIDE_MESHES   0x3B
#define BTL_ATTR_SHOW_MESHES   0x3C
#define BTL_ATTR_ADD_REWARDS   0x47 /* set: btl_addEnemyRewards */
#define BTL_ATTR_STRENGTH      0x48
#define BTL_ATTR_MAGIC         0x49
#define BTL_ATTR_DEFENCE       0x4A
#define BTL_ATTR_EVADE         0x4B
#define BTL_ATTR_MAGIC_DEFENCE 0x4C
#define BTL_ATTR_MAGIC_EVADE   0x4D

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
    u32 flags; /* bit 0 party, 1-4 slot, 5-7 id, 8-9 row, 16 ATB fills, 18 weak stance, 21 motion held, 22 hidden, 23 model part, 24 shadow shown (btlseq_opShowShadow), 28 targetable, 29 targeted */
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
    u8 koState; /* KO state machine step (btl_updateKo) */
    u16 popupFlags; /* btl_spawnPopups: BtlCalc.targetResult bits (1 hp, 2 heal, 4 critical, 8 mp, 0x10 mp heal), 0x20 miss, 0x40 killed, 0x80 guard */
    s16 hpChange; /* damage or heal shown by btl_spawnPopups */
    s16 mpChange;
    u16 modelId;
    s16 rot[3]; /* rot[1] = facing */
    s32 pos[3]; /* position, 16.16 */
    struct BtlUnit98 *home; /* home position block */
    BtlUnit9C *model;
    BtlUnitA0 *obj; /* model object entry (GameData.unk8E0): start position/rotation, motion frame */
    s32 *motions; /* motion table (BtlUnit9CC * per slot); model->motion is the playing one */
    BtlUnit9C *weaponModel;
    u16 hiddenMeshes; /* BTL_ATTR_HIDE_MESHES / BTL_ATTR_SHOW_MESHES */
    u16 vanishMeshes; /* mesh groups Vanish hides */
    u16 unitBit; /* bit in target masks (BtlCmd.targets): party 0-3, enemies from 4 */
    u8 targetableReq; /* 2: become untargetable, 3: targetable (btlui_applyTargetable applies it next frame and leaves 0/1); BTL_ATTR 0x34, Jump, landing */
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
    u8 tickFlags; /* 1 regen, 2 poison, 4 venom, 8 regen hurts */
    u8 inMenuQueue; /* waiting for its command menu (btlui_queueMenuUnit) */
} BtlUnit; /* 0xD0 */

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
    u16 flags; /* 1 miss, 2 dodged, 8 absorbed, 0x40 guard, 0x80 hit without damage, 0x100 add the weapon status */
    u8 casterResult; /* caster popup flags (BtlUnit.popupFlags) */
    u8 targetResult; /* target popup flags: 1 hp, 2 heal, 4 critical, 8 mp, 0x10 mp heal */
    s16 casterHpChange; /* caster hp change */
    s16 targetHpChange; /* target hp change */
    s16 casterMpChange; /* caster mp change */
    s16 targetMpChange; /* target mp change */
} BtlCalc;

/* BtlCmd as the sequence interpreter reads it (BtlScript.cmd) */
typedef struct {
    u8 pad0[4];
    struct BtlUnit *caster;
    struct BtlCmd8 *ability;
    u16 targets;
} BtlScript4;

typedef struct BtlScript {
    u32 flags; /* 1 waiting for the effect hit, 2 hit seen, 4 end motion done, 8 started without a command (btlseq_startDirect) */
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
    u8 targetCount;          /* count of unk4 */
    u8 fxArgs[2]; /* btl_playCmdEffect arg2[0-1] - unit ids (btlfx_setPartnerByArg) or bone numbers (btlfx_getUnitArgBone*) */
    u8 bounceCount;          /* count of unk28 */
    BtlUnit *bounced[4]; /* ids 8..: units a reflected spell bounces to (btl_playCmdEffect) */
    s16 centerX; /* average target position (btl_playCmdEffect) */
    s16 centerY;
    s16 centerZ;
    u8 pad3E[0x40 - 0x3E];
    u16 kind; /* btl_playCmdEffect: 1 Eidolon follow-up, 2 item, 4 Swd Mag, 0x11 bounce */
    u16 fxParam; /* btl_playCmdEffect arg2[2] (fx_031: distance ahead of the fxArgs bones) */
    BtlUnit *partner; /* id 0x11: the Swd Mag partner (character kind 2) */
    u8 pad48[0x4C - 0x48];
} BtlUnitList; /* 0x4C */

typedef struct Unk800F78A40 {
    s16 phase; /* -1 idle, else the btlfx_runScript state (0 save VRAM .. 4 run ops) */
    s16 effectId; /* effect being played (btlfx_start, after btlfx_resolveFirstUse) */
    u8 pad4[0x8 - 4];
    s16 file; /* effect file slot of the script (btlfx_getFileData) */
    u8 padA[0xF - 0xA];
    u8 seqRelease; /* set by btlseq op 0A; control op 8 waits for it with an enemy caster */
    u8 pad10[0x1C - 0x10];
    u32 flags;
    u8 retry; /* btlfx_retryOp: run the same op again next frame */
    u8 pad21[0x23 - 0x21];
    u8 seqSync; /* Unk800FEC40.seqSync */
} Unk800F78A40;

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
    u8 *apCounters[0xC0]; /* -> Character.unk58[], AP gathered per ability id */
    u8 apNeeded[0xC0];
    u8 fromEquip[0xC0]; /* abilities available from equipment */
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
    u8 unk12C6;
    u8 pad12C7;
    struct BtlUnit *queue[4];
} Unk80104F88;
extern Unk80104F88 *g_btlUi;

typedef struct {
    s16 itemCount; /* count of items */
    u16 throwCount; /* count of throwItems */
    UnkPair *items[31]; /* inventory slots with item ids >= 0xE0 (btlui_buildItemList) */
    UnkPair *throwItems[0x58]; /* inventory slots of throwable weapons (g_weaponTable flag 4, btlui_buildThrowList) */
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
    u8 unk5;
    u8 started; /* btlui_init ran */
    u8 pad7;
    u8 hideWindows; /* Select (pad bit 19) held: btlui_drawWindows skips the windows */
    u8 unk9;
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
    s8 unk14;
    u8 port; /* controller port of the last command menu (btlui_onCmdMenuOpened, pad_updateRepeat) */
    u8 pad16[0x18 - 0x16];
    s32 vsyncTick; /* btlui_vsync calls (blinks the port icon, btlui_drawPortIcons) */
    s32 vsyncCount; /* vsyncs since btlui_resetVsyncCount (btlui_waitFrame) */
    s32 unk20;
    s32 breakMisses; /* BreakDraw found nothing to break (btlui_frame) */
} Unk80104E68;
extern volatile Unk80104E68 g_btluiState;

typedef struct {
    s16 phase; /* -1 idle, else the btlfx_runScript state (0 save VRAM .. 4 run ops) */
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
        s32 w; /* cleared as a word */
        u8 b[4]; /* b[2]: done */
    } signals; /* control op 2 sets b[a], op 1 waits for b[b] (b[2]: the caster task finished) */
    s8 motionDone; /* unit ops: the motion op waited for has ended (btlfx_stepPlayMotionWait) */
    s8 moveDone; /* unit ops: the move op has arrived (btlfx_opRunHome, btlfx_opJumpForward, ...) */
    s8 turnDone; /* unit ops: the turn op has finished (btlfx_opFaceTarget, btlfx_opFaceHome) */
    s8 waitDone; /* unit ops 0x79-0x7B: the side/timing wait finished */
    u8 *pc; /* script pointer, 3-byte commands */
    s32 flags;
    u8 retry; /* btlfx_retryOp: run the same op again next frame */
    u8 ending; /* end op: cleanup ran, waiting for btlfx_reloadCommonData */
    u8 partyReload; /* control op 11: btl_reloadParty running */
    u8 seqSync; /* control op 8 ran with an enemy caster (btlseq waits for it) */
    u8 modelSwap; /* control op 13: unit slot + 1 whose model btl_swapPartyModel swaps */
    u8 castEnded; /* the cast op (btlfx_opCast) finished its caster motion; the loader holds common texture blocks (kind 9) until then */
} Unk800FEC40;
extern Unk800FEC40 g_btlfxState;

typedef struct {
    s32 sector; /* CD sector of the next read */
    s32 effectId; /* effect file being loaded (btlfx_loadFile) */
    s32 slot; /* buffer slot 0/1 of the current block */
    s16 state; /* state machine step (btlfx_updateLoader), 0 idle */
    s16 stage; /* 1 data read, 2 textures read, 3 module ready (control op 4 waits for it) */
    s16 blockType; /* block header word 0: 1 loads into the second module buffer in slot 1 */
    s16 cmdsLeft; /* commands left in the current block */
    u8 pad14[0x16 - 0x14];
    u8 cmdKind; /* command kind: 0/1 textures, 2 data, 3 module code, 4/5/6 summon, 7 sound bank, 8 load buffer, 9 common textures, 10 data ring */
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
    s16 summonState; /* kind 4: 1 reading, 2 summon model registered */
    u8 summonDataState; /* kind 6: 1 reading, 2 read */
    u8 commonTexLoaded; /* kind 9 loaded (control op 6 a=2 waits for it) */
    struct Unk80100DC054 *summon; /* summon file in Battle.partyBufs[0] */
    s32 workCursor; /* summon work area (btlfx_takeWork) */
    s32 workLeft;
    s32 summonModel; /* Hi_RegisterSummonModel result */
    u8 soundState; /* kind 7: 1 reading, 2 read */
    u8 soundBlocks; /* kind 7 blocks read */
    u8 soundUploading; /* sound bank upload running (btlfx_waitSoundBank) */
    u8 soundUploaded; /* btlfx_loadSoundBank ran: the loader may continue */
    u8 bufState; /* kind 8: 1 reading, 2 read (control op 12 waits for it) */
    u8 commonTexCount; /* kind 9 blocks loaded (control op 7 a=2 waits for b) */
} Unk80100DC0;
extern Unk80100DC0 g_btlfxLoader;

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
extern s32 g_btlbgLayerMask;
extern s32 g_btlbgSkyAngle;
extern struct BtlUnit9C50 *g_btlbgTexAnims;
extern s32 g_btlbgSkyOwnProj;
/* battle background: texture scroll entry (g_btlbgScrolls) */
typedef struct {
    s16 uvOfs; /* the uv pair it moves, offset from the first group (Battle11C0.bgData) */
    u8 mode;   /* 0 = wrap (step back period x (du, dv)), 1 = bounce (negate du, dv) */
    u8 du;
    u8 dv;
    u8 step;   /* frames since the last wrap/bounce (written at run time) */
    u8 period; /* frames per cycle */
    u8 pad7;
} BtlBgScroll;

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
    u16 layer; /* 8 sky (turns with g_btlbgSkyAngle), 4, 2, 0 (btlbg_draw, g_btlbgLayerMask bits 8/4/2/1);
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
extern s32 *g_btlfxCommonTable; /* offset table, entries relative to its start */
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
    BtlUnit9CC *motions[6]; /* motion table (BtlUnit.motions) */
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
    u32 flags; /* bit 6 its death triggers the event script (btl_applyCalcResult), 8 model owner (btl_findBodyUnit), 9 no model */
    union {
        s32 w;
        u8 b[4]; /* items to steal, 0xFF = none (btl_calcSteal) */
    } steal;
    u8 home[0x18 - 0xC]; /* home position block (BtlUnit.home) */
} BtlEnemy; /* 0x18 */

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
    BtlUnit9CC **anims; /* animations the sequences play (btlseq_init) */
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
    u8 unk0;
    u8 groupCount; /* count of groups */
    u8 typeCount; /* count of enemy type records following the groups */
    u8 abilityCount; /* count of enemy abilities (BtlCmd8) following the enemy records */
    u16 flags; /* Battle.scene.flags bits 2 and up (btl_initSceneFlags) */
    u8 pad6[8 - 6];
    Battle1330 groups[1]; /* enemy groups (btl_pickEnemyGroup) */
} Battle11C020;

/* battle command (queued action) */
typedef struct BtlCmd8 {
    u32 targetType : 4; /* target type; 6-12 make enemy commands multi-target (btl_setEnemyCmd), 3-5 spread casts (btl_halveForSpread) */
    u32 defaultSide : 1; /* target select starts on this side (Battle16CC.defaultSide) */
    u32 listKind : 3; /* nonzero: targets are picked from a list window (Battle16CC.listKind) */
    u32 effect : 9; /* effect id (btl_startCmdEffect -> btl_playCmdEffect) */
    u32 unk0_17 : 15;
    u8 formula; /* btl_calcCmd formula */
    u8 power; /* power, or the formula's parameter (an item id, a divisor) */
    u8 element; /* element bits */
    u8 rate; /* hit/status rate */
    u8 flags; /* 1 reflectable, 2 needs a voice (Silence blocks it), 8 Counter answers it, 0x80 Return Magic answers it */
    u8 statusSet; /* index into Battle.statusSets */
    u8 mpCost; /* runtime: Fire 6, Fira 12, Sleep 10 as the spell list shows */
    u8 unkB;
    u16 altEffect; /* effect played for short summons, Seiken and single-target casts (btl_startCmdEffect) */
    u16 nameOfs; /* name text offset (runtime: Fire 161, Fira 166, Firaga 171, Sleep 178 - steps = name lengths + 1) */
} BtlCmd8;

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
            u32 unk0_2 : 2;
            u32 automatic : 1; /* set for the ids above BTL_CMD_TABLE_COUNT (btl_setCmd, btl_setEnemyCmd) */
            u32 covered : 1; /* retargeted to a Cover unit (btl_findCover) */
            u32 dodged : 1; /* the target dodged; its step aside is undone at the end (btl_applyCalcResult) */
            u32 reflect : 2; /* 2 = bounce pending, 1 = bounced (btl_playCmdEffect, btl_onCmdEffectDone) */
            u32 magicFailed : 1; /* btl_rollMagicFail: the spell misses (btl_calcCmd formula 0x12) */
            u32 shortSummon : 1; /* plays the ability's altEffect (btl_rollShortSummon) */
            u32 enemyBounce : 1; /* an enemy's reflected spell: btl_processCmdQueue steps it like a party command (btl_updatePartyCmd) */
        } b;
        u32 w; /* cleared as a word */
    } info;
} BtlCmd;

typedef struct BtlShadow {
    u32 shade : 8; /* grey level of the shadow quads (btl_drawShadow) */
    u32 width : 12; /* btl_setShadowSize */
    u32 depth : 12;
    u32 unk4_0 : 12;
    u32 offsetX : 10; /* x offset of the shadow (Unk800AD4E4, btl_calcShadowMatrix) */
    u32 offsetZ : 10; /* z offset */
    POLY_FT4 *unk8[2][4]; /* shadow quads, per draw buffer */
} BtlShadow; /* 0x28 */

typedef struct Battle11C0 {
    s32 unk0;
    s16 returnId; /* field id or world position to return to (btl_initReturnInfo) */
    u8 returnMode; /* GameState mode before the battle (btl_cleanup) */
    u8 unk7[1];
    s32 freeHeap; /* g_modeHeap once the shadows are allocated (btl_load, btl_initReturnInfo); never read */
    s32 *loadBuf; /* loader work buffer (CD requests and step states at 0x9FA0/0x9FD8) */
    s32 unk10;
    u8 *texts; /* enemy and enemy-ability names (BtlEnemyType.nameOfs, btl_showCmdName) */
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
    u32 unk0_8 : 21;
    u32 allowKo : 1; /* KO units can be targeted */
    u32 unk0_30 : 1;
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
            u8 kind; /* 0 hp, 1 mp (btl_popupHp/Mp), 2 text (btl_popupText): g_btlPopupDrawFns draw handler; 3+ removes it */
            u8 noClamp; /* status-tick popups (btl_spawnTickPopups): keep yOfs, no clamping */
        } b;
    } info;
    union {
        u32 w; /* packed digits (btl_packDigits) */
        Unk800F6A54 *p; /* text popup: animation and glyph */
    } data;
    u16 clut;
    u8 pad1A[0x1C - 0x1A];
} BtlPopup; /* 0x1C */

typedef struct {
    BtlPopup entries[16];
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

/* Inlined copy of btl_clearCmd (clear a command); the original source inlines it. */
static inline void btl_clearCmd_inline(BtlCmd *arg0) {
    arg0->next = 0;
    arg0->ability = 0;
    arg0->targets = 0;
    arg0->cmdNo = 0;
    arg0->subNo = 0;
    arg0->info.b.targeting = 0;
    arg0->info.b.unk0_2 = 0;
    arg0->info.b.automatic = 0;
    arg0->info.b.covered = 0;
    arg0->info.b.dodged = 0;
    arg0->info.b.reflect = 0;
    arg0->info.b.magicFailed = 0;
    arg0->info.b.shortSummon = 0;
    arg0->info.b.enemyBounce = 0;
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
    u32 flags; /* bits 0-1 ambush (BTL_GETVAR_AMBUSH: 0 back attack, 1 preemptive, 2 normal, btl_rollAmbush); bits 2 and up
                * are the encounter's Battle11C020.flags ^ 0x30, shifted left 2 (btl_initSceneFlags) */
    Battle1330 *group; /* enemy group chosen by btl_pickEnemyGroup */
    Battle1334 *enemyTypes; /* the encounter's enemy type records */
    u16 *seqTable; /* enemy battle sequences: counts, offsets, animations (btlseq_init) */
    s16 *sceneData; /* offset table, entries relative to its start (btl_getSceneData) */
} Battle132C;

typedef struct {
    BtlUnit9C model;
    u8 pad7C[0x98 - sizeof(BtlUnit9C)];
} BtlModel; /* 0x98 */

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u16 flags; /* 1 background loaded (btlbg_draw draws), 2 enemy models loaded, 4 party models loaded, 0x10 UI started, 0x20 disc cover open, 0x40 exit preloaded, 0x1000 battle over (btl_main) */
    u8 unk2[2];
    s32 savedHeap; /* g_modeHeap after btl_initMode; btl_load allocates from here again */
    s32 partyMotions[4][0x22]; /* per party member: motion data addresses; BtlUnit.motions points here (btl_loadPartyMotions) */
    BtlModel weaponModels[4]; /* per party member: weapon model (BtlUnit.weaponModel, btl_initPartyUnit) */
    BtlEnemy enemies[4];
    BtlEnemyType enemyTypes[3]; /* enemy type records (BtlEnemy.unk0 points here) */
    struct BtlCmd8 enemyAbilities[18]; /* enemy attacks: BtlCmd.ability of enemy commands (btl_setEnemyCmd) and Return Magic */
    BtlUnit *unitList; /* active units, linked through BtlUnit.next */
    BtlUnit units[8]; /* runtime: party at [0..3] (Zidane 0x80108EFC, Vivi +0xD0, ...) */
    s32 tick; /* btl_update count; low 4 bits animate (bobbing, cursor) */
    u8 phase; /* BTL_PHASE_* */
    u8 subPhase; /* in BTL_PHASE_END how the battle ended (BTL_END_*); a step counter in the exit phases */
    u8 exitTimer; /* exit phases: frames of the fade before btl_saveParty */
    u8 escapeHeld; /* runtime: 1 while L1+R1 are held (btlui_pollEscape), units take the run-away stance */
    u8 fleeBrightness; /* escaping units' brightness, -2 per tick (btl_victory) */
    u8 loadFlags; /* 1/2/4 setup steps (btlui_init runs once all three are set), 8 btl_initEnemies, 0x10 btl_initParty */
    u8 partyBrightness; /* intro fade-in, +4 per tick to 0x20 (btl_updateIntro) */
    u8 enemyBrightness; /* intro fade-in, +4 per tick to 0x20 */
    u8 padD88[0xD90 - 0xD88];
    BtlCmd cmdSlots[36]; /* per unit: 6 (players) or 3 (enemies); BtlUnit.cmd.. point here */
    BtlCmd escapeCmd; /* the party's escape (BTL_CMD_ESCAPE) command */
    BtlCmd cmdQueue; /* list head (only unk0 used); runtime: commands are queued here when issued */
    BtlCmd *curCmd; /* runtime: Vivi's command while her Attack executes */
    struct Battle108CSet cursorPrims[2]; /* target cursor, per draw buffer */
    BtlUnitList fxTargets; /* effect target list (btl_playCmdEffect -> btlfx_setTargets) */
    u16 cmdFlags; /* 1 escape queued, 2 active marker and target projection on (btl_setMarkersVisible), 4/8 trance Eidolon timer running/fired, 0x10 Spear pending */
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
    s32 unk1530;
    s32 dataSector; /* battle data file sector */
    struct Battle1538 **partyBufEnds; /* per party slot: allocation header in the last 0x28 bytes of partyBufs[i] */
    s32 partyBufs[4]; /* per party slot: 0x13000-byte model file buffer (btl_allocBuffers); the effect loader reuses [0] for summons */
    BtlStatusInfo statusInfo[32];
    Battle16CC abilities[0xC0]; /* runtime: BtlCmd.ability = &abilities[subNo] (Attack 176, Focus 188) */
    s32 statusSets[64]; /* status masks indexed by ability/weapon status-set bytes (btl_applyAbilityStatus) */
} Battle; /* 0x23CC */

/* battle work area, allocated at g_loadAreaEnd + 0x61800 */
typedef struct {
    Battle battle;
    u8 filesBuf[0x240C - 0x23CC]; /* Battle.files */
    u8 partyBufEndsBuf[0x241C - 0x240C]; /* Battle.partyBufEnds */
    u8 heap[1]; /* becomes g_modeHeap */
} BattleMem;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
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
    u16 unk0;
    u16 unk2;
} U16Pair;


typedef struct {
    s16 pos[4]; /* x, y, z, and the fourth value (distance / roll) */
    u8 anchor; /* bits 0-4 anchor kind (btlcam_resolveAnchor: 1 caster, 5 target, 0xA focusUnit, 0xF partner, 0x15+ points), 0x20 use the unit's rotation */
    u8 anchorFlags;
    u8 padA[0xC - 0xA];
} Unk800C412C0; /* 0xC */

typedef struct {
    Unk800C412C0 keys[2]; /* from [0] and to [1] (btlcam_setPathKey shifts them) */
    u8 mode; /* 0 off, 1 hold the key, 2 lerp, 3 lerp between anchored points, 4 cubic over g_btlCamera.points */
    u8 wrapModes; /* per axis, 2 bits: how angles wrap while lerping */
    u8 easeModes; /* 2 bits per weight: 0 linear, 1 ease out, 2 ease in (g_btlCamScript.accel) */
    u8 locked; /* keys set by camera flag 0x8000: the next command doesn't replace the mode or timing */
    u8 frames;
    u8 framesLeft; /* counts down in btlcam_step */
    u8 pad1E[0x20 - 0x1E];
} Unk800C412C; /* 0x20 */

typedef struct Unk800FE3B8 {
    SVECTOR points[6]; /* camera path control points (cubic, btlcam_step); [4]/[5] keep the eye/target for script flag 0x80 */
    SVECTOR pointRots[4]; /* rotations kept with points[0-3] (zeroed by btlcam_loadScript; btlcam_resolveAnchor anchors 0x15+) */
    s16 yaw; /* from eye to target (btlcam_step) */
    u8 paused; /* script frame stops counting (camera flag 0x100100); effect steps resume it */
    u8 effectPick; /* camera choice btlfx_opPlayCamera mode 3 plays: random 0/1 per effect, or the variant (camera flag 0x2000) */
    SVECTOR eye; /* eye position given to btlcam_lookAt */
    u8 flags; /* 1 a camera script took over (projDist reset), 2 the effect script started (btlfx_runScript) */
    u8 stepped; /* the effect frame already stepped the camera: btlcam_update skips this frame */
    u16 projDist; /* projection distance (SetGeomScreen through GameData.unkA) */
    u16 baseProjDist; /* default projection distance (0x200) */
    u8 pad62[0x65 - 0x62];
    u8 prevViewState; /* viewState of the previous frame */
    u8 projFixed; /* the target view set the projection (btlcam_step leaves GameData.unkA) */
    u8 endIntro; /* camera command 0x4000 with 1: ends the intro once loadFlags has all three setup bits */
    Unk800C412C paths[2]; /* eye path [0], target path [1] (btlcam_evalPath) */
    s32 variant; /* script variant 0-2 the camera plays (btlcam_loadScript: from the script header, Battle.cameraNo, the unit size, ...) */
    u32 idleSway; /* sway used when the script sets none (camera flag 0x400): amplitude bits 4-7, speed 0-3 */
    s32 idleSwayPhase;
    s32 viewBlend; /* frames left of the blend into the target view (btlcam_applyTargetView) */
    u8 nextVariant; /* variant chosen by a header with flag 0x200 for the next script (2 bits per variant) */
    u8 mode; /* 0 scene (btlcam_playScene), 1 effect (btlcam_play), 2 enemy sequence (btlcam_playSeq) */
    u8 targetView; /* 1 while the target-select view is on */
    u8 viewState; /* 2: at the target view; btlcam_evalPath counts anchors 0x19/0x1A up to it */
    SVECTOR viewFromEye; /* eye when the target view started */
    SVECTOR viewFromAt; /* target when the target view started */
    BtlUnit *focusUnit; /* unit the scene camera follows (anchor 0xA; btlcam_loadScript variant from its state) */
} Unk800FE3B8;
extern Unk800FE3B8 g_btlCamera;

typedef struct {
    s32 gil; /* gil (BtlEnemyType.gil of the defeated enemies; ovl_65 BattleResult.gil) */
    s32 exp; /* exp (BtlEnemyType.exp; ovl_65 splits it among expMask) */
    s16 ap; /* ap of the enemy group (Battle1330.ap, btl_initSceneFlags). runtime: Malboro group 3 = "AP 3" */
    u8 expMask; /* bit per party slot in the battle (btl_initPartyUnit), cleared when a unit leaves (btl_calcCmd 0x6A); ovl_65: receives exp */
    u8 unkB;
    u8 items[16]; /* dropped item ids, 0xFF = none (btl_addEnemyRewards) */
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

typedef struct LinkNode {
    struct LinkNode *next;
    u8 pad4[0x10 - 4];
    u32 flags; /* BtlCmd.info.w */
} LinkNode;
void btl_linkCmd(LinkNode *, LinkNode **);
Unk16D8 *evt_getBattleObjects(void);
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
    u8 flags; /* weapon table flags: 8 = the reach is along x (btl_getWeaponOffset) */
    u8 pad1[7];
    u16 reach; /* tip offset along y (or x) in the weapon bone's space */
    u16 offsetZ; /* tip offset along z */
} Unk800BC1E0;

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
    u16 unk0;
    u16 tpage; /* tpage (abr bits added) */
    u16 clut; /* clut */
    u16 pad6;
    u16 abr; /* abr, 0xFF = opaque */
} SprInfo; /* sprite texture info (btlfx_addSpriteQuad) */

void btlfx_addPrim(u32, s32, void *);

/* scrolling/waving 2D background layer (btlfx_drawBackdrop) */
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
            s32 dataOfs; /* sprite set offset from the layer block (btlfx_drawBackdropSprite) */
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
} BtlBgLayers;


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
/* sprite set header (btlfx_drawAnimQuads, btlfx_drawBackdropSprite): u16 frame offsets, then at
   unk8[count] a palette (u16 count, u16 entries[]) and per frame a byte count of
   3-byte sprites */
typedef struct {
    u16 frameCount; /* frame count (& 0x7FFF) */
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
void btlfx_drawBackdropSprite(u8 *, s32, s32, s32, s32, s32);


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
    BtlUnit9C *model;
    BtlTexInfo tex; /* texture tpage/clut pairs (btl_applyModelTextures) */
    u8 used; /* in use */
    u8 slice; /* drawn sliced (Hi_SetEffModelSlice) */
    s16 id; /* index */
    s16 kind; /* 0 solid, 1 gouraud, 2 textured (Hi_RegisterSolid/Gou/Tex*EffModel) */
    s16 otOffset; /* OT offset when drawn (Hi_SetEffModelOffset, btl_drawMeshOffset) */
    s16 sliceY; /* slice height (Hi_SetEffModelSlice) */
    u8 pad26[0x28 - 0x26];
} BtlObjSlot; /* 0x28 */
extern BtlObjSlot g_btlfxEffModels[32];


typedef struct {
    BtlUnit9C *model;
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
extern u16 g_btlfxKindTimings[][19];
extern u8 g_btlMessages[];
extern char s_hiraishiErrorFmt[];
extern char s_hiFreeEffModelErr[];
extern char s_hiSplitMdlVertexErr[];
/* effect element (0x28) in an effect file, after the header */
typedef struct EffElem {
    u8 kind; /* kind, 9 = no texture */
    u8 kind2; /* 0xFF = no second texture */
    u8 unk2;
    u8 unk3; /* flags */
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    s8 unk11[3]; /* base rotation */
    s8 unk14[3]; /* offset */
    u8 unk17[3]; /* rotation spread */
    u8 unk1A;
    u8 unk1B;
    s32 tex; /* texture data offset, relocated on load */
    s32 tex2; /* second texture data, relocated on load */
    void (*unk24)();
} EffElem; /* 0x28 */

typedef struct {
    s32 weightCount; /* count of weights */
    s16 elemCount; /* count of EffElem following */
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s16 weightAnim; /* 1 = blend weights animated */
    union {
        s32 w;
        u8 b[4];
    } weights;
} EffElemHdr; /* 0x14 */

/* effect file header */
typedef struct {
    u8 unk0; /* kind */
    u8 pad1[8 - 1];
    s16 unk8;
    s16 unkA;
    u8 padC[0x18 - 0xC];
    s16 unk18;
    s16 unk1A;
    u8 pad1C[0x20 - 0x1C];
    s16 unk20;
    s16 unk22;
} EffFileHdr;

/* emitter animation parameters */
typedef struct {
    s16 unk0; /* speed curve: 0 linear, 1 sine, 2 table, 3 constant */
    s16 unk2; /* duration */
    s16 unk4;
    s16 unk6;
    s16 unk8; /* particle limit */
    s16 unkA;
    s16 unkC[3];
    s16 unk12;
    s16 unk14[3];
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20[2];
    s16 unk24[2];
    SVECTOR unk28;
    SVECTOR unk30;
} EffAnim; /* 0x38 */

typedef struct {
    s32 unk0;
    EffAnim unk4;
    s16 unk3C;
    s16 unk3E;
    s16 unk40[8];
    s16 unk50[8];
} EffTail;

/* EffTail as seen by the particle spawner (btlfx_runEmitters), by EffTail.unk0 kind */
typedef struct {
    s32 unk0; /* kind */
    EffAnim unk4;
    s32 (*unk3C)(SVECTOR *, SVECTOR *, s32, s32); /* kind -1 */
    u8 pad40[4];
    CVECTOR unk44; /* kind 0: colour fade */
    union {
        CVECTOR c;
        s16 h; /* kind 4: rotation mode */
    } unk48;
    s16 unk4C; /* kinds 1-3: rotation mode */
} EffTailSpawn;
void LoadAverageCol(u8 *, u8 *, s32, s32, u8 *);

/* 0x38-byte block in front of Pool6CEntry.unk4 */
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4[4]; /* element indices */
    u8 unk8[4]; /* element weights */
    u8 unkC;    /* spawn rotation mode */
    u8 unkD;    /* element count */
    s16 unkE;
    u8 unk10;   /* particle limit */
    u8 unk11;   /* first spawn frame + 1 */
    s16 unk12;
    s16 unk14[3];
    s16 unk1A;
    s16 unk1C[3];
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28[2];
    s16 unk2C[2];
    s16 unk30[2];
    s16 unk34[2];
} EffSub; /* 0x38 */

/* particle emitter parameters at Pool6CEntry.unk4 (kinds 0-5) */
typedef struct {
    u8 unk0; /* kind */
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
} EffEmit;

typedef union {
    s32 w;
    struct {
        s16 x;
        s16 y;
    } s;
} XY32;

typedef struct Pool6CEntry {
    EffFileHdr *file; /* effect file: header, motion parameters */
    EffSub *sub; /* sub-emitter parameters (EffMotion), NULL = none; the EffSub block is just before it */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    EffElem *elems; /* elements (btlfx_spawnEmitter relocates their textures) */
    s32 unk1C;
    s16 unk20;
    s16 otZ; /* otZ of its particles (btlfx_updateParticles); btlfx_opDrawOnUnit / btlfx_startCastEmitter set it */
    EffTail *tail;
    SVECTOR *origin; /* emitter position (btlfx_moveEmitter shifts its particles with it) */
    s32 unk2C;
    s16 state; /* in use, -1 = release */
    s16 unk32;
    XY32 unk34; /* live particle counts (s.x, s.y) */
    s16 frame; /* frame */
    s16 unk3A;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44[3];
    s16 unk4A;
    XY32 unk4C;
    s16 unk50[3];
    s16 unk56;
    XY32 unk58;
    SVECTOR rot; /* emitter rotation (from its EffAnim, or set every frame by the ops: btlfx_opBounce, btlfx_opDrawOnUnit); pad = floor height of the particles */
    SVECTOR *fieldPos; /* centre of the force-field modes 1-9 (btlfx_updateParticles) */
    SVECTOR *fieldRot; /* angles of the force field (modes 5, 6, 9: btlfx_toLocalPolar) */
} Pool6CEntry; /* 0x6C */

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
    s16 unkC; /* rotation */
    s16 unkE;
    s16 unk10;
    u8 unk12; /* flags */
    s8 unk13;
    XY32 unk14;
    s16 unk18;
    u8 frame; /* frame */
    u8 frames; /* lifetime in frames */
    u8 elem; /* element index, 0xFF = none */
    u8 elemFrame; /* element frame */
    s8 timeStep; /* time step of the motion (btlfx_stepParticle*) */
    u8 unk1F; /* in use */
    s16 unk20;
    s16 unk22;
    XY32 unk24;
    XY32 unk28;
    Pool6CEntry *owner; /* emitter (btlfx_killEmitter, btlfx_moveEmitter) */
} Pool30Entry; /* 0x30 */

/* particle motion parameters: the effect file (Pool6CEntry.unk0) or the
 * block at Pool6CEntry.unk4 */
typedef struct {
    u8 unk0; /* kind */
    u8 pad1[8 - 1];
    s32 (*unk8)();
    u8 padC[0xE - 0xC];
    s16 unkE;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} EffMotion;

typedef struct {
    s32 capacity; /* entries in the buffer */
    s32 live; /* live particles (btlfx_updateParticles) */
    s32 maxUsed; /* highest index in use */
    Pool30Entry *entries;
} Pool30;

extern Pool30Entry g_btlfxParticleBuf[0x100];
extern Pool30 g_btlfxParticles;
extern Pool6C g_btlfxEmitters; /* 0x920 bytes */
extern Unk80100D88 g_btlfxResArgs;
extern Unk80100D88 g_btlfxResArgs2;
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
    u16 magic; /* 'so' (0x6F73) */
    u16 unk2;
    u16 modelOfs; /* model header offset; (modelOfs - 8) / 8 texture entries */
    u16 texPosOfs; /* texture position table offset */
    Unk800E9000Ent tex[1]; /* texture entries */
} Unk800E9000;

typedef struct Unk80100DC054 {
    s16 dataOfs; /* offset of the model data */
    s16 motionCount; /* count of motions */
    s16 texCount; /* count of the texture tables */
    u16 clutRows; /* CLUT rows loaded at y 0xE6 */
    s32 clutOfs; /* CLUT data offset from the model data */
    u8 padC[0x10 - 0xC];
    s32 workOfs; /* work area offset (work area: 0x50000 - workOfs bytes) */
    s32 texAnimOfs; /* texture animation offset */
    u16 texTpage[6]; /* per texture: tpage (bits 0-3 x / 64, bit 4 y + 256) */
    u16 texClut[6]; /* per texture: clut */
    u16 texY[6]; /* per texture: y in its page */
    u8 *modelHdr; /* model header */
    BtlUnit9C50 *texAnims;
    u8 *work; /* work area (g_btlfxLoader.workCursor) */
    s32 workSize;
    s32 unk4C;
    s32 *data[16]; /* file data tables (btlfx_getFileData 0x80-0xBF) */
    BtlModel model;
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

extern u8 g_btlAbilityNames[];
extern void (*g_btluiMenuUpdate[])(void);


typedef struct {
    u8 open; /* help window shown (btlui_toggleHelp) */
    u8 opening; /* sliding in (unkC frames) */
    u8 unk2;
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
    u8 unk5;
    u16 frame; /* slide frame */
    u16 x; /* x */
    u16 y; /* y */
    Unk800F3F10 *slide; /* running slide, NULL when still */
} Unk800F412C; /* 0x10 */

typedef struct {
    u8 shown; /* panel drawn (btlui_showPanel) */
    u8 unk1;
    u8 highlightRow; /* row of the menu unit, 0xFF none (btlui_findHighlightRow) */
    u8 pad3;
    Unk800F412C labels[5]; /* header labels with their slides */
    u32 pulse; /* highlight pulse 0..0x1F (btlui_pulsePanelHighlight) */
    s32 returnState; /* menu state btlui_hideListHeader restores */
    u32 rowCount; /* party rows */
    s32 y; /* panel top */
    BtlUnit *menuUnit; /* unit whose row is highlighted */
    BtlUnit *units[4]; /* unit per row */
    s8 rowColor[4]; /* text colour per row (btlui_updatePanelRowColor: 3 KO, 4 low HP) */
    u8 cantAct[4]; /* per row: the unit can't act (gauge greyed) */
    u8 slot[4]; /* per row: unit slot id */
    u8 atbKind[4]; /* per row: 0 normal, 2 slow/stop, 3 haste (gauge colour) */
    u8 tranceKind[4]; /* per row: 0 no trance gauge, 1 gauge, 2 in trance */
} Unk80105078;
extern Unk80105078 *g_btluiPanel;
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
    u8 unk5;
    u8 targetAll; /* the whole side is marked */
    u8 unk7;
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
extern UiSprite D_80068E78[3];
s32 win_getPalClut(s32);
void SetTexWindow(void *, RECT *); /* PsyQ */
void win_reapClosed(void);
u8 chr_getAppearanceType(s32, u8);
void ui_drawItemIcon(s32, s32, s32, s32, u8);
extern u32 D_800686D8;
void math_rotMatrixYXZ(void *, void *);
void gfx_selectBuffer(s32);
void pad_setRepeatEnabled(s32, s32, s32);
void win_setVisible(Window *, s32);
void win_setCloseFrames(Window *, s32);
void pad_resetRepeats(s32);
void win_close(Window *);
Window *win_get(s32);
ItemSlot *item_findSlot(s32);
typedef struct {
    u8 flags; /* 4: throwable (Throw list) */
    u8 pad1;
    u16 modelId; /* weapon model (| 0x20000: db key; btl_initWeapon) */
    u8 pad4;
    u8 attack; /* attack power (Throw: power x2) */
    u8 pad6[0xC - 6];
} Unk8006E0E8; /* 0xC */
extern Unk8006E0E8 g_weaponTable[];
void VSyncCallback(void (*)(void));
void text_uploadPartyNames(s32);
s32 akao_playSe(s32, s32, s32, s32);
s32 item_getCount(s32); /* main.c defines it returning u8 */
typedef struct {
    u32 unk0_0 : 8;
    u32 unk0_8 : 9;
    u32 unk0_17 : 15;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    s32 unk8;
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
void btl_fillModelRgb(BtlUnit9C *, s32, s32, s32);
void btl_setModelAbr(BtlUnit9C *, s32);
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
void func_800F48DC(void);
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
    s16 unk2;
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
void btl_applyBgLight(BtlUnit9C *);
void btl_setMeshRgb(BtlUnit9C *, u8, s16, s16, s16);
Battle122C *btlseq_start(BtlCmd *);
struct BtlBgHeader *btlbg_getHeader(void);
s32 btlfx_stepPlayMotionWait(struct Unk800D8EA4 *, s32, s32 *);
s32 btlfx_stepPlayMotionHold(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8EA4(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8F1C(struct Unk800D8EA4 *, s32, s32 *);
s32 func_800D8FAC(struct Unk800D8EA4 *, s32, s32 *);
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
void btl_setModelRgb(BtlUnit9C *, s32, s32, s32);
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
u16 btlfx_getMeshPrims(BtlUnit9C *, u16, s32, s32 *, s32);
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
void btl_getBonePos(BtlUnit9C *, s32, u16 *);
void btl_getBoneMatrix(BtlUnit9C *, s32, BtlBone *);
BtlObjSlot *btlfx_getEffModel(s32, char *);
u8 btl_getMeshCount(BtlUnit9C *);
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
s32 btl_reloadPartyStep(s32);
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
    u8 unk2;
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

typedef struct {
    s16 state; /* state */
    u16 nextState; /* next state */
    u8 pad4[0x10 - 4];
    s32 script; /* battle event script (dependency 0x05xxxx; evt_init) */
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    s32 sceneBuf; /* encounter scene file buffer (btl_initEncounter) */
} Unk800AA08C;

typedef struct {
    s16 state; /* state */
    u16 nextState; /* state after the CD read */
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 slot; /* party slot of the file being loaded */
    u8 padC[0xE - 0xC];
    s16 unkE;
    union {
        s32 w[4];
        u16 h[8];
    } fileIds; /* file ids still to load, -1 = done */
    s32 unk20;
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


s32 btlfx_stepJumpHome(Unk800D955C *arg0, s32 arg1, s32 *arg2);
s32 btlfx_moveUnitLinear(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5);
void btlfx_lerpVec(s16 *, s16 *, s32, s16 *);
u16 Hi_RegisterTexListModel(BtlUnit9C8 *, BtlTexInfo *, s16 *, s32);
u16 Hi_RegisterGouEffModel(BtlUnit9C8 *);
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
    s32 unk0;
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
    s32 rgb; /* colour word: rgb, bit 25 semi-transparent (popups: animation bit 7), bit 24 set */
    u8 padC[0x10 - 0xC];
    XY32 xy; /* screen position (projected from the popup, then offset) */
    u16 clut; /* clut (BtlPopup.clut), shifted into the uv word */
} Unk800CCD00;

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
extern SpriteDef D_80068BE8;
s32 evt_battleTrigger(s32, s32, s32, s32);
void math_rotMatrixXYZ(void *, void *); /* rotation -> matrix */
s32 btlfx_transformPolar(SVECTOR *, s32, s32);
void btlfx_transformPoint(s32, s32, s32);
extern SpriteDef D_80068BF0[]; /* digits */

typedef struct {
    u8 *pc; /* camera script pointer (btlcam_step reads its commands) */
    u32 frame; /* script frame, compared with each command's 10-bit frame */
    s32 accel; /* ease acceleration of the paths (camera flag 0x40) */
    s32 viewHold; /* frames the target view is kept after target select (btlcam_applyTargetView) */
    u32 sway; /* sway: amplitude bits 4-7, speed bits 0-3 (btlcam_applySway) */
    s32 swayPhase;
    s32 savedVariant; /* variant restored for the scene camera after an effect (btlcam_endEffect) */
    s32 shakeAxes; /* shake: bit 0 no vertical, bit 1 no horizontal (btlcam_applyShake) */
    s32 shakeFrames;
    s32 shakeLeft;
    s32 shakeFrom;
    s32 shakeTo;
    s32 zoomFrames; /* projection-distance tween (btlcam_step) */
    s32 zoomLeft;
    s32 zoomTarget; /* 1 base distance (g_btlCamera.baseProjDist), 2 current (projDist) */
    s32 zoomFrom;
    s32 zoomTo;
    u8 saveView; /* camera flag 0x80: 1 requested, 2 eye/target saved in points[4]/[5] */
    u8 keepCamera; /* camera flag 0x80000: btlcam_endEffect doesn't reload the scene camera */
    s16 casterAnchors[5]; /* btlcam_getUnitAnchors of the acting unit */
    s16 partnerAnchors[5]; /* btlcam_getUnitAnchors of the partner (effect flag 4) */
    s16 targetAnchors[5]; /* btlcam_getUnitAnchors of the target */
} Unk800FE488; /* 0x64 */
extern Unk800FE488 g_btlCamScript;
extern u32 g_btluiTargetMovePads[][4];
extern void (*g_btluiTargetMoveFns[])(void);
extern RECT g_btlBannerDefaultRect;
u8 *btl_buildBannerFrame(u8 *, RECT *);
void btl_applyModelScale(BtlUnit9C *);
void btl_scaleModelMatrix(BtlUnit9C *, VECTOR *);
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
extern u16 g_btlCharModelIds[];
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
void btlfx_projectSliceMesh(BtlUnit9C *, u8);
u8 *btlfx_drawSliceMesh(BtlUnit9C *, u8, u8 *);
void btlfx_projectMorphMesh(BtlUnit9C *, u8, u16);
u8 *btl_drawMeshOffset(BtlUnit9C *, u8, u8 *, s32);
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
extern Unk80068EE8 D_80068EE8[];
extern u16 D_80068EEE; /* D_80068EE8[0].unk6 */
extern Unk80068EE8 D_80068E90;
extern Unk80068EE8 D_80068ED0;
extern Unk80068EE8 D_80068ED8;
extern Unk80068EE8 D_80068EE0;
extern u8 g_btluiPort;
extern SprtWords g_btluiPortIconSprt;
extern SprtWords g_btluiPortBlinkSprt;
extern DR_TPAGE g_btluiPortIconTpage;
extern DR_STP g_btluiPortIconStp;
extern u32 g_btluiPortIconOt;
s32 btl_findBackgroundId(void);
Unk8010141C *btlfx_allocSummonModel(void);
typedef struct {
    s16 unk0[6][2];
} Unk800DD120;
u8 *db_getModelTexPages(s32);
u8 *db_getModelUvOffsets(s32);
typedef struct {
    u32 unk0_0 : 4;
    u32 unk0_4 : 1;
    u32 unk0_5 : 3;
    u32 unk0_8 : 21;
    u32 unk0_29 : 1;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    u8 pad4[0xC - 4];
} Unk8006A774;
extern Unk8006A774 D_8006A774[];
u8 *btl_calcBoneMatrices(BtlUnit9C *, s32, u8 *);
void btl_projectMesh(BtlUnit9C *, u8);
u8 *btl_drawMesh(BtlUnit9C *, u8, u8 *);
void btl_setModelTransform(BtlUnit9C *, SVECTOR *, VECTOR *, VECTOR *);
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
    u8 flags; /* 1 playing, 2 stop at the end (else loop), 4 vertical scroll (frameCount 0) */
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

/* random table: two refill checks, then one byte read as (b << 8 | b). RAND16 reads g_randPtr in both operands
 * and increments it in one, which C leaves unsequenced: gcc 2.8 reads the byte once and stores the increment
 * after the enclosing expression (a statement expression that sequences it moves the store and breaks the match).
 * A port must read the byte before the increment. */
#define RAND_REFILL2()                         \
    if (g_randPtr >= g_randTableEnd) {            \
        rand_refill();                       \
        if (g_randPtr >= g_randTableEnd) {        \
            rand_refill();                   \
        }                                      \
    }
#define RAND16() ((*g_randPtr << 8) | *g_randPtr++)

/* main exe: candidates for ff9.h */
u8 chr_isAbilityLearned(s32, s32);
void btl_damageHp(BtlUnit *, s32, u8);
void btl_healHp(BtlUnit *, s32);
void btl_damageMp(BtlUnit *, s32);
void btl_healMp(BtlUnit *, s32);
void btl_updateModelVisibility(BtlUnit9C *);
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
    u8 unk2;
    u8 playMotion; /* btlfx_stepJumpForward plays the caster's motion 0x1D */
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
    u8 unk3;
    s32 frames; /* turn frames */
    s32 unk8;
    s16 delta[3]; /* rotation delta (btlfx_stepFaceTarget) */
} Unk800D805C;

typedef struct {
    u8 unit; /* unit id turned */
    u8 target;
    u8 unk2;
    u8 unk3;
    s32 frames; /* turn frames */
    s16 delta[3]; /* rotation delta back to its start rotation (btlfx_stepFaceHome) */
} Unk800D8124;

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

extern s32 D_800F78AC;
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
s32 btl_loadEncounterFile(Unk800AA08C *, s32);
void btl_beginFrame(void);
void btl_checkDiscCover(void);
u8 btlui_isPauseRequested(void);
void btl_updatePause(void);
s32 btl_update(void);
extern u8 D_8007AFB1;
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
    u32 flags; /* flags (0x10 off screen) */
    s32 modelId; /* model id */
    BtlUnit9C *model;
    s32 unkC;
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
s32 func_800A8F10(void);
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
void func_800EA548(void);
void btlui_readPad(void);
void func_800EB578(s32);
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

/* first unit flagged as the current target (bits 28 and 29 of BtlUnit.flags) */
static inline BtlUnit *find_target(void) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            return u;
        }
    }
    return NULL;
}

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
    u32 unk0; /* tag */
    u32 unk4;
    u32 unk8;
    u32 unkC; /* ordering table */
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
void btl_applyModelTextures(BtlUnit9C *, s16 *, s16 *);

void GeoSplitVertices(BtlUnit9C *, s32, s32);
s32 btlfx_getSplitVertexSize(BtlUnit9C *, s32);

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
s32 func_800DA064(u8 *, s32, s32);
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
u16 *func_800E4530(s32);
s32 btlfx_resolveFirstUse(s32);
extern u8 g_btlfxLargeTargetFx[][2];  /* summon id upgrades */
extern u16 g_btlfxFirstUsePairs[16][2];
void btlcam_focusCaster(void);
u32 btlfx_splitMeshVertices(BtlUnit9C *, u16, s32);
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

/* battle menu: unit under the target cursor (inlined by its callers) */
static inline BtlUnit *BtlFindTargetCursor(void) {
    BtlUnit *p;
    for (p = g_gameState->gameData->ctx.battle->unitList; p != NULL; p = p->next) {
        if ((p->flags & 0x30000000) == 0x30000000) {
            return p;
        }
    }
    return NULL;
}

/* inlined copy of btlui_getTargeting */
static inline s32 btlui_getTargeting_inline(void) {
    BtlUnit *u;
    s32 n;
    if (g_btlTarget.targetType >= 9 && g_btlTarget.targetType <= 11) {
        return 2;
    }
    n = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            n++;
        }
    }
    return n >= 2;
}

/* Queue-entry validity check inlined into btlui_pruneMenuQueue. */
static inline u8 BtlUnitCanAct(BtlUnit *u) {
    if (u == NULL) {
        return 0;
    }
    if (u->hp == 0 || (u8)btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) != 0) {
        return 0;
    }
    if ((g_btlRewards.expMask >> ((u8)u->flags >> 5)) & 1) {
        return 1;
    }
    return 0;
}

/* -O1 unit (ovl_0b_o1.c) */
s32 btl_getSceneData(s32);
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
void btlcam_applySway(s16 *, s16 *);
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
    s16 unk4;
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
    s32 unk28;
    s32 unk2C;
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
    s32 unk28;
    s32 unk2C;
} Unk800D4D6CCtx;
void btlfx_emitTrailRibbonFlat(s32, Unk800D4D6CCtx *, u8 *, s32 *);
void btlfx_emitTrailLines(s32, Unk800D4D6CCtx *, u8 *, s32 *);

/* sprite animation header (btlfx_drawTrail); followed by u32 frame entries and colors */
typedef struct {
    u16 unk0; /* frame count */
    u16 unk2;
    s16 unk4;
    u16 unk6;
    u8 unk8;
    u8 unk9;
    u16 unkA; /* first frame entry, in 8-byte units */
} Unk800D1494;

s32 btlfx_pushTrailPoint(u8 *, s32, s32, s32);
void btlfx_drawTrailScreen(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);
void btlfx_drawTrail3D(s32, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);
void btlfx_drawTrailFlat(s32, s32, s32, s32, s32, s32, s32, s32, s32, u32 *, u32 *, s32);

typedef struct {
    u16 frameCount; /* frame count (& 0x7FFF) */
    u16 tpage; /* tpage */
    u16 unk4;
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
    v.vx = (x); \
    v.vy = 0; \
    v.vz = (z); \
    gte_ldv0(&v); \
    gte_rtps()

/* btl_drawShadow: shadow quad q of entry e in the current buffer */
#define SHADOW_QUAD(q) e->unk8[g_gameState->gameData->drawBuf][q]

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
    DR_STP unk0;
    DR_STP unkC;
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
    s32 unk4;
    s32 unk8;
    s32 unkC;
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
} Unk800AA504Pos;

typedef struct {
    s16 x; /* x */
    s16 y; /* y (CLUT row; becomes h) */
    s16 w;
    s16 h;
} Unk800AA504Pos8;

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
    u8 unk2;
    u8 unk3;
    u8 flags; /* 2: gouraud colours */
    u8 type; /* ring type */
    u8 count; /* rings */
    u8 w;
} Unk800D23D8Hdr;

typedef struct {
    u16 unk0;
    u16 unk2;
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
    u8 motionMode; /* cast motion handling (0-3) */
    u16 frame; /* frames run */
    s32 loopFrames; /* cast motion length */
    s16 pos[4]; /* emitter position */
    s16 pos2[4];
    s16 unk1C[4];
    Pool6CEntry *emitter; /* cast emitter */
} Unk800E4C88; /* 0x28 */

/* btlfx_opDrawOnUnit effect state (sprite sheet on a unit) */
typedef struct {
    u8 kind; /* kind (resident op argument >> 6) */
    u8 unit; /* unit id, 0xFF = none (targets' centre) */
    s16 otBias; /* otZ for the unit (btlfx_addRadiusSixth) */
    s16 scale; /* sprite scale, -1 = none (btlfx_drawAnimQuads/btlfx_drawParticles arg 3) */
    s16 roll; /* z rotation of the sprites (op argument b << 8) */
    s16 dataId; /* common data id of the sprite set (func_800E4530) */
    s16 emitterId; /* common data id of the emitter (kind 3) */
    s16 colorIdx; /* g_btlfxCommonCluts palette index */
    s16 frames; /* frame count (kind 2: frame the emitter starts) */
    void *data; /* sprite set */
    s32 frame; /* frame */
    s16 offX; /* x offset (x4) */
    s16 offY; /* y offset (x4) */
    SVECTOR pos; /* position */
    s16 rot[3]; /* rotation (btlfx_aimAngles) */
} Unk800E57B0; /* 0x2C */

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
/* BtlUnit9C.texAnims seen as an array member: indexing it as an ARRAY_REF keeps `c ? 1 : 0` from folding into the address (btlfx_setUnitTexAnim) */
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
void btlfx_initSummonModels(BtlModel *, s32);
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
extern u8 D_8007AF01;

/* the command's caster can't act (status 8 with flag 2): prints message 0x15 */
static inline u8 btl_checkCmdUsable_can(BtlCmd *arg1) {
    if ((arg1->caster->status & BTL_STATUS_SILENCE) && (arg1->ability->flags & 2)) {
        btl_showMessage(0x15);
        return 0;
    }
    return 1;
}

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void btl_allocBuffers(void);
void btl_applyAutoAbilities(BtlUnit *arg0);
s32 btl_applyReflect(BtlCmd *arg0);
void btl_autoAttack(BtlUnit *arg0);
void btl_calcShadowMatrix(Unk800AD4E4 *arg0, BtlObj *arg1, MATRIX *arg2, s32 arg3, s32 arg4, s32 arg5);
void btl_copyStats(Unk800BD8C0 *arg0, Unk800BD8C0 *arg1);
s32 btl_getAnimFileId(s32 arg0);
u16 btl_getEntranceSound(void);
void btl_initBattleState(void);
s32 btl_initEncounter(Unk800AA08C *arg0, s32 arg1);
void btl_initEnemies(Battle *arg0);
void btl_initEquipElements(Character *c, BtlUnit *u);
void btl_initReturnInfo(void);
void btl_initWeapon(Character *arg0, BtlUnit *arg1);
s32 btl_loadBackgroundStep(Battle11C0 *arg0, s32 arg1, Unk800AA08C *arg2);
BtlPopup *btl_popupText(); /* (s32, u8, s16, s8); unprototyped: callers pass the argument unconverted */
void btl_queueAutoCmd(BtlUnit *arg0, s32 arg1, s32 arg2, s32 arg3);
void btl_reserveItem(s32 arg0);
s32 btl_saveParty(Battle *arg0);
void btl_setUnitAttr(BtlUnit *arg0, s32 arg1, u32 arg2);
void btl_spreadTroubleDamage(BtlUnit *arg0);
void btl_startBattle(void);
void btl_tickPoison(BtlUnit *arg0);
void btl_tickRegen(BtlUnit *arg0);
void btl_tickVenomMp(BtlUnit *arg0);
s32 btl_updateUnits(GameData *arg0, Battle *arg1);
void btlcam_update(void);
void btlfx_drawParticles(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void btlfx_drawRings(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void btlfx_drawTrail(u8 *p, s32 arg1, s32 *arg2, s32 frame, s32 scale, s32 arg5, s32 tp, u8 *out, s32 mode);
s32 btlfx_getScaledRadius(s32 arg0);
void btlfx_getStrikePos(s32 arg0, s32 arg1, s16 *arg2);
void btlfx_getUnitBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2);
void btlfx_getUnitBonePos(s32 arg0, s32 arg1, u16 *arg2);
s32 btlfx_heapWrapIfFull(Heap *arg0, s32 arg1);
void btlfx_rotateOffset(SVECTOR *arg0, s32 arg1, s32 arg2);
void btlfx_setTargets(BtlUnitList *arg0);
s32 btlfx_start(s32 arg0);
s32 btlfx_stepWaitSameSide(u8 *arg0, s32 arg1, s32 *arg2);
void btlfx_stopEmitter(Pool6CEntry *arg0);
void btlfx_turnToward(s16 *arg0, s16 *arg1, s32 arg2);
void btlui_buildAbilityList(Unk80104F88Entry *arg0);
void btlui_clearCursor(Unk800EB518 *arg0);
s32 btlui_confirmDoubleCast(s32 arg0);
void btlui_drawListScroll(Window *arg0);
void btlui_drawPanelHighlight(void);
void btlui_listConfirm(void);
void btlui_openListWindow(s32 arg0);
void btlui_openTargetWindow(s8 arg0);
void btlui_reset(void);
s32 btlui_resolveAbility(s32 arg0);
void btlui_startLabelSlide(Unk800F412C *arg0, Unk800F3F10 *arg1);
void btlui_targetMove(void);
void btlui_updatePanelRowColor(s32 arg0);
s32 clut_getX(s32 a); /* main */
void btl_setMusicVolumeFade(s32 arg0, s32 arg1);
void btl_cancelTranceEidolon(BtlUnit *arg0);
void btl_applyInvertedPower(Unk800BA938 *arg0);
void btl_getWeaponOffset(Unk800BC1E0 *arg0, s16 *arg1);
void btl_orElements(u8 *arg0, u8 *arg1);
s32 btlcam_getNextVariant(void);
void btlcam_orbitAnchor(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);
void btlfx_offsetPoint(SVECTOR *arg0, s32 arg1, s32 arg2, XY32 *arg3, s32 arg4, s32 arg5);
void btlfx_forwardAngles(s16 *arg0, XY32 *arg1, s32 arg2);
void btlfx_toLocalPolar(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 *arg5);
void btlui_updateTargetFlags(void);

#endif

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
    u8 unk4;
    u8 pad5[0xC - 5];
    s32 unkC;
    u8 pad10[0x12 - 0x10];
    s16 unk12;
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
    DRAWENV unk0;
    DISPENV unk5C;
    s16 unk70;
    u8 pad72[0x74 - 0x72];
} Unk80100E30Buf; /* 0x74 */

typedef struct {
    Unk80100E30Buf unk0[2];
    Unk80100E30Buf *unkE8; /* current */
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
    } unk0;
    u8 pad4[0x10 - 4];
} BtlAnimEvt; /* 0x10 */

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    BtlAnimEvt unk4[1];
} BtlAnimEvtList;

/* battle task with a step counter (func_800AE618) */
typedef struct {
    u8 pad0[6];
    s16 unk6; /* step */
} BtlStepTask;

/* work area of the effect step btlfx_opReturnHome */
typedef struct {
    u8 unk0;
    u8 unk1; /* phase */
    u8 unk2; /* unit id */
    u8 unk3;
    u8 pad4[0xC - 4];
    s16 unkC[8];
    s16 unk1C[6];
} Unk800E5538; /* 0x28 */

void func_800E436C(s32, s16 *, s32, s32, s32);
s32 btlfx_moveUnitHome(s32, s32, s16 *, s32);
s32 func_800E84F0(s32);

typedef struct {
    u8 pad0[7];
    u8 unk7; /* count */
    s32 unk8; /* index into g_btlCmdAbilities */
} Unk800FD6E0;
extern Unk800FD6E0 D_800FD6E0;
extern u8 D_800F8D84[]; /* ability ids */

/* GPU mask-bit setting word (GP0 E6, what libgpu's SetDrawStp writes): set = draw with the mask bit set,
 * check = don't draw over masked pixels. Candidate for psyq.h. */
#define GPU_STP(set, check) (0xE6000000 | ((check) << 1) | (set))

/* main exe: candidates for ff9.h */
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */
typedef struct {
    u8 unk0;
    u8 unk1; /* ap needed */
} Unk8006CB88;
extern Unk8006CB88 D_8006CB88[]; /* abilities */

/* draw environment pair built in the primitive buffer by btlfx_drawBgOffscreen */
typedef struct {
    DRAWENV unk0;
    DRAWENV unk5C; /* previous */
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

/* one primitive in a model's packed GPU packet buffer (BtlUnit9C.unk10) */
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
    s16 unk5A;
    BtlUnit9C8 *morph; /* morph target */
    void *unk60;
    u8 pad64[0x74 - 0x64];
    s16 attachX; /* offset from the parent bone (btl_calcBoneMatrices; event op 0xD4 writes it through main's Unk8004AFF0) */
    s16 attachY;
    s16 attachZ;
} BtlUnit9C;

/* GameData.unk8E0 entry as btl_registerModel fills it (ff9.h's Unk8E0 has pad4) */
typedef struct {
    s32 unk0; /* flags */
    s32 unk4; /* model id */
    BtlUnit9C *unk8;
    s32 unkC;
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
    u16 unk0; /* hp */
    s16 unk2; /* mp */
    s16 unk4; /* atb */
    s8 unk6;  /* atb speed */
} BtlUnit28;

/* battle unit; 8 of them at Battle.unk6FC, linked through unk0 */
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
#define BTL_CMD_TRANCE_EIDOLON 0x39 /* code: trance Eidolon timer (btl_updateUnitStatus) casts Battle.unk11BB */
#define BTL_CMD_REBIRTH_FLAME 0x3A /* code: the item 0xF9 chance in btl_checkSideDefeated, ability 0x49 Rebirth Flame */
#define BTL_CMD_TRANCE        0x3B /* code: adding/removing BTL_STATUS_TRANCE queues it; effect 0x101 in trance */
#define BTL_CMD_KO            0x3C /* code: adding BTL_STATUS_KO queues it (btl_addStatus) */
#define BTL_CMD_AUTO_LIFE     0x3D /* code: KO with BTL_STATUS_AUTO_LIFE queues it */
#define BTL_CMD_PETRIFY       0x3E /* code: adding BTL_STATUS_PETRIFY during combat queues it */

/* BtlCmd.subNo / Battle.abilities indices with a fixed use in the code (names from the ability text D_800F909C) */
#define BTL_ABIL_REBIRTH_FLAME 0x49
#define BTL_ABIL_ATTACK        0xB0
#define BTL_ABIL_FLEE          0xB4
#define BTL_ABIL_MUG           0xB5
#define BTL_ABIL_SPEAR         0xB9
#define BTL_ABIL_SPEAR_TRANCE  0xBA
#define BTL_ABIL_TERRA_HOMING  0xBB
#define BTL_ABIL_THROW         0xBE

/* Battle.unkD80 (phase), as btl_update switches on it */
#define BTL_PHASE_SCRIPT   1 /* code: waits for the event script (btl_setBattleVar 33-36 act only now) */
#define BTL_PHASE_INTRO    2 /* code: btl_startBattle; btl_updateIntro loads and fades the units in */
#define BTL_PHASE_ENTRANCE 3 /* code: waits for the entrance motions, then combat and the ambush messages */
#define BTL_PHASE_COMBAT   4 /* runtime */
#define BTL_PHASE_END      5 /* runtime: btl_victory; Battle.unkD81 says how (victory, defeat, escape) */
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

/* btl_showMessage ids: entry id + 8 of the message table D_800F6CB4 (its first 8 entries are the element names);
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
    struct BtlUnit *next; /* active-unit list (Battle.unk6F8) */
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
    u32 flags; /* bit 0 party, 1-4 slot, 5-7 id, 8-9 row, 16 ATB fills, 18 weak stance, 21 motion held, 22 hidden, 23 model part, 28 targetable, 29 targeted */
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
    u8 unk7B;
    u8 baseBone; /* bone used as the unit position (btl_getBaseBonePos) */
    u8 koState; /* KO state machine step (btl_updateKo) */
    u16 popupFlags; /* btl_spawnPopups: BtlCalc.unk1B bits (1 hp, 2 heal, 4 critical, 8 mp, 0x10 mp heal), 0x20 miss, 0x40 killed, 0x80 guard */
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
    u8 unkB2;
    u8 cmdIssued; /* set when the menu issues its command, cleared when it ends */
    struct Unk80104F88Entry *menu; /* party member menu data */
    s16 screenX;
    s16 screenY;
    u8 onScreen; /* screenX/Y valid (btl_projectUnitPos) */
    u8 shadowBone; /* bone the shadow sits under (btl_calcShadowMatrix) */
    u8 shadowDirBone; /* bone the shadow faces toward from shadowBone; = shadowBone: that bone's own axis */
    u8 shadowW; /* shadow width / 14 (btl_setShadowSize) */
    u8 shadowD; /* shadow depth / 12 */
    u8 unkC1[3];
    u32 *support; /* support-ability bits (Character.unk88) */
    s16 tickHeal; /* btl_tickRegen */
    s16 tickDamage; /* btl_tickPoison */
    s16 tickMpDamage; /* btl_tickVenomMp */
    u8 tickFlags; /* 1 regen, 2 poison, 4 venom, 8 regen hurts */
    u8 inMenuQueue; /* waiting for its command menu (btlui_queueMenuUnit) */
} BtlUnit; /* 0xD0 */

/* ability/command calculation context */
/* BtlCmd seen by the formulas (btl_calcCmd's sub = the BtlCmd; flags = BtlCmd.unk10.w) */
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
    void (*unk0)(BtlScript *, BtlUnit *);
    s32 (*unk4)(BtlScript *, BtlUnit *);
} BtlScriptOp; /* script opcode handlers: on entry, per frame */
extern BtlScriptOp g_btlseqOps[];
void btl_setUnitFade(BtlUnit *, s32);


/* effect unit list (Battle.fxTargets, g_btlfxEnv->targets); effect scripts name units by index: 0-7 targets, 8.. bounced, 0x10 caster, 0x11 partner */
typedef struct {
    BtlUnit *caster; /* id 0x10 */
    BtlUnit *targets[8]; /* ids 0..7 */
    u8 targetCount;          /* count of unk4 */
    u8 unk25[2];
    u8 bounceCount;          /* count of unk28 */
    BtlUnit *bounced[4]; /* ids 8..: units a reflected spell bounces to (btl_playCmdEffect) */
    s16 centerX; /* average target position (btl_playCmdEffect) */
    s16 centerY;
    s16 centerZ;
    u8 pad3E[0x40 - 0x3E];
    u16 kind; /* btl_playCmdEffect: 1 Eidolon follow-up, 2 item, 4 Swd Mag, 0x11 bounce */
    u16 unk42;
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
    u8 unk23;
} Unk800F78A40;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x30 - 0x10];
    s32 *unk30[8]; /* offset tables, entries relative to their start */
    s16 unk50;
    u8 unk52;
    u8 unk53;
    u8 pad54[0x68 - 0x54];
    u8 unk68;
    u8 unk69;
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
    s32 unk28;
    struct Unk800FE3B8 *camera; /* g_btlCamera (btlcam_init) */
    s32 unk30;
} Unk800F78A4;
extern Unk800F78A4 *g_btlfxEnv;
/* u16 table inside .text (0x800CFA80): 0x1000 * sqrt(64 / i) for i = 0x40-0xFF, the trail ribbons' normalization */
extern s16 g_btlfxRsqrtTable[];

typedef struct {
    s32 unk0;
    s8 unk4[4];
    s8 unk8[4];
} Unk800EB518;

/* animated hp/mp readout */
typedef struct {
    s16 unk0; /* step */
    s16 unk2; /* shown */
    s16 unk4; /* target */
    u8 unk6; /* frames left */
    u8 pad7;
} Unk80104F88Gauge;

typedef struct Unk80104F88Entry {
    Unk80104F88Gauge gauges[2]; /* hp, mp readouts (btlui_updateGauges) */
    struct BtlUnit *unit;
    Character *chr;
    u8 slot; /* unit slot id (flags bits 5-7) */
    u8 learns; /* the character learns abilities (func_80034A30); 0: btlui_addLearnedAbilities skips it */
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
    s16 unk0;          /* count of unk4 */
    u16 unk2;          /* count of unk80 */
    UnkPair *unk4[31]; /* inventory entries, item ids >= 0xE0 */
    UnkPair *unk80[0x58];
    u8 unk1E0[0x100];
} Unk80104FA8;
extern Unk80104FA8 *g_btluiItems;

/* battle menu / frame state */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 pad7;
    u8 unk8;
    u8 unk9;
    u8 padA;
    s8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 pad11;
    u8 unk12;
    u8 unk13;
    s8 unk14;
    u8 unk15;
    u8 pad16[0x18 - 0x16];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} Unk80104E68;
extern volatile Unk80104E68 g_btluiState;

typedef struct {
    s16 phase; /* -1 idle, else the btlfx_runScript state (0 save VRAM .. 4 run ops) */
    s16 effectId; /* effect being played (btlfx_start, after btlfx_resolveFirstUse) */
    s16 unk4;
    s16 unk6;
    s16 file; /* effect file slot of the script (btlfx_getFileData) */
    union {
        struct {
            u8 unk0[3];
            u8 unk3; /* count of unk0 */
        } b;
        u16 h[2]; /* cleared as two halfwords (btlfx_opAddCameraChoice) */
    } cameras; /* camera choices of the effect (btlfx_opAddCameraChoice) */
    u8 cameraPick; /* camera choice picked (btlfx_opPlayCamera) */
    u8 seqRelease; /* set by btlseq op 0A; control op 8 waits for it with an enemy caster */
    union {
        s32 w; /* cleared as a word */
        u8 b[4]; /* b[2]: done */
    } unk10;
    s8 motionDone; /* unit ops: the motion op waited for has ended (btlfx_stepPlayMotionWait) */
    s8 moveDone; /* unit ops: the move op has arrived (btlfx_opRunHome, btlfx_opJumpForward, ...) */
    s8 turnDone; /* unit ops: the turn op has finished (btlfx_opFaceTarget, btlfx_opFaceHome) */
    s8 unk17; /* unit ops 0x79-0x7B */
    u8 *pc; /* script pointer, 3-byte commands */
    s32 flags;
    u8 retry; /* btlfx_retryOp: run the same op again next frame */
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
} Unk800FEC40;
extern Unk800FEC40 g_btlfxState;

typedef struct {
    s32 unk0; /* CD sector */
    s32 unk4;
    s32 unk8; /* buffer index */
    s16 unkC; /* state */
    s16 unkE;
    s16 unk10;
    s16 unk12; /* commands left in the current block */
    u8 pad14[0x16 - 0x14];
    u8 unk16; /* command kind */
    u8 unk17;
    u16 unk18; /* block count */
    u16 unk1A;
    u16 *unk1C; /* command pointer */
    u8 *unk20;
    s32 unk24;
    u8 *unk28;
    union {
        s16 h[2];
        s32 w; /* set to -1 as a word */
    } unk2C;
    u8 *unk30[2];
    s32 unk38;
    struct Heap *unk3C[2];
    RECT *unk44;
    s16 unk48;
    u8 pad4A[0x4C - 0x4A];
    u32 *unk4C;
    s16 unk50;
    u8 unk52;
    u8 unk53;
    struct Unk80100DC054 *unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 unk69;
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

extern u8 D_800FE470;
extern s32 g_btlbgScrollPaused;
extern s32 g_btlbgRainCount;
extern MATRIX D_800F8D50;
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
extern s16 D_80100DCC;
extern s32 *g_btlfxCommonTable; /* offset table, entries relative to its start */
typedef struct {
    s16 unk0;          /* count of unk14 */
    s16 unk2;          /* count of unk4 */
    struct BtlUnit *unk4[4]; /* player units */
    s16 unk14[8][2];   /* screen positions */
} Unk80105020;
extern Unk80105020 g_btluiCursors;
typedef struct {
    u8 unk0;
    u8 unk1;
} Unk80105058;
extern Unk80105058 D_80105058;
extern s32 D_80105274;
extern u8 D_8010527C;
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
    u8 unk32[6];
    u8 unk38[6];
    u8 unk3E[6];
    u16 exp; /* exp */
    u16 gil; /* gil */
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
    s32 unk4;
    u32 prevents; /* statuses this one keeps from being added (btl_addStatus) */
} BtlStatusInfo; /* 0xC, one per status bit */

typedef struct Battle1538 {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
} Battle1538;

typedef struct {
    s32 unk0;
    struct BtlCmd *unk4;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 pad18[0x21 - 0x18];
    u8 unk21;
    u8 pad22[0x25 - 0x22];
    u8 unk25;
    u8 unk26;
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
    u8 unk0; /* enemy type */
    u8 unk1;
    u8 pad2[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Unk800C9EE0;

typedef struct {
    u8 unk0; /* weight */
    u8 unk1; /* count of unk8 */
    u8 unk2;
    u8 pad3;
    u16 unk4;
    u8 pad6[8 - 6];
    Unk800C9EE0 unk8[4]; /* enemy placements */
} Battle1330; /* 0x38 */

/* enemy type record as the scene sees it (Unk800C9D1C is the same record) */
typedef struct {
    u8 pad0[0x1E];
    u16 modelId; /* model (Unk800C9D1C.modelId) */
    u8 pad20[0x62 - 0x20];
    u16 unk62;
    u8 pad64[0x74 - 0x64];
} Battle1334; /* 0x74 */

typedef struct Battle11C020 {
    u8 unk0;
    u8 unk1; /* count of unk8 */
    u8 unk2; /* count of enemy records following unk8 */
    u8 unk3; /* count of commands following the enemy records */
    u16 unk4;
    u8 pad6[8 - 6];
    Battle1330 unk8[1];
} Battle11C020;

/* battle command (queued action) */
typedef struct BtlCmd8 {
    u32 targetType : 4; /* target type; 6-12 make enemy commands multi-target (btl_setEnemyCmd), 3-5 spread casts (btl_halveForSpread) */
    u32 unk0_4 : 4;
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
            u32 unk0_9 : 1;
            u32 shortSummon : 1; /* plays the ability's altEffect (btl_rollShortSummon) */
            u32 unk0_11 : 1;
        } b;
        u32 w; /* cleared as a word */
    } unk10;
} BtlCmd;

typedef struct BtlShadow {
    u32 shade : 8; /* grey level of the shadow quads (btl_drawShadow) */
    u32 width : 12; /* btl_setShadowSize */
    u32 depth : 12;
    u32 unk4_0 : 12;
    u32 unk4_12 : 10;
    u32 unk4_22 : 10;
    POLY_FT4 *unk8[2][4]; /* shadow quads, per draw buffer */
} BtlShadow; /* 0x28 */

typedef struct Battle11C0 {
    s32 unk0;
    s16 returnId; /* field id or world position to return to (btl_initReturnInfo) */
    u8 returnMode; /* GameState mode before the battle (btl_cleanup) */
    u8 unk7[1];
    s32 unk8;
    s32 *loadBuf; /* loader work buffer (CD requests and step states at 0x9FA0/0x9FD8) */
    s32 unk10;
    u8 *texts; /* enemy and enemy-ability names (BtlEnemyType.nameOfs, btl_showCmdName) */
    struct BtlBgHeader *bgHeader; /* background header (btlbg_init) */
    s32 bgData; /* background groups (btlbg_draw) */
    Battle11C020 *encounter; /* encounter header: groups, enemy records, abilities (btl_initScene) */
    u16 *sceneTables; /* two offsets: sequence table, scene data table */
    struct BtlShadow *shadows; /* per unit model slot (btl_drawShadow) */
    s32 ot[2]; /* small ordering table per draw buffer (btl_beginFrame) */
    s32 prims[2]; /* primitive buffer per draw buffer */
    s32 *weaponBufs; /* per party slot: weapon model file buffer (btl_loadWeaponModelsStep) */
    BtlUnit9C50 *bgTexAnims; /* background texture animations (btlbg_init) */
    u8 *heapStart; /* work heap start */
    u8 *heap; /* work heap cursor */
} Battle11C0;

typedef struct {
    u32 unk0_0 : 4;
    u32 unk0_4 : 1;
    u32 unk0_5 : 3;
    u32 unk0_8 : 21;
    u32 unk0_29 : 1;
    u32 unk0_30 : 1;
    u32 unk0_31 : 1;
    u8 pad4[8 - 4];
    u8 unk8;
    u8 pad9;
    u8 mpCost; /* runtime: Fire 6, Fira 12, Sleep 10 as the spell list shows */
    u8 padB[0xE - 0xB];
    u16 nameOfs; /* as BtlCmd8.nameOfs */
} Battle16CC; /* 0x10 */

/* Battle.unk1358 ring entry (btl_newPopup hands out the next one) */
typedef struct {
    u8 *unk0; /* animation, 0xFF-terminated */
    u16 unk4; /* glyph index into g_uiSprites */
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
            u8 unk0; /* delay */
            s8 unk1; /* y offset */
            u8 unk2;
            u8 unk3;
        } b;
    } unk10;
    union {
        u32 w; /* packed digits (btl_packDigits) */
        Unk800F6A54 *p;
    } unk14;
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
    } unk1C8;
    BtlEnemy *unk1CC;
    u8 unk1D0;
    u8 unk1D1;
    u8 pad1D2[0x1D4 - 0x1D2];
    u8 hiddenMask; /* units hidden from status effects (mask) */
    u8 pad1D5[0x1D8 - 0x1D5];
} BtlPopupRing; /* Battle.unk1358 (0x1D8) */

/* Inlined copy of btl_clearCmd (clear a command); the original source inlines it. */
static inline void btl_clearCmd_inline(BtlCmd *arg0) {
    arg0->next = 0;
    arg0->ability = 0;
    arg0->targets = 0;
    arg0->cmdNo = 0;
    arg0->subNo = 0;
    arg0->unk10.b.targeting = 0;
    arg0->unk10.b.unk0_2 = 0;
    arg0->unk10.b.automatic = 0;
    arg0->unk10.b.covered = 0;
    arg0->unk10.b.dodged = 0;
    arg0->unk10.b.reflect = 0;
    arg0->unk10.b.unk0_9 = 0;
    arg0->unk10.b.shortSummon = 0;
    arg0->unk10.b.unk0_11 = 0;
}

typedef struct Battle108C {
    DR_TPAGE unk0;
    POLY_F3 unk8;
} Battle108C; /* 0x1C */

/* target cursor: four faces per draw buffer */
typedef struct Battle108CSet {
    Battle108C unk0[4];
} Battle108CSet;

/* battle scene data (Battle.unk132C) */
typedef struct {
    u32 unk0; /* flags */
    Battle1330 *unk4;
    Battle1334 *unk8;
    u16 *unkC; /* model file table */
    s16 *unk10; /* offset table, entries relative to its start */
} Battle132C;

typedef struct {
    BtlUnit9C unk0;
    u8 pad7C[0x98 - sizeof(BtlUnit9C)];
} BtlModel; /* 0x98 */

/* GameData.unk8E4 while in battle */
typedef struct Battle {
    u16 flags; /* 1 background loaded (btlbg_draw draws), 2 enemy models loaded, 4 party models loaded, 0x10 UI started, 0x20 disc cover open, 0x40 exit preloaded, 0x1000 battle over (btl_main) */
    u8 unk2[2];
    s32 unk4;
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
    u16 cmdFlags; /* 1 escape queued, 4/8 trance Eidolon timer running/fired, 0x10 Spear pending */
    u8 cmdPhase; /* curCmd's step in btl_processCmdQueue (0 check, 1 start effect, 2 wait, 3 finish) */
    u8 eidolonAbility; /* ability of the trance Eidolon follow-up (btl_startCmdEffect, BTL_CMD_EIDOLON) */
    s16 eidolonTimer; /* counts down by atbSpeed until BTL_CMD_TRANCE_EIDOLON */
    u16 unk11BE;
    struct Battle11C0 *work; /* battle work area: buffers, background, encounter, shadows (btl_allocBuffers) */
    struct Battle11C4 seq; /* enemy battle sequences (btlseq_*) */
    u8 unk131C;
    u8 pad131D[0x132C - 0x131D];
    Battle132C scene;
    u8 unk1340;
    u8 pad1341[0x1354 - 0x1341];
    BtlFiles *files;
    BtlPopupRing popups;
    s32 unk1530;
    s32 dataSector; /* battle data file sector */
    struct Battle1538 **unk1538;
    s32 unk153C[4];
    BtlStatusInfo statusInfo[32];
    Battle16CC abilities[0xC0]; /* runtime: BtlCmd.ability = &abilities[subNo] (Attack 176, Focus 188) */
    s32 statusSets[64]; /* status masks indexed by ability/weapon status-set bytes (btl_applyAbilityStatus) */
} Battle; /* 0x23CC */

/* battle work area, allocated at g_loadAreaEnd + 0x61800 */
typedef struct {
    Battle unk0;
    u8 unk23CC[0x240C - 0x23CC];
    u8 unk240C[0x241C - 0x240C];
    u8 unk241C[1]; /* becomes g_modeHeap */
} BattleMem;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk80100D88;
extern Unk80100D88 g_btlfxOpArgs;


typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 pad6;
    u8 unk7;
} Unk800BD8C0;

typedef struct {
    u16 unk0;
    u16 unk2;
} U16Pair;

extern BtlUnit *D_800FE484;

typedef struct {
    s16 unk0[4];
    u8 unk8;
    u8 unk9;
    u8 padA[0xC - 0xA];
} Unk800C412C0; /* 0xC */

typedef struct {
    Unk800C412C0 unk0[2];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
} Unk800C412C; /* 0x20 */

typedef struct Unk800FE3B8 {
    SVECTOR points[6]; /* camera path control points (cubic, btlcam_step); [4]/[5] keep the eye/target for script flag 0x80 */
    SVECTOR unk30[4];
    s16 yaw; /* from eye to target (btlcam_step) */
    u8 unk52;
    u8 unk53;
    SVECTOR eye; /* eye position given to btlcam_lookAt */
    u8 unk5C;
    u8 unk5D;
    u16 projDist; /* projection distance (SetGeomScreen through GameData.unkA) */
    u16 baseProjDist; /* default projection distance (0x200) */
    u8 pad62[0x65 - 0x62];
    u8 unk65;
    u8 unk66;
    u8 unk67;
    Unk800C412C unk68[2];
    s32 unkA8;
    u32 unkAC;
    s32 unkB0;
    s32 unkB4;
    u8 unkB8;
    u8 unkB9;
    u8 unkBA;
    u8 unkBB;
    SVECTOR unkBC;
    SVECTOR unkC4;
    BtlUnit *unkCC;
} Unk800FE3B8;
extern Unk800FE3B8 g_btlCamera;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC[16];
    u8 unk1C;
    u8 unk1D;
} Unk800FDF30;
extern Unk800FDF30 g_btlRewards;



typedef struct {
    u8 pad0[0xC];
    s32 unkC;
} Unk800DA1B88;

typedef struct Unk800DA1B8 {
    s16 unk0; /* entry stride in bytes */
    s16 unk2; /* entry count */
    s32 (*unk4)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32);
    Unk800DA1B88 *unk8;
    s32 unkC[1]; /* entries: s16 in use, s16, data */
} Unk800DA1B8;

extern s32 D_800FEC38;
extern u8 *D_800FEC3C;
extern s16 D_80100D8C;
extern u16 D_800F7460[];

typedef struct LinkNode {
    struct LinkNode *unk0;
    u8 pad4[0x10 - 4];
    u32 unk10;
} LinkNode;
void btl_linkCmd(LinkNode *, LinkNode **);
Unk16D8 *evt_getBattleObjects(void);
s16 btl_getEnemyModelId(s16);
void btl_resetUnit(struct BtlUnit *, struct Unk8E0 *);
void btl_setupEnemies(void);
void btlseq_init(void);
extern u8 D_800F5244[];

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C[4];
} Unk800B44A0;


typedef struct {
    u8 unk0;
    u8 pad1[7];
    u16 unk8;
    u16 unkA;
} Unk800BC1E0;

typedef struct {
    u8 pad0[0x33];
    u8 unk33;
    u8 pad34[0x38 - 0x34];
    u8 unk38;
    u8 pad39[0x3C - 0x39];
    u8 unk3C;
    u8 pad3D[0x66 - 0x3D];
    s16 unk66;
} Unk800C09D8;

typedef struct {
    u8 pad0[0x2E];
    u8 unk2E;
} Unk800BD8F0;


typedef struct Heap {
    u8 *unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC[1];
} Heap;

/* effect task (g_btlfxTasks, btlfx_spawnTask): an op handler called with mode 0 size query, 1 start, 2 step */
typedef struct {
    s32 frame; /* steps run */
    union {
        s32 w; /* cleared as a word */
        s16 h;
    } state; /* 0 free */
    s16 file; /* effect file slot the op came from */
    s16 unkA; /* use the shared heap */
    void *work; /* work data (size from mode 0) */
    struct Unk800DA1B8 *pool; /* element pool (btlfx_poolInit) */
    s32 (*handler)(s32, void *, Unk800F78A4 *, void *, void *);
} Unk80100C78Entry; /* 0x18 */

/* effect handler: (mode, work, camera, entry, args) */
typedef s32 (*EffFn)(s32, void *, Unk800F78A4 *, void *, void *);
extern EffFn *D_80100C70[];
extern EffFn g_btlfxResidentOps[];
extern EffFn g_btlfxUnitOps[];

/* filled by an EffFn in mode 0 */
typedef struct {
    s32 unk0; /* work size */
    s32 unk4;
    s32 unk8;
    s32 padC[2];
    s32 unk14;
} EffInit;

s32 btlfx_allocTaskMem(Unk80100C78Entry *, s32);
s32 btlfx_poolSize(s32, s32);
void btlfx_poolInit(struct Unk800DA1B8 *, Unk80100C78Entry *, s32, s32, s32);

typedef struct {
    u16 unk0;
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u16 pad6;
    u16 unk8; /* abr, 0xFF = opaque */
} SprInfo; /* sprite texture info (btlfx_addSpriteQuad) */

void btlfx_addPrim(u32, s32, void *);

/* scrolling/waving 2D background layer (btlfx_drawBackdrop) */
typedef struct {
    u16 unk0; /* kind: 0 scroll, 1 horizontal wave, 2 fade, 3 vertical wave */
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u8 unk6;  /* u */
    u8 unk7;  /* v */
    u16 unk8; /* abr */
    u16 unkA; /* palette frames */
    s32 unkC; /* palette animation offset */
    union {
        struct BtlBgScrollP {
            u8 unk0; /* x: sine motion */
            u8 unk1; /* y: sine motion */
            u8 pad2[2];
            s16 unk4; /* x speed */
            s16 unk6; /* y speed */
            u16 unk8; /* x amplitude */
            u16 unkA; /* y amplitude */
        } scroll;
        struct BtlBgWaveP {
            u16 unk0; /* strip size */
            u16 unk2; /* strip count */
            u16 unk4; /* phase step */
            u16 unk6; /* phase speed */
            u16 unk8; /* amplitude */
            s16 unkA; /* scroll speed */
        } wave;
        struct BtlBgFadeP {
            s32 unk0; /* data offset */
            u8 pad4;
            u8 unk5;  /* delay */
            u16 unk6; /* duration */
            s16 unk8;
            s16 unkA;
        } fade;
    } unk10;
} BtlBgLayer;

typedef struct {
    u16 unk0; /* layer count */
    u8 unk2;  /* width */
    u8 unk3;  /* height */
    u16 unk4; /* x */
    u16 unk6; /* y */
    s32 unk8[1]; /* layer offsets */
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
    u16 unk0; /* frame count (& 0x7FFF) */
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u8 unk6;  /* height */
    u8 unk7;  /* width */
    u16 unk8[1];
} Unk800D1D74Hdr;
/* sprite set draw context (btlfx_drawAnimQuads, btlfx_drawSprites and their per-sprite workers) */
typedef struct {
    u16 *unk0; /* palette */
    u16 *unk4; /* colour table */
    s32 unk8;  /* from the width byte */
    s32 unkC;  /* from the height byte */
    s32 unk10; /* tpage */
    s32 unk14; /* clut */
    s32 unk18;
    s32 unk1C; /* sprite count */
    s32 unk20;
    s32 unk24; /* code */
} Unk800D3ED4Ctx;
void btlfx_emitAnimQuads(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitSprites(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitFixedSprites(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
void btlfx_emitDots(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);
DR_TPAGE *btlfx_allocDrMode(s32, s32);
void btlfx_drawBackdropSprite(u8 *, s32, s32, s32, s32, s32);


typedef struct {
    s32 unk0;
    Unk80100C78Entry unk4[11];
} Unk80100C78;
extern Unk80100C78 g_btlfxTasks;
extern Heap D_800FF470; /* 0x1800 bytes */

typedef struct {
    s16 unk0[6][2];
} BtlTexInfo; /* 0x18, passed to btl_applyModelTextures */

typedef struct {
    BtlUnit9C *unk0;
    BtlTexInfo unk4;
    u8 unk1C; /* in use */
    u8 unk1D;
    s16 unk1E; /* index */
    s16 unk20; /* kind */
    s16 unk22;
    s16 unk24;
    u8 pad26[0x28 - 0x26];
} BtlObjSlot; /* 0x28 */
extern BtlObjSlot g_btlfxEffModels[32];


typedef struct {
    BtlUnit9C *unk0;
    s16 unk4[6][2];
    s16 unk1C[6][4];
    u8 unk4C; /* in use */
    u8 unk4D;
    s16 unk4E; /* index */
    u16 unk50; /* frame */
    u8 pad52[0x54 - 0x52];
} Unk8010141C; /* 0x54 */
extern Unk8010141C g_btlfxSummonModels[1];


extern u8 D_800FE415;
extern u8 D_800FEC65;
extern s32 g_btlfxDrawSyncState;
extern u16 D_800F73A0[][19];
extern u8 D_800F6CB4[];
extern char D_800A7F60[];
extern char D_800A7F78[];
extern char D_800A81A8[];
/* effect element (0x28) in an effect file, after the header */
typedef struct EffElem {
    u8 unk0; /* kind, 9 = no texture */
    u8 unk1; /* 0xFF = no second texture */
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
    s32 unk1C; /* offset, relocated on load */
    s32 unk20; /* offset, relocated on load */
    void (*unk24)();
} EffElem; /* 0x28 */

typedef struct {
    s32 unk0; /* count of unk10 weights */
    s16 unk4; /* count of EffElem following */
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s16 unkE; /* 1 = blend weights animated */
    union {
        s32 w;
        u8 b[4];
    } unk10;
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
    EffFileHdr *unk0; /* effect file */
    EffSub *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    EffElem *unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    EffTail *unk24;
    SVECTOR *unk28;
    s32 unk2C;
    s16 unk30; /* in use, -1 = release */
    s16 unk32;
    XY32 unk34; /* live particle counts (s.x, s.y) */
    s16 unk38; /* frame */
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
    SVECTOR unk5C;
    SVECTOR *unk64;
    SVECTOR *unk68;
} Pool6CEntry; /* 0x6C */

typedef struct {
    s32 unk0; /* count */
    s32 unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry unk10[1];
} Pool6C;

typedef struct {
    struct {
        s32 vx, vy, vz;
    } unk0; /* position, 20.12 */
    s16 unkC; /* rotation */
    s16 unkE;
    s16 unk10;
    u8 unk12; /* flags */
    s8 unk13;
    XY32 unk14;
    s16 unk18;
    u8 unk1A; /* frame */
    u8 unk1B; /* frame count */
    u8 unk1C; /* element index, 0xFF = none */
    u8 unk1D; /* element frame */
    s8 unk1E;
    u8 unk1F; /* in use */
    s16 unk20;
    s16 unk22;
    XY32 unk24;
    XY32 unk28;
    Pool6CEntry *unk2C;
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
    s32 unk0; /* count */
    s32 unk4;
    s32 unk8;
    Pool30Entry *unkC;
} Pool30;

extern Pool30Entry g_btlfxParticleBuf[0x100];
extern Pool30 g_btlfxParticles;
extern Pool6C g_btlfxEmitters; /* 0x920 bytes */
extern Unk80100D88 g_btlfxResArgs;
extern Unk80100D88 g_btlfxResArgs2;
extern s32 g_btlfxSavedGeomScreen;
extern MATRIX g_btlfxSavedView;
extern u16 D_800F8D0C[];
extern Unk800F6A54 D_800F6A54[];
extern char D_800A7F34[];

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
} Unk800BA938;

typedef struct {
    u8 pad0[0x8C];
    s32 unk8C[3];
    s32 *unk98;
} Unk800CB090;


extern RECT g_btlClutRect;
extern u16 g_btlClutBuf[][0x100]; /* CLUT rows */
extern u32 *g_btlSavedCluts;
extern s32 D_800FE490;


extern u16 D_800FDF38;
extern u8 D_800F8CF4[][4];
extern char D_800A825C[];

typedef struct Unk800D8EA4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 unk4;
} Unk800D8EA4;

typedef struct Unk800D7E14 {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
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
    u16 unk0; /* sound id, 0 = free */
    s16 unk2; /* age */
    s32 unk4; /* unit id */
} Unk80104E30Entry;

typedef struct {
    s32 unk0;
    Unk80104E30Entry unk4[6];
} Unk80104E30;
extern Unk80104E30 D_80104E30;

extern u8 D_800FDF3A;
extern u8 D_800FDF3B;
void btlbg_init(struct BtlBgHeader *, BtlUnit9C50 *);
void btl_initCmdQueue(struct Battle *);
void btl_initTargetCursor(struct Battle *);
void func_800C972C(void);
s16 btl_loadModelsStep(s32);

extern char g_btlfxErrorMsg[];

/* summon model header ('so' magic) */
typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800E9000Ent;

typedef struct {
    u16 unk0; /* 0x6F73 */
    u16 unk2;
    u16 unk4; /* offset */
    u16 unk6; /* offset */
    Unk800E9000Ent unk8[1];
} Unk800E9000;

typedef struct Unk80100DC054 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
    s32 unk8;
    u8 padC[0x10 - 0xC];
    s32 unk10;
    s32 unk14;
    u16 unk18[6];
    u16 unk24[6];
    u16 unk30[6];
    u8 *unk3C;
    BtlUnit9C50 *unk40;
    u8 *unk44;
    s32 unk48;
    s32 unk4C;
    s32 *unk50[16];
    BtlModel unk90;
    u8 pad128[0x180 - 0x128];
    s32 unk180[1];
} Unk80100DC054;

typedef struct {
    u16 unk0;
    u8 pad2[2];
    s32 unk4;
} Unk80104E34; /* 8 */
extern Unk80104E34 D_80104E34[6];

extern u16 D_800F51E8[];
extern u8 D_800F8D70[];
extern s32 g_btlfxCameraOn;
extern u8 D_800F8D78[5]; /* battle message ids */
BtlUnit *btlfx_getFirstTarget(void);
extern u8 D_800F8D80;
extern u8 D_800F8D81;
extern char g_battleText[];

extern u8 D_800F909C[];
extern void (*g_btluiMenuUpdate[])(void);


typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    u8 pad8[0xC - 8];
    s32 unkC;
} Unk80104F90;
extern volatile Unk80104F90 g_btluiHelp;
extern u8 D_80104E75;

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
extern s32 D_800F9028[];
extern Unk800F3F10 D_800F9084;
extern Unk800F3F10 D_800F9090;

extern void *g_btluiHelpText;
extern s32 (*g_btluiMenuHelp[])(void);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk80104FC0; /* g_btlCmdMenu.unk10 */

/* battle command menu (runtime: Vivi's menu -> Eiko's, docs/runtime-findings.md) */
typedef struct {
    u8 cursor;    /* row: 0 Attack, 1, 2, 3 Item */
    u8 unk1;
    u8 charIndex; /* GameData.characters index of the commanding member */
    u8 pad3;
    u8 cmdIds[4]; /* command per row: Attack 1, Blk Mag 22, Focus 13, Wht Mag 19, Summon 20, Item 14 */
    u8 unk8[4];
    u8 unkC[4];
    Unk80104FC0 unk10;
    u8 pad14[0x18 - 0x14];
    BtlUnit *unit; /* the unit choosing a command */
} Unk80104FB0;
extern Unk80104FB0 g_btlCmdMenu;
extern Unk80104FC0 D_80104FC0; /* g_btlCmdMenu.unk10, also addressed by its own label */
extern void (*g_btluiMenuInput[])(void);



typedef struct Unk80104FD4 {
    u8 pad0[6];
    u8 unk6;
    u8 unk7; /* count */
    s32 unk8;
} Unk80104FD4;
extern s32 (*D_800F8E58[])(void);
extern void (*D_800F8E48[])(s32, s32, s32, MenuView *);

typedef struct {
    u8 returnState; /* menu state btlui_cancelTarget returns to */
    u8 cmdNo;
    u8 targetType; /* target type of the ability (D_800F8E84 / D_800F8E68 by type; 0xC all, 0xD self) */
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 targetAll; /* the whole side is marked */
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 side; /* side the cursor is on (unit flag bit 0) */
    u8 allowKo; /* KO units can be targeted */
    u8 windowOpen; /* target-name window open (btlui_openTargetWindow) */
    u8 halfMp; /* the caster has support ability 0xE2 (MP costs halved, btlui_isCmdUsable) */
    u8 unkE;
    u8 unkF;
    u8 doubleCast; /* Dbl Wht / Dbl Blk: the first pick is stored (btlui_confirmDoubleCast) */
    u8 unk11;
    s8 unk12;
    u8 firstCmdNo; /* double cast: first pick */
    u8 firstTargeting;
    u8 pad15[0x18 - 0x15];
    s16 firstSubNo;
    s16 subNo; /* ability or item id */
    s32 firstTargets;
    BtlUnit *unit; /* the unit choosing targets */
    BtlUnit *unk24; /* last target */
} Unk80104FF0;
extern Unk80104FF0 g_btlTarget;
extern u8 D_800F8EE4[];
extern void (*D_800F8F04[])(Window *);

typedef struct {
    s32 unk0;
    s32 unk4; /* next free */
    s32 unk8;
    s32 unkC; /* used */
    s32 unk10; /* free */
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
extern s32 D_800FDE3C[];
extern s16 D_800F5850[];
/* enemy type record (btl_initEnemyType; 0x74 bytes, the encounter header's records after its groups; Battle1334 is the
 * same record) */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    s32 unk14;
    s32 unk18;
    u16 unk1C;
    u16 modelId; /* model (key 0x02xxxx; FF9.IMG dir index 7, file id = model id): btl_getEnemyModelId via Battle1334 */
    u16 motionIds[6]; /* motions (keys 0x03xxxx, in the model file): BtlEnemyType.motions */
    s32 meshMasks; /* BtlUnit.hiddenMeshes | vanishMeshes << 16: mesh-group bits (btl_setEnemyStats) */
    u16 unk30;
    u8 pad32[0x34 - 0x32];
    u8 unk34[4];
    u8 pad38[0x3C - 0x38];
    s32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 pad42;
    u8 unk43[4];
    u8 unk47;
    u8 bones[3]; /* BtlEnemyType.bones: camera/effect anchor bones */
    u8 baseBone; /* BtlUnit.baseBone */
    u16 unk4C;
    u8 unk4E;
    u8 unk4F;
    u8 unk50[6];
    u8 unk56[6];
    u8 unk5C[6];
    u8 pad62[0x64 - 0x62];
    u16 unk64;
    u16 unk66;
    u8 shadowBone; /* BtlUnit.shadowBone */
    u8 unk69;
    s16 unk6A;
    s16 unk6C;
    u8 shadowDirBone; /* BtlUnit.shadowDirBone */
    u8 pad6F[0x74 - 0x6F];
} Unk800C9D1C;
extern u32 D_80104E80;
typedef struct {
    SPRT unk0[4][2];
    DR_TPAGE unkA0[2];
    SPRT unkB0[2];
    SPRT unkD8[2];
    DR_TPAGE unk100[2];
    SPRT unk110[4][2];
    SPRT unk1B0[4][2];
    SPRT unk250[4][2];
    DR_TPAGE unk2F0[2];
    DR_TPAGE unk300[2];
    POLY_G4 unk310[2];
    DR_TPAGE unk358[2];
} Unk801050D8; /* 0x368, double-buffered menu primitives */
extern Unk801050D8 *g_btluiPrims;
extern u8 g_btluiBuf;
extern u8 D_80104E94[2][0x18];

typedef struct {
    s32 unk0; /* frames */
    s16 unk4;
    s16 unk6;
    s32 unk8;
} Unk800E6270;
s32 btlfx_fadeUnitGroup(u32, s32);

/* main exe: candidates for ff9.h */
extern Unk800689B0 D_80068E78[3];
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
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u8 pad4;
    u8 unk5; /* attack */
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
extern Unk8006B1F4 D_8006B1F4[];
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
void func_800EA908(Unk80104F88Entry *);
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
void func_800EA518(void);
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
    s16 unk0; /* offset from unk4 */
    s16 unk2;
} TextTblEnt;

typedef struct {
    u16 unk0; /* count */
    u16 pad2;
    TextTblEnt unk4[1];
} TextTbl; /* message table (D_800F6CB4) */

void text_formatInt(s32, u8 *, s32);

u8 *btl_getItemName(u8);
typedef struct {
    u8 pad0[8];
    u8 unk8[4]; /* steal items, 0xFF = none */
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
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
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
void func_800DA4E8(void);
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

/* func_800E4634 work area */
typedef struct {
    s32 unk0; /* next point, -1 = done */
    s32 unk4; /* timer, 0xFFFF = skipped */
    SVECTOR unk8[4];
    s16 unk28[3];
    u8 pad2E[2];
    Pool6CEntry *unk30[4];
} Unk800E4634;

typedef struct {
    u8 pad0[0x27];
    u8 unk27;
} Unk800E4634Arg2C;

typedef struct {
    u8 pad0[0xC];
    Unk800E4634Arg2C *unkC;
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
    u16 unk0; /* frame count */
    s16 unk2;
    u16 unk4; /* gouraud flag */
    u16 unk6;
} Unk800D2938;

typedef struct {
    s32 unk0; /* primitive code */
    s32 unk4; /* ot base z */
    s32 unk8;
    s32 unkC;
    u16 *unk10;
    s32 unk14;
    s32 unk18; /* tpage */
    u32 unk1C; /* set by btlfx_emitFrameQuad: the quad's last corner (x word, z) */
    u32 unk20;
    u8 pad24[0x28 - 0x24];
} Unk800D57D0Ctx;

typedef struct {
    s32 unk0; /* primitive code */
    s32 unk4; /* ot base z */
    s32 unk8;
    s32 unkC; /* tpage */
    s32 unk10;
    s32 unk14;
} Unk800D59D0Ctx;

void btlfx_emitFrameQuad(u8 *, Unk800D57D0Ctx *, u8 *, s32 *);
void btlfx_emitFrameFan(u8 *, Unk800D59D0Ctx *, u8 *, s32 *);
/* asm convention: packet $a2, OT $a3, SZ $t2, z base $t4 (src_equiv/ovl_0b/frames.c) */
void btlfx_linkFrameTri(u8 *, s32 *, u32, s32);
void btlfx_drawFan(Unk800D2938 *, SVECTOR *, SVECTOR *, s32, s32, s32, s32);
void func_800E2F84(Pool30 *, Pool30Entry *, SVECTOR *, SVECTOR *);
extern s32 D_801014B0; /* saved g_btlfxEnv->unk28 */
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
void func_800E9D84(void);
void btlui_addItem(s32);
void btlui_reserveItem(s32);
void btlui_unreserveItem(s32);
void btlui_buildCmdList(void);
extern RECT D_800F8DDC; /* command window rect */
extern RECT D_800F8E10; /* title bar texture window (btlui_drawSideFrame) */
void btlui_drawCmdList(Window *);
void btlui_drawCmdOverlay(Window *);
void btlui_onCmdMenuOpened(); /* used as a Window callback */
s32 btlui_setMenuState(s32);
u8 func_800F1360(void);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s16 unk8[4];
    s16 unk10[4];
} Unk800D955C;

typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800E8F50Entry;

typedef struct {
    u16 unk0; /* magic 'so' */
    u16 unk2;
    u16 unk4; /* offset */
    u16 unk6; /* offset */
    Unk800E8F50Entry unk8[1];
} Unk800E8F50;

typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    u8 pad4[0x10 - 4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} Unk800AA08C;

typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 padC[0xE - 0xC];
    s16 unkE;
    union {
        s32 w[4];
        u16 h[8];
    } unk10;
    s32 unk20;
    s32 unk24;
} Unk800AA958; /* party model load state */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk800AA958Req; /* CD read request */

/* btl_swapPartyModelStep: model texture table (db_lookup section 2, tag 0x12) */
typedef struct {
    u16 unk0; /* model id */
    u8 pad2[4 - 2];
    union {
        s32 w; /* low 24 bits: offset of the texture lists from this entry */
        u8 b[4]; /* b[3]: texture count */
    } unk4;
    u8 pad8[0xC - 8];
} Unk800ABBC4Ent; /* 0xC */

typedef struct {
    u8 pad0;
    u8 unk1; /* entry count */
    u8 pad2[4 - 2];
    Unk800ABBC4Ent unk4[1];
} Unk800ABBC4Tbl;

typedef struct {
    s16 unk0;
    s16 unk2;
} Unk800ABBC4Tex;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Unk800ABBC4Rect;

u8 cd_readChunks(s32, void *, s32, s32);
typedef struct {
    u8 pad0[8];
    s32 unk8; /* file address */
    s32 unkC; /* bytes done */
    s32 unk10; /* chunk size */
} Unk800AE844; /* streamed file read */

extern s16 D_800F51EC;
s32 db_getSize(s32);
s32 func_80056048(s32 *, s32, s32);

/* btl_soundCmd: sound/streamed music command handler */
s32 btl_getSoundFileSectors(s32, s32 *);
s32 btl_getDepLink(s32);
s32 func_800AE7C8(s32);
void btl_onSoundRead(Unk800AE844 *);
extern s32 D_800F51F0;
extern u8 D_800F51F2; /* byte 2 of D_800F51F0 */
extern s32 D_800F51F4;
extern s8 D_800F51F8;
/* main exe: candidates for ff9.h */
void akao_stopMusic(s32);
s32 akao_playMusic(void *);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);
void akao_sendCmd92(s32);
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
    u16 unk0; /* text offset into g_btlCmdNames */
    u16 unk2; /* text offset into g_btlCmdHelp */
    u8 pad4[6 - 4];
    u8 unk6;
    u8 unk7;
    s32 unk8;
} Unk800FD56C; /* 0xC */
extern Unk800FD56C g_btlCmdTable[];
extern u8 g_btlCmdHelp[];
extern u16 D_800FD36C[];
extern u8 D_800FB4F0[];
extern s32 D_800F8E04[];
extern RECT D_800F8EF4;
extern RECT D_800F8EFC;
Window *win_open(RECT *, s32, s32, u32);
void btlui_onTargetWindowOpened(Window *);
void btlui_drawListWindow(Window *);
void btlui_drawTargetList(Window *);
void btlui_drawTargetListOverlay(Window *);
void btlui_onSideCmdClosed(Window *);

typedef struct {
    s32 unk0;
    u32 unk4 : 24; /* offset of the tables from this header */
    u32 unk7 : 8;  /* table length */
} Unk800AB758;

/* rotation part of a MATRIX, used where no translation is stored */
typedef struct {
    s16 m[3][3];
} RotMatrix33;

/* packed screen coordinate */

typedef struct {
    u32 *unk0; /* next free primitive */
    u32 *unk4; /* ordering table entry */
    s32 unk8;  /* rgb */
    u8 padC[0x10 - 0xC];
    XY32 unk10;
    u16 unk14; /* clut */
} Unk800CCD00;

void btl_drawPopupGlyph(Unk800CCD00 *, Unk800686E8 *);
void btl_drawPopupDigits(Unk800CCD00 *, u32);
void btl_clampPopup(struct BtlPopup *, Unk800CCD00 *, s32, s32);
extern void (*D_800F6A7C[])(struct BtlPopup *, Unk800CCD00 *);
void btl_drawStatusCountdown(Unk800CCD00 *);
void btl_drawStatusIcons(Unk800CCD00 *);

/* status-effect sprites drawn on battle units (btl_drawStatusIcons) */
typedef struct {
    s32 unk0; /* status mask that shows it */
    s32 unk4; /* status mask that hides it */
    u8 *unk8; /* sprite data */
    u8 unkC;  /* attach point index */
    u8 unkD;  /* semi-trans mode */
    u8 unkE;  /* flat (btlfx_drawSprite) */
    u8 unkF;  /* turns with the unit */
} Unk800F6A88;

extern Unk800F6A88 g_btlStatusIcons[];
/* per player model (Character.unk34.b.unk4): attach points (bone, y, z) */
extern u8 D_800F6B48[][6];
extern u8 D_800F6BBC[][6];
extern u8 D_800F6C30[][6];

/* status countdowns drawn as digits over battle units (btl_drawStatusCountdown) */
typedef struct {
    s32 unk0; /* status mask */
    s16 unk4; /* status timer index (BtlUnit.statusTimers) */
    u16 unk6; /* nonzero: colour by level */
} Unk800F6CA4;

extern Unk800F6CA4 D_800F6CA4[];

void btlfx_beginPrims(u8 *, s32 *);
u8 *btlfx_endPrims(void);
void btlfx_drawSprite(u8 *, SVECTOR *, s32, s32, s32);
void btl_updateScan(void);
void btl_updateDetect(void);
extern u8 D_800F6A28[];
extern u8 D_800F6A38[];
/* main exe: candidates for ff9.h */
extern Unk800686E8 D_80068BE8;
s32 evt_battleTrigger(s32, s32, s32, s32);
void math_rotMatrixXYZ(void *, void *); /* rotation -> matrix */
s32 btlfx_transformPolar(SVECTOR *, s32, s32);
void btlfx_transformPoint(s32, s32, s32);
extern Unk800686E8 D_80068BF0[]; /* digits */

typedef struct {
    u8 *pc; /* camera script pointer (btlcam_step reads its commands) */
    u32 frame; /* script frame, compared with each command's 10-bit frame */
    s32 unk8;
    s32 unkC;
    u32 sway; /* sway: amplitude bits 4-7, speed bits 0-3 (btlcam_applySway) */
    s32 swayPhase;
    s32 unk18;
    s32 shakeAxes; /* shake: bit 0 no vertical, bit 1 no horizontal (btlcam_applyShake) */
    s32 shakeFrames;
    s32 shakeLeft;
    s32 shakeFrom;
    s32 shakeTo;
    s32 zoomFrames; /* projection-distance tween (btlcam_step) */
    s32 zoomLeft;
    s32 zoomTarget; /* 1 base distance (g_btlCamera.unk60), 2 current (unk5E) */
    s32 zoomFrom;
    s32 zoomTo;
    u8 unk44;
    u8 unk45;
    s16 casterAnchors[5]; /* btlcam_getUnitAnchors of the acting unit */
    s16 partnerAnchors[5]; /* btlcam_getUnitAnchors of the partner (effect flag 4) */
    s16 targetAnchors[5]; /* btlcam_getUnitAnchors of the target */
} Unk800FE488; /* 0x64 */
extern Unk800FE488 g_btlCamScript;
extern u32 D_800F8EA4[][4];
extern void (*D_800F8E94[])(void);
extern RECT D_800FDE58;
u8 *btl_buildBannerFrame(u8 *, RECT *);
void btl_applyModelScale(BtlUnit9C *);
void btl_scaleModelMatrix(BtlUnit9C *, VECTOR *);
void btlcam_getUnitAnchors(BtlUnit *, s16 *);
s32 btlfx_rand(void);
s32 btlfx_opJumpHome(s32, Unk800D955C *, s32, s32 *, s32 *);
s32 btlfx_opWaitSameSide(s32, u8 *, s32, s32 *, s32 *);


extern u8 D_80104E6A;
extern u8 D_80104E6B;
extern u8 D_800F8E84[];
extern u8 g_btlCharTranceCmds[][3];
extern u8 g_btlCharCmds[][2];
extern RECT D_800A8584;
void btl_initUnitCmds(BtlUnit *);
extern u16 g_btlCharModelIds[];
void btl_initPartyUnit(Character *, BtlUnit *, u16, u16);
void btl_insertUnit(BtlUnit *);
void btl_placeParty(void);
void btl_initDotTimer(BtlUnit *, s32);
void func_800BBF30(EvtObject *, s16);
extern s32 D_800FDF34;
extern RECT D_800A858C;
extern RECT D_800A8594;
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
extern u8 D_800F7124[];
extern u8 D_800F71E4[];
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
    u32 unk0; /* uv/clut */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Unk80068EE8; /* sprite descriptor */
extern Unk80068EE8 D_80068EE8[];
extern u16 D_80068EEE; /* D_80068EE8[0].unk6 */
extern Unk80068EE8 D_80068E90;
extern Unk80068EE8 D_80068ED0;
extern Unk80068EE8 D_80068ED8;
extern Unk80068EE8 D_80068EE0;
extern u8 D_80104E7D;
extern SprtWords D_801050A0;
extern SprtWords D_801050C0;
extern DR_TPAGE D_80105080;
extern DR_STP D_80105090;
extern u32 D_801050D4;
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
    s16 unk0; /* dest x */
    s16 unk2; /* dest y */
    s16 unk4; /* w */
    s16 unk6; /* h */
    s16 (*unk8)[2]; /* source x/y per frame */
} TexAnimRect; /* 0xC */

typedef struct {
    u8 unk0; /* flags */
    u8 unk1; /* frame count */
    s16 unk2; /* speed (20.12) */
    u16 unk4;
    u16 unk6;
    s32 unk8; /* position (20.12) */
    u8 unkC; /* rect count */
    u8 unkD; /* index into the offset table (btl_relocTexAnims) */
    u16 unkE; /* last frame shown */
    TexAnimRect *unk10;
} TexAnim; /* 0x14 */

typedef struct {
    u16 unk0; /* count */
    u16 pad2;
    TexAnim unk4[1];
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
u8 func_800B9DDC(BtlCalc *);
u8 btl_checkNotSelf(BtlCalc *);
u8 btl_guardIfDeathImmune(BtlCalc *);
void btl_setAbilityPhysParams(BtlCalc *);
void btl_setMagicParams(BtlCalc *);
void btl_setMultiplierFromPower(BtlCalc *);
void btl_setWeaponAbilityParams(BtlCalc *);
void btl_setEnemyAttackParams(BtlCalc *);
void btl_applyCasterStatusMod(BtlCalc *);
void func_800BA308(BtlCalc *);
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
extern s32 D_800F8DE4[];
Window *btlui_getTargetWindow(void);
void btlui_getHelpRect(RECT *, RECT *);
void btlui_updateHelpText(void);
s32 text_getColor(void);
void text_setColor(s32);
void gfx_setBufferIndex(u8);
void win_drawHelpFrame(RECT *, s32, s32, s32);
extern RECT D_800F8DBC;
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
s32 func_800C4E84(s32, s16 *, s16 *, s32 *);
void func_800C4DC4(s32, s32, s16 *, s16 *, s32 *);
s32 btlcam_getAnchorUnit(s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s16 unkC[4];
    s16 unk14[4];
} Unk800D7C9C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s32 unk8;
    s16 unkC[3]; /* rotation delta (btlfx_stepFaceTarget) */
} Unk800D805C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s16 unk8[3];
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
extern u8 D_80104E76;
void bcopy(void *, void *, s32);
struct Battle11C020 *btl_initScene(void);
u8 pad_isRepeatEnabled(s32, s32);
void pad_setRepeatTiming(s32, s32, s32, s32);
void pad_clearRepeats(s32);
u8 btlui_anyOnSecondPort(void);
void btlfx_getUnitRot(s32, s16 *);

extern u16 D_800F524C[][0x22];
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
void akao_sendCmd99(s32);
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
extern u8 D_800F6FB8[];
extern u8 D_800A7C20[]; /* "/\xFF" suffix string */
void btl_setBanner(s32, s32);
void btl_setBannerAt(s32, s32);
s32 func_800F4FC8(void);
extern u8 D_80104E77;
void btlui_onUnitUntargetable(BtlUnit *);
void btlui_onUnitTargetable(BtlUnit *);
u32 btl_registerModel(s32, s32);
void btl_load(); /* takes the battle id (s32); unprototyped, the caller passes an s16 unconverted */
s32 func_800AD4AC(s32, s32);
s32 evt_countNpcActors(s32);
void func_8001E3A4(void *);
void btl_makeEnemyGreyCluts(void);
void btl_execCmd(struct BtlUnit *);
void btl_onCmdEffectDone(void);
void btl_enqueueCmd(struct BtlCmd *);
void btl_makePartyGreyCluts(void);
void btl_saveEnemyCluts(u32 *);
void btl_savePartyCluts(u32 *);
void btl_setUnitBrightness(struct BtlUnit *, u32);
void func_800B95BC(struct BtlCmd *);
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
void func_800EA290(void);
void btlui_drawOTagEnv(u32 *, DRAWENV *);
void btl_cleanup(void);
s32 btl_getModelTexInfo(u32, s32 **, s32 **, s32 **);
u32 evt_getObjectChrIndex(void *);
/* object slot returned by game_getModel in battle */
typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* model id */
    BtlUnit9C *unk8;
    s32 unkC;
} BtlObj;
/* placement data passed to btl_calcShadowMatrix */
typedef struct {
    u32 pad0_0 : 8;
    u32 unk0_8 : 12;  /* x scale */
    u32 unk0_20 : 12; /* z scale */
    s32 pad4_0 : 12;
    s32 unk4_12 : 10; /* x offset */
    s32 unk4_22 : 10; /* z offset */
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
s32 func_800A8F18(void);
s32 func_800A8F8C(void);
s32 func_800A902C(void);
s32 func_800A9090(void);
s32 btl_loadCommonData(void);
s32 func_800AF1C8(void);
extern void (*D_800F8DF0[])(Unk800FD56C *);
extern void (*D_800F8E00)(Unk800FD56C *);
extern void (*D_800F9038[])(BtlUnit *, s32);
void btlui_drawPanelHeader(void);
u8 btl_isCmdQueued(BtlUnit *, u8);
void btlui_targetFirst(u8, u8);
typedef struct {
    u32 unk0; /* status mask */
    s16 unk4; /* icon */
    s16 unk6; /* width */
} Unk800F8F98;
extern Unk800F8F98 D_800F8F98[];
extern Unk800F8F98 D_800F8F18[];
void btl_removeQueuedCmd(BtlUnit *, u8);
void btlui_frameUpdate(void);
s32 btlui_isEnabled(void);
void func_800EA5C4(void);
void func_800EA548(void);
void btlui_readPad(void);
void func_800EB578(s32);
void btlui_beginFrame(void);
void func_800EA4D0(void);
void btlui_input(void);
void btlui_drawWindows(void);
void btlui_update(void);
void btlui_submitOt(void);
s32 btl_partyHasSupport(s32, u32);
u32 btl_addRgbClamped(u32, s32, s32, s32);
void menuList_pageUp(MenuList *, s32);
void menuList_pageDown(MenuList *, s32);
extern s16 D_800F9048[];
void btlfx_heapFree(void *);
s32 btlfx_poolUpdate(Unk800DA1B8 *);
void btlui_targetDefault(u8, u8);
BtlUnit *btlui_findKoUnit(u8);

/* first unit flagged as the current target (bits 28 and 29 of BtlUnit.flags) */
static inline BtlUnit *find_target(void) {
    BtlUnit *u;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            return u;
        }
    }
    return NULL;
}

extern u8 D_800F8E68[][2];
void btlui_closeTargetWindow(s32);
s32 clut_getY(s16);
s32 db_getModelTexCount(s32);
void btl_startHitMotion(BtlUnit *);
s32 btlfx_heapAlloc(Heap *, s32);
void btl_playCmdEffect(BtlCmd *, s32, s16 *);

s32 func_800D9350(Unk800D7C9C *, s32, s32 *);
s32 math_sin(s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
} Unk800EF80C;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} Unk800EDE6C;

typedef struct {
    u32 unk0; /* tag */
    u32 unk4;
    u32 unk8;
    u32 unkC; /* ordering table */
} Unk800D0940;

extern s32 D_800F7278;
void btlui_startTargetSelect(Unk800EF80C *);

void btl_endPartyHitMotion(BtlUnit *);
void btl_endEnemyHitMotion(BtlUnit *);
s32 btlfx_stepWaitKindTime(u8 *, s32, s32 *);

extern u16 D_800FEC42;
extern Unk800F412C D_800F8FD8[5];
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
    s16 unk0;
    u8 unk2;
    u8 pad3;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk800EE37C;
void btlui_openList(Unk800EE37C *);
extern RECT D_800F8E30;
void win_setList(Window *, MenuList *);
void btlui_drawListContent(Window *);
void btlui_drawListOverlay(Window *);
void btlui_onListOpened(void);
s32 btlfx_isSameSide(s32, s32);
s32 func_800DA064(u8 *, s32, s32);
extern u8 D_80104E70;
extern u8 g_btlCmdAbilities[];
extern U16Pair D_800FB1F0[];
extern char D_800F96E4[];
s32 btlui_getAbilityState(s32);
s32 *btl_getBannerOt(s32);
u8 *btl_buildBanner(u8 *);
u8 *btl_drawBanner(u8 *);
typedef struct Unk800D9134 {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s16 unk8[4];
    s16 unk10[4];
} Unk800D9134;
s32 btlfx_stepMoveToUnit(Unk800D9134 *, s32, s32 *);
extern s32 D_80104DE8;
extern s32 D_800F78B4[];
extern u16 D_800F8CB4[];
extern s32 (*D_800F8E38[])(void);
s32 btlfx_reloadCommonData(s32);
u16 *func_800E4530(s32);
s32 btlfx_resolveFirstUse(s32);
extern u8 D_800F727C[][2];  /* summon id upgrades */
extern u16 g_btlfxFirstUsePairs[16][2];
void func_800C30B8(void);
u32 btlfx_splitMeshVertices(BtlUnit9C *, u16, s32);
extern Unk800F3F10 D_800F9058;
extern Unk800F3F10 D_800F9064;
extern U16Pair D_800F9070[];
s32 btl_pickRandomUnit(s32);
s32 btlfx_stepSwingSound(u8 *, s32, s32);
void btlui_closeList(void);
extern u16 D_800F6A74[];
extern struct Unk80100DC054 *D_80100E14; /* g_btlfxLoader.unk54 */
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
    for (p = g_gameState->gameData->unk8E4.battle->unitList; p != NULL; p = p->next) {
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
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
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
    if ((g_btlRewards.unkA >> ((u8)u->flags >> 5)) & 1) {
        return 1;
    }
    return 0;
}

/* -O1 unit (ovl_0b_o1.c) */
s32 btl_getSceneData(s32);
void btlcam_evalPath(s32, Unk800C412C *, s16 *);
s32 btlfx_lerp(s32, s32, s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2_0 : 7;
    u16 unk2_7 : 7;
    u16 unk2_14 : 2;
    u16 unk4_0 : 7;
    u16 unk4_7 : 7;
} Unk800C3F78;
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk800F5758;
extern Unk800F5758 D_800F5758[];
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
s32 func_800EA094(void);

typedef struct {
    u8 pad0[0x17];
    u8 unk17;
    u8 pad18[0x44 - 0x18];
    RECT *unk44;
    s16 unk48;
} Unk800DA790;

typedef struct {
    s32 unk0; /* frames */
    s32 unk4; /* shade */
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
    s32 unk0; /* offset of the sprite data */
    s16 unk4;
    s16 unk6;
    u16 unk8; /* start time */
    u16 unkA; /* fade-in time */
} Unk800D3B6CEntry;

typedef struct {
    u16 unk0; /* duration */
    u16 unk2; /* count */
    u16 unk4; /* mode */
    u8 unk6;
    u8 unk7;
    s16 unk8[2];
    s16 unkC[2];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    Unk800D3B6CEntry unk18[1];
} Unk800D3B6C;

/* sprite-strip context passed to btlfx_emitTrailRibbonScreen / btlfx_emitTrailRibbon3D */
typedef struct {
    s32 unk0; /* ot base z */
    s32 unk4;
    s32 unk8; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } unkC;   /* rgb */
    u32 unk10; /* rgb */
    s16 unk14;
    s16 unk16;
    s32 unk18; /* tpage << 16 */
    s32 unk1C; /* clut << 16 */
    s32 unk20; /* 0x1000 / count */
    s16 unk24;
    s16 unk26;
    u8 pad28[0x70 - 0x28];
} Unk800D5C30Ctx;
void btlfx_emitTrailRibbonScreen(s32, Unk800D5C30Ctx *, u8 *, s32 *);

/* same as Unk800D5C30Ctx plus two words (btlfx_emitTrailRibbon3D) */
typedef struct {
    s32 unk0; /* ot base z */
    s32 unk4;
    s32 unk8; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } unkC;   /* rgb */
    u32 unk10; /* rgb */
    s16 unk14;
    s16 unk16;
    s32 unk18; /* tpage << 16 */
    s32 unk1C; /* clut << 16 */
    s32 unk20; /* 0x1000 / count */
    s16 unk24;
    s16 unk26;
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x60 - 0x30];
} Unk800D50A0Ctx;
void btlfx_emitTrailRibbon3D(s32, Unk800D50A0Ctx *, u8 *, s32 *);

/* same fields as Unk800D50A0Ctx, without the padding (btlfx_emitTrailRibbonFlat / btlfx_emitTrailLines) */
typedef struct {
    s32 unk0; /* ot base z */
    s32 unk4;
    s32 unk8; /* 0x1000 / count */
    union {
        u32 w;
        u8 b[4]; /* b[3]: primitive code */
    } unkC;   /* rgb */
    u32 unk10; /* rgb */
    s16 unk14;
    s16 unk16;
    s32 unk18; /* tpage << 16 */
    s32 unk1C; /* clut << 16 */
    s32 unk20; /* 0x1000 / count */
    s16 unk24;
    s16 unk26;
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
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6[1];
} Unk800D1BC4;

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 *unk18;
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

extern DR_OFFSET D_801050E0[2];
extern DR_AREA D_80105100[2];
extern DR_MODE D_80105120[2];
extern DR_STP D_80105140[2];
extern DR_ENV g_btluiSavedEnv[2];
extern RECT D_800FDE34;

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
#define SHADOW_QUAD(q) e->unk8[g_gameState->gameData->unk8][q]

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
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
} Unk800FDED0;
extern Unk800FDED0 D_800FDED0[2];
extern Unk800FDED0 D_800FDEB8[2];
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
extern u32 D_800FDEB0[1];
extern DR_STP D_800FDF18;
extern TILE D_800FDE98;
extern DR_TPAGE D_800FDEA8;
extern DR_STP D_800FDF24;
extern u8 D_800FDE60[];
void game_resumeClock(s32);
void akao_sendCmd98(s32);
void func_800EA34C(void);

s32 btl_loadWeaponModelsStep(s32, Unk800AA958 *);
s32 btl_loadPartyModelsStep(s32, Unk800AA958 *);
s32 btl_getWeaponModelHeader(s32);
BtlUnit *btl_getUnitBySlot(s32);
void btl_loadPartyMotions(BtlUnit *, s32, s32);
u8 *db_getModelTexPos(s32);

typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    s16 unk4;
    u8 pad6[8 - 6];
    union {
        s32 w[4];
        s16 h[8];
    } unk8;
    u8 pad18[0x24 - 0x18];
    s32 unk24;
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
    FileEntryId unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} FileEntryX;

s32 btl_loadSoundsStep(s16, s32, Unk800AA24C *);
/* battle scene model load state (btl_loadEnemyModelsStep) */
typedef struct {
    s16 unk0; /* state */
    u16 unk2; /* next state */
    u8 pad4[8 - 4];
    s16 unk8; /* file id being loaded */
    s16 unkA; /* files loaded */
    s32 unkC; /* load address */
    union {
        s32 w[2];
        s16 h[4];
    } unk10; /* file ids still to load, -1 = done */
} Unk800AA504;

typedef struct {
    u16 unk0; /* id */
    u8 pad2[4 - 2];
    union {
        s32 w; /* low 24 bits: offset of the texture lists from this entry */
        u8 b[4]; /* b[3]: texture count */
    } unk4;
    u8 pad8[0xC - 8];
} Unk800AA504Ent; /* 0xC */

typedef struct {
    u8 pad0;
    u8 unk1; /* entry count */
    u8 pad2[4 - 2];
    Unk800AA504Ent unk4[1];
} Unk800AA504Tbl;

typedef struct {
    s16 unk0;
    s16 unk2;
} Unk800AA504Pos;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Unk800AA504Pos8;

s32 btl_loadEnemyModelsStep(s32, s32, Unk800AA504 *);

/* inlined into btl_loadSoundsStep: append the ids of file-table entries of the
 * given type/kind to dst, skipping duplicates */
static inline void btl_loadSoundsStep_collect(s16 *dst, u32 type, u32 kind) {
    FileTable *t = g_gameState->gameData->depTable;
    FileEntryX *first = (FileEntryX *)t->unk4;
    FileEntryX *e = &first[t->unk2] - 1;
    s16 *q = dst;
    s16 *s;
    u32 w;

    do {
        w = e->unk0.w;
        if (e->unk0.b.type == type && (w >> 24) == kind) {
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
void func_8001DA78(void *, s32);
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */

/* btlfx_drawRings: multi-frame sprite sheets */
#define gte_zero_vz012() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5" : : )

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} Unk800D23D8Hdr;

typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800D23D8Scale;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 *unk8;
    u16 *unkC;
    u16 *unk10;
    u16 *unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x40 - 0x2C];
} Unk800D4768Ctx;

typedef struct {
    u32 unk0;
    s32 unk4;
    u32 *unk8;
    u32 *unkC;
    u16 *unk10;
    u32 *unk14;
    s32 unk18;
    s32 unk1C;
    u32 *unk20;
    s32 unk24;
    s32 unk28;
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
    u8 unk0; /* kind */
    u8 unk1; /* phase */
    u8 unk2; /* unit id */
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u16 unk6; /* frame */
    s32 unk8;
    s16 unkC[4];
    s16 unk14[4];
    s16 unk1C[4];
    Pool6CEntry *unk24;
} Unk800E4C88; /* 0x28 */

/* func_800E57B0 effect state (sprite sheet on a unit) */
typedef struct {
    u8 unk0; /* kind */
    u8 unk1; /* unit id, 0xFF = none */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE; /* frame count */
    void *unk10;
    s32 unk14; /* frame */
    s16 unk18;
    s16 unk1A;
    SVECTOR unk1C;
    s16 unk24[3];
} Unk800E57B0; /* 0x2C */

extern u16 D_800F8D04[];
s32 btlfx_compareDepth(s32, s32);
s32 func_800E7CC4(s32, s32);
void btlfx_getWeaponPos(s32, s32, s16 *);

void btlfx_getUnitPos(s32, s16 *);
void btlfx_getUnitBodyPos(s32, s32, s16 *);
void btlfx_startFadeBackground(s32, s32);
Pool6CEntry *func_800E4A3C(s32, s32, s16 *, s16 *);
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
Pool6CEntry *func_800E4A3C_def(s32, s32, Unk800E4A3CArgs) __asm__("func_800E4A3C");
void func_800E8C84(s32);
void btlfx_showCasterCmdName(void);


/* btlfx_runScript: battle event script */
s32 func_800AB3E4(s32, s32);
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
s32 func_800E8CE4(void);
void btlfx_updateCameraEnable(void);
void btlfx_playCmdCamera(s32);
void btlfx_stopAllSounds(void);
void btlfx_retryOp(void);
extern s32 D_800FE530[];
extern u8 D_800FF068[];
extern const u8 D_800A857C[]; /* "TRANCE" label */
/* BtlUnit9C.unk50 seen as an array member: indexing it as an ARRAY_REF keeps `c ? 1 : 0` from folding into the address (btlfx_setUnitTexAnim) */
typedef struct {
    BtlUnit9C50 e[3];
} BtlUnit9C50Arr;
/* btl_placeParty: per-character battle setup row (5 bytes, indexed by Character.unk34.b.unk4) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} Unk800F57F0;
extern Unk800F57F0 D_800F57F0[];
void btl_setShadowSize(BtlUnit *, s32, u32);
/* btlui_drawTranceLabel */
extern u8 D_800F8E18[]; /* text measured for the highlight bar width */
extern CVECTOR D_800F8E20[4]; /* highlight bar gradient colours */
void btlui_drawPulseGradient(s32, s32, s32, s32, s32, CVECTOR *);
/* btlfx_updateLoader */
extern u16 D_800FEC68[];
extern s32 D_80100D90[][2];
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
void func_800AE9C8(s32 arg0, s32 arg1);
void func_800B3DBC(BtlUnit *arg0);
void func_800BA938(Unk800BA938 *arg0);
void func_800BC1E0(Unk800BC1E0 *arg0, s16 *arg1);
void func_800BDACC(u8 *arg0, u8 *arg1);
s32 func_800C2DF4(void);
void func_800C4B9C(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);
void func_800D044C(SVECTOR *arg0, s32 arg1, s32 arg2, XY32 *arg3, s32 arg4, s32 arg5);
void func_800D0580(s16 *arg0, XY32 *arg1, s32 arg2);
void func_800D0674(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 *arg5);
void func_800EA378(void);

#endif

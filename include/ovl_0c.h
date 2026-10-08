#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    u8 pad0[0x1B];
    u8 unk1B;
    u8 pad1C[0x2C - 0x1C];
    u8 *unk2C;
    u8 pad30[0x4C - 0x30];
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
} WmNode;

typedef struct Unk800D3EAC {
    s32 unk0;
    struct Unk800D3EAC *unk4; /* next */
    s32 unk8;
    u16 unkC; /* low 6 bits: id */
    u8 unkE;
} Unk800D3EAC;

/* ---- overlay functions ---- */
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
void func_800A8FF0(void);
s32 func_800A919C(void);
s32 func_800A91A4(void);
s32 func_800A91AC(void);
s32 func_800A91B4(void);
s32 func_800A9220(void);
void func_800A924C(void);
void func_800A95A0(void);
void func_800AA15C(s32, void *);
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
void func_800AB0C0(s32, s32, s32, s32, s32);
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
s32 func_800C497C(s32);
void func_800CBF18(void);
void func_800CC050(void);
void func_800CC13C(void);
void func_800CC82C(void);
void func_800CC890(void);
void func_800CC8C8(void);
void func_800CC940(void);

/* ---- overlay data ---- */
extern char D_800A71F4[]; /* "w_frameMapDestructor:\n" */
extern s32 D_800CCE50; /* debug flags */
extern Unk800CE0E8 *D_800CE0E8;
extern u8 D_800CE25C;
extern u8 D_800CE26C;
extern s32 D_800CE3E4;
extern s32 D_800CF480;
extern Unk800D3EAC *D_800D3EAC;
extern WmNode D_800D4848;
extern u8 D_800CCA1C;
extern RECT D_800CE0B8[];
extern u8 D_800CCFB4[];
extern u8 D_800CE0EC;
extern u8 D_800CE0ED;
extern u8 D_800CE0EE;
extern s32 D_800CE108[];
extern s32 D_800CE198[];
extern s32 D_800CE27C;
extern s32 D_800CF408;
extern u8 D_800D30C4;
extern u8 D_800D30C5;
extern u8 D_800D30C6;
extern u8 D_800D3338;
extern s32 D_800D3F8C;
extern s32 D_800D3F90;
extern s32 D_800D3F94;
extern s32 D_800D3F98;
extern u8 D_800D3F9C;
extern u8 D_800D4818;
extern s32 D_800D481C;
extern s32 D_800D4820;
extern s32 D_800D4824;
extern s32 D_800D4828;
extern u8 D_800D5438;
extern u8 D_800CF46C;

#endif

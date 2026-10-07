/* Compiler-identification probe #2: functions in 0x80026000+ that use the
 * game state pointer (Square code, outside any PsyQ signature match).
 * tools/docker.sh python3 tools/ccprobe.py tools/probes/compiler_id_game.c */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct {
    u8 unk0;
    u8 unk1;
} Pair;

typedef struct {
    u8 pad0[0x54C];
    s32 unk54C[10];
    u8 pad574[0x574 - 0x54C - 40];
    Pair unk574[0x100];
    u8 unk774[0x40];
} FieldData;

typedef struct {
    u8 pad0[0x1C];
    FieldData *unk1C;
    u8 pad20[4];
    u32 unk24;
} GameState;

extern GameState *D_8006794C;

void func_8002423C(s32, s32, s32, s32);

void func_80026F8C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8002423C(arg0, arg1, arg2, D_8006794C->unk1C->unk54C[arg3]);
}

u16 func_80027480(s32 arg0) {
    return arg0 - ((D_8006794C->unk24 >> 18) & 1) * 0xC0;
}

void func_80027D18(void) {
    s32 i;

    for (i = 0; i < 0x100; i++) {
        D_8006794C->unk1C->unk574[i].unk1 = 0;
    }
}

s32 func_800281A4(s32 arg0) {
    return (D_8006794C->unk1C->unk774[arg0 >> 2] >> ((arg0 & 3) * 2)) & 1;
}

extern s32 D_80077140;
extern s32 D_80077144;
void func_8001D898(s32, s32, s32);

s32 func_8002B43C(s32 arg0) {
    D_80077140 = arg0;
    D_80077144 = arg0 + 0x4000;
    func_8001D898(D_80077144, 0, 0x3F0);
    return D_80077144;
}

extern void (*D_80068F80[])(s32, s32);

void func_8002705C(s32 arg0, s32 arg1, s32 arg2) {
    void (*fn)(s32, s32) = D_80068F80[arg2];

    if (fn != 0) {
        fn(arg0, arg1);
    }
}

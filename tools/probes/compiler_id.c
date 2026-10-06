typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;

/* ---- func_80021C60 ---- */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
    u8 pad4[0x11C - 4];
    s32 unk11C;
} Probe1;
typedef struct {
    u8 pad[0x8C8];
    Probe1 *unk8C8;
} Probe1Owner;
typedef struct {
    u8 pad[0x1C];
    Probe1Owner *unk1C;
    u8 pad20[0xC];
    s32 unk2C;
} GameState;
extern GameState *D_8006794C;
void func_80021938(void *);

void func_80021C60(void) {
    s32 i;
    s32 off;
    Probe1 *p;

    p = D_8006794C->unk1C->unk8C8;
    p->unk1 = 4;
    p->unk0 = 0;
    p->unk2 = 0;
    p->unk3 = -1;
    for (i = 5, off = 0xCC; i >= 0; i--, off -= 0x28) {
        func_80021938((u8 *)p + off);
    }
    func_80021938((u8 *)p + 0xF4);
    p->unk11C = 0;
}

/* ---- func_800233CC ---- */
typedef struct {
    u8 unk0;
    u8 pad1[0x17];
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    u8 unk944;
    u8 pad945[0x958 - 0x945];
    void (*unk958)();
} Probe2;
extern Probe2 D_800761D0;
void func_800213A0(void (*)());
void func_80023580(s32, s32);
void func_800235A0();

void func_800233CC(void) {
    s32 arg;

    if (D_800761D0.unk944 & 2) {
        D_800761D0.unk18 = 0;
        D_800761D0.unk0 = 3;
        if (D_800761D0.unk944 & 8) {
            func_800213A0(func_800235A0);
            arg = 0x1B;
        } else {
            func_800213A0(D_800761D0.unk958);
            arg = 6;
        }
    } else {
        D_800761D0.unk0 = 4;
        arg = 0x15;
        if (D_800761D0.unk944 & 8) {
            arg = 0x16;
        }
    }
    func_80023580(arg, 0);
}

/* ---- func_80021140 ---- */
extern void *D_80068250;
extern void *D_80068254;
extern void *D_80068618;
extern s32 D_8006861C;
extern char D_800108AC[];
s32 func_800211D0(void);
void func_80015C38(char *);
void func_8002120C(void);
void func_80021234(void);
void func_8002125C(void);

s32 func_80021140(void) {
    s32 i = 4;

    do {
        if (func_800211D0() == 1) {
            D_80068250 = func_8002120C;
            D_80068254 = func_80021234;
            D_80068618 = func_8002125C;
            D_8006861C = 0;
            return 1;
        }
    } while (--i != -1);
    func_80015C38(D_800108AC);
    return 0;
}

/* ---- func_8002417C ---- */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad[10];
    s32 unkC;
} Timer;
typedef struct {
    s32 unk0;
    u8 pad[0x404];
    Timer unk408;
} Probe4;
extern Probe4 D_80076B60;
s32 func_80015C58(s32);

void func_8002417C(void) {
    s32 now;
    s32 dt;
    Timer *t;

    now = func_80015C58(-1);
    dt = now - D_80076B60.unk0;
    t = &D_80076B60.unk408;
    if (t->unk0 != 0 && t->unk1 != 0 && t->unkC != 0) {
        t->unkC -= dt;
        if (t->unkC <= 0) {
            t->unkC = 0;
        }
    }
    D_8006794C->unk2C += dt;
    D_80076B60.unk0 = now;
}

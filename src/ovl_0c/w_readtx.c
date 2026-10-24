#include "ovl_0c.h"

s32 w_readtxRequest(s16 arg0) {
    Unk800D3EAC *p;
    u32 i;

    p = D_800D3EA8.unk4;
    i = 0;
    while (1) {
        if (p->unkC == arg0) {
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = D_800D3EA8.unk4;
            p->unk0 = &D_800D3EA8;
            D_800D3EA8.unk4->unk0 = p;
            D_800D3EA8.unk4 = p;
            break;
        }
        if (p->unkC == -1) {
            D_800D3E90++;
            if (D_800D3E90 >= 4) {
                sprintf(g_worldMapErrorMsg, "w_readtxRequest/reqover[%d/%d]\n", D_800D3E90, 3);
                printf("==== ERROR ===== / %s", g_worldMapErrorMsg);
                g_worldMapError = 1;
            }
            p->unkC = arg0;
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = D_800D3EA8.unk4;
            p->unk0 = &D_800D3EA8;
            D_800D3EB8++;
            D_800D3EA8.unk4->unk0 = p;
            D_800D3EA8.unk4 = p;
            break;
        }
        p = p->unk4;
        i++;
        if (i >= 6) {
            break;
        }
    }
    if (i == 6) {
        printf("-- w_readtxRequest Error --\n");
        return 0;
    }
    return 1;
}

void func_800C1F3C(void) {
    Unk800D3EAC *p;
    Unk800D3EAC *q;
    u32 i;

    D_800D3E90 = 0;
    if (D_800D3EB8 != 0) {
        p = D_800D3EA8.unk4;
        i = 0;
        D_800D3EB8 = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 3);
        for (;;) {
            while ((p != &D_800D3E98) & (p->unk8 == NULL)) {
                if ((p->unk8 == NULL) & (p->unkC != -1)) {
                    p->unkC = -1;
                }
                p = p->unk4;
            }
            if (p == &D_800D3E98) {
                break;
            }
            while (q->unk8 != NULL) {
                q = q->unk4;
            }
            q->unk8 = p->unk8;
            p->unk8 = NULL;
            p->unkE = 0;
            p->unkC = -1;
            p = p->unk4;
        }
    }
}

void func_800C2040(void) {
}

void func_800C2048(void) {
}

/* find the loaded frame pack for (id, variant) */
s32 func_800C2050(s32 arg0, s32 arg1) {
    Unk800D3EAC *p = D_800D3EA8.unk4;
    s32 i;

    for (i = 0; i < 3; i++, p = p->unk4) {
        if ((p->unkC & 0x3F) == (arg0 & 0x3F) && p->unkE == (arg1 & 0xFF)) {
            return (s32)p->unk8;
        }
    }
    return 0;
}

void func_800C20B0(s32 *arg0) {
    Unk800D3EAC *p;
    s32 i;

    if (g_worldMapDebugFlags & 0x200) {
        printf(D_800A8098, arg0);
    }
    w_frameExtruct(arg0);
    p = D_800D3EA8.unk4;
    for (i = 0; i < 3;) {
        if (p->unk8 == arg0) {
            p->unkE = 1;
            return;
        }
        i++;
        p = p->unk4;
    }
}

void func_800C2134(void) {
    u32 i;
    WmLNode *cur;

    D_800D3F48[0].unk0 = &D_800D3F38;
    D_800D3F48[0].unkA = 1;
    D_800D3F48[0].unk4 = &D_800D3F48[1];
    D_800D3F48[0].unkB = 0;
    D_800D3F48[0].unk8 = -1;
    for (i = 1; i < 3; i++) {
        D_800D3F48[i].unk0 = &D_800D3F48[i - 1];
        D_800D3F48[i].unkA = 0;
        D_800D3F48[i].unkB = 0;
        D_800D3F48[i].unk4 = &D_800D3F48[i + 1];
        D_800D3F48[i].unk8 = -1;
    }
    for (i = 1; i < 2; i++) {
        D_800D3F48[i].unkA = i + 1;
    }
    D_800D3F48[3].unk0 = &D_800D3F48[2];
    cur = &D_800D3F48[3];
    cur->unk4 = &D_800D3F28;
    cur->unk8 = -1;
    cur->unkA = 0;
    cur->unkB = 0;
    D_800D3F38.unk0 = 0;
    D_800D3F38.unk4 = D_800D3F48;
    D_800D3F20 = 0;
    D_800D3F28.unk4 = &D_800D3F48[2];
    D_800D3F28.unk0 = 0;
    D_800D3F44 = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_readtx", D_800A8098);

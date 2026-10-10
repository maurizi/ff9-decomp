#include "ovl_64.h"

u8 *func_801990A0(Ovl64Out *out) {
    RECT r;
    u8 *data;
    Ovl64RingHdr *hdr;
    Ovl64FrameInfo *info;
    Ovl64Pair *pair;
    Ovl64Flag *flag;
    s32 tries;
    s32 ret;
    s8 hi;

    tries = 1000;
    info = NULL;
    pair = NULL;
    flag = NULL;
    ret = func_8019C2F4(&data, &hdr, &info, &pair, &flag);
    while (ret != 0) {
        if (ret == 5) {
            D_8019DC94.unk0 = 1;
            return NULL;
        }
        if (--tries == 0) {
            return NULL;
        }
        ret = func_8019C2F4(&data, &hdr, &info, &pair, &flag);
    }
    if (info != NULL) {
        gte_SetGeomOffset((s16)info->unk20 + 0xA0, (s16)info->unk22 + 0x70);
        if (D_8019DC94.unk6 != 0) {
            D_8019DC88.unk0 = (s16)info->unk20 + 0xA0;
            D_8019DC88.unk4 = (s16)info->unk22 + 0x70;
            D_8019F258.unk0 = info->unk2;
            D_8019F258.unk2 = info->unk4;
            D_8019F258.unk4 = info->unk6;
            D_8019F258.unk6 = info->unk8;
            D_8019F258.unk8 = info->unkA;
            D_8019F258.unkA = info->unkC;
            D_8019F258.unkC = info->unkE;
            D_8019F258.unkE = info->unk10;
            D_8019F258.unk10 = info->unk12;
            D_8019F258.unk14 = info->unk14;
            D_8019F258.unk18 = info->unk18;
            D_8019F258.unk1C = info->unk1C;
            D_8019F278 = info->unk0;
            D_8019DC94.unk6 = 0;
        } else {
            D_8006794C->unk1C->unk18.m[0][0] = info->unk2;
            D_8006794C->unk1C->unk18.m[0][1] = info->unk4;
            D_8006794C->unk1C->unk18.m[0][2] = info->unk6;
            D_8006794C->unk1C->unk18.m[1][0] = info->unk8;
            D_8006794C->unk1C->unk18.m[1][1] = info->unkA;
            D_8006794C->unk1C->unk18.m[1][2] = info->unkC;
            D_8006794C->unk1C->unk18.m[2][0] = info->unkE;
            D_8006794C->unk1C->unk18.m[2][1] = info->unk10;
            D_8006794C->unk1C->unk18.m[2][2] = info->unk12;
            D_8006794C->unk1C->unk18.t[0] = info->unk14;
            D_8006794C->unk1C->unk18.t[1] = info->unk18;
            D_8006794C->unk1C->unk18.t[2] = info->unk1C;
            D_8006794C->unk1C->unkA = info->unk0;
        }
    }
    if (flag != NULL && (!((Ovl64Cfg *)&D_8006794C->unk24)->b7 || D_8006794C->unk8 == 5)) {
        D_800739B0[0].unk0 = flag->unk0;
        D_800739B0[0].unk1 = flag->unk1;
    }
    if (pair != NULL && pair->unk0 != 0) {
        func_80063864((void *)pair->unk4, &D_8019E028[D_8006794C->unk1C->unk8 ^ 1], pair->unk0);
        D_8019E018[D_8006794C->unk1C->unk8 ^ 1] = pair->unk0;
    } else {
        D_8019E018[D_8006794C->unk1C->unk8 ^ 1] = 0;
    }
    if (D_80071E34 >= D_8019DCF8->unk8) {
        D_8019DC94.unk0 = 1;
    }
    if (D_8019DC94.unk2C != hdr->unk10 || D_8019DC94.unk30 != hdr->unk12) {
        hi = D_80071E31;
        if (hi) {
            r.x = 0;
            r.y = 0;
            r.w = 0x3C0;
            r.h = 0xE0;
        }
        D_8019DC94.unk2C = hdr->unk10;
        D_8019DC94.unk30 = hdr->unk12;
        if (hi) {
            out->unk1C = D_8019DC94.unk2C * 3 / 2;
            out->unk24 = D_8019DC94.unk2C * 3 / 2;
        } else {
            out->unk1C = D_8019DC94.unk2C;
            out->unk24 = D_8019DC94.unk2C;
        }
        out->unk1E = D_8019DC94.unk30;
        out->unk26 = D_8019DC94.unk30;
        out->unk2E = D_8019DC94.unk30;
    }
    return data;
}

void func_80199554(void) {
    u8 res[8];
    s32 cur;

    func_8001D898(res, 0, 8);
    if (D_8019DEA8 != 0) {
        func_8019C434(2, res);
        D_8019DEA8 = 0;
    }
    if (D_80071E31 == 0 && D_8019F4C8.unk4 != 0) {
        func_8019B3D4(&D_8019F4C8, D_8019F4D8.unkC[D_8019F4D8.unk14], D_8019DC84);
    }
    cur = D_8019F4D8.unk14;
    D_8019F488[D_8019DC90] = D_8019F4D8.unk28;
    D_8019F4D8.unk14 ^= 1;
    D_8019F4D8.unk28.x += D_8019F4D8.unk28.w;
    if (D_8019F4D8.unk28.x < D_8019F4D8.unk18[D_8006794C->unk1C->unk8].x + D_8019F4D8.unk18[D_8006794C->unk1C->unk8].w) {
        if (DecDCTinSync(1) == 0) {
            D_8019F4D8.unk30 = 1;
            D_8019F4D8.unk28.x = D_8019F4D8.unk18[D_8006794C->unk1C->unk8 ^ 1].x;
            D_8019F4D8.unk28.y = D_8019F4D8.unk18[D_8006794C->unk1C->unk8 ^ 1].y;
        }
        DecDCTout(D_8019F4D8.unkC[D_8019F4D8.unk14], (D_8019F4D8.unk28.w * D_8019F4D8.unk28.h) >> 1);
    } else {
        if (DecDCTinSync(1) == 1) {
            DecDCTReset(1);
        }
        D_8019F4D8.unk30 = 1;
        if ((s8)D_8019DC94.unk3 == 1) {
            if (D_80071E31 == 1 && D_8006794C->unk1C->unk8 == 0) {
                D_80071E31 = 0;
                D_8019DC94.unk1 = 0;
                D_8019F4D8.unk18[0].x = 0x140;
                D_8019F4D8.unk18[0].w = 0x140;
                D_8019F4D8.unk18[1].w = 0x140;
                D_8019F4D8.unk28.w = 0x10;
                if (D_8006794C->unk8 == 1 || D_8006794C->unk8 == 5) {
                    D_800C9EF0.unk18 = 9;
                    D_800C9EF0.unk8 |= 0x14;
                }
                D_8019DC97 = 2;
            }
            if (D_80071E31 == 0 && D_8006794C->unk1C->unk8 == 1) {
                D_8019DC94.unk1 = 1;
                D_8019F4D8.unk18[0].x = 0x1E0;
                D_8019F4D8.unk18[0].w = 0x1E0;
                D_8019F4D8.unk18[1].w = 0x1E0;
                D_8019F4D8.unk28.w = 0x18;
                D_8006794C->unk1C->unk0 |= 0x14;
                D_80071E31 = 1;
                if (D_8006794C->unk8 == 1 || D_8006794C->unk8 == 5) {
                    D_8006794C->unk1C->unk8E4.ovl65->unk0 |= 9;
                }
                D_8019DC97 = 2;
            }
        }
        if ((s8)D_8019DC94.unk4 != 0) {
            D_80071E38 = 1;
            D_8019DC94.unk4 = 0;
            func_8019CC3C(1);
            D_800739B0[0].unk0 = 0;
            D_800739B0[0].unk1 = 0;
        }
        D_8019F4D8.unk28.x = D_8019F4D8.unk18[D_8006794C->unk1C->unk8 ^ 1].x;
        D_8019F4D8.unk28.y = D_8019F4D8.unk18[D_8006794C->unk1C->unk8 ^ 1].y;
    }
    LoadImage(&D_8019F488[D_8019DC90], D_8019F4D8.unkC[cur]);
    D_8019DC90 = (D_8019DC90 + 1) % 4;
}

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_80198700);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_801999B4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_80199B28);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_80199F30);

s32 func_8019A5B0(s32 on) {
    s8 k;

    if (D_80071E31 >= 0) {
        if (on != 0) {
            if (D_80071E31 != 0) {
                return 0;
            }
        } else if (D_80071E31 != 1) {
            return 0;
        }
    }
    if (func_80012AD4() & 2) {
        D_8019DC97 = 1;
    } else if (on != 0) {
        k = 1;
        D_80071E31 = k;
        D_8019DC94.unk1 = k;
        D_8019F4D8.unk18[0].x = 0x1E0;
        D_8019F4D8.unk28.w = 0x18;
        D_8019DC94.unk3 = 2;
    } else {
        D_80071E31 = 0;
        D_8019DC94.unk1 = 0;
        D_8019F4D8.unk18[0].x = 0x140;
        D_8019F4D8.unk28.w = 0x10;
    }
    return 1;
}

void func_8019A688(s32 h, s32 hires, s32 i) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&D_8006794C->unkC[i], w, 0, w, h);
    if (hires != 0) {
        D_8006794C->unkC[i].isrgb24 = 1;
        D_8006794C->unkC[i].disp.w = D_8006794C->unkC->disp.w * 2 / 3;
    }
    D_8006794C->unkC[i].isinter = 0;
    D_8006794C->unkC[i].screen.x = 0;
    D_8006794C->unkC[i].screen.y = (0xF0 - h) >> 1;
    D_8006794C->unkC[i].screen.h = h;
}

void func_8019A7A8(s32 h, s32 hires) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&D_8006794C->unkC[0], w, 0, w, h);
    SetDefDispEnv(&D_8006794C->unkC[1], 0, 0, w, h);
    if (hires != 0) {
        D_8006794C->unkC[0].isrgb24 = 1;
        D_8006794C->unkC[0].disp.w = D_8006794C->unkC[0].disp.w * 2 / 3;
        D_8006794C->unkC[1].isrgb24 = 1;
        D_8006794C->unkC[1].disp.w = D_8006794C->unkC[1].disp.w * 2 / 3;
    }
    D_8006794C->unkC[0].isinter = 0;
    D_8006794C->unkC[0].screen.x = 0;
    D_8006794C->unkC[0].screen.y = (0xF0 - h) >> 1;
    D_8006794C->unkC[0].screen.h = h;
    D_8006794C->unkC[1].isinter = 0;
    D_8006794C->unkC[1].screen.x = 0;
    D_8006794C->unkC[1].screen.y = (0xF0 - h) >> 1;
    D_8006794C->unkC[1].screen.h = h;
    SetDefDrawEnv(&D_8006794C->unk10[0], 0, 0, w, h);
    SetDefDrawEnv(&D_8006794C->unk10[1], w, 0, w, h);
    D_8006794C->unk10[0].dfe = 0;
    D_8006794C->unk10[0].dtd = 1;
    D_8006794C->unk10[0].isbg = 1;
    D_8006794C->unk10[1].dfe = 0;
    D_8006794C->unk10[1].dtd = 1;
    D_8006794C->unk10[1].isbg = 1;
}

s32 func_8019A9D8(void) {
    if (D_80071E32 == 0) {
        return 0;
    }
    D_8019DC94.unk48 = 0;
    D_8019DC94.unk4C = 0;
    D_8019DC94.unk58 = 0;
    D_8019DC94.unk5C = 0;
    D_8019DC94.unk44 = 0;
    D_8019DC94.unk50 = 0;
    D_8019DC94.unk54 = 0;
    D_80071E32 = 0;
    return 1;
}

s32 func_8019AA20(DISPENV *env) {
    VSync(0);
    PutDispEnv(env);
    SetDispMask(1);
    return 1;
}

s32 func_8019AA5C(void) {
    u8 done;

    if (D_80071E32 < 2) {
        return 0;
    }
    done = 0;
    while (!done) {
        if (func_80199F30(1) == 0) {
            done = 1;
        }
    }
    return 1;
}

s32 func_8019AAB4(s32 arg0) {
    if (arg0 != 0) {
        D_8019DC94.unk4 = 1;
    } else {
        D_8019DC94.unk4 = 0;
        D_80071E38 = 0;
        func_8019CC3C(0);
    }
    return 1;
}

s32 func_8019AAF8(void) {
    func_80056228();
    func_8019C864();
    D_800739B0[0].unk0 = 0;
    D_800739B0[0].unk1 = 0;
    D_80071E32--;
    if (D_8019DC94.unk60 != NULL) {
        D_8019DC94.unk60();
    }
    DecDCToutCallback(0);
    func_8019A7A8(0xE0, 0);
    ResetGraph(3);
    return 0;
}

s32 func_8019AB70(void) {
    if (D_80071E32 != 0) {
        return D_80071E34;
    }
    return -1;
}

void func_8019AB98(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 0) {
        D_8019DC94.unk38 = 3;
    } else {
        D_8019DC94.unk38 = arg0;
    }
    if (arg1 < 0) {
        D_8019DC94.unk3C = 4;
    } else {
        D_8019DC94.unk3C = arg1;
    }
    if (arg2 < 0) {
        D_8019DC94.unk40 = 0x7F;
    } else {
        D_8019DC94.unk40 = arg2;
    }
}

s32 func_8019ABF0(void) {
    s32 v;

    func_8019C99C(&v);
    return v;
}

void func_8019AC14(u32 arg0) {
    if (arg0 < 2) {
        func_8019C9B4(arg0);
    }
}

void func_8019AC3C(void (*arg0)()) {
    D_8019DC94.unk60 = arg0;
}

void func_8019AC48(s32 arg0) {
    if (arg0 != 0) {
        D_8019DC94.unk44 = NULL;
    } else {
        D_8019DC94.unk44 = D_8019F288;
    }
}

s32 func_8019AC6C(s32 *arg0, u32 arg1) {
    if (arg1 >= 6) {
        return 0;
    }
    *arg0 = D_8019DC5C[arg1].unk0;
    return 1;
}

s32 func_8019AC9C(s32 arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 6 || arg2 < 0 || arg2 >= D_8019DC5C[arg1].unk0) {
        return 0;
    }
    func_80065238(arg0, D_8019DC5C[arg1].unk4[arg2].unk0);
    return 1;
}

s32 func_8019AD0C(void) {
    s32 v;

    if (D_80071E32 < 2) {
        return -1;
    }
    func_8019CBF4(&v);
    return v;
}

s32 func_8019AD4C(Ovl64Header *arg0, u16 *arg1) {
    if (D_8019DC94.unk6 != 0) {
        return 0;
    }
    arg0->unk0 = D_8019F258.unk0;
    arg0->unk2 = D_8019F258.unk2;
    arg0->unk4 = D_8019F258.unk4;
    arg0->unk6 = D_8019F258.unk6;
    arg0->unk8 = D_8019F258.unk8;
    arg0->unkA = D_8019F258.unkA;
    arg0->unkC = D_8019F258.unkC;
    arg0->unkE = D_8019F258.unkE;
    arg0->unk10 = D_8019F258.unk10;
    arg0->unk14 = D_8019F258.unk14;
    arg0->unk18 = D_8019F258.unk18;
    arg0->unk1C = D_8019F258.unk1C;
    *arg1 = D_8019F278;
    return 1;
}

s32 func_8019AE0C(s32 *arg0, s32 *arg1) {
    if (D_8019DC94.unk6 != 0) {
        return 0;
    }
    *arg0 = D_8019DC88.unk0;
    *arg1 = D_8019DC88.unk4;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AE44);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AE98);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AEC0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B00C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B188);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B3D4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B7C4);

void func_8019BA70(void) {
    Ovl64SectHdr *s;
    u32 *p;
    u32 *src;
    u32 *dst;
    u32 i;
    Ovl64RingHdr *h;

    p = (u32 *)(D_8019DE10.unk78 +
                (D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 + 1 - D_8019DE10.unk5C)) * 0x8F4);
    p -= 8;
    if (D_8019DE10.unk64 == 0) {
        dst = (u32 *)&D_8019DE10.unk74[D_8019DE10.unk68];
        for (i = 0, src = p; i < 8; i++) {
            *dst++ = *src++;
        }
        s = (Ovl64SectHdr *)p;
        if (s->unk8 != D_8019DE10.unk58) {
            D_8019DE10.unk2 = 2;
            func_80023018();
        }
        if (s->unk2 != 4) {
            goto end;
        }
    }
    s = (Ovl64SectHdr *)p;
    if ((s->unk1C.w >> 28) & 2) {
        D_8019F750[D_8019DE10.unk68].unk0 = s->unk1C.w & 0xFFFF;
        D_8019F750[D_8019DE10.unk68].unk4 = (s32)((u8 *)s + (s->unk1C.h.hi & 0xFFF));
    }
end:
    D_8019DE10.unk44++;
    if (++D_8019DE10.unk48 >= D_8019DE10.unk54) {
        D_8019DE10.unk64++;
        D_8019DE10.unk48 = 0;
        D_8019DE10.unk58++;
        if (D_8019DE10.unk8 != NULL) {
            D_8019DE10.unk8();
        }
        h = &D_8019DE10.unk74[D_8019DE10.unk68];
        h->unk4 = D_8019DE10.unk5C - D_8019DE10.unk64;
        h->unk0 = (h->unk0 & 0xFFF) | 0xC000;
        if (D_8019DE10.unk58 > D_8019DE10.unk3C) {
            D_8019DE10.unk2 = 1;
            func_80023018();
        }
        D_8019DE10.unk64 = 0;
        if (D_8019DE10.unk68 == 0) {
            D_8019DE10.unk68 = D_8019DE10.unk6C;
        }
        D_8019DE10.unk68--;
        D_8019F750[D_8019DE10.unk68].unk0 = 0;
        D_8019F750[D_8019DE10.unk68].unk4 = 0;
    } else {
        D_8019DE10.unk64++;
    }
    if (++D_8019DE10.unk4C >= D_8019DE10.unk50) {
        D_8019DE10.unk4C = 0;
    }
}

void func_8019BCE8(void) {
    Ovl64StreamSect *s;
    Ovl64FrameInfo *info;
    Ovl64RingHdr *h;
    u32 cur;

    info = (Ovl64FrameInfo *)&D_8019DE10; /* what the callback gets when the sector has no frame info */
    D_8019DE10.unk44++;
    s = (Ovl64StreamSect *)(D_8019DE10.unk90 + (D_8019DE10.unk80 << 11));
    if (++D_8019DE10.unk4C >= D_8019DE10.unk50) {
        D_8019DE10.unk4C = 0;
    }
    if (D_8019DE10.unk4C == 2) {
        if (s->unk12 == 1) {
            if (D_8019DE10.unk10 == 1) {
                info = &s->unk14[1];
            } else {
                info = &s->unk14[0];
            }
            D_8019F510[D_8019DE10.unk68].unk0 = info->unk0;
            D_8019F510[D_8019DE10.unk68].unk2 = info->unk2;
            D_8019F510[D_8019DE10.unk68].unk4 = info->unk4;
            D_8019F510[D_8019DE10.unk68].unk6 = info->unk6;
            D_8019F510[D_8019DE10.unk68].unk8 = info->unk8;
            D_8019F510[D_8019DE10.unk68].unkA = info->unkA;
            D_8019F510[D_8019DE10.unk68].unkC = info->unkC;
            D_8019F510[D_8019DE10.unk68].unkE = info->unkE;
            D_8019F510[D_8019DE10.unk68].unk10 = info->unk10;
            D_8019F510[D_8019DE10.unk68].unk12 = info->unk12;
            D_8019F510[D_8019DE10.unk68].unk14 = info->unk14;
            D_8019F510[D_8019DE10.unk68].unk18 = info->unk18;
            D_8019F510[D_8019DE10.unk68].unk1C = info->unk1C;
            D_8019F510[D_8019DE10.unk68].unk20 = info->unk20;
            D_8019F510[D_8019DE10.unk68].unk22 = info->unk22;
            D_8019DE10.unk3 = 1;
        }
        if (s->unk5C < 0) {
            if (((u32)s->unk5C >> 8 & 0xFF) >= 0x80) {
                D_8019F7D0[D_8019DE10.unk68].unk0 = 1;
            } else {
                D_8019F7D0[D_8019DE10.unk68].unk0 = 0;
            }
            D_8019F7D0[D_8019DE10.unk68].unk1 = s->unk5C;
        } else {
            D_8019F7D0[D_8019DE10.unk68].unk0 = 0;
            D_8019F7D0[D_8019DE10.unk68].unk1 = 0;
        }
        if (D_8019DE10.unk4 != NULL) {
            D_8019DE10.unk4(D_8019DE10.unk90 + ((D_8019DE10.unk80 - 1) << 11),
                            D_8019DE10.unk90 + (D_8019DE10.unk80 << 11), info);
        }
    }
    cur = D_8019DE10.unk80;
    D_8019DE10.unk84 = cur;
    D_8019DE10.unk88++;
    if (D_8019DE10.unk88 > D_8019DE10.unk8C) {
        D_8019DE10.unk8C = D_8019DE10.unk88;
    }
    if (++D_8019DE10.unk80 >= D_8019DE10.unk7C) {
        D_8019DE10.unk80 = 0;
    }
    if (++D_8019DE10.unk48 >= D_8019DE10.unk54) {
        D_8019DE10.unk48 = 0;
        D_8019DE10.unk58++;
        if (D_8019DE10.unk8 != NULL) {
            D_8019DE10.unk8();
        }
        h = &D_8019DE10.unk74[D_8019DE10.unk68];
        h->unk4 = D_8019DE10.unk5C - D_8019DE10.unk64;
        h->unk0 = (h->unk0 & 0xFFF) | 0xC000;
        if (D_8019DE10.unk58 > D_8019DE10.unk3C) {
            D_8019DE10.unk2 = 1;
            func_80023018();
        }
        D_8019DE10.unk64 = 0;
        if (D_8019DE10.unk68 == 0) {
            D_8019DE10.unk68 = D_8019DE10.unk6C;
        }
        D_8019DE10.unk68--;
        D_8019F750[D_8019DE10.unk68].unk0 = 0;
        D_8019F750[D_8019DE10.unk68].unk4 = 0;
    }
    if (s->unk60 != 0xEEEEEEEE && s->unk60 != D_8019DE10.unk3C) {
        D_8019DE10.unk3C = s->unk60;
    }
}

s32 func_8019C1B0(s32 unused, u32 arg1, s32 arg2, void (*arg3)(void), s32 arg4) {
    CdlLOC *loc;
    s32 i;
    Ovl64Pair *p;

    CdIntToPos(D_8019DE10.unk1C, &D_8019DE10.unk20);
    D_8019DE10.unk40 = CdPosToInt(&D_8019DE10.unk20);
    if (arg1 >= 2) {
        D_8019DE10.unk40 += (arg1 - 1) * D_8019DE10.unk54;
    }
    D_8019DE0C = 0;
    D_8019DE10.unk18 = -1;
    D_8019DE10.unk38 = arg1;
    D_8019DE10.unk3C = arg2;
    D_8019DE10.unk8 = arg3;
    D_8019DE10.unkC = arg4;
    D_8019DE10.unk48 = 0;
    D_8019DE10.unk4C = 0;
    D_8019DE10.unk10 = 0;
    D_8019DE10.unk58 = 1;
    D_8019DE10.unk44 = D_8019DE10.unk40;
    D_8019DE10.unk14 = VSync(-1);
    D_8019DEA8 = 0;
    for (i = 0; i < 16; i++) {
        D_8019F750[i].unk0 = 0;
        D_8019F750[i].unk4 = 0;
    }
    D_8019DEA4 = 0;
    func_800236A4(0);
    func_80022F1C(D_8019DE10.unk44, func_8019C434);
    func_80056448((s32)D_8019DE10.unk90, D_8019DE10.unk7C << 11);
    return 0;
}

s32 func_8019C2F4(u8 **data, Ovl64RingHdr **hdr, Ovl64FrameInfo **arg2, Ovl64Pair **arg3, Ovl64Flag **arg4) {
    Ovl64RingHdr *h = &D_8019DE10.unk74[D_8019DE10.unk70];

    if (!(h->unk0 & 0x8000)) {
        if (D_8019DE10.unk2 == 0) {
            return 3;
        }
        return 5;
    }
    *hdr = h;
    *data = D_8019DE10.unk78 + (h->unk4 + D_8019DE10.unk70 * D_8019DE10.unk5C) * 0x8F4;
    if (D_8019DE10.unk3 != 0) {
        *arg2 = &D_8019F510[D_8019DE10.unk70];
        *arg3 = &D_8019F750[D_8019DE10.unk70];
    } else {
        *arg2 = NULL;
        *arg3 = NULL;
    }
    *arg4 = &D_8019F7D0[D_8019DE10.unk70];
    if (D_8019DE10.unk70 == 0) {
        D_8019DE10.unk70 = D_8019DE10.unk6C;
    }
    D_8019DE10.unk70--;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C434);

s32 func_8019C7E4(void) {
    if (D_8019DE10.unk90 == NULL || D_8019DE10.unk74 == NULL || D_8019DE10.unk78 == NULL || D_8019DE10.unk60 == 0 ||
        D_8019DE10.unk7C == 0) {
        return 4;
    }
    D_8019DE10.unk0 = 1;
    D_8019DE10.unk1 = 0;
    D_8019DE10.unk2 = 0;
    D_8019DE10.unk3 = 0;
    D_8019DE10.unk18 = -1;
    return 0;
}

s32 func_8019C864(void) {
    func_800213A0(0);
    return 0;
}

s32 func_8019C884(Ovl64RingHdr *ring, u32 per, u32 size) {
    u32 n;
    u32 i;

    if (size / per * per != size) {
        size = size / per * per;
    }
    if (per == 0 || size < per) {
        return 2;
    }
    n = size / per;
    if (n > 16) {
        return 2;
    }
    D_8019DE10.unk78 = (u8 *)ring + (n + 1) * sizeof(Ovl64RingHdr);
    D_8019DE10.unk74 = ring;
    D_8019DE10.unk5C = per;
    D_8019DE10.unk60 = size;
    D_8019DE10.unk64 = 0;
    D_8019DE10.unk68 = n - 1;
    D_8019DE10.unk6C = n;
    D_8019DE10.unk70 = n - 1;
    for (i = 0; i < n; i++) {
        ring->unk0 = (ring->unk0 & 0xFFF) | 0x1000;
        ring++;
    }
    return 0;
}

s32 func_8019C954(u8 *arg0, s32 arg1, u32 arg2) {
    if (arg1 != 2) {
        return 2;
    }
    D_8019DE10.unk7C = arg2;
    D_8019DE10.unk80 = 0;
    D_8019DE10.unk84 = 0;
    D_8019DE10.unk88 = 0;
    D_8019DE10.unk8C = 0;
    D_8019DE10.unk90 = arg0;
    return 0;
}

s32 func_8019C98C(s32 arg0) {
    D_8019DE10.unk1C = arg0;
    return 0;
}

s32 func_8019C99C(s32 *arg0) {
    *arg0 = D_8019DE10.unk10;
    return 0;
}

s32 func_8019C9B4(u32 arg0) {
    if (arg0 >= 2) {
        return 2;
    }
    D_8019DE10.unk10 = arg0;
    return 0;
}

s32 func_8019C9D4(void) {
    if (D_8019DE10.unk88 < 2) {
        return 6;
    }
    D_8019DE10.unk88 -= 2;
    func_80056488(&D_8019DE10);
    return 0;
}

s32 func_8019CA1C(CdlLOC *loc) {
    s32 pos;

    func_8019CC7C();
    pos = CdPosToInt(loc);
    if (func_80022B18() != 0) {
        func_80022FD0(0);
        while (func_80022B18() != 0) {
        }
    }
    func_80022F1C(pos, func_8019C434);
    return 0;
}

s32 func_8019CA80(CdlLOC *loc) {
    loc->minute = D_8019DE10.unk20.minute;
    loc->second = D_8019DE10.unk20.second;
    loc->sector = D_8019DE10.unk20.sector;
    loc->track = D_8019DE10.unk20.track;
    return 0;
}

s32 func_8019CAC0(CdlLOC *loc) {
    CdIntToPos(D_8019DE10.unk44, loc);
    return 0;
}

s32 func_8019CAEC(u8 **data, Ovl64RingHdr **hdr, s32 *kind) {
    Ovl64RingHdr *h;

    h = &D_8019DE10.unk74[D_8019DE10.unk70];
    if (h->unk0 & 0x1000) {
        *kind = 0;
    } else if (h->unk0 & 0x8000) {
        *kind = 1;
    } else if (h->unk0 & 0x2000) {
        *kind = 2;
    } else if (h->unk0 & 0x4000) {
        *kind = 3;
    }
    *hdr = h;
    *data = D_8019DE10.unk78 + (h->unk4 + D_8019DE10.unk70 * D_8019DE10.unk5C) * 0x8F4;
    return 0;
}

s32 func_8019CBA0(s32 *count) {
    Ovl64RingHdr *h = D_8019DE10.unk74;
    s32 n = D_8019DE10.unk6C;
    s32 i;

    for (i = 0; i < n; i++, h++) {
        if (h->unk0 & 0x1000) {
            (*count)++;
        }
    }
    return 0;
}

s32 func_8019CBF4(s32 *arg0) {
    *arg0 = D_8019DE10.unk3C;
    return 0;
}

s32 func_8019CC0C(void) {
    if (++D_8019DE10.unk70 >= D_8019DE10.unk6C) {
        D_8019DE10.unk70 = 0;
    }
    return 0;
}

s32 func_8019CC3C(s32 arg0) {
    if (arg0 != 0) {
        func_80023018();
    } else {
        func_80022F1C(D_8019DE10.unk44, func_8019C434);
    }
    return 0;
}

s32 func_8019CC7C(void) {
    Ovl64RingHdr *h = D_8019DE10.unk74;
    s32 n = D_8019DE10.unk6C;
    s32 i;

    for (i = 0; i < n; i++, h++) {
        h->unk0 = (h->unk0 & 0xFFF) | 0x1000;
    }
    return 0;
}

s32 func_8019CCBC(u8 *data) {
    Ovl64RingHdr *h = &D_8019DE10.unk74[(u32)(data - D_8019DE10.unk78) / 0x8F4 / D_8019DE10.unk5C];

    h->unk0 = (h->unk0 & 0xFFF) | 0x1000;
    if (D_8019DE10.unk1 != 0) {
        func_80022F1C(D_8019DE10.unk44, func_8019C434);
        D_8019DE10.unk1 = 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTReset);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTGetEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTPutEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTin);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTout);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinCallback);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutCallback);

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_8019903C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D020);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D110);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D1A0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D22C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D2C0);

u32 func_8019D354(void) {
    return *D_8019E008;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D36C);

u16 func_8019D3F0(u16 *p) {
    return *p;
}

#include "ovl_15.h"

s32 func_800AB490(void) {
    s32 timer;
    s32 wait;
    s32 n;

    timer = -1;
    wait = 0;
    func_800AC374();
    D_800AE3EC = 0;
    do {
        func_800AC450();
        pad_update();
        switch (D_800B0FD0) {
        case 0:
            timer = -1;
            D_800B0FD0 = 1;
            break;
        case 1:
            if (timer == -1) {
                func_800320B8(1, 1, 0, 0xFFFFFF);
                timer = 0x21C;
                func_800320B8(1, 1, 0xB4, 0);
            } else if (timer != 0) {
                setlen(&D_800B0F60[g_gameState->unk1C->unk8][0], 4);
                D_800B0F60[g_gameState->unk1C->unk8][0].code = 0x64;
                D_800B0F60[g_gameState->unk1C->unk8][0].r0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].g0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].b0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].x0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].y0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].u0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].v0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].w = 0xC0;
                D_800B0F60[g_gameState->unk1C->unk8][0].h = 0xE0;
                D_800B0F60[g_gameState->unk1C->unk8][0].clut = 0x3800;
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0F60[g_gameState->unk1C->unk8][0]);
                setDrawTPage(&D_800B0FB0[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0FB0[g_gameState->unk1C->unk8][0]);
                setlen((D_800B0F60[g_gameState->unk1C->unk8] + 1), 4);
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->code = 0x64;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->y0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->u0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->v0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->w = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0F60[g_gameState->unk1C->unk8] + 1));
                setDrawTPage(D_800B0FB0[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0FB0[g_gameState->unk1C->unk8] + 1));
            }
            break;
        case 2:
            setlen(&D_800B0F60[g_gameState->unk1C->unk8][0], 4);
            D_800B0F60[g_gameState->unk1C->unk8][0].code = 0x64;
            D_800B0F60[g_gameState->unk1C->unk8][0].r0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].g0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].b0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].x0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].y0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].u0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].v0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].w = 0xC0;
            D_800B0F60[g_gameState->unk1C->unk8][0].h = 0xE0;
            D_800B0F60[g_gameState->unk1C->unk8][0].clut = 0x3800;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0F60[g_gameState->unk1C->unk8][0]);
            setDrawTPage(&D_800B0FB0[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0FB0[g_gameState->unk1C->unk8][0]);
            setlen((D_800B0F60[g_gameState->unk1C->unk8] + 1), 4);
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->code = 0x64;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->y0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->u0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->v0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->w = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0F60[g_gameState->unk1C->unk8] + 1));
            setDrawTPage(D_800B0FB0[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0FB0[g_gameState->unk1C->unk8] + 1));
            if (timer == -1) {
                timer = 0x5A;
                func_800320B8(1, 1, timer, 0xFFFFFF);
            } else if (--timer == 0) {
                D_800B0FD0 = 4;
            }
            break;
        }
        if (D_800AE3EC == 0x10) {
            if (g_padPortHeld[0] & 8) {
                timer = -1;
                D_800B0FD0 = 2;
                snd_playSe(0xC18);
                D_80073998[0] = -1;
            }
        } else if (g_padPortPressed[0] & D_800AE3F0[D_800AE3EC]) {
            if (++D_800AE3EC == 0x10) {
                snd_playSe(0x67);
            }
        } else if (g_padPortPressed[0] != 0) {
            wait = 5;
            D_800AE3EC = 0;
        }
        D_800739B0[0].unk0 = 0;
        if (wait > 0) {
            wait--;
        }
        func_80032098();
        func_800AC494();
    } while ((u32)D_800B0FD0 < 3);
    func_800AC448();
    return D_800B0FD0 != 3;
}

s32 func_800AC374(void) {
    s32 p = (s32)db_lookupRegistered(4, D_800AE432 | 0x40000, 0);

    func_8001D9CC(p, 0, 0xE0, 0x280, 0);
    func_8001D8A8(p);
    func_800552B8(0);
    func_800AC598(0, 0x7009C, 0, 0, 0);
    func_800AC598(0xC401, 0x7009C, 0x5A, 0, 0x7F);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800B0FD0 = 0;
    D_800AE430 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 func_800AC448(void) {
    return 0;
}

void func_800AC450(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AC494(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}

s32 func_800AC598(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800AE43C == -1) {
                D_800AE43C = kind;
            }
            if (g_gameState->unk1C->unk81C[0] != arg1) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800552B8(g_gameState->unk1C->unk824[0]);
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                g_gameState->unk1C->unk820[0] = 0x1F3;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk826[0] = 0x7F;
            }
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            g = g_gameState->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800AE43C = -1;
            g_gameState->unk1C->unk0 |= 0x30000000;
            return 0;
        case 1:
            if (g_gameState->unk1C->unk81C[0] == (u16)id) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
            }
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(g_gameState->unk1C->unk824[0], arg2);
            g_gameState->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(g_gameState->unk1C->unk824[0], arg2, arg3);
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
            return 0;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)db_lookupRegistered(4, id, 0);
        }
        switch (kind) {
        case 0:
            func_800553EC(h, arg2, arg3, arg4);
            return 0;
        case 1:
            func_8005546C(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                return 0;
            case 3:
                func_8005593C(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8(h, arg2, arg3, arg4);
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                return 0;
            case 3:
                func_80055980(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                return 0;
            case 5:
                func_800557F0(arg2);
                return 0;
            case 3:
                func_800558DC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                return -1;
            case 3:
                func_80055908(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void func_800ACB98(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    g_gameState->unk1C->unk83C = func_800AC598;
    func_800ACD70();
    func_800ACCB4();
    func_800239D0();
    SetDispMask(1);
}

void func_800ACC08(void) {
    s32 i;

    SetDispMask(0);
    if (g_gameState->unk1C->unk0 & 0x20000000) {
        func_800AC598(0x8301, 0x7009C, 0x5A, 0, 0);
        for (i = 0x59; i >= 0; i--) {
            VSync(0);
        }
    }
    func_800AC598(0x100, 0x7009C, 0, 0, 0);
    db_unregister(D_800AE440);
    D_800AE440 = NULL;
}

void func_800ACCB4(void) {
    s32 a;
    s32 b;

    func_800552B8(0);
    a = (s32)db_lookupRegistered(4, 0x7009C, 0);
    memmove(g_gameState->unk1C->unk814, (void *)a, (s32)db_lookupRegistered(5, 0x7009C, 0));
    a = (s32)db_lookupRegistered(4, 0x901F3, 0);
    b = (s32)db_lookupRegistered(5, 0x901F3, 0);
    func_80055DF4();
    func_80055E14(a, b, 1);
}

void func_800ACD70(void) {
    u8 *buf;
    FileEntry *e;

    buf = D_80067944 + 0x10000;
    e = file_find(1, 0x2F44);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    D_800AE440 = buf;
    db_register((s32)buf, 0);
}

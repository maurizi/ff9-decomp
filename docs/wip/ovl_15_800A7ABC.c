/* ovl_15 func_800A7ABC (2668 B): WIP, about 95% (669 vs 667 insns, ~31 diff rows).
 * Draws the WIN/LOSE/DRAW banner (two SPRTs and two tpages per kind) into the
 * D_800AE5F8 / D_800AE4F8 buffers. Control flow, prologue and constant hoisting
 * match. What's left is reload scheduling: the original hoists the reload of y
 * (lhu 0x54(sp)) and of c (lw 0x18(sp)) above the u0/v0/rgb stores, ours
 * leaves them in place with a load-delay nop. Making y s16/u16 is worse. */

extern DR_TPAGE D_800AE4F8[2][16];

s32 func_800A7ABC(s32 kind, s32 c, s32 x, s32 y) {
    SPRT *p;
    DR_TPAGE *t;
    s32 x1;
    s32 x2;

    switch (kind) {
    case 0:
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->u0 = 0;
        p->v0 = 0;
        setRGB0(p, c, c, c);
        p->y0 = y;
        p->w = 0x68;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->y0 = y;
        p->u0 = 0x68;
        p->v0 = 0;
        setRGB0(p, c, c, c);
        p->w = 0x70;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    case 1:
        x1 = x - 2;
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x1;
        setRGB0(p, c, c, c);
        p->u0 = 0;
        p->y0 = y;
        p->w = 0x68;
        p->h = 0x28;
        p->v0 = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x1;
        p->y0 = y;
        p->u0 = 0x68;
        p->v0 = 0x28;
        p->w = 0x70;
        setRGB0(p, c, c, c);
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    case 2:
        x2 = x - 0x10;
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x2;
        p->w = 0x88;
        setRGB0(p, c, c, c);
        p->u0 = 0;
        p->v0 = 0xA0;
        p->y0 = y;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x2;
        p->y0 = y;
        p->v0 = 0xC8;
        p->u0 = 0;
        setRGB0(p, c, c, c);
        p->w = 0x90;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[D_8006794C->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    }
    return 0;
}

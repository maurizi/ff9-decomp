/* ovl_0d func_800B1A84 (3800 B): WIP, about 98% (949 vs 950 insns, ~17 diff rows).
 * Only the else-branch of the first if is off. The original does not CSE the
 * 0x3A40 clut or ot + 0xFFA between the 2nd and 3rd sprite there (it reloads ot
 * from the stack each time); ours keeps both in s-registers across the
 * c = 0x40/0x80 select. Everything else (digit loop, SPRT_8 frame loops, text)
 * matches. The do { } while (0) around the card sprite tail and reusing c for
 * the digits are permuter finds that fix register allocation. */

void SetSprt16(SPRT_16 *);
s32 func_80062498(void);
s32 func_800624D8(void);
s32 func_8006251C(void);
s32 func_800AF268(TmState *);
extern u8 D_800A74A8[];

void *func_800B1A84(u32 *ot, SPRT *p, TmState *s) {
    u8 buf[0x20];
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 c;

    SetSprt(p);
    if ((u32)(s->unk0 - 0x12) < 2 || s->unk0 == 0x23 || s->unk0 == 0x1B) {
        SetSprt(p);
        p->w = 0x30;
        p->h = 0x18;
        p->u0 = 0xB8;
        p->v0 = 0x78;
        p->x0 = 0x58;
        p->y0 = 0xA;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x61;
        p->h = 0x20;
        p->u0 = 0x94;
        p->v0 = 0xD2;
        p->x0 = 0x8C;
        p->y0 = 0xC;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
    } else {
        SetSprt(p);
        p->w = 0x4E;
        p->h = 0x15;
        p->u0 = 0x98;
        p->v0 = 0xBC;
        p->x0 = 0x79;
        p->y0 = 0xE;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = 0xF0;
        p->v0 = 0xB8;
        p->x0 = 0xCA;
        p->y0 = 0x12;
        if (!(s->unk2E8 & 0x20)) {
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
        } else {
            p->r0 = 0x40;
            p->g0 = 0x40;
            p->b0 = 0x40;
        }
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = 0xF0;
        p->v0 = 0xB8;
        p->x0 = 0x66;
        p->y0 = 0x12;
        c = 0x40;
        if (!(s->unk2E8 & 0x20)) {
            c = 0x80;
        }
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
    }
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x280, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFFA, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);

    for (i = 0; i < 100; i++) {
        m = n = func_80062560(i);
        if (n != 1) {
            c = m % 10;
            if (c != 0 || n >= 10) {
                SetSprt(p);
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->clut = 0x3B40;
                p->w = 4;
                p->h = 5;
                p->u0 = (c % 5) * 4;
                p->v0 = (c / 5) * 8 - 0x10;
                p->x0 = (i / 10) * 15 + 0x1B;
                p->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, p);
                p++;
            }
            m /= 10;
            c = m / 10;
            if (m != c * 10 || n >= 100) {
                c = m;
                c %= 10;
                SetSprt(p);
                p->w = 4;
                p->h = 5;
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->clut = 0x3B40;
                p->u0 = (c % 5) * 4;
                p->v0 = (c / 5) * 8 - 0x10;
                p->x0 = (i / 10) * 15 + 0x17;
                p->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, p);
                p++;
                if (n >= 100) {
                    SetSprt(p);
                    p->w = 4;
                    p->h = 5;
                    p->u0 = 4;
                    p->v0 = 0xF0;
                    p->x0 = (i / 10) * 15 + 0x13;
                    p->y0 = (i % 10) * 15 + 0x35;
                    p->r0 = 0x80;
                    p->g0 = 0x80;
                    p->b0 = 0x80;
                    p->clut = 0x3B40;
                    AddPrim(ot + 11, p);
                    p++;
                }
            }
        }
        SetSprt16((SPRT_16 *)p);
        ((SPRT_16 *)p)->x0 = (i / 10) * 15 + 0x12;
        ((SPRT_16 *)p)->y0 = (i % 10) * 15 + 0x2D;
        if (n == 0) {
            ((SPRT_16 *)p)->u0 = 0;
            ((SPRT_16 *)p)->v0 = 0xE0;
        } else {
            k = func_800627BC(i);
            if (n >= 2) {
                k += 7;
            }
            ((SPRT_16 *)p)->u0 = k * 16 + 0x10;
            ((SPRT_16 *)p)->v0 = 0xE0;
        }
        do {
            ((SPRT_16 *)p)->r0 = 0x80;
            ((SPRT_16 *)p)->g0 = 0x80;
            ((SPRT_16 *)p)->b0 = 0x80;
            ((SPRT_16 *)p)->clut = 0x3B40;
            AddPrim(ot + 11, p);
        } while (0);
        p = (SPRT *)((SPRT_16 *)p + 1);
    }

    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x340, 0x1E0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);
    SetSprt(p);
    p->w = 0xA8;
    p->h = 8;
    p->u0 = 0x58;
    p->v0 = 0xF0;
    p->x0 = 9;
    p->y0 = 0x29;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 0xA8;
    p->h = 8;
    p->u0 = 0x58;
    p->v0 = 0xF8;
    p->x0 = 9;
    p->y0 = 0xBE;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 8;
    p->h = 0x98;
    p->u0 = 0xF0;
    p->v0 = 0x58;
    p->x0 = 0xD;
    p->y0 = 0x2B;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 8;
    p->h = 0x98;
    p->u0 = 0xF8;
    p->v0 = 0x58;
    p->x0 = 0xA5;
    p->y0 = 0x2B;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x300, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);
    setlen((TILE *)p, 3);
    ((TILE *)p)->code = 0x60;
    ((TILE *)p)->x0 = 0x12;
    ((TILE *)p)->y0 = 0x2D;
    ((TILE *)p)->w = 0x95;
    ((TILE *)p)->h = 0x95;
    ((TILE *)p)->r0 = 0x20;
    ((TILE *)p)->g0 = 0x18;
    ((TILE *)p)->b0 = 0x18;
    AddPrim(ot + 11, p);
    p = (SPRT *)((TILE *)p + 1);
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 2, 0, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);

    func_8002B56C(0x19, 0xCB, 0xFFA, "Stock:", 7);
    func_8002BAE4(D_800B3CF0, buf, 3, 3);
    func_8002B5B4(0x43, 0xC9, 0xFFA, buf, 0);
    func_8002B56C(0x65, 0xCB, 0xFFA, "Type:", 7);
    func_8002BAE4(D_800B3CF4, buf, 3, 3);
    func_8002B5B4(0x89, 0xC9, 0xFFA, buf, 0);

    if ((u32)(s->unk0 - 0x12) < 2 || s->unk0 == 0x23 || s->unk0 == 0x1B) {

        func_8002B56C(0xBC, 0x42, 0xFFA, "WIN", 7);
        func_8002BAE4(func_80062498(), buf, 4, 1);
        func_8002B56C(0xDC, 0x42, 0xFFA, buf, 0);
        func_8002B56C(0xBC, 0x4C, 0xFFA, "LOSE", 7);
        func_8002BAE4(func_800624D8(), buf, 4, 1);
        func_8002B56C(0xDC, 0x4C, 0xFFA, buf, 0);
        func_8002B56C(0xBC, 0x56, 0xFFA, "DRAW", 7);
        func_8002BAE4(func_8006251C(), buf, 4, 1);
        func_8002B56C(0xDC, 0x56, 0xFFA, buf, 0);

        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xB5;
        ((SPRT_8 *)p)->y0 = 0x3D;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x38;
        ((SPRT_8 *)p)->v0 = 0xF0;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xF6;
        ((SPRT_8 *)p)->y0 = 0x3D;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x48;
        ((SPRT_8 *)p)->v0 = 0xF0;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xB5;
        ((SPRT_8 *)p)->y0 = 0x5A;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x38;
        ((SPRT_8 *)p)->v0 = 0xF8;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xF6;
        ((SPRT_8 *)p)->y0 = 0x5A;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x48;
        ((SPRT_8 *)p)->v0 = 0xF8;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        for (i = 0; i < 8; i++) {
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->y0 = 0x3D;
            ((SPRT_8 *)p)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x40;
            ((SPRT_8 *)p)->v0 = 0xF0;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->y0 = 0x5A;
            ((SPRT_8 *)p)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x40;
            ((SPRT_8 *)p)->v0 = 0xF8;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
        }
        for (i = 0; i < 3; i++) {
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->x0 = 0xB5;
            ((SPRT_8 *)p)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x50;
            ((SPRT_8 *)p)->v0 = 0xF8;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->x0 = 0xF6;
            ((SPRT_8 *)p)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x50;
            ((SPRT_8 *)p)->v0 = 0xF0;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
        }
        setlen((TILE *)p, 3);
        ((TILE *)p)->code = 0x60;
        ((TILE *)p)->x0 = 0xB8;
        ((TILE *)p)->y0 = 0x41;
        ((TILE *)p)->w = 0x46;
        ((TILE *)p)->h = 0x1E;
        ((TILE *)p)->r0 = 0x20;
        ((TILE *)p)->g0 = 0x18;
        ((TILE *)p)->b0 = 0x18;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((TILE *)p + 1);
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x300, 0) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
    }
    if (s->unk47C != 5 && s->unk0 != 0x16 && func_80062560(func_800AF268(s)) >= 2) {
        func_8002BAE4(s->unk4B4 + 1, buf, 3, 1);
        func_8002B56C(0xBF, 0xBA, 0xFFA, buf, 0);
        func_8002B56C(0xD4, 0xBA, 0xFFA, D_800A74A8, 0);
        func_8002BAE4(func_80062560(func_800AF268(s)), buf, -1, 1);
        func_8002B56C(0xDB, 0xBA, 0xFFA, buf, 0);
        func_8002B5FC(0xB5, 0xB6, 0xFFA, func_80035774(0x1E), 0);
        func_8002B5FC(0xF1, 0xB6, 0xFFA, func_80035774(0x1C), 0);
    }
    if (s->unk47C == 0) {
        D_800B3CF4 = func_80062638();
        D_800B3CF0 = func_800625F8();
    }
    return p;
}

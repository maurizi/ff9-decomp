#include "ovl_0a.h"

s32 func_800AA3F4(s32 uid) {
    s32 id;

    id = func_800AAEF8(uid);
    if (id == -1) {
        return 0;
    }
    id = func_800AAEC4(id);
    if (db_lookupRegistered(4, id, 0) == NULL) {
        D_800C9F20[g_gameState->unk1C->unk8E4.field->unkC->unk14->unk60] = id;
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk60++;
    }
    return 0;
}

void func_800AA4BC(void) {
    s32 i;

    for (i = 15; i >= 0; i--) {
        D_800C9F20[i] = -1;
    }
}

s32 func_800AA4E8(s32 arg0) {
    FileEntry *f;
    s32 size;
    Unk800C9CCC *c;
    s32 n;
    FieldVramPos *vp;
    s32 i;
    s32 j;
    s32 pack2;
    s32 pack1;
    u8 *hdr;
    s16 *tp;
    s16 *uv;
    FieldTexEnt *ent;
    s32 img;
    s32 x, y, cx, cy, e;
    s32 e1;
    s32 cx2;
    s32 cy2;
    s32 x2;
    s32 y2;

    f = file_find(10, arg0);
    size = (f->unkC - f->unk4) << 11;
    c = func_800AC344(arg0);
    if (c->unk4 == 0) {
        cdRead_init(g_gameState->unk1C->unk854, f->unk4, size,
                      (void *)(func_800AC3D8(arg0, size) != 0 ? c->unk4 : D_80067948), 0, -1, 0);
        while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
        }
    }
    if (c->unk4 != 0) {
        s32 m = file_copyViaVram(D_80067948, c->unk4, size, NULL);
        DrawSync(0);
        func_800AC67C(D_80067948 + m);
    }
    vp = db_lookupRegistered(4, 0x1C0000, 0);
    n = D_80067948;
    pack1 = db_lookup(n, 2, 0x1B0000, 0);
    pack2 = db_lookup(n, 2, 0x1B0001, 0);
    hdr = (u8 *)db_lookup(pack1, 2, 0x120000, 0) + 4;
    tp = (s16 *)(hdr + (*(u32 *)(hdr + 4) & 0xFFFFFF));
    uv = tp + hdr[7] * 2;
    ent = (FieldTexEnt *)(uv + hdr[7] * 2);
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, 2, (i & 0xFFFF) | 0x40000, 0);
        e1 = ent[i].unk0;
        if (e1 & 1) {
            x = vp->unk0;
            y = vp->unk2;
            if (vp->unk2 == 0x100) {
                vp->unk0 += 0x40;
                vp->unk2 = 0x180;
            } else {
                vp->unk2 -= 0x80;
            }
            if (vp->unk0 == vp->unk20) {
                vp->unk0 += 0x40;
            }
            cy = vp->unkA;
            cx = vp->unk8;
            vp->unkA += 2;
            tp[i * 2] = getTPage(1, 0, x, y);
            tp[i * 2 + 1] = getClut(cx, cy);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y % 256;
            ent[i].unk4 = x;
            ent[i].unk6 = y;
            file_setTimPos(img, cx, cy, x, y);
            file_loadTim(img);
        }
    }
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, 2, (i & 0xFFFF) | 0x40000, 0);
        e = ent[i].unk0;
        if (!(e & 1)) {
            x2 = vp->unk10;
            y2 = vp->unk12;
            if (vp->unk12 >= 0x1E0) {
                vp->unk10 -= 0x40;
                vp->unk12 = 0x100;
            } else {
                vp->unk12 += 0x20;
            }
            cy2 = vp->unk1A;
            cx2 = vp->unk18;
            vp->unk1A += 2;
            tp[i * 2] = getTPage(1, 0, x2, y2);
            tp[i * 2 + 1] = getClut(cx2, cy2);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y2 % 256 - ((e - 2) % 8 * 32 + 0x100) % 128;
            ent[i].unk0 = x2;
            ent[i].unk2 = y2;
            for (j = hdr[7] - 1; j >= 0; j--) {
                if (ent[j].unk0 == e - 1) {
                    ent[i].unk4 = ent[j].unk4;
                    ent[i].unk6 = ent[j].unk6;
                    break;
                }
            }
            file_setTimPos(img, cx2, cy2, x2, y2);
            file_loadTim(img);
        }
    }
    DrawSync(0);
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk28[g_gameState->unk1C->unk8E4.field->unkC->unk14->unk40] = pack1;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk40++;
    db_register(pack1, 0);
    D_80067948 = pack1 + db_getSize(pack1);
    return 0;
}

s32 func_800AAA54(void) {
    s32 i;

    for (i = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk40 - 1; i >= 0; i--) {
        db_unregister((void *)g_gameState->unk1C->unk8E4.field->unkC->unk14->unk28[i]);
    }
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk40 = 0;
    return 0;
}

s32 func_800AAB30(s32 arg0, void *arg1) {
    s32 *tex;
    s32 *uv;
    s32 *spr;
    s32 sprId;
    FieldObjSlot *s;
    s32 slot;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    Unk800AA39C *t;
    Unk800AA39CEntry *e;
    s32 i;
    u32 r;
    WmModel *a;

    sprId = -1;
    r = func_8004AB64(arg1);
    if (r < 9) {
        slot = r;
        s = (FieldObjSlot *)func_8001E350(r);
    } else {
        slot = g_gameState->unk1C->unk39 + 9;
        s = (FieldObjSlot *)func_8001E350(slot);
        g_gameState->unk1C->unk39++;
    }
    arg0 = func_800AAEC4(arg0);
    s->unk4 = arg0;
    s->unkC = arg1;
    s->unk8->unk8 = db_lookupRegistered(4, arg0, 0);
    if ((s->unk8->unkC = db_lookupRegistered(4, func_800AAF5C(arg0), 0)) != NULL) {
        a = s->unk8->unkC;
        if (IS_OFFSET(a->unkC)) {
            a->unkC = (s32)a + a->unkC;
            if (a->unk10 != NULL) {
                a->unk10 = (WmModelGroup *)((u8 *)s->unk8->unkC + (s32)a->unk10);
            }
        }
    }
    func_800AAFD4(arg0, &tex, &uv, &spr);
    m = s->unk8->unk8;
    g = m->unk10;
    sum = 0;
    if (IS_OFFSET(g)) {
        g = (WmModelGroup *)((u8 *)m + (s32)g);
    }
    for (k = 0; k < m->unk3; k++) {
        sum += g->unk0;
        g++;
    }
    sum *= 2;
    sum += s->unk8->unk8->unk2 * 32 + s->unk8->unk8->unk3 * 12;
    func_800B6438(s->unk8, (u8 *)g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20);
    func_800B6584(s->unk8, (WmTexInfo *)tex, (s16(*)[2])uv);
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20 += sum;
    func_800AC620();
    t = db_lookupRegistered(8, arg0, 0x120000);
    e = t->unk4;
    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            sprId = e->unk8 | 0x190000;
        }
    }
    if (sprId != -1) {
        s->unk8->unk50 = db_lookupRegistered(4, sprId, 0);
        func_800BB3C4((BgSprHdr *)s->unk8->unk50, (BgSprOfs *)spr);
    }
    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk18 != NULL) {
        if (slot < 9) {
            func_800C74A8(slot, s);
        } else {
            func_800C74F0(s);
        }
        func_800C3E18(slot, 1);
    }
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x10000;
    s->unk0 |= 1;
    return slot;
}

s32 func_800AAEC4(s32 arg0) {
    if ((arg0 & 0xFFFF) == 0xB9 && g_evtGlobalVars.h[0] >= 0x283C) {
        arg0 = 0x2004C;
    }
    return arg0;
}

s32 func_800AAEF8(s32 arg0) {
    FileTable *t = g_gameState->unk1C->depTable;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unkC == arg0) {
            return e[i].unk0;
        }
    }
    return -1;
}

s32 func_800AAF5C(s32 arg0) {
    Unk800AA39C *t = db_lookupRegistered(8, arg0, 0x120000);
    s32 i;
    Unk800AA39CEntry *e;

    e = t->unk4;
    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            return e->unk2 | 0x30000;
        }
    }
    return -1;
}

/* look up the three tables of an area entry */
s32 func_800AAFD4(s32 arg0, s32 **arg1, s32 **arg2, s32 **arg3) {
    Unk800AA39C *t = db_lookupRegistered(8, arg0, 0x120000);
    Unk800AA39CEntry *e = t->unk4;
    s32 i;

    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            *arg1 = (s32 *)((u8 *)e + e->unk4);
            *arg2 = *arg1 + e->unk7;
            *arg3 = *arg2 + e->unk7;
            break;
        }
        e++;
    }
    return 0;
}

s32 func_800AB0B0(Unk800C95C8 *arg0) {
    Unk800AB0B0 *t = arg0->unk50;
    s32 i;
    Unk800AB0B0Entry *e;

    if (t != NULL) {
        e = t->unk4;
        for (i = t->unk0 - 1; i >= 0; i--) {
            if (e[i].unk0 & 1) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800AB110(FieldObjSlot **arg0) {
    s32 n;
    Unk16D8 *e;
    FieldObjSlot *s;
    s32 i;
    WmModel *m;
    WmModelGroup *g;
    u32 sum;
    u32 k;
    Unk800C95C8 *o;
    Unk8004AB64 *a;
    u16 *p;

    n = 0;
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)func_8001E350(a->unk3B);
            if (s->unk8->unk1C == NULL) {
                arg0[n++] = s;
            }
        }
    }
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)func_8001E350(a->unk3B);
            p = s->unk8->unk1C;
            if (p != NULL) {
                for (i = 0; i < n; i++) {
                    if ((u16 *)arg0[i]->unk8 == p) {
                        arg0[n++] = s;
                        break;
                    }
                }
            }
        }
    }
    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 != 0) {
        for (i = 0; i < n; i++) {
            o = arg0[i]->unk8;
            m = o->unk8;
            g = m->unk10;
            sum = 0;
            if (IS_OFFSET(g)) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->unk3; k++) {
                sum += g->unk0;
                g++;
            }
            if (o->unk10 + (sum * 2 + (o->unk8->unk2 * 32 + o->unk8->unk3 * 12)) >= D_80067944 + 0xF1700) {
                arg0[i] = NULL;
            }
        }
    }
    return n;
}

void func_800AB3CC(void) {
    /* block scope: func_800AC02C calls it undeclared (int argument) */
    void func_800B61A4(Unk800C95C8 *, u8);
    FieldEvtCtx *list[16];
    SVECTOR rot;
    DVECTOR sxy;
    s32 ofs[2];
    s16 ax;
    s16 az;
    s32 z;
    u32 *ot;
    s32 n;
    s32 i;
    s32 j;
    s32 j1;
    s32 cnt;
    s32 lim;
    s32 f;
    Unk800C95C8 *m;
    FieldEvtObj *o;
    FieldEvtCtx *c;
    WmModel *w;

    lim = 100;
    j = 0;
    if (D_800C9D44 & 0x40) {
        mbg_getGeomOffset(&ofs[0], &ofs[1]);
        SetGeomOffset(ofs[0], ofs[1]);
    } else {
        func_800C0F08();
    }
    i = 0;
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];
    g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] = ot + g_gameState->unk1C->unk8E4.field->unkC->unk14->unk30;
    n = func_800AB110((FieldObjSlot **)list);
    for (; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if ((c->unk0 & 2) && func_800C768C(o->unk3B, &ax, &az)) {
                rot.vx = ax;
                rot.vy = o->unk22;
                rot.vz = az;
                func_80030EA4(&rot, &m->unk24);
            } else {
                rot.vx = o->unk20;
                rot.vy = (o->unk22 + j) & 0xFFF;
                rot.vz = o->unk24;
                func_80031244(&rot, &m->unk24);
            }
            m->unk24.t[0] = o->unk14.vx;
            m->unk24.t[1] = o->unk14.vy;
            m->unk24.t[2] = o->unk14.vz;
            func_800B7CEC(m);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            o = c->unkC;
            m = c->unk8;
            w = db_lookupRegistered(4, o->unk36 | 0x30000, 0);
            m->unkC = w;
            if (w != NULL && IS_OFFSET(w->unkC)) {
                w->unkC = (void *)((u8 *)w + (s32)w->unkC);
                if (w->unk10 != NULL) {
                    w->unk10 = (void *)((u8 *)m->unkC + (s32)w->unk10);
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if (m->unk0 & 1) {
                func_800BA560(m, o->unk3A);
            } else {
                func_800B7F14(m, o->unk3A);
            }
        }
    }
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            rot.vx = ((Unk8004AB64 *)m->unk20)->pos.vx;
            rot.vy = ((Unk8004AB64 *)m->unk20)->pos.vy;
            rot.vz = ((Unk8004AB64 *)m->unk20)->pos.vz;
            gte_ldv0(&rot);
            gte_rtps();
            gte_stsxy(&sxy);
            gte_stsz(&z);
            z = z / 4 + g_gameState->unk1C->unk8E4.field->unkC->unk14->unk30;
            if (z < lim || z > 0x1000 - lim) {
                list[i] = NULL;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            f = c->unk0 & 0x100000;
            lim = m->unk8->unk3;
            SetSpadStack12(0x1F8003FC);
            if (f) {
                for (j = 0; j < lim; j++) {
                    f = 1 << j;
                    if (!(m->unk14 & (u32)f)) {
                        func_800BB508(m, j);
                        func_800BB714(m, j);
                    }
                }
            } else {
                cnt = lim;
                for (j1 = 0; j1 < cnt; j1++) {
                    if (!(m->unk14 & (u32)(1 << j1))) {
                        func_800B61A4(m, j1);
                        func_800B5958(m, j1);
                    }
                }
            }
            ResetSpadStack12();
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800AC6C0(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk2C, c->unkC->unk3B);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL && !(c->unk0 & 0x1000)) {
            m = c->unk8;
            if (m->unk50->unk0 >= 2 && func_800AB0B0(m) == 0) {
                m->unk50->unk4[2].unk0 |= 1;
                m->unk50->unk4[2].unk8 = 0;
                m->unk50->unk4[2].unkE = 0x1000;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800BB090((BgAnimHdr *)((Unk800C95C8 *)c->unk8)->unk50);
        }
    }
    func_800B2D10(list, n);
    if (!(g_gameState->unk1C->unk0 & 2)) {
        func_800B1E8C(list, n);
    }
    func_800AC02C(list, n);
    g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] = ot;
    if (!(D_800C9D44 & 0x40)) {
        func_800C0FB0();
    }
}

/* object command arg1 for object arg0 (palette, mirror, triggers) */
s32 func_800ABA98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FieldActorInfo *info;
    FieldEvtCtx *c;
    FieldActorExt *x;
    FieldActorExt *y;
    s32 *tex;
    s32 *uv;
    s32 *spr;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    s32 id;
    FieldTrig *t;

    info = &g_gameState->unk1C->unk8E4.field->unkC->unk14->unk54[arg0];
    c = (FieldEvtCtx *)func_8001E350(arg0);
    switch (arg1) {
    case 0:
        if ((u8)arg2 == 0xFF) {
            c->unk0 &= ~0x10000;
        } else {
            if (arg2 != (s8)info->unk5) {
                c->unk0 |= 0x40000;
            }
            c->unk0 |= 0x10000;
        }
        info->unk5 = arg2;
        break;
    case 4:
        if (arg2 != 0) {
            ((Unk800C95C8 *)c->unk8)->unk5A = arg3;
            c->unk0 |= 0x100000;
        } else {
            c->unk0 &= ~0x100000;
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        x = info->unk10;
        if (x == NULL) {
            x = info->unk10 = (FieldActorExt *)g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20;
            x->unk0 = 0;
            x->unk4 = c->unk4;
            x->unk8 = (Unk800C95C8 *)x->unk14;
            x->unkC = (FieldEvtObj *)x->unkAC;
            x->unk10 = 0;
            x->unkF8 = c;
            x->unkFC.v.vx = 0;
            x->unkFC.v.vy = 0;
            x->unkFC.v.vz = 0;
            x->unk104.v.vx = 0;
            x->unk104.v.vy = 0x1000;
            x->unk104.v.vz = 0;
            x->unk10C.w = 0;
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20 += sizeof(FieldActorExt);
        }
        y = x;
        if (!(x->unk0 & 1)) {
            *y->unkC = *c->unkC;
            y->unk8->unk8 = ((Unk800C95C8 *)c->unk8)->unk8;
            y->unk8->unkC = ((Unk800C95C8 *)c->unk8)->unkC;
            func_800AAFD4(y->unk4, &tex, &uv, &spr);
            m = y->unk8->unk8;
            g = m->unk10;
            sum = 0;
            if (IS_OFFSET(g)) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->unk3; k++) {
                sum += g->unk0;
                g++;
            }
            sum *= 2;
            sum += y->unk8->unk8->unk2 * 32 + y->unk8->unk8->unk3 * 12;
            func_800B6438(y->unk8, (u8 *)g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20);
            func_800B6584(y->unk8, (WmTexInfo *)tex, (s16(*)[2])uv);
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20 += sum;
            id = (s32)db_lookupRegistered(9, y->unk4, 0x190000);
            if (id != -1) {
                y->unk8->unk50 = db_lookupRegistered(4, id, 0);
                func_800BB3C4((BgSprHdr *)y->unk8->unk50, (BgSprOfs *)spr);
            }
            func_800AC620();
            x->unk0 |= 0x02000001;
        }
        switch (arg1) {
        case 8:
            if (arg2 != 0) {
                y->unk8->unk60 = (Unk800C95C8 *)c->unk8;
                y->unk8->unk64.w[0] = x->unk104.w[0];
                y->unk8->unk64.w[1] = x->unk104.w[1];
                y->unk8->unk6C.w[0] = x->unkFC.w[0];
                y->unk8->unk6C.w[1] = x->unkFC.w[1];
                x->unk0 |= 0x01000000;
            } else {
                x->unk0 &= ~0x01000000;
            }
            break;
        case 9:
            x->unkFC.v.vx = arg2;
            x->unkFC.v.vy = arg3;
            x->unkFC.v.vz = arg4;
            break;
        case 10:
            x->unk104.v.vx = arg2;
            x->unk104.v.vy = arg3;
            x->unk104.v.vz = arg4;
            break;
        case 11:
            x->unk10C.c[0] = arg2;
            x->unk10C.c[1] = arg3;
            x->unk10C.c[2] = arg4;
            x->unk10C.c[3] = 2;
            break;
        }
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        t = func_800B2100(c, arg2, arg3);
        if (t == NULL && arg1 != 19) {
            t = func_800B21A0(c, arg2, arg3);
        }
        switch (arg1) {
        case 16:
            t->unk8[0] = arg4;
            break;
        case 17:
            t->unk8[1] = arg4;
            break;
        case 18:
            t->unk1 = arg4 != 0;
            break;
        case 19:
            func_800B2218(c, arg2, arg3);
            break;
        }
        break;
    }
    return 0;
}

void func_800AC02C(FieldEvtCtx **arg0, s32 arg1) {
    s32 i;
    s32 j;
    FieldEvtCtx *c;
    FieldActorInfo *info;
    FieldActorExt *x;
    u32 vis;
    u32 chg;
    u32 f4;
    u32 f2;
    s32 upd;
    s32 n;
    FieldActorExt *y;

    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        upd = 0;
        info = &g_gameState->unk1C->unk8E4.field->unkC->unk14->unk54[c->unkC->unk3B];
        vis = c->unk0 & 0x10000;
        chg = (info->unk0 & 0x10000) ^ vis;
        f4 = c->unk0 & 0x40000;
        f2 = c->unk0 & 0x20000;
        if (chg || f4 || f2) {
            upd = 1;
        }
        if (upd) {
            if (c->unk0 & 0x10000) {
                func_800BA190(c->unk8, 1);
            } else {
                func_800BA190(c->unk8, 0);
            }
            if (vis) {
                func_800BA3CC(c->unk8, ((s32)(info->unk5 << 24) >> 19) & 0xFFE0);
            }
            if (c->unk0 & 0x20000) {
                c->unk0 &= ~0x20000;
            }
            c->unk0 &= ~0x40000;
            if (chg || f4) {
                c->unk0 |= 0x20000;
            }
            info->unk0 = (info->unk0 & ~0x10000) | vis;
        }
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        info = &g_gameState->unk1C->unk8E4.field->unkC->unk14->unk54[c->unkC->unk3B];
        x = info->unk10;
        if (x != NULL && (x->unk0 & 0x01000000)) {
            func_800BC33C(x->unk8);
            y = x;
            func_800B7F14(x->unk8, x->unkF8->unkC->unk3A);
            n = x->unk8->unk8->unk3;
            for (j = 0; j < n; j++) {
                func_800B61A4(y->unk8, j & 0xFF);
                func_800BC600(y->unk8, j & 0xFF);
            }
            if (x->unk10C.c[3] != 0) {
                for (j = n - 1; j >= 0; j--) {
                    func_800B9E4C(y->unk8, j & 0xFF, x->unk10C.c[0], x->unk10C.c[1], x->unk10C.c[2]);
                }
                x->unk10C.c[3]--;
            }
        }
    }
}

Unk800C9CCC *func_800AC344(s32 arg0) {
    u32 id;
    s32 i;

    id = (u16)arg0;
    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk2 == id) {
            return &D_800C9CCC[i];
        }
    }
    return NULL;
}

u32 func_800AC380(Unk800C9CCC **arg0) {
    u32 min;
    s32 i;

    min = 0x801EDF00;
    if (arg0 != NULL) {
        *arg0 = NULL;
    }
    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk4 != 0 && D_800C9CCC[i].unk4 < min) {
            min = D_800C9CCC[i].unk4;
            if (arg0 != NULL) {
                *arg0 = &D_800C9CCC[i];
            }
        }
    }
    return min;
}

s32 func_800AC3D8(s32 arg0, s32 arg1) {
    Unk800C9CCC *e;
    u32 lo;
    u32 min;
    u32 a;

    e = func_800AC344(arg0);
    min = func_800AC380(NULL);
    lo = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20;
    if (lo == 0) {
        lo = D_80067948;
    }
    a = min - arg1;
    if (lo < a) {
        e->unk4 = a;
        e->unk8 = arg1;
        return 1;
    }
    return 0;
}

/* drop the cached textures no longer listed and pack the rest at the top of RAM */
void func_800AC474(void) {
    u32 top;
    Unk800C9CCC *s;
    s32 i;
    s32 j;
    s32 found;
    s32 bi;
    u32 end;
    u32 best;
    u32 addr;
    s32 id;
    u32 sz;
    Unk800C9CCC *t;

    top = 0x801EDF00;
    for (i = 9; i >= 0; i--) {
        s = &D_800C9CCC[i];
        id = s->unk2;
        if (s->unk4 != 0) {
            found = 0;
            for (j = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk60 - 1; j >= 0; j--) {
                if ((u16)D_800C9F20[j] == id) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                s->unk4 = 0;
                s->unk8 = 0;
            }
        }
    }
    for (;;) {
        best = 0;
        bi = -1;
        for (i = 9; i >= 0; i--) {
            t = &D_800C9CCC[i];
            sz = t->unk8;
            end = t->unk4 + sz;
            if (t->unk4 != 0 && end <= top && best < end) {
                best = end;
                bi = i;
            }
        }
        if (bi == -1) {
            break;
        }
        s = &D_800C9CCC[bi];
        addr = top - s->unk8;
        if (s->unk4 != addr) {
            file_copyViaVram(addr, s->unk4, s->unk8, NULL);
            s->unk4 = addr;
        }
        top = addr;
    }
    DrawSync(0);
}

void func_800AC5F0(void) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        D_800C9CCC[i].unk0 = 0;
        D_800C9CCC[i].unk4 = 0;
        D_800C9CCC[i].unk8 = 0;
    }
}

void func_800AC620(void) {
    s32 v = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20;

    if (v == 0) {
        v = D_80067948;
    }
    func_800AC67C(v);
}

void func_800AC67C(s32 arg0) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk4 != 0 && (u32)arg0 >= D_800C9CCC[i].unk4) {
            D_800C9CCC[i].unk8 = 0;
            D_800C9CCC[i].unk4 = 0;
        }
    }
}

void func_800AC6C0(FieldObjPlace *arg0, s32 arg1) {
    MATRIX m;
    POLY_FT4 *p[4];
    SVECTOR v;
    s32 z[9];
    s32 bias;
    FieldObjPlace *e;
    FieldObjSlot *s;
    void *lt;
    Unk800B3294Bias *t;
    u32 *mask;
    u32 *mk;
    s32 i;
    s32 k;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    e = &arg0[arg1];
    s = (FieldObjSlot *)func_8001E350(arg1);
    bias = 0;
    if (s->unk0 & 0x10) {
        return;
    }
    lt = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk1C;
    if (lt != NULL) {
        t = (Unk800B3294Bias *)func_800B3294(lt, s->unk4);
        if (t == NULL) {
            t = (Unk800B3294Bias *)func_800B3294(lt, -1);
        }
        bias = t->unk7;
    }
    func_800AD488(&e->unk0, s, &m);
    u0 = (s16)(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk50 % 64) * 2;
    v0 = (s16)(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk52 % 256);
    for (i = 0; i < 4; i++) {
        mask = NULL;
        if (g_gameState->unk1C->unk8E4.field->unk0 & 0x200) {
            mask = e->unk8[g_gameState->unk1C->unk8][i];
            setlen(mask, 2);
            mask[1] = 0xE6000002;
            mask[2] = 0;
            p[i] = (POLY_FT4 *)(mask + 3);
        } else {
            p[i] = e->unk8[g_gameState->unk1C->unk8][i];
        }
        setlen(p[i], 9);
        setcode(p[i], 0x2C);
        setSemiTrans(p[i], 1);
        setRGB0(p[i], e->unk0.unk0, e->unk0.unk0, e->unk0.unk0);
        p[i]->tpage = getTPage(1, 2, g_gameState->unk1C->unk8E4.field->unkC->unk14->unk50,
                               g_gameState->unk1C->unk8E4.field->unkC->unk14->unk52);
        p[i]->clut = 0x3FD0;
        switch (i) {
        case 0:
            setUV4(p[0], u0, v0, u0 + 31, v0, u0, v0 + 31, u0 + 31, v0 + 31);
            break;
        case 1:
            setUV4(p[1], u0 + 31, v0, u0, v0, u0 + 31, v0 + 31, u0, v0 + 31);
            break;
        case 2:
            setUV4(p[2], u0, v0 + 31, u0 + 31, v0 + 31, u0, v0, u0 + 31, v0);
            break;
        case 3:
            setUV4(p[3], u0 + 31, v0 + 31, u0, v0 + 31, u0 + 31, v0, u0, v0);
            break;
        }
        if (g_gameState->unk1C->unk8E4.field->unk0 & 0x200) {
            mk = (u32 *)(p[i] + 1);
            setlen(mk, 2);
            mk[1] = 0xE6000002;
            mk[2] = 0;
            MargePrim(p[i], mk);
            MargePrim(mask, p[i]);
        }
    }
    gte_MulMatrix0(&g_gameState->unk1C->unk18, &m, &m);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    FIELD_SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&p[0]->x0);
    gte_stsz(&z[0]);
    FIELD_SHADOW_PT(0, 0x1000);
    gte_stsxy(&p[0]->x1);
    gte_stsxy(&p[1]->x0);
    gte_stsz(&z[1]);
    FIELD_SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&p[1]->x1);
    gte_stsz(&z[2]);
    FIELD_SHADOW_PT(-0x1000, 0);
    gte_stsxy(&p[0]->x2);
    gte_stsxy(&p[2]->x0);
    gte_stsz(&z[3]);
    FIELD_SHADOW_PT(0, 0);
    gte_stsxy(&p[0]->x3);
    gte_stsxy(&p[1]->x2);
    gte_stsxy(&p[2]->x1);
    gte_stsxy(&p[3]->x0);
    gte_stsz(&z[4]);
    FIELD_SHADOW_PT(0x1000, 0);
    gte_stsxy(&p[1]->x3);
    gte_stsxy(&p[3]->x1);
    gte_stsz(&z[5]);
    FIELD_SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&p[2]->x2);
    gte_stsz(&z[6]);
    FIELD_SHADOW_PT(0, -0x1000);
    gte_stsxy(&p[2]->x3);
    gte_stsxy(&p[3]->x2);
    gte_stsz(&z[7]);
    FIELD_SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&p[3]->x3);
    gte_stsz(&z[8]);

    k = z[0];
    if (k < z[1]) {
        k = z[1];
    }
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[4]) {
        k = z[4];
    }
    FIELD_SHADOW_ADD(k, 0);
    k = z[1];
    if (k < z[2]) {
        k = z[2];
    }
    if (k < z[4]) {
        k = z[4];
    }
    if (k < z[5]) {
        k = z[5];
    }
    FIELD_SHADOW_ADD(k, 1);
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    FIELD_SHADOW_ADD(k, 2);
    k = z[4];
    if (k < z[5]) {
        k = z[5];
    }
    if (k < z[7]) {
        k = z[7];
    }
    if (k < z[8]) {
        k = z[8];
    }
    FIELD_SHADOW_ADD(k, 3);
}

s32 func_800AD44C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_80062E3C(arg0, 0x100, 0xFF, arg2, arg3, arg1, 3);
}

/* object placement matrix: either a plain y rotation or facing the model's joint */
void func_800AD488(FieldObjEnt *arg0, FieldObjSlot *arg1, MATRIX *arg2) {
    MATRIX m;
    SVECTOR v;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;
    s32 *d;
    s32 *s;
    s32 c;
    s32 sn;

    if (arg1->unk0 & 0x20) {
        c = math_cos(arg0->unk4);
        sn = math_sin(arg0->unk4);
        arg2->m[0][0] = c * arg0->unk1 >> 12;
        arg2->m[1][0] = 0;
        arg2->m[2][0] = -sn * arg0->unk1 >> 12;
        arg2->m[0][1] = 0;
        arg2->m[1][1] = 0x1000;
        arg2->m[2][1] = 0;
        arg2->m[0][2] = sn * arg0->unk2 >> 12;
        arg2->m[1][2] = 0;
        arg2->m[2][2] = c * arg0->unk2 >> 12;
        arg2->t[0] = ((c * arg0->unk4_12 + sn * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->pos.vx;
        arg2->t[1] = ((Unk8004AB64 *)arg1->unkC)->pos.vy;
        arg2->t[2] = ((-sn * arg0->unk4_12 + c * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->pos.vz;
    } else {
        /* word-wise copy of the joint matrix */
        d = (s32 *)&m;
        s = (s32 *)&((MATRIX *)arg1->unk8->unk20)[1];
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
        d[3] = s[3];
        d[4] = s[4];
        d[5] = s[5];
        d[6] = s[6];
        d[7] = s[7];
        m.t[2] = 0;
        m.t[1] = 0;
        m.t[0] = 0;
        v.vx = 0;
        v.vy = -0x1000;
        v.vz = 0;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&dir);
        dir.vy = 0;
        VectorNormal(&dir, &fwd);
        up.vy = 0x1000;
        up.vx = 0;
        up.vz = 0;
        fwd.vx = -fwd.vx;
        fwd.vy = -fwd.vy;
        fwd.vz = -fwd.vz;
        gte_ldopv1(&up);
        gte_ldopv2(&fwd);
        gte_op12();
        gte_stlvnl(&side);
        arg2->m[0][0] = side.vx * arg0->unk1 >> 12;
        arg2->m[1][0] = side.vy * arg0->unk1 >> 12;
        arg2->m[2][0] = side.vz * arg0->unk1 >> 12;
        arg2->m[0][1] = up.vx;
        arg2->m[1][1] = up.vy;
        arg2->m[2][1] = up.vz;
        arg2->m[0][2] = fwd.vx * arg0->unk2 >> 12;
        arg2->m[1][2] = fwd.vy * arg0->unk2 >> 12;
        arg2->m[2][2] = fwd.vz * arg0->unk2 >> 12;
        arg2->t[0] = ((side.vx * arg0->unk4_12 + fwd.vx * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[1].t[0];
        arg2->t[1] = ((side.vy * arg0->unk4_12 + fwd.vy * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->pos.vy;
        arg2->t[2] = ((side.vz * arg0->unk4_12 + fwd.vz * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[1].t[2];
    }
}

/* field request dispatcher */
s32 fld_movieCommand(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg0 & 0xF) {
    case 0:
        fld_movieStart((arg1 >> 8) & 0xFF, arg1 & 0xFF, arg2);
        return 0;
    case 1:
        func_800AE4F8();
        return 0;
    case 2:
        D_800C9D44 |= 1;
        break;
    case 3:
        return fld_movieCommand_state();
    case 4:
        return mbg_getFrame();
    case 5:
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43 = arg1;
        mbg_setRgb24(arg1);
        break;
    }
    return 0;
}

void fld_movieStart(s8 arg0, s8 arg1, s8 arg2) {
    D_800C9D44 = 0;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk48 = arg0;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk49 = arg1;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43 = arg2;
    func_800AC5F0();
    preload_reset();
    g_gameState->unk1C->unk8E4.field->unk0 |= 0x1000;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 1;
}

void fld_movieUpdate(void) {
    s32 b;
    s32 done;
    s32 t;
    s32 d;

    switch (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42) {
    case 1:
        if (cd_getState() == 0) {
            if (cdRead_update(g_gameState->unk1C->unk854) == 0) {
                FileEntry *f = file_find(0, 0x64);
                cdRead_init(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11,
                              (void *)(D_80067944 + 0xF1700), 0, 0x15000, NULL);
            }
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 2;
        }
        break;
    case 2:
        if (cdRead_update(g_gameState->unk1C->unk854) == 0) {
            D_800C9D44 |= 8;
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 3;
        }
        break;
    case 3:
        if (!(D_800C9D44 & 8)) {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk44 = cd_findFile(
                g_mbgMovieLists[g_gameState->unk1C->unk8E4.field->unkC->unk14->unk48]
                    .unk4[g_gameState->unk1C->unk8E4.field->unkC->unk14->unk49].unk0);
            if (cd_getState() == 0) {
                cd_seekStreamAsync(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk44);
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 4;
            }
        }
        break;
    case 4:
        if (cd_getState() == 0) {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 5;
        }
        break;
    case 5:
        if (D_800C9D44 & 1) {
            b = 1;
            if (g_gameState->unk1C->unk8 != 0) {
                b = 2;
            }
            D_800C9EF0.unk10[0][b - 1] = 0x600;
            D_800C9EF0.unk0[0][b - 1] = 0x19;
            D_800C9EF0.unk10[0][b - 1] |= 0x800;
            if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43 != 0) {
                D_800C9EF0.unk0[0][b - 1] |= 0x44;
                D_800C9EF0.unk10[0][b - 1] |= 1;
            }
            D_800C9EF0.unk20[0][b - 1] = 0x40;
            D_800C9EF0.unk20[1][b] = 0x40;
            D_800C9EF0.unk20[0][b] = 2;
            if (!(g_padPortMask[0] & 0x04000000)) {
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x1000;
                g_padPortMask[0] |= 0x04000000;
            }
            g_gameState->vsyncMode = 4;
            func_80056248(0);
            func_80056228();
            DrawSync(0);
            MBG_init(1, &g_gameState->unk1C->unk8E4.field->unkC->unk14->unk34);
            mbg_setRgb24(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43);
            mbg_setEndCallback(func_800AE4F8);
            D_800C9D46 = g_mbgFrame;
            func_8019AB98(2, 4, g_gameState->unk1C->unk8E4.field->unkC->unk14->unk5D);
            mbg_play(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk48,
                          g_gameState->unk1C->unk8E4.field->unkC->unk14->unk49);
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 6;
        }
        break;
    case 6:
        if (D_800C9D44 & 0x40) {
            mbg_getStartCamera((void *)&g_gameState->unk1C->unk18, &g_gameState->unk1C->unkA);
        }
        if ((D_800C9D44 & 2) && (func_80012AD4() & 2)) {
            done = mbg_update(0);
            t = mbg_getLastFrame();
            if (!(D_800C9D44 & 0x20) && (u32)t < 99999) {
                d = t - g_mbgFrame;
                if (d < 4) {
                    func_80056278(d * 4, 0);
                    D_800C9D44 |= 0x20;
                }
            }
            if (done != 0) {
                setlen(&D_800C9F6C[g_gameState->unk1C->unk8 ^ 1], 2);
                D_800C9F6C[g_gameState->unk1C->unk8 ^ 1].code[0] = 0xE6000002;
                D_800C9F6C[g_gameState->unk1C->unk8 ^ 1].code[1] = 0;
                addPrim(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1],
                        &D_800C9F6C[g_gameState->unk1C->unk8 ^ 1]);
                DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1]);
            } else if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43 != 0 && g_gameState->unk1C->unk8 == 0) {
                g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
            }
            if ((g_mbgFrame >= t || D_800C9D46 >= g_mbgFrame) && !(D_800C9D44 & 0x100)) {
                if (D_800C9D46 >= g_mbgFrame) {
                    ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
                    g_gameState->unk1C->unk0 &= ~0x1D;
                    g_gameState->unk1C->unk8E4.field->unk0 &= ~0xE01;
                    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk43 != 0 && g_gameState->unk1C->unk8 == 0) {
                        g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                    }
                    printf("Fr: %d\n", D_800C9D46);
                } else {
                    D_800C9EF0.unk0[1][0] = 0x1D;
                    D_800C9EF0.unk10[1][0] = 0xE01;
                }
                D_800C9D44 |= 0x100;
            }
            D_800C9D46 = g_mbgFrame;
        }
        break;
    case 7:
        if ((D_800C9D44 & 4) && cd_getState() == 0) {
            func_800AE5D4();
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 8;
        }
        break;
    }
}

void func_800AE4F8(void) {
    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x1000) {
        g_padPortMask[0] &= ~0x04000000;
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x1000;
    }
    mbg_shutdown();
    cd_cancel();
    D_800C9D44 |= 4;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 = 7;
}

void func_800AE5D4(void) {
    FieldMap *f;

    g_gameState->unk1C->unk0 &= ~0x5D;
    f = g_gameState->unk1C->unk8E4.field;
    D_800C9D44 &= 0xFE00;
    f->unk0 &= ~0x1E01;
    g_gameState->vsyncMode = 2;
    g_gameState->unkC[0].disp.x = 0x140;
    g_gameState->unkC[1].disp.x = 0;
    func_800B0124(0x6201, -1, g_gameState->unk1C->unk8E4.field->unkC->unk14->unk5C, 0, 0);
}

s32 func_800AE68C(void) {
    if ((u32)(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk42 - 6) < 2 && (D_800C9D44 & 2)
        && (func_80012AD4() & 2)) {
        return 1;
    }
    return 0;
}

void func_800AE710(u16 arg0, s32 arg1) {
    fld_movieCommand(0, arg0, arg1);
}

void func_800AE738(void) {
    fld_movieCommand(2, 0, 0);
}

void func_800AE760(void) {
    fld_movieCommand(1, 0, 0);
}

void func_800AE788(void) {
    if (g_gameState->unk1C->unk8E4.field->unk0 & 0x200) {
        ClearOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
        setlen(&D_800C9F60[g_gameState->unk1C->unk8 ^ 1], 2);
        D_800C9F60[g_gameState->unk1C->unk8 ^ 1].code[0] = 0xE6000000;
        D_800C9F60[g_gameState->unk1C->unk8 ^ 1].code[1] = 0;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1][0xFFF], &D_800C9F60[g_gameState->unk1C->unk8 ^ 1]);
    } else {
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
    }
    func_8004C204();
}

/* end of frame: flip buffers, draw and apply the queued flag changes */
void func_800AE930(void) {
    s32 i;
    s32 j;
    u32 f;

    if ((g_padPortHeld[0] & 0xF09) == 0xF09 || (g_padPortHeld[1] & 0xF09) == 0xF09) {
        g_gameState->unk0 |= 1;
    }
    DrawSync(0);
    if (!(func_80012AD4() & 2)) {
        if ((g_gameState->unk1C->unk0 & 0x1C) == 0x1C && (g_gameState->unk1C->unk8E4.field->unk0 & 4)) {
            g_gameState->vsyncMode = 0;
        } else {
            g_gameState->vsyncMode = 2;
        }
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    if (D_800C9D44 & 8) {
        EnterCriticalSection();
        FlushCache();
        ExitCriticalSection();
        D_800C9D44 &= ~8;
    }
    if (!(g_gameState->unk1C->unk8E4.field->unk0 & 0x20)) {
        PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    }
    if (!(g_gameState->unk1C->unk0 & 0x100)) {
        f = g_gameState->unk1C->unk8E4.field->unk0;
        if (!(f & 0x40)) {
            if (f & 0x400) {
                PutDrawEnv(&g_gameState->unk10[g_gameState->unk1C->unk8]);
            } else if (f & 4) {
                DrawOTagEnv(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][0xFFF], &g_gameState->unk10[g_gameState->unk1C->unk8]);
            } else {
                DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8]);
            }
        }
    }
    if (!(g_gameState->unk1C->unk0 & 0x20)) {
        g_gameState->unk1C->unk8 ^= 1;
        func_8002C0DC(g_gameState->unk1C->unk8);
    }
    if (!(g_gameState->unk1C->unk0 & 0x100)) {
        g_gameState->unk1C->unk0 |= D_800C9EF0.unk0[0][0];
        g_gameState->unk1C->unk0 &= ~D_800C9EF0.unk0[1][0];
        g_gameState->unk1C->unk8E4.field->unk0 |= D_800C9EF0.unk10[0][0];
        g_gameState->unk1C->unk8E4.field->unk0 &= ~D_800C9EF0.unk10[1][0];
        D_800C9D44 |= D_800C9EF0.unk20[0][0];
        D_800C9D44 &= ~D_800C9EF0.unk20[1][0];
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                D_800C9EF0.unk0[i][j] = D_800C9EF0.unk0[i][j + 1];
                D_800C9EF0.unk10[i][j] = D_800C9EF0.unk10[i][j + 1];
                D_800C9EF0.unk20[i][j] = D_800C9EF0.unk20[i][j + 1];
            }
            D_800C9EF0.unk0[i][3] = 0;
            D_800C9EF0.unk10[i][3] = 0;
            D_800C9EF0.unk20[i][3] = 0;
        }
    }
}

void func_800AED04(void) {
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk41 = 0;
    D_800C9EF0.unk10[1][3] = 4;
    D_800C9EF0.unk0[0][0] = 0x43;
    D_800C9EF0.unk0[0][2] = 0x3C;
    D_800C9EF0.unk10[0][2] = 0x1919;
    g_padPortMask[0] |= 0x04000000;
}

void func_800AED6C(void) {
    Unk8004AB64 *a;
    SVECTOR v;
    RECT r;
    PreloadEntry *e;
    s32 i;
    s16 w;

    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk41 == 0) {
        D_8007BD28 = 0;
        D_8007BD30 = D_80067944 + 0x153F00;
        func_800AC5F0();
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk20 = D_80067948;
        a = func_8004AE00();
        if (a != NULL) {
            func_800C0F08();
            v.vx = a->pos.vx;
            v.vy = a->pos.vy;
            v.vz = a->pos.vz;
            gte_SetRotMatrix(&g_gameState->unk1C->unk18);
            gte_SetTransMatrix(&g_gameState->unk1C->unk18);
            gte_ldv0(&v);
            gte_rtps();
            gte_stsxy(&g_gameState->unk1C->unk8E4.field->unkC->unk14->unk38);
            if ((u16)g_gameState->unk1C->unk8E4.field->unkC->unk14->unk38 >= 320
                || g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3A < 0
                || g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3A >= 224) {
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk38 = 160;
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3A = 112;
            }
            func_800C0FB0();
        }
        func_800B238C();
        if (g_gameState->unk1C->unk81C[0] != g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C
            && (g_gameState->unk1C->unk0 & 0x20000000)) {
            func_800559FC(0, 0xF, 0);
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 = 7;
        }
        if (g_gameState->unk1C->unk830[0] != 0xFFFF && (g_gameState->unk1C->unk0 & 0x02000000)) {
            func_800557A8(g_gameState->unk1C->unk828, 0x100000, 0xF, 0);
        }
        if (g_gameState->unk1C->unk830[1] != 0xFFFF && (g_gameState->unk1C->unk0 & 0x08000000)) {
            func_800557A8(g_gameState->unk1C->unk82C, 0x200000, 0xF, 0);
        }
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].unk1 = 0;
            g_padActuator[i].unk0 = 0;
        }
        func_80056228();
        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk41++;
    } else {
        if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 2)) {
            if (preload_isReady(0x1000B) != 0) {
                e = preload_find(0x1000B);
                ClearOTag(&D_800C9F98, 1);
                setlen(&D_800C9F60[g_gameState->unk1C->unk8 ^ 1], 2);
                D_800C9F60[g_gameState->unk1C->unk8 ^ 1].code[0] = 0xE6000000;
                D_800C9F60[g_gameState->unk1C->unk8 ^ 1].code[1] = 0;
                addPrim(&D_800C9F98, &D_800C9F60[g_gameState->unk1C->unk8]);
                DrawOTag(&D_800C9F98);
                r.x = 0;
                r.y = 0x100;
                r.h = 0x100;
                r.w = w = (u32)(e->unkC / r.h) >> 1;
                if (e->unkC % r.h != 0) {
                    r.w = w + 1;
                }
                LoadImage(&r, (u32 *)e->unk8);
                DrawSync(0);
                preload_setVramRect(e, r.x, r.y, r.w, r.h);
            } else {
                preload_remove(preload_waitCurrent()->unk4);
            }
            preload_discardRange((s32)D_80067944, 0x156F00);
            func_800B0124(0xD000, 0x27C, 0, 0x80, 0x7F);
            func_800B0124(0xD000, 0x27B, 0, 0x80, 0x7F);
            func_800B0124(0xD000, 0x27A, 0, 0x80, 0x7F);
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 2;
        }
        if (!(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 1)
            && func_80053A10(g_gameState->unk1C->unk8E4.field->unkC->unk14->unk38,
                             g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3A)) {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 |= 1;
        }
        if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 > 0) {
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9--;
        } else if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 == 0) {
            if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C != 0xFFFF
                && g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C != g_gameState->unk1C->unk81C[0]) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800B0124(0x800, g_gameState->unk1C->unk81C[0], 0, 0, 0);
                }
                func_800B0124(0x700, g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C, 0, 0, 0);
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 = -1;
            } else {
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 = -2;
            }
        } else if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 == -1) {
            if (func_800B0124(0xC00, -1, 0, 0, 0) == 0) {
                g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 = -2;
            }
        } else if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 == -2) {
            func_800B1C4C();
            if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C != 0xFFFF
                && !(g_gameState->unk1C->unk0 & 0x20000000)) {
                func_800B0124(0, g_gameState->unk1C->unk8E4.field->unkC->unk14->unk3C, 0, 0, 0);
                func_800559C8(0, 0x7F);
            }
            g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 = -3;
        }
    }
    if ((g_gameState->unk1C->unk8E4.field->unkC->unk14->unk0 & 1)
        && g_gameState->unk1C->unk8E4.field->unkC->unk14->unk9 == -3) {
        g_gameState->unk1C->unk0 &= ~0x7F;
        g_gameState->unk1C->unk8E4.field->unk0 &= ~0x1919;
        g_padPortMask[0] &= 0xFBFFFFFF;
        g_gameState->unk0 |= 2;
    }
}

/* toggle the pause screen */
void func_800AF740(void) {
    u8 c[4];
    u8 buf[8];
    s32 tmp;
    s32 i;
    s32 bright;
    s32 v;
    u32 a;
    SPRT *sp;

    g_gameState->unk1C->unk0 ^= 0x100;
    if (g_gameState->unk1C->unk0 & 0x100) {
        func_8005562C(0);
        func_80023F60(1);
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].unk1 = 0;
            g_padActuator[i].unk0 = 0;
        }
        ClearOTag(&D_800C9F98, 1);
        func_80027834((320 - D_800686D0[235].unk4) / 2, (224 - D_800686D0[235].unk5) / 2, &D_800C9F98, 0xEB, 0, &tmp, &D_800C9FA0);
        bright = 0;
        if (func_8002F800() || func_8002F810()) {
            bright = 1;
        }
        v = func_8002F864();
        if (bright && v == 0) {
            if (func_8002F854() == 2) {
                func_8002FC10(buf);
                c[0] = (buf[0] + buf[4]) >> 1;
                c[1] = (buf[1] + buf[5]) >> 1;
                c[2] = (buf[2] + buf[6]) >> 1;
            } else {
                func_8002F9A8(c);
            }
            if (c[0] >= 200 && c[1] >= 200 && c[2] >= 200) {
                D_800C9FA0.unk0.code |= 2;
                sp = &D_800C9FA0.unk0;
                sp++;
                SetDrawTPage(sp, 1, 1,
                             getTPage(0, 2, tpage_getX((s16)D_800686D0[235].unk6), tpage_getY((s16)D_800686D0[235].unk6)));
            }
        }
        setlen(&D_800C9FE0, 3);
        D_800C9FE0.code = 0x62;
        setRGB0(&D_800C9FE0, 0x60, 0x60, 0x60);
        setWH(&D_800C9FE0, 320, 224);
        setXY0(&D_800C9FE0, 0, 0);
        setlen(&D_800C9F90, 1);
        a = getaddr(&D_800C9F98);
        D_800C9F90.code[0] = 0xE1000240;
        setaddr(&D_800C9FE0, a), setaddr(&D_800C9F98, &D_800C9FE0);
        addPrim(&D_800C9F98, &D_800C9F90);
        DrawOTag(&D_800C9F98);
        g_gameState->unk1C->unk0 |= 0x20;
    } else {
        func_80023F60(0);
        func_80055698(0);
        g_gameState->unk1C->unk0 &= ~0x20;
    }
}

void func_800AFAC0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Unk800AFCBC *h;
    FieldEnt *e;

    h = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk10;
    if (h == NULL) {
        return;
    }
    e = &h->unk8[arg0];
    if (arg1 == 0x82) {
        if (arg2 != -1) {
            func_800B00B0(e);
            e->unk1 = arg2;
            e->unk18 = db_lookupRegistered(4, (arg2 & 0xFFFF) | 0x160000, 0);
            e->unkE = func_800C9700(e->unk18) * 16;
        } else {
            e->unk18 = NULL;
        }
    } else if (arg1 == 0x83) {
        if (arg3 != 0) {
            e->unk0_0 |= arg2;
        } else {
            e->unk0_0 &= ~arg2;
        }
    } else if (arg1 == 0x87) {
        e->unk8[0] = arg2;
        e->unk8[1] = arg3;
        e->unk8[2] = arg4;
    } else if (arg1 == 0x8C) {
        e->unk10[0] = arg2;
        e->unk10[1] = arg3;
        e->unk10[2] = arg4;
    } else if (arg1 == 0x91) {
        e->unk4 = arg2;
    } else if (arg1 == 0x96) {
        e->unk2 = func_8004B098(arg2);
        e->unk3 = arg3;
    } else if (arg1 == 0x9B) {
        e->unk7 = arg2;
    } else if (arg1 == 0x9C) {
        e->unk0_4 = arg2;
    } else if (arg1 == 0xA0) {
        e->unk6 = arg2;
    } else if (arg1 == 0xA1) {
        e->unk16 = arg2 * 16;
    } else if (arg1 == 0xA5) {
        e->unk1C[0] = arg2;
        e->unk1C[1] = arg3;
        e->unk1C[2] = arg4;
    } else if (arg1 == 0xAA) {
        e->unk22 = arg2;
    }
}

u8 *func_800AFCBC(u8 *arg0) {
    s32 i;
    s32 n;
    Unk800AFCBC *h;

    func_800AA0C8(g_gameState->unk1C->unk8E4.field->unkC->unk14, (s16)g_gameState->unk1C->fieldId);
    i = 0;
    n = 0x10;
    h = (Unk800AFCBC *)arg0;
    arg0 += sizeof(Unk800AFCBC);
    h->unk4 = (u16 *)arg0;
    arg0 += 0x20;
    h->unk0 = 1;
    h->unk2 = n;
    h->unk3 = h->unk2;
    for (; i < n; i++) {
        h->unk4[i] = 0xFFFF;
    }
    h->unk8 = (FieldEnt *)arg0;
    arg0 += h->unk3 * 0x24;
    for (i = 0; i < h->unk3; i++) {
        func_800B00B0(&h->unk8[i]);
    }
    h->unkC[0] = arg0;
    h->unkC[1] = arg0 + 0x3000;
    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk10 = h;
    return arg0 + 0x6000;
}

void func_800AFDE8(void) {
    Unk800AFCBC *h;
    SVECTOR rot;
    s32 xy[2];
    s32 i;
    FieldEnt *e;
    u8 *prim;
    u32 *ot;
    long *t;

    h = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk10;
    if (h == NULL) {
        return;
    }
    if (D_800C9D44 & 0x40) {
        mbg_getGeomOffset(&xy[0], &xy[1]);
        SetGeomOffset(xy[0], xy[1]);
    } else {
        func_800C0F08();
    }
    prim = h->unkC[g_gameState->unk1C->unk8];
    ot = &g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][g_gameState->unk1C->unk8E4.field->unkC->unk14->unk30];
    for (i = 0; i < h->unk3; i++) {
        e = &h->unk8[i];
        if (e->unk18 != NULL && (e->unk0_0 & 1)) {
            if ((s8)e->unk2 != -1) {
                t = ((MATRIX *)((FieldObjSlot *)func_8001E350((s8)e->unk2))->unk8->unk20)[e->unk3].t;
                e->unk8[0] = t[0] + e->unk1C[0];
                e->unk8[1] = t[1] + e->unk1C[1];
                e->unk8[2] = t[2] + e->unk1C[2];
            }
            rot.vx = e->unk10[0];
            rot.vy = e->unk10[1];
            rot.vz = e->unk10[2];
            rot.pad = 0;
            prim = func_800C970C(prim, e->unk18, e->unk8, &rot, e->unk4, e->unk16 >> 4, e->unk0_4 < 4 ? e->unk0_4 : 0xFF, e->unk7 << 4, &ot[e->unk22]);
            e->unk16 += (s8)e->unk6;
            if (e->unk16 >= e->unkE) {
                e->unk16 = 0;
            } else if (e->unk16 < 0) {
                e->unk16 = ((e->unkE >> 4) - 1) << 4;
            }
        }
    }
    if (!(D_800C9D44 & 0x40)) {
        func_800C0FB0();
    }
}

/* reset a field entity */
void func_800B00B0(FieldEnt *arg0) {
    s32 i;

    arg0->unk4 = 0x1000;
    arg0->unk6 = 0x10;
    arg0->unk7 = 0x80;
    arg0->unk0_0 = 1;
    arg0->unk0_4 = 0xF;
    arg0->unk1 = -1;
    arg0->unk2 = -1;
    arg0->unk3 = 0;
    for (i = 0; i < 3; i++) {
        arg0->unk8[i] = 0;
        arg0->unk10[i] = 0;
        arg0->unk1C[i] = 0;
    }
    arg0->unk16 = 0;
    arg0->unkE = 0;
    arg0->unk18 = NULL;
    arg0->unk22 = 0;
}

#include "ff9.h"

EvtObject *evt_allocObject(s32 n) {
    EvtAllocNode *cur = g_evtState->unk15D0;
    EvtAllocNode *nn = NULL;
    EvtAllocNode *next;

    if (cur != NULL) {
        if ((u32 *)cur->unk0 >= g_evtState->unk50 + n) {
            nn = g_evtState->unk15D4;
            g_evtState->unk15D4 = nn->unk8;
            nn->unk8 = g_evtState->unk15D0;
            g_evtState->unk15D0 = nn;
            {
                u32 *p1 = g_evtState->unk50;
                nn->unk0 = (EvtObject *)p1;
                nn->unk4 = p1 + n;
            }
        } else {
            for (next = cur->unk8; next != NULL; next = cur->unk8) {
                if ((u32 *)next->unk0 >= cur->unk4 + n) {
                    break;
                }
                cur = next;
            }
            if (next != NULL) {
                nn = g_evtState->unk15D4;
                g_evtState->unk15D4 = nn->unk8;
                nn->unk8 = cur->unk8;
                cur->unk8 = nn;
                {
                    u32 *p2 = cur->unk4;
                    nn->unk0 = (EvtObject *)p2;
                    nn->unk4 = p2 + n;
                }
            } else if (cur->unk4 + n <= g_evtState->unk50 + 0x500) {
                nn = g_evtState->unk15D4;
                g_evtState->unk15D4 = nn->unk8;
                nn->unk8 = NULL;
                cur->unk8 = nn;
                {
                    u32 *p3 = cur->unk4;
                    nn->unk0 = (EvtObject *)p3;
                    nn->unk4 = p3 + n;
                }
            }
        }
    } else if (n <= 0x500) {
        nn = g_evtState->unk15D4;
        g_evtState->unk15D4 = nn->unk8;
        nn->unk8 = NULL;
        g_evtState->unk15D0 = nn;
        {
            u32 *p4 = g_evtState->unk50;
            nn->unk0 = (EvtObject *)p4;
            nn->unk4 = p4 + n;
        }
    }
    if (nn != NULL) {
        return nn->unk0;
    }
    return NULL;
}

EvtObject *evt_createObject(s32 idx, s32 id, s32 a2, s32 a3) {
    EvtObject *o;
    Unk16D8 *n;
    EvtState *d;
    s32 nw, nv;
    u32 *p;
    u32 *end;

    if (id == 0) {
        id = idx;
    }
    o = evt_getObject(id);
    if (o != NULL) {
        evt_deleteObject(o);
    }
    nw = (a2 + 3) >> 2;
    nv = (((ScriptEntry *)g_evtScripts)[idx].unk4 + 3) >> 2;
    o = evt_allocObject(nw + nv + a3);
    end = (u32 *)o + nw + nv;
    for (p = (u32 *)o; p < end; p++) {
        *p = 0;
    }
    d = g_evtState;
    n = d->unk16E0;
    d->unk16E0 = n->unk0;
    if (d->unk16DC != NULL) {
        d->unk16DC->unk0 = n;
        d->unk16DC = n;
    } else {
        d->unk16D8 = n;
        d->unk16DC = n;
    }
    n->unk0 = NULL;
    n->unk4 = o;
    o->scriptIdx = idx;
    o->uid = id;
    o->kind = 0;
    {
        u8 *h = g_evtScripts + ((ScriptEntry *)g_evtScripts)[idx].unk0;
        s32 c = *(u16 *)h >> 8;
        u16 *q = (u16 *)(h + 2);

        while (c != 0) {
            if (*q++ == 0) {
                break;
            }
            c--;
            q++;
        }
        if (c != 0) {
            h += *q + 2;
        } else {
            h = NULL;
        }
        o->ip = h;
    }
    o->stackOfs = nw + nv;
    o->varsOfs = nw;
    o->unkC = a3;
    o->state = 0;
    o->windowId = 0xFF;
    return o;
}

Unk16D8 *evt_deleteObject(EvtObject *obj) {
    Unk16D8 *n;
    Unk16D8 *prev;
    Unk16D8 *next = NULL;

    prev = NULL;
    for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
        if (n->unk4 == obj) {
            break;
        }
        prev = n;
    }
    if (n != NULL) {
        next = n->unk0;
        if (prev != NULL) {
            prev->unk0 = next;
        } else {
            g_evtState->unk16D8 = next;
        }
        if (g_evtState->unk16DC == n) {
            g_evtState->unk16DC = prev;
        }
        n->unk0 = g_evtState->unk16E0;
        g_evtState->unk16E0 = n;
        func_8004B2F4_inline(obj);
        if (g_evtState->playerUid == n->unk4->uid) {
            g_evtState->playerUid = 0;
        }
    }
    return next;
}

s32 evt_exprUnitIncDec(s32 op) {
    s32 r = 0;
    s32 w = g_evtExprSp[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp--;
        mask = evt_popResult();
        g_evtExprSp += 2;
    }
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case 8:
                r = evt_popResult();
                g_evtExprSp++;
                evt_assignResult(r + 1);
                break;
            case 9:
                r = evt_popResult();
                g_evtExprSp++;
                evt_assignResult(r - 1);
                break;
            case 10:
                r = evt_popResult() + 1;
                g_evtExprSp++;
                evt_assignResult(r);
                break;
            case 11:
                r = evt_popResult() - 1;
                g_evtExprSp++;
                evt_assignResult(r);
                break;
            }
            g_evtExprSp++;
        }
        mask >>= 1;
    }
    g_evtExprSp -= 2;
    return r;
}

s32 evt_exprUnitAssign(s32 op) {
    s32 r = 0;
    s32 w = g_evtExprSp[-3];
    s32 mask;
    s32 v;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp -= 2;
        mask = evt_popResult();
        g_evtExprSp += 3;
    }
    v = evt_popResult();
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case 0x2D:
                r = v;
                break;
            case 0x36:
                r = evt_popResult() * v;
                g_evtExprSp++;
                break;
            case 0x37:
                if (v != 0) {
                    r = evt_popResult() / v;
                } else {
                    r = evt_popResult();
                }
                g_evtExprSp++;
                break;
            case 0x38:
                if (v != 0) {
                    r = evt_popResult() % v;
                } else {
                    r = evt_popResult();
                }
                g_evtExprSp++;
                break;
            case 0x39:
                r = evt_popResult() + v;
                g_evtExprSp++;
                break;
            case 0x3A:
                r = evt_popResult() - v;
                g_evtExprSp++;
                break;
            case 0x3B:
                r = evt_popResult() << v;
                g_evtExprSp++;
                break;
            case 0x3C:
                r = evt_popResult() >> v;
                g_evtExprSp++;
                break;
            case 0x40:
                r = evt_popResult() & v;
                g_evtExprSp++;
                break;
            case 0x41:
                r = evt_popResult() ^ v;
                g_evtExprSp++;
                break;
            case 0x42:
                r = evt_popResult() | v;
                g_evtExprSp++;
                break;
            }
            evt_assignResult(r);
            g_evtExprSp++;
        }
        mask >>= 1;
    }
    g_evtExprSp -= 2;
    return r;
}

s32 evt_exprUnitSelect(s32 op) {
    s32 r = 0;
    s32 idx = r;
    s32 w = g_evtExprSp[-2];
    s32 mask;
    s32 i;
    s32 x;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp--;
        mask = evt_popResult();
        g_evtExprSp += 2;
    }
    switch (op) {
    case 0xF:
        for (i = 0; i < 8; i++) {
            r >>= 1;
            if (mask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                if (evt_popResult() == 0) {
                    r |= 0x80;
                }
                g_evtExprSp++;
            }
            mask >>= 1;
        }
        break;
    case 0x4B:
        r = -0x8000;
        for (i = 0; i < 8; i++) {
            if (mask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                x = evt_popResult();
                g_evtExprSp++;
                if (r < x) {
                    r = x;
                    idx = i;
                }
            }
            mask >>= 1;
        }
        r = 1 << idx;
        break;
    case 0x4C:
        r = 0x7FFF;
        for (i = 0; i < 8; i++) {
            if (mask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                x = evt_popResult();
                g_evtExprSp++;
                if (x < r) {
                    r = x;
                    idx = i;
                }
            }
            mask >>= 1;
        }
        r = 1 << idx;
        break;
    }
    g_evtExprSp -= 2;
    return r;
}

s32 evt_exprUnitCompare(s32 op) {
    s32 r = 0;
    s32 c = r;
    s32 w = g_evtExprSp[-3];
    s32 mask;
    s32 v;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp -= 2;
        mask = evt_popResult();
        g_evtExprSp += 3;
    }
    v = evt_popResult();
    for (i = 0; i < 8; i++) {
        r >>= 1;
        if (mask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case 0x1C:
                c = evt_popResult() < v;
                break;
            case 0x1D:
                c = evt_popResult() > v;
                break;
            case 0x1E:
                c = evt_popResult() <= v;
                break;
            case 0x1F:
                c = evt_popResult() >= v;
                break;
            case 0x22:
                c = evt_popResult() == v;
                break;
            case 0x23:
                c = evt_popResult() != v;
                break;
            case 0x54:
                c = (evt_popResult() & v) != 0;
                break;
            case 0x55:
                c = (evt_popResult() & v) == 0;
                break;
            case 0x56:
                c = (evt_popResult() ^ v) != 0;
                break;
            case 0x57:
                c = (evt_popResult() | v) != 0;
                break;
            }
            r |= c << 7;
            g_evtExprSp++;
        }
        mask >>= 1;
    }
    g_evtExprSp -= 2;
    return r;
}

s32 evt_exprUnitLogic(s32 op) {
    s32 r = 0;
    s32 w = g_evtExprSp[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp--;
        mask = evt_popResult();
        g_evtExprSp += 2;
    }
    for (i = 0; i < 8; i++) {
        r >>= 1;
        if (mask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            if (evt_popResult() != 0) {
                r |= 0x80;
            }
            g_evtExprSp++;
        }
        mask >>= 1;
    }
    g_evtExprSp -= 2;
    switch (op) {
    case 0x43:
        r &= evt_popResult();
        g_evtExprSp++;
        break;
    case 0x44:
        r ^= evt_popResult();
        g_evtExprSp++;
        break;
    case 0x45:
        r |= evt_popResult();
        g_evtExprSp++;
        break;
    }
    evt_assignResult(r);
    return r;
}

s32 evt_exprUnitFirst(void) {
    s32 r = 0;
    s32 w = g_evtExprSp[-2];
    s32 mask;
    s32 i;

    if (((w >> 26) & 7) == 5) {
        mask = func_8004B404_inline(w);
    } else {
        g_evtExprSp--;
        mask = evt_popResult();
        g_evtExprSp += 2;
    }
    i = 0;
    if (!(mask & 1)) {
        i = 1;
        mask >>= 1;
        while (i < 8 && !(mask & 1)) {
            mask >>= 1;
            i++;
        }
    }
    if (i < 8) {
        g_evtExprObj = g_evtUnitObjects[i];
        r = evt_popResult();
    } else {
        g_evtExprSp--;
    }
    g_evtExprSp--;
    return r;
}

s32 evt_joinParty(s32 slot) {
    s32 i;
    s32 r;

    if (func_8004B85C_find(func_8004B85C_key(slot), slot)) {
        r = 1;
    } else {
        slot = func_8004B85C_key(slot);
        if (slot >= 0 && slot < 9) {
            for (i = 0; i < 4; i++) {
                if (g_gameState->gameData->party[i] == NULL) {
                    break;
                }
            }
            r = i >= 4;
            if (r == 0) {
                party_setMember(i, slot);
                evt_execInstr_party();
            }
        } else {
            r = 1;
        }
    }
    return r;
}

s32 evt_exprSpecial(s32 op) {
    s32 r = 0;
    s16 px;
    s16 py;
    s32 x;
    GameData *gd;
    EvtObject *o;

    switch (op) {
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
        r = evt_exprUnitIncDec(op);
        break;
    case 0xF:
    case 0x4B:
    case 0x4C:
        r = evt_exprUnitSelect(op);
        break;
    case 0x2E:
    case 0x43:
    case 0x44:
    case 0x45:
        r = evt_exprUnitLogic(op);
        break;
    case 0x64:
        x = evt_popResult();
        if (x < 0x100) {
            r = item_getCount(x);
        } else if (x < 0x200) {
            r = (u8)keyItem_has(x - 0x100);
        } else {
            r = card_countOf(x - 0x200);
        }
        break;
    case 0x65:
        bgi_getAnimFrame(evt_popResult(), &r);
        break;
    case 0x52:
        gd = g_gameState->gameData;
        r = gd->characters[evt_exprSpecial_chr(evt_popResult())].hp;
        break;
    case 0x53:
        gd = g_gameState->gameData;
        r = gd->characters[evt_exprSpecial_chr(evt_popResult())].maxHp;
        break;
    case 0x6E:
        gd = g_gameState->gameData;
        r = gd->characters[evt_exprSpecial_chr(evt_popResult())].mp;
        break;
    case 0x6F:
        gd = g_gameState->gameData;
        r = gd->characters[evt_exprSpecial_chr(evt_popResult())].maxMp;
        break;
    case 0x70:
        px = -1;
        o = (EvtObject *)evt_popResult();
        if (o != NULL && o->kind == 4) {
            bgi_getActorTriangle(o->modelSlot, (u16 *)&px, (u16 *)&py);
        }
        r = px;
        break;
    case 0x71:
        py = -1;
        o = (EvtObject *)evt_popResult();
        if (o != NULL && o->kind == 4) {
            bgi_getActorTriangle(o->modelSlot, (u16 *)&px, (u16 *)&py);
        }
        r = py;
        break;
    case 0x6C:
        evt_popResult();
    case 0x5A:
    case 0x5B:
    case 0x5C:
        evt_popResult();
        break;
    }
    return r;
}

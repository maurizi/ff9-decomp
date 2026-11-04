#include "ff9.h"

EvtObject *evt_allocObject(s32 words) {
    EvtAllocNode *cur = g_evtState->allocUsed;
    EvtAllocNode *nn = NULL;
    EvtAllocNode *next;

    if (cur != NULL) {
        if ((u32 *)cur->start >= g_evtState->objPool + words) {
            nn = g_evtState->allocFree;
            g_evtState->allocFree = nn->next;
            nn->next = g_evtState->allocUsed;
            g_evtState->allocUsed = nn;
            {
                u32 *p1 = g_evtState->objPool;
                nn->start = (EvtObject *)p1;
                nn->end = p1 + words;
            }
        } else {
            for (next = cur->next; next != NULL; next = cur->next) {
                if ((u32 *)next->start >= cur->end + words) {
                    break;
                }
                cur = next;
            }
            if (next != NULL) {
                nn = g_evtState->allocFree;
                g_evtState->allocFree = nn->next;
                nn->next = cur->next;
                cur->next = nn;
                {
                    u32 *p2 = cur->end;
                    nn->start = (EvtObject *)p2;
                    nn->end = p2 + words;
                }
            } else if (cur->end + words <= g_evtState->objPool + 0x500) {
                nn = g_evtState->allocFree;
                g_evtState->allocFree = nn->next;
                nn->next = NULL;
                cur->next = nn;
                {
                    u32 *p3 = cur->end;
                    nn->start = (EvtObject *)p3;
                    nn->end = p3 + words;
                }
            }
        }
    } else if (words <= 0x500) {
        nn = g_evtState->allocFree;
        g_evtState->allocFree = nn->next;
        nn->next = NULL;
        g_evtState->allocUsed = nn;
        {
            u32 *p4 = g_evtState->objPool;
            nn->start = (EvtObject *)p4;
            nn->end = p4 + words;
        }
    }
    if (nn != NULL) {
        return nn->start;
    }
    return NULL;
}

EvtObject *evt_createObject(s32 scriptIdx, s32 id, s32 objBytes, s32 stackWords) {
    EvtObject *obj;
    EvtListNode *node;
    EvtState *evt;
    s32 nw, nv;
    u32 *p;
    u32 *end;

    if (id == 0) {
        id = scriptIdx;
    }
    obj = evt_getObject(id);
    if (obj != NULL) {
        evt_deleteObject(obj);
    }
    nw = (objBytes + 3) >> 2;
    nv = (((ScriptEntry *)g_evtScripts)[scriptIdx].varBytes + 3) >> 2;
    obj = evt_allocObject(nw + nv + stackWords);
    end = (u32 *)obj + nw + nv;
    for (p = (u32 *)obj; p < end; p++) {
        *p = 0;
    }
    evt = g_evtState;
    node = evt->listFree;
    evt->listFree = node->next;
    if (evt->objectsTail != NULL) {
        evt->objectsTail->next = node;
        evt->objectsTail = node;
    } else {
        evt->objects = node;
        evt->objectsTail = node;
    }
    node->next = NULL;
    node->obj = obj;
    obj->scriptIdx = scriptIdx;
    obj->uid = id;
    obj->kind = 0;
    {
        u8 *h = g_evtScripts + ((ScriptEntry *)g_evtScripts)[scriptIdx].offset;
        s32 tagsLeft = *(u16 *)h >> 8;
        u16 *tag = (u16 *)(h + 2);

        while (tagsLeft != 0) {
            if (*tag++ == 0) {
                break;
            }
            tagsLeft--;
            tag++;
        }
        if (tagsLeft != 0) {
            h += *tag + 2;
        } else {
            h = NULL;
        }
        obj->ip = h;
    }
    obj->stackOfs = nw + nv;
    obj->varsOfs = nw;
    obj->stackWords = stackWords;
    obj->state = EVT_STATE_NEW;
    obj->windowId = 0xFF;
    return obj;
}

EvtListNode *evt_deleteObject(EvtObject *obj) {
    EvtListNode *node;
    EvtListNode *prev;
    EvtListNode *next = NULL;

    prev = NULL;
    for (node = g_evtState->objects; node != NULL; node = node->next) {
        if (node->obj == obj) {
            break;
        }
        prev = node;
    }
    if (node != NULL) {
        next = node->next;
        if (prev != NULL) {
            prev->next = next;
        } else {
            g_evtState->objects = next;
        }
        if (g_evtState->objectsTail == node) {
            g_evtState->objectsTail = prev;
        }
        node->next = g_evtState->listFree;
        g_evtState->listFree = node;
        evt_freeAllocNode_inline(obj);
        if (g_evtState->playerUid == node->obj->uid) {
            g_evtState->playerUid = 0;
        }
    }
    return next;
}

s32 evt_exprUnitIncDec(s32 op) {
    s32 result = 0;
    s32 w = g_evtExprSp[-2];
    s32 unitMask;
    s32 i;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp--;
        unitMask = evt_popResult();
        g_evtExprSp += 2;
    }
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        if (unitMask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case EVT_EXPR_UNIT_POST_INC:
                result = evt_popResult();
                g_evtExprSp++;
                evt_assignResult(result + 1);
                break;
            case EVT_EXPR_UNIT_POST_DEC:
                result = evt_popResult();
                g_evtExprSp++;
                evt_assignResult(result - 1);
                break;
            case EVT_EXPR_UNIT_PRE_INC:
                result = evt_popResult() + 1;
                g_evtExprSp++;
                evt_assignResult(result);
                break;
            case EVT_EXPR_UNIT_PRE_DEC:
                result = evt_popResult() - 1;
                g_evtExprSp++;
                evt_assignResult(result);
                break;
            }
            g_evtExprSp++;
        }
        unitMask >>= 1;
    }
    g_evtExprSp -= 2;
    return result;
}

s32 evt_exprUnitAssign(s32 op) {
    s32 result = 0;
    s32 w = g_evtExprSp[-3];
    s32 unitMask;
    s32 operand;
    s32 i;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp -= 2;
        unitMask = evt_popResult();
        g_evtExprSp += 3;
    }
    operand = evt_popResult();
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        if (unitMask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case EVT_EXPR_UNIT_ASSIGN:
                result = operand;
                break;
            case EVT_EXPR_UNIT_MUL_ASSIGN:
                result = evt_popResult() * operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_DIV_ASSIGN:
                if (operand != 0) {
                    result = evt_popResult() / operand;
                } else {
                    result = evt_popResult();
                }
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_MOD_ASSIGN:
                if (operand != 0) {
                    result = evt_popResult() % operand;
                } else {
                    result = evt_popResult();
                }
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_ADD_ASSIGN:
                result = evt_popResult() + operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_SUB_ASSIGN:
                result = evt_popResult() - operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_SHL_ASSIGN:
                result = evt_popResult() << operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_SHR_ASSIGN:
                result = evt_popResult() >> operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_AND_ASSIGN:
                result = evt_popResult() & operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_XOR_ASSIGN:
                result = evt_popResult() ^ operand;
                g_evtExprSp++;
                break;
            case EVT_EXPR_UNIT_OR_ASSIGN:
                result = evt_popResult() | operand;
                g_evtExprSp++;
                break;
            }
            evt_assignResult(result);
            g_evtExprSp++;
        }
        unitMask >>= 1;
    }
    g_evtExprSp -= 2;
    return result;
}

s32 evt_exprUnitSelect(s32 op) {
    s32 result = 0;
    s32 idx = result;
    s32 w = g_evtExprSp[-2];
    s32 unitMask;
    s32 i;
    s32 x;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp--;
        unitMask = evt_popResult();
        g_evtExprSp += 2;
    }
    switch (op) {
    case EVT_EXPR_UNIT_NOT:
        for (i = 0; i < EVT_UNIT_COUNT; i++) {
            result >>= 1;
            if (unitMask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                if (evt_popResult() == 0) {
                    result |= 0x80;
                }
                g_evtExprSp++;
            }
            unitMask >>= 1;
        }
        break;
    case EVT_EXPR_UNIT_MAX:
        result = -0x8000;
        for (i = 0; i < EVT_UNIT_COUNT; i++) {
            if (unitMask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                x = evt_popResult();
                g_evtExprSp++;
                if (result < x) {
                    result = x;
                    idx = i;
                }
            }
            unitMask >>= 1;
        }
        result = 1 << idx;
        break;
    case EVT_EXPR_UNIT_MIN:
        result = 0x7FFF;
        for (i = 0; i < EVT_UNIT_COUNT; i++) {
            if (unitMask & 1) {
                g_evtExprObj = g_evtUnitObjects[i];
                x = evt_popResult();
                g_evtExprSp++;
                if (x < result) {
                    result = x;
                    idx = i;
                }
            }
            unitMask >>= 1;
        }
        result = 1 << idx;
        break;
    }
    g_evtExprSp -= 2;
    return result;
}

s32 evt_exprUnitCompare(s32 op) {
    s32 result = 0;
    s32 hit = result;
    s32 w = g_evtExprSp[-3];
    s32 unitMask;
    s32 operand;
    s32 i;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp -= 2;
        unitMask = evt_popResult();
        g_evtExprSp += 3;
    }
    operand = evt_popResult();
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        result >>= 1;
        if (unitMask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            switch (op) {
            case EVT_EXPR_UNIT_LT:
                hit = evt_popResult() < operand;
                break;
            case EVT_EXPR_UNIT_GT:
                hit = evt_popResult() > operand;
                break;
            case EVT_EXPR_UNIT_LE:
                hit = evt_popResult() <= operand;
                break;
            case EVT_EXPR_UNIT_GE:
                hit = evt_popResult() >= operand;
                break;
            case EVT_EXPR_UNIT_EQ:
                hit = evt_popResult() == operand;
                break;
            case EVT_EXPR_UNIT_NE:
                hit = evt_popResult() != operand;
                break;
            case EVT_EXPR_UNIT_ANY_BITS:
                hit = (evt_popResult() & operand) != 0;
                break;
            case EVT_EXPR_UNIT_NO_BITS:
                hit = (evt_popResult() & operand) == 0;
                break;
            case EVT_EXPR_UNIT_XOR_NZ:
                hit = (evt_popResult() ^ operand) != 0;
                break;
            case EVT_EXPR_UNIT_OR_NZ:
                hit = (evt_popResult() | operand) != 0;
                break;
            }
            result |= hit << 7;
            g_evtExprSp++;
        }
        unitMask >>= 1;
    }
    g_evtExprSp -= 2;
    return result;
}

s32 evt_exprUnitLogic(s32 op) {
    s32 result = 0;
    s32 w = g_evtExprSp[-2];
    s32 unitMask;
    s32 i;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp--;
        unitMask = evt_popResult();
        g_evtExprSp += 2;
    }
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        result >>= 1;
        if (unitMask & 1) {
            g_evtExprObj = g_evtUnitObjects[i];
            if (evt_popResult() != 0) {
                result |= 0x80;
            }
            g_evtExprSp++;
        }
        unitMask >>= 1;
    }
    g_evtExprSp -= 2;
    switch (op) {
    case EVT_EXPR_UNIT_MASK_AND:
        result &= evt_popResult();
        g_evtExprSp++;
        break;
    case EVT_EXPR_UNIT_MASK_XOR:
        result ^= evt_popResult();
        g_evtExprSp++;
        break;
    case EVT_EXPR_UNIT_MASK_OR:
        result |= evt_popResult();
        g_evtExprSp++;
        break;
    }
    evt_assignResult(result);
    return result;
}

s32 evt_exprUnitFirst(void) {
    s32 result = 0;
    s32 w = g_evtExprSp[-2];
    s32 unitMask;
    s32 i;

    if (EVT_VAR_SPACE(w) == EVT_SPACE_BATTLE) {
        unitMask = evt_getBattleVar_inline(w);
    } else {
        g_evtExprSp--;
        unitMask = evt_popResult();
        g_evtExprSp += 2;
    }
    i = 0;
    if (!(unitMask & 1)) {
        i = 1;
        unitMask >>= 1;
        while (i < EVT_UNIT_COUNT && !(unitMask & 1)) {
            unitMask >>= 1;
            i++;
        }
    }
    if (i < EVT_UNIT_COUNT) {
        g_evtExprObj = g_evtUnitObjects[i];
        result = evt_popResult();
    } else {
        g_evtExprSp--;
    }
    g_evtExprSp--;
    return result;
}

s32 evt_joinParty(s32 slot) {
    s32 i;
    s32 result;

    if (evt_isInParty_find(evt_isInParty_key(slot), slot)) {
        result = 1;
    } else {
        slot = evt_isInParty_key(slot);
        if (slot >= 0 && slot < CHR_COUNT) {
            for (i = 0; i < PARTY_SIZE; i++) {
                if (g_gameState->gameData->party[i] == NULL) {
                    break;
                }
            }
            result = i >= PARTY_SIZE;
            if (result == 0) {
                party_setMember(i, slot);
                evt_execInstr_party();
            }
        } else {
            result = 1;
        }
    }
    return result;
}

s32 evt_exprSpecial(s32 op) {
    s32 result = 0;
    s16 px;
    s16 py;
    s32 x;
    GameData *gd;
    EvtObject *obj;

    switch (op) {
    case EVT_EXPR_UNIT_POST_INC:
    case EVT_EXPR_UNIT_POST_DEC:
    case EVT_EXPR_UNIT_PRE_INC:
    case EVT_EXPR_UNIT_PRE_DEC:
        result = evt_exprUnitIncDec(op);
        break;
    case EVT_EXPR_UNIT_NOT:
    case EVT_EXPR_UNIT_MAX:
    case EVT_EXPR_UNIT_MIN:
        result = evt_exprUnitSelect(op);
        break;
    case EVT_EXPR_UNIT_MASK:
    case EVT_EXPR_UNIT_MASK_AND:
    case EVT_EXPR_UNIT_MASK_XOR:
    case EVT_EXPR_UNIT_MASK_OR:
        result = evt_exprUnitLogic(op);
        break;
    case EVT_EXPR_ITEM_COUNT:
        x = evt_popResult();
        if (x < ITEM_ID_KEY_FIRST) {
            result = item_getCount(x);
        } else if (x < ITEM_ID_CARD_FIRST) {
            result = (u8)keyItem_has(x - ITEM_ID_KEY_FIRST);
        } else {
            result = card_countOf(x - ITEM_ID_CARD_FIRST);
        }
        break;
    case EVT_EXPR_BGI_ANIM_FRAME:
        bgi_getAnimFrame(evt_popResult(), &result);
        break;
    case EVT_EXPR_CHR_HP:
        gd = g_gameState->gameData;
        result = gd->characters[evt_exprSpecial_chr(evt_popResult())].hp;
        break;
    case EVT_EXPR_CHR_MAX_HP:
        gd = g_gameState->gameData;
        result = gd->characters[evt_exprSpecial_chr(evt_popResult())].maxHp;
        break;
    case EVT_EXPR_CHR_MP:
        gd = g_gameState->gameData;
        result = gd->characters[evt_exprSpecial_chr(evt_popResult())].mp;
        break;
    case EVT_EXPR_CHR_MAX_MP:
        gd = g_gameState->gameData;
        result = gd->characters[evt_exprSpecial_chr(evt_popResult())].maxMp;
        break;
    case EVT_EXPR_ACTOR_TRIANGLE:
        px = -1;
        obj = (EvtObject *)evt_popResult();
        if (obj != NULL && obj->kind == EVT_KIND_ACTOR) {
            bgi_getActorTriangle(obj->modelSlot, (u16 *)&px, (u16 *)&py);
        }
        result = px;
        break;
    case EVT_EXPR_ACTOR_FLOOR:
        py = -1;
        obj = (EvtObject *)evt_popResult();
        if (obj != NULL && obj->kind == EVT_KIND_ACTOR) {
            bgi_getActorTriangle(obj->modelSlot, (u16 *)&px, (u16 *)&py);
        }
        result = py;
        break;
    case EVT_EXPR_UNUSED_6C:
        evt_popResult();
    case EVT_EXPR_UNUSED_5A:
    case EVT_EXPR_UNUSED_5B:
    case EVT_EXPR_UNUSED_5C:
        evt_popResult();
        break;
    }
    return result;
}

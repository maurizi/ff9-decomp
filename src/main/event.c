#include "ff9.h"

s32 evt_getSysVar(s32 op) {
    s32 result;
    u8 camera[8];
    EvtObject *obj;
    s32 id;

    switch ((u32)op) {
    case EVT_SYSVAR_RAND:
        result = rand8_inline();
        break;
    case EVT_SYSVAR_CAMERA:
        fldbg_getCamera(camera);
        result = camera[0];
        break;
    case EVT_SYSVAR_CONTROL_ENABLED:
        result = g_evtState->controlEnabled;
        break;
    case EVT_SYSVAR_SOUND_STATE:
        result = fld_soundCommand(SND_GET_STATE, -1, 0, 0, 0);
        break;
    case EVT_SYSVAR_TALK_ANGLE:
        result = g_evtTalkAngle;
        break;
    case EVT_SYSVAR_OBJ_CHAR:
        id = ((u8 *)&((u32 *)g_evtCurObj + g_evtCurObj->stackOfs)[g_evtCurObj->sp])[-1];
        obj = evt_getObject_inline(id);
        result = -1;
        if (obj != NULL) {
            result = obj->scriptIdx - (g_evtScriptCount - 9);
        }
        break;
    case EVT_SYSVAR_GIL:
        result = g_gameState->gameData->gil;
        break;
    case EVT_SYSVAR_STEP_COUNT:
        result = g_evtStepCount;
        break;
    case EVT_SYSVAR_MSG_SYNC_TAG:
        result = g_msgSyncTag;
        break;
    case EVT_SYSVAR_MSG_CHOICE:
        result = msg_getChoiceIndex();
        break;
    case EVT_SYSVAR_LINE_POINT_X:
        result = g_evtLinePointX;
        break;
    case EVT_SYSVAR_LINE_POINT_Z:
        result = g_evtLinePointZ;
        break;
    case EVT_SYSVAR_SCREEN_X:
        result = g_evtScreenX;
        break;
    case EVT_SYSVAR_SCREEN_Y:
        result = g_evtScreenY;
        break;
    case EVT_SYSVAR_MOVIE_FRAME:
        result = fld_movieCommand(FLD_MOVIE_CMD_FRAME, 0, 0);
        break;
    case EVT_SYSVAR_MOVIE_STATE:
        result = fld_movieCommand(FLD_MOVIE_CMD_STATE, 0, 0);
        break;
    case EVT_SYSVAR_FROG_COUNT:
        result = g_gameState->gameData->frogCount;
        break;
    case EVT_SYSVAR_TIMER_SECONDS:
        result = timer_getSeconds();
        break;
    case EVT_SYSVAR_CARD_RESULT:
        result = card_getGameResult();
        break;
    case EVT_SYSVAR_CARD_COUNT:
        result = card_countAll();
        break;
    case EVT_SYSVAR_PLAY_TIME:
        result = game_getPlayTimeSeconds();
        if (result > 0x7FFFFF) {
            result = 0x7FFFFF;
        }
        break;
    case EVT_SYSVAR_TIMER_ENABLED:
        result = timer_isEnabled();
        break;
    case EVT_SYSVAR_SOUND_VOLUME:
        result = g_evtSoundVolume;
        break;
    case EVT_SYSVAR_SOUND_PAN:
        result = g_evtSoundPan;
        break;
    case EVT_SYSVAR_BATTLE_ID:
        result = (s16)g_gameState->gameData->battleId;
        break;
    case EVT_SYSVAR_BTLSEQ_BUSY:
        result = (u8)btlseq_isBusy();
        break;
    case EVT_SYSVAR_BATTLE_END:
        result = g_evtMode == EVT_MODE_BATTLE_END;
        break;
    case EVT_SYSVAR_BATTLE_RESULT:
        result = g_gameState->gameData->battleResult;
        break;
    case EVT_SYSVAR_BATTLE_ARG_HI:
        result = (g_evtBattleTriggerArg >> 8) & 0xFF;
        break;
    case EVT_SYSVAR_BATTLE_ARG_LO:
        result = (u8)g_evtBattleTriggerArg;
        break;
    case EVT_SYSVAR_BATTLE_PHASE:
        result = g_gameState->gameData->ctx.ovl65->phase;
        break;
    case EVT_SYSVAR_BATTLE_GROUP:
        result = g_gameState->gameData->ctx.ovl65->groupNo;
        break;
    default:
        if (op >= EVT_SYSVAR_WORLD_FIRST) {
            result = w_frameGetParameter(op);
        } else {
            result = btl_getBattleVar(op);
        }
        break;
    }
    return result;
}

EvtObject *evt_getObjectArg(void) {
    return evt_resolveObjectId_inline(evt_getArg8_inline());
}

/* Find the entry point for `key` in script `idx`. */
static inline u8 *evt_callObjectFunc_find(s32 idx, s32 key) {
    u8 *h = g_evtScripts + *(u16 *)(g_evtScripts + idx * 8);
    s32 count = *(u16 *)h >> 8;
    u16 *entry = (u16 *)(h + 2);

    while (count != 0) {
        if (*entry++ == key) {
            break;
        }
        count--;
        entry++;
    }
    return count != 0 ? h + (*entry + 2) : NULL;
}

void evt_returnFromFunc(EvtObject *obj) {
    s32 saved;
    EvtListNode *node;
    EvtListNode *iter;
    EvtObject *other;
    EvtObject *caller;
    u32 *base;
    u32 *slot;

    if (obj->sp >= 2) {
        slot = (u32 *)obj + obj->stackOfs + obj->sp;
        slot--;
        saved = slot[0];
        if (!(*(u32 *)&obj->level & EVT_OBJ_LEVEL_UID)) {
            for (iter = g_evtState->objects; iter != NULL; iter = iter->next) {
                other = iter->obj;
                if (other->uid != 0) {
                    other->state = other->savedState;
                }
            }
        }
        obj->ip = g_evtState->scriptBase + slot[-1];
        obj->wait = saved;
        obj->sp -= 2;
        obj->level = saved >> 8;
        if (EVT_FRAME_CALLER_LEVEL_OF(saved) != EVT_FRAME_NO_WAIT) {
            for (node = g_evtState->objects; node != NULL; node = node->next) {
                if (node->obj->uid == (u32)saved >> 24) {
                    break;
                }
            }
            caller = NULL;
            if (node != NULL) {
                caller = node->obj;
            }
            if (caller != NULL) {
                if (caller->wait == EVT_WAIT_CALL) {
                    caller->wait = 0;
                    return;
                }
                base = (u32 *)caller + caller->stackOfs;
                slot = base + (caller->sp - 1);
                while (base < slot) {
                    if (*(u8 *)slot == EVT_WAIT_CALL) {
                        break;
                    }
                    slot -= 2;
                }
                if (base < slot) {
                    *slot &= ~EVT_FRAME_WAIT;
                }
            }
        }
    } else if (obj->state == EVT_STATE_INIT) {
        obj->state = EVT_STATE_RUNNING;
        obj->ip = evt_callObjectFunc_find(obj->scriptIdx, EVT_FUNC_LOOP);
        obj->level = 7;
    } else {
        obj->ip = NULL;
    }
}

s32 evt_callObjectFunc(EvtObject *obj, s32 lv, s32 key, s32 save) {
    u8 *entry = NULL;
    u32 *slot;
    u16 w;
    u32 hi;
    u32 x;

    if (obj != NULL && lv < obj->level) {
        entry = evt_callObjectFunc_find(obj->scriptIdx, key);
        if (entry != NULL) {
            slot = &((u32 *)obj + obj->stackOfs)[obj->sp];
            slot[0] = obj->ip - g_evtState->scriptBase;
            w = obj->wait | (obj->level << 8);
            hi = g_evtCurObj->uid << 24;
            if (save) {
                x = (g_evtCurObj->level << 16);
                x = x | w;
                x |= hi;
            } else {
                x = hi | (EVT_FRAME_NO_WAIT << 16);
                x = x | w;
            }
            slot[1] = x;
            if (save) {
                g_evtCurObj->wait = EVT_WAIT_CALL;
            }
            obj->ip = entry;
            obj->level = lv;
            obj->wait = 0;
            obj->sp += 2;
        }
    }
    return entry != NULL;
}

void *evt_findNearestObject(EvtObject *obj, s32 flags, s32 *dist) {
    EvtObject *best = NULL;
    s32 bestd = 0;
    s32 x, y, z;
    s32 back;
    s32 radius;
    EvtListNode *node;
    EvtObject *other;
    s32 selfSkip;
    s32 otherSkip;
    s32 dx, dz;
    s32 rr;
    s32 distSq;
    s32 flagByte;
    s32 cosA;
    s32 angleDiff;
    u8 otherFlags;
    u8 objFlags;
    s32 key;

    x = obj->pos.vx;
    y = obj->pos.vy;
    back = flags & EVT_FIND_TALK;
    z = obj->pos.vz;
    if (back) {
        radius = obj->talkRadius * 4;
    } else {
        radius = obj->collisionRadius * 4;
    }
    for (node = g_evtState->objects; node != NULL; node = node->next) {
        other = node->obj;
        selfSkip = 0;
        if (back) {
            selfSkip = 1;
        } else {
            objFlags = obj->flags;
            if (other->uid == g_evtState->playerUid ? (objFlags & EVT_OBJ_NO_PLAYER_COLLISION) : (objFlags & EVT_OBJ_NO_NPC_COLLISION)) {
                selfSkip = 1;
            }
        }
        flagByte = other->flags;
        if (best) {
            otherFlags = flagByte;
        } else {
            otherFlags = other->flags;
        }
        if (back) {
            otherSkip = otherFlags & EVT_OBJ_NO_TALK;
        } else if (obj->uid == g_evtState->playerUid) {
            otherSkip = otherFlags & EVT_OBJ_NO_PLAYER_COLLISION;
        } else {
            otherSkip = otherFlags & EVT_OBJ_NO_NPC_COLLISION;
        }
        if (other == obj || (selfSkip && otherSkip)) {
            continue;
        }
        if ((flags & EVT_FIND_SCRIPTED) && evt_callObjectFunc_find(other->scriptIdx, back ? EVT_FUNC_TALK : EVT_FUNC_RANGE) == NULL) {
            continue;
        }
        if (other->kind != EVT_KIND_ACTOR) {
            continue;
        }
        {
            EvtObject *cand = other;
        if ((u32)(cand->pos.vy - y + 399) >= 799) {
            continue;
        }
        dx = cand->pos.vx - x;
        dz = cand->pos.vz - z;
        if ((dx < 0 ? -dx : dx) >= 0x800) {
            continue;
        }
        if ((dz < 0 ? -dz : dz) >= 0x800) {
            continue;
        }
        if (back) {
            rr = cand->talkRadius * 4;
        } else {
            if (selfSkip) {
                rr = cand->collisionRadius * 4;
            } else {
                rr = cand->collisionRadius * 4;
            }
        }
        if (cand->collisionEllipse != 0) {
            cosA = math_cos(evt_angleToObject(cand, obj, cand->pos.rotY));
            rr = (rr * math_isqrt(((cosA * cosA) >> 4) * cand->collisionEllipse + 0x1000000)) >> 12;
        }
        rr += radius;
        if (flags & EVT_FIND_SCRIPTED) {
            rr += 0x3C + cand->speed;
        }
        distSq = math_distSq2(dx, dz, rr);
        if (back) {
            angleDiff = evt_angleToObject(obj, cand, obj->pos.rotY);
            if (angleDiff < 0) {
                angleDiff = -angleDiff;
            }
            angleDiff -= 640;
            distSq += angleDiff;
            if (angleDiff >= 0) {
                continue;
            }
        }
        if (distSq < bestd) {
            bestd = distSq;
            best = cand;
        }
        }
    }
    if (dist != NULL) {
        *dist = bestd;
    }
    return best;
}

s32 evt_moveObjectTo(s32 x, s32 y, s32 z, s32 flags) {
    EvtObject *actor;
    s32 dz;
    s32 dx;
    s32 dy;
    s32 speed;
    s16 delta[4];
    s32 dist;
    s32 ry;
    s32 rx;
    s32 cur;
    s32 diff;
    s32 step;
    s32 hit;
    s32 moving;

    actor = PSX_NULLABLE(g_evtActor);
    /* g_evtActor is NULL when a move op runs in a script that has no actor (seen in d1_f0801); the PS1 then reads and
     * writes these fields in low RAM; a port guards it via PSX_NULLABLE (ff9-ds F39). */
    speed = actor->speed;
    dx = x - actor->pos.vx;
    dz = z - actor->pos.vz;
    if (actor->moveFlags & EVT_MOVE_FACING_SET) {
        ry = actor->pos.rotY & 0xFFF;
    } else {
        ry = math_atan2(dx, dz);
        if (!(flags & EVT_MOVETO_NO_TURN)) {
            cur = actor->pos.rotY & 0xFFF;
            diff = ry - cur;
            if (diff < -0x800) {
                diff += 0x1000;
            } else if (diff >= 0x800) {
                diff -= 0x1000;
            }
            if (diff == 0) {
                actor->moveFlags |= EVT_MOVE_FACING_SET;
            }
            step = actor->moveTurnRate * 8;
            if (diff > 0) {
                if (step < diff) {
                    ry = cur + step;
                }
            } else {
                if (diff < -step) {
                    ry = cur - step;
                }
            }
            ry &= 0xFFF;
            actor->pos.rotY = ry;
        }
    }
    if (flags & EVT_MOVETO_3D) {
        dy = y - actor->pos.vy;
        rx = math_atan2(-dy, -math_length3(dx, 0, dz)) & 0xFFF;
        cur = actor->pitch & 0xFFF;
        diff = rx - cur;
        if (diff < -0x800) {
            diff += 0x1000;
        } else if (diff >= 0x800) {
            diff -= 0x1000;
        }
        step = actor->moveTurnRate * 8;
        if (diff > 0) {
            if (step < diff) {
                rx = cur + step;
            }
        } else {
            if (diff < -step) {
                rx = cur - step;
            }
        }
        rx &= 0xFFF;
        actor->pitch = rx;
    } else {
        rx = 0;
        dy = 0;
    }
    math_dirVector(delta, rx, ry, speed);
    actor->pos.vx += delta[0];
    actor->pos.vy += delta[1];
    actor->pos.vz += delta[2];
    hit = evt_findNearestObject(actor, 0, &dist);
    if (hit != 0 && dist <= 0) {
        actor->pos.vx -= delta[0];
        actor->pos.vy -= delta[1];
        actor->pos.vz -= delta[2];
    }
    if (actor->animLoops != 0 && actor->animLoops != EVT_ANIM_FOREVER) {
        actor->animLoops--;
    }
    moving = actor->animLoops != 0;
    if (PSX_IS_PTR(flags)) { /* EVT_OP_MOVE_TO_OBJ: flags is the target object; stop once touching it */
        moving = 0;
        if (hit != flags || dist > 0) {
            moving = 1;
        }
    }
    dist = math_dist2(dx, dy, dz);
    if (dist < speed * speed || ((actor->moveFlags & EVT_MOVE_APPROACHING) && actor->moveDistSq < dist)) {
        moving = 0;
    }
    if (actor->moveDistSq < EVT_MOVE_DIST_NONE && dist < actor->moveDistSq) {
        actor->moveFlags |= EVT_MOVE_APPROACHING;
    }
    actor->moveDistSq = dist;
    if (moving == 0) {
        actor->moveDistSq = EVT_MOVE_DIST_NONE;
        actor->pitch = 0;
        actor->moveFlags &= ~(EVT_MOVE_FACING_SET | EVT_MOVE_APPROACHING);
    }
    if (actor->uid == g_evtState->playerUid) {
        g_evtPlayerAnimCount++;
    }
    return moving;
}

s32 evt_rollEncounter(EvtObject *obj) {
    s32 id = 0;

    if (g_evtState->controlEnabled != 0 && g_evtEncounterDist > EVT_ENCOUNTER_STEP_DIST) {
        g_evtEncounterDist = 0;
        g_evtEncounterChance += g_evtState->encounterRate;
        if (rand8_inline() < (g_evtEncounterChance >> 3)) {
            g_evtEncounterChance = 0;
            id = evt_rollEncounter_pick();
            if (id == g_evtLastEncounter) {
                id = evt_rollEncounter_pick();
            }
            g_evtLastEncounter = id;
            evt_setNextBattle(id);
            g_gameState->gameData->enemyGroup = -1;
        }
    }
    return id != 0;
}

/* screen position of an actor's speech-bubble anchor */
static inline void evt_placeBalloon_pos(EvtObject *obj, s32 *x, s32 *y) {
    SVECTOR sv;

    sv.vx = obj->pos.vx;
    sv.vy = obj->pos.vy + ((obj->height * (s8)obj->scale) >> 6) - EVT_HEAD_ANCHOR_RAISE;
    sv.vz = obj->pos.vz;
    if (obj->kind == EVT_KIND_ACTOR) {
        sv.vx += obj->balloonOfsX;
        sv.vy += obj->balloonOfsY;
        sv.vz += obj->balloonOfsZ;
    }
    evt_projectToScreen(&sv, x, y);
}

static inline void evt_placeBalloon_x(WinLayout *w, s32 sx, s32 right, s32 *ptx) {
    s32 width = w->w;
    s32 tx, lo, hi;

    if (right) {
        tx = sx - BALLOON_TAIL_OFS;
        if (tx < BALLOON_TAIL_MIN_R) {
            tx = BALLOON_TAIL_MIN_R;
        }
        if (tx > BALLOON_TAIL_MAX_R) {
            tx = BALLOON_TAIL_MAX_R;
        }
        lo = tx + 8 - width;
        hi = tx - 24;
    } else {
        tx = sx + BALLOON_TAIL_OFS;
        if (tx > BALLOON_TAIL_MAX_L) {
            tx = BALLOON_TAIL_MAX_L;
        }
        if (tx < BALLOON_TAIL_MIN_L) {
            tx = BALLOON_TAIL_MIN_L;
        }
        lo = tx + 24 - width;
        hi = tx - 8;
    }
    *ptx = tx;
    if (lo < BALLOON_LEFT) {
        lo = BALLOON_LEFT;
    }
    if (hi > BALLOON_RIGHT - width) {
        hi = BALLOON_RIGHT - width;
    }
    w->x = (lo + hi) >> 1;
}

static inline void evt_placeBalloon_y(WinLayout *w, s32 sy, s32 *above, s32 fixed) {
    s32 h = w->h;
    s32 ty;

    if (*above) {
        ty = sy - h - BALLOON_GAP_ABOVE;
        if (!fixed && ty < BALLOON_TOP) {
            *above ^= 1;
            ty = sy + BALLOON_GAP_BELOW;
        }
    } else {
        ty = sy + BALLOON_GAP_BELOW;
        if (!fixed && ty > BALLOON_BOTTOM - h) {
            *above = 1;
            ty = sy - h - BALLOON_GAP_ABOVE;
        }
    }
    if (ty < BALLOON_TOP) {
        ty = BALLOON_TOP;
    } else if (ty > BALLOON_BOTTOM - h) {
        ty = BALLOON_BOTTOM - h;
    }
    w->y = ty;
}

void evt_placeBalloon(EvtObject *obj, WinLayout *w, s32 dir, s32 fixed) {
    s32 sx, sy;
    s32 ox, oy;
    s32 tx;
    s32 above;
    s32 right;
    s32 id;
    EvtObject *other;

    if (obj->kind == EVT_KIND_ACTOR) {
        obj->balloonOfsX = g_evtBalloonOfsX;
        obj->balloonOfsY = g_evtBalloonOfsY;
        obj->balloonOfsZ = g_evtBalloonOfsZ;
    }
    evt_placeBalloon_pos(obj, &sx, &sy);
    if (dir < 0) {
        if (obj->uid == g_evtState->playerUid) {
            id = obj->talkTarget;
        } else {
            id = g_evtState->playerUid;
        }
        other = evt_getObject_inline(id);
        if (other != NULL) {
            evt_placeBalloon_pos(other, &ox, &oy);
            above = sy < oy;
            right = sx < ox;
        } else {
            above = 1;
            right = 0;
        }
    } else {
        above = (dir >> 1) & 1;
        right = dir & 1;
    }
    if (fixed == 0) {
        right = right ? right ^ (sx < BALLOON_FLIP_L_X) : sx > BALLOON_FLIP_R_X;
    }
    evt_placeBalloon_x(w, sx, right, &tx);
    evt_placeBalloon_y(w, sy, &above, fixed);
    if (fixed == 0 && msg_overlapsRect(w)) {
        above ^= 1;
        evt_placeBalloon_y(w, sy, &above, 0);
    }
    w->tailOffset = tx - w->x;
    w->tailMode = (above << 1) | right;
}

/* bubble x/y targets for evt_followBalloon (evt_placeBalloon_x/_y without the flip logic) */
static inline s32 evt_followBalloon_x(s32 width, s32 sx, s32 right, s32 *ptx) {
    s32 tx, lo, hi;

    if (right) {
        tx = sx - BALLOON_TAIL_OFS;
        if (tx < BALLOON_TAIL_MIN_R) {
            tx = BALLOON_TAIL_MIN_R;
        }
        if (tx > BALLOON_TAIL_MAX_R) {
            tx = BALLOON_TAIL_MAX_R;
        }
        lo = tx + 8 - width;
        hi = tx - 24;
    } else {
        tx = sx + BALLOON_TAIL_OFS;
        if (tx > BALLOON_TAIL_MAX_L) {
            tx = BALLOON_TAIL_MAX_L;
        }
        if (tx < BALLOON_TAIL_MIN_L) {
            tx = BALLOON_TAIL_MIN_L;
        }
        lo = tx + 24 - width;
        hi = tx - 8;
    }
    *ptx = tx;
    if (lo < BALLOON_LEFT) {
        lo = BALLOON_LEFT;
    }
    if (hi > BALLOON_RIGHT - width) {
        hi = BALLOON_RIGHT - width;
    }
    return (lo + hi) >> 1;
}

static inline s32 evt_followBalloon_y(s32 h, s32 *sy, s32 above) {
    s32 ty;

    if (above) {
        ty = *sy - h - BALLOON_GAP_ABOVE;
    } else {
        ty = *sy + BALLOON_GAP_BELOW;
    }
    if (ty < BALLOON_TOP) {
        ty = BALLOON_TOP;
    } else if (ty > BALLOON_BOTTOM - h) {
        ty = BALLOON_BOTTOM - h;
    }
    return ty;
}

void evt_followBalloon(EvtObject *obj, Window *w) {
    s32 sx, sy;
    s32 tx;
    s32 ox, oy;
    s32 right, above;
    s32 x, y;

    ox = w->x;
    oy = w->y;
    evt_placeBalloon_pos(obj, &sx, &sy);
    above = (w->tailMode >> 1) & 1;
    right = w->tailMode & 1;
    x = evt_followBalloon_x(w->w - 3, sx, right, &tx);
    y = evt_followBalloon_y(w->h - 3, &sy, above);
    w->x = (ox + x) >> 1;
    w->y = (oy + y) >> 1;
    w->tailX = tx - w->x - right * 16;
    w->cur.x += w->x - ox;
    w->cur.y += w->y - oy;
}

WinLayout *evt_getMsgLayout(EvtObject *obj, s32 id, s32 flags) {
    WinLayout *w = msg_getLayout(id, &g_evtBalloonOfsX, &g_evtBalloonOfsY, &g_evtBalloonOfsZ);
    s32 ww;
    s32 wh;

    w->tailOffset = 0;
    w->tailMode = 0;
    if ((flags & MSG_FLAG_ATE) && w->w < MSGWIN_ATE_MIN_W) {
        w->w = MSGWIN_ATE_MIN_W;
    }
    if (flags & MSG_FLAG_BALLOON) {
        if (w->w < MSGWIN_BALLOON_MIN_W) {
            w->w = MSGWIN_BALLOON_MIN_W;
        }
        switch (w->anchor) {
        case TEXT_WIN_BOTTOM_RIGHT:
        case TEXT_WIN_BOTTOM_LEFT:
        case TEXT_WIN_TOP_RIGHT:
        case TEXT_WIN_TOP_LEFT:
            evt_placeBalloon(PSX_NULLABLE(obj), w, TEXT_WIN_SIDE(w->anchor), 0);
            break;
        case TEXT_WIN_FIXED_BOTTOM_RIGHT:
        case TEXT_WIN_FIXED_BOTTOM_LEFT:
        case TEXT_WIN_FIXED_TOP_RIGHT:
        case TEXT_WIN_FIXED_TOP_LEFT:
            evt_placeBalloon(PSX_NULLABLE(obj), w, TEXT_WIN_SIDE(w->anchor), 1);
            break;
        default:
            evt_placeBalloon(PSX_NULLABLE(obj), w, -1, 0);
            break;
        }
    } else {
        ww = w->w;
        wh = w->h;
        switch (w->anchor) {
        case TEXT_WIN_TOP_LEFT:
            w->x = MSGWIN_MARGIN;
            w->y = MSGWIN_MARGIN;
            break;
        case TEXT_WIN_TOP:
            w->x = MSGWIN_CENTER_X - (ww >> 1);
            w->y = MSGWIN_MARGIN;
            break;
        case TEXT_WIN_TOP_RIGHT:
            w->x = MSGWIN_RIGHT - ww;
            w->y = MSGWIN_MARGIN;
            break;
        case TEXT_WIN_CENTER:
            w->x = MSGWIN_CENTER_X - (ww >> 1);
            w->y = MSGWIN_CENTER_Y - (wh >> 1);
            break;
        case TEXT_WIN_BOTTOM_LEFT:
            w->x = MSGWIN_MARGIN;
            w->y = MSGWIN_BOTTOM_Y - wh;
            break;
        case TEXT_WIN_BOTTOM:
            w->x = MSGWIN_CENTER_X - (ww >> 1);
            w->y = MSGWIN_BOTTOM_Y - wh;
            break;
        case TEXT_WIN_BOTTOM_RIGHT:
            w->x = MSGWIN_RIGHT - ww;
            w->y = MSGWIN_BOTTOM_Y - wh;
            break;
        case WINLAYOUT_ANCHOR_NONE:
            w->x = MSGWIN_CENTER_X - (ww >> 1);
            w->y = MSGWIN_DEFAULT_Y - (wh >> 1);
            break;
        }
    }
    if ((flags & MSG_FLAG_NO_TAIL) && w->w < MSGWIN_BALLOON_MIN_W) {
        w->w = MSGWIN_BALLOON_MIN_W;
    }
    w->tailMode |= flags & (MSG_FLAG_FRAMELESS | MSG_FLAG_NO_FOLLOW);
    return w;
}

void evt_turnObject(EvtObject *obj, s32 angle, s32 wrap, s32 speed) {
    s32 cur;
    s32 rate;
    s32 frames;
    s16 dir;

    obj->turnTarget = angle & 0xFFF;
    cur = obj->pos.rotY & 0xFFF;
    angle -= cur;
    if (wrap) {
        if (angle > 0x800) {
            angle -= 0x1000;
        } else if (angle < -0x800) {
            angle += 0x1000;
        }
    }
    if (angle == 0) {
        return;
    }
    obj->animStart = 0;
    obj->animEnd = 0xFF;
    obj->turnStartDir = cur;
    obj->turnPrevAnim = obj->anim;
    obj->flags |= EVT_OBJ_TURNING;
    if (speed == 0) {
        speed = 0x10;
    }
    if (angle < 0) {
        frames = evt_getAnimFrameCount(obj->modelId, (u16)obj->prevPos.rotX);
        rate = frames * 16 / speed;
        if (-angle < 0x400) {
            rate = rate * -angle >> 10;
        }
        if (rate >= 6) {
            obj->turnRate = (angle + 0x400) / rate;
            obj->animFlags = 0;
            evt_execInstr_anim(obj, (u16)obj->prevPos.rotX);
            if (obj->uid == g_evtState->playerUid) {
                g_evtPlayerAnimCount++;
            }
            obj->animSpeed = frames * 16 / rate;
        } else {
            obj->turnRate = EVT_TURN_DIRECT_DOWN;
            dir = cur;
            if (obj->turnTarget >= cur) {
                dir = cur | 0x1000;
            }
            obj->pos.rotY = dir;
        }
    } else {
        frames = evt_getAnimFrameCount(obj->modelId, (u16)obj->prevPos.rotY);
        rate = frames * 16 / speed;
        if (angle < 0x400) {
            rate = rate * angle >> 10;
        }
        if (rate >= 6) {
            obj->turnRate = (angle - 0x400) / rate;
            obj->animFlags = 0;
            evt_execInstr_anim(obj, (u16)obj->prevPos.rotY);
            if (obj->uid == g_evtState->playerUid) {
                g_evtPlayerAnimCount++;
            }
            obj->animSpeed = frames * 16 / rate;
        } else {
            obj->turnRate = EVT_TURN_DIRECT_UP;
            dir = cur;
            if (cur >= obj->turnTarget) {
                dir = cur | 0xF000;
            }
            obj->pos.rotY = dir;
        }
    }
    if (rate <= 0) {
        rate = 1;
    }
    obj->turnStep = angle / rate;
}

s32 evt_execInstr(void) {
    EvtObject *po = g_evtActor;
    EvtObject *actor = po;
    EvtObject *obj;
    EvtObject *argObj;
    EvtListNode *node;
    s32 op;
    /* decoded script operands; their meaning depends on the opcode. Per-case locals change the register
       allocation of this function, so the cases share these. */
    s32 operand0, operand1, operand2, operand3, operand4, operand5, i;
    SVECTOR sv;
    EvtRegionPt *pt;
    Unk8004AFF0 *mdl, *m2;
    u8 *str;

    g_evtInstrStart = g_evtCurObj->ip;
    op = *g_evtCurObj->ip++;
    g_evtArgFlags = *g_evtCurObj->ip++;
    g_evtArgRead = 0;

    switch (op) {
    case EVT_OP_NEW_CODE:
        func_8004B3A8_inline(g_evtArgFlags, *g_evtCurObj->ip++);
        g_evtArgRead = 1;
        return 0;
    case EVT_OP_NEW_REGION:
        evt_createRegion_inline(g_evtArgFlags, *g_evtCurObj->ip++);
        g_evtArgRead = 1;
        return 0;
    case EVT_OP_NEW_ACTOR:
        operand0 = g_evtArgFlags;
        operand1 = *g_evtCurObj->ip++;
        if ((u32)(operand0 - EVT_OBJ_PARTY) < PARTY_SIZE) {
            operand0 = g_evtState->partyUids[operand0 - EVT_OBJ_PARTY];
        }
        evt_execInstr_newactor(operand0, operand1);
        g_evtArgRead = 1;
        return 0;
    case EVT_OP_NOP_46:
    case EVT_OP_NOP_79:
    case EVT_OP_NOP_D2:
    case EVT_OP_NOP_D3:
        return 0;
    case EVT_OP_CALL:
        operand0 = evt_getArg8_inline();
        obj = evt_getObjectArg(); /* reads the script before the key byte */
        evt_callObjectFunc(obj, operand0, *g_evtCurObj->ip++, 0);
        return 0;
    case EVT_OP_CALL_WAIT_FREE:
        operand0 = evt_getArg8_inline();
        obj = evt_getObjectArg();
        operand2 = *g_evtCurObj->ip++;
        if (obj != NULL && operand0 < obj->level) {
            evt_callObjectFunc(obj, operand0, operand2, 0);
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_CALL_SYNC:
        operand0 = evt_getArg8_inline();
        obj = evt_getObjectArg();
        operand2 = *g_evtCurObj->ip++;
        if (obj != NULL && operand0 < obj->level) {
            evt_callObjectFunc(obj, operand0, operand2, 1);
        } else {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_CALL_CALLER:
        operand0 = evt_getArg8_inline();
        argObj = evt_getObject_inline(CUR_UID());
        evt_callObjectFunc(argObj, operand0, *g_evtCurObj->ip++, 0);
        return 0;
    case EVT_OP_CALL_CALLER_WAIT_FREE:
        operand0 = evt_getArg8_inline();
        if (evt_getObject_inline(CUR_UID()) != NULL && operand0 < evt_getObject_inline(CUR_UID())->level) {
            argObj = evt_getObject_inline(CUR_UID());
            evt_callObjectFunc(argObj, operand0, *g_evtCurObj->ip++, 0);
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_CALL_CALLER_SYNC:
        operand0 = evt_getArg8_inline();
        if (evt_getObject_inline(CUR_UID()) != NULL && operand0 < evt_getObject_inline(CUR_UID())->level) {
            argObj = evt_getObject_inline(CUR_UID());
            evt_callObjectFunc(argObj, operand0, *g_evtCurObj->ip++, 1);
        } else {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_RESTART_OBJ:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            obj->sp = 0;
            obj->state = EVT_STATE_INIT;
            evt_returnFromFunc(obj);
        }
        return obj == g_evtCurObj;
    case EVT_OP_SET_OBJ_FLAGS:
        g_evtActor->flags &= ~EVT_OBJ_SCRIPT_BITS;
        g_evtActor->flags |= evt_getArg8_inline() & EVT_OBJ_SCRIPT_BITS;
        return 0;
    case EVT_OP_WALK_ONLY:
        g_evtState->walkOnly = 1;
        return 0;
    case EVT_OP_WALK_ONLY_OFF:
        g_evtState->walkOnly = 0;
        return 0;
    case EVT_OP_DISABLE_ACTORS:
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            EvtObject *listObj = node->obj;
            if (listObj->kind == EVT_KIND_ACTOR) {
                listObj->savedFlags = listObj->flags;
                if (!(listObj->flags & EVT_OBJ_KEEP_ENABLED)) {
                    listObj->flags &= ~EVT_OBJ_ENABLED;
                }
            }
        }
        return 0;
    case EVT_OP_RESTORE_ACTORS:
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            EvtObject *listObj = node->obj;
            if (listObj->kind == EVT_KIND_ACTOR) {
                listObj->flags &= ~EVT_OBJ_ENABLED;
                listObj->flags |= listObj->savedFlags & 1;
            }
        }
        return 0;
    case EVT_OP_SET_RADIUS:
        if (g_evtActor->kind == EVT_KIND_ACTOR) {
            operand0 = evt_getArg8_inline();
            if (g_evtMode == EVT_MODE_FIELD && po->modelId != EVT_MODEL_NONE) {
                bgi_setActorRadius(po->modelSlot, (operand0 << 2) & 0xFFFC);
            }
            po->collisionRadius = evt_getArg8_inline();
            po->talkRadius = evt_getArg8_inline();
        } else {
            evt_getArg8_inline();
            evt_getArg8_inline();
            evt_getArg8_inline();
        }
        return 0;
    case EVT_OP_NOP_A3:
        evt_getArg8_inline();
        evt_getArg8_inline();
        evt_getArg8_inline();
        evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_COLLISION_ELLIPSE:
        po->collisionEllipse = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ACTOR_TYPE:
        g_evtCurObj->actorType = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_POS_XZ_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_POS_XZ:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = g_evtMode == EVT_MODE_FIELD && po->modelId != EVT_MODEL_NONE;
        if (operand2) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        po->pos.vx = po->prevPos.vx = operand0;
        po->pos.vy = po->prevPos.vy = -0x8000;
        po->pos.vz = po->prevPos.vz = operand1;
        if (operand2) {
            bgi_setActorActive(po->modelSlot, 1);
        }
        if (po->kind == EVT_KIND_ACTOR) {
            po->moveDistSq = EVT_MOVE_DIST_NONE;
            po->pitch = 0;
            po->moveFlags &= ~(EVT_MOVE_FACING_SET | EVT_MOVE_APPROACHING);
        }
        g_evtPosSetThisFrame = 1;
        return 0;
    case EVT_OP_SET_POS_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_POS:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        operand3 = g_evtMode == EVT_MODE_FIELD && po->modelId != EVT_MODEL_NONE;
        if (operand3) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        po->pos.vx = po->prevPos.vx = operand0;
        po->pos.vy = po->prevPos.vy = operand1;
        po->pos.vz = po->prevPos.vz = operand2;
        if (po->kind == EVT_KIND_ACTOR) {
            po->moveDistSq = EVT_MOVE_DIST_NONE;
            po->pitch = 0;
            po->moveFlags &= ~(EVT_MOVE_FACING_SET | EVT_MOVE_APPROACHING);
        }
        return 0;
    case EVT_OP_SET_DIR_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_DIR:
        po->pos.rotY = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_SET_ROT_XZ:
        po->pos.rotX = evt_getArg8_inline() << 4;
        po->rotZ = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_SET_LOOK:
        actor->lookParam0 = evt_getArg8_inline();
        actor->lookParam1 = evt_getArg8_inline();
        actor->moveFlags |= EVT_MOVE_LOOK_TARGET | EVT_MOVE_LOOK_AT_ACTORS;
        return 0;
    case EVT_OP_SET_MOVE_FLAGS:
        actor->moveFlags &= ~EVT_MOVE_LOOK_BITS;
        actor->moveFlags |= evt_getArg8_inline() & EVT_MOVE_LOOK_BITS;
        return 0;
    case EVT_OP_SET_TALK_TURN:
        actor->talkTurnScript = evt_getArg8_inline() ? EVT_OP_TURN_TO_ANGLE : EVT_OP_RETURN;
        return 0;
    case EVT_OP_SET_TURN_SPEED:
        actor->turnSpeed = evt_getArg8_inline();
        if (actor->turnSpeed == 0) {
            actor->turnSpeed = 0x10;
        }
        return 0;
    case EVT_OP_TURN_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_TURN:
        operand0 = evt_getArg8_inline() << 4;
        evt_turnObject(actor, operand0, 1, evt_getArg8_inline());
        return 0;
    case EVT_OP_TURN_TO_ANGLE:
        evt_turnObject(actor, evt_getArg8_inline() << 4, 1, actor->turnSpeed);
        return 0;
    case EVT_OP_TURN_TO_POINT:
        operand0 = evt_getArg16_inline() - actor->pos.vx;
        operand1 = evt_getArg16_inline() - actor->pos.vz;
        if (operand0 | operand1) {
            evt_turnObject(actor, math_atan2(operand0, operand1), 1, actor->turnSpeed);
        }
        return 0;
    case EVT_OP_TURN_TO_OBJ:
        obj = evt_getObjectArg();
        operand0 = evt_getArg8_inline();
        if (obj != NULL) {
            evt_turnObject(actor, math_atan2(obj->pos.vx - actor->pos.vx, obj->pos.vz - actor->pos.vz), 1, operand0);
        }
        return 0;
    case EVT_OP_WAIT_TURN_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_WAIT_TURN:
        if (actor->flags & EVT_OBJ_TURNING) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MSG_NUMBER:
        operand0 = evt_getArg8_inline();
        msg_setNumber(operand0, evt_getArg16_inline());
        return 0;
    case EVT_OP_MSG_SYNC_TAG:
        g_msgSyncTag = evt_getArg8_inline();
        return 0;
    case EVT_OP_MSG_CHOICE_MASK:
        operand0 = evt_getArg16_inline();
        msg_setChoiceMask(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_MSG_KEEP_WINDOWS:
        msg_keepWindowsOnInit();
        return 0;
    case EVT_OP_MSG_WAIT:
        g_evtActor->windowId = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        evt_execInstr_window(g_evtActor, operand2, g_evtActor->windowId);
        operand1 = evt_getArg16_inline();
        msg_open(operand1, g_evtActor->windowId, evt_getMsgLayout(g_evtActor, operand1, operand2), operand2);
        g_evtActor->wait = EVT_WAIT_WINDOW;
        return 1;
    case EVT_OP_MSG:
        g_evtActor->windowId = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        evt_execInstr_window(g_evtActor, operand2, g_evtActor->windowId);
        operand1 = evt_getArg16_inline();
        msg_open(operand1, g_evtActor->windowId, evt_getMsgLayout(g_evtActor, operand1, operand2), operand2);
        return 0;
    case EVT_OP_MSG_OBJ_WAIT:
        po = evt_getObjectArg();
        g_evtActor->windowId = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        evt_execInstr_window(PSX_NULLABLE(po), operand2, g_evtActor->windowId);
        operand1 = evt_getArg16_inline();
        msg_open(operand1, g_evtActor->windowId, evt_getMsgLayout(po, operand1, operand2), operand2);
        g_evtActor->wait = EVT_WAIT_WINDOW;
        return 1;
    case EVT_OP_MSG_OBJ:
        po = evt_getObjectArg();
        g_evtActor->windowId = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        evt_execInstr_window(PSX_NULLABLE(po), operand2, g_evtActor->windowId);
        operand1 = evt_getArg16_inline();
        msg_open(operand1, g_evtActor->windowId, evt_getMsgLayout(po, operand1, operand2), operand2);
        return 0;
    case EVT_OP_MSG_CLOSE:
        operand0 = evt_getArg8_inline();
        msg_close(operand0);
        return 0;
    case EVT_OP_MSG_CLOSE_ALL:
        msg_resetWindows();
        win_closeAll();
        return 0;
    case EVT_OP_MSG_WAIT_WIN:
        g_evtActor->windowId = evt_getArg8_inline();
        g_evtActor->wait = EVT_WAIT_WINDOW;
        return 1;
    case EVT_OP_MSG_CLEAR_OTZ:
        msg_clearWindowOtz();
        return 0;
    case EVT_OP_BTL_BANNER:
        btl_setBanner((s32)msg_getText(evt_getArg16_inline()), 3);
        return 0;
    case EVT_OP_MOVE_RESET:
        actor->animLoops = EVT_ANIM_FOREVER;
        actor->moveDistSq = EVT_MOVE_DIST_NONE;
        actor->pitch = 0;
        actor->moveFlags &= ~(EVT_MOVE_FACING_SET | EVT_MOVE_APPROACHING);
        return 0;
    case EVT_OP_SET_MOVE_COUNT:
        actor->animLoops = evt_getArg8_inline();
        return 0;
    case EVT_OP_MOVE_XZ:
        operand0 = evt_getArg16_inline();
        if (evt_moveObjectTo(operand0, 0, evt_getArg16_inline(), 0)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_XZ_NOTURN:
        operand0 = evt_getArg16_inline();
        if (evt_moveObjectTo(operand0, 0, evt_getArg16_inline(), EVT_MOVETO_NO_TURN)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_TO_OBJ:
        po = evt_getObjectArg();
        if (evt_moveObjectTo(po->pos.vx, po->pos.vy, po->pos.vz, (s32)po)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_MOVE_TO_LINE_POINT:
        if (evt_moveObjectTo(g_evtLinePointX, 0, g_evtLinePointZ, 0)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_XYZ:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        if (evt_moveObjectTo(operand0, operand1, operand2, EVT_MOVETO_3D)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_XYZ_NOTURN:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        if (evt_moveObjectTo(operand0, operand1, operand2, EVT_MOVETO_NO_TURN | EVT_MOVETO_3D)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_SET_MOVE_TURN:
        operand0 = evt_getArg8_inline();
        if (operand0 == 0) {
            operand0 = 0xFF;
        }
        actor->moveTurnRate = operand0;
        return 0;
    case EVT_OP_SET_WALK_SPEED:
        actor->speed = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_RUN_SPEED:
        actor->runSpeed = evt_getArg8_inline();
        return 0;
    case EVT_OP_ANIM_FWD:
        actor->animFlags = EVT_ANIM_HOLD;
        actor->animStart = 0;
        actor->animEnd = actor->jumpAnimStart - 1;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_ANIM_BACK:
        ((ModelSlot *)game_getModel(actor->modelSlot))->flags &= ~MODEL_SLOT_NO_SHADOW;
        actor->animEnd = 0xFF;
        actor->animFlags = 0;
        actor->animStart = actor->jumpAnimEnd + 1;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_JUMP:
        actor->jumpTargetX = evt_getArg16_inline();
        actor->jumpTargetY = evt_getArg16_inline();
        actor->jumpTargetZ = evt_getArg16_inline();
        operand0 = evt_getArg8_inline();
        if (operand0 == 0) {
            operand0 = 8;
        }
        actor->moveFlags |= EVT_MOVE_JUMPING;
        ((ModelSlot *)game_getModel(actor->modelSlot))->flags |= MODEL_SLOT_NO_SHADOW;
        actor->animStart = actor->jumpAnimStart;
        actor->animEnd = actor->jumpAnimEnd;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        actor->jumpFrame = 0;
        actor->jumpFrames = operand0;
        actor->jumpStartX = actor->pos.vx;
        actor->jumpStartY = actor->pos.vy;
        actor->jumpStartZ = actor->pos.vz;
        actor->animSpeed = ((actor->animEnd - actor->animStart) << 4) / operand0;
        if (g_evtMode == EVT_MODE_FIELD && po->modelId != EVT_MODEL_NONE) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        return 0;
    case EVT_OP_JUMP_STEP:
        operand1 = actor->jumpFrames;
        operand0 = ++actor->jumpFrame;
        actor->pos.vx = ((s16)actor->jumpStartX * (operand1 - operand0) + actor->jumpTargetX * operand0) / operand1;
        actor->pos.vy = (s16)actor->jumpStartY + operand0 * operand0 * 8 + operand0 * (actor->jumpTargetY - (s16)actor->jumpStartY) / operand1 - operand0 * (operand1 << 3);
        actor->pos.vz = ((s16)actor->jumpStartZ * (operand1 - operand0) + actor->jumpTargetZ * operand0) / operand1;
        if (operand0 >= operand1) {
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_SET_REGION: {
        u32 cnt;
        pt = ((EvtRegion *)g_evtActor)->pts;
        cnt = ((EvtRegion *)g_evtActor)->count = evt_getArg8_inline();
        while (cnt-- != 0) {
            pt->x = evt_getArg16_inline();
            pt->z = evt_getArg16_inline();
            pt++;
        }
        return 0;
    }
    case EVT_OP_LINE_POINT:
        po = evt_getObject_inline(g_evtState->playerUid);
        if (po != NULL) {
            EvtRegionPt *seg = ((EvtRegion *)g_evtActor)->pts;
            s32 dx = seg[1].x - seg[0].x;
            s32 dz = seg[1].z - seg[0].z;
            s32 t = (dx * dx + dz * dz) >> 8;
            if (t != 0) {
                t = (dx * (po->pos.vx - seg[0].x) + dz * (po->pos.vz - seg[0].z)) / t;
                if (t < 0) {
                    t = 0;
                } else if (t > 0x100) {
                    t = 0x100;
                }
            }
            g_evtLinePointX = ((t * dx) >> 8) + seg[0].x;
            g_evtLinePointZ = ((t * dz) >> 8) + seg[0].z;
        } else {
            g_evtLinePointZ = 0;
            g_evtLinePointX = 0;
        }
        return 0;
    case EVT_OP_PLAYER_TO_LINE_POINT:
        po = evt_getObject_inline(g_evtState->playerUid);
        if (po != NULL) {
            s32 *sp = (s32 *)po + po->stackOfs + po->sp;
            EvtListNode *nn;
            sp[0] = po->ip - g_evtState->scriptBase;
            sp[1] = po->wait | po->level << 8 | 0xFF << 16 | g_evtCurObj->uid << 24;
            po->level = 0;
            po->wait = 0;
            po->sp += 2;
            po->ip = g_evtPlayerToLinePointScript;
            g_evtState->controlEnabled = 0;
            for (nn = g_evtState->objects; nn != NULL; nn = nn->next) {
                nn->obj->flags |= EVT_OBJ_NO_NPC_COLLISION | EVT_OBJ_NO_PLAYER_COLLISION;
            }
        }
        return 0;
    case EVT_OP_REGION_START: {
        EvtRegion *region = (EvtRegion *)g_evtActor;
        region->count = 2;
        region->pts[0].x = region->pts[1].x = evt_getArg16_inline();
        region->pts[0].z = region->pts[1].z = evt_getArg16_inline();
        return 0;
    }
    case EVT_OP_REGION_MOVE_LAST: {
        EvtRegion *region = (EvtRegion *)g_evtActor;
        EvtRegionPt *last;
        operand0 = region->count;
        last = &region->pts[(operand0 > 0 ? operand0 : 1) - 1];
        last->x = evt_getArg16_inline();
        last->z = evt_getArg16_inline();
        return 0;
    }
    case EVT_OP_REGION_DUP_LAST: {
        EvtRegion *region = (EvtRegion *)g_evtActor;
        s32 k = region->count;
        if (k >= 1 && k < 8) {
            region->pts[k] = region->pts[k - 1];
            region->count++;
        }
        return 0;
    }
    case EVT_OP_GOTO_FIELD:
        evt_setNextLocation(evt_getArg16_inline());
        return EVT_UPD_FIELD;
    case EVT_OP_GOTO_WORLD:
        evt_setNextLocation(evt_getArg16_inline());
        return EVT_UPD_WORLD;
    case EVT_OP_CARD_GAME:
        evt_setCardOpponent(evt_getArg16_inline() & 0xFFFF);
        return EVT_UPD_CARD_GAME;
    case EVT_OP_GAME_OVER:
        return EVT_UPD_GAME_OVER;
    case EVT_OP_BATTLE:
        g_swirlVariant = evt_getArg8_inline();
        g_gameState->gameData->enemyGroup = -1;
        operand0 = evt_getArg16_inline();
        g_gameState->gameData->darkBattle = (operand0 >> EVT_BATTLE_DARK_BIT) & 1;
        evt_setNextBattle(operand0 & EVT_BATTLE_ID_MASK);
        return EVT_UPD_BATTLE;
    case EVT_OP_BATTLE_EX:
        g_swirlVariant = evt_getArg8_inline();
        g_gameState->gameData->enemyGroup = evt_getArg8_inline();
        operand0 = evt_getArg16_inline();
        g_gameState->gameData->darkBattle = (operand0 >> EVT_BATTLE_DARK_BIT) & 1;
        evt_setNextBattle(operand0 & EVT_BATTLE_ID_MASK);
        return EVT_UPD_BATTLE;
    case EVT_OP_ENCOUNTER_RATE:
        if ((g_evtState->encounterRate = evt_getArg8_inline()) == 0) {
            g_evtEncounterChance = 0;
        }
        return 0;
    case EVT_OP_ENCOUNTER_TABLE:
        g_evtEncounterTable.pattern = evt_getArg8_inline();
        g_evtEncounterTable.battle0 = evt_getArg16_inline();
        g_evtEncounterTable.battle1 = evt_getArg16_inline();
        g_evtEncounterTable.battle2 = evt_getArg16_inline();
        g_evtEncounterTable.battle3 = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_PLAYER:
        g_evtState->playerUid = g_evtCurObj->uid;
        return 0;
    case EVT_OP_DISABLE_CONTROL:
        g_evtState->controlEnabled = 0;
        msg_startHereIcon(0);
        g_evtPlayerAnimCount = 0;
        g_evtMsgCount = 0;
        return 0;
    case EVT_OP_ENABLE_CONTROL: {
        EvtState *evt;
        g_evtState->controlEnabled = 1;
        msg_startHereIcon(1);
        evt = g_evtState;
        if (evt->idleTimer >= 0) {
            evt->idleTimer = (rand8_inline() + 0xC8) * 4;
        }
        return 0;
    }
    case EVT_OP_SET_MODEL:
        po->modelId = evt_getArg16_inline();
        po->modelSlot = evt_createModel(g_evtCurObj);
        g_evtCurObj->flags |= EVT_OBJ_ENABLED;
        po->height = -(evt_getArg8_inline() << 2);
        return 0;
    case EVT_OP_ATTACH_OFFSET:
        mdl = evt_getObjectModelData_inline(g_evtActor);
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        if (mdl != NULL) {
            mdl->attachX = operand0;
            mdl->attachY = operand1;
            mdl->attachZ = operand2;
        }
        return 0;
    case EVT_OP_ATTACH:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        argObj = evt_getObjectArg();
        m2 = evt_getObjectModelData_inline(argObj);
        operand0 = evt_getArg8_inline();
        if (mdl != NULL && m2 != NULL) {
            mdl->parent = m2;
            mdl->parentBone = operand0;
        }
        return 0;
    case EVT_OP_DETACH:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        if (mdl != NULL) {
            mdl->parent = NULL;
        }
        return 0;
    case EVT_OP_MODEL_SCALE:
        po = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(po);
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        if (mdl != NULL) {
            mdl->scaleX = (s8)operand0 << 6;
            mdl->scaleY = (s8)operand1 << 6;
            mdl->scaleZ = (s8)operand2 << 6;
            mdl->flags |= MDL_SCALED;
        }
        PSX_NULLABLE(po)->scale = operand1;
        return 0;
    case EVT_OP_TEX_ANIM_PLAY:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        operand1 = evt_getArg8_inline();
        if (mdl != NULL) {
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].flags |= TEXANIM_ACTIVE;
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].pos = 0;
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].lastFrame = 0x1000;
        }
        return 0;
    case EVT_OP_TEX_ANIM_PLAY_ONCE:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        operand1 = evt_getArg8_inline();
        if (mdl != NULL) {
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].flags |= TEXANIM_ACTIVE | TEXANIM_ONESHOT;
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].pos = 0;
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].lastFrame = 0x1000;
        }
        return 0;
    case EVT_OP_TEX_ANIM_STOP:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        operand1 = evt_getArg8_inline();
        if (mdl != NULL) {
            PSX_NULLABLE(mdl->texAnims)->unk0[operand1].flags &= ~(TEXANIM_ACTIVE | TEXANIM_ONESHOT);
        }
        return 0;
    case EVT_OP_MODEL_FLAGS_SET: {
        ((ModelSlot *)game_getModel(po->modelSlot))->flags |= evt_getArg16_inline();
        return 0;
    }
    case EVT_OP_MODEL_FLAGS_CLEAR: {
        ((ModelSlot *)game_getModel(po->modelSlot))->flags &= ~evt_getArg16_inline();
        return 0;
    }
    case EVT_OP_NOP_STRING:
        str = g_evtCurObj->ip - 1;
        while (*str++ != 0) {
        }
        evt_skipVarRef(&str);
        g_evtArgRead = 1;
        g_evtCurObj->ip = str + 1;
        return 0;
    case EVT_OP_END_FRAME:
        g_evtArgRead = 1;
        g_evtCurObj->ip++;
        return EVT_UPD_END_FRAME;
    case EVT_OP_SET_STAND_ANIM:
        actor->standAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_WALK_ANIM:
        actor->walkAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_RUN_ANIM:
        actor->runAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_TURN_LEFT_ANIM:
        actor->prevPos.rotX = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_TURN_RIGHT_ANIM:
        actor->prevPos.rotY = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_IDLE_ANIM:
        actor->idleAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_JUMP_ANIM:
        actor->jumpAnim = evt_getArg16_inline();
        actor->jumpAnimStart = evt_getArg8_inline();
        actor->jumpAnimEnd = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_LOOP:
        actor->animFlags = (evt_getArg8_inline() << 3) & EVT_ANIM_LOOP_MODE;
        actor->animLoops = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_SPEED:
        actor->defaultAnimSpeed = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_IDLE_SPEEDS:
        actor->idleAnimSpeeds.b[0] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[1] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[2] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[3] = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_RANGE:
        actor->animStart = evt_getArg8_inline();
        actor->animEnd = evt_getArg8_inline();
        return 0;
    case EVT_OP_ANIM_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_ANIM:
        evt_execInstr_anim(actor, evt_getArg16_inline());
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_WAIT_ANIM_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_WAIT_ANIM:
        if (!(actor->animFlags & EVT_ANIM_SCRIPTED)) {
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_ANIM_END:
        evt_onAnimEnd(actor);
        return 0;
    case EVT_OP_ANIM_SYNC:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            if (obj != po) {
                actor->animSyncSource = obj;
                actor->animFlags = EVT_ANIM_SCRIPTED;
            } else {
                actor->animSyncSource = NULL;
                actor->animFlags = 0;
            }
        }
        return 0;
    case EVT_OP_RUN_THREAD: {
        EvtObject *oldThread;
        operand0 = g_evtCurObj->uid + 0x40;
        oldThread = evt_getObject_inline(operand0);
        if (oldThread != NULL) {
            evt_deleteObject(oldThread);
        }
        evt_createObject(evt_getArg8_inline(), operand0, 0x14, 0)->kind = EVT_KIND_THREAD;
        return 0;
    }
    case EVT_OP_WAIT_THREAD:
        if (evt_getObject_inline(g_evtCurObj->uid + 0x40) != NULL) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
            return 1;
        }
        return 0;
    case EVT_OP_KILL_THREAD: {
        EvtObject *thread = evt_getObject_inline(g_evtCurObj->uid + 0x40);
        if (thread != NULL) {
            evt_deleteObject(thread);
        }
        return 0;
    }
    case EVT_OP_BTL_ENEMY_CMD:
        switch (g_evtCurObj->level) {
        case 0: {
            s32 x = (u16)g_evtBattleVars[1], z = (u16)g_evtBattleVars[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_DEATH, evt_getArg8_inline());
            break;
        }
        case 1: {
            s32 x = (u16)g_evtBattleVars[1], z = (u16)g_evtBattleVars[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_COUNTER, evt_getArg8_inline());
            break;
        }
        case 3: {
            s32 x = (u16)g_evtBattleVars[1], z = (u16)g_evtBattleVars[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_TURN, evt_getArg8_inline());
            g_evtCurObj->battleTurnState = 0;
            break;
        }
        }
        return 0;
    case EVT_OP_BTLSEQ_START: {
        s32 x = (u16)g_evtBattleVars[1], z = (u16)g_evtBattleVars[0];
        btlseq_startDirect(x, z, evt_getArg8_inline());
        return 0;
    }
    case EVT_OP_SET_BATTLE_VAR:
        operand0 = evt_getArg8_inline();
        btl_setBattleVar(operand0, evt_getArg16_inline());
        return 0;
    case EVT_OP_BTL_KEEP_MUSIC:
        if (evt_getArg8_inline()) {
            g_gameState->gameData->battleFlags |= GD_BTL_KEEP_MUSIC;
        } else {
            g_gameState->gameData->battleFlags &= ~GD_BTL_KEEP_MUSIC;
        }
        return 0;
    case EVT_OP_REWARDS_CLOSE:
        rewards_requestClose();
        g_evtHalted = 1;
        return 0;
    case EVT_OP_SET_CHR_NAME:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)operand0 < CHR_COUNT) {
            u8 *src = msg_getText(evt_getArg16_inline());
            u8 *dst = g_gameState->gameData->characters[operand0].name;
            u8 *end = dst + 9;
            while (dst < end) {
                *dst++ = *src++;
            }
            *dst = TEXT_END;
        } else {
            evt_getArg16_inline();
        }
        return 0;
    case EVT_OP_SET_VICTORY_POSE:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)operand0 < CHR_COUNT) {
            g_gameState->gameData->characters[operand0].info.w = (g_gameState->gameData->characters[operand0].info.w & ~CHR_INFO_VICTORY_POSE) | ((evt_getArg8_inline() & 1) << CHR_INFO_VICTORY_POSE_BIT);
        }
        return 0;
    case EVT_OP_SET_DIR_OFFSETS:
        g_evtState->analogDirOfs = evt_getArg8_inline() << 4;
        g_evtState->padDirOfs = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_CLEAR_COLOR:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        game_setClearColor(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_MOVE_ORIGIN:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        fldbg_moveOrigin(operand0, operand1, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_SCROLL_TO:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg8_inline();
        fldbg_scrollTo(operand0, operand1, operand2, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_SCROLL_TO_PLAYER:
        operand0 = evt_getArg8_inline();
        fldbg_scrollToPlayer(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_FOLLOW:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        fldbg_setFollow(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_FOLLOW_HEIGHT:
        operand0 = evt_getArg16_inline();
        fldbg_setFollowHeight(operand0);
        return 0;
    case EVT_OP_BG_LIMITS_ON:
        fldbg_enableScrollLimits();
        return 0;
    case EVT_OP_BG_LIMITS_OFF:
        fldbg_disableScrollLimits();
        return 0;
    case EVT_OP_BG_LAYER_COLOR:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        fldbg_setOverlayColor(operand0, operand1, operand2, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_LAYER_MOVE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        fldbg_moveOverlay(operand0, operand1, operand2, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_SHOW:
        operand0 = evt_getArg8_inline();
        fldbg_showOverlay(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_LAYER_LOOP:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg16_inline();
        fldbg_setOverlayLoop(operand0, operand1, operand2, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_PARALLAX:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg16_inline();
        fldbg_setOverlayParallax(operand0, operand1, operand2, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_POS:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        fldbg_setOverlayPos(operand0, operand1, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_FOLLOW_LAYER:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg8_inline();
        operand5 = evt_getArg8_inline();
        {
            s32 red = evt_getArg8_inline();
            operand4 = evt_getArg8_inline();
            fldbg_addFollowOverlay(operand0, operand1, operand2, operand5, red, operand4, evt_getArg8_inline());
        }
        return 0;
    case EVT_OP_BG_OVERLAY_SCREEN:
        operand0 = evt_getArg8_inline();
        fldbg_setOverlayScreen(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_FRAME:
        operand0 = evt_getArg8_inline();
        fldbg_setAnimFrame(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_PLAY:
        operand0 = evt_getArg8_inline();
        fldbg_playAnim(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_SPEED:
        operand0 = evt_getArg8_inline();
        fldbg_setAnimSpeed(operand0, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_ANIM_DELAY:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        fldbg_setAnimStepDelay(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_FLAGS:
        operand0 = evt_getArg8_inline();
        fldbg_setAnimFlags(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_RANGE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        fldbg_playAnimRange(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_RESET:
        operand0 = evt_getArg8_inline();
        fldbg_resetAnim(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_SHOW_FRAME:
        operand0 = evt_getArg8_inline();
        fldbg_showAnimFrame(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_TRI_ACTIVE:
        operand0 = evt_getArg16_inline();
        bgi_setTriangleActive(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_FLOOR_ACTIVE:
        operand0 = evt_getArg8_inline();
        bgi_setFloorActive(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_ACTOR_ACTIVE:
        operand0 = evt_getArg8_inline();
        if (po->modelId != EVT_MODEL_NONE) {
            bgi_setActorActive(po->modelSlot, operand0);
        }
        return 0;
    case EVT_OP_BG_MASK_FILTER:
        operand0 = evt_getArg8_inline();
        bgi_setMaskFilter(operand0);
        return 0;
    case EVT_OP_BG_SCROLL_LIMITS:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        operand3 = evt_getArg16_inline();
        fldbg_setScrollLimits(operand0, operand1, operand2, operand3, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_VIEWPORT:
        operand0 = evt_getArg8_inline();
        fldbg_setOverlayViewport(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_VIEWPORT:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        operand3 = evt_getArg16_inline();
        fldbg_setViewport(operand0, operand1, operand2, operand3, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_VIEWPORT_RATE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        fldbg_setViewportScrollRate(operand0, operand1, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_SKEW:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg16_inline();
        operand3 = evt_getArg16_inline();
        fldbg_setOverlaySkewLoop(operand0, operand1, operand2, operand3, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_CAMERA:
        operand0 = evt_getArg8_inline();
        fldbg_setCamera(operand0);
        return 0;
    case EVT_OP_GET_SCROLL: {
        s16 x, z;
        fldbg_getScroll(&x, &z);
        g_evtScreenX = x;
        g_evtScreenY = z;
        return 0;
    }
    case EVT_OP_OBJ_COLOR:
        obj = evt_getObjectArg();
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        if (fld_setObjectColor(obj->modelSlot, operand0, operand1, evt_getArg8_inline())) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
            return 1;
        }
        return 0;
    case EVT_OP_MODEL_CMD:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg16_inline();
        fld_modelCommand(po->modelSlot, operand0, operand1, operand2, evt_getArg16_inline());
        return 0;
    case EVT_OP_MESH_HIDE:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        operand0 = evt_getArg8_inline();
        if (mdl != NULL) {
            mdl->hiddenGroups |= 1 << operand0;
        }
        return 0;
    case EVT_OP_MESH_SHOW:
        argObj = evt_getObjectArg();
        mdl = evt_getObjectModelData_inline(argObj);
        operand0 = evt_getArg8_inline();
        if (mdl != NULL) {
            mdl->hiddenGroups &= ~(1 << operand0);
        }
        return 0;
    case EVT_OP_PLAYER_BUBBLE:
        msg_showPlayerBubble(evt_getArg8_inline());
        return 0;
    case EVT_OP_ATE_PROMPT:
        msg_setAtePrompt(evt_getArg8_inline());
        return 0;
    case EVT_OP_HERE_ICON:
        msg_startHereIcon(evt_getArg8_inline());
        return 0;
    case EVT_OP_IDLE_OFF:
        g_evtState->idleTimer = -1;
        return 0;
    case EVT_OP_IDLE_TIMER_RESET:
        g_evtState->idleTimer = 0;
        return 0;
    case EVT_OP_PAD_MASK_SET:
        operand0 = evt_getArg8_inline();
        g_padPortMask[operand0] |= evt_getArg16_inline();
        return 0;
    case EVT_OP_PAD_MASK_CLEAR:
        operand0 = evt_getArg8_inline();
        g_padPortMask[operand0] &= ~evt_getArg16_inline();
        return 0;
    case EVT_OP_SOUND:
        operand0 = evt_getArg16_inline();
        g_gameState->gameData->soundHook(operand0, evt_getArg16_inline(), 0, 0, 0);
        return 0;
    case EVT_OP_SOUND_ARG:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg24_inline();
        g_gameState->gameData->soundHook(operand0, operand1, operand2, 0, 0);
        return 0;
    case EVT_OP_SOUND_ARG_PAN:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg24_inline();
        g_gameState->gameData->soundHook(operand0, operand1, operand2, evt_getArg8_inline(), 0);
        return 0;
    case EVT_OP_SOUND_ARG_PAN_VOL:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        operand2 = evt_getArg24_inline();
        operand3 = evt_getArg8_inline();
        g_gameState->gameData->soundHook(operand0, operand1, operand2, operand3, evt_getArg8_inline());
        return 0;
    case EVT_OP_SOUND_POS: {
        u8 vol, pan;
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg16_inline();
        fld_getSoundPanVolume(operand0, operand1, evt_getArg16_inline(), &vol, &pan);
        g_evtSoundVolume = vol;
        g_evtSoundPan = (pan * evt_getArg8_inline()) >> 7;
        if (g_evtSoundPan >= 0x80) {
            g_evtSoundPan = 0x7F;
        }
        return 0;
    }
    case EVT_OP_SOUND_POS_OBJ: {
        u8 vol, pan;
        po = evt_getObjectArg();
        fld_getSoundPanVolume(po->pos.vx, po->pos.vy, po->pos.vz, &vol, &pan);
        g_evtSoundVolume = vol;
        g_evtSoundPan = (pan * evt_getArg8_inline()) >> 7;
        if (g_evtSoundPan >= 0x80) {
            g_evtSoundPan = 0x7F;
        }
        return 0;
    }
    case EVT_OP_SPRITE_CMD:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg16_inline();
        operand3 = evt_getArg16_inline();
        fld_spriteCommand(operand0, operand1, operand2, operand3, evt_getArg16_inline());
        return 0;
    case EVT_OP_SPRITE_CMD_U8:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        operand3 = evt_getArg16_inline();
        fld_spriteCommand(operand0, operand1, operand2, operand3, evt_getArg16_inline());
        return 0;
    case EVT_OP_MENU:
        operand0 = evt_getArg8_inline();
        menu_open(operand0, evt_getArg8_inline());
        evt_onMenuOpen();
        return 1;
    case EVT_OP_DISC_CHANGE:
        operand0 = evt_getArg16_inline();
        fld_requestDiscChange(EVT_DISC_CHANGE_DISC(operand0), EVT_DISC_CHANGE_LOCATION(operand0));
        return 1;
    case EVT_OP_PAD_UNLOCK_MENU:
        g_padPortMask[0] &= ~BTN_TRIANGLE;
        return 0;
    case EVT_OP_PAD_LOCK_MENU:
        g_padPortMask[0] |= BTN_TRIANGLE;
        return 1;
    case EVT_OP_ADD_ITEM:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg8_inline();
        if (operand0 < ITEM_ID_KEY_FIRST) {
            item_add(operand0, operand1);
        } else if (operand0 < ITEM_ID_CARD_FIRST) {
            keyItem_add(operand0 - ITEM_ID_KEY_FIRST);
        } else {
            while (--operand1 != -1) {
                card_add(operand0 - ITEM_ID_CARD_FIRST);
            }
        }
        return 0;
    case EVT_OP_REMOVE_ITEM:
        operand0 = evt_getArg16_inline();
        operand1 = evt_getArg8_inline();
        if (operand0 < ITEM_ID_KEY_FIRST) {
            item_remove(operand0, operand1);
        } else if (operand0 < ITEM_ID_CARD_FIRST) {
            keyItem_remove(operand0 - ITEM_ID_KEY_FIRST);
        }
        return 0;
    case EVT_OP_ADD_GIL:
        if ((g_gameState->gameData->gil += evt_getArg24_inline()) > GIL_MAX) {
            g_gameState->gameData->gil = GIL_MAX;
        }
        return 0;
    case EVT_OP_REMOVE_GIL:
        if ((g_gameState->gameData->gil -= evt_getArg24_inline()) > GIL_MAX) {
            g_gameState->gameData->gil = 0;
        }
        return 0;
    case EVT_OP_SET_RECRUITED:
        operand0 = evt_getArg16_inline();
        operand0 |= EVT_CHR_ALIAS_BITS(operand0);
        for (i = 0; i < CHR_COUNT; i++) {
            chr_clearRecruited(i);
        }
        for (i = 0; i < CHR_COUNT; i++) {
            if (operand0 & 1) {
                chr_setRecruited(i);
            }
            operand0 >>= 1;
        }
        return 0;
    case EVT_OP_PARTY_JOIN:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        operand1 = evt_getArg8_inline();
        operand2 = evt_getArg8_inline();
        if ((u32)operand0 < CHR_COUNT) {
            Character *ch = &g_gameState->gameData->characters[operand0];
            operand3 = evt_getArg8_inline();
            if (operand3 != 0xFF) {
                ch->classFlags = operand3;
            }
            operand3 = evt_getArg8_inline();
            if (operand3 != 0xFF) {
                ch->info.w = (ch->info.w & ~CHR_INFO_ROW) | ((operand3 & 0x1F) << CHR_INFO_ROW_SHIFT);
            }
            chr_join(operand0, operand1 & 0xFF, operand2);
        } else {
            evt_getArg8_inline();
            evt_getArg8_inline();
        }
        return 0;
    case EVT_OP_PARTY_REMOVE:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        for (i = 0; i < PARTY_SIZE; i++) {
            Character *ch = g_gameState->gameData->party[i];
            if (ch != NULL && (ch->info.w & CHR_INFO_ID) == operand0) {
                break;
            }
        }
        if (i < PARTY_SIZE) {
            party_setMember(i, -1);
            evt_execInstr_party();
        }
        return 0;
    case EVT_OP_PARTY_MENU:
        g_evtPartyRequest.size = evt_getArg8_inline();
        operand0 = 0;
        for (i = CHR_COUNT - 1; i >= 0; i--) {
            operand0 <<= 1;
            operand0 |= (g_gameState->gameData->characters[i].info.w >> CHR_INFO_RECRUITED_BIT) & 1;
        }
        operand1 = evt_getArg16_inline();
        for (i = 0; i < CHR_COUNT; i++) {
            g_evtPartyRequest.fixed[i] = operand1 & 1;
            operand1 >>= 1;
        }
        for (i = 0; i < PARTY_SIZE; i++) {
            Character *ch = g_gameState->gameData->party[i];
            if (ch != NULL) {
                operand1 = ch->info.w & CHR_INFO_ID;
                operand0 &= ~(1 << operand1);
                g_evtPartyRequest.party[i] = operand1;
            } else {
                g_evtPartyRequest.party[i] = 0xFF;
            }
        }
        for (operand1 = 0, i = 0; i < CHR_COUNT && operand1 < 8 && operand0 != 0; i++, operand0 >>= 1) {
            if (operand0 & 1) {
                g_evtPartyRequest.reserve[operand1++] = i;
            }
        }
        for (; operand1 < 8; operand1++) {
            g_evtPartyRequest.reserve[operand1] = 0xFF;
        }
        menu_open(MENU_PARTY, (s32)&g_evtPartyRequest);
        return 1;
    case EVT_OP_PARTY_UPDATE_UIDS:
        evt_execInstr_party();
        return 0;
    case EVT_OP_SET_HP:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)operand0 < CHR_COUNT) {
            Character *ch = &g_gameState->gameData->characters[operand0];
            operand0 = evt_getArg16_inline();
            if (ch->maxHp < operand0) {
                operand0 = ch->maxHp;
            }
            ch->hp = operand0;
        }
        return 0;
    case EVT_OP_SET_MP:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)operand0 < CHR_COUNT) {
            Character *ch = &g_gameState->gameData->characters[operand0];
            operand0 = evt_getArg16_inline();
            if (ch->maxMp < operand0) {
                operand0 = ch->maxMp;
            }
            ch->mp = operand0;
        }
        return 0;
    case EVT_OP_RESET_AP:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        chr_resetAp(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_LEARN_ABILITY:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        chr_learnAbility(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_CURE_STATUS:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        chr_cureStatusBit(&g_gameState->gameData->characters[operand0], evt_getArg8_inline() & 0xFF);
        return 0;
    case EVT_OP_SET_ROW:
        operand0 = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)operand0 < CHR_COUNT) {
            g_gameState->gameData->characters[operand0].info.w = (g_gameState->gameData->characters[operand0].info.w & ~CHR_INFO_BATTLE_ROW) | ((evt_getArg8_inline() & 1) << CHR_INFO_BATTLE_ROW_BIT);
        }
        return 0;
    case EVT_OP_ADD_FROG:
        if (g_gameState->gameData->frogCount < 0x7FFF) {
            g_gameState->gameData->frogCount++;
        }
        return 0;
    case EVT_OP_SET_LOCATION_NAME: {
        u8 *src = msg_getText(evt_getArg16_inline());
        u8 *dst = g_evtLocationNameBuf;
        u8 *end = dst + 0xF;
        while (dst < end) {
            *dst++ = *src++;
        }
        *dst = TEXT_END;
        g_gameState->gameData->locationName = g_evtLocationNameBuf;
        return 0;
    }
    case EVT_OP_RESET_LOCATION_NAME:
        g_gameState->gameData->locationName = g_evtLocationName;
        return 0;
    case EVT_OP_TIMER_SET:
        timer_setSeconds(evt_getArg16_inline());
        return 0;
    case EVT_OP_TIMER_ENABLE:
        timer_enable(evt_getArg8_inline());
        return 0;
    case EVT_OP_TIMER_RUN:
        timer_setRunning(evt_getArg8_inline());
        return 0;
    case EVT_OP_FADE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        fade_clearEase(operand0 & 1);
        fade_setAngle(((0x80 - evt_getArg8_inline()) << 4) & 0xFFF0);
        operand2 = evt_getArg8_inline();
        operand3 = evt_getArg8_inline();
        fade_setOtz(1);
        game_startFade(((operand0 >> 2) & 1) + 1, (operand0 >> 1) & 1, operand1, operand2 | (operand3 << 8) | (evt_getArg8_inline() << 16));
        return 0;
    case EVT_OP_OBJ_SCREEN_POS:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            sv.vx = obj->pos.vx;
            sv.vy = obj->pos.vy;
            sv.vz = obj->pos.vz;
            evt_projectToScreen(&sv, &g_evtScreenX, &g_evtScreenY);
        }
        return 0;
    case EVT_OP_MOVIE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg16_inline();
        fld_movieCommand(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_WM_CAMERA_EYE_ACTOR:
        if (g_evtMode == EVT_MODE_WORLD) {
            VECTOR *camEye = wm_getCameraEye();
            actor->pos.vx = camEye->vx;
            actor->pos.vy = camEye->vy;
            actor->pos.vz = camEye->vz;
            actor->moveFlags |= EVT_MOVE_WM_CAMERA_EYE;
        }
        return 0;
    case EVT_OP_WM_CAMERA_TARGET_ACTOR:
        if (g_evtMode == EVT_MODE_WORLD) {
            VECTOR *camTarget = wm_getCameraTarget();
            actor->pos.vx = camTarget->vx;
            actor->pos.vy = camTarget->vy;
            actor->pos.vz = camTarget->vz;
            actor->moveFlags |= EVT_MOVE_WM_CAMERA_TARGET;
        }
        return 0;
    case EVT_OP_WM_SET_PARAM:
        operand0 = evt_getArg8_inline();
        w_frameSetParameter(operand0, evt_getArg16_inline());
        return 0;
    case EVT_OP_CLEAR_GLOBALS:
        evt_clearGlobalVars();
        return 0;
    case EVT_OP_VIB_PLAY:
        vib_play(evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_SET_PLAYING:
        vib_setPlaying(evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_TRACK:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        vib_selectTrack(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_TRACK_ENABLE:
        operand0 = evt_getArg8_inline();
        operand1 = evt_getArg8_inline();
        vib_setTrackEnabled(operand0, operand1, evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_RATE:
        vib_setRate(evt_getArg16_inline());
        return 0;
    case EVT_OP_VIB_LOOP_MODE:
        vib_setLoopMode(evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_RANGE:
        operand0 = evt_getArg8_inline();
        vib_setRange(operand0, evt_getArg8_inline());
        return 0;
    case EVT_OP_PRELOAD:
        operand0 = evt_getArg8_inline();
        ((s32 (*)(s32, s32))g_gameState->gameData->preloadRequest)(operand0 & 0xFF, evt_getArg16_inline() & 0xFFFF);
        return 0;
    case EVT_OP_EXT:
        /* padding between the two jump tables in the original rodata */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        op = g_evtArgFlags | 0xFF00;
        g_evtArgFlags = *g_evtCurObj->ip++;
        switch (op) {
        case EVT_OP_BGI_FLOOR_MOVE_START:
            operand0 = evt_getArg8_inline();
            bgi_startFloorMove(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_STOP:
            operand0 = evt_getArg8_inline();
            bgi_stopFloorMove(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_PLAYING:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMovePlaying(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_MODE:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMoveMode(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_FLOOR:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMoveFloor(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_SPEED:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMoveSpeed(operand0, evt_getArg16_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_CURVE:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMoveCurve(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_RANGE:
            operand0 = evt_getArg8_inline();
            operand1 = evt_getArg16_inline();
            bgi_setFloorMoveRange(operand0, operand1, evt_getArg16_inline());
            return 0;
        case EVT_OP_BGI_FLOOR_MOVE_AXIS:
            operand0 = evt_getArg8_inline();
            bgi_setFloorMoveAxis(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_FRAME:
            operand0 = evt_getArg8_inline();
            bgi_setAnimFrame(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_SHOW_FRAME:
            operand0 = evt_getArg8_inline();
            bgi_showAnimFrame(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_PLAYING:
            operand0 = evt_getArg8_inline();
            bgi_setAnimPlaying(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_MODE:
            operand0 = evt_getArg8_inline();
            bgi_setAnimMode(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_RATE:
            operand0 = evt_getArg8_inline();
            bgi_setAnimRate(operand0, evt_getArg16_inline());
            return 0;
        case EVT_OP_BGI_ANIM_FRAME_VALUES:
            operand0 = evt_getArg8_inline();
            bgi_setAnimFrameValues(operand0, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_FRAME_VALUE:
            operand0 = evt_getArg8_inline();
            operand1 = evt_getArg8_inline();
            bgi_setAnimFrameValue(operand0, operand1, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_RANGE:
            operand0 = evt_getArg8_inline();
            operand1 = evt_getArg8_inline();
            bgi_playAnimRange(operand0, operand1, evt_getArg8_inline());
            return 0;
        case EVT_OP_BGI_ANIM_RESET:
            operand0 = evt_getArg8_inline();
            bgi_resetAnim(operand0, evt_getArg8_inline());
            return 0;
        }
        return 0;
    default:
        switch (g_evtMode) {
        case EVT_MODE_FIELD:
            return evt_modelOpField(po, op);
        case EVT_MODE_BATTLE:
            return evt_modelOpBattle(po, op);
        case EVT_MODE_WORLD:
            return evt_modelOpWorld(po, op);
        }
        return 1;
    }
}

void evt_movePlayer(EvtObject *obj) {
    SVECTOR step;
    u8 dz[4];
    s32 dist;
    s32 speed;
    s32 dir;
    s32 analog;
    s32 pad;
    s32 x, y;
    s32 run;
    s32 cur, diff;
    s32 hitAngle, k;
    EvtObject *hit;
    s32 magSq;

    hit = NULL;
    speed = EVT_PLAYER_WALK_SPEED;
    dir = EVT_DIR_NONE;
    analog = 0;
    memset(&step, 0, 8);
    pad = msg_getPadHeld();
    if (PAD_TYPE(g_padBuf[0].hdr) == PAD_ID_ANALOG) {
        x = g_padAnalog[0].b[2];
        y = g_padAnalog[0].b[3];
        pad_getAnalogThresholds((Unk4 *)dz);
        /* The do-while(0) (a macro in the original?) doubles y's reference count in flow,
           which puts y ahead of x in global-alloc order (y s0, x s1). */
        if (x < dz[2] || dz[3] < x) {
            analog = 1;
        } else {
            do {
                if (y < dz[0] || dz[1] < y) {
                    analog = 1;
                }
            } while (0);
        }
        x -= 0x80;
        if (analog) {
            y = 0x80 - y;
            dir = math_atan2(x, y);
            x *= x;
            magSq = y * y;
            if (magSq < x) {
                magSq = x;
            }
            speed = (magSq * 16 - magSq) * 4 >> 14;
        }
    }
    if (!analog) {
        run = ((GameConfig *)&g_gameState->config)->movementRun;
        if ((pad & BTN_CIRCLE) ? (run ^ 1) != 0 : run != 0) {
            speed = EVT_PLAYER_RUN_SPEED;
        }
        if (pad & PAD_LEFT) {
            dir = 0x400;
        }
        if (pad & PAD_RIGHT) {
            dir = -0x400;
        }
        if (pad & PAD_DOWN) {
            if (dir == EVT_DIR_NONE) {
                dir = 0;
            } else {
                dir -= dir >> 1;
            }
        }
        if (pad & PAD_UP) {
            if (dir == EVT_DIR_NONE) {
                dir = 0x800;
            } else {
                dir += dir >> 1;
            }
        }
    }
    g_evtPlayerMoved = dir != EVT_DIR_NONE;
    if (dir != EVT_DIR_NONE) {
        if (analog) {
            dir += g_evtState->analogDirOfs;
        } else {
            dir += g_evtState->padDirOfs;
        }
        dir &= 0xFFF;
        cur = obj->pos.rotY & 0xFFF;
        diff = dir - cur;
        if (diff < -0x7FF) {
            cur -= 0x1000;
        } else if (diff >= 0x800) {
            cur += 0x1000;
        }
        obj->pos.rotY = ((cur + dir) >> 1) & 0xFFF;
        if (g_evtState->walkOnly != 0 && speed > EVT_PLAYER_WALK_SPEED) {
            speed = EVT_PLAYER_WALK_SPEED;
        }
        if (speed > EVT_PLAYER_WALK_SPEED) {
            g_evtStepCount += 2;
        } else {
            g_evtStepCount += 1;
        }
        obj->speed = speed;
        step.vz = -speed;
        math_rotateSVector(&step, 0, dir);
        obj->pos.vx += step.vx;
        obj->pos.vz += step.vz;
        hit = evt_findNearestObject(obj, 0, &dist);
        g_evtPushable = ((PSX_NULLABLE(hit)->flags >> 4) ^ 1) & 1;
        if (g_evtPushable == 0) {
            g_evtPushFrames = 0;
        }
        if (hit != NULL && dist <= 0 && g_evtPushFrames >= 0) {
            obj->collidedObject = hit;
            obj->collisionDepth = dist;
            hitAngle = evt_angleToObject(obj, hit, dir);
            if ((u32)(hitAngle + 0x400) <= 0x800) {
                obj->pos.vx -= step.vx;
                obj->pos.vz -= step.vz;
                k = math_sin(hitAngle);
                step.vx = (step.vx * k) >> 13;
                step.vz = (step.vz * k) >> 13;
                math_rotateSVector(&step, 0, 0x400);
                obj->pos.vx += step.vx;
                obj->pos.vz += step.vz;
                if (evt_findNearestObject(obj, 0, &dist) != NULL && dist <= 0) {
                    obj->pos.vx -= step.vx;
                    obj->pos.vz -= step.vz;
                }
                g_evtPlayerBlocked = 2;
            }
        }
    } else {
        /* No code: flow deletes the dead store and jump2 the empty test, but the test keeps
           hit = NULL live up to here (the s3 zero that analog and memset's argument copy), and
           its loop-weighted references put hit ahead of obj in global-alloc order. */
        do {
            if (hit != NULL && hit != obj) {
                hit = NULL;
            }
        } while (0);
    }
    g_evtPushFrames = g_evtPlayerBlocked != 0 ? (g_evtPushFrames < EVT_PUSH_FRAMES ? g_evtPushFrames + g_evtPushable : -EVT_PUSH_FRAMES)
                                 : (g_evtPushFrames < 0 ? g_evtPushFrames + 1 : 0);
}

s32 evt_callTurnScript(EvtObject *obj, s32 angle) {
    s32 *sp;
    s32 ok;
    s32 x;
    u32 hi;
    s32 w;

    if (!(obj->moveFlags & EVT_MOVE_LOOK_RELEASED)) {
        ok = 0;
        if (!(obj->animFlags & EVT_ANIM_SCRIPTED) && obj->talkTurnScript == EVT_OP_TURN_TO_ANGLE) {
            ok = 0 < obj->level;
        }
        if (ok) {
            obj->talkTurnAngle = angle >> 4;
            sp = &((s32 *)obj + obj->stackOfs)[obj->sp];
            sp[0] = obj->ip - g_evtState->scriptBase;
            w = obj->wait | (obj->level << 8);
            hi = g_evtCurObj->uid << 24;
            x = hi | (EVT_FRAME_NO_WAIT << 16);
            sp[1] = w | x;
            obj->ip = (u8 *)&obj->talkTurnScript;
            obj->level = 0;
            obj->wait = 0;
            obj->sp += 2;
        }
    }
    return 1;
}

/* point 500 units ahead of obj at angle a, at head height */
static inline void evt_updateLookAt_ahead(EvtObject *obj, s32 angle, VECTOR *out) {
    SVECTOR ofs;
    MATRIX mtx;

    mtx.m[0][2] = math_sin(angle);
    mtx.m[1][1] = ONE;
    mtx.m[2][2] = math_cos(angle);
    mtx.m[0][0] = mtx.m[0][1] = mtx.m[1][0] = mtx.m[1][2] = mtx.m[2][0] = mtx.m[2][1] = 0;
    mtx.t[0] = obj->pos.vx;
    mtx.t[1] = obj->pos.vy;
    mtx.t[2] = obj->pos.vz;
    SetRotMatrix(&mtx);
    SetTransMatrix(&mtx);
    ofs.vx = 0;
    ofs.vy = (obj->height * (s8)obj->scale) >> 6;
    ofs.vz = -EVT_LOOK_AHEAD;
    gte_ldv0(&ofs);
    gte_rtv0tr();
    gte_stlvnl(out);
}

void evt_relaxLookAt(EvtObject *obj, Unk8004AFF0 *mdl) {
    VECTOR out;
    s32 dist;

    evt_updateLookAt_ahead(obj, obj->pos.rotY, &out);
    dist = math_length3(out.vx - obj->lookAtX, out.vy - obj->lookAtY, out.vz - obj->lookAtZ);
    if (dist < EVT_LOOK_STEP) {
        dist = EVT_LOOK_STEP;
        obj->moveFlags &= ~(EVT_MOVE_LOOK_RELEASED | EVT_MOVE_LOOKED_AT_SPEAKER | EVT_MOVE_LOOKING);
    }
    obj->lookAtX += (out.vx - obj->lookAtX) * EVT_LOOK_STEP / dist;
    obj->lookAtY += (out.vy - obj->lookAtY) * EVT_LOOK_STEP / dist;
    obj->lookAtZ += (out.vz - obj->lookAtZ) * EVT_LOOK_STEP / dist;
    mdl->flags |= MDL_LOOK_AT;
    mdl->lookAtX = obj->lookAtX;
    mdl->lookAtY = obj->lookAtY;
    mdl->lookAtZ = obj->lookAtZ;
}

/* move the look-at point towards target */
static inline void evt_updateLookAt_approach(EvtObject *obj, Unk8004AFF0 *mdl, VECTOR *target) {
    s32 dist;

    dist = math_length3(target->vx - obj->lookAtX, target->vy - obj->lookAtY, target->vz - obj->lookAtZ);
    if (dist < EVT_LOOK_STEP) {
        dist = EVT_LOOK_STEP;
    }
    obj->lookAtX += (target->vx - obj->lookAtX) * EVT_LOOK_STEP / dist;
    obj->lookAtY += (target->vy - obj->lookAtY) * EVT_LOOK_STEP / dist;
    obj->lookAtZ += (target->vz - obj->lookAtZ) * EVT_LOOK_STEP / dist;
    mdl->flags |= MDL_LOOK_AT;
    mdl->lookAtX = obj->lookAtX;
    mdl->lookAtY = obj->lookAtY;
    mdl->lookAtZ = obj->lookAtZ;
}

/* is the look-at point still in front of obj (within ~80 degrees)? */
static inline s32 evt_updateLookAt_infront(EvtObject *obj) {
    s32 ok = 1;
    s32 diff;

    if (!(obj->moveFlags & EVT_MOVE_LOOK_RELEASED)) {
        diff = (math_atan2(obj->lookAtX - obj->pos.vx, obj->lookAtZ - obj->pos.vz) - obj->pos.rotY) & 0xFFF;
        if (diff > 0x800) {
            diff -= 0x1000;
        }
        ok = (u32)(diff + EVT_LOOK_FRONT - 1) < 2 * EVT_LOOK_FRONT - 1;
        if (!ok) {
            obj->moveFlags &= ~EVT_MOVE_LOOKING;
        }
    }
    return ok;
}

static inline s32 evt_updateLookAt_infront2(EvtObject *obj, s32 *pa) {
    s32 ok = 1;
    s32 angle;
    s32 diff;

    if (!(obj->moveFlags & EVT_MOVE_LOOK_RELEASED)) {
        angle = math_atan2(obj->lookAtX - obj->pos.vx, obj->lookAtZ - obj->pos.vz);
        diff = (angle - obj->pos.rotY) & 0xFFF;
        if (diff > 0x800) {
            diff -= 0x1000;
        }
        ok = (u32)(diff + EVT_LOOK_FRONT - 1) < 2 * EVT_LOOK_FRONT - 1;
        if (!ok) {
            obj->moveFlags &= ~EVT_MOVE_LOOKING;
        }
        *pa = angle;
    }
    return ok;
}

void evt_updateLookAt(EvtObject *obj) {
    VECTOR point;
    s32 ang;
    s32 save;
    s32 x, y, z;
    Unk8004AFF0 *mdl;
    EvtObject *tgt;
    EvtObject *best;
    EvtObject *other;
    EvtListNode *node;
    u8 lookMask;
    s32 bestd;
    s32 score;
    s32 relAngle;
    s32 angle;
    s32 ok;

    save = 0;
    mdl = (Unk8004AFF0 *)((ModelSlot *)game_getModel(obj->modelSlot))->model;
    tgt = NULL;
    if ((obj->moveFlags & EVT_MOVE_LOOK_AT_SPEAKER) && g_evtLookTarget != NULL && g_evtLookTarget != obj &&
        math_dist2(obj->pos.vx - g_evtLookTarget->pos.vx, obj->pos.vz - g_evtLookTarget->pos.vz, 0) >= EVT_LOOK_MIN_DIST) {
        tgt = g_evtLookTarget;
        obj->moveFlags |= EVT_MOVE_LOOKED_AT_SPEAKER;
    }
    if (tgt != NULL || ((obj->moveFlags & EVT_MOVE_LOOK_AT_ACTORS) && !(obj->animFlags & EVT_ANIM_IDLE))) {
        if (obj->flags & EVT_OBJ_TURNING) {
            save = obj->pos.rotY;
            obj->pos.rotY = obj->turnStartDir;
        }
        if (tgt == NULL) {
            best = NULL;
            x = obj->pos.vx;
            y = obj->pos.vy;
            z = obj->pos.vz;
            lookMask = obj->lookParam1;
            bestd = 0;
            for (node = g_evtState->objects; node != NULL; node = node->next) {
                other = node->obj;
                if (other != obj && other->kind == EVT_KIND_ACTOR && (u32)(other->pos.vy - y + EVT_LOOK_MAX_DY) < 2 * EVT_LOOK_MAX_DY + 1 && (other->moveFlags & EVT_MOVE_LOOK_TARGET) &&
                    (lookMask & other->lookParam0)) {
                    score = math_distSq2(other->pos.vx - x, other->pos.vz - z, 0);
                    if (score >= EVT_LOOK_MIN_DIST) {
                        angle = evt_angleToObject(obj, other, obj->pos.rotY);
                        score -= EVT_LOOK_SCORE_BIAS;
                        if (angle < 0) {
                            angle = -angle;
                        }
                        angle -= EVT_LOOK_CONE;
                        if (angle < 0) {
                            score += angle;
                            if (score < bestd) {
                                bestd = score;
                                best = other;
                            }
                        }
                    }
                }
            }
            tgt = best;
        }
        if (tgt != NULL) {
            if (!(obj->moveFlags & EVT_MOVE_LOOKING)) {
                if (!(obj->flags & EVT_OBJ_TURNING)) {
                    obj->moveFlags |= EVT_MOVE_LOOKING;
                    evt_updateLookAt_ahead(obj, obj->pos.rotY, &point);
                    obj->lookAtX = point.vx;
                    obj->lookAtY = point.vy;
                    obj->lookAtZ = point.vz;
                }
            }
            if ((obj->moveFlags & EVT_MOVE_LOOKING) && evt_updateLookAt_infront(obj)) {
                {
                    point.vx = tgt->pos.vx;
                    point.vy = tgt->pos.vy;
                    point.vz = tgt->pos.vz;
                    ang = math_atan2(point.vx - obj->pos.vx, point.vz - obj->pos.vz);
                    relAngle = (ang - obj->pos.rotY) & 0xFFF;
                    if (relAngle > 0x800) {
                        relAngle -= 0x1000;
                    }
                    if ((u32)(relAngle + EVT_LOOK_CONE - 1) < 2 * EVT_LOOK_CONE - 1) {
                        point.vy = (((obj->pos.vy << 6) + obj->height * (s8)obj->scale) * 3 + (point.vy << 6) +
                                tgt->height * (s8)tgt->scale) >> 8;
                        evt_updateLookAt_approach(obj, mdl, &point);
                    } else if (evt_callTurnScript(obj, ang)) {
                        angle = relAngle > EVT_LOOK_CONE ? EVT_LOOK_CONE : -EVT_LOOK_CONE;
                        evt_updateLookAt_ahead(obj, angle + obj->pos.rotY, &point);
                        evt_updateLookAt_approach(obj, mdl, &point);
                    }
                }
            }
        } else if ((obj->moveFlags & EVT_MOVE_LOOKING) && evt_updateLookAt_infront2(obj, &ang) && evt_callTurnScript(obj, ang)) {
            {
                evt_relaxLookAt(obj, mdl);
            }
        }
        if (obj->flags & EVT_OBJ_TURNING) {
            obj->pos.rotY = save;
        }
    } else if ((obj->moveFlags & EVT_MOVE_LOOKING) && evt_updateLookAt_infront(obj)) {
        evt_relaxLookAt(obj, mdl);
    }
    if (!(obj->moveFlags & EVT_MOVE_LOOKING)) {
        mdl->flags &= ~MDL_LOOK_AT;
    }
}

void evt_updateIdleTimer(void) {
    EvtState *evt = g_evtState;
    EvtObject *player;
    EvtObject *obj;
    EvtState *e2;

    if (evt == NULL || evt->controlEnabled == 0) {
        return;
    }
    evt->idleTimer -= evt->idleTimer > 0;
    if (evt->idleTimer != 0) {
        return;
    }
    obj = evt_getObject_inline(evt->playerUid);
    if (obj == NULL || obj->kind != EVT_KIND_ACTOR) {
        return;
    }
    player = obj;
    if (player->animFrame != player->animFrames - 1) {
        return;
    }
    e2 = g_evtState;
    if (e2->idleTimer >= 0) {
        e2->idleTimer = (rand8_inline() + 0xC8) * 2;
    }
    if (player->idleAnim != 0 && !(player->animFlags & (EVT_ANIM_SCRIPTED | EVT_ANIM_HELD))) {
        evt_updateIdleTimer_idle(player);
        if (player->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        player->animFlags |= EVT_ANIM_IDLE;
    }
}

/* Nearest kind-3 object whose script has entry `key` and that evt_isInRegion
 * accepts for player's position. */
static inline EvtObject *evt_checkPlayerAction_find(EvtObject *player, s32 key) {
    EvtObject *found = NULL;
    s32 x = player->pos.vx;
    EvtListNode *node = g_evtState->objects;
    s32 z = player->pos.vz;
    EvtObject *obj;

    for (; node != NULL; node = node->next) {
        obj = node->obj;
        if (obj->kind == EVT_KIND_REGION && evt_callObjectFunc_find(obj->scriptIdx, key) != NULL && evt_isInRegion(obj, x, z)) {
            found = obj;
            break;
        }
    }
    return found;
}

void evt_checkPlayerAction(EvtObject *player) {
    s32 btn;
    s32 pressed;
    EvtObject *target;

    pressed = msg_getPadPressed();
    btn = pressed & (g_evtMode == EVT_MODE_FIELD ? (BTN_CROSS | BTN_SQUARE) : BTN_CROSS);
    if (btn && (target = evt_findNearestObject(player, EVT_FIND_TALK, NULL)) != NULL) {
        g_evtTalkAngle = evt_angleToObject(player, target, player->pos.rotY);
        if ((u32)(g_evtTalkAngle + 0x3FF) < 0x7FF) {
            player->talkTarget = target->uid;
            if (btn == BTN_SQUARE && evt_callObjectFunc(target, 1, EVT_FUNC_SQUARE, 0)) {
                return;
            }
            if (evt_callObjectFunc(target, 1, EVT_FUNC_TALK, 0)) {
                return;
            }
        }
        target = NULL;
    } else {
        target = evt_findNearestObject(player, EVT_FIND_RANGE, NULL);
    }
    if (target != NULL) {
        evt_callObjectFunc(target, 1, EVT_FUNC_RANGE, 0);
    }
    if (btn) {
        target = evt_checkPlayerAction_find(player, EVT_FUNC_TALK);
        if (target != NULL) {
            if (btn == BTN_SQUARE && evt_callObjectFunc(target, 1, EVT_FUNC_SQUARE, 0)) {
                return;
            }
            evt_callObjectFunc(target, 1, EVT_FUNC_TALK, 0);
            target = NULL;
        } else {
            target = evt_checkPlayerAction_find(player, EVT_FUNC_RANGE);
        }
    } else {
        target = evt_checkPlayerAction_find(player, EVT_FUNC_RANGE);
    }
    if (target != NULL) {
        evt_callObjectFunc(target, 1, EVT_FUNC_RANGE, 0);
    }
}

void evt_updateBattleObjects(void) {
    s32 i;
    s32 unitMask;
    EvtObject *obj;

    if (g_gameState->gameData->ctx.ovl65->phase != 0) {
        i = 3;
        g_evtBattleVars[i] = btl_getUnitMask(1);
        unitMask = (u16)g_evtBattleVars[i];
        for (i = EVT_UNIT_FIRST_ENEMY; i < EVT_UNIT_COUNT; i++) {
            obj = g_evtUnitObjects[i];
            if (obj != NULL) {
                switch (obj->state) {
                case EVT_STATE_RUNNING:
                    if (!(unitMask & 0x10)) {
                        obj->state = EVT_STATE_SUSPENDED;
                    }
                    break;
                case EVT_STATE_SUSPENDED:
                    if (unitMask & 0x10) {
                        obj->state = EVT_STATE_RUNNING;
                    }
                    break;
                }
            }
            unitMask >>= 1;
        }
    }
}

/* turn towards turnTarget by turnStep per frame (EVT_TURN_DIRECT_DOWN / UP: direction) */
static inline void evt_updateObjects_turn(EvtObject *obj) {
    s32 dir;

    if ((s16)obj->turnRate == EVT_TURN_DIRECT_DOWN) {
        dir = obj->pos.rotY + (s16)obj->turnStep;
        if (obj->turnTarget < dir) {
            obj->pos.rotY = dir;
        } else {
            obj->pos.rotY = obj->turnTarget;
            obj->flags &= ~EVT_OBJ_TURNING;
        }
    } else if ((s16)obj->turnRate == EVT_TURN_DIRECT_UP) {
        dir = obj->pos.rotY + (s16)obj->turnStep;
        if (dir >= obj->turnTarget) {
            obj->pos.rotY = obj->turnTarget;
            obj->flags &= ~EVT_OBJ_TURNING;
        } else {
            obj->pos.rotY = dir;
        }
    }
}

/* remember the position at the start of the frame */
static inline void evt_updateObjects_save(EvtObject *obj) {
    obj->prevPos.vx = obj->pos.vx;
    obj->prevPos.vy = obj->pos.vy;
    obj->prevPos.vz = obj->pos.vz;
}

s32 evt_updateObjects(void) {
    EvtListNode *node;
    EvtObject *obj;
    EvtObject *actor;
    EvtObject *blocker;
    EvtObject *t1;
    EvtObject *x;
    EvtListNode *y;
    EvtObject *z;
    Unk800C748C *bgiActor;
    s32 world;
    s32 result;
    s32 dist;
    u8 camera;
    s32 h;
    s32 moved;
    s32 dx, dy, dz;
    s32 walkResult;
    u32 anim;
    Window *w;
    EvtListNode *start;
    EvtListNode *saveNode;
    EvtState *evt;

    g_evtPlayerMoved = 0;
    world = g_evtMode == EVT_MODE_BATTLE;
    for (node = g_evtState->objects; node != NULL; node = node->next) {
        obj = node->obj;
        if (obj->kind == EVT_KIND_ACTOR) {
            z = obj;
            if (obj->uid == g_evtState->playerUid && obj->state == EVT_STATE_RUNNING) {
                if (g_evtState->controlEnabled != 0) {
                    if (g_evtMode == EVT_MODE_FIELD) {
                        obj->prevPos.vx = obj->pos.vx;
                        obj->prevPos.vy = obj->pos.vy;
                        obj->prevPos.vz = obj->pos.vz;
                        evt_movePlayer(obj);
                    } else if (g_evtMode == EVT_MODE_WORLD) {
                        g_evtPlayerMoved = g_worldPlayerStepped;
                        evt_checkPlayerAction(obj);
                        t1 = evt_findNearestObject(obj, 0, &dist);
                        if (t1 != NULL && dist <= 0 &&
                            (u32)(evt_angleToObject(obj, t1, math_atan2(obj->pos.vx - obj->prevPos.vx, obj->pos.vz - obj->prevPos.vz)) + 0x400) <= 0x800) {
                            obj->pos.vx = obj->prevPos.vx;
                            obj->pos.vy = obj->prevPos.vy;
                            obj->pos.vz = obj->prevPos.vz;
                        }
                    }
                    if (g_evtPlayerMoved != 0) {
                        evt = g_evtState;
                        if (evt->idleTimer >= 0) {
                            evt->idleTimer = (rand8_inline() + 200) * 4;
                        }
                    }
                } else {
                    evt_updateObjects_save(obj);
                }
            } else {
                evt_updateObjects_save(z);
            }
            if (obj->state == EVT_STATE_RUNNING && !world) {
                evt_stepAnim(obj);
            }
        }
    }
    if (world) {
        evt_updateBattleObjects();
    }
    start = (EvtListNode *)g_evtResumeNode;
    g_evtPosSetThisFrame = 0;
    if (start == NULL) {
        start = g_evtState->objects;
    }
    result = evt_runScripts(start);
    if (result == EVT_UPD_END_FRAME) {
        result = 0;
    } else {
        g_evtResumeNode = 0;
    }
    g_evtWorldCamTargetObj = NULL;
    g_evtWorldCamEyeObj = NULL;
    if (g_evtMode == EVT_MODE_FIELD) {
        fldbg_getCamera(&camera);
        g_evtCameraChanged = camera != g_evtCamera;
        g_evtCamera = camera;
    }
    if (world) {
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            obj = node->obj;
            if (obj->battleTurnState == 2 && evt_callObjectFunc(obj, 3, EVT_FUNC_BTL_TURN, 0)) {
                obj->battleTurnState = 1;
            }
        }
    } else {
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            obj = node->obj;
            if (obj->kind == EVT_KIND_ACTOR) {
                actor = obj;
                if (g_evtMode == EVT_MODE_FIELD) {
                    walkResult = evt_updateActorWalkmesh(actor);
                    if ((actor->uid != g_evtState->playerUid || g_evtPushFrames >= 0) && (walkResult & 1) &&
                        (blocker = evt_findNearestObject(actor, 0, NULL)) != NULL) {
                        bgi_revertActor(actor->modelSlot);
                        if (blocker->flags & EVT_OBJ_NOT_PUSHABLE) {
                            g_evtPushFrames = 0;
                        }
                        if (obj->uid == g_evtState->playerUid) {
                            g_evtPlayerBlocked = 2;
                        }
                    }
                    if (obj->uid == g_evtState->playerUid) {
                        msg_drawHereIcon(actor);
                        if (g_evtState->controlEnabled != 0) {
                            evt_checkPlayerAction(actor);
                        }
                    }
                } else if (g_evtMode == EVT_MODE_WORLD) {
                    if (actor->moveFlags & EVT_MOVE_WM_CAMERA_EYE) {
                        VECTOR *camEye = wm_getCameraEye();
                        camEye->vx = actor->pos.vx;
                        camEye->vy = actor->pos.vy;
                        g_evtWorldCamEyeObj = actor;
                        camEye->vz = actor->pos.vz;
                    } else if (actor->moveFlags & EVT_MOVE_WM_CAMERA_TARGET) {
                        VECTOR *camTarget = wm_getCameraTarget();
                        camTarget->vx = actor->pos.vx;
                        camTarget->vy = actor->pos.vy;
                        g_evtWorldCamTargetObj = actor;
                        camTarget->vz = actor->pos.vz;
                    }
                }
                if (actor->balloonWindow != EVT_BALLOON_NONE) {
                    w = msg_getWindow(actor->balloonWindow);
                    if (w != NULL) {
                        evt_followBalloon(actor, w);
                    } else {
                        actor->balloonWindow = 0xFF;
                        if (g_evtLookTarget == actor) {
                            g_evtLookReleaseTimer = 30;
                        }
                    }
                }
                moved = 0;
                dx = actor->pos.vx - actor->prevPos.vx;
                dy = actor->pos.vy - actor->prevPos.vy;
                dz = actor->pos.vz - actor->prevPos.vz;
                if (dx != 0 || dz != 0) {
                    moved = 1;
                }
                if (g_evtMode == EVT_MODE_FIELD) {
                    if (actor->flags & EVT_OBJ_TURNING) {
                        evt_updateObjects_turn(actor);
                    }
                    if (actor->modelId != EVT_MODEL_NONE) {
                        evt_updateLookAt(actor);
                    }
                }
                if (g_evtMode == EVT_MODE_WORLD && (actor->flags & EVT_OBJ_TURNING)) {
                    evt_updateObjects_turn(actor);
                }
                if (moved) {
                    if ((actor->animFlags & (EVT_ANIM_SCRIPTED | EVT_ANIM_HELD | EVT_ANIM_IDLE)) == 0 ||
                        (actor->animFlags & (EVT_ANIM_SCRIPTED | EVT_ANIM_HELD | EVT_ANIM_IDLE)) == (EVT_ANIM_SCRIPTED | EVT_ANIM_IDLE)) {
                        actor->animFlags &= ~(EVT_ANIM_SCRIPTED | EVT_ANIM_IDLE);
                        anim = actor->speed >= actor->runSpeed ? (u16)actor->runAnim : (u16)actor->walkAnim;
                        goto set_anim;
                    }
                } else if (!(actor->animFlags & (EVT_ANIM_SCRIPTED | EVT_ANIM_HELD)) && (g_evtPlayerBlocked == 0 || obj->uid != g_evtState->playerUid)) {
                    anim = (u16)actor->standAnim;
                set_anim:
                    if (anim != actor->anim) {
                        actor->anim = anim;
                        actor->animFrame = 0;
                        actor->animSubFrame = 0;
                        actor->animFrames = evt_getAnimFrameCount(actor->modelId, anim);
                        actor->animSpeed = actor->defaultAnimSpeed;
                    }
                }
                if (moved && obj->uid == g_evtState->playerUid && g_evtPlayerMoved != 0) {
                    g_evtEncounterDist += math_length3(dx, dy, dz);
                    if (evt_rollEncounter(actor)) {
                        g_evtEncounterPending = 1;
                    }
                }
            }
        }
    }
    if (g_evtPlayerBlocked > 0) {
        g_evtPlayerBlocked--;
    }
    if (g_evtLookReleaseTimer > 0 && --g_evtLookReleaseTimer == 0) {
        for (y = g_evtState->objects; y != NULL; y = y->next) {
            x = y->obj;
            if (x->kind == EVT_KIND_ACTOR && (x->moveFlags & EVT_MOVE_LOOKED_AT_SPEAKER)) {
                x->moveFlags |= EVT_MOVE_LOOK_RELEASED;
            }
        }
        g_evtLookTarget = NULL;
    }
    if (g_evtEncounterPending != 0 && g_evtPosSetThisFrame == 0) {
        g_evtEncounterPending = 0;
        result = 3;
    }
    if (result == 3 || result == 7) {
        if (g_evtMode == EVT_MODE_FIELD) {
            for (saveNode = g_evtState->objects; saveNode != NULL; saveNode = saveNode->next) {
                actor = saveNode->obj;
                if (actor->kind == EVT_KIND_ACTOR && actor->modelId != EVT_MODEL_NONE) {
                    bgiActor = bgi_getActor(actor->modelSlot);
                    bgi_getActorRadius(actor->modelSlot, &h);
                    actor->savedWalkRadius = h / 4;
                    if (bgiActor != NULL) {
                        actor->pos.vx = actor->prevPos.vx;
                        actor->pos.vy = actor->prevPos.vy;
                        actor->pos.vz = actor->prevPos.vz;
                        actor->savedWalkFloor = bgiActor->walkFloor;
                        actor->savedWalkTri = bgiActor->walkTri;
                        actor->savedWalkFlags = bgiActor->walkFlags;
                    }
                }
            }
            fld_enableRain(0, 0, 0);
        }
        if (result == 7) {
            g_evtSavedState = g_evtStateData;
        }
    }
    return result;
}

void evt_onAnimEnd(EvtObject *obj) {
    s32 animId;
    u32 *slot;

    if (!(obj->animFlags & EVT_ANIM_HOLD)) {
        animId = (u16)obj->standAnim;
        if (animId != obj->anim) {
            obj->anim = animId;
            obj->animFrame = 0;
            obj->animSubFrame = 0;
            obj->animFrames = evt_getAnimFrameCount(obj->modelId, animId);
            obj->animSpeed = obj->defaultAnimSpeed;
        }
        obj->animFrame = 0;
        /* rand8_inline() written out: calling it changes the register allocation here */
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        obj->animSpeed = obj->idleAnimSpeeds.b[*g_randPtr++ & 3];
        obj->animFlags &= ~(EVT_ANIM_SCRIPTED | EVT_ANIM_REVERSE | EVT_ANIM_HELD | EVT_ANIM_IDLE);
        if (obj->flags & EVT_OBJ_TURNING) {
            evt_finishTurn_inline(obj);
        }
        if (obj->moveFlags & EVT_MOVE_JUMPING) {
            obj->moveFlags &= ~EVT_MOVE_JUMPING;
            slot = (u32 *)game_getModel(obj->modelSlot);
            *slot &= ~MODEL_SLOT_NO_SHADOW;
            obj->animStart = 0;
            obj->animEnd = 0xFF;
        }
    } else {
        obj->animFlags = (obj->animFlags | EVT_ANIM_HELD) & ~(EVT_ANIM_SCRIPTED | EVT_ANIM_IDLE);
        obj->animFrame = obj->animPrevFrame;
    }
}

static inline void evt_onAnimLimit_bounce(EvtObject *obj, u8 mode) {
    u8 oldStart;
    u8 pos;

    if (mode & EVT_ANIM_BOUNCE) {
        pos = obj->animFrame;
        if (!(mode & EVT_ANIM_REVERSE)) {
            obj->animFrame = pos - 2;
        } else {
            obj->animFrame = pos + 2;
        }
        obj->animFlags ^= EVT_ANIM_REVERSE;
        oldStart = obj->animStart;
        obj->animStart = obj->animEnd;
        obj->animEnd = oldStart;
    } else if (mode & EVT_ANIM_LOOP) {
        obj->animFrame = obj->animStart;
    }
}

void evt_onAnimLimit(EvtObject *obj) {
    u8 mode = obj->animFlags;

    if (mode & (EVT_ANIM_LOOP | EVT_ANIM_BOUNCE)) {
        if (obj->animLoops != 0) {
            obj->animLoops--;
            if (obj->animLoops != 0) {
                evt_onAnimLimit_bounce(obj, mode);
            } else {
                evt_onAnimEnd(obj);
            }
        } else {
            evt_onAnimLimit_bounce(obj, mode);
        }
    } else {
        evt_onAnimEnd(obj);
    }
}

void evt_stepAnim(EvtObject *obj) {
    EvtObject *src;
    u8 mode;
    s32 last;
    s32 frame16;
    s32 lo;
    u8 old;
    s32 t2, lo2, t3, lo3;
    u8 old2, old3;

    mode = obj->animFlags;
    if (mode & EVT_ANIM_HELD) {
        return;
    }
    if (mode & EVT_ANIM_SCRIPTED) {
        if (obj->animSyncSource != NULL) {
            src = obj->animSyncSource;
            if (src->anim == (u16)src->standAnim) {
                obj->anim = obj->standAnim;
            } else if (src->anim == (u16)src->walkAnim) {
                obj->anim = obj->walkAnim;
            } else if (src->anim == (u16)src->runAnim) {
                obj->anim = obj->runAnim;
            }
            obj->animFrame = src->animFrame;
        } else if (!(mode & EVT_ANIM_REVERSE)) {
            last = obj->animEnd;
            if (obj->animFrames - 1 < last) {
                last = obj->animFrames - 1;
            }
            frame16 = obj->animFrame * 16 + (s8)obj->animSubFrame + obj->animSpeed;
            lo = frame16 & 0xF;
            obj->animSubFrame = lo;
            if (obj->animFlags & EVT_ANIM_REVERSE) {
                obj->animSubFrame = lo - 0xF;
            }
            old = obj->animFrame;
            frame16 >>= 4;
            obj->animFrame = frame16;
            obj->animPrevFrame = old;
            if (last < frame16) {
                evt_onAnimLimit(obj);
            }
            if (obj->flags & EVT_OBJ_TURNING) {
                obj->pos.rotY = (obj->pos.rotY + obj->turnRate) & 0xFFF;
                obj->turnStartDir = (obj->turnStartDir + obj->turnStep) & 0xFFF;
            }
        } else {
            t2 = obj->animFrame * 16 + (s8)obj->animSubFrame - obj->animSpeed;
            t2 += 0xF;
            lo2 = t2 & 0xF;
            obj->animSubFrame = lo2;
            if (obj->animFlags & EVT_ANIM_REVERSE) {
                obj->animSubFrame = lo2 - 0xF;
            }
            old2 = obj->animFrame;
            t2 >>= 4;
            obj->animFrame = t2;
            obj->animPrevFrame = old2;
            if (t2 < obj->animEnd) {
                evt_onAnimLimit(obj);
            }
        }
    } else {
        lo3 = obj->animFrame * 16 + (s8)obj->animSubFrame;
        if (mode & EVT_ANIM_REVERSE) {
            t3 = lo3 - obj->animSpeed;
        } else {
            t3 = lo3 + obj->animSpeed;
        }
        if (obj->animFlags & EVT_ANIM_REVERSE) {
            t3 += 0xF;
        }
        lo3 = t3 & 0xF;
        obj->animSubFrame = lo3;
        if (obj->animFlags & EVT_ANIM_REVERSE) {
            obj->animSubFrame = lo3 - 0xF;
        }
        old3 = obj->animFrame;
        t3 >>= 4;
        obj->animFrame = t3;
        obj->animPrevFrame = old3;
        if (t3 >= obj->animFrames) {
            obj->animFrame = 0;
            if (obj->anim == (u16)obj->standAnim) {
                /* rand8_inline() written out: calling it changes the register allocation here */
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                obj->animSpeed = obj->idleAnimSpeeds.b[*g_randPtr++ & 3];
            }
        }
    }
}

u8 *evt_loadScripts(u8 *data) {
    s32 charMask = 0;
    s32 i = 0;
    s32 chr;
    s32 count;
    u8 *dst;
    ScriptEntry *entry;
    u16 *sz;
    s32 bit;
    u8 *src;

    for (; i < PARTY_SIZE; i++) {
        chr = msg_getPartyMemberId(i);
        if (chr >= 0) {
            charMask |= 1 << chr;
        }
    }
    count = data[3];
    g_evtScripts = data + 0x80;
    g_evtScriptCount = count;
    if (g_evtScriptCount >= 10) {
        dst = g_evtScripts + ((ScriptEntry *)g_evtScripts)[g_evtScriptCount - 9].offset;
    } else {
        dst = g_evtScripts + g_evtScriptCount * 8;
    }
    bit = 1;
    for (entry = &((ScriptEntry *)g_evtScripts)[g_evtScriptCount - 9]; entry < &((ScriptEntry *)g_evtScripts)[g_evtScriptCount]; entry++) {
        if (!(entry->flags & 1)) {
            charMask |= bit;
        }
        if (charMask & bit) {
            if (entry->offset != 0) {
                src = g_evtScripts + entry->offset;
                if (src != dst && entry->size != 0) {
                    memmove(dst, src, entry->size);
                    entry->offset = dst - g_evtScripts;
                }
                dst += entry->size & ~3;
            }
        }
        bit <<= 1;
    }
    g_evtCharScriptMask = charMask;
    return dst;
}

void evt_init(u8 *data) {
    Unk4 thresholds;
    s32 k;
    u8 mode;
    EvtAllocNode *allocNode;
    EvtListNode *listNode;
    EvtListNode *node;
    EvtListNode *iter;
    s32 i;
    s32 chr;
    s32 ipDelta;
    EvtObject *obj;
    EvtState *evt;
    GameState *gs;

    thresholds = g_evtDefaultAnalogThresholds;
    pad_setAnalogThresholds(&thresholds);
    k = data[3];
    g_evtScripts = data + 0x80;
    gs = g_gameState;
    g_evtLocationName = data + 0x2C;
    gs->gameData->debugLocationName = data + 4;
    g_evtScriptCount = k;
    gs->gameData->locationName = data + 0x2C;
    mode = g_gameState->mode;
    switch (mode) {
    case GS_MODE_FIELD:
        g_evtMode = mode;
        break;
    case GS_MODE_BATTLE:
        g_evtMode = mode;
        break;
    case GS_MODE_WORLD:
        g_evtMode = mode;
        break;
    case GS_MODE_BATTLE_END:
        g_evtMode = EVT_MODE_BATTLE_END;
        break;
    }
    if ((g_evtStateData.mode == EVT_MODE_FIELD || g_evtStateData.mode == EVT_MODE_WORLD) && g_evtMode == EVT_MODE_BATTLE) {
        g_evtSavedState = g_evtStateData;
    }
    g_evtState = &g_evtStateData;
    for (allocNode = g_evtState->allocNodes; allocNode < &g_evtState->allocNodes[31]; allocNode++) {
        allocNode->next = allocNode + 1;
    }
    allocNode->next = NULL;
    g_evtState->allocFree = g_evtState->allocNodes;
    g_evtState->allocUsed = NULL;
    for (listNode = g_evtState->listNodes; listNode < &g_evtState->listNodes[31]; listNode++) {
        listNode->next = listNode + 1;
    }
    listNode->next = NULL;
    g_evtState->listFree = g_evtState->listNodes;
    g_evtState->objectsTail = NULL;
    g_evtState->objects = NULL;
    msg_resetPlayerBubble();
    msg_setAtePrompt(ATE_PROMPT_OFF);
    for (i = 0; i < 0x50; i++) {
        g_evtState->fieldVars[i] = 0;
    }
    g_evtState->controlEnabled = 0;
    g_evtState->playerUid = 0;
    g_evtState->idleTimer = 0;
    msg_startHereIcon(0);
    g_evtResumeNode = 0;
    g_evtState->walkOnly = 0;
    g_evtLookTarget = NULL;
    g_evtPlayerAnimCount = 10;
    g_evtMsgCount = 10;
    g_msgReady = 0;
    g_evtHalted = 0;
    g_evtCameraChanged = 0;
    g_evtCamera = 0;
    g_evtEncounterDist = EVT_ENCOUNTER_GRACE;
    evt = g_evtState;
    evt->encounterRate = 0;
    g_evtEncounterChance = 0;
    g_evtEncounterPending = 0;
    evt->analogDirOfs = 0;
    evt->padDirOfs = 0;
    g_swirlVariant = 0;
    func_8004B3A8_inline(0, 0);
    g_evtState->objects->obj->state = EVT_STATE_INIT;
    evt_execInstr_party();
    for (i = EVT_UNIT_COUNT - 1; i >= 0; i--) {
        g_evtUnitObjects[i] = NULL;
    }
    g_evtUnitObjectCount = 0;
    if (g_evtMode == EVT_MODE_BATTLE) {
        for (i = 0; i < PARTY_SIZE; i++) {
            chr = msg_getPartyMemberId(i);
            if (chr >= 0) {
                evt_execInstr_newactor(chr + g_evtScriptCount - 9, 0);
            }
        }
        g_evtState->battleObjectsTail = (s32)g_evtState->objectsTail;
    } else {
        g_gameState->gameData->rainCount = 0;
    }
    g_evtUnitObjectCount = EVT_UNIT_FIRST_ENEMY;
    if ((g_evtMode == EVT_MODE_FIELD && g_evtSavedState.mode == g_evtMode && g_evtSavedState.locationId == (s16)g_gameState->gameData->fieldId) ||
        (g_evtMode == EVT_MODE_WORLD && g_evtSavedState.mode == g_evtMode && g_evtSavedState.locationId == g_gameState->gameData->worldId) ||
        (g_gameState->prevMode == GS_MODE_TETRA_MASTER && g_evtModelsPending != 0)) {
        ipDelta = data - g_evtSavedState.scriptBase;
        g_evtStateData = g_evtSavedState;
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            obj = node->obj;
            if (obj->ip != NULL) {
                obj->ip += ipDelta;
            }
            if (obj->kind == EVT_KIND_ACTOR && g_evtMode == EVT_MODE_FIELD) {
                fld_queueModelBundle(obj->scriptIdx);
            }
        }
        evt_callObjectFunc(evt_getObject_inline(0), 0, EVT_FUNC_RESUME, 0);
        for (iter = g_evtState->objects; iter != NULL; iter = iter->next) {
            obj = iter->obj;
            if (obj->uid != 0) {
                obj->savedState = obj->state;
                obj->state = EVT_STATE_SUSPENDED;
            }
        }
    } else {
        evt_updateObjects();
    }
    g_evtState->scriptBase = data;
    g_evtState->mode = g_evtMode;
    g_evtState->locationId = g_evtMode == EVT_MODE_FIELD   ? g_gameState->gameData->fieldId
                          : g_evtMode == EVT_MODE_WORLD ? g_gameState->gameData->worldId
                                            : 0;
}

void evt_createModels(void) {
    EvtListNode *node;
    EvtObject *obj;
    s16 *bgiActor;

    if ((g_evtMode == EVT_MODE_FIELD || g_evtMode == EVT_MODE_WORLD || g_gameState->prevMode == GS_MODE_TETRA_MASTER) && g_evtModelsPending != 0) {
        for (node = g_evtState->objects; node != NULL; node = node->next) {
            obj = node->obj;
            if (obj->kind == EVT_KIND_ACTOR && obj->modelId != EVT_MODEL_NONE) {
                obj->modelSlot = evt_createModel(obj);
                if (g_evtMode == EVT_MODE_FIELD) {
                    bgiActor = (s16 *)bgi_getActor(obj->modelSlot);
                    bgi_setActorRadius(obj->modelSlot, obj->savedWalkRadius * 4);
                    if (bgiActor != NULL) {
                        bgiActor[4] = bgiActor[2] = obj->savedWalkFloor;
                        bgiActor[5] = bgiActor[3] = obj->savedWalkTri;
                        bgiActor[0] = obj->savedWalkFlags;
                    }
                }
            }
        }
        g_evtModelsPending = 0;
    }
}

void evt_onMenuOpen(void) {
    u8 *name;
    s32 skip;
    msg_closeAll();
    msg_startHereIcon(0);
    if (g_evtMode == EVT_MODE_WORLD) {
        name = g_worldAreaNames;
        skip = (evt_getSysVar(WM_PARAM_AREA) * 2) & 0x7F;
        while (skip != 0) {
            while (*name++ != 0) {
            }
            skip--;
        }
        g_gameState->gameData->locationName = name;
        while (*name++ != 0) {
        }
        g_gameState->gameData->debugLocationName = name;
        evt_callObjectFunc(evt_getObject_inline(0), 1, EVT_FUNC_MENU, 0);
    }
}

s32 evt_countNpcActors(s32 unused) { /* the overlays pass 0 */
    EvtState *evt = g_evtState;
    EvtListNode *node;
    s32 lim;
    s32 cnt = 0;
    if (evt != NULL) {
        node = evt->objects;
        if (node != NULL) {
            lim = g_evtScriptCount - 9;
            do {
                EvtObject *obj = node->obj;
                s32 isActor = 0;
                if (obj->scriptIdx < lim) {
                    isActor = obj->kind == EVT_KIND_ACTOR;
                }
                cnt += isActor;
                node = node->next;
            } while (node != NULL);
        }
    }
    return cnt;
}

s32 evt_getObjectChrIndex(EvtObject *obj) {
    s32 idx = obj->scriptIdx + 9;
    return idx - g_evtScriptCount;
}

s32 evt_battleTrigger(s32 op, s32 bits, s32 value, s32 w) {
    s32 result = 0;
    s32 i;
    EvtObject *obj;
    s32 lv;
    s32 key;

    i = 0;
    if (!(bits & 1)) {
        bits >>= 1;
        i = 1;
        while (i < EVT_UNIT_COUNT && !(bits & 1)) {
            bits >>= 1;
            i++;
        }
    }
    if (i < EVT_UNIT_COUNT) {
        obj = g_evtUnitObjects[i];
        if (op == BTL_CMD_ENEMY_TURN) {
            if (obj->level >= 4) {
                obj->battleTurnState = 2;
                result = 1;
            }
        } else {
            lv = 2;
            key = EVT_FUNC_BTL_REACT;
            switch (op) {
            case BTL_CMD_ENEMY_COUNTER:
                key = EVT_FUNC_BTL_COUNTER;
                g_evtBattleVars[0] = value;
                lv = 1;
                break;
            case BTL_CMD_ENEMY_DEATH:
                key = EVT_FUNC_BTL_DEATH;
                g_evtBattleVars[0] = value;
                lv = 0;
                break;
            case BTL_CMD_ENEMY_REACT:
                break;
            }
            g_evtBattleTriggerArg = w;
            result = evt_callObjectFunc(obj, lv, key, 0);
        }
    }
    return result;
}

void evt_reset(void) {
    g_evtState = NULL;
    g_evtLastEncounter = 0;
    g_evtStateData.mode = 0;
    g_evtModelsPending = 0;
}

void evt_setDrawPosAtObject(EvtObject *obj, s32 dx, s32 dy) {
    SVECTOR vec;
    SVECTOR *pv = &vec;
    s32 screen[2];
    volatile s32 sxy;
    GameData *gd;

    vec.vx = obj->pos.vx;
    vec.vy = obj->pos.vy + ((obj->height * (s8)obj->scale) >> 6) - EVT_HEAD_ANCHOR_RAISE;
    vec.vz = obj->pos.vz;
    if (g_evtMode == EVT_MODE_FIELD) {
        fldbg_applyGeomOffset();
    }
    gd = g_gameState->gameData;
    gte_SetRotMatrix(&gd->viewMatrix);
    gte_SetTransMatrix(&gd->viewMatrix);
    gte_ldv0(pv);
    gte_rtps();
    gte_stsxy((s32 *)&sxy);
    if (g_evtMode == EVT_MODE_FIELD) {
        fldbg_resetGeomOffset();
    }
    screen[0] = *(s16 *)&sxy;
    screen[1] = sxy >> 16;
    pen_begin();
    pen_setPos(screen[0] + dx, screen[1] + dy);
    pen_end();
}

EvtObject *evt_getPlayer(void) {
    EvtObject *obj = NULL;
    EvtObject *player;

    if (g_evtState != NULL) {
        obj = evt_getObject_inline(g_evtState->playerUid);
    }
    if (obj != NULL && obj->kind == EVT_KIND_ACTOR) {
        player = obj;
    } else {
        player = NULL;
    }
    return player;
}

s32 evt_isControlEnabled(void) {
    return g_evtState->controlEnabled;
}

EvtListNode *evt_getObjectList(void) {
    EvtState *evt = g_evtState;
    EvtListNode *list = NULL;
    if (evt != NULL) {
        list = evt->objects;
    }
    return list;
}

EvtListNode *evt_getBattleObjects(void) {
    EvtState *evt = g_evtState;
    EvtListNode *list = NULL;
    if (evt != NULL) {
        list = evt->battleObjectsTail;
    }
    return list;
}

s32 evt_getArg8(void) {
    return evt_getArg8_inline();
}

s32 evt_getArg16(void) {
    return evt_getArg16_inline();
}

s32 evt_getObjectModelData(EvtObject *obj) {
    return (s32)evt_getObjectModelData_inline(obj);
}

EvtObject *evt_getObject(s32 id) {
    return evt_getObject_inline(id);
}

s32 evt_getModelSlot(s32 id) {
    EvtObject *obj = evt_getObject_inline(id);
    s32 slot;

    if (obj != NULL) {
        if (obj->kind == EVT_KIND_ACTOR) {
            slot = obj->modelSlot;
        } else {
            slot = -1;
        }
    } else {
        slot = -1;
    }
    return slot;
}

s32 evt_getPartyChrIndex(s32 i) {
    s32 biased = g_evtState->partyUids[i] + 9;
    u32 chrIdx = biased - g_evtScriptCount;
    s32 idx = 0;
    if (chrIdx < CHR_COUNT) {
        idx = chrIdx;
    }
    return idx;
}

void evt_callWorldCellTrigger(s32 cellX, s32 cellZ, s32 kind) {
    evt_callObjectFunc(evt_getObject_inline(0), 1, EVT_FUNC_WORLD_CELL_KEY(cellX, cellZ, kind), 0);
}

s32 evt_getWorldCamEyeObj(void) {
    return (s32)g_evtWorldCamEyeObj;
}

s32 evt_getWorldCamTargetObj(void) {
    return (s32)g_evtWorldCamTargetObj;
}

void evt_resetIdleTimer(s32 halfDelay) {
    EvtState *evt = g_evtState;
    s32 delay;
    if (evt->idleTimer >= 0) {
        delay = rand8_inline() + 200;
        if (halfDelay == 0) {
            evt->idleTimer = delay * 4;
        } else {
            evt->idleTimer = delay * 2;
        }
    }
}

void evt_initAllocNodes(void) {
    EvtAllocNode *node = g_evtState->allocNodes;
    for (; node < &g_evtState->allocNodes[31]; node++) {
        node->next = node + 1;
    }
    node->next = NULL;
    g_evtState->allocFree = g_evtState->allocNodes;
    g_evtState->allocUsed = NULL;
}

void evt_freeAllocNode(EvtObject *key) {
    evt_freeAllocNode_inline(key);
}

void evt_initObjectList(void) {
    EvtListNode *node = g_evtState->listNodes;
    for (; node < &g_evtState->listNodes[31]; node++) {
        node->next = node + 1;
    }
    node->next = NULL;
    g_evtState->listFree = g_evtState->listNodes;
    g_evtState->objectsTail = NULL;
    g_evtState->objects = NULL;
}

void evt_createRegion(s32 scriptIdx, s32 id) {
    evt_createRegion_inline(scriptIdx, id);
}

u16 evt_getBattleVar(s32 var) {
    return evt_getBattleVar_inline(var);
}

void evt_setBattleVar(s32 i, s16 value) {
    g_evtBattleVars[i & 7] = value;
}

EvtObject *evt_resolveObjectId(s32 id) {
    return evt_resolveObjectId_inline(id);
}

s32 evt_popBitCount(void) {
    s32 count = 0;
    s32 bits = evt_popResult();
    s16 bit;
    for (bit = 1; bit != 0; bit <<= 1) {
        count += (bits & bit) != 0;
    }
    return count;
}

s32 evt_popRandomBit(void) {
    u8 setBitIdx[8];
    s32 bits = evt_popResult();
    s32 count = 0;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (bits & 1) {
            setBitIdx[count++] = i;
        }
        bits >>= 1;
    }
    if (count != 0) {
        /* rand8_inline() written out: calling it changes the register allocation here */
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        return 1 << setBitIdx[(count * *g_randPtr++) >> 8];
    }
    return 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/event", g_evtDefaultAnalogThresholds);

s32 evt_getObjectProp(EvtObject *obj, s32 prop) {
    s32 result;
    s32 i;
    void *unit;

    switch (prop) {
    case EVT_GETPROP_X:
        return obj->pos.vx;
    case EVT_GETPROP_Y:
        return obj->pos.vy;
    case EVT_GETPROP_Z:
        return obj->pos.vz;
    case EVT_GETPROP_ROT:
        return (u8)((u16)obj->pos.rotY >> 4);
    case EVT_GETPROP_FLAGS:
        return obj->flags;
    case EVT_GETPROP_UID:
        return obj->uid;
    case EVT_GETPROP_LEVEL:
        return obj->level;
    case EVT_GETPROP_ANIM_FRAME:
        return obj->animFrame;
    }
    result = 0;
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        if (g_evtUnitObjects[i] == obj) {
            break;
        }
    }
    if (i < EVT_UNIT_COUNT) {
        unit = btl_getUnitById(1 << i);
        if (unit != NULL) {
            result = btl_getUnitAttr(unit, prop);
        }
    }
    return result;
}

void evt_setObjectProp(EvtObject *obj, s32 prop, s32 value) {
    s32 i;
    struct BtlUnit *(*fn)(u16);
    struct BtlUnit *unit;

    if (obj != NULL) {
        i = 0;
        if (prop == EVT_SETPROP_ROT) {
            obj->pos.rotY = value * 16;
            return;
        }
        for (; i < EVT_UNIT_COUNT; i++) {
            if (g_evtUnitObjects[i] == obj) {
                break;
            }
        }
        if (i < EVT_UNIT_COUNT) {
            if (prop == EVT_SETPROP_INSERT) {
                fn = btl_findUnitByBit;
            } else {
                fn = btl_getUnitById;
            }
            unit = fn(1 << i);
            if (unit != NULL) {
                btl_setUnitAttr(unit, prop, value);
            }
        }
    }
}

s32 evt_isInParty(s32 slot) {
    return evt_isInParty_find(evt_isInParty_key(slot), slot);
}

s32 evt_getArg24(void) {
    return evt_getArg24_inline();
}

void evt_finishTurn(EvtObject *obj) {
    evt_finishTurn_inline(obj);
}

void evt_projectToScreen(SVECTOR *pos, s32 *x, s32 *y) {
    DVECTOR screen;
    GameData *gd;

    if (x != NULL && y != NULL) {
        if (g_evtMode == EVT_MODE_FIELD) {
            fldbg_applyGeomOffset();
        }
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->viewMatrix);
        gte_SetTransMatrix(&gd->viewMatrix);
        gte_ldv0(pos);
        gte_rtps();
        gte_stsxy(&screen);
        if (g_evtMode == EVT_MODE_FIELD) {
            fldbg_resetGeomOffset();
        }
        *x = screen.vx;
        *y = screen.vy;
    }
}

void evt_setCurUnit(void *obj) {
    s32 i;
    s32 bit;
    for (i = 0; i < EVT_UNIT_COUNT; i++) {
        if (g_evtUnitObjects[i] == obj) {
            break;
        }
    }
    if (i < EVT_UNIT_COUNT) {
        bit = 1 << i;
        g_evtCurUnitMask = bit;
    }
}

#include "ovl_0a.h"

/* does the segment from a0 to a1 cross the edge b0-b1 (on axes x, z)? */
s32 bgi_segmentsCross(s16 *a0, s16 *a1, s16 *b0, s16 *b1, u8 x, u8 z) {
    s32 t2;
    s32 t3;
    s32 t4;
    s32 ex;
    s32 ez;

    t2 = (a1[x] - a0[x]) * (ez = b1[z] - b0[z]);
    ex = b1[x] - b0[x];
    t3 = (a0[z] - b0[z]) * ex;
    t2 -= (a1[z] - a0[z]) * ex;
    t3 -= (a0[x] - b0[x]) * ez;
    if (ABS(t3) <= ABS(t2)) {
        if ((t3 >= 0 && t2 >= 0) || (t3 <= 0 && t2 < 0)) {
            t4 = (a0[z] - b0[z]) * (a1[x] - a0[x]);
            t4 -= (a0[x] - b0[x]) * (a1[z] - a0[z]);
            if ((t4 >= 0 && t2 >= 0) || (t4 <= 0 && t2 < 0)) {
                return 1;
            }
        }
    }
    return 0;
}

BgiTriangle *bgi_getPassableNeighbor(BgiActor *actor, BgiTriangle *tri, s32 edge) {
    BgiTriangle *n;
    EvtObject *a;
    s16 id;
    u8 m;
    u8 mode;

    id = tri->neighbors[edge];
    if (id == -1) {
        goto fail;
    }
    n = &g_fieldBgi.triangles[id];
    if (!(n->flags & BGI_FLAG_ACTIVE)) {
        goto fail;
    }
    if ((m = (n->flags >> 8) & g_fieldBgi.maskFilter) & (BGI_MASK_NO_PLAYER | BGI_MASK_NO_OTHERS)) {
        mode = g_gameState->mode;
        if (mode == 5) {
            a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
        } else if (mode == GS_MODE_FIELD) {
            a = evt_getPlayer();
        } else {
            a = NULL;
        }
        if (actor->actor == a) {
            if (m & BGI_MASK_NO_PLAYER) {
            fail:
                return NULL;
            }
            return n;
        }
        if (m & BGI_MASK_NO_OTHERS) {
            return NULL;
        }
    }
    return n;
}

/* closest point to pt on segment a-b -> out; returns squared distance */
s32 bgi_closestPointOnSegment(SVECTOR *pt, SVECTOR *a, SVECTOR *b, SVECTOR *out, s16 *outInside) {
    SVECTOR c;
    s32 ta;
    s32 tb;
    s32 s;
    s32 dx;
    s32 dy;
    s32 dz;
    s16 px;
    s16 py;
    s16 pz;
    s16 ax;
    s16 ay;
    s16 az;
    s16 bx;
    s16 by;
    s16 bz;

    px = pt->vx;
    ax = a->vx;
    bx = b->vx;
    py = pt->vy;
    ay = a->vy;
    by = b->vy;
    pz = pt->vz;
    az = a->vz;
    bz = b->vz;
    ta = (px - ax) * (bx - ax);
    ta += (py - ay) * (by - ay);
    ta += (pz - az) * (bz - az);
    if (ta <= 0) {
        c.vx = a->vx;
        c.vy = a->vy;
        c.vz = a->vz;
        if (ta == 0) {
            *outInside = 1;
        } else {
            *outInside = 0;
        }
    } else {
        tb = (px - bx) * (ax - bx);
        tb += (py - by) * (ay - by);
        tb += (pz - bz) * (az - bz);
        if (tb <= 0) {
            c.vx = b->vx;
            c.vy = b->vy;
            c.vz = b->vz;
            if (tb == 0) {
                *outInside = 1;
            } else {
                *outInside = 0;
            }
        } else {
            while (ta > 0x10000 || tb > 0x10000) {
                ta >>= 1;
                tb = tb >> 1;
            }
            s = ta + tb;
            c.vx = a->vx + (b->vx - a->vx) * ta / s;
            c.vy = a->vy + (b->vy - a->vy) * ta / s;
            c.vz = a->vz + (b->vz - a->vz) * ta / s;
            *outInside = 1;
        }
    }
    dx = c.vx - pt->vx;
    dy = c.vy - pt->vy;
    dz = c.vz - pt->vz;
    out->vx = c.vx;
    out->vy = c.vy;
    s = dx * dx;
    s += dy * dy;
    out->vz = c.vz;
    return s + dz * dz;
}

s32 bgi_projectOnPlane(VECTOR *normal, SVECTOR *dir, s32 unused, VECTOR *outAxis) {
    VECTOR a;
    VECTOR v;

    v.vx = dir->vx << 16;
    v.vy = dir->vy << 16;
    v.vz = dir->vz << 16;
    a.vy = 0;
    a.vz = v.vx;
    a.vx = -v.vz;
    outAxis->vx = -dir->vz;
    outAxis->vy = 0;
    outAxis->vz = dir->vx;
    if (normal->vy >= 0) {
        v.vx = fld_fixMul(normal->vy, a.vz);
        v.vy = fld_fixMul(normal->vz, a.vx) - fld_fixMul(normal->vx, a.vz);
        v.vz = -fld_fixMul(normal->vy, a.vx);
    } else {
        v.vx = -fld_fixMul(normal->vy, a.vz);
        v.vy = fld_fixMul(normal->vx, a.vz) - fld_fixMul(normal->vz, a.vx);
        v.vz = fld_fixMul(normal->vy, a.vx);
    }
    v.vx = (v.vx + 0x8000) & 0xFFFF0000;
    v.vy = (v.vy + 0x8000) & 0xFFFF0000;
    v.vz = (v.vz + 0x8000) & 0xFFFF0000;
    dir->vx = v.vx >> 16;
    dir->vy = v.vy >> 16;
    dir->vz = v.vz >> 16;
    return 1;
}

/* push a position out of the walkmesh triangle edges (recurses into neighbours) */
s32 bgi_pushOutOfEdges(BgiActor *actor, SVECTOR *pos, BgiTriangle *tri, s16 *outEdge) {
    BgiVertex *e[2];
    SVECTOR p;
    BgiVertex v[3];
    VECTOR push;
    s16 on;
    BgiFloor *t;
    BgiTriangle *nb;
    s16 i;
    s32 hit;
    s32 d;
    s16 n;
    BgiVertex *vt;

    t = &g_fieldBgi.floors[tri->floor];
    p.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    p.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    p.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    vt = g_fieldBgi.vertices;
    for (i = 0; i < 3; i++) {
        v[i].vx = p.vx + vt[tri->vertices[i]].vx;
        v[i].vy = p.vy + vt[tri->vertices[i]].vy;
        v[i].vz = p.vz + vt[tri->vertices[i]].vz;
    }
    e[0] = &v[2];
    hit = 0;
    for (i = 0; i < 3; i++) {
        e[1] = &v[i];
        d = bgi_closestPointOnSegment(pos, (SVECTOR *)e[0], (SVECTOR *)e[1], &p, &on);
        if ((u32)actor->radiusSq >= d) {
            nb = bgi_getPassableNeighbor(actor, tri, i);
            if (nb != NULL) {
                if (!(nb->flags & BGI_TRI_VISITED)) {
                    nb->flags |= BGI_TRI_VISITED;
                    if (bgi_pushOutOfEdges(actor, pos, nb, outEdge) == 0) {
                        *outEdge = i;
                        hit = 1;
                    }
                }
            } else {
                do {
                    hit = 1;
                } while (0);
                if (d != 0) {
                    d = (s16)SquareRoot0(d);
                    if (p.vx == pos->vx) {
                        push.vx = 0;
                    } else {
                        push.vx = (s16)((pos->vx - p.vx) * actor->radius / d);
                    }
                    if (p.vz == pos->vz) {
                        push.vz = 0;
                    } else {
                        push.vz = (s16)((pos->vz - p.vz) * actor->radius / d);
                    }
                    n = g_fieldBgiPushCount;
                    if (n + 1 < 16) {
                        push.vx = p.vx - pos->vx + push.vx;
                        push.vz = p.vz - pos->vz + push.vz;
                        g_fieldBgiPushVecs[n][0] = push.vx;
                        g_fieldBgiPushCount++;
                        g_fieldBgiPushVecs[n][2] = push.vz;
                        g_fieldBgiPushOnEdge[n] = on;
                        g_fieldBgiPushDist[n] = d;
                    }
                    hit = 1;
                    tri->flags |= BGI_TRI_VISITED;
                    *outEdge = i;
                }
            }
        }
        e[0] = e[1];
    }
    return hit ^ 1;
}

u16 bgi_findTriangleNear(BgiActor *actor, BgiTriangle **triInOut, s32 pos, s32 depth) {
    BgiTriangle *p[2];
    u16 i;

    if (depth < 9) {
        p[0] = *triInOut;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[p[0]->floor], p[0], (s16 *)pos)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(actor, p[0], i);
            if (p[1] != NULL && (u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[p[1]->floor], p[1], (s16 *)pos)) {
                actor->triangle = p[0]->neighbors[i];
                if (actor->triangle >= 0) {
                    p[1] = &g_fieldBgi.triangles[actor->triangle];
                    actor->floor = p[1]->floor;
                    *triInOut = p[1];
                    return 1;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(actor, p[0], i);
            if (p[1] != NULL && !(p[1]->flags & BGI_TRI_VISITED)) {
                p[1]->flags |= BGI_TRI_VISITED;
                if (bgi_findTriangleNear(actor, &p[1], pos, depth + 1) == 1) {
                    *triInOut = p[1];
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* slide a position along the walkmesh from `from` towards `to`, following neighbour triangles */
s32 bgi_slide(BgiActor *actor, BgiTriangle **triInOut, s16 *from, SVECTOR *to, s16 unusedFlag, s16 depth, s32 fracX, s32 fracZ) {
    SVECTOR p;
    SVECTOR *pp;
    VECTOR n;
    SVECTOR save;
    SVECTOR d;
    SVECTOR e;
    VECTOR f;
    s16 edge;
    BgiTriangle *cur;
    s16 prev;
    u8 done;
    s16 saved;
    s32 off;
    s16 s;
    s16 l;
    s16 nb;
    BgiTriangle *q;
    u8 r;

    cur = *triInOut;
    prev = -1;
    p.vx = from[0];
    p.vy = from[1];
    p.vz = from[2];
    save.vx = to->vx;
    save.vy = to->vy;
    done = 0;
    pp = &p;
    save.vz = to->vz;
    edge = -1;
    saved = actor->triangle;
    off = actor->triangle * sizeof(BgiTriangle);
    do {
        d.vx = to->vx - p.vx;
        d.vy = to->vy - pp->vy;
        d.vz = to->vz - pp->vz;
        if (cur->normal >= 0) {
            bgi_projectOnPlane((VECTOR *)&g_fieldBgi.normals[cur->normal], &d, (s32)pp, &n);
        } else {
            bgi_projectOnPlane(&g_fieldBgiDefaultNormal, &d, (s32)pp, &n);
        }
        to->vx = pp->vx + d.vx;
        to->vy = pp->vy + d.vy;
        to->vz = pp->vz + d.vz;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)to)) {
            done = 1;
        } else if ((r = bgi_findEdgeCrossing(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)to, from, pp, &edge, edge))) {
            if (bgi_getPassableNeighbor(actor, cur, edge) != NULL) {
                if (cur->normal >= 0) {
                    e.vx = to->vx - p.vx;
                    e.vy = to->vy - pp->vy;
                    e.vz = to->vz - pp->vz;
                    s = SquareRoot0(n.vx * n.vx + n.vz * n.vz);
                    if (s == 0) {
                        to->vy = pp->vy;
                    } else {
                        l = SquareRoot0(e.vx * e.vx + e.vy * e.vy + e.vz * e.vz);
                        f.vz = n.vz * l / s;
                        f.vx = n.vx * l / s;
                        f.vy = 0;
                        e.vy = 0;
                        e.vx = f.vz;
                        e.vz = -f.vx;
                        to->vx = p.vx + e.vx;
                        to->vy = pp->vy + e.vy;
                        to->vz = pp->vz + e.vz;
                    }
                }
                nb = cur->neighbors[edge];
                if (nb == prev) {
                    actor->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    actor->floor = cur->floor;
                    *triInOut = cur;
                    return 1;
                }
                prev = actor->triangle;
                actor->triangle = cur->neighbors[edge];
                q = &g_fieldBgi.triangles[nb];
                {
                    BgiEdge *ee = &g_fieldBgi.edges[cur->edges[edge]];
                    actor->floor = q->floor;
                    edge = ee->clone;
                }
                cur = q;
                if (edge == -1) {
                    actor->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    actor->floor = cur->floor;
                    *triInOut = cur;
                    return 1;
                }
            } else {
                actor->triangle = saved;
                cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                actor->floor = cur->floor;
                *triInOut = cur;
                return 1;
            }
        } else {
            bgi_slide_clear();
            if ((u8)bgi_findTriangleNear(actor, &cur, (s32)to, 0)) {
                done = 1;
            } else {
                if (depth < 6) {
                    to->vx = save.vx;
                    to->vy = save.vy;
                    to->vz = save.vz;
                    bgi_slide_push(to, from, &fracX, &fracZ);
                    actor->actor->pos.vx = to->vx;
                    actor->actor->pos.vy = to->vy;
                    actor->actor->pos.vz = to->vz;
                    actor->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    actor->floor = cur->floor;
                    if (bgi_slide(actor, &cur, from, to, unusedFlag, depth + 1, fracX, fracZ) == 0) {
                        break;
                    }
                }
                actor->triangle = saved;
                cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                actor->floor = cur->floor;
                *triInOut = cur;
                return 1;
            }
        }
    } while (!done);
    *triInOut = cur;
    return 0;
}

s32 bgi_applyPushes(BgiActor *actor, SVECTOR *pos, BgiTriangle **triInOut, s16 *unused) {
    VECTOR sum;
    SVECTOR prev;
    u8 i;
    u8 j;
    u8 dup;
    u8 any;
    s32 n;

    if (g_fieldBgiPushCount == 0) {
        return 1;
    }
    prev.vx = pos->vx;
    prev.vy = pos->vy;
    prev.vz = pos->vz;
    if (g_fieldBgiPushCount == 1) {
        pos->vx += g_fieldBgiPushVecs[0][0];
        pos->vz += g_fieldBgiPushVecs[0][2];
        actor->actor->pos.vx = pos->vx;
        actor->actor->pos.vz = pos->vz;
    } else {
        any = 0;
        i = 0;
        n = 0;
        sum.vx = 0;
        sum.vz = 0;
        for (; i < g_fieldBgiPushCount; i++) {
            if (g_fieldBgiPushOnEdge[i] != 0) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (g_fieldBgiPushVecs[j][0] == g_fieldBgiPushVecs[i][0] && g_fieldBgiPushVecs[j][2] == g_fieldBgiPushVecs[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    any = 1;
                    sum.vx += g_fieldBgiPushVecs[i][0];
                    n++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
        }
        if (any) {
            if (n >= 2) {
                sum.vx /= n;
                sum.vz /= n;
            }
        } else {
            n = 0;
            for (i = 0; i < g_fieldBgiPushCount; i++) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (g_fieldBgiPushVecs[j][0] == g_fieldBgiPushVecs[i][0] && g_fieldBgiPushVecs[j][2] == g_fieldBgiPushVecs[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    sum.vx += g_fieldBgiPushVecs[i][0];
                    n++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
            if (n > 0) {
                sum.vx /= n;
                sum.vy /= n;
            }
        }
        pos->vx += sum.vx;
        pos->vz += sum.vz;
        actor->actor->pos.vx = pos->vx;
        actor->actor->pos.vz = pos->vz;
    }
    g_fieldBgiPushCount = 0;
    return bgi_slide(actor, triInOut, &prev, pos, 0, 0, 0, 0) == 0;
}

/* clears the visited flag of every walkmesh triangle first */
static inline s32 bgi_pushOutOfEdges_inline(BgiActor *e, SVECTOR *pos, BgiTriangle *nd, s16 *edge) {
    u16 k;
    BgiTriangle *n;
    s32 cnt;
    u16 m = ~0x80;

    n = g_fieldBgi.triangles;
    cnt = g_fieldBgi.header->triangleCount;
    for (k = 0; k < cnt; k++) {
        n->flags &= m;
        n++;
    }
    g_fieldBgiPushCount = 0;
    return bgi_pushOutOfEdges(e, pos, nd, edge);
}

s32 bgi_moveActor(BgiActor *actor) {
    SVECTOR cur;
    SVECTOR prev;
    BgiTriangle *node;
    s16 edge;
    s32 result;
    s32 id;
    s16 save;
    s32 k;
    BgiVertex *vt;
    BgiNormal *pl;
    s32 a;
    BgiFloor *t;

    result = BGI_MOVE_OK;
    g_fieldBgiMoveTriCount = 0;
    for (k = 0; k < 10; k++) {
        g_fieldBgiMoveTris[k] = -1;
    }
    g_fieldBgiMoveEdgeCount = 0;
    for (k = 0; k < 10; k++) {
        g_fieldBgiMoveEdges[k] = -1;
    }
    cur.vx = actor->actor->pos.vx;
    cur.vy = actor->actor->pos.vy;
    cur.vz = actor->actor->pos.vz;
    prev.vx = actor->actor->prevPos.vx;
    prev.vy = actor->actor->prevPos.vy;
    prev.vz = actor->actor->prevPos.vz;
    if (cur.vx == prev.vx && cur.vy == prev.vy && cur.vz == prev.vz) {
        actor->lastFloor = actor->floor;
        actor->lastTriangle = actor->triangle;
        return result | BGI_MOVE_STILL;
    }
    id = actor->triangle;
    node = &g_fieldBgi.triangles[id];
    save = actor->triangle;
    if (bgi_slide(actor, &node, (s16 *)&prev, &cur, 1, 0, 0, 0) != 0) {
        actor->triangle = save;
        if (id >= 0) {
            actor->floor = g_fieldBgi.triangles[id].floor;
        } else {
            actor->floor = -1;
        }
        actor->actor->pos.vx = actor->actor->prevPos.vx;
        actor->actor->pos.vy = actor->actor->prevPos.vy;
        actor->actor->pos.vz = actor->actor->prevPos.vz;
        result |= BGI_MOVE_BLOCKED;
        if (actor->lastTriangle >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[actor->lastFloor], &g_fieldBgi.triangles[actor->lastTriangle], &prev.vx)) {
                actor->lastFloor = actor->floor;
                actor->lastTriangle = actor->triangle;
            }
        }
        return result;
    }
    if (!(u8)bgi_pushOutOfEdges_inline(actor, &cur, node, &edge)) {
        if ((u8)bgi_applyPushes(actor, &cur, &node, &edge)) {
            result |= BGI_MOVE_PUSHED | BGI_MOVE_BLOCKED;
        } else {
            actor->triangle = save;
            if (save >= 0) {
                actor->floor = g_fieldBgiTriangles[save].floor;
            } else {
                actor->floor = -1;
            }
            actor->actor->pos.vx = actor->actor->prevPos.vx;
            actor->actor->pos.vy = actor->actor->prevPos.vy;
            actor->actor->pos.vz = actor->actor->prevPos.vz;
            result |= BGI_MOVE_BLOCKED;
            if (actor->lastTriangle >= 0 && actor->lastFloor >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[actor->lastFloor], &g_fieldBgi.triangles[actor->lastTriangle], &prev.vx)) {
                actor->lastFloor = actor->floor;
                actor->lastTriangle = actor->triangle;
            }
            }
            return result;
        }
    }
    if (cur.vx != actor->actor->pos.vx || cur.vz != actor->actor->pos.vz) {
        actor->actor->pos.vx = cur.vx;
        result |= BGI_MOVE_CORRECTED;
        actor->actor->pos.vz = cur.vz;
    }
    if (node->normal >= 0) {
        pl = &g_fieldBgi.normals[node->normal];
        if ((u32)(pl->ny + 0x50F4) > 0xA1E8) {
            a = fld_fixMul(pl->nx, actor->actor->pos.vx << 16);
            actor->actor->pos.vy = fld_fixMul(node->d - a - fld_fixMul(pl->nz, actor->actor->pos.vz << 16), pl->invNy) >> 16;
            actor->actor->pos.vy += (s16)g_fieldBgi.floors[node->floor].curPos[1] - g_fieldBgi.floors[node->floor].orgPos[1];
        } else {
            actor->actor->pos.vy = cur.vy;
        }
    } else {
        t = g_fieldBgi.floors + node->floor;
        vt = g_fieldBgi.vertices + node->vertices[0];
        actor->actor->pos.vy = (s16)(vt->vy + (g_fieldBgi.header->curPos[1] + t->curPos[1]));
    }
    actor->lastTriangle = save;
    if (save >= 0) {
        actor->lastFloor = g_fieldBgiTriangles[save].floor;
    } else {
        actor->lastFloor = -1;
    }
    return result;
}

s32 bgi_init(BgiHeader *bgi) {
    u16 i;
    BgiAnim *p;

    g_fieldBgi.header = bgi;
    g_fieldBgi.vertices = (BgiVertex *)((u8 *)bgi + bgi->vertexOffset);
    g_fieldBgi.triangles = (BgiTriangle *)((u8 *)bgi + bgi->triangleOffset);
    g_fieldBgi.normals = (BgiNormal *)((u8 *)bgi + bgi->normalOffset);
    g_fieldBgi.edges = (u8 *)bgi + bgi->edgeOffset;
    g_fieldBgi.floors = (BgiFloor *)((u8 *)bgi + bgi->floorOffset);
    p = (BgiAnim *)((u8 *)bgi + bgi->animOffset);
    g_fieldBgi.maskFilter = 0xFF;
    g_fieldBgi.actorCount = 9;
    g_fieldBgi.actors = g_fieldBgiActors;
    g_fieldBgi.anims = p;
    for (i = 0; i < 24; i++) {
        g_fieldBgi.actors[i].flags = 0;
        g_fieldBgi.actors[i].unk2 = 0;
        g_fieldBgi.actors[i].floor = -1;
        g_fieldBgi.actors[i].triangle = -1;
        g_fieldBgi.actors[i].lastFloor = -1;
        g_fieldBgi.actors[i].lastTriangle = -1;
        g_fieldBgi.actors[i].radius = 0x60;
        g_fieldBgi.actors[i].radiusSq = 0x2400;
        g_fieldBgi.actors[i].actor = NULL;
    }
    g_fieldBgi.loaded = 1;
    for (i = 0; i < g_fieldBgi.header->animCount; i++) {
        bgi_initAnim(i);
    }
    bgi_resetFloorMoves();
    return 1;
}

BgiActor *bgi_findPlayerActor(s32 *outIndex) {
    EvtObject *a;
    s32 i;
    u8 mode;

    *outIndex = 0;
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else if (mode == GS_MODE_FIELD) {
        a = evt_getPlayer();
    } else {
        a = NULL;
    }
    if (a != NULL) {
        for (i = 0; i < g_fieldBgi.actorCount; i++) {
            if (g_fieldBgi.actors[i].actor == a) {
                *outIndex = i;
                return &g_fieldBgi.actors[i];
            }
        }
    }
    return NULL;
}

s32 bgi_setActorActive(s32 actorId, s32 active) {
    if (active != 0) {
        g_fieldBgi.actors[actorId].flags |= BGI_FLAG_ACTIVE;
    } else {
        g_fieldBgi.actors[actorId].flags &= ~BGI_FLAG_ACTIVE;
        g_fieldBgi.actors[actorId].floor = -1;
        g_fieldBgi.actors[actorId].triangle = -1;
        g_fieldBgi.actors[actorId].lastFloor = -1;
        g_fieldBgi.actors[actorId].lastTriangle = -1;
    }
    return 1;
}

/* put actor actorId on the floor below (x, y, z) */
s32 bgi_placeActor(s32 actorId, s16 x, s16 y, s16 z) {
    SVECTOR pos;
    VECTOR v;
    s16 edge;
    BgiTriangle *n;
    BgiHeader *b;
    BgiActor *a;
    BgiNormal *pl;
    s16 ymin;
    u16 found;
    s16 bi;
    s16 bt;
    u16 i;
    u16 hit;
    s32 t;
    s32 cnt;
    BgiTriangle *p;
    u16 j;
    u16 m;
    s32 k;
    BgiFloor *tri;
    BgiVertex *vt;

    ymin = 0x7FFF;
    bi = -1;
    bt = -1;
    found = 0;
    i = 0;
    pos.vx = x;
    pos.vy = y;
    pos.vz = z;
    n = g_fieldBgi.triangles;
    b = g_fieldBgi.header;
    a = &g_fieldBgi.actors[actorId];
    for (; i < b->triangleCount; i++, n++) {
        hit = 0;
        if ((u8)bgi_pointInTriangleXZ(b, &g_fieldBgi.floors[n->floor], n, &pos)) {
            m = ~0x80;
            p = g_fieldBgi.triangles;
            cnt = g_fieldBgi.header->triangleCount;
            for (j = hit; j < cnt; j++) {
                p->flags &= m;
                p++;
            }
            g_fieldBgiPushCount = 0;
            if (bgi_pushOutOfEdges(a, &pos, n, &edge) || bgi_applyPushes(a, &pos, &n, &edge)) {
                hit = 1;
            }
            if (hit) {
                k = n->normal;
                if (k >= 0) {
                    v.vx = pos.vx;
                    v.vy = pos.vy;
                    v.vz = pos.vz;
                    pl = &g_fieldBgi.normals[k];
                    t = fld_fixMul(pl->nx, v.vx << 16);
                    v.vy = fld_fixMul(n->d - t - fld_fixMul(pl->nz, v.vz << 16), pl->invNy) >> 16;
                    v.vy += (s16)g_fieldBgi.floors[n->floor].curPos[1] - g_fieldBgi.floors[n->floor].orgPos[1];
                    pos.vy = v.vy;
                } else {
                    tri = g_fieldBgi.floors + n->floor;
                    vt = g_fieldBgi.vertices + n->vertices[0];
                    pos.vy = vt->vy + (g_fieldBgi.header->curPos[1] + tri->curPos[1]);
                }
                if (pos.vy >= y && pos.vy < ymin) {
                    found = 1;
                    ymin = pos.vy;
                    bi = i;
                    bt = n->floor;
                }
            }
        }
    }
    if (found) {
        a->floor = bt;
        a->triangle = bi;
        a->actor->pos.vx = x;
        a->actor->pos.vy = ymin;
        a->actor->pos.vz = z;
        return 1;
    }
    return 0;
}

s32 bgi_setActorTriangle(s32 actorId, s16 triId) {
    BgiTriangle *n;
    BgiFloor *t;
    SVECTOR v;

    n = &g_fieldBgi.triangles[(u16)triId];
    t = &g_fieldBgi.floors[n->floor];
    v.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    v.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    v.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    g_fieldBgi.actors[actorId].floor = n->floor;
    g_fieldBgi.actors[actorId].triangle = triId;
    g_fieldBgi.actors[actorId].actor->pos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[actorId].actor->pos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[actorId].actor->pos.vz = v.vz + n->centerZ;
    g_fieldBgi.actors[actorId].actor->prevPos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[actorId].actor->prevPos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[actorId].actor->prevPos.vz = v.vz + n->centerZ;
    return 1;
}

s32 bgi_setActorFloorTriangle(s32 actorId, u16 floorId, u16 triIdx) {
    BgiFloor *t;
    BgiTriangle *n;
    s32 *ids;
    s32 id;
    SVECTOR v;

    t = &g_fieldBgi.floors[floorId];
    ids = (s32 *)((u8 *)g_fieldBgi.header + t->triangleOffset);
    id = ids[triIdx];
    v.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    v.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    v.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    n = &g_fieldBgi.triangles[id];
    g_fieldBgi.actors[actorId].floor = floorId;
    g_fieldBgi.actors[actorId].triangle = ids[triIdx];
    g_fieldBgi.actors[actorId].actor->pos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[actorId].actor->pos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[actorId].actor->pos.vz = v.vz + n->centerZ;
    g_fieldBgi.actors[actorId].actor->prevPos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[actorId].actor->prevPos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[actorId].actor->prevPos.vz = v.vz + n->centerZ;
    return 1;
}

s32 bgi_updateActor(u32 actorId) {
    BgiActor *e;

    if (g_fieldBgi.loaded == 0) {
        return BGI_ERR_NO_MESH;
    }
    if (actorId >= g_fieldBgi.actorCount) {
        return BGI_ERR_BAD_ACTOR;
    }
    e = &g_fieldBgi.actors[actorId];
    if (e->flags & BGI_FLAG_ACTIVE) {
        if (e->floor == -1 || e->triangle == -1) {
            return (bgi_placeActor(actorId, e->actor->pos.vx, e->actor->pos.vy, e->actor->pos.vz) != 0) * 2;
        }
        return bgi_moveActor(e);
    }
    return 0;
}

s16 bgi_findTriangleBelow(s16 posX, s16 posY, s16 posZ, s16 *outTriId, BgiTriangle **outTri) {
    SVECTOR p;
    VECTOR v;
    s16 y0;
    s32 best;
    s16 found;
    s16 i;
    BgiTriangle *n;
    BgiHeader *b;
    BgiNormal *pl;
    s32 a;
    BgiFloor *t;
    s16 k;
    s16 y;
    BgiVertex *vx;

    best = 0x7FFF;
    *outTriId = -1;
    *outTri = NULL;
    n = g_fieldBgi.triangles;
    b = g_fieldBgi.header;
    found = 0;
    p.vx = posX;
    p.vy = posY;
    p.vz = posZ;
    i = 0;
    y0 = posY;
    for (; i < b->triangleCount; n++, i++) {
        if ((u8)bgi_pointInTriangleXZ(b, &g_fieldBgi.floors[n->floor], n, &p)) {
            k = n->normal;
            if (k >= 0) {
                v.vx = p.vx;
                v.vy = p.vy;
                v.vz = p.vz;
                pl = &g_fieldBgi.normals[k];
                a = fld_fixMul(pl->nx, v.vx << 16);
                v.vy = fld_fixMul(n->d - a - fld_fixMul(pl->nz, v.vz << 16), pl->invNy) >> 16;
                v.vy += (s16)g_fieldBgi.floors[n->floor].curPos[1] - g_fieldBgi.floors[n->floor].orgPos[1];
                p.vy = v.vy;
            } else {
                t = &g_fieldBgi.floors[n->floor];
                vx = &g_fieldBgi.vertices[n->vertices[0]];
                p.vy = vx->vy + (g_fieldBgi.header->curPos[1] + t->curPos[1]);
            }
            y = p.vy;
            if (y >= y0 && y < best) {
                found = 1;
                *outTriId = i;
                best = y;
                *outTri = n;
            }
        }
    }
    return found;
}

s32 bgi_setFloorActive(s32 floorId, s32 active) {
    BgiHeader *b = g_fieldBgi.header;
    BgiFloor *t = &((BgiFloor *)((u8 *)b + b->floorOffset))[floorId];
    s32 *ids;
    s32 i;
    BgiTriangle *n;

    if (active != 0) {
        ids = (s32 *)((u8 *)b + t->triangleOffset);
        t->flags |= BGI_FLAG_ACTIVE;
        n = g_fieldBgi.triangles;
        for (i = 0; i < t->triangleCount; i++) {
            n[ids[i]].flags |= BGI_FLAG_ACTIVE;
        }
    } else {
        ids = (s32 *)((u8 *)b + t->triangleOffset);
        t->flags &= ~BGI_FLAG_ACTIVE;
        n = g_fieldBgi.triangles;
        for (i = 0; i < t->triangleCount; i++) {
            n[ids[i]].flags &= ~BGI_FLAG_ACTIVE;
        }
    }
    return 1;
}

s16 bgi_getHeight(s16 x, s16 z) {
    SVECTOR p;
    VECTOR v;
    u16 i;
    BgiTriangle *n;
    BgiNormal *pl;
    s32 a;
    s16 k;
    u8 hit;
    GameData *g;

    if (g_fieldBgi.header == NULL) {
        if (g_gameState->mode == 5) {
            g = g_gameState->gameData;
            return ((EvtObject *)g->npcModels[g->ctx.field->playerObj].actor)->pos.vy;
        }
        if (g_gameState->mode == GS_MODE_FIELD) {
            return evt_getPlayer()->pos.vy;
        }
        return 0;
    }
    goto start;
plane:
    v.vx = p.vx;
    v.vy = p.vy;
    v.vz = p.vz;
    pl = &g_fieldBgi.normals[k];
    a = fld_fixMul(pl->nx, v.vx << 16);
    v.vy = fld_fixMul(n->d - a - fld_fixMul(pl->nz, v.vz << 16), pl->invNy) >> 16;
    v.vy += (s16)g_fieldBgi.floors[n->floor].curPos[1] - g_fieldBgi.floors[n->floor].orgPos[1];
    p.vy = v.vy;
    return p.vy;
start:
    p.vx = x;
    p.vy = -0x5CD7;
    p.vz = z;
    n = g_fieldBgi.triangles;
    for (i = 0; i < g_fieldBgi.header->triangleCount; i++, n++) {
        if ((u8)bgi_pointInTriangleXZ(g_fieldBgi.header, &g_fieldBgi.floors[n->floor], n, &p)) {
            k = n->normal;
            if (k >= 0) {
                goto plane;
            }
            {
                p.vy = g_fieldBgi.header->curPos[1];
                p.vy += g_fieldBgi.floors[n->floor].curPos[1];
                p.vy += g_fieldBgi.vertices[n->vertices[0]].vy;
                return p.vy;
            }
        }
    }
    return -0x7FFF;
}

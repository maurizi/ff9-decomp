#include "ovl_0a.h"

/* does the segment from arg0 to arg1 cross the edge arg2-arg3 (on axes x, z)? */
s32 bgi_segmentsCross(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 x, u8 z) {
    s32 t2;
    s32 t3;
    s32 t4;
    s32 ex;
    s32 ez;

    t2 = (arg1[x] - arg0[x]) * (ez = arg3[z] - arg2[z]);
    ex = arg3[x] - arg2[x];
    t3 = (arg0[z] - arg2[z]) * ex;
    t2 -= (arg1[z] - arg0[z]) * ex;
    t3 -= (arg0[x] - arg2[x]) * ez;
    if (ABS(t3) <= ABS(t2)) {
        if ((t3 >= 0 && t2 >= 0) || (t3 <= 0 && t2 < 0)) {
            t4 = (arg0[z] - arg2[z]) * (arg1[x] - arg0[x]);
            t4 -= (arg0[x] - arg2[x]) * (arg1[z] - arg0[z]);
            if ((t4 >= 0 && t2 >= 0) || (t4 <= 0 && t2 < 0)) {
                return 1;
            }
        }
    }
    return 0;
}

BgiTriangle *bgi_getPassableNeighbor(BgiActor *arg0, BgiTriangle *arg1, s32 arg2) {
    BgiTriangle *n;
    EvtObject *a;
    s16 id;
    u8 m;
    u8 mode;

    id = arg1->neighbors[arg2];
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
        } else if (mode == 1) {
            a = evt_getPlayer();
        } else {
            a = NULL;
        }
        if (arg0->actor == a) {
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

/* closest point to arg0 on segment arg1-arg2 -> arg3; returns squared distance */
s32 bgi_closestPointOnSegment(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, SVECTOR *arg3, s16 *arg4) {
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

    px = arg0->vx;
    ax = arg1->vx;
    bx = arg2->vx;
    py = arg0->vy;
    ay = arg1->vy;
    by = arg2->vy;
    pz = arg0->vz;
    az = arg1->vz;
    bz = arg2->vz;
    ta = (px - ax) * (bx - ax);
    ta += (py - ay) * (by - ay);
    ta += (pz - az) * (bz - az);
    if (ta <= 0) {
        c.vx = arg1->vx;
        c.vy = arg1->vy;
        c.vz = arg1->vz;
        if (ta == 0) {
            *arg4 = 1;
        } else {
            *arg4 = 0;
        }
    } else {
        tb = (px - bx) * (ax - bx);
        tb += (py - by) * (ay - by);
        tb += (pz - bz) * (az - bz);
        if (tb <= 0) {
            c.vx = arg2->vx;
            c.vy = arg2->vy;
            c.vz = arg2->vz;
            if (tb == 0) {
                *arg4 = 1;
            } else {
                *arg4 = 0;
            }
        } else {
            while (ta > 0x10000 || tb > 0x10000) {
                ta >>= 1;
                tb = tb >> 1;
            }
            s = ta + tb;
            c.vx = arg1->vx + (arg2->vx - arg1->vx) * ta / s;
            c.vy = arg1->vy + (arg2->vy - arg1->vy) * ta / s;
            c.vz = arg1->vz + (arg2->vz - arg1->vz) * ta / s;
            *arg4 = 1;
        }
    }
    dx = c.vx - arg0->vx;
    dy = c.vy - arg0->vy;
    dz = c.vz - arg0->vz;
    arg3->vx = c.vx;
    arg3->vy = c.vy;
    s = dx * dx;
    s += dy * dy;
    arg3->vz = c.vz;
    return s + dz * dz;
}

s32 bgi_projectOnPlane(VECTOR *arg0, SVECTOR *arg1, s32 arg2, VECTOR *arg3) {
    VECTOR a;
    VECTOR v;

    v.vx = arg1->vx << 16;
    v.vy = arg1->vy << 16;
    v.vz = arg1->vz << 16;
    a.vy = 0;
    a.vz = v.vx;
    a.vx = -v.vz;
    arg3->vx = -arg1->vz;
    arg3->vy = 0;
    arg3->vz = arg1->vx;
    if (arg0->vy >= 0) {
        v.vx = fld_fixMul(arg0->vy, a.vz);
        v.vy = fld_fixMul(arg0->vz, a.vx) - fld_fixMul(arg0->vx, a.vz);
        v.vz = -fld_fixMul(arg0->vy, a.vx);
    } else {
        v.vx = -fld_fixMul(arg0->vy, a.vz);
        v.vy = fld_fixMul(arg0->vx, a.vz) - fld_fixMul(arg0->vz, a.vx);
        v.vz = fld_fixMul(arg0->vy, a.vx);
    }
    v.vx = (v.vx + 0x8000) & 0xFFFF0000;
    v.vy = (v.vy + 0x8000) & 0xFFFF0000;
    v.vz = (v.vz + 0x8000) & 0xFFFF0000;
    arg1->vx = v.vx >> 16;
    arg1->vy = v.vy >> 16;
    arg1->vz = v.vz >> 16;
    return 1;
}

/* push a position out of the walkmesh triangle edges (recurses into neighbours) */
s32 bgi_pushOutOfEdges(BgiActor *arg0, SVECTOR *arg1, BgiTriangle *arg2, s16 *arg3) {
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

    t = &g_fieldBgi.floors[arg2->floor];
    p.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    p.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    p.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    vt = g_fieldBgi.vertices;
    for (i = 0; i < 3; i++) {
        v[i].vx = p.vx + vt[arg2->vertices[i]].vx;
        v[i].vy = p.vy + vt[arg2->vertices[i]].vy;
        v[i].vz = p.vz + vt[arg2->vertices[i]].vz;
    }
    e[0] = &v[2];
    hit = 0;
    for (i = 0; i < 3; i++) {
        e[1] = &v[i];
        d = bgi_closestPointOnSegment(arg1, (SVECTOR *)e[0], (SVECTOR *)e[1], &p, &on);
        if ((u32)arg0->radiusSq >= d) {
            nb = bgi_getPassableNeighbor(arg0, arg2, i);
            if (nb != NULL) {
                if (!(nb->flags & BGI_TRI_VISITED)) {
                    nb->flags |= BGI_TRI_VISITED;
                    if (bgi_pushOutOfEdges(arg0, arg1, nb, arg3) == 0) {
                        *arg3 = i;
                        hit = 1;
                    }
                }
            } else {
                do {
                    hit = 1;
                } while (0);
                if (d != 0) {
                    d = (s16)SquareRoot0(d);
                    if (p.vx == arg1->vx) {
                        push.vx = 0;
                    } else {
                        push.vx = (s16)((arg1->vx - p.vx) * arg0->radius / d);
                    }
                    if (p.vz == arg1->vz) {
                        push.vz = 0;
                    } else {
                        push.vz = (s16)((arg1->vz - p.vz) * arg0->radius / d);
                    }
                    n = g_fieldBgiPushCount;
                    if (n + 1 < 16) {
                        push.vx = p.vx - arg1->vx + push.vx;
                        push.vz = p.vz - arg1->vz + push.vz;
                        g_fieldBgiPushVecs[n][0] = push.vx;
                        g_fieldBgiPushCount++;
                        g_fieldBgiPushVecs[n][2] = push.vz;
                        g_fieldBgiPushOnEdge[n] = on;
                        g_fieldBgiPushDist[n] = d;
                    }
                    hit = 1;
                    arg2->flags |= BGI_TRI_VISITED;
                    *arg3 = i;
                }
            }
        }
        e[0] = e[1];
    }
    return hit ^ 1;
}

u16 bgi_findTriangleNear(BgiActor *arg0, BgiTriangle **arg1, s32 arg2, s32 arg3) {
    BgiTriangle *p[2];
    u16 i;

    if (arg3 < 9) {
        p[0] = *arg1;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[p[0]->floor], p[0], (s16 *)arg2)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(arg0, p[0], i);
            if (p[1] != NULL && (u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[p[1]->floor], p[1], (s16 *)arg2)) {
                arg0->triangle = p[0]->neighbors[i];
                if (arg0->triangle >= 0) {
                    p[1] = &g_fieldBgi.triangles[arg0->triangle];
                    arg0->floor = p[1]->floor;
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(arg0, p[0], i);
            if (p[1] != NULL && !(p[1]->flags & BGI_TRI_VISITED)) {
                p[1]->flags |= BGI_TRI_VISITED;
                if (bgi_findTriangleNear(arg0, &p[1], arg2, arg3 + 1) == 1) {
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* slide a position along the walkmesh from arg2 towards arg3, following neighbour triangles */
s32 bgi_slide(BgiActor *arg0, BgiTriangle **arg1, s16 *arg2, SVECTOR *arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7) {
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

    cur = *arg1;
    prev = -1;
    p.vx = arg2[0];
    p.vy = arg2[1];
    p.vz = arg2[2];
    save.vx = arg3->vx;
    save.vy = arg3->vy;
    done = 0;
    pp = &p;
    save.vz = arg3->vz;
    edge = -1;
    saved = arg0->triangle;
    off = arg0->triangle * sizeof(BgiTriangle);
    do {
        d.vx = arg3->vx - p.vx;
        d.vy = arg3->vy - pp->vy;
        d.vz = arg3->vz - pp->vz;
        if (cur->normal >= 0) {
            bgi_projectOnPlane((VECTOR *)&g_fieldBgi.normals[cur->normal], &d, (s32)pp, &n);
        } else {
            bgi_projectOnPlane(&g_fieldBgiDefaultNormal, &d, (s32)pp, &n);
        }
        arg3->vx = pp->vx + d.vx;
        arg3->vy = pp->vy + d.vy;
        arg3->vz = pp->vz + d.vz;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)arg3)) {
            done = 1;
        } else if ((r = bgi_findEdgeCrossing(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)arg3, arg2, pp, &edge, edge))) {
            if (bgi_getPassableNeighbor(arg0, cur, edge) != NULL) {
                if (cur->normal >= 0) {
                    e.vx = arg3->vx - p.vx;
                    e.vy = arg3->vy - pp->vy;
                    e.vz = arg3->vz - pp->vz;
                    s = SquareRoot0(n.vx * n.vx + n.vz * n.vz);
                    if (s == 0) {
                        arg3->vy = pp->vy;
                    } else {
                        l = SquareRoot0(e.vx * e.vx + e.vy * e.vy + e.vz * e.vz);
                        f.vz = n.vz * l / s;
                        f.vx = n.vx * l / s;
                        f.vy = 0;
                        e.vy = 0;
                        e.vx = f.vz;
                        e.vz = -f.vx;
                        arg3->vx = p.vx + e.vx;
                        arg3->vy = pp->vy + e.vy;
                        arg3->vz = pp->vz + e.vz;
                    }
                }
                nb = cur->neighbors[edge];
                if (nb == prev) {
                    arg0->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    arg0->floor = cur->floor;
                    *arg1 = cur;
                    return 1;
                }
                prev = arg0->triangle;
                arg0->triangle = cur->neighbors[edge];
                q = &g_fieldBgi.triangles[nb];
                {
                    BgiEdge *ee = &g_fieldBgi.edges[cur->edges[edge]];
                    arg0->floor = q->floor;
                    edge = ee->clone;
                }
                cur = q;
                if (edge == -1) {
                    arg0->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    arg0->floor = cur->floor;
                    *arg1 = cur;
                    return 1;
                }
            } else {
                arg0->triangle = saved;
                cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                arg0->floor = cur->floor;
                *arg1 = cur;
                return 1;
            }
        } else {
            bgi_slide_clear();
            if ((u8)bgi_findTriangleNear(arg0, &cur, (s32)arg3, 0)) {
                done = 1;
            } else {
                if (arg5 < 6) {
                    arg3->vx = save.vx;
                    arg3->vy = save.vy;
                    arg3->vz = save.vz;
                    bgi_slide_push(arg3, arg2, &arg6, &arg7);
                    arg0->actor->pos.vx = arg3->vx;
                    arg0->actor->pos.vy = arg3->vy;
                    arg0->actor->pos.vz = arg3->vz;
                    arg0->triangle = saved;
                    cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                    arg0->floor = cur->floor;
                    if (bgi_slide(arg0, &cur, arg2, arg3, arg4, arg5 + 1, arg6, arg7) == 0) {
                        break;
                    }
                }
                arg0->triangle = saved;
                cur = (BgiTriangle *)((u8 *)g_fieldBgi.triangles + off);
                arg0->floor = cur->floor;
                *arg1 = cur;
                return 1;
            }
        }
    } while (!done);
    *arg1 = cur;
    return 0;
}

s32 bgi_applyPushes(BgiActor *arg0, SVECTOR *arg1, BgiTriangle **arg2, s16 *arg3) {
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
    prev.vx = arg1->vx;
    prev.vy = arg1->vy;
    prev.vz = arg1->vz;
    if (g_fieldBgiPushCount == 1) {
        arg1->vx += g_fieldBgiPushVecs[0][0];
        arg1->vz += g_fieldBgiPushVecs[0][2];
        arg0->actor->pos.vx = arg1->vx;
        arg0->actor->pos.vz = arg1->vz;
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
        arg1->vx += sum.vx;
        arg1->vz += sum.vz;
        arg0->actor->pos.vx = arg1->vx;
        arg0->actor->pos.vz = arg1->vz;
    }
    g_fieldBgiPushCount = 0;
    return bgi_slide(arg0, arg2, &prev, arg1, 0, 0, 0, 0) == 0;
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

s32 bgi_moveActor(BgiActor *arg0) {
    SVECTOR cur;
    SVECTOR prev;
    BgiTriangle *node;
    s16 edge;
    s32 ret;
    s32 id;
    s16 save;
    s32 k;
    BgiVertex *vt;
    BgiNormal *pl;
    s32 a;
    BgiFloor *t;

    ret = BGI_MOVE_OK;
    g_fieldBgiMoveTriCount = 0;
    for (k = 0; k < 10; k++) {
        g_fieldBgiMoveTris[k] = -1;
    }
    g_fieldBgiMoveEdgeCount = 0;
    for (k = 0; k < 10; k++) {
        g_fieldBgiMoveEdges[k] = -1;
    }
    cur.vx = arg0->actor->pos.vx;
    cur.vy = arg0->actor->pos.vy;
    cur.vz = arg0->actor->pos.vz;
    prev.vx = arg0->actor->prevPos.vx;
    prev.vy = arg0->actor->prevPos.vy;
    prev.vz = arg0->actor->prevPos.vz;
    if (cur.vx == prev.vx && cur.vy == prev.vy && cur.vz == prev.vz) {
        arg0->lastFloor = arg0->floor;
        arg0->lastTriangle = arg0->triangle;
        return ret | BGI_MOVE_STILL;
    }
    id = arg0->triangle;
    node = &g_fieldBgi.triangles[id];
    save = arg0->triangle;
    if (bgi_slide(arg0, &node, (s16 *)&prev, &cur, 1, 0, 0, 0) != 0) {
        arg0->triangle = save;
        if (id >= 0) {
            arg0->floor = g_fieldBgi.triangles[id].floor;
        } else {
            arg0->floor = -1;
        }
        arg0->actor->pos.vx = arg0->actor->prevPos.vx;
        arg0->actor->pos.vy = arg0->actor->prevPos.vy;
        arg0->actor->pos.vz = arg0->actor->prevPos.vz;
        ret |= BGI_MOVE_BLOCKED;
        if (arg0->lastTriangle >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[arg0->lastFloor], &g_fieldBgi.triangles[arg0->lastTriangle], &prev.vx)) {
                arg0->lastFloor = arg0->floor;
                arg0->lastTriangle = arg0->triangle;
            }
        }
        return ret;
    }
    if (!(u8)bgi_pushOutOfEdges_inline(arg0, &cur, node, &edge)) {
        if ((u8)bgi_applyPushes(arg0, &cur, &node, &edge)) {
            ret |= BGI_MOVE_PUSHED | BGI_MOVE_BLOCKED;
        } else {
            arg0->triangle = save;
            if (save >= 0) {
                arg0->floor = g_fieldBgiTriangles[save].floor;
            } else {
                arg0->floor = -1;
            }
            arg0->actor->pos.vx = arg0->actor->prevPos.vx;
            arg0->actor->pos.vy = arg0->actor->prevPos.vy;
            arg0->actor->pos.vz = arg0->actor->prevPos.vz;
            ret |= BGI_MOVE_BLOCKED;
            if (arg0->lastTriangle >= 0 && arg0->lastFloor >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[arg0->lastFloor], &g_fieldBgi.triangles[arg0->lastTriangle], &prev.vx)) {
                arg0->lastFloor = arg0->floor;
                arg0->lastTriangle = arg0->triangle;
            }
            }
            return ret;
        }
    }
    if (cur.vx != arg0->actor->pos.vx || cur.vz != arg0->actor->pos.vz) {
        arg0->actor->pos.vx = cur.vx;
        ret |= BGI_MOVE_CORRECTED;
        arg0->actor->pos.vz = cur.vz;
    }
    if (node->normal >= 0) {
        pl = &g_fieldBgi.normals[node->normal];
        if ((u32)(pl->ny + 0x50F4) > 0xA1E8) {
            a = fld_fixMul(pl->nx, arg0->actor->pos.vx << 16);
            arg0->actor->pos.vy = fld_fixMul(node->d - a - fld_fixMul(pl->nz, arg0->actor->pos.vz << 16), pl->invNy) >> 16;
            arg0->actor->pos.vy += (s16)g_fieldBgi.floors[node->floor].curPos[1] - g_fieldBgi.floors[node->floor].orgPos[1];
        } else {
            arg0->actor->pos.vy = cur.vy;
        }
    } else {
        t = g_fieldBgi.floors + node->floor;
        vt = g_fieldBgi.vertices + node->vertices[0];
        arg0->actor->pos.vy = (s16)(vt->vy + (g_fieldBgi.header->curPos[1] + t->curPos[1]));
    }
    arg0->lastTriangle = save;
    if (save >= 0) {
        arg0->lastFloor = g_fieldBgiTriangles[save].floor;
    } else {
        arg0->lastFloor = -1;
    }
    return ret;
}

s32 bgi_init(BgiHeader *arg0) {
    u16 i;
    BgiAnim *p;

    g_fieldBgi.header = arg0;
    g_fieldBgi.vertices = (BgiVertex *)((u8 *)arg0 + arg0->vertexOffset);
    g_fieldBgi.triangles = (BgiTriangle *)((u8 *)arg0 + arg0->triangleOffset);
    g_fieldBgi.normals = (BgiNormal *)((u8 *)arg0 + arg0->normalOffset);
    g_fieldBgi.edges = (u8 *)arg0 + arg0->edgeOffset;
    g_fieldBgi.floors = (BgiFloor *)((u8 *)arg0 + arg0->floorOffset);
    p = (BgiAnim *)((u8 *)arg0 + arg0->animOffset);
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

BgiActor *bgi_findPlayerActor(s32 *arg0) {
    EvtObject *a;
    s32 i;
    u8 mode;

    *arg0 = 0;
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else if (mode == 1) {
        a = evt_getPlayer();
    } else {
        a = NULL;
    }
    if (a != NULL) {
        for (i = 0; i < g_fieldBgi.actorCount; i++) {
            if (g_fieldBgi.actors[i].actor == a) {
                *arg0 = i;
                return &g_fieldBgi.actors[i];
            }
        }
    }
    return NULL;
}

s32 bgi_setActorActive(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        g_fieldBgi.actors[arg0].flags |= BGI_FLAG_ACTIVE;
    } else {
        g_fieldBgi.actors[arg0].flags &= ~BGI_FLAG_ACTIVE;
        g_fieldBgi.actors[arg0].floor = -1;
        g_fieldBgi.actors[arg0].triangle = -1;
        g_fieldBgi.actors[arg0].lastFloor = -1;
        g_fieldBgi.actors[arg0].lastTriangle = -1;
    }
    return 1;
}

/* put entry arg0 on the floor below (arg1, arg2, arg3) */
s32 bgi_placeActor(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    SVECTOR pos;
    VECTOR v;
    s16 tmp;
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
    pos.vx = arg1;
    pos.vy = arg2;
    pos.vz = arg3;
    n = g_fieldBgi.triangles;
    b = g_fieldBgi.header;
    a = &g_fieldBgi.actors[arg0];
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
            if (bgi_pushOutOfEdges(a, &pos, n, &tmp) || bgi_applyPushes(a, &pos, &n, &tmp)) {
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
                if (pos.vy >= arg2 && pos.vy < ymin) {
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
        a->actor->pos.vx = arg1;
        a->actor->pos.vy = ymin;
        a->actor->pos.vz = arg3;
        return 1;
    }
    return 0;
}

s32 bgi_setActorTriangle(s32 arg0, s16 arg1) {
    BgiTriangle *n;
    BgiFloor *t;
    SVECTOR v;

    n = &g_fieldBgi.triangles[(u16)arg1];
    t = &g_fieldBgi.floors[n->floor];
    v.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    v.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    v.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    g_fieldBgi.actors[arg0].floor = n->floor;
    g_fieldBgi.actors[arg0].triangle = arg1;
    g_fieldBgi.actors[arg0].actor->pos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[arg0].actor->pos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[arg0].actor->pos.vz = v.vz + n->centerZ;
    g_fieldBgi.actors[arg0].actor->prevPos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[arg0].actor->prevPos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[arg0].actor->prevPos.vz = v.vz + n->centerZ;
    return 1;
}

s32 bgi_setActorFloorTriangle(s32 arg0, u16 arg1, u16 arg2) {
    BgiFloor *t;
    BgiTriangle *n;
    s32 *ids;
    s32 id;
    SVECTOR v;

    t = &g_fieldBgi.floors[arg1];
    ids = (s32 *)((u8 *)g_fieldBgi.header + t->triangleOffset);
    id = ids[arg2];
    v.vx = g_fieldBgi.header->curPos[0] + t->curPos[0];
    v.vy = g_fieldBgi.header->curPos[1] + t->curPos[1];
    v.vz = g_fieldBgi.header->curPos[2] + t->curPos[2];
    n = &g_fieldBgi.triangles[id];
    g_fieldBgi.actors[arg0].floor = arg1;
    g_fieldBgi.actors[arg0].triangle = ids[arg2];
    g_fieldBgi.actors[arg0].actor->pos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[arg0].actor->pos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[arg0].actor->pos.vz = v.vz + n->centerZ;
    g_fieldBgi.actors[arg0].actor->prevPos.vx = v.vx + n->centerX;
    g_fieldBgi.actors[arg0].actor->prevPos.vy = v.vy + n->centerY;
    g_fieldBgi.actors[arg0].actor->prevPos.vz = v.vz + n->centerZ;
    return 1;
}

s32 bgi_updateActor(u32 arg0) {
    BgiActor *e;

    if (g_fieldBgi.loaded == 0) {
        return BGI_ERR_NO_MESH;
    }
    if (arg0 >= g_fieldBgi.actorCount) {
        return BGI_ERR_BAD_ACTOR;
    }
    e = &g_fieldBgi.actors[arg0];
    if (e->flags & BGI_FLAG_ACTIVE) {
        if (e->floor == -1 || e->triangle == -1) {
            return (bgi_placeActor(arg0, e->actor->pos.vx, e->actor->pos.vy, e->actor->pos.vz) != 0) * 2;
        }
        return bgi_moveActor(e);
    }
    return 0;
}

s16 bgi_findTriangleBelow(s16 arg0, s16 arg1, s16 arg2, s16 *arg3, BgiTriangle **arg4) {
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
    *arg3 = -1;
    *arg4 = NULL;
    n = g_fieldBgi.triangles;
    b = g_fieldBgi.header;
    found = 0;
    p.vx = arg0;
    p.vy = arg1;
    p.vz = arg2;
    i = 0;
    y0 = arg1;
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
                *arg3 = i;
                best = y;
                *arg4 = n;
            }
        }
    }
    return found;
}

s32 bgi_setFloorActive(s32 arg0, s32 arg1) {
    BgiHeader *b = g_fieldBgi.header;
    BgiFloor *t = &((BgiFloor *)((u8 *)b + b->floorOffset))[arg0];
    s32 *ids;
    s32 i;
    BgiTriangle *n;

    if (arg1 != 0) {
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

s16 bgi_getHeight(s16 arg0, s16 arg1) {
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
        if (g_gameState->mode == 1) {
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
    p.vx = arg0;
    p.vy = -0x5CD7;
    p.vz = arg1;
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

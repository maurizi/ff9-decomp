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
    BgiTriangle *nb;
    EvtObject *player;
    s16 id;
    u8 mask;
    u8 mode;

    id = tri->neighbors[edge];
    if (id == -1) {
        goto fail;
    }
    nb = &g_fieldBgi.triangles[id];
    if (!(nb->flags & BGI_FLAG_ACTIVE)) {
        goto fail;
    }
    if ((mask = (nb->flags >> 8) & g_fieldBgi.maskFilter) & (BGI_MASK_NO_PLAYER | BGI_MASK_NO_OTHERS)) {
        mode = g_gameState->mode;
        if (mode == GS_MODE_UNUSED_5) {
            player = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
        } else if (mode == GS_MODE_FIELD) {
            player = evt_getPlayer();
        } else {
            player = NULL;
        }
        if (actor->actor == player) {
            if (mask & BGI_MASK_NO_PLAYER) {
            fail:
                return NULL;
            }
            return nb;
        }
        if (mask & BGI_MASK_NO_OTHERS) {
            return NULL;
        }
    }
    return nb;
}

/* closest point to pt on segment segA-segB -> out; returns squared distance */
s32 bgi_closestPointOnSegment(SVECTOR *pt, SVECTOR *segA, SVECTOR *segB, SVECTOR *out, s16 *outInside) {
    SVECTOR closest;
    s32 ta;
    s32 tb;
    s32 sum; /* interpolation denominator, then the squared distance (separate locals don't match) */
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
    ax = segA->vx;
    bx = segB->vx;
    py = pt->vy;
    ay = segA->vy;
    by = segB->vy;
    pz = pt->vz;
    az = segA->vz;
    bz = segB->vz;
    ta = (px - ax) * (bx - ax);
    ta += (py - ay) * (by - ay);
    ta += (pz - az) * (bz - az);
    if (ta <= 0) {
        closest.vx = segA->vx;
        closest.vy = segA->vy;
        closest.vz = segA->vz;
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
            closest.vx = segB->vx;
            closest.vy = segB->vy;
            closest.vz = segB->vz;
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
            sum = ta + tb;
            closest.vx = segA->vx + (segB->vx - segA->vx) * ta / sum;
            closest.vy = segA->vy + (segB->vy - segA->vy) * ta / sum;
            closest.vz = segA->vz + (segB->vz - segA->vz) * ta / sum;
            *outInside = 1;
        }
    }
    dx = closest.vx - pt->vx;
    dy = closest.vy - pt->vy;
    dz = closest.vz - pt->vz;
    out->vx = closest.vx;
    out->vy = closest.vy;
    sum = dx * dx;
    sum += dy * dy;
    out->vz = closest.vz;
    return sum + dz * dz;
}

s32 bgi_projectOnPlane(VECTOR *normal, SVECTOR *dir, s32 unused, VECTOR *outAxis) {
    VECTOR perp;
    VECTOR vec;

    vec.vx = dir->vx << 16;
    vec.vy = dir->vy << 16;
    vec.vz = dir->vz << 16;
    perp.vy = 0;
    perp.vz = vec.vx;
    perp.vx = -vec.vz;
    outAxis->vx = -dir->vz;
    outAxis->vy = 0;
    outAxis->vz = dir->vx;
    if (normal->vy >= 0) {
        vec.vx = fld_fixMul(normal->vy, perp.vz);
        vec.vy = fld_fixMul(normal->vz, perp.vx) - fld_fixMul(normal->vx, perp.vz);
        vec.vz = -fld_fixMul(normal->vy, perp.vx);
    } else {
        vec.vx = -fld_fixMul(normal->vy, perp.vz);
        vec.vy = fld_fixMul(normal->vx, perp.vz) - fld_fixMul(normal->vz, perp.vx);
        vec.vz = fld_fixMul(normal->vy, perp.vx);
    }
    vec.vx = (vec.vx + 0x8000) & 0xFFFF0000;
    vec.vy = (vec.vy + 0x8000) & 0xFFFF0000;
    vec.vz = (vec.vz + 0x8000) & 0xFFFF0000;
    dir->vx = vec.vx >> 16;
    dir->vy = vec.vy >> 16;
    dir->vz = vec.vz >> 16;
    return 1;
}

/* push a position out of the walkmesh triangle edges (recurses into neighbours) */
s32 bgi_pushOutOfEdges(BgiActor *actor, SVECTOR *pos, BgiTriangle *tri, s16 *outEdge) {
    BgiVertex *edgeVerts[2];
    SVECTOR pt; /* floor origin offset, then the closest point on the current edge (separate locals don't match) */
    BgiVertex verts[3];
    VECTOR push;
    s16 on;
    BgiFloor *floorInfo;
    BgiTriangle *nb;
    s16 i;
    s32 hit;
    s32 dist;
    s16 pushIdx;
    BgiVertex *vt;

    floorInfo = &g_fieldBgi.floors[tri->floor];
    pt.vx = g_fieldBgi.header->curPos[0] + floorInfo->curPos[0];
    pt.vy = g_fieldBgi.header->curPos[1] + floorInfo->curPos[1];
    pt.vz = g_fieldBgi.header->curPos[2] + floorInfo->curPos[2];
    vt = g_fieldBgi.vertices;
    for (i = 0; i < (s32)ARRAY_COUNT(verts); i++) {
        verts[i].vx = pt.vx + vt[tri->vertices[i]].vx;
        verts[i].vy = pt.vy + vt[tri->vertices[i]].vy;
        verts[i].vz = pt.vz + vt[tri->vertices[i]].vz;
    }
    edgeVerts[0] = &verts[2];
    hit = 0;
    for (i = 0; i < (s32)ARRAY_COUNT(verts); i++) {
        edgeVerts[1] = &verts[i];
        dist = bgi_closestPointOnSegment(pos, (SVECTOR *)edgeVerts[0], (SVECTOR *)edgeVerts[1], &pt, &on);
        if ((u32)actor->radiusSq >= dist) {
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
                if (dist != 0) {
                    dist = (s16)SquareRoot0(dist);
                    if (pt.vx == pos->vx) {
                        push.vx = 0;
                    } else {
                        push.vx = (s16)((pos->vx - pt.vx) * actor->radius / dist);
                    }
                    if (pt.vz == pos->vz) {
                        push.vz = 0;
                    } else {
                        push.vz = (s16)((pos->vz - pt.vz) * actor->radius / dist);
                    }
                    pushIdx = g_fieldBgiPushCount;
                    if (pushIdx + 1 < 16) {
                        push.vx = pt.vx - pos->vx + push.vx;
                        push.vz = pt.vz - pos->vz + push.vz;
                        g_fieldBgiPushVecs[pushIdx][0] = push.vx;
                        g_fieldBgiPushCount++;
                        g_fieldBgiPushVecs[pushIdx][2] = push.vz;
                        g_fieldBgiPushOnEdge[pushIdx] = on;
                        g_fieldBgiPushDist[pushIdx] = dist;
                    }
                    hit = 1;
                    tri->flags |= BGI_TRI_VISITED;
                    *outEdge = i;
                }
            }
        }
        edgeVerts[0] = edgeVerts[1];
    }
    return hit ^ 1;
}

u16 bgi_findTriangleNear(BgiActor *actor, BgiTriangle **triInOut, s32 pos, s32 depth) {
    BgiTriangle *tris[2];
    u16 i;

    if (depth < 9) {
        tris[0] = *triInOut;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[tris[0]->floor], tris[0], (s16 *)pos)) {
            return 1;
        }
        for (i = 0; i < (s32)ARRAY_COUNT(tris[0]->neighbors); i++) {
            tris[1] = bgi_getPassableNeighbor(actor, tris[0], i);
            if (tris[1] != NULL && (u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[tris[1]->floor], tris[1], (s16 *)pos)) {
                actor->triangle = tris[0]->neighbors[i];
                if (actor->triangle >= 0) {
                    tris[1] = &g_fieldBgi.triangles[actor->triangle];
                    actor->floor = tris[1]->floor;
                    *triInOut = tris[1];
                    return 1;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            tris[1] = bgi_getPassableNeighbor(actor, tris[0], i);
            if (tris[1] != NULL && !(tris[1]->flags & BGI_TRI_VISITED)) {
                tris[1]->flags |= BGI_TRI_VISITED;
                if (bgi_findTriangleNear(actor, &tris[1], pos, depth + 1) == 1) {
                    *triInOut = tris[1];
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* slide a position along the walkmesh from `from` towards `to`, following neighbour triangles */
s32 bgi_slide(BgiActor *actor, BgiTriangle **triInOut, s16 *from, SVECTOR *to, s16 unusedFlag, s16 depth, s32 fracX, s32 fracZ) {
    SVECTOR pos;
    SVECTOR *pp;
    VECTOR axis;
    SVECTOR save;
    SVECTOR delta;
    SVECTOR slideVec;
    VECTOR scaled;
    s16 edge;
    BgiTriangle *cur;
    s16 prev;
    u8 done;
    s16 saved;
    s32 off;
    s16 axisLen;
    s16 len;
    s16 nb;
    BgiTriangle *nextTri;
    u8 crossed;

    cur = *triInOut;
    prev = -1;
    pos.vx = from[0];
    pos.vy = from[1];
    pos.vz = from[2];
    save.vx = to->vx;
    save.vy = to->vy;
    done = 0;
    pp = &pos;
    save.vz = to->vz;
    edge = -1;
    saved = actor->triangle;
    off = actor->triangle * sizeof(BgiTriangle);
    do {
        delta.vx = to->vx - pos.vx;
        delta.vy = to->vy - pp->vy;
        delta.vz = to->vz - pp->vz;
        if (cur->normal >= 0) {
            bgi_projectOnPlane((VECTOR *)&g_fieldBgi.normals[cur->normal], &delta, (s32)pp, &axis);
        } else {
            bgi_projectOnPlane(&g_fieldBgiDefaultNormal, &delta, (s32)pp, &axis);
        }
        to->vx = pp->vx + delta.vx;
        to->vy = pp->vy + delta.vy;
        to->vz = pp->vz + delta.vz;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)to)) {
            done = 1;
        } else if ((crossed = bgi_findEdgeCrossing(g_fieldBgi.header, &g_fieldBgi.floors[cur->floor], cur, (s16 *)to, from, pp, &edge, edge))) {
            if (bgi_getPassableNeighbor(actor, cur, edge) != NULL) {
                if (cur->normal >= 0) {
                    slideVec.vx = to->vx - pos.vx;
                    slideVec.vy = to->vy - pp->vy;
                    slideVec.vz = to->vz - pp->vz;
                    axisLen = SquareRoot0(axis.vx * axis.vx + axis.vz * axis.vz);
                    if (axisLen == 0) {
                        to->vy = pp->vy;
                    } else {
                        len = SquareRoot0(slideVec.vx * slideVec.vx + slideVec.vy * slideVec.vy + slideVec.vz * slideVec.vz);
                        scaled.vz = axis.vz * len / axisLen;
                        scaled.vx = axis.vx * len / axisLen;
                        scaled.vy = 0;
                        slideVec.vy = 0;
                        slideVec.vx = scaled.vz;
                        slideVec.vz = -scaled.vx;
                        to->vx = pos.vx + slideVec.vx;
                        to->vy = pp->vy + slideVec.vy;
                        to->vz = pp->vz + slideVec.vz;
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
                nextTri = &g_fieldBgi.triangles[nb];
                {
                    BgiEdge *ee = &g_fieldBgi.edges[cur->edges[edge]];
                    actor->floor = nextTri->floor;
                    edge = ee->clone;
                }
                cur = nextTri;
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
    s32 count;

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
        count = 0;
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
                    count++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
        }
        if (any) {
            if (count >= 2) {
                sum.vx /= count;
                sum.vz /= count;
            }
        } else {
            count = 0;
            for (i = 0; i < g_fieldBgiPushCount; i++) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (g_fieldBgiPushVecs[j][0] == g_fieldBgiPushVecs[i][0] && g_fieldBgiPushVecs[j][2] == g_fieldBgiPushVecs[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    sum.vx += g_fieldBgiPushVecs[i][0];
                    count++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
            if (count > 0) {
                sum.vx /= count;
                sum.vy /= count;
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
static inline s32 bgi_pushOutOfEdges_inline(BgiActor *actor, SVECTOR *pos, BgiTriangle *nd, s16 *edge) {
    u16 k;
    BgiTriangle *tri;
    s32 cnt;
    u16 mask = ~BGI_TRI_VISITED;

    tri = g_fieldBgi.triangles;
    cnt = g_fieldBgi.header->triangleCount;
    for (k = 0; k < cnt; k++) {
        tri->flags &= mask;
        tri++;
    }
    g_fieldBgiPushCount = 0;
    return bgi_pushOutOfEdges(actor, pos, nd, edge);
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
    s32 nxTerm;
    BgiFloor *floorInfo;

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
        if (!BGI_IS_STEEP(pl->ny)) {
            nxTerm = fld_fixMul(pl->nx, actor->actor->pos.vx << 16);
            actor->actor->pos.vy = fld_fixMul(node->d - nxTerm - fld_fixMul(pl->nz, actor->actor->pos.vz << 16), pl->invNy) >> 16;
            actor->actor->pos.vy += (s16)g_fieldBgi.floors[node->floor].curPos[1] - g_fieldBgi.floors[node->floor].orgPos[1];
        } else {
            actor->actor->pos.vy = cur.vy;
        }
    } else {
        floorInfo = g_fieldBgi.floors + node->floor;
        vt = g_fieldBgi.vertices + node->vertices[0];
        actor->actor->pos.vy = (s16)(vt->vy + (g_fieldBgi.header->curPos[1] + floorInfo->curPos[1]));
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
    BgiAnim *anims;

    g_fieldBgi.header = bgi;
    g_fieldBgi.vertices = (BgiVertex *)((u8 *)bgi + bgi->vertexOffset);
    g_fieldBgi.triangles = (BgiTriangle *)((u8 *)bgi + bgi->triangleOffset);
    g_fieldBgi.normals = (BgiNormal *)((u8 *)bgi + bgi->normalOffset);
    g_fieldBgi.edges = (u8 *)bgi + bgi->edgeOffset;
    g_fieldBgi.floors = (BgiFloor *)((u8 *)bgi + bgi->floorOffset);
    anims = (BgiAnim *)((u8 *)bgi + bgi->animOffset);
    g_fieldBgi.maskFilter = 0xFF;
    g_fieldBgi.actorCount = 9;
    g_fieldBgi.actors = g_fieldBgiActors;
    g_fieldBgi.anims = anims;
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
    EvtObject *player;
    s32 i;
    u8 mode;

    *outIndex = 0;
    mode = g_gameState->mode;
    if (mode == GS_MODE_UNUSED_5) {
        player = (EvtObject *)g_gameState->gameData->npcModels[g_gameState->gameData->ctx.field->playerObj].actor;
    } else if (mode == GS_MODE_FIELD) {
        player = evt_getPlayer();
    } else {
        player = NULL;
    }
    if (player != NULL) {
        for (i = 0; i < g_fieldBgi.actorCount; i++) {
            if (g_fieldBgi.actors[i].actor == player) {
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
    VECTOR vec;
    s16 edge;
    BgiTriangle *node;
    BgiHeader *bgi;
    BgiActor *actor;
    BgiNormal *pl;
    s16 ymin;
    u16 found;
    s16 bi;
    s16 bt;
    u16 i;
    u16 hit;
    s32 nxTerm;
    s32 cnt;
    BgiTriangle *clearTri;
    u16 j;
    u16 mask;
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
    node = g_fieldBgi.triangles;
    bgi = g_fieldBgi.header;
    actor = &g_fieldBgi.actors[actorId];
    for (; i < bgi->triangleCount; i++, node++) {
        hit = 0;
        if ((u8)bgi_pointInTriangleXZ(bgi, &g_fieldBgi.floors[node->floor], node, &pos)) {
            mask = ~BGI_TRI_VISITED;
            clearTri = g_fieldBgi.triangles;
            cnt = g_fieldBgi.header->triangleCount;
            for (j = hit; j < cnt; j++) {
                clearTri->flags &= mask;
                clearTri++;
            }
            g_fieldBgiPushCount = 0;
            if (bgi_pushOutOfEdges(actor, &pos, node, &edge) || bgi_applyPushes(actor, &pos, &node, &edge)) {
                hit = 1;
            }
            if (hit) {
                k = node->normal;
                if (k >= 0) {
                    vec.vx = pos.vx;
                    vec.vy = pos.vy;
                    vec.vz = pos.vz;
                    pl = &g_fieldBgi.normals[k];
                    nxTerm = fld_fixMul(pl->nx, vec.vx << 16);
                    vec.vy = fld_fixMul(node->d - nxTerm - fld_fixMul(pl->nz, vec.vz << 16), pl->invNy) >> 16;
                    vec.vy += (s16)g_fieldBgi.floors[node->floor].curPos[1] - g_fieldBgi.floors[node->floor].orgPos[1];
                    pos.vy = vec.vy;
                } else {
                    tri = g_fieldBgi.floors + node->floor;
                    vt = g_fieldBgi.vertices + node->vertices[0];
                    pos.vy = vt->vy + (g_fieldBgi.header->curPos[1] + tri->curPos[1]);
                }
                if (pos.vy >= y && pos.vy < ymin) {
                    found = 1;
                    ymin = pos.vy;
                    bi = i;
                    bt = node->floor;
                }
            }
        }
    }
    if (found) {
        actor->floor = bt;
        actor->triangle = bi;
        actor->actor->pos.vx = x;
        actor->actor->pos.vy = ymin;
        actor->actor->pos.vz = z;
        return 1;
    }
    return 0;
}

s32 bgi_setActorTriangle(s32 actorId, s16 triId) {
    BgiTriangle *tri;
    BgiFloor *floorInfo;
    SVECTOR origin;

    tri = &g_fieldBgi.triangles[(u16)triId];
    floorInfo = &g_fieldBgi.floors[tri->floor];
    origin.vx = g_fieldBgi.header->curPos[0] + floorInfo->curPos[0];
    origin.vy = g_fieldBgi.header->curPos[1] + floorInfo->curPos[1];
    origin.vz = g_fieldBgi.header->curPos[2] + floorInfo->curPos[2];
    g_fieldBgi.actors[actorId].floor = tri->floor;
    g_fieldBgi.actors[actorId].triangle = triId;
    g_fieldBgi.actors[actorId].actor->pos.vx = origin.vx + tri->centerX;
    g_fieldBgi.actors[actorId].actor->pos.vy = origin.vy + tri->centerY;
    g_fieldBgi.actors[actorId].actor->pos.vz = origin.vz + tri->centerZ;
    g_fieldBgi.actors[actorId].actor->prevPos.vx = origin.vx + tri->centerX;
    g_fieldBgi.actors[actorId].actor->prevPos.vy = origin.vy + tri->centerY;
    g_fieldBgi.actors[actorId].actor->prevPos.vz = origin.vz + tri->centerZ;
    return 1;
}

s32 bgi_setActorFloorTriangle(s32 actorId, u16 floorId, u16 triIdx) {
    BgiFloor *floorInfo;
    BgiTriangle *tri;
    s32 *ids;
    s32 id;
    SVECTOR origin;

    floorInfo = &g_fieldBgi.floors[floorId];
    ids = (s32 *)((u8 *)g_fieldBgi.header + floorInfo->triangleOffset);
    id = ids[triIdx];
    origin.vx = g_fieldBgi.header->curPos[0] + floorInfo->curPos[0];
    origin.vy = g_fieldBgi.header->curPos[1] + floorInfo->curPos[1];
    origin.vz = g_fieldBgi.header->curPos[2] + floorInfo->curPos[2];
    tri = &g_fieldBgi.triangles[id];
    g_fieldBgi.actors[actorId].floor = floorId;
    g_fieldBgi.actors[actorId].triangle = ids[triIdx];
    g_fieldBgi.actors[actorId].actor->pos.vx = origin.vx + tri->centerX;
    g_fieldBgi.actors[actorId].actor->pos.vy = origin.vy + tri->centerY;
    g_fieldBgi.actors[actorId].actor->pos.vz = origin.vz + tri->centerZ;
    g_fieldBgi.actors[actorId].actor->prevPos.vx = origin.vx + tri->centerX;
    g_fieldBgi.actors[actorId].actor->prevPos.vy = origin.vy + tri->centerY;
    g_fieldBgi.actors[actorId].actor->prevPos.vz = origin.vz + tri->centerZ;
    return 1;
}

s32 bgi_updateActor(u32 actorId) {
    BgiActor *actor;

    if (g_fieldBgi.loaded == 0) {
        return BGI_ERR_NO_MESH;
    }
    if (actorId >= g_fieldBgi.actorCount) {
        return BGI_ERR_BAD_ACTOR;
    }
    actor = &g_fieldBgi.actors[actorId];
    if (actor->flags & BGI_FLAG_ACTIVE) {
        if (actor->floor == -1 || actor->triangle == -1) {
            return (bgi_placeActor(actorId, actor->actor->pos.vx, actor->actor->pos.vy, actor->actor->pos.vz) != 0) * 2;
        }
        return bgi_moveActor(actor);
    }
    return 0;
}

s16 bgi_findTriangleBelow(s16 posX, s16 posY, s16 posZ, s16 *outTriId, BgiTriangle **outTri) {
    SVECTOR pos;
    VECTOR vec;
    s16 y0;
    s32 best;
    s16 found;
    s16 i;
    BgiTriangle *tri;
    BgiHeader *bgi;
    BgiNormal *pl;
    s32 nxTerm;
    BgiFloor *floorInfo;
    s16 k;
    s16 y;
    BgiVertex *vx;

    best = 0x7FFF;
    *outTriId = -1;
    *outTri = NULL;
    tri = g_fieldBgi.triangles;
    bgi = g_fieldBgi.header;
    found = 0;
    pos.vx = posX;
    pos.vy = posY;
    pos.vz = posZ;
    i = 0;
    y0 = posY;
    for (; i < bgi->triangleCount; tri++, i++) {
        if ((u8)bgi_pointInTriangleXZ(bgi, &g_fieldBgi.floors[tri->floor], tri, &pos)) {
            k = tri->normal;
            if (k >= 0) {
                vec.vx = pos.vx;
                vec.vy = pos.vy;
                vec.vz = pos.vz;
                pl = &g_fieldBgi.normals[k];
                nxTerm = fld_fixMul(pl->nx, vec.vx << 16);
                vec.vy = fld_fixMul(tri->d - nxTerm - fld_fixMul(pl->nz, vec.vz << 16), pl->invNy) >> 16;
                vec.vy += (s16)g_fieldBgi.floors[tri->floor].curPos[1] - g_fieldBgi.floors[tri->floor].orgPos[1];
                pos.vy = vec.vy;
            } else {
                floorInfo = &g_fieldBgi.floors[tri->floor];
                vx = &g_fieldBgi.vertices[tri->vertices[0]];
                pos.vy = vx->vy + (g_fieldBgi.header->curPos[1] + floorInfo->curPos[1]);
            }
            y = pos.vy;
            if (y >= y0 && y < best) {
                found = 1;
                *outTriId = i;
                best = y;
                *outTri = tri;
            }
        }
    }
    return found;
}

s32 bgi_setFloorActive(s32 floorId, s32 active) {
    BgiHeader *bgi = g_fieldBgi.header;
    BgiFloor *floorInfo = &((BgiFloor *)((u8 *)bgi + bgi->floorOffset))[floorId];
    s32 *ids;
    s32 i;
    BgiTriangle *tris;

    if (active != 0) {
        ids = (s32 *)((u8 *)bgi + floorInfo->triangleOffset);
        floorInfo->flags |= BGI_FLAG_ACTIVE;
        tris = g_fieldBgi.triangles;
        for (i = 0; i < floorInfo->triangleCount; i++) {
            tris[ids[i]].flags |= BGI_FLAG_ACTIVE;
        }
    } else {
        ids = (s32 *)((u8 *)bgi + floorInfo->triangleOffset);
        floorInfo->flags &= ~BGI_FLAG_ACTIVE;
        tris = g_fieldBgi.triangles;
        for (i = 0; i < floorInfo->triangleCount; i++) {
            tris[ids[i]].flags &= ~BGI_FLAG_ACTIVE;
        }
    }
    return 1;
}

s16 bgi_getHeight(s16 x, s16 z) {
    SVECTOR pos;
    VECTOR vec;
    u16 i;
    BgiTriangle *tri;
    BgiNormal *pl;
    s32 nxTerm;
    s16 k;
    u8 hit;
    GameData *gd;

    if (g_fieldBgi.header == NULL) {
        if (g_gameState->mode == GS_MODE_UNUSED_5) {
            gd = g_gameState->gameData;
            return ((EvtObject *)gd->npcModels[gd->ctx.field->playerObj].actor)->pos.vy;
        }
        if (g_gameState->mode == GS_MODE_FIELD) {
            return evt_getPlayer()->pos.vy;
        }
        return 0;
    }
    goto start;
plane:
    vec.vx = pos.vx;
    vec.vy = pos.vy;
    vec.vz = pos.vz;
    pl = &g_fieldBgi.normals[k];
    nxTerm = fld_fixMul(pl->nx, vec.vx << 16);
    vec.vy = fld_fixMul(tri->d - nxTerm - fld_fixMul(pl->nz, vec.vz << 16), pl->invNy) >> 16;
    vec.vy += (s16)g_fieldBgi.floors[tri->floor].curPos[1] - g_fieldBgi.floors[tri->floor].orgPos[1];
    pos.vy = vec.vy;
    return pos.vy;
start:
    pos.vx = x;
    pos.vy = -0x5CD7;
    pos.vz = z;
    tri = g_fieldBgi.triangles;
    for (i = 0; i < g_fieldBgi.header->triangleCount; i++, tri++) {
        if ((u8)bgi_pointInTriangleXZ(g_fieldBgi.header, &g_fieldBgi.floors[tri->floor], tri, &pos)) {
            k = tri->normal;
            if (k >= 0) {
                goto plane;
            }
            {
                pos.vy = g_fieldBgi.header->curPos[1];
                pos.vy += g_fieldBgi.floors[tri->floor].curPos[1];
                pos.vy += g_fieldBgi.vertices[tri->vertices[0]].vy;
                return pos.vy;
            }
        }
    }
    return -0x7FFF;
}

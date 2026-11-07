#include "ovl_0a.h"

s32 bgi_pointInTriangle(BgiHeader *bgi, BgiFloor *floor, BgiTriangle *tri, s16 *pos) {
    s8 tmp[2]; /* on-axis vertex counts, then the side flags of the crossing test (separate locals would change the frame) */
    s32 cross[2];
    BgiVertexArr *pv[2];
    s16 origin[3];
    BgiVertexArr corner[3];
    u8 i;
    s32 inside;
    u8 x;
    u8 z;
    BgiVertexArr *vtx;
    BgiNormal *pl;
    s32 cx;
    s32 cz;

    x = 0;
    z = 2;
    if (tri->normal >= 0) {
        pl = &g_fieldBgi.normals[tri->normal];
        if (BGI_IS_STEEP(pl->ny)) {
            cx = pl->nx;
            cz = pl->nz;
            if (ABS(cx) > ABS(cz)) {
                x = 1;
                z = 2;
            } else {
                x = 0;
                z = 1;
            }
        }
    }
    inside = 0;
    i = 0;
    vtx = (BgiVertexArr *)g_fieldBgi.vertices;
    origin[x] = bgi->curPos[x] + floor->curPos[x];
    origin[z] = bgi->curPos[z] + floor->curPos[z];
    tmp[0] = 0;
    tmp[1] = 0;
    for (; i < (s32)ARRAY_COUNT(corner); i++) {
        corner[i].xyz[x] = origin[x] + vtx[tri->vertices[i]].xyz[x];
        corner[i].xyz[z] = origin[z] + vtx[tri->vertices[i]].xyz[z];
        if (corner[i].xyz[x] == pos[x]) {
            tmp[0]++;
            if (corner[i].xyz[z] <= pos[z]) {
                tmp[0] = -tmp[0];
            }
        }
        if (corner[i].xyz[z] == pos[z]) {
            tmp[1]++;
            if (corner[i].xyz[x] <= pos[x]) {
                tmp[1] = -tmp[1];
            }
        }
    }
    if (tmp[0] == -2) {
        return 1;
    }
    if (tmp[1] == -2) {
        return 1;
    }
    pv[0] = &corner[2];
    tmp[0] = pos[z] >= pv[0]->xyz[z];
    for (i = 0; i < (s32)ARRAY_COUNT(corner); i++) {
        pv[1] = &corner[i];
        tmp[1] = pos[z] >= pv[1]->xyz[z];
        if (tmp[0] != tmp[1]) {
            cross[0] = (pv[1]->xyz[z] - pos[z]) * (pv[0]->xyz[x] - pv[1]->xyz[x]);
            cross[1] = (pv[1]->xyz[x] - pos[x]) * (pv[0]->xyz[z] - pv[1]->xyz[z]);
            if (cross[0] == cross[1]) {
                return 1;
            }
            if ((cross[1] < cross[0]) == tmp[1]) {
                inside ^= 1;
            }
        }
        pv[0] = pv[1];
        tmp[0] = tmp[1];
    }
    return inside;
}

s32 bgi_pointInTriangleXZ(BgiHeader *bgi, BgiFloor *floor, BgiTriangle *tri, SVECTOR *sv) {
    s16 *pos = (s16 *)sv;
    s8 tmp[2]; /* on-axis vertex counts, then the side flags of the crossing test (separate locals would change the frame) */
    s32 cross[2];
    BgiVertexArr *pv[2];
    s16 origin[3];
    BgiVertexArr corner[3];
    u8 i;
    u8 inside;
    u8 x;
    u8 z;
    BgiVertexArr *vtx;

    x = 0;
    z = 2;
    inside = 0;
    i = 0;
    vtx = (BgiVertexArr *)g_fieldBgi.vertices;
    origin[x] = bgi->curPos[x] + floor->curPos[x];
    origin[z] = bgi->curPos[z] + floor->curPos[z];
    tmp[0] = 0;
    tmp[1] = 0;
    for (; i < (s32)ARRAY_COUNT(corner); i++) {
        corner[i].xyz[x] = origin[x] + vtx[tri->vertices[i]].xyz[x];
        corner[i].xyz[z] = origin[z] + vtx[tri->vertices[i]].xyz[z];
        if (corner[i].xyz[x] == pos[x]) {
            tmp[0]++;
            if (corner[i].xyz[z] <= pos[z]) {
                tmp[0] = -tmp[0];
            }
        }
        if (corner[i].xyz[z] == pos[z]) {
            tmp[1]++;
            if (corner[i].xyz[x] <= pos[x]) {
                tmp[1] = -tmp[1];
            }
        }
    }
    if (tmp[0] == -2) {
        return 1;
    }
    if (tmp[1] == -2) {
        return 1;
    }
    pv[0] = &corner[2];
    tmp[0] = pos[z] >= pv[0]->xyz[z];
    for (i = 0; i < (s32)ARRAY_COUNT(corner); i++) {
        pv[1] = &corner[i];
        tmp[1] = pos[z] >= pv[1]->xyz[z];
        if (tmp[0] != tmp[1]) {
            cross[0] = (pv[1]->xyz[z] - pos[z]) * (pv[0]->xyz[x] - pv[1]->xyz[x]);
            cross[1] = (pv[1]->xyz[x] - pos[x]) * (pv[0]->xyz[z] - pv[1]->xyz[z]);
            if (cross[0] == cross[1]) {
                return 1;
            }
            if ((cross[1] < cross[0]) == tmp[1]) {
                inside = !inside;
            }
        }
        pv[0] = pv[1];
        tmp[0] = tmp[1];
    }
    return inside;
}

s32 bgi_findEdgeCrossing(BgiHeader *bgi, BgiFloor *floor, BgiTriangle *tri, s16 *to, s16 *from, SVECTOR *hitOut, s16 *edgeOut, s32 skipEdge) {
    s16 origin[3];
    BgiVertexArr *pv[3];
    BgiVertexArr corner[3];
    s16 hit[3][3];
    s16 idx[3];
    BgiVertexArr *vtx;
    BgiNormal *pl;
    s16 i;
    s16 cnt;
    s16 edge;
    u8 x;
    u8 y;
    u8 z;
    u8 best;
    s32 cx;
    s32 cz;
    s32 denom;
    s32 edgeNum;
    s32 moveNum;
    s32 ax;
    s32 distSq;
    s32 min;
    s32 ex;
    s32 ez;

    edge = skipEdge;
    x = 0;
    z = 2;
    y = 1;
    if (tri->normal >= 0) {
        pl = &g_fieldBgi.normals[tri->normal];
        if (BGI_IS_STEEP(pl->ny)) {
            cx = pl->nx;
            cz = pl->nz;
            if (ABS(cx) > ABS(cz)) {
                x = 1;
                z = 2;
                y = 0;
            } else {
                x = 0;
                z = 1;
                y = 2;
            }
        }
    }
    vtx = (BgiVertexArr *)((u8 *)bgi + bgi->vertexOffset);
    origin[0] = bgi->curPos[0] + floor->curPos[0];
    origin[1] = bgi->curPos[1] + floor->curPos[1];
    origin[2] = bgi->curPos[2] + floor->curPos[2];
    for (i = 0; i < (s32)ARRAY_COUNT(corner); i++) {
        corner[i].xyz[0] = origin[0] + vtx[tri->vertices[i]].xyz[0];
        corner[i].xyz[1] = origin[1] + vtx[tri->vertices[i]].xyz[1];
        corner[i].xyz[2] = origin[2] + vtx[tri->vertices[i]].xyz[2];
    }
    pv[0] = &corner[2];
    cnt = 0;
    for (i = 0; i < (s32)ARRAY_COUNT(corner); i++) {
        pv[1] = &corner[i];
        if (i != edge) {
            denom = (pv[1]->xyz[x] - pv[0]->xyz[x]) * (ez = to[z] - from[z]);
            ex = to[x] - from[x];
            edgeNum = (pv[0]->xyz[z] - from[z]) * ex;
            denom -= (pv[1]->xyz[z] - pv[0]->xyz[z]) * ex;
            edgeNum -= (pv[0]->xyz[x] - from[x]) * ez;
            if (ABS(edgeNum) <= ABS(denom)) {
                moveNum = (pv[0]->xyz[z] - from[z]) * (pv[1]->xyz[x] - pv[0]->xyz[x]);
                moveNum -= (pv[0]->xyz[x] - from[x]) * (pv[1]->xyz[z] - pv[0]->xyz[z]);
                if (ABS(moveNum) <= ABS(denom)) {
                    if ((edgeNum >= 0 && denom >= 0) || (edgeNum <= 0 && denom < 0)) {
                        if ((moveNum >= 0 && denom >= 0) || (moveNum <= 0 && denom < 0)) {
                            if (denom != 0) {
                                while (edgeNum > 0x80000) {
                                    edgeNum >>= 1;
                                    denom >>= 1;
                                }
                                hit[cnt][x] = pv[0]->xyz[x] + edgeNum * (pv[1]->xyz[x] - pv[0]->xyz[x]) / denom;
                                hit[cnt][z] = pv[0]->xyz[z] + edgeNum * (pv[1]->xyz[z] - pv[0]->xyz[z]) / denom;
                                ax = pv[1]->xyz[x] - pv[0]->xyz[x];
                                if (ax < 0) {
                                    ax = pv[0]->xyz[x] - pv[1]->xyz[x];
                                }
                                if ((pv[1]->xyz[z] - pv[0]->xyz[z] >= 0 ? pv[1]->xyz[z] - pv[0]->xyz[z] : pv[0]->xyz[z] - pv[1]->xyz[z]) < ax) {
                                    hit[cnt][y] = pv[1]->xyz[y] - (pv[1]->xyz[x] - hit[cnt][x]) * (pv[0]->xyz[y] - pv[1]->xyz[y]) / (pv[0]->xyz[x] - pv[1]->xyz[x]);
                                } else {
                                    hit[cnt][y] = pv[1]->xyz[y] - (pv[1]->xyz[z] - hit[cnt][z]) * (pv[0]->xyz[y] - pv[1]->xyz[y]) / (pv[0]->xyz[z] - pv[1]->xyz[z]);
                                }
                            }
                            idx[cnt] = i;
                            cnt++;
                        }
                    }
                }
            }
        }
        pv[0] = pv[1];
    }
    if (cnt == 1) {
        *edgeOut = idx[0];
        hitOut->vx = hit[0][0];
        hitOut->vy = hit[0][1];
        hitOut->vz = hit[0][2];
    } else if (cnt < 2) {
        *edgeOut = -1;
        return 0;
    } else switch (cnt) {
    case 2:
        switch (idx[0] + idx[1]) {
        case 1:
            pv[0] = &corner[1];
            pv[1] = &corner[2];
            pv[2] = &corner[0];
            break;
        case 2:
            pv[0] = &corner[1];
            pv[1] = &corner[0];
            pv[2] = &corner[2];
            break;
        case 3:
            pv[0] = &corner[2];
            pv[1] = &corner[0];
            pv[2] = &corner[1];
            break;
        }
        if (bgi_segmentsCross(pv[0]->xyz, to, pv[2]->xyz, pv[1]->xyz, x, z)) {
            *edgeOut = idx[0];
            hitOut->vx = hit[0][0];
            hitOut->vy = hit[0][1];
            hitOut->vz = hit[0][2];
        } else {
            *edgeOut = idx[1];
            hitOut->vx = hit[1][0];
            hitOut->vy = hit[1][1];
            hitOut->vz = hit[1][2];
        }
        break;
    case 3:
        best = 0;
        min = 0x7FFFFFFF;
        for (i = 0; i < (s32)ARRAY_COUNT(hit); i++) {
            distSq = hit[i][0] * hit[i][0] + hit[i][1] * hit[i][1] + hit[i][2] * hit[i][2];
            if (distSq < min) {
                best = i;
                min = distSq;
            }
        }
        *edgeOut = best;
        hitOut->vx = hit[best][0];
        hitOut->vy = hit[best][1];
        hitOut->vz = hit[best][2];
        break;
    default:
        *edgeOut = -1;
        return 0;
    }
    return 1;
}

/* debug: print a walkmesh */
void bgi_dumpText(BgiHeader *bgi) {
    BgiTriangle *tri;
    BgiEdgeView *edge;
    BgiAnim *anim;
    BgiKeyframe *frame;
    BgiFloor *floor;
    BgiVertex *vtx;
    BgiNormal *nrm;
    s32 *ids;
    s32 i;
    s32 j;
    s32 k;

    if (bgi == NULL) {
        return;
    }
    printf("BGI Text Dump\n");
    printf("=============\n");
    printf("Data Size\t\t : %d [+4 for file magic]\n", bgi->dataSize);
    printf("Original Position: (%6d,%6d,%6d)\n", bgi->orgX, bgi->orgY, bgi->orgZ);
    printf("Current Position : (%6d,%6d,%6d)\n", bgi->curPos[0], bgi->curPos[1], bgi->curPos[2]);
    printf("Minimum Values   : (%6d,%6d,%6d)\n", bgi->minX, bgi->minY, bgi->minZ);
    printf("Maximum Values   : (%6d,%6d,%6d)\n", bgi->maxX, bgi->maxY, bgi->maxZ);
    printf("Default Char Pos : (%6d,%6d,%6d)\n", bgi->charX, bgi->charY, bgi->charZ);
    printf("Active Floor\t : %d\n", bgi->activeFloor);
    printf("Active Triangle  : %d\n", bgi->activeTriangle);
    printf("Triangle Count   : %d\n", bgi->triangleCount);
    printf("Triangle Offset  : %d\n", bgi->triangleOffset);
    tri = (BgiTriangle *)((u8 *)bgi + bgi->triangleOffset);
    for (i = 0; i < bgi->triangleCount; tri++, i++) {
        printf("  %4d: Flags\t\t\t= 0x%04X\n", i, tri->flags);
        printf("\t\tData\t\t\t = %d\n", tri->data);
        printf("\t\tFloor Index \t = %d\n", tri->floor);
        printf("\t\tNormal Index\t = %d\n", tri->normal);
        printf("\t\tTheta X \t\t = %d\n", (s16)tri->thetaX);
        printf("\t\tTheta Z \t\t = %d\n", tri->thetaZ);
        printf("\t\tD\t\t\t\t = %ld\n", tri->d);
        printf("\t\tCenter  \t\t = (%6d,%6d,%6d)\n", tri->centerX, tri->centerY, tri->centerZ);
        printf("\t\tVertex Indices   = (%6d,%6d,%6d)\n", tri->vertices[0], tri->vertices[1], tri->vertices[2]);
        printf("\t\tEdge Indices\t = (%6d,%6d,%6d)\n", tri->edges[0], tri->edges[1], tri->edges[2]);
        printf("\t\tNeighbor Indices = (%6d,%6d,%6d)\n", tri->neighbors[0], tri->neighbors[1], tri->neighbors[2]);
    }
    printf("Edge Count  \t : %d\n", bgi->edgeCount);
    printf("Edge Offset \t : %d\n", bgi->edgeOffset);
    edge = (BgiEdgeView *)((u8 *)bgi + bgi->edgeOffset);
    for (i = 0; i < bgi->edgeCount; edge++, i++) {
        printf("  %4d: Flags\t   = 0x%04X\n", i, edge->flags);
        printf("\t\tEdge Clone  = %d\n", edge->clone);
    }
    printf("Keyframe Animation Count  : %d\n", bgi->animCount);
    printf("Keyframe Animation Offset : %d\n", bgi->animOffset);
    anim = (BgiAnim *)((u8 *)bgi + bgi->animOffset);
    for (i = 0; i < bgi->animCount; anim++, i++) {
        printf("  %4d: Flags\t\t  = 0x%04X\n", i, anim->flags);
        printf("\t\tFrame Count    = %d\n", anim->frameCount);
        printf("\t\tCurrent Frame  = %ld\n", anim->curFrame);
        printf("\t\tFrame Rate     = %d\n", anim->frameRate);
        printf("\t\tCounter \t   = %d\n", (u16)anim->counter);
        printf("\t\tFrame Offset   = %ld\n", anim->frameOffset);
        frame = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
        for (j = 0; j < anim->frameCount; frame++, j++) {
            printf("\t\t%4d: Frame Flags     = 0x%04X\n", j, frame->flags);
            printf("              Value           = %d\n", frame->value);
            printf("              Triangle Count  = %d\n", frame->triangleCount);
            printf("\t\t\t  Triangle Offset = %d\n", frame->triangleOffset);
            ids = (s32 *)((u8 *)bgi + frame->triangleOffset);
            for (k = 0; k < frame->triangleCount; ids++, k++) {
                printf("\t\t\t\t%4d: Triangle Ndx = %ld\n", j, *ids);
            }
        }
    }
    printf("Floor Count \t : %d\n", bgi->floorCount);
    printf("Floor Offset\t : %d\n", bgi->floorOffset);
    floor = (BgiFloor *)((u8 *)bgi + bgi->floorOffset);
    for (i = 0; i < bgi->floorCount; floor++, i++) {
        printf("  %4d: Flags\t\t\t\t= %d\n", i, floor->flags);
        printf("\t\tFloor Index \t\t  = %d\n", floor->floorIndex);
        printf("\t\tOriginal Position\t  = (%6d,%6d,%6d)\n", floor->orgPos[0], floor->orgPos[1], floor->orgPos[2]);
        printf("\t\tCurrent Position\t  = (%6d,%6d,%6d)\n", (s16)floor->curPos[0], (s16)floor->curPos[1], (s16)floor->curPos[2]);
        printf("\t\tMinumim Values  \t  = (%6d,%6d,%6d)\n", floor->min[0], floor->min[1], floor->min[2]);
        printf("\t\tMaximum Values  \t  = (%6d,%6d,%6d)\n", floor->max[0], floor->max[1], floor->max[2]);
        printf("\t\tTriangle Count  \t  = %d\n", floor->triangleCount);
        printf("\t\tTriangle Index Offset = %d\n", floor->triangleOffset);
        ids = (s32 *)((u8 *)bgi + floor->triangleOffset);
        for (j = 0; j < floor->triangleCount; ids++, j++) {
            printf("\t\t  %4d: Triangle Ndx = %ld\n", j, *ids);
        }
    }
    printf("Vertex Count\t : %d\n", bgi->vertexCount);
    printf("Vertex Offset\t : %d\n", bgi->vertexOffset);
    vtx = (BgiVertex *)((u8 *)bgi + bgi->vertexOffset);
    for (i = 0; i < bgi->vertexCount; vtx++, i++) {
        printf("  %4d: (%d,%d,%d)\n", i, vtx->vx, vtx->vy, vtx->vz);
    }
    printf("Normal Count\t : %d\n", bgi->normalCount);
    printf("Normal Offset\t : %d\n", bgi->normalOffset);
    nrm = (BgiNormal *)((u8 *)bgi + bgi->normalOffset);
    for (i = 0; i < bgi->normalCount; nrm++, i++) {
        printf("  %4d: (%lx,%lx,%lx)\n", i, nrm->nx, nrm->ny, nrm->nz);
        printf(s_bgiStrInvNy, nrm->invNy);
    }
}

s32 bgi_updateAnims(void) {
    BgiHeader *bgi;
    BgiAnim *anim;
    u16 cnt;
    BgiTriangle *nodes;
    u16 i;
    s16 old;
    BgiKeyframe *pts;
    BgiKeyframe *pt;
    u16 j;
    u16 k;
    s32 *ids;
    BgiTriangle *tri;
    s32 pos;

    s32 frameNo;
    u16 triFlags;
    u16 num;
    u16 triCnt;

    bgi = g_fieldBgi.header;
    anim = g_fieldBgi.anims;
    cnt = bgi->animCount;
    nodes = g_fieldBgi.triangles;
    for (i = 0; i < cnt; anim++, i++) {
        if (!(anim->flags & BGI_FLAG_ACTIVE)) {
            continue;
        }
        if (!(anim->flags & (BGI_ANIM_PLAYING | BGI_ANIM_LOOP))) {
            continue;
        }
        old = -1;
        pts = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
        num = anim->frameCount;
        for (j = 0; j < num; j++) {
            ids = (s32 *)((u8 *)bgi + pts[j].triangleOffset);
            triCnt = pts[j].triangleCount;
            for (k = 0; k < triCnt; k++, ids++) {
                triFlags = nodes[*ids].flags;
                if (triFlags & BGI_FLAG_ACTIVE) {
                    old = j;
                }
                nodes[*ids].flags = triFlags & ~BGI_FLAG_ACTIVE;
            }
        }
        pos = anim->curFrame;
        j = pos >> 8;
        ids = (s32 *)((u8 *)bgi + pts[j].triangleOffset);
        triCnt = pts[j].triangleCount;
        for (k = 0; k < triCnt; k++) {
            nodes[*ids++].flags |= BGI_FLAG_ACTIVE;
        }
        if (old != j) {
            pt = &pts[j];
            bgi_remapActors(bgi, anim, pt, old, j);
            bgi_linkFrameNeighbors(bgi, anim, pt);
        }
        if (pts[j].value < 0) {
            pts[j].value = 0;
            anim->counter = 0;
            anim->flags &= ~BGI_ANIM_PLAYING;
            return 1;
        }
        anim->counter++;
        if ((u16)anim->counter >= pts[j].value) {
            anim->counter = 0;
            anim->curFrame = pos + anim->frameRate;
            frameNo = anim->curFrame >> 8;
            if (frameNo >= anim->frameCount) {
                if (anim->flags & BGI_ANIM_BOUNCE) {
                    anim->curFrame = ((anim->frameCount - 1) << 8) | 0xFF;
                    bgi_setAnimRate(i, (s16)-anim->frameRate);
                } else {
                    anim->curFrame = 0;
                }
                anim->flags &= ~BGI_ANIM_PLAYING;
            } else if (frameNo < 0) {
                if (anim->flags & BGI_ANIM_BOUNCE) {
                    anim->curFrame = 0;
                    bgi_setAnimRate(i, (s16)-anim->frameRate);
                } else {
                    anim->curFrame = ((anim->frameCount - 1) << 8) | 0xFF;
                }
                anim->flags &= ~BGI_ANIM_PLAYING;
            }
        }
    }
    return 1;
}

s32 bgi_showAnimFrame(u32 animId, u32 frame) {
    BgiHeader *bgi;
    BgiAnim *anim;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    s16 old;
    BgiTriangle *tri;
    u16 triFlags;

    bgi = g_fieldBgi.header;
    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    old = -1;
    if (frame >= anim->frameCount) {
        return 0;
    }
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    for (i = 0; i < anim->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)bgi + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; ids++, j++) {
            tri = &g_fieldBgi.triangles[*ids];
            if (tri->flags & BGI_FLAG_ACTIVE) {
                old = i;
                tri->flags &= ~BGI_FLAG_ACTIVE;
            } else {
                tri->flags &= ~BGI_FLAG_ACTIVE;
            }
        }
    }
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    pt += frame;
    ids = (s32 *)((u8 *)bgi + pt->triangleOffset);
    for (i = 0; i < pt->triangleCount; ids++, i++) {
        tri = &g_fieldBgi.triangles[*ids];
        tri->flags |= BGI_FLAG_ACTIVE;
    }
    if (frame != old) {
        bgi_remapActors(bgi, anim, pt, old, frame & 0xFFFF);
        bgi_linkFrameNeighbors(bgi, anim, pt);
    }
    anim->curFrame = frame << 8;
    return 1;
}

/* walkmesh triangle transfer: express the actor position relative to triangle n0 by cross products (nv, over
   the triangle's doubled area) and re-project it onto triangle n1; the height comes from n1's plane or, without
   one, from n1's first vertex */
s32 bgi_transferActorPos(BgiHeader *bgi, BgiActor *act, BgiTriangle *n0, BgiTriangle *n1) {
    SVECTOR origin;
    SVECTOR corner[3];
    SVECTOR relPos;
    VECTOR nv;
    VECTOR newPos;
    BgiFloor *floor;
    BgiVertex *vtx;
    BgiNormal *pl;
    s32 dx0, dx1, dx2, dz1, dz2;
    s32 dz0;
    s32 area;
    s32 k;
    s32 xTerm;

    floor = &g_fieldBgi.floors[n0->floor];
    origin.vx = bgi->curPos[0] + floor->curPos[0];
    origin.vy = bgi->curPos[1] + floor->curPos[1];
    origin.vz = bgi->curPos[2] + floor->curPos[2];
    vtx = (BgiVertex *)((u8 *)bgi + bgi->vertexOffset);
    corner[0].vx = vtx[n0->vertices[0]].vx;
    corner[1].vx = vtx[n0->vertices[1]].vx;
    corner[2].vx = vtx[n0->vertices[2]].vx;
    relPos.vx = act->actor->pos.vx - origin.vx;
    corner[0].vy = vtx[n0->vertices[0]].vy;
    corner[1].vy = vtx[n0->vertices[1]].vy;
    corner[2].vy = vtx[n0->vertices[2]].vy;
    relPos.vy = act->actor->pos.vy - origin.vy;
    corner[0].vz = vtx[n0->vertices[0]].vz;
    corner[1].vz = vtx[n0->vertices[1]].vz;
    corner[2].vz = vtx[n0->vertices[2]].vz;
    relPos.vz = act->actor->pos.vz - origin.vz;
    dx0 = corner[0].vx - relPos.vx;
    dx1 = corner[1].vx - relPos.vx;
    area = (corner[1].vx - corner[0].vx) * (corner[2].vz - corner[0].vz);
    area -= (corner[2].vx - corner[0].vx) * (corner[1].vz - corner[0].vz);
    dx2 = corner[2].vx - relPos.vx;
    dz0 = corner[0].vz - relPos.vz;
    dz1 = corner[1].vz - relPos.vz;
    dz2 = corner[2].vz - relPos.vz;
    nv.vx = dx1 * dz2 - dx2 * dz1;
    nv.vy = dx2 * dz0 - dx0 * dz2;
    nv.vz = dx0 * dz1 - dx1 * dz0;
    floor = &g_fieldBgi.floors[n1->floor];
    origin.vx = bgi->curPos[0] + floor->curPos[0];
    origin.vy = bgi->curPos[1] + floor->curPos[1];
    origin.vz = bgi->curPos[2] + floor->curPos[2];
    corner[0].vx = vtx[n1->vertices[0]].vx;
    corner[1].vx = vtx[n1->vertices[1]].vx;
    corner[2].vx = vtx[n1->vertices[2]].vx;
    corner[0].vy = vtx[n1->vertices[0]].vy;
    corner[1].vy = vtx[n1->vertices[1]].vy;
    corner[2].vy = vtx[n1->vertices[2]].vy;
    corner[0].vz = vtx[n1->vertices[0]].vz;
    corner[1].vz = vtx[n1->vertices[1]].vz;
    corner[2].vz = vtx[n1->vertices[2]].vz;
    newPos.vx = (nv.vx * corner[0].vx + nv.vy * corner[1].vx + nv.vz * corner[2].vx) / area;
    newPos.vz = (nv.vx * corner[0].vz + nv.vy * corner[1].vz + nv.vz * corner[2].vz) / area;
    act->actor->pos.vx = newPos.vx + origin.vx;
    act->actor->pos.vz = newPos.vz + origin.vz;
    k = n1->normal;
    if (k >= 0) {
        pl = &g_fieldBgi.normals[k];
        xTerm = fld_fixMul(pl->nx, act->actor->pos.vx << 16);
        act->actor->pos.vy = fld_fixMul(n1->d - xTerm - fld_fixMul(pl->nz, act->actor->pos.vz << 16), pl->invNy) >> 16;
        act->actor->pos.vy += (s16)g_fieldBgi.floors[n1->floor].curPos[1] - g_fieldBgi.floors[n1->floor].orgPos[1];
    } else {
        BgiFloor *t3;
        BgiVertex *vt;
        t3 = g_fieldBgi.floors + n1->floor;
        vt = g_fieldBgi.vertices + n1->vertices[0];
        act->actor->pos.vy = (s16)(vt->vy + (g_fieldBgi.header->curPos[1] + t3->curPos[1]));
    }
    return 1;
}

s32 bgi_remapActors(u8 *bgi, BgiAnim *anim, BgiKeyframe *frame, s16 oldFrame, s32 unused) { /* the callers pass a 5th argument */
    BgiKeyframe *oldKf;
    s32 *tbl;
    s32 *ids;
    BgiActor *act;
    u16 i;
    u16 j;

    if (oldFrame == -1) {
        return 0;
    }
    oldKf = (BgiKeyframe *)(bgi + anim->frameOffset);
    oldKf += oldFrame;
    tbl = (s32 *)(bgi + frame->triangleOffset);
    act = g_fieldBgi.actors;
    for (i = 0; i < g_fieldBgi.actorCount; act++, i++) {
        if (act->flags & BGI_FLAG_ACTIVE) {
            ids = (s32 *)(bgi + oldKf->triangleOffset);
            for (j = 0; j < oldKf->triangleCount; j++, ids++) {
                if (act->triangle == *ids) {
                    bgi_transferActorPos(bgi, act, &g_fieldBgi.triangles[act->triangle], &g_fieldBgi.triangles[tbl[j]]);
                    act->triangle = tbl[j];
                    act->floor = g_fieldBgi.triangles[tbl[j]].floor;
                }
            }
        }
    }
    return 1;
}

/* link the neighbour edges between the nodes of different triangles */
s32 bgi_linkFrameNeighbors(BgiHeader *bgi, BgiAnim *anim, BgiKeyframe *frame) {
    s32 *ids = (s32 *)((u8 *)bgi + frame->triangleOffset);
    BgiTriangle *tri;
    BgiTriangle *nbTri;
    BgiEdge *edge;
    s16 nb;
    u16 i;
    u16 j;
    u16 k;

    for (i = 0; i < frame->triangleCount; i++, ids++) {
        tri = &g_fieldBgi.triangles[*ids];
        for (j = 0; j < (s32)ARRAY_COUNT(tri->neighbors); j++) {
            nb = tri->neighbors[j];
            if (nb >= 0) {
                nbTri = &g_fieldBgi.triangles[nb];
                if (tri->floor != nbTri->floor) {
                    edge = &g_fieldBgi.edges[tri->edges[j]];
                    if (edge->clone >= 0) {
                        k = edge->clone;
                        nbTri->neighbors[k] = *ids;
                        edge = &g_fieldBgi.edges[nbTri->edges[k]];
                        edge->clone = j;
                    }
                }
            }
        }
    }
    return 1;
}

s32 bgi_resetFloorMoves(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        g_fieldBgiFloorMoves[i].flags = 2;
        g_fieldBgiFloorMoves[i].floor = 0;
        g_fieldBgiFloorMoves[i].axis = 0;
        g_fieldBgiFloorMoves[i].curve = 0;
        g_fieldBgiFloorMoves[i].range = 0;
        g_fieldBgiFloorMoves[i].accum = 0;
        g_fieldBgiFloorMoves[i].speed = 0x100;
        g_fieldBgiFloorMoves[i].min = 0;
        g_fieldBgiFloorMoves[i].max = 0;
        g_fieldBgiFloorMoves[i].cur = 0;
        g_fieldBgiFloorMoves[i].prev = 0;
    }
    return 1;
}

/* old-style definition: bgi_startFloorMove/bgi_setFloorMovePlaying call it without arguments */
s32 bgi_stepFloorMove(moveId)
s32 moveId;
{
    BgiFloorMove *move;
    s32 step;
    s16 range;
    s16 pos;

    move = &g_fieldBgiFloorMoves[moveId];
    move->accum += move->speed;
    step = move->accum >> 8;
    range = move->max - move->min;
    if (move->range != 0 && move->speed != 0 && range > 0) {
        switch (move->curve) {
        case 0:
            pos = move->min + step;
            break;
        case 1:
            pos = move->min + range * ((s16)math_cos((s16)((step << 12) / range) + 0x800) + ONE) / 0x2000;
            break;
        default:
            pos = move->min + step;
            break;
        }
        move->prev = move->cur;
        move->cur = pos;
        if (move->max < pos) {
            if (move->flags & BGI_ANIM_BOUNCE) {
                move->cur = move->max;
                move->speed = -move->speed;
            } else {
                move->cur = move->min;
            }
            move->accum = (range << 8) - 0x100;
            move->flags &= ~BGI_ANIM_PLAYING;
        } else if (pos < move->min) {
            if (move->flags & BGI_ANIM_BOUNCE) {
                move->cur = move->min;
                move->speed = -move->speed;
            } else {
                move->cur = move->max;
            }
            move->accum = 0;
            move->flags &= ~BGI_ANIM_PLAYING;
        }
    } else {
        return 0;
    }
    return 1;
}

s32 bgi_updateFloorMoves(void) {
    BgiFloorMove *move;
    BgiActor *act;
    BgiFloor *floor;
    s32 i;
    s32 j;
    EvtObject *obj;
    u16 flags;

    move = g_fieldBgiFloorMoves;
    for (i = 0; i < 30; move++, i++) {
        if (!(move->flags & BGI_FLAG_ACTIVE) || !(move->flags & (BGI_ANIM_PLAYING | BGI_ANIM_LOOP))) {
            continue;
        }
        floor = &g_fieldBgi.floors[move->floor];
        floor->curPos[move->axis] = floor->orgPos[move->axis] + move->cur;
        act = g_fieldBgi.actors;
        for (j = 0; j < g_fieldBgi.actorCount; act++, j++) {
            flags = act->flags;
            if ((flags & BGI_FLAG_ACTIVE) && act->floor == move->floor && act->triangle >= 0) {
                obj = act->actor;
                ((ActorPosView *)obj)->pos[move->axis] += move->cur - move->prev;
                switch (move->axis) {
                case 0:
                    act->actor->prevPos.vx = act->actor->pos.vx;
                    break;
                case 1:
                    act->actor->prevPos.vy = act->actor->pos.vy;
                    break;
                case 2:
                    act->actor->prevPos.vz = act->actor->pos.vz;
                    break;
                }
            }
        }
        bgi_stepFloorMove(i);
    }
    return 1;
}

s32 bgi_setMesh(BgiHeader *bgi) {
    s32 i;

    g_fieldBgi.header = bgi;
    g_fieldBgi.vertices = (BgiVertex *)((u8 *)bgi + bgi->vertexOffset);
    g_fieldBgi.triangles = (BgiTriangle *)((u8 *)bgi + bgi->triangleOffset);
    g_fieldBgi.normals = (BgiNormal *)((u8 *)bgi + bgi->normalOffset);
    g_fieldBgi.edges = (u8 *)bgi + bgi->edgeOffset;
    g_fieldBgi.floors = (BgiFloor *)((u8 *)bgi + bgi->floorOffset);
    g_fieldBgi.anims = (BgiAnim *)((u8 *)bgi + bgi->animOffset);
    for (i = 0; i < g_fieldBgi.header->animCount; i++) {
        bgi_initAnim(i);
    }
    bgi_resetFloorMoves();
    return 1;
}

s32 bgi_updateActors(void) {
    s32 moved = 0;
    s16 i = 0;

    if (g_fieldBgi.actorCount != 0) {
        do {
            if (bgi_updateActor(i) > 0) {
                moved++;
            }
            i++;
        } while (i < g_fieldBgi.actorCount);
    }
    return moved & 0xFFFF;
}

s32 bgi_clear(void) {
    g_fieldBgi.header = 0;
    g_fieldBgi.actorCount = 0;
    g_fieldBgi.actors = 0;
    g_fieldBgi.loaded = 0;
    return 1;
}

s32 bgi_nopOk(void) {
    return 1;
}

void bgi_getOne(s32 *out) {
    *out = 1;
}

s32 bgi_setMaskFilter(u8 filter) {
    g_fieldBgi.maskFilter = filter;
    return 1;
}

s32 bgi_getMaskFilter(u8 *outFilter) {
    *outFilter = g_fieldBgi.maskFilter;
    return 1;
}

BgiActor *bgi_getActor(s32 actorId) {
    return &g_fieldBgi.actors[actorId];
}

s32 bgi_bindActor(s32 actorId, FieldObjSlot *slot) {
    g_fieldBgi.actors[actorId].flags = 0;
    g_fieldBgi.actors[actorId].unk2 = 0;
    g_fieldBgi.actors[actorId].actor = slot->actor;
    return 1;
}

s32 bgi_addActor(FieldObjSlot *slot) {
    g_fieldBgi.actors[g_fieldBgi.actorCount].flags = 0;
    g_fieldBgi.actors[g_fieldBgi.actorCount].unk2 = 0;
    g_fieldBgi.actors[g_fieldBgi.actorCount].actor = slot->actor;
    g_fieldBgi.actorCount++;
    return 1;
}

s32 bgi_setActorRadius(s32 actorId, u16 radius) {
    g_fieldBgi.actors[actorId].radius = radius;
    g_fieldBgi.actors[actorId].radiusSq = radius * radius;
    return 1;
}

s32 bgi_getActorRadius(s32 actorId, s32 *outRadius) {
    *outRadius = g_fieldBgi.actors[actorId].radius;
    return 1;
}

s32 bgi_getActorRadiusSq(s32 actorId, s32 *outRadiusSq) {
    *outRadiusSq = g_fieldBgi.actors[actorId].radiusSq;
    return 1;
}

s32 bgi_getActorNormal(s32 actorId, s32 *outNx, s32 *outNy, s32 *outNz, s32 *outInvNy) {
    BgiNormal *nrm;
    BgiTriangle *tri;
    struct BgiActor *act;

    act = g_fieldBgi.actors + actorId;
    tri = g_fieldBgi.triangles + act->triangle;
    nrm = g_fieldBgi.normals + tri->normal;
    *outNx = nrm->nx;
    *outNy = nrm->ny;
    *outNz = nrm->nz;
    *outInvNy = nrm->invNy;
    return 1;
}

s32 bgi_getActorTilt(s32 actorId, s16 *outThetaZ, u16 *outThetaX) {
    BgiActor *act = &g_fieldBgi.actors[actorId];
    BgiTriangle *tri = &g_fieldBgi.triangles[act->triangle];

    *outThetaZ = -tri->thetaZ;
    *outThetaX = tri->thetaX;
    return 1;
}

s32 bgi_getActorTriangle(s32 actorId, u16 *outTriangle, u16 *outFloor) {
    BgiActor *act = &g_fieldBgi.actors[actorId];

    *outTriangle = act->triangle;
    *outFloor = act->floor;
    return 1;
}

s32 bgi_revertActor(s32 actorId) {
    BgiActor *act = &g_fieldBgi.actors[actorId];

    act->actor->pos.vx = act->actor->prevPos.vx;
    act->actor->pos.vy = act->actor->prevPos.vy;
    act->actor->pos.vz = act->actor->prevPos.vz;
    act->floor = act->lastFloor;
    act->triangle = act->lastTriangle;
    return 1;
}

s32 bgi_setTriangleActive(s32 triId, s32 active) {
    BgiTriangle *tri = bgi_trianglePtr(triId);
    u16 flags;

    if (active != 0) {
        flags = tri->flags | BGI_FLAG_ACTIVE;
    } else {
        flags = tri->flags & ~BGI_FLAG_ACTIVE;
    }
    tri->flags = flags;
    return 1;
}

s32 bgi_getMoveTris(u16 *outCount, s16 **outTris) {
    *outCount = g_fieldBgiMoveTriCount;
    *outTris = g_fieldBgiMoveTris;
    return 1;
}

s32 bgi_getTriangleFlags(s16 triId, u16 *outFlags) {
    *outFlags = bgi_trianglePtr(triId)->flags & ((g_fieldBgi.maskFilter << 8) | 0xFF);
    return 1;
}

s32 bgi_setTriangleMask(s16 triId, s32 triMask) {
    BgiTriangle *tri = bgi_trianglePtr(triId);

    tri->flags = ((triMask & 0xFF) << 8) | (tri->flags & 0xFF);
    return 1;
}

s32 bgi_getTriangleMask(s16 triId, u8 *outMask) {
    *outMask = (bgi_trianglePtr(triId)->flags >> 8) & g_fieldBgi.maskFilter;
    return 1;
}

s32 bgi_getTriangleFloor(s16 triId, u16 *outFloor) {
    *outFloor = bgi_trianglePtr(triId)->floor;
    return 1;
}

s32 bgi_getMoveEdges(u16 *outCount, s16 **outEdges) {
    *outCount = g_fieldBgiMoveEdgeCount;
    *outEdges = g_fieldBgiMoveEdges;
    return 1;
}

s32 bgi_getEdgeFlags(s16 edgeId, u16 *outFlags) {
    *outFlags = bgi_edgePtr(edgeId)->flags;
    return 1;
}

s32 bgi_getFloorPosition(s32 floorId, u16 *outX, u16 *outY, u16 *outZ) {
    BgiFloor *floor = bgi_floorPtr(floorId);

    *outX = floor->curPos[0];
    *outY = floor->curPos[1];
    *outZ = floor->curPos[2];
    return 1;
}

s32 bgi_initAnim(u32 animId) {
    BgiHeader *bgi;
    BgiAnim *anim;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    BgiTriangle *tri;

    bgi = g_fieldBgi.header;
    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    for (i = 0; i < anim->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)bgi + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; j++) {
            tri = &g_fieldBgi.triangles[*ids];
            if (i == 0) {
                tri->flags |= BGI_FLAG_ACTIVE;
                bgi_linkFrameNeighbors(bgi, anim, pt);
            } else {
                tri->flags &= ~BGI_FLAG_ACTIVE;
            }
            ids++;
        }
    }
    return 1;
}

s32 bgi_setAnimFrame(u32 animId, u32 frame) {
    BgiAnim *anim;

    if (animId >= g_fieldBgi.header->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    if (frame >= anim->frameCount) {
        return 0;
    }
    anim->curFrame = frame << 8;
    anim->counter = 0;
    anim->flags |= BGI_FLAG_ACTIVE;
    return 1;
}

s32 bgi_setAnimPlaying(u32 animId, s32 playing) {
    BgiAnim *anim;

    if (animId >= g_fieldBgi.header->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    if (playing != 0) {
        anim->flags |= BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING;
    } else {
        anim->flags &= ~(BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING);
    }
    return 1;
}

s32 bgi_setAnimRate(u32 animId, s16 rate) {
    BgiAnim *anim;

    if (animId >= g_fieldBgi.header->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    anim->frameRate = rate;
    return 1;
}

s32 bgi_setAnimFrameValues(u32 animId, s16 value) {
    BgiHeader *bgi = g_fieldBgi.header;
    BgiAnim *anim;
    BgiKeyframe *pt;
    s32 i;

    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    for (i = 0; i < anim->frameCount; pt++, i++) {
        pt->value = value;
    }
    return 1;
}

s32 bgi_setAnimFrameValue(u32 animId, u32 frame, s16 value) {
    BgiHeader *bgi = g_fieldBgi.header;
    BgiAnim *anim;
    BgiKeyframe *pt;

    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    if (frame >= anim->frameCount) {
        return 0;
    }
    pt = &((BgiKeyframe *)((u8 *)bgi + anim->frameOffset))[frame];
    pt->value = value;
    return 1;
}

s32 bgi_setAnimMode(u32 animId, s32 mode) {
    BgiAnim *anim;

    if (animId >= g_fieldBgi.header->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    anim->flags = (anim->flags & ~BGI_ANIM_MODE) | (mode & BGI_ANIM_MODE);
    return 1;
}

s32 bgi_playAnimRange(u32 animId, u32 first, u32 last) {
    BgiHeader *bgi = g_fieldBgi.header;
    BgiAnim *anim;
    BgiKeyframe *pt;

    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    if (first >= anim->frameCount) {
        return 0;
    }
    if (last >= anim->frameCount) {
        return 0;
    }
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    anim->curFrame = first << 8;
    anim->counter = 0;
    anim->flags |= BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING;
    pt += last;
    pt->value = -1;
    if ((anim->frameRate > 0 && last < first) || (anim->frameRate < 0 && first < last)) {
        anim->frameRate = -anim->frameRate;
    }
    return 1;
}

s32 bgi_resetAnim(u32 animId, s32 activateFirst) {
    BgiHeader *bgi = g_fieldBgi.header;
    BgiAnim *anim;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    BgiTriangle *tri;

    if (animId >= bgi->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    anim->frameRate = 0x100;
    anim->flags = 2;
    anim->curFrame = 0;
    anim->counter = 0;
    pt = (BgiKeyframe *)((u8 *)bgi + anim->frameOffset);
    for (i = 0; i < anim->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)bgi + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; ids++, j++) {
            tri = &g_fieldBgi.triangles[*ids];
            if (activateFirst != 0 && i == 0) {
                tri->flags |= BGI_FLAG_ACTIVE;
            } else {
                tri->flags &= ~BGI_FLAG_ACTIVE;
            }
        }
    }
    return 1;
}

s32 bgi_getAnimFrame(u32 animId, s32 *outFrame) {
    BgiAnim *anim;

    if (animId >= g_fieldBgi.header->animCount) {
        return 0;
    }
    anim = &g_fieldBgi.anims[animId];
    *outFrame = anim->curFrame >> 8;
    return 1;
}

s32 bgi_startFloorMove(s32 moveId, s32 pos) {
    BgiFloorMove *move = &g_fieldBgiFloorMoves[moveId];

    move->accum = pos << 8;
    move->flags |= BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING;
    bgi_stepFloorMove(moveId);
    return 1;
}

s32 bgi_stopFloorMove(s32 moveId, s32 pos) {
    BgiFloorMove *move = bgi_getFloorMove_inline(moveId);

    pos <<= 8;
    move->accum = pos;
    move->flags = 0;
    bgi_stepFloorMove(moveId);
    return 1;
}

s32 bgi_setFloorMovePlaying(s32 moveId, s32 playing) {
    BgiFloorMove *move = bgi_getFloorMove_inline(moveId);
    u16 flags;

    if (playing != 0) {
        flags = move->flags | BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING;
    } else {
        flags = move->flags & ~(BGI_FLAG_ACTIVE | BGI_ANIM_PLAYING);
    }
    move->flags = flags;
    bgi_stepFloorMove(moveId);
    return 1;
}

s32 bgi_setFloorMoveMode(s32 moveId, s32 mode) {
    BgiFloorMove *move = bgi_getFloorMove_inline(moveId);

    move->flags = (move->flags & ~BGI_ANIM_MODE) | (mode & BGI_ANIM_MODE);
    return 1;
}

s32 bgi_setFloorMoveFloor(s32 moveId, s16 floor) {
    bgi_getFloorMove_inline(moveId)->floor = floor;
    return 1;
}

s32 bgi_setFloorMoveSpeed(s32 moveId, s16 speed) {
    bgi_getFloorMove_inline(moveId)->speed = speed;
    return 1;
}

s32 bgi_setFloorMoveCurve(s32 moveId, s16 curve) {
    bgi_getFloorMove_inline(moveId)->curve = curve;
    return 1;
}

s32 bgi_setFloorMoveRange(s32 moveId, s16 min, s16 max) {
    BgiFloorMove *move = bgi_getFloorMove_inline(moveId);

    move->min = min;
    move->max = max;
    move->range = move->max - move->min;
    return 1;
}

s32 bgi_setFloorMoveAxis(s32 moveId, s16 axis) {
    bgi_getFloorMove_inline(moveId)->axis = axis;
    return 1;
}

BgiFloorMove *bgi_getFloorMove(s32 moveId) {
    return bgi_getFloorMove_inline(moveId);
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/bgi", s_bgiStrInvNy);

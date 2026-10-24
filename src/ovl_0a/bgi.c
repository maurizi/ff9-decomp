#include "ovl_0a.h"

s32 bgi_pointInTriangle(BgiHeader *b, BgiFloor *t, BgiTriangle *n, s16 *p) {
    s8 c[2];
    s32 d[2];
    BgiVertexArr *pv[2];
    s16 o[3];
    BgiVertexArr v[3];
    u8 i;
    s32 r;
    u8 x;
    u8 z;
    BgiVertexArr *vtx;
    BgiNormal *pl;
    s32 a;
    s32 cz;

    x = 0;
    z = 2;
    if (n->normal >= 0) {
        pl = &g_fieldBgi.normals[n->normal];
        if ((u32)(pl->ny + 0x50F4) <= 0xA1E8) {
            a = pl->nx;
            cz = pl->nz;
            if (ABS(a) > ABS(cz)) {
                x = 1;
                z = 2;
            } else {
                x = 0;
                z = 1;
            }
        }
    }
    r = 0;
    i = 0;
    vtx = (BgiVertexArr *)g_fieldBgi.vertices;
    o[x] = b->curPos[x] + t->curPos[x];
    o[z] = b->curPos[z] + t->curPos[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->vertices[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->vertices[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r ^= 1;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

s32 bgi_pointInTriangleXZ(BgiHeader *b, BgiFloor *t, BgiTriangle *n, SVECTOR *sv) {
    s16 *p = (s16 *)sv;
    s8 c[2];
    s32 d[2];
    BgiVertexArr *pv[2];
    s16 o[3];
    BgiVertexArr v[3];
    u8 i;
    u8 r;
    u8 x;
    u8 z;
    BgiVertexArr *vtx;

    x = 0;
    z = 2;
    r = 0;
    i = 0;
    vtx = (BgiVertexArr *)g_fieldBgi.vertices;
    o[x] = b->curPos[x] + t->curPos[x];
    o[z] = b->curPos[z] + t->curPos[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->vertices[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->vertices[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r = !r;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

s32 bgi_findEdgeCrossing(BgiHeader *b, BgiFloor *t, BgiTriangle *n, s16 *arg3, s16 *arg4, SVECTOR *arg5, s16 *arg6, s32 arg7) {
    s16 o[3];
    BgiVertexArr *pv[3];
    BgiVertexArr v[3];
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
    s32 a;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
    s32 ax;
    s32 m;
    s32 min;
    s32 ex;
    s32 ez;

    edge = arg7;
    x = 0;
    z = 2;
    y = 1;
    if (n->normal >= 0) {
        pl = &g_fieldBgi.normals[n->normal];
        if ((u32)(pl->ny + 0x50F4) <= 0xA1E8) {
            a = pl->nx;
            c = pl->nz;
            if (ABS(a) > ABS(c)) {
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
    vtx = (BgiVertexArr *)((u8 *)b + b->vertexOffset);
    o[0] = b->curPos[0] + t->curPos[0];
    o[1] = b->curPos[1] + t->curPos[1];
    o[2] = b->curPos[2] + t->curPos[2];
    for (i = 0; i < 3; i++) {
        v[i].unk0[0] = o[0] + vtx[n->vertices[i]].unk0[0];
        v[i].unk0[1] = o[1] + vtx[n->vertices[i]].unk0[1];
        v[i].unk0[2] = o[2] + vtx[n->vertices[i]].unk0[2];
    }
    pv[0] = &v[2];
    cnt = 0;
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        if (i != edge) {
            d = (pv[1]->unk0[x] - pv[0]->unk0[x]) * (ez = arg3[z] - arg4[z]);
            ex = arg3[x] - arg4[x];
            e = (pv[0]->unk0[z] - arg4[z]) * ex;
            d -= (pv[1]->unk0[z] - pv[0]->unk0[z]) * ex;
            e -= (pv[0]->unk0[x] - arg4[x]) * ez;
            if (ABS(e) <= ABS(d)) {
                f = (pv[0]->unk0[z] - arg4[z]) * (pv[1]->unk0[x] - pv[0]->unk0[x]);
                f -= (pv[0]->unk0[x] - arg4[x]) * (pv[1]->unk0[z] - pv[0]->unk0[z]);
                if (ABS(f) <= ABS(d)) {
                    if ((e >= 0 && d >= 0) || (e <= 0 && d < 0)) {
                        if ((f >= 0 && d >= 0) || (f <= 0 && d < 0)) {
                            if (d != 0) {
                                while (e > 0x80000) {
                                    e >>= 1;
                                    d >>= 1;
                                }
                                hit[cnt][x] = pv[0]->unk0[x] + e * (pv[1]->unk0[x] - pv[0]->unk0[x]) / d;
                                hit[cnt][z] = pv[0]->unk0[z] + e * (pv[1]->unk0[z] - pv[0]->unk0[z]) / d;
                                ax = pv[1]->unk0[x] - pv[0]->unk0[x];
                                if (ax < 0) {
                                    ax = pv[0]->unk0[x] - pv[1]->unk0[x];
                                }
                                if ((pv[1]->unk0[z] - pv[0]->unk0[z] >= 0 ? pv[1]->unk0[z] - pv[0]->unk0[z] : pv[0]->unk0[z] - pv[1]->unk0[z]) < ax) {
                                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[x] - hit[cnt][x]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[x] - pv[1]->unk0[x]);
                                } else {
                                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[z] - hit[cnt][z]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[z] - pv[1]->unk0[z]);
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
        *arg6 = idx[0];
        arg5->vx = hit[0][0];
        arg5->vy = hit[0][1];
        arg5->vz = hit[0][2];
    } else if (cnt < 2) {
        *arg6 = -1;
        return 0;
    } else switch (cnt) {
    case 2:
        switch (idx[0] + idx[1]) {
        case 1:
            pv[0] = &v[1];
            pv[1] = &v[2];
            pv[2] = &v[0];
            break;
        case 2:
            pv[0] = &v[1];
            pv[1] = &v[0];
            pv[2] = &v[2];
            break;
        case 3:
            pv[0] = &v[2];
            pv[1] = &v[0];
            pv[2] = &v[1];
            break;
        }
        if (bgi_segmentsCross(pv[0]->unk0, arg3, pv[2]->unk0, pv[1]->unk0, x, z)) {
            *arg6 = idx[0];
            arg5->vx = hit[0][0];
            arg5->vy = hit[0][1];
            arg5->vz = hit[0][2];
        } else {
            *arg6 = idx[1];
            arg5->vx = hit[1][0];
            arg5->vy = hit[1][1];
            arg5->vz = hit[1][2];
        }
        break;
    case 3:
        best = 0;
        min = 0x7FFFFFFF;
        for (i = 0; i < 3; i++) {
            m = hit[i][0] * hit[i][0] + hit[i][1] * hit[i][1] + hit[i][2] * hit[i][2];
            if (m < min) {
                best = i;
                min = m;
            }
        }
        *arg6 = best;
        arg5->vx = hit[best][0];
        arg5->vy = hit[best][1];
        arg5->vz = hit[best][2];
        break;
    default:
        *arg6 = -1;
        return 0;
    }
    return 1;
}

/* debug: print a walkmesh */
void bgi_dumpText(BgiHeader *arg0) {
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

    if (arg0 == NULL) {
        return;
    }
    printf("BGI Text Dump\n");
    printf("=============\n");
    printf("Data Size\t\t : %d [+4 for file magic]\n", arg0->dataSize);
    printf("Original Position: (%6d,%6d,%6d)\n", arg0->orgX, arg0->orgY, arg0->orgZ);
    printf("Current Position : (%6d,%6d,%6d)\n", arg0->curPos[0], arg0->curPos[1], arg0->curPos[2]);
    printf("Minimum Values   : (%6d,%6d,%6d)\n", arg0->minX, arg0->minY, arg0->minZ);
    printf("Maximum Values   : (%6d,%6d,%6d)\n", arg0->maxX, arg0->maxY, arg0->maxZ);
    printf("Default Char Pos : (%6d,%6d,%6d)\n", arg0->charX, arg0->charY, arg0->charZ);
    printf("Active Floor\t : %d\n", arg0->activeFloor);
    printf("Active Triangle  : %d\n", arg0->activeTriangle);
    printf("Triangle Count   : %d\n", arg0->triangleCount);
    printf("Triangle Offset  : %d\n", arg0->triangleOffset);
    tri = (BgiTriangle *)((u8 *)arg0 + arg0->triangleOffset);
    for (i = 0; i < arg0->triangleCount; tri++, i++) {
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
    printf("Edge Count  \t : %d\n", arg0->edgeCount);
    printf("Edge Offset \t : %d\n", arg0->edgeOffset);
    edge = (BgiEdgeView *)((u8 *)arg0 + arg0->edgeOffset);
    for (i = 0; i < arg0->edgeCount; edge++, i++) {
        printf("  %4d: Flags\t   = 0x%04X\n", i, edge->flags);
        printf("\t\tEdge Clone  = %d\n", edge->clone);
    }
    printf("Keyframe Animation Count  : %d\n", arg0->animCount);
    printf("Keyframe Animation Offset : %d\n", arg0->animOffset);
    anim = (BgiAnim *)((u8 *)arg0 + arg0->animOffset);
    for (i = 0; i < arg0->animCount; anim++, i++) {
        printf("  %4d: Flags\t\t  = 0x%04X\n", i, anim->flags);
        printf("\t\tFrame Count    = %d\n", anim->frameCount);
        printf("\t\tCurrent Frame  = %ld\n", anim->curFrame);
        printf("\t\tFrame Rate     = %d\n", anim->frameRate);
        printf("\t\tCounter \t   = %d\n", (u16)anim->counter);
        printf("\t\tFrame Offset   = %ld\n", anim->frameOffset);
        frame = (BgiKeyframe *)((u8 *)arg0 + anim->frameOffset);
        for (j = 0; j < anim->frameCount; frame++, j++) {
            printf("\t\t%4d: Frame Flags     = 0x%04X\n", j, frame->flags);
            printf("              Value           = %d\n", frame->value);
            printf("              Triangle Count  = %d\n", frame->triangleCount);
            printf("\t\t\t  Triangle Offset = %d\n", frame->triangleOffset);
            ids = (s32 *)((u8 *)arg0 + frame->triangleOffset);
            for (k = 0; k < frame->triangleCount; ids++, k++) {
                printf("\t\t\t\t%4d: Triangle Ndx = %ld\n", j, *ids);
            }
        }
    }
    printf("Floor Count \t : %d\n", arg0->floorCount);
    printf("Floor Offset\t : %d\n", arg0->floorOffset);
    floor = (BgiFloor *)((u8 *)arg0 + arg0->floorOffset);
    for (i = 0; i < arg0->floorCount; floor++, i++) {
        printf("  %4d: Flags\t\t\t\t= %d\n", i, floor->flags);
        printf("\t\tFloor Index \t\t  = %d\n", floor->floorIndex);
        printf("\t\tOriginal Position\t  = (%6d,%6d,%6d)\n", floor->orgPos[0], floor->orgPos[1], floor->orgPos[2]);
        printf("\t\tCurrent Position\t  = (%6d,%6d,%6d)\n", (s16)floor->curPos[0], (s16)floor->curPos[1], (s16)floor->curPos[2]);
        printf("\t\tMinumim Values  \t  = (%6d,%6d,%6d)\n", floor->min[0], floor->min[1], floor->min[2]);
        printf("\t\tMaximum Values  \t  = (%6d,%6d,%6d)\n", floor->max[0], floor->max[1], floor->max[2]);
        printf("\t\tTriangle Count  \t  = %d\n", floor->triangleCount);
        printf("\t\tTriangle Index Offset = %d\n", floor->triangleOffset);
        ids = (s32 *)((u8 *)arg0 + floor->triangleOffset);
        for (j = 0; j < floor->triangleCount; ids++, j++) {
            printf("\t\t  %4d: Triangle Ndx = %ld\n", j, *ids);
        }
    }
    printf("Vertex Count\t : %d\n", arg0->vertexCount);
    printf("Vertex Offset\t : %d\n", arg0->vertexOffset);
    vtx = (BgiVertex *)((u8 *)arg0 + arg0->vertexOffset);
    for (i = 0; i < arg0->vertexCount; vtx++, i++) {
        printf("  %4d: (%d,%d,%d)\n", i, vtx->vx, vtx->vy, vtx->vz);
    }
    printf("Normal Count\t : %d\n", arg0->normalCount);
    printf("Normal Offset\t : %d\n", arg0->normalOffset);
    nrm = (BgiNormal *)((u8 *)arg0 + arg0->normalOffset);
    for (i = 0; i < arg0->normalCount; nrm++, i++) {
        printf("  %4d: (%lx,%lx,%lx)\n", i, nrm->nx, nrm->ny, nrm->nz);
        printf(D_800A7814, nrm->invNy);
    }
}

s32 bgi_updateAnims(void) {
    BgiHeader *b;
    BgiAnim *e;
    u16 cnt;
    BgiTriangle *nodes;
    u16 i;
    s16 old;
    BgiKeyframe *pts;
    BgiKeyframe *pt;
    u16 j;
    u16 k;
    s32 *ids;
    BgiTriangle *n;
    s32 pos;

    s32 v;
    u16 f;
    u16 num;
    u16 m;

    b = g_fieldBgi.header;
    e = g_fieldBgi.anims;
    cnt = b->animCount;
    nodes = g_fieldBgi.triangles;
    for (i = 0; i < cnt; e++, i++) {
        if (!(e->flags & 1)) {
            continue;
        }
        if (!(e->flags & 0xC)) {
            continue;
        }
        old = -1;
        pts = (BgiKeyframe *)((u8 *)b + e->frameOffset);
        num = e->frameCount;
        for (j = 0; j < num; j++) {
            ids = (s32 *)((u8 *)b + pts[j].triangleOffset);
            m = pts[j].triangleCount;
            for (k = 0; k < m; k++, ids++) {
                f = nodes[*ids].flags;
                if (f & 1) {
                    old = j;
                }
                nodes[*ids].flags = f & ~1;
            }
        }
        pos = e->curFrame;
        j = pos >> 8;
        ids = (s32 *)((u8 *)b + pts[j].triangleOffset);
        m = pts[j].triangleCount;
        for (k = 0; k < m; k++) {
            nodes[*ids++].flags |= 1;
        }
        if (old != j) {
            pt = &pts[j];
            bgi_remapActors(b, e, pt, old, j);
            bgi_linkFrameNeighbors(b, e, pt);
        }
        if (pts[j].value < 0) {
            pts[j].value = 0;
            e->counter = 0;
            e->flags &= ~4;
            return 1;
        }
        e->counter++;
        if ((u16)e->counter >= pts[j].value) {
            e->counter = 0;
            e->curFrame = pos + e->frameRate;
            v = e->curFrame >> 8;
            if (v >= e->frameCount) {
                if (e->flags & 0x10) {
                    e->curFrame = ((e->frameCount - 1) << 8) | 0xFF;
                    bgi_setAnimRate(i, (s16)-e->frameRate);
                } else {
                    e->curFrame = 0;
                }
                e->flags &= ~4;
            } else if (v < 0) {
                if (e->flags & 0x10) {
                    e->curFrame = 0;
                    bgi_setAnimRate(i, (s16)-e->frameRate);
                } else {
                    e->curFrame = ((e->frameCount - 1) << 8) | 0xFF;
                }
                e->flags &= ~4;
            }
        }
    }
    return 1;
}

s32 bgi_showAnimFrame(u32 arg0, u32 arg1) {
    BgiHeader *b;
    BgiAnim *e;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    s16 old;
    BgiTriangle *n;
    u16 f;

    b = g_fieldBgi.header;
    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    old = -1;
    if (arg1 >= e->frameCount) {
        return 0;
    }
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    for (i = 0; i < e->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; ids++, j++) {
            n = &g_fieldBgi.triangles[*ids];
            if (n->flags & 1) {
                old = i;
                n->flags &= ~1;
            } else {
                n->flags &= ~1;
            }
        }
    }
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    pt += arg1;
    ids = (s32 *)((u8 *)b + pt->triangleOffset);
    for (i = 0; i < pt->triangleCount; ids++, i++) {
        n = &g_fieldBgi.triangles[*ids];
        n->flags |= 1;
    }
    if (arg1 != old) {
        bgi_remapActors(b, e, pt, old, arg1 & 0xFFFF);
        bgi_linkFrameNeighbors(b, e, pt);
    }
    e->curFrame = arg1 << 8;
    return 1;
}

/* walkmesh triangle transfer: express the actor position relative to triangle n0 by cross products (nv, over
   the triangle's doubled area) and re-project it onto triangle n1; the height comes from n1's plane or, without
   one, from n1's first vertex */
s32 bgi_transferActorPos(BgiHeader *b, BgiActor *a, BgiTriangle *n0, BgiTriangle *n1) {
    SVECTOR o;
    SVECTOR v[3];
    SVECTOR p;
    VECTOR nv;
    VECTOR r;
    BgiFloor *t;
    BgiVertex *vtx;
    BgiNormal *pl;
    s32 dx0, dx1, dx2, dz1, dz2;
    s32 d;
    s32 area;
    s32 k;
    s32 c;

    t = &g_fieldBgi.floors[n0->floor];
    o.vx = b->curPos[0] + t->curPos[0];
    o.vy = b->curPos[1] + t->curPos[1];
    o.vz = b->curPos[2] + t->curPos[2];
    vtx = (BgiVertex *)((u8 *)b + b->vertexOffset);
    v[0].vx = vtx[n0->vertices[0]].vx;
    v[1].vx = vtx[n0->vertices[1]].vx;
    v[2].vx = vtx[n0->vertices[2]].vx;
    p.vx = a->actor->pos.vx - o.vx;
    v[0].vy = vtx[n0->vertices[0]].vy;
    v[1].vy = vtx[n0->vertices[1]].vy;
    v[2].vy = vtx[n0->vertices[2]].vy;
    p.vy = a->actor->pos.vy - o.vy;
    v[0].vz = vtx[n0->vertices[0]].vz;
    v[1].vz = vtx[n0->vertices[1]].vz;
    v[2].vz = vtx[n0->vertices[2]].vz;
    p.vz = a->actor->pos.vz - o.vz;
    dx0 = v[0].vx - p.vx;
    dx1 = v[1].vx - p.vx;
    area = (v[1].vx - v[0].vx) * (v[2].vz - v[0].vz);
    area -= (v[2].vx - v[0].vx) * (v[1].vz - v[0].vz);
    dx2 = v[2].vx - p.vx;
    d = v[0].vz - p.vz;
    dz1 = v[1].vz - p.vz;
    dz2 = v[2].vz - p.vz;
    nv.vx = dx1 * dz2 - dx2 * dz1;
    nv.vy = dx2 * d - dx0 * dz2;
    nv.vz = dx0 * dz1 - dx1 * d;
    t = &g_fieldBgi.floors[n1->floor];
    o.vx = b->curPos[0] + t->curPos[0];
    o.vy = b->curPos[1] + t->curPos[1];
    o.vz = b->curPos[2] + t->curPos[2];
    v[0].vx = vtx[n1->vertices[0]].vx;
    v[1].vx = vtx[n1->vertices[1]].vx;
    v[2].vx = vtx[n1->vertices[2]].vx;
    v[0].vy = vtx[n1->vertices[0]].vy;
    v[1].vy = vtx[n1->vertices[1]].vy;
    v[2].vy = vtx[n1->vertices[2]].vy;
    v[0].vz = vtx[n1->vertices[0]].vz;
    v[1].vz = vtx[n1->vertices[1]].vz;
    v[2].vz = vtx[n1->vertices[2]].vz;
    r.vx = (nv.vx * v[0].vx + nv.vy * v[1].vx + nv.vz * v[2].vx) / area;
    r.vz = (nv.vx * v[0].vz + nv.vy * v[1].vz + nv.vz * v[2].vz) / area;
    a->actor->pos.vx = r.vx + o.vx;
    a->actor->pos.vz = r.vz + o.vz;
    k = n1->normal;
    if (k >= 0) {
        pl = &g_fieldBgi.normals[k];
        c = func_800C1DC4(pl->nx, a->actor->pos.vx << 16);
        a->actor->pos.vy = func_800C1DC4(n1->d - c - func_800C1DC4(pl->nz, a->actor->pos.vz << 16), pl->invNy) >> 16;
        a->actor->pos.vy += (s16)g_fieldBgi.floors[n1->floor].curPos[1] - g_fieldBgi.floors[n1->floor].orgPos[1];
    } else {
        BgiFloor *t3;
        BgiVertex *vt;
        t3 = g_fieldBgi.floors + n1->floor;
        vt = g_fieldBgi.vertices + n1->vertices[0];
        a->actor->pos.vy = (s16)(vt->vy + (g_fieldBgi.header->curPos[1] + t3->curPos[1]));
    }
    return 1;
}

s32 bgi_remapActors(u8 *arg0, BgiAnim *arg1, BgiKeyframe *arg2, s16 arg3) {
    BgiKeyframe *e;
    s32 *tbl;
    s32 *ids;
    BgiActor *a;
    u16 i;
    u16 j;

    if (arg3 == -1) {
        return 0;
    }
    e = (BgiKeyframe *)(arg0 + arg1->frameOffset);
    e += arg3;
    tbl = (s32 *)(arg0 + arg2->triangleOffset);
    a = g_fieldBgi.actors;
    for (i = 0; i < g_fieldBgi.actorCount; a++, i++) {
        if (a->flags & 1) {
            ids = (s32 *)(arg0 + e->triangleOffset);
            for (j = 0; j < e->triangleCount; j++, ids++) {
                if (a->triangle == *ids) {
                    bgi_transferActorPos(arg0, a, &g_fieldBgi.triangles[a->triangle], &g_fieldBgi.triangles[tbl[j]]);
                    a->triangle = tbl[j];
                    a->floor = g_fieldBgi.triangles[tbl[j]].floor;
                }
            }
        }
    }
    return 1;
}

/* link the neighbour edges between the nodes of different triangles */
s32 bgi_linkFrameNeighbors(BgiHeader *arg0, BgiAnim *arg1, BgiKeyframe *arg2) {
    s32 *ids = (s32 *)((u8 *)arg0 + arg2->triangleOffset);
    BgiTriangle *n;
    BgiTriangle *m;
    BgiEdge *e;
    s16 nb;
    u16 i;
    u16 j;
    u16 k;

    for (i = 0; i < arg2->triangleCount; i++, ids++) {
        n = &g_fieldBgi.triangles[*ids];
        for (j = 0; j < 3; j++) {
            nb = n->neighbors[j];
            if (nb >= 0) {
                m = &g_fieldBgi.triangles[nb];
                if (n->floor != m->floor) {
                    e = &g_fieldBgi.edges[n->edges[j]];
                    if (e->clone >= 0) {
                        k = e->clone;
                        m->neighbors[k] = *ids;
                        e = &g_fieldBgi.edges[m->edges[k]];
                        e->clone = j;
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
s32 bgi_stepFloorMove(arg0)
s32 arg0;
{
    BgiFloorMove *e;
    s32 step;
    s16 range;
    s16 v;

    e = &g_fieldBgiFloorMoves[arg0];
    e->accum += e->speed;
    step = e->accum >> 8;
    range = e->max - e->min;
    if (e->range != 0 && e->speed != 0 && range > 0) {
        switch (e->curve) {
        case 0:
            v = e->min + step;
            break;
        case 1:
            v = e->min + range * ((s16)math_cos((s16)((step << 12) / range) + 0x800) + 0x1000) / 0x2000;
            break;
        default:
            v = e->min + step;
            break;
        }
        e->prev = e->cur;
        e->cur = v;
        if (e->max < v) {
            if (e->flags & 0x10) {
                e->cur = e->max;
                e->speed = -e->speed;
            } else {
                e->cur = e->min;
            }
            e->accum = (range << 8) - 0x100;
            e->flags &= ~4;
        } else if (v < e->min) {
            if (e->flags & 0x10) {
                e->cur = e->min;
                e->speed = -e->speed;
            } else {
                e->cur = e->max;
            }
            e->accum = 0;
            e->flags &= ~4;
        }
    } else {
        return 0;
    }
    return 1;
}

s32 bgi_updateFloorMoves(void) {
    BgiFloorMove *e;
    BgiActor *a;
    BgiFloor *t;
    s32 i;
    s32 j;
    Unk8004AB64 *p;
    u16 flags;

    e = g_fieldBgiFloorMoves;
    for (i = 0; i < 30; e++, i++) {
        if (!(e->flags & 1) || !(e->flags & 0xC)) {
            continue;
        }
        t = &g_fieldBgi.floors[e->floor];
        t->curPos[e->axis] = t->orgPos[e->axis] + e->cur;
        a = g_fieldBgi.actors;
        for (j = 0; j < g_fieldBgi.actorCount; a++, j++) {
            flags = a->flags;
            if ((flags & 1) && a->floor == e->floor && a->triangle >= 0) {
                p = a->actor;
                ((ActorPosView *)p)->unk14[e->axis] += e->cur - e->prev;
                switch (e->axis) {
                case 0:
                    a->actor->unk40.vx = a->actor->pos.vx;
                    break;
                case 1:
                    a->actor->unk40.vy = a->actor->pos.vy;
                    break;
                case 2:
                    a->actor->unk40.vz = a->actor->pos.vz;
                    break;
                }
            }
        }
        bgi_stepFloorMove(i);
    }
    return 1;
}

s32 bgi_setMesh(BgiHeader *arg0) {
    s32 i;

    g_fieldBgi.header = arg0;
    g_fieldBgi.vertices = (BgiVertex *)((u8 *)arg0 + arg0->vertexOffset);
    g_fieldBgi.triangles = (BgiTriangle *)((u8 *)arg0 + arg0->triangleOffset);
    g_fieldBgi.normals = (BgiNormal *)((u8 *)arg0 + arg0->normalOffset);
    g_fieldBgi.edges = (u8 *)arg0 + arg0->edgeOffset;
    g_fieldBgi.floors = (BgiFloor *)((u8 *)arg0 + arg0->floorOffset);
    g_fieldBgi.anims = (BgiAnim *)((u8 *)arg0 + arg0->animOffset);
    for (i = 0; i < g_fieldBgi.header->animCount; i++) {
        bgi_initAnim(i);
    }
    bgi_resetFloorMoves();
    return 1;
}

s32 bgi_updateActors(void) {
    s32 n = 0;
    s16 i = 0;

    if (g_fieldBgi.actorCount != 0) {
        do {
            if (bgi_updateActor(i) > 0) {
                n++;
            }
            i++;
        } while (i < g_fieldBgi.actorCount);
    }
    return n & 0xFFFF;
}

s32 bgi_clear(void) {
    g_fieldBgi.header = 0;
    g_fieldBgi.actorCount = 0;
    g_fieldBgi.actors = 0;
    g_fieldBgi.loaded = 0;
    return 1;
}

s32 func_800C7450(void) {
    return 1;
}

void func_800C7458(s32 *arg0) {
    *arg0 = 1;
}

s32 bgi_setMaskFilter(u8 arg0) {
    g_fieldBgi.maskFilter = arg0;
    return 1;
}

s32 bgi_getMaskFilter(u8 *arg0) {
    *arg0 = g_fieldBgi.maskFilter;
    return 1;
}

BgiActor *bgi_getActor(s32 arg0) {
    return &g_fieldBgi.actors[arg0];
}

s32 bgi_bindActor(s32 arg0, FieldObjSlot *arg1) {
    g_fieldBgi.actors[arg0].flags = 0;
    g_fieldBgi.actors[arg0].unk2 = 0;
    g_fieldBgi.actors[arg0].actor = arg1->unkC;
    return 1;
}

s32 bgi_addActor(FieldObjSlot *arg0) {
    g_fieldBgi.actors[g_fieldBgi.actorCount].flags = 0;
    g_fieldBgi.actors[g_fieldBgi.actorCount].unk2 = 0;
    g_fieldBgi.actors[g_fieldBgi.actorCount].actor = arg0->unkC;
    g_fieldBgi.actorCount++;
    return 1;
}

s32 bgi_setActorRadius(s32 arg0, u16 arg1) {
    g_fieldBgi.actors[arg0].radius = arg1;
    g_fieldBgi.actors[arg0].radiusSq = arg1 * arg1;
    return 1;
}

s32 bgi_getActorRadius(s32 arg0, s32 *arg1) {
    *arg1 = g_fieldBgi.actors[arg0].radius;
    return 1;
}

s32 bgi_getActorRadiusSq(s32 arg0, s32 *arg1) {
    *arg1 = g_fieldBgi.actors[arg0].radiusSq;
    return 1;
}

s32 bgi_getActorNormal(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    BgiNormal *e;
    BgiTriangle *n;
    struct BgiActor *a;

    a = g_fieldBgi.actors + arg0;
    n = g_fieldBgi.triangles + a->triangle;
    e = g_fieldBgi.normals + n->normal;
    *arg1 = e->nx;
    *arg2 = e->ny;
    *arg3 = e->nz;
    *arg4 = e->invNy;
    return 1;
}

s32 bgi_getActorTilt(s32 arg0, s16 *arg1, u16 *arg2) {
    BgiActor *a = &g_fieldBgi.actors[arg0];
    BgiTriangle *e = &g_fieldBgi.triangles[a->triangle];

    *arg1 = -e->thetaZ;
    *arg2 = e->thetaX;
    return 1;
}

s32 bgi_getActorTriangle(s32 arg0, u16 *arg1, u16 *arg2) {
    BgiActor *e = &g_fieldBgi.actors[arg0];

    *arg1 = e->triangle;
    *arg2 = e->floor;
    return 1;
}

s32 bgi_revertActor(s32 arg0) {
    BgiActor *e = &g_fieldBgi.actors[arg0];

    e->actor->pos.vx = e->actor->unk40.vx;
    e->actor->pos.vy = e->actor->unk40.vy;
    e->actor->pos.vz = e->actor->unk40.vz;
    e->floor = e->lastFloor;
    e->triangle = e->lastTriangle;
    return 1;
}

s32 bgi_setTriangleActive(s32 arg0, s32 arg1) {
    BgiTriangle *n = bgi_trianglePtr(arg0);
    u16 v;

    if (arg1 != 0) {
        v = n->flags | 1;
    } else {
        v = n->flags & ~1;
    }
    n->flags = v;
    return 1;
}

s32 func_800C77DC(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DDE;
    *arg1 = D_800CAA98;
    return 1;
}

s32 bgi_getTriangleFlags(s16 arg0, u16 *arg1) {
    *arg1 = bgi_trianglePtr(arg0)->flags & ((g_fieldBgi.maskFilter << 8) | 0xFF);
    return 1;
}

s32 bgi_setTriangleMask(s16 arg0, s32 arg1) {
    BgiTriangle *n = bgi_trianglePtr(arg0);

    n->flags = ((arg1 & 0xFF) << 8) | (n->flags & 0xFF);
    return 1;
}

s32 bgi_getTriangleMask(s16 arg0, u8 *arg1) {
    *arg1 = (bgi_trianglePtr(arg0)->flags >> 8) & g_fieldBgi.maskFilter;
    return 1;
}

s32 bgi_getTriangleFloor(s16 arg0, u16 *arg1) {
    *arg1 = bgi_trianglePtr(arg0)->floor;
    return 1;
}

s32 func_800C7914(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DE0;
    *arg1 = D_800CAAB8;
    return 1;
}

s32 bgi_getEdgeFlags(s16 arg0, u16 *arg1) {
    *arg1 = bgi_edgePtr(arg0)->flags;
    return 1;
}

s32 bgi_getFloorPosition(s32 arg0, u16 *arg1, u16 *arg2, u16 *arg3) {
    BgiFloor *t = bgi_floorPtr(arg0);

    *arg1 = t->curPos[0];
    *arg2 = t->curPos[1];
    *arg3 = t->curPos[2];
    return 1;
}

s32 bgi_initAnim(u32 arg0) {
    BgiHeader *b;
    BgiAnim *e;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    BgiTriangle *n;

    b = g_fieldBgi.header;
    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    for (i = 0; i < e->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; j++) {
            n = &g_fieldBgi.triangles[*ids];
            if (i == 0) {
                n->flags |= 1;
                bgi_linkFrameNeighbors(b, e, pt);
            } else {
                n->flags &= ~1;
            }
            ids++;
        }
    }
    return 1;
}

s32 bgi_setAnimFrame(u32 arg0, u32 arg1) {
    BgiAnim *e;

    if (arg0 >= g_fieldBgi.header->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    if (arg1 >= e->frameCount) {
        return 0;
    }
    e->curFrame = arg1 << 8;
    e->counter = 0;
    e->flags |= 1;
    return 1;
}

s32 bgi_setAnimPlaying(u32 arg0, s32 arg1) {
    BgiAnim *e;

    if (arg0 >= g_fieldBgi.header->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    if (arg1 != 0) {
        e->flags |= 5;
    } else {
        e->flags &= ~5;
    }
    return 1;
}

s32 bgi_setAnimRate(u32 arg0, s16 arg1) {
    BgiAnim *e;

    if (arg0 >= g_fieldBgi.header->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    e->frameRate = arg1;
    return 1;
}

s32 bgi_setAnimFrameValues(u32 arg0, s16 arg1) {
    BgiHeader *b = g_fieldBgi.header;
    BgiAnim *e;
    BgiKeyframe *pt;
    s32 i;

    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    for (i = 0; i < e->frameCount; pt++, i++) {
        pt->value = arg1;
    }
    return 1;
}

s32 bgi_setAnimFrameValue(u32 arg0, u32 arg1, s16 arg2) {
    BgiHeader *b = g_fieldBgi.header;
    BgiAnim *e;
    BgiKeyframe *pt;

    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    if (arg1 >= e->frameCount) {
        return 0;
    }
    pt = &((BgiKeyframe *)((u8 *)b + e->frameOffset))[arg1];
    pt->value = arg2;
    return 1;
}

s32 bgi_setAnimMode(u32 arg0, s32 arg1) {
    BgiAnim *e;

    if (arg0 >= g_fieldBgi.header->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    e->flags = (e->flags & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 bgi_playAnimRange(u32 arg0, u32 arg1, u32 arg2) {
    BgiHeader *b = g_fieldBgi.header;
    BgiAnim *e;
    BgiKeyframe *pt;

    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    if (arg1 >= e->frameCount) {
        return 0;
    }
    if (arg2 >= e->frameCount) {
        return 0;
    }
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    e->curFrame = arg1 << 8;
    e->counter = 0;
    e->flags |= 5;
    pt += arg2;
    pt->value = -1;
    if ((e->frameRate > 0 && arg2 < arg1) || (e->frameRate < 0 && arg1 < arg2)) {
        e->frameRate = -e->frameRate;
    }
    return 1;
}

s32 bgi_resetAnim(u32 arg0, s32 arg1) {
    BgiHeader *b = g_fieldBgi.header;
    BgiAnim *e;
    BgiKeyframe *pt;
    s32 *ids;
    s32 i;
    s32 j;
    BgiTriangle *n;

    if (arg0 >= b->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    e->frameRate = 0x100;
    e->flags = 2;
    e->curFrame = 0;
    e->counter = 0;
    pt = (BgiKeyframe *)((u8 *)b + e->frameOffset);
    for (i = 0; i < e->frameCount; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->triangleOffset);
        for (j = 0; j < pt->triangleCount; ids++, j++) {
            n = &g_fieldBgi.triangles[*ids];
            if (arg1 != 0 && i == 0) {
                n->flags |= 1;
            } else {
                n->flags &= ~1;
            }
        }
    }
    return 1;
}

s32 bgi_getAnimFrame(u32 arg0, s32 *arg1) {
    BgiAnim *e;

    if (arg0 >= g_fieldBgi.header->animCount) {
        return 0;
    }
    e = &g_fieldBgi.anims[arg0];
    *arg1 = e->curFrame >> 8;
    return 1;
}

s32 bgi_startFloorMove(s32 arg0, s32 arg1) {
    BgiFloorMove *e = &g_fieldBgiFloorMoves[arg0];

    e->accum = arg1 << 8;
    e->flags |= 5;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_stopFloorMove(s32 arg0, s32 arg1) {
    BgiFloorMove *e = bgi_getFloorMove_inline(arg0);

    arg1 <<= 8;
    e->accum = arg1;
    e->flags = 0;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_setFloorMovePlaying(s32 arg0, s32 arg1) {
    BgiFloorMove *e = bgi_getFloorMove_inline(arg0);
    u16 v;

    if (arg1 != 0) {
        v = e->flags | 5;
    } else {
        v = e->flags & ~5;
    }
    e->flags = v;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_setFloorMoveMode(s32 arg0, s32 arg1) {
    BgiFloorMove *e = bgi_getFloorMove_inline(arg0);

    e->flags = (e->flags & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 bgi_setFloorMoveFloor(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->floor = arg1;
    return 1;
}

s32 bgi_setFloorMoveSpeed(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->speed = arg1;
    return 1;
}

s32 bgi_setFloorMoveCurve(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->curve = arg1;
    return 1;
}

s32 bgi_setFloorMoveRange(s32 arg0, s16 arg1, s16 arg2) {
    BgiFloorMove *e = bgi_getFloorMove_inline(arg0);

    e->min = arg1;
    e->max = arg2;
    e->range = e->max - e->min;
    return 1;
}

s32 bgi_setFloorMoveAxis(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->axis = arg1;
    return 1;
}

BgiFloorMove *bgi_getFloorMove(s32 arg0) {
    return &g_fieldBgiFloorMoves[arg0];
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/bgi", D_800A7814);

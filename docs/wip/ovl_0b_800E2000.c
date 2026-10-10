#include "ovl_0b.h"

void func_800E2000(Pool30 *pool, Pool6C *list) {
    SVECTOR off;
    SVECTOR rot;
    SVECTOR tmp;
    SVECTOR ang;
    CVECTOR col;
    s32 k;
    s32 r;
    s32 d58;
    Pool6C *pl;
    s32 i;
    s32 j;
    s32 dur;
    s32 t;
    Pool6CEntry *e;
    Pool30Entry *p;
    EffElemHdr *hdr;
    EffElem *el;
    EffAnim *anim;
    SVECTOR *base;
    s32 frame;
    s32 acc;
    s32 n;

    el = NULL;
    pl = list;
    e = pl->unkC;
    base = NULL;
    for (i = 0; i <= pl->unk8; i++, e++) {
        if (e->unk30 == 0) {
            continue;
        }
        if (e->unk30 == -1) {
            continue;
        }
        for (j = 0; j < e->unk32; j++) {
            if (e->unk34.s.x >= e->unk24->unk4.unk8) {
                break;
            }
            p = func_800E1F90(pool);
            if (p == NULL) {
                break;
            }
            frame = e->unk38;
            anim = &e->unk24->unk4;
            e->unk34.s.x++;
            dur = anim->unk2;
            t = (frame << 12) / dur;
            hdr = (EffElemHdr *)e->unk18 - 1;
            acc = 0;
            r = func_800CF690() & 0x7F;
            for (k = 0; k < hdr->unk0; k++) {
                acc += hdr->unk10.b[k];
                if (r < acc) {
                    break;
                }
            }
            if (k >= hdr->unk0) {
                k = 0xFF;
            }
            p->unk1C = k;
            p->unk1D = 0;
            if (k == 0xFF) {
                acc = func_800CF9B0(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                acc += func_800CF748(((EffEmit *)e->unk0)->unk6);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &e->unk18[k];
                if (el->unk4 == 1) {
                    acc = el->unk8;
                } else {
                    acc = func_800CF9B0(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                    acc += func_800CF748(((EffEmit *)e->unk0)->unk6);
                    if (acc <= 0) {
                        acc = 1;
                    }
                }
                off.vx = (el->unk14[0] * 8) & 0xFFF;
                off.vy = (el->unk14[1] * 8) & 0xFFF;
                off.vz = (el->unk14[2] * 8) & 0xFFF;
                gte_lddp(frame);
                gte_ldsv(&off);
                gte_gpf0();
                ang.vx = (el->unk11[0] * 16 + func_800CF748(el->unk17[0] * 16)) & 0xFFF;
                ang.vy = (el->unk11[1] * 16 + func_800CF748(el->unk17[1] * 16)) & 0xFFF;
                ang.vz = (el->unk11[2] * 16 + func_800CF748(el->unk17[2] * 16)) & 0xFFF;
                gte_lddp(1);
                gte_ldsv(&ang);
                gte_gpl0();
                p->unk13 = func_800CF748(el->unk10);
                gte_stsv(&p->unk14);
            }
            p->unk1F = e->unkE % 255 + 1;
            p->unk1B = acc;
            p->unk1A = 0;
            p->unk2C = e;
            p->unk1E = func_800CF748(anim->unkA) + 0x40;
            if (((EffEmit *)e->unk0)->unk1 != 0) {
                p->unk12 = 0x7F;
            } else {
                p->unk12 = 0;
            }
            e->unkE++;
            if (e->unk14 != 0) {
                e->unkE %= e->unk12;
            }
            switch (e->unk24->unk0) {
            case 0:
                func_800CF788(&rot);
                acc = func_800CF708(e->unk3C, e->unk3E);
                if (((EffTailSpawn *)e->unk24)->unk44.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (((EffTailSpawn *)e->unk24)->unk44.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                func_800CFCA8(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&((EffTailSpawn *)e->unk24)->unk44, (u8 *)&((EffTailSpawn *)e->unk24)->unk48.c, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = e->unk28;
                break;
            case 1:
                rot.vx = 0;
                rot.vy = func_800CF690();
                rot.vz = 0;
                func_800CFCA8(&rot, func_800CF708(e->unk3C, e->unk3E), (s32)&off);
                off.vy = func_800CF708(e->unk40, e->unk42);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = e->unk28;
                break;
            case 2:
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                d58 = func_800CF690();
                acc = func_800CF708(e->unk40, e->unk42);
                r = func_800CF708(e->unk3C, e->unk3E);
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
                break;
            case 3:
                func_800CF808((s16 *)e->unk28, (s16 *)e->unk2C, &ang.vx);
                d58 = func_800CF690();
                acc = func_800CF708(e->unk40, e->unk42);
                r = func_800CF708(e->unk3C, e->unk3E);
                func_800CFA00((s16 *)e->unk28, (s16 *)e->unk2C, acc, &off.vx);
                func_800D044C(&off, (s32)&off, 0, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
                off.vx -= base->vx;
                off.vy -= base->vy;
                off.vz -= base->vz;
                break;
            case 4: {
                s32 y4;
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                n = 1;
                if (e->unk40 != 0) {
                    n = e->unk40;
                }
                acc = func_800CF690() % n;
                r = func_800CF690() & 0xFFF;
                d58 = (e->unk3C * r) >> 12;
                y4 = (e->unk3E * r) >> 12;
                r = func_800CF9DC(d58, y4, acc, n);
                n = func_800CF690();
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, n);
                if (((EffTailSpawn *)e->unk24)->unk48.h == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk48.h == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                    func_800D044C(&tmp, (s32)&tmp, 0, (XY32 *)&ang, d58, n);
                    *(s32 *)&rot.vx = 0;
                    *(s32 *)&rot.vz = 0;
                    func_800D044C(&rot, (s32)&rot, e->unk40, (XY32 *)&ang, y4, n);
                    ((XY32 *)&ang)[0].w = ((XY32 *)&rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&rot)[1].w;
                    func_800CF808(&tmp.vx, &ang.vx, &rot.vx);
                }
                base = e->unk28;
                break;
            }
            case -1:
                off.pad = 0;
                if (((EffTailSpawn *)e->unk24)->unk3C(&off, &rot, frame, dur) != 0) {
                    anim->unk2 = 0;
                }
                if (off.pad != 0) {
                    base = &tmp;
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                } else {
                    base = e->unk28;
                }
                break;
            }
            p->unk0.vx = (off.vx + base->vx) << 12;
            p->unk0.vy = (off.vy + base->vy) << 12;
            p->unk0.vz = (off.vz + base->vz) << 12;
            switch (anim->unk28.pad) {
            case 1:
                rot.vx += func_800CF748(anim->unk30.pad);
                rot.vy += func_800CF748(anim->unk30.pad);
                rot.vx = (rot.vx + 0x800) & 0xFFF;
                break;
            case 2:
                func_800CF788(&rot);
                break;
            case 3:
                ((XY32 *)&rot)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&rot)[1].w = ((XY32 *)&e->unk5C)[1].w;
            case 0:
                rot.vx += func_800CF748(anim->unk30.pad);
                rot.vy += func_800CF748(anim->unk30.pad);
                break;
            }
            if (el != NULL && el->unk7 != 0) {
                p->unk14.s.x += rot.vx;
                p->unk14.s.y += rot.vy;
            }
            switch (((EffEmit *)e->unk0)->unk0) {
            case 1: {
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unkA;
                func_800CFCA8(&rot, acc + func_800CF748(mot->unk24), (s32)&p->unkC);
                acc = mot->unk10 + func_800CF748(mot->unk12);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk28.s.y = acc;
                p->unk24.w = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + func_800CF748(mot->unkC);
                p->unk22 = mot->unkA + func_800CF748(mot->unkE);
                p->unk28.s.x = e->unk8 + func_800CF748(mot->unk16);
                break;
            }
            case 2: {
                EffEmit *mot = (EffEmit *)e->unk0;
                p->unk28.w = *(s32 *)&rot.vx;
                func_800CFA00(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + func_800CF748(mot->unkE);
                p->unk24.s.y = tmp.vx + func_800CF748(mot->unk18);
                p->unk22 = e->unk8 + func_800CF748(mot->unkC);
                *(s32 *)&tmp.vx = 0;
                *(s32 *)&tmp.vz = 0;
                func_800D0674(&off, &tmp, &rot, (s32)&k, (s32)&r, &d58);
                p->unkC = d58;
                p->unk20 = k;
                p->unk24.s.x = r;
                p->unk0.vx = base->vx << 12;
                p->unk0.vy = base->vy << 12;
                p->unk0.vz = base->vz << 12;
                p->unk10 = 0;
                break;
            }
            case 3: {
                EffEmit *mot = (EffEmit *)e->unk0;
                *(s32 *)&p->unkC = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + func_800CF748(mot->unkC);
                p->unk22 = mot->unkA + func_800CF748(mot->unkE);
                p->unk10 = e->unk8 + func_800CF748(mot->unk16);
                p->unk24.s.y = rot.vx + func_800CF748(mot->unk10);
                p->unk28.s.x = rot.vy + func_800CF748(mot->unk12);
                p->unk24.s.x = 0;
                break;
            }
            case 4: {
                EffEmit *mot = (EffEmit *)e->unk0;
                p->unk28.w = *(s32 *)&rot.vx;
                func_800CFA00(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + func_800CF748(mot->unk1A);
                p->unk20 = tmp.vx + func_800CF748(mot->unk18);
                p->unk22 = e->unk8 + func_800CF748(mot->unkC);
                p->unkC = func_800CF690();
                p->unk24.w = 0;
                p->unk10 = 0;
                break;
            }
            case 0:
            case 5: {
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unk8;
                func_800CFCA8(&rot, acc + func_800CF748(mot->unkC), (s32)&p->unkC);
                acc = mot->unkE + func_800CF748(mot->unk10);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk20 = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->unk0 - 4) < 2 || el->unk0 == 8 || el->unk1 == 4 || el->unk1 == 5 || el->unk1 == 8)) {
                tmp.vx = p->unk0.vx >> 12;
                tmp.vy = p->unk0.vy >> 12;
                tmp.vz = p->unk0.vz >> 12;
                func_800D1494((u8 *)el->unk1C, (s32)&tmp, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, e), 0);
            }
        }
    }
}

#include "ff9.h"

void func_80021938(PreloadEntry *e) {
    func_80021974(e, 0, -1, 0, 0, 0, -1, -1);
}

void func_80021974(PreloadEntry *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 t;
    p->unk0 = 0;
    p->unk2 = a;
    p->fileId = b;
    p->unk8 = c;
    p->unkC = 0;
    t = d;
    p->unk10 = t / 2048 * 2048;
    p->unk14 = e;
    p->unk18 = t = f;
    p->unk1C = g;
    p->unk20 = 0;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk26 = 0;
}

void func_800219D8(Unk800219D8 *a, s16 b, s16 c, s16 d, s16 e) {
    a->unk20 = b;
    a->unk22 = c;
    a->unk24 = d;
    a->unk0 |= 0x20;
    a->unk26 = e;
}

s32 func_800219FC(PreloadEntry *a) {
    s32 r = 0;
    if (a != NULL) {
        r = a->fileId != -1;
    }
    return r;
}

s32 func_80021A1C(PreloadEntry *a) {
    return a->unk0 & 1;
}

s32 func_80021A28(PreloadEntry *p) {
    u16 f;
    if (p->unk2 == 0) {
        return func_80021A8C(p);
    }
    if (p->unk2 != 1) {
        return 0;
    }
    f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        return 1;
    }
    return 0;
}

s32 func_80021A8C(PreloadEntry *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1) && p->unk2 == 0 && !(f & 0x20) && p->unk8 != 0) {
        return 1;
    }
    return 0;
}

s32 func_80021AE0(PreloadEntry *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        if ((f & 0x20) && p->unk24 != 0 && p->unk26 != 0) {
            return 1;
        }
    }
    return 0;
}

PreloadEntry *func_80021B34(PreloadEntry *dst, PreloadEntry *src) {
    s32 len;
    s32 t;

    if (func_80021A8C(src)) {
        func_80021938(dst);
    } else {
        func_80021938(dst);
        dst->unk2 = src->unk2;
        dst->fileId = src->fileId;
        t = src->unk8;
        t += src->unkC;
        dst->unk8 = t;
        len = src->unk8 + src->unk10 - t;
        dst->unk10 = len;
        t = src->unk14;
        if (len < t) {
            t = len;
        }
        dst->unk14 = t;
        dst->unk14 = t / 2048 * 2048;
        t = dst->unk14;
        dst->unk18 = src->unk18 + (src->unkC >> 11);
        dst->unk1C = dst->unk14;
    }
    return dst;
}

s32 func_80021C20(PreloadEntry *e) {
    s32 r = 0;
    if (e->unk0 & 1) {
        while (func_80022D3C(NULL) == 0) {
        }
        r = 1;
    }
    return r;
}

void preload_init() {
    s32 i;
    PreloadQueue *p;

    p = g_gameState->unk1C->unk8C8;
    p->state = 4;
    p->unk0 = 0;
    p->count = 0;
    p->cur = -1;
    for (i = 5; i >= 0; i--) {
        func_80021938(&p->unk4[i]);
    }
    func_80021938(&p->unk4[6]);
    p->unk11C = 0;
}

void func_80021CDC(s32 a) {
    g_gameState->unk1C->unk8C8->unk11C = a;
}

PreloadEntry *preload_add(void) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    PreloadEntry *e = NULL;
    if (t->count < 6) {
        e = &t->unk4[t->count];
        func_80021938(e);
        t->count++;
    }
    return e;
}

PreloadEntry *func_80021D7C(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    while (p->cur < p->count - 1) {
        p->cur++;
        e = &p->unk4[p->cur];
        if (e->fileId != -1) {
            e->unk0 |= 0x40;
            return e;
        }
    }
    return NULL;
}

PreloadEntry *func_80021E20(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    return &p->unk4[p->cur];
}

PreloadEntry *preload_find(s32 id) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    s32 i;
    id &= 0xFFFFFF;
    for (i = t->count - 1; i >= 0; i--) {
        PreloadEntry *e = &t->unk4[i];
        if ((e->fileId & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

s32 func_80021ED4(void) {
    PreloadEntry *e = &g_gameState->unk1C->unk8C8->unk4[6];
    PreloadEntry *p = func_80021E20();
    u8 r;
    s32 ret;

    if (p == NULL) {
        return 0;
    }
    do {
        if (!func_800219FC(p)) {
            return -1;
        }
        if (p->unk2 == 0) {
            r = cd_readAsync(g_gameState->unk20 + e->unk18, e->unk1C, (void *)e->unk8, preload_onReadDone);
        } else if (p->unk2 == 1) {
            r = cd_seekAsync(g_gameState->unk20 + e->unk18, preload_onReadDone);
        } else {
            goto done;
        }
        if (r == 0) {
            return -1;
        }
    done:
        p->unk0 |= 1;
        ret = 0;
    } while (0);
    return ret;
}

s32 func_80021FB4(void) {
    PreloadEntry *p = func_80021E20();
    s32 t;

    if (p == NULL) {
        return 0;
    }
    if (func_800219FC(p) == 0) {
        return -1;
    }
    if (p->unk0 & 1) {
        if (p->unk0 & 0x80) {
            return -1;
        }
        t = p->unk0 & 2;
        return t == 0;
    }
    return 0;
}

void func_80022020(void) {
    while (func_80021FB4() > 0) {
    }
    func_80022050();
}

void func_80022050(void) {
    PreloadEntry *e = func_80021E20();
    if (e != NULL) {
        e->unk0 &= 0xFF7E;
    }
}

PreloadEntry *func_8002208C(void) {
    PreloadEntry *p = func_80021E20();
    if (func_800219FC(p) == 0) {
        return NULL;
    }
    if (func_80021A1C(p) != 0) {
        func_80021C20(p);
    }
    while (func_80021FB4() > 0) {
    }
    return p;
}

s32 func_800220E8(s32 id) {
    s32 r = 0;
    PreloadEntry *p = preload_find(id);

    if (p != NULL) {
        if (func_80021A1C(p)) {
            func_80022020();
        }
        if (func_80021AE0(p) || func_80021A8C(p)) {
            r = 1;
        }
    } else {
        p = func_80021E20();
        if (p != NULL && func_800219FC(p)) {
            if (((p->fileId >> 16) & 0xFF) == ((id >> 16) & 0xFF) && (p->fileId & 0xFFFF) != (id & 0xFFFF)) {
                func_80021C20(p);
            }
        }
    }
    return r;
}

s32 func_800221C0(u32 addr, s32 size) {
    PreloadQueue *c = g_gameState->unk1C->unk8C8;
    s32 i;
    PreloadEntry *e;
    u32 end;
    s32 n;

    end = addr + size;
    for (i = c->count - 1; i >= 0; i--) {
        e = &c->unk4[i];
        if (e->fileId != -1 && e->unk2 == 0) {
            if (e->unk1C < e->unk10) {
                n = e->unk1C;
            } else {
                n = e->unk10;
            }
            if (e->unk8 < end && addr < e->unk8 + n) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80022280(u32 addr, s32 size) {
    PreloadQueue *q = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;
    s32 ret = 0;
    s32 i;
    u32 end = addr + size;

    for (i = q->count - 1; i >= 0; i--) {
        e = &q->unk4[i];
        if (e->fileId != -1 && (*(u32 *)&e->unk0 & 0x60) == 0x40 && e->unk8 != 0 && e->unkC != 0 &&
            e->unk8 < end && addr < e->unk8 + e->unkC) {
            if (func_80021C20(e)) {
                ret = 1;
            }
            func_80021938(e);
        }
    }
    return ret;
}

void func_80022390(s32 id) {
    PreloadEntry *e = preload_find(id);
    if (e != NULL) {
        func_80021C20(e);
        func_80021938(e);
    }
}

void func_800223CC(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    s32 i;

    for (i = p->count - 1; i >= 0; i--) {
        func_80021C20(&p->unk4[i]);
        func_80021938(&p->unk4[i]);
    }
    p->state = 4;
    p->count = 0;
    p->cur = -1;
}

s32 preload_update(void) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    if (t->unk11C != NULL && t->unk11C(t) != 0) {
        return 1;
    }
    for (;;) {
        switch (t->state) {
        case 4:
            if (func_80021D7C() != NULL) {
                t->state = 5;
            } else {
                t->state = 0;
            }
            break;
        case 5:
            e = func_80021E20();
            if (func_800219FC(e) == 0 || func_80021A28(e) != 0) {
                t->state = 4;
                break;
            }
            func_80021B34(&t->unk4[6], e);
            t->state = 8;
            break;
        case 8:
            if (cd_getState() != 0) {
                return 0;
            }
            t->state = 9;
            return 0;
        case 9:
            if (func_80021ED4() == -1) {
                t->state = 5;
                return 0;
            }
            t->state = 10;
            return 0;
        case 10:
            if (func_80021FB4() > 0) {
                return 0;
            }
            func_80022050();
            t->state = 5;
            return 0;
        case 0:
        default:
            return 0;
        }
    }
}

void preload_onReadDone(u8 *arg0) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    if (p != NULL) {
        e = func_80021E20();
        if (e != NULL) {
            if (func_800219FC(e) && *arg0 == 0) {
                PreloadEntry *x = &p->unk4[6];
                if (e->unk2 == 0) {
                    e->unkC += x->unk1C;
                }
                e->unk0 |= 2;
            } else {
                e->unk0 |= 0x80;
            }
        }
    }
}

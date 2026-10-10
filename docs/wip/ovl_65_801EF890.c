/* STATUS: draft, 102 rows (mostly s-register naming). ovl_65 func_801EF890 (0x308, battle-result module setup: load the result overlays/data with func_80021974). Logic and call arguments right; the end-of-RAM constant has to stay in a local (k = -0x146F00) or gcc folds it into D_80067944 (the original keeps 0xFFEB9100 in s5). Left: register assignment (original s0 = the constant 1 for both b->unk6 == 1 tests and later the rounded size, s1 = size, s2 = f/sector, s3 = id; ours s0 = size, s1 = f, s2 = id, s3 = 1/at) and the first if/else layout (original: bne, j). Permuter (15 min) found nothing. */
#include "ovl_65.h"

void func_801EF890(void) {
    Unk11C0 *b;
    FileEntry *f;
    GameData *g;
    s32 id;
    s32 size;
    s32 at;
    u8 *dst;
    s32 sector;
    s32 k;

    b = D_8006794C->unk1C->unk8E4.ovl65->unk11C0;
    func_80021C60();
    D_8006794C->unk1C->unk8BC = NULL;
    if (D_8006794C->unk1C->unk56E == 6) {
        return;
    }
    id = 0x1000C;
    if (b->unk6 == 1) {
        id = 0x1000A;
    }
    f = func_8001E218(0, id);
    dst = b->unk1C;
    size = (f->unkC - f->unk4) << 11;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, size, size, f->unk4, size);
    dst += size;
    if (b->unk6 == 1) {
        id = 0x2714;
    } else {
        id = 0x2712;
        if (D_8007AEF0.h[0] >= 0x2B52) {
            id = 0x2716;
        }
    }
    f = func_8001E218(1, id);
    k = -0x146F00;
    size = (f->unkC - f->unk4) << 11;
    at = (D_80067944 - (dst + k)) / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, f->unk4, size);
    dst += size;
    if (b->unk6 != 1) {
        return;
    }
    g = D_8006794C->unk1C;
    sector = g->unk7FC[0];
    id = g->unk3A | 0x60000;
    size = g->unk804[0] << 11;
    at = (D_80067944 - (dst + k)) / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, sector, size);
    dst += size;
    g = D_8006794C->unk1C;
    sector = g->unk7FC[1];
    id = g->unkC | 0x50000;
    size = g->unk804[1] << 11;
    at = (D_80067944 - (dst + k)) / 0x800 * 0x800;
    func_80021974(func_80021CFC(), 0, id, (s32)dst, at, size, sector, size);
    g = D_8006794C->unk1C;
    if ((s16)g->unkC == g->unk7EA && g->unk8AC != -1) {
        sector = g->unk8AC;
        id = g->unk7E8 | 0x70000;
        func_80021974(func_80021CFC(), 1, id, 0, 0, 0, sector, -1);
    }
}

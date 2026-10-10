/* STATUS: near miss, 7 rows (was 8). ovl_3c func_801F5BCC (0x148, buy list row). y = v->unk2 + row * 15 + 0x26 gets the add order closest; still v0/v1 swapped and 0x8A loaded before the second ternary in the original. */
#include "ovl_3c.h"

void func_801F5BCC(s32 unused, s32 row, s32 idx, MenuView *v) {
    u8 buf[16];
    Unk80069018 *it;
    s32 y;
    u8 id;
    u8 ok;

    ok = 0;
    id = D_801FC2F8.unk3C[idx];
    it = &D_80069018[id];
    if (func_80027F34(id) < 99) {
        ok = D_8006794C->unk1C->unk560 >= it->unk4;
    }
    y = v->unk2 + row * 15 + 0x26;
    func_8002BAE4(it->unk4, buf, 5, 1);
    func_80026C34(0x18, y, 0x1E, id, ok);
    func_8002B5FC(0x28, y, 0x1E, D_8006A218 + it->unk0, ok ? 0 : 2);
    func_8002B56C(0x8A, y + 4, 0x1E, buf, ok ? 0 : 2);
}

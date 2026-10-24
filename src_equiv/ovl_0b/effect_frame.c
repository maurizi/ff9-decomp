/* ovl_0b btlfx_runFrame (0x800E3EEC), hand-written asm in the original: the battle-effect frame update
 * btl_drawScene calls with its packet pointer.
 *
 * Register convention: standard C ABI, except that it runs btlfx_updateLoader, func_800E3C98, func_80054B78
 * and func_800E9578 on a stack at the top of the scratchpad (0x1F8003F8), saving $sp in g_btlfxSavedSp, and
 * btlfx_updateTasks back on the caller's stack. The C below leaves the stack alone (a port has no 1 KB
 * scratchpad stack to switch to) and so never writes g_btlfxSavedSp.
 *
 * The frame: the effects' packets start at `out` (g_btlfxEnv->unk24), bracketed by a DR_STP pair at the
 * back of the OT (entry 0xFFF): mask-bit setting on before them, off after. The btlfx_rand seed (scratchpad
 * 0x1F8003FC) is loaded from g_btlfxRandSeed for the frame and saved back after it, so the effects draw
 * from their own random stream.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py; g_btlfxSavedSp ignored).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

u8 *btlfx_runFrame(u8 *out) {
    u8 *stp;

    D_800F7278 = 0;
    g_btlfxEnv->unk24 = out;
    func_800D0940();
    func_800E702C();
    stp = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = stp + 0x18;
    SetDrawStp((DR_STP *)stp, 1);
    AddPrim((u8 *)g_btlfxEnv->unk20 + 0x3FFC, stp);
    *(u32 *)(SCRATCHPAD + 0x3FC) = g_btlfxRandSeed;
    g_btlfxEnv->unk0->unk20 = 0;
    btlfx_runScript();
    btlfx_updateLoader(); /* scratchpad stack */
    if (g_btlfxEnv->unk0->unk20 == 0) {
        if (D_800F78AC != 0) {
            D_800F78AC = 0;
            akao_sendCmd98(2);
        }
        btlfx_updateTasks(); /* caller's stack */
        func_800E3C98();     /* scratchpad stack */
        func_80054B78();
    } else {
        func_800E3E08();
        g_btlfxEnv->unk2C->unk5D = 1;
    }
    func_800E9578();
    stp += 0xC;
    g_btlfxRandSeed = *(u32 *)(SCRATCHPAD + 0x3FC);
    SetDrawStp((DR_STP *)stp, 0);
    AddPrim((u8 *)g_btlfxEnv->unk20 + 0x3FFC, stp);
    return g_btlfxEnv->unk24;
}

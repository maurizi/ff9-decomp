#include "ff9.h"

void func_8004BBF0(void) {
    func_80051568();
    func_8004E924();
    func_8004E7E0();
    rand_init(GetRCnt(2));
    func_8004AC80();
}

s32 func_8004BC38(a)
u8 a;
{
    s32 r = 0;
    if (D_8007B718 == 0) {
        func_8004E7FC();
        func_8004883C();
        r = func_80048F54();
        func_8004FBD8();
        func_8004FE0C();
    }
    return r;
}

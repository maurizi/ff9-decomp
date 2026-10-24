#include "ff9.h"

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadSetAct);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadSetActAlign);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_80019854);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001987C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadSetMainMode);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_800199E4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_80019A3C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadGetState);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadInfoMode);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadStartCom);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadStopCom);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", PadInitDirect);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padInitDirPort);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_80019F28);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_80019F90);

void func_8001A084(Unk8001A084 *a) {
    a->unk38 = a->unk37;
    a->unk37 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A094);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A150);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A3CC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A3EC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A4FC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _dirFailAuto);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001A848);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", bzero);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padIsVsync);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padIntPad);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padInitSioMode);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001ADB4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padSioRW);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padSioRW2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padClrIntSio0);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padWaitRXready);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padSetCmd);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padSendAtLoadInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padRecvAtLoadInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padGetActSize);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padLoadActInfo);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001B80C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001B8B4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padCmdParaMode);

void func_8001BCCC(Unk8001A084 *a) {
    a->unk37 = 0x45;
    a->unk2C = 0;
    a->unk36 = 0;
}

void func_8001BCE0(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x4C;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD00(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x46;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD20(Unk8001A084 *a, u8 b) {
    a->unk37 = 0x47;
    a->unk2C = (s32)&a->unk24;
    a->unk24 = b;
    a->unk36 = 1;
}

void func_8001BD40(Unk8001A084 *a) {
    a->unk37 = 0x4B;
    a->unk2C = 0;
    a->unk36 = 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padSetRC2wait);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", _padChkRC2wait);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001BE14);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001BE5C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001BF34);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001BFE0);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80019798", func_8001C0FC);

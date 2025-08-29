#ifndef _PATCH_H_
#define _PATCH_H_

#include <stdint.h>

enum instruction {
    NOP = 0x90,
    MOVIMM8 = 0xB0,
    JMP8 = 0xEB, // jump with 8 bit relative address
    JMP = 0xE9,
    CALL = 0xE8 // NOTE: relative
};

void patchNop(void *addr, size_t size);
void patchInst(void *addr, enum instruction inst);
void patchByte(void *addr, uint8_t val);
void patchBytes(void* addr, uint8_t* b, size_t sz);
void patchDWord(void *addr, uint32_t val);
void patchFloat(void *addr, float val);
void patchCall(void *addr, void *func);
void patchJmp(void *addr, void *func);
void patchThisToCdecl(void *addr, void *func);
void callFunc(void *addr);

#endif
#include <patch.h>

#include <stdint.h>
#include <windows.h>

void patchNop(void *addr, size_t size) {
    DWORD oldProtect;

    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(addr, NOP, size);
    VirtualProtect(addr, size, oldProtect, &oldProtect);
}

void patchInst(void *addr, enum instruction inst) {
    DWORD oldProtect;

    VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(uint8_t *)addr = inst;
    VirtualProtect(addr, 1, oldProtect, &oldProtect);
}

void patchByte(void* addr, uint8_t val) {
    DWORD oldProtect;

    VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(uint8_t *)addr = val;
    VirtualProtect(addr, 1, oldProtect, &oldProtect);
}

void patchDWord(void *addr, uint32_t val) {
    DWORD oldProtect;

    VirtualProtect(addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(uint32_t *)addr = val;
    VirtualProtect(addr, 4, oldProtect, &oldProtect);
}

void patchFloat(void *addr, float val) {
    DWORD oldProtect;

    VirtualProtect(addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(float *)addr = val;
    VirtualProtect(addr, 4, oldProtect, &oldProtect);
}

void patchCall(void *addr, void *func) {
    DWORD oldProtect;

    VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(uint8_t *)addr = CALL;
    *(uint32_t *)((uint8_t *)addr + 1) = (uint32_t)func - (uint32_t)addr - 5;
    VirtualProtect(addr, 5, oldProtect, &oldProtect);
}

void patchJmp(void *addr, void *func) {
    DWORD oldProtect;

    VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(uint8_t *)addr = JMP;
    *(uint32_t *)((uint8_t *)addr + 1) = (uint32_t)func - (uint32_t)addr - 5;
    VirtualProtect(addr, 5, oldProtect, &oldProtect);
}

void patchThisToCdecl(void *addr, void *func) {
    // convenience function that takes 'this' and calls a c function with a pointer to it - results in 7 bytes written
    DWORD oldProtect;

    VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    // PUSH ECX (0x51)
    *((uint8_t *)addr) = 0x51;

    // CALL (func)
    *((uint8_t *)addr + 1) = CALL;
    *(uint32_t *)((uint8_t *)addr + 2) = (uint32_t)func - (uint32_t)((uint8_t *)addr + 1) - 5;
    
    // POP ECX (0x59)
    *((uint8_t *)addr + 6) = 0x59;

    VirtualProtect(addr, 1, oldProtect, &oldProtect);
}

void callFunc(void *addr) {
    // calls function with the assumption of it returning and accepting void
    void (*fp)() = addr;
    fp();
}

struct parsedpattern {
    uint32_t len;
    uint8_t pattern[64];
    uint64_t mask;
};

uint8_t valueForChar(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 0xa;
    }
    else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 0xa;
    }

    return 0;
}

uint8_t parsePatternString(char* patternStr, struct parsedpattern* out) {
    out->len = 0;
    out->mask = 0L;

    uint8_t expectingCharacter = 0;
    for (char* i = patternStr; i < patternStr + strlen(patternStr); i++) {
        if (*i == ' ') {
            if (expectingCharacter) {
                // invalid character
                goto failure;
            }
        }
        else if ((*i >= '0' && *i <= '9') || (*i >= 'A' && *i <= 'F') || (*i >= 'a' && *i <= 'f')) {
            if (expectingCharacter & 1) {
                expectingCharacter = 0;
                out->pattern[out->len - 1] |= valueForChar(*i);
            }
            else if (expectingCharacter & 2) {
                // expecting question mark.  invalid character
                goto failure;
            }
            else {
                expectingCharacter = 1;
                out->len++;
                out->pattern[out->len - 1] = valueForChar(*i) << 4;
                out->mask |= 1L << out->len - 1;
            }
        }
        else if (*i == '?') {
            if (expectingCharacter & 1) {
                // expecting hex value. invalid character
                goto failure;
            }
            else if (expectingCharacter & 2) {
                expectingCharacter = 0;
                out->pattern[out->len - 1] = 0x00;
            }
            else {
                expectingCharacter = 2;
                out->len++;
            }
        }
        else {
            // invalid character
            goto failure;
        }
    }

    return 1;

failure:

    out->len = 0;
    return 0;
}

uint8_t findPattern(char* patternStr, void* startAddr, uint32_t len, uint32_t* addrOut) {
    struct parsedpattern pattern;
    if (!parsePatternString(patternStr, &pattern)) {
        return 0;
    }

    for (uint8_t* i = startAddr; i < (uint8_t*)startAddr + len - pattern.len; i++) {
        uint8_t found = 0;
        for (int j = 0; j < pattern.len; j++) {
            if (1L & pattern.mask >> j && *(i + j) != pattern.pattern[j]) {
                break;
            }
            if (j == pattern.len - 1) {
                found = 1;
            }
        }
        if (found) {
            *addrOut = i;
            return 1;
        }
    }
    return 0;
}

#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

namespace MemoryUtils {
    // Get the base address of a loaded library
    uintptr_t get_base_address(const char* name) {
        uintptr_t base = 0;
        char line[512];
        FILE* f = fopen("/proc/self/maps", "r");
        if (!f) return 0;

        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, name)) {
                base = strtoull(line, NULL, 16);
                break;
            }
        }
        fclose(f);
        return base;
    }

    // Patch memory at a specific address
    bool patch(uintptr_t address, const uint8_t* data, size_t size) {
        size_t page_size = sysconf(_SC_PAGESIZE);
        uintptr_t page_start = address & ~(page_size - 1);

        if (mprotect((void*)page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
            return false;
        }

        memcpy((void*)address, data, size);

        mprotect((void*)page_start, page_size, PROT_READ | PROT_EXEC);
        return true;
    }
}

#endif // MEMORY_UTILS_H

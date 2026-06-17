#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace MemoryUtils {
    // Get the base address of a loaded library
    inline uintptr_t get_base_address(const char* name) {
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

    // Find a pattern in a specific module
    inline uintptr_t find_pattern(const char* module, const char* pattern) {
        uintptr_t base = get_base_address(module);
        if (!base) return 0;

        // Get module size from /proc/self/maps
        size_t size = 0;
        char line[512];
        FILE* f = fopen("/proc/self/maps", "r");
        if (f) {
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, module)) {
                    uintptr_t start, end;
                    sscanf(line, "%lx-%lx", &start, &end);
                    size = end - start;
                    break;
                }
            }
            fclose(f);
        }

        if (!size) return 0;

        auto parse_pattern = [](const char* pattern, std::vector<int>& bytes) {
            char* start = const_cast<char*>(pattern);
            char* end = const_cast<char*>(pattern) + strlen(pattern);
            for (char* current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?') ++current;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
        };

        std::vector<int> pattern_bytes;
        parse_pattern(pattern, pattern_bytes);

        uint8_t* scan_start = reinterpret_cast<uint8_t*>(base);
        size_t pattern_size = pattern_bytes.size();

        for (size_t i = 0; i < size - pattern_size; ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern_size; ++j) {
                if (pattern_bytes[j] != -1 && scan_start[i + j] != pattern_bytes[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return reinterpret_cast<uintptr_t>(&scan_start[i]);
        }

        return 0;
    }

    // Resolve ADRP + ADD/LDR (ARM64)
    inline uintptr_t get_absolute_address(uintptr_t pc, int32_t adrp_ins, int32_t add_ldr_ins) {
        uintptr_t base = pc & ~0xFFF;

        // Extract ADRP immediate
        int64_t adrp_imm = ((adrp_ins >> 5) & 0x7FFFF) << 2;
        adrp_imm |= (adrp_ins >> 29) & 0x3;
        if (adrp_ins & 0x800000) adrp_imm |= 0xFFFFFFFFFFF80000ULL;

        uintptr_t page = base + (adrp_imm << 12);

        // Extract ADD/LDR offset
        int32_t offset = 0;
        if ((add_ldr_ins & 0xFF000000) == 0x91000000) { // ADD
            offset = (add_ldr_ins >> 10) & 0xFFF;
        } else if ((add_ldr_ins & 0xFFC00000) == 0xF9400000) { // LDR 64-bit
            offset = ((add_ldr_ins >> 10) & 0xFFF) << 3;
        }

        return page + offset;
    }
}

#endif // MEMORY_UTILS_H

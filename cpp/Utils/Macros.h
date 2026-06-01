#ifndef MACROS_H
#define MACROS_H

// Helper to define hooks if using Dobby or similar
#define HOOK_DEF(ret, name, ...) \
    ret (*orig_##name)(__VA_ARGS__); \
    ret hook_##name(__VA_ARGS__)

#endif // MACROS_H

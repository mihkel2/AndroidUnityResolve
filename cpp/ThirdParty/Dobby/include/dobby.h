#ifndef DOBBY_H
#define DOBBY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// This is a placeholder header for Dobby.
// In a real project, you would replace this and the .a files with the actual Dobby release.

int DobbyHook(void *address, void *replace_call, void **origin_call);
int DobbyDestroy(void *address);

#ifdef __cplusplus
}
#endif

#endif

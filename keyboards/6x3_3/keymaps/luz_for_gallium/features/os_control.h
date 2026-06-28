#pragma once

#include <stdint.h>

/*
 * OS Control - Operating System Platform Management
 *
 * Centralized OS platform tracking for cross-platform keyboard features.
 * Features like semantic keys can query the current OS to send the correct
 * keycodes (e.g., Cmd on Mac, Ctrl on Linux).
 */

// Supported operating systems
typedef enum {
    OS_MacOS = 0,
    OS_Linux,
    _OS_COUNT  // Total number of supported OSes
} os_platform_t;

// Get the current OS platform
uint8_t get_os_platform(void);

// Get the current OS platform name as a string
const char* get_os_platform_name(void);

// Toggle to the next OS platform (cycles through all supported OSes)
void toggle_os_platform(void);

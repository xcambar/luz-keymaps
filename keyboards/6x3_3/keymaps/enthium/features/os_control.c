/*
 * OS Control - Operating System Platform Management
 *
 * Centralized OS platform tracking for cross-platform keyboard features.
 */

#include "os_control.h"

// Current OS platform (defaults to Linux)
static uint8_t current_os = OS_Linux;

// OS platform names
static const char* os_names[] = {
    [OS_MacOS] = "MacOS",
    [OS_Linux] = "Linux",
};

// Get the current OS platform
uint8_t get_os_platform(void) {
    return current_os;
}

// Get the current OS platform name as a string
const char* get_os_platform_name(void) {
    if (current_os < _OS_COUNT) {
        return os_names[current_os];
    }
    return "Unknown";
}

// Toggle to the next OS platform (cycles through all supported OSes)
void toggle_os_platform(void) {
    current_os = (current_os + 1) % _OS_COUNT;
}

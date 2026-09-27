// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Host OS: which operating system the keyboard is talking to, as a setting the other Luz
// modules read (semantic_keys, dead_keys, cmd_ctrl_morph) to pick OS-specific chords. It is
// a setting, not detection: HOST_TOG cycles it, and it starts at HOST_OS_DEFAULT.

#pragma once

#include <stdint.h>

typedef enum {
    HOST_OS_MACOS = 0,
    HOST_OS_LINUX,
    HOST_OS_COUNT
} host_os_t;

host_os_t   host_os_get(void);
void        host_os_set(host_os_t os);
void        host_os_toggle(void);   // cycles through every host_os_t
const char *host_os_name(void);     // "MacOS" / "Linux"

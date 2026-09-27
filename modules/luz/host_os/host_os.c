// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "host_os.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#ifndef HOST_OS_DEFAULT
#    define HOST_OS_DEFAULT HOST_OS_LINUX
#endif

static host_os_t current_os = HOST_OS_DEFAULT;

static const char *const os_names[HOST_OS_COUNT] = {
    [HOST_OS_MACOS] = "MacOS",
    [HOST_OS_LINUX] = "Linux",
};

host_os_t host_os_get(void) {
    return current_os;
}

void host_os_set(host_os_t os) {
    if (os < HOST_OS_COUNT) current_os = os;
}

void host_os_toggle(void) {
    current_os = (current_os + 1) % HOST_OS_COUNT;
}

const char *host_os_name(void) {
    return os_names[current_os];
}

bool process_record_host_os(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_host_os_kb(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case COMMUNITY_MODULE_HOST_OS_TOGGLE:
            if (record->event.pressed) host_os_toggle();
            return false;
        case COMMUNITY_MODULE_HOST_OS_PRINT:
            if (record->event.pressed) send_string(host_os_name());
            return false;
    }
    return true;
}

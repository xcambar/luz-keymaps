// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Luz Compose: armed by compose_arm() (Luz calls it from a Shift+Space combo), consumed by
// the next key, which picks by letter, not position:
//
//     E acute   A grave   U diaeresis   O circumflex   N tilde    (dead keys: the accent
//     C ç       W €                                                 lands on the next letter)
//     Esc cancels. Plain modifiers don't consume it (Shift+E still composes).
//     Any other key disarms it and does what it normally does.
//
// Requires luz/dead_keys and luz/semantic_keys, and must be listed BEFORE them in
// keymap.json so it sees the next key first.

#pragma once

#include <stdbool.h>

void compose_arm(void);
bool compose_is_armed(void);

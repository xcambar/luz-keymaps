#pragma once

#include "quantum.h"

// Token concatenation helper macros for custom keycodes
#define X_CONCAT_IMPL(a, b) a ## b
#define X_CONCAT(a, b) X_CONCAT_IMPL(a, b)

#define LIST_START_MARKER(PREFIX) X_CONCAT(X_CONCAT(_, PREFIX), _START)
#define LIST_LENGTH(PREFIX) X_CONCAT(X_CONCAT(_, PREFIX), _COUNT)

#define CUSTOM_KEYCODES(PREFIX, ...) \
    LIST_START_MARKER(PREFIX), \
    X_FIRST(__VA_ARGS__), \
    X_REST(__VA_ARGS__), \
    LIST_LENGTH(PREFIX)

// Helper macros to extract first and rest of arguments
#define X_FIRST(first, ...) first
#define X_REST(first, ...) __VA_ARGS__

#include "features/semantic_keys.h"
#include "features/dead_keys.h"
#include "features/alt_symbols.h"
#include "features/alt_symbols_layer.h"

// Custom keycodes for the xavier keymap
enum custom_keycodes {
    SW_WIN = SAFE_RANGE,     // Switch window (cmd-tab)
    MM_GUICTRL,              // Modifier swappable between GUI and Ctrl
    SEL_LATCH,               // Shift latch scoped to the FAVS layer (tap to toggle)
    MD_FENCE,                // Markdown code fence: types ```
    ARROW_OP,                // Tap: ->  Shift: =>

    // Semantic keys (expanded from SEMANTIC_KEYS_LIST macro)
    SEMANTIC_KEYS_LIST,

    // Dead keys (expanded from DEAD_KEYS_LIST macro)
    DEAD_KEYS_LIST,

#ifdef XC_ALT_BASE_SYMBOLS
    // Alternative symbols (expanded from ALT_SYMBOLS_LIST macro)
    ALT_SYMBOLS_LIST,
#endif

    // Alternative symbols layer (expanded from ALT_SYMBOLS_LAYER_LIST macro)
    ALT_SYMBOLS_LAYER_LIST,
};

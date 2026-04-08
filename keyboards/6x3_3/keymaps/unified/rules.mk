###################
# QMK OPTIONS
#

COMBO_ENABLE = yes
LAYER_LOCK_ENABLE = yes
LEADER_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
LTO_ENABLE = yes

MOUSEKEY_ENABLE = no
RGBLIGHT_ENABLE = no

# Crafted feature sources
SRC += features/os_control.c
SRC += features/oneshot.c
SRC += features/swapper.c
SRC += features/mod_morph.c
SRC += features/semantic_keys.c
SRC += features/dead_keys.c
SRC += features/alt_symbols.c
SRC += features/alt_symbols_layer.c

###################
# Custom options
#

# Layout selection: qwerty, gallium, focal, graphite, or radial
XC_LAYOUT ?= qwerty

# Secondary (alternate base) layout: qwerty, gallium, focal, graphite, or gallium_east
XC_SECONDARY_LAYOUT ?= gallium_east

XC_WEAK_CORNERS = yes

# Alternative symbols for the base layer
XC_ALT_BASE_SYMBOLS ?= yes

###################
# Preprocessor defines
#

OPT_DEFS += -DXC_LAYOUT=$(XC_LAYOUT)
OPT_DEFS += -DXC_SECONDARY_LAYOUT=$(XC_SECONDARY_LAYOUT)

ifeq ($(strip $(XC_WEAK_CORNERS)), yes)
    OPT_DEFS += -DXC_WEAK_CORNERS
endif

ifeq ($(strip $(XC_ALT_BASE_SYMBOLS)), yes)
    OPT_DEFS += -DXC_ALT_BASE_SYMBOLS
endif

###################
# QMK OPTIONS
#

COMBO_ENABLE = yes
# Adjust combo timing if needed (default is 50ms)
# COMBO_TERM = 50

LAYER_LOCK_ENABLE = yes
LEADER_ENABLE = yes
#AUTO_SHIFT_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Callum-style oneshot modifiers and swapper
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
XC_LAYOUT ?=gallium_east

# Secondary (alternate base) layout: qwerty, gallium, focal, graphite, or gallium_east
XC_SECONDARY_LAYOUT ?= qwerty
XC_WEAK_CORNERS = yes

# Alternative symbols for the base layer
XC_ALT_BASE_SYMBOLS ?= yes
###################
# This manipulates the options
#

# Pass layout names to the preprocessor as lowercase tokens
OPT_DEFS += -DXC_LAYOUT=$(XC_LAYOUT)
OPT_DEFS += -DXC_SECONDARY_LAYOUT=$(XC_SECONDARY_LAYOUT)

ifeq ($(strip $(XC_WEAK_CORNERS)), yes)
    OPT_DEFS += -DXC_WEAK_CORNERS
endif

ifeq ($(strip $(XC_ALT_BASE_SYMBOLS)), yes)
    OPT_DEFS += -DXC_ALT_BASE_SYMBOLS
endif


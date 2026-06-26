###################
# QMK OPTIONS
#

COMBO_ENABLE = yes
# Adjust combo timing if needed (default is 50ms)
# COMBO_TERM = 50

LAYER_LOCK_ENABLE = yes
CAPS_WORD_ENABLE = yes
#AUTO_SHIFT_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Platform-aware features and swapper
SRC += features/os_control.c
SRC += features/swapper.c
SRC += features/semantic_keys.c
SRC += features/dead_keys.c

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

# Shared Luz headers live at <userspace>/keyboards/6x3_3/luz. Add that parent dir to the
# include path (absolute, via QMK_USERSPACE) so `#include "luz/..."` resolves regardless of
# where QMK lays out the keymap when building — a relative/symlinked path does not survive
# the userspace CI build layout.
VPATH += $(QMK_USERSPACE)/keyboards/6x3_3

# Platform-aware features and swapper
SRC += features/os_control.c
SRC += features/swapper.c
SRC += features/semantic_keys.c
SRC += features/dead_keys.c

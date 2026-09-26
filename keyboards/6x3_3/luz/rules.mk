# Shared Luz build options. Each variant's rules.mk is a one-line include of this file.

COMBO_ENABLE = yes
LAYER_LOCK_ENABLE = yes
CAPS_WORD_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Shared Luz sources live at <userspace>/keyboards/6x3_3/luz. Add that parent dir to the
# include path (absolute, via QMK_USERSPACE) so `#include "luz/..."` resolves regardless of
# where QMK lays out the keymap when building — a relative/symlinked path does not survive
# the userspace CI build layout.
LUZ_DIR := $(QMK_USERSPACE)/keyboards/6x3_3/luz
VPATH += $(QMK_USERSPACE)/keyboards/6x3_3

SRC += $(LUZ_DIR)/os_control.c
SRC += $(LUZ_DIR)/swapper.c
SRC += $(LUZ_DIR)/semantic_keys.c
SRC += $(LUZ_DIR)/dead_keys.c

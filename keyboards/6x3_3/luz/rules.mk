# Shared Luz build options. Each variant's rules.mk is a one-line include of this file.
# The OS-aware features are QMK community modules (modules/luz/*), listed in each
# variant's keymap.json.

COMBO_ENABLE = yes
LAYER_LOCK_ENABLE = yes
CAPS_WORD_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Shared Luz headers live at <userspace>/keyboards/6x3_3/luz. Add that parent dir to the
# include path (absolute, via QMK_USERSPACE) so `#include "luz/..."` resolves regardless of
# where QMK lays out the keymap when building — a relative/symlinked path does not survive
# the userspace CI build layout.
VPATH += $(QMK_USERSPACE)/keyboards/6x3_3

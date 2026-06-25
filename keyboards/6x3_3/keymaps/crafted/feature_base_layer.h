// XC_LAYOUT is a lowercase layout name token passed from rules.mk (e.g., qwerty, graphite)

// Include path lookup (token → quoted path string)
#define _LAYOUT_FILE_qwerty   "layouts/qwerty.h"
#define _LAYOUT_FILE_gallium  "layouts/gallium.h"
#define _LAYOUT_FILE_focal    "layouts/focal.h"
#define _LAYOUT_FILE_graphite "layouts/graphite.h"
#define _LAYOUT_FILE_gallium_east "layouts/gallium_east.h"
#define _LAYOUT_FILE_x(n)    _LAYOUT_FILE_##n
#define LAYOUT_FILE(n)        _LAYOUT_FILE_x(n)

#include LAYOUT_FILE(XC_LAYOUT)

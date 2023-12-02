#ifndef LANGS_HPP
#define LANGS_HPP

#include "htts_cfg.h"

enum class Lang {
#ifdef HTTS_LANG_EU
    eu,
#endif
#ifdef HTTS_LANG_ES
    es
#endif
};
const char * Lang2Str(Lang l);

#endif


#include "langs.hpp"

const char * Lang2Str(Lang l){
    switch( l ){
#ifdef HTTS_LANG_ES
        case Lang::es:
            return "es";
#endif
#ifdef HTTS_LANG_EU
        case Lang::eu:
            return "eu";
#endif
        default: return "";
    }
}

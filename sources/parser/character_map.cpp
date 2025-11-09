#include "parser/character_map.h"
#include <string.h>

using namespace csv_parser;

character_map_o::character_map_o() {
    p_map = new char[256];
    ::memset(p_map,TO_SELF,256);
    ::memset(p_map,TO_SPACE,32);
    p_map[','] = TO_DELIMITER;
    p_map['"'] = TO_QUOTE;
    p_map['\''] = TO_QUOTE;
    p_map['\n'] = TO_EOL;
    p_map['\r'] = TO_SKIP;
}

#pragma once

namespace csv_parser {

enum {
    TO_SELF = 0,
    TO_SPACE = 1,
    TO_EOL = 2,
    TO_SKIP = 3,
    TO_QUOTE = 4,
    TO_DELIMITER = 5
};

class character_map_o {
    char* p_map;

public:
    character_map_o();
    char* map_get() {
        return p_map;
    }
};

}  // namespace csv_parser
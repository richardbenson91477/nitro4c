#include "../enums_map.h"

#include "enums_map_1.h"
#include "enums_map_1_enums_map.h"

int main (void) {
    dlog_debug_u (_, "enums_map_get_id (., \"ID_TWO\")", 
        enums_map_get_id (_enums_map_1_enums_map, "ID_TWO"));

    dlog_debug_s (_, "enums_map_get_s (., 3)",
        enums_map_get_name (_enums_map_1_enums_map, 3));

    dlog_debug_s (_, "enums_map_get_s (., ID_FOUR)",
        enums_map_get_name (_enums_map_1_enums_map, ID_FOUR));

    return 0;
} 


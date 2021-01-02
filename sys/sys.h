#ifndef NITRO_SYS_INCLUDED
#define NITRO_SYS_INCLUDED

#include "../nitro_defs.h"
#include "../name_map/name_map.h"
#include "../comp/comp.h"
#include "../arc/arc.h"
 
enum {
// enums_map_begin
    SYS_CMD_END = 1,
    SYS_CMD_DEF_STATES,
    SYS_CMD_DEF_COMP,
    SYS_CMD_STATE,
    SYS_CMD_LOAD_MSG_TYPES,
    SYS_CMD_SEND_MSG,
    SYS_MSG_TYPE_END,
    SYS_MSG_TYPE_ID_OFFSET,
    SYS_MSG_TYPE_DEF,
    SYS_MSG_TYPE_ARG_TYPE_N,
    SYS_MSG_TYPE_ARG_TYPE_S,
    SYS_MSG_TYPE_ARG_TYPE_U,
    SYS_MSG_TYPE_ARG_TYPE_F,
    SYS_MSG_TYPE_ARG_TYPE_V,
    SYS_MSG_TYPE_ARG_TYPE_COMP_NAME,
    SYS_MSG_TYPE_ARG_TYPE_STATE_NAME,
// enums_map_end
};

typedef struct {
    uint32_t state_id;
    uint32_t sys_id;

    char *_arg_s;
    uint32_t arg_u;
    float arg_f;    
    float _arg_v [3];

    name_map_item *_type_item;
} sys_msg;

typedef struct {
    name_map *_parent_name_map;
    uint32_t id;

    blaze *_msgs;
    name_map *_name_map_smt;
    bool (* _ext_process_msg_fn)(void *_this, sys_msg *_msg);
    void *_ext_process_msg_this;
 
    blaze *_comps;
    name_map *_name_map_comp;

    bool state_ignore_;
    uint32_t state_cur, state_next;
    float state_t, state_len;
    name_map *_name_map_state;
} sys;

extern bool sys_cons (sys *_sys);
extern bool sys_decons (sys *_sys);
extern void sys_free (void *_v);
extern void sys_msg_free (void *_v);
extern bool sys_tick (sys *_sys, float t);
extern bool sys_set_state (sys *_sys, uint32_t state_id);
extern bool sys_process_msg (sys *_sys, sys_msg *_msg);
extern bool sys_load (sys *_sys, arc *_arc, char const *_name_s);
extern bool sys_msg_type_load (name_map *_name_map_smt, arc *_arc,
        char const *_name_s);

#endif


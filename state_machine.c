#include "state_machine.h"

static uint8_t CalculateDepth(MyState_t *state);

void InitState(MyState_t *state_out, my_fun state_fun, MyState_t *parent_state)
{
    state_out->parent = parent_state;
    state_out->depth = CalculateDepth(state_out);
    state_out->fun = state_fun;
}


static uint8_t CalculateDepth(MyState_t *state)
{
    uint8_t ret_val = 0;
    while(state->parent != NULL)
    {
        ret_val++;
        state = state->parent;
    }
    return ret_val;
}


MyState_t *find_LCA(MyState_t *src_state, MyState_t *dest_state, MyState_t **EntryPath, uint8_t *entry_path_size, MyState_t **ExitPath, uint8_t *exit_path_size)
{
    MyState_t *temp_src = src_state;
    MyState_t *temp_dest = dest_state;

    uint8_t exit_path_idx = 0;
    uint8_t entry_path_idx = 0;

    if(src_state->depth > dest_state->depth)
    {
        // source jest wieksze od dest przygotowujemy exit path
        uint8_t depth_diff = src_state->depth - dest_state->depth;
        for(uint8_t i = 0; i < depth_diff; i++)
        {
            ExitPath[exit_path_idx] = temp_src;
            exit_path_idx++;
            temp_src = temp_src->parent; 

        }

    }
    else if(src_state->depth < dest_state->depth)
    {
        uint8_t depth_diff = dest_state->depth - src_state->depth; 
        for(uint8_t i = 0; i < depth_diff; i++)
        {
            EntryPath[entry_path_idx] = temp_dest;
            entry_path_idx++;
            temp_dest = temp_dest->parent; 
        }
    }
    else{
        /* do nothing */
    }

    while(temp_dest != temp_src)
    {
        ExitPath[exit_path_idx] = temp_src;
        exit_path_idx++;
        EntryPath[entry_path_idx] = temp_dest;
        entry_path_idx++;
        temp_dest = temp_dest->parent;
        temp_src = temp_src->parent;
    }

    *entry_path_size = entry_path_idx;
    *exit_path_size = exit_path_idx;

    return temp_dest;
}

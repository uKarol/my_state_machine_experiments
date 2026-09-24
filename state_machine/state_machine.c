/**
 * @file state_machine.c
 * @brief file contains implementation of state machine with hierarchy 
 * @author Karol Ujda 
 * https://github.com/uKarol
 */

#include "state_machine.h"

static uint8_t CalculateDepth(MyState_t *state);

void InitState(MyState_t *state_out, state_handler state_fun, MyState_t *parent_state)
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

#define MAX_DEPTH 10

void StateMachineInitialize(MyStateMachine_t *ctx, MyState_t *initial_state)
{
    FsmEvent_t temp_evt = {ENTRY_EVT, NULL};
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t arr_write_idx = 0;
    MyState_t *temp_state = initial_state;
    while(temp_state != NULL)
    {
        entry_path[arr_write_idx] = temp_state;
        arr_write_idx++;
        temp_state = temp_state->parent;
    }

    for(uint8_t ctr = arr_write_idx-1; ctr != 0; ctr--)
    {
        entry_path[ctr]->fun(ctx, &temp_evt);
    }
    ctx->current_state = initial_state;

    ctx->current_state->fun(ctx, &temp_evt);

}

void StateMachine_ProcessEvent(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal state_ret;
    FsmEvent_t entry_evt = {ENTRY_EVT, NULL};
    FsmEvent_t exit_evt = {EXIT_EVT, NULL};
    MyState_t *entry_path[MAX_DEPTH];
    MyState_t *exit_path[MAX_DEPTH];
    uint8_t entry_path_size = 0;
    uint8_t exit_path_size = 0;

    MyState_t *temp_state = ctx->current_state;

    state_ret = temp_state->fun(ctx, evt);

    // state ignored means that current state does not handle the event, try to call superstate
    while(state_ret == STATE_IGNORED)
    {
        temp_state = temp_state->parent;
        if(temp_state == NULL) return; // it's an error
        state_ret = temp_state->fun(ctx, evt);
    }

    if(state_ret == STATE_TRANSITION)
    {
        find_LCA(temp_state, ctx->next_state, &entry_path, &entry_path_size, &exit_path, &exit_path_size);
        uint8_t depth_diff = ctx->current_state->depth - temp_state->depth;
        temp_state = ctx->current_state;
        printf("total path exit: %d\n", depth_diff + exit_path_size);
        for(uint8_t ctr = 0; ctr < depth_diff + exit_path_size; ctr++)
        {
            temp_state->fun(ctx, &exit_evt);
            temp_state = temp_state->parent;
        }
        for(uint8_t ctr = entry_path_size-1; ctr != 0; ctr--)
        {
            entry_path[ctr]->fun(ctx, &entry_evt);
        }
        ctx->current_state = ctx->next_state;
        ctx->current_state->fun(ctx, &entry_evt);
    }
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

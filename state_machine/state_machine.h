/**
 * @file state_machine.c
 * @brief header file contains declarations of state machine with hierarchy 
 * @author Karol Ujda 
 * https://github.com/uKarol
 */

#include <stdint.h>

typedef enum 
{
    STATE_HANDLED,
    STATE_IGNORED,
    STATE_TRANSITION,
}StateRetVal;

enum
{
    ENTRY_EVT = 0,
    EXIT_EVT,
    SPECIAL_EVT_END,
};

typedef struct
{
	uint16_t user_event;
	void *user_data;
}FsmEvent_t;

typedef struct MyState_t MyState_t;

typedef struct MyStateMachine_t MyStateMachine_t;

typedef StateRetVal (*state_handler) (MyStateMachine_t *ctx, FsmEvent_t *evt);

struct MyState_t
{
    state_handler fun;
    uint8_t idx;
    uint8_t depth;
    MyState_t *parent;
};


struct MyStateMachine_t
{
    MyState_t *current_state;
    MyState_t *next_state;
    void *context;
};


MyState_t *find_LCA(MyState_t *src_state, MyState_t *dest_state, MyState_t **EntryPath, uint8_t *entry_path_size, uint8_t *exit_path_size);
void InitState(MyState_t *state_out, state_handler state_fun, MyState_t *parent_state);
void StateMachine_ProcessEvent(MyStateMachine_t *ctx, FsmEvent_t *evt);
void StateMachineInitialize(MyStateMachine_t *ctx, MyState_t *initial_state);
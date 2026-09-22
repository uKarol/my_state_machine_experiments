#include <stdio.h>
#include <stdlib.h>

#include "state_machine.h"

// struct MyState_t
// {
//     my_fun fun;
//     uint8_t idx;
//     uint8_t depth;
//     MyState_t *parent;
// };

//test set 1
// MyState_t State1 = {NULL, 1, 0, NULL};
// MyState_t State2 = {NULL, 2, 1, &State1};
// MyState_t State3 = {NULL, 3, 1, &State1};
// MyState_t State4 = {NULL, 4, 2, &State2};
// MyState_t State5 = {NULL, 5, 2, &State2};
// MyState_t State6 = {NULL, 6, 3, &State4};
// MyState_t State7 = {NULL, 7, 3, &State5};
// MyState_t State8 = {NULL, 8, 4, &State6};


MyState_t S0;// = {NULL, 0, 0, NULL};
MyState_t S1;// = {NULL, 1, 1, &S0};
MyState_t S2;// = {NULL, 2, 1, &S0};
MyState_t S21;// = {NULL, 21, 2, &S2};
MyState_t S211;// = {NULL, 211, 3, &S21};
MyState_t S11;// = {NULL, 11, 2, &S1};
#define MAX_DEPTH 10

StateRetVal S0_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal S1_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal S2_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal S21_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal S211_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal S11_State(MyStateMachine_t *ctx, FsmEvent_t *evt);


StateRetVal S0_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S0 ENTRY\n");
        break;
    }
    return retval;
}

StateRetVal S1_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S1 ENTRY\n");
        break;
    }
    return retval;
}

StateRetVal S2_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S2 ENTRY\n");
        break;
    }
    return retval;
}

StateRetVal S21_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S21 ENTRY\n");
        break;
    }
    return retval;
}

StateRetVal S211_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S211 ENTRY\n");
        break;
    }
    return retval;
}

StateRetVal S11_State(MyStateMachine_t *ctx, FsmEvent_t *evt)
{
    StateRetVal retval = STATE_HANDLED;
    switch(evt->user_event)
    {
        case ENTRY_EVT:
            printf("S11 ENTRY\n");
        break;
    }
    return retval;
}


int main()
{

    InitState(&S0, S0_State, NULL);
    InitState(&S1, S1_State, &S0);
    InitState(&S2, S2_State, &S0);
    InitState(&S21, S21_State, &S2);
    InitState(&S211, S211_State, &S21);
    InitState(&S11, S11_State, &S1);

    printf("depth S0 %d\n", S0.depth);
    printf("depth S1 %d\n", S1.depth);
    printf("depth S2 %d\n", S2.depth);
    printf("depth S21 %d\n", S21.depth);
    printf("depth S211 %d\n", S211.depth);
    printf("depth S11 %d\n", S11.depth);

    MyStateMachine_t MyFSM;

    StateMachineInitialize(&MyFSM, &S211);
    printf("end");
    // S0.idx = 0;
    // S1.idx = 1;
    // S2.idx = 2;
    // S21.idx = 21;
    // S211.idx = 211;
    // S11.idx = 11;

    // uint8_t entry_path_size = 0;
    // uint8_t exit_path_size = 0;
    // MyState_t *entry_path[MAX_DEPTH];
    // MyState_t *exit_path[MAX_DEPTH];
    // MyState_t *result = find_LCA(&S11, &S211, &entry_path, &entry_path_size, &exit_path, &exit_path_size);
    // printf("THE LCA IS %d\n", result->idx);

    // printf("exiting order: ");
    // for(uint8_t i = 0; i < exit_path_size; i++)
    // {
    //     printf(" %d,", exit_path[i]->idx);
    // }

    // printf("\nentering order: ");
    // for(uint8_t i = entry_path_size-1; i != 0; i--)
    // {
    //     printf(" %d,", entry_path[i]->idx);
    // }

}
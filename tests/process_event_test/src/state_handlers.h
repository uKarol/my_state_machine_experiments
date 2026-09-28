#ifndef _STATE_HANDLERS_H_
#define _STATE_HANDLERS_H_

#include "state_machine.h"

StateRetVal ROOT_State(MyStateMachine_t *ctx, FsmEvent_t *evt);

StateRetVal A_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal B_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal C_State(MyStateMachine_t *ctx, FsmEvent_t *evt);

StateRetVal A1_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal A2_State(MyStateMachine_t *ctx, FsmEvent_t *evt);

StateRetVal A11_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal A111_State(MyStateMachine_t *ctx, FsmEvent_t *evt);

StateRetVal B1_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal B11_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal B12_State(MyStateMachine_t *ctx, FsmEvent_t *evt);
StateRetVal B121_State(MyStateMachine_t *ctx, FsmEvent_t *evt);

#endif
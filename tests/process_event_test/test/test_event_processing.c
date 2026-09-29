
#ifdef TEST

#include "unity.h"

#include "event_processing.h"
#include "state_handlers.h"
#include "mock_state_handlers.h"
#include "state_machine.h"

void setUp(void)
{
}

void tearDown(void)
{
}

typedef enum
{
    EVT1 = SPECIAL_EVT_END,
    EVT2,
    EVT3, 
};

MyState_t ROOT;
//substate of ROOT
MyState_t A;
MyState_t B;
MyState_t C;

//substate of A
MyState_t A1;
MyState_t A2;

//substates of A1
MyState_t A11;

//substates of A11
MyState_t A111;

//substates of B
MyState_t B1;

//substates of B1
MyState_t B11;
MyState_t B12;

//substates of B12
MyState_t B121;

MyStateMachine_t test_machine;

void test_StateInitialization(void)
{
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&ROOT, ROOT_State , NULL));
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&A, A_State, &ROOT));
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&B, B_State, &ROOT));
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&C, C_State, &ROOT));
    // substates of A
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&A1, A1_State, &A));
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&A2, A2_State, &A));
    // substates of A1
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&A11, A11_State, &A1));
    // substates of A11
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&A111, A111_State, &A11));
    
    // substates of B
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&B1, B1_State, &B));
    // substates of B1
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&B11, B11_State, &B1));
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&B12, B12_State, &B1));

    // substates of B1
    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&B121, B121_State, &B12));

    ROOT_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    ROOT_State_IgnoreArg_evt();

    A_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A_State_IgnoreArg_evt();

    A1_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A1_State_IgnoreArg_evt();

    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A11_State_IgnoreArg_evt();

    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A111_State_IgnoreArg_evt();

    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, StateMachineInitialize(&test_machine, &A111));

    TEST_ASSERT_EQUAL_PTR(&A111, test_machine.current_state);
}


StateRetVal Test01Callback_A111(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(EVT3, evt->user_event);
    return STATE_HANDLED;
}
/*
    Test 01
    State machine is in initial state A111, an event EVT3 is set and handled by that state
    No transition expected, no upper state should be called
*/
void test_Test01_StateProcessing_NoTransition_EvtHandled(void)
{
    A111_State_AddCallback(Test01Callback_A111);
    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
     A111_State_IgnoreArg_evt();
    FsmEvent_t test_evt = {EVT3, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL(&A111, test_machine.current_state);
}


StateRetVal Test02Callback_A111(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(EVT3, evt->user_event);
    return STATE_IGNORED;
}

StateRetVal Test02Callback_A11(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(EVT3, evt->user_event);
    return STATE_HANDLED;
}

/*
    Test 02
    State machine is in initial state A111, an event EVT3 is set but it is handled now by upper state - A11
    No transition expected, upper state A11 should be called
*/
void test_Test02_StateProcessing_NoTransition_EvtHandled(void)
{
    A111_State_AddCallback(Test02Callback_A111);
    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A111_State_IgnoreArg_evt();

    A11_State_AddCallback(Test02Callback_A11);
    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A11_State_IgnoreArg_evt();

    FsmEvent_t test_evt = {EVT3, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL(&A111, test_machine.current_state);
}


StateRetVal Test03ExitCallback(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
    return STATE_HANDLED;
}

StateRetVal Test03EntryCallback(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(ENTRY_EVT, evt->user_event);
    return STATE_HANDLED;
}

StateRetVal Test03Callback_A111(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT1, evt->user_event);
        ctx->next_state = &B121;
        return STATE_TRANSITION;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
    }
}

/*
    Test 03
    Transition from A111 to state B121
    expected sequence:
    EXIT SEQUEBCE:
    A111
    A11
    A1
    A
    ENTRY SEQUENCE:
    B
    B1
    B12
    B121
*/
void test_Test03_StateProcessing_Transition_A111_to_B121(void)
{
    A111_State_AddCallback(Test03Callback_A111);
    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    A111_State_IgnoreArg_evt();

    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    A111_State_IgnoreArg_evt();

    A11_State_AddCallback(Test03ExitCallback);
    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A11_State_IgnoreArg_evt();

    A1_State_AddCallback(Test03ExitCallback);
    A1_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A1_State_IgnoreArg_evt();

    A_State_AddCallback(Test03ExitCallback);
    A_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A_State_IgnoreArg_evt();

    B_State_AddCallback(Test03EntryCallback);
    B_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B_State_IgnoreArg_evt();

    B1_State_AddCallback(Test03EntryCallback);
    B1_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B1_State_IgnoreArg_evt();

    B12_State_AddCallback(Test03EntryCallback);
    B12_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B12_State_IgnoreArg_evt();

    B121_State_AddCallback(Test03EntryCallback);
    B121_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B121_State_IgnoreArg_evt();

    FsmEvent_t test_evt = {EVT1, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL_PTR(&B121, test_machine.next_state);
    TEST_ASSERT_EQUAL_PTR(&B121, test_machine.current_state);
}


StateRetVal Test04Callback_B121(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT1, evt->user_event);
        ctx->next_state = &B121;
        return STATE_IGNORED;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
    }
}

StateRetVal Test04Callback_B12(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT1, evt->user_event);
        ctx->next_state = &B11;
        return STATE_TRANSITION;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
    }
}

/*
    Test04 - transition from B121 to B11
    The transition is triggered from state B12 (superstate of B121)
    Expected sequence:
    B121 - returns ignored
    B12 - returns state transition
    exit sequence:
    B121
    B12
    entry sequence:
    B11
*/
void test_Test04_StateProcessing_Transition_B121_to_B11(void)
{
    B121_State_AddCallback(Test04Callback_B121);
    B121_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    B121_State_IgnoreArg_evt();

    B12_State_AddCallback(Test04Callback_B12);
    B12_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    B12_State_IgnoreArg_evt();

    B121_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B121_State_IgnoreArg_evt();

    B12_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B12_State_IgnoreArg_evt();

    // reuse callback from prev test
    B11_State_AddCallback(Test03EntryCallback);
    B11_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B11_State_IgnoreArg_evt();


    FsmEvent_t test_evt = {EVT1, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL_PTR(&B11, test_machine.next_state);
    TEST_ASSERT_EQUAL_PTR(&B11, test_machine.current_state);
}


StateRetVal Test05Callback_B11(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT2, evt->user_event);
        ctx->next_state = &A111;
        return STATE_TRANSITION;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
        return STATE_HANDLED;
    }
}

/*
    Test05 - transition from B11 to A111
    The transition is triggered from state B11
    Expected sequence:
    B11 - returns state transition
    exit sequence:
    B11
    B1
    B
    entry sequence:
    A
    A1
    A11
    A111
*/
void test_Test05_StateProcessing_Transition_B11_to_A111(void)
{

    B11_State_AddCallback(Test05Callback_B11);
    B11_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    B11_State_IgnoreArg_evt();

    B11_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    B11_State_IgnoreArg_evt();

    // reuse callback from prev test
    B1_State_AddCallback(Test03ExitCallback);
    B1_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B1_State_IgnoreArg_evt();

    B_State_AddCallback(Test03ExitCallback);
    B_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    B_State_IgnoreArg_evt();

    // reuse callback from prev test
    A_State_AddCallback(Test03EntryCallback);
    A_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A_State_IgnoreArg_evt();

    // reuse callback from prev test
    A1_State_AddCallback(Test03EntryCallback);
    A1_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A1_State_IgnoreArg_evt();

    // reuse callback from prev test
    A11_State_AddCallback(Test03EntryCallback);
    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A11_State_IgnoreArg_evt();

    // reuse callback from prev test
    A111_State_AddCallback(Test03EntryCallback);
    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A111_State_IgnoreArg_evt();


    FsmEvent_t test_evt = {EVT2, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL_PTR(&A111, test_machine.next_state);
    TEST_ASSERT_EQUAL_PTR(&A111, test_machine.current_state);
}

StateRetVal Test06Callback_A111(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT2, evt->user_event);
        return STATE_IGNORED;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
        return STATE_HANDLED;
    }
}

StateRetVal Test06Callback_A11(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT2, evt->user_event);
        return STATE_IGNORED;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
        return STATE_HANDLED;
    }
}

StateRetVal Test06Callback_A1(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    if(cmock_num_calls == 0)
    {
        TEST_ASSERT_EQUAL(EVT2, evt->user_event);
        ctx->next_state = &A2;
        return STATE_TRANSITION;
    }
    else
    {
        TEST_ASSERT_EQUAL(EXIT_EVT, evt->user_event);
        return STATE_HANDLED;
    }
}

/*
    Test06 - transition from A111 to A2
    The transition is triggered from state A1
    Expected sequence:
    A111 - returns state ignored
    A11 - returns state ignored
    A1 - returns state transition
    exit sequence:
    A111
    A11
    A1
    entry sequence:
    A2
*/
void test_Test06_StateProcessing_Transition_A111_to_A2(void)
{
    A111_State_AddCallback(Test06Callback_A111);
    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A111_State_IgnoreArg_evt();

    A11_State_AddCallback(Test06Callback_A11);
    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A11_State_IgnoreArg_evt();

    A1_State_AddCallback(Test06Callback_A1);
    A1_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    A1_State_IgnoreArg_evt();

    A111_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A111_State_IgnoreArg_evt();

    A11_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A11_State_IgnoreArg_evt();

    A1_State_ExpectAndReturn(&test_machine, NULL, STATE_TRANSITION);
    A1_State_IgnoreArg_evt();

    // reuse callback from prev test
    A2_State_AddCallback(Test03EntryCallback);
    A2_State_ExpectAndReturn(&test_machine, NULL, STATE_HANDLED);
    A2_State_IgnoreArg_evt();

    FsmEvent_t test_evt = {EVT2, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL_PTR(&A2, test_machine.next_state);
    TEST_ASSERT_EQUAL_PTR(&A2, test_machine.current_state);
}

StateRetVal Test07IgnoredCallback(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(EVT2, evt->user_event);
    return STATE_IGNORED;
}

/*
    Test07 - state machine in state A2 - event is not handled by any state
    No state transition
    Expected sequence:
    A2 - returns state ignored
    A - returns state ignored
    ROOT - returns state ignored
    state machine remains in A2 state
*/
void test_Test07_StateProcessing_IgnoredEvent(void)
{
    A2_State_AddCallback(Test07IgnoredCallback);
    A2_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A2_State_IgnoreArg_evt();

    A_State_AddCallback(Test07IgnoredCallback);
    A_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    A_State_IgnoreArg_evt();

    ROOT_State_AddCallback(Test07IgnoredCallback);
    ROOT_State_ExpectAndReturn(&test_machine, NULL, STATE_IGNORED);
    ROOT_State_IgnoreArg_evt();

    FsmEvent_t test_evt = {EVT2, NULL};
    StateMachine_ProcessEvent(&test_machine, &test_evt);
    TEST_ASSERT_EQUAL_PTR(&A2, test_machine.current_state);
}

#endif // TEST

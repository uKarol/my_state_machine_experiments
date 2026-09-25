
#ifdef TEST

#include "unity.h"

#include "state_machine.h"
#include "state_handlers.h"
#include "mock_state_handlers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

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

void helper_StateMachineSetup(void)
{
    InitState(&ROOT, ROOT_State , NULL);
    InitState(&A, A_State, &ROOT);
    InitState(&B, B_State, &ROOT);
    InitState(&C, C_State, &ROOT);
    // substates of A
    InitState(&A1, A1_State, &A);
    InitState(&A2, A2_State, &A);
    // substates of A1
    InitState(&A11, A11_State, &A1);
    // substates of A11
    InitState(&A111, A111_State, &A11);
    
    // substates of B
    InitState(&B1, B1_State, &B);
    // substates of B1
    InitState(&B11, B11_State, &B1);
    InitState(&B12, B12_State, &B1);

    // substates of B1
    InitState(&B121, B121_State, &B12);

}

void test_StateMachineInitializeTest(void)
{
    MyStateMachine_t test_machine;
    helper_StateMachineSetup();

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

    StateMachineInitialize(&test_machine, &A111);

    TEST_ASSERT_EQUAL_PTR(&A111, test_machine.current_state);
}

//typedef StateRetVal(* CMOCK_ROOT_State_CALLBACK)(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls) 

StateRetVal EntryTestCallback(MyStateMachine_t *ctx,FsmEvent_t *evt, int cmock_num_calls)
{
    TEST_ASSERT_EQUAL(ENTRY_EVT, evt->user_event);
    return STATE_HANDLED;
}

void test_StateMachineInitializeTest_Enhanced(void)
{
    MyStateMachine_t test_machine;
    helper_StateMachineSetup();

    ROOT_State_AddCallback(EntryTestCallback);
    A_State_AddCallback(EntryTestCallback);
    A1_State_AddCallback(EntryTestCallback);
    A11_State_AddCallback(EntryTestCallback);
    A111_State_AddCallback(EntryTestCallback);

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

    StateMachineInitialize(&test_machine, &A111);

    TEST_ASSERT_EQUAL_PTR(&A111, test_machine.current_state);
}


#endif // TEST

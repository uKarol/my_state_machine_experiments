
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
}

StateRetVal DummyStateHandler(MyStateMachine_t *ctx,FsmEvent_t *evt)
{
    return STATE_HANDLED;
}

void test_StateInitializationInvalid(void)
{
    MyState_t invalid[MAX_DEPTH + 1];

    TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&invalid[0], DummyStateHandler, NULL));
    for(int i = 1; i<MAX_DEPTH; i++)
    {
        TEST_ASSERT_EQUAL(STATE_MACHINE_OK, InitState(&invalid[i], DummyStateHandler, &invalid[i-1]));
    }
    // exceed max depth
    TEST_ASSERT_EQUAL(STATE_MACHINE_ERROR, InitState(&invalid[MAX_DEPTH], DummyStateHandler, &invalid[MAX_DEPTH-1]));
    // null state
    TEST_ASSERT_EQUAL(STATE_MACHINE_ERROR, InitState(NULL, DummyStateHandler, &invalid[MAX_DEPTH-1]));
    // null state handler
    TEST_ASSERT_EQUAL(STATE_MACHINE_ERROR, InitState(&invalid[MAX_DEPTH], NULL, &invalid[MAX_DEPTH-1]));

}

void test_StateMachineInitializeTest(void)
{
    MyStateMachine_t test_machine;

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


#ifdef TEST

#include "unity.h"

#include "find_LCA.h"
#include "state_machine.h"

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

StateRetVal TestStateHandler(MyStateMachine_t *ctx,FsmEvent_t *evt)
{
    return STATE_HANDLED;
}

void helper_StateMachineSetup(void)
{
    InitState(&ROOT, TestStateHandler, NULL);
    InitState(&A, TestStateHandler, &ROOT);
    InitState(&B, TestStateHandler, &ROOT);
    InitState(&C, TestStateHandler, &ROOT);
    // substates of A
    InitState(&A1, TestStateHandler, &A);
    InitState(&A2, TestStateHandler, &A);
    // substates of A1
    InitState(&A11, TestStateHandler, &A1);
    // substates of A11
    InitState(&A111, TestStateHandler, &A11);
    
    // substates of B
    InitState(&B1, TestStateHandler, &B);
    // substates of B1
    InitState(&B11, TestStateHandler, &B1);
    InitState(&B12, TestStateHandler, &B1);

    // substates of B1
    InitState(&B121, TestStateHandler, &B12);

}

void test_StateROOT_Init(void)
{
    helper_StateMachineSetup();

    TEST_ASSERT_EQUAL(0, ROOT.depth);
    TEST_ASSERT_EQUAL_PTR(NULL, ROOT.parent);

    TEST_ASSERT_EQUAL(1, A.depth);
    TEST_ASSERT_EQUAL_PTR(&ROOT, A.parent);

    TEST_ASSERT_EQUAL(2, A1.depth);
    TEST_ASSERT_EQUAL_PTR(&A, A1.parent);

    TEST_ASSERT_EQUAL(2, A2.depth);
    TEST_ASSERT_EQUAL_PTR(&A, A2.parent);

    TEST_ASSERT_EQUAL(3, A11.depth);
    TEST_ASSERT_EQUAL_PTR(&A1, A11.parent);

    TEST_ASSERT_EQUAL(4, A111.depth);
    TEST_ASSERT_EQUAL_PTR(&A11, A111.parent);

    TEST_ASSERT_EQUAL(1, B.depth);
    TEST_ASSERT_EQUAL_PTR(&ROOT, B.parent);

    TEST_ASSERT_EQUAL(2, B1.depth);
    TEST_ASSERT_EQUAL_PTR(&B, B1.parent);

    TEST_ASSERT_EQUAL(3, B11.depth);
    TEST_ASSERT_EQUAL_PTR(&B1, B11.parent);

    TEST_ASSERT_EQUAL(3, B12.depth);
    TEST_ASSERT_EQUAL_PTR(&B1, B12.parent);

    TEST_ASSERT_EQUAL(4, B121.depth);
    TEST_ASSERT_EQUAL_PTR(&B12, B121.parent); 

    TEST_ASSERT_EQUAL(1, C.depth);
    TEST_ASSERT_EQUAL_PTR(&ROOT, C.parent);

}

#define MAX_DEPTH 10

void test_findLCA_A1_A2(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&A, find_LCA(&A1, &A2, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(1, exit_path_size);
    TEST_ASSERT_EQUAL(1, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&A2, entry_path[0]);
}

void test_findLCA_A111_A2(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&A, find_LCA(&A111, &A2, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(3, exit_path_size);
    TEST_ASSERT_EQUAL(1, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&A2, entry_path[0]);
}

void test_findLCA_A111_B121(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&ROOT, find_LCA(&A111, &B121, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(4, exit_path_size);
    TEST_ASSERT_EQUAL(4, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&B, entry_path[3]);
    TEST_ASSERT_EQUAL_PTR(&B1, entry_path[2]);
    TEST_ASSERT_EQUAL_PTR(&B12, entry_path[1]);
    TEST_ASSERT_EQUAL_PTR(&B121, entry_path[0]);
}

void test_findLCA_B11_B121(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&B1, find_LCA(&B11, &B121, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(1, exit_path_size);
    TEST_ASSERT_EQUAL(2, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&B12, entry_path[1]);
    TEST_ASSERT_EQUAL_PTR(&B121, entry_path[0]);
}

void test_findLCA_ROOT_A111(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&ROOT, find_LCA(&ROOT, &A111, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(0, exit_path_size);
    TEST_ASSERT_EQUAL(4, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&A, entry_path[3]);
    TEST_ASSERT_EQUAL_PTR(&A1, entry_path[2]);
    TEST_ASSERT_EQUAL_PTR(&A11, entry_path[1]);
    TEST_ASSERT_EQUAL_PTR(&A111, entry_path[0]);
}

void test_findLCA_A111_ROOT(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&ROOT, find_LCA(&A111, &ROOT, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(4, exit_path_size);
    TEST_ASSERT_EQUAL(0, entry_path_size);
}

void test_findLCA_B1_B121(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&B1, find_LCA(&B1, &B121, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(0, exit_path_size);
    TEST_ASSERT_EQUAL(2, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&B12, entry_path[1]);
    TEST_ASSERT_EQUAL_PTR(&B121, entry_path[0]);
}

void test_findLCA_B121_B1(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&B1, find_LCA(&B121, &B1, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(2, exit_path_size);
    TEST_ASSERT_EQUAL(0, entry_path_size);
}

void test_findLCA_A111_A111(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&A111, find_LCA(&A111, &A111, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(0, exit_path_size);
    TEST_ASSERT_EQUAL(0, entry_path_size);
}

void test_findLCA_A_C(void)
{
    MyState_t *entry_path[MAX_DEPTH];
    uint8_t exit_path_size;
    uint8_t entry_path_size;
    TEST_ASSERT_EQUAL_PTR(&ROOT, find_LCA(&A, &C, &entry_path, &entry_path_size, &exit_path_size));
    TEST_ASSERT_EQUAL(1, exit_path_size);
    TEST_ASSERT_EQUAL(1, entry_path_size);
    TEST_ASSERT_EQUAL_PTR(&C, entry_path[0]);
}

#endif // TEST

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


MyState_t S0 = {NULL, 0, 0, NULL};
MyState_t S1 = {NULL, 1, 1, &S0};
MyState_t S2 = {NULL, 2, 1, &S0};
MyState_t S21 = {NULL, 21, 2, &S2};
MyState_t S211 = {NULL, 211, 3, &S21};
MyState_t S11 = {NULL, 11, 2, &S1};
#define MAX_DEPTH 10

int main()
{
    uint8_t entry_path_size = 0;
    uint8_t exit_path_size = 0;
    MyState_t *entry_path[MAX_DEPTH];
    MyState_t *exit_path[MAX_DEPTH];
    MyState_t *result = find_LCA(&S11, &S211, &entry_path, &entry_path_size, &exit_path, &exit_path_size);
    printf("THE LCA IS %d\n", result->idx);

    printf("exiting order: ");
    for(uint8_t i = 0; i < exit_path_size; i++)
    {
        printf(" %d,", exit_path[i]->idx);
    }

    printf("\nentering order: ");
    for(uint8_t i = entry_path_size-1; i >= 0; i--)
    {
        printf(" %d,", entry_path[i]->idx);
    }

}
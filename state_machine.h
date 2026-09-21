#include <stdint.h>

typedef void (*my_fun) (void *ctx);

typedef struct MyState_t MyState_t;

struct MyState_t
{
    my_fun fun;
    uint8_t idx;
    uint8_t depth;
    MyState_t *parent;
};

MyState_t *find_LCA(MyState_t *src_state, MyState_t *dest_state, MyState_t **EntryPath, uint8_t *entry_path_size, MyState_t **ExitPath, uint8_t *exit_path_size);

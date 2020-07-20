#include "btn_cstdlib.h"

#include "btn_memory_manager.h"

namespace btn
{

void* malloc(int bytes)
{
    return memory_manager::ewram_alloc(bytes);
}

void free(void* ptr)
{
    memory_manager::ewram_free(ptr);
}

}

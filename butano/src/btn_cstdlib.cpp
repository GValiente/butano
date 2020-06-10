#include "btn_cstdlib.h"

#include <stdlib.h>
#include "btn_memory_manager.h"

void* malloc(size_t bytes)
{
    return btn::memory_manager::ewram_alloc(bytes);
}

void free(void* ptr)
{
    btn::memory_manager::ewram_free(ptr);
}

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

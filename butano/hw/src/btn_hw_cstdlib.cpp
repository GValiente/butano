#include "btn_cstdlib.h"

#include <stdlib.h>

void* malloc(size_t bytes)
{
    return btn::malloc(bytes);
}

void free(void* ptr)
{
    btn::free(ptr);
}

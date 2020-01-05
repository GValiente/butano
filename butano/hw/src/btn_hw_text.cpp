#include "../include/btn_hw_text.h"

#include "btn_math.h"
#include "btn_array.h"
#include "btn_memory.h"
#include "btn_string_view.h"

extern "C"
{
    #include "../3rd_party/posprintf/include/posprintf.h"
}

namespace btn::hw::text
{

int parse(int value, array<char, 32>& output)
{
    char* output_data = output.data();
    int abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output_data, "%d", value);
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%d", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(long value, array<char, 32>& output)
{
    char* output_data = output.data();
    long abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output_data, "%d", int(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", value);
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%ld", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(int64_t value, array<char, 32>& output)
{
    char* output_data = output.data();
    int64_t abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output_data, "%d", int(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%lld", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(unsigned value, array<char, 32>& output)
{
    char* output_data = output.data();
    int size;

    if(value < 65536)
    {
        posprintf(output_data, "%d", int(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%u", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(unsigned long value, array<char, 32>& output)
{
    char* output_data = output.data();
    int size;

    if(value < 65536)
    {
        posprintf(output_data, "%d", int(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%lu", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(uint64_t value, array<char, 32>& output)
{
    char* output_data = output.data();
    int size;

    if(value < 65536)
    {
        posprintf(output_data, "%d", int(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = ::snprintf(output_data, output.size(), "%llu", value);
        BTN_ASSERT(size > 0, "snprintf call failed: ", size);
    }

    return size;
}

int parse(const void* ptr, array<char, 32>& output)
{
    char* output_data = output.data();
    int size;

    if(ptr)
    {
        posprintf(output_data, "0x%x", ptr);
        size = int(string_view(output_data).size());
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        const char tag[] = "nullptr";
        size = sizeof(tag);
        btn::memory::copy(*tag, size, *output_data);
    }

    return size;
}

}

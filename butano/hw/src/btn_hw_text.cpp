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

namespace
{
    [[nodiscard]] int _append_high_value(char* output_data)
    {
        output_data[0] = '+';
        output_data[1] = '!';
        output_data[2] = '+';
        return 3;
    }
}

int parse(int value, array<char, 32>& output)
{
    char* output_data = output.data();
    int abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output_data, "%d", value);
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
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
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", value);
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
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
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
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
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
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
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
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
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
        BTN_ASSERT(size > 0, "posprintf call failed");
    }
    else
    {
        size = _append_high_value(output_data);
    }

    return size;
}

int parse(const void* ptr, array<char, 32>& output)
{
    char* output_data = output.data();
    posprintf(output_data, "0x%x", ptr);

    int size = string_view(output_data).size();
    BTN_ASSERT(size > 0, "posprintf call failed");

    return size;
}

}

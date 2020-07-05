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
    template<typename Type>
    [[nodiscard]] int _to_string(Type value, char* output_data)
    {
        if(value < 0)
        {
            *output_data = '-';
            ++output_data;
            return _to_string(-value, output_data) + 1;
        }

        char* current_output_data = output_data;

        while(value > 9)
        {
            auto digit = int(value % 10);
            value /= 10;
            *current_output_data = '0' + digit;
            ++current_output_data;
        }

        auto digit = int(value);
        *current_output_data = '0' + digit;
        ++current_output_data;
        std::reverse(output_data, current_output_data);
        return current_output_data - output_data;
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
        size = _to_string(value, output_data);
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
        size = _to_string(value, output_data);
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
        size = _to_string(value, output_data);
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
        size = _to_string(value, output_data);
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
        size = _to_string(value, output_data);
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
        size = _to_string(value, output_data);
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

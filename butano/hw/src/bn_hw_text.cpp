/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_text.h"

#include "bn_math.h"
#include "bn_array.h"
#include "bn_string_view.h"

extern "C"
{
    #include "../3rd_party/posprintf/include/posprintf.h"
}

namespace bn::hw::text
{

namespace
{
    template<typename Type>
    [[nodiscard]] int _to_string(Type value, char* output)
    {
        if(value < 0)
        {
            *output = '-';
            ++output;
            return _to_string(-value, output) + 1;
        }

        array<char, 16> extra_array;
        char* extra_data = extra_array.data();
        char* current_extra_data = extra_data;

        do
        {
            auto digit = int(value % 10);
            value /= 10;
            *current_extra_data = char('0' + digit);
            ++current_extra_data;
        }
        while(value >= 500000000);

        posprintf(output, "%l", long(value));

        int size = string_view(output).size();
        int extra_size = current_extra_data - extra_data;

        for(int index = extra_size - 1; index >= 0; --index)
        {
            output[size] = extra_data[index];
            ++size;
        }

        return size;
    }
}

int parse(int value, char* output)
{
    int abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output, "%d", value);
        size = string_view(output).size();
    }
    else if(abs_value < 500000000)
    {
        posprintf(output, "%l", long(value));
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(long value, char* output)
{
    long abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output, "%d", int(value));
        size = string_view(output).size();
    }
    else if(abs_value < 500000000)
    {
        posprintf(output, "%l", value);
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(int64_t value, char* output)
{
    int64_t abs_value = abs(value);
    int size;

    if(abs_value < 65536)
    {
        posprintf(output, "%d", int(value));
        size = string_view(output).size();
    }
    else if(abs_value < 500000000)
    {
        posprintf(output, "%l", long(value));
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(unsigned value, char* output)
{
    int size;

    if(value < 65536)
    {
        posprintf(output, "%d", int(value));
        size = string_view(output).size();
    }
    else if(value < 500000000)
    {
        posprintf(output, "%l", long(value));
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(unsigned long value, char* output)
{
    int size;

    if(value < 65536)
    {
        posprintf(output, "%d", int(value));
        size = string_view(output).size();
    }
    else if(value < 500000000)
    {
        posprintf(output, "%l", long(value));
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(uint64_t value, char* output)
{
    int size;

    if(value < 65536)
    {
        posprintf(output, "%d", int(value));
        size = string_view(output).size();
    }
    else if(value < 500000000)
    {
        posprintf(output, "%l", long(value));
        size = string_view(output).size();
    }
    else
    {
        size = _to_string(value, output);
    }

    return size;
}

int parse(const void* ptr, char* output)
{
    posprintf(output, "0x%x", ptr);
    return string_view(output).size();
}

}

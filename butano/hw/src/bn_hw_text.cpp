/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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
    [[nodiscard]] int _to_string(Type value, char* output_data)
    {
        if(value < 0)
        {
            *output_data = '-';
            ++output_data;
            return _to_string(-value, output_data) + 1;
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

        posprintf(output_data, "%l", long(value));

        int size = string_view(output_data).size();
        int extra_size = current_extra_data - extra_data;

        for(int index = extra_size - 1; index >= 0; --index)
        {
            output_data[size] = extra_data[index];
            ++size;
        }

        return size;
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
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
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
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", value);
        size = string_view(output_data).size();
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
    }
    else if(abs_value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
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
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
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
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
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
    }
    else if(value < 500000000)
    {
        posprintf(output_data, "%l", long(value));
        size = string_view(output_data).size();
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
    return string_view(output_data).size();
}

}

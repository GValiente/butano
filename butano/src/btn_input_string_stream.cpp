#include "btn_input_string_stream.h"

#include "btn_array.h"
#include "btn_assert.h"
#include "btn_string_view.h"
#include "../hw/include/btn_hw_text.h"

namespace btn
{

void input_string_stream::append(const string_view& str_view)
{
    _string.append(str_view.data(), str_view.size());
}

void input_string_stream::append(const char* char_array)
{
    if(char_array)
    {
        _string.append(char_array);
    }
}

void input_string_stream::append(const char* char_array, size_t size)
{
    _string.append(char_array, size);
}

void input_string_stream::append(int value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(long value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(int64_t value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(unsigned value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(unsigned long value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(uint64_t value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    _string.append(buffer.data(), size_t(size));
}

void input_string_stream::append(float value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%g", double(value));
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    _string.append(buffer.data(), size_t(buffer_size));
}

void input_string_stream::append(double value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%lg", value);
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    _string.append(buffer.data(), size_t(buffer_size));
}

void input_string_stream::append(long double value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%Lg", value);
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    _string.append(buffer.data(), size_t(buffer_size));
}

void input_string_stream::append(const void* ptr)
{
    if(ptr)
    {
        array<char, 32> buffer;
        int size = btn::hw::text::parse(ptr, buffer);
        _string.append(buffer.data(), size_t(size));
    }
    else
    {
        _string.append("nullptr");
    }
}

}

#include "btn_input_string_stream.h"

#include "btn_array.h"
#include "btn_assert.h"
#include "btn_string_view.h"
#include "../hw/include/btn_hw_text.h"

namespace btn
{

input_string_stream::input_string_stream(istring_base& string) :
    _string(string)
{
    BTN_ASSERT(string.data(), "Data is null");
    BTN_ASSERT(string.size() >= 0, "Invalid size: ", string.size());
    BTN_ASSERT(string.max_size() >= 2, "Invalid max size: ", string.max_size());
    BTN_ASSERT(string.size() < string.max_size(), "Invalid size or max size: ", string.size(), " - ", string.max_size());

    string.data()[string.size()] = 0;
}

input_string_stream::input_string_stream(istring& string) :
    _string(string)
{
}

const istring& input_string_stream::string() const
{
    return static_cast<const istring&>(_string);
}

istring& input_string_stream::string()
{
    return static_cast<istring&>(_string);
}

void input_string_stream::append(char character)
{
    string().push_back(character);
}

void input_string_stream::append(const string_view& str_view)
{
    string().append(str_view.data(), str_view.size());
}

void input_string_stream::append(const istring& _string)
{
    string().append(_string);
}

void input_string_stream::append(const char* char_array)
{
    string().append(char_array);
}

void input_string_stream::append(const char* char_array, int size)
{
    string().append(char_array, size);
}

void input_string_stream::append(int value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(long value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(int64_t value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(unsigned value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(unsigned long value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(uint64_t value)
{
    array<char, 32> buffer;
    int size = btn::hw::text::parse(value, buffer);
    string().append(buffer.data(), size);
}

void input_string_stream::append(float value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%g", double(value));
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    string().append(buffer.data(), buffer_size);
}

void input_string_stream::append(double value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%lg", value);
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    string().append(buffer.data(), buffer_size);
}

void input_string_stream::append(long double value)
{
    array<char, 32> buffer;
    int buffer_size = ::snprintf(buffer.data(), buffer.size(), "%Lg", value);
    BTN_ASSERT(buffer_size > 0, "snprintf call failed: ", buffer_size);

    string().append(buffer.data(), buffer_size);
}

void input_string_stream::append(const void* ptr)
{
    if(ptr)
    {
        array<char, 32> buffer;
        int size = btn::hw::text::parse(ptr, buffer);
        string().append(buffer.data(), size);
    }
    else
    {
        string().append("nullptr");
    }
}

void input_string_stream::clear()
{
    string().clear();
}

}

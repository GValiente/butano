#include "btn_input_string_stream.h"

#include "btn_array.h"
#include "btn_string.h"
#include "btn_string_view.h"
#include "../hw/include/btn_hw_text.h"

namespace btn
{

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

void input_string_stream::append(const string_view& string_view)
{
    string().append(string_view.data(), string_view.size());
}

void input_string_stream::append(const istring& istring)
{
    string().append(istring);
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

#pragma once

#include <string>
#include <cstddef>

class UTF8Sizer {
public:
    size_t utf8_char_len(unsigned char c);
    size_t utf8_word_length(const std::string& str);
};
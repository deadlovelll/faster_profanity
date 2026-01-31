#include "utf8_sizer/utf8_sizer.hpp"

size_t UTF8Sizer::utf8_char_len(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

size_t UTF8Sizer::utf8_word_length(const std::string& str) {
    size_t length = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80) {
            length++;
        }
    }
    return length;
}
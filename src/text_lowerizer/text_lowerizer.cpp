#include "text_lowerizer/text_lowerizer.hpp"

#include <utf8proc.h>

void TextLowerizer::lower(std::string& text) {
    std::string out;
    out.reserve(text.size());

    const utf8proc_uint8_t* bytes = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
    const utf8proc_ssize_t size = static_cast<utf8proc_ssize_t>(text.size());
    utf8proc_ssize_t pos = 0;

    while (pos < size) {
        // ASCII fast path
        if (bytes[pos] < 0x80) {
            out.push_back(lower_ascii(static_cast<char>(bytes[pos])));
            ++pos;
            continue;
        }

        utf8proc_int32_t code_point;
        utf8proc_ssize_t len = utf8proc_iterate(bytes + pos, size - pos, &code_point);
        if (len < 0) {
            out.push_back(static_cast<char>(bytes[pos]));
            ++pos;
            continue;
        }

        utf8proc_uint8_t encoded[4];
        utf8proc_ssize_t encoded_len = utf8proc_encode_char(utf8proc_tolower(code_point), encoded);
        out.append(reinterpret_cast<const char*>(encoded), static_cast<std::size_t>(encoded_len));
        pos += len;
    }
    text.swap(out);
}

// Mapped by hand: std::tolower depends on the process C locale
char TextLowerizer::lower_ascii(char c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<char>(c - 'A' + 'a');
    }
    return c;
}

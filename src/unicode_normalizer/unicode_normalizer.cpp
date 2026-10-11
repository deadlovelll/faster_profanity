#include "unicode_normalizer/unicode_normalizer.hpp"

#include <utf8proc.h>

void UnicodeNormalizer::normalize(std::string& text) {
    std::string out;
    out.reserve(text.size());

    const utf8proc_uint8_t* bytes = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
    const utf8proc_ssize_t size = static_cast<utf8proc_ssize_t>(text.size());
    utf8proc_ssize_t pos = 0;

    while (pos < size) {
        // ASCII fast path: no ASCII character is invisible
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

        char32_t current = static_cast<char32_t>(code_point);
        if (is_invisible(current)) {
            pos += len;
            continue;
        }

        char32_t lowered = lower(current);
        if (lowered == current) {
            out.append(text, static_cast<std::size_t>(pos), static_cast<std::size_t>(len));
        } else {
            utf8proc_uint8_t encoded[4];
            utf8proc_ssize_t encoded_len = utf8proc_encode_char(static_cast<utf8proc_int32_t>(lowered), encoded);
            out.append(reinterpret_cast<const char*>(encoded), static_cast<std::size_t>(encoded_len));
        }
        pos += len;
    }
    text.swap(out);
}

// Сheck for invisible chars such as zero-width chars, soft hyphen, BOM, bidi controls,
bool UnicodeNormalizer::is_invisible(char32_t code_point) {
    return utf8proc_get_property(static_cast<utf8proc_int32_t>(code_point))->ignorable != 0;
}

char32_t UnicodeNormalizer::lower(char32_t code_point) {
    return static_cast<char32_t>(utf8proc_tolower(static_cast<utf8proc_int32_t>(code_point)));
}

char UnicodeNormalizer::lower_ascii(char c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<char>(c - 'A' + 'a');
    }
    return c;
}

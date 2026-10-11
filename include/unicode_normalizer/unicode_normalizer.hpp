#pragma once

#include <string>

// Unambiguous per-character canonicalization done in a single pass over UTF-8 text:
// removes invisible characters and lowercases everything else.
class UnicodeNormalizer {
public:
    void normalize(std::string& text);
private:
    bool is_invisible(char32_t code_point);
    char32_t lower(char32_t code_point);
    char lower_ascii(char c);
};

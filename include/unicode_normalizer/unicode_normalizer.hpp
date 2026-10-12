#pragma once

#include <string>

// Unambiguous per-character canonicalization done in a single pass over UTF-8 text:
// removes invisible characters, replaces compatibility characters (fullwidth, mathematical,
// circled, ligatures...) with ordinary ones and applies full Unicode case folding (ß -> ss).
class UnicodeNormalizer {
public:
    void normalize(std::string& text);
private:
    enum class CharKind { Invisible, Compatibility, CaseFolding, Regular };

    CharKind classify(char32_t code_point);
    void append_compatibility_form(std::string& out, char32_t code_point);
    void append_case_folded(std::string& out, char32_t code_point);
    void append_code_point(std::string& out, char32_t code_point);
    char32_t lower(char32_t code_point);
    char lower_ascii(char c);
};

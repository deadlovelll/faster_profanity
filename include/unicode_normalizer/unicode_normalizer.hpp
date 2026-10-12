#pragma once

#include <string>

#include "char_classifier/char_classifier.hpp"
#include "diacritics_stripper/diacritics_stripper.hpp"

// Unambiguous per-character canonicalization done in a single pass over UTF-8 text:
// removes invisible characters, replaces compatibility characters (fullwidth, mathematical,
// circled, ligatures...) with ordinary ones, applies full Unicode case folding (ß -> ss)
// and strips diacritics (é -> e).
class UnicodeNormalizer {
public:
    void normalize(std::string& text);
private:
    CharClassifier char_classifier;
    DiacriticsStripper diacritics_stripper;

    void append_compatibility_form(std::string& out, char32_t code_point);
    void append_case_folded(std::string& out, char32_t code_point);
    void append_without_diacritics(std::string& out, char32_t code_point);
    void append_code_point(std::string& out, char32_t code_point);
    char32_t lower(char32_t code_point);
    char lower_ascii(char c);
};

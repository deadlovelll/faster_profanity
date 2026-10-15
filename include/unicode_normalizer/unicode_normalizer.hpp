#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "char_classifier/char_classifier.hpp"
#include "diacritics_stripper/diacritics_stripper.hpp"

// Unambiguous per-character canonicalization done in a single pass over UTF-8 text:
// removes invisible characters, replaces compatibility characters (fullwidth, mathematical,
// circled, ligatures...) with ordinary ones, applies full Unicode case folding (ß -> ss),
// strips diacritics (é -> e) and replaces lookalikes of Latin letters with them (Cyrillic с -> c).
class UnicodeNormalizer {
public:
    void normalize(std::string& text);
    void normalize(std::string& text, std::vector<std::size_t>& source_offsets);
private:
    struct SourceChar {
        char32_t code_point;
        std::ptrdiff_t start;
        std::ptrdiff_t end;
    };

    class [[nodiscard]] OutputBuffer {
    public:
        OutputBuffer(std::string& storage, std::size_t capacity);
        void push_back(char c);
        void append(const char* bytes, std::size_t count);
        std::size_t size() const;
        std::string_view view() const;
        void finish();
    private:
        std::string* storage;
        char* data;
        std::size_t used;
        std::size_t capacity;

        static char* grow(std::string& storage, std::size_t needed);
    };

    CharClassifier char_classifier;
    DiacriticsStripper diacritics_stripper;

    template <bool with_offsets>
    void normalize_text(std::string& text, std::vector<std::size_t>* source_offsets);

    void reset_offsets(std::vector<std::size_t>& source_offsets, std::size_t text_size);
    void record_offsets(std::vector<std::size_t>& source_offsets, std::size_t out_size, std::ptrdiff_t char_start);
    void close_offsets(std::vector<std::size_t>& source_offsets, std::size_t text_size);
    SourceChar read_char(std::string_view text, std::ptrdiff_t pos);
    CharKind classify_char(SourceChar source_char);
    void recompose_separate_letter(std::string_view text, SourceChar& source_char, CharKind& kind);
    OutputBuffer append_regional_indicator(OutputBuffer out, SourceChar source_char, std::string_view text);
    OutputBuffer append_source(OutputBuffer out, SourceChar source_char, std::string_view text);
    OutputBuffer append_compatibility_form(OutputBuffer out, char32_t code_point);
    OutputBuffer append_case_folded(OutputBuffer out, char32_t code_point);
    OutputBuffer append_without_diacritics(OutputBuffer out, char32_t code_point);
    OutputBuffer append_code_point(OutputBuffer out, char32_t code_point);
    bool is_regional_indicator(char32_t code_point);
    bool is_regional_indicator_at(std::string_view text, std::ptrdiff_t pos);
    char32_t last_code_point(std::string_view text);
    char32_t lower(char32_t code_point);
    char lower_ascii(char c);
};

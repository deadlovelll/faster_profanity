#include "diacritics_stripper/diacritics_stripper.hpp"
#include "diacritics_stripper/diacritic_marks.hpp"
#include "diacritics_stripper/separate_letters.hpp"

#include <utf8proc.h>

// Letters below U+2000 (Latin, Greek, Cyrillic) are stripped once, at program start: one lookup per letter
const DiacriticsStripper::Table DiacriticsStripper::table = DiacriticsStripper().build_table();

int DiacriticsStripper::strip(char32_t code_point, char32_t* stripped) {
    if (code_point < table.size() && table[code_point] != 0) {
        stripped[0] = table[code_point];
        return 1;
    }
    return strip_uncached(code_point, stripped);
}

bool DiacriticsStripper::is_diacritic_mark(char32_t code_point) {
    for (const CodePointRange& range : diacritic_marks) {
        if (code_point >= range.first && code_point <= range.last) {
            return true;
        }
    }
    return false;
}

// Stripped form of every character that strips to exactly one code point, 0 for the rest
DiacriticsStripper::Table DiacriticsStripper::build_table() {
    Table built{};
    for (char32_t c = 0; c < built.size(); ++c) {
        char32_t stripped[4];
        if (strip_uncached(c, stripped) == 1) {
            built[c] = static_cast<char16_t>(stripped[0]);
        }
    }
    return built;
}

int DiacriticsStripper::strip_uncached(char32_t code_point, char32_t* stripped) {
    if (is_diacritic_mark(code_point)) {
        return 0;
    }
    utf8proc_int32_t lowered = utf8proc_tolower(static_cast<utf8proc_int32_t>(code_point));
    if (is_separate_letter(code_point)) {
        stripped[0] = static_cast<char32_t>(lowered);
        return 1;
    }

    // The longest canonical decomposition is 4 code points
    const utf8proc_ssize_t capacity = 4;
    utf8proc_int32_t decomposed[capacity];
    utf8proc_int32_t boundclass = UTF8PROC_BOUNDCLASS_START;
    utf8proc_ssize_t count = utf8proc_decompose_char(
        static_cast<utf8proc_int32_t>(code_point), decomposed, capacity,
        static_cast<utf8proc_option_t>(UTF8PROC_DECOMPOSE | UTF8PROC_COMPAT), &boundclass
    );
    bool has_diacritics = false;
    for (utf8proc_ssize_t i = 0; count <= capacity && i < count; ++i) {
        has_diacritics = has_diacritics || is_diacritic_mark(static_cast<char32_t>(decomposed[i]));
    }
    if (!has_diacritics) {
        stripped[0] = static_cast<char32_t>(lowered);
        return 1;
    }

    int written = 0;
    for (utf8proc_ssize_t i = 0; i < count; ++i) {
        if (!is_diacritic_mark(static_cast<char32_t>(decomposed[i])) && decomposed[i] != ' ') {
            stripped[written++] = static_cast<char32_t>(utf8proc_tolower(decomposed[i]));
        }
    }
    return written;
}

bool DiacriticsStripper::is_separate_letter(char32_t code_point) {
    for (char32_t letter : separate_letters) {
        if (code_point == letter) {
            return true;
        }
    }
    return false;
}

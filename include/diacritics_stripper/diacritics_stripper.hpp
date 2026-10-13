#pragma once

#include <array>

// é -> e, ё -> е, f + U+0336 -> nothing. Keeps й, ў, ї: there the mark makes a separate letter
class DiacriticsStripper {
public:
    // Writes up to 4 code points into `stripped`, returns how many
    int strip(char32_t code_point, char32_t* stripped);
    bool is_diacritic_mark(char32_t code_point);
    char32_t compose_separate_letter(char32_t letter, char32_t mark);
    bool is_separate_letter_base(char32_t code_point);
private:
    using Table = std::array<char16_t, 0x2000>;

    static const Table table;

    Table build_table();
    int strip_uncached(char32_t code_point, char32_t* stripped);
    bool is_separate_letter(char32_t code_point);
};

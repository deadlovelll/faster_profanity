#pragma once

#include <array>

// é -> e, ё -> е, f + U+0336 -> nothing. Keeps й, ў, ї: there the mark makes a separate letter
class DiacriticsStripper {
public:
    // Writes up to 4 code points into `stripped`, returns how many
    int strip(char32_t code_point, char32_t* stripped);
    static bool is_diacritic_mark(char32_t code_point);
private:
    using Table = std::array<char16_t, 0x2000>;

    static const Table table;

    static Table build_table();
    static int strip_uncached(char32_t code_point, char32_t* stripped);
    static bool is_separate_letter(char32_t code_point);
};

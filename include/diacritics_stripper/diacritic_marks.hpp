#pragma once

#include <array>

struct CodePointRange {
    char32_t first;
    char32_t last;
};

// Diacritics shared by Latin, Greek and Cyrillic.
// Blocks from https://www.unicode.org/Public/18.0.0/ucd/Blocks.txt, comments quote their lines
inline constexpr std::array<CodePointRange, 10> diacritic_marks = {{
    {0x0300, 0x036F},  // 0300..036F; Combining Diacritical Marks
    {0x0483, 0x0489},  // 0400..04FF; Cyrillic - only its combining marks: titlo, pokrytie, millions sign
    {0x1AB0, 0x1AFF},  // 1AB0..1AFF; Combining Diacritical Marks Extended
    {0x1DC0, 0x1DFF},  // 1DC0..1DFF; Combining Diacritical Marks Supplement
    {0x20D0, 0x20FF},  // 20D0..20FF; Combining Diacritical Marks for Symbols
    {0x2DE0, 0x2DFF},  // 2DE0..2DFF; Cyrillic Extended-A
    {0xA66F, 0xA672},  // A640..A69F; Cyrillic Extended-B - combining vzmet, ten/hundred/thousand millions signs
    {0xA674, 0xA67D},  // A640..A69F; Cyrillic Extended-B - combining letters, kavyka, payerok
    {0xA69E, 0xA69F},  // A640..A69F; Cyrillic Extended-B - combining letters ef, iotified e
    {0xFE20, 0xFE2F},  // FE20..FE2F; Combining Half Marks
}};

#pragma once

#include <array>

struct SeparateLetter {
    char32_t letter;
    char32_t base;
    char32_t mark;
};

// Letters whose mark makes a separate letter of the alphabet, not an accent.
// Stripping it would change the word (мой -> мои), so these are kept as is
inline constexpr std::array<SeparateLetter, 6> separate_letters = {{
    {U'й', U'и', U'\u0306'}, {U'Й', U'И', U'\u0306'},  // short i = и + U+0306 (breve)
    {U'ў', U'у', U'\u0306'}, {U'Ў', U'У', U'\u0306'},  // short u = у + U+0306 (breve), Belarusian
    {U'ї', U'і', U'\u0308'}, {U'Ї', U'І', U'\u0308'},  // yi = і + U+0308 (diaeresis), Ukrainian
}};

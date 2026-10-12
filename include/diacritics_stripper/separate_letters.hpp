#pragma once

#include <array>

// Letters whose mark makes a separate letter of the alphabet, not an accent.
// Stripping it would change the word (мой -> мои), so these are kept as is
inline constexpr std::array<char32_t, 6> separate_letters = {
    U'й', U'Й',  // short i = и + U+0306 (breve)
    U'ў', U'Ў',  // short u = у + U+0306 (breve), Belarusian
    U'ї', U'Ї',  // yi = і + U+0308 (diaeresis), Ukrainian
};

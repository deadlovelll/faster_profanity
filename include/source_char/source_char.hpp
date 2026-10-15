#pragma once

#include <cstddef>

struct SourceChar {
    char32_t code_point;
    std::ptrdiff_t start;
    std::ptrdiff_t end;
};

#pragma once

#include <array>

// fuсk with Cyrillic с -> fuck: letters of other scripts that look like Latin ones become those Latin letters
class ConfusablesMapper {
public:
    // The lowercase Latin letter `code_point` looks like, 0 if none
    static char to_latin(char32_t code_point);
private:
    using Table = std::array<char, 0x10000>;

    static const Table table;

    static Table build_table();
};

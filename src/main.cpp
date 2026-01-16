#include <iostream>
#include <codecvt>
#include <locale>

#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "allowed_characters_loader/allowed_characters_loader.hpp"

std::string to_utf8(const std::u32string& s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}

int main(int argc, char** argv) {
    AllowedCharactersLoader acl("src/data/alphabetic_unicode.json");
    auto res = acl.get_symbols();
    for (auto r: res) {
        std::cout << to_utf8(r) << std::endl;
    }
}
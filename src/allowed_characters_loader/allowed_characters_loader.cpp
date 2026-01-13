#include <fstream>
#include <stdexcept>
#include <string>
#include <cwchar>
#include <clocale>
#include <unordered_set>

#include "allowed_characters_loader/allowed_characters_loader.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unordered_set<char32_t> AllowedCharactersLoader::get_symbols() {
    return symbols;
}

void AllowedCharactersLoader::load_regular_chars() {
    char32_t speciaL_symbols[] = { U'@', U'$', U'*', U'"', U'\'' };
    for (char32_t c = U'a'; c <= U'z'; ++c) symbols.insert(c);
    for (char32_t c = U'A'; c <= U'Z'; ++c) symbols.insert(c);
    for (char32_t c = U'0'; c <= U'9'; ++c) symbols.insert(c);
    for (char32_t c : speciaL_symbols) symbols.insert(c);
}

void AllowedCharactersLoader::load_unicode_json(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot load unicode characters");
    }
    nlohmann::json j; 
    file >> j;

    std::mbstate_t state{};

    for (const auto& item : j) {
        std::string s = item.get<std::string>();
        if (s.empty()) continue;

        char32_t codepoint{};
        const char* ptr = s.data();

        size_t len = std::mbrtoc32(&codepoint, ptr, s.size(), &state);
        if (len == static_cast<size_t>(-1) || len == static_cast<size_t>(-2)) {
            throw std::runtime_error("Invalid UTF-8 sequence");
        }

        symbols.insert(codepoint);
    }
}
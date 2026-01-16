#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>

#include "allowed_characters_loader/allowed_characters_loader.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unordered_set<std::u32string> AllowedCharactersLoader::get_symbols() {
    return symbols;
}

void AllowedCharactersLoader::load_regular_chars() {
    char speciaL_symbols[] = { '@', '$', '*', '"', '\'' };
    for (char c = 'a'; c <= 'z'; ++c) symbols.insert(std::u32string(1, c));
    for (char c = 'A'; c <= 'Z'; ++c) symbols.insert(std::u32string(1, c));
    for (char c = '0'; c <= '9'; ++c) symbols.insert(std::u32string(1, c));
    for (char c : speciaL_symbols) symbols.insert(std::u32string(1, c));
}

void AllowedCharactersLoader::load_unicode_json(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "no file found" << std::endl;
        return;
    }
    nlohmann::json j; 
    file >> j;

    for (const auto& item : j) {
        std::u32string s = item.get<std::u32string>();
        if (s.empty()) continue;
        symbols.insert(s);
    }
}
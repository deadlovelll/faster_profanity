#include "allowed_characters_loader/allowed_characters_loader.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void AllowedCharactersLoader::load_regular_chars(
    std::unordered_set<char32_t>& allowed_characters
) {
    char symbols[] = { U'@', U'$', U'*', U'"', U'\'' };

    for (char c = U'a'; c <= U'z'; ++c) allowed_characters.insert(c);
    for (char c = U'A'; c <= U'Z'; ++c) allowed_characters.insert(c);
    for (char c = U'0'; c <= U'9'; ++c) allowed_characters.insert(c);
    for (char c : symbols) allowed_characters.insert(c);
}

void AllowedCharactersLoader::load_unicode_json(
    std::unordered_set<char32_t>& allowed_characters
) {
    
}

std::unordered_set<char32_t> AllowedCharactersLoader::load() {
    std::unordered_set<char32_t> allowed_characters;
    load_regular_chars(allowed_characters);
    load_unicode_json(allowed_characters);
    return allowed_characters;
}
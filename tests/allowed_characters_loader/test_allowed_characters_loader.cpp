#include <unordered_set>

#include <gtest/gtest.h>

#include "allowed_characters_loader/allowed_characters_loader.hpp"

TEST(AllowedCharactersLoader, LoadsLowercaseLetters) {
    AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
    std::unordered_set<char32_t> result = loader.get_symbols();

    for (char32_t c = U'a'; c <= U'z'; ++c) {
        EXPECT_TRUE(result.contains(c));
    }
}
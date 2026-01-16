// #include <unordered_set>

// #include <gtest/gtest.h>

// #include "allowed_characters_loader/allowed_characters_loader.hpp"

// TEST(AllowedCharactersLoader, LoadsLowercaseLetters) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     for (char32_t c = U'a'; c <= U'z'; ++c) {
//         EXPECT_TRUE(symbols.contains(c)) << "Missing: " << static_cast<int>(c);
//     }
// }

// TEST(AllowedCharactersLoader, LoadsUppercaseLetters) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     for (char32_t c = U'A'; c <= U'Z'; ++c) {
//         EXPECT_TRUE(symbols.contains(c));
//     }
// }

// TEST(AllowedCharactersLoader, LoadsDigits) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     for (char32_t c = U'0'; c <= U'9'; ++c) {
//         EXPECT_TRUE(symbols.contains(c));
//     }
// }

// TEST(AllowedCharactersLoader, LoadsSpecialSymbols) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     EXPECT_TRUE(symbols.contains(U'@'));
//     EXPECT_TRUE(symbols.contains(U'$'));
//     EXPECT_TRUE(symbols.contains(U'*'));
//     EXPECT_TRUE(symbols.contains(U'"'));
//     EXPECT_TRUE(symbols.contains(U'\''));
// }

// TEST(AllowedCharactersLoader, LoadsUnicodeFromJson) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     EXPECT_TRUE(symbols.contains(U'µ'));
//     EXPECT_TRUE(symbols.contains(U'ß'));
//     EXPECT_TRUE(symbols.contains(U'à'));
//     EXPECT_TRUE(symbols.contains(U'á'));
//     EXPECT_TRUE(symbols.contains(U'â'));
// }

// TEST(AllowedCharactersLoader, EmptyJsonDoesNotCrash) {
//     EXPECT_NO_THROW({
//         AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     });
// }

// TEST(AllowedCharactersLoader, ThrowsIfFileDoesNotExist) {
//     EXPECT_THROW(
//         AllowedCharactersLoader("no_such_file.json"),
//         std::runtime_error
//     );
// }

// TEST(AllowedCharactersLoader, NoDuplicateSymbols) {
//     AllowedCharactersLoader loader("src/data/alphabetic_unicode.json");
//     auto symbols = loader.get_symbols();

//     size_t before = symbols.size();
//     symbols.insert(U'µ');
//     EXPECT_EQ(symbols.size(), before);
// }
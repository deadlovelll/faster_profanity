#include <gtest/gtest.h>
#include "text_normalizer/text_normalizer.hpp"

class ToLowerTest : public ::testing::Test, public TextNormalizer {};

TEST_F(ToLowerTest, AllUppercase) {
    std::string text = "HELLO";
    to_lower(text);
    EXPECT_EQ(text, "hello");
}

TEST_F(ToLowerTest, MixedCase) {
    std::string text = "HeLLo WoRLd";
    to_lower(text);
    EXPECT_EQ(text, "hello world");
}

TEST_F(ToLowerTest, AlreadyLowercase) {
    std::string text = "already lowercase";
    to_lower(text);
    EXPECT_EQ(text, "already lowercase");
}

TEST_F(ToLowerTest, NumbersAndSymbolsUnchanged) {
    std::string text = "1234!@#$%^&*()";
    std::string expected = text;
    to_lower(text);
    EXPECT_EQ(text, expected);
}

TEST_F(ToLowerTest, MixedLettersAndSymbols) {
    std::string text = "Sh!Th0u$e @2026";
    to_lower(text);
    EXPECT_EQ(text, "sh!th0u$e @2026");
}

TEST_F(ToLowerTest, EmptyString) {
    std::string text = "";
    to_lower(text);
    EXPECT_EQ(text, "");
}
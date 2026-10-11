#include <gtest/gtest.h>
#include <clocale>
#include <initializer_list>

#include "text_lowerizer/text_lowerizer.hpp"

class TextLowerizerTest : public ::testing::Test {
protected:
    TextLowerizer lowerizer;
};

TEST_F(TextLowerizerTest, AllUppercase) {
    std::string text = "HELLO";
    lowerizer.lower(text);
    EXPECT_EQ(text, "hello");
}

TEST_F(TextLowerizerTest, MixedCase) {
    std::string text = "HeLLo WoRLd";
    lowerizer.lower(text);
    EXPECT_EQ(text, "hello world");
}

TEST_F(TextLowerizerTest, AlreadyLowercase) {
    std::string text = "already lowercase";
    lowerizer.lower(text);
    EXPECT_EQ(text, "already lowercase");
}

TEST_F(TextLowerizerTest, NumbersAndSymbolsUnchanged) {
    std::string text = "1234!@#$%^&*()";
    std::string expected = text;
    lowerizer.lower(text);
    EXPECT_EQ(text, expected);
}

TEST_F(TextLowerizerTest, MixedLettersAndSymbols) {
    std::string text = "Sh!Th0u$e @2026";
    lowerizer.lower(text);
    EXPECT_EQ(text, "sh!th0u$e @2026");
}

TEST_F(TextLowerizerTest, EmptyString) {
    std::string text = "";
    lowerizer.lower(text);
    EXPECT_EQ(text, "");
}

TEST_F(TextLowerizerTest, CyrillicUppercase) {
    std::string text = "БЛЯТЬ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "блять");
}

TEST_F(TextLowerizerTest, CyrillicAndLatinMixed) {
    std::string text = "ПрИвЕт WoRLd";
    lowerizer.lower(text);
    EXPECT_EQ(text, "привет world");
}

TEST_F(TextLowerizerTest, LatinWithDiacriticsKeepsDiacritics) {
    std::string text = "ÜBER ÇA ÉTÉ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "über ça été");
}

TEST_F(TextLowerizerTest, Greek) {
    std::string text = "ΣΟΦΊΑ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "σοφία");
}

TEST_F(TextLowerizerTest, TurkishDottedCapitalI) {
    std::string text = "İ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "i");
}

TEST_F(TextLowerizerTest, ByteLengthChanges) {
    // U+023A (2 bytes) lowercases to U+2C65 (3 bytes)
    std::string text = "Ⱥ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "ⱥ");
}

TEST_F(TextLowerizerTest, FullwidthLettersAreLoweredNotFolded) {
    std::string text = "ＦＵＣＫ";
    lowerizer.lower(text);
    EXPECT_EQ(text, "ｆｕｃｋ");
}

TEST_F(TextLowerizerTest, EmojiAndCjkUnchanged) {
    std::string text = "😀 漢字";
    std::string expected = text;
    lowerizer.lower(text);
    EXPECT_EQ(text, expected);
}

TEST_F(TextLowerizerTest, InvalidUtf8BytesPreserved) {
    // stray continuation byte, invalid 0xFF and a truncated 2-byte sequence at the end
    std::string text = std::string("A\x80" "B\xFF" "C\xC3", 6);
    lowerizer.lower(text);
    EXPECT_EQ(text, std::string("a\x80" "b\xFF" "c\xC3", 6));
}

TEST_F(TextLowerizerTest, AllAsciiCharacters) {
    std::string text;
    std::string expected;
    for (int c = 0; c < 128; ++c) {
        text.push_back(static_cast<char>(c));
        expected.push_back(static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c));
    }
    lowerizer.lower(text);
    EXPECT_EQ(text, expected);
}

TEST_F(TextLowerizerTest, IndependentOfTurkishLocale) {
    const char* previous = std::setlocale(LC_CTYPE, nullptr);
    std::string saved = previous ? previous : "C";

    const char* turkish = nullptr;
    for (const char* name : {"tr_TR.ISO8859-9", "tr_TR.ISO-8859-9", "tr_TR", "tr_TR.UTF-8"}) {
        turkish = std::setlocale(LC_CTYPE, name);
        if (turkish) break;
    }
    if (!turkish) {
        GTEST_SKIP() << "no Turkish locale installed";
    }

    std::string text = "I";
    lowerizer.lower(text);
    std::setlocale(LC_CTYPE, saved.c_str());
    EXPECT_EQ(text, "i");
}

#include <gtest/gtest.h>
#include <clocale>
#include <initializer_list>

#include "unicode_normalizer/unicode_normalizer.hpp"

class UnicodeNormalizerTest : public ::testing::Test {
protected:
    UnicodeNormalizer normalizer;
};

TEST_F(UnicodeNormalizerTest, AllUppercase) {
    std::string text = "HELLO";
    normalizer.normalize(text);
    EXPECT_EQ(text, "hello");
}

TEST_F(UnicodeNormalizerTest, MixedCase) {
    std::string text = "HeLLo WoRLd";
    normalizer.normalize(text);
    EXPECT_EQ(text, "hello world");
}

TEST_F(UnicodeNormalizerTest, AlreadyLowercase) {
    std::string text = "already lowercase";
    normalizer.normalize(text);
    EXPECT_EQ(text, "already lowercase");
}

TEST_F(UnicodeNormalizerTest, NumbersAndSymbolsUnchanged) {
    std::string text = "1234!@#$%^&*()";
    std::string expected = text;
    normalizer.normalize(text);
    EXPECT_EQ(text, expected);
}

TEST_F(UnicodeNormalizerTest, MixedLettersAndSymbols) {
    std::string text = "Sh!Th0u$e @2026";
    normalizer.normalize(text);
    EXPECT_EQ(text, "sh!th0u$e @2026");
}

TEST_F(UnicodeNormalizerTest, EmptyString) {
    std::string text = "";
    normalizer.normalize(text);
    EXPECT_EQ(text, "");
}

TEST_F(UnicodeNormalizerTest, CyrillicUppercase) {
    std::string text = "БЛЯТЬ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "блять");
}

TEST_F(UnicodeNormalizerTest, CyrillicAndLatinMixed) {
    std::string text = "ПрИвЕт WoRLd";
    normalizer.normalize(text);
    EXPECT_EQ(text, "привет world");
}

TEST_F(UnicodeNormalizerTest, LatinWithDiacriticsKeepsDiacritics) {
    std::string text = "ÜBER ÇA ÉTÉ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "über ça été");
}

TEST_F(UnicodeNormalizerTest, Greek) {
    std::string text = "ΣΟΦΊΑ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "σοφία");
}

TEST_F(UnicodeNormalizerTest, TurkishDottedCapitalI) {
    std::string text = "İ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "i");
}

TEST_F(UnicodeNormalizerTest, ByteLengthChanges) {
    // U+023A (2 bytes) lowercases to U+2C65 (3 bytes)
    std::string text = "Ⱥ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "ⱥ");
}

TEST_F(UnicodeNormalizerTest, FullwidthLetters) {
    std::string text = "ＦＵＣＫ ｆｕｃｋ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck");
}

TEST_F(UnicodeNormalizerTest, EmojiAndCjkUnchanged) {
    std::string text = "😀 漢字";
    std::string expected = text;
    normalizer.normalize(text);
    EXPECT_EQ(text, expected);
}

TEST_F(UnicodeNormalizerTest, InvalidUtf8BytesPreserved) {
    // stray continuation byte, invalid 0xFF and a truncated 2-byte sequence at the end
    std::string text = std::string("A\x80" "B\xFF" "C\xC3", 6);
    normalizer.normalize(text);
    EXPECT_EQ(text, std::string("a\x80" "b\xFF" "c\xC3", 6));
}

TEST_F(UnicodeNormalizerTest, AllAsciiCharacters) {
    std::string text;
    std::string expected;
    for (int c = 0; c < 128; ++c) {
        text.push_back(static_cast<char>(c));
        expected.push_back(static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c));
    }
    normalizer.normalize(text);
    EXPECT_EQ(text, expected);
}

TEST_F(UnicodeNormalizerTest, IndependentOfTurkishLocale) {
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
    normalizer.normalize(text);
    std::setlocale(LC_CTYPE, saved.c_str());
    EXPECT_EQ(text, "i");
}

TEST_F(UnicodeNormalizerTest, PlainAsciiUnchanged) {
    std::string text = "fuck you";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck you");
}

TEST_F(UnicodeNormalizerTest, OnlyInvisibleChars) {
    std::string text = "​‌‍﻿";
    normalizer.normalize(text);
    EXPECT_EQ(text, "");
}

TEST_F(UnicodeNormalizerTest, ZeroWidthSpace) {
    std::string text = "f​u​c​k";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, ZeroWidthJoinerAndNonJoiner) {
    std::string text = "f‌uc‍k";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, SoftHyphen) {
    std::string text = "fu­ck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, WordJoiner) {
    std::string text = "sh⁠it";
    normalizer.normalize(text);
    EXPECT_EQ(text, "shit");
}

TEST_F(UnicodeNormalizerTest, ByteOrderMarkAtStartAndInside) {
    std::string text = "﻿fu﻿ck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, BidiEmbeddingsAndOverrides) {
    std::string text = "‪fu‮ck‬";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, BidiIsolatesAndMarks) {
    std::string text = "⁦f‎u‏c⁩k";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, VariationSelectors) {
    std::string text = "f️u︀ck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, VariationSelectorSupplement) {
    std::string text = "fu\U000E0100ck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, TagCharacters) {
    std::string text = "f\U000E0001u\U000E0041ck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, HangulFiller) {
    std::string text = "fㅤuck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, CyrillicAndEmojiKept) {
    std::string text = "сука 😀 漢字";
    std::string expected = text;
    normalizer.normalize(text);
    EXPECT_EQ(text, expected);
}

TEST_F(UnicodeNormalizerTest, EmojiVariationSelectorRemoved) {
    std::string text = "❤️";
    normalizer.normalize(text);
    EXPECT_EQ(text, "❤");
}

TEST_F(UnicodeNormalizerTest, WhitespaceKeptNoBreakSpaceBecomesSpace) {
    std::string text = "a b\tc\nd\u00A0e";
    normalizer.normalize(text);
    EXPECT_EQ(text, "a b\tc\nd e");
}

TEST_F(UnicodeNormalizerTest, InvisibleCharsInsideUppercaseLatin) {
    std::string text = "F\u200BU\u00ADC\u2060K";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, InvisibleCharsInsideUppercaseCyrillic) {
    std::string text = "\uFEFFСУ\u200BКА\u202E";
    normalizer.normalize(text);
    EXPECT_EQ(text, "сука");
}

TEST_F(UnicodeNormalizerTest, MathematicalBoldItalicScript) {
    std::string text = "𝐟𝐮𝐜𝐤 𝙛𝙪𝙘𝙠 𝓯𝓾𝓬𝓴";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck fuck");
}

TEST_F(UnicodeNormalizerTest, MathematicalFrakturDoubleStruckMonospace) {
    std::string text = "𝔣𝔲𝔠𝔨 𝕗𝕦𝕔𝕜 𝚏𝚞𝚌𝚔";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck fuck");
}

TEST_F(UnicodeNormalizerTest, MathematicalBoldCapitalsAreLowered) {
    std::string text = "𝐅𝐔𝐂𝐊";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, CircledLetters) {
    std::string text = "ⓕⓤⓒⓚ ⒻⓊⒸⓀ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck");
}

TEST_F(UnicodeNormalizerTest, SquaredLetters) {
    std::string text = "🄵🅄🄲🄺";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, ParenthesizedLettersLoseParentheses) {
    std::string text = "⒡⒰⒞⒦ 🄕🄤🄒🄚";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck");
}

TEST_F(UnicodeNormalizerTest, ParenthesizedMultiDigitNumberKept) {
    std::string text = "⑽";
    normalizer.normalize(text);
    EXPECT_EQ(text, "(10)");
}

TEST_F(UnicodeNormalizerTest, SuperscriptModifierLetters) {
    std::string text = "ᶠᵘᶜᵏ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, Ligatures) {
    std::string text = "ﬁnger ﬀ ﬆ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "finger ff st");
}

TEST_F(UnicodeNormalizerTest, MiscCompatibilitySymbols) {
    std::string text = "™ ½ ㎏ Ⅳ ① ℌ ſ …";
    normalizer.normalize(text);
    EXPECT_EQ(text, "tm 1\u20442 kg iv 1 h s ...");
}

TEST_F(UnicodeNormalizerTest, SpecialSpacesBecomeSpace) {
    std::string text = "a\u2002b\u3000c";
    normalizer.normalize(text);
    EXPECT_EQ(text, "a b c");
}

TEST_F(UnicodeNormalizerTest, CompatibilityLettersWithInvisibleChars) {
    std::string text = "Ｓ\u200BＨ\u00ADＩＴ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "shit");
}

TEST_F(UnicodeNormalizerTest, TitlecaseDigraphKeepsComposedCaron) {
    std::string text = "ǅ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "d\u017E");
}

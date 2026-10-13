#include <gtest/gtest.h>
#include <clocale>
#include <initializer_list>
#include <vector>

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
    EXPECT_EQ(text, "бляtb");  // Т and Ь look like Latin T and b
}

TEST_F(UnicodeNormalizerTest, CyrillicAndLatinMixed) {
    std::string text = "ПрИвЕт WoRLd";
    normalizer.normalize(text);
    EXPECT_EQ(text, "пpиbet world");  // пpиbet world: р, в, Е, т look like Latin p, b, e, t
}

TEST_F(UnicodeNormalizerTest, LatinDiacriticsStripped) {
    std::string text = "ÜBER ÇA ÉTÉ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "uber ca ete");
}

TEST_F(UnicodeNormalizerTest, Greek) {
    std::string text = "ΣΟΦΊΑ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "ooφia");  // ooφia: σ, Ο, ι, Α look like Latin o, o, i, a
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
    std::string text = "дядя 😀 漢字";
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
    EXPECT_EQ(text, "cyka");  // С, У, К, А look like Latin C, y, K, A
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

TEST_F(UnicodeNormalizerTest, SeparatorsAndBrailleBlankBecomeSpace) {
    std::string text = "a\u2000b\u1680c\u2028d\u2029e\u2800f";
    normalizer.normalize(text);
    EXPECT_EQ(text, "a b c d e f");
}

TEST_F(UnicodeNormalizerTest, CompatibilityLettersWithInvisibleChars) {
    std::string text = "Ｓ\u200BＨ\u00ADＩＴ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "shit");
}

TEST_F(UnicodeNormalizerTest, TitlecaseDigraphLosesCaron) {
    std::string text = "ǅ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "dz");
}

TEST_F(UnicodeNormalizerTest, SharpS) {
    std::string text = "ß ẞ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "ss ss");
}

TEST_F(UnicodeNormalizerTest, SharpSInsideWords) {
    std::string text = "STRAẞE Straße";
    normalizer.normalize(text);
    EXPECT_EQ(text, "strasse strasse");
}

TEST_F(UnicodeNormalizerTest, GreekFinalSigma) {
    std::string text = "ΚΟΣΜΟΣ κόσμος";
    normalizer.normalize(text);
    // koomoo koomoo: Σ and ς fold to σ, which looks like o. Κ and Μ look like K and M, small κ and μ follow them
    EXPECT_EQ(text, "koomoo koomoo");
}

TEST_F(UnicodeNormalizerTest, GreekCapitalWithProsgegrammeni) {
    std::string text = "ᾈ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "a");
}

TEST_F(UnicodeNormalizerTest, CherokeeFoldsToCapitals) {
    std::string text = "ꭰ Ꭰ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "d d");  // both fold to Ꭰ, which looks like Latin D
}

TEST_F(UnicodeNormalizerTest, KelvinAndAngstromSigns) {
    std::string text = "\u212A \u212B";
    normalizer.normalize(text);
    EXPECT_EQ(text, "k a");
}

TEST_F(UnicodeNormalizerTest, AccentedObfuscation) {
    std::string text = "fùck fúck fûck fück fŭck fǔck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck fuck fuck fuck fuck");
}

TEST_F(UnicodeNormalizerTest, MultipleMarksOnOneLetter) {
    std::string text = "ǖ ǛȐ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "u ur");
}

TEST_F(UnicodeNormalizerTest, CyrillicYoBecomesYe) {
    std::string text = "ЁБАНЫЙ ёб";
    normalizer.normalize(text);
    EXPECT_EQ(text, "eбahый eб");  // eбahый eб: е, А, Н look like Latin e, A, H
}

TEST_F(UnicodeNormalizerTest, CyrillicLettersWithMarkKept) {
    std::string text = "хуй ХУЙ ў Ў ї Ї";
    normalizer.normalize(text);
    // xyй xyй ў ў ї ї: х and у look like Latin x and y, й, ў, ї keep their marks
    EXPECT_EQ(text, "xyй xyй ў ў ї ї");
}

TEST_F(UnicodeNormalizerTest, DecomposedCyrillicLettersWithMarkKept) {
    std::string text = "хуи\u0306 ХУИ\u0306 у\u0306 У\u0306 і\u0308 І\u0308";
    normalizer.normalize(text);
    EXPECT_EQ(text, "xyй xyй ў ў ї ї");
}

TEST_F(UnicodeNormalizerTest, SourceOffsetsPointToOriginalCharacters) {
    std::string text = "Ｆu​ß";
    std::vector<std::size_t> source_offsets;
    normalizer.normalize(text, source_offsets);
    EXPECT_EQ(text, "fuss");
    EXPECT_EQ(source_offsets, (std::vector<std::size_t>{0, 3, 7, 7, 9}));

    std::string decomposed = "хй";
    normalizer.normalize(decomposed, source_offsets);
    EXPECT_EQ(source_offsets, (std::vector<std::size_t>{0, 2, 2, 6}));
}

TEST_F(UnicodeNormalizerTest, StrikethroughAndUnderline) {
    std::string text = "f\u0336u\u0336c\u0336k\u0336 s\u0332h\u0332i\u0332t\u0332";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck shit");
}

TEST_F(UnicodeNormalizerTest, ZalgoText) {
    std::string text = "f\u0315\u0321\u0358u\u035Ec\u0489\u0483k";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, DecomposedAccentsStripped) {
    std::string text = "fu\u0300ck E\u0301TE\u0301";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck ete");
}

TEST_F(UnicodeNormalizerTest, IndicMarksKept) {
    std::string text = "चूतिया क़";
    normalizer.normalize(text);
    EXPECT_EQ(text, "चूतिया क़");
}

TEST_F(UnicodeNormalizerTest, HangulUnchanged) {
    std::string text = "한국어";
    normalizer.normalize(text);
    EXPECT_EQ(text, "한국어");
}

TEST_F(UnicodeNormalizerTest, ChurchSlavonicCombiningMarks) {
    std::string text = "сⷠу꙰к꙼аꚞ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "cyka");  // cyka: с, у, к, а look like Latin c, y, k, a
}

TEST_F(UnicodeNormalizerTest, CyrillicLookalikeInLatinWord) {
    std::string text = "fuсk";  // Cyrillic с
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, UppercaseCyrillicLookalikes) {
    std::string text = "ВІТСН";  // ВІТСН, all Cyrillic
    normalizer.normalize(text);
    EXPECT_EQ(text, "bitch");
}

TEST_F(UnicodeNormalizerTest, GreekLookalikes) {
    std::string text = "ΤΙΤ";  // ΤΙΤ, all Greek
    normalizer.normalize(text);
    EXPECT_EQ(text, "tit");
}

TEST_F(UnicodeNormalizerTest, LookalikeWithDiacritic) {
    std::string text = "fùсk";  // ù and Cyrillic с
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, LookalikeAfterCaseFolding) {
    std::string text = "ꭰ";  // Cherokee small letter a folds to Ꭰ, which looks like D
    normalizer.normalize(text);
    EXPECT_EQ(text, "d");
}

TEST_F(UnicodeNormalizerTest, CyrillicWordWithoutLookalikesUnchanged) {
    std::string text = "дядя";
    normalizer.normalize(text);
    EXPECT_EQ(text, "дядя");
}

TEST_F(UnicodeNormalizerTest, LookalikesIndependentOfCase) {
    std::string text = "сука СУКА Сука";
    normalizer.normalize(text);
    EXPECT_EQ(text, "cyka cyka cyka");
}

TEST_F(UnicodeNormalizerTest, SmallCapitals) {
    std::string text = "ꜰᴜᴄᴋ";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, NegativeCircledLetters) {
    std::string text = "🅕🅤🅒🅚";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, NegativeSquaredLetters) {
    std::string text = "🅵🆄🅲🅺";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, RegionalIndicatorsSeparatedByInvisibleChars) {
    std::string text = "\U0001F1EB​\U0001F1FA​\U0001F1E8​\U0001F1F0";  // 🇫 🇺 🇨 🇰 split by ZWSP
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, RegionalIndicatorsSeparatedBySpaces) {
    std::string text = "\U0001F1EB \U0001F1FA \U0001F1E8 \U0001F1F0";  // 🇫 🇺 🇨 🇰
    normalizer.normalize(text);
    EXPECT_EQ(text, "f u c k");
}

TEST_F(UnicodeNormalizerTest, RegionalIndicatorNextToLetters) {
    std::string text = "\U0001F1EBuck";  // 🇫uck
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck");
}

TEST_F(UnicodeNormalizerTest, FlagsKept) {
    std::string text = "\U0001F1FA\U0001F1F8 \U0001F1E6\U0001F1F7\U0001F1F8\U0001F1EA";  // 🇺🇸 🇦🇷🇸🇪, letters would spell arse
    std::string expected = text;
    normalizer.normalize(text);
    EXPECT_EQ(text, expected);
}

TEST_F(UnicodeNormalizerTest, SpacingAccentsInsideWord) {
    std::string text = "f\u00B4uck fu\u02DCck";
    normalizer.normalize(text);
    EXPECT_EQ(text, "fuck fuck");
}

TEST_F(UnicodeNormalizerTest, CaseFoldingLosesDiacritics) {
    std::string text = "\u01F0erk \u1E97wat \u0390";
    normalizer.normalize(text);
    EXPECT_EQ(text, "jerk twat i");
}

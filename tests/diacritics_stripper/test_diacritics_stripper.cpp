#include <gtest/gtest.h>

#include "diacritics_stripper/diacritics_stripper.hpp"

class DiacriticsStripperTest : public ::testing::Test {
protected:
    DiacriticsStripper stripper;
};

TEST_F(DiacriticsStripperTest, AccentedLetter) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'é', stripped), 1);
    EXPECT_EQ(stripped[0], U'e');
}

TEST_F(DiacriticsStripperTest, UppercaseAccentedLetterIsLowered) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'Ё', stripped), 1);
    EXPECT_EQ(stripped[0], U'е');
}

TEST_F(DiacriticsStripperTest, SeveralMarksOnOneLetter) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'ǖ', stripped), 1);
    EXPECT_EQ(stripped[0], U'u');
}

TEST_F(DiacriticsStripperTest, LettersWithSeparateMarkKept) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'Й', stripped), 1);
    EXPECT_EQ(stripped[0], U'й');
    ASSERT_EQ(stripper.strip(U'ў', stripped), 1);
    EXPECT_EQ(stripped[0], U'ў');
    ASSERT_EQ(stripper.strip(U'ї', stripped), 1);
    EXPECT_EQ(stripped[0], U'ї');
}

TEST_F(DiacriticsStripperTest, SeparateLetterComposedFromMark) {
    EXPECT_EQ(stripper.compose_separate_letter(U'и', U'\u0306'), U'й');
    EXPECT_EQ(stripper.compose_separate_letter(U'У', U'\u0306'), U'Ў');
    EXPECT_EQ(stripper.compose_separate_letter(U'і', U'\u0308'), U'ї');
    EXPECT_EQ(stripper.compose_separate_letter(U'е', U'\u0308'), U'\0');
}

TEST_F(DiacriticsStripperTest, StandaloneMarkRemoved) {
    char32_t stripped[4];
    EXPECT_EQ(stripper.strip(U'́', stripped), 0);
    EXPECT_EQ(stripper.strip(U'̶', stripped), 0);
}

TEST_F(DiacriticsStripperTest, LetterWithoutDiacriticsKept) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'क़', stripped), 1);  // क़: nukta is not a diacritic
    EXPECT_EQ(stripped[0], U'क़');
}

TEST_F(DiacriticsStripperTest, CharacterOutsideTable) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'Å', stripped), 1);  // Å angstrom sign: A + ring above
    EXPECT_EQ(stripped[0], U'a');
}

TEST_F(DiacriticsStripperTest, DiacriticMarkRangeBoundaries) {
    EXPECT_FALSE(stripper.is_diacritic_mark(U'˿'));
    EXPECT_TRUE(stripper.is_diacritic_mark(U'̀'));
    EXPECT_TRUE(stripper.is_diacritic_mark(U'ͯ'));
    EXPECT_FALSE(stripper.is_diacritic_mark(U'Ͱ'));
    EXPECT_TRUE(stripper.is_diacritic_mark(U'︯'));
    EXPECT_FALSE(stripper.is_diacritic_mark(U'︰'));
    EXPECT_FALSE(stripper.is_diacritic_mark(U'़'));  // Devanagari nukta
}

TEST_F(DiacriticsStripperTest, CompatibilityLetterWithDiacritic) {
    char32_t stripped[4];
    ASSERT_EQ(stripper.strip(U'ǆ', stripped), 2);
    EXPECT_EQ(stripped[0], U'd');
    EXPECT_EQ(stripped[1], U'z');
}

TEST_F(DiacriticsStripperTest, SpacingAccentRemoved) {
    char32_t stripped[4];
    EXPECT_EQ(stripper.strip(U'´', stripped), 0);
}

#include <gtest/gtest.h>

#include "char_classifier/char_classifier.hpp"

class CharClassifierTest : public ::testing::Test {
protected:
    CharClassifier classifier;
};

TEST_F(CharClassifierTest, RegularLetters) {
    EXPECT_EQ(classifier.classify(U'a'), CharKind::Regular);
    EXPECT_EQ(classifier.classify(U'Ж'), CharKind::Regular);
    EXPECT_EQ(classifier.classify(U'漢'), CharKind::Regular);
}

TEST_F(CharClassifierTest, InvisibleCharacters) {
    EXPECT_EQ(classifier.classify(U'​'), CharKind::Invisible);  // zero width space
    EXPECT_EQ(classifier.classify(U'­'), CharKind::Invisible);  // soft hyphen
}

TEST_F(CharClassifierTest, CompatibilityCharacters) {
    EXPECT_EQ(classifier.classify(U'Ｆ'), CharKind::Compatibility);
    EXPECT_EQ(classifier.classify(U'ﬁ'), CharKind::Compatibility);
}

TEST_F(CharClassifierTest, FoldDiffersFromLowercase) {
    EXPECT_EQ(classifier.classify(U'ß'), CharKind::CaseFolding);
}

TEST_F(CharClassifierTest, AccentedLetters) {
    EXPECT_EQ(classifier.classify(U'é'), CharKind::Diacritic);
    EXPECT_EQ(classifier.classify(U'Ё'), CharKind::Diacritic);
}

TEST_F(CharClassifierTest, StandaloneMarks) {
    EXPECT_EQ(classifier.classify(U'́'), CharKind::Diacritic);  // combining acute accent
    EXPECT_EQ(classifier.classify(U'̶'), CharKind::Diacritic);  // combining long stroke overlay
    EXPECT_EQ(classifier.classify(U'⃝'), CharKind::Diacritic);  // combining enclosing circle
}

TEST_F(CharClassifierTest, IndicMarkIsRegular) {
    EXPECT_EQ(classifier.classify(U'़'), CharKind::Regular);  // Devanagari nukta
}

#include <gtest/gtest.h>
#include "text_normalizer/text_normalizer.hpp"

class TestRemoveLetterFillerssormalizer : public ::testing::Test, public TextNormalizer {};

TEST_F(TestRemoveLetterFillerssormalizer, DoesNotRemoveSingleSubsChar) {
    std::string text = "p!orn";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "p!orn");
}

TEST_F(TestRemoveLetterFillerssormalizer, RemoveDoubleSubsChar) {
    std::string text = "p!o!rn";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "porn");
}

TEST_F(TestRemoveLetterFillerssormalizer, RemoveSubsCharWithBoundaries) {
    std::string text = "!p!o!r!n!";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "porn");
}

TEST_F(TestRemoveLetterFillerssormalizer, DoesNotRemoveSingleSubsCharInShithouse) {
    std::string text = "sh!thouse";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "sh!thouse");
}

TEST_F(TestRemoveLetterFillerssormalizer, RemoveDoubleSubsCharInShithouse) {
    std::string text = "sh!t!house";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "shthouse");
}

TEST_F(TestRemoveLetterFillerssormalizer, RemoveSubsChar) {
    std::string text = "p!o!r!n";
    remove_letter_fillers(text);
    EXPECT_EQ(text, "porn");
}
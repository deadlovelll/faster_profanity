#include <gtest/gtest.h>
#include "text_trimmer/text_trimmer.hpp"

TEST(TextTrimmerTest, TrimsTextFromLeft) {
    TextTrimmer tt;
    std::string text = "   Hello";

    tt.trim(text);

    EXPECT_EQ(text, "Hello");
}

TEST(TextTrimmerTest, TrimsTextFromRight) {
    TextTrimmer tt;
    std::string text = "Hello   ";

    tt.trim(text);

    EXPECT_EQ(text, "Hello");
}

TEST(TextTrimmerTest, TrimsTextFromRightAndLeft) {
    TextTrimmer tt;
    std::string text = "   Hello   ";

    tt.trim(text);

    EXPECT_EQ(text, "Hello");
}

TEST(TextTrimmerTest, TrimsTextFromLeftMultipleWords) {
    TextTrimmer tt;
    std::string text = "   Hello     world";

    tt.trim(text);

    EXPECT_EQ(text, "Hello     world");
}

TEST(TextTrimmerTest, TrimsTextFromRightMultipleWords) {
    TextTrimmer tt;
    std::string text = "Hello     world              ";

    tt.trim(text);

    EXPECT_EQ(text, "Hello     world");
}

TEST(TextTrimmerTest, TrimsTextFromRightAndLeftMultipleWords) {
    TextTrimmer tt;
    std::string text = "                Hello     world              ";

    tt.trim(text);

    EXPECT_EQ(text, "Hello     world");
}
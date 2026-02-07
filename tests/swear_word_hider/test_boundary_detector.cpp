#include <gtest/gtest.h>
#include "swear_word_hider/swear_word_hider.hpp"

class SwearWordHiderTest : public ::testing::Test, public SwearWordHider {
};

TEST_F(SwearWordHiderTest, WordAtStartOfString) {
    std::string text = "house is nice";
    EXPECT_TRUE(is_word_boundary(text, 0, 5));
}

TEST_F(SwearWordHiderTest, WordAtEndOfString) {
    std::string text = "I live in a house";
    EXPECT_TRUE(is_word_boundary(text, 12, 17));
}

TEST_F(SwearWordHiderTest, WordInMiddleWithSpaces) {
    std::string text = "I see a house here";
    EXPECT_TRUE(is_word_boundary(text, 8, 13));
}

TEST_F(SwearWordHiderTest, WordWithPunctuation) {
    std::string text = "That is a house!";
    EXPECT_TRUE(is_word_boundary(text, 10, 15));
}

TEST_F(SwearWordHiderTest, WordInsideAnotherWord) {
    std::string text = "penthouse";
    EXPECT_FALSE(is_word_boundary(text, 4, 9));
}

TEST_F(SwearWordHiderTest, WordWithUnderscore) {
    std::string text = "this_is_house";
    EXPECT_FALSE(is_word_boundary(text, 8, 13));
}

TEST_F(SwearWordHiderTest, WordSurroundedByNonAlphaNumeric) {
    std::string text = "(house)";
    EXPECT_TRUE(is_word_boundary(text, 1, 6));
}

TEST_F(SwearWordHiderTest, SingleCharacterWord) {
    std::string text = "a b c";
    EXPECT_TRUE(is_word_boundary(text, 2, 3));
}

TEST_F(SwearWordHiderTest, WordWithComma) {
    std::string text = "Hello, house is here";
    EXPECT_TRUE(is_word_boundary(text, 7, 12)); 
}

TEST_F(SwearWordHiderTest, WordWithDot) {
    std::string text = "I live in a house.";
    EXPECT_TRUE(is_word_boundary(text, 12, 17)); 
}

TEST_F(SwearWordHiderTest, WordWithMultipleSpaces) {
    std::string text = "I  see  house  here";
    EXPECT_TRUE(is_word_boundary(text, 8, 13)); 
}

TEST_F(SwearWordHiderTest, WordWithNumbersInside) {
    std::string text = "house123 is not censored";
    EXPECT_FALSE(is_word_boundary(text, 0, 9)); 
}

TEST_F(SwearWordHiderTest, SingleLetterWordWithPunctuation) {
    std::string text = "A! b? c.";
    EXPECT_TRUE(is_word_boundary(text, 0, 1)); 
    EXPECT_TRUE(is_word_boundary(text, 3, 4)); 
    EXPECT_TRUE(is_word_boundary(text, 6, 7));
}

// TEST_F(SwearWordHiderTest, UTF8CharactersBeforeAndAfter) {
//     std::string text = u8"Привет house мир";
//     EXPECT_TRUE(is_word_boundary(text, 7, 12)); 
// }

TEST_F(SwearWordHiderTest, WordAtStartWithPunctuation) {
    std::string text = "\"house\" is nice";
    EXPECT_TRUE(is_word_boundary(text, 1, 6)); 
}

// TEST_F(SwearWordHiderTest, WordAtEndWithPunctuation) {
//     std::string text = "I live in a \"house\"";
//     EXPECT_TRUE(is_word_boundary(text, 12, 17)); 
// }

TEST_F(SwearWordHiderTest, WordWithHyphenInside) {
    std::string text = "shithouse is bad";
    EXPECT_FALSE(is_word_boundary(text, 0, 10));
}

// TEST_F(SwearWordHiderTest, MultipleWordsTogether) {
//     std::string text = "house penthouse cathouse";
//     EXPECT_TRUE(is_word_boundary(text, 0, 5));  
//     EXPECT_FALSE(is_word_boundary(text, 6, 15));
//     EXPECT_FALSE(is_word_boundary(text, 16, 25)); 
// }

TEST_F(SwearWordHiderTest, WordWithQuotes) {
    std::string text = "'house' is here";
    EXPECT_TRUE(is_word_boundary(text, 1, 6)); 
}
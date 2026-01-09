#include <gtest/gtest.h>
#include "word_finder/word_finder.hpp"

TEST(WordFinderTest, FindsNextWord) {
    WordFinder wf;
    std::string text = "Hello world";

    auto result = (
        wf.get_next_word_and_end_index(text, 0)
    );
    EXPECT_EQ(std::get<0>(result), "Hello");
    EXPECT_EQ(std::get<1>(result), 5);
}

TEST(WordFinderTest, FindsNextWordSecond) {
    WordFinder wf;
    std::string text = "Stupid Faggot";

    auto result = (
        wf.get_next_word_and_end_index(text, 0)
    );
    EXPECT_EQ(std::get<0>(result), "Stupid");
    EXPECT_EQ(std::get<1>(result), 6);
}

TEST(WordFinderTest, FindsNextWordStartIndex) {
    WordFinder wf;
    std::string text = "Stupid Faggot";

    auto result = (
        wf.get_next_word_and_end_index(text, 0)
    );
    EXPECT_EQ(std::get<0>(result), "Stupid");
    EXPECT_EQ(std::get<1>(result), 6);
}
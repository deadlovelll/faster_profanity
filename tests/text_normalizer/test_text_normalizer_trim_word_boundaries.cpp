#include <gtest/gtest.h>
#include "text_normalizer/text_normalizer.hpp"

class TestNormalizer : public ::testing::Test, public TextNormalizer {};

TEST_F(TestNormalizer, RemovesPunctuationAroundSingleWord) {
    std::string text = "!!porn!!";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "porn");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAround) {
    std::string text = "!!porn!! !!sex!!";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "porn sex");
}

TEST_F(TestNormalizer, PreservesInnerSymbols) {
    std::string text = "sh!thouse";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse");
}

TEST_F(TestNormalizer, PreservesInnerSymbolsAndTrailingOnes) {
    std::string text = "!!sh!thouse!!";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse");
}

TEST_F(TestNormalizer, PreservesInnerSymbolsWithNum) {
    std::string text = "11sh!thouse11";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "11sh!thouse11");
}

TEST_F(TestNormalizer, PreservesInnerSymbolsWithDollarSign) {
    std::string text = "$$sh!thouse$$";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse");
}

TEST_F(TestNormalizer, PreservesInnerSymbolsWithAnSign) {
    std::string text = "@nal";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "@nal");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithExclamationPoint) {
    std::string text = "!!sh!thouse!! !!a$$hole!!";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithDot) {
    std::string text = "..sh!thouse.. ..a$$hole..";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithComma) {
    std::string text = ",,sh!thouse,, ,,a$$hole,,";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithColon) {
    std::string text = "::sh!thouse:: ::a$$hole::";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithSemiColon) {
    std::string text = ";;sh!thouse;; ;;a$$hole;;";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithQuestionMark) {
    std::string text = "??sh!thouse?? ??a$$hole??";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithAtMark) {
    std::string text = "@@sh!thouse@@ @@a$$hole@@";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithHashtag) {
    std::string text = "##sh!thouse## ##a$$hole##";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithCaret) {
    std::string text = "^^sh!thouse^^ ^^a$$hole^^";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithAnd) {
    std::string text = "&&sh!thouse&& &&a$$hole&&";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithStarSign) {
    std::string text = "**sh!thouse** **a$$hole**";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithLeftBracets) {
    std::string text = "((sh!thouse(( ((a$$hole((";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithRightBracets) {
    std::string text = "))sh!thouse)) ))a$$hole))";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithMinusSign) {
    std::string text = "--sh!thouse-- --a$$hole--";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithPlusSign) {
    std::string text = "++sh!thouse++ ++a$$hole++";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsAndTrailingOnesWithEqualSign) {
    std::string text = "==sh!thouse== ==a$$hole==";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithBrackets) {
    std::string text = "[]sh!thouse[] {}a$$hole{}";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithQuotes) {
    std::string text = "\"sh!thouse\" 'a$$hole'";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithSlashes) {
    std::string text = "/sh!thouse/ \\a$$hole\\";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithPipes) {
    std::string text = "|sh!thouse| |a$$hole|";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithAngles) {
    std::string text = "<sh!thouse> <a$$hole>";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, MultipleWordsWithGarbageAroundAndInnerSymbolsWithTildesAndBackticks) {
    std::string text = "~sh!thouse~ `a$$hole`";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "sh!thouse a$$hole");
}

TEST_F(TestNormalizer, HandlesLeadingAndTrailingSpaces) {
    std::string text = "   !!!bad_word!!!   ";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "bad_word");
}

TEST_F(TestNormalizer, OnlyPunctuationResultsInEmptyString) {
    std::string text = "!!!@@@###";
    trim_word_boundaries(text);
    EXPECT_TRUE(text.empty());
}

TEST_F(TestNormalizer, AlreadyCleanTextUnchanged) {
    std::string text = "porn sex drugs";
    trim_word_boundaries(text);
    EXPECT_EQ(text, "porn sex drugs");
}

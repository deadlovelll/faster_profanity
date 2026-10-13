#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "tokenizer/tokenizer.hpp"

class TokenizerTest : public ::testing::Test {
protected:
    Tokenizer tokenizer;

    std::vector<std::string> texts(const std::string& text) {
        std::vector<std::string> result;
        for (const Token& token : tokenizer.tokenize(text)) {
            result.push_back(token.text);
        }
        return result;
    }
};

TEST_F(TokenizerTest, WordsWithPositions) {
    std::vector<Token> tokens = tokenizer.tokenize("hello, world!");
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].text, "hello");
    EXPECT_EQ(tokens[0].start, 0u);
    EXPECT_EQ(tokens[0].end, 5u);
    EXPECT_EQ(tokens[1].text, "world");
    EXPECT_EQ(tokens[1].start, 7u);
    EXPECT_EQ(tokens[1].end, 12u);
}

TEST_F(TokenizerTest, NonLatinWordsWithBytePositions) {
    std::vector<Token> tokens = tokenizer.tokenize("привет мир");
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].text, "привет");
    EXPECT_EQ(tokens[0].start, 0u);
    EXPECT_EQ(tokens[0].end, 12u);
    EXPECT_EQ(tokens[1].text, "мир");
    EXPECT_EQ(tokens[1].start, 13u);
    EXPECT_EQ(tokens[1].end, 19u);
}

TEST_F(TokenizerTest, MixedScriptWordIsOneToken) {
    EXPECT_EQ(texts("wopты"), (std::vector<std::string>{"wopты"}));
}

TEST_F(TokenizerTest, PunctuationHyphensAndUnderscoresSplitWords) {
    EXPECT_EQ(texts("-fuck- _shit_ (ass) arse-licking"), (std::vector<std::string>{"fuck", "shit", "ass", "arse", "licking"}));
}

TEST_F(TokenizerTest, DigitsStayInsideWords) {
    EXPECT_EQ(texts("c0ck 2010 a55"), (std::vector<std::string>{"c0ck", "2010", "a55"}));
}

TEST_F(TokenizerTest, EmptyAndSeparatorsOnly) {
    EXPECT_TRUE(tokenizer.tokenize("").empty());
    EXPECT_TRUE(tokenizer.tokenize(" ,.!\n").empty());
}

TEST_F(TokenizerTest, BrokenUtf8SplitsWords) {
    EXPECT_EQ(texts("ab\xFF" "cd"), (std::vector<std::string>{"ab", "cd"}));
}

TEST_F(TokenizerTest, SpacedLettersJoined) {
    for (const std::string& text : {"f u c k", "f.u.c.k", "f-u-c-k", "f_u_c_k", "f*u*c*k"}) {
        std::vector<Token> tokens = tokenizer.tokenize(text);
        ASSERT_EQ(tokens.size(), 1u) << text;
        EXPECT_EQ(tokens[0].text, "fuck") << text;
        EXPECT_EQ(tokens[0].start, 0u) << text;
        EXPECT_EQ(tokens[0].end, 7u) << text;
    }
}

TEST_F(TokenizerTest, SpacedLettersInsideSentence) {
    std::vector<Token> tokens = tokenizer.tokenize("you f u c k off");
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0].text, "you");
    EXPECT_EQ(tokens[1].text, "fuck");
    EXPECT_EQ(tokens[1].start, 4u);
    EXPECT_EQ(tokens[1].end, 11u);
    EXPECT_EQ(tokens[2].text, "off");
}

TEST_F(TokenizerTest, SpacedCyrillicLettersJoined) {
    EXPECT_EQ(texts("с у к а"), (std::vector<std::string>{"сука"}));
}

TEST_F(TokenizerTest, TwoSpacedLettersNotJoined) {
    EXPECT_EQ(texts("a b"), (std::vector<std::string>{"a", "b"}));
}

TEST_F(TokenizerTest, MixedOrDoubleSeparatorsNotJoined) {
    EXPECT_EQ(texts("f.u-c k"), (std::vector<std::string>{"f", "u", "c", "k"}));
    EXPECT_EQ(texts("f  u  c  k"), (std::vector<std::string>{"f", "u", "c", "k"}));
}

TEST_F(TokenizerTest, JoinSeparatesWordsWithSingleSpaces) {
    EXPECT_EQ(tokenizer.join(tokenizer.tokenize("arse-licking,  you f u c k")), "arse licking you fuck");
    EXPECT_EQ(tokenizer.join(tokenizer.tokenize("")), "");
}

TEST_F(TokenizerTest, SpacedDigitsAndLongerPiecesNotJoined) {
    EXPECT_EQ(texts("1 2 3"), (std::vector<std::string>{"1", "2", "3"}));
    EXPECT_EQ(texts("fu ck"), (std::vector<std::string>{"fu", "ck"}));
}

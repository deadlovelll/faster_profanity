#include <gtest/gtest.h>
#include "text_normalizer/text_normalizer.hpp"

class TextNormalizerTest : public ::testing::Test {
protected:
    TextNormalizer normalizer;
};

// Тест обычной нормализации
TEST_F(TextNormalizerTest, BasicNormalization) {
    std::string input = "Hello World!";
    std::string expected = "hello world";
    std::string result = normalizer.normalize(input);
    EXPECT_EQ(result, expected) << "output is " << result;
}

// Тест с повторяющимися буквами
TEST_F(TextNormalizerTest, RepeatedLetters) {
    std::string input = "Soooo cooool";
    std::string expected = "soo cool"; // max_repeat по умолчанию = 2
    EXPECT_EQ(normalizer.normalize(input), expected);
}

// Тест с соединительными символами
TEST_F(TextNormalizerTest, ConnectorsAreIgnored) {
    std::string input = "shit-house_is.cool";
    std::string expected = "shithouseiscool";
    EXPECT_EQ(normalizer.normalize(input), expected);
}

// Тест с subsymbol-символами
TEST_F(TextNormalizerTest, SubsymbolsAreKept) {
    std::string input = "h€llø@wørld";
    std::string expected = "h€llø@wørld";
    EXPECT_EQ(normalizer.normalize(input), expected);
}

// Тест с разными разделителями слов
TEST_F(TextNormalizerTest, SpacesAndPunctuation) {
    std::string input = "hello, world! this-is_a.test";
    std::string expected = "hello world thisisatest";
    EXPECT_EQ(normalizer.normalize(input), expected);
}

// Тест с max_repeat = 3
TEST_F(TextNormalizerTest, CustomMaxRepeat) {
    std::string input = "yeeeees";
    std::string expected = "yeeees";
    EXPECT_EQ(normalizer.normalize(input, 3), expected);
}


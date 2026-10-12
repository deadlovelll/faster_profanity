#include <gtest/gtest.h>

#include "confusables_mapper/confusables_mapper.hpp"

TEST(ConfusablesMapperTest, CyrillicLookalikes) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'с'), 'c');
    EXPECT_EQ(ConfusablesMapper::to_latin(U'а'), 'a');
    EXPECT_EQ(ConfusablesMapper::to_latin(U'Р'), 'p');
}

TEST(ConfusablesMapperTest, LookalikeOnlyInUppercase) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'В'), 'b');
    EXPECT_EQ(ConfusablesMapper::to_latin(U'в'), 0);
}

TEST(ConfusablesMapperTest, UppercaseLookalikesOfIAreI) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'І'), 'i');  // Ukrainian I
    EXPECT_EQ(ConfusablesMapper::to_latin(U'Ι'), 'i');  // Greek iota
    EXPECT_EQ(ConfusablesMapper::to_latin(U'Ꮮ'), 'l');  // Cherokee TLE looks like L, not I
}

TEST(ConfusablesMapperTest, GreekLookalikes) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'Η'), 'h');
    EXPECT_EQ(ConfusablesMapper::to_latin(U'ν'), 'v');
}

TEST(ConfusablesMapperTest, NoLookalike) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'a'), 0);
    EXPECT_EQ(ConfusablesMapper::to_latin(U'ж'), 0);
    EXPECT_EQ(ConfusablesMapper::to_latin(U'漢'), 0);
    EXPECT_EQ(ConfusablesMapper::to_latin(U'😀'), 0);
}

TEST(ConfusablesMapperTest, BeyondBasicMultilingualPlane) {
    EXPECT_EQ(ConfusablesMapper::to_latin(U'\U0001042C'), 'o');  // Deseret small long o
    EXPECT_EQ(ConfusablesMapper::to_latin(U'\U000118C0'), 'v');  // Warang Citi small ngaa
}

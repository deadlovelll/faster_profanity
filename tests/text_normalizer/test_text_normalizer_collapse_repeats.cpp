#include <gtest/gtest.h>
#include "text_normalizer/text_normalizer.hpp"

class TestCollapseRepeatsNormalizer : public ::testing::Test, public TextNormalizer {};

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingCharInStart) {
    std::string text = "ppppppppppppporn";
    collapse_repeats(text);
    EXPECT_EQ(text, "pporn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingCharInEnd) {
    std::string text = "pornnnnnnnnnnnnnnnnn";
    collapse_repeats(text);
    EXPECT_EQ(text, "pornn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingCharInMiddle) {
    std::string text = "poooooooooooooooooorn";
    collapse_repeats(text);
    EXPECT_EQ(text, "poorn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingCharWholeWord) {
    std::string text = "ppppppppppppppooooooooooooorrrrrrrrrrrrrrrrrrnnnnnnnnnnnnnnnn";
    collapse_repeats(text);
    EXPECT_EQ(text, "ppoorrnn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerInMiddle) {
    std::string text = "po...................................rn";
    collapse_repeats(text);
    EXPECT_EQ(text, "po..rn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerInEnd) {
    std::string text = "por...................................n";
    collapse_repeats(text);
    EXPECT_EQ(text, "por..n");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerInStart) {
    std::string text = "p...................................orn";
    collapse_repeats(text);
    EXPECT_EQ(text, "p..orn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerAfterWord) {
    std::string text = "porn...................................";
    collapse_repeats(text);
    EXPECT_EQ(text, "porn..");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerBeforeWord) {
    std::string text = "...................................porn";
    collapse_repeats(text);
    EXPECT_EQ(text, "..porn");
}

TEST_F(TestCollapseRepeatsNormalizer, RemovesRepeatingFillerInWholeWord) {
    std::string text = "...................................p...................................o...................................r...................................n...................................";
    collapse_repeats(text);
    EXPECT_EQ(text, "..p..o..r..n..");
}
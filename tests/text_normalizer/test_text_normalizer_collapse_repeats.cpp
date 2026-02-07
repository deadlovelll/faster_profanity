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
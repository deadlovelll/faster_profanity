#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>

#include "censor_words_loader/censor_words_loader.hpp"

class CensorWordsLoaderTest : public ::testing::Test {
protected:
    CensorWordsLoader loader;
};

TEST_F(CensorWordsLoaderTest, WordsNormalizedLikeInput) {
    std::filesystem::path path = std::filesystem::temp_directory_path() / "censor_words_loader_test.txt";
    std::ofstream(path) << "Сука\npédé\nFUCK\n";
    loader.load_censor_words("", path.string());
    std::filesystem::remove(path);
    std::unordered_set<std::string> expected = {"cyкa", "pede", "fuck"};
    EXPECT_EQ(loader.get_censor_words(), expected);
}

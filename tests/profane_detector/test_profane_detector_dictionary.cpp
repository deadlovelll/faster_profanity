#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "profane_detector/profane_detector.hpp"

TEST(ProfaneDetectorDictionaryTest, NonAsciiDictionaryWordFound) {
    std::filesystem::path path = std::filesystem::temp_directory_path() / "profane_detector_dictionary_test.txt";
    std::ofstream(path) << "сука\n";
    ProfaneDetector detector("", path.string());
    std::filesystem::remove(path);
    std::string censored = detector.censor("Сука", '*');
    EXPECT_EQ(censored.find_first_not_of('*'), std::string::npos) << censored;
}

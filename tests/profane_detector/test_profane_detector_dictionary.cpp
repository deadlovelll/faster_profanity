#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "profane_detector/profane_detector.hpp"

static ProfaneDetector make_detector(const std::string& words) {
    std::filesystem::path path = std::filesystem::temp_directory_path() / "profane_detector_dictionary_test.txt";
    std::ofstream(path) << words;
    ProfaneDetector detector("", path.string());
    std::filesystem::remove(path);
    return detector;
}

TEST(ProfaneDetectorDictionaryTest, NonAsciiDictionaryWordFound) {
    ProfaneDetector detector = make_detector("сука\n");
    std::string censored = detector.censor("Сука", '*');
    EXPECT_EQ(censored.find_first_not_of('*'), std::string::npos) << censored;
}

TEST(ProfaneDetectorDictionaryTest, SpacedLettersFound) {
    ProfaneDetector detector = make_detector("fuck\n");
    EXPECT_EQ(detector.censor("you f u c k off", '*'), "you ******* off");
    EXPECT_EQ(detector.censor("f.u.c.k", '*'), "*******");
}

TEST(ProfaneDetectorDictionaryTest, WordsBetweenHyphensAndUnderscoresFound) {
    ProfaneDetector detector = make_detector("fuck\n");
    EXPECT_EQ(detector.censor("-fuck- _fuck_", '*'), "-****- _****_");
}

TEST(ProfaneDetectorDictionaryTest, LatinPartOfNonLatinWordNotFound) {
    ProfaneDetector detector = make_detector("wop\n");
    EXPECT_EQ(detector.censor("шорты", '*').find('*'), std::string::npos);
}

TEST(ProfaneDetectorDictionaryTest, PhraseAcrossPunctuationAndNewlinesFound) {
    ProfaneDetector detector = make_detector("alabama hot pocket\n");
    EXPECT_EQ(detector.censor("alabama,  hot\npocket", '*'), "*******,  ***\n******");
}

TEST(ProfaneDetectorDictionaryTest, HyphenatedDictionaryEntryFound) {
    ProfaneDetector detector = make_detector("arse-licking\n");
    EXPECT_EQ(detector.censor("arse licking", '*'), "**** *******");
    EXPECT_EQ(detector.censor("arse-licking", '*'), "****-*******");
}

TEST(ProfaneDetectorDictionaryTest, ContainsProfanity) {
    ProfaneDetector detector = make_detector("fuck\n");
    EXPECT_TRUE(detector.contains_profanity("F.U.C.K"));
    EXPECT_FALSE(detector.contains_profanity("hello"));
}

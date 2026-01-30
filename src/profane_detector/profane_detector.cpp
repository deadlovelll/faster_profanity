#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(const std::string &text, const char censor_char) {
    std::string censored_word = swear_word_hider.hide(text, censor_char);
    return censored_word;
}

bool ProfaneDetector::contains_profanity(const std::string &text, const char censor_char) {
    std::string censored_word = swear_word_hider.hide(text, censor_char);
    bool is_censored = censored_word == text;
    return is_censored;
}
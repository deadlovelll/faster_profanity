#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(const std::string &text, const char censor_char) {
    Node* root_ptr = root.get();
    std::string normalized_text = text;
    // text_normalizer.normalize(normalized_text);
    std::string censored_text = swear_word_hider.hide(normalized_text, censor_char, root_ptr);
    return censored_text;
}

bool ProfaneDetector::contains_profanity(const std::string &text) {
    Node* root_ptr = root.get();
    std::string normalized_text = text;
    // text_normalizer.normalize(normalized_text);
    std::string censored_word = swear_word_hider.hide(normalized_text, '*', root_ptr);
    bool is_censored = censored_word == text;
    return is_censored;
}
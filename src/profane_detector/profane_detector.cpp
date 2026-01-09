#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(std::string text) {
    return "censored";
}

bool ProfaneDetector::contains_profanity(std::string text) {
    return true;
}

int ProfaneDetector::get_start_index_of_next_word(
    std::string text, 
    int start_idx
) {
    size_t start_idx_next_word = text.size();
    return start_idx_next_word;
}
#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(std::string text) {
    return "censored";
}

bool ProfaneDetector::contains_profanity(std::string text) {
    return true;
}
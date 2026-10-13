#pragma once

#include <fstream>
#include <unordered_set>

#include "unicode_normalizer/unicode_normalizer.hpp"

class CensorWordsLoader {
    std::ifstream file;
    std::unordered_set<std::string> censor_words;
    UnicodeNormalizer unicode_normalizer;
public:
    void load_censor_words(const std::string& lang, const std::string& custom_dicstionary);
    const std::unordered_set<std::string>& get_censor_words();
};
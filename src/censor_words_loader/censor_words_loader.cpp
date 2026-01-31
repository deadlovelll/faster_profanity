#include <iostream>

#include "censor_words_loader/censor_words_loader.hpp"

void CensorWordsLoader::load_censor_words() {
    std::ifstream file("data/en/profanity_wordslist.txt");
    if (!file.is_open()) {
        std::cout << "no file found" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        censor_words.insert(line);
    }
}

const std::unordered_set<std::string>& CensorWordsLoader::get_censor_words() {
    return censor_words;
}
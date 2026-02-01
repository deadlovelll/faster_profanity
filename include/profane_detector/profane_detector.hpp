#pragma once

#include <string>
#include <unordered_set>

#include "censor_words_loader/censor_words_loader.hpp"
#include "trie_loader/trie_loader.hpp"
#include "swear_word_hider/swear_word_hider.hpp"

class ProfaneDetector {
public:
    ProfaneDetector() {
        censor_words_loader.load_censor_words();
        std::unordered_set<std::string> words = censor_words_loader.get_censor_words();
        TrieLoader trie_loader(words);
        this->root = trie_loader.get_trie();
    }
    std::string censor(const std::string &text, const char censor_char);
    bool contains_profanity(const std::string &text, const char censor_char);
private:
    std::unique_ptr<Node> root;
    SwearWordHider swear_word_hider;
    CensorWordsLoader censor_words_loader;
};
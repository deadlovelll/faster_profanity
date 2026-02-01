#pragma once

#include <string>
#include <unordered_set>

#include "node/node.hpp"
#include "utf8_sizer/utf8_sizer.hpp"

class TrieLoader {
public:
    TrieLoader(const std::unordered_set<std::string>& words)
        : root(new Node())
    {
        add_words(words);
        build();
    }
    std::unique_ptr<Node> get_trie();
private:
    std::unique_ptr<Node> root = std::make_unique<Node>();
    UTF8Sizer utf8_sizer;
    void add_words(const std::unordered_set<std::string>& words);
    void build();
};
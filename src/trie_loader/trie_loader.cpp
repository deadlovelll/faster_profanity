#include "trie_loader/trie_loader.hpp"

#include <queue>

Node* TrieLoader::get_trie() {
    return root.get();
}

// Loads all profane words from set into trie
void TrieLoader::add_words(const std::unordered_set<std::string>& words) {
    for (const std::string& word: words) {
        if (word.size() <= 1) continue;
        Node* node = root.get();

        for (size_t i = 0; i < word.size();) {
            // Get char length and cast it to string
            size_t len = utf8_sizer.utf8_char_len(static_cast<unsigned char>(word[i]));
            std::string ch = word.substr(i, len);
            // Create new node if it doesn't exist
            if (!node->children.count(ch)) {
                node->children[ch] = std::make_unique<Node>();
                node->children[ch]->depth = node->depth + 1;
            }
            // Descend to child
            node = node->children[ch].get();
            i += len;
        }
        // Mark the end of word
        node->outputs.push_back(word);
    }
}

void TrieLoader::build() {
    // Bfs queue
    std::queue<Node*> q;

    using Child = std::pair<const std::string, std::unique_ptr<Node>>&;

    for (Child entry : root->children) {
        Node* node = entry.second.get();
        node->fail = root.get();
        q.push(node);
    }

    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        for (Child entry : curr->children) {
            const std::string& ch = entry.first;
            Node* child = entry.second.get();
            
            // Get fail links for child node
            Node* f = curr->fail;
            while (f && !f->children.count(ch)) {
                f = f->fail;
            }
            child->fail = f ? f->children[ch].get() : root.get();

            // Inherit outputs from fail node
            child->outputs.insert(
                child->outputs.end(),
                child->fail->outputs.begin(),
                child->fail->outputs.end()
            );
            // Remove child from queue
            q.push(child);
        }
    }
}
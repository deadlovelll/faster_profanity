#include <string>
#include <queue>
#include <unordered_set>

#include "swear_word_hider/swear_word_hider.hpp"

// Checks if char is valid or not, see swear_word_hider.hpp above
bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

// Returns byte length of current UTF-8 codepoint based on leading byte.
// Assumes valid UTF-8 input.
size_t SwearWordHider::utf8_char_len(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

size_t SwearWordHider::get_utf8_length(const std::string& str) {
    size_t length = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80) {
            length++;
        }
    }
    return length;
}

std::string SwearWordHider::hide(const std::string& text, char censor_char) {
    std::string result = text;
    std::string prev_char;

    std::vector<size_t> history;
    history.push_back(text.size());

    Node* node = root.get();
    for (size_t i = 0; i < text.size();) {
        size_t len = utf8_char_len(static_cast<unsigned char>(text[i]));
        std::string ch = text.substr(i, len);

        bool is_stutter = (prev_char == ch && !node->children.count(ch));
        if (!is_stutter) {
            while (node && !node->children.count(ch)) node = node->fail;
            node = node ? node->children[ch].get() : root.get();

            history.resize(node->depth);
            if (node != root.get()) history.push_back(i);
        }
        
        if (node && !node->outputs.empty()) {
            for (const std::string& word : node->outputs) {
                size_t pattern_length = get_utf8_length(word);
                if (history.size() >= pattern_length) {
                    size_t start_pos = history[history.size() - pattern_length];
                    size_t end_pos = i + len;
                    for (size_t j = start_pos; j < end_pos; ++j) {
                        result[j] = censor_char;
                    }
                }
            }
        }
        prev_char = ch;
        i += len;
    }
    return result;
}

// Loads all profane words from set into trie
void SwearWordHider::add_words(const std::unordered_set<std::string>& words) {
    for (const std::string& word: words) {
        if (word.size() <= 1) continue;
        Node* node = root.get();

        for (size_t i = 0; i < word.size();) {
            // Get char length and cast it to string
            size_t len = utf8_char_len(static_cast<unsigned char>(word[i]));
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

void SwearWordHider::build() {
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
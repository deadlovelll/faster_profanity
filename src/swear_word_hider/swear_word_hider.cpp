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

// Scans input text with aho automaton 
std::string SwearWordHider::hide(const std::string& text, char censor_char) {
    // Current node
    Node* node = root.get();
    // List of matches
    std::vector<Match> matches;
    // Position in input 
    size_t i = 0;

    while (i < text.size()) {
        // Get the length of utf8 char
        size_t len = utf8_char_len((unsigned char)text[i]);
        // Get char as string
        std::string ch = text.substr(i, len);

        while (node && !node->children.count(ch)) {
            node = node->fail;
        }
        
        // If exists go to the next node or back to root otherwise 
        node = node ? node->children[ch].get() : root.get();

        if (node) {
            // Iter over nodes that ends with current one
            for (const std::string& word : node->outputs) {
                // Calc start index of the match
                size_t start = i + len - word.size();
                bool left_ok = (
                    start == 0 || 
                    !std::isalnum((unsigned char)text[start - 1])
                );
                bool right_ok = (
                    start + word.size() >= text.size() ||
                    !std::isalnum((unsigned char)text[start + word.size()])
                );
                if (left_ok && right_ok) {
                    // Store match for censor it later
                    matches.push_back({start, word.size()});
                }
            }
        }

        i += len;
    }

    std::string result = text;

    // Apply censor for all matches
    for (Match& m : matches) {
        if (m.start + m.len <= result.size()) {
            for (size_t j = 0; j < m.len; ++j) {
                // Replace with censor char 
                result[m.start + j] = censor_char;
            }
        }
    }
    // Return censored word
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
#include <string>
#include <unordered_set>

#include "swear_word_hider/swear_word_hider.hpp"

// Checks if char is valid or not, see swear_word_hider.hpp above
bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

std::string SwearWordHider::hide(
    const std::string& text, 
    char censor_char,
    Node* root
) {
    std::string result = text;
    std::string prev_char;

    std::vector<size_t> history;
    history.push_back(text.size());

    Node* node = root;
    for (size_t i = 0; i < text.size();) {
        size_t len = utf8_sizer.utf8_char_len(static_cast<unsigned char>(text[i]));
        std::string ch = text.substr(i, len);

        bool is_stutter = (prev_char == ch && !node->children.count(ch));
        if (!is_stutter) {
            while (node && !node->children.count(ch)) node = node->fail;
            node = node ? node->children[ch].get() : root;

            history.resize(node->depth);
            if (node != root) history.push_back(i);
        }
        
        if (node && !node->outputs.empty()) {
            for (const std::string& word : node->outputs) {
                size_t pattern_length = utf8_sizer.utf8_word_length(word);
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
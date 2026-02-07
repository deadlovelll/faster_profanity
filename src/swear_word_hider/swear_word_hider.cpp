#include <cctype>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "swear_word_hider/swear_word_hider.hpp"

// Checks if char is valid or not, see swear_word_hider.hpp above
bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

bool SwearWordHider::is_word_boundary(const std::string& text, size_t start, size_t end) {
    auto is_boundary = [](unsigned char c) {
        return !std::isalnum(c) && c != '_' && c != '-';
    };

    if (start > 0) {
        unsigned char left = static_cast<unsigned char>(text[start - 1]);
        if (!is_boundary(left)) return false;
    }

    if (end < text.size()) {
        unsigned char right = static_cast<unsigned char>(text[end]);
        if (!is_boundary(right)) return false;
    }

    return true;
}

std::string SwearWordHider::hide(
    const std::string& text,
    char censor_char,
    Node* root
) {
    std::string result = text;
    Node* node = root;
    for (size_t i = 0; i < text.size();) {
        size_t len = utf8_sizer.utf8_char_len(static_cast<unsigned char>(text[i]));
        std::string ch = text.substr(i, len);

        while (node && !node->children.count(ch)) node = node->fail;
        node = node ? node->children[ch].get() : root;

        if (node && !node->outputs.empty()) {
            for (const auto& word : node->outputs) {
                size_t start = i + len - word.size();
                size_t end = start + word.size();
                if (start < result.size() && is_word_boundary(result, start, end)) {
                    for (size_t j = 0; j < word.size(); ++j)
                        result[start + j] = censor_char;
                }
            }
        }

        i += len;
    }
    return result;
}
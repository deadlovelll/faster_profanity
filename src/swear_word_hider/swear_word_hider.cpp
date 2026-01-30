#include <string>
#include <queue>
#include <unordered_set>

#include "swear_word_hider/swear_word_hider.hpp"

bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

size_t SwearWordHider::utf8_char_len(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

std::string SwearWordHider::hide(const std::string& text, char censor_char) {
    Node* node = root;
    size_t i = 0;

    std::vector<Match> matches;

    while (i < text.size()) {
        size_t len = utf8_char_len((unsigned char)text[i]);
        std::string ch = text.substr(i, len);

        while (node && !node->children.count(ch)) {
            node = node->fail;
        }

        node = node ? node->children[ch] : root;

        if (node) {
            for (const std::string& word : node->outputs) {
                size_t start = i + len - word.size();
                bool left_ok = start == 0 ||
                    !std::isalnum((unsigned char)text[start - 1]);

                bool right_ok = start + word.size() >= text.size() ||
                    !std::isalnum((unsigned char)text[start + word.size()]);

                if (left_ok && right_ok) {
                    matches.push_back({start, word.size()});
                }
            }
        }

        i += len;
    }

    std::string result = text;

    for (auto& m : matches) {
        if (m.start + m.len <= result.size()) {
            for (size_t j = 0; j < m.len; ++j) {
                result[m.start + j] = censor_char;
            }
        }
    }

    return result;
}

void SwearWordHider::add_words(const std::unordered_set<std::string>& words) {
    for (const std::string& word: words) {
        if (word.size() <= 1) continue;
        Node* node = root;
        for (size_t i = 0; i < word.size();) {
            size_t len = utf8_char_len(static_cast<unsigned char>(word[i]));
            std::string ch = word.substr(i, len);
            if (!node->children.count(ch)) node->children[ch] = new Node();
            node = node->children[ch];
            i += len;
        }
        node->outputs.push_back(word);
    }
}

void SwearWordHider::build() {
    std::queue<Node*> q;
    for (auto& [ch, node] : root->children) {
        node->fail = root;
        q.push(node);
    }

    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        for (auto& [ch, child] : curr->children) {
            Node* f = curr->fail;
            while (f && !f->children.count(ch)) f = f->fail;
            child->fail = f ? f->children[ch] : root;
            child->outputs.insert(
                child->outputs.end(),
                child->fail->outputs.begin(),
                child->fail->outputs.end()
            );
            q.push(child);
        }
    }
}
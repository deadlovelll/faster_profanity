#include <string>

#include "swear_word_hider/swear_word_hider.hpp"

bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

size_t SwearWordHider::utf8_char_len(unsigned char c, size_t remaining) {
    size_t len = 1;
    if ((c & 0x80) == 0) len = 1;
    else if ((c & 0xE0) == 0xC0) len = 2;
    else if ((c & 0xF0) == 0xE0) len = 3;
    else if ((c & 0xF8) == 0xF0) len = 4;

    return len <= remaining ? len : 1;
}

std::string SwearWordHider::hide(
    const std::string& text,
    const char censor_char,
    const std::unordered_set<std::string>& censor_full_list
) {
    std::string result;
    std::string cur_word;
    size_t i = 0;

    while (i < text.size()) {
        size_t len = utf8_char_len(
            static_cast<unsigned char>(text[i]),
            text.size() - i
        );
        std::string_view utf8_char(text.data() + i, len);

        if (is_allowed_char(utf8_char)) {
            cur_word.append(utf8_char);
        } else {
            if (!cur_word.empty()) {
                if (censor_full_list.count(cur_word)) {
                    result += std::string(cur_word.size(), censor_char);
                } else {
                    result += cur_word;
                }
                cur_word.clear();
            }
            result.append(utf8_char);
        }

        i += len;
    }

    if (!cur_word.empty()) {
        if (censor_full_list.count(cur_word)) {
            result += std::string(cur_word.size(), censor_char);
        } else {
            result += cur_word;
        }
    }

    for (const std::string& bad_word : censor_full_list) {
        size_t pos = 0;
        while ((pos = result.find(bad_word, pos)) != std::string::npos) {
            result.replace(pos, bad_word.size(), bad_word.size(), censor_char);
            pos += bad_word.size();
        }
    }

    return result;
}
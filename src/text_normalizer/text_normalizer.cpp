#include "text_normalizer/text_normalizer.hpp"

void TextNormalizer::normalize(std::string& text) {
    to_lower(text);
    collapse_repeats(text);
}

void TextNormalizer::to_lower(std::string& text) {
    for (char& c: text) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
}

void TextNormalizer::collapse_repeats(std::string& text, int max_repeat) {
    std::string out;
    char last = 0;
    int count = 0;

    for (char& c : text) {
        if (c == last) {
            if (++count > max_repeat) continue;
        } else {
            last = c;
            count = 1;
        }
        out.push_back(c);
    }
}

void TextNormalizer::trim_word_boundaries(std::string& text) {
    size_t i = 0;

    while (i < text.size()) {
        while (i < text.size() && !is_word_char(text[i])) i++;

        size_t start = i;

        while (i < text.size() && is_word_char(text[i])) i++;

        size_t end = i;

        if (start < end) {
            if (!text.empty()) text += ' ';
            text += text.substr(start, end - start);
        }
    }
}
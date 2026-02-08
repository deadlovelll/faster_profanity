#include "text_normalizer/text_normalizer.hpp"
#include <sstream>

void TextNormalizer::normalize(std::string& text) {
    to_lower(text);
    collapse_repeats(text);
    trim_word_boundaries(text);
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
    text.swap(out);
}

void TextNormalizer::trim_word_boundaries(std::string& text) {
    std::string out;
    std::istringstream iss(text);
    std::string token;

    while (iss >> token) {
        size_t start = 0;
        while (
            start < token.size() && 
            !std::isalnum(static_cast<unsigned char>(token[start])) &&
            !is_subsymbol(token[start])
        ) ++start;

        size_t end = token.size();
        while (
            end > start && 
            !std::isalnum(static_cast<unsigned char>(token[end - 1])) &&
            !is_subsymbol(token[start])
        ) --end;

        if (start < end) {
            if (!out.empty()) out += ' ';
            out += token.substr(start, end - start);
        }
    }

    text.swap(out);
}

// bool TextNormalizer::is_word_char(char c) {
//     return std::isalpha(static_cast<unsigned char>(c));
// }

bool TextNormalizer::is_subsymbol(char c) {
    static const std::string subs = "@*48β(<ç∂3€εφ96#ħ1!|;κµ0øρ®$5§7+v×¥2α"; 
    return subs.find(c) != std::string::npos;
}
#include "text_normalizer/text_normalizer.hpp"
#include <sstream>
#include <iostream>

void TextNormalizer::normalize(std::string& text) {
    to_lower(text);
    collapse_repeats(text);
    trim_word_boundaries(text);
    remove_letter_fillers(text);
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

bool TextNormalizer::is_obfuscated_word(const std::string& s, size_t start, size_t end) {
    int transitions = 0;
    for (size_t i = start + 1; i < end; ++i) {
        bool prev_letter = is_word_char(s[i - 1]);
        bool curr_filler = !is_word_char(s[i]);
        bool next_letter = (i + 1 < end) && is_word_char(s[i + 1]);

        if (prev_letter && curr_filler && next_letter) {
            transitions++;
            i++;
        }
    }

    return transitions >= 2;
}

std::string TextNormalizer::clean_obfuscated_word(const std::string& s, size_t start, size_t end) {
    std::string out;
    for (size_t i = start; i < end; ++i) {
        if (!is_word_char(s[i])) {
            bool left = (i > start) && is_word_char(s[i-1]);
            bool right = (i + 1 < end) && is_word_char(s[i+1]);
            if (left && right) {
                continue;
            }
        }
        out.push_back(s[i]);
    }
    return out;
}

void TextNormalizer::remove_letter_fillers(std::string& text) {
    std::string out;
    size_t i = 0;
    while (i < text.size()) {
        if (!is_word_char(text[i])) {
            out.push_back(text[i++]);
            continue;
        }

        size_t start = i;
        while (i < text.size() && (is_word_char(text[i]) || is_subsymbol(text[i]))) {
            ++i;
        }
        size_t end = i;
        bool is_obfuscated = is_obfuscated_word(text, start, end);
        if (is_obfuscated) {
            std::string appendix = clean_obfuscated_word(text, start, end);
            out += appendix;
        } else {
            out.append(text, start, end-start);
        }
    }
    std::cout << out << std::endl;
    text.swap(out);
}

bool TextNormalizer::is_filler(char c) { return (!is_word_char(c) && !is_subsymbol(c)); } 
bool TextNormalizer::is_word_char(char c) { return std::isalpha(static_cast<unsigned char>(c)); }

bool TextNormalizer::is_subsymbol(char c) {
    static const std::string subs = "@*48β(<ç∂3€εφ96#ħ1!|;κµ0øρ®$5§7+v×¥2α"; 
    return subs.find(c) != std::string::npos;
}
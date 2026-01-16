#include "text_trimmer/text_trimmer.hpp"

void TextTrimmer::trim(std::string& text) {
    left_trim(text);
    right_trim(text);
}

void TextTrimmer::left_trim(std::string& text) {
    text.erase(
        text.begin(),
        std::find_if(text.begin(), text.end(),
            [](unsigned char ch) { return !std::isspace(ch); })
    );
}

void TextTrimmer::right_trim(std::string& text) {
    text.erase(
        std::find_if(text.rbegin(), text.rend(),
            [](unsigned char ch) { return !std::isspace(ch); }).base(),
        text.end()
    );
}
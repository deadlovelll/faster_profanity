#include <string>

#include "swear_word_hider/swear_word_hider.hpp"

std::string SwearWordHider::hide(std::string &text, std::string &censor_char) {
    std::string censored_text = "";
    std::string current_word = "";
    int skip_index = -1;

    size_t start_idx_of_next_word = word_finder.get_start_index_of_next_word(text, 0);
    if (start_idx_of_next_word >= text.size()) { 
        return text; 
    }
    censored_text = text.substr(0, start_idx_of_next_word);
    text = text.substr(start_idx_of_next_word, text.size());
    for (size_t index = 0; index < text.size(); ++index) { 
        char text_char = text[index];
        std::string text_char_string = std::string(1, text_char); 
        if (allowed_characters.contains(text[index])) {
            current_word += text_char;
            continue;
        }
        text_trimmer.trim(current_word);
        if (current_word == "") {
            censored_text += text_char;
            current_word = "";
            continue;
        }
    }
    return "";
}
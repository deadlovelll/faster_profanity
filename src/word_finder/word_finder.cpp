#include "word_finder.hpp"
#include <cstddef>
#include <vector>

std::vector<std::tuple<std::string, size_t>> WordFinder::get_next_words(
    std::string& text,
    size_t start_idx,
    int num_of_next_words
) {
    size_t start_idx_of_next_word = get_start_index_of_next_word(text, start_idx);
    if (start_idx_of_next_word >= text.size() - 1) {
        std::vector<std::tuple<std::string, size_t>> result = {
            {"", start_idx_of_next_word},
            {"", start_idx_of_next_word}
        };
        return result;
    }
    std::tuple<std::string, size_t> nw_tuple = get_next_word_and_end_index(text, start_idx);
    std::string next_word = std::get<0>(nw_tuple);
    size_t end_index = std::get<1>(nw_tuple);
    size_t to = start_idx_of_next_word - start_idx;

    std::string concatenated = text.substr(start_idx, to) + next_word;
    std::vector<std::tuple<std::string, size_t>> words = {
        nw_tuple,
        {concatenated, end_index}
    };
    if (num_of_next_words > 1) {
        std::vector<std::tuple<std::string, size_t>> next_words = get_next_words(
            text, 
            start_idx, 
            num_of_next_words -1
        );
        words.insert(words.end(), next_words.begin(), next_words.end());
    }

    return words;
}

size_t WordFinder::get_start_index_of_next_word(
    std::string& text, 
    size_t start_idx
) {
    // Getting the size of current text
    size_t start_index_of_next_word = text.size();
    // Iterate over the string starting from start_idx
    for (size_t index = start_idx; index < text.size(); ++index) {
        // Skip characters that are not in the allowed set
        if (ALLOWED_CHARACTERS.find(text[index]) == ALLOWED_CHARACTERS.end()) {
            continue;
        }
        // Found the first character of the next word
        start_index_of_next_word = index;
        // Stop after finding the first allowed character
        break;
    }
    // Return the index of the start of the next word,
    // or text.size() if no word was found
    return start_index_of_next_word;
}

std::tuple<std::string, size_t> WordFinder::get_next_word_and_end_index(
    std::string& text, 
    size_t start_idx
) {
    // Initializing empty string 
    std::string next_word = "";
    // Getting the size of current text
    size_t start_index_of_next_word = text.size();
    // Setting the index to current start_idx
    size_t index = start_idx;
    // Iterate over the string starting from index
    for (; index < start_index_of_next_word; ++index) {
        // Getting the character by index
        char character = text[index];
        // Skip characters that are not in the allowed set
        if (ALLOWED_CHARACTERS.find(character) == ALLOWED_CHARACTERS.end()) {
            continue;
        }
        // Adding charcter to the word
        next_word += character;
    }
    // Return the tuple of the new word and end index
    return {next_word, index};
}
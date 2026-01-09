#include "word_finder.hpp"
#include <cstddef>

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
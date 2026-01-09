#include "word_finder.hpp"
#include <cstddef>
#include <vector>

std::vector<std::tuple<std::string, size_t>> WordFinder::get_next_words(
    std::string& text,
    size_t start_idx,
    int num_of_next_words
) {
    // Find the index of the first character of the next word
    size_t start_idx_of_next_word = get_start_index_of_next_word(text, start_idx);
    // If no next word exists (or we're at the end of the string),
    // return placeholder results with empty strings
    if (start_idx_of_next_word >= text.size() - 1) {
        std::vector<std::tuple<std::string, size_t>> result = {
            {"", start_idx_of_next_word},
            {"", start_idx_of_next_word}
        };
        return result;
    }
    // Get the next word starting from start_idx and the index
    // of the first character after that word
    std::tuple<std::string, size_t> nw_tuple = get_next_word_and_end_index(text, start_idx);
    // Extract the word and its end index from the tuple
    std::string next_word = std::get<0>(nw_tuple);
    size_t end_index = std::get<1>(nw_tuple);
    // Calculate the length of the substring before the next word
    size_t to = start_idx_of_next_word - start_idx;

    // Concatenate the skipped part of the text with the next word
    std::string concatenated = text.substr(start_idx, to) + next_word;
    // Initialize the result vector with:
    // 1) the raw next word
    // 2) the concatenated prefix + next word
    std::vector<std::tuple<std::string, size_t>> words = {
        nw_tuple,
        {concatenated, end_index}
    };
    // If more words are requested, recursively collect them
    // and append them to the result vector
    if (num_of_next_words > 1) {
        std::vector<std::tuple<std::string, size_t>> next_words = get_next_words(
            text, 
            start_idx, 
            num_of_next_words -1
        );
        // Append all recursively obtained words to the current result
        words.insert(words.end(), next_words.begin(), next_words.end());
    }
    // Return the collected list of words with their end indices
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
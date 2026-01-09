#include <string>
#include <unordered_set>

class WordFinder {
public:
    // Set with allowed characters
    std::unordered_set<char> ALLOWED_CHARACTERS;

    // Wordfinder constructor
    WordFinder() {
        for (char c = 'a'; c <= 'z'; ++c) ALLOWED_CHARACTERS.insert(c);
        for (char c = 'A'; c <= 'Z'; ++c) ALLOWED_CHARACTERS.insert(c);
    }
    std::vector<std::tuple<std::string, size_t>> get_next_words(
        std::string& text, 
        size_t start_idx, 
        int num_of_next_words
    );
    // Method for getting the index of the next word
    size_t get_start_index_of_next_word(std::string& text, size_t start_idx);
    // Method for getting the next word and end index
    std::tuple<std::string, size_t> get_next_word_and_end_index(
        std::string& text, 
        size_t start_idx
    );
};
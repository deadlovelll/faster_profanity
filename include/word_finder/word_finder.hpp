#include <cstddef>
#include <string>
#include <unordered_set>

class WordFinder {
public:
    // Set of characters that are considered valid word characters
    std::unordered_set<char32_t> ALLOWED_CHARACTERS;

    // Default constructor.
    // Initializes the set of allowed characters with ASCII letters [a-zA-Z].
    WordFinder() {}

    /**
     * Returns a list of next words starting from a given index.
     *
     * Each element of the result is a tuple containing:
     *  - the word (or a derived concatenated string),
     *  - the index of the first character after that word.
     *
     * The number of returned words is controlled by num_of_next_words.
     *
     * @param text The input text to search in.
     * @param start_idx The index from which the search begins.
     * @param num_of_next_words The number of next words to collect.
     * @return A vector of (string, index) tuples.
     */
    std::vector<std::tuple<std::string, size_t>> get_next_words(
        std::string& text, 
        size_t start_idx, 
        int num_of_next_words
    );

    /**
     * Finds the index of the first character of the next word.
     *
     * Scans the string starting from start_idx and returns the index
     * of the first character that belongs to ALLOWED_CHARACTERS.
     *
     * @param text The input text.
     * @param start_idx The index to start searching from.
     * @return The index of the next word, or text.size() if none is found.
     */
    size_t get_start_index_of_next_word(std::string& text, size_t start_idx);

    /**
     * Extracts the next word starting at a given index and returns
     * the word along with the index of the first character after it.
     *
     * @param text The input text.
     * @param start_idx The index from which to start extracting the word.
     * @return A tuple containing the extracted word and its end index.
     */
    std::tuple<std::string, size_t> get_next_word_and_end_index(
        std::string& text, 
        size_t start_idx
    );
};
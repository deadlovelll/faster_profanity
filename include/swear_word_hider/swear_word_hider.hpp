#include <string>
#include <unordered_set>

#include "word_finder/word_finder.hpp"
#include "text_trimmer/text_trimmer.hpp"

class SwearWordHider {
public:
    SwearWordHider(const std::unordered_set<char32_t>& symbols): 
        allowed_characters(symbols)
    {}
    std::string hide(std::string &text, std::string &censor_char);
    int max_number_combinations;
private:
    std::unordered_set<char32_t> allowed_characters;
    WordFinder word_finder;
    TextTrimmer text_trimmer;
};
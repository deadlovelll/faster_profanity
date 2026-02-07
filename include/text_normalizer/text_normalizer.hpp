#include <string>

class TextNormalizer {
public:
    void normalize(std::string& text);
protected:
    void collapse_repeats(std::string& text, int max_repeat = 2);
    void to_lower(std::string& text);
    void remove_letter_fillers(std::string& text);
    void trim_word_boundaries(std::string& text);
    void normalize_spaces(std::string& text);
    bool is_word_char(char c);
    bool is_subsymbol(char c);
    bool is_word_inner(char c);
};
#include <unordered_set>

class AllowedCharactersLoader {
public: 
    std::unordered_set<char32_t> load();
private:
    void load_regular_chars(std::unordered_set<char32_t>& allowed_characters);
    void load_unicode_json(std::unordered_set<char32_t>& allowed_characters);
};
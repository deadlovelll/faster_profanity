#include <unordered_set>

class AllowedCharactersLoader {
public:
    AllowedCharactersLoader(const std::string& path) {
        load_regular_chars();
        load_unicode_json(path);
    }
    std::unordered_set<char32_t> get_symbols();
private:
    std::unordered_set<char32_t> symbols;
    void load_regular_chars();
    void load_unicode_json(const std::string& path);
    char32_t utf8_to_char32_t(const std::string& s);
};
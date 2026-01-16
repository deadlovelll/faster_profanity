#include <unordered_set>

class AllowedCharactersLoader {
public:
    AllowedCharactersLoader(const std::string& path) {
        load_regular_chars();
        // load_unicode_json(path);
    }
    std::unordered_set<std::u32string> get_symbols();
private:
    std::unordered_set<std::u32string> symbols;
    void load_regular_chars();
    void load_unicode_json(const std::string& path);
};
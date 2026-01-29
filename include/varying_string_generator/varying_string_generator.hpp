#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class VaryingStringGenerator {
public:
    void generate_word_combinations(
        std::unordered_set<std::string>& censor_full_list,
        std::string& word
    );
private:
    std::unordered_map<char, std::vector<std::string>> CHARS_MAPPING = {
        {'a', {"a", "@", "*", "4", "α"}},
        {'b', {"b", "8", "β"}},
        {'c', {"c", "(", "<", "ç"}},
        {'d', {"d", "∂"}},
        {'e', {"e", "3", "€", "ε"}},
        {'f', {"f", "φ"}},
        {'g', {"g", "9", "6"}},
        {'h', {"h", "#", "ħ"}},
        {'i', {"i", "1", "!", "l", "|"}},
        {'j', {"j", ";"}},
        {'k', {"k", "κ"}},
        {'l', {"l", "1", "|"}},
        {'m', {"m", "µ", "/\\/\\", "nn"}},
        {'n', {"n", "η"}},
        {'o', {"o", "0", "*", "@", "ø"}},
        {'p', {"p", "ρ"}},
        {'q', {"q", "9"}},
        {'r', {"r", "®"}},
        {'s', {"s", "$", "5", "§"}},
        {'t', {"t", "7", "+"}},
        {'u', {"u", "v", "µ"}},
        {'v', {"v", "u"}},
        {'w', {"w", "vv"}},
        {'x', {"x", "×"}},
        {'y', {"y", "¥"}},
        {'z', {"z", "2"}}
    };
    void generate_recursive(
        const std::string& word,
        size_t index,
        std::string& current,
        std::unordered_set<std::string>& out
    );
};
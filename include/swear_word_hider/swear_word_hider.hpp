#pragma once

#include <string>
#include <unordered_set>

#include "node/node.hpp"
#include "utf8_sizer/utf8_sizer.hpp"

class SwearWordHider {
public:
    std::string hide(const std::string& text, const char censor_char, Node* root);
private:
    UTF8Sizer utf8_sizer;
    const std::unordered_set<std::string_view> allowed_chars = {
        "a","b","c","d","e","f","g",
        "h","i","j","k","l","m", "n",
        "o","p","q","r","s","t","u",
        "v","w","x","y","z", "A","B",
        "C","D","E","F","G","H","I",
        "J","K","L","M", "N","O","P",
        "Q","R","S","T","U","V","W","X",
        "Y","Z", "0","1","2","3","4",
        "5","6","7","8","9", "@","*",
        "$","€","®","¥","+","#","!",
        "|",";", "(", "<", "α","β","µ",
        "ç","∂","ε","φ","ø","×"
    };
    const std::unordered_map<std::string, std::vector<std::string>> reverse_map = {
        {"a", {"a", "@", "*", "4", "α"}},
        {"b", {"b", "8", "β"}},
        {"c", {"c", "(", "<", "ç"}},
        {"d", {"d", "∂"}},
        {"e", {"e", "3", "€", "ε"}},
        {"f", {"f", "φ"}},
        {"g", {"g", "9", "6"}},
        {"h", {"h", "#", "ħ"}},
        {"i", {"i", "1", "!", "l", "|"}},
        {"j", {"j", ";"}},
        {"k", {"k", "κ"}},
        {"l", {"l", "1", "|"}},
        {"m", {"m", "µ", "/\\/\\", "nn"}},
        {"n", {"n", "η"}},
        {"o", {"o", "0", "*", "@", "ø"}},
        {"p", {"p", "ρ"}},
        {"q", {"q", "9"}},
        {"r", {"r", "®"}},
        {"s", {"s", "$", "5", "§"}},
        {"t", {"t", "7", "+"}},
        {"u", {"u", "v", "µ"}},
        {"v", {"v", "u"}},
        {"w", {"w", "vv"}},
        {"x", {"x", "×"}},
        {"y", {"y", "¥"}},
        {"z", {"z", "2"}}
    };
    bool is_allowed_char(std::string_view utf8_char) const;
};
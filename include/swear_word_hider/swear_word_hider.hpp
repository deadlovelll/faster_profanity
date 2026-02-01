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
    const std::vector<std::pair<std::string, std::vector<char>>> reverse_map = {
        {"/\\/\\", {'m'}},
        {"nn", {'m'}},
        {"vv", {'w'}},

        {"α", {'a'}},
        {"β", {'b'}},
        {"ç", {'c'}},
        {"∂", {'d'}},
        {"€", {'e'}},
        {"ε", {'e'}},
        {"φ", {'f'}},
        {"ħ", {'h'}},
        {"κ", {'k'}},
        {"µ", {'m', 'u'}},
        {"η", {'n'}},
        {"ø", {'o'}},
        {"ρ", {'p'}},
        {"®", {'r'}},
        {"§", {'s'}},
        {"×", {'x'}},
        {"¥", {'y'}},

        {"@", {'a', 'o'}},
        {"*", {'a', 'o'}},
        {"4", {'a'}},
        {"8", {'b'}},
        {"(", {'c'}},
        {"<", {'c'}},
        {"3", {'e'}},
        {"9", {'g', 'q'}},
        {"6", {'g'}},
        {"#", {'h'}},
        {"1", {'i', 'l'}},
        {"!", {'i'}},
        {"|", {'i', 'l'}},
        {";", {'j'}},
        {"0", {'o'}},
        {"$", {'s'}},
        {"5", {'s'}},
        {"7", {'t'}},
        {"+", {'t'}},
        {"v", {'u', 'w'}},
        {"2", {'z'}}
    };
    bool is_allowed_char(std::string_view utf8_char) const;
};
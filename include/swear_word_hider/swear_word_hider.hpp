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
    const std::unordered_map<std::string, std::vector<char>> reverse_map = {
        {"@", {'a', 'o'}},
        {"*", {'a', 'o'}},
        {"4", {'a'}},
        {"α", {'a'}},
        {"8", {'b'}},
        {"β", {'b'}},
        {"(", {'c'}},
        {"<", {'c'}},
        {"ç", {'c'}},
        {"∂", {'d'}},
        {"3", {'e'}},
        {"€", {'e'}},
        {"ε", {'e'}},
        {"φ", {'f'}},
        {"9", {'g', 'q'}},
        {"6", {'g'}},
        {"#", {'h'}},
        {"ħ", {'h'}},
        {"1", {'i', 'l'}},
        {"!", {'i'}},
        {"|", {'i', 'l'}},
        {";", {'j'}},
        {"κ", {'k'}},
        {"µ", {'m', 'u'}},
        {"/\\/\\", {'m'}},
        {"nn", {'m'}},
        {"η", {'n'}},
        {"0", {'o'}},
        {"ø", {'o'}},
        {"ρ", {'p'}},
        {"®", {'r'}},
        {"$", {'s'}},
        {"5", {'s'}},
        {"§", {'s'}},
        {"7", {'t'}},
        {"+", {'t'}},
        {"v", {'u', 'w'}},
        {"vv", {'w'}},
        {"×", {'x'}},
        {"¥", {'y'}},
        {"2", {'z'}}
    };
    bool is_allowed_char(std::string_view utf8_char) const;
protected:
    bool is_word_boundary(const std::string& text, size_t start, size_t end);
};
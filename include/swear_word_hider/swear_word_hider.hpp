#include <string>
#include <unordered_set>

#include "node/node.hpp"
#include "match/match.hpp"

class SwearWordHider {
public:
    SwearWordHider(const std::unordered_set<std::string>& words)
        : root(new Node())
    {
        add_words(words);
        build();
    }
    std::string hide(const std::string& text, const char censor_char);
private:
    std::unique_ptr<Node> root = std::make_unique<Node>();
    const std::unordered_set<std::string_view> allowed_chars = {
        "a","b","c","d","e","f","g","h","i","j","k","l","m",
        "n","o","p","q","r","s","t","u","v","w","x","y","z",
        "A","B","C","D","E","F","G","H","I","J","K","L","M",
        "N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
        "0","1","2","3","4","5","6","7","8","9",
        "@","*","$","€","®","¥","+","#","!","|",";", "(", "<",
        "α","β","µ","ç","∂","ε","φ","ø","×"
    };
    bool is_allowed_char(std::string_view utf8_char) const;
    void add_words(const std::unordered_set<std::string>& words);
    void build();
    size_t utf8_char_len(unsigned char c);
};
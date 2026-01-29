#include <string>
#include <unordered_set>

class SwearWordHider {
public:
    std::string hide(
        const std::string& text,
        const char censor_char,
        const std::unordered_set<std::string>& censor_full_list
    );
private:
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
    size_t utf8_char_len(unsigned char c, size_t remaining);
};
#include <string>

#include "swear_word_hider/swear_word_hider.hpp"

class ProfaneDetector {
public:
    std::string censor(
        const std::string &text, 
        const char censor_char
    );
    bool contains_profanity(
        const std::string &text, 
        const char censor_char
    );
private:
    SwearWordHider swear_word_hider;
};
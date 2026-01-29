#include <string>

#include "swear_word_hider/swear_word_hider.hpp"

class ProfaneDetector {
public:
    std::string censor(
        const std::string &text, 
        const char censor_char, 
        const std::unordered_set<std::string> &censor_full_list
    );
    bool contains_profanity(
        const std::string &text, 
        const char censor_char, 
        const std::unordered_set<std::string> &censor_full_list
    );
private:
    SwearWordHider swear_word_hider;
};
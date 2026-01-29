#include <string>

class ProfaneDetector {
public:
    std::string censor(std::string text);
    bool contains_profanity(std::string text);
};
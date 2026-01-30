#include <fstream>
#include <unordered_set>

class CensorWordsLoader {
    std::ifstream file;
    std::unordered_set<std::string> censor_words;
public:
    void load_censor_words();
    const std::unordered_set<std::string>& get_censor_words();
};
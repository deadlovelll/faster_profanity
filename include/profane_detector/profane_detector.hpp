#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

class ProfaneDetector {
public:
    std::string censor(std::string text);
    int get_start_index_of_next_word(std::string text, int start_idx);
    bool contains_profanity(std::string text);
private:
    std::vector<std::string> censor_wordset;
    std::unordered_map<
        std::string, 
        std::tuple<std::string, std::string, std::string, std::string>
    > character_mapping;
    std::string hide_swear_words(std::string text, std::string censor_char);
};
#include <string>

class TextTrimmer {
public:
    void trim(std::string& text);
private:
    void left_trim(std::string& text);
    void right_trim(std::string& text);
};
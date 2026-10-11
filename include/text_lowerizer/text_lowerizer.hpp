#pragma once

#include <string>

class TextLowerizer {
public:
    void lower(std::string& text);
private:
    char lower_ascii(char c);
};

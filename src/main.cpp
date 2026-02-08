#include <ostream>
#include <iostream>

#include "profane_detector/profane_detector.hpp"

int main(int argc, char** argv) {
    std::string lang = "en";
    std::string custom_dictionary = "";
    ProfaneDetector profane_detector(lang, custom_dictionary);

    auto start = std::chrono::high_resolution_clock::now();

    std::string str = "ishit"; 
    auto result = profane_detector.censor(str, '*');
    std::cout << result << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
    std::cout << result << std::endl;

    return 0;
}
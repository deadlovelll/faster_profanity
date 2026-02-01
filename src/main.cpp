#include <ostream>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

#include "profane_detector/profane_detector.hpp"

int main(int argc, char** argv) {
    ProfaneDetector profane_detector;

    // старт таймера
    auto start = std::chrono::high_resolution_clock::now();

    std::string str = "ssssshhhhhhhiiiiiitttttttttttttt"; 
    auto result = profane_detector.censor(str, '*');
    std::cout << result << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
    std::cout << result << std::endl;

    return 0;
}
#include <ostream>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

#include "censor_words_loader/censor_words_loader.hpp"
#include "swear_word_hider/swear_word_hider.hpp"

int main(int argc, char** argv) {
    CensorWordsLoader cwd;
    cwd.load_censor_words();
    auto listtt = cwd.get_censor_words();
    SwearWordHider swh(listtt);

    int undetected = 0;

    for (const auto& ls : listtt) {
        std::string result = swh.hide(ls, '*');
        if (ls == result) {
            undetected++;
        }
    }

    // старт таймера
    auto start = std::chrono::high_resolution_clock::now();

    auto result = swh.hide("dog style", '/');

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Total undetected = " << undetected << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
    std::cout << result << std::endl;

    return 0;
}
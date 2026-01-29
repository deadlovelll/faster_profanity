#include <ostream>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

#include "censor_words_loader/censor_words_loader.hpp"
#include "swear_word_hider/swear_word_hider.hpp"

int main(int argc, char** argv) {
    CensorWordsLoader cwd;
    SwearWordHider swh;
    cwd.load_censor_words();
    int undetected = 0;
    auto listtt = cwd.get_censor_words();
    for (auto ls: listtt) {
        std::string result = swh.hide(ls, '*', listtt);
        if (ls == result) {
            undetected++;
        }
    }
    std::cout << "Total undetected = " << undetected << std::endl;
}
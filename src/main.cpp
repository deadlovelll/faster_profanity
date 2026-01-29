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
    for (auto ls: listtt) {
        std::cout << ls << std::endl;
    }
    SwearWordHider swh;
    std::string text = "bitchsukablyatpidorasina";
    std::string result = swh.hide(text, '*', listtt);
    std::cout << result << std::endl;
}
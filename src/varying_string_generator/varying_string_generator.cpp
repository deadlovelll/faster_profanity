#include <unordered_set>

#include "varying_string_generator/varying_string_generator.hpp"

void VaryingStringGenerator::generate_recursive(
    const std::string& word,
    size_t index,
    std::string& current,
    std::unordered_set<std::string>& out
) {
    if (index == word.size()) {
        out.insert(current);
        return;
    }

    char c = word[index];
    auto it = CHARS_MAPPING.find(c);
    const std::vector<std::string>& replacements = (
        (it != CHARS_MAPPING.end()) ?
        it->second : std::vector<std::string>{std::string(1, c)}
    );

    for (const auto& rep : replacements) {
        current += rep;
        generate_recursive(word, index + 1, current, out);
        current.resize(current.size() - rep.size());
    }
}

void VaryingStringGenerator::generate_word_combinations(
    std::unordered_set<std::string>& censor_full_list,
    std::string& word
) {
    std::string current;
    generate_recursive(word, 0, current, censor_full_list);
}
#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(const std::string &text, const char censor_char) {
    std::string censored_text = text;
    unicode_normalizer.normalize(censored_text);
    for (const Token& token : find_profane_tokens(censored_text)) {
        censored_text.replace(token.start, token.end - token.start, token.end - token.start, censor_char);
    }
    return censored_text;
}

bool ProfaneDetector::contains_profanity(const std::string &text) {
    std::string normalized_text = text;
    unicode_normalizer.normalize(normalized_text);
    return !find_profane_tokens(normalized_text).empty();
}

std::vector<Token> ProfaneDetector::find_profane_tokens(const std::string& normalized_text) {
    std::vector<Token> tokens = tokenizer.tokenize(normalized_text);
    std::string hidden = swear_word_hider.hide(tokenizer.join(tokens), '*', root.get());
    std::vector<Token> profane_tokens;
    std::size_t offset = 0;
    for (const Token& token : tokens) {
        if (hidden.find_first_not_of('*', offset) >= offset + token.text.size()) {
            profane_tokens.push_back(token);
        }
        offset += token.text.size() + 1;
    }
    return profane_tokens;
}

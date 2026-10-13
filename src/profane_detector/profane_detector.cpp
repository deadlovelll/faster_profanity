#include "profane_detector/profane_detector.hpp"

std::string ProfaneDetector::censor(const std::string &text, const char censor_char) {
    std::string normalized_text = text;
    std::vector<std::size_t> source_offsets;
    unicode_normalizer.normalize(normalized_text, source_offsets);
    std::string censored_text;
    std::size_t copied = 0;
    for (const Token& token : find_profane_tokens(normalized_text)) {
        std::size_t start = source_offsets[token.start];
        censored_text.append(text, copied, start - copied);
        std::string span = normalized_text.substr(token.start, token.end - token.start);
        censored_text.append(utf8_sizer.utf8_word_length(span), censor_char);
        copied = source_offsets[token.end];
    }
    censored_text.append(text, copied);
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

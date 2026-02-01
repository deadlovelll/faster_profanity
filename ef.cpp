#include <cctype>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "swear_word_hider/swear_word_hider.hpp"
#include "state/state.hpp"

// Checks if char is valid or not, see swear_word_hider.hpp above
bool SwearWordHider::is_allowed_char(std::string_view utf8_char) const {
    return allowed_chars.contains(utf8_char);
}

std::string SwearWordHider::hide(
    const std::string& text, 
    char censor_char,
    Node* root
) {
    std::string result = text;
    std::string prev_char;

    std::vector<State> current_states;
    current_states.push_back({root, {}});

    for (size_t i = 0; i < text.size();) {
        std::vector<std::string> virtual_chars;
        std::vector<State> next_states;
        std::unordered_set<Node*> seen;

        size_t len = utf8_sizer.utf8_char_len(static_cast<unsigned char>(text[i]));
        std::string ch = text.substr(i, len);

        std::unordered_map<std::string, std::vector<char>>::const_iterator it = (
            reverse_map.find(ch)
        );
        if (it != reverse_map.end()) {
            for (char c: it->second) {
                virtual_chars.push_back(std::string(1, c));
            }
        } else {
            virtual_chars.push_back(ch);
        }

        next_states.push_back({root, {}});
        for (State& curr_state: current_states) {
            bool is_skipable = (
                // Check if it's a repeatable char
                prev_char == ch && !curr_state.node->children.count(ch)
                // Or it's a filler (not a char, e.g: ".", " ", ";" etc)
                || !allowed_chars.contains(ch)
            );
            if (!is_skipable && curr_state.node != root) {
                if (seen.insert(curr_state.node).second) {
                    next_states.push_back(curr_state);
                }
            }
            for (const std::string& v_ch: virtual_chars) {
                Node* temp_node = curr_state.node;
                while (temp_node != root && !temp_node->children.count(v_ch)) {
                    temp_node = temp_node->fail;
                }
                if (temp_node->children.count(v_ch)) {
                    Node* next_node = temp_node->children[v_ch].get();
                    if (seen.insert(next_node).second) {
                        std::vector<size_t> next_history = curr_state.history;
                        next_history.push_back(i);
                        if (!next_node->outputs.empty()) {
                            for (const std::string& word: next_node->outputs) {
                                size_t patter_len = utf8_sizer.utf8_word_length(word);
                                if (next_history.size() >= patter_len) {
                                    size_t start_pos = next_history[next_history.size() - patter_len];
                                    size_t end_pos = i + len;
                                    printf("Match: [%s], Bounds: [%c|%c]\n", 
                                        text.substr(start_pos, end_pos-start_pos).c_str(),
                                        start_pos > 0 ? text[start_pos-1] : '^',
                                        end_pos < text.size() ? text[end_pos] : '$');
                                        bool is_left_border_ok = (
                                            start_pos > 0 
                                            || std::isalpha((unsigned char) text[start_pos-1])
                                        );
                                        bool is_right_border_ok = (
                                            end_pos < text.size()
                                            || std::isalpha((unsigned char) text[end_pos])
                                        );
                                        if (!is_left_border_ok && !is_right_border_ok) {
                                            for (size_t j = start_pos; j < end_pos; ++j) {
                                                result[j] = censor_char;
                                            } 
                                        }
                                }
                            }
                        }
                        next_states.push_back({next_node, std::move(next_history)});
                    }
                }
            }
        }
        current_states = std::move(next_states);
        prev_char = ch;
        i += len;
    }
    return result;
}
#include "confusables_mapper/confusables_mapper.hpp"
#include "confusables_mapper/confusables.hpp"

#include <algorithm>

// Lookalikes below U+10000 are put in a table once, at program start: one lookup per letter
const ConfusablesMapper::Table ConfusablesMapper::table = build_table();

char ConfusablesMapper::to_latin(char32_t code_point) {
    if (code_point < table.size()) {
        return table[code_point];
    }
    // Rare letters beyond U+FFFF: binary search in the sorted list
    std::array<Confusable, confusables.size()>::const_iterator found = std::ranges::lower_bound(
        confusables, code_point, {}, &Confusable::code_point
    );
    if (found != confusables.end() && found->code_point == code_point) {
        return found->latin;
    }
    return 0;
}

// Latin letter for every lookalike below U+10000, 0 for the rest
ConfusablesMapper::Table ConfusablesMapper::build_table() {
    Table built{};
    for (const Confusable& confusable : confusables) {
        if (confusable.code_point < built.size()) {
            built[confusable.code_point] = confusable.latin;
        }
    }
    return built;
}

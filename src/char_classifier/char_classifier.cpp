#include "char_classifier/char_classifier.hpp"

#include <utf8proc.h>

// One property lookup per character
CharKind CharClassifier::classify(char32_t code_point) {
    const utf8proc_property_t* property = utf8proc_get_property(static_cast<utf8proc_int32_t>(code_point));
    if (
        // invisible: ZWSP, soft hyphen
        property->ignorable
        // has a decomposition: Ｆ -> F, é -> e + U+0301
        || property->decomp_seqindex != UINT16_MAX
        // fold differs from lowercase: ß -> ss
        || property->casefold_seqindex != property->lowercase_seqindex
        // standalone accent: U+0301
        || property->category == UTF8PROC_CATEGORY_MN
        // standalone enclosing mark: U+20DD
        || property->category == UTF8PROC_CATEGORY_ME) [[unlikely]]
    {
        return classify_special(code_point, property);
    }
    return CharKind::Regular;
}

CharKind CharClassifier::classify_special(char32_t code_point, const utf8proc_property_t* property) {
    // invisible: ZWSP, soft hyphen
    if (property->ignorable) {
        return CharKind::Invisible;
    }
    // compatibility decomposition: Ｆ -> F, ﬁ -> fi
    if (property->decomp_type != 0) {
        return CharKind::Compatibility;
    }
    if (
        // accented letter: é -> e + U+0301
        property->decomp_seqindex != UINT16_MAX
        // standalone accent: Zalgo, f + U+0336
        || diacritics_stripper.is_diacritic_mark(code_point))
    {
        return CharKind::Diacritic;
    }
    // fold differs from lowercase: ß -> ss
    if (property->casefold_seqindex != property->lowercase_seqindex) {
        return CharKind::CaseFolding;
    }
    return CharKind::Regular;
}

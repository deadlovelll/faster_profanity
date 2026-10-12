#pragma once

struct utf8proc_property_struct;

// Which normalization step a character needs. Regular ones are only lowercased
enum class CharKind { Invisible, Compatibility, CaseFolding, Diacritic, Regular };

class CharClassifier {
public:
    CharKind classify(char32_t code_point);
private:
    CharKind classify_special(char32_t code_point, const utf8proc_property_struct* property);
};

#include "unicode_normalizer/unicode_normalizer.hpp"
#include "confusables_mapper/confusables_mapper.hpp"

#include <utf8proc.h>

void UnicodeNormalizer::normalize(std::string& text) {
    normalize_text<false>(text, nullptr);
}

void UnicodeNormalizer::normalize(std::string& text, std::vector<std::size_t>& source_offsets) {
    normalize_text<true>(text, &source_offsets);
}

template <bool with_offsets>
void UnicodeNormalizer::normalize_text(std::string& text, std::vector<std::size_t>* source_offsets) {
    std::string out;
    out.reserve(text.size());

    const utf8proc_uint8_t* bytes = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
    const utf8proc_ssize_t size = static_cast<utf8proc_ssize_t>(text.size());
    utf8proc_ssize_t pos = 0;
    utf8proc_ssize_t unit_start = 0;
    if constexpr (with_offsets) {
        source_offsets->clear();
        source_offsets->reserve(text.size() + 1);
    }

    while (pos < size) {
        if constexpr (with_offsets) {
            while (source_offsets->size() < out.size()) {
                source_offsets->push_back(static_cast<std::size_t>(unit_start));
            }
            unit_start = pos;
        }
        // ASCII fast path: no ASCII character is invisible
        if (bytes[pos] < 0x80) {
            out.push_back(lower_ascii(static_cast<char>(bytes[pos])));
            ++pos;
            continue;
        }

        utf8proc_int32_t code_point;
        utf8proc_ssize_t len = utf8proc_iterate(bytes + pos, size - pos, &code_point);
        if (len < 0) {
            out.push_back(static_cast<char>(bytes[pos]));
            ++pos;
            continue;
        }

        char32_t current = static_cast<char32_t>(code_point);
        CharKind kind = char_classifier.classify(current);
        recompose_separate_letter(bytes, size, pos, len, current, kind);
        switch (kind) {
            case CharKind::Invisible:
                pos += len;
                continue;
            case CharKind::Space:
                out.push_back(' ');
                pos += len;
                continue;
            case CharKind::CaseFolding:
                append_case_folded(out, current);
                pos += len;
                continue;
            case CharKind::Compatibility:
                if (append_compatibility_form(out, current)) {
                    pos += len;
                    continue;
                }
                [[fallthrough]];
            case CharKind::Diacritic:
                append_without_diacritics(out, current);
                pos += len;
                continue;
            case CharKind::Regular:
                break;
        }

        // Checked before lowering: some letters look Latin only in uppercase, Cyrillic В -> b
        char latin = ConfusablesMapper::to_latin(current);
        if (latin != 0) {
            out.push_back(latin);
            pos += len;
            continue;
        }

        if (is_regional_indicator(current)) [[unlikely]] {
            if (is_regional_indicator_at(text, pos - 4) || is_regional_indicator_at(text, pos + len)) {
                out.append(text, static_cast<std::size_t>(pos), static_cast<std::size_t>(len));
            } else {
                out.push_back(static_cast<char>('a' + (current - U'\U0001F1E6')));
            }
            pos += len;
            continue;
        }

        char32_t lowered = lower(current);
        if (lowered == current) {
            out.append(text, static_cast<std::size_t>(pos), static_cast<std::size_t>(len));
        } else {
            append_code_point(out, lowered);
        }
        pos += len;
    }
    if constexpr (with_offsets) {
        source_offsets->resize(out.size(), static_cast<std::size_t>(unit_start));
        source_offsets->push_back(text.size());
    }
    text.swap(out);
}

void UnicodeNormalizer::recompose_separate_letter(
    const std::uint8_t* bytes, std::ptrdiff_t size, std::ptrdiff_t pos, std::ptrdiff_t& len,
    char32_t& code_point, CharKind& kind
) {
    if (
        kind == CharKind::Regular && pos + len < size && bytes[pos + len] == 0xCC
        && diacritics_stripper.is_separate_letter_base(code_point)) [[unlikely]]
    {
        utf8proc_int32_t mark;
        utf8proc_ssize_t mark_len = utf8proc_iterate(bytes + pos + len, size - pos - len, &mark);
        char32_t separate_letter = diacritics_stripper.compose_separate_letter(code_point, static_cast<char32_t>(mark));
        if (separate_letter != 0) {
            code_point = separate_letter;
            len += mark_len;
            kind = CharKind::Diacritic;
        }
    }
}

bool UnicodeNormalizer::append_compatibility_form(std::string& out, char32_t code_point) {
    // The longest compatibility decomposition is 18 code points (U+FDFA).
    // 32 leaves headroom for future.
    // See https://stackoverflow.com/questions/61779904
    const utf8proc_ssize_t capacity = 32;
    utf8proc_int32_t decomposed[capacity];
    utf8proc_int32_t boundclass = UTF8PROC_BOUNDCLASS_START;
    utf8proc_ssize_t count = utf8proc_decompose_char(
        static_cast<utf8proc_int32_t>(code_point), decomposed, capacity,
        static_cast<utf8proc_option_t>(UTF8PROC_DECOMPOSE | UTF8PROC_COMPAT), &boundclass
    );
    for (utf8proc_ssize_t i = 0; count <= capacity && i < count; ++i) {
        if (diacritics_stripper.is_diacritic_mark(static_cast<char32_t>(decomposed[i]))) {
            return false;
        }
    }
    if (count > 0 && count <= capacity) {
        count = utf8proc_normalize_utf32(
            decomposed, count, static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE)
        );
    }
    if (count <= 0 || count > capacity) {
        append_code_point(out, lower(code_point));
        return true;
    }

    // There can be cases that leave the parentheses:
    // - ⑽ ... ⒇ (U+247D-U+2487) -> "(10)" ... "(20)"
    // - ㈝ (U+321D) -> "(오전)"
    // - ㈞ (U+321E) -> "(오후)"
    if (count == 3 && decomposed[0] == '(' && decomposed[2] == ')') {
        decomposed[0] = decomposed[1];
        count = 1;
    }

    for (utf8proc_ssize_t i = 0; i < count; ++i) {
        append_code_point(out, lower(static_cast<char32_t>(decomposed[i])));
    }
    return true;
}

void UnicodeNormalizer::append_case_folded(std::string& out, char32_t code_point) {
    // The longest full case folding is 3 code points (ΐ)
    const utf8proc_ssize_t capacity = 4;
    utf8proc_int32_t folded[capacity];
    utf8proc_int32_t boundclass = UTF8PROC_BOUNDCLASS_START;
    utf8proc_ssize_t count = utf8proc_decompose_char(
        static_cast<utf8proc_int32_t>(lower(code_point)), folded, capacity, UTF8PROC_CASEFOLD, &boundclass
    );
    if (count > 1 && count <= capacity) {
        count = utf8proc_normalize_utf32(
            folded, count, static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE)
        );
    }
    if (count <= 0 || count > capacity) {
        append_code_point(out, lower(code_point));
        return;
    }

    for (utf8proc_ssize_t i = 0; i < count; ++i) {
        append_code_point(out, static_cast<char32_t>(folded[i]));
    }
}

void UnicodeNormalizer::append_without_diacritics(std::string& out, char32_t code_point) {
    char32_t stripped[4];
    int count = diacritics_stripper.strip(code_point, stripped);
    for (int i = 0; i < count; ++i) {
        append_code_point(out, stripped[i]);
    }
}

void UnicodeNormalizer::append_code_point(std::string& out, char32_t code_point) {
    if (code_point < 0x80) {
        out.push_back(static_cast<char>(code_point));
        return;
    }
    char latin = ConfusablesMapper::to_latin(code_point);
    if (latin != 0) {
        out.push_back(latin);
        return;
    }
    utf8proc_uint8_t encoded[4];
    utf8proc_ssize_t encoded_len = utf8proc_encode_char(static_cast<utf8proc_int32_t>(code_point), encoded);
    out.append(reinterpret_cast<const char*>(encoded), static_cast<std::size_t>(encoded_len));
}

bool UnicodeNormalizer::is_regional_indicator(char32_t code_point) {
    return code_point >= U'\U0001F1E6' && code_point <= U'\U0001F1FF';
}

bool UnicodeNormalizer::is_regional_indicator_at(const std::string& text, std::ptrdiff_t pos) {
    if (pos < 0 || pos + 4 > static_cast<std::ptrdiff_t>(text.size())) {
        return false;
    }
    utf8proc_int32_t code_point;
    utf8proc_ssize_t len = utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(text.data()) + pos, 4, &code_point);
    return len == 4 && is_regional_indicator(static_cast<char32_t>(code_point));
}

char32_t UnicodeNormalizer::lower(char32_t code_point) {
    return static_cast<char32_t>(utf8proc_tolower(static_cast<utf8proc_int32_t>(code_point)));
}

char UnicodeNormalizer::lower_ascii(char c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<char>(c - 'A' + 'a');
    }
    return c;
}

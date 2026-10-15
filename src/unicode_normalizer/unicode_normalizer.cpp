#include "unicode_normalizer/unicode_normalizer.hpp"
#include "confusables_mapper/confusables_mapper.hpp"

#include <algorithm>
#include <cstring>

#include <utf8proc.h>

void UnicodeNormalizer::normalize(std::string& text) {
    normalize_text<false>(text, nullptr);
}

void UnicodeNormalizer::normalize(std::string& text, std::vector<std::size_t>& source_offsets) {
    normalize_text<true>(text, &source_offsets);
}

template <bool with_offsets>
void UnicodeNormalizer::normalize_text(std::string& text, std::vector<std::size_t>* source_offsets) {
    std::string normalized;
    OutputBuffer out(normalized, text.size());
    if constexpr (with_offsets) {
        reset_offsets(*source_offsets, text.size());
    }
    const std::string_view source = text;
    const std::ptrdiff_t size = static_cast<std::ptrdiff_t>(source.size());
    std::ptrdiff_t pos = 0;
    while (pos < size) {
        SourceChar source_char = read_char(source, pos);
        CharKind kind = classify_char(source_char);
        recompose_separate_letter(source, source_char, kind);
        switch (kind) {
            case CharKind::Ascii:
                out.push_back(lower_ascii(source[static_cast<std::size_t>(source_char.start)]));
                break;
            case CharKind::Invalid:
                out.push_back(source[static_cast<std::size_t>(source_char.start)]);
                break;
            case CharKind::Invisible:
                break;
            case CharKind::Space:
                out.push_back(' ');
                break;
            case CharKind::CaseFolding:
                out = append_case_folded(out, source_char.code_point);
                break;
            case CharKind::Compatibility:
                out = append_compatibility_form(out, source_char.code_point);
                break;
            case CharKind::Diacritic:
                out = append_without_diacritics(out, source_char.code_point);
                break;
            case CharKind::Mark:
                if (diacritics_stripper.is_same_script(last_code_point(out.view()), source_char.code_point)) {
                    out = append_source(out, source_char, source);
                }
                break;
            case CharKind::Regular: {
                // Checked before lowering: some letters look Latin only in uppercase, Cyrillic В -> b
                char latin = ConfusablesMapper::to_latin(source_char.code_point);
                if (latin != 0) {
                    out.push_back(latin);
                    break;
                }
                if (is_regional_indicator(source_char.code_point)) [[unlikely]] {
                    out = append_regional_indicator(out, source_char, source);
                    break;
                }
                char32_t lowered = lower(source_char.code_point);
                if (lowered == source_char.code_point) {
                    out = append_source(out, source_char, source);
                } else {
                    out = append_code_point(out, lowered);
                }
                break;
            }
        }
        if constexpr (with_offsets) {
            record_offsets(*source_offsets, out.size(), source_char.start);
        }
        pos = source_char.end;
    }
    if constexpr (with_offsets) {
        close_offsets(*source_offsets, text.size());
    }
    out.finish();
    text.swap(normalized);
}

void UnicodeNormalizer::reset_offsets(std::vector<std::size_t>& source_offsets, std::size_t text_size) {
    source_offsets.clear();
    source_offsets.reserve(text_size + 1);
}

void UnicodeNormalizer::record_offsets(
    std::vector<std::size_t>& source_offsets, std::size_t out_size, std::ptrdiff_t char_start
) {
    while (source_offsets.size() < out_size) {
        source_offsets.push_back(static_cast<std::size_t>(char_start));
    }
}

void UnicodeNormalizer::close_offsets(std::vector<std::size_t>& source_offsets, std::size_t text_size) {
    source_offsets.push_back(text_size);
}

UnicodeNormalizer::SourceChar UnicodeNormalizer::read_char(std::string_view text, std::ptrdiff_t pos) {
    const utf8proc_uint8_t* bytes = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
    if (bytes[pos] < 0x80) {
        return {bytes[pos], pos, pos + 1};
    }
    utf8proc_int32_t code_point;
    utf8proc_ssize_t len = utf8proc_iterate(bytes + pos, static_cast<utf8proc_ssize_t>(text.size()) - pos, &code_point);
    return {static_cast<char32_t>(code_point), pos, pos + (len > 0 ? len : 1)};
}

CharKind UnicodeNormalizer::classify_char(SourceChar source_char) {
    if (source_char.code_point < 0x80) {
        return CharKind::Ascii;
    }
    if (source_char.code_point > 0x10FFFF) {
        return CharKind::Invalid;
    }
    return char_classifier.classify(source_char.code_point);
}

void UnicodeNormalizer::recompose_separate_letter(std::string_view text, SourceChar& source_char, CharKind& kind) {
    const utf8proc_uint8_t* bytes = reinterpret_cast<const utf8proc_uint8_t*>(text.data());
    const utf8proc_ssize_t size = static_cast<utf8proc_ssize_t>(text.size());
    if (
        kind == CharKind::Regular && source_char.end < size && bytes[source_char.end] == 0xCC
        && diacritics_stripper.is_separate_letter_base(source_char.code_point)) [[unlikely]]
    {
        utf8proc_int32_t mark;
        utf8proc_ssize_t mark_len = utf8proc_iterate(bytes + source_char.end, size - source_char.end, &mark);
        char32_t separate_letter = diacritics_stripper.compose_separate_letter(source_char.code_point, static_cast<char32_t>(mark));
        if (separate_letter != 0) {
            source_char.code_point = separate_letter;
            source_char.end += mark_len;
            kind = CharKind::Diacritic;
        }
    }
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_regional_indicator(
    OutputBuffer out, SourceChar source_char, std::string_view text
) {
    if (is_regional_indicator_at(text, source_char.start - 4) || is_regional_indicator_at(text, source_char.end)) {
        out = append_source(out, source_char, text);
    } else {
        out.push_back(static_cast<char>('a' + (source_char.code_point - U'\U0001F1E6')));
    }
    return out;
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_source(
    OutputBuffer out, SourceChar source_char, std::string_view text
) {
    out.append(text.data() + source_char.start, static_cast<std::size_t>(source_char.end - source_char.start));
    return out;
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_compatibility_form(OutputBuffer out, char32_t code_point) {
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
            return append_without_diacritics(out, code_point);
        }
    }
    if (count > 0 && count <= capacity) {
        count = utf8proc_normalize_utf32(
            decomposed, count, static_cast<utf8proc_option_t>(UTF8PROC_STABLE | UTF8PROC_COMPOSE)
        );
    }
    if (count <= 0 || count > capacity) {
        return append_code_point(out, lower(code_point));
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
        out = append_code_point(out, lower(static_cast<char32_t>(decomposed[i])));
    }
    return out;
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_case_folded(OutputBuffer out, char32_t code_point) {
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
        return append_code_point(out, lower(code_point));
    }

    for (utf8proc_ssize_t i = 0; i < count; ++i) {
        out = append_code_point(out, static_cast<char32_t>(folded[i]));
    }
    return out;
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_without_diacritics(OutputBuffer out, char32_t code_point) {
    char32_t stripped[4];
    int count = diacritics_stripper.strip(code_point, stripped);
    for (int i = 0; i < count; ++i) {
        out = append_code_point(out, stripped[i]);
    }
    return out;
}

UnicodeNormalizer::OutputBuffer UnicodeNormalizer::append_code_point(OutputBuffer out, char32_t code_point) {
    if (code_point < 0x80) {
        out.push_back(static_cast<char>(code_point));
        return out;
    }
    char latin = ConfusablesMapper::to_latin(code_point);
    if (latin != 0) {
        out.push_back(latin);
        return out;
    }
    utf8proc_uint8_t encoded[4];
    utf8proc_ssize_t encoded_len = utf8proc_encode_char(static_cast<utf8proc_int32_t>(code_point), encoded);
    out.append(reinterpret_cast<const char*>(encoded), static_cast<std::size_t>(encoded_len));
    return out;
}

bool UnicodeNormalizer::is_regional_indicator(char32_t code_point) {
    return code_point >= U'\U0001F1E6' && code_point <= U'\U0001F1FF';
}

bool UnicodeNormalizer::is_regional_indicator_at(std::string_view text, std::ptrdiff_t pos) {
    if (pos < 0 || pos + 4 > static_cast<std::ptrdiff_t>(text.size())) {
        return false;
    }
    utf8proc_int32_t code_point;
    utf8proc_ssize_t len = utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(text.data()) + pos, 4, &code_point);
    return len == 4 && is_regional_indicator(static_cast<char32_t>(code_point));
}

char32_t UnicodeNormalizer::last_code_point(std::string_view text) {
    std::size_t start = text.size();
    while (start > 0 && text.size() - start < 4) {
        --start;
        if ((static_cast<unsigned char>(text[start]) & 0xC0) != 0x80) {
            utf8proc_int32_t code_point;
            utf8proc_ssize_t len = utf8proc_iterate(
                reinterpret_cast<const utf8proc_uint8_t*>(text.data()) + start,
                static_cast<utf8proc_ssize_t>(text.size() - start), &code_point
            );
            return len == static_cast<utf8proc_ssize_t>(text.size() - start) ? static_cast<char32_t>(code_point) : 0;
        }
    }
    return 0;
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

UnicodeNormalizer::OutputBuffer::OutputBuffer(std::string& storage, std::size_t capacity)
    : storage(&storage), data(nullptr), used(0), capacity(capacity) {
    storage.resize(capacity);
    data = storage.data();
}

void UnicodeNormalizer::OutputBuffer::push_back(char c) {
    if (used == capacity) [[unlikely]] {
        data = grow(*storage, used + 1);
        capacity = storage->size();
    }
    data[used] = c;
    ++used;
}

void UnicodeNormalizer::OutputBuffer::append(const char* bytes, std::size_t count) {
    if (capacity - used < count) [[unlikely]] {
        data = grow(*storage, used + count);
        capacity = storage->size();
    }
    std::memcpy(data + used, bytes, count);
    used += count;
}

std::size_t UnicodeNormalizer::OutputBuffer::size() const {
    return used;
}

std::string_view UnicodeNormalizer::OutputBuffer::view() const {
    return std::string_view(data, used);
}

void UnicodeNormalizer::OutputBuffer::finish() {
    storage->resize(used);
}

char* UnicodeNormalizer::OutputBuffer::grow(std::string& storage, std::size_t needed) {
    storage.resize(std::max(storage.size() * 2, needed));
    return storage.data();
}

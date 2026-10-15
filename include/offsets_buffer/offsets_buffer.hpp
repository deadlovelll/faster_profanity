#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

class [[nodiscard]] OffsetsBuffer {
public:
    OffsetsBuffer() : storage(nullptr), data(nullptr), used(0), capacity(0) {
    }
    OffsetsBuffer(std::vector<std::size_t>& storage, std::size_t capacity)
        : storage(&storage), data(nullptr), used(0), capacity(capacity) {
        storage.resize(capacity);
        data = storage.data();
    }
    void push_back(std::size_t offset) {
        if (used == capacity) [[unlikely]] {
            data = grow(*storage, used + 1);
            capacity = storage->size();
        }
        data[used] = offset;
        ++used;
    }
    std::size_t size() const {
        return used;
    }
    void finish() {
        storage->resize(used);
    }
private:
    std::vector<std::size_t>* storage;
    std::size_t* data;
    std::size_t used;
    std::size_t capacity;

    static std::size_t* grow(std::vector<std::size_t>& storage, std::size_t needed) {
        storage.resize(std::max(storage.size() * 2, needed));
        return storage.data();
    }
};

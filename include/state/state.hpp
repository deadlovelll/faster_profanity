#pragma once

#include "node/node.hpp"

#include <cstddef>
#include <vector>

struct State {
    Node* node;
    std::vector<size_t> history;
};
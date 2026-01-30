#include <unordered_map>
#include <vector>

struct Node {
    std::unordered_map<std::string, std::unique_ptr<Node>> children;
    Node* fail = nullptr;
    std::vector<std::string> outputs;
};

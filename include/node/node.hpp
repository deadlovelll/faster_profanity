#include <unordered_map>
#include <vector>

struct Node {
    std::unordered_map<std::string, Node*> children;
    Node* fail = nullptr;
    std::vector<std::string> outputs;
};

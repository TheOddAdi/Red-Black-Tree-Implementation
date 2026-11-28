#include "RedBlackTree.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <vector>

template <typename Node>
int checkBlackHeight(const Node* node, const Node* parent) {
    if (node == nullptr) {
        return 1;
    }

    using Color = decltype(node->color);
    assert(node->parent == parent);
    assert(node->color == Color::Red || node->color == Color::Black);
    if (node->color == Color::Red) {
        assert(!node->left || node->left->color == Color::Black);
        assert(!node->right || node->right->color == Color::Black);
    }

    int left = checkBlackHeight<Node>(node->left, node);
    int right = checkBlackHeight<Node>(node->right, node);
    assert(left == right);
    return left + (node->color == Color::Black);
}

void checkTree(RedBlackTree& tree, const std::multiset<int>& expected) {
    auto root = tree.getRoot();
    using Color = decltype(root->color);
    assert(!root || root->color == Color::Black);
    checkBlackHeight(root, static_cast<decltype(root)>(nullptr));
    assert(tree.inOrder() == std::vector<int>(expected.begin(), expected.end()));
}

void testBasicMethods() {
    RedBlackTree tree;
    assert(tree.inOrder().empty());
    assert(tree.search(10) == nullptr);

    for (int key : {20, 10, 30}) {
        tree.insert(key);
    }
    assert(tree.search(10) != nullptr);
    assert(tree.search(99) == nullptr);
    assert(tree.getMin(tree.getRoot())->data == 10);
    assert(tree.getMax(tree.getRoot())->data == 30);
    checkTree(tree, {10, 20, 30});

    tree.deleteNode(tree.getRoot(), 99);
    checkTree(tree, {10, 20, 30});
    tree.deleteNode(tree.getRoot(), 20);
    checkTree(tree, {10, 30});
    tree.deleteNode(tree.getRoot(), 10);
    checkTree(tree, {30});
    tree.deleteNode(tree.getRoot(), 30);
    checkTree(tree, {});
}

void testInsertAndDeleteTogether() {
    const int insertionOrders[][8] = {
        {30, 20, 10, 25, 40, 5, 20, 35},
        {10, 20, 30, 25, 40, 5, 20, 35},
        {30, 10, 20, 25, 40, 5, 20, 35},
        {10, 30, 20, 25, 40, 5, 20, 35}
    };

    for (const auto& keys : insertionOrders) {
        RedBlackTree tree;
        std::multiset<int> expected;
        for (int key : keys) {
            tree.insert(key);
            expected.insert(key);
            checkTree(tree, expected);
        }

        for (int key : {20, 5, 30, 20, 40, 25, 10, 35}) {
            tree.deleteNode(tree.getRoot(), key);
            expected.erase(expected.find(key));
            checkTree(tree, expected);
            assert((tree.search(key) != nullptr) == (expected.count(key) != 0));
        }
    }
}

int main() {
    testBasicMethods();
    testInsertAndDeleteTogether();
    std::cout << "All red-black tree tests passed.\n";
}

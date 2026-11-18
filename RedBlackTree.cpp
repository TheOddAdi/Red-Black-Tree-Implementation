#include "RedBlackTree.hpp"

void RedBlackTree::insert(int data) {
    Node* newNode = createNode(data);
    if (root == nullptr) {
        root = newNode;
        root->color = Color::Black; // Root must be black
        return;
    }
    return;
}

RedBlackTree::Node* RedBlackTree::search(int data) {
    if (root == nullptr) {
        return nullptr;
    } else {
        Node* current = root;
        while (current != nullptr) {
            if (data == current->data) {
                return current;
            } else if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
}

RedBlackTree::Node* RedBlackTree::getRoot() {
    return root;
}

RedBlackTree::Node* RedBlackTree::getMin(RedBlackTree::Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

RedBlackTree::Node* RedBlackTree::getMax(RedBlackTree::Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

RedBlackTree::Node* RedBlackTree::deleteNode(RedBlackTree::Node* root, int data) { 
    return createNode(0);
}

RedBlackTree::Node* RedBlackTree::createNode(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    newNode->color = Color::Red;
    return newNode;
}
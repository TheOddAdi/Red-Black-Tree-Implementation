#include "RedBlackTree.hpp"
#include <utility>

void RedBlackTree::insert(int data) {
    Node* newNode = createNode(data);
    if (root == nullptr) {
        root = newNode;
        root->color = Color::Black; // Root must be black
        return;
    }
    newNode->parent = locateParent(root, data);
    if (data < newNode->parent->data) {
        newNode->parent->left = newNode;
    } else {
        newNode->parent->right = newNode;
    }
    newNode->color = Color::Red;
    newNode->left = nullptr;
    newNode->right = nullptr;
    insertFixup(newNode);
}

RedBlackTree::Node* RedBlackTree::getUncle(RedBlackTree::Node* node) {
    if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr;
    }
    Node* grandparent = node->parent->parent;
    if (node->parent == grandparent->left) {
        return grandparent->right;
    } else {
        return grandparent->left;
    }
}

RedBlackTree::Color RedBlackTree::colorOf(const Node* node) {
    return node == nullptr ? Color::Black : node->color;
}

void RedBlackTree::replaceNode(Node* node, Node* replacement) {
    Node* parent = node->parent;
    if (parent == nullptr) {
        root = replacement;
    } else if (node == parent->left) {
        parent->left = replacement;
    } else {
        parent->right = replacement;
    }
    if (replacement != nullptr) {
        replacement->parent = parent;
    }
}

void RedBlackTree::rotateLeft(Node* node) {
    Node* pivot = node->right;
    node->right = pivot->left;
    if (node->right != nullptr) {
        node->right->parent = node;
    }
    replaceNode(node, pivot);
    pivot->left = node;
    node->parent = pivot;
}

void RedBlackTree::rotateRight(Node* node) {
    Node* pivot = node->left;
    node->left = pivot->right;
    if (node->left != nullptr) {
        node->left->parent = node;
    }
    replaceNode(node, pivot);
    pivot->right = node;
    node->parent = pivot;
}

void RedBlackTree::insertFixup(Node* node) {
    if (node == nullptr) {
        return;
    }
    while (colorOf(node->parent) == Color::Red) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;
        Node* uncle = getUncle(node);
        if (colorOf(uncle) == Color::Red) {
            parent->color = Color::Black;
            uncle->color = Color::Black;
            grandparent->color = Color::Red;
            node = grandparent;
            continue;
        }

        if (parent == grandparent->left) {
            if (node == parent->right) {
                rotateLeft(parent);
                std::swap(node, parent);
            }
            parent->color = Color::Black;
            grandparent->color = Color::Red;
            rotateRight(grandparent);
        } else {
            if (node == parent->left) {
                rotateRight(parent);
                std::swap(node, parent);
            }
            parent->color = Color::Black;
            grandparent->color = Color::Red;
            rotateLeft(grandparent);
        }
    }
    root->color = Color::Black;
}

std::vector<int> RedBlackTree::inOrder() const {
    std::vector<int> keys;
    appendInOrder(root, keys);
    return keys;
}

void RedBlackTree::appendInOrder(const Node* node, std::vector<int>& keys) {
    if (node == nullptr) {
        return;
    }
    appendInOrder(node->left, keys);
    keys.push_back(node->data);
    appendInOrder(node->right, keys);
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

RedBlackTree::Node* RedBlackTree::deleteNode(Node* subtree, int data) {
    Node* target = subtree;
    while (target != nullptr && target->data != data) {
        target = data < target->data ? target->left : target->right;
    }
    if (target == nullptr) {
        return root;
    }

    Color removedColor = target->color;
    Node* replacement;
    Node* replacementParent;
    if (target->left == nullptr || target->right == nullptr) {
        replacement = target->left != nullptr ? target->left : target->right;
        replacementParent = target->parent;
        replaceNode(target, replacement);
    } else {
        Node* successor = getMin(target->right);
        removedColor = successor->color;
        replacement = successor->right;
        replacementParent = successor;
        if (successor->parent != target) {
            replacementParent = successor->parent;
            replaceNode(successor, replacement);
            successor->right = target->right;
            successor->right->parent = successor;
        }
        replaceNode(target, successor);
        successor->left = target->left;
        successor->left->parent = successor;
        successor->color = target->color;
    }

    delete target;
    if (removedColor == Color::Black) {
        deleteFixup(replacement, replacementParent);
    }
    return root;
}

void RedBlackTree::deleteFixup(Node* node, Node* parent) {
    // A null replacement needs its parent passed separately.
    while (node != root && colorOf(node) == Color::Black) {
        const bool isLeftChild = node == parent->left;
        Node* sibling = isLeftChild ? parent->right : parent->left;
        const auto rotateParent = isLeftChild
            ? &RedBlackTree::rotateLeft : &RedBlackTree::rotateRight;
        const auto rotateSibling = isLeftChild
            ? &RedBlackTree::rotateRight : &RedBlackTree::rotateLeft;

        if (colorOf(sibling) == Color::Red) {
            sibling->color = Color::Black;
            parent->color = Color::Red;
            (this->*rotateParent)(parent);
            sibling = isLeftChild ? parent->right : parent->left;
        }

        Node* nearChild = sibling == nullptr ? nullptr
            : (isLeftChild ? sibling->left : sibling->right);
        Node* farChild = sibling == nullptr ? nullptr
            : (isLeftChild ? sibling->right : sibling->left);
        if (colorOf(nearChild) == Color::Black && colorOf(farChild) == Color::Black) {
            if (sibling != nullptr) {
                sibling->color = Color::Red;
            }
            node = parent;
            parent = node->parent;
            continue;
        }

        if (colorOf(farChild) == Color::Black) {
            nearChild->color = Color::Black;
            sibling->color = Color::Red;
            (this->*rotateSibling)(sibling);
            sibling = isLeftChild ? parent->right : parent->left;
            farChild = isLeftChild ? sibling->right : sibling->left;
        }

        sibling->color = parent->color;
        parent->color = Color::Black;
        farChild->color = Color::Black;
        (this->*rotateParent)(parent);
        return;
    }

    if (node != nullptr) {
        node->color = Color::Black;
    }
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

RedBlackTree::Node* RedBlackTree::locateParent(RedBlackTree::Node* node, int data) {
    if (node == nullptr) {
        return nullptr;
    }
    if (data < node->data) {
        if (node->left == nullptr) {
            return node;
        } else {
            return locateParent(node->left, data);
        }
    } else {
        if (node->right == nullptr) {
            return node;
        } else {
            return locateParent(node->right, data);
        }
    }
}

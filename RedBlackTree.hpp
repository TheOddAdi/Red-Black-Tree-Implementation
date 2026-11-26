class RedBlackTree {
private:
    enum class Color {
        Red,
        Black
    };
    struct Node {
        Color color;
        int data;
        Node *parent, *left, *right;
    };
    Node* root = nullptr;
    static Color colorOf(const Node* node);
    void replaceNode(Node* node, Node* replacement);
    void rotateLeft(Node* node);
    void rotateRight(Node* node);
    void deleteFixup(Node* node, Node* parent);
public:
    RedBlackTree() {
        root = nullptr;
    }
    void insert(int data);
    RedBlackTree::Node* search(int data);
    RedBlackTree::Node* getRoot();
    RedBlackTree::Node* getMin(RedBlackTree::Node* node);
    RedBlackTree::Node* getMax(RedBlackTree::Node* node);
    // Search from subtree and return the updated root of the whole tree.
    RedBlackTree::Node* deleteNode(RedBlackTree::Node* subtree, int data);
    RedBlackTree::Node* createNode(int data);
    RedBlackTree::Node* locateParent(RedBlackTree::Node* node, int data);
    RedBlackTree::Node* getUncle(RedBlackTree::Node* node);
    void insertFixup(RedBlackTree::Node* node);
};

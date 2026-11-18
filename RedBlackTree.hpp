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
public:
    RedBlackTree() {
        root = nullptr;
    }
    void insert(int data);
    RedBlackTree::Node* search(int data);
    RedBlackTree::Node* getRoot();
    RedBlackTree::Node* getMin(RedBlackTree::Node* node);
    RedBlackTree::Node* getMax(RedBlackTree::Node* node);
    RedBlackTree::Node* deleteNode(RedBlackTree::Node* root, int data);
    RedBlackTree::Node* createNode(int data);
    RedBlackTree::Node* locateParent(RedBlackTree::Node* node, int data);
    
};
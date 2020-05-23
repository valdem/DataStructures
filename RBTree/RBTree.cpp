#include "RBTree.h"

Node::Node(const ValueType& value, const ValueType key) {
    this->value = value;
    this->key = key;
    color = Red;
    left = right = parent = nullptr;
}

RBTree::RBTree() {
    root = nullptr;
    _size = 0;
}

void RBTree::forceNodeDelete(Node*& root) {
    if (root->left != nullptr && root->right != nullptr) {
        
        Node* nextDeleteNodeLeft = root->left;
        Node* nextDeleteNodeRight = root->right;
        delete root;
        forceNodeDelete(nextDeleteNodeLeft);
        forceNodeDelete(nextDeleteNodeRight);

    }
    else if (root->left != nullptr) {
        Node* nextDeleteNodeLeft = root->left;
        delete root;
        forceNodeDelete(nextDeleteNodeLeft);
    }
    else if(root->right != nullptr) {
        Node* nextDeleteNodeRight = root->right;
        delete root;
        forceNodeDelete(nextDeleteNodeRight);
    }
}


RBTree::~RBTree() {
    forceNodeDelete(root);
    _size = 0;
}

void RBTree::copyNodes(Node*& root) {
    if (root->left != nullptr && root->right != nullptr) {
        this->root->left = new Node(root->left->value, root->left->key);
        this->root->right = new Node(root->right->value, root->right->key);
        if (root->left->left != nullptr) {
            this->root->left->left = root->left->left;
        }
        if (root->right->right != nullptr) {
            this->root->right->right = root->right->right;
        }
        this->root->parent = root->parent;
        this->root->left->color = root->left->color;
        this->root->right->color = root->right->color;
        copyNodes(root->left);
        copyNodes(root->right);
    }
    else if (root->left != nullptr) {
        this->root->left = new Node(root->left->value, root->left->key);
        if (root->left->left) {
            this->root->left->left = root->left->left;
        }
        this->root->parent = root->parent;
        this->root->left->color = root->left->color;
        copyNodes(root->left);
    }
    else if(root->right != nullptr) {
        this->root->right = new Node(root->right->value, root->right->key);
        if (root->right->right) {
            this->root->right->right = root->right->right;
        }
        this->root->right = new Node(root->right->value, root->right->key);
        this->root->right->color = root->right->color;
        copyNodes(root->right);
    }
}

RBTree::RBTree(RBTree& copyTree) {
    this->_size = copyTree._size;
    this->root = new Node(copyTree.root->value, copyTree.root->key);
    copyNodes(copyTree.root);
}

RBTree& RBTree::operator=(RBTree& copyTree) {
    if (this == &copyTree) {
        return *this;
    }
    else {
        delete this;
        
        RBTree bufTree(copyTree);
        this->_size = bufTree._size;
        this->root = bufTree.root;
        
        bufTree._size = 0;
        bufTree.root = nullptr;
        
        return *this;
    }
}

RBTree::RBTree(RBTree&& moveTree) noexcept {
    this->_size = moveTree._size;
    this->root = moveTree.root;
    
    moveTree._size = 0;
    moveTree.root = nullptr;
}

RBTree& RBTree::operator=(RBTree&& moveTree) noexcept {
    if (this == &moveTree) {
        return *this;
    }
    else {
        delete this;
        this->_size = moveTree._size;
        this->root = moveTree.root;
        
        moveTree._size = 0;
        moveTree.root = nullptr;
        return *this;
    }
}

void RBTree::add(const ValueType key, const ValueType& value) {
    Node*& rootAdd = root;
    insertNode(rootAdd, key, value);
}

ValueType RBTree::find(ValueType key) {
    Node*& rootFind = root;
    return findNode(rootFind, key);
}

void RBTree::remove(ValueType key) {
    Node*& rootRemove = root;
    deleteOneNode(rootRemove, key);
}

Color RBTree::color(ValueType key) {
    Node* rootColor = root;
    return getColor(rootColor, key);
}

Color RBTree::getColor(Node*& root, ValueType key) {
    if (root->key == key) {
        return root->color;
    }
    if (key < root->key) {
        return getColor(root->left, key);
    }
    else {
        return getColor(root->right, key);
    }
}

void RBTree::insertNode(Node*& root, const ValueType key, const ValueType& value) {
    if (root == nullptr) {
        root = new Node(value, key);
        _size++;
        first_add_case(root);
        return;
    }
    if (root->key > key) {
        if (root->left == nullptr) {
            root->left = new Node(value, key);
            root->left->parent = root;
            first_add_case(root->left);
            _size++;
        }
        else {
            insertNode(root->left, key, value);
        }
    }
    else {
        if (root->right == nullptr) {
            root->right = new Node(value, key);
            root->right->parent = root;
            first_add_case(root->right);
            _size++;
        }
        else {
            insertNode(root->right, key, value);
        }
    }
}

void RBTree::deleteOneNode(Node*& root, ValueType key) {
    if (root == nullptr) {
        return;
    }
    if (key > root->key) {
        return deleteOneNode(root->right, key);
    }
    if (key < root->key) {
        return deleteOneNode(root->left, key);
    }
    if (key == root->key) {
        replaceAndDelete(root);
        delete root;
        _size--;
    }
}

void RBTree::deleteAllNodes(Node*& root, ValueType key) {
    while(find(key)) {
        deleteOneNode(root, key);
    }
}

ValueType RBTree::findNode(Node*& root, ValueType key) {
    if (root == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    if (root->key == key) {
        return root->value;
    }
    if (key < root->key) {
        return findNode(root->left, key);
    }
    else {
        return findNode(root->right, key);
    }
}

Node* RBTree::findMinNode(Node*& root) {
    Node* temp = root;
    while (temp->left != nullptr) {
        temp = temp->left;
    }
    return temp;
}

Node* RBTree::findMaxNode(Node*& root) {
    Node* temp = root;
    while (temp->right != nullptr) {
        temp = temp->right;
    }
    return temp;
}

size_t RBTree::size() {
    return _size;
}

bool RBTree::isEmpty() {
    return !size();
}

Node* RBTree::grandpa(Node*& node) {
    Node* grandpa = node->parent->parent;
    return grandpa;
}

Node* RBTree::uncle(Node*& node) {
    Node* uncle;
    if (grandpa(node)->left == node->parent) {
        uncle = grandpa(node)->right;
    }
    else {
        uncle = grandpa(node)->left;
    }
    return uncle;
}

Node* RBTree::sibling(Node*& node) {
    if (node == node->parent->left) {
        return node->parent->right;
    }
    else {
        return node->parent->left;
    }
}

void RBTree::first_add_case(Node*& node) {
    if (node == root) {
        node->color = Black;
    }
    else {
        second_add_case(node);
    }
}

void RBTree::second_add_case(Node*& node) {
    if (node->parent->color == Red) {
        third_add_case(node);
    }
    else {
        return;
    }
}

void RBTree::third_add_case(Node*& node) {
    if (uncle(node)!= nullptr && uncle(node)->color == Red) {
        node->parent->color = Black;
        uncle(node)->color = Black;
        grandpa(node)->color = Red;
        Node* g = grandpa(node);
        first_add_case(g);
    }
    else {
        fourth_add_case(node);
    }
}

void RBTree::fourth_add_case(Node*& node) {
    if (node == node->parent->right && node->parent == grandpa(node)->left) {
        rotateLeft(node->parent);
        node = node->left;
    }
    else if (node == node->parent->left && node->parent == grandpa(node)->right) {
        rotateRight(node->parent);
        node = node->right;
    }
    else {
        fifth_add_case(node);
    }
}

void RBTree::fifth_add_case(Node*& node) {
    node->parent->color = Black;
    grandpa(node)->color = Red;
    if (node == node->parent->left && node->parent == grandpa(node)->left) {
        Node* g = grandpa(node);
        rotateRight(g);
    }
    else {
        Node* g = grandpa(node);
        rotateLeft(g);
    }
}

void RBTree::first_delete_case(Node*& node) {
    if (node->parent != nullptr) {
        second_delete_case(node);
    }
}

void RBTree::second_delete_case(Node*& node) {
    if (!sibling(node)) {
        third_delete_case(node);
        return;
    }
    if (sibling(node)->color == Red) {
        node->parent->color = Red;
        sibling(node)->color = Black;
        if (node->parent->left == node) {
            rotateLeft(node->parent);
        }
        else {
            rotateRight(node->parent);
        }
    }
    third_delete_case(node);
}

void RBTree::third_delete_case(Node*& node) {
    if (!sibling(node)) {
        fourth_delete_case(node);
        return;
    }
    if (node->parent->color == Black && sibling(node)->color == Black && (!sibling(node)->left || sibling(node)->left->color == Black) && (!sibling(node)->right || sibling(node)->right->color == Black)) {
        sibling(node)->color = Red;
        first_delete_case(node->parent);
    }
    else {
        fourth_delete_case(node);
    }
}

void RBTree::fourth_delete_case(Node*& node) {
    if (!sibling(node)) {
        fifth_delete_case(node);
        return;
    }
    if (node->parent->color == Red && sibling(node)->color == Black && (!sibling(node)->left || sibling(node)->left->color == Black) && (!sibling(node)->right || sibling(node)->right->color == Black)) {
        sibling(node)->color = Red;
        node->parent->color = Black;
    }
    else {
        fifth_delete_case(node);
    }
}

void RBTree::fifth_delete_case(Node*& node) {
    if (!sibling(node)) {
        sixth_delete_case(node);
        return;
    }
    if (sibling(node)->color == Black) {
        if (node->parent->left == node && (!sibling(node)->right || sibling(node)->right->color == Black) && (sibling(node)->left && sibling(node)->left->color == Red)) {
            sibling(node)->color = Red;
            sibling(node)->left->color = Black;
            Node* s = sibling(node);
            rotateRight(s);
        }
        else if (node->parent->right == node && (!sibling(node)->left || sibling(node)->left->color == Black) && (sibling(node)->right && sibling(node)->right->color == Red)) {
            sibling(node)->color = Red;
            sibling(node)->right->color = Black;
            Node* s = sibling(node);
            rotateLeft(s);
        }
    }
    sixth_delete_case(node);
}

void RBTree::sixth_delete_case(Node*& node) {
    if (!sibling(node)) {
        return;
    }
    sibling(node)->color = node->parent->color;
    node->parent->color = Black;
    if (node->parent->left == node) {
        sibling(node)->right->color = Black;
        rotateLeft(node->parent);
    }
    else {
        sibling(node)->left->color = Black;
        rotateRight(node->parent);
    }
}

void RBTree::rotateLeft(Node*& node) {
    if (node->right == nullptr) {
        return;
    }
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (node->right != nullptr) {
       node->right->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = rightChild;
    }
    else if (node == node->parent->left) {
        node->parent->left = rightChild;
    }
    else {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
}

void RBTree::rotateRight(Node*& node) {
    if (node->left == nullptr) {
        return;
    }
    Node* leftChild = node->left;
    node->left = leftChild->left;
    if (node->left != nullptr) {
        node->left->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = leftChild;
    }
    else if (node == node->parent->left) {
        node->parent->left = leftChild;
    }
    else {
        node->parent->right = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
}

void RBTree::replaceAndDelete(Node* node) {
    Node* deleteNode;
    if (node->left == nullptr && node->right == nullptr) {
        if (node->color == Black) {
           first_delete_case(node);
        }
        if (node->parent->left == node) {
            node->parent->left = nullptr;
        }
        else {
            node->parent->right = nullptr;
        }
        return;
    }
    if (node->left == nullptr && node->right != nullptr) {
        Node* right = node->right;
        node->color = right->color;
        node->key = right->key;
        node->value = right->value;
        node->left = right->left;
        node->right = right->right;
        if (right->color == Black) {
           first_delete_case(right);
        }
        return;
    }
    if (node->right == nullptr && node->left != nullptr) {
        Node* left = node->left;
        node->color = left->color;
        node->key = left->key;
        node->value = left->value;
        node->left = left->left;
        node->right = left->right;
        if (left->color == Black) {
           first_delete_case(left);
        }
        return;
    }
    if (node->right != nullptr && node->left != nullptr) {
        Node* n = node;
        if (n->left->right == nullptr) {
            n->key = n->left->key;
            n->value = n->left->value;
            n->color = n->left->color;
            Node* buf = n->left->right;
            deleteNode = n->left;
            n->left = buf;
            if (deleteNode->color == Black) {
               first_delete_case(deleteNode);
            }
            return;
        }
        else {
            Node* l = n->left;
            while (l->right != nullptr) {
                l = l->right;
            }
            if (l->color == Black) {
               first_delete_case(l);
            }
            return;
        }
    }
    throw std::invalid_argument("-1");
}


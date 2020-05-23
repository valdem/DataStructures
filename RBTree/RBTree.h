#pragma once
#include <iostream>

using ValueType = int;
using KeyType = int[];

enum Color {
    Red,
    Black
};

struct Node {
    Node(const ValueType& value,const ValueType key);
    ~Node(){};
    
    ValueType value;
    ValueType key;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
};

class RBTree {
    
    
public:
    
    RBTree();
    ~RBTree();
    
    RBTree(RBTree& copyTree);
    RBTree& operator=(RBTree& copyTree);
    
    RBTree(RBTree&& moveTree) noexcept;
    RBTree& operator=(RBTree&& moveTree) noexcept;
    
    void add(const ValueType key, const ValueType& value);
    ValueType find(ValueType key);
    void remove(ValueType key);
    
    Color color(ValueType key);
    Color getColor(Node*& root, ValueType key);
    
    void insertNode(Node*& root, const ValueType key, const ValueType& value);
    
    void deleteOneNode(Node*& root, ValueType key);
    void deleteAllNodes(Node*& root, ValueType key);
    
    ValueType findNode(Node*& root, ValueType key);
    Node* findMaxNode(Node*& root);
    Node* findMinNode(Node*& root);
    
    void first_add_case(Node*& node);
    void second_add_case(Node*& node);
    void third_add_case(Node*& node);
    void fourth_add_case(Node*& node);
    void fifth_add_case(Node*& node);
    
    void first_delete_case(Node*& node);
    void second_delete_case(Node*& node);
    void third_delete_case(Node*& node);
    void fourth_delete_case(Node*& node);
    void fifth_delete_case(Node*& node);
    void sixth_delete_case(Node*& node);
    
    void rotateLeft(Node*& node);
    void rotateRight(Node*& node);
    void replaceAndDelete(Node* node);
    
    Node* grandpa(Node*& node);
    Node* uncle(Node*& node);
    Node* sibling(Node*& node);
    
    size_t size();
    bool isEmpty();
  
private:
    Node* root;
    size_t _size;
    
    void copyNodes(Node*& root);
    void forceNodeDelete(Node*& root);
};

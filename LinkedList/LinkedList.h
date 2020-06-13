#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>

template <class ValueType>
class LinkedList
{
    // класс узла списка
    // по своей сути, может содержать любые данные,
    // можно реализовать и ассоциативный массив, просто добавив
    // поле с ключем в узел и, с учетом этого, поменять методы LinkedList
    // (доступ по ключу, поиск по ключу и т.д.)
    struct Node {
        Node(const ValueType& value, Node* next = nullptr);
        ~Node();

        void insertNext(const ValueType& value);
        void removeNext();

        ValueType value;
        Node* next;
    };

public:
    ////
    LinkedList();
    LinkedList(const LinkedList& copyList);
    LinkedList& operator=(const LinkedList& copyList);

    LinkedList(LinkedList&& moveList) noexcept;
    LinkedList& operator=(LinkedList&& moveList) noexcept;

    ~LinkedList();
    ////

    // доступ к значению элемента по индексу
    const ValueType& operator[](const size_t pos) const;
    ValueType& operator[](const size_t pos);
    // доступ к узлу по индексу
    LinkedList::Node* getNode(const size_t pos) const;
    
    // вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
    void insert(const size_t pos, const ValueType& value);
    // вставка элемента после узла, (O(1))
    void insertAfterNode(Node* node, const ValueType& value);
    // вставка в конец (О(n))
    void pushBack(const ValueType& value);
    // вставка в начало (О(1))
    void pushFront(const ValueType& value);

    // удаление
    void remove(const size_t pos);
    void removeNextNode(Node* node);
    void removeFront();
    void removeBack();
    
    // поиск, О(n)
    long long int findIndex(const ValueType& value) const;
    Node* findNode(const ValueType& value) const;

    // разворот списка
    void reverse();                        // изменение текущего списка
    LinkedList reverse() const;            // полчение нового списка (для константных объектов)
    LinkedList getReverseList() const;    // чтобы неконстантный объект тоже мог возвращать новый развернутый список

    size_t size() const;
    
    void print();
private:
    Node*    _head;
    size_t    _size;

    void forceNodeDelete(Node* node);
};

template <class ValueType>
LinkedList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

template <class ValueType>
LinkedList<ValueType>::Node::~Node()
{
    // ничего не удаляем, т.к. агрегация
}

template <class ValueType>
void LinkedList<ValueType>::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;
}

template <class ValueType>
void LinkedList<ValueType>::Node::removeNext()
{
    if (this->next == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    delete removeNode;
    this->next = newNext;
}

template <class ValueType>
LinkedList<ValueType>::LinkedList()
    : _head(nullptr), _size(0)
{
    
}

template <class ValueType>
LinkedList<ValueType>::LinkedList(const LinkedList& copyList)
{
    this->_size = copyList._size;
    if (this->_size == 0) {
        this->_head = nullptr;
        return;
    }
    else {

        this->_head = new Node(copyList._head->value);

        Node* currentNode = this->_head;
        Node* currentCopyNode = copyList._head;

        while (currentCopyNode->next) {
            currentNode->next = new Node(currentCopyNode->next->value);
            currentCopyNode = currentCopyNode->next;
            currentNode = currentNode->next;
        }
    }
}

template <class ValueType>
LinkedList<ValueType>& LinkedList<ValueType>::operator=(const LinkedList& copyList)
{
    if (this == &copyList) {
        return *this;
    }
    else {
        forceNodeDelete(_head);
        LinkedList bufList(copyList);
        
        this->_size = bufList._size;
        this->_head = bufList._head;
        
        bufList._size = 0;
        bufList._head = nullptr;
        
        return *this;
    }
}

template <class ValueType>
LinkedList<ValueType>::LinkedList(LinkedList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;

    moveList._size = 0;
    moveList._head = nullptr;
}

template <class ValueType>
LinkedList<ValueType>& LinkedList<ValueType>::operator=(LinkedList&& moveList) noexcept
{
    if (this == &moveList) {
        return *this;
    }
    else {
        forceNodeDelete(_head);
        this->_size = moveList._size;
        this->_head = moveList._head;

        moveList._size = 0;
        moveList._head = nullptr;

        return *this;
    }
}

template <class ValueType>
LinkedList<ValueType>::~LinkedList()
{
    forceNodeDelete(_head);
}

template <class ValueType>
ValueType& LinkedList<ValueType>::operator[](const size_t pos)
{
    return getNode(pos)->value;
}

template <class ValueType>
const ValueType& LinkedList<ValueType>::operator[](const size_t pos) const {
    return getNode(pos)->value;
}

template <class ValueType>
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::getNode(const size_t pos) const
{
    if (pos < 0) {
        assert(pos < 0);
    }
    else if (pos >= this->_size) {
        assert(pos >= this->_size);
    }

    Node* bufNode = this->_head;
    for (int i = 0; i < pos; ++i) {
        bufNode = bufNode->next;
    }

    return bufNode;
}

template <class ValueType>
void LinkedList<ValueType>::insert(const size_t pos, const ValueType& value)
{
    if (pos < 0) {
        assert(pos < 0);
    }
    else if (pos > this->_size) {
        assert(pos > this->_size);
    }

    if (pos == 0) {
        pushFront(value);
    }
    else {
        Node* bufNode = this->_head;
        for (size_t i = 0; i < pos-1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(value);
        ++_size;
    }
}

template <class ValueType>
void LinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    _size += 1;
}

template <class ValueType>
void LinkedList<ValueType>::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
    }
    else {
        insert(_size, value);
    }
}

template <class ValueType>
void LinkedList<ValueType>::pushFront(const ValueType& value)
{
    _head = new Node(value, _head);
    ++_size;
}

template <class ValueType>
void LinkedList<ValueType>::remove(const size_t pos)
{
    if (pos>_size-1 || pos<0) {
        throw std::invalid_argument("Element's missing");
    }
    if (pos == 0) {
        removeFront();
    }
    else {
        Node* temp = this->_head;
        for (size_t i = 0; i<pos-1; i++) {
            temp = temp->next;
        }
        Node* remNode = temp->next;
        temp->next = temp->next->next;
        delete remNode;
        _size--;
    }
}

template <class ValueType>
void LinkedList<ValueType>::removeNextNode(Node* node)
{
    if (node->next == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* remNode = node->next;
    Node* curr = remNode->next;
    node->next = curr;
    delete remNode;
    _size--;
}

template <class ValueType>
void LinkedList<ValueType>::removeFront() {
    if (_head == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _head;
    _head = _head->next;
    delete temp;
    _size--;
}

template <class ValueType>
void LinkedList<ValueType>::removeBack() {
    if (_size == 0) {
        throw std::invalid_argument("Element's missing");
    }
    remove(_size-1);
}

template <class ValueType>
long long int LinkedList<ValueType>::findIndex(const ValueType& value) const
{
    Node* findNext = _head->next;
    size_t index = -1;
    size_t i = 1;
    if (_head->value == value) {
        index = 0;
        return index;
    }
    else {
        while (findNext) {
            if (findNext->value == value) {
                index = i;
                return index;
            }
            findNext = findNext->next;
            i++;
        }
    }
    throw std::invalid_argument("Element's missing");
}

template <class ValueType>
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::findNode(const ValueType& value) const
{
    Node* findNode = _head;
    for (size_t i = 0; i < _size; i++) {
        if (findNode->value == value) {
            return findNode;
        }
        findNode = findNode->next;
    }
    throw std::invalid_argument("Element isn't found");
}

template <class ValueType>
void LinkedList<ValueType>::reverse()
{
    Node* current = _head;
    Node* next = nullptr;
    Node* prev = nullptr;
    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    _head = prev;
}

template <class ValueType>
LinkedList<ValueType> LinkedList<ValueType>::reverse() const
{
    LinkedList newList(*this);
    
    newList.reverse();
    return newList;
}

template <class ValueType>
LinkedList<ValueType> LinkedList<ValueType>::getReverseList() const
{
    LinkedList newList(*this);
    
    newList.reverse();
    return newList;
}

template <class ValueType>
size_t LinkedList<ValueType>::size() const
{
    return _size;
}

template <class ValueType>
void LinkedList<ValueType>::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

template <class ValueType>
void LinkedList<ValueType>:: print() {
    Node* temp = _head;
    std::cout<<_head<<std::endl;
    while (temp != nullptr) {
        std::cout<<temp->next<<" "<<temp->value<<std::endl;
        temp = temp->next;
    }
}

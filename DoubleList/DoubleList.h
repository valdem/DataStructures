#pragma once
#include <iostream>
#include <cassert>

template <class ValueType>

class DoubleList
{
    struct Node {
        Node(const ValueType& value, Node* next = nullptr, Node* prev = nullptr);
        ~Node();

        void insertNext(const ValueType& value);
        void removeNext();
        
        void insertPrev(const ValueType& value);
        void removePrev();

        ValueType value;
        Node* next;
        Node* prev;

    };
public:
    ////
    DoubleList();
    DoubleList(const DoubleList& copyList);
    DoubleList& operator=(const DoubleList& copyList);

    DoubleList(DoubleList&& moveList) noexcept;
    DoubleList& operator=(DoubleList&& moveList) noexcept;
    
    ~DoubleList();
    ////
    // доступ к значению элемента по индексу
    ValueType& operator[](const size_t pos) const;
    // доступ к узлу по индексу
    DoubleList::Node* getNode(const size_t pos) const;
        
    // вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
    void insert(const size_t pos, const ValueType& value);
    // вставка элемента после узла, (O(1))
    void insertAfterNode(Node* node, const ValueType& value);
    //вставка элемента перед узлом
    void insertBeforeNode(Node* node, const ValueType& value);
    // вставка в конец (О(n))
    void pushBack(const ValueType& value);
    // вставка в начало (О(1))
    void pushFront(const ValueType& value);

    // удаление
    void remove(const size_t pos);
    void removeNextNode(Node* node);
    void removePrevNode(Node* node);
    void removeFront();
    void removeBack();
        
    // поиск, О(n)
    long long int findIndex(const ValueType& value) const;
    Node* findNode(const ValueType& value) const;

    // разворот списка
    void reverse();                        // изменение текущего списка
    DoubleList reverse() const;            // полчение нового списка (для константных объектов)
    DoubleList getReverseList() const;    // чтобы неконстантный объект тоже мог возвращать новый развернутый список

    size_t size() const;
        
    void print();
private:
    Node*    _head;
    Node*    _tail;
    size_t   _size;
    
    void forceNodeDelete(Node* node);
};

template <class ValueType>
DoubleList<ValueType>::Node::Node(const ValueType& value, Node* next, Node* prev){
    this->value = value;
    this->next = next;
    this->prev = prev;
}

template <class ValueType>
DoubleList<ValueType>::Node::~Node()
{
    
}

template <class ValueType>
void DoubleList<ValueType>::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next, this);
    this->next = newNode;
}

template <class ValueType>
void DoubleList<ValueType>::Node::removeNext()
{
    if (this->next == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    this->next = newNext;
    this->next->prev = this;
    delete removeNode;
}

template <class ValueType>
void DoubleList<ValueType>::Node::insertPrev(const ValueType& value)
{
    Node* newNode = new Node(value, this, this->prev);
    this->prev = newNode;
}

template <class ValueType>
void DoubleList<ValueType>::Node::removePrev()
{
    if (this->prev == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* removeNode = this->prev;
    Node* newPrev = removeNode->prev;
    this->prev = newPrev;
    newPrev->next = this;
    delete removeNode;
}

template <class ValueType>
DoubleList<ValueType>::DoubleList()
    : _head(nullptr), _tail(nullptr), _size(0)
{
    
}

template <class ValueType>
DoubleList<ValueType>::DoubleList(const DoubleList& copyList)
{
    _size = copyList._size;
    if (_size == 0) {
        this->_head = nullptr;
        this->_tail = nullptr;
        return;
    }
    else {
        this->_head = new Node(copyList._head->value);

        Node* currentNode = this->_head;
        Node* currentCopyNode = copyList._head;

        while (currentCopyNode->next) {
            currentNode->next = new Node(currentCopyNode->next->value);
            currentNode->next->prev = currentNode;
            currentCopyNode = currentCopyNode->next;
            currentNode = currentNode->next;
        }
        _tail = currentNode;
    }
}

template <class ValueType>
DoubleList<ValueType>& DoubleList<ValueType>::operator=(const DoubleList& copyList)
{
    if (this == &copyList) {
        return *this;
    }
    else {
        forceNodeDelete(_head);
        DoubleList bufList(copyList);
        
        this->_size = bufList._size;
        this->_head = bufList._head;
        this->_tail = bufList._tail;

        bufList._size = 0;
        bufList._head = nullptr;
        bufList._tail = nullptr;
        
        return *this;
    }
}

template <class ValueType>
DoubleList<ValueType>::DoubleList(DoubleList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;
    this->_tail = moveList._tail;

    moveList._size = 0;
    moveList._head = nullptr;
    moveList._tail = nullptr;
}

template <class ValueType>
DoubleList<ValueType>& DoubleList<ValueType>::operator=(DoubleList&& moveList) noexcept
{
    if (this == &moveList) {
        return *this;
    }
    else {
        forceNodeDelete(_head);
        this->_size = moveList._size;
        this->_head = moveList._head;
        this->_tail = moveList._tail;

        moveList._size = 0;
        moveList._head = nullptr;
        moveList._tail = nullptr;

        return *this;
    }
}

template <class ValueType>
DoubleList<ValueType>::~DoubleList()
{
    forceNodeDelete(_head);
}

template <class ValueType>
ValueType& DoubleList<ValueType>::operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

template <class ValueType>
typename DoubleList<ValueType>::Node* DoubleList<ValueType>::getNode(const size_t pos) const
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
void DoubleList<ValueType>::insert(const size_t pos, const ValueType& value)
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
    else if (pos == _size) {
        pushBack(value);
    }
    else {
        Node* bufNode = getNode(pos-1);
        bufNode->insertNext(value);
        ++_size;
    }
}

template <class ValueType>
void DoubleList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    _size++;
}

template <class ValueType>
void DoubleList<ValueType>::insertBeforeNode(Node* node, const ValueType& value) {
    node->insertPrev(value);
    _size++;
}

template <class ValueType>
void DoubleList<ValueType>::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
    }
    else if (_size == 1) {
        Node* newNode = new Node(value, nullptr, _head);
        _head->next = newNode;
        _tail = newNode;
        _size++;
    }
    else {
        _tail->next = new Node(value, nullptr, _tail);
        _tail = _tail->next;
        _size++;
    }
}

template <class ValueType>
void DoubleList<ValueType>::pushFront(const ValueType& value)
{
    if (_size == 1) {
        Node* newNode = new Node(value, nullptr, _head);
        _head->next = newNode;
        _tail = newNode;
        _size++;
    }
    else {
        Node* bufNode = _head;
        _head = new Node(value);
        if (bufNode != nullptr) {
            _head->next = bufNode;
            _head->next->prev = _head;
        }
    }
    _size++;
}

template <class ValueType>
void DoubleList<ValueType>::remove(const size_t pos)
{
    if (pos<0 || pos >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    if (pos == 0) {
        removeFront();
    }
    else if (pos == _size-1) {
        removeBack();
    }
    else {
        Node* bufNode = getNode(pos-1);
        bufNode->removeNext();
        _size--;
    }
}

template <class ValueType>
void DoubleList<ValueType>::removeNextNode(Node* node)
{
    node->removeNext();
    _size--;
}

template <class ValueType>
void DoubleList<ValueType>::removePrevNode(Node* node) {
    node->removePrev();
    _size--;
}

template <class ValueType>
void DoubleList<ValueType>::removeFront() {
    if (_head == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _head->next;
    delete _head;
    _head = temp;
    _size--;
}

template <class ValueType>
void DoubleList<ValueType>::removeBack() {
    if (_tail == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _tail->prev;
    delete _tail;
    _tail = temp;
    _size--;
}

template <class ValueType>
long long int DoubleList<ValueType>::findIndex(const ValueType& value) const
{
    Node* findNext = _head->next;
    size_t index = -1;
    size_t i = 1;
    if (_head->value == value) {
        index = 0;
    }
    else {
        while (findNext) {
            if (findNext->value == value) {
                index = i;
            }
            findNext = findNext->next;
            i++;
        }
    }
    return index;
}

template <class ValueType>
typename DoubleList<ValueType>::Node* DoubleList<ValueType>::findNode(const ValueType& value) const
{
    Node* findNode = _head;
    for (size_t i = 0; i<_size; i++) {
        if (findNode->value == value) {
            return findNode;
        }
        findNode = findNode->next;
    }
    throw std::invalid_argument("Element's missing");
}

template <class ValueType>
void DoubleList<ValueType>::reverse()
{
    Node* temp = _tail;
    _tail->next = _tail->prev;
    _tail->prev = nullptr;
    _tail = _head;
    _head = temp;
    while (temp->next) {
        temp->next->next = temp->next->prev;
        temp->next->prev = temp;
        temp = temp->next;
    }
}

template <class ValueType>
DoubleList<ValueType> DoubleList<ValueType>:: reverse() const {
    DoubleList newList(*this);
    newList.reverse();

    return newList;
}

template <class ValueType>
DoubleList<ValueType> DoubleList<ValueType>:: getReverseList() const {
    DoubleList newList(*this);
    newList.reverse();

    return newList;
}

template <class ValueType>
size_t DoubleList<ValueType>::size() const
{
    return _size;
}

template <class ValueType>
void DoubleList<ValueType>::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

template <class ValueType>
void DoubleList<ValueType>:: print() {
    for (int i = 0; i<_size; i++) {
        std::cout<<getNode(i)->value<<std::endl;
    }
}

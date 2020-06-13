#include "DoubleList.h"
#include <cassert>

DoubleList::Node::Node(const ValueType& value, Node* next, Node* prev){
    this->value = value;
    this->next = next;
    this->prev = prev;
}

DoubleList::Node::~Node()
{
    
}

void DoubleList::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next, this);
    this->next = newNode;
}

void DoubleList::Node::removeNext()
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

void DoubleList::Node::insertPrev(const ValueType& value)
{
    Node* newNode = new Node(value, this, this->prev);
    this->prev = newNode;
}

void DoubleList::Node::removePrev()
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

DoubleList::DoubleList() 
    : _head(nullptr), _tail(nullptr), _size(0)
{
    
}

DoubleList::DoubleList(const DoubleList& copyList)
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

DoubleList& DoubleList::operator=(const DoubleList& copyList)
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

DoubleList::DoubleList(DoubleList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;
    this->_tail = moveList._tail;

    moveList._size = 0;
    moveList._head = nullptr;
    moveList._tail = nullptr;
}

DoubleList& DoubleList::operator=(DoubleList&& moveList) noexcept
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

DoubleList::~DoubleList()
{
    forceNodeDelete(_head);
}

ValueType& DoubleList::operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

DoubleList::Node* DoubleList::getNode(const size_t pos) const
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

void DoubleList::insert(const size_t pos, const ValueType& value)
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

void DoubleList::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    _size++;
}

void DoubleList::insertBeforeNode(Node* node, const ValueType& value) {
    node->insertPrev(value);
    _size++;
}

void DoubleList::pushBack(const ValueType& value)
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

void DoubleList::pushFront(const ValueType& value)
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

void DoubleList::remove(const size_t pos)
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

void DoubleList::removeNextNode(Node* node)
{
    node->removeNext();
    _size--;
}

void DoubleList::removePrevNode(Node* node) {
    node->removePrev();
    _size--;
}

void DoubleList::removeFront() {
    if (_head == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _head->next;
    delete _head;
    _head = temp;
    _size--;
}

void DoubleList::removeBack() {
    if (_tail == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _tail->prev;
    delete _tail;
    _tail = temp;
    _size--;
}

long long int DoubleList::findIndex(const ValueType& value) const
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

DoubleList::Node* DoubleList::findNode(const ValueType& value) const
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

void DoubleList::reverse()
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

DoubleList DoubleList:: reverse() const {
    DoubleList newList(*this);
    newList.reverse();

    return newList;
}
DoubleList DoubleList:: getReverseList() const {
    DoubleList newList(*this);
    newList.reverse();

    return newList;
}

size_t DoubleList::size() const
{
    return _size;
}

void DoubleList::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

void DoubleList:: print() {
    for (int i = 0; i<_size; i++) {
        std::cout<<getNode(i)->value<<std::endl;
    }
}

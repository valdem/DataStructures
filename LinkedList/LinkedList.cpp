#include "LinkedList.h"

#include <cassert>

LinkedList::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

LinkedList::Node::~Node()
{
    // ничего не удаляем, т.к. агрегация
}

void LinkedList::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;
}

void LinkedList::Node::removeNext()
{
    if (this->next == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    delete removeNode;
    this->next = newNext;
}

LinkedList::LinkedList()
    : _head(nullptr), _size(0)
{
    
}

LinkedList::LinkedList(const LinkedList& copyList)
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

LinkedList& LinkedList::operator=(const LinkedList& copyList)
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

LinkedList::LinkedList(LinkedList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;

    moveList._size = 0;
    moveList._head = nullptr;
}

LinkedList& LinkedList::operator=(LinkedList&& moveList) noexcept
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

LinkedList::~LinkedList()
{
    forceNodeDelete(_head);
}

ValueType& LinkedList::operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

LinkedList::Node* LinkedList::getNode(const size_t pos) const
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

void LinkedList::insert(const size_t pos, const ValueType& value)
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

void LinkedList::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    _size += 1;
}

void LinkedList::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
    }
    else {
        insert(_size, value);
    }
}

void LinkedList::pushFront(const ValueType& value)
{
    _head = new Node(value, _head);
    ++_size;
}

void LinkedList::remove(const size_t pos)
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

void LinkedList::removeNextNode(Node* node)
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

void LinkedList::removeFront() {
    if (_head == nullptr) {
        throw std::invalid_argument("Element's missing");
    }
    Node* temp = _head;
    _head = _head->next;
    delete temp;
    _size--;
}

void LinkedList::removeBack() {
    if (_size == 0) {
        throw std::invalid_argument("Element's missing");
    }
    remove(_size-1);
}

long long int LinkedList::findIndex(const ValueType& value) const
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

LinkedList::Node* LinkedList::findNode(const ValueType& value) const
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

void LinkedList::reverse()
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

LinkedList LinkedList::reverse() const
{
    LinkedList newList(*this);
    
    newList.reverse();
    return newList;
}

LinkedList LinkedList::getReverseList() const
{
    LinkedList newList(*this);
    
    newList.reverse();
    return newList;
}

size_t LinkedList::size() const
{
    return _size;
}

void LinkedList::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

void LinkedList:: print() {
    Node* temp = _head;
    std::cout<<_head<<std::endl;
    while (temp != nullptr) {
        std::cout<<temp->next<<" "<<temp->value<<std::endl;
        temp = temp->next;
    }
}

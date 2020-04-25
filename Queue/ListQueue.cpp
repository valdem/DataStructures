#include "ListQueue.h"

void ListQueue:: enqueue(const ValueType& value) {
    LinkedList::pushBack(value);
}
void ListQueue:: dequeue() {
    LinkedList::removeFront();
}
const ValueType& ListQueue:: front() const {
    return LinkedList::getNode(0)->value;
}
bool ListQueue:: isEmpty() const {
    return !LinkedList::size();
}
size_t ListQueue:: size() const {
    return LinkedList::size();
}

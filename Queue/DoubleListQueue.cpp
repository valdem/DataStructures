#include "DoubleListQueue.h"

void DoubleListQueue:: enqueue(const ValueType& value) {
    DoubleList::pushBack(value);
}
void DoubleListQueue:: dequeue() {
    DoubleList::removeFront();
}
const ValueType& DoubleListQueue:: front() const {
    return DoubleList::getNode(0)->value;
}
bool DoubleListQueue:: isEmpty() const {
    if (DoubleList::getNode(0)) {
        if (DoubleList::getNode(0)->value) {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}
size_t DoubleListQueue:: size() const {
    return DoubleList::size();
}

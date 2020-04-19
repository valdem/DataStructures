#include "VectorQueue.h"

void VectorQueue:: enqueue(const ValueType& value) {
    MyVector::pushBack(value);
}
void VectorQueue:: dequeue() {
    if (index != size()) {
        index++;
    }
    else {
        MyVector::~MyVector();
    }
}
const ValueType& VectorQueue:: front() const {
    return MyVector:: operator[](index);
}
bool VectorQueue:: isEmpty() const {
    if (MyVector:: operator[](0)) {
        return false;
    }
    else {
        return true;
    }
}
size_t VectorQueue:: size() const {
    return MyVector::size();
}



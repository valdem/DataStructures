#include "VectorQueue.h"

void VectorQueue:: enqueue(const ValueType& value) {
    MyVector::pushBack(value);
}
void VectorQueue:: dequeue() {
    if (index != size()) {
        index++;
    }
    else {
        throw std::length_error("Неправильная операция");
    }
}
const ValueType& VectorQueue:: front() const {
    if (index == size()) {
        throw std::length_error("Неправильная операция");
    }
    else {
        return MyVector:: operator[](index);
    }
}
bool VectorQueue:: isEmpty() const {
    return !MyVector::size();
}
size_t VectorQueue:: size() const {
    return MyVector::size();
}



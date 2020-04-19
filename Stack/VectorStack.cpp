#include "VectorStack.h"

// добавление в хвост
void VectorStack:: push(const ValueType& value) {
    MyVector::pushBack(value);
}
// удаление с хвоста
void VectorStack:: pop() {
    MyVector::popBack();
}
// посмотреть элемент в хвосте
const ValueType& VectorStack:: top() const {
    return MyVector:: operator[](size()-1);
}
// проверка на пустоту
bool VectorStack:: isEmpty() const {
    if (MyVector:: operator[](0)) {
        return false;
    }
    else {
        return true;
    }
}
// размер
size_t VectorStack:: size() const {
   return MyVector:: size();
}

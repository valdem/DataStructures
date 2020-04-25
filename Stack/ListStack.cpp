#include "ListStack.h"



// добавление в хвост
void ListStack:: push(const ValueType& value) {
    LinkedList::pushFront(value);
}
// удаление с хвоста
void ListStack:: pop() {
    LinkedList::removeFront();
}
// посмотреть элемент в хвосте
const ValueType& ListStack:: top() const {
    return LinkedList::getNode(0)->value;
}
// проверка на пустоту
bool ListStack:: isEmpty() const {
    return !LinkedList::size();
}
// размер
size_t ListStack:: size() const {
    return LinkedList::size();
}

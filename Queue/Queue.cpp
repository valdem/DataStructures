#include "Queue.h"
#include "ListQueue.h"
#include "VectorQueue.h"
#include "DoubleListQueue.h"
#include "QueueImplementation.h"

#include <stdexcept>

Queue::Queue(QueueContainer container)
    : _containerType(container)
{
    switch (container)
    {
    case QueueContainer::List: {
        _pimpl = new ListQueue();
        break;
    }
    case QueueContainer::Vector: {
        _pimpl = new VectorQueue();
        break;
    }
    case QueueContainer::DoubleList: {
        _pimpl = new DoubleListQueue();
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
    
}

Queue::Queue(const ValueType* valueArray, const size_t arraySize,
             QueueContainer container)
    :_containerType(container)
{
    switch (container)
    {
    case QueueContainer::List: {
        _pimpl = new ListQueue();
        break;
    }
    case QueueContainer::Vector: {
        _pimpl = new VectorQueue();
        break;
    }
    case QueueContainer::DoubleList: {
        _pimpl = new DoubleListQueue();
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
    for (size_t i = 0; i<arraySize; i++) {
        _pimpl->enqueue(valueArray[i]);
    }
    
}

Queue::Queue(const Queue& copyQueue)
    :Queue(copyQueue._containerType)
{
    switch (_containerType)
    {
    case QueueContainer::List: {
        _pimpl = new ListQueue(*(static_cast<ListQueue*>(copyQueue._pimpl)));
        break;
    }
    case QueueContainer::Vector: {
        _pimpl = new VectorQueue(*(static_cast<VectorQueue*>(copyQueue._pimpl)));
        break;
    }
    case QueueContainer::DoubleList: {
        _pimpl = new DoubleListQueue(*(static_cast<DoubleListQueue*>(copyQueue._pimpl)));
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
}
Queue& Queue:: operator=(const Queue& copyQueue) {
    if (this == &copyQueue) {
        return *this;
    }
    delete _pimpl;
    
    switch (_containerType)
    {
    case QueueContainer::List: {
        _pimpl = new ListQueue(*(static_cast<ListQueue*>(copyQueue._pimpl)));
        break;
    }
    case QueueContainer::Vector: {
        _pimpl = new VectorQueue(*(static_cast<VectorQueue*>(copyQueue._pimpl)));
        break;
    }
    case QueueContainer::DoubleList: {
        _pimpl = new DoubleListQueue(*(static_cast<DoubleListQueue*>(copyQueue._pimpl)));
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
    
    return *this;
}

Queue::Queue(Queue&& moveQueue) noexcept {
    this->_pimpl = moveQueue._pimpl;
    this->_containerType = moveQueue._containerType;
    
    moveQueue._pimpl = 0;
}
Queue& Queue:: operator=(Queue&& moveQueue) noexcept {
    if (this == &moveQueue) {
        return *this;
    }
    delete _pimpl;
    this->_pimpl = moveQueue._pimpl;
    this->_containerType = moveQueue._containerType;
    
    moveQueue._pimpl = 0;
    
    return *this;
}

Queue:: ~Queue() {
    delete _pimpl;
}

void Queue:: enqueue(const ValueType& value) {
    _pimpl->enqueue(value);
}
void Queue:: dequeue() {
    _pimpl->dequeue();
}
const ValueType& Queue:: front() const {
    return _pimpl->front();
}
bool Queue:: isEmpty() const {
    return _pimpl->isEmpty();
}
size_t Queue:: size() const {
    return _pimpl->size();
}

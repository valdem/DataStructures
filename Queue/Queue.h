#pragma once
#include <iostream>

using ValueType = double;

enum class QueueContainer {
    Vector = 0,
    List,
    DoubleList,
};

class QueueImplementation;

class Queue
{
public:
    // Большая пятерка
    Queue(QueueContainer container = QueueContainer::Vector);
    // элементы массива последовательно подкладываются в стек
    Queue(const ValueType* valueArray, const size_t arraySize,
          QueueContainer container = QueueContainer::Vector);
    
    explicit Queue(const Queue& copyQueue);
    Queue& operator=(const Queue& copyQueue);

    Queue(Queue&& moveQueue) noexcept;
    Queue& operator=(Queue&& moveQueue) noexcept;

    ~Queue();

    void enqueue(const ValueType& value);
    void dequeue();
    const ValueType& front() const;
    bool isEmpty() const;
    size_t size() const;
    
private:
    QueueImplementation* _pimpl = nullptr;
    
    QueueContainer _containerType;
};

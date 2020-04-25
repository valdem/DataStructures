#pragma once

#include "QueueImplementation.h"
#include "DoubleList.h"

class DoubleListQueue : public QueueImplementation, public DoubleList
{
public:
    DoubleListQueue(){};
    ~DoubleListQueue(){};
    
    void enqueue(const ValueType& value);
    void dequeue();
    const ValueType& front() const;
    bool isEmpty() const;
    size_t size() const;
    
};

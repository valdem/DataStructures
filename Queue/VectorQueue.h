#pragma once

#include "QueueImplementation.h"
#include "Vector/MyVector.h"

class VectorQueue : public QueueImplementation, public MyVector
{
public:
    VectorQueue(){};
    ~VectorQueue(){};
    
    void enqueue(const ValueType& value);
    void dequeue();
    const ValueType& front() const;
    bool isEmpty() const;
    size_t size() const;
    
private:
    int index = 0;
};


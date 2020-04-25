#include "MyVector.h"

MyVector::MyVector(size_t size, ResizeStrategy strategy, float coef) {
    switch (strategy) {
        case ResizeStrategy::Additive:
            _capacity = size+coef;
            _data = new ValueType[_capacity];
            _size = size;
        break;
        case ResizeStrategy::Multiplicative:
            if (size == 0) {
                _capacity = 1;
            }
            else {
                _capacity = size*coef;
            }
            _data = new ValueType[_capacity];
            _size = size;
        break;
    }
}


MyVector::MyVector(size_t size, ValueType value, ResizeStrategy strategy, float coef) {
    switch (strategy) {
        case ResizeStrategy::Additive:
            _capacity = size+coef;
            _data = new ValueType[_capacity];
            _size = size;
        break;
        case ResizeStrategy::Multiplicative:
            if (size == 0) {
                _capacity = 1;
            }
            else {
                _capacity = size*coef;
            }
            _data = new ValueType[_capacity];
            _size = size;
        break;
    }
    for (size_t i = 0; i<_size; i++) {
        _data[i] = value;
    }
}

MyVector::MyVector(const MyVector& copy) {
    for (size_t i = 0; i<_size; i++) {
        _data[i] = copy[i];
    }
    this->_data = copy._data;
    this->_size = copy._size;
    this->_capacity = copy._capacity;
}

MyVector& MyVector::operator=(const MyVector& copy) {
    if (this == &copy) {
        return *this;
    }
    _size = copy._size;
    _capacity = copy._capacity;
    
    delete [] _data;
    
    _data = new ValueType[copy._capacity];
    for (size_t i = 0; i<_size; i++) {
        _data[i] = copy._data[i];
    }
    return *this;
}

MyVector::~MyVector() {
    _capacity = 0;
    _size = 0;
}

size_t MyVector:: capacity() const {
    return _capacity;
}

size_t MyVector:: size() const {
    return _size;
}

float MyVector:: loadFactor() {
    return (float)_size/_capacity;
}

MyVector::iterator MyVector::begin() const {
    return MyVector::iterator{this, 0};
}

MyVector::iterator MyVector::end() const{
    return MyVector::iterator{this, static_cast<int>(_size)};
}

ValueType& MyVector:: operator[](const size_t i) const {
    if (i >= _size) {
        std::cout<<"error"<<std::endl;
    }
    return _data[i];
}

void MyVector:: pushBack(const ValueType& value) {
    if (_size+1 >= _capacity) {
        MyVector puVector(_capacity+1);
        for (size_t i = 0; i<_size; i++) {
            puVector._data[i] = _data[i];
        }
        puVector._data[_size] = value;
        _data = puVector._data;
        _size += 1;
        _capacity = puVector._capacity;
    }
    else {
        _data[_size] = value;
        _size += 1;
    }
}

void MyVector:: insert(const size_t i, const ValueType& value) {
    if (i == _size) {
        pushBack(value);
        return;
    }
    if (_size+1 >= _capacity) {
        MyVector inVector(_capacity+1);
        for (size_t j = 0; j<i; j++) {
            inVector._data[j] = _data[j];
        }
        inVector._data[i] = value;
        for (size_t j = i+1; j<inVector._size; j++) {
            inVector._data[j] = _data[j-1];
        }
        _data = inVector._data;
        _size += 1;
        _capacity = inVector._capacity;
    }
    else {
        _size += 1;
        for (size_t j = i; j<_size; j++) {
            _data[j+1] = _data[j];
        }
        _data[i] = value;
    }
}

void MyVector:: insert(const size_t i, const MyVector& value) {
    if (i == _size) {
        pushBack(value._data[i]);
        return;
    }
    if (_size+value._size >= _capacity) {
        MyVector inVector(_size+value._size);
        for (auto idx = 0; idx<i; ++idx) {
            inVector[idx] = _data[idx];
        }
        size_t j = 0;
        for(auto idx = i; idx<i + value.size(); ++idx) {
            inVector[idx] = value[j];
            j++;
        }
        for (auto idx = i + value.size(); idx<_size+value.size(); ++idx) {
            size_t k = i;
            inVector[idx] = _data[k];
            k++;
        }
        _data = inVector._data;
        _size += value._size;
        _capacity = inVector._capacity;
    }
    else {
        for (size_t idx = _size; idx>i; idx--) {
            _data[i] = _data[i-value._size];
        }
        for (size_t j = i; j<i+value._size; j++) {
            _data[j] = value._data[value._size-i];
        }
        _size += value._size;
    }
}


void MyVector:: popBack() {
    if (loadFactor()==1/4) {
        MyVector popVector(_capacity-1);
        for (size_t i = 0; i<_size-1; i++) {
            popVector._data[i] = _data[i];
        }
        _data = popVector._data;
        _size -= 1;
        _capacity = popVector._capacity;
    }
    else {
        _size -= 1;
    }
}

void MyVector:: erase(const size_t i) {
    if (i == _size-1) {
        popBack();
        return;
    }
    if (loadFactor() == 1/4) {
        MyVector erVector(_capacity-1);
        for (size_t j = 0; j<i; j++) {
            erVector._data[j] = _data[j];
        }
        for (size_t j = i; j<_size-1; j++) {
            erVector._data[j] = _data[j+1];
        }
        _data = erVector._data;
        _size -= 1;
        _capacity = erVector._capacity;
    }
    else {
        for (size_t j = i; j<_size-1; j++) {
            _data[j] = _data[j+1];
        }
        _size -= 1;
    }
}

void MyVector:: erase(const size_t i, const size_t len) {
    if (loadFactor() == 1/4) {
        MyVector erVector2(_capacity-len);
        for (size_t j = 0; j<i; j++) {
            erVector2._data[j] = _data[j];
        }
        for (size_t j = i; j<_size-len; j++) {
            erVector2._data[j] = _data[j+len];
        }
        _data = erVector2._data;
        _size -= len;
        _capacity = erVector2._capacity;
    }
    else {
        for (size_t j = i; j<_size-len; j++) {
            _data[j] = _data[j+len];
        }
        _size -= len;
    }
}

long long int MyVector:: find(const ValueType& value, bool isBegin) const {
    if (isBegin == true) {
        size_t i = 0;
        while (_data[i] != value ) {
            i++;
        }
        return i+1;
    }
    else {
        size_t i = _size-1;
        while (_data[i] != value ) {
            i--;
        }
        return i-1;
    }
    return -1;
}

void MyVector:: reserve(const size_t capacity) {
    if (capacity<_capacity) {
        resize(capacity);
        return;
    }
    MyVector bufVector(*this);
    delete[] _data;
    _capacity = capacity;
    _data = new ValueType[_capacity];
    for (size_t i = 0; i<_size; i++) {
        _data[i] = bufVector._data[i];
    }
}


void MyVector:: resize(const size_t size, const ValueType value) {
    if (_size == size) {
        return;
    }
    
    MyVector bufVector(*this);
    _size = size;
    MyVector resVector(_size);
    
    if (_size > bufVector._size) {
        for (size_t i = 0; i<bufVector._size; i++) {
            resVector._data[i] = _data[i];
        }
        for (size_t i = bufVector._size; i<_size; i++) {
            resVector._data[i] = value;
        }
    }
    else {
        for (size_t i = 0; i<_size; i++) {
            resVector._data[i] = _data[i];
        }
    }
    delete[] _data;
    _data = resVector._data;
    _capacity = resVector._capacity;
}

void MyVector:: clear() {
    for (size_t i = 0; i<_size; i++) {
        _data[i] = 0;
    }
    _size = 0;
}

void MyVector:: print() {
    for (size_t i = 0; i<_size; i++) {
        std::cout<<_data[i]<<std::endl;
    }
}

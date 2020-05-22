#include "MyVector.h"

MyVector::MyVector(size_t size, ResizeStrategy strategy, float coef) {
    switch (strategy) {
        case ResizeStrategy::Additive:
            _capacity = size+coef;
            _data = new ValueType[_capacity];
            _size = size;
            _coef = coef;
            _strategy = strategy;
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
            _coef = coef;
            _strategy = strategy;
        break;
    }
}


MyVector::MyVector(size_t size, ValueType value, ResizeStrategy strategy, float coef) {
    switch (strategy) {
        case ResizeStrategy::Additive:
            _capacity = size+coef;
            _data = new ValueType[_capacity];
            _size = size;
            _coef = coef;
            _strategy = strategy;
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
            _coef = coef;
            _strategy = strategy;
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
    this->_coef = copy._coef;
    this->_strategy = copy._strategy;
}

MyVector& MyVector::operator=(const MyVector& copy) {
    if (this == &copy) {
        return *this;
    }
    _size = copy._size;
    _capacity = copy._capacity;
    _coef = copy._coef;
    _strategy = copy._strategy;

    delete [] _data;
    
    _data = new ValueType[copy._capacity];
    for (size_t i = 0; i<_size; i++) {
        _data[i] = copy._data[i];
    }
    return *this;
}

MyVector::~MyVector() {
    delete[] _data;
    _capacity = 0;
    _size = 0;
    _coef = 0;
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

ValueType& MyVector:: operator[](const size_t i) {
    if (i >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    return _data[i];
}

ValueType& MyVector:: operator[](const size_t i) const {
    if (i >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    return this->_data[i];
}

void MyVector:: pushBack(const ValueType& value) {
    if (_size+1 >= _capacity) {
        changeCapacityInsert();
    }
    _data[_size] = value;
    _size++;
}

void MyVector:: insert(const size_t i, const ValueType& value) {
    if (i<0 || i<_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (i == _size) {
        pushBack(value);
        return;
    }
    if (_size+1 >= _capacity) {
        changeCapacityInsert();
    }
    _size++;
    for (size_t j = i; j<_size; j++) {
        _data[j+1] = _data[j];
    }
    _data[i] = value;
}

void MyVector:: insert(const size_t i, const MyVector& value) {
    if (i<0 || i<_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (_size+value._size >= _capacity) {
        changeCapacityInsert();
    }
    for (size_t j = i; j<_size; j++) {
        _data[j+value._size] = _data[j];
    }
    size_t k = 0;
    for (size_t j = i; j<i+value._size; j++) {
        _data[j] = value._data[k];
        k++;
    }
    _size += value._size;
}


void MyVector:: popBack() {
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove();
    }
    _size--;
}

void MyVector:: erase(const size_t i) {
    if (i<0 || i<_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (i == _size-1) {
        popBack();
        return;
    }
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove();
    }
    for (size_t j = i; j<_size-1; j++) {
        _data[j] = _data[j+1];
    }
    _size--;
}

void MyVector:: erase(const size_t i, const size_t len) {
    if (i<0 || i<_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove();
    }
    for (size_t j = i; j<_size-len; j++) {
        _data[j] = _data[j+len];
    }
    _size -= len;
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
    if (capacity == _capacity) {
        return;
    }
    if (capacity<_size) {
        if (capacity-1 < 0) {
            throw std::invalid_argument ("Too small capacity");
        }
        _capacity = capacity;
        resize(capacity-1);
        return;
    }
    _capacity = capacity;
}


void MyVector:: resize(const size_t size, const ValueType value) {
    if (size < 0) {
        throw std::invalid_argument("Too small size");
    }
    if (_size == size) {
        return;
    }
    if (size>_capacity) {
        changeCapacityInsert();
    }
    if (size>_size) {
        for (size_t i = _size; i<size; i++) {
            _data[i] = value;
        }
        _size = size;
    }
    else {
        erase(size, _size-size);
    }
    
}

void MyVector:: clear() {
    if (_size == 0) {
        return;
    }
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

void MyVector:: changeCapacityInsert() {
    if (_strategy == ResizeStrategy::Additive) {
        _capacity += _coef;
    }
    else {
        _capacity *= _coef;
    }
}

void MyVector:: changeCapacityRemove() {
    if (_strategy == ResizeStrategy::Additive) {
        _capacity += 1/_coef;
    }
    else {
        _capacity *= 1/_coef;
    }
}

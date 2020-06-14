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
    for (size_t i = 0; i<_size; i++) {
        _data[i] = ValueType();
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
    _size = copy._size;
    _capacity = copy._capacity;
    _coef = copy._coef;
    _strategy = copy._strategy;
    _data = new ValueType[_capacity];
    for (size_t i = 0; i<copy._capacity; i++) {
        _data[i] = copy._data[i];
    }
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
    for (size_t i = 0; i<copy._size; i++) {
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

const ValueType& MyVector:: operator[](const size_t i) const {
    if (i >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    return _data[i];
}

void MyVector:: pushBack(const ValueType& value) {
    _size++;
    if (loadFactor()>=1) {
        changeCapacityInsert(_size-1);
    }
    _data[_size-1] = value;
}

void MyVector:: insert(const size_t i, const ValueType& value) {
    if (i<0 || i>_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (i == _size) {
        pushBack(value);
        return;
    }
    _size++;
    if (loadFactor()>=1) {
        changeCapacityInsert(_size-1);
    }
    for (size_t j = _size-1; j>=i; j--) {
        _data[j+1] = _data[j];
    }
    _data[i] = value;
}

void MyVector:: insert(const size_t i, const MyVector& value) {
    if (i<0 || i>_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    _size += value._size;
    while (loadFactor()>=1) {
        changeCapacityInsert(_size-value._size);
    }
    for (size_t j = _size-1; j>=i; j--) {
        _data[j+value._size] = _data[j];
    }
    size_t k = 0;
    for (size_t j = i; j<i+value._size; j++) {
        _data[j] = value._data[k];
        k++;
    }
}


void MyVector:: popBack() {
    if (_size == 0) {
        return;
    }
    _size--;
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove(_size);
    }
    _data[_size] = 0;
}

void MyVector:: erase(const size_t i) {
    if (i<0 || i>_size) {
        throw std::invalid_argument ("Incorrect position");
    }
    if (i == _size-1) {
        popBack();
        return;
    }
    _size--;
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove(_size);
    }
    for (size_t j = i; j<_size-1; j++) {
        _data[j] = _data[j+1];
    }
}

void MyVector:: erase(const size_t i, const size_t len) {
    if (i<0 || i>_size ) {
        throw std::invalid_argument ("Incorrect position1");
    }
    for (size_t j = i; j<_size-len; j++) {
        _data[j] = _data[j+len];
    }
    _size -= len;
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove(_size);
    }
}

long long int MyVector:: find(const ValueType& value, bool isBegin) const {
    if (isBegin == true) {
        size_t i = 0;
        while (_data[i] != value) {
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
    if (capacity > _capacity) {
        MyVector bufVector(*this);
        delete[] _data;
        _data = new ValueType[capacity];
        _capacity = capacity;
        _size = bufVector._size;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<_size; i++) {
            _data[i] = bufVector._data[i];
        }
    }
    else {
        MyVector bufVector(*this);
        delete[] _data;
        _data = new ValueType[capacity];
        _capacity = capacity;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<capacity; i++) {
            _data[i] = bufVector._data[i];
        }
        resize(capacity-1);
    }
}


void MyVector:: resize(const size_t size, const ValueType value) {
    if (size < 0) {
        throw std::invalid_argument("Too small size");
    }
    if (_size == size) {
        return;
    }
    if (size>_capacity) {
        changeCapacityInsert(_size);
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

void MyVector:: changeCapacityInsert(size_t copyNum) {
    if (_strategy == ResizeStrategy::Additive) {
        MyVector bufVector(*this);
        size_t bufCapacity = std::ceil(bufVector._capacity + bufVector._coef);
        delete[] _data;
        _data = new ValueType[bufCapacity];
        _capacity = bufCapacity;
        _size = bufVector._size;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<copyNum; i++) {
            _data[i] = bufVector._data[i];
        }
    }
    else {
        MyVector bufVector(*this);
        size_t bufCapacity = std::ceil(bufVector._capacity * bufVector._coef);
        delete[] _data;
        _data = new ValueType[bufCapacity];
        _capacity = bufCapacity;
        _size = bufVector._size;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<copyNum; i++) {
            _data[i] = bufVector._data[i];
        }
    }
}

void MyVector:: changeCapacityRemove(size_t copyNum) {
    if (_strategy == ResizeStrategy::Additive) {
        MyVector bufVector(*this);
        size_t bufCapacity = std::ceil(bufVector._size + bufVector._coef);
        delete[] _data;
        _data = new ValueType[bufCapacity];
        _capacity = bufCapacity;
        _size = bufVector._size;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<copyNum; i++) {
            _data[i] = bufVector._data[i];
        }
    }
    else {
        MyVector bufVector(*this);
        size_t bufCapacity;
        if (bufVector._size == 0) {
            bufCapacity = 1;
        }
        else {
            bufCapacity = std::ceil(bufVector._size * bufVector._coef);
        }
        delete[] _data;
        _data = new ValueType[bufCapacity];
        _capacity = bufCapacity;
        _size = bufVector._size;
        _coef = bufVector._coef;
        _strategy = bufVector._strategy;
        for (size_t i = 0; i<copyNum; i++) {
            _data[i] = bufVector._data[i];
        }
    }
}

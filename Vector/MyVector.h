#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>

// стратегия изменения capacity
enum class ResizeStrategy {
    Additive,
    Multiplicative
};

enum class SortedStrategy {
    decrease,
    increase
};

template <class ValueType>

class MyVector
{
public:
    MyVector(size_t size = 0, ResizeStrategy strategy = ResizeStrategy::Multiplicative, float coef = 1.5f);
    MyVector(size_t size, ValueType value, ResizeStrategy strategy = ResizeStrategy::Multiplicative, float coef = 1.5f);
    
    MyVector(const MyVector& copy);
    MyVector& operator=(const MyVector& copy);

    ~MyVector();

    // для умненьких — реализовать конструктор и оператор для перемещения

    size_t capacity() const;
    size_t size() const;
    float loadFactor();
    
    class iterator
    {
    public:
        iterator(const MyVector* vector, int index):iVector(),index(){
            
        }
        const ValueType& operator*() const {
            return iVector->operator[](index);
        }
        const ValueType& operator*() {
            return iVector->operator[](index);
        }
        const ValueType operator->() const{
            return iVector->operator[](index);
        }
        ValueType operator->() {
            return iVector->operator[](index);
        }
        iterator &operator++() {
            ++index;
            return *this;
        }
        bool operator!=(const iterator& other) {
            return index != other.index;
        }
        bool operator==(const iterator& other) {
            return index == other.index;
        }
    private:
        const MyVector* iVector;
        int index = -1;
    };
    
    iterator begin() const;
    iterator end() const;
    
    // доступ к элементу,
    // должен работать за O(1)
    ValueType& operator[](const size_t i);
    const ValueType& operator[](const size_t i) const;

    // добавить в конец,
    // должен работать за amort(O(1))
    void pushBack(const ValueType& value);
    // вставить,
    // должен работать за O(n)
    void insert(const size_t i, const ValueType& value);    // версия для одного значения
    void insert(const size_t i, const MyVector& value);        // версия для вектора

    // удалить с конца,
    // должен работать за amort(O(1))
    void popBack();
    // удалить
    // должен работать за O(n)
    void erase(const size_t i);
    void erase(const size_t i, const size_t len);            // удалить len элементов начиная с i

    // найти элемент,
    // должен работать за O(n)
    // если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
    // если искомого элемента нет, вернуть -1
    long long int find(const ValueType& value, bool isBegin = true) const;

    // зарезервировать память (принудительно задать capacity)
    void reserve(const size_t capacity);

    // изменить размер
    // если новый размер больше текущего, то новые элементы забиваются дефолтными значениями
    // если меньше - обрезаем вектор
    void resize(const size_t size, const ValueType value = 2);

    // очистка вектора, без изменения capacity
    void clear();
    
    void print();
    void changeCapacityInsert(size_t copyNum);
    void changeCapacityRemove(size_t copyNum);
    
    MyVector sortedSquares(const MyVector& vec, SortedStrategy strategy) {
        for (size_t i = 0; i<vec._size; i++) {
            vec._data[i] *= vec._data[i];
        }
        switch (strategy) {
            case SortedStrategy::increase: {
                break;
            }
            case SortedStrategy::decrease: {
                break;
            }
            default:
                break;
        }
        return vec;
    }
    
private:
    ValueType* _data = nullptr;
    size_t _size = 0;
    size_t _capacity;
    float _coef;
    ResizeStrategy _strategy;
};

template <class ValueType>
MyVector<ValueType>::MyVector(size_t size, ResizeStrategy strategy, float coef) {
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

template <class ValueType>
MyVector<ValueType>::MyVector(size_t size, ValueType value, ResizeStrategy strategy, float coef) {
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

template <class ValueType>
MyVector<ValueType>::MyVector(const MyVector& copy) {
    _size = copy._size;
    _capacity = copy._capacity;
    _coef = copy._coef;
    _strategy = copy._strategy;
    _data = new ValueType[_capacity];
    for (size_t i = 0; i<copy._capacity; i++) {
        _data[i] = copy._data[i];
    }
}

template <class ValueType>
MyVector<ValueType>& MyVector<ValueType>::operator=(const MyVector& copy) {
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

template <class ValueType>
MyVector<ValueType>::~MyVector() {
    delete[] _data;
    _capacity = 0;
    _size = 0;
    _coef = 0;
}

template <class ValueType>
size_t MyVector<ValueType>:: capacity() const {
    return _capacity;
}

template <class ValueType>
size_t MyVector<ValueType>:: size() const {
    return _size;
}

template <class ValueType>
float MyVector<ValueType>:: loadFactor() {
    return (float)_size/_capacity;
}

template <class ValueType>
typename MyVector<ValueType>::iterator MyVector<ValueType>::begin() const {
    return MyVector::iterator{this, 0};
}

template <class ValueType>
typename MyVector<ValueType>::iterator MyVector<ValueType>::end() const{
    return MyVector::iterator{this, static_cast<int>(_size)};
}

template <class ValueType>
ValueType& MyVector<ValueType>:: operator[](const size_t i) {
    if (i >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    return _data[i];
}

template <class ValueType>
const ValueType& MyVector<ValueType>:: operator[](const size_t i) const {
    if (i >= _size) {
        throw std::invalid_argument("Element's missing");
    }
    return _data[i];
}

template <class ValueType>
void MyVector<ValueType>:: pushBack(const ValueType& value) {
    _size++;
    if (loadFactor()>=1) {
        changeCapacityInsert(_size-1);
    }
    _data[_size-1] = value;
}

template <class ValueType>
void MyVector<ValueType>:: insert(const size_t i, const ValueType& value) {
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

template <class ValueType>
void MyVector<ValueType>:: insert(const size_t i, const MyVector& value) {
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

template <class ValueType>
void MyVector<ValueType>:: popBack() {
    if (_size == 0) {
        return;
    }
    _size--;
    if (loadFactor()<=(1/_coef)*(1/_coef)) {
        changeCapacityRemove(_size);
    }
    _data[_size] = 0;
}

template <class ValueType>
void MyVector<ValueType>:: erase(const size_t i) {
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

template <class ValueType>
void MyVector<ValueType>:: erase(const size_t i, const size_t len) {
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

template <class ValueType>
long long int MyVector<ValueType>:: find(const ValueType& value, bool isBegin) const {
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

template <class ValueType>
void MyVector<ValueType>:: reserve(const size_t capacity) {
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

template <class ValueType>
void MyVector<ValueType>:: resize(const size_t size, const ValueType value) {
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

template <class ValueType>
void MyVector<ValueType>:: clear() {
    if (_size == 0) {
        return;
    }
    for (size_t i = 0; i<_size; i++) {
        _data[i] = 0;
    }
    _size = 0;
}

template <class ValueType>
void MyVector<ValueType>:: print() {
    for (size_t i = 0; i<_size; i++) {
        std::cout<<_data[i]<<std::endl;
    }
}

template <class ValueType>
void MyVector<ValueType>:: changeCapacityInsert(size_t copyNum) {
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

template <class ValueType>
void MyVector<ValueType>:: changeCapacityRemove(size_t copyNum) {
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

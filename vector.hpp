#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace usu
{
    template <typename T>
    class vector
    {
      public:
        using size_type = std::size_t;
        using reference = T&;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;
        reference operator[](size_type index);

        // Constructors
        vector();
        vector(size_type size);
        vector(resize_type resize);
        vector(size_type size, resize_type resize);
        vector(std::initializer_list<T> list);
        vector(std::initializer_list<T> list, resize_type resize);

        // Vector modifiers
        void add(T value);
        void insert(size_type index, T value);
        void remove(size_type index);
        void clear();

        // Vector info
        size_type size();
        size_type capacity();

        class iterator
        {
          private:
            T* ptr;

          public:
            iterator(T* p) :
                ptr(p) {}
            iterator(const iterator& it) :
                ptr(it.ptr) {}
            iterator& operator=(const iterator& it);
            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);
            bool operator==(const iterator& rhs);
            bool operator!=(const iterator& rhs);
            T& operator*();
            T* operator->();
        };

        iterator begin();
        iterator end();

      private:
        size_type _size;
        size_type _capacity;
        resize_type _resize;
        pointer _data;
    };

    // Constructors
    template <typename T>
    vector<T>::vector() :
        _size(0), _capacity(10), _resize([](typename vector<T>::size_type currentCapacity)
                                         {
                                             return currentCapacity * 2;
                                         }),
        _data(new T[_capacity])
    {
    }

    template <typename T>
    vector<T>::vector(size_type size) :
        _size(size), _capacity(size > 5 ? size * 2 : 10),
        _resize([](size_type currentCapacity)
                {
                    return currentCapacity * 2;
                }),
        _data(new T[_capacity])
    {
    }

    template <typename T>
    vector<T>::vector(resize_type resize) :
        _size(0), _capacity(10), _resize(resize),
        _data(new T[_capacity])
    {
    }

    template <typename T>
    vector<T>::vector(size_type size, resize_type resize) :
        _size(size), _capacity(size > 5 ? size * 2 : 10), _resize(resize),
        _data(new T[_capacity])
    {
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list) :
        _size(0),
        _capacity(10),
        _resize([](size_type currentCapacity)
                {
                    return currentCapacity * 2;
                }),
        _data(new T[_capacity])
    {
        for (const auto& value : list)
        {
            add(value);
        }
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list, resize_type resize) :
        vector(list.size(), resize)
    {
        std::copy(list.begin(), list.end(), _data.get());
    }

    // Methods and what nots
    // Vector stuff
    template <typename T>
    typename vector<T>::reference vector<T>::operator[](size_type index)
    {
        if (index >= _size)
            throw std::range_error("Index out of range");
        return _data[index];
    }

    template <typename T>
    void vector<T>::add(T value)
    {
        if (_size == _capacity)
        {
            _capacity = _resize(_capacity);
            pointer newData(new T[_capacity]);
            std::copy(_data.get(), _data.get() + _size, newData.get());
            _data = newData;
        }
        _data[_size++] = value;
    }

    template <typename T>
    void vector<T>::insert(size_type index, T value)
    {
        if (index > _size)
            throw std::range_error("Index out of range");
        if (_size == _capacity)
        {
            _capacity = _resize(_capacity);
            pointer newData(new T[_capacity]);
            std::copy(_data.get(), _data.get() + _size, newData.get());
            _data = newData;
        }
        std::copy_backward(_data.get() + index, _data.get() + _size, _data.get() + _size + 1);
        _data[index] = value;
        ++_size;
    }

    template <typename T>
    void vector<T>::remove(size_type index)
    {
        if (index >= _size)
            throw std::range_error("Index out of range");
        std::copy(_data.get() + index + 1, _data.get() + _size, _data.get() + index);
        --_size;
    }

    template <typename T>
    void vector<T>::clear()
    {
        _size = 0;
    }

    template <typename T>
    typename vector<T>::size_type vector<T>::size()
    {
        return _size;
    }

    template <typename T>
    typename vector<T>::size_type vector<T>::capacity()
    {
        return _capacity;
    }

    // Iterator stuff
    template <typename T>
    typename vector<T>::iterator vector<T>::begin()
    {
        return iterator(_data.get());
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::end()
    {
        return iterator(_data.get() + _size);
    }

    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator=(const iterator& it)
    {
        ptr = it.ptr;
        return *this;
    }

    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator++()
    {
        ++ptr;
        return *this;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator++(int)
    {
        iterator tmp(*this);
        ++ptr;
        return tmp;
    }

    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator--()
    {
        --ptr;
        return *this;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator--(int)
    {
        iterator tmp(*this);
        --ptr;
        return tmp;
    }

    template <typename T>
    bool vector<T>::iterator::operator==(const iterator& rhs)
    {
        return ptr == rhs.ptr;
    }

    template <typename T>
    bool vector<T>::iterator::operator!=(const iterator& rhs)
    {
        return ptr != rhs.ptr;
    }

    template <typename T>
    T& vector<T>::iterator::operator*()
    {
        return *ptr;
    }

    template <typename T>
    T* vector<T>::iterator::operator->()
    {
        return ptr;
    }

} // namespace usu
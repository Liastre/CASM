/// @file buffer_array.hpp
/// @brief class BufferStream to storage stream data

#pragma once

#include <exception>
#include <type_traits>
#include <valarray>
#include <vector>

namespace CASM {

template <typename T>
class CircleBuffer {
public:
    static_assert(!std::is_pointer<T>::value, "expected a non pointer type");

public:
    /// @name defines
    /// @{
    using value_type = T;
    /// @}

    CircleBuffer();
    CircleBuffer(std::size_t size);
    ~CircleBuffer();
    bool push(T const data);
    bool push(std::vector<T> const& data);
    bool push(T const* data, std::size_t size);
    bool push(CircleBuffer const& data);
    T pop() const;
    bool pop(T& data) const;
    bool pop(T* data, std::size_t size) const;
    bool pop(std::vector<T>& data) const;
    void clear();
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    T operator[](std::size_t index) const;

private:
    std::valarray<T> _buffer;
    std::size_t _allocatedSize{ 0 };
    mutable std::size_t _currentSize{ 0 };
    mutable std::size_t _headIndex{ 0 };
    std::size_t _tailIndex{ 0 };
};

template <typename T>
CircleBuffer<T>::CircleBuffer() {
}

template <typename T>
CircleBuffer<T>::CircleBuffer(std::size_t size) {
    _buffer = std::valarray<T>(size);
    _allocatedSize = size;
}

template <typename T>
CircleBuffer<T>::~CircleBuffer() {
}

template <typename T>
bool
CircleBuffer<T>::push(T const data) {
    if (_currentSize + 1 > _allocatedSize)
        return false;

    _buffer[_tailIndex++] = data;
    if (_tailIndex == _allocatedSize)
        _tailIndex = 0;
    _currentSize++;

    return true;
}

template <typename T>
bool
CircleBuffer<T>::push(std::vector<T> const& data) {
    auto elementsSize = data.size();
    if (_currentSize + elementsSize > _allocatedSize)
        return false;

    for (std::size_t i = 0; i < elementsSize; i++) {
        _buffer[_tailIndex] = data[i];
        if (++_tailIndex == _allocatedSize)
            _tailIndex = 0;
    }

    _currentSize += elementsSize;

    return true;
}

template <typename T>
bool
CircleBuffer<T>::push(T const* data, std::size_t size) {
    if (_currentSize + size > _allocatedSize)
        return false;

    for (std::size_t i = 0; i < size; i++) {
        _buffer[_tailIndex] = data[i];
        if (++_tailIndex == _allocatedSize)
            _tailIndex = 0;
    }

    _currentSize += size;

    return true;
}

template <typename T>
bool
CircleBuffer<T>::push(CircleBuffer const& data) {
    if (_currentSize + data._currentSize > _allocatedSize)
        return false;

    for (std::size_t i = 0; i < data._currentSize; i++) {
        _buffer[_tailIndex] = data[i];
        if (++_tailIndex == _allocatedSize)
            _tailIndex = 0;
    }

    _currentSize += data._currentSize;

    return false;
}

template <typename T>
T
CircleBuffer<T>::pop() const {
    T value;
    if (!pop(value))
        throw std::length_error("trying to pop from empty buffer");

    return value;
}

template <typename T>
bool
CircleBuffer<T>::pop(T& data) const {
    if (_currentSize == 0)
        return false;

    data = _buffer[_headIndex];
    if (++_headIndex == _allocatedSize)
        _headIndex = 0;
    --_currentSize;

    return true;
}

template <typename T>
bool
CircleBuffer<T>::pop(T* data, std::size_t size) const {
    if (size > _currentSize)
        return false;

    for (std::size_t i = 0; i < size; i++) {
        data[i] = _buffer[_headIndex];
        if (++_headIndex == _allocatedSize)
            _headIndex = 0;
        --_currentSize;
    }

    return true;
}

template <typename T>
bool
CircleBuffer<T>::pop(std::vector<T>& data) const {
    auto elementsSize = data.size();
    if (elementsSize > _currentSize)
        return false;

    for (std::size_t i = 0; i < elementsSize; i++) {
        data[i] = _buffer[_headIndex];
        if (++_headIndex == _allocatedSize)
            _headIndex = 0;
        _currentSize--;
    }

    return true;
}

template <typename T>
void
CircleBuffer<T>::clear() {
    _tailIndex = _headIndex = _currentSize = 0;
}

template <typename T>
std::size_t
CircleBuffer<T>::size() const noexcept {
    return _currentSize;
}

template <typename T>
bool
CircleBuffer<T>::empty() const noexcept {
    return _currentSize == 0;
}

template <typename T>
bool
CircleBuffer<T>::full() const noexcept {
    return _currentSize == _allocatedSize;
}

template <typename T>
T
CircleBuffer<T>::operator[](std::size_t index) const {
    return _buffer[_headIndex + index];
}

} // namespace CASM

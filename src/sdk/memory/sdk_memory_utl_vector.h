//
// Created by sexey on 05.12.2025.
//

#ifndef DXX_DLC_SDK_MEMORY_UTL_VECTOR_H
#define DXX_DLC_SDK_MEMORY_UTL_VECTOR_H

#include <algorithm>
#include <cstdint>
#include <new>
#include <stdexcept>

#include "sdk_memory.h"

namespace sdk::memory
{
    //@note You definitely cant use in for non-POD sturctures. Btw its a `Valve` wish.
    template<typename T>
    class C_UtlVector
    {
    public:
        using iterator = T*;
        using const_iterator = const T*;

    private:
        size_t size_;
        T* elements_;
        size_t capacity_;

        void growIfNeeded()
        {
            if (this->size_ + 1 <= capacity_) {
                return;
            }

            reserve((this->capacity_) ? this->capacity_ * 2 : 4);
        }

    public:

        [[nodiscard]] size_t size() const { return this->size_; }

        [[nodiscard]] size_t capacity() const { return this->capacity_; }

        T* data() { return this->elements_; }

        [[nodiscard]] bool empty() const { return this->size_ == 0; }

        void push_back(T&& value)
        {
            growIfNeeded();

            new(&this->elements_[this->size_]) T(std::move(value));
            ++this->size_;
        }

        void push_back(const T& value)
        {
            growIfNeeded();

            new (&this->elements_[this->size_]) T(value);
            ++this->size_;
        }

        template<typename ... arg_t>
        void emplace_back(arg_t&&... args)
        {
            growIfNeeded();

            new (&elements_[size_]) T(std::forward<arg_t>(args)...);
            ++this->size_;
        }

        void reserve(uint32_t newCapacity)
        {
            if (newCapacity <= this->capacity_) {
                return;
            }

            void* tmp = nullptr;
            if (this->elements_) {
                tmp = realloc(this->elements_, sizeof(T) * newCapacity);
            } else {
                tmp = alloc(newCapacity * sizeof(T));
            }

            if (!tmp) {
                throw std::bad_alloc();
            }

            this->elements_ = static_cast<T*>(tmp);
            this->capacity_ = newCapacity;
        }

        void pop_back()
        {
            if (!this->size_) {
                throw std::bad_array_new_length();
            }

            --this->size_;
            this->elements_[this->size_].~T();
        }

        void clear()
        {
            for (auto i = 0; i < this->size_; ++i) {
                this->elements_[i].~T();
            }

            this->size_ = 0;
        }

        T& operator[](uint32_t index)
        {
            if (index >= this->size_) {
                throw std::out_of_range("index out of bounds!");
            }

            return elements_[index];
        }

        const T& operator[](uint32_t index) const
        {
            if (index >= this->size_) {
                throw std::out_of_range("index out of bounds!");
            }

            return elements_[index];
        }

        iterator begin() { return this->elements_; }

        iterator end() { return this->elements_ + this->size_; }

        const_iterator begin() const { return this->elements_; }

        const_iterator end() const { return this->elements_ + this->size_; }

        C_UtlVector() : size_(0), elements_(nullptr), capacity_(0) {}

        C_UtlVector(C_UtlVector&& other) noexcept
        {
            this->size_ = other.size_;
            this->elements_ = other.elements_;
            this->capacity_ = other.capacity_;

            other.size_ = 0;
            other.elements_ = nullptr;
            other.capacity_ = 0;
        }

        C_UtlVector(const C_UtlVector&) = delete;

        C_UtlVector& operator=(const C_UtlVector&) = delete;

        ~C_UtlVector()
        {
            if (!this->elements_) {
                return;
            }

            clear();
            free(this->elements_);
            this->capacity_ = 0;
        }
    };
} // sdk

#endif //DXX_DLC_SDK_MEMORY_UTL_VECTOR_H
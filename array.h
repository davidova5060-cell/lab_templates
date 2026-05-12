#pragma once

#include <cstddef>

template <typename T>
class Array {
public:
    // ======================== Constructors ========================

    Array() : arr_(nullptr), sz_(0) {}

    explicit Array(size_t size) : arr_(size > 0 ? new T[size]() : nullptr), sz_(size) {}

    Array(size_t size, const T& value) : arr_(size > 0 ? new T[size] : nullptr), sz_(size) {
        for (size_t i = 0; i < sz_; ++i) {
            arr_[i] = value;
        }
    }

    // =================== Copy and Move ===========================

    Array(const Array& other) : sz_(other.sz_), arr_(sz_ > 0 ? new T[sz_] : nullptr) {
        for (size_t i = 0; i < sz_; ++i) {
            arr_[i] = other.arr_[i];
        }
    }

    Array(Array&& other) noexcept : arr_(other.arr_), sz_(other.sz_) {
        other.arr_ = nullptr;
        other.sz_ = 0;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            T* new_arr = other.sz_ > 0 ? new T[other.sz_] : nullptr;
            for (size_t i = 0; i < other.sz_; ++i) {
                new_arr[i] = other.arr_[i];
            }
            delete[] arr_;
            arr_ = new_arr;
            sz_ = other.sz_;
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] arr_;
            arr_ = other.arr_;
            sz_ = other.sz_;
            other.arr_ = nullptr;
            other.sz_ = 0;
        }
        return *this;
    }

    // =================== Destructor ==============================

    ~Array() {
        delete[] arr_;
    }

    // =================== Element access ==========================

    T& operator[](size_t index) {
        return arr_[index];
    }

    const T& operator[](size_t index) const {
        return arr_[index];
    }

    T& front() {
        return arr_[0];
    }

    const T& front() const {
        return arr_[0];
    }

    T& back() {
        return arr_[sz_ - 1];
    }

    const T& back() const {
        return arr_[sz_ - 1];
    }

    T* data() {
        return arr_;
    }

    const T* data() const {
        return arr_;
    }

    // =================== Capacity ================================

    size_t size() const {
        return sz_;
    }

    bool empty() const {
        return sz_ == 0;
    }

    // =================== Operations ==============================

    void fill(const T& value) {
        for (size_t i = 0; i < sz_; ++i) {
            arr_[i] = value;
        }
    }

    void swap(Array& other) noexcept {
        T* tmp_arr = arr_;
        size_t tmp_sz = sz_;
        
        arr_ = other.arr_;
        sz_ = other.sz_;
        
        other.arr_ = tmp_arr;
        other.sz_ = tmp_sz;
    }

    // =================== Comparison ==============================

    bool operator==(const Array& rhs) const {
        if (sz_ != rhs.sz_) return false;
        for (size_t i = 0; i < sz_; ++i) {
            if (arr_[i] != rhs.arr_[i]) return false;
        }
        return true;
    }

    bool operator!=(const Array& rhs) const {
        return !(*this == rhs);
    }

private:
    T* arr_ = nullptr;
    size_t sz_ = 0;
};

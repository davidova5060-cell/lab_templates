#pragma once

#include <utility>
#include <memory>

template <typename T>
class UniquePtr {
public:
    UniquePtr() : ptr_(nullptr) {}
    explicit UniquePtr(T* ptr) : ptr_(ptr) {}
    UniquePtr(std::nullptr_t) : ptr_(nullptr) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete ptr_;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        delete ptr_;
        ptr_ = new_ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
    }

private:
    T* ptr_;
};

// Специализация для массивов
template <typename T>
class UniquePtr<T[]> {
public:
    UniquePtr() : ptr_(nullptr) {}
    explicit UniquePtr(T* ptr) : ptr_(ptr) {}
    UniquePtr(std::nullptr_t) : ptr_(nullptr) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete[] ptr_;
    }

    T& operator[](size_t index) const { return ptr_[index]; }
    T* get() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        delete[] ptr_;
        ptr_ = new_ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
    }

private:
    T* ptr_;
};

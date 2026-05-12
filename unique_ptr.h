\#pragma once

#include <cstddef>
#include <memory>
#include <utility>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    UniquePtr() : m_ptr(nullptr), m_deleter() {}
    explicit UniquePtr(T* ptr) : m_ptr(ptr), m_deleter() {}
    UniquePtr(T* ptr, const Deleter& deleter) : m_ptr(ptr), m_deleter(deleter) {}
    UniquePtr(T* ptr, Deleter&& deleter) : m_ptr(ptr), m_deleter(std::move(deleter)) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) : m_ptr(other.m_ptr), m_deleter(std::move(other.m_deleter)) {
        other.m_ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            reset();
            m_ptr = other.m_ptr;
            m_deleter = std::move(other.m_deleter);
            other.m_ptr = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        reset();
    }

    T* get() { return m_ptr; }
    const T* get() const { return m_ptr; }
    explicit operator bool() const { return m_ptr != nullptr; }
    T& operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }
    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }
    Deleter& get_deleter() { return m_deleter; }
    const Deleter& get_deleter() const { return m_deleter; }

    T* release() {
        T* old = m_ptr;
        m_ptr = nullptr;
        return old;
    }

    void reset(T* ptr = nullptr) {
        T* old = m_ptr;
        m_ptr = ptr;
        if (old) {
            m_deleter(old);
        }
    }

    void swap(UniquePtr& other) {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_deleter, other.m_deleter);
    }

private:
    T* m_ptr = nullptr;
    [[no_unique_address]] Deleter m_deleter;
};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    UniquePtr() : m_ptr(nullptr), m_deleter() {}
    explicit UniquePtr(T* ptr) : m_ptr(ptr), m_deleter() {}
    UniquePtr(T* ptr, const Deleter& deleter) : m_ptr(ptr), m_deleter(deleter) {}
    UniquePtr(T* ptr, Deleter&& deleter) : m_ptr(ptr), m_deleter(std::move(deleter)) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) : m_ptr(other.m_ptr), m_deleter(std::move(other.m_deleter)) {
        other.m_ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            reset();
            m_ptr = other.m_ptr;
            m_deleter = std::move(other.m_deleter);
            other.m_ptr = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        reset();
    }

    T* get() { return m_ptr; }
    const T* get() const { return m_ptr; }
    explicit operator bool() const { return m_ptr != nullptr; }
    T& operator[](size_t index) { return m_ptr[index]; }
    const T& operator[](size_t index) const { return m_ptr[index]; }
    Deleter& get_deleter() { return m_deleter; }
    const Deleter& get_deleter() const { return m_deleter; }

    T* release() {
        T* old = m_ptr;
        m_ptr = nullptr;
        return old;
    }

    void reset(T* ptr = nullptr) {
        T* old = m_ptr;
        m_ptr = ptr;
        if (old) {
            m_deleter(old);
        }
    }

    void swap(UniquePtr& other) {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_deleter, other.m_deleter);
    }

private:
    T* m_ptr = nullptr;
    [[no_unique_address]] Deleter m_deleter;
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
#

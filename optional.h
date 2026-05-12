#pragma once

#include <stdexcept>
#include <utility>

template <typename T>
class Optional {
public:
    Optional() : m_has_value(false) {}
    Optional(const T& value) : m_has_value(true), m_value(value) {}
    Optional(T&& value) : m_has_value(true), m_value(std::move(value)) {}
    Optional(std::nullopt_t) : m_has_value(false) {}

    Optional(const Optional& other) : m_has_value(other.m_has_value) {
        if (m_has_value) m_value = other.m_value;
    }

    Optional(Optional&& other) noexcept : m_has_value(other.m_has_value) {
        if (m_has_value) m_value = std::move(other.m_value);
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            m_has_value = other.m_has_value;
            if (m_has_value) m_value = other.m_value;
        }
        return *this;
    }

    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            m_has_value = other.m_has_value;
            if (m_has_value) m_value = std::move(other.m_value);
        }
        return *this;
    }

    ~Optional() = default;

    T* operator->() { return &m_value; }
    const T* operator->() const { return &m_value; }
    T& operator*() { return m_value; }
    const T& operator*() const { return m_value; }

    explicit operator bool() const { return m_has_value; }
    bool has_value() const { return m_has_value; }

    T& value() {
        if (!m_has_value) throw std::bad_optional_access();
        return m_value;
    }

    const T& value() const {
        if (!m_has_value) throw std::bad_optional_access();
        return m_value;
    }

    T value_or(const T& default_value) const {
        return m_has_value ? m_value : default_value;
    }

    void reset() {
        m_has_value = false;
    }

    void swap(Optional& other) {
        std::swap(m_has_value, other.m_has_value);
        if (m_has_value && other.m_has_value) {
            std::swap(m_value, other.m_value);
        } else if (m_has_value) {
            other.m_value = std::move(m_value);
            reset();
        } else if (other.m_has_value) {
            m_value = std::move(other.m_value);
            other.reset();
        }
    }

    template <typename... Args>
    T& emplace(Args&&... args) {
        m_value = T(std::forward<Args>(args)...);
        m_has_value = true;
        return m_value;
    }

    bool operator==(const Optional& rhs) const {
        if (m_has_value != rhs.m_has_value) return false;
        if (!m_has_value) return true;
        return m_value == rhs.m_value;
    }

    bool operator!=(const Optional& rhs) const {
        return !(*this == rhs);
    }

private:
    bool m_has_value;
    T m_value;
};

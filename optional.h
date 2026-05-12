#pragma once

#include <stdexcept>
#include <utility>

template <typename T>
class Optional {
public:
    // ==================== Constructors ====================

    Optional() : has_value_(false) {}

    Optional(const T& value) : has_value_(true), value_(value) {}

    Optional(T&& value) : has_value_(true), value_(std::move(value)) {}

    Optional(std::nullopt_t) : has_value_(false) {}

    // ================= Copy and Move ===================

    Optional(const Optional& other) : has_value_(other.has_value_) {
        if (has_value_) {
            value_ = other.value_;
        }
    }

    Optional(Optional&& other) noexcept : has_value_(other.has_value_) {
        if (has_value_) {
            value_ = std::move(other.value_);
        }
        other.has_value_ = false;
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            has_value_ = other.has_value_;
            if (has_value_) {
                value_ = other.value_;
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            has_value_ = other.has_value_;
            if (has_value_) {
                value_ = std::move(other.value_);
            }
            other.has_value_ = false;
        }
        return *this;
    }

    // ================= Destructor ===================

    ~Optional() = default;

    // ================= Observers ===================

    T* operator->() {
        return &value_;
    }

    const T* operator->() const {
        return &value_;
    }

    T& operator*() {
        return value_;
    }

    const T& operator*() const {
        return value_;
    }

    explicit operator bool() const {
        return has_value_;
    }

    bool has_value() const {
        return has_value_;
    }

    T& value() {
        if (!has_value_) {
            throw std::bad_optional_access();
        }
        return value_;
    }

    const T& value() const {
        if (!has_value_) {
            throw std::bad_optional_access();
        }
        return value_;
    }

    T value_or(const T& default_value) const {
        return has_value_ ? value_ : default_value;
    }

    // ================= Modifiers ===================

    void reset() {
        has_value_ = false;
    }

    template <typename... Args>
    T& emplace(Args&&... args) {
        value_ = T(std::forward<Args>(args)...);
        has_value_ = true;
        return value_;
    }

    // ================= Comparison ===================

    bool operator==(const Optional& rhs) const {
        if (has_value_ != rhs.has_value_) return false;
        if (!has_value_) return true;
        return value_ == rhs.value_;
    }

    bool operator!=(const Optional& rhs) const {
        return !(*this == rhs);
    }

private:
    bool has_value_;
    T value_;
};

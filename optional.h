#pragma once

#include <stdexcept>
#include <utility>

template <typename T>
class Optional
{
public:
    Optional() : has_value(false) {}
    Optional(const T& value) : has_value(true), val(value) {}
    Optional(T&& value) : has_value(true), val(std::move(value)) {}
    Optional(std::nullopt_t) : has_value(false) {}

    Optional(const Optional& other) : has_value(other.has_value)
    {
        if (has_value) val = other.val;
    }

    Optional(Optional&& other) noexcept : has_value(other.has_value)
    {
        if (has_value) val = std::move(other.val);
        other.has_value = false;
    }

    Optional& operator=(const Optional& other)
    {
        if (this != &other)
        {
            has_value = other.has_value;
            if (has_value) val = other.val;
        }
        return *this;
    }

    Optional& operator=(Optional&& other) noexcept
    {
        if (this != &other)
        {
            has_value = other.has_value;
            if (has_value) val = std::move(other.val);
            other.has_value = false;
        }
        return *this;
    }

    ~Optional() = default;

    T* operator->() { return &val; }
    const T* operator->() const { return &val; }
    T& operator*() { return val; }
    const T& operator*() const { return val; }

    explicit operator bool() const { return has_value; }
    bool has_value() const { return has_value; }

    T& value()
    {
        if (!has_value) throw std::bad_optional_access();
        return val;
    }

    const T& value() const
    {
        if (!has_value) throw std::bad_optional_access();
        return val;
    }

    T value_or(const T& default_value) const
    {
        return has_value ? val : default_value;
    }

    void reset()
    {
        has_value = false;
    }

    void swap(Optional& other)
    {
        std::swap(has_value, other.has_value);
        if (has_value && other.has_value)
            std::swap(val, other.val);
        else if (has_value)
            other.val = std::move(val);
        else if (other.has_value)
            val = std::move(other.val);
    }

    template <typename... Args>
    T& emplace(Args&&... args)
    {
        val = T(std::forward<Args>(args)...);
        has_value = true;
        return val;
    }

    bool operator==(const Optional& rhs) const
    {
        if (has_value != rhs.has_value) return false;
        if (!has_value) return true;
        return val == rhs.val;
    }

    bool operator!=(const Optional& rhs) const
    {
        return !(*this == rhs);
    }

private:
    bool has_value;
    T val;
};

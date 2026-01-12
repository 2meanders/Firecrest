
#pragma once

namespace fc {

    /// @brief A wrapper around a value that tracks whether it has been modified.
    template<typename T>
    class Tracked {
    private:
        T _currentValue{};
        bool _hasChanged{false};

    public:
        // Default constructor
        Tracked() = default;

        // Constructor with initial value
        explicit Tracked(const T& initialValue) : _currentValue(initialValue), _hasChanged(false) {}

        explicit Tracked(const T& initialValue, bool hasChanged) : _currentValue(initialValue), _hasChanged(hasChanged) {}

        // Assignment operator
        Tracked& operator=(const T& newVal) {
            _hasChanged = (newVal != _currentValue);
            _currentValue = newVal;
            return *this;
        }

        // Conversion operator to T
        operator const T& () const {
            return _currentValue;
        }

        // Comparison operators
        bool operator==(const T& other) const { return _currentValue == other; }
        bool operator!=(const T& other) const { return _currentValue != other; }

        /// @brief Checks whether the value has been modified since the last call to `isModified`.
        bool isModified() {
            const bool changed = _hasChanged;
            _hasChanged = false;
            return changed;
        }

        /// @brief Manually mark the value as modified.
        void markModified() {
            _hasChanged = true;
        }

    };

}

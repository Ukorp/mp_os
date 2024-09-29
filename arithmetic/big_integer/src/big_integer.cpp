#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer::big_integer(
    int const *digits,
    size_t digits_count,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;
    if (digits == nullptr) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];


    if (digits_count != 1) {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    if (_other_digits == nullptr) {
        throw std::logic_error("Memory allocation failed");
    }
    *_other_digits = digits_count;
    std::memcpy(_other_digits + 1, digits, (digits_count - 1) * sizeof(unsigned int));
    
}

big_integer::big_integer(
    unsigned int const *digits,
    size_t digits_count,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;

    if (digits == nullptr) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];

    if (digits_count != 1) {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    if (_other_digits == nullptr) {
        throw std::logic_error("Memory allocation failed");
    }
    *_other_digits = digits_count;
    std::memcpy(_other_digits + 1, digits, (digits_count - 1) * sizeof(unsigned int));

}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;
    size_t digits_count = digits.size();
    if (digits.empty()) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];

    if (digits_count != 1) {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    if (_other_digits == nullptr) {
        throw std::logic_error("Memory allocation failed");
    }
    *_other_digits = digits_count;
    for (int i = 0; i < digits_count - 1; i++) {
        _other_digits[i + 1] = std::move(digits[i]);
    }

}

big_integer::big_integer(
    std::vector<unsigned int> const &digits,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;
    size_t digits_count = digits.size();
    if (digits.empty()) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];

    if (digits_count != 1) {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    if (_other_digits == nullptr) {
        throw std::logic_error("Memory allocation failed");
    }
    *_other_digits = digits_count;
    for (int i = 0; i < digits_count - 1; i++) {
        _other_digits[i + 1] = std::move(digits[i]);
    }

}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator): big_integer(convert_to_base(value_as_string, base), allocator)
{
    std::cout << convert_to_base(value_as_string, base)[0] << std::endl;
}

big_integer::~big_integer()
{
    if (_other_digits != nullptr) {
        delete [] _other_digits;
    }
}

big_integer::big_integer(
    big_integer const &other): big_integer(other._other_digits, other._other_digits[0],other._allocator)

{
    std::cout << other._oldest_digit << std::endl;
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this == &other) {
        return *this;
    }
    _allocator = other._allocator;
    _oldest_digit = other._oldest_digit;
    if (_other_digits != nullptr) {
        deallocate_with_guard(_other_digits);
    }
    _other_digits = nullptr;
    if (other._other_digits == nullptr) {
        return *this;
    }
    _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), other._other_digits[0]));
    for (int i = 0; i < other._other_digits[0]; i++) {
        _other_digits[i] = other._other_digits[i];
    }
    return *this;
}

big_integer::big_integer(
    big_integer &&other) noexcept: _allocator(other._allocator),
                           _oldest_digit(std::move(other._oldest_digit))
{
    _other_digits = nullptr;
    other._allocator = nullptr;
    if (other._other_digits == nullptr) {
        return;
    }

    memcpy(_other_digits, other._other_digits, (other._other_digits[0]) * sizeof(unsigned int));
    other._other_digits = nullptr;
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this == &other) {
        return *this;
    }
    _allocator = other._allocator;
    _allocator = nullptr;
    _oldest_digit = std::move(other._oldest_digit);
    if (_other_digits != nullptr) {
        delete [] _other_digits;
    }
    _other_digits = nullptr;
    if (other._other_digits == nullptr) {
        return *this;
    }
    memcpy(_other_digits, other._other_digits, (other._other_digits[0]) * sizeof(unsigned int));
    _other_digits = nullptr;
    return *this;
}

bool big_integer::operator==(
    big_integer const &other) const
{
    if (this == &other) {
        return true;
    }
    if (_oldest_digit != other._oldest_digit) {
        return false;
    }
    if (_other_digits[0] != other._other_digits[0]) {
        return false;
    }
    for (int i = 1; i <= _other_digits[0]; i++) {
        if (_other_digits[i] != other._other_digits[i]) {
            return false;
        }
    }
    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator!=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator<(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator>(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator<=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator>=(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-() const
{
    throw not_implemented("big_integer big_integer::operator-() const", "your code should be here...");
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator+=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator-=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer big_integer::operator~() const
{
    throw not_implemented("big_integer big_integer::operator~() const", "your code should be here...");
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator&=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator|=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator^=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator<<=(size_t)", "your code should be here...");
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(size_t) const", "your code should be here...");
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator>>=(size_t)", "your code should be here...");
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator>>(size_t) const", "your code should be here...");
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator>>(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    throw not_implemented("std::ostream &operator<<(std::ostream &, big_integer const &)", "your code should be here...");
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept {
    return this->_allocator;
}

std::vector<unsigned int> big_integer::convert_to_base(std::string const &biiiiiiiiiiig_number, size_t base)
{

}

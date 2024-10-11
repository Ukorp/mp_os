#include "../include/big_integer.h"

#include <limits>

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

void big_integer::copy(big_integer const& other) {
    if (_other_digits != nullptr) {
        deallocate_with_guard(_other_digits);
        _other_digits = nullptr;
    }
    _oldest_digit = other._oldest_digit;
    if (other._other_digits == nullptr) {
        _other_digits = nullptr;
        return;
    }
    _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), *other._other_digits));

    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::move(big_integer && other) {
    if (_other_digits != nullptr) {
        deallocate_with_guard(_other_digits);
        _other_digits = nullptr;
    }
    _oldest_digit = other._oldest_digit;
    if (other._other_digits == nullptr) {
        _other_digits = nullptr;
        return;
    }
    _allocator = other._allocator;
    other._allocator = nullptr;
    _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), *other._other_digits));

    std::memmove(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
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
    if (digits_count > 1) {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    else {
        return;
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
    _oldest_digit = static_cast<int>(digits[digits_count - 1]);

    if (digits_count > 1) {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    else {
        return;
    }
    *_other_digits = digits_count;
    std::memcpy(_other_digits + 1, digits, (digits_count - 1) * sizeof(unsigned int));
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;
    const unsigned int digits_count = digits.size();
    if (digits.empty()) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];
    if (digits_count > 1) {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    else {
        return;
    }
    *_other_digits = digits_count;
    for (unsigned int i = 0; i < digits_count - 1; i++) {
        _other_digits[i + 1] = digits[i];
    }

}

big_integer::big_integer(
    std::vector<unsigned int> const &digits,
    allocator *allocator): _allocator(allocator)
{
    _other_digits = nullptr;

    unsigned int digits_count = digits.size();
    if (digits.empty()) {
        throw std::logic_error("Integer array must be not null");
    }
    if (digits_count == 0) {
        throw std::logic_error("Quantity of digits must be more than 0");
    }
    _oldest_digit = digits[digits_count - 1];
    // std::cout << _oldest_digit << " " << digits[digits_count - 1];
    if (digits_count > 1) {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    else {
        return;
    }

    *_other_digits = digits_count;
    for (unsigned int i = 0; i < digits_count - 1; i++) {
        _other_digits[i + 1] = digits[i];
    }
}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator): big_integer(convert_to_base(value_as_string, base), allocator)
{

}

big_integer::~big_integer()
{
    if (_other_digits != nullptr) {
        deallocate_with_guard(_other_digits);
    }
    _other_digits = nullptr;
}

big_integer::big_integer(
    big_integer const &other)
{
    copy(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this == &other) {
        return *this;
    }

    copy(other);
    return *this;
}

big_integer::big_integer(
    big_integer &&other) noexcept
{
    move(std::move(other));
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this == &other) {
        return *this;
    }
    move(std::move(other));
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
    if (digits_count() != other.digits_count()) {
        return false;
    }
    for (int i = 0; i < digits_count(); i++) {
        if (get_digit(i) != other.get_digit(i)) {
            return false;
        }
    }
    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return !(*this == other);
}

bool big_integer::operator<(
    big_integer const &other) const
{
    if (*this == other) {

        return false;
    }
    if (is_negative() && !other.is_negative()) {
        return true;
    }
    if (!is_negative() && other.is_negative()) {
        return false;
    }
    if (digits_count() < other.digits_count()) {
        return true;
    }
    if (digits_count() > other.digits_count()){
        return false;
    }
    for (unsigned int i = digits_count() - 1; i >= 0; --i) {
        if (get_digit(i) < other.get_digit(i)) {
            if (!is_negative())
                return true;
            return false;
        }
        if (get_digit(i) > other.get_digit(i)) {
            if (!is_negative())
                return false;
            return true;
        }
    }
    return false;

}

bool big_integer::operator>(
    big_integer const &other) const
{
    if (*this == other) {
        return false;
    }
    return !(*this < other);
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    if (*this == other) {
        return true;
    }

    return *this < other;
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    if (*this == other) {
        return true;
    }
    return *this > other;
}

big_integer big_integer::operator-() const
{
    big_integer result(*this);
    result.change_sign();
    return result;
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    big_integer tmp(*this);
    tmp = *this + other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator+(
    big_integer const &other) const {
    big_integer copy1(*this);
    big_integer copy2(other);

    if (is_negative() && other.is_negative()) {
        copy1.change_sign();
        copy2.change_sign();
    }

    if (is_negative() && !other.is_negative()) {
        copy2.change_sign();
        return copy1 - copy2;
    }

    if (!is_negative() && other.is_negative()) {
        copy1.change_sign();
        return copy2 - copy1;
    }
    if (copy1.is_zero()) return copy2;
    if (copy2.is_zero()) return copy1;

    const unsigned int first = copy1.digits_count();
    const unsigned int second = copy2.digits_count();

    constexpr unsigned int shift = sizeof(unsigned int) << 2;
    const unsigned int mask = (1 << shift) - 1;
    const unsigned int max_digits = std::max(first, second);
    unsigned int sum_result = 0;
    std::vector<unsigned int> digits(max_digits + 1, 0);
    for (unsigned int i = 0; i < max_digits; ++i) {
        unsigned int first_elem = (i >= first) ? 0 : copy1.get_digit(i);
        unsigned int second_elem = (i >= second) ? 0 : copy2.get_digit(i);
        for (int j = 0; j < 2; ++j) {
            sum_result += (first_elem & mask) + (second_elem & mask);
            first_elem >>= shift;
            second_elem >>= shift;
            digits[i] |= (sum_result & mask) << (shift * j);
            sum_result >>= shift;
        }
    }
    while(digits.back() == 0) {
        digits.pop_back();
    }
    if(digits[digits.size() - 1] & (1 << ((sizeof(unsigned int) << 3) - 1))) {
        digits.push_back(0);
    }

    if (is_negative() && other.is_negative()) {
        digits.back() & ((1 << ((sizeof(unsigned int) << 3) - 1)));
    }

    return {digits, _allocator};

}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    big_integer tmp = *this - other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    big_integer copy1(*this);
    big_integer copy2(other);
    bool need_to_change = false;
    if (copy1.is_negative() && copy2.is_negative()) {
        copy2.change_sign();
        copy1.change_sign();
        swap(copy1, copy2);
    }

    if (copy1.is_negative() ^ copy2.is_negative()) {
        return copy1 + -other;
    }

    if (copy1 < copy2) {
        swap(copy1, copy2);
        need_to_change = true;
    }


    std::vector<unsigned int> result;
    const unsigned int size = copy1.digits_count();
    int minus_num = 0;
    for (unsigned int i = 0; i < size; ++i) {
        unsigned int first_digit = copy1.get_digit(i);
        unsigned int second_digit = (i >= copy2.digits_count()) ? 0 : copy2.get_digit(i);
        result.push_back(first_digit - second_digit - minus_num);
        minus_num = first_digit < second_digit;
    }
    return need_to_change ? -big_integer{result, _allocator} : big_integer{result, _allocator};
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    big_integer tmp = *this * other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{

    big_integer copy1(*this);
    big_integer copy2(other);
    unsigned int shift = sizeof(unsigned int) << 2;
    unsigned int mask = (1U << shift) - 1;
    big_integer sum_result("0");

    if (is_negative()) {
        copy1.change_sign();
    }
    if (other.is_negative()) {
        copy2.change_sign();
    }
    if (copy1 == sum_result || copy2 == sum_result) return sum_result;


    unsigned int result_num = 0;
    for (int i = 0; i < 2 * copy1.digits_count(); ++i) {
        unsigned int first_digit = copy1.get_digit(i / 2);
        unsigned int current_digit = (first_digit >> (shift * ((i + 2) % 2))) & mask;
        result_num = 0;
        for (int j = 0; j < 2 * copy2.digits_count(); ++j) {
            std::vector<unsigned int> tmp_vector;
            unsigned int second_digit = copy2.get_digit(j / 2);
            unsigned int multiply_digit = (second_digit >> (shift * ((j + 2) % 2))) & mask;
            result_num += current_digit * multiply_digit;
            tmp_vector.push_back(result_num & mask);
            result_num >>= shift;
            big_integer tmp(tmp_vector, _allocator);
            tmp <<= shift * (i + j);
            sum_result += tmp;

        }
        if (result_num) {
            big_integer last(std::to_string(result_num & mask));
            last <<= shift * (2 * copy2.digits_count() + i);
            sum_result += last;
        }
    }
    if (is_negative() ^ other.is_negative()) {
        sum_result.change_sign();
    }
    return sum_result;
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this * other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    big_integer tmp = *this / other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    if (other.is_zero()) {
        throw std::logic_error("divider must be not 0");
    }
    if (is_zero()) {return big_integer("0");}
    big_integer copy1(*this);
    big_integer copy2(other);
    bool need_to_change = false;
    if (copy1.is_negative() ^ copy2.is_negative()) {
        if (copy1.is_negative()) {
            copy1.change_sign();
        }
        if (copy2.is_negative()) {
            copy2.change_sign();
        }
        need_to_change = true;
    }
    if (copy1.is_negative() && copy2.is_negative()) {
        copy1.change_sign();
        copy2.change_sign();
    }
    big_integer result("0");
    big_integer local_result("0");
    for (int i = 0; i < copy1.digits_count() * sizeof(unsigned int) << 3; ++i) {
        // std::cout << result << std::endl;
        big_integer current = copy1.first_bytes(i);
        result <<= 1;
        local_result <<= 1;
        local_result |= current;
        if (local_result < copy2) {
            continue;
        }
        else {
            result |= big_integer("1");
            local_result -= copy2;
        }
    }
    return (need_to_change) ? -result : result;
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this / other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    big_integer tmp = *this / other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return *this - (*this / other) * other;
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this % other.first);
    result._allocator = other.second;
    return result;
}

big_integer big_integer::operator~() const
{
    big_integer result(*this);

    for (int i = 0; i < result.digits_count(); ++i) {
        result.get_link(i) = ~result.get_link(i);
    }
    return result;
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    big_integer tmp = *this & other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    unsigned int min_digits = std::min<unsigned int>(digits_count(), other.digits_count());
    std::vector<unsigned int> digits;
    for (unsigned int i = 0; i < min_digits; ++i) {
        digits.push_back(get_digit(i) & other.get_digit(i));
    }
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
    if (!is_negative() && (digits.back() &  (1 << (sizeof(unsigned int) << 3) - 1))) {
        digits.push_back(0);
    }

    return {digits, _allocator};
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this & other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    big_integer tmp = *this | other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    unsigned int min_digits = std::min<unsigned int>(digits_count(), other.digits_count());
    std::vector<unsigned int> digits;
    for (unsigned int i = 0; i < min_digits; ++i) {
        digits.push_back(get_digit(i) | other.get_digit(i));
    }
    while (digits_count() > min_digits) {
        digits.push_back(get_digit(min_digits++));
    }
    while (other.digits_count() > min_digits) {
        digits.push_back(other.get_digit(min_digits++));
    }
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
    if (!is_negative() && digits.back() & (1 << ((sizeof(unsigned int) << 3) - 1))) digits.push_back(0);
    return {digits, _allocator};

}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this | other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    big_integer tmp = *this ^ other;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    unsigned int min_digits = std::min<unsigned int>(digits_count(), other.digits_count());
    std::vector<unsigned int> digits;
    for (unsigned int i = 0; i < min_digits; ++i) {
        digits.push_back(get_digit(i) ^ other.get_digit(i));
    }
    while (digits_count() > min_digits) {
        digits.push_back(get_digit(min_digits++) ^ 0);
    }
    while (other.digits_count() > min_digits) {
        digits.push_back(other.get_digit(min_digits++) ^ 0);
    }
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
    return {digits, _allocator};
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    big_integer result(*this ^ other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    big_integer tmp = *this << shift;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    unsigned int number = 0;
    int count = 0;

    while (shift >= (sizeof(unsigned int) << 3)) {
        count = shift / (sizeof(unsigned int) << 3);
        shift = shift % (sizeof(unsigned int) << 3);

    }
    unsigned int mask = ((1 << shift) - 1) << ((sizeof(unsigned int) << 3) - shift);
    std::vector<unsigned int> answer_vec;
    if (is_negative()) {
        answer_vec = big_integer_to_vector(- *this);
    }
    else {
        answer_vec = big_integer_to_vector(*this);
    }

    for (int i = 0; i < answer_vec.size(); ++i) {
        unsigned int &changed_elem = answer_vec[i];
        unsigned int next_number = changed_elem & mask;
        changed_elem <<= shift;

        // if (shift >= sizeof(unsigned int) << 3) changed_elem = 0;
        changed_elem |= number >> (sizeof(unsigned int) << 3) - shift;
        number = next_number;
    }

    while (count--) answer_vec.insert(answer_vec.begin(), 0);
    if (number) answer_vec.push_back(number);
    if (is_negative() && !((answer_vec.back()) & (1 << (sizeof(unsigned int) << 3) - 1))) {
        answer_vec.back() ^= 1 << (sizeof(unsigned int) << 3) - 1;
    }
    if (!is_negative() && ((answer_vec.back()) & (1 << (sizeof(unsigned int) << 3) - 1))) {
        answer_vec.push_back(0);
    }
    return {answer_vec, _allocator};
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    big_integer result(*this << shift.first);
    result._allocator = shift.second;
    return result;
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    big_integer tmp = *this >> shift;
    *this = tmp;
    return *this;
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    unsigned int number = 0;
    unsigned int mask = (1 << shift) - 1;
    std::vector<unsigned int> answer_vec;
    if (is_zero()) {
        return *this;
    }
    if (is_negative()) {
        answer_vec = big_integer_to_vector(- *this);
    }
    else {
        answer_vec = big_integer_to_vector(*this);
    }
    while (shift >= (sizeof(unsigned int) << 3)) {
        answer_vec.erase(answer_vec.begin());
        shift -= (sizeof(unsigned int) << 3);
    }
    for (int i = answer_vec.size() - 1; i >= 0 ; --i) {
        unsigned int &changed_elem = answer_vec[i];
        unsigned int next_number = changed_elem & mask;
        changed_elem >>= shift;
        if (shift >= sizeof(unsigned int) << 3) changed_elem = 0;
        changed_elem |= number << (sizeof(unsigned int) << 3) - shift;
        number = next_number;
    }
    if (is_negative() && !((answer_vec.back()) & (1 << (sizeof(unsigned int) << 3) - 1))) {
        answer_vec.back() ^= 1 << (sizeof(unsigned int) << 3) - 1;
    }
    if (!is_negative() && ((answer_vec.back()) & (1 << (sizeof(unsigned int) << 3) - 1))) {
        answer_vec.push_back(0);
    }
    return {answer_vec, _allocator};
}


big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &other) const
{
    big_integer result(*this << other.first);
    result._allocator = other.second;
    return result;
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{

    return first_multiplier *= second_multiplier;
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    return first_multiplier * second_multiplier;
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
   return dividend /= divisor;
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    return dividend / divisor;
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    return dividend /= divisor;
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    return dividend / divisor;
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{

    return stream << value.to_string();
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    std::string input_string;
    stream >> input_string;
    value = big_integer(input_string);
    return stream;
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept
{
    return this->_allocator;
}

std::vector<unsigned int> big_integer::convert_to_base(std::string const &value_as_string, size_t base)
{
    bool is_negative = false;
    int pos = 0;
    if(value_as_string[0] == '-')
    {
        is_negative = true;
        pos = 1;
    }
    std::vector<unsigned int> result = divide_str_on_int(value_as_string, pos);
    if((result[result.size() - 1] & (1 << (sizeof(unsigned int) << 3) - 1)) != 0)
    {
        result.push_back(0);
    }
    if(result.size() == 1 && result.back() == 0)
        return result;

    if(is_negative)
    {
        result[result.size() - 1] |= 1<< (sizeof(int) << 3) - 1;
    }
    return result;

}

std::vector<unsigned int> big_integer::divide_str_on_int(std::string str, int position = 0)
{
    std::vector<unsigned int> result;
    size_t converted = 0;
    constexpr unsigned int max_int = -1;
    constexpr size_t base = static_cast<size_t>(max_int) + 1;
    while(position != str.length())
    {
        std::string next_number_to_divide;
        while(converted < base)
        {
            if(position == str.length()) break;

            converted = converted * 10 + (str[position++] - '0');
        }
        if(position == str.length())
        {
            if(converted >= base)
            {
                result.push_back(converted % base);
                converted /= base;
            }
            result.push_back(converted);
            return result;
        }
        while(position != str.length())
        {
            if(converted >= base)
            {
                next_number_to_divide.push_back(converted / base + '0');
                converted %= base;
            }
            else {
                next_number_to_divide.push_back('0');
            }
            if(position != str.length()) converted = converted * 10 + (str[position] - '0');
            position++;
        }
        if(converted >= base)
        {
            next_number_to_divide.push_back(converted / base + '0');
            converted %= base;
        }
        else {
            next_number_to_divide.push_back('0');
        }
        result.push_back(converted);
        str = std::move(next_number_to_divide);
        converted = 0;
        position = 0;
    }
    return result;
};

std::string big_integer::string_plus(std::string const &first, std::string const &second) {
    std::string result;
    unsigned int append_num = 0;
    size_t min_size = std::min(first.length(), second.length());
    for (int i = 0; i < min_size; ++i) {
        unsigned int new_digit = (first[i] - '0') + (second[i] - '0');
        result += static_cast<char>((new_digit + append_num + 10) % 10 + '0');
        append_num = (new_digit + append_num) / 10;
    }
    if (first.length() > min_size) {
        for (size_t i = min_size; i < first.length(); ++i) {
            unsigned int new_digit = (first[i] - '0');
            result += static_cast<char>((new_digit + append_num + 10) % 10 + '0');
            append_num = (new_digit + append_num) / 10;
        }
    }
    else {
        for (size_t i = min_size; i < second.length(); ++i) {
            unsigned int new_digit = (second[i] - '0');
            result += static_cast<char>((new_digit + append_num + 10) % 10 + '0');
            append_num = (new_digit + append_num) / 10;
        }
    }
    if (append_num != 0) result += static_cast<char>(append_num + '0');
    while (result.back() == '0') result.pop_back();
    return result;
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));
    return *this;
}

bool big_integer::is_negative() const {
    if (_oldest_digit == 0) {
        return false;
    }
    return (_oldest_digit >> ((sizeof(int) << 3) - 1)) & 1;
}

void big_integer::swap(big_integer &first, big_integer &second) noexcept {
    big_integer tmp(first);
    first = second;
    second = tmp;
}

unsigned int big_integer::digits_count() const {
    if (_other_digits == nullptr) {
        return 1;
    }
    return *_other_digits;
}

unsigned int &big_integer::get_link(unsigned int position){
    if (_other_digits == nullptr) {
        if (position == 0) {
            return *reinterpret_cast<unsigned int *>(&_oldest_digit);
        }
        throw std::out_of_range("big_integer::index_of: out of range");
    }
    if (position >= *_other_digits) {
        throw std::out_of_range("big_integer::index_of: out of range");
    }
    return (*_other_digits == position + 1) ?
        *reinterpret_cast<unsigned int *>(&_oldest_digit) :
        _other_digits[position + 1];
}

unsigned int big_integer::get_digit(unsigned int position) const {
    if (_other_digits == nullptr) {
        if (position == 0) {
            return static_cast<unsigned int>(_oldest_digit);
        }
        throw std::out_of_range("big_integer::index_of: out of range (" +
            std::to_string(position) +
            " with size " +
            std::to_string(digits_count()) + ")");
    }
    if (position >= *_other_digits) {
        throw std::out_of_range("big_integer::index_of: out of range (" +
            std::to_string(position) +
            " with size " +
            std::to_string(digits_count()) + ")");
    }
    return (*_other_digits == position + 1) ?
        static_cast<unsigned int>(_oldest_digit ):
        _other_digits[position + 1];
}

std::string big_integer::multiply_string(std::string first, std::string second) {
    std::string result = "0";
    std::reverse(first.begin(), first.end());
    std::reverse(second.begin(), second.end());
    while (first.back() == '0') {
        first.pop_back();
    }
    while (second.back() == '0') {
        second.pop_back();
    }
    for (int i = 0; i < first.length(); ++i) {
        std::string local_result;
        int append_num = 0;
        for (int j = 0; j < i; ++j) {
            local_result += '0';
        }
        for (int j = 0; j < second.length(); ++j) {
            const int multiply =  (first[i] - '0') * (second[j] - '0') + append_num;
            append_num = multiply / 10;
            local_result += static_cast<char>((multiply + 10) % 10 + '0');
        }
        local_result += static_cast<char>(append_num + '0');
        result = string_plus(result, local_result);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string big_integer::to_string() const {

    std::string result = "0";
    unsigned int max = -1;
    size_t base = static_cast<size_t>(max) + 1;
    big_integer copy1(*this);
    if (is_negative()) {
        copy1.change_sign();
    }
    if (copy1.digits_count() == 1 && copy1._oldest_digit == 0) {
        return "0";
    }
    std::string string_base = std::to_string(base);
    for (int i = 0; i < copy1.digits_count(); ++i) {

        std::string current_number = std::to_string(copy1.get_digit(i));
        for (int j = 0; j < i; ++j) {
            current_number = multiply_string(current_number, string_base);
        }

        result = string_plus_with_reverse(result, current_number);
        std::reverse(result.begin(), result.end());
    }
    if (is_negative()) {
        result.insert(result.begin(), '-');
    }
    return result;
}

std::string big_integer::string_plus_with_reverse(std::string first, std::string second) {
    std::reverse(first.begin(), first.end());
    std::reverse(second.begin(), second.end());
    return string_plus(first, second);
}

void big_integer::print_bytes(unsigned int i) {
    int k = 0;
    while (i > 0) {
        std::cout << (i & 1);
        i >>= 1;
        if ((k++ + 9) % 8 == 0) {
            std::cout << " ";
        }
    }
    if (k == 0) std::cout << 0;
    std::cout << std::endl;
}

std::vector<unsigned int> big_integer::big_integer_to_vector(big_integer const &number) const {
    std::vector<unsigned int> result;
    for (int i = 0; i < number.digits_count(); ++i) {
        result.push_back(number.get_digit(i));
    }
    while (result.back() == 0 && result.size() > 1) result.pop_back();
    if (!number.is_negative() && result.back() & 1 << (sizeof(unsigned int) << 3) - 1) {
        result.push_back(0);
    }
    return result;
}

bool big_integer::is_zero() const{
    if (digits_count() == 1 && get_digit(0) == 0) {
        return true;
    }
    return false;
}

std::vector<unsigned int> big_integer::vector_x_int(std::vector<unsigned int> vec, unsigned int num) {
    std::vector<unsigned int> result;
    unsigned int const max_int = -1;
    size_t base = static_cast<size_t>(max_int) + 1;
    std::reverse(vec.begin(), vec.end());
    unsigned int additional_number = 0;
    for(int i = 0; i < vec.size(); ++i)
    {
        size_t local_result = static_cast<size_t>(vec[i]) * static_cast<size_t>(num) + additional_number;
        result.push_back(local_result % base);
        additional_number = local_result / base;
    }
    if(additional_number)
        result.push_back(additional_number);
    std::reverse(result.begin(), result.end());
    return result;
}

big_integer big_integer::get_mask(unsigned int bites, int shift) {
    big_integer result("1");
    result <<= bites;
    result -= big_integer("1");
    if (shift < 0) shift = 0;
    return result << (shift << 3);
}

big_integer big_integer::first_bytes(unsigned int index) {
    big_integer result = (*this >> ((digits_count() * sizeof(unsigned int) << 3) - index - 1)) & big_integer("1");
    return result;
}

unsigned int big_integer::bytes_count(unsigned int n) {
    big_integer number(*this);
    int i = 0;
    while (!number.is_zero()) {
        number >>= 8;
        ++i;
    }
    return i;
}
#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H

#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <algorithm>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>

class big_integer final:
    allocator_guardant
{

public:

    enum class multiplication_rule
    {
        trivial,
        Karatsuba,
        SchonhageStrassen
    };

private:

    class multiplication
    {

    public:

        virtual ~multiplication() noexcept = default;

    public:

        virtual big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const = 0;

    };

    class trivial_multiplication final:
        public multiplication
    {

    public:

        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

    class Karatsuba_multiplication final:
        public multiplication
    {

    public:

        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

    class Schonhage_Strassen_multiplication final:
        public multiplication
    {

    public:

        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

public:

    enum class division_rule
    {
        trivial,
        Newton,
        BurnikelZiegler
    };

private:

    class division
    {

    public:

        virtual ~division() noexcept = default;

    public:

        virtual big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;

        virtual big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;

    };

    class trivial_division final:
        public division
    {

    public:

        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };

    class Newton_division final:
        public division
    {

    public:

        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };

    class Burnikel_Ziegler_division final:
        public division
    {

    public:

        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };

private:

    int _oldest_digit{};
    unsigned int *_other_digits{};
    allocator *_allocator{};

public:
    void copy(big_integer const &other);

    void move(big_integer &&other);

    big_integer(
        int const *digits,
        size_t digits_count,
        allocator *allocator = nullptr);

    big_integer(unsigned int const *digits, size_t digits_count, allocator *allocator);

    explicit big_integer(
        std::vector<int> const &digits,
        allocator *allocator = nullptr);

    big_integer(std::vector<unsigned int> const &digits, allocator *allocator);

    explicit big_integer(
        std::string const &value_as_string,
        size_t base = 10,
        allocator *allocator = nullptr);

public:

    ~big_integer() noexcept;

    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);

    big_integer(
        big_integer &&other) noexcept;

    big_integer &operator=(
        big_integer &&other) noexcept;

public:

    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;

public:

    bool operator<(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;

public:

    big_integer operator-() const;

    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    big_integer operator+(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer operator-(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer operator*(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer operator/(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

    big_integer operator%(
        std::pair<big_integer, allocator *> const &other) const;

public:

    big_integer operator~() const;

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer operator&(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer operator|(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer operator^(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator<<=(
        size_t shift);

    big_integer operator<<(
        size_t shift) const;

    big_integer operator<<(
        std::pair<size_t, allocator *> const &shift) const;

    big_integer &operator>>=(
        size_t shift);

    big_integer operator>>(
        size_t shift) const;

    big_integer operator>>(
        std::pair<size_t, allocator *> const &shift) const;

public:

    static big_integer &multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer multiply(
        big_integer const &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

public:

    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);

    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);

public: // must be private

    [[nodiscard]] allocator *get_allocator() const noexcept override;

    std::vector<unsigned int> convert_to_base(std::string const &value_as_string, size_t base);

    static std::vector<unsigned int> divide_str_on_int(std::string str, int position);

    static std::string string_plus(std::string const &first, std::string const &second);

    big_integer &change_sign();

    bool is_negative() const;

    static void swap(big_integer &first, big_integer &second) noexcept;

    unsigned int digits_count() const;

    unsigned int get_digit(unsigned int position) const;

    unsigned int &get_link(unsigned int position);

    static std::string multiply_string(std::string first, std::string second);

    [[nodiscard]] std::string to_string() const;

    static std::string string_plus_with_reverse(std::string first, std::string second);

    static void print_bytes(unsigned int i);

    std::vector<unsigned int> big_integer_to_vector(big_integer const &number) const;

    bool is_zero() const;

    std::vector<unsigned int> vector_x_int(std::vector<unsigned int> vec, unsigned int);

    static int greater_vectors(std::vector<unsigned int> &first, std::vector<unsigned int> &second);

    static void left_swipe(unsigned int &number, unsigned int &left_number, unsigned int &right_number);

    static void right_swipe(unsigned int &number, unsigned int &left_number, unsigned int &right_number);

    static big_integer get_mask(unsigned int bites, int shift);

    big_integer first_bytes(unsigned int n);

    unsigned int bytes_count(unsigned int n);

    std::vector<unsigned int> str_to_vectorint(std::string str, int position);
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
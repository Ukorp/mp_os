#ifndef PERSON_DATA_H
#define PERSON_DATA_H
#include <string>
#include "../../string_pool/include/string_pool.h"

class person_data {
private:
    class person_index_data {
        friend class person_data;

        size_t id;

        size_t name_id;

        size_t surname_id;

    public:
        person_index_data(size_t id, std::string const &name_id, std::string const &surname_id): id(id),
            name_id(string_pool::add_string(name_id)),
            surname_id(string_pool::add_string(surname_id)) {
        }

        person_index_data(size_t id, std::string &&name_id, std::string &&surname_id): id(id),
            name_id(string_pool::add_string(name_id)),
            surname_id(string_pool::add_string(surname_id)) {
        }

        person_index_data() = default;

        person_index_data(person_index_data const &other): id(other.id), name_id(other.name_id),
                                                           surname_id(other.surname_id) {
        }

        person_index_data &operator=(person_index_data const &other) {
            if (this != &other) {
                id = other.id;
                name_id = other.name_id;
                surname_id = other.surname_id;
            }
            return *this;
        }

        person_index_data(person_index_data &&) = delete;

        person_index_data &operator=(person_index_data &&) = delete;

        ~person_index_data() = default;
    };

    friend class person_index_data;

    person_index_data current_data{};

public:
    person_data(size_t id, std::string const &name, std::string const &surname);

    person_data(size_t id, std::string &&name, std::string &&surname);

    person_data() = default;

    person_data(person_data const &other);

    person_data(person_data &&other);

    person_data &operator=(person_data const &other);


    person_data &operator=(person_data &&other);

    ~person_data() = default;

public:
    void set_id(size_t id);

    void set_name(const std::string &name);

    void set_surname(const std::string &surname);

    void set_name(std::string &&name);

    void set_surname(std::string &&surname);

    size_t get_id() const;

    const std::string &get_name() const;

    const std::string &get_surname() const;

    size_t get_name_id() const;

    size_t get_surname_id() const;
};

#endif //PERSON_DATA_H

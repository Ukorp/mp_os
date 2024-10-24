#include "../include/person_data.h"

person_data::person_data(size_t id,
                         std::string const &name,
                         std::string const &surname): current_data(id, name, surname) {


}

person_data::person_data(size_t id,
                         std::string &&name,
                         std::string &&surname): current_data(id, std::move(name), std::move(surname)) {
}

person_data::person_data(person_data const &other): current_data(other.current_data) {
}

person_data &person_data::operator=(person_data const &other){
    if (this != &other) {
        current_data = other.current_data;
    }
    return *this;
}

person_data::person_data(person_data &&other) {
    current_data.id = other.current_data.id;
    current_data.name_id = other.current_data.name_id;
    current_data.surname_id = other.current_data.surname_id;
}

person_data &person_data::operator=(person_data &&other) {
    if (this != &other) {
        current_data = other.current_data;
    }
    return *this;
}

size_t person_data::get_id() const {
    return current_data.id;
}

const std::string &person_data::get_name() const {
    return string_pool::get_string(current_data.name_id);
}

const std::string &person_data::get_surname() const {
    return string_pool::get_string(current_data.surname_id);
}

size_t person_data::get_name_id() const {
    return current_data.name_id;
}

size_t person_data::get_surname_id() const {
    return current_data.surname_id;
}

void person_data::set_id(size_t id) {
    current_data.id = id;
}

void person_data::set_name(const std::string &name) {
    current_data.name_id = string_pool::add_string(name);
}

void person_data::set_name(std::string &&name) {
    current_data.name_id = string_pool::add_string(std::move(name));
}

void person_data::set_surname(const std::string &surname) {
    current_data.name_id = string_pool::add_string(surname);
}

void person_data::set_surname(std::string &&surname) {
    current_data.name_id = string_pool::add_string(std::move(surname));
}

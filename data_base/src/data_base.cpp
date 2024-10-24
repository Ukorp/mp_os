#include "../include/data_base.h"

#define _additional_storage "data_storage"

std::function<int(const std::string &, const std::string &)> data_base::_default_string_comparer =
        [](const std::string &a, const std::string &b) -> int { return a.compare(b); };

std::function<int(const int &a, const int &b)> data_base::_int_comparer =
        [](const int &a, const int &b) -> int { return a - b; };


data_base::data_base(std::size_t t, strategy _strategy,
                     std::string const &instance_name,
                     const std::function<int(const std::string &, const std::string &)> &keys_comparer,
                     allocator *allocator, logger *logger): _data(std::make_unique<b_tree<std::string, schemas_pool> >(
    t,
    keys_comparer,
    allocator,
    logger)) {
    this->_allocator = allocator;
    this->_logger = logger;
    this->_strategy = _strategy;

    _instance_path = std::filesystem::absolute(instance_name);
    if (std::filesystem::exists(_instance_path)) {
        return;
    }
    std::filesystem::create_directories(_instance_path);
}

data_base::data_base(): data_base(4, memory, "data_base", _default_string_comparer, nullptr, nullptr) {
}

data_base::~data_base() {
}

data_base::data_base(data_base &&other) noexcept: _data(std::move(other._data)) {
}

data_base &data_base::operator=(data_base &&other) noexcept {
    if (this != &other) {
        _data = std::move(other._data);
    }

    return *this;
}

void data_base::throw_if_invalid(std::string const &key) {
    size_t length = key.length();
    if (length > 20) {
        throw std::logic_error("Key length too big");
    }

    for (size_t i = 0; i < length; ++i) {
        if (key[i] == '#' || key[i] == '|') {
            throw std::logic_error("invalid symbol in key: " + std::string{key[i]});
        }
    }
}

void data_base::insert_schemas_pool(std::string const &pool_name, schemas_pool const &value) {
    throw_if_invalid(pool_name);
    switch (this->get_strategy()) {
        case memory:
            insert(pool_name, value);
            break;
        case file:
            add_pool_to_file(pool_name);
            break;
    }
}

void data_base::insert_schemas_pool(std::string const &pool_name, schemas_pool &&value) {
    throw_if_invalid(pool_name);
    switch (this->get_strategy()) {
        case memory:
            insert(pool_name, std::move(value));
            break;
        case file:
            add_pool_to_file(pool_name);
            break;
    }
}

void data_base::insert_schema(std::string const &pool_name, std::string const &schema_name, schema const &value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            pool.insert(schema_name, value);
        }
        break;
        case file:
            add_schema_to_file(pool_name, schema_name);
            break;
    }
}

void data_base::insert_schema(std::string const &pool_name, std::string const &schema_name, schema &&value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            pool.insert(schema_name, std::move(value));
        }
        break;
        case file:
            add_schema_to_file(pool_name, schema_name);
            break;
    }
}

void data_base::insert_table(std::string const &pool_name, std::string const &schema_name,
                             std::string const &table_name, table const &value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            schm.insert(table_name, value);
        }
        break;
        case file:
            add_table_to_file(pool_name, schema_name, table_name);
            break;
    }
}

void data_base::insert_table(std::string const &pool_name, std::string const &schema_name,
                             std::string const &table_name, table &&value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            schm.insert(table_name, std::move(value));
        }
        break;
        case file:
            add_table_to_file(pool_name, schema_name, table_name);
            break;
    }
}

void data_base::insert_data(std::string const &pool_name, std::string const &schema_name, std::string const &table_name,
                            std::string const &user_data_key, person_data &&value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    throw_if_invalid(user_data_key);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            tbl.insert(user_data_key, value);
        }
        break;
        case file:
            insert_data_to_file(pool_name, schema_name, table_name, user_data_key, std::move(value));
            break;
    }
}

void data_base::insert_data(std::string const &pool_name, std::string const &schema_name, std::string const &table_name,
                            std::string const &user_data_key, person_data const &value) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    throw_if_invalid(user_data_key);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            tbl.insert(user_data_key, value);
        }
        break;
        case file:
            insert_data_to_file(pool_name, schema_name, table_name, user_data_key, value);
            break;
    }
}

person_data data_base::obtain_data(std::string const &pool_name, std::string const &schema_name,
                                   std::string const &table_name, std::string const &user_data_key) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    throw_if_invalid(user_data_key);

    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            const auto &data = tbl.obtain(user_data_key);
            return data;
        }
        case file: {
            auto person = obtain_data_in_file(pool_name, schema_name, table_name, user_data_key);
            return person;
        }
    }
}

table &data_base::obtain_table(std::string const &pool_name, std::string const &schema_name,
                               std::string const &table_name) const {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    if (_strategy == file) {
        throw std::logic_error("table &data_base::obtain_table(...) -> not implemented for \"file\" strategy");
    }
    auto &pool = _data->obtain(pool_name);
    auto &schm = pool.obtain(schema_name);
    auto &tbl = schm.obtain(table_name);
    return tbl;
}

std::map<std::string, person_data> data_base::obtain_between_data(const std::string &pool_name,
                                                                  const std::string &schema_name,
                                                                  const std::string &table_name,
                                                                  const std::string &lower_bound,
                                                                  const std::string &upper_bound,
                                                                  bool lower_bound_inclusive,
                                                                  bool upper_bound_inclusive) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    throw_if_invalid(lower_bound);
    throw_if_invalid(upper_bound);


    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            return tbl.obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
        }
        case file:
            return obtain_between_person_in_file(pool_name, schema_name, table_name, lower_bound, upper_bound,
                                                 lower_bound_inclusive, upper_bound_inclusive);
    }
}

void data_base::update_data(std::string const &pool_name, std::string const &schema_name, std::string const &table_name,
                            std::string const &user_data_key, person_data &value) {
    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            tbl.update(user_data_key, value);
        }
        break;
        case file:
            update_person_in_file(pool_name, schema_name, table_name, user_data_key, value);
            break;
    }
}

void data_base::update_data(std::string const &pool_name, std::string const &schema_name, std::string const &table_name,
                            std::string const &user_data_key, person_data &&value) {
    switch (this->get_strategy()) {
        case memory: {
            auto &pool = _data->obtain(pool_name);
            auto &schm = pool.obtain(schema_name);
            auto &tbl = schm.obtain(table_name);
            tbl.update(user_data_key, value);
        }
        break;
        case file:
            update_person_in_file(pool_name, schema_name, table_name, user_data_key, std::move(value));
            break;
    }
}

void data_base::dispose_schemas_pool(std::string const &pool_name) {
    throw_if_invalid(pool_name);

    switch (this->get_strategy()) {
        case memory:
            _data->dispose(pool_name);
            break;
        case file:
            dispose_pool_in_file(pool_name);
            break;
    }
}

void data_base::dispose_schema(std::string const &pool_name, std::string const &schema_name) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);


    switch (this->get_strategy()) {
        case memory: {
            auto &target_pool = _data->obtain(pool_name);
            target_pool.dispose(schema_name);
        }
        break;
        case file:
            dispose_schema_in_file(pool_name, schema_name);
            break;
    }
}

void data_base::dispose_table(std::string const &pool_name, std::string const &schema_name,
                              std::string const &table_name) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);


    switch (this->get_strategy()) {
        case memory: {
            auto &target_pool = _data->obtain(pool_name);
            auto &schema = target_pool.obtain(schema_name);
            schema.dispose(table_name);
        }
        break;
        case file:
            dispose_table_in_file(pool_name, schema_name, table_name);
            break;
    }
}

void data_base::dispose_person(std::string const &pool_name, std::string const &schema_name,
                               std::string const &table_name, std::string const &person_key) {
    throw_if_invalid(pool_name);
    throw_if_invalid(schema_name);
    throw_if_invalid(table_name);
    throw_if_invalid(person_key);


    switch (this->get_strategy()) {
        case memory: {
            auto &target_pool = _data->obtain(pool_name);
            auto &schema = target_pool.obtain(schema_name);
            auto &tbl = schema.obtain(table_name);
            tbl.dispose(person_key);
        }
        break;
        case file:
            dispose_person_in_file(pool_name, schema_name, table_name, person_key);
            break;
    }
}

void data_base::update(const std::string &key, const schemas_pool &value) {
    _data->update(key, value);
}

void data_base::update(const std::string &key, schemas_pool &&value) {
    _data->update(key, std::move(value));
}

void data_base::insert(const std::string &key, const schemas_pool &value) {
    _data->insert(key, value);
}

void data_base::insert(const std::string &key, schemas_pool &&value) {
    _data->insert(key, std::move(value));
}

schemas_pool &data_base::obtain(const std::string &key) {
    return _data->obtain(key);
}

std::map<std::string, schemas_pool> data_base::obtain_between(std::string const &lower_bound,
                                                              std::string const &upper_bound,
                                                              bool lower_bound_inclusive, bool upper_bound_inclusive) {
    auto vec = _data->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
    std::map<std::string, schemas_pool> result_map;
    for (auto &elem: vec) {
        result_map.emplace(elem.key, elem.value);
    }

    return result_map;
}

void data_base::dispose(const std::string &key) {
    try {
        _data->dispose(key);
    } catch (std::exception const &e) {
        throw;
    }
}

void data_base::load_data_base_state() {
    if (get_strategy() != memory) {
        throw std::logic_error("void data_base::load_data_base_state() -> invalid strategy for this operation");
    }

    std::filesystem::path db_storage_path = _instance_path;

    if (!std::filesystem::exists(db_storage_path)) {
        throw std::runtime_error("void data_base::load_data_base_state() -> data base directory doesn't exist");
    }

    for (auto &pool_entry: std::filesystem::directory_iterator(db_storage_path)) {
        if (!pool_entry.is_directory()) {
            continue;
        }

        const auto &pool_path = pool_entry.path();
        std::cout << "Loading pool: " << pool_path << std::endl;
        schemas_pool pool;
        for (auto &schema_entry: std::filesystem::directory_iterator(pool_entry)) {
            if (!schema_entry.is_directory()) {
                continue;
            }

            const auto &schema_path = schema_entry.path();
            std::cout << "Loading schema: " << schema_path << std::endl;
            schema schm;
            for (auto &table_entry: std::filesystem::directory_iterator(schema_entry)) {
                if (!table_entry.is_regular_file()) {
                    continue;
                }

                if (table_entry.path().filename().string().find("index_") != std::string::npos) {
                    continue;
                }

                const auto &table_path = table_entry.path();
                std::cout << "Found table: " << table_path << std::endl;

                std::cout << "Path to table file: " << table_entry.path() << std::endl;

                table tbl = table::load_data_from_filesystem(table_path.string());
                auto tbl_name = table_path.filename().string();
                for (int i = 0; i < _format.length(); ++i) {
                    tbl_name.pop_back();
                }

                schm.insert(tbl_name, std::move(tbl));
            }

            pool.insert(schema_path.filename().string(), schm);
        }
        _data->insert(pool_path.filename().string(), pool);
    }
}

void data_base::save_data_base_state() {
    if (get_strategy() != memory) {
        throw std::logic_error("void data_base::save_data_base_state() -> invalid strategy for this operation");
    }
    auto it = _data->begin_infix();
    auto it_end = _data->end_infix();
    while (it != it_end) {
        auto string_key = std::get<2>(*it);
        auto target_schemas_pool = std::get<3>(*it);

        insert_pool_to_file(string_key, std::move(target_schemas_pool));
        ++it;
    }
}

std::string data_base::find_key_in_file(const std::string &key) {
    if (key.empty()) {
        throw std::logic_error("std::string data_base::find_key_in_file(...) -> invalid argument");
    }

    //TODO: think about std:::Absolute!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    auto filename = get_instance_name() + _format;
    auto index_filename = std::string{"index_"} + get_instance_name() + _format;

    std::ifstream index_file(index_filename);
    throw_if_open_fails(index_file);

    std::vector<std::streamoff> index_array;
    std::string index_str;
    while (std::getline(index_file, index_str, '#')) {
        index_array.push_back(std::stol(index_str));
    }

    index_file.close();

    std::ifstream main_file(filename);
    throw_if_open_fails(main_file);

    size_t left = 0;
    size_t right = index_array.size() - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;

        main_file.seekg(index_array[mid]);

        std::string file_key;
        std::getline(main_file, file_key, '|');

        if (key == file_key) {
            main_file.close();
            return file_key;
        }

        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    main_file.close();
    throw std::logic_error("std::string data_base::find_key_in_file(...) -> key not found");
}

void data_base::insert_pool_to_file(const std::string &pool_name, schemas_pool &&value) {
    std::filesystem::path inmem_storage_path = _instance_path;
    create_if_not_exists(inmem_storage_path);
    std::filesystem::path pool_path = inmem_storage_path / pool_name;
    create_if_not_exists(pool_path);
    auto it = value._data->begin_infix();
    auto it_end = value._data->end_infix();
    while (it != it_end) {
        auto schema_name = std::get<2>(*it);
        auto target_schema = std::get<3>(*it);

        std::filesystem::path schema_path = pool_path / schema_name;

        create_if_not_exists(schema_path);
        target_schema.insert_schema_to_file(schema_path);
        ++it;
    }
}

void data_base::insert_pool_to_file(const std::string &pool_name, const schemas_pool &value) {
    std::filesystem::path inmem_storage_path = _instance_path / (_additional_storage);
    create_if_not_exists(inmem_storage_path);
    std::filesystem::path pool_path = inmem_storage_path / pool_name;
    create_if_not_exists(pool_path);
    auto it = value._data->begin_infix();
    auto it_end = value._data->end_infix();
    while (it != it_end) {
        auto schema_name = std::get<2>(*it);
        auto target_schema = std::get<3>(*it);

        std::filesystem::path schema_path = pool_path / schema_name;

        create_if_not_exists(schema_path);
        target_schema.insert_schema_to_file(schema_path);
        ++it;
    }
}

void data_base::add_pool_to_file(const std::string &pool_name) {
    std::filesystem::path pool_path = _instance_path / pool_name;
    create_if_not_exists(pool_path);
}

void data_base::add_schema_to_file(const std::string &pool_name, const std::string &schema_name) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error("void data_base::add_schema_to_file() -> pool directory does not exist: " + pool_name);
    }
    std::filesystem::path schema_path = pool_path / schema_name;
    create_if_not_exists(schema_path);
}

void data_base::add_table_to_file(const std::string &pool_name, const std::string &schema_name,
                                  const std::string &table_name) {
    std::filesystem::path pool_path = _instance_path / pool_name;
    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::add_table_to_file() -> table directory does not exist: " + table_name);
    }
    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "void data_base::add_table_to_file() -> schema directory does not exist: " + schema_name);
    }
    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);
    if (!std::filesystem::exists(table_path)) {
        table::check_and_create_empty(table_path, index_table_path);
    }
}

void data_base::create_if_not_exists(std::filesystem::path const &pool_path) {
    if (!std::filesystem::exists(pool_path)) {
        if (!std::filesystem::create_directory(pool_path)) {
            throw std::runtime_error("Could not create schema directory: " + pool_path.string());
        }
    }
}

void data_base::insert_schema_to_file(const std::string &pool_name, const std::string &schema_name, schema &&value) {
    std::filesystem::path pool_path = _instance_path / pool_name;
    create_if_not_exists(pool_path);
    std::filesystem::path schema_path = pool_path / schema_name;
    create_if_not_exists(schema_path);
    value.insert_schema_to_file(schema_path);
}

void data_base::update_person_in_file(const std::string &pool_name, const std::string &schema_name,
                                      const std::string &table_name, const std::string &person_key,
                                      person_data &&value) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::update_person_in_file(...) - > could not find pool directory: " + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "void data_base::update_person_in_file(...) - > could not find schema directory: " + schema_name);
    }

    const auto table_path = schema_path / (table_name + _format);
    const auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path) || !std::filesystem::exists(index_table_path)) {
        throw std::runtime_error(
            "void data_base::update_person_in_file(...) - > could not obtain in undefined Table" + table_path.string());
    }

    table::update_in_file(table_path, index_table_path, person_key, std::move(value));
}

void data_base::update_person_in_file(const std::string &pool_name, const std::string &schema_name,
                                      const std::string &table_name, const std::string &person_key,
                                      person_data &value) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::update_person_in_file(...) -> could not find pool directory: " + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "void data_base::update_person_in_file(...) -> could not find schema directory: " + schema_name);
    }

    const auto table_path = schema_path / (table_name + _format);
    const auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path) || !std::filesystem::exists(index_table_path)) {
        throw std::runtime_error("Could not obtain in undefined Table" + table_path.string());
    }

    table::update_in_file(table_path, index_table_path, person_key, value);
}

void data_base::insert_data_to_file(const std::string &pool_name, const std::string &schema_name,
                                    const std::string &table_name, const std::string &user_data_key,
                                    person_data &&value) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    create_if_not_exists(pool_path);

    std::filesystem::path schema_path = pool_path / schema_name;

    create_if_not_exists(schema_path);

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    table::insert_person_to_file(table_path, index_table_path, user_data_key, std::move(value));
}

void data_base::insert_data_to_file(const std::string &pool_name, const std::string &schema_name,
                                    const std::string &table_name, const std::string &user_data_key,
                                    person_data const &value) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    create_if_not_exists(pool_path);

    std::filesystem::path schema_path = pool_path / schema_name;

    create_if_not_exists(schema_path);

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    table::insert_person_to_file(table_path, index_table_path, user_data_key, value);
}

person_data data_base::obtain_data_in_file(const std::string &pool_name, const std::string &schema_name,
                                           const std::string &table_name, const std::string &person_key) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "person_data data_base::obtain_data_in_file(...) -> could not find pool: " + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "person_data data_base::obtain_data_in_file(...) -> could not find schema: " + schema_name);
    }

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path) || !std::filesystem::exists(index_table_path)) {
        throw std::runtime_error(
            "person_data data_base::obtain_data_in_file(...) -> could not obtain in undefined Table" + table_path.
            string());
    }

    return interface::obtain_in_file(table_path, index_table_path, person_key);
}

void data_base::dispose_pool_in_file(const std::string &pool_name) {
    if (const std::filesystem::path pool_path = _instance_path / pool_name;
        std::filesystem::exists(pool_path) && std::filesystem::is_directory(pool_path)) {
        try {
            std::filesystem::remove_all(pool_path);
        } catch (const std::filesystem::filesystem_error &e) {
            throw std::runtime_error(
                "void data_base::dispose_pool_in_file(const std::string &) -> could not dispose pool directory: " +
                pool_path.string() + " Error: " + e.what());
        }
    } else {
        throw std::runtime_error(
            "void data_base::dispose_pool_in_file(const std::string &) -> could not find pool directory: " + pool_path.
            string());
    }
}

void data_base::dispose_schema_in_file(const std::string &pool_name, const std::string &schema_name) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::dispose_schema_in_file(...) -> pool directory does not exist: " + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (std::filesystem::exists(schema_path) && std::filesystem::is_directory(schema_path)) {
        try {
            std::filesystem::remove_all(schema_path);
        } catch (...) {
            throw std::runtime_error(
                "void data_base::dispose_schema_in_file(...) -> could not dispose schema directory: " + schema_name);
        }
    }
}

void data_base::dispose_table_in_file(const std::string &schemas_pool_name, const std::string &schema_name,
                                      const std::string &table_name) {
    std::filesystem::path pool_path = _instance_path / schemas_pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::dispose_table_in_file(...) -> could not find pool directory: " + schemas_pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "void data_base::dispose_table_in_file(...) -> could not find schema directory: " + schema_name);
    }

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path)) {
        throw std::runtime_error(
            "void data_base::dispose_table_in_file(...) -> Table file does not exist: " + table_path.string());
    }

    if (!std::filesystem::exists(index_table_path)) {
        throw std::runtime_error(
            "void data_base::dispose_table_in_file(...) -> index file does not exist: " + index_table_path.string());
    }

    create_backup(table_path);
    create_backup(index_table_path);
    try {
        std::filesystem::remove(table_path);
        std::filesystem::remove(index_table_path);
        delete_backup(table_path);
        delete_backup(index_table_path);
    } catch (...) {
        table::load_backup(table_path);
        table::load_backup(index_table_path);
        throw;
    }
}

void data_base::dispose_person_in_file(const std::string &pool_name, const std::string &schema_name,
                                       const std::string &table_name, const std::string &person_key) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "void data_base::dispose_person_in_file(...) -> could not find pool directory: " + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "void data_base::dispose_person_in_file(...) -> could not find schema directory: " + schema_name);
    }

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path) || !std::filesystem::exists(index_table_path)) {
        throw std::runtime_error(
            "void data_base::dispose_person_in_file(...) -> could not obtain in undefined Table" + table_path.string());
    }

    table::dispose_person_from_file(table_path, index_table_path, person_key);
}

std::map<std::string, person_data> data_base::obtain_between_person_in_file(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &table_name,
    std::string const &lower_bound,
    std::string const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive) {
    std::filesystem::path pool_path = _instance_path / pool_name;

    if (!std::filesystem::exists(pool_path) || !std::filesystem::is_directory(pool_path)) {
        throw std::runtime_error(
            "std::map<std::string, person_data> data_base::obtain_between_person_in_file(...) -> could not find pool directory: "
            + pool_name);
    }

    std::filesystem::path schema_path = pool_path / schema_name;

    if (!std::filesystem::exists(schema_path) || !std::filesystem::is_directory(schema_path)) {
        throw std::runtime_error(
            "std::map<std::string, person_data> data_base::obtain_between_person_in_file(...) -> could not find schema directory: "
            + schema_name);
    }

    auto table_path = schema_path / (table_name + _format);
    auto index_table_path = schema_path / ("index_" + table_name + _format);

    if (!std::filesystem::exists(table_path)) {
        throw std::runtime_error(
            "std::map<std::string, person_data> data_base::obtain_between_person_in_file(...) -> table file does not exist: "
            + table_path.string());
    }

    if (!std::filesystem::exists(index_table_path)) {
        throw std::runtime_error(
            "std::map<std::string, person_data> data_base::obtain_between_person_in_file(...) -> index file does not exist: "
            + index_table_path.string());
    }

    return table::obtain_between_person_in_file(table_path, index_table_path, lower_bound, upper_bound,
                                                lower_bound_inclusive, upper_bound_inclusive);
}

#include "../include/schema.h"

std::function<int(const std::string &, const std::string &)> schema::_default_string_comparer =
        [](const std::string &a, const std::string &b) -> int {
    return a.compare(b);
};

schema::schema(size_t t, strategy _strategy,
                      const std::function<int(std::string const &, std::string const &)> &keys_comparer,
                      allocator *allocator,
                      logger *logger): _data(std::make_unique<b_tree<std::string, table> >(t,
    keys_comparer,
    allocator,
    logger))
{
    this->_strategy = _strategy;
    this->_logger = logger;
    this->_allocator = allocator;
    _storage_filename = "table.txt";
}

schema::schema(): schema(4, memory, _default_string_comparer, nullptr, nullptr) {

}

schema::~schema() {
}

schema::schema(const schema &other): _data(std::make_unique<b_tree<std::string, table> >(*other._data)) {
}

schema::schema(schema &&other) noexcept: _data(std::move(other._data)) {
}

schema &schema::operator=(const schema &other) {
    if (this != &other)
    {
        if (other._data)
        {
            _data = std::make_unique<b_tree<std::string, table>>(*other._data);
        }
        else
        {
            _data.reset();
        }
    }
    return *this;
}

schema &schema::operator=(schema &&other) noexcept {
    if (this != &other)
    {
        if (other._data)
        {
            _data = std::move(other._data);
        }
        else
        {
            _data.reset();
        }
    }
    return *this;
}

void schema::insert(const std::string &key, const table &value) {
    _data->insert(key, value);
}

void schema::insert(const std::string &key, table &&value) {

    _data->insert(key, std::move(value));

}

table &schema::obtain(const std::string &key) {
    return _data->obtain(key);
}

void schema::update(const std::string &key, const table &value) {
    _data->update(key, value);
}

void schema::update(const std::string &key, table &&value) {
    _data->update(key, std::move(value));

}

std::map<std::string, table> schema::obtain_between(std::string const &lower_bound,
                                                           std::string const &upper_bound, bool lower_bound_inclusive,
                                                           bool upper_bound_inclusive) {
    auto vec = _data->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
    std::map<std::string, table> result_map;

    for (auto &item: vec)
    {
        result_map.emplace(item.key, item.value);
    }
    return result_map;
}

void schema::dispose(const std::string &key) {
    try
    {
        _data->dispose(key);
    }
    catch (std::logic_error const &e)
    {
        throw;
    }
}

void schema::set_storage_filename(std::string &filename) {
    this->_storage_filename = filename;
}

void schema::set_storage_filename(std::string &&filename) {
    this->_storage_filename = std::move(filename);
}

schema schema::load_schema_from_file(std::string const &filename) {
    schema result;

    std::ifstream input_file(filename);
    throw_if_open_fails(input_file);

    std::string line;
    while (std::getline(input_file, line))
    {
        if (line.empty() || line.back() != '|')
        {
            continue;
        }
        line.pop_back();
        std::string table_filename = line + _format;
        table new_table;
        table::load_data_from_filesystem(table_filename);
        result.insert(line, new_table);
    }

    input_file.close();
    return result;
}

void schema::save_schema_to_file(std::string const &filename) {
    std::string filename_copy = filename.empty() ? _storage_filename : filename;

    std::ofstream output_file(filename_copy);
    throw_if_open_fails(output_file);

    auto it = _data->begin_infix();
    auto it_end = _data->end_infix();
    while (it != it_end)
    {
        auto string_key = std::get<2>(*it);
        auto target_table = std::get<3>(*it);

        output_file << string_key << "|" << std::endl;

        auto table_filename = string_key + _format;
        ++it;
    }

    output_file.close();
}

void schema::insert_schema_to_file(const std::filesystem::path &path)
{
    auto schm_it = _data->begin_infix();
    auto schm_end = _data->end_infix();
    while (schm_it != schm_end)
    {
        auto table_name = std::get<2>(*schm_it);
        auto target_tbl = std::get<3>(*schm_it);

        std::filesystem::path table_file_path = path / (table_name + _format);

        target_tbl.insert_table_to_file(table_file_path);

        ++schm_it;
    }
}



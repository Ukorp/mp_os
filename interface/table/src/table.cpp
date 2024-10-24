#include "../include/table.h"

#include <sstream>

std::function<int(const std::string &, const std::string &)> table::_default_string_comparer =
        [](const std::string &a, const std::string &b) ->
    int {
    return a.compare(b);
};

table::table(size_t t, strategy _strategy,
             const std::function<int(std::string const &, std::string const &)> &keys_comparer,
             const std::string &instance_name,
             allocator *allocator,
             logger *logger): _data(std::make_unique<b_tree<std::string, person_data> >(t,
    keys_comparer,
    allocator,
    logger)) {
    set_instance_name(instance_name);
    _logger = nullptr;
    this->_strategy = _strategy;
}

table::table(): table(4,
                      strategy::file,
                      _default_string_comparer,
                      "table_name",
                      nullptr,
                      nullptr) {
}

table::table(const table &other): _data(make_unique<b_tree<std::string, person_data> >(*other._data)) {
}

table::table(table &&other) noexcept: _data(std::move(other._data)) {
    if (!_data) {
        _data.reset(nullptr);
    }
}

table &table::operator=(const table &other) {
    if (this != &other) {
        if (other._data) {
            _data = std::make_unique<b_tree<std::string, person_data> >(*other._data);
        } else {
            _data.reset(nullptr);
        }
    }
    return *this;
}

table &table::operator=(table &&other) noexcept {
    if (this != &other) {
        if (other._data) {
            _data = std::move(other._data);
        } else {
            _data.reset(nullptr);
        }
    }
    return *this;
}

table::~table() {
}

void table::insert(const std::string &key, const person_data &value) {
    std::string out_str = std::to_string(string_pool::add_string(key)) + "#"
                          + std::to_string(value.get_id()) + "#"
                          + std::to_string(value.get_name_id()) + "#"
                          + std::to_string(value.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    _data->insert(key, value);
}

void table::insert(const std::string &key, person_data &&value) {
    std::string out_str = std::to_string(string_pool::add_string(key)) + "#"
                          + std::to_string(value.get_id()) + "#"
                          + std::to_string(value.get_name_id()) + "#"
                          + std::to_string(value.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    _data->insert(key, std::move(value));
}

person_data &table::obtain(const std::string &key) {
    return _data->obtain(key);
}

std::map<std::string, person_data> table::obtain_between(std::string const &lower_bound,
                                                         std::string const &upper_bound,
                                                         bool lower_bound_inclusive,
                                                         bool upper_bound_inclusive) {
    std::map<std::string, person_data> result_map;
    auto collection = _data->obtain_between(lower_bound, upper_bound, lower_bound_inclusive,
                                            upper_bound_inclusive);
    for (auto const &elem: collection) {
        auto key = elem.key;
        auto value = elem.value;
        result_map.emplace(key, value);
    }
    return result_map;
}

void table::update(const std::string &key, const person_data &value) {
    _data->update(key, value);
}

void table::update(const std::string &key, person_data &&value) {
    _data->update(key, std::move(value));
}

void table::dispose(const std::string &key) {
    _data->dispose(key);
}

void table::insert_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                           std::string const &key, person_data const &person) {
    std::string out_str = std::to_string(string_pool::add_string(key)) + "#"
                          + std::to_string(person.get_id()) + "#"
                          + std::to_string(person.get_name_id()) + "#"
                          + std::to_string(person.get_surname_id()) + "|";
    make_same_length(out_str);

    if (check_and_create_with_insertion(path, index_path, out_str)) {
        return;
    }

    auto index_array = load_index(index_path.string());

    if (index_array.empty()) {
        std::ofstream out_f(path);
        throw_if_open_fails(out_f);
        std::ofstream index_f(index_path);
        if (!index_f.is_open()) {
            out_f.close();
            throw_if_open_fails(index_f);
        }
        out_f << out_str << std::endl;
        out_f.close();
        interface::update_index(index_array);
        interface::save_index(index_array, index_f);
        index_f.close();
        return;
    }

    std::ifstream src(path);
    throw_if_open_fails(src);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    std::string file_key;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        src.seekg(index_array[mid]);
        std::string key_index;
        std::getline(src, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(key_index));
        if (file_key == key) {
            src.close();
            throw std::logic_error("void table::insert_to_file(...) -> dublicate key: " + key);
        }

        if (right == left) {
            src.close();
            break;
        }

        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    create_backup(path);
    create_backup(index_path);

    bool is_target_greater = file_key < key;

    if (left == index_array.size() - 1 && is_target_greater) {
        try {
            std::ofstream data_file(path, std::ios::app);
            throw_if_open_fails(data_file);
            data_file << out_str << std::endl;
            data_file.close();
            update_index(index_array);
            save_index(index_array, index_path.string());
        } catch (...) {
            load_backup(path);
            load_backup(index_path);
            throw;
        }

        delete_backup(path);
        delete_backup(index_path);
        return;
    }

    try {
        std::ifstream data_file(path);
        throw_if_open_fails(data_file);
        auto tmp_filename = path.string() + "_temp" + _format;
        std::ofstream tmp_file(tmp_filename);
        if (!tmp_file.is_open()) {
            data_file.close();
            throw_if_open_fails(tmp_file);
        }

        std::string src_line;
        size_t pos;
        while (std::getline(data_file, src_line)) {
            pos = src_line.find('#');
            if (pos != std::string::npos) {
                std::string key_index = src_line.substr(0, pos);
                key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
                std::string current_key = string_pool::get_string(std::stol(src_line));
                if (current_key == file_key) {
                    if (is_target_greater) {
                        tmp_file << src_line << std::endl;
                        tmp_file << out_str << std::endl;
                    } else {
                        tmp_file << out_str << std::endl;
                        tmp_file << src_line << std::endl;
                    }

                    continue;
                }
            }

            tmp_file << src_line << std::endl;
        }

        update_index(index_array);
        save_index(index_array, index_path.string());
        data_file.close();
        tmp_file.close();

        std::filesystem::remove(path);
        std::filesystem::rename(tmp_filename, path);
    } catch (...) {
        load_backup(path);
        load_backup(index_path);
        throw;
    }

    delete_backup(path);
    delete_backup(index_path);
}

void table::dispose_from_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                              std::string const &key) {
    std::vector<std::streamoff> index_array = load_index(index_path.string());
    if (index_array.empty()) {
        throw std::logic_error("void table::dispose_from_file(...) -> empty file");
    }

    std::ifstream src(path);
    throw_if_open_fails(src);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    bool is_found = false;
    std::string file_key;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        src.seekg(index_array[mid]);
        std::string key_index;
        std::getline(src, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(key_index));

        if (key == file_key) {
            is_found = true;
            break;
        }

        if (right == left) {
            src.close();
            throw std::logic_error("void table::dispose_from_file(...) -> key not found: " + key);
        }
        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    create_backup(path);
    create_backup(index_path);

    if (is_found && index_array.size() == 1) {
        try {
            src.close();
            std::ofstream clear_file(path, std::ios::trunc);
            throw_if_open_fails(clear_file);
            clear_file.close();
            decrease_index(index_array);
            save_index(index_array, index_path.string());
            delete_backup(path);
            delete_backup(index_path);
            return;
        } catch (...) {
            load_backup(path);
            load_backup(index_path);
            throw;
        }
    }

    src.seekg(0);

    try {
        auto temp_filename = path.string() + (std::string{"temp"} + _format);

        std::ofstream tmp_file(temp_filename);
        throw_if_open_fails(tmp_file);

        std::string src_line;
        size_t pos;
        while (std::getline(src, src_line)) {
            pos = src_line.find('#');
            if (pos != std::string::npos) {
                std::string key_index = src_line.substr(0, pos);
                key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
                std::string current_key = string_pool::get_string(std::stol(key_index));
                if (current_key == file_key) {
                    continue;
                }
            }

            tmp_file << src_line << std::endl;
        }

        index_array.pop_back();
        save_index(index_array, index_path.string());
        src.close();
        tmp_file.close();

        std::filesystem::remove(path);
        std::filesystem::rename(temp_filename, path);
    } catch (...) {
        load_backup(path);
        load_backup(index_path);
        throw;
    }

    delete_backup(path);
    delete_backup(index_path);
}

void table::update_in_file(std::filesystem::path const &table_path,
                           std::filesystem::path const &index_table_path,
                           std::string const &person_key,
                           person_data &&value) {
    std::string out_str = std::to_string(string_pool::add_string(person_key)) + "#"
                          + std::to_string(value.get_id()) + "#"
                          + std::to_string(value.get_name_id()) + "#"
                          + std::to_string(value.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    make_same_length(out_str);

    dispose_person_from_file(table_path,index_table_path, person_key);
    insert_person_to_file(table_path, index_table_path, person_key, value);
}

void table::update_in_file(std::filesystem::path const &table_path,
                           std::filesystem::path const &index_table_path,
                           std::string const &person_key,
                           person_data const &value) {
    std::string out_str = std::to_string(string_pool::add_string(person_key)) + "#"
                      + std::to_string(value.get_id()) + "#"
                      + std::to_string(value.get_name_id()) + "#"
                      + std::to_string(value.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    make_same_length(out_str);

    dispose_person_from_file(table_path,index_table_path, person_key);
    insert_person_to_file(table_path, index_table_path, person_key, value);
}

std::map<std::string, person_data> table::obtain_between_person_in_file(
    std::filesystem::path const &filepath,
    std::filesystem::path const &index_filepath,
    std::string const &lower_bound,
    std::string const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive) {

    if (upper_bound < lower_bound) {
        throw std::logic_error("std::map<std::string, person_data> table::obtain_between_person_in_file(...) -> wrong interval");
    }

    std::vector<std::streamoff> index_array = load_index(index_filepath.string());

    std::ifstream data_file(filepath);
    throw_if_open_fails(data_file);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    std::string file_key;
    bool is_lower_found = false;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        std::string temp_str;
        data_file.seekg(index_array[mid]);
        std::getline(data_file, temp_str, '#');
        temp_str = std::regex_replace(temp_str, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(temp_str));
        if (lower_bound == file_key) {
            left = mid;
            is_lower_found = true;
            break;
        }
        if (right == left) {
            break;
        }
        if (file_key < lower_bound) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    if (!is_lower_found && left >= index_array.size() - 1) {
        data_file.close();
        throw std::logic_error("std::map<std::string, person_data> table::obtain_between_person_in_file(...) -> lower bound didn't find");
    }

    size_t start_index = is_lower_found && (lower_bound_inclusive) ? left : left + 1;
    if (is_lower_found) {
        if (!lower_bound_inclusive) {
            start_index = left + 1;
        } else {
            start_index = left;
        }
    } else {
        if (file_key > lower_bound) {
            start_index = left;
        } else {
            start_index = left + 1;
        }
    }

    std::map<std::string, person_data> result;

    std::string readln;
    size_t pos;
    data_file.seekg(index_array[start_index]);
    while (std::getline(data_file, readln)) {
        pos = readln.find('#');
        if (pos != std::string::npos) {
            std::string curr_key_ind = readln.substr(0, pos);
            curr_key_ind = std::regex_replace(curr_key_ind, std::regex(R"([^\d])"), "");
            std::string current_key = string_pool::get_string(std::stol(curr_key_ind));

            if (current_key >= upper_bound) {
                if (current_key == upper_bound && upper_bound_inclusive) {
                    auto data = create_user_data(readln);
                    result.emplace(upper_bound, data);
                }

                break;
            }

            auto data = create_user_data(readln);
            result.emplace(current_key, data);
        }
    }

    data_file.close();
    return result;
}

bool table::check_and_create_with_insertion(const std::filesystem::path &path,
                                            const std::filesystem::path &index_filename, const std::string &out_str) {
    if (std::filesystem::exists(path)) {
        return false;
    }

    std::ofstream new_file(path);
    if (!new_file.is_open()) {
        throw std::runtime_error("bool table::check_and_create_with_insertion(...) -> Cannot create a new file: " + path.string());
    }
    std::vector<std::streamoff> new_index_array = {0};
    new_file << out_str << std::endl;
    new_file.close();

    save_index(new_index_array, index_filename.string());

    return true;
}

bool table::check_and_create_empty(const std::filesystem::path &path, const std::filesystem::path &index_filename) {
    if (std::filesystem::exists(path)) {
        return false;
    }

    std::ofstream new_file(path, std::ios::out | std::ios::trunc);
    if (!new_file.is_open()) {
        throw std::runtime_error("bool table::check_and_create_empty(...) -> Cannot create a new file: " + path.string());
    }
    std::vector<std::streamoff> new_index_array = {};
    new_file.close();

    save_index(new_index_array, index_filename.string());

    return true;
}

void table::insert_table_to_file(const std::filesystem::path &path) const {
    std::ofstream out_file(path, std::ios::trunc | std::ios::out);
    throw_if_open_fails(out_file);
    std::vector<std::streamoff> index_vec;
    auto tbl_it = _data->begin_infix();
    auto tbl_end = _data->end_infix();
    while (tbl_it != tbl_end) {
        auto target_key = std::get<2>(*tbl_it);
        auto target_value = std::get<3>(*tbl_it);

        std::string out_str = std::to_string(string_pool::add_string(target_key)) + "#" + std::to_string(target_value.get_id()) + "#" +
                              std::to_string(target_value.get_name_id()) + "#" +
                              std::to_string(target_value.get_surname_id()) + "|";
        throw_wrong_length(out_str);
        make_same_length(out_str);
        out_file << out_str << std::endl;
        update_index(index_vec);
        ++tbl_it;
    }
    save_index(index_vec, path.parent_path() / ("index_" + path.filename().string()));
    out_file.close();
}

std::map<std::string, person_data> table::obtain_between_in_file(
    std::filesystem::path const &filepath,
    std::filesystem::path const &index_filepath,
    std::string const &lower_bound,
    std::string const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive) {
    if (upper_bound < lower_bound) {
        throw std::logic_error("std::map<std::string, person_data> table::obtain_between_in_file(...) -> wrong interval");
    }

    std::vector<std::streamoff> index_array = load_index(index_filepath.string());

    std::ifstream data_file(filepath);
    throw_if_open_fails(data_file);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    std::string file_key;
    bool is_lower_found = false;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        std::string temp_str;
        data_file.seekg(index_array[mid]);
        std::getline(data_file, temp_str, '#');
        file_key = string_pool::get_string(std::stol(temp_str));
        if (lower_bound == file_key) {
            left = mid;
            is_lower_found = true;
            break;
        }
        if (right == left) {
            break;
        }
        if (file_key < lower_bound) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    if (!is_lower_found && left >= index_array.size() - 1) {
        data_file.close();
        throw std::logic_error("std::map<std::string, person_data> table::obtain_between_in_file(...) -> left bound doesn't exist");
    }

    size_t start_index;
    if (is_lower_found) {
        if (!lower_bound_inclusive) {
            start_index = left + 1;
        } else {
            start_index = left;
        }
    } else {
        if (file_key > lower_bound) {
            start_index = left;
        } else {
            start_index = left + 1;
        }
    }

    std::map<std::string, person_data> result;

    std::string readln;
    size_t pos;
    data_file.seekg(index_array[start_index]);
    while (std::getline(data_file, readln)) {
        pos = readln.find('#');
        if (pos != std::string::npos) {
            std::string curr_key_ind = readln.substr(0, pos);
            curr_key_ind = std::regex_replace(curr_key_ind, std::regex(R"([^\d])"), "");
            std::string current_key = string_pool::get_string(std::stol(curr_key_ind));

            if (current_key >= upper_bound) {
                if (current_key == upper_bound && upper_bound_inclusive) {
                    auto data = create_user_data(readln);
                    result.emplace(upper_bound, data);
                }

                break;
            }

            auto data = create_user_data(readln);
            result.emplace(current_key, data);
        }
    }

    data_file.close();
    return result;
}

void table::update_index(std::vector<std::streamoff> &vec) {
    if (vec.empty()) {
        vec.push_back(0);
        return;
    }

    vec.push_back(vec.back() + index_item_max_length);
}

void table::save_index(std::vector<std::streamoff> const &vec, std::string const &filename) {
    std::ofstream file(filename, std::ios::trunc);
    throw_if_open_fails(file);
    size_t size = vec.size();
    file << size << "#" << std::endl;
    file.close();
}

std::vector<std::streamoff> table::load_index(const std::string &index_filename) {
    std::ifstream index_file(index_filename);
    throw_if_open_fails(index_file);

    std::vector<std::streamoff> index_array;
    std::string array_size;
    std::getline(index_file, array_size, '#');
    index_file.close();

    const size_t arr_size = std::stol(array_size);

    index_array.reserve(arr_size);
    for (long i = 0; i < arr_size; ++i) {
        index_array.push_back(i * index_item_max_length);
    }

    return index_array;
}

void table::set_storage_filename(std::string const &filename) {
    this->_storage_filename = filename;
}

void table::set_storage_filename(std::string &&filename) {
    this->_storage_filename = std::move(filename);
}

void table::print_table() const {
    auto it = _data->begin_infix();
    const auto it_end = _data->end_infix();

    while (it != it_end) {
        auto tuple = *it;
        person_data person = std::get<3>(tuple);
        std::cout << std::get<2>(tuple) << " "
                << person.get_name() << " " << person.get_surname() << " " << person.
                get_id() << " "
                << std::endl;
        ++it;
    }
}

table table::load_data_from_filesystem(std::string const &filename) {
    table src_table;

    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        return src_table;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty()) {
            break;
        }

        std::string new_line;
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] != '|') {
                new_line += line[i];
            } else {
                break;
            }
        }

        std::istringstream line_stream(new_line);
        std::string segment;
        std::vector<std::string> seg_list;

        while (std::getline(line_stream, segment, '#')) {
            seg_list.push_back(std::regex_replace(segment, std::regex(R"([^\d])"), ""));
        }

        if (seg_list.size() != 4) {
            throw std::logic_error("table table::load_data_from_filesystem(std::string const&) -> fail file reading");
        }

        auto const &string_key = string_pool::get_string(std::stol(seg_list[0]));
        auto id = std::stol(seg_list[1]);
        std::string name = seg_list[2];
        std::string surname = seg_list[3];

        src_table.insert(string_key, person_data(id,
                        string_pool::get_string(std::stol(name)),
                        string_pool::get_string(std::stol(name))));
    }

    input_file.close();
    return src_table;
}

void table::save_data_to_filesystem(std::string const &filename) {
    std::string filename_copy = filename.empty()
                                    ? get_instance_name()
                                    : filename;

    auto index_filename = "index_" + filename_copy + _format;

    std::ofstream index_file(index_filename);
    throw_if_open_fails(index_file);

    std::string src_filename = filename_copy + _format;
    std::ofstream output_file(src_filename);
    throw_if_open_fails(output_file);

    auto it = _data->begin_infix();
    auto it_end = _data->end_infix();

    size_t counter = 0;
    while (it != it_end) {
        auto string_key = std::get<2>(*it);
        auto user_data_value = std::get<3>(*it);

        auto serialized_line = string_key + "#" + std::to_string(user_data_value.get_id()) + "#" + user_data_value.
                               get_name() + "#" + user_data_value.get_surname() + "|";

        make_same_length(serialized_line);
        output_file << serialized_line << std::endl;
        ++it;
        ++counter;
    }
    index_file << counter << "#" << std::endl;
    index_file.close();
    output_file.close();
}

void table::copy_file(const std::string &source_path, const std::string &dest_path) {
    std::ifstream source_file(source_path, std::ios::binary);
    if (!source_file.is_open()) {
        throw std::runtime_error("void table::copy_file(const std::string &, const std::string &) -> open file error: " + source_path);
    }

    std::ofstream dest_file(dest_path, std::ios::binary);
    if (!dest_file.is_open()) {
        source_file.close();
        throw std::runtime_error("void table::copy_file(const std::string &, const std::string &) -> open file error: " + source_path);
    }

    dest_file << source_file.rdbuf();

    if (source_file.bad()) {
        throw std::runtime_error("void table::copy_file(const std::string &, const std::string &) -> reading file error: " + source_path);
    }

    if (dest_file.bad()) {
        throw std::runtime_error("void table::copy_file(const std::string &, const std::string &) -> reading file error: " + source_path);

    }

    source_file.close();
    dest_file.close();
}

person_data table::create_user_data(const std::string &line) {
    if (line.empty()) {
        throw std::logic_error("person_data table::create_user_data(const std::string&) -> empty string");
    }

    std::istringstream iss(line);
    std::string key, id_str, name, surname;

    std::getline(iss, key, '#');

    std::getline(iss, id_str, '#');
    std::getline(iss, name, '#');
    std::getline(iss, surname, '|');

    const size_t id = std::stoul(id_str);

    person_data person(id, string_pool::get_string(std::stol(name)), string_pool::get_string(std::stol(surname)));

    return person;
}

void table::load_backup(const std::filesystem::path &source_path) {
    std::filesystem::path backup_path = source_path;
    backup_path += ".backup";

    if (!std::filesystem::exists(backup_path)) {
        throw std::runtime_error("Load backup file failed. Backup file does not exist: " + backup_path.string());
    }

    try {
        std::filesystem::path temp_backup_path = backup_path;
        temp_backup_path += ".tmp";
        std::filesystem::copy(backup_path, temp_backup_path);

        if (std::filesystem::exists(source_path)) {
            std::filesystem::remove(source_path);
        }

        std::filesystem::rename(temp_backup_path, source_path);
    } catch (...) {
        if (std::filesystem::exists(source_path)) {
            std::filesystem::remove(source_path);
        }
        std::filesystem::path temp_backup_path = backup_path;
        temp_backup_path += ".tmp";
        if (std::filesystem::exists(temp_backup_path)) {
            std::filesystem::rename(temp_backup_path, source_path);
        }
        throw;
    }

    std::filesystem::path temp_backup_path = backup_path;
    temp_backup_path += ".tmp";
    if (std::filesystem::exists(temp_backup_path)) {
        std::filesystem::remove(temp_backup_path);
    }
}

void table::insert_person_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                                  std::string const &key, person_data const &person) {
    std::string out_str = std::to_string(string_pool::add_string(key)) + "#" + std::to_string(person.get_id()) + "#" +
                          std::to_string(person.get_name_id()) + "#" +
                          std::to_string(person.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    make_same_length(out_str);


    if (check_and_create_with_insertion(path, index_path, out_str)) {
        return;
    }

    auto index_array = load_index(index_path.string());

    if (index_array.empty()) {
        std::ofstream out_f(path);
        throw_if_open_fails(out_f);
        std::ofstream index_f(index_path);
        if (!index_f.is_open()) {
            out_f.close();
            throw_if_open_fails(index_f);
        }
        out_f << out_str << std::endl;
        out_f.close();
        update_index(index_array);
        interface::save_index(index_array, index_f);
        index_f.close();
        return;
    }

    std::ifstream src(path);
    throw_if_open_fails(src);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    std::string file_key;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        src.seekg(index_array[mid]);
        std::string key_index;
        std::getline(src, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(key_index));
        if (file_key == key) {
            src.close();
            throw std::logic_error("void table::insert_person_to_file(...) -> key duplicate: " + key);
        }

        if (right == left) {
            src.close();
            break;
        }

        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    create_backup(path);
    create_backup(index_path);

    bool is_target_greater = file_key < key;

    if (left == index_array.size() - 1 && is_target_greater) {
        try {
            std::ofstream data_file(path, std::ios::app);
            throw_if_open_fails(data_file);
            data_file << out_str << std::endl;
            data_file.close();
            update_index(index_array);
            save_index(index_array, index_path.string());
        } catch (...) {
            load_backup(path);
            load_backup(index_path);
            throw;
        }

        delete_backup(path);
        delete_backup(index_path);
        return;
    }

    try {
        std::ifstream data_file(path);
        throw_if_open_fails(data_file);
        auto tmp_filename = path.string() + "_temp" + _format;
        std::ofstream tmp_file(tmp_filename);
        if (!tmp_file.is_open()) {
            data_file.close();
            throw_if_open_fails(tmp_file);
        }

        std::string src_line;
        size_t pos;
        while (std::getline(data_file, src_line)) {
            pos = src_line.find('#');
            if (pos != std::string::npos) {
                std::string key_index = src_line.substr(0, pos);
                key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
                std::string current_key = string_pool::get_string(std::stol(key_index));
                if (current_key == file_key) {
                    if (is_target_greater) {
                        tmp_file << src_line << std::endl;
                        tmp_file << out_str << std::endl;
                    } else {
                        tmp_file << out_str << std::endl;
                        tmp_file << src_line << std::endl;
                    }

                    continue;
                }
            }

            tmp_file << src_line << std::endl;
        }

        update_index(index_array);
        save_index(index_array, index_path.string());
        data_file.close();
        tmp_file.close();

        std::filesystem::remove(path);
        std::filesystem::rename(tmp_filename, path);
    } catch (...) {
        load_backup(path);
        load_backup(index_path);
        throw;
    }

    delete_backup(path);
    delete_backup(index_path);
}

void table::insert_person_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                                  std::string const &key, person_data const &&person) {
    std::string out_str = std::to_string(string_pool::add_string(key)) + "#" + std::to_string(person.get_id()) + "#" +
                          std::to_string(person.get_name_id()) + "#" +
                          std::to_string(person.get_surname_id()) + "|";
    throw_wrong_length(out_str);
    make_same_length(out_str);


    if (check_and_create_with_insertion(path, index_path, out_str)) {
        return;
    }

    auto index_array = load_index(index_path.string());

    if (index_array.empty()) {
        std::ofstream out_f(path);
        throw_if_open_fails(out_f);
        std::ofstream index_f(index_path);
        if (!index_f.is_open()) {
            out_f.close();
            throw_if_open_fails(index_f);
        }
        out_f << out_str << std::endl;
        out_f.close();
        update_index(index_array);
        interface::save_index(index_array, index_f);
        index_f.close();
        return;
    }

    std::ifstream src(path);
    throw_if_open_fails(src);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    std::string file_key;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        src.seekg(index_array[mid]);
        std::string key_index;
        std::getline(src, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(key_index));
        if (file_key == key) {
            src.close();
            throw std::logic_error("void table::insert_person_to_file(...&&) -> duplicate key: " + key);
        }

        if (right == left) {
            src.close();
            break;
        }

        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    create_backup(path);
    create_backup(index_path);

    bool is_target_greater = file_key < key;

    if (left == index_array.size() - 1 && is_target_greater) {
        try {
            std::ofstream data_file(path, std::ios::app);
            throw_if_open_fails(data_file);
            data_file << out_str << std::endl;
            data_file.close();
            update_index(index_array);
            save_index(index_array, index_path.string());
        } catch (...) {
            load_backup(path);
            load_backup(index_path);
            throw;
        }

        delete_backup(path);
        delete_backup(index_path);
        return;
    }

    try {
        std::ifstream data_file(path);
        throw_if_open_fails(data_file);
        auto tmp_filename = path.string() + "_temp" + _format;
        std::ofstream tmp_file(tmp_filename);
        if (!tmp_file.is_open()) {
            data_file.close();
            throw_if_open_fails(tmp_file);
        }

        std::string src_line;
        size_t pos;
        while (std::getline(data_file, src_line)) {
            pos = src_line.find('#');
            if (pos != std::string::npos) {
                std::string key_index = src_line.substr(0, pos);
                key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
                std::string current_key = string_pool::get_string(std::stol(key_index));
                if (current_key == file_key) {
                    if (is_target_greater) {
                        tmp_file << src_line << std::endl;
                        tmp_file << out_str << std::endl;
                    } else {
                        tmp_file << out_str << std::endl;
                        tmp_file << src_line << std::endl;
                    }

                    continue;
                }
            }

            tmp_file << src_line << std::endl;
        }

        update_index(index_array);
        save_index(index_array, index_path.string());
        data_file.close();
        tmp_file.close();

        std::filesystem::remove(path);
        std::filesystem::rename(tmp_filename, path);
    } catch (...) {
        load_backup(path);
        load_backup(index_path);
        throw;
    }

    delete_backup(path);
    delete_backup(index_path);
}

void table::dispose_person_from_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                                       std::string const &key) {
    std::vector<std::streamoff> index_array = load_index(index_path.string());
    if (index_array.empty()) {
        throw std::logic_error("void table::dispose_person_from_file(...) -> index array is empty");
    }

    std::ifstream src(path);
    throw_if_open_fails(src);

    size_t left = 0;
    size_t right = index_array.size() - 1;
    bool is_found = false;
    std::string file_key;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        src.seekg(index_array[mid]);
        std::string key_index;
        std::getline(src, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        file_key = string_pool::get_string(std::stol(key_index));


        if (key == file_key) {
            is_found = true;
            break;
        }

        if (right == left) {
            src.close();
            throw std::logic_error("void table::dispose_person_from_file(...) -> key not found: " + key);
        }
        if (file_key < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    create_backup(path);
    create_backup(index_path);

    if (is_found && index_array.size() == 1) {
        try {
            src.close();
            std::ofstream clear_file(path, std::ios::trunc);
            throw_if_open_fails(clear_file);
            clear_file.close();
            decrease_index(index_array);
            save_index(index_array, index_path.string());
            delete_backup(path);
            delete_backup(index_path);
            return;
        } catch (...) {
            load_backup(path);
            load_backup(index_path);
            throw;
        }
    }

    src.seekg(0);

    try {
        auto temp_filename = path.string() + (std::string{"temp"} + _format);

        std::ofstream tmp_file(temp_filename);
        throw_if_open_fails(tmp_file);

        std::string src_line;
        size_t pos;
        while (std::getline(src, src_line)) {
            pos = src_line.find('#');
            if (pos != std::string::npos) {
                std::string key_index = src_line.substr(0, pos);
                key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
                std::string current_key = string_pool::get_string(std::stol(key_index));
                if (current_key == file_key) {
                    continue;
                }
            }

            tmp_file << src_line << std::endl;
        }

        index_array.pop_back();
        save_index(index_array, index_path.string());
        src.close();
        tmp_file.close();

        std::filesystem::remove(path);
        std::filesystem::rename(temp_filename, path);
    } catch (...) {
        load_backup(path);
        load_backup(index_path);
        throw;
    }

    delete_backup(path);
    delete_backup(index_path);
}

#include "../include/string_pool.h"
#include <vector>

int string_pool::_pool_size = 0;

string_pool *string_pool::_instance;

void string_pool::load_data_from_file() {
    _file.open(std::filesystem::absolute(_storage_filename), std::ios::in);
    if (!_file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + _storage_filename);
    }

    std::string line;
    int index = 0;
    while (std::getline(_file, line)) {
        size_t pos1 = line.find('#');
        if (pos1 == std::string::npos) {
            break;
        }

        std::string key = line.substr(0, pos1);

        std::string rest = line.substr(pos1 + 1);

        size_t pos2 = rest.find('#');
        if (pos2 == std::string::npos) {
            continue;
        }
        std::string value = rest.substr(0, pos2);

        _pool.emplace(std::stol(key), std::make_shared<std::string>(value));
        ++index;
    }
    _pool_size = index;
    _file.close();
}

void string_pool::save_to_file() {
    _file.seekg(0);

    if (!_file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + _storage_filename);
    }

    for (const auto &pair: _pool) {
        _file << (pair).second.get() << '\n';
    }
}

std::pair<bool, std::string> string_pool::obtain_in_file(size_t index) {
    std::string result_str;
    size_t ind = 0;
    _file.open(std::filesystem::absolute(_storage_filename), std::ios::in);
    if (!_file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + _storage_filename);
    }
    std::string line;
    bool is_found = false;
    while (std::getline(_file, line)) {
        ++ind;
        std::istringstream line_stream(line);
        std::string segment;
        std::vector<std::string> seg_list;

        while (std::getline(line_stream, segment, '#')) {
            seg_list.push_back(segment);
        }

        if (seg_list.size() != 2) {
            throw std::runtime_error("string pool file failed");
        }

        std::string file_index = seg_list[0];
        std::string file_string = seg_list[1];

        ind = std::stol(file_index);

        if (ind == index) {
            result_str = file_string;
            is_found = true;
            break;
        }
    }

    _file.close();
    return std::make_pair(is_found, result_str);
}

std::pair<bool, size_t> string_pool::obtain_in_file(const std::string &str) {
    size_t result_ind = -1;
    _file.open(std::filesystem::absolute(_storage_filename), std::ios::in);
    if (!_file.is_open()) {
        throw std::runtime_error(
            "std::pair<bool, size_t> string_pool::obtain_in_file(const std::string &str) -> open file fails " +
            _storage_filename);
    }

    std::string line;
    bool is_found = false;
    while (std::getline(_file, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream line_stream(line);
        std::string segment;
        std::vector<std::string> seg_list;

        while (std::getline(line_stream, segment, '#')) {
            seg_list.push_back(segment);
        }
        std::string file_index = seg_list[0];
        std::string file_string = seg_list[1];

        auto ind = std::stol(file_index);

        if (file_string == str) {
            result_ind = ind;
            is_found = true;
            break;
        }
    }

    _file.close();
    return std::make_pair(is_found, result_ind);
}

string_pool::string_pool() {
    auto path = std::filesystem::absolute(_storage_filename);

    if (!exists(path)) {
        std::ofstream ofs(path);
        if (!ofs.is_open()) {
            throw std::runtime_error("string_pool::string_pool() -> file open error: " + _storage_filename);
        }
        _pool_size = 0;
        ofs.close();
    }
    load_data_from_file();
}

string_pool::~string_pool() {
    if (_file.is_open()) {
        _file.close();
    }
    delete _instance;
}

string_pool *string_pool::get_instance() {
    if (!_instance) {
        _instance = new string_pool();
    }

    return _instance;
}

const std::string &string_pool::get_string(size_t index, bool get_string_by_index) {
    auto instance = get_instance();

    auto target = instance->obtain_in_file(index);
    if (target.first) {
        auto it = instance->_pool.find(index);
        if (it != instance->_pool.end()) {
            return *(*it).second;
        }
        auto sh_ptr = std::make_shared<std::string>(target.second);

        instance->_pool.emplace(index, sh_ptr);

        return *sh_ptr;
    }

    static const std::string empty_string;
    return empty_string;
}

size_t string_pool::add_string(const std::string &str, bool get_string_by_index) {
    auto inst = get_instance();
    std::string current_str;
    if (get_string_by_index) {
        current_str = get_string(std::stol(str));
        if (current_str.empty()) {
            current_str = str;
        }
    } else {
        current_str = str;
    }

    auto find_result = inst->obtain_in_file(current_str);
    if (find_result.first) {
        return find_result.second;
    }

    auto out_str = std::to_string(_pool_size) + "#" + current_str + "#" + '\n';

    inst->_file.open(std::filesystem::absolute(inst->_storage_filename), std::ios::app);
    if (!inst->_file.is_open()) {
        throw std::runtime_error(
            "size_t string_pool::add_string(const std::string&, bool): file open error " + inst->_storage_filename);
    }

    inst->_pool.emplace(_pool_size, std::make_shared<std::string>(str));
    inst->_file << out_str;

    inst->_file.close();
    return _pool_size++;
}

size_t string_pool::add_string(std::string &&str, bool get_string_by_index) {
    auto inst = get_instance();
    std::string current_str;
    if (get_string_by_index) {
        current_str = get_string(std::stol(str));
        if (current_str.empty()) {
            current_str = str;
        }
    } else {
        current_str = str;
    }

    auto find_result = inst->obtain_in_file(current_str);
    if (find_result.first) {
        return find_result.second;
    }

    auto out_str = std::to_string(_pool_size) + "#" + current_str + "#" + '\n';

    inst->_file.open(std::filesystem::absolute(inst->_storage_filename), std::ios::app);
    if (!inst->_file.is_open()) {
        throw std::runtime_error(
            "size_t string_pool::add_string(std::string &&, bool) -> file open error " + inst->_storage_filename);
    }

    inst->_pool.emplace(_pool_size, std::make_shared<std::string>(str));
    inst->_file << out_str;

    inst->_file.close();
    return _pool_size++;
}

std::string string_pool::find_in_file(size_t index) {
    _file.seekg(0);

    std::string line;
    bool is_found = false;
    while (std::getline(_file, line)) {
        size_t pos = line.find('#');
        std::string file_ind_str = line.substr(0, pos);
        size_t file_index = std::stol(file_ind_str);

        if (file_index == index) {
            is_found = true;
        }
    }

    return is_found ? line : "";
}

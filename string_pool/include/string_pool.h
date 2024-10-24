#ifndef STRING_POOL_H
#define STRING_POOL_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

class string_pool final {
private:
    std::unordered_map<int, std::shared_ptr<std::string> > _pool;

    static int _pool_size;

    static string_pool *_instance;

    std::string _storage_filename = "string_pool.txt";

    std::fstream _file;

    void load_data_from_file();

    void save_to_file();

    std::pair<bool, std::string> obtain_in_file(size_t index);

    std::pair<bool, size_t> obtain_in_file(const std::string &str);

private:
    string_pool();

public:
    ~string_pool();

public:
    static string_pool *get_instance();

    string_pool(const string_pool &) = delete;

    string_pool(string_pool &&) = delete;

    string_pool &operator=(const string_pool &) = delete;

    string_pool &operator=(string_pool &&) = delete;

public:
    static const std::string &get_string(size_t index, bool get_string_by_index = false);

    static size_t add_string(const std::string &str, bool get_string_by_index = false);

    static size_t add_string(std::string &&str, bool get_string_by_index = false);

private:
    std::string find_in_file(size_t index);
};

#endif //STRING_POOL_H

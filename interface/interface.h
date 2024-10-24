#ifndef INTERFACE_H
#define INTERFACE_H
#include <fstream>
#include <map>
#include <vector>
#include <filesystem>
#include <regex>

#include "../string_pool/include/string_pool.h"
#include "../allocator/include/allocator_guardant.h"
#include "../logger/logger/include/logger_guardant.h"
#include "../person_data/include/person_data.h"



template <
    typename tkey,
    typename tvalue>
class interface: public logger_guardant,
                 public allocator_guardant
{

public:

    enum strategy {
        memory,
        file
    };

protected:

    static constexpr size_t max_data_length = 50;

    static constexpr size_t index_item_max_length = max_data_length + 2;

    static void throw_wrong_length(std::string const &data);

    static void make_same_length(std::string &data);


public:

    [[nodiscard]] allocator *get_allocator() const override;

    [[nodiscard]] logger *get_logger() const override;

    [[nodiscard]] strategy get_strategy() const;

    std::string get_instance_name();

    void set_instance_name(std::string const &name);

protected:
    allocator * _allocator;

    logger * _logger;

protected:

    strategy _strategy;

    inline static std::string _format = ".txt";

    std::string _instance_name;

protected:
    void set_strategy(strategy Strategy);

    static void throw_if_open_fails(std::ifstream const &file);

    static void throw_if_open_fails(std::ofstream const &file);

    static void decrease_index(std::vector<std::streamoff> &vec);

    static int find_index(std::ifstream &src, std::vector<std::streamoff> const &vec, std::string const &key);

public:

    virtual void insert(const tkey &key, const tvalue &value) = 0;

    virtual void insert(const tkey &key, tvalue &&value) = 0;

    virtual tvalue &obtain(const tkey &key) = 0;

    virtual std::map<tkey, tvalue> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) = 0;

    virtual void update(const tkey &key, const tvalue &value) = 0;

    virtual void update(const tkey &key, tvalue &&value) = 0;

    virtual void dispose(const tkey &key) = 0;

    ~interface() override = default;

protected:

    static std::vector<std::streamoff> load_index(const std::string &index_filename);

    static void save_index(const std::vector<std::streamoff> &vec, const std::string &filename);

    static void update_index(std::vector<std::streamoff> &vec);

    static void save_index(std::vector<std::streamoff> const &vec, std::ofstream &file);

    static person_data obtain_in_file(const std::filesystem::path &data_path, const std::filesystem::path &index_path,
                                       const std::string &key);

    static void delete_backup(const std::filesystem::path &source_path);

    static void create_backup(const std::filesystem::path &source_path);
};


template <
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::throw_wrong_length(std::string const &data)
{
    if (data.length() > max_data_length) {
        throw std::logic_error("Data error: data length is too long");
    }
}

template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::make_same_length(std::string &data) {
    throw_wrong_length(data);
    const unsigned int len = data.length();
    std::string tmp;

    for (unsigned int i = len; i < index_item_max_length; ++i) {
        tmp += "_";
    }
    data = tmp + data;
}

template<
    typename tkey,
    typename tvalue>
allocator * interface<tkey, tvalue>::get_allocator() const
{
    return _allocator;
}

template<
    typename tkey,
    typename tvalue>
logger * interface<tkey, tvalue>::get_logger() const
{
    return _logger;
}

template<
    typename tkey,
    typename tvalue>
typename interface<tkey, tvalue>::strategy interface<tkey, tvalue>::get_strategy() const
{
    return _strategy;
}

template<
    typename tkey,
    typename tvalue>
std::string interface<tkey, tvalue>::get_instance_name()
{
    return _instance_name;
}

template<typename tkey, typename tvalue>
void interface<tkey, tvalue>::set_instance_name(std::string const &name)
{
    _instance_name = name;
}

template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::set_strategy(strategy Strategy)
{
    _strategy = Strategy;
}

template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::throw_if_open_fails(std::ifstream const &file)
{
    if (!file.is_open())
    {
        throw std::runtime_error("Open file error: file doesn't open");
    }
}


template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::throw_if_open_fails(std::ofstream const &file)
{
    if (!file.is_open())
    {
        throw std::runtime_error("Open file error: file doesn't open");
    }
}

template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::decrease_index(std::vector<std::streamoff> &vec)
{
    if (vec.empty())
    {
        return;
    }
    vec.pop_back();
}

template<
    typename tkey,
    typename tvalue>
int interface<tkey, tvalue>::find_index(std::ifstream &src,
                                        std::vector<std::streamoff> const &vec,
                                        std::string const &key)
{
    throw_if_open_fails(src);
    size_t left = 0;
    size_t right = vec.size() - 1;
    std::string file_key;
    while (left < right)
    {
        const size_t mid = left + (right - left) / 2;
        src.seekg(vec[mid]);
        std::getline(src, file_key, '#');
        if (key == file_key)
        {
            return static_cast<int>(mid);
        }
        if (right == left)
        {
            break;
        }
        if (file_key < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
template<
    typename tkey,
    typename tvalue>
std::vector<std::streamoff> interface<tkey, tvalue>::load_index(const std::string &index_filename)
{
    {
        std::ifstream index_file(index_filename);
        throw_if_open_fails(index_file);

        std::vector<std::streamoff> index_array;
        std::string array_size;
        std::getline(index_file, array_size, '#');
        index_file.close();

        const size_t arr_size = std::stol(array_size);

        for (long long i = 0; i < arr_size; ++i)
        {
            index_array.push_back(i * index_item_max_length);
        }

        return index_array;
    }
}

template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::save_index(std::vector<std::streamoff> const &vec,
                                                 std::string const &filename)
{
    std::ofstream file(filename, std::ios::trunc);
    throw_if_open_fails(file);

    size_t size = vec.size();
    file << size << "#" << std::endl;

    file.close();
}


template<
    typename tkey,
    typename tvalue>
void interface<tkey, tvalue>::update_index(std::vector<std::streamoff> &vec) {
    if (vec.empty())
    {
        vec.push_back(0);
        return;
    }
    vec.push_back(vec.back() + index_item_max_length);
}

template<typename tkey, typename tvalue>
void interface<tkey, tvalue>::save_index(std::vector<std::streamoff> const &vec, std::ofstream &file)
{
    throw_if_open_fails(file);

    size_t size = vec.size();
    file << size << "#" << std::endl;

}

template<typename tkey, typename tvalue>
person_data interface<tkey, tvalue>::obtain_in_file(const std::filesystem::path &data_path, const std::filesystem::path &index_path, const std::string &key) {

    std::vector<std::streamoff> index_array = load_index(index_path.string());

    if (index_array.empty())
    {
        throw std::logic_error("Target file is empty :((");
    }

    std::ifstream data_file(data_path);
    throw_if_open_fails(data_file);

    size_t left = 0;
    size_t right = index_array.size() - 1;

    while (left <= right)
    {
        size_t mid = left + (right - left) / 2;

        data_file.seekg(index_array[mid]);

        std::string key_index;
        std::getline(data_file, key_index, '#');
        key_index = std::regex_replace(key_index, std::regex(R"([^\d])"), "");
        std::string file_key = string_pool::get_string(std::stol(key_index));
        if (key == file_key)
        {
            std::string user_info;
            std::getline(data_file, user_info, '|');
            std::istringstream iss(user_info);
            std::string id_str, name_ind, surname_ind;

            std::getline(iss, id_str, '#');
            std::getline(iss, name_ind, '#');
            std::getline(iss, surname_ind, '#');

            std::string name_value = string_pool::get_string(std::stol(name_ind));
            std::string surname_value = string_pool::get_string(std::stol(surname_ind));
            size_t id = std::stoul(id_str);

            return {id, name_value, surname_value};
        }

        if (right == left)
        {
            break;
        }
        if (file_key < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }

    data_file.close();
    throw std::logic_error("key not found");
}

template <typename tkey, typename tvalue>
void interface<tkey, tvalue>::delete_backup(const std::filesystem::path &source_path)
{
    std::filesystem::path backup_path = source_path;
    backup_path += ".backup";

    if (std::filesystem::exists(backup_path))
    {
        try
        {
            std::filesystem::remove(backup_path);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            throw std::runtime_error("Failed to delete backup file: " + backup_path.string() + ". Error: " + e.what());
        }
    }
    else
    {
        throw std::runtime_error("Backup file does not exist: " + backup_path.string());
    }
}

template <typename tkey, typename tvalue>
void interface<tkey, tvalue>::create_backup(const std::filesystem::path &source_path)
{
    if (!std::filesystem::exists(source_path))
    {
        throw std::runtime_error("Source file does not exist: " + source_path.string());
    }

    std::filesystem::path backup_path = source_path;
    backup_path += ".backup";

    std::ifstream src_orig(source_path, std::ios::binary);
    throw_if_open_fails(src_orig);

    std::ofstream backup_file(backup_path, std::ios::trunc | std::ios::binary);
    if (!backup_file.is_open())
    {
        src_orig.close();
        throw std::runtime_error("Failed to create backup file: " + backup_path.string());
    }

    backup_file << src_orig.rdbuf();

    src_orig.close();
    backup_file.close();
}


#endif //INTERFACE_H

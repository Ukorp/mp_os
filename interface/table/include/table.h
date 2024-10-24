#ifndef TABLE_H
#define TABLE_H
#include "../../interface.h"
#include "../../../b_tree/b_tree.h"
#include "../../../person_data/include/person_data.h"
#include "../../../string_pool/include/string_pool.h"
#include <filesystem>
#include <memory>
#include <regex>


class table: public interface<std::string, person_data>
{

    friend class schema;

    friend class data_base;

private:
    std::string _storage_filename;
    std::unique_ptr<b_tree<std::string, person_data>> _data;

public:
    static std::function<int(const std::string &, const std::string &)> _default_string_comparer;

    explicit table(size_t t,
                   strategy _strategy = strategy::memory,
                   const std::function<int(std::string const &, std::string const &)>& keys_comparer = _default_string_comparer,
                   const std::string& instance_name = "table_name",
                   allocator *allocator = nullptr,
                   logger *logger = nullptr);

public:

    table();

    table(const table& other);

    table(table&& other) noexcept;

    table& operator=(const table& other);

    table& operator=(table&& other) noexcept;

    ~table() override;

public:

    void insert(const std::string &key, const person_data &value) override;

    void insert(const std::string &key, person_data &&value) override;

    person_data &obtain(const std::string &key) override;

    std::map<std::string, person_data> obtain_between(
        std::string const &lower_bound,
        std::string const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

    void update(const std::string &key, const person_data &value) override;

    void update(const std::string &key, person_data &&value) override;

    void dispose(const std::string &key) override;

private:

    static void insert_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path, const std::string &key, const person_data &
                        ud);

    static void dispose_from_file(std::filesystem::path const &path, std::filesystem::path const &index_path, const std::string &key);

    static void update_in_file(std::filesystem::path const &table_path, std::filesystem::path const &index_table_path, const std::string &
                        user_data_key, person_data &&value);

    static void update_in_file(std::filesystem::path const &table_path, std::filesystem::path const &index_table_path,
                        std::string const &person_key, person_data const &value);

    static std::map<std::string, person_data> obtain_between_person_in_file(std::filesystem::path const &filepath,
                                                                            std::filesystem::path const &index_filepath,
                                                                            std::string const &lower_bound,
                                                                            std::string const &upper_bound,
                                                                            bool lower_bound_inclusive,
                                                                            bool upper_bound_inclusive);


    static void update_index(std::vector<std::streamoff> &vec);

    static void save_index(std::vector<std::streamoff> const &vec, std::string const &filename);

    static std::vector<std::streamoff> load_index(const std::string &index_filename);

public:

    void set_storage_filename(std::string const &filename);

    void set_storage_filename(std::string &&filename);

    void print_table() const;

    static table load_data_from_filesystem(std::string const &filename = "");

    void save_data_to_filesystem(std::string const &filename = "");

    static void copy_file(const std::string &source_path, const std::string &dest_path);

    static person_data create_user_data(const std::string &line);

    static void load_backup(const std::filesystem::path &source_path);

    static void insert_person_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                               std::string const &key, person_data const &person);

    static void insert_person_to_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                               std::string const &key, person_data const &&person);

    static void dispose_person_from_file(std::filesystem::path const &path, std::filesystem::path const &index_path,
                                  std::string const &key);


private:

    static bool check_and_create_with_insertion(const std::filesystem::path &path, const std::filesystem::path &index_filename, const std::string &out_str);

    static bool check_and_create_empty(const std::filesystem::path &path, const std::filesystem::path &index_filename);

    void insert_table_to_file(std::filesystem::path const &path) const;

    static std::map<std::string, person_data> obtain_between_in_file(
                                                                std::filesystem::path const &filepath,
                                                                std::filesystem::path const &index_filepath,
                                                                std::string const &lower_bound,
                                                                std::string const &upper_bound,
                                                                bool lower_bound_inclusive,
                                                                bool upper_bound_inclusive);


};


#endif //TABLE_H

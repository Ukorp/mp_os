#ifndef SCHEMA_H
#define SCHEMA_H

#include "../../interface.h"
#include "../../table/include/table.h"

class schema :
    public interface<std::string, table>
{

private:

    friend class table;

    static std::function<int(const std::string& , const std::string&)> _default_string_comparer;

    std::string _storage_filename;

    std::unique_ptr<b_tree<std::string, table>> _data;

public:
    explicit schema(size_t t, strategy _strategy = memory,
           const std::function<int(std::string const &, std::string const &)> &keys_comparer = _default_string_comparer,
           allocator *allocator = nullptr, logger *logger = nullptr);

    schema();

    ~schema() override;

    schema(const schema& other);

    schema(schema&& other) noexcept;

    schema& operator=(const schema& other);

    schema& operator=(schema&& other) noexcept;

public:

    void insert(const std::string &key, const table &value) override;

    void insert(const std::string &key, table &&value) override;

    table &obtain(const std::string &key) override;

    void update(const std::string &key, const table &value) override;

    void update(const std::string &key, table &&value) override;

    std::map<std::string, table> obtain_between(std::string const &lower_bound, std::string const &upper_bound, bool lower_bound_inclusive, bool upper_bound_inclusive) override;

    void dispose(const std::string &key) override;

private:

    void set_storage_filename(std::string &filename);

    void set_storage_filename(std::string &&filename);

public:

    static schema load_schema_from_file(std::string const &filename = "");

    void save_schema_to_file(std::string const &filename = "");

    void insert_schema_to_file(const std::filesystem::path &path);

};

#endif //SCHEMA_H

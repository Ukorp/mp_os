#ifndef SCHEMA_POOL_H
#define SCHEMA_POOL_H

#include "../../interface.h"
#include "../../schema/include/schema.h"

class schemas_pool :
        public interface<std::string, schema> {

    friend class data_base;

private:
    static std::function<int(const std::string &, const std::string &)> _default_string_comparer;

    std::unique_ptr<b_tree<std::string, schema>> _data;

public:
    schemas_pool();

    ~schemas_pool() override;

    schemas_pool(const schemas_pool &other);

    schemas_pool(schemas_pool &&other) noexcept;

    schemas_pool &operator=(const schemas_pool &other);

    schemas_pool &operator=(schemas_pool &&other) noexcept;

public:
    void insert(const std::string &key, const schema &value) override;

    void insert(const std::string &key, schema &&value) override;

    schema &obtain(const std::string &key) override;

    void update(const std::string &key, schema &&value) override;

    void update(const std::string &key, const schema &value) override;

    std::map<std::string, schema> obtain_between(std::string const &lower_bound, std::string const &upper_bound,
                                                 bool lower_bound_inclusive, bool upper_bound_inclusive) override;

    void dispose(const std::string &key) override;

public:
    static schemas_pool load_schemas_pool_from_filesystem(std::string const &filename) ;

    void save_schemas_pool_to_filesystem(std::string const &filename = "") const;
};

#endif //SCHEMA_POOL_H

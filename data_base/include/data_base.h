#ifndef DATA_BASE_H
#define DATA_BASE_H
#include "../../interface/schema_pool/include/schema_pool.h"
#include <set>


class data_base final : public interface<std::string, schemas_pool> {
public:

	static std::function<int(const std::string &, const std::string &)> _default_string_comparer;

	static std::function<int(const int &, const int &)> _int_comparer;

private:

    friend class table;

    friend class schema;

    friend class schema_pool;

private:

	std::filesystem::path _instance_path;

    std::unique_ptr<b_tree<std::string, schemas_pool>> _data;

public:

	explicit data_base(std::size_t t, strategy _strategy = memory,
						 std::string const &instance_name = "data_base",
						 const std::function<int(const std::string &, const std::string &)> &keys_comparer = _default_string_comparer,
						 allocator *allocator = nullptr, logger *logger = nullptr);

    data_base();

    ~data_base() override;

    data_base(const data_base &other) = delete;

    data_base(data_base &&other) noexcept;

    data_base &operator=(const data_base &other) = delete;

    data_base &operator=(data_base &&other) noexcept;

private:

    static void throw_if_invalid(std::string const &key);

public:

    void insert_schemas_pool(
	    std::string const &pool_name,
	    schemas_pool const &value);

    void insert_schemas_pool(
	    std::string const &pool_name,
	    schemas_pool &&value);

    void insert_schema(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    schema const &value);

    void insert_schema(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    schema &&value);

    void insert_table(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name,
	    table const &value);

    void insert_table(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name,
	    table &&value);

    void insert_data(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name,
	    std::string const &user_data_key,
	    person_data &&value);

    void insert_data(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name,
	    std::string const &user_data_key,
	    person_data const &value);

    person_data obtain_data(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name,
	    std::string const &user_data_key);

	[[nodiscard]] table &obtain_table(std::string const &pool_name, std::string const &schema_name, std::string const &table_name) const;

	std::map<std::string, person_data> obtain_between_data(const std::string &pool_name, const std::string &schema_name,
	                                                       const std::string &table_name,
	                                                       const std::string &lower_bound,
	                                                       const std::string &upper_bound, bool lower_bound_inclusive,
	                                                       bool upper_bound_inclusive);

	void update_data(std::string const &pool_name,
	                 std::string const &schema_name,
	                 std::string const &table_name,
	                 std::string const &user_data_key,
	                 person_data &value);

    void update_data(std::string const &pool_name,
		     std::string const &schema_name,
		     std::string const &table_name,
		     std::string const &user_data_key,
		     person_data &&value);

    void dispose_schemas_pool(
	    std::string const &pool_name);

    void dispose_schema(
	    std::string const &pool_name,
	    std::string const &schema_name);

    void dispose_table(
	    std::string const &pool_name,
	    std::string const &schema_name,
	    std::string const &table_name);

	void dispose_person(std::string const &pool_name, std::string const &schema_name, std::string const &table_name,
	                    std::string const &person_key);


private:

    void update(const std::string &key, const schemas_pool &value) override;

    void update(const std::string &key, schemas_pool &&value) override;

    void insert(const std::string &key, const schemas_pool &value) override;

    void insert(const std::string &key, schemas_pool &&value) override;

    schemas_pool &obtain(const std::string &key) override;

    std::map<std::string, schemas_pool> obtain_between(std::string const &lower_bound, std::string const &upper_bound, bool lower_bound_inclusive, bool upper_bound_inclusive) override;

    void dispose(const std::string &key) override;

public:

	void load_data_base_state();

	void save_data_base_state();

private:

    std::string find_key_in_file(const std::string &key);

	void insert_pool_to_file(const std::string &pool_name, schemas_pool &&value);

	void insert_pool_to_file(const std::string &pool_name, const schemas_pool &value);

	void add_pool_to_file(const std::string &pool_name);

	void add_schema_to_file(const std::string &pool_name, const std::string &schema_name);

	void add_table_to_file(const std::string &pool_name, const std::string &schema_name, const std::string &table_name);

	static void create_if_not_exists(std::filesystem::path const &pool_path);

	void insert_schema_to_file(const std::string &pool_name, const std::string &schema_name, schema &&value);

	void update_person_in_file(const std::string &pool_name, const std::string &schema_name,
	                           const std::string &table_name,
	                           const std::string &user_data_key, person_data &&value);

	void update_person_in_file(const std::string &pool_name, const std::string &schema_name,
	                           const std::string &table_name,
	                           const std::string &person_key, person_data &value);

	void insert_data_to_file(const std::string &pool_name, const std::string &schema_name,
	                         const std::string &table_name,
	                         const std::string &user_data_key, person_data &&value);

	void insert_data_to_file(const std::string &pool_name, const std::string &schema_name,
	                         const std::string &table_name,
	                         const std::string &user_data_key, person_data const &value);

	person_data obtain_data_in_file(const std::string &pool_name, const std::string &schema_name,
	                                const std::string &table_name, const std::string &person_key);

	void dispose_pool_in_file(const std::string &pool_name);

	void dispose_schema_in_file(const std::string &pool_name, const std::string &schema_name);

	void dispose_table_in_file(const std::string &schemas_pool_name, const std::string &schema_name,
	                           const std::string &table_name);

	void dispose_person_in_file(const std::string &pool_name, const std::string &schema_name,
	                            const std::string &table_name,
	                            const std::string &person_key);

	std::map<std::string, person_data> obtain_between_person_in_file(std::string const &pool_name,
	                                                                 std::string const &schema_name,
	                                                                 std::string const &table_name,
	                                                                 std::string const &lower_bound,
	                                                                 std::string const &upper_bound,
	                                                                 bool lower_bound_inclusive,
	                                                                 bool upper_bound_inclusive);

};

#endif //DATA_BASE_H

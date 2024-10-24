#include "../include/data_base.h"
#include <iostream>

interface<std::string, schemas_pool>::strategy strategy = interface<std::string, schemas_pool>::memory;

int main(int argc, char *argv[])
{
	if (argc > 2) {
		std::cout << "Error: too many flags (must be 0 or 1)" << std::endl;
		return EXIT_FAILURE;
	}
	if (argc == 2) {
		if (*argv == "memory") {
			strategy = interface<std::string, schemas_pool>::memory;
		} else if (*argv == "file") {
			strategy = interface<std::string, schemas_pool>::file;
		} else {
			return EXIT_FAILURE;
		}
	}
    data_base db(4, strategy);
	// db.obtain_table("pl", "new_schema", "tbl").print_table();
	// auto test = db.obtain_between_data("pl", "new_schema", "tbl", "key12323", "key1234", true, true);
	db.insert_schemas_pool("pl", schemas_pool());
	db.insert_schema("pl", "new_schema", schema());
	db.insert_table("pl", "new_schema", "tbl", table());
	for (int i = 3; i < 10; ++i)
	{
	    db.insert_data("pl", "new_schema", "tbl", std::string{"key"} + std::to_string(i), person_data(i, "Robsoon", "BBBAt"));
	    db.dispose_person("pl", "new_schema", "tbl", std::string{"key"} + std::to_string(i));
	}

	db.insert_table("pl", "new_schema", "table2", table());
	db.insert_data("pl", "new_schema", "table2", "key123", person_data(1, "nm1", "srn1"));
	db.insert_data("pl", "new_schema", "tbl", "key1234", person_data(1, "nm1", "srn1"));
	db.insert_data("pl", "new_schema", "tbl", "key1235", person_data(2, "nm1", "srn1"));
	db.insert_data("pl", "new_schema", "tbl", "key12323", person_data(3, "nm1", "srn1"));

	db.update_data("pl", "new_schema", "tbl", "key1234", person_data(0, "noootmrteregfhfbds", ""));
	db.dispose_person("pl", "new_schema", "tbl", "key1234");
	db.insert_data("pl", "new_schema", "tbl", "key1234", person_data(0, "bebrov", ""));


	// auto data = db.obtain_data("pl", "new_schema", "tbl", "key1234");
	// std::cout << data.get_id() << " " << data.get_name() << " " << data.get_surname() << std::endl;
	//
	// auto data2 = db.obtain_data("pl","new_schema", "table2", "key123");
	// std::cout << data2.get_name() << " " << data2.get_surname() << '\n';
	db.save_data_base_state();
    return 0;
}
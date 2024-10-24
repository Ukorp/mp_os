cmake_minimum_required(VERSION 3.22)
project(CWOS)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)



add_executable(CWOS main.cpp
        b_tree/b_tree.h
        b_tree/search_tree.h
        allocator/include/allocator_with_fit_mode.h
        allocator/include/allocator_test_utils.h
        allocator/src/allocator_guardant.cpp
        allocator/src/allocator_test_utils.cpp
        logger/client_logger/include/client_logger.h
        logger/client_logger/src/client_logger.cpp
        logger/client_logger/src/client_logger_builder.cpp
        logger/logger/src/logger.cpp
        logger/logger/src/logger_builder.cpp
        interface/interface.h
        interface/schema/include/schema.h
        person_data/include/person_data.h
        person_data/src/person_data.cpp
        interface/table/include/table.h
        interface/table/src/table.cpp
        interface/schema/src/schema.cpp
        interface/schema_pool/src/schema_pool.cpp
        interface/schema_pool/include/schema_pool.h
        data_base/src/data_base.cpp
        data_base/include/data_base.h
        data_base/src/main.cpp
        logger/logger/src/logger_guardant.cpp
        string_pool/include/string_pool.h
        string_pool/src/string_pool.cpp)

target_link_libraries(
        CWOS
        PUBLIC
        nlohmann_json::nlohmann_json)

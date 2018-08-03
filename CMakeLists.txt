cmake_minimum_required(VERSION 3.5)

project(EOSIO_sql_plugin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

find_package(Soci REQUIRED)

message(STATUS "[Additional Plugin] EOSIO sql plugin enabled")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} include db)

add_library(sql_db_plugin
    db/database.cpp
    db/accounts_table.cpp
    db/transactions_table.cpp
    db/blocks_table.cpp
    db/actions_table.cpp
    sql_db_plugin.cpp
    )

target_link_libraries(sql_db_plugin
    chain_plugin
    eosio_chain
    ${SOCI_LIBRARY}
    )

add_subdirectory(test)

eosio_additional_plugin(sql_db_plugin)



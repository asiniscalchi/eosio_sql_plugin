#include "accounts_table.h"

#include <fc/log/logger.hpp>

namespace eosio {

accounts_table::accounts_table(std::shared_ptr<soci::session> session):
    m_session(session)
{
    backend = m_session->get_backend_name();
}

void accounts_table::drop()
{
    try {
        *m_session << "DROP TABLE IF EXISTS accounts_keys CASCADE";
        *m_session << "DROP TABLE IF EXISTS accounts CASCADE";
    }
    catch(std::exception& e){
        wlog(e.what());
    }
}

void accounts_table::create()
{
    if (backend == "postgresql") {
        this->create_postgresql();
    }
    else if (backend == "mysql") {
        this->create_mysql();
    }
}

void accounts_table::add(string name)
{
    *m_session << "INSERT INTO accounts (name) VALUES (:name)",
            soci::use(name, "name");
}

bool accounts_table::exist(string name)
{
    int amount;
    try {
        *m_session << "SELECT COUNT(*) FROM accounts WHERE name = :name", soci::into(amount), soci::use(name, "name");
    }
    catch (std::exception const & e)
    {
        amount = 0;
    }
    return amount > 0;
}

// private

void accounts_table::create_mysql()
{
    *m_session <<  "CREATE TABLE accounts("
        "name VARCHAR(12) PRIMARY KEY,"
        "abi JSON DEFAULT NULL,"
        "created_at DATETIME DEFAULT NOW(),"
        "updated_at DATETIME DEFAULT NOW()) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE utf8mb4_general_ci;";
    
    *m_session << "CREATE TABLE accounts_keys("
        "account VARCHAR(12),"
        "public_key VARCHAR(53),"
        "permission VARCHAR(12), FOREIGN KEY (account) REFERENCES accounts(name)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE utf8mb4_general_ci;";
}

void accounts_table::create_postgresql()
{
    *m_session << "CREATE TABLE accounts ("
        "name TEXT PRIMARY KEY,"
        "abi JSONB DEFAULT NULL,"
        "created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,"
        "updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP);";

    *m_session << "CREATE TABLE accounts_keys ("
        "account TEXT REFERENCES accounts (name),"
        "public_key TEXT,"
        "permission TEXT);";
}

} // namespace

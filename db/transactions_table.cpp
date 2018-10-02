#include "transactions_table.h"

#include <chrono>
#include <fc/log/logger.hpp>

namespace eosio {

transactions_table::transactions_table(std::shared_ptr<soci::session> session):
    m_session(session)
{
    backend = m_session->get_backend_name();
}

void transactions_table::drop()
{
    try {
        *m_session << "DROP TABLE IF EXISTS transactions CASCADE;";
    }
    catch(std::exception& e){
        wlog(e.what());
    }
}

void transactions_table::create()
{
    if (backend == "postgresql") {
        this->create_postgresql();
    }
    else if (backend == "mysql") {
        this->create_mysql();
    }

    // indices

    *m_session << "CREATE INDEX transactions_block_id ON transactions (block_id);";

}

void transactions_table::add(uint32_t block_id, chain::transaction transaction)
{
    const auto transaction_id_str = transaction.id().str();
    const auto expiration = std::chrono::seconds{transaction.expiration.sec_since_epoch()}.count();

    *m_session << this->add_transaction(),
            soci::use(transaction_id_str, "id"),
            soci::use(block_id, "bi"),
            soci::use(transaction.ref_block_num, "rbi"),
            soci::use(transaction.ref_block_prefix, "rb"),
            soci::use(expiration, "ex"),
            soci::use(0, "pe"),
            soci::use(expiration, "ca"),
            soci::use(expiration, "ua"),
            soci::use(transaction.total_actions(), "na");
}

void transactions_table::create_mysql()
{
    *m_session << "CREATE TABLE transactions("
        "id VARCHAR(64) PRIMARY KEY,"
        "block_id INT NOT NULL,"
        "ref_block_num INT NOT NULL,"
        "ref_block_prefix INT,"
        "expiration DATETIME DEFAULT NOW(),"
        "pending TINYINT(1),"
        "created_at DATETIME DEFAULT NOW(),"
        "num_actions INT DEFAULT 0,"
        "updated_at DATETIME DEFAULT NOW(), FOREIGN KEY (block_id) REFERENCES blocks(block_number) ON DELETE CASCADE) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE utf8mb4_general_ci;";
}

void transactions_table::create_postgresql()
{
    *m_session << "CREATE TABLE transactions ("
            "id TEXT PRIMARY KEY,"
            "block_id INT NOT NULL REFERENCES blocks (block_number) ON DELETE CASCADE,"
            "ref_block_num INT NOT NULL,"
            "ref_block_prefix INT,"
            "expiration TIMESTAMPTZ DEFAULT NOW(),"
            "pending INT,"
            "created_at TIMESTAMPTZ DEFAULT NOW(),"
            "num_actions INT DEFAULT 0,"
            "updated_at TIMESTAMPTZ DEFAULT NOW());";
}

// transactions_table::add_transaction() defaults to MySQL syntax
std::string transactions_table::add_transaction()
{
    if (backend == "postgresql") {
        return "INSERT INTO transactions (id, block_id, ref_block_num, ref_block_prefix,"
            "expiration, pending, created_at, updated_at, num_actions) VALUES (:id, :bi, :rbi, :rb, TO_TIMESTAMP(:ex), :pe, TO_TIMESTAMP(:ca), TO_TIMESTAMP(:ua), :na)";
    }

    return "INSERT INTO transactions(id, block_id, ref_block_num, ref_block_prefix,"
        "expiration, pending, created_at, updated_at, num_actions) VALUES (:id, :bi, :rbi, :rb, FROM_UNIXTIME(:ex), :pe, FROM_UNIXTIME(:ca), FROM_UNIXTIME(:ua), :na)";
}

} // namespace

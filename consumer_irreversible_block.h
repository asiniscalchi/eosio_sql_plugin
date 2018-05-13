#ifndef CONSUMER_IRREVERSIBLE_BLOCK_H
#define CONSUMER_IRREVERSIBLE_BLOCK_H

#include "consumer.h"

#include <eosio/chain/block.hpp>
#include <eosio/chain/block_trace.hpp>

#include "fifo.h"

namespace eosio {

class consumer_irreversible_block : public consumer
{
public:
    consumer_irreversible_block(std::shared_ptr<database> db);

    void push(const chain::signed_block& b);
    void consume() final;

private:
    fifo<chain::signed_block> m_fifo;
};

} // namespace

#endif // CONSUMER_IRREVERSIBLE_BLOCK_H

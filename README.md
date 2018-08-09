# eosio_sql_plugin
EOSIO plugin to register blockchain data into an SQL database supported by SOCI ( https://github.com/SOCI/soci )

**You need to compile EOSIO from source: additional plugins are linked statically**



on ubuntu 18.04 install the package libsoci-dev
```
$ sudo apt install libsoci-dev
```
add the following param on EOSIO cmake execution:
```
-DEOSIO_ADDITIONAL_PLUGINS=<path_to_eosio_sql_plugin_source>
```
compile and run.
```
$ nodeos --help

....
Config Options for eosio::sql_db_plugin:
  --sql_db-queue-size arg (=256)        The queue size between nodeos and SQL 
                                        DB plugin thread.
  --sql_db-block-start arg (=0)         The block to start sync.
  --sql_db-uri arg                      Sql DB URI connection string If not 
                                        specified then plugin is disabled. 
                                        Default database 'EOS' is used if not 
                                        specified in URI.
....
```

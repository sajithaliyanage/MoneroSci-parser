//
// Created by sajithaliyanage on 12/4/18.
//


#ifndef MONEROSCI_QUERY_H
#define MONEROSCI_QUERY_H

#endif //MONEROSCI_QUERY_H

using namespace std;

//block detail table
char const *blockDetailsTable = "CREATE TABLE IF NOT EXISTS BLOCK_DATA( DATA_ID INT PRIMARY KEY NOT NULL,HEIGHT INT NOT NULL,"\
        "HASH TEXT NOT NULL, TIMESTAMP INT NOT NULL, TIMESTAMP_UTC TEXT NOT NULL, SIZE INT NOT NULL, "
        "TX_COUNT INT NOT NULL, NONCE INTEGER, PREV_HASH TEXT, NEXT_HASH TEXT);";
//block index table
char const *blockIndexTable = "CREATE TABLE IF NOT EXISTS BLOCK_INDEX( ID INT PRIMARY KEY NOT NULL,HASH_ID INT "\
        "NOT NULL,DATA_ID INT NOT NULL);CREATE INDEX blk_index ON BLOCK_INDEX (HASH_ID);";

//stealth-address table
char const *saDetailTable = "CREATE TABLE IF NOT EXISTS SA_DATA (DATA_ID INTEGER PRIMARY KEY NOT NULL,P_KEY TEXT NOT NULL,AMOUNT INTEGER,AMOUNT_INDEX INTEGER);";

//stealth-address table
char const *saIndexTable = "CREATE TABLE IF NOT EXISTS SA_INDEX (DATA_ID INT PRIMARY KEY NOT NULL,BLOCK_ID INT NOT NULL,TX_ID INT NOT NULL,HASH_ID INT NOT NULL);"\
                            "CREATE INDEX output_index ON SA_INDEX (HASH_ID);";

//ring-member detail table
char const *rmDetailTable = "CREATE TABLE IF NOT EXISTS RM_DATA (DATA_ID INTEGER PRIMARY KEY NOT NULL,P_KEY TEXT NOT NULL,BLOCK_NO INT, KEY_OFFSET INTEGER);";

//ring-members index table
char const *rmIndexTable = "CREATE TABLE IF NOT EXISTS RM_INDEX (DATA_ID INT PRIMARY KEY NOT NULL,BLOCK_ID INT NOT NULL,TX_ID INT NOT NULL,KEY_IMAGE INT NOT NULL,HASH_ID INT NOT NULL);"
        "CREATE INDEX input_index ON RM_INDEX (HASH_ID);";

//tx details table
char const *txDetailsTable = "CREATE TABLE IF NOT EXISTS TX_DATA(DATA_ID INTEGER PRIMARY KEY NOT NULL, COIN_BASE INT NOT NULL,HEIGHT INT NOT NULL,"
        "HASH TEXT NOT NULL, CONFIRMATIONS INT, EXTRA TEXT, INPUT_COUNT INT NOT NULL, MIXIN INT, OUTPUT_COUNT INT NOT NULL,"
        "PAYMENT_ID INT,RCT_TYPE INT, TX_FEE INTEGER,TX_VERSION INT,XMR_INPUT INTEGER, XMR_OUTPUT INTEGER, TIMESTAMP INT NOT NULL, "
        "TIMESTAMP_UTC TEXT NOT NULL, SIZE INT NOT NULL);";

//tx index table
char const *txIndexTable = "CREATE TABLE IF NOT EXISTS TX_INDEX (DATA_ID INT PRIMARY KEY NOT NULL,BLOCK_ID INT NOT NULL,HASH_ID INT NOT NULL);"
        "CREATE INDEX transaction_index ON TX_INDEX (HASH_ID);";

char const *keyImageTable = "CREATE TABLE IF NOT EXISTS KI_DATA (DATA_ID INTEGER PRIMARY KEY NOT NULL,K_HASH TEXT NOT NULL,BLOCK_ID INT NOT NULL, TX_ID INT NOT NULL);";
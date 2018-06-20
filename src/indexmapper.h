//
// Created by sajithaliyanage on 10/4/18.
//

#ifndef MONEROSCI_INDEXMAPPER_H
#define MONEROSCI_INDEXMAPPER_H

#endif //MONEROSCI_INDEXMAPPER_H

#include <iostream>
#include <stdio.h>
#include <sqlite3.h>



namespace indexMapper{

    using json = nlohmann::json;

    class indexes{
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        sqlite3_stmt *stmt;
        const char *pzTest;
        sqlite3_stmt *res;
        std::mutex mtx;

    public:
        indexes(string homeDir, string subDir, string storeDir, bool firstTry = false){
            string homeDirName = homeDir;
            string subDirName = subDir;
            string storeDirName = storeDir+".db";

            struct passwd *pw = getpwuid(getuid());
            const char *homedir = pw->pw_dir;
            std::string h = std::string(homedir);
            string folderPathName = "/"+homeDirName+"/"+subDirName+"/";
            string path = h+folderPathName;
            string finalPath = path;
            string dbPath = path + storeDirName;
//            cout << dbPath << endl;
            //create folder for sqlite db location
            if(firstTry){
                createFolderinPath(path);
            }

            rc = sqlite3_open(dbPath.c_str(), &db);

            if(rc != SQLITE_OK)
            {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

            }

        }

        sqlite3* getDb(){
            return this->db;
        }

        int lastInsertedId(){
            /* Execute SQL statement */
            char const *sqlQuery = "SELECT last_insert_rowid();";
            rc = sqlite3_prepare_v2(db, sqlQuery, -1, &res, 0);

            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            rc = sqlite3_step(res);

            int lastId;
            if (rc == SQLITE_ROW) {
                lastId =  atoi((const char *)sqlite3_column_text(res, 0));
            }

            sqlite3_finalize(res);
            sqlite3_close(db);
            return lastId;

        }

        static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
            int i;
            for(i = 0; i<argc; i++) {
                printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            }
            printf("\n");
            return 0;
        }

        void createFolderinPath(string path){
            mtx.lock();
            system(("mkdir -p "+path).c_str());
            mtx.unlock();
        }

        void createTable(char const *sqlQuery){
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);

            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
//            sqlite3_close(db);
        }

        void insertBlockData(vector<json>  blockData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO BLOCK_DATA (DATA_ID,HEIGHT,HASH,TIMESTAMP,TIMESTAMP_UTC,SIZE,TX_COUNT,NONCE,PREV_HASH,NEXT_HASH) VALUES (?,?,?,?,?,?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<blockData.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, blockData[x].at("lastBlockId"));
                    sqlite3_bind_int(stmt, 2, blockData[x].at("block_height"));
                    std::string hash = blockData[x].at("hash");
                    sqlite3_bind_text(stmt, 3,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 4,  blockData[x].at("timestamp"));
                    std::string timestamp_utc = blockData[x].at("timestamp_utc");
                    sqlite3_bind_text(stmt, 5,  timestamp_utc.c_str(), strlen(timestamp_utc.c_str()), 0);
                    sqlite3_bind_int(stmt, 6,  blockData[x].at("size"));
                    sqlite3_bind_int(stmt, 7, blockData[x].at("txCount"));
                    sqlite3_bind_int(stmt, 8, blockData[x].at("nonce"));
                    std::string pre_hash = blockData[x].at("prev_hash");
                    sqlite3_bind_text(stmt, 9,  pre_hash.c_str(), strlen(pre_hash.c_str()), 0);
                    std::string next_hash = blockData[x].at("next_hash");
                    sqlite3_bind_text(stmt, 10,  next_hash.c_str(), strlen(next_hash.c_str()), 0);

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }
                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);
            }else{
                fprintf(stderr, "SQL error: %d\n", rc);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertBlockIndex(vector<std::vector<int>> blockIndex){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO BLOCK_INDEX (ID,HASH_ID, DATA_ID) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<blockIndex.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, blockIndex[x][0]);
                    sqlite3_bind_int(stmt, 2, blockIndex[x][0]);
                    sqlite3_bind_int(stmt, 3, blockIndex[x][0]);

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }
                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertSAData(vector<json> saData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO SA_DATA (DATA_ID,P_KEY,AMOUNT,AMOUNT_INDEX) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<saData.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, saData[x].at("lastSAId"));
                    std::string hash = saData[x].at("public_key");
                    sqlite3_bind_text(stmt, 2,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int64(stmt, 3, saData[x].at("amount"));
                    string amount_index = saData[x].at("amount_idx");
                    sqlite3_bind_int(stmt, 4, atoi(amount_index.c_str()));

                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }

                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));

        }

        void insertSAIndex(vector<std::vector<int>> saIndex){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO SA_INDEX (DATA_ID,BLOCK_ID,TX_ID,HASH_ID) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL,-1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<saIndex.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, saIndex[x][2]);
                    sqlite3_bind_int(stmt, 2, saIndex[x][3]);
                    sqlite3_bind_int(stmt, 3, saIndex[x][0]);
                    sqlite3_bind_int(stmt, 4, saIndex[x][1]);

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }

                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertKIData(vector<std::vector<string>> kiData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO KI_DATA (DATA_ID,K_HASH,BLOCK_ID,TX_ID) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<kiData.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, atoi(kiData[x][0].c_str()));
                    sqlite3_bind_text(stmt, 2,  kiData[x][1].c_str(), strlen(kiData[x][1].c_str()), 0);
                    sqlite3_bind_int(stmt, 3, atoi(kiData[x][2].c_str()));
                    sqlite3_bind_int(stmt, 4, atoi(kiData[x][3].c_str()));

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }

                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertRMData(vector<json>  rmData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO RM_DATA (DATA_ID,P_KEY,BLOCK_NO,KEY_OFFSET) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<rmData.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, rmData[x].at("lastRingId"));
                    std::string hash = rmData[x].at("public_key");
                    sqlite3_bind_text(stmt, 2,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 3, rmData[x].at("block_no"));

                    json checkType = rmData[x]["key_offset"];
                    if(checkType.is_null()){
                        rmData[x]["key_offset"] = 0;
                    }
                    sqlite3_bind_int(stmt, 4, rmData[x].at("key_offset"));

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }
                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertRMIndex(vector<std::vector<int>> rmIndex){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO RM_INDEX (DATA_ID,BLOCK_ID,TX_ID,KEY_IMAGE,HASH_ID) VALUES (?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<rmIndex.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, rmIndex[x][0]);
                    sqlite3_bind_int(stmt, 2, rmIndex[x][4]);
                    sqlite3_bind_int(stmt, 3, rmIndex[x][1]);
                    sqlite3_bind_int(stmt, 4, rmIndex[x][2]);
                    sqlite3_bind_int(stmt, 5, rmIndex[x][3]);

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }
                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertTxIndex(vector<std::vector<int>> txIndex){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO TX_INDEX (DATA_ID,BLOCK_ID,HASH_ID) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<txIndex.size();x++){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, txIndex[x][0]);
                    sqlite3_bind_int(stmt, 2, txIndex[x][1]);
                    sqlite3_bind_int(stmt, 3, txIndex[x][2]);

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }

                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertTxData(vector<json>  txData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char* errorMessage;
            sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errorMessage);
            sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errorMessage);

            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

            char const *szSQL = "INSERT INTO TX_DATA (DATA_ID,COIN_BASE,HEIGHT,HASH,CONFIRMATIONS,EXTRA, INPUT_COUNT,MIXIN,OUTPUT_COUNT,PAYMENT_ID,RCT_TYPE,TX_FEE,"
                    "TX_VERSION,XMR_INPUT,XMR_OUTPUT,TIMESTAMP,TIMESTAMP_UTC,SIZE) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                for(int x=0;x<txData.size();x++){
                    // bind the value
                    int isCoinBase;
                    if(txData[x].at("coinbase")){
                        isCoinBase = 1;
                    }else{
                        isCoinBase = 0;
                    }
                    sqlite3_bind_int(stmt, 1, txData[x].at("lastTxId"));
                    sqlite3_bind_int(stmt, 2, isCoinBase);
                    sqlite3_bind_int(stmt, 3, txData[x].at("block_height"));
                    std::string hash = txData[x].at("tx_hash");
                    sqlite3_bind_text(stmt, 4,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 5,  txData[x].at("confirmations"));
                    std::string extra = txData[x].at("extra");
                    sqlite3_bind_text(stmt, 6,  extra.c_str(), strlen(extra.c_str()), 0);
                    sqlite3_bind_int(stmt, 7, txData[x].at("inputs").size());
                    sqlite3_bind_int(stmt, 8, txData[x].at("mixin"));
                    sqlite3_bind_int(stmt, 9, txData[x].at("outputs").size());
                    std::string payment_id = txData[x].at("payment_id");
                    sqlite3_bind_text(stmt, 10,  payment_id.c_str(), strlen(payment_id.c_str()), 0);
                    sqlite3_bind_int(stmt, 11, txData[x].at("rct_type"));
                    sqlite3_bind_int64(stmt, 12, txData[x].at("tx_fee"));
                    sqlite3_bind_int(stmt, 13, txData[x].at("tx_version"));
                    sqlite3_bind_int64(stmt, 14, txData[x].at("xmr_inputs"));
                    sqlite3_bind_int64(stmt, 15, txData[x].at("xmr_outputs"));
                    sqlite3_bind_int(stmt, 16,  txData[x].at("timestamp"));
                    std::string timestamp_utc = txData[x].at("timestamp_utc");
                    sqlite3_bind_text(stmt, 17,  timestamp_utc.c_str(), strlen(timestamp_utc.c_str()), 0);
                    sqlite3_bind_int(stmt, 18,  txData[x].at("tx_size"));

                    // commit
                    int retVal = sqlite3_step(stmt);
                    if (retVal != SQLITE_DONE)
                    {
                        printf("Commit Failed! %d\n", retVal);
                    }

                    sqlite3_reset(stmt);
                }

                sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
                sqlite3_finalize(stmt);

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void close(){
            sqlite3_close(db);
        }
    };
}
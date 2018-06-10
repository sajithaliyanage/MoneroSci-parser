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

        void insertBlockData(json blockData){
//            sqlite3_mutex_enter(sqlite3_db_mutex(db));
            char const *szSQL = "INSERT INTO BLOCK_DATA (DATA_ID,HEIGHT,HASH,TIMESTAMP,TIMESTAMP_UTC,SIZE,TX_COUNT) VALUES (?,?,?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!blockData.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, blockData.at("lastBlockId"));
                    sqlite3_bind_int(stmt, 2, blockData.at("block_height"));
                    std::string hash = blockData.at("hash");
                    sqlite3_bind_text(stmt, 3,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 4,  blockData.at("timestamp"));
                    std::string timestamp_utc = blockData.at("timestamp_utc");
                    sqlite3_bind_text(stmt, 5,  timestamp_utc.c_str(), strlen(timestamp_utc.c_str()), 0);
                    sqlite3_bind_int(stmt, 6,  blockData.at("size"));
                    sqlite3_bind_int(stmt, 7, blockData.at("txCount"));

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %d\n", rc);
                sqlite3_free(zErrMsg);
            }

//            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertBlockIndex(vector<int> blockIndex){

            char const *szSQL = "INSERT INTO BLOCK_INDEX (ID,HASH_ID, DATA_ID) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!blockIndex.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, blockIndex[0]);
                    sqlite3_bind_int(stmt, 2, blockIndex[0]);
                    sqlite3_bind_int(stmt, 3, blockIndex[0]);

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertSAData(json saData){
            sqlite3_mutex_enter(sqlite3_db_mutex(db));

            char const *szSQL = "INSERT INTO SA_DATA (DATA_ID,P_KEY,AMOUNT) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!saData.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, saData.at("lastSAId"));
                    std::string hash = saData.at("public_key");
                    sqlite3_bind_text(stmt, 2,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int64(stmt, 3, saData.at("amount"));

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            sqlite3_mutex_leave(sqlite3_db_mutex(db));
        }

        void insertSAIndex(vector<int> saIndex){
            char const *szSQL = "INSERT INTO SA_INDEX (DATA_ID,BLOCK_ID,TX_ID,HASH_ID) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL,-1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!saIndex.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, saIndex[2]);
                    sqlite3_bind_int(stmt, 2, saIndex[3]);
                    sqlite3_bind_int(stmt, 3, saIndex[0]);
                    sqlite3_bind_int(stmt, 4, saIndex[1]);

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertKIData(vector<string> kiData){
            char const *szSQL = "INSERT INTO KI_DATA (DATA_ID,K_HASH,BLOCK_ID,TX_ID) VALUES (?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!kiData.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, atoi(kiData[0].c_str()));
                    sqlite3_bind_text(stmt, 2,  kiData[1].c_str(), strlen(kiData[1].c_str()), 0);
                    sqlite3_bind_int(stmt, 3, atoi(kiData[2].c_str()));
                    sqlite3_bind_int(stmt, 4, atoi(kiData[3].c_str()));

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertRMData(json rmData){
            char const *szSQL = "INSERT INTO RM_DATA (DATA_ID,P_KEY,BLOCK_NO) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!rmData.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, rmData.at("lastRingId"));
                    std::string hash = rmData.at("public_key");
                    sqlite3_bind_text(stmt, 2,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 3, rmData.at("block_no"));

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertRMIndex(vector<int> rmIndex){
            char const *szSQL = "INSERT INTO RM_INDEX (DATA_ID,BLOCK_ID,TX_ID,KEY_IMAGE,HASH_ID) VALUES (?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!rmIndex.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, rmIndex[0]);
                    sqlite3_bind_int(stmt, 2, rmIndex[4]);
                    sqlite3_bind_int(stmt, 3, rmIndex[1]);
                    sqlite3_bind_int(stmt, 4, rmIndex[2]);
                    sqlite3_bind_int(stmt, 5, rmIndex[3]);

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertTxIndex(vector<int> txIndex){
            char const *szSQL = "INSERT INTO TX_INDEX (DATA_ID,BLOCK_ID,HASH_ID) VALUES (?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, -1, &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!txIndex.empty()){
                    // bind the value
                    sqlite3_bind_int(stmt, 1, txIndex[0]);
                    sqlite3_bind_int(stmt, 2, txIndex[1]);
                    sqlite3_bind_int(stmt, 3, txIndex[2]);

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void insertTxData(json txData){
            char const *szSQL = "INSERT INTO TX_DATA (DATA_ID,COIN_BASE,HEIGHT,HASH,CONFIRMATIONS,EXTRA, INPUT_COUNT,MIXIN,OUTPUT_COUNT,PAYMENT_ID,RCT_TYPE,TX_FEE,"
                    "TX_VERSION,XMR_INPUT,XMR_OUTPUT,TIMESTAMP,TIMESTAMP_UTC,SIZE) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
            int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

            if( rc == SQLITE_OK ) {
                if(!txData.empty()){
                    // bind the value
                    int isCoinBase;
                    if(txData.at("coinbase")){
                        isCoinBase = 1;
                    }else{
                        isCoinBase = 0;
                    }
                    sqlite3_bind_int(stmt, 1, txData.at("lastTxId"));
                    sqlite3_bind_int(stmt, 2, isCoinBase);
                    sqlite3_bind_int(stmt, 3, txData.at("block_height"));
                    std::string hash = txData.at("tx_hash");
                    sqlite3_bind_text(stmt, 4,  hash.c_str(), strlen(hash.c_str()), 0);
                    sqlite3_bind_int(stmt, 5,  txData.at("confirmations"));
                    std::string extra = txData.at("extra");
                    sqlite3_bind_text(stmt, 6,  extra.c_str(), strlen(extra.c_str()), 0);
                    sqlite3_bind_int(stmt, 7, txData.at("inputs").size());
                    sqlite3_bind_int(stmt, 8, txData.at("mixin"));
                    sqlite3_bind_int(stmt, 9, txData.at("outputs").size());
                    std::string payment_id = txData.at("payment_id");
                    sqlite3_bind_text(stmt, 10,  payment_id.c_str(), strlen(payment_id.c_str()), 0);
                    sqlite3_bind_int(stmt, 11, txData.at("rct_type"));
                    sqlite3_bind_int64(stmt, 12, txData.at("tx_fee"));
                    sqlite3_bind_int(stmt, 13, txData.at("tx_version"));
                    sqlite3_bind_int64(stmt, 14, txData.at("xmr_inputs"));
                    sqlite3_bind_int64(stmt, 15, txData.at("xmr_outputs"));
                    sqlite3_bind_int(stmt, 16,  txData.at("timestamp"));
                    std::string timestamp_utc = txData.at("timestamp_utc");
                    sqlite3_bind_text(stmt, 17,  timestamp_utc.c_str(), strlen(timestamp_utc.c_str()), 0);
                    sqlite3_bind_int(stmt, 18,  txData.at("tx_size"));

                    // commit
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }

            }else{
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
        }

        void close(){
            sqlite3_close(db);
        }
    };
}
//
// Created by sajithaliyanage on 10/4/18.
//

#ifndef MONEROSCI_IDHASHMAPPER_H
#define MONEROSCI_IDHASHMAPPER_H
#endif //MONEROSCI_IDHASHMAPPER_H

#include <iostream>
#include <assert.h>
#include <pwd.h>
#include <zconf.h>
#include "rocksdb/db.h"

using namespace std;

namespace hashmapper {

    class idHashMapperDB{
        string homeDirName;
        string subDirName;
        string storeDirName;
        string finalPath;
        rocksdb::DB* db;
        rocksdb::Options options;
        rocksdb::Status status;

    public:

        idHashMapperDB(string homeDir,string subDir, string storeDir){
            homeDirName = homeDir;
            subDirName = subDir;
            storeDirName = storeDir;

            options.create_if_missing = true;

            struct passwd *pw = getpwuid(getuid());
            const char *homedir = pw->pw_dir;
            std::string h = std::string(homedir);
            string folderPathName = "/"+homeDirName+"/"+subDirName+"/";
            string path = h+folderPathName+storeDirName;
            finalPath = path;
//            cout << finalPath << endl;
            //create folder for rocksDB location
            createFolderinPath(path);

            // open a database with a name which corresponds to a file system directory
            status = rocksdb::DB::Open(options, path, &db);
            // check status
            if (!status.ok()) cerr << status.ToString() << endl;
        }

        void createFolderinPath(string path){
            system(("mkdir -p "+path).c_str());
        }

        void insertKey(string key, string value){

            // Slice type can (should) be used in place of std::string for key-value
            rocksdb::Slice keyVal = key;
            std::string val = value;

            status = db->Put(rocksdb::WriteOptions(), key, val);
            if (!status.ok()) cerr << status.ToString() << endl;
        }

        string getValueFromKey(string key){
            rocksdb::Slice keyVal = key;
            std::string value;
            status = db->Get(rocksdb::ReadOptions(), keyVal, &value);
            if (!status.ok()){
                value = "null";
//                cerr << status.ToString() << endl;
            }

            return value;
        }

        void deleteFromKey(string key){
            rocksdb::Slice keyVal = key;
            status = db->Delete(rocksdb::WriteOptions(), keyVal);
            if (!status.ok()) cerr << status.ToString() << endl;
        }

        string isKeyExist(string key){

            rocksdb::Slice keyVal = key;
            std::string value;
            status = db->Get(rocksdb::ReadOptions(), keyVal, &value);

            if (value.empty()){
                value = "null";
            }

            return value;
        }

        void close(){
            // close the database
            delete db;
        }

    };
}
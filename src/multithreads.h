//
// Created by sajithaliyanage on 2/4/18.
//

#ifndef MONEROSCI_PARSER_H
#define MONEROSCI_PARSER_H

#endif //MONEROSCI_PARSER_H

#include "mstch/mstch.hpp"

#include "version.h"

#include "monero_headers.h"

#include "MicroCore.h"
#include "tools.h"
#include "rpccalls.h"

#include "CurrentBlockchainStatus.h"
#include "MempoolStatus.h"

#include "../ext/crow/crow.h"

#include "../ext/vpetrigocaches/cache.hpp"
#include "../ext/vpetrigocaches/lru_cache_policy.hpp"
#include "../ext/vpetrigocaches/fifo_cache_policy.hpp"

#include <algorithm>
#include <limits>
#include <ctime>
#include <future>
#include <regex>

#include "idhashmapper.h"
#include "indexmapper.h"
#include "query.h"
#include "bloomFilter.h"
#include <mutex>
#include <ctime>

namespace parse {
    using namespace xmreg;
    using json = nlohmann::json;

    bool isAllBlockDone = false;
    int lastBlockId;
    int lastTransactionId;
    int lastKeyImageId;
    int lastStealthAddressId;
    int lastOutputId;
    int lastInputId;
    int lastRingMemberId;

    BloomFilter bloomFilter; //instance of bloom-filter
    hashmapper::idHashMapperDB ringMemberHashMap ("blockchain-xmr", "hash-id-mapping", "ring-members");

    std::mutex mtx;           // mutex for critical section
    std::mutex mtx2;           // mutex for critical section
    std::mutex mtx3;           // mutex for critical section
    std::mutex mtx4;           // mutex for critical section

    std::mutex mtxh1;          //mutex for id-hash vectors
    std::mutex mtxh2;          //mutex for id-hash vectors
    std::mutex mtxh3;          //mutex for id-hash vectors
    std::mutex mtxh4;          //mutex for id-hash vectors
    std::mutex mtxh5;          //mutex for id-hash vectors

    std::mutex mtxb1;          //mutex for blockData vectors
    std::mutex mtxb2;          //mutex for blockIndex vectors

    std::mutex mtxt1;          //mutex for txData vectors
    std::mutex mtxt2;          //mutex for txIndex vectors

    std::mutex mtxk;          //mutex for KIdata vectors

    //global data store in memory as vectors for HASH-ID MAPPING
    std::vector<std::vector<string>> blockHashIdVector;         //store block hash-id
    std::vector<std::vector<string>> txHashIdVector;            //store tx hash-id
    std::vector<std::vector<string>> keyImageHashIdVector;      //store key-image hash-id
    std::vector<std::vector<string>> saHashIdVector;            //store stealth-address hash-id
    std::map<string,int> rmHashIdVector;                        //store ring-member hash-id

    //global data store in memory as vector for INDEX and DETAILS
    std::vector<json> outputOfBlock;
    std::vector<std::vector<int>> outputIndexOfBlock;            //store block details & indexes

    std::vector<json> outputOfStealthAddresses;
    std::vector<std::vector<int>> outputIndexOfStealthAddresses; //store Stealth-Addresses details & indexes

    std::vector<std::vector<string>> outputOfKeyImages;          //store KeyImages details & indexes

    std::vector<json> outputOfRingMembers;
    std::vector<std::vector<int>> outputIndexOfRingMembers;      //store RingMembers details & indexes

    std::vector<json> outputOfTx;
    std::vector<std::vector<int>> outputIndexOfTx;               //store Tx details & indexes

    void storeLastBlockId(int lastBlockId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastBlockId",to_string(lastBlockId));
        map.close();
    }

    void storeLastTransactionId(int lastTransactionId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastTransactionId",to_string(lastTransactionId));
        map.close();
    }

    void storeLastKeyImageId(int lastKeyImageId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastKeyImageId",to_string(lastKeyImageId));
        map.close();
    }

    void storeLastStealthAddressId(int lastStealthAddressId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastStealthAddressId",to_string(lastStealthAddressId));
        map.close();
    }

    void storeLastRingMemberId(int lastRingMemberId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastRingMemberId",to_string(lastRingMemberId));
        map.close();
    }

    void storeLastOutputId(int lastOutputId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastOutputId",to_string(lastOutputId));
        map.close();
    }

    void storeLastInputId(int lastInputId){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        map.insertKey("lastInputId",to_string(lastInputId));
        map.close();
    }

    string getLastBlockId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastBlockId");
        map.close();
        return val;
    }

    string getLastTransactionId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastTransactionId");
        map.close();
        return val;
    }

    string getLastKeyImageId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastKeyImageId");
        map.close();
        return val;
    }

    string getLastStealthAddressId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastStealthAddressId");
        map.close();
        return val;
    }

    string getLastRingMemberId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastRingMemberId");
        map.close();
        return val;
    }

    string getLastOutputId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastOutputId");
        map.close();
        return val;
    }

    string getLastInputId(){
        hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "lastIds");
        string val = map.getValueFromKey("lastInputId");
        map.close();
        return val;
    }

    json blockDetails(page& monerosci, string blk_height) {
        json jsonResponse  = monerosci.json_block(blk_height);

        return jsonResponse;

    }

    json transactionDetails(page& monerosci, string curTx){
        json jsonResponse  = monerosci.json_transaction(curTx);

        return jsonResponse;
    }

    void getLastIdList(){
        //get last block id
        string last_blk_id = getLastBlockId();
        if(last_blk_id == "null"){
            lastBlockId = 1;
        }else{
            lastBlockId = atoi(last_blk_id.c_str());
        }

        string last_tx_id = getLastTransactionId();
        if(last_tx_id == "null"){
            lastTransactionId = 1;
        }else{
            lastTransactionId = atoi(last_tx_id.c_str());
        }

        string last_sa_id = getLastStealthAddressId();
        if(last_sa_id == "null"){
            lastStealthAddressId = 1;
        }else{
            lastStealthAddressId = atoi(last_sa_id.c_str());
        }

        string last_output_id = getLastOutputId();
        if(last_output_id == "null"){
            lastOutputId = 1;
        }else{
            lastOutputId = atoi(last_output_id.c_str());
        }

        string last_input_id = getLastInputId();
        if(last_input_id == "null"){
            lastInputId = 1;
        }else{
            lastInputId = atoi(last_input_id.c_str());
        }

        string last_ki_id = getLastKeyImageId();
        if(last_ki_id == "null"){
            lastKeyImageId = 1;
        }else{
            lastKeyImageId = atoi(last_ki_id.c_str());
        }

        string last_rm_id = getLastRingMemberId();
        if(last_rm_id == "null"){
            lastRingMemberId = 1;
        }else{
            lastRingMemberId = atoi(last_rm_id.c_str());
        }

    }

    void storeLastIdList(){
        //store last block id
        storeLastBlockId(lastBlockId);
        storeLastTransactionId(lastTransactionId);
        storeLastStealthAddressId(lastStealthAddressId);
        storeLastOutputId(lastOutputId);
        storeLastKeyImageId(lastKeyImageId);
        storeLastRingMemberId(lastRingMemberId);
        storeLastInputId(lastInputId);
    }

#pragma warning(disable:4700)
    void xmrProcessor(page& monerosci,string blk_height){
        //get block details
        json blockData= blockDetails(monerosci,blk_height);
        string blk_hash = blockData.at("hash");

        //store block map of hash-id in vector
        std::vector<string> blockVector = {blk_hash,to_string(lastBlockId)};
        mtxh2.lock();
        blockHashIdVector.push_back(blockVector);
        mtxh2.unlock();

        //store data on block detail vector
        blockData["lastBlockId"] = lastBlockId;
        mtxb1.lock();
        outputOfBlock.push_back(blockData);
        mtxb1.unlock();

        //insert data into the indexes table
        std::vector<int> blockId = {lastBlockId};
        mtxb2.lock();
        outputIndexOfBlock.push_back(blockId);
        mtxb2.unlock();

        /*
         ###################### BLOCK TRANSACTIONS ###########################
         First look at the transaction outputs
         Second look at the transaction input key-images
         Third look at the transaction input ring-members
        */

        int txCount =  blockData.at("txCount");
        json txs = blockData.at("txs");

//        cout << "Include Transaction Count - "<< txCount<<endl;
        for(int x=0;x<txCount;x++){
            string curTx = txs[x]["tx_hash"];
            json transactionData = transactionDetails(monerosci, curTx);

            string txHash = transactionData.at("tx_hash");

            //store tx map of hash-id in vector
            std::vector<string> txVector = {txHash,to_string(lastTransactionId)};
            mtxh3.lock();
            txHashIdVector.push_back(txVector);
            mtxh3.unlock();

            /*
                 ###################### TRANSACTION STEALTH-ADDRESSES ###########################
                 First store hash-id mapping of SA
                 Second store details of SA
                 Third store indexes of SA
            */
            json outputs = transactionData.at("outputs");

            for(int y=0; y < outputs.size(); y++){
                //output public-key
                string sa_hash = outputs[y]["public_key"];

                //store tx map of hash-id in vector
                std::vector<string> saVector = {sa_hash,to_string(lastStealthAddressId)};
                mtxh1.lock();
                saHashIdVector.push_back(saVector);
                mtxh1.unlock();

                //store SA data in SA detail table
                outputs[y]["lastSAId"] = lastOutputId;
                mtx.lock();
                outputOfStealthAddresses.push_back(outputs[y]);
                mtx.unlock();

                //insert data into the indexes table
                std::vector<int> tempIndexData = {lastTransactionId,lastStealthAddressId,lastOutputId,lastBlockId};
                mtx2.lock();
                outputIndexOfStealthAddresses.push_back(tempIndexData);
                mtx2.unlock();

                //increment stealth-address id
                lastStealthAddressId += 1;
                lastOutputId += 1;
            }

            /*
               ###################### TRANSACTION RING-MEMBERS AS INPUTS & KEY-IMAGES ###########################
               First store hash-id mapping of key-image
               Second store details of
               Third store indexes of SA
           */
            json inputs = transactionData.at("inputs");

            for(int y=0; y < inputs.size(); y++){
                //output public-key
                string key_image = inputs[y]["key_image"];

                //store tx map of hash-id in vector
                std::vector<string> keyImageVector = {key_image,to_string(lastKeyImageId)};
                mtxh4.lock();
                keyImageHashIdVector.push_back(keyImageVector);
                mtxh4.unlock();

                //store RM data in RM detail table
                std::vector<string> tempDetailData = {to_string(lastKeyImageId),key_image,to_string(lastBlockId),to_string(lastTransactionId)};
                mtxk.lock();
                outputOfKeyImages.push_back(tempDetailData);
                mtxk.unlock();

                //consider transaction inputs - Ring Members
                json mixins = inputs[y]["mixins"];
                for(int z=0; z < mixins.size(); z++){

                    //ring member public-key
                    int currentRingMemberId;
                    string ring_key_image = mixins[z]["public_key"];

                    //find that ring-member is already in the bloom-filter
                    bool res = bloomFilter.exists(ring_key_image.c_str());

                    if(res){
                        //value is in the bloom-filter
                        int nowId;
                        mtxh5.lock();
                        nowId= rmHashIdVector.find(ring_key_image)->second;
                        mtxh5.unlock();

                        if(nowId){
                            //value is in the memory
                            currentRingMemberId = nowId;
                        }else{
                            //value is in the database
                            string idIsExists = ringMemberHashMap.isKeyExist(ring_key_image);

                            if(idIsExists == "null"){
                                currentRingMemberId = lastRingMemberId;
                            }else{
                                currentRingMemberId = atoi(idIsExists.c_str());
                            }
                        }
                    }else{
                        currentRingMemberId = lastRingMemberId;
                        bloomFilter.add(ring_key_image.c_str());              //add the ring-hash to the bloom filter

                        mtxh5.lock();
                        rmHashIdVector[ring_key_image]=lastRingMemberId;       //add ringMemHash-id mapping to the memory
                        mtxh5.unlock();

                        lastRingMemberId += 1;
                    }

                    //store RM data in RM detail table
                    mixins[z]["lastRingId"] = lastInputId;
                    mtx3.lock();
                    outputOfRingMembers.push_back(mixins[z]);
                    mtx3.unlock();

                    //insert data into the indexes table
                    std::vector<int> tempIndexRingData = {lastInputId,lastTransactionId,lastKeyImageId,currentRingMemberId,lastBlockId};
                    mtx4.lock();
                    outputIndexOfRingMembers.push_back(tempIndexRingData);
                    mtx4.unlock();

                    lastInputId += 1;
                }

                lastKeyImageId += 1;
            }

            //store data on block detail table
            transactionData["lastTxId"] = lastTransactionId;
            mtxt1.lock();
            outputOfTx.push_back(transactionData);
            mtxt1.unlock();

            //insert data into the indexes table
            std::vector<int> txId = {lastTransactionId,lastBlockId,lastTransactionId};
            mtxt2.lock();
            outputIndexOfTx.push_back(txId);
            mtxt2.unlock();

            lastTransactionId += 1;
        }

        lastBlockId += 1;
    }

    void printMap(){
        std::map<std::string, int>::iterator it = rmHashIdVector.begin();

        // Iterate over the map using c++11 range based for loop
        for (std::pair<std::string, int> element : rmHashIdVector) {
            // Accessing KEY from element
            std::string word = element.first;
            // Accessing VALUE from element.
            int count = element.second;
            std::cout << word << " :: " << count << std::endl;
        }

    }

    //ID-HASH THREADS
    void runBlockIdHashMappingThread(){
        std::thread::id this_id = std::this_thread::get_id();
        hashmapper::idHashMapperDB map1 ("blockchain-xmr", "hash-id-mapping", "blocks");
        int i = 0;
        while (true){
            if(!blockHashIdVector.empty()){
                std::vector<std::vector<string>> h2;

                mtxh2.lock();
                h2.push_back(blockHashIdVector[0]);
                blockHashIdVector.erase (blockHashIdVector.begin());
                mtxh2.unlock();

                string blk_hash = h2[0][0];
                string lastBlockId = h2[0][1];

                map1.insertKey(blk_hash,lastBlockId);
                i++;
                h2.erase (h2.begin());
            }else if(blockHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map1.close();
        cout <<this_id<< " - BLOCK ID-HASH STORING THREAD COMPLETED. STORED " << i << " HASHES"<< endl;
    }
    void runTxIdHashMappingThread(){
        std::thread::id this_id = std::this_thread::get_id();
        hashmapper::idHashMapperDB map2 ("blockchain-xmr", "hash-id-mapping", "tx");

        int i = 0;
        while (true){
            if(!txHashIdVector.empty()){
                std::vector<std::vector<string>> h3;

                mtxh3.lock();
                h3.push_back(txHashIdVector[0]);
                txHashIdVector.erase (txHashIdVector.begin());
                mtxh3.unlock();

                string tx_hash = h3[0][0];
                string lastTransactionId = h3[0][1];

                map2.insertKey(tx_hash,lastTransactionId);
                i++;
                h3.erase (h3.begin());
            }else if(txHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map2.close();
        cout <<this_id<< " - TX ID-HASH STORING THREAD COMPLETED. STORED " << i << " HASHES" << endl;
    }
    void runKeyImageIdHashMappingThread(){
        std::thread::id this_id = std::this_thread::get_id();
        hashmapper::idHashMapperDB map3 ("blockchain-xmr", "hash-id-mapping", "key-images");
        int i = 0;

        while (true){
            if(!keyImageHashIdVector.empty()){
                std::vector<std::vector<string>> h4;

                mtxh4.lock();
                h4.push_back(keyImageHashIdVector[0]);
                keyImageHashIdVector.erase (keyImageHashIdVector.begin());
                mtxh4.unlock();

                string ki_hash = h4[0][0];
                string lastKIId = h4[0][1];

                map3.insertKey(ki_hash,lastKIId);
                i++;
                h4.erase (h4.begin());
            }else if(keyImageHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map3.close();
        cout <<this_id << " - KEY-IMAGE ID-HASH STORING THREAD COMPLETED. STORED " <<i << " HASHES"<< endl;
    }
    void runSAIdHashMappingThread(){
        std::thread::id this_id = std::this_thread::get_id();
        hashmapper::idHashMapperDB map4 ("blockchain-xmr", "hash-id-mapping", "stealth-address");
        int i=0;

        while (true){
            if(!saHashIdVector.empty()){
                std::vector<std::vector<string>> h1;

                mtxh1.lock();
                h1.push_back(saHashIdVector[0]);
                saHashIdVector.erase (saHashIdVector.begin());
                mtxh1.unlock();
                string sa_hash = h1[0][0];
                string lastSAId = h1[0][1];

                map4.insertKey(sa_hash,lastSAId);
                h1.erase(h1.begin());
                i++;
            }else if(saHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map4.close();
        cout << this_id << " - STEALTH-ADDRESS ID-HASH STORING THREAD COMPLETED. STORED " <<i << " HASHES" << endl;
    }
    void runRMIdHashMappingThread(){
        std::thread::id this_id = std::this_thread::get_id();
        int i = 0;
        while (true){
            if(!rmHashIdVector.empty()){
                string rm_hash;
                int lastRMId;
                mtxh5.lock();
                rm_hash = rmHashIdVector.begin()->first;
                lastRMId = rmHashIdVector.begin()->second;
                rmHashIdVector.erase(rmHashIdVector.begin());
                mtxh5.unlock();

                ringMemberHashMap.insertKey(rm_hash,to_string(lastRMId));
                i++;
            }else if(rmHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        //map5.close();
        cout <<this_id << " - RING-MEMBER ID-HASH STORING THREAD COMPLETED. STORED " <<i<< " HASHES" << endl;
    }

    //DETAILS THREADS
    void runBlockDataStoreThread(){
        std::thread::id this_id = std::this_thread::get_id();
        indexMapper::indexes dbConnectionBlock ("blockchain-xmr", "monero-data", "blocks" ,true);    //init database instance globally
        int i = 0;
        //create and store block indexes table
        dbConnectionBlock.createTable(blockDetailsTable);

        while (true){
            if(!outputOfBlock.empty()){
                std::vector<json> bData;

                mtxb1.lock();
                bData.push_back(outputOfBlock[0]);
                outputOfBlock.erase(outputOfBlock.begin());
                mtxb1.unlock();

                dbConnectionBlock.insertBlockData(bData[0]);
                i++;
                bData.erase(bData.begin());
            }else if(outputOfBlock.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionBlock.close();
        cout <<this_id<< " - BLOCK DATA STORING THREAD COMPLETED. STORED " << i<<" BLOCKS" << endl;
    }

    void runTxDataStoreThread(){
        std::thread::id this_id = std::this_thread::get_id();
        indexMapper::indexes dbConnectionTx ("blockchain-xmr", "monero-data", "tx",true);    //init database instance globally
        int i =0;

        //create data table for tx details
        dbConnectionTx.createTable(txDetailsTable);

        while (true){
            if(!outputOfTx.empty()){
                std::vector<json> tData;

                mtxt1.lock();
                tData.push_back(outputOfTx[0]);
                outputOfTx.erase(outputOfTx.begin());
                mtxt1.unlock();

                dbConnectionTx.insertTxData(tData[0]);
                i++;
                tData.erase(tData.begin());
            }else if(outputOfTx.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionTx.close();

        cout <<this_id << " - TX DATA STORING THREAD COMPLETED. STORED " <<i<<" TXES"<< endl;
    }

    void runSaDataStoreThread1(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_ids = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA ("blockchain-xmr", "monero-data","stealth-address-1" ,true );    //init database instance globally
        int i = 0;

        //create data table for tx details
        dbConnectionSA.createTable(saDetailTable);

        while (true){
            std::vector<json> temp;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp.empty()){
                dbConnectionSA.insertSAData(temp[0]);
                temp.erase(temp.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA.close();
        cout << this_ids <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread2(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids2 = std::this_thread::get_id();
        //creating sqlite connection
        indexMapper::indexes dbConnectionSA1 ("blockchain-xmr", "monero-data","stealth-address-2" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA1.createTable(saDetailTable);

        while (true){
            std::vector<json> temp2;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp2.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp2.empty()){
                dbConnectionSA1.insertSAData(temp2[0]);
                temp2.erase(temp2.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids2<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSA1.close();
        cout << this_ids2 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread3(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA3 ("blockchain-xmr", "monero-data","stealth-address-3" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA3.createTable(saDetailTable);

        while (true){
            std::vector<json> temp3;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp3.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp3.empty()){
                dbConnectionSA3.insertSAData(temp3[0]);
                temp3.erase(temp3.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids3<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }


        }
        dbConnectionSA3.close();
        cout << this_ids3 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread4(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-4" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread5(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-5" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread6(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-6" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread7(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-7" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread8(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-8" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread9(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-9" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }
    void runSaDataStoreThread10(){
        time_t Start,End;
        bool oneTime = true;

        int i=0;
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-10" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            std::vector<json> temp4;

            mtx.lock();
            if(!outputOfStealthAddresses.empty()){
                temp4.push_back(outputOfStealthAddresses[0]);
                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
            }
            mtx.unlock();

            if(!temp4.empty()){
                dbConnectionSA4.insertSAData(temp4[0]);
                temp4.erase(temp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_ids4<<" - STORED STEALTH-ADDRESS DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }

        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED. STORED " <<i << " STEALTH-ADDRESSES" << endl;
    }

    void runKeyImageDataStoreThread(){
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionKI ("blockchain-xmr", "monero-data", "key-image",true);    //init database instance globally
        int i = 0;
        //create data table for tx details
        dbConnectionKI.createTable(keyImageTable);

        while (true){
            if(!outputOfKeyImages.empty()){
                std::vector<std::vector<string>> kData;

                mtxk.lock();
                kData.push_back(outputOfKeyImages[0]);
                outputOfKeyImages.erase(outputOfKeyImages.begin());
                mtxk.unlock();

                dbConnectionKI.insertKIData(kData[0]);
                i++;
                kData.erase(kData.begin());
            }else if(outputOfKeyImages.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionKI.close();

        cout <<this_ids4<< " - KEY-IMAGE DATA STORING THREAD COMPLETED. STORED " <<i << " KEY-IMAGES" << endl;
    }

    void runRingMemberStoreThread1(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_id_In = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM ("blockchain-xmr", "monero-data", "ring-members-1",true);    //init database instance globally
        int i = 0;

        //create data table for tx details
        dbConnectionRM.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp.empty()){
                dbConnectionRM.insertRMData(tempInp[0]);
                tempInp.erase(tempInp.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }
            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM.close();
        cout << this_id_In <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread2(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_id_In2 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM2 ("blockchain-xmr", "monero-data", "ring-members-2",true);    //init database instance globally
        int i = 0;

        //create data table for tx details
        dbConnectionRM2.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp2;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp2.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp2.empty()){
                dbConnectionRM2.insertRMData(tempInp2[0]);
                tempInp2.erase(tempInp2.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In2<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM2.close();
        cout << this_id_In2 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread3(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_id_In3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM3 ("blockchain-xmr", "monero-data", "ring-members-3",true);    //init database instance globally
        int i = 0;

        //create data table for tx details
        dbConnectionRM3.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp3;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp3.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp3.empty()){
                dbConnectionRM3.insertRMData(tempInp3[0]);
                tempInp3.erase(tempInp3.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In3<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM3.close();
        cout << this_id_In3 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread4(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-4",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread5(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-5",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread6(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-6",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread7(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-7",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread8(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-8",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread9(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-9",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }
    void runRingMemberStoreThread10(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-10",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            std::vector<json> tempInp4;

            mtx3.lock();
            if(!outputOfRingMembers.empty()){
                tempInp4.push_back(outputOfRingMembers[0]);
                outputOfRingMembers.erase(outputOfRingMembers.begin());
            }
            mtx3.unlock();

            if(!tempInp4.empty()){
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id_In4<<" - STORED RING-MEMBER DATA COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED. STORED " <<i << " RING-MEMBERS" << endl;
    }

    //INDEX THREADS
    void runBlockIndexStoreThread(){
        int i = 0;
        std::thread::id this_ids = std::this_thread::get_id();
        indexMapper::indexes dbConnectionBlockIndex ("blockchain-xmr", "monero-indexes", "blocks",true);    //init database instance globally
        dbConnectionBlockIndex.createTable(blockIndexTable);

        while (true){
            if(!outputIndexOfBlock.empty()){
                std::vector<std::vector<int>> bIndex;

                mtxb2.lock();
                bIndex.push_back(outputIndexOfBlock[0]);
                outputIndexOfBlock.erase(outputIndexOfBlock.begin());
                mtxb2.unlock();

                dbConnectionBlockIndex.insertBlockIndex(bIndex[0]);
                i++;
                bIndex.erase(bIndex.begin());
            }else if(outputIndexOfBlock.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionBlockIndex.close();

        cout << this_ids << " - BLOCK INDEX STORING THREAD COMPLETED. STORED " <<i << " BLOCK INDEXES" << endl;
    }

    void runTxIndexStoreThread(){
        std::thread::id this_ids = std::this_thread::get_id();
        indexMapper::indexes dbConnectionTxIndex ("blockchain-xmr", "monero-indexes", "tx",true);    //init database instance globally
        dbConnectionTxIndex.createTable(txIndexTable);
        int i = 0;

        while (true){
            if(!outputIndexOfTx.empty()){
                std::vector<std::vector<int>> tIndex;

                mtxt2.lock();
                tIndex.push_back(outputIndexOfTx[0]);
                outputIndexOfTx.erase(outputIndexOfTx.begin());
                mtxt2.unlock();

                dbConnectionTxIndex.insertTxIndex(tIndex[0]);
                i++;
                tIndex.erase(tIndex.begin());
            }else if(outputIndexOfTx.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionTxIndex.close();

        cout <<this_ids << " - TX INDEX STORING THREAD COMPLETED. STORED " <<i << " TX INDEXES" << endl;
    }

    void runSAIndexStoreThread1(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id1 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex ("blockchain-xmr", "monero-indexes", "stealth-address-1",true);    //init database instance globally
        dbConnectionSAIndex.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());

            }
            mtx2.unlock();

            if(!tempAr.empty()){
                dbConnectionSAIndex.insertSAIndex(tempAr[0]);
                tempAr.erase(tempAr.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id1<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex.close();
        cout << this_id1 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread2(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id2 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex2 ("blockchain-xmr", "monero-indexes", "stealth-address-2",true);    //init database instance globally
        dbConnectionSAIndex2.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr2;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr2.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr2.empty()){
                dbConnectionSAIndex2.insertSAIndex(tempAr2[0]);
                tempAr2.erase(tempAr2.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id2<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex2.close();
        cout << this_id2 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread3(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex3 ("blockchain-xmr", "monero-indexes", "stealth-address-3",true);    //init database instance globally
        dbConnectionSAIndex3.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr3;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr3.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr3.empty()){
                dbConnectionSAIndex3.insertSAIndex(tempAr3[0]);
                tempAr3.erase(tempAr3.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id3<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex3.close();
        cout << this_id3 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread4(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-4",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread5(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-5",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread6(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-6",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread7(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-7",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread8(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-8",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread9(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-9",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }
    void runSAIndexStoreThread10(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-10",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            std::vector<json> tempAr4;

            mtx2.lock();
            if(!outputIndexOfStealthAddresses.empty()){
                tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
            }
            mtx2.unlock();

            if(!tempAr4.empty()){
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
                i++;
                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_id4<<" - STORED STEALTH-ADDRESS INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();
        cout << this_id4 <<" - SA INDEX STORING THREAD COMPLETED. STORED " <<i << " SA INDEXES" << endl;
    }

    void runRMIndexStoreThread1(){
        time_t Start,End;
        bool oneTime = true;

        int i = 0;
        std::thread::id this_idRI1 = std::this_thread::get_id();

        indexMapper::indexes dbConnectionRMIndex ("blockchain-xmr", "monero-indexes", "ring-members-1",true);    //init database instance globally
        dbConnectionRMIndex.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI.empty()){
                dbConnectionRMIndex.insertRMIndex(tempArRI[0]);
                tempArRI.erase(tempArRI.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI1<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex.close();
        cout << this_idRI1 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread2(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI2 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex2 ("blockchain-xmr", "monero-indexes", "ring-members-2",true);    //init database instance globally
        dbConnectionRMIndex2.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI2;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI2.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI2.empty()){
                dbConnectionRMIndex2.insertRMIndex(tempArRI2[0]);
                tempArRI2.erase(tempArRI2.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI2<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex2.close();
        cout << this_idRI2 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread3(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI3 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex3 ("blockchain-xmr", "monero-indexes", "ring-members-3",true);    //init database instance globally
        dbConnectionRMIndex3.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI3;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI3.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI3.empty()){
                dbConnectionRMIndex3.insertRMIndex(tempArRI3[0]);
                tempArRI3.erase(tempArRI3.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI3<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex3.close();
        cout << this_idRI3 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread4(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-4",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread5(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-5",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread6(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-6",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread7(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-7",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread8(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-8",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread9(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-9",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }
    void runRMIndexStoreThread10(){
        time_t Start,End;
        bool oneTime = true;

        std::thread::id this_idRI4 = std::this_thread::get_id();
        int i = 0;
        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-10",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            std::vector<json> tempArRI4;

            mtx4.lock();
            if(!outputIndexOfRingMembers.empty()){
                tempArRI4.push_back(outputIndexOfRingMembers[0]);
                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
            }
            mtx4.unlock();

            if(!tempArRI4.empty()){
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());
                i++;

                if(isAllBlockDone){
                    time (& End);
                    if(oneTime){
                        time (& Start);
                        oneTime = false;
                    }
                    double dif= difftime (End, Start);
                    if(dif > 90){
                        cout << this_idRI4<<" - STORED RING-MEMBER INDEX COUNT -" << i <<"/"<<lastStealthAddressId << endl;
                        time (& Start);

                    }
                }
            }

            if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        cout << this_idRI4 <<" - RM INDEX STORING THREAD COMPLETED. STORED " <<i << " RM INDEXES" << endl;
    }

    //MAIN FUNCTION
    void mainFunction(page& monerosci, uint64_t current_blockchain_height){
        //load last ids to global variables
        getLastIdList();

        for(int x = 1; x < 1000; x++){
            xmrProcessor(monerosci, to_string(x));
            cout << "MoneroSci-parser has parsed blockchain data of "+ std::to_string(x)+ "/" << current_blockchain_height << endl;

            if((x%1000) == 0){
                cout << x << " - Data loaded to the main memory. Main process going to sleep 60 seconds"<<endl;
                sleep(60);
            }

            if((x%10000) == 0){
                cout << x << " - Data loaded to the main memory. Main process going to sleep 100 seconds"<<endl;
                sleep(100);
            }
        }

        //store the last id in rocksdb hash-id mapping
        storeLastIdList();
        ringMemberHashMap.close();

        cout << "" << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "BLOCKCHAIN DATA HAS LOADED TO THE MEMORY" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << endl;
        cout << "TOTAL BLOCKS            -" << lastBlockId -1 << endl;
        cout << "TOTAL TRANSACTIONS      -" << lastTransactionId -1 << endl;
        cout << "TOTAL KEY-IMAGES        -" << lastKeyImageId -1 << endl;
        cout << "TOTAL STEALTH ADDRESSES -" << lastStealthAddressId -1 << endl;
        cout << "TOTAL RING MEMBERS      -" << lastRingMemberId -1 <<"  (WITHOUT DUPLICATES)"<< endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "" << endl;

        isAllBlockDone = true;

    }
}



























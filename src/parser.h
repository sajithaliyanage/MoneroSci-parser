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
//    indexMapper::indexes dbConnectionSA ("blockchain-xmr", "monero-data", "stealth-address");    //init database instance globally
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
                            hashmapper::idHashMapperDB map ("blockchain-xmr", "hash-id-mapping", "ring-members");
                            string idIsExists = map.isKeyExist(ring_key_image);
                            map.close();

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
        hashmapper::idHashMapperDB map1 ("blockchain-xmr", "hash-id-mapping", "blocks");

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
                h2.erase (h2.begin());
            }else if(blockHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map1.close();
        cout << "BLOCK ID-HASH STORING THREAD COMPLETED!" << endl;
    }

    void runTxIdHashMappingThread(){
        hashmapper::idHashMapperDB map2 ("blockchain-xmr", "hash-id-mapping", "tx");

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
                h3.erase (h3.begin());
            }else if(txHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map2.close();
        cout << "TX ID-HASH STORING THREAD COMPLETED!" << endl;
    }

    void runKeyImageIdHashMappingThread(){
        hashmapper::idHashMapperDB map3 ("blockchain-xmr", "hash-id-mapping", "key-images");

        while (true){
            if(!keyImageHashIdVector.empty()){
                std::vector<std::vector<string>> h4;

                mtxh4.lock();
                h4.push_back(keyImageHashIdVector[0]);
                keyImageHashIdVector.erase (keyImageHashIdVector.begin());
                mtxh4.unlock();

                string ki_hash = keyImageHashIdVector[0][0];
                string lastKIId = keyImageHashIdVector[0][1];

                map3.insertKey(ki_hash,lastKIId);
                h4.erase (h4.begin());
            }else if(keyImageHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map3.close();
        cout << "KEY-IMAGE ID-HASH STORING THREAD COMPLETED!" << endl;
    }

    void runSAIdHashMappingThread(){
        hashmapper::idHashMapperDB map4 ("blockchain-xmr", "hash-id-mapping", "stealth-address");

        while (true){
            if(!saHashIdVector.empty()){
                std::vector<std::vector<string>> h1;

                mtxh1.lock();
                    h1.push_back(saHashIdVector[0]);
                    saHashIdVector.erase (saHashIdVector.begin());
                mtxh1.unlock();
                string sa_hash = h1[0][0];
                string lastSAId = h1[0][1];

                h1.erase(h1.begin());
                map4.insertKey(sa_hash,lastSAId);
            }else if(saHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map4.close();
        cout << "STEALTH-ADDRESS ID-HASH STORING THREAD COMPLETED!" << endl;
    }

    void runRMIdHashMappingThread(){
        hashmapper::idHashMapperDB map5 ("blockchain-xmr", "hash-id-mapping", "ring-members");

        while (true){
            if(!rmHashIdVector.empty()){
                string rm_hash;
                int lastRMId;

                mtxh5.lock();
                rm_hash = rmHashIdVector.begin()->first;
                lastRMId = rmHashIdVector.begin()->second;
                rmHashIdVector.erase(rmHashIdVector.begin());
                mtxh5.unlock();

                map5.insertKey(rm_hash,to_string(lastRMId));
            }else if(rmHashIdVector.empty() && isAllBlockDone){
                break;
            }
        }

        map5.close();
        cout << "RING-MEMBER ID-HASH STORING THREAD COMPLETED!" << endl;
    }

    //DETAILS THREADS
    void runBlockDataStoreThread(){
        indexMapper::indexes dbConnectionBlock ("blockchain-xmr", "monero-data", "blocks" ,true);    //init database instance globally

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
                bData.erase(bData.begin());
            }else if(outputOfBlock.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionBlock.close();
        cout << "BLOCK DATA STORING THREAD COMPLETED!" << endl;
    }

    void runTxDataStoreThread(){
        indexMapper::indexes dbConnectionTx ("blockchain-xmr", "monero-data", "tx",true);    //init database instance globally

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
                tData.erase(tData.begin());
            }else if(outputOfTx.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionTx.close();

        cout << "TX DATA STORING THREAD COMPLETED!" << endl;
    }

    void runSaDataStoreThread1(){
        time_t StartSD1, EndSD1;
        time (& StartSD1);

        std::thread::id this_ids = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA ("blockchain-xmr", "monero-data","stealth-address-1" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA.createTable(saDetailTable);

        while (true){
            if(!outputOfStealthAddresses.empty()){
                std::vector<json> temp;
                mtx.lock();
                    temp.push_back(outputOfStealthAddresses[0]);
                    outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
                mtx.unlock();
                dbConnectionSA.insertSAData(temp[0]);
                temp.erase(temp.begin());
            }else if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSA.close();

        time (& EndSD1);
        double difSD1 = difftime (EndSD1, StartSD1);
        cout << "\n Stealth-address-1 Process Done in " << difSD1 << " second(s)." << endl;

        cout << this_ids <<" - SA DATA STORING THREAD COMPLETED!" << endl;
    }
    void runSaDataStoreThread2(){
        time_t StartSD2, EndSD2;
        time (& StartSD2);

        std::thread::id this_ids2 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA1 ("blockchain-xmr", "monero-data","stealth-address-2" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA1.createTable(saDetailTable);

        while (true){
            if(!outputOfStealthAddresses.empty()){
                std::vector<json> temp2;
                mtx.lock();
                    temp2.push_back(outputOfStealthAddresses[0]);
                    outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
                mtx.unlock();

                dbConnectionSA1.insertSAData(temp2[0]);
                temp2.erase(temp2.begin());
            }else if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSA1.close();

        time (& EndSD2);
        double difSD2 = difftime (EndSD2, StartSD2);
        cout << "\n Stealth-address-2 Process Done in " << difSD2 << " second(s)." << endl;
        cout << this_ids2 <<" - SA DATA STORING THREAD COMPLETED!" << endl;
    }
    void runSaDataStoreThread3(){

        std::thread::id this_ids3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA3 ("blockchain-xmr", "monero-data","stealth-address-3" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA3.createTable(saDetailTable);

        while (true){
            if(!outputOfStealthAddresses.empty()){
                std::vector<json> temp3;
                mtx.lock();
                    temp3.push_back(outputOfStealthAddresses[0]);
                    outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
                mtx.unlock();

                dbConnectionSA3.insertSAData(temp3[0]);
                temp3.erase(temp3.begin());
            }else if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSA3.close();

        cout << this_ids3 <<" - SA DATA STORING THREAD COMPLETED!" << endl;
    }
    void runSaDataStoreThread4(){
        std::thread::id this_ids4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSA4 ("blockchain-xmr", "monero-data","stealth-address-4" ,true );    //init database instance globally

        //create data table for tx details
        dbConnectionSA4.createTable(saDetailTable);

        while (true){
            if(!outputOfStealthAddresses.empty()){
                std::vector<json> temp1;
                mtx.lock();
                    temp1.push_back(outputOfStealthAddresses[0]);
                    outputOfStealthAddresses.erase(outputOfStealthAddresses.begin());
                mtx.unlock();

                dbConnectionSA4.insertSAData(temp1[0]);
                temp1.erase(temp1.begin());
            }else if(outputOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSA4.close();

        cout << this_ids4 <<" - SA DATA STORING THREAD COMPLETED!" << endl;
    }

    void runKeyImageDataStoreThread(){
        indexMapper::indexes dbConnectionKI ("blockchain-xmr", "monero-data", "key-image",true);    //init database instance globally

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
                kData.erase(kData.begin());
            }else if(outputOfKeyImages.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionKI.close();

        cout << "KEY-IMAGE DATA STORING THREAD COMPLETED!" << endl;
    }

    void runRingMemberStoreThread1(){
        time_t StartI1, EndI1;
        time (& StartI1);

        std::thread::id this_id_In = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM ("blockchain-xmr", "monero-data", "ring-members-1",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM.createTable(rmDetailTable);

        while (true){
            if(!outputOfRingMembers.empty()){
                std::vector<json> tempInp;
                mtx3.lock();
                    tempInp.push_back(outputOfRingMembers[0]);
                    outputOfRingMembers.erase(outputOfRingMembers.begin());
                mtx3.unlock();
                dbConnectionRM.insertRMData(tempInp[0]);
                tempInp.erase(tempInp.begin());
            }else if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM.close();

        time (& EndI1);
        double difI1 = difftime (EndI1, StartI1);
        cout << "\n RING-MEMBER-1 Process Done in " << difI1 << " second(s)." << endl;
        cout << this_id_In <<" - RING-MEMBER DATA STORING THREAD COMPLETED!" << endl;
    }
    void runRingMemberStoreThread2(){
        time_t StartI2, EndI2;
        time (& StartI2);

        std::thread::id this_id_In2 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM2 ("blockchain-xmr", "monero-data", "ring-members-2",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM2.createTable(rmDetailTable);

        while (true){
            if(!outputOfRingMembers.empty()){
                std::vector<json> tempInp2;
                mtx3.lock();
                    tempInp2.push_back(outputOfRingMembers[0]);
                    outputOfRingMembers.erase(outputOfRingMembers.begin());
                mtx3.unlock();
                dbConnectionRM2.insertRMData(tempInp2[0]);
                tempInp2.erase(tempInp2.begin());
            }else if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM2.close();

        time (& EndI2);
        double difI2 = difftime (EndI2, StartI2);
        cout << "\n RING-MEMBER-2 Process Done in " << difI2 << " second(s)." << endl;
        cout << this_id_In2 <<" - RING-MEMBER DATA STORING THREAD COMPLETED!" << endl;
    }
    void runRingMemberStoreThread3(){
        time_t StartI3, EndI3;
        time (& StartI3);

        std::thread::id this_id_In3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM3 ("blockchain-xmr", "monero-data", "ring-members-3",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM3.createTable(rmDetailTable);

        while (true){
            if(!outputOfRingMembers.empty()){
                std::vector<json> tempInp3;
                mtx3.lock();
                    tempInp3.push_back(outputOfRingMembers[0]);
                    outputOfRingMembers.erase(outputOfRingMembers.begin());
                mtx3.unlock();
                dbConnectionRM3.insertRMData(tempInp3[0]);
                tempInp3.erase(tempInp3.begin());
            }else if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM3.close();

        time (& EndI3);
        double difI3 = difftime (EndI3, StartI3);
        cout << "\n RING-MEMBER-3 Process Done in " << difI3 << " second(s)." << endl;
        cout << this_id_In3 <<" - RING-MEMBER DATA STORING THREAD COMPLETED!" << endl;
    }
    void runRingMemberStoreThread4(){
        time_t StartI4, EndI4;
        time (& StartI4);

        std::thread::id this_id_In4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionRM4 ("blockchain-xmr", "monero-data", "ring-members-4",true);    //init database instance globally

        //create data table for tx details
        dbConnectionRM4.createTable(rmDetailTable);

        while (true){
            if(!outputOfRingMembers.empty()){
                std::vector<json> tempInp4;
                mtx3.lock();
                    tempInp4.push_back(outputOfRingMembers[0]);
                    outputOfRingMembers.erase(outputOfRingMembers.begin());
                mtx3.unlock();
                dbConnectionRM4.insertRMData(tempInp4[0]);
                tempInp4.erase(tempInp4.begin());
            }else if(outputOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRM4.close();

        time (& EndI4);
        double difI4 = difftime (EndI4, StartI4);
        cout << "\n RING-MEMBER-4 Process Done in " << difI4 << " second(s)." << endl;
        cout << this_id_In4 <<" - RING-MEMBER DATA STORING THREAD COMPLETED!" << endl;
    }

    //INDEX THREADS
    void runBlockIndexStoreThread(){
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
                bIndex.erase(bIndex.begin());
            }else if(outputIndexOfBlock.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionBlockIndex.close();

        cout << "BLOCK INDEX STORING THREAD COMPLETED!" << endl;
    }

    void runTxIndexStoreThread(){
        indexMapper::indexes dbConnectionTxIndex ("blockchain-xmr", "monero-indexes", "tx",true);    //init database instance globally
        dbConnectionTxIndex.createTable(txIndexTable);

        while (true){
            if(!outputIndexOfTx.empty()){
                std::vector<std::vector<int>> tIndex;

                mtxt2.lock();
                tIndex.push_back(outputIndexOfTx[0]);
                outputIndexOfTx.erase(outputIndexOfTx.begin());
                mtxt2.unlock();

                dbConnectionTxIndex.insertTxIndex(tIndex[0]);
                tIndex.erase(tIndex.begin());
            }else if(outputIndexOfTx.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionTxIndex.close();

        cout << "TX INDEX STORING THREAD COMPLETED!" << endl;
    }

    void runSAIndexStoreThread1(){
        time_t StartS1, EndS1;
        time (& StartS1);

        std::thread::id this_id1 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex ("blockchain-xmr", "monero-indexes", "stealth-address-1",true);    //init database instance globally
        dbConnectionSAIndex.createTable(saIndexTable);

        while (true){
            if(!outputIndexOfStealthAddresses.empty()){
                std::vector<json> tempAr;
                mtx2.lock();
                tempAr.push_back(outputIndexOfStealthAddresses[0]);
                    outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
                mtx2.unlock();
                dbConnectionSAIndex.insertSAIndex(tempAr[0]);
                tempAr.erase(tempAr.begin());
            }else if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex.close();

        time (& EndS1);
        double difS1 = difftime (EndS1, StartS1);
        cout << "\n Stealth-address-Index-1 Process Done in " << difS1 << " second(s)." << endl;
        cout << this_id1 <<"SA INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runSAIndexStoreThread2(){
        time_t StartS2, EndS2;
        time (& StartS2);

        std::thread::id this_id2 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex2 ("blockchain-xmr", "monero-indexes", "stealth-address-2",true);    //init database instance globally
        dbConnectionSAIndex2.createTable(saIndexTable);

        while (true){
            if(!outputIndexOfStealthAddresses.empty()){
                std::vector<json> tempAr2;
                mtx2.lock();
                tempAr2.push_back(outputIndexOfStealthAddresses[0]);
                    outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
                mtx2.unlock();
                dbConnectionSAIndex2.insertSAIndex(tempAr2[0]);
                tempAr2.erase(tempAr2.begin());
            }else if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex2.close();

        time (& EndS2);
        double difS2 = difftime (EndS2, StartS2);
        cout << "\n Stealth-address-Index-2 Process Done in " << difS2 << " second(s)." << endl;
        cout << this_id2 <<"SA INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runSAIndexStoreThread3(){
        time_t StartS3, EndS3;
        time (& StartS3);

        std::thread::id this_id3 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex3 ("blockchain-xmr", "monero-indexes", "stealth-address-3",true);    //init database instance globally
        dbConnectionSAIndex3.createTable(saIndexTable);

        while (true){
            if(!outputIndexOfStealthAddresses.empty()){
                std::vector<json> tempAr3;
                mtx2.lock();
                    tempAr3.push_back(outputIndexOfStealthAddresses[0]);
                    outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
                mtx2.unlock();
                dbConnectionSAIndex3.insertSAIndex(tempAr3[0]);
                tempAr3.erase(tempAr3.begin());
            }else if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex3.close();

        time (& EndS3);
        double difS3 = difftime (EndS3, StartS3);
        cout << "\n Stealth-address-Index-3 Process Done in " << difS3 << " second(s)." << endl;
        cout << this_id3 <<"SA INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runSAIndexStoreThread4(){
        time_t StartS4, EndS4;
        time (& StartS4);

        std::thread::id this_id4 = std::this_thread::get_id();
        indexMapper::indexes dbConnectionSAIndex4 ("blockchain-xmr", "monero-indexes", "stealth-address-4",true);    //init database instance globally
        dbConnectionSAIndex4.createTable(saIndexTable);

        while (true){
            if(!outputIndexOfStealthAddresses.empty()){
                std::vector<json> tempAr4;
                mtx2.lock();
                    tempAr4.push_back(outputIndexOfStealthAddresses[0]);
                    outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin());
                mtx2.unlock();
                dbConnectionSAIndex4.insertSAIndex(tempAr4[0]);
                tempAr4.erase(tempAr4.begin());
            }else if(outputIndexOfStealthAddresses.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionSAIndex4.close();

        time (& EndS4);
        double difS4 = difftime (EndS4, StartS4);
        cout << "\n Stealth-address-Index-4 Process Done in " << difS4 << " second(s)." << endl;
        cout << this_id4 <<"SA INDEX STORING THREAD COMPLETED!" << endl;
    }

    void runRMIndexStoreThread1(){
        time_t StartRI1, EndRI1;
        time (& StartRI1);

        std::thread::id this_idRI1 = std::this_thread::get_id();

        indexMapper::indexes dbConnectionRMIndex ("blockchain-xmr", "monero-indexes", "ring-members-1",true);    //init database instance globally
        dbConnectionRMIndex.createTable(rmIndexTable);

        while (true){
            if(!outputIndexOfRingMembers.empty()){
                std::vector<json> tempArRI;
                mtx4.lock();
                    tempArRI.push_back(outputIndexOfRingMembers[0]);
                    outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
                mtx4.unlock();
                dbConnectionRMIndex.insertRMIndex(tempArRI[0]);
                tempArRI.erase(tempArRI.begin());

            }else if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex.close();

        time (& EndRI1);
        double difRI1 = difftime (EndRI1, StartRI1);
        cout << "\n Ring Member-Index-1 Process Done in " << difRI1 << " second(s)." << endl;
        cout << this_idRI1 <<" RM INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runRMIndexStoreThread2(){
        time_t StartRI2, EndRI2;
        time (& StartRI2);

        std::thread::id this_idRI2 = std::this_thread::get_id();

        indexMapper::indexes dbConnectionRMIndex2 ("blockchain-xmr", "monero-indexes", "ring-members-2",true);    //init database instance globally
        dbConnectionRMIndex2.createTable(rmIndexTable);

        while (true){
            if(!outputIndexOfRingMembers.empty()){
                std::vector<json> tempArRI2;
                mtx4.lock();
                    tempArRI2.push_back(outputIndexOfRingMembers[0]);
                    outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
                mtx4.unlock();
                dbConnectionRMIndex2.insertRMIndex(tempArRI2[0]);
                tempArRI2.erase(tempArRI2.begin());

            }else if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex2.close();

        time (& EndRI2);
        double difRI2 = difftime (EndRI2, StartRI2);
        cout << "\n Ring Member-Index-2 Process Done in " << difRI2 << " second(s)." << endl;
        cout << this_idRI2 <<" RM INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runRMIndexStoreThread3(){
        time_t StartRI3, EndRI3;
        time (& StartRI3);

        std::thread::id this_idRI3 = std::this_thread::get_id();

        indexMapper::indexes dbConnectionRMIndex3 ("blockchain-xmr", "monero-indexes", "ring-members-3",true);    //init database instance globally
        dbConnectionRMIndex3.createTable(rmIndexTable);

        while (true){
            if(!outputIndexOfRingMembers.empty()){
                std::vector<json> tempArRI3;
                mtx4.lock();
                    tempArRI3.push_back(outputIndexOfRingMembers[0]);
                    outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
                mtx4.unlock();
                dbConnectionRMIndex3.insertRMIndex(tempArRI3[0]);
                tempArRI3.erase(tempArRI3.begin());

            }else if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex3.close();

        time (& EndRI3);
        double difRI3 = difftime (EndRI3, StartRI3);
        cout << "\n Ring Member-Index-3 Process Done in " << difRI3 << " second(s)." << endl;
        cout << this_idRI3 <<" RM INDEX STORING THREAD COMPLETED!" << endl;
    }
    void runRMIndexStoreThread4(){
        time_t StartRI4, EndRI4;
        time (& StartRI4);

        std::thread::id this_idRI4 = std::this_thread::get_id();

        indexMapper::indexes dbConnectionRMIndex4 ("blockchain-xmr", "monero-indexes", "ring-members-4",true);    //init database instance globally
        dbConnectionRMIndex4.createTable(rmIndexTable);

        while (true){
            if(!outputIndexOfRingMembers.empty()){
                std::vector<json> tempArRI4;
                mtx4.lock();
                    tempArRI4.push_back(outputIndexOfRingMembers[0]);
                    outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin());
                mtx4.unlock();
                dbConnectionRMIndex4.insertRMIndex(tempArRI4[0]);
                tempArRI4.erase(tempArRI4.begin());

            }else if(outputIndexOfRingMembers.empty() && isAllBlockDone){
                break;
            }
        }
        dbConnectionRMIndex4.close();

        time (& EndRI4);
        double difRI4 = difftime (EndRI4, StartRI4);
        cout << "\n Ring Member-Index-4 Process Done in " << difRI4 << " second(s)." << endl;
        cout << this_idRI4 <<" RM INDEX STORING THREAD COMPLETED!" << endl;
    }

    //MAIN FUNCTION
    void mainFunction(page& monerosci, uint64_t current_blockchain_height){
        //load last ids to global variables
        getLastIdList();

        for(int x = 1; x < 300; x++){
            xmrProcessor(monerosci, to_string(x));
            cout << "MoneroSci-parser has parsed blockchain data of "+ std::to_string(x)+ "/" << current_blockchain_height << endl;

            if((x%100) == 0){
                cout << x << " - Data loaded to the main memory. Main process going to sleep 10 seconds"<<endl;
                std::this_thread::sleep_for (std::chrono::seconds(1));
            }
        }

        //store the last id in rocksdb hash-id mapping
        storeLastIdList();
        isAllBlockDone = true;
//        dbConnection.close();

    }
}



























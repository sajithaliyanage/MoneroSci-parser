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
    std::map<string,string> rmHashIdVector;                        //store ring-member hash-id

    //global data store in memory as vector for INDEX and DETAILS
    std::vector<json> outputOfBlock;
    std::vector<std::vector<string>> outputIndexOfBlock;            //store block details & indexes

    std::vector<json> outputOfStealthAddresses;
    std::vector<std::vector<string>> outputIndexOfStealthAddresses; //store Stealth-Addresses details & indexes

    std::vector<std::vector<string>> outputOfKeyImages;          //store KeyImages details & indexes

    std::vector<json> outputOfRingMembers;
    std::vector<std::vector<string>> outputIndexOfRingMembers;      //store RingMembers details & indexes

    std::vector<json> outputOfTx;
    std::vector<std::vector<string>> outputIndexOfTx;               //store Tx details & indexes

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

    void showCurrentStatus(int currentBlockHeight){
        cout << "" << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << currentBlockHeight << " - BLOCK DATA HAS LOADED TO THE MAIN MEMORY" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << endl;
        cout << "TOTAL BLOCKS            -" << lastBlockId<< endl;
        cout << "TOTAL TRANSACTIONS      -" << lastTransactionId<< endl;
        cout << "TOTAL KEY-IMAGES        -" << lastKeyImageId<< endl;
        cout << "TOTAL STEALTH ADDRESSES -" << lastStealthAddressId<< endl;
        cout << "TOTAL RING MEMBERS      -" << lastRingMemberId<<"  (WITHOUT DUPLICATES)"<< endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "" << endl;
    }

    #pragma warning(disable:4700)
    void xmrProcessor(page& monerosci,string blk_height){
        //block_id
        string currentBlockID = blk_height;

        //get block details
        json blockData= blockDetails(monerosci,blk_height);
        string blk_hash = blockData.at("hash");

        //store block map of hash-id in vector
        std::vector<string> blockVector = {blk_hash,currentBlockID};
        mtxh2.lock();
        blockHashIdVector.push_back(blockVector);
        mtxh2.unlock();

        //store data on block detail vector
        blockData["lastBlockId"] = currentBlockID;
        mtxb1.lock();
        outputOfBlock.push_back(blockData);
        mtxb1.unlock();

        //insert data into the indexes table
        std::vector<string> blockId = {currentBlockID};
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
            string currentTxID = currentBlockID +"-"+to_string(x);

            string curTx = txs[x]["tx_hash"];
            json transactionData = transactionDetails(monerosci, curTx);

            string txHash = transactionData.at("tx_hash");

            //store tx map of hash-id in vector
            std::vector<string> txVector = {txHash,currentTxID};
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
                string currentOutputID = currentBlockID+"-"+to_string(x)+"-"+to_string(y);

                //output public-key
                string sa_hash = outputs[y]["public_key"];

                //store tx map of hash-id in vector
                std::vector<string> saVector = {sa_hash,currentOutputID};
                mtxh1.lock();
                saHashIdVector.push_back(saVector);
                mtxh1.unlock();

                //store SA data in SA detail table
                outputs[y]["lastSAId"] = currentOutputID;
                mtx.lock();
                outputOfStealthAddresses.push_back(outputs[y]);
                mtx.unlock();

                //insert data into the indexes table
                std::vector<string> tempIndexData = {currentTxID,currentOutputID,currentOutputID,currentBlockID};
                mtx2.lock();
                outputIndexOfStealthAddresses.push_back(tempIndexData);
                mtx2.unlock();
            }

            /*
               ###################### TRANSACTION RING-MEMBERS AS INPUTS & KEY-IMAGES ###########################
               First store hash-id mapping of key-image
               Second store details of
               Third store indexes of SA
           */
            json inputs = transactionData.at("inputs");

            for(int y=0; y < inputs.size(); y++){
                string currentInputID = currentBlockID+"-"+to_string(x)+"-"+to_string(y);

                //output public-key
                string key_image = inputs[y]["key_image"];

                //store tx map of hash-id in vector
                std::vector<string> keyImageVector = {key_image,currentInputID};
                mtxh4.lock();
                keyImageHashIdVector.push_back(keyImageVector);
                mtxh4.unlock();

                //store RM data in RM detail table
                std::vector<string> tempDetailData = {currentInputID,key_image,currentBlockID,currentTxID};
                mtxk.lock();
                outputOfKeyImages.push_back(tempDetailData);
                mtxk.unlock();

                //consider transaction inputs - Ring Members
                json mixins = inputs[y]["mixins"];
                for(int z=0; z < mixins.size(); z++){
                    string currentRingMemberID = currentBlockID+"-"+to_string(x)+"-"+to_string(y)+"-"+to_string(z);

                    //ring member public-key
                    string currentRingMemberId_temp;
                    string ring_key_image = mixins[z]["public_key"];

                    //find that ring-member is already in the bloom-filter
                    bool res = bloomFilter.exists(ring_key_image.c_str());

                    if(res){
                        //value is in the bloom-filter
                        int nowId;
                        mtxh5.lock();
                        nowId = rmHashIdVector.count(ring_key_image);
                        mtxh5.unlock();

                        if(nowId){
                            //value is in the memory
                            currentRingMemberId_temp = rmHashIdVector.find(ring_key_image)->second;
                        }else{
                            //value is in the database
                            string idIsExists = ringMemberHashMap.isKeyExist(ring_key_image);

                            if(idIsExists == "null"){
                                currentRingMemberId_temp = currentRingMemberID;
                            }else{
                                currentRingMemberId_temp = idIsExists;
                            }
                        }
                    }else{
                        currentRingMemberId_temp = currentRingMemberID;
                        bloomFilter.add(ring_key_image.c_str());              //add the ring-hash to the bloom filter

                        mtxh5.lock();
                        rmHashIdVector[ring_key_image]=currentRingMemberID;   //add ringMemHash-id mapping to the memory
                        mtxh5.unlock();
                    }

                    //store RM data in RM detail table
                    mixins[z]["lastRingId"] = currentRingMemberID;
                    mtx3.lock();
                    outputOfRingMembers.push_back(mixins[z]);
                    mtx3.unlock();

                    //insert data into the indexes table
                    std::vector<string> tempIndexRingData = {currentRingMemberID,currentTxID,currentInputID,currentRingMemberId_temp,currentBlockID};
                    mtx4.lock();
                    outputIndexOfRingMembers.push_back(tempIndexRingData);
                    mtx4.unlock();
                }
            }

            //store data on block detail table
            transactionData["lastTxId"] = currentTxID;
            mtxt1.lock();
            outputOfTx.push_back(transactionData);
            mtxt1.unlock();

            //insert data into the indexes table
            std::vector<string> txId = {currentTxID,currentBlockID,currentTxID};
            mtxt2.lock();
            outputIndexOfTx.push_back(txId);
            mtxt2.unlock();
        }
    }

    void dataConsumerThread(int all_block_size){
        //create id-hash map connection
        hashmapper::idHashMapperDB mapCon ("blockchain-xmr", "hash-id-mapping", "blocks");
        hashmapper::idHashMapperDB map2 ("blockchain-xmr", "hash-id-mapping", "tx");
        hashmapper::idHashMapperDB map3 ("blockchain-xmr", "hash-id-mapping", "key-images");
        hashmapper::idHashMapperDB map4 ("blockchain-xmr", "hash-id-mapping", "stealth-address");


        //create database connection for blocks
        indexMapper::indexes dbConnectionBlock ("blockchain-xmr", "monero-data", "blocks" ,true);
        indexMapper::indexes dbConnectionTx ("blockchain-xmr", "monero-data", "tx",true);
        indexMapper::indexes dbConnectionKI ("blockchain-xmr", "monero-data", "key-image", true);
        indexMapper::indexes dbConnectionSA ("blockchain-xmr", "monero-data","stealth-address" ,true );
        indexMapper::indexes dbConnectionSAIndex ("blockchain-xmr", "monero-indexes", "stealth-address",true);
        indexMapper::indexes dbConnectionRM ("blockchain-xmr", "monero-data", "ring-members",true);
        indexMapper::indexes dbConnectionRMIndex ("blockchain-xmr", "monero-indexes", "ring-members",true);


        //create block data table and index table
        dbConnectionBlock.createTable(blockDetailsTable);
        dbConnectionBlock.createTable(blockIndexTable);
        dbConnectionTx.createTable(txDetailsTable);
        dbConnectionTx.createTable(txIndexTable);
        dbConnectionKI.createTable(keyImageTable);
        dbConnectionSA.createTable(saDetailTable);
        dbConnectionSAIndex.createTable(saIndexTable);
        dbConnectionRM.createTable(rmDetailTable);
        dbConnectionRMIndex.createTable(rmIndexTable);

        int blockHash,blockData,blockIndex = 0;
        std::vector<json> bData;
        std::vector<std::vector<string>> bIndex;

        int txHash,txData,txIndex = 0;
        std::vector<json> tData;
        std::vector<std::vector<string>> tIndex;

        int keyImageData,keyImageHash = 0;
        std::vector<std::vector<string>> kData;

        int saHash,saData,saIndex =0;
        std::vector<json> temp;
        std::vector<std::vector<string>> tempAr;

        int rmHash,rmData,rmIndex = 0;
        std::vector<json> tempInp;
        std::vector<std::vector<string>> tempArRI;

        while (true){
            //######################################################-----BLOCK-THREAD-START-------#############################
            if(!blockHashIdVector.empty()){
                std::vector<std::vector<string>> h2;

                mtxh2.lock();
                h2.push_back(blockHashIdVector[0]);
                blockHashIdVector.erase (blockHashIdVector.begin());
                mtxh2.unlock();

                string blk_hash = h2[0][0];
                string lastBlockId = h2[0][1];

                mapCon.insertKey(blk_hash,lastBlockId);
                h2.erase (h2.begin());
                blockHash++;
            }

            //check block-data store
            mtxb1.lock();
            if(outputOfBlock.size() >= 1000){
                long currentSize = outputOfBlock.size();
                bData = outputOfBlock;
                blockData += currentSize;

                outputOfBlock.erase(outputOfBlock.begin(),outputOfBlock.begin()+currentSize);
            }else if(outputOfBlock.size() < 1000 & isAllBlockDone){
                long currentSize = outputOfBlock.size();
                bData = outputOfBlock;
                blockData += currentSize;

                outputOfBlock.erase(outputOfBlock.begin(),outputOfBlock.begin()+currentSize);
            }
            mtxb1.unlock();

            if(!bData.empty()){
                dbConnectionBlock.insertBlockData(bData);
                bData.clear();
            }

            //chech block-index store
            mtxb2.lock();
            if(outputIndexOfBlock.size() >= 1000){
                long currentSize = outputIndexOfBlock.size();
                bIndex = outputIndexOfBlock;
                blockIndex += currentSize;

                outputIndexOfBlock.erase(outputIndexOfBlock.begin(),outputIndexOfBlock.begin()+currentSize);

            }else if(outputIndexOfBlock.size() < 1000 & isAllBlockDone){
                long currentSize = outputIndexOfBlock.size();
                bIndex = outputIndexOfBlock;
                blockIndex += currentSize;

                outputIndexOfBlock.erase(outputIndexOfBlock.begin(),outputIndexOfBlock.begin()+currentSize);
            }
            mtxb2.unlock();

            if(!bIndex.empty()){
                dbConnectionBlock.insertBlockIndex(bIndex);
                bIndex.clear();
            }

            //######################################################-----TX-THREAD-START-------#############################
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
                txHash++;
            }

            //check tx-data store
            mtxt1.lock();
            if(outputOfTx.size() >= 1000){
                long currentSize = outputOfTx.size();
                tData = outputOfTx;
                txData += currentSize;

                outputOfTx.erase(outputOfTx.begin(),outputOfTx.begin()+currentSize);

            }else if(outputOfTx.size() < 1000 & isAllBlockDone){
                long currentSize = outputOfTx.size();
                tData = outputOfTx;
                txData += currentSize;

                outputOfTx.erase(outputOfTx.begin(),outputOfTx.begin()+currentSize);
            }
            mtxt1.unlock();

            if(!tData.empty()){
                dbConnectionTx.insertTxData(tData);
                tData.clear();
            }

            //check tx-index store
            mtxt2.lock();
            if(outputIndexOfTx.size() >= 1000){
                long currentSize = outputIndexOfTx.size();
                tIndex = outputIndexOfTx;
                txIndex += currentSize;

                outputIndexOfTx.erase(outputIndexOfTx.begin(),outputIndexOfTx.begin()+currentSize);

            }else if(outputIndexOfTx.size() < 1000 & isAllBlockDone){
                long currentSize = outputIndexOfTx.size();
                tIndex = outputIndexOfTx;
                txIndex += currentSize;

                outputIndexOfTx.erase(outputIndexOfTx.begin(),outputIndexOfTx.begin()+currentSize);
            }
            mtxt2.unlock();

            if(!tIndex.empty()){
                dbConnectionTx.insertTxIndex(tIndex);
                tIndex.clear();
            }

            //######################################################-----KEY-IMAGE-THREAD-START-------#############################
            if(!keyImageHashIdVector.empty()){
                std::vector<std::vector<string>> h4;

                mtxh4.lock();
                h4.push_back(keyImageHashIdVector[0]);
                keyImageHashIdVector.erase (keyImageHashIdVector.begin());
                mtxh4.unlock();

                string ki_hash = h4[0][0];
                string lastKIId = h4[0][1];

                map3.insertKey(ki_hash,lastKIId);
                h4.erase (h4.begin());
                keyImageHash++;

            }

            //check key-image data store
            mtxk.lock();
            if(outputOfKeyImages.size() >= 1000){
                long currentSize = outputOfKeyImages.size();
                kData = outputOfKeyImages;
                keyImageData += currentSize;

                outputOfKeyImages.erase(outputOfKeyImages.begin(),outputOfKeyImages.begin()+currentSize);

            }else if(outputOfKeyImages.size() < 1000 & isAllBlockDone){
                long currentSize = outputOfKeyImages.size();
                kData = outputOfKeyImages;
                keyImageData += currentSize;

                outputOfKeyImages.erase(outputOfKeyImages.begin(),outputOfKeyImages.begin()+currentSize);
            }
            mtxk.unlock();

            if(!kData.empty()){
                dbConnectionKI.insertKIData(kData);
                kData.clear();
            }

            //######################################################-----STEALTH-ADDRESS-THREAD-START-------##########################
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
                saHash++;
            }

            mtx.lock();
            if(outputOfStealthAddresses.size() >= 1000){
                long currentSize = outputOfStealthAddresses.size();
                temp = outputOfStealthAddresses;
                saData += currentSize;

                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin(),outputOfStealthAddresses.begin()+currentSize);
            }else if(outputOfStealthAddresses.size() < 1000 & isAllBlockDone){
                long currentSize = outputOfStealthAddresses.size();
                temp = outputOfStealthAddresses;
                saData += currentSize;

                outputOfStealthAddresses.erase(outputOfStealthAddresses.begin(),outputOfStealthAddresses.begin()+currentSize);
            }
            mtx.unlock();

            if(!temp.empty()){
                dbConnectionSA.insertSAData(temp);
                temp.clear();
            }

            mtx2.lock();
            if(outputIndexOfStealthAddresses.size() >= 1000){
                long currentSize = outputIndexOfStealthAddresses.size();
                tempAr = outputIndexOfStealthAddresses;
                saIndex += currentSize;

                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin(),outputIndexOfStealthAddresses.begin()+currentSize);

            }else if(outputIndexOfStealthAddresses.size() < 1000 & isAllBlockDone){
                long currentSize = outputIndexOfStealthAddresses.size();
                tempAr = outputIndexOfStealthAddresses;
                saIndex += currentSize;

                outputIndexOfStealthAddresses.erase(outputIndexOfStealthAddresses.begin(),outputIndexOfStealthAddresses.begin()+currentSize);
            }
            mtx2.unlock();

            if(!tempAr.empty()){
                dbConnectionSAIndex.insertSAIndex(tempAr);
                tempAr.clear();
            }

            //######################################################-----RING-MEMBER-THREAD-START-------##########################
            if(!rmHashIdVector.empty()){
                string rm_hash;
                string lastRMId;
                mtxh5.lock();
                rm_hash = rmHashIdVector.begin()->first;
                lastRMId = rmHashIdVector.begin()->second;
                rmHashIdVector.erase(rmHashIdVector.begin());
                mtxh5.unlock();

                ringMemberHashMap.insertKey(rm_hash,lastRMId);
                rmHash++;
            }

            mtx3.lock();
            if(outputOfRingMembers.size() >= 1000){
                long currentSize = outputOfRingMembers.size();
                tempInp = outputOfRingMembers;
                rmData += currentSize;

                outputOfRingMembers.erase(outputOfRingMembers.begin(),outputOfRingMembers.begin()+currentSize);

            }else if(outputOfRingMembers.size() < 1000 & isAllBlockDone){
                long currentSize = outputOfRingMembers.size();
                tempInp = outputOfRingMembers;
                rmData += currentSize;

                outputOfRingMembers.erase(outputOfRingMembers.begin(),outputOfRingMembers.begin()+currentSize);
            }
            mtx3.unlock();

            if(!tempInp.empty()){
                dbConnectionRM.insertRMData(tempInp);
                tempInp.clear();
            }

            mtx4.lock();
            if(outputIndexOfRingMembers.size() >= 1000){
                long currentSize = outputIndexOfRingMembers.size();
                tempArRI = outputIndexOfRingMembers;
                rmIndex += currentSize;

                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin(),outputIndexOfRingMembers.begin()+currentSize);

            }else if(outputIndexOfRingMembers.size() < 1000 & isAllBlockDone){
                long currentSize = outputIndexOfRingMembers.size();
                tempArRI = outputIndexOfRingMembers;
                rmIndex += currentSize;

                outputIndexOfRingMembers.erase(outputIndexOfRingMembers.begin(),outputIndexOfRingMembers.begin()+currentSize);
            }
            mtx4.unlock();

            if(!tempArRI.empty()){
                dbConnectionRMIndex.insertRMIndex(tempArRI);
                tempArRI.clear();
            }

            if(outputIndexOfRingMembers.empty() && outputOfRingMembers.empty() && rmHashIdVector.empty() && outputIndexOfStealthAddresses.empty() && outputOfStealthAddresses.empty() && saHashIdVector.empty() && keyImageHashIdVector.empty() && outputOfKeyImages.empty() && blockHashIdVector.empty() && outputOfBlock.empty() && outputIndexOfBlock.empty() && txHashIdVector.empty() && isAllBlockDone && outputOfTx.empty() && outputIndexOfTx.empty()){
                break;
            }
        }


        mapCon.close();
        map2.close();
        map3.close();
        map4.close();
        dbConnectionBlock.close();
        dbConnectionTx.close();
        dbConnectionKI.close();
        dbConnectionSA.close();
        dbConnectionSAIndex.close();
        dbConnectionRM.close();
        dbConnectionRMIndex.close();
        cout <<"BLOCK ID-HASH STORING THREAD COMPLETED. STORED " << blockHash << " BLOCK HASHES" << endl;
        cout <<"BLOCK DATA STORING THREAD COMPLETED. STORED " << blockData << " BLOCK DETAILS" << endl;
        cout <<"BLOCK INDEX STORING THREAD COMPLETED. STORED " << blockIndex << " BLOCK INDEXES" << endl;
        cout <<"TX ID-HASH STORING THREAD COMPLETED. STORED " << txHash << " HASHES" << endl;
        cout <<"TX DATA STORING THREAD COMPLETED. STORED " << txData <<" TXES"<< endl;
        cout <<"TX INDEX STORING THREAD COMPLETED. STORED " << txIndex << " TX INDEXES" << endl;
        cout <<"KEY-IMAGE ID-HASH STORING THREAD COMPLETED. STORED " << keyImageHash << " HASHES"<< endl;
        cout <<"KEY-IMAGE DATA STORING THREAD COMPLETED. STORED " << keyImageData << " KEY-IMAGES" << endl;
        cout <<"STEALTH-ADDRESS ID-HASH STORING THREAD COMPLETED. STORED " << saHash << " HASHES" << endl;
        cout <<"STEALTH-ADDRESS DATA STORING THREAD COMPLETED. STORED " << saData << " STEALTH-ADDRESSES" << endl;
        cout <<"STEALTH-ADDRESS INDEX STORING THREAD COMPLETED. STORED " << saIndex << " SA INDEXES" << endl;
        cout <<"RING-MEMBER ID-HASH STORING THREAD COMPLETED. STORED " << rmHash<< " HASHES" << endl;
        cout <<"RING-MEMBER DATA STORING THREAD COMPLETED. STORED " << rmData << " RING-MEMBERS" << endl;
        cout <<"RM INDEX STORING THREAD COMPLETED. STORED " << rmIndex << " RM INDEXES" << endl;

        lastBlockId = blockData;
        lastTransactionId = txData;
        lastStealthAddressId = saData;
        lastKeyImageId = keyImageData;
        lastRingMemberId = rmData;

        showCurrentStatus(all_block_size);
        ringMemberHashMap.close();
    }

    //MAIN FUNCTION
    void dataProducerFunction(page& monerosci, int start, int end){
        int x;
        for(x = start; x <= end; x++){
            xmrProcessor(monerosci, to_string(x));
            cout << "MoneroSci-parser has parsed blockchain data of "+ std::to_string(x) << " - BLOCK" << endl;
        }
    }

    vector<std::vector<int>> getProducerRangers(int numProducer, uint64_t current_blockchain_height){
        vector<std::vector<int>> ranges;

        int numRanges = current_blockchain_height / numProducer;
        int offset = current_blockchain_height % numProducer;

        for(int x=1; x <= numProducer; x++){
            if(x == numProducer){
                std::vector<int> temp = {numRanges*(x-1)+1,numRanges*x+offset};
                ranges.push_back(temp);
            }else if(x == 1){
                std::vector<int> temp = {1,numRanges*x};
                ranges.push_back(temp);
            }else{
                std::vector<int> temp = {numRanges*(x-1)+1,numRanges*x};
                ranges.push_back(temp);
            }
        }

        return ranges;
    }
}




















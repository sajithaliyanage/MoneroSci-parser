#include "src/page.h"
#include "src/parser.h"
#include "ext/crow/crow.h"
#include "src/CmdLineOptions.h"
#include <thread>
#include <vector>
#include <ctime>

using boost::filesystem::path;

using namespace parse;
using namespace std;

int main(int ac, const char* av[]) {
    // get command line options
    xmreg::CmdLineOptions opts {ac, av};

    auto help_opt = opts.get_option<bool>("help");

    // if help was chosen, display help text and finish
    if (*help_opt)
    {
        return EXIT_SUCCESS;
    }

    auto port_opt                      = opts.get_option<string>("port");
    auto bc_path_opt                   = opts.get_option<string>("bc-path");
    auto deamon_url_opt                = opts.get_option<string>("deamon-url");
    auto ssl_crt_file_opt              = opts.get_option<string>("ssl-crt-file");
    auto ssl_key_file_opt              = opts.get_option<string>("ssl-key-file");
    auto no_blocks_on_index_opt        = opts.get_option<string>("no-blocks-on-index");
    auto testnet_url                   = opts.get_option<string>("testnet-url");
    auto stagenet_url                  = opts.get_option<string>("stagenet-url");
    auto mainnet_url                   = opts.get_option<string>("mainnet-url");
    auto mempool_info_timeout_opt      = opts.get_option<string>("mempool-info-timeout");
    auto mempool_refresh_time_opt      = opts.get_option<string>("mempool-refresh-time");
    auto testnet_opt                   = opts.get_option<bool>("testnet");
    auto stagenet_opt                  = opts.get_option<bool>("stagenet");
    auto enable_key_image_checker_opt  = opts.get_option<bool>("enable-key-image-checker");
    auto enable_output_key_checker_opt = opts.get_option<bool>("enable-output-key-checker");
    auto enable_autorefresh_option_opt = opts.get_option<bool>("enable-autorefresh-option");
    auto enable_pusher_opt             = opts.get_option<bool>("enable-pusher");
    auto enable_js_opt                 = opts.get_option<bool>("enable-js");
    auto enable_mixin_details_opt      = opts.get_option<bool>("enable-mixin-details");
    auto enable_json_api_opt           = opts.get_option<bool>("enable-json-api");
    auto enable_tx_cache_opt           = opts.get_option<bool>("enable-tx-cache");
    auto enable_block_cache_opt        = opts.get_option<bool>("enable-block-cache");
    auto show_cache_times_opt          = opts.get_option<bool>("show-cache-times");
    auto enable_emission_monitor_opt   = opts.get_option<bool>("enable-emission-monitor");

    bool testnet                      {*testnet_opt};
    bool stagenet                     {*stagenet_opt};

    if (testnet && stagenet)
    {
        cerr << "testnet and stagenet cannot be specified at the same time!" << endl;
        return EXIT_FAILURE;
    }

    const cryptonote::network_type nettype = testnet ?
          cryptonote::network_type::TESTNET : stagenet ?
          cryptonote::network_type::STAGENET : cryptonote::network_type::MAINNET;

    bool enable_pusher                {*enable_pusher_opt};
    bool enable_js                    {*enable_js_opt};
    bool enable_key_image_checker     {*enable_key_image_checker_opt};
    bool enable_autorefresh_option    {*enable_autorefresh_option_opt};
    bool enable_output_key_checker    {*enable_output_key_checker_opt};
    bool enable_mixin_details         {*enable_mixin_details_opt};
    bool enable_json_api              {*enable_json_api_opt};
    bool enable_tx_cache              {*enable_tx_cache_opt};
    bool enable_block_cache           {*enable_block_cache_opt};
    bool enable_emission_monitor      {*enable_emission_monitor_opt};
    bool show_cache_times             {*show_cache_times_opt};

    // set  monero log output level
    uint32_t log_level = 0;
    mlog_configure("", true);

    //cast port number in string to uint
    uint16_t app_port = boost::lexical_cast<uint16_t>(*port_opt);

    // cast no_blocks_on_index_opt to uint
    uint64_t no_blocks_on_index = boost::lexical_cast<uint64_t>(*no_blocks_on_index_opt);

    cout << "" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "Welcome to the MoneroSci - Analysing ang Exploring Tool for Monero Blockchain" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "" << endl;

    // get blockchain path
    path blockchain_path;

    if (!xmreg::get_blockchain_path(bc_path_opt, blockchain_path, nettype))
    {
        cerr << "Error getting blockchain path." << endl;
        return EXIT_FAILURE;
    }

    cout << "Connect with Monero blockchain data in path - " << blockchain_path << endl;


    // create instance of our MicroCore
    // and make pointer to the Blockchain
    xmreg::MicroCore mcore;
    cryptonote::Blockchain* core_storage;
    string deamon_url {*deamon_url_opt};
    uint64_t mempool_info_timeout {5000};

    // initialize mcore and core_storage
    if (!xmreg::init_blockchain(blockchain_path.string(), mcore, core_storage,nettype))
    {
        cerr << "Error accessing blockchain." << endl;
        return EXIT_FAILURE;
    }

    xmreg::page monerosci(&mcore,
                          core_storage,
                          deamon_url,
                          nettype,
                          enable_pusher,
                          enable_js,
                          enable_key_image_checker,
                          enable_output_key_checker,
                          enable_autorefresh_option,
                          enable_mixin_details,
                          enable_tx_cache,
                          enable_block_cache,
                          show_cache_times,
                          no_blocks_on_index,
                          mempool_info_timeout,
                          *testnet_url,
                          *stagenet_url,
                          *mainnet_url);


    //get current block-height
    uint64_t current_blockchain_height =  core_storage->get_current_blockchain_height();
    cout << "Current blockchain Height - " << current_blockchain_height << endl;
    // run the crow http server

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "Started Time -" << std::put_time(std::localtime(&now_c), "%c") << '\n';
    cout << "" <<endl;
    time_t Start, End;
    time (& Start);
    //check data in the sqlite3 storage, if not save it in it

    //iterate parser through the blockchain
    auto thread1 = std::thread(parse::mainFunction,std::ref(monerosci),current_blockchain_height);

    auto thread8 = std::thread(parse::runSAIdHashMappingThread);
    auto thread9 = std::thread(parse::runRMIdHashMappingThread);

    auto thread2 = std::thread(parse::runBlockIdHashMappingThread);
    auto thread3 = std::thread(parse::runBlockDataStoreThread);
    auto thread4 = std::thread(parse::runBlockIndexStoreThread);

    auto thread5 = std::thread(parse::runTxIdHashMappingThread);
    auto thread6 = std::thread(parse::runTxDataStoreThread);
    auto thread10 = std::thread(parse::runTxIndexStoreThread);

    auto thread7 = std::thread(parse::runKeyImageIdHashMappingThread);
    auto thread13 = std::thread(parse::runKeyImageDataStoreThread);

    auto thread14 = std::thread(parse::runRingMemberStoreThread1);
    auto thread22 = std::thread(parse::runRingMemberStoreThread2);
    auto thread23 = std::thread(parse::runRingMemberStoreThread3);
    auto thread24 = std::thread(parse::runRingMemberStoreThread4);

    auto thread15 = std::thread(parse::runRMIndexStoreThread1);
    auto thread25 = std::thread(parse::runRMIndexStoreThread2);
    auto thread26 = std::thread(parse::runRMIndexStoreThread3);
    auto thread27 = std::thread(parse::runRMIndexStoreThread4);

    auto thread12 = std::thread(parse::runSAIndexStoreThread1);
    auto thread19 = std::thread(parse::runSAIndexStoreThread2);
    auto thread20 = std::thread(parse::runSAIndexStoreThread3);
    auto thread21 = std::thread(parse::runSAIndexStoreThread4);

    auto thread11 = std::thread(parse::runSaDataStoreThread1);
    auto thread16 = std::thread(parse::runSaDataStoreThread2);
    auto thread17 = std::thread(parse::runSaDataStoreThread3);
    auto thread18 = std::thread(parse::runSaDataStoreThread4);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread10.join();
    thread13.join();
    thread14.join();
    thread15.join();
    thread8.join();
    thread9.join();
    thread11.join();
    thread12.join();
    thread16.join();
    thread17.join();
    thread18.join();
    thread19.join();
    thread20.join();
    thread21.join();
    thread22.join();
    thread23.join();
    thread24.join();
    thread25.join();
    thread26.join();
    thread27.join();

    //end date and time
    time (& End);
    double dif = difftime (End, Start);
    cout << "\nMoneroSci Process Done in " << dif << " second(s)." << endl;

    now = std::chrono::system_clock::now();
    now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "\nEnded Time - " << std::put_time(std::localtime(&now_c), "%c") << '\n';

    cout << "The MoneroSci-parser is terminating." << endl;

    return EXIT_SUCCESS;
}

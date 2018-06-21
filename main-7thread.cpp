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


    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    cout << "NUMBER OF CORES IN THE RUNNING MACHINE IS - "<< concurentThreadsSupported << " CORES" << endl;

    int producerThredCount = concurentThreadsSupported -1;
    cout << "PRODUCER THREAD COUNT IS - " << producerThredCount << endl;
    cout << "CONSUMER THREAD COUNT IS - 1" << endl;

//    int maxBlockHeight = current_blockchain_height;
    int maxBlockHeight = 10000;

    vector<std::vector<int>> blockchainRanges = parse::getProducerRangers(producerThredCount, maxBlockHeight);

    //producer threads - iterate parser through the blockchain by deviding eqal potions
    std::thread t[producerThredCount];
    for(int i = 0; i<producerThredCount; i++){
        cout << "STARTED PRODUCER THREAD NUMBER -"<< i+1 << " RANGE ( "<<blockchainRanges[i][0]<<" TO "<< blockchainRanges[i][1] << " )"<< endl;
        t[i]= std::thread(parse::dataProducerFunction, std::ref(monerosci),blockchainRanges[i][0],blockchainRanges[i][1]);
    }

    //start consumer thread
    cout << "STARTED CONSUMER THREAD NUMBER -"<< 1 << endl;
    auto thread2 = std::thread(parse::dataConsumerThread,maxBlockHeight);

    // Wait for threads, which are running concurrently, to finish
    for(int i = 0; i<producerThredCount; i++){
        t[i].join();
    }

    parse::isAllBlockDone = true;
    thread2.join();


    //end date and time
    time (& End);
    double dif = difftime (End, Start);
    cout << "\nMoneroSci PROCESS DONE IN " << dif << " SECOND(s)." << endl;

    now = std::chrono::system_clock::now();
    now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "\nEnded Time - " << std::put_time(std::localtime(&now_c), "%c") << '\n';

    cout << "The MoneroSci-parser is terminating." << endl;

    return EXIT_SUCCESS;
}

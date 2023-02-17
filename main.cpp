#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include "USRP.h"

int UHD_SAFE_MAIN(int argc, char *argv[])
{
    std::string address("serial=30C5254");
    std::string subdev("A:A");
    std::string antenna("RX2");
    std::string ref("internal");

    std::vector<double> wifi_freqs = {2.412e9, 2.417e9, 2.422e9, 2.427e9, 2.432e9, 2.437e9, 2.442e9, 2.447e9, 2.452e9, 2.457e9, 2.462e9};

    USRP usrp = USRP(address)
                    .with_frequency(wifi_freqs[0]);

    // Stream for a number of samples and track the time taken
    const auto start_time = std::chrono::steady_clock::now();
    usrp.stream_for_number_of_samples(1000000, "output.txt");
    const auto end_time = std::chrono::steady_clock::now();

    const auto time_elapsed = end_time - start_time;
    std::cout << "\nTime Elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(time_elapsed).count() << std::endl;

    std::cout << std::endl;
    std::cout << usrp.usrp_internal->get_rx_bandwidth() << std::endl;
    std::cout << usrp.usrp_internal->get_rx_antenna() << std::endl;
    std::cout << usrp.usrp_internal->get_rx_gain() << std::endl;
    std::cout << usrp.usrp_internal->get_rx_rate() << std::endl;

    return EXIT_SUCCESS;
}
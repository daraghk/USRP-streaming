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
    std::string address("serial=30C524E");
    std::string subdev("A:A");
    std::string antenna("RX2");
    std::string ref("internal");

    double sample_rate(8e6);
    double frequency(2.48e9);
    double gain(50);

    USRP usrp = USRP(address)
                .with_sample_rate(sample_rate)
                .with_frequency(frequency)
                .with_gain(gain)
                .with_IF_filter_bandwidth(sample_rate)
                .with_antenna(antenna);

    usrp.print_summary();

    usrp.stream_for_number_of_samples(100000, "output.txt");

    // // Lock mboard clocks
    // std::cout << boost::format("Lock mboard clocks: %f") % ref << std::endl;
    // usrp->set_clock_source(ref);

    // // always select the subdevice first, the channel mapping affects the other settings
    // std::cout << boost::format("subdev set to: %f") % subdev << std::endl;
    // usrp->set_rx_subdev_spec(subdev);
    // std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

    return EXIT_SUCCESS;
}
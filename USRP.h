#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <iostream>
#include <fstream>

class USRP
{
public:
    USRP(const std::string &address){
        this->usrp_internal = uhd::usrp::multi_usrp::make(address);
    }

    // Builder functions
    USRP& with_sample_rate(double sample_rate);
    USRP& with_frequency(double frequency);
    USRP& with_gain(double gain);
    USRP& with_IF_filter_bandwidth(double bandwidth);
    USRP& with_antenna(std::string antenna);

    void print_summary();

    // Functions for streaming
    void stream_for_number_of_samples(const size_t number_of_samples, const std::string& output_file_name);
    void stream_for_time(const size_t max_time, const std::string& output_file_name);
    
    // Functions for calculating metrics
    double calculate_power_in_watts_from_IQ_sample(const std::complex<float> IQ_sample);
    double calculate_dBm_from_power_in_watts(double power_watts);

    uhd::usrp::multi_usrp::sptr usrp_internal;
};
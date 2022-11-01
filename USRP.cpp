#include"USRP.h"

USRP& USRP::with_sample_rate(double sample_rate)
{
    std::cout << boost::format("Setting RX Rate: %f Msps") % (sample_rate / 1e6) << std::endl;
    usrp->set_rx_rate(sample_rate);
    std::cout << boost::format("Actual RX Rate: %f Msps") % (usrp->get_rx_rate() / 1e6) << std::endl
              << std::endl;
    return *this;
}

USRP& USRP::with_frequency(double frequency)
{
    std::cout << boost::format("Setting RX Freq: %f MHz") % (frequency / 1e6) << std::endl;
    uhd::tune_request_t tune_request(frequency);
    usrp->set_rx_freq(tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz") % (usrp->get_rx_freq() / 1e6) << std::endl
              << std::endl;
    return *this;
}

USRP& USRP::with_gain(double gain)
{
    std::cout << boost::format("Setting RX Gain: %f dB") % gain << std::endl;
    usrp->set_rx_gain(gain);
    std::cout << boost::format("Actual RX Gain: %f dB") % usrp->get_rx_gain() << std::endl
              << std::endl;
    return *this;
}

USRP& USRP::with_IF_filter_bandwidth(double bandwidth)
{
    std::cout << boost::format("Setting RX Bandwidth: %f MHz") % (bandwidth / 1e6) << std::endl;
    usrp->set_rx_bandwidth(bandwidth);
    std::cout << boost::format("Actual RX Bandwidth: %f MHz") % (usrp->get_rx_bandwidth() / 1e6) << std::endl
              << std::endl;
    return *this;
}

USRP& USRP::with_antenna(std::string antenna){
    std::cout << boost::format("Setting RX Antenna: %s") % antenna << std::endl;
    usrp->set_rx_antenna(antenna);
    std::cout << boost::format("Actual RX Antenna: %s") % usrp->get_rx_antenna() << std::endl
              << std::endl;
    return *this;
}

void USRP::print_summary(){
    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;
}

void USRP::stream_for_number_of_samples(const size_t number_of_samples, const std::string& output_file_name){
    uhd::stream_args_t stream_args("fc32", "sc16");
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    std::vector<std::complex<float>> receive_buffer(rx_stream->get_max_num_samps());

    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps  = number_of_samples;
    stream_cmd.stream_now = true;
    rx_stream->issue_stream_cmd(stream_cmd);

    std::ofstream output(output_file_name);

    uhd::rx_metadata_t metadata;
    size_t current_number_samples = 0;
    while (current_number_samples < number_of_samples){
        size_t number_rx_samples_received = rx_stream->recv(&receive_buffer.front(), receive_buffer.size(), metadata);
        
        auto receive_buffer_iter_index = 0;
        while(receive_buffer_iter_index < number_rx_samples_received){
            double power_watts = calculate_power_in_watts_from_IQ_sample(receive_buffer[receive_buffer_iter_index]);
            double dBm = calculate_dBm_from_power_in_watts(power_watts);
            std::cout << dBm << std::endl;
            output << power_watts << " " << dBm << std::endl;
            receive_buffer_iter_index++;
        }

        current_number_samples += number_rx_samples_received;
    }

    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);
}

double USRP::calculate_power_in_watts_from_IQ_sample(const std::complex<float> IQ_sample){
    return pow(IQ_sample.real(), 2) + pow(IQ_sample.imag(), 2);
}

double USRP::calculate_dBm_from_power_in_watts(double power_watts){
    double power_milli_watts = power_watts * 1000;
    return 10 * log10(power_milli_watts);
}
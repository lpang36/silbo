#pragma once

#include <string>

namespace silbo {

struct Config {
    std::string save_path;
    size_t default_sample_rate = 44100;
    size_t block_size = 128;
    size_t min_freq = 1;
    size_t max_freq = 128;
    double spectrogram_threshold = 0.1;
    size_t mask_width = 10;
    size_t mask_height = 10;
    size_t window_shift = 48;
    size_t window_size = 64;
    double db_scale = 0.5;
    double lookup_tolerance = 0.01;
    int bucket_size = 128;
    double score_threshold = 0;
};

}
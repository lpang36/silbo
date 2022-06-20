#pragma once

#include <string>

namespace silbo {

struct Config {
    std::string save_path;
    size_t block_size = 128;
    size_t mask_width = 128;
    size_t mask_height = 128;
    size_t window_shift = 192;
    size_t window_size = 256;
    double db_scale = 0.01;
    double lookup_tolerance = 0.01;
    size_t bucket_size = 1024;
    double threshold = 0.1;
};

}
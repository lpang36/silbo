#pragma once

#include <opencv2/core/core.hpp>

#include <string>

namespace silbo {

class Config;

namespace fft {

class FFT {
public:
    static cv::Mat fft(const std::string& path, const Config& config);
};

}
}
#pragma once

#include <string>

namespace cv {

class Mat;

}

namespace silbo {

class Config;

namespace fft {

class FFT {
public:
    static cv::Mat fft(const std::string& path, const Config& config);
}

}
}
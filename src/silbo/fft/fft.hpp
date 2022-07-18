#pragma once

#include "AudioFile.h"
#include <opencv2/core/core.hpp>

#include <string>

namespace silbo {

class Config;

namespace fft {

class FFT {
public:
    static cv::Mat fft(const AudioFile<double>& path, const Config& config);
};

}
}
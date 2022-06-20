#include <opencv2/imgproc/imgproc.hpp>
#include "AudioFile.h"

#include "../config.hpp"
#include "fft.hpp"

namespace silbo {
namespace fft {

cv::Mat FFT::fft(const std::string& path, const Config& config) {
    AudioFile<double> file;
    file.load(path);

    const size_t cols = config.block_size;
    const size_t rows = file.getNumSamplesPerChannel() / cols;
    auto samples = cv::Mat{rows, config.block_size, CV_32F};
    for (size_t i = 0; i < file.getNumSamplesPerChannel(); ++i) {
        // assuming one channel
        // access is possibly inefficient https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#performance-difference
        samples.at<float>(i / cols, i % cols) = file.samples[0][i];
    }

    // omitting hanning window for now
  	// http://docs.opencv.org/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
    cv::Mat planes[] = {cv::Mat::zeros(1, cols, CV_32F), cv::Mat::zeros(1, cols, CV_32F)};
    cv::Mat complex;
    cv::merge(planes, 2, complex);
    for (size_t i = 0; i < cols; ++i) {
        auto row = samples.row(i);
        cv::dft(row, complex);
        cv::split(complex, planes);
        cv::magnitude(planes[0], planes[1], planes[0]);
        planes[0] += cv::Scalar::all(1);
        cv::log(planes[0], planes[0]);
        row = planes[0];
    }
    return samples;
}

}
}
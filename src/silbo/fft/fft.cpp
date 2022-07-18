#include <opencv2/imgproc/imgproc.hpp>

#include "../config.hpp"
#include "../utils.hpp"
#include "fft.hpp"

#include <math.h>

namespace silbo {
namespace fft {

cv::Mat FFT::fft(const AudioFile<double>& file, const Config& config) {
    const size_t n = file.getNumSamplesPerChannel();
    const size_t cols = config.block_size * 2;
    const size_t rows = n / cols + (n % cols > 0);
    auto samples = cv::Mat{rows, cols, CV_32F};
    for (size_t i = 0; i < n; ++i) {
        // assuming one channel
        // access is possibly inefficient https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#performance-difference
        samples.at<float>(i / cols, i % cols) = file.samples[0][i];
    }

    /*auto window = cv::Mat{1, cols, CV_32F};
    for (size_t i = 0; i < cols; ++i) {
        window.at<float>(0, i) = 0.5 * (1 - std::cos(2 * M_PI * i / (cols)));
    }*/

    // TODO: overlap
  	// http://docs.opencv.org/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
    for (size_t i = 0; i < rows; ++i) {
        // could be more efficient without repeated allocation
        auto row = samples.row(i);
        cv::Mat planes[] = {row, cv::Mat::zeros(row.size(), CV_32F)};
        cv::Mat complex;
        cv::merge(planes, 2, complex);

        cv::dft(complex, complex);
        cv::split(complex, planes);
        cv::magnitude(planes[0], planes[1], planes[0]);
        planes[0] += cv::Scalar::all(1);
        cv::log(planes[0], planes[0]);
        row = planes[0];
    }

    samples = samples(cv::Rect{config.min_freq, 0, config.max_freq - config.min_freq, samples.size().height}).t();
    cv::normalize(samples, samples, 0, 1, cv::NORM_MINMAX);
    cv::resize(samples, samples, cv::Size(samples.size().width * config.default_sample_rate / file.getSampleRate(), samples.size().height), cv::INTER_LINEAR);
    display_spectrogram(samples);
    return samples;
}

}
}
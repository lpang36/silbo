#pragma once

#include <opencv2/highgui.hpp>

namespace silbo {

inline void display_spectrogram(cv::Mat input) {
    cv::Mat output;
    cv::normalize(input, output, 0, 1, cv::NORM_MINMAX);
    cv::imshow("spectrogram", output);
    cv::waitKey();
}

}
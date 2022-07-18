#pragma once

#include <opencv2/highgui.hpp>

#include <iostream>

namespace silbo {

inline void display_spectrogram(cv::Mat input) {
    /*static size_t id = 0;
    cv::Mat output;
    input.convertTo(output, CV_32F);
    cv::normalize(output, output, 0, 1, cv::NORM_MINMAX);
    cv::flip(output, output, 0);   
    // output = output(cv::Rect{0, 0, 30000, 127});
    cv::imshow("spectrogram", output);
    cv::waitKey();*/
}

}
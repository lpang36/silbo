#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <kdtree.hpp>

#include "../config.hpp"
#include "../utils.hpp"
#include "fingerprint.hpp"

#include <iostream>

namespace silbo {
namespace fingerprint {

std::vector<Hash> Fingerprint::hashes() const {
    return hashes_;
}

void Fingerprint::add(const cv::Point& p1, const Kdtree::KdNode& p2, size_t id, size_t offset) {
    hashes_.emplace_back(Hash{
        p1.x,
        (p1.y + offset) / (p2.point[1] + offset),
        p2.point[0] - p1.x,
        id,
    });
}

Fingerprint Fingerprint::fingerprint(cv::Mat input, const Config& config, size_t id) {
    // https://stackoverflow.com/questions/5550290/find-local-maxima-in-grayscale-image-using-opencv
    auto mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(config.mask_width, config.mask_height), cv::Point(-1, -1));
    mask.at<float>(config.mask_width / 2, config.mask_height / 2) = 0;
    cv::Mat constellation;
    cv::dilate(input, constellation, mask);
    constellation = input >= constellation;
    display_spectrogram(constellation);
    cv::Mat points;
    cv::findNonZero(constellation, points);

    Fingerprint f;
    if (points.size().width == 0) {
        return f;
    }

    // TODO: assumes same sampling rate
    Kdtree::KdNodeVector nodes;
    for (size_t i = 0; i < points.total(); ++i) {
        const auto& point = points.at<cv::Point>(i);
        nodes.emplace_back(Kdtree::CoordPoint{double(point.x), double(point.y)});
    }
    auto tree = Kdtree::KdTree{&nodes};

    // would need a transform to use a rectangular window
    for (size_t i = 0; i < points.total(); ++i) {
        const auto& point = points.at<cv::Point>(i);
        auto anchor = std::vector<double>{double(point.x + config.window_shift), double(point.y)};
        Kdtree::KdNodeVector result;
        tree.range_nearest_neighbors(anchor, config.window_size / 2, &result);
        for (const auto& p : result) {
            f.add(point, p, id, config.min_freq);
        }
    }
    return f;
}

}
}
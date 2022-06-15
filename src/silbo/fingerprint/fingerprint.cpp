#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <kdtree-cpp/kdtree.hpp>

#include <fingerprint.hpp>

namespace silbo {
namespace fingerprint {

std::vector<Hash> Fingerprint::hashes() const {
    return hashes_;
}

void Fingerprint::add(const cv::Point& p1, const Kdtree::KdNode& p2, size_t id) {
    hashes_.emplace_back({
        p1.x,
        p1.y / p2.point[1],
        p2.point[0] - p1.x,
        id,
    });
}

static Fingerprint Fingerprint::fingerprint(cv::Mat input, const Config& config, size_t id) {
    // https://stackoverflow.com/questions/5550290/find-local-maxima-in-grayscale-image-using-opencv
    auto mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(config.mask_width, config.mask_height), cv::Point(-1, -1));
    mask.at<float>(config.mask_width / 2, config.mask_height / 2) = 0;
    Mat constellation;
    cv::dilate(input, constellation, mask);
    constellation = input > constellation;
    Mat points;
    cv::findNonZero(constellation, points);

    // assumes same sampling rate - must fix
    Kdtree::KdNodeVector nodes;
    for (size_t i = 0; i < points.total(); ++i) {
        const auto& point = points.at<cv::Point>(i);
        nodes.emplace_back({point.x, point.y});
    }
    auto tree = Kdtree::KdTree{nodes};

    // would need a transform to use a rectangular window
    Fingerprint f;
    for (size_t i = 0; i < points.total(); ++i) {
        const auto& point = points.at<cv::Point>(i);
        auto anchor = std::vector{point.x + config.window_shift, point.y};
        Kdtree::KdNodeVector result;
        tree.range_nearest_neighbors(anchor, config.window_size / 2, &result);
        for (const auto& p : result) {
            f.add(point, p, id);
        }
    }
    return f;
}

}
}
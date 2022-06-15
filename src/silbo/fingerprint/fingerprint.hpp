#pragma once

#include <hash.hpp>

#include <string>
#include <vector>

namespace cv {

class Mat;
class Point;

}

namespace Kdtree {

class KdNode;

}

namespace silbo {

class Config;

namespace fingerprint {

class Fingerprint {
public:
    std::vector<Hash> hashes() const;
    void add(const cv::Point& p1, const Kdtree::KdNode& p2, size_t id);
    
    static Fingerprint fingerprint(cv::Mat input, const Config& config, size_t id);

private:
    std::vector<Hash> hashes_;
}

}
}
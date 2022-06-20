#pragma once

#include <boost/serialization/access.hpp>

// could be optimized
namespace silbo {
namespace fingerprint {

struct Hash {
    float start = 0;
    float freq_ratio = 0;
    float time_delta = 0;
    size_t id = 0;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & start;
        ar & freq_ratio;
        ar & time_delta;
        ar & id;
    }
};

}
}
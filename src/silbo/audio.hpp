#pragma once

#include <boost/serialization/access.hpp>

#include <string>

namespace silbo {

struct Audio {
    size_t id = 0;
    std::string name;
    size_t sample_rate = 0;
    size_t num_samples = 0;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & id;
        ar & name;
        ar & sample_rate;
        ar & num_samples;
    }
};

}
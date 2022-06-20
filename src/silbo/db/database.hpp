#pragma once

#include <kdtree.hpp>
#include <boost/serialization/vector.hpp>

#include "../fingerprint/hash.hpp"
#include "../fingerprint/fingerprint.hpp"
#include "match.hpp"
#include "../config.hpp"

#include <memory>

namespace silbo {
namespace db {

class Database {
public:
    explicit Database(const Config& config) : config_(config) {};

    void add(const fingerprint::Fingerprint& f);
    std::vector<Match> lookup(const fingerprint::Fingerprint& f);

private:
    friend class boost::serialization::access;

    void initialize();
    Kdtree::KdNode make_node(const fingerprint::Hash& hash) const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & hashes_;
    }

    Config config_;
    std::vector<fingerprint::Hash> hashes_;
    std::unique_ptr<Kdtree::KdTree> tree_;
    bool initialized = false;
};

}
}
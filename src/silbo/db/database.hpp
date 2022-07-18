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

    void add(const std::string& name, const fingerprint::Fingerprint& f);
    std::vector<Match> lookup(const fingerprint::Fingerprint& f);
    size_t get_next_id() const;

private:
    friend class boost::serialization::access;

    void initialize();
    Kdtree::KdNode make_node(const fingerprint::Hash& hash) const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & hashes_;
        ar & paths_;
    }

    const Config config_;
    std::vector<fingerprint::Hash> hashes_;
    std::unique_ptr<Kdtree::KdTree> tree_;
    bool initialized_ = false;
    std::vector<std::string> paths_;
};

}
}
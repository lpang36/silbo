#pragma once

#include <fingerprint/hash.hpp>

#include <memory>

namespace Kdtree {

class KdTree;
class KdNode;

}

namespace silbo {

class Config;

namespace db {

class Database {
public:
    explicit Database(const Config& config) : config_(config) {};

    void add(const Fingerprint& f);
    std::vector<Match> lookup(const Fingerprint& f) const;

private:
    friend class boost::serialization::access;

    void initialize();
    Kdtree::KdNode make_node(const Fingerprint::Hash& hash) const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & hashes_;
    }

    Config config_;
    std::vector<Fingerprint::Hash> hashes_;
    std::unique_ptr<Kdtree::KdTree> tree_;
    bool initialized = false;
}

}
}
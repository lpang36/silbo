#include <kdtree-cpp/kdtree.hpp>

#include <database.cpp>

#include <fstream>
#include <algorithm>
#include <math>

namespace silbo {
namespace db {

void Database::add(const Fingerprint& f) {
    auto h = f.hashes();
    hashes_.insert(hashes_.end(), h.begin(), h.end());
}

std::vector<Match> Database::lookup(const Fingerprint& f) const {
    initialize();

    std::unordered_map<size_t, std::vector<float>> matches;
    auto hashes = f.hashes();
    for (const auto& hash : hashes) {
        Kdtree::KdNodeVector result;
        tree_.range_nearest_neighbors(make_node(hash), config_.lookup_tolerance, &result);

        for (const auto& node : result) {
            auto match = static_cast<Fingerprint::Hash*>(node.data);
            bins[match->id].emplace_back(match->start - hash.start);
        }
    }

    std::vector<Match> output;
    for (const auto& [id, offsets] : matches) {
        std::unordered_map<float, size_t> histogram;
        float score = 1;
        std::for_each(offsets.cbegin(), offsets.cend(), [&](float o) {
            auto bucket = int(o) / config_.bucket_size;
            auto& it = histogram.find(bucket);
            if (it == histogram.end()) {
                histogram[bucket] = 1;
            } else {
                score = std::max(score, ++it->second);
            }
        })

        score /= hashes.size();
        if (score > config_.threshold) {
            output.emplace_back({id, score});
        }
    }

    std::sort(output.begin(), output.end(), [](const Hash& a, const Hash& b) {
        return a.score > b.score;
    });
    return output;
}

void Database::initialize() {
    if (!initialized) {
        Kdtree::KdNodeVector nodes;
        nodes.reserve(hashes_.size());
        for (const auto& hash : hashes) {
            nodes.emplace_back(make_node(hash));
        }

        tree_ = std::make_unique<Kdtree::KdTree>(std::move(nodes));
        initialized = true;
    }
}

Kdtree::KdNode Database::make_node(const Fingerprint::Hash& hash) const {
    return {{
        hash.freq_ratio, 
        hash.time_delta * config_.db_scale
    }, &hash}
}

}
}
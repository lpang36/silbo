#include "database.hpp"

#include <fstream>
#include <algorithm>
#include <unordered_map>

namespace silbo {
namespace db {

void Database::add(const Audio& audio, const fingerprint::Fingerprint& f) {
    // assumes no duplicate paths
    paths_.emplace_back(audio);
    
    auto h = f.hashes();
    hashes_.insert(hashes_.end(), h.begin(), h.end());
}

std::vector<Match> Database::lookup(const fingerprint::Fingerprint& f) {
    initialize();

    std::unordered_map<size_t, std::vector<float>> matches;
    const auto hashes = f.hashes();
    for (const auto& hash : hashes) {
        Kdtree::KdNodeVector result;
        tree_->range_nearest_neighbors(make_node(hash).point, config_.lookup_tolerance, &result);

        for (const auto& node : result) {
            auto match = static_cast<fingerprint::Hash*>(node.data);
            matches[match->id].emplace_back(match->start - hash.start);
        }
    }

    std::vector<Match> output;
    for (const auto& [id, offsets] : matches) {
        std::unordered_map<float, size_t> histogram;
        float score = 1;
        int best_bucket = -1;
        std::for_each(offsets.cbegin(), offsets.cend(), [&](float o) {
            auto bucket = int(o) / config_.bucket_size;
            auto it = histogram.find(bucket);
            if (it == histogram.end()) {
                histogram[bucket] = 1;
            } else {
                float count = ++it->second;
                if (score < count) {
                    score = count;
                    best_bucket = bucket;
                }
            }
        });

        const auto& audio = paths_.at(id - 1);
        /*std::cout << audio.name << " " << best_bucket << " " << audio.sample_rate << std::endl;
        for (const auto& [bucket, count]: histogram) {
            std::cout << bucket << " " << count << std::endl;
        }*/
        score /= hashes.size();
        if (score > config_.score_threshold) {
            output.emplace_back(Match{
                audio, 
                score, 
                2 * best_bucket * config_.block_size * config_.bucket_size / config_.default_sample_rate, // TODO: adjust if overlap used in fft
            });
        }
    }

    std::sort(output.begin(), output.end(), [](const Match& a, const Match& b) {
        return a.score > b.score;
    });
    return output;
}

size_t Database::get_next_id() const {
    return paths_.size() + 1;
}

void Database::initialize() {
    if (!initialized_) {
        Kdtree::KdNodeVector nodes;
        nodes.reserve(hashes_.size());
        for (const auto& hash : hashes_) {
            nodes.emplace_back(make_node(hash));
        }

        tree_ = std::make_unique<Kdtree::KdTree>(&nodes);
        initialized_ = true;
    }
}

Kdtree::KdNode Database::make_node(const fingerprint::Hash& hash) const {
    return {Kdtree::CoordPoint{
        hash.freq_ratio, 
        hash.time_delta * config_.db_scale
    }, (void*)&hash};
}

}
}
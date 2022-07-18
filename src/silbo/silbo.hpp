#pragma once

#include "config.hpp"
#include "db/match.hpp"
#include "db/database.hpp"
#include "fingerprint/fingerprint.hpp"

#include <utility>
#include <unordered_map>

namespace silbo {

class Silbo {
public:
    explicit Silbo(const Config& config) : 
        config_(config), 
        db_(std::make_unique<db::Database>(config)) {};
    void add(const std::string& path);
    void save() const;
    std::vector<db::Match> lookup(const std::string& path) const;    

private:
    std::pair<Audio, fingerprint::Fingerprint> process_file(const std::string& path) const;

    const Config config_;
    const std::unique_ptr<db::Database> db_;
};

}
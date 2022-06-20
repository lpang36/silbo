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
    fingerprint::Fingerprint process_file(const std::string& path) const;

    Config config_;
    std::unique_ptr<db::Database> db_;
    mutable std::unordered_map<std::string, size_t> ids_;
    mutable std::vector<std::string> paths;
    mutable size_t next_id_ = 0;
};

}
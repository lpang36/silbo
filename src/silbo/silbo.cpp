#include <glob.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/algorithm/string.hpp>

#include "silbo.hpp"
#include "fft/fft.hpp"

#include <algorithm>
#include <fstream>

namespace silbo {

void Silbo::add(const std::string& path) {
    if (boost::algorithm::ends_with(path, ".bin")) {
        std::ifstream ifs(path);
        boost::archive::text_iarchive ia(ifs);
        ia >> *db_;
    } else {
        glob_t globbuf;
        if (glob(path.c_str(), 0, NULL, &globbuf) == 0) {
            for (size_t i = 0; i < globbuf.gl_pathc; ++i) {
                db_->add(process_file(globbuf.gl_pathv[i]));
            }
            globfree(&globbuf);
        }
    }
}

void Silbo::save() const {
    if (!config_.save_path.empty()) {
        std::ofstream ofs(config_.save_path);
        boost::archive::text_oarchive oa(ofs);
        oa << *db_;
    }
}

std::vector<db::Match> Silbo::lookup(const std::string& path) const {
    auto results = db_->lookup(process_file(path));
    std::for_each(results.begin(), results.end(), [&](auto& match) {
        match.name = paths.at(match.id - 1);
    });
    return results;
}

fingerprint::Fingerprint Silbo::process_file(const std::string& path) const {
    if (ids_.find(path) == ids_.end()) {
        ids_[path] = ++next_id_;
        paths.emplace_back(path);
        return fingerprint::Fingerprint::fingerprint(fft::FFT::fft(path, config_), config_, next_id_);
    }
}

}
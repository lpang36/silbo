#include <glob.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <silbo.hpp>
#include <fft/fft.hpp>

#include <algorithm>

namespace silbo {

void Silbo::add(const std::string path) {
    if (path.ends_with(".bin")) {
        boost::archive::text_iarchive ia(std::ifstream(path));
        ia >> *db;
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
        boost::archive::text_oarchive oa(std::ofstream(config_.save_path));
        oa << *db;
    }
}

std::vector<Match> Silbo::lookup(const std::string path) const {
    auto results = db_->lookup(process_file(path));
    std::for_each(results.begin(), results.end(), [&](auto& match) {
        match.name = paths.at(match.id - 1);
    });
    return results;
}

std::unique_ptr<fingerprint::Fingerprint> Silbo::process_file(const std::string& path) const {
    if (ids_.find(path) == ids_.end()) {
        ids_[path] = ++next_id_;
        paths.emplace_back(path);
        return fingerprint::Fingerprint::fingerprint(fft::FFT::fft(path, config_), config_, next_id_);
    }
}

}
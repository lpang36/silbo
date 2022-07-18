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
                db_->add(path, process_file(globbuf.gl_pathv[i]));
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
    return db_->lookup(process_file(path));
}

fingerprint::Fingerprint Silbo::process_file(const std::string& path) const {
    return fingerprint::Fingerprint::fingerprint(fft::FFT::fft(path, config_), config_, db_->get_next_id());
}

}
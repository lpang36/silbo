#include <glob.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/algorithm/string.hpp>

#include "silbo.hpp"
#include "fft/fft.hpp"

#include <algorithm>
#include <fstream>
#include <cassert>

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
                const auto& [audio, fingerprint] = process_file(globbuf.gl_pathv[i]);
                db_->add(audio, fingerprint);
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
    return db_->lookup(process_file(path).second);
}

std::pair<Audio, fingerprint::Fingerprint> Silbo::process_file(const std::string& path) const {
    AudioFile<double> file;
    file.load(path);
    auto id = db_->get_next_id();

    return std::make_pair<Audio, fingerprint::Fingerprint>({
        id, 
        path, 
        file.getSampleRate(),
        file.getNumSamplesPerChannel()
    }, fingerprint::Fingerprint::fingerprint(fft::FFT::fft(file, config_), config_, id));
}

}
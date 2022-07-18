#pragma once

#include "../audio.hpp"

#include <iostream>

namespace silbo {
namespace db {

struct Match {
    Audio audio;
    float score = 0;
    int timestamp = 0;

    friend std::ostream& operator<<(std::ostream& os, const Match& m) {
        os << m.audio.name << ": " << m.score << " at " << m.timestamp << "s";
        return os;
    }
};

}
}
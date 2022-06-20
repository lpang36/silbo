#pragma once

#include <iostream>

namespace silbo {
namespace db {

struct Match {
    size_t id = 0;
    float score = 0;
    std::string name;

    friend std::ostream& operator<<(std::ostream& os, const Match& m) {
        os << m.name << " " << m.score;
        return os;
    }
};

}
}
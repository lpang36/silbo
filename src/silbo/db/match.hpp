#include <iostream>

namespace silbo {
namespace db {

struct Match {
    size_t id;
    float score;
    std::string name;
}

}
}

std::ostream& operator<<(std::ostream& os, const Match& m) {
    os << m.name << " " << m.score;
    return os;
}
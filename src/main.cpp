#include <GetPot>

#include <silbo/silbo.hpp>
#include <silbo/config.hpp>

#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    GetPot cl(argc, argv);
    const auto data = cl.nominus_followers("-d")
    const auto inputs = cl.nominus_followers("-i")
    const auto save_path = cl.nominus_followers("-s")
    const auto 
    
    auto config = silbo::Config{};
    if (!save_path.empty()) {
        config.save_path = save_path[0];
    }

    auto silbo = silbo::Silbo{config};
    for (const auto& d : data) {
        silbo.add(d);
    }
    silbo.save();

    for (const auto& i : inputs) {
        std::cout << i << std::endl;
        for (const auto& m : silbo.lookup(i)) {
            std::cout << m << std::endl;
        }
        std::cout << std::endl;
    }
}
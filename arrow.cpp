// https://spin.atomicobject.com/2015/06/12/objects-around-svg-circle-d3-js/

#include <cmath>
#include <iostream>
#include <string>

constexpr double PI = 3.14159265;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " <degrees>" << std::endl;
        return EXIT_FAILURE;
    }
    try {
        double deg = std::stod(argv[1]);
        const auto originX = 32.2;
        const auto originY = 32.0;
        const auto radius  = 23.2;
        const auto reverse  = 180.0;
        auto x = originX + radius * sin((reverse + deg) * PI / 180.0);
        auto y = originY - radius * cos((reverse + deg) * PI / 180.0);
        std::cout << x << "," << y << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::invalid_argument& ia) {
        std::cout << "0.0,0.0" << std::endl;
    } catch (const std::out_of_range& oor) {
        std::cout << "0.0,0.0" << std::endl;
    }
    return EXIT_SUCCESS;
}

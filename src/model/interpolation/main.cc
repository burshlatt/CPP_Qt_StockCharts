#include <iostream>
#include <vector>

#include "interpolation.hpp"

int main() {
    s21::CubicSplineInterpolator interpolator;

    auto data_points{interpolator.LoadFile("/home/burshlat/Desktop/Projects/A8_Algorithmic_trading-1/src/datasets/table.csv")};

    std::vector<long> x_values{data_points.first};
    std::vector<double> y_values{data_points.second};

    std::size_t size{x_values.size()};

    for (std::size_t i{}; i < size; ++i) {
        std::cout << x_values[i] << " = " << y_values[i] << "\n";
    }

    return 0;
}


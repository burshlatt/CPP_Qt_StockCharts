#ifndef A8_ALGORITHMIC_TRADING_MODEL_INTERPOLATION_INTERPOLATION_HPP
#define A8_ALGORITHMIC_TRADING_MODEL_INTERPOLATION_INTERPOLATION_HPP

#include <vector>
#include <string_view>

#include "../csv/CsvManager.h"

namespace s21 {
class Interpolator {
private:
    using Coords = std::pair<std::vector<long>, std::vector<double>>;

private:
    struct Splines {
        double a;
        double b;
        double c;
        double d;
        double x;
    };

public:
    Coords LoadFile(std::string_view path);

private:
    void Clear();
    void ComputeCoefficients();
    double Interpolate(double x_input);

private:
    CsvManager csv_;
    std::vector<long> x_;
    std::vector<double> y_;
    std::vector<Splines> splines_;
};
} // namespace s21

#endif  // A8_ALGORITHMIC_TRADING_MODEL_INTERPOLATION_INTERPOLATION_HPP

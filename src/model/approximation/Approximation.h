#ifndef A8_ALGORITHMIC_TRADING_SRC_MODEL_APPROXIMATION_APPROXIMATION_H
#define A8_ALGORITHMIC_TRADING_SRC_MODEL_APPROXIMATION_APPROXIMATION_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <boost/date_time.hpp>

namespace s21 {

class Approximation {
    public: 
        Approximation(const unsigned polynom);
        ~Approximation() {}

        std::vector<std::pair<double, double>> ApproximationAlgorithm();
        std::vector<std::vector<double>> AugementedMatrixCreate(std::vector<std::tuple<boost::gregorian::date, int, double>>& data);
    private:
        const unsigned polynom_;
        double SumX(const unsigned& k, std::vector<std::tuple<boost::gregorian::date, int, double>>& data);
        double SumXY(const unsigned& k, std::vector<std::tuple<boost::gregorian::date, int, double>>& data);
};

} // namespace s21

#endif  // A8_ALGORITHMIC_TRADING_SRC_MODEL_APPROXIMATION_APPROXIMATION_H


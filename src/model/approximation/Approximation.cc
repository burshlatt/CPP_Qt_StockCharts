#include "Approximation.h"

namespace s21 {
    
Approximation::Approximation(const unsigned polynom) : polynom_(polynom) {}
    
std::vector<std::pair<double, double>> Approximation::ApproximationAlgorithm() {
    return {};
}

std::vector<std::vector<double>> Approximation::AugementedMatrixCreate(std::vector<std::tuple<boost::gregorian::date, int, double>>& data) {
    if (polynom_ == 0) {
        throw std::runtime_error("Error: Polynom 0");
    }
    std::size_t rows = polynom_ + 1;
    std::size_t cols = polynom_ + 2;
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0));
    for (std::size_t i = 0; i < rows; i++) {
        unsigned k = polynom_ * 2 - i;
        for (std::size_t j = 0; j < cols; j++) {
            if (j < cols - 1) matrix[i][j] = SumX(k, data);
            else matrix[i][j] = SumXY(k, data);
            if (j < cols - 2) k--;
        }
    }
    return matrix;
}

double Approximation::SumX(const unsigned& k, std::vector<std::tuple<boost::gregorian::date, int, double>>& data) {
    double result = 0;
    for (const auto& t : data) {
        result += pow(std::get<1>(t), k);
    }
    return result;
}

double Approximation::SumXY(const unsigned& k, std::vector<std::tuple<boost::gregorian::date, int, double>>& data) {
    double result = 0;
    for (const auto& t : data) {
        result += pow(std::get<1>(t), k) * std::get<2>(t);
    }
    return result;
}

// 1
// 2 1 1
// 1 0 0

// 2
// 4 3 2 2
// 3 2 1 1
// 2 1 0 0

// 3
// 6 5 4 3 3
// 5 4 3 2 2
// 4 3 2 1 1
// 3 2 1 0 0
// a * sum(x^2) + b * sum(x^1) = sum(x^1 * y)
// a * sum(x^1) + b * sum(x^0) = sum(x^0 * y)

// a * sum(x^4) + b * sum(x^3) + c * sum(x^2) = sum(x^2 * y)
// a * sum(x^3) + b * sum(x^2) + c * sum(x^1) = sum(x^1 * y)        
// a * sum(x^2) + b * sum(x^1) + c * sum(x^0) = sum(x^0 * y)        

// a * sum(6) + b * sum(5) + c * sum(4) + d * sum(3) = sum(3y)
// a * sum(5) + b * sum(4) + c * sum(3) + d * sum(2) = sum(2y)
// a * sum(4) + b * sum(3) + c * sum(2) + d * sum(1) = sum(1y)
// a * sum(3) + b * sum(2) + c * sum(1) + d * sum(0) = sum(0y)   


} // namespace s21

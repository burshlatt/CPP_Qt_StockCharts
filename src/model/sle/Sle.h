#ifndef A8_ALGORITHMIC_TRADING_SRC_MODEL_SLE_SLE_H
#define A8_ALGORITHMIC_TRADING_SRC_MODEL_SLE_SLE_H

#include <vector>
#include <algorithm>
#include <cmath>

namespace s21 {

class Sle {
    public: 
        Sle(const std::vector<std::vector<double>>& augmentedMatrix);
        ~Sle() {}

        std::vector<double> SleAlgorithm();

    private:
        const std::size_t& size_;
        const double eps_ = 1e-16;
        std::vector<std::vector<double>> matrix_;
        std::vector<double> constantTerm_;
        void Init(std::vector<std::vector<double>>& matrix, std::vector<double>& constantTerm, const std::vector<std::vector<double>>& augmentedMatrix);
        double StepFindMaxIndex(std::size_t nowIndex);
        void StepSwapCols(std::size_t nowIndex, std::size_t maxIndex);
        std::vector<double> StepBackSubstitution();
        void StepNormalize(std::size_t nowIndex);
        void StepSub(std::size_t nowIndex);


};

} // namespace s21

#endif  // A8_ALGORITHMIC_TRADING_SRC_MODEL_SLE_SLE_H

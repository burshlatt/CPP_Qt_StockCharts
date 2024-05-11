#include "Sle.h"

namespace s21 {

Sle::Sle(const std::vector<std::vector<double>>& augmentedMatrix) 
    : size_(augmentedMatrix.size()) {
    Init(matrix_, constantTerm_, augmentedMatrix);
}


void Sle::Init(std::vector<std::vector<double>>& matrix, std::vector<double>& constantTerm, const std::vector<std::vector<double>>& augmentedMatrix) {
    matrix.resize(size_, std::vector<double>(size_));
    constantTerm.resize(size_);
    for (std::size_t i = 0; i < size_; i++) {
        constantTerm[i] = augmentedMatrix[i][size_];
        for (std::size_t j = 0; j < size_; j++) {
            matrix[i][j] = augmentedMatrix[i][j]; 
        }
    }
}

double Sle::StepFindMaxIndex(std::size_t nowIndex) {
  double max = std::abs(matrix_[nowIndex][nowIndex]);
  std::size_t maxIndex = nowIndex;
  for (std::size_t i = nowIndex + 1; i < size_; i++) {
    if (std::abs(matrix_[i][nowIndex]) > max) {
      max = std::abs(matrix_[i][nowIndex]);
      maxIndex = i;
    }
  }
  return maxIndex;
}

void Sle::StepSwapCols(std::size_t nowIndex, std::size_t maxIndex) {
  for (std::size_t j = 0; j < size_; j++) {
    std::swap(matrix_[nowIndex][j], matrix_[maxIndex][j]);
  }
  std::swap(constantTerm_[nowIndex], constantTerm_[maxIndex]);
}

std::vector<double> Sle::StepBackSubstitution() {
  std::vector<double> result(size_);
    for (int k = size_ - 1; k != -1; k--) {
      result[k] = constantTerm_[k];
      for (int i = 0; i < k; i++) {
        constantTerm_[i] = constantTerm_[i] - matrix_[i][k] * result[k];
      }
  }
  return result;
}

void Sle::StepNormalize(std::size_t nowIndex) {
    for (std::size_t i = nowIndex; i < size_; i++) {
      double div = matrix_[i][nowIndex];
      if (std::abs(div) < eps_) continue;
      for (std::size_t j = nowIndex; j < size_; j++) {
        matrix_[i][j] /= div;
      }
      constantTerm_[i] /= div;
    }
}

void Sle::StepSub(std::size_t nowIndex) {
    for (std::size_t i = nowIndex + 1; i < size_; ++i) {
      for (std::size_t j = nowIndex; j < size_; j++) {
        matrix_[i][j] -= matrix_[nowIndex][j];
      }
      constantTerm_[i] -= constantTerm_[nowIndex];
    }
}

std::vector<double> Sle::SleAlgorithm() {
    for (std::size_t nowIndex = 0; nowIndex < size_; nowIndex++) {
      std::size_t maxIndex = StepFindMaxIndex(nowIndex);
      if (std::abs(matrix_[maxIndex][nowIndex]) < eps_) return {};
      if (nowIndex != maxIndex) StepSwapCols(nowIndex, maxIndex);
      StepNormalize(nowIndex);
      StepSub(nowIndex);
    }
    return StepBackSubstitution();
}




} // namespace s21
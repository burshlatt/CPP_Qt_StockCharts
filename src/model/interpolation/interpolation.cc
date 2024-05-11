#include "interpolation.hpp"

namespace s21 {
Interpolator::Coords Interpolator::LoadFile(std::string_view path) {
    Clear();

    std::vector<long> x_coords;
    std::vector<double> y_coords;

    if (csv_.ParseCsv(path)) {
        auto csv_data{csv_.GetData()};

        for (const auto& [x_c, y_c] : csv_data) {
            x_.push_back(x_c);
            y_.push_back(y_c);
        }

        ComputeCoefficients();

        long first_day{x_.front()};
        long last_day{x_.back()};

        static constexpr int seconds_in_day{86400};

        for (long i{first_day}; i < last_day; i += seconds_in_day) {
            if (csv_data.find(i) == csv_data.end()) {
                csv_data[i] = Interpolate(i);
            }

            x_coords.push_back(i);
            y_coords.push_back(csv_data[i]);
        }
    }

    return std::make_pair(x_coords, y_coords);
}

void Interpolator::Clear() {
    x_.clear();
    y_.clear();
}

void Interpolator::ComputeCoefficients() {
    std::size_t n{x_.size()};

    splines_.resize(n);

    for (std::size_t i{}; i < n; ++i) {
        splines_[i].x = x_[i];
        splines_[i].a = y_[i];
    }

    splines_[0].c = splines_[n - 1].c = 0.0;

    double A{};
    double B{};
    double C{};
    double F{};
    double z{};
    double h_i{};
    double h_i1{};

    std::vector<double> alpha(n - 1);
    std::vector<double> beta(n - 1);

    for (std::size_t i{1}; i < n - 1; ++i) {
        h_i = x_[i] - x_[i - 1], h_i1 = x_[i + 1] - x_[i];
        A = h_i;
        B = h_i1 * 2.0;
        C = h_i1;
        F = 6.0 * ((y_[i + 1] - y_[i]) / h_i1 - (y_[i] - y_[i - 1]) / h_i);
        z = (A * alpha[i - 1] + B);

        alpha[i] = -C / z;
        beta[i] = (F - A * beta[i - 1]) / z;
    }

    for (int i{static_cast<int>(n) - 2}; i > 0; --i) {
        splines_[i].c = alpha[i] * splines_[i + 1].c + beta[i];
    }

    for (int i{static_cast<int>(n) - 1}; i > 0; --i) {
        long h_i{x_[i] - x_[i - 1]};

        splines_[i].d = (splines_[i].c - splines_[i - 1].c) / h_i;
        splines_[i].b = h_i * (2.0 * splines_[i].c + splines_[i - 1].c) / 6.0 + (y_[i] - y_[i - 1]) / h_i;
    }
}

double Interpolator::Interpolate(double x) {
    if (splines_.empty()) {
        return 0.0;
    }

    Splines* s{nullptr};

    if (x <= splines_[0].x) {
        s = &splines_[0];
    } else if (x >= splines_.back().x) {
        s = &splines_[splines_.size() - 1];
    } else {
        int i{};
        int j{static_cast<int>(splines_.size()) - 1};

        while (i + 1 < j) {
            int k{i + (j - i) / 2};

            if (x <= splines_[k].x) {
                j = k;
            } else {
                i = k;
            }
        }

        s = &splines_[j];
    }

    double dx{(x - s->x)};

    return s->a + (s->b + (s->c / 2.0 + s->d * dx / 6.0) * dx) * dx;
}
} // namespace s21

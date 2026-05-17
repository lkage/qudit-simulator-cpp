#pragma once
#include <Eigen/Dense>
#include <complex>
#include <string>

namespace sqdvqe {

using Complex = std::complex<double>;
using Matrix = Eigen::MatrixXcd;

class PauliOperator {
public:
    // matrix는 정사각 + Hermitian이어야 함. 검증 후 저장
    explicit PauliOperator(const Matrix& matrix, std::string label = "");

    int dimension() const { return static_cast<int>(matrix_.rows()); }
    const Matrix& matrix() const { return matrix_; }
    const std::string& label() const { return label_; }

private:
    Matrix matrix_;
    std::string label_;
};

// 단일 큐비트 Pauli 행렬 (2x2)
namespace pauli {
    const PauliOperator& I();
    const PauliOperator& X();
    const PauliOperator& Y();
    const PauliOperator& Z();
}

} // namespace sqdvqe
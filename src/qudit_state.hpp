#pragma once
#include <Eigen/Dense>
#include <complex>

namespace sqdvqe {

using Complex = std::complex<double>;
using StateVector = Eigen::VectorXcd;

class QuditState {
public:
    // |0> 상태로 초기화된 d차원 qudit 생성
    explicit QuditState(int dimension);

    // 주어진 상태벡터로 초기화 (자동 정규화 X, 검증만)
    explicit QuditState(const StateVector& amplitudes);

    // 정규화되지 않은 벡터를 받아 정규화 후 생성
    static QuditState normalize_and_create(const StateVector& amplitudes);
    
    int dimension() const { return static_cast<int>(amplitudes_.size()); }
    const StateVector& amplitudes() const { return amplitudes_; }

    double norm_squared() const;
    void normalize();

    Complex inner_product(const QuditState& other) const;
    
    double probability(int basis_index) const;

    //static QuditState basis_state(int dimension, int index);<-머지이건?

private:
    StateVector amplitudes_;
};

} // namespace sqdvqe
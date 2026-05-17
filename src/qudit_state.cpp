#include "qudit_state.hpp"
#include <stdexcept>

namespace sqdvqe {

namespace {
    constexpr double kNormTolerance = 1e-10;
}

QuditState::QuditState(int dimension) : amplitudes_(dimension) {
    if (dimension <= 0) {
        throw std::invalid_argument("Dimension must be positive");
    }
    amplitudes_.setZero();
    amplitudes_(0) = Complex(1.0, 0.0);
}

QuditState::QuditState(const StateVector& amplitudes) : amplitudes_(amplitudes) {
    if (amplitudes.size() == 0) {
        throw std::invalid_argument("State vector cannot be empty");
    }
    double n2 = amplitudes.squaredNorm();
    if (std::abs(n2 - 1.0) > kNormTolerance) {
        throw std::invalid_argument("State vector must be normalized");
    }
}

QuditState QuditState::normalize_and_create(const StateVector& amplitudes) {
    if (amplitudes.size() == 0) {
        throw std::invalid_argument("State vector cannot be empty");
    }
    double n2 = amplitudes.squaredNorm();
    if (n2 < 1e-15) {
        throw std::invalid_argument("Cannot normalize zero vector");
    }
    return QuditState(amplitudes / std::sqrt(n2));
}

double QuditState::norm_squared() const {
    return amplitudes_.squaredNorm();
}

void QuditState::normalize() {
    double n = std::sqrt(norm_squared());
    if (n < 1e-15) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    amplitudes_ /= n;
}

Complex QuditState::inner_product(const QuditState& other) const {
    if (other.dimension() != dimension()) {
        throw std::invalid_argument("Dimension mismatch in inner_product");
    }
    return amplitudes_.dot(other.amplitudes_);
}

double QuditState::probability(int basis_index) const{
        if (basis_index < 0 || basis_index >= dimension()) {
        throw std::out_of_range("basis_index out of range");
    }
    return std::norm(amplitudes_(basis_index));
}

//static QuditState::QuditState basis_state(int dimension, int index) {}<-머지이건? 왜생겨있지?


} // namespace sqdvqe
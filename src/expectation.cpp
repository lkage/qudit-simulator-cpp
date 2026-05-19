#include "expectation.hpp"
#include <stdexcept>
#include <cmath>

namespace sqdvqe {

namespace {
    constexpr double kImagTolerance = 1e-10;
}

double expectation_value(const QuditState& state, const PauliOperator& op) {
    if (state.dimension() != op.dimension()) {
        throw std::invalid_argument(
            "Dimension mismatch between state and operator");
    }
    
    // ψ† P ψ
    Complex result = state.amplitudes().dot(op.matrix() * state.amplitudes());
    
    if (std::abs(result.imag()) > kImagTolerance) {
        throw std::runtime_error(
            "Expectation value has non-trivial imaginary part; "
            "operator may not be Hermitian");
    }
    
    return result.real();
}

double hamiltonian_expectation(const QuditState& state, const HamiltonianTerms& terms) {
    if (terms.empty()) {
        throw std::invalid_argument("Hamiltonian terms cannot be empty");
    }
    
    double total = 0.0;
    for (const auto& [weight, op] : terms) {
        total += weight * expectation_value(state, op);
    }
    return total;
}

} // namespace sqdvqe
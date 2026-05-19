#pragma once
#include "qudit_state.hpp"
#include "pauli_operator.hpp"
#include <vector>
#include <utility>

namespace sqdvqe {

// <ψ|P|ψ> 계산. Hermitian operator의 결과는 실수.
// state와 op의 dimension이 다르면 invalid_argument.
// 결과의 imaginary part가 tolerance 초과면 runtime_error.
double expectation_value(const QuditState& state, const PauliOperator& op);

// <ψ|H|ψ> for H = Σ W_m P_m
// 모든 P_m의 dimension이 동일하고 state와 같아야 함.
// terms의 형식: (weight, PauliOperator) 페어 리스트
using HamiltonianTerms = std::vector<std::pair<double, PauliOperator>>;

double hamiltonian_expectation(const QuditState& state, const HamiltonianTerms& terms);

} // namespace sqdvqe
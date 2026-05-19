#include "expectation.hpp"
#include <gtest/gtest.h>
#include <cmath>

using namespace sqdvqe;

// 가장 기본: <0|Z|0> = +1, <1|Z|1> = -1
TEST(ExpectationTest, ZOnZeroState) {
    QuditState zero(2);  // |0>
    EXPECT_NEAR(expectation_value(zero, pauli::Z()), 1.0, 1e-12);
}

TEST(ExpectationTest, ZOnOneState) {
    StateVector v(2);
    v << Complex(0, 0), Complex(1, 0);
    QuditState one(v);  // |1>
    EXPECT_NEAR(expectation_value(one, pauli::Z()), -1.0, 1e-12);
}

// <+|X|+> = +1, <-|X|-> = -1
TEST(ExpectationTest, XOnPlusState) {
    StateVector v(2);
    double inv_sqrt2 = 1.0 / std::sqrt(2.0);
    v << Complex(inv_sqrt2, 0), Complex(inv_sqrt2, 0);
    QuditState plus(v);
    EXPECT_NEAR(expectation_value(plus, pauli::X()), 1.0, 1e-12);
}

TEST(ExpectationTest, XOnMinusState) {
    StateVector v(2);
    double inv_sqrt2 = 1.0 / std::sqrt(2.0);
    v << Complex(inv_sqrt2, 0), Complex(-inv_sqrt2, 0);
    QuditState minus(v);
    EXPECT_NEAR(expectation_value(minus, pauli::X()), -1.0, 1e-12);
}

// <0|X|0> = 0 (orthogonal)
TEST(ExpectationTest, XOnZeroIsZero) {
    QuditState zero(2);
    EXPECT_NEAR(expectation_value(zero, pauli::X()), 0.0, 1e-12);
}

// <0|I|0> = 1 (norm)
TEST(ExpectationTest, IdentityOnAnyStateIsOne) {
    QuditState zero(2);
    EXPECT_NEAR(expectation_value(zero, pauli::I()), 1.0, 1e-12);
    
    StateVector v(2);
    v << Complex(0.6, 0), Complex(0, 0.8);
    QuditState mixed(v);
    EXPECT_NEAR(expectation_value(mixed, pauli::I()), 1.0, 1e-12);
}

// <00|IZ|00> = +1, <01|IZ|01> = -1
TEST(ExpectationTest, IZOnTwoQubitStates) {
    auto IZ = pauli_string("IZ");
    
    // |00>
    StateVector v00(4);
    v00 << Complex(1, 0), Complex(0, 0), Complex(0, 0), Complex(0, 0);
    EXPECT_NEAR(expectation_value(QuditState(v00), IZ), 1.0, 1e-12);
    
    // |01>
    StateVector v01(4);
    v01 << Complex(0, 0), Complex(1, 0), Complex(0, 0), Complex(0, 0);
    EXPECT_NEAR(expectation_value(QuditState(v01), IZ), -1.0, 1e-12);
    
    // |10>
    StateVector v10(4);
    v10 << Complex(0, 0), Complex(0, 0), Complex(1, 0), Complex(0, 0);
    EXPECT_NEAR(expectation_value(QuditState(v10), IZ), 1.0, 1e-12);
}

// Dimension mismatch
TEST(ExpectationTest, DimensionMismatch) {
    QuditState s4(4);
    EXPECT_THROW(expectation_value(s4, pauli::Z()), std::invalid_argument);
}

// 가중합: H = 0.5 Z + 0.3 X on |0>
// <0|H|0> = 0.5 * <0|Z|0> + 0.3 * <0|X|0> = 0.5 + 0 = 0.5
TEST(ExpectationTest, WeightedSumSimple) {
    QuditState zero(2);
    HamiltonianTerms H = {
        {0.5, pauli::Z()},
        {0.3, pauli::X()}
    };
    EXPECT_NEAR(hamiltonian_expectation(zero, H), 0.5, 1e-12);
}

// H₂ Hamiltonian at R=0.73 (Table S1)
// H = -0.326 II + 0.401 IZ - 0.401 ZI - 0.011 ZZ + 0.181 XX
// 알려진 ground state energy ≈ -1.137 Hartree
// Eigen으로 직접 대각화한 결과와 비교
TEST(ExpectationTest, H2GroundStateEnergyMatchesDiagonalization) {
    HamiltonianTerms H = {
        {-0.326386, pauli_string("II")},
        { 0.401061, pauli_string("IZ")},
        {-0.401061, pauli_string("ZI")},
        {-0.011314, pauli_string("ZZ")},
        { 0.180653, pauli_string("XX")}
    };
    
    // Hamiltonian 행렬 직접 빌드
    Matrix H_matrix = Matrix::Zero(4, 4);
    for (const auto& [w, op] : H) {
        H_matrix += w * op.matrix();
    }
    
    // 대각화로 lowest eigenvalue 구하기
    Eigen::SelfAdjointEigenSolver<Matrix> solver(H_matrix);
    double lowest_eigenvalue = solver.eigenvalues()(0);
    
    // 그 eigenvector로 expectation value 계산
    StateVector ground_state = solver.eigenvectors().col(0);
    QuditState ground(ground_state);
    
    double E = hamiltonian_expectation(ground, H);
    
    EXPECT_NEAR(E, lowest_eigenvalue, 1e-10);
    
    // 알려진 H2 ground state energy (~-1.137 Hartree)와 가까운지
    EXPECT_NEAR(E, -1.137, 0.01);  // 1e-2 tolerance (Hamiltonian 계수 자체가 근사)
}
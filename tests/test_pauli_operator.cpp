#include "pauli_operator.hpp"
#include <gtest/gtest.h>

using namespace sqdvqe;

TEST(PauliOperatorTest, RejectsNonSquareMatrix) {
    Matrix m(2, 3);
    m.setZero();
    EXPECT_THROW(PauliOperator{m}, std::invalid_argument);
}

TEST(PauliOperatorTest, RejectsNonHermitianMatrix) {
    Matrix m(2, 2);
    m << Complex(0, 0), Complex(1, 0),
         Complex(0, 0), Complex(0, 0);  // 비대칭, 비-Hermitian
    EXPECT_THROW(PauliOperator{m}, std::invalid_argument);
}

TEST(PauliOperatorTest, IdentityIsHermitian) {
    EXPECT_EQ(pauli::I().dimension(), 2);
    EXPECT_EQ(pauli::I().label(), "I");
}

TEST(PauliOperatorTest, XSquaredEqualsIdentity) {
    Matrix x_squared = pauli::X().matrix() * pauli::X().matrix();
    Matrix identity = Matrix::Identity(2, 2);
    EXPECT_NEAR((x_squared - identity).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, YSquaredEqualsIdentity) {
    Matrix y_squared = pauli::Y().matrix() * pauli::Y().matrix();
    Matrix identity = Matrix::Identity(2, 2);
    EXPECT_NEAR((y_squared - identity).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, ZSquaredEqualsIdentity) {
    Matrix z_squared = pauli::Z().matrix() * pauli::Z().matrix();
    Matrix identity = Matrix::Identity(2, 2);
    EXPECT_NEAR((z_squared - identity).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, XYAnticommute) {
    // {X, Y} = XY + YX = 0
    Matrix xy = pauli::X().matrix() * pauli::Y().matrix();
    Matrix yx = pauli::Y().matrix() * pauli::X().matrix();
    EXPECT_NEAR((xy + yx).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, XYEqualsiZ) {
    // XY = iZ
    Matrix xy = pauli::X().matrix() * pauli::Y().matrix();
    Matrix iz = Complex(0, 1) * pauli::Z().matrix();
    EXPECT_NEAR((xy - iz).norm(), 0.0, 1e-12);
}
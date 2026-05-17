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

TEST(PauliOperatorTest, TensorProductDimension) {
    auto IZ = tensor_product(pauli::I(), pauli::Z());
    EXPECT_EQ(IZ.dimension(), 4);
    EXPECT_EQ(IZ.label(), "IZ");
}

TEST(PauliOperatorTest, TensorProductIZ) {
    // I ⊗ Z = diag(1, -1, 1, -1)
    auto IZ = tensor_product(pauli::I(), pauli::Z());
    Matrix expected(4, 4);
    expected << Complex(1, 0),  Complex(0, 0), Complex(0, 0),  Complex(0, 0),
                Complex(0, 0), Complex(-1, 0), Complex(0, 0),  Complex(0, 0),
                Complex(0, 0),  Complex(0, 0), Complex(1, 0),  Complex(0, 0),
                Complex(0, 0),  Complex(0, 0), Complex(0, 0), Complex(-1, 0);
    EXPECT_NEAR((IZ.matrix() - expected).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, TensorProductZI) {
    // Z ⊗ I = diag(1, 1, -1, -1)
    auto ZI = tensor_product(pauli::Z(), pauli::I());
    Matrix expected(4, 4);
    expected <<  Complex(1, 0), Complex(0, 0),  Complex(0, 0),  Complex(0, 0),
                 Complex(0, 0), Complex(1, 0),  Complex(0, 0),  Complex(0, 0),
                 Complex(0, 0), Complex(0, 0), Complex(-1, 0),  Complex(0, 0),
                 Complex(0, 0), Complex(0, 0),  Complex(0, 0), Complex(-1, 0);
    EXPECT_NEAR((ZI.matrix() - expected).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, TensorProductIsAssociative) {
    // (I ⊗ Z) ⊗ X == I ⊗ (Z ⊗ X)
    auto IZ_then_X = tensor_product(tensor_product(pauli::I(), pauli::Z()), pauli::X());
    auto I_then_ZX = tensor_product(pauli::I(), tensor_product(pauli::Z(), pauli::X()));
    EXPECT_NEAR((IZ_then_X.matrix() - I_then_ZX.matrix()).norm(), 0.0, 1e-12);
    EXPECT_EQ(IZ_then_X.label(), "IZX");
    EXPECT_EQ(I_then_ZX.label(), "IZX");
}

TEST(PauliOperatorTest, PauliStringBuilderSingle) {
    auto Z = pauli_string("Z");
    EXPECT_EQ(Z.dimension(), 2);
    EXPECT_EQ(Z.label(), "Z");
    EXPECT_NEAR((Z.matrix() - pauli::Z().matrix()).norm(), 0.0, 1e-12);
}

TEST(PauliOperatorTest, PauliStringBuilderTwoQubit) {
    auto IZ = pauli_string("IZ");
    auto manual_IZ = tensor_product(pauli::I(), pauli::Z());
    EXPECT_NEAR((IZ.matrix() - manual_IZ.matrix()).norm(), 0.0, 1e-12);
    EXPECT_EQ(IZ.label(), "IZ");
}

TEST(PauliOperatorTest, PauliStringBuilderFourQubit) {
    // LiH의 Pauli string 중 하나: "IZXZ" (Table S2)
    auto op = pauli_string("IZXZ");
    EXPECT_EQ(op.dimension(), 16);  // 2^4
    EXPECT_EQ(op.label(), "IZXZ");
}

TEST(PauliOperatorTest, PauliStringRejectsInvalidChar) {
    EXPECT_THROW(pauli_string("IA"), std::invalid_argument);
    EXPECT_THROW(pauli_string("Q"), std::invalid_argument);
    EXPECT_THROW(pauli_string(""), std::invalid_argument);
}

TEST(PauliOperatorTest, PauliStringResultIsHermitian) {
    // 빌더가 만든 결과도 Hermitian이어야 함 (생성자가 검증함)
    EXPECT_NO_THROW(pauli_string("XYZX"));
    EXPECT_NO_THROW(pauli_string("ZZZZ"));
}
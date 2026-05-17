#include "qudit_state.hpp"
#include <gtest/gtest.h>

using namespace sqdvqe;

TEST(QuditStateTest, DefaultInitToZeroState) {
    QuditState s(4);
    EXPECT_EQ(s.dimension(), 4);
    EXPECT_DOUBLE_EQ(s.norm_squared(), 1.0);
    EXPECT_EQ(s.amplitudes()(0), Complex(1.0, 0.0));
    EXPECT_EQ(s.amplitudes()(1), Complex(0.0, 0.0));
}

TEST(QuditStateTest, RejectsZeroDimension) {
    EXPECT_THROW(QuditState(0), std::invalid_argument);
}

TEST(QuditStateTest, RejectsUnnormalizedInput) {
    StateVector v(2);
    v << Complex(3.0, 0.0), Complex(4.0, 0.0);
    EXPECT_THROW(QuditState{v}, std::invalid_argument);
}

TEST(QuditStateTest, AcceptsNearlyNormalizedInput) {
    StateVector v(2);
    v << Complex(1.0, 0.0), Complex(1e-11, 0.0);
    EXPECT_NO_THROW(QuditState{v});
}

TEST(QuditStateTest, NormalizeAndCreate) {
    StateVector v(2);
    v << Complex(3.0, 0.0), Complex(4.0, 0.0);
    QuditState s = QuditState::normalize_and_create(v);
    EXPECT_NEAR(s.norm_squared(), 1.0, 1e-12);
    EXPECT_NEAR(s.probability(0), 0.36, 1e-12);  // (3/5)^2
    EXPECT_NEAR(s.probability(1), 0.64, 1e-12);  // (4/5)^2
}

TEST(QuditStateTest, NormalizeAndCreateRejectsZeroVector) {
    StateVector v(2);
    v << Complex(0.0, 0.0), Complex(0.0, 0.0);
    EXPECT_THROW(QuditState::normalize_and_create(v), std::invalid_argument);
}

TEST(QuditStateTest, InnerProductWithSelfEqualsNormSquared) {
    StateVector v(2);
    v << Complex(0.6, 0.0), Complex(0.0, 0.8);  // |v|^2 = 1
    QuditState s(v);
    Complex ip = s.inner_product(s);
    EXPECT_NEAR(ip.real(), 1.0, 1e-12);
    EXPECT_NEAR(ip.imag(), 0.0, 1e-12);  // 자기자신 내적은 실수여야 함
}

TEST(QuditStateTest, InnerProductOrthogonalStates) {
    QuditState s0(2);  // |0>
    StateVector v(2);
    v << Complex(0.0, 0.0), Complex(1.0, 0.0);
    QuditState s1(v);  // |1>
    Complex ip = s0.inner_product(s1);
    EXPECT_NEAR(std::abs(ip), 0.0, 1e-12);
}

TEST(QuditStateTest, InnerProductConjugateSymmetry) {
    // <a|b> = conj(<b|a>) 여야 함
    StateVector va(2), vb(2);
    va << Complex(1.0/std::sqrt(2.0), 0.0), Complex(0.0, 1.0/std::sqrt(2.0));
    vb << Complex(0.0, 1.0/std::sqrt(2.0)), Complex(1.0/std::sqrt(2.0), 0.0);
    QuditState a(va), b(vb);
    Complex ab = a.inner_product(b);
    Complex ba = b.inner_product(a);
    EXPECT_NEAR(ab.real(), ba.real(), 1e-12);
    EXPECT_NEAR(ab.imag(), -ba.imag(), 1e-12);
}

TEST(QuditStateTest, InnerProductDimensionMismatch) {
    QuditState s2(2), s4(4);
    EXPECT_THROW(s2.inner_product(s4), std::invalid_argument);
}


TEST(QuditStateTest, ProbabilityOfZeroState) {
    QuditState s(5);  // |0>
    EXPECT_DOUBLE_EQ(s.probability(0), 1.0);
    EXPECT_DOUBLE_EQ(s.probability(1), 0.0);
}

TEST(QuditStateTest, ProbabilityOfSuperposition) {
    StateVector v(2);
    v << Complex(1.0/std::sqrt(2.0), 0.0), Complex(0.0, 1.0/std::sqrt(2.0));
    QuditState s(v);
    EXPECT_NEAR(s.probability(0), 0.5, 1e-12);
    EXPECT_NEAR(s.probability(1), 0.5, 1e-12);
}

TEST(QuditStateTest, ProbabilityOutOfRange) {
    QuditState s(4);
    EXPECT_THROW(s.probability(-1), std::out_of_range);
    EXPECT_THROW(s.probability(4), std::out_of_range);
}
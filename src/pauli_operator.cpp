#include "pauli_operator.hpp"
#include <stdexcept>
#include <unsupported/Eigen/KroneckerProduct>

namespace sqdvqe {

namespace {
    constexpr double kHermitianTolerance = 1e-10;
}

PauliOperator::PauliOperator(const Matrix& matrix, std::string label)
    : matrix_(matrix), label_(std::move(label)) {
    if (matrix.rows() == 0 || matrix.cols() == 0) {
        throw std::invalid_argument("Matrix cannot be empty");
    }
    if (matrix.rows() != matrix.cols()) {
        throw std::invalid_argument("Matrix must be square");
    }
    double diff = (matrix - matrix.adjoint()).norm();
    if (diff > kHermitianTolerance) {
        throw std::invalid_argument("Matrix must be Hermitian");
    }
}

namespace pauli {

const PauliOperator& I() {
    static const PauliOperator op = [] {
        Matrix m(2, 2);
        m << Complex(1, 0), Complex(0, 0),
             Complex(0, 0), Complex(1, 0);
        return PauliOperator(m, "I");
    }();
    return op;
}

const PauliOperator& X() {
    static const PauliOperator op = [] {
        Matrix m(2, 2);
        m << Complex(0, 0), Complex(1, 0),
             Complex(1, 0), Complex(0, 0);
        return PauliOperator(m, "X");
    }();
    return op;
}

const PauliOperator& Y() {
    static const PauliOperator op = [] {
        Matrix m(2, 2);
        m << Complex(0, 0), Complex(0, -1),
             Complex(0, 1), Complex(0, 0);
        return PauliOperator(m, "Y");
    }();
    return op;
}

const PauliOperator& Z() {
    static const PauliOperator op = [] {
        Matrix m(2, 2);
        m << Complex(1, 0), Complex(0, 0),
             Complex(0, 0), Complex(-1, 0);
        return PauliOperator(m, "Z");
    }();
    return op;
}

} // namespace pauli

PauliOperator tensor_product(const PauliOperator& a, const PauliOperator& b) {
    Matrix result = Eigen::kroneckerProduct(a.matrix(), b.matrix());
    return PauliOperator(result, a.label() + b.label());
}

PauliOperator pauli_string(const std::string& label) {
    if (label.empty()) {
        throw std::invalid_argument("Pauli string label cannot be empty");
    }
    
    auto char_to_pauli = [](char c) -> const PauliOperator& {
        switch (c) {
            case 'I': return pauli::I();
            case 'X': return pauli::X();
            case 'Y': return pauli::Y();
            case 'Z': return pauli::Z();
            default:
                throw std::invalid_argument(
                    std::string("Invalid Pauli character: ") + c);
        }
    };
    
    // 첫 문자로 초기화
    PauliOperator result = char_to_pauli(label[0]);
    
    // 나머지 문자들을 텐서곱으로 누적
    for (size_t i = 1; i < label.size(); ++i) {
        result = tensor_product(result, char_to_pauli(label[i]));
    }
    
    return result;
}

} // namespace sqdvqe
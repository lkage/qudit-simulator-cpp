# qudit-simulator-cpp

A C++ library for simulating high-dimensional quantum states (qudits),
built with Eigen and tested with GoogleTest.

This is a personal study project: I'm implementing the mathematics of qudit
quantum information from scratch in C++, both to deepen my understanding and
to keep a portable C++ codebase. The first concrete application is reproducing
the algorithmic portion of:

> Kim et al., "Qudit-based variational quantum eigensolver using photonic
> orbital angular momentum states", *Sci. Adv.* **10**, eado3472 (2024).
> [DOI: 10.1126/sciadv.ado3472](https://www.science.org/doi/10.1126/sciadv.ado3472)

A companion Python repository will host the full VQE reproduction
(Hamiltonian generation via Qiskit Nature, ansatz parameterization, COBYLA
optimization, etc.). This C++ library serves as an independent numerical
backbone for that work.

## Scope

This is a **single-qudit state simulator**. Its responsibility is narrow and
intentional:

- Represent normalized $d$-dimensional quantum states
- Represent Hermitian operators (Pauli strings and their weighted sums)
- Compute exact expectation values $\langle\psi|H|\psi\rangle$

What this library does **not** do:

- Quantum chemistry calculations (Hamiltonian generation is delegated to
  Qiskit Nature / PySCF on the Python side)
- Qudit gate operations (the target paper parameterizes ansatz states
  directly, cf. Eqs. 5 and 9 of the paper, so gates are not required)
- Shot-noise sampling (exact expectation values only)
- Optimizer routines (delegated to SciPy COBYLA on the Python side)

This narrow scope is deliberate. The simulator is the precise, fast numerical
kernel; everything else lives upstream.

## Status (Current Project)

- [x] **Week 1**: Project setup, `QuditState` class with normalized-state invariant
- [ ] **Week 2**: `PauliOperator` class, single-qubit Pauli matrices, tensor product builder, expectation value
- [ ] **Week 3**: H₂ Hamiltonian validation, code cleanup, example program

## Roadmap (Beyond the Initial Project)

The current scope is intentionally narrow — exact expectation values on
single-qudit states, sufficient for reproducing the target paper. Beyond
that initial goal, this library is intended to grow into a more complete
qudit simulator. Planned directions:

- Single-qudit gates
- Circuit representation and unitary evolution
- Multi-qudit systems and entanglement
- Noise models 
- Density matrix representation for mixed states
- etc

These are study goals, pursued incrementally as understanding deepens. The
"simulator" in the name reflects this trajectory rather than the current
feature set.

## Requirements

- C++20 compiler (tested with GCC 13.3)
- CMake 3.20+
- Eigen 3.4+
- GoogleTest

On Ubuntu 24.04:

```bash
sudo apt install -y build-essential cmake libeigen3-dev libgtest-dev
```

## Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest --output-on-failure
```

## Project Structure

```
qudit-simulator-cpp/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── qudit_state.{hpp,cpp}
│   └── pauli_operator.{hpp,cpp}    (week 2)
└── tests/
    ├── test_qudit_state.cpp
    └── test_pauli_operator.cpp     (week 2)
```

## Design Notes

### Normalized-state invariant

`QuditState` enforces normalization at construction. Any `QuditState` object
that exists is guaranteed to represent a valid quantum state ($\sum_i |c_i|^2 = 1$
within tolerance). Two construction paths:

```cpp
// Path 1: provide an already-normalized vector (verified)
QuditState s(amplitudes);

// Path 2: provide an arbitrary vector, let the class normalize
QuditState s = QuditState::normalize_and_create(amplitudes);
```

The first path throws if the input is not normalized, making bugs in unitary
evolution visible immediately rather than silently corrected.

### Pauli operators as plain $d \times d$ matrices

The paper's key abstraction is that an $N$-qubit Pauli string ($2^N \times 2^N$
matrix) can be reinterpreted as a single $d = 2^N$ qudit operator. The matrix
is the same; only the physical interpretation changes.

This simulator follows that abstraction: Pauli operators are $d \times d$
Hermitian matrices, period. The tensor-product structure that produced them
(from single-qubit $I, X, Y, Z$) is handled by builder functions, not by the
operator class itself.

## References

Paper PDF and supplementary materials are not redistributed here.
Access via the journal: [Sci. Adv. 10, eado3472 (2024)](https://www.science.org/doi/10.1126/sciadv.ado3472).
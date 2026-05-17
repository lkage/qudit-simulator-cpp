# Qudit Simulator

A C++ library for simulating high-dimensional quantum states (qudits),
built with Eigen and tested with GoogleTest.

Developed as the computational backbone for reproducing
[Kim et al., *Sci. Adv.* **10**, eado3472 (2024)](https://www.science.org/doi/10.1126/sciadv.ado3472):
"Qudit-based variational quantum eigensolver using photonic orbital angular momentum states".

The companion Python repository for the VQE reproduction will be linked here when available.

## Roadmap

- [x] `QuditState`: normalized d-dimensional state vector with inner product, probability
- [ ] Pauli operators in qudit representation
- [ ] Expectation value computation for Pauli strings
- [ ] (Optional) Single-qudit gates and ansatz state preparation
- [ ] Examples: H₂ and LiH Hamiltonians

## Requirements

- C++20 compiler (tested with GCC 13)
- CMake 3.20+
- Eigen 3.4+
- GoogleTest

## Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest --output-on-failure
```

Qudit-Simulator/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── src/
│   ├── CMakeLists.txt
│   ├── qudit_state.hpp
│   └── qudit_state.cpp
└── tests/
    ├── CMakeLists.txt
    └── test_qudit_state.cpp
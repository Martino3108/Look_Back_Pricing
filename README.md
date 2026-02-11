# LookBack Pricing Library

## Overview

The **LookBack Pricing Library** is a C++ university project focused on the **Monte Carlo pricing of lookback options**.

The goal of the project is to combine:

* financial theory (stochastic modeling and option pricing), and
* practical implementation aspects (C++, numerical methods, and basic performance considerations).

The library is documented using **Doxygen**, and this file serves as the **main documentation page**.

---

## Objectives of the Project

This project was developed with the following educational objectives:

* Implement Monte Carlo pricing for path-dependent options
* Understand the numerical implications of simulating stochastic processes
* Apply risk-neutral valuation in practice
* Gain experience with structured C++ code for quantitative finance
* Learn how to document a numerical project using Doxygen

---

## Financial Model


The pricing methodology implemented in this project is based on a **continuous-time stochastic framework**, following the exact simulation of lookback options as presented in Crépey (2013).

Let \f$X_t\f$ denote a one-dimensional diffusion process defined on the time interval \f$[0,T]\f$, satisfying the stochastic differential equation:

$$dX_t = b \ dt + \sigma \, dW_t,$$

where $W_t$ is a standard Brownian motion. This dynamics corresponds to the explicit solution of the Black-Scholes model (applied to log-prices).
The associated running maximum process is defined as:

$$M_t = \sup_{0 \le s \le t} X_s.$$

A lookback option payoff is written in the general form \f$\phi(X_T, M_T)\f$.

Since the model assumes constant coefficients, the SDE admits a **closed-form solution**. Therefore, the Monte Carlo pricing procedure does not require a time-discretized approximation. Instead, it relies on the **exact simulation of the joint law** of the terminal value and the running maximum, \f$(X_T, M_T)\f$.

Specifically, the terminal value \f$X_T\f$ is simulated directly from its Gaussian distribution. Conditioned on the value of \f$X_T\f$, the running maximum \f$M_T\f$ follows a known distribution (derived from the reflection principle for Brownian motion with drift), which can be sampled exactly using an auxiliary independent uniform random variable.

This approach preserves the **continuous-time nature of the maximum process** without any discretization bias, ensuring high accuracy and computational efficiency within the Monte Carlo framework.

---

## Numerical Method

* Monte Carlo simulation of asset price paths
* Log-space simulation for improved numerical stability
* Estimation of the discounted expected payoff
* Basic performance optimization using OpenMP parallelization

---

## Project Structure

The project is organized in a **simple and compact structure**, consistent with a university assignment:

```
Look_Back_Pricing/
│
├── *.h               # Header files
├── *.cpp             # C++ source files
├── LookBackUI.xlsm   # Excel/VBA interface (macro-enabled workbook)
├── Doxyfile          # Doxygen configuration file
├── html/             # Generated Doxygen documentation
└── README.md         # Main documentation page
```

The focus of the project is on the **pricing logic and numerical implementation**, rather than on software engineering abstractions.

---

## Build Instructions

### Requirements

* C++17 compatible compiler
* OpenMP (for parallel execution)

### Compilation Example

The following example shows how to compile the project as a **dynamic library (`.dylib`) on macOS**, with OpenMP support via `libomp` installed through Homebrew:

```bash
clang++ -std=c++20 -O3 -fPIC -dynamiclib \
  Look_Back.cpp Date_Dealing.cpp LookBackDll.cpp \
  -Xpreprocessor -fopenmp \
  -I"$(brew --prefix libomp)/include" \
  -L"$(brew --prefix libomp)/lib" \
  -Wl,-rpath,"$(brew --prefix libomp)/lib" \
  -lomp \
  -Wl,-install_name,@rpath/libLookBack.dylib \
  -o libLookBack.dylib
```

This command produces the dynamic library `libLookBack.dylib`, which can be used directly from Excel/VBA on macOS.

---

## VBA Interface and DLL Path Configuration

When using the library from **Excel/VBA** (via the provided `.xlsm` interface), the path to the compiled dynamic library **must be adjusted manually** to match the local system.

In particular, the `Lib` attribute in each `Declare PtrSafe Function` statement must point to the actual location of `libLookBack.dylib` on the user's machine.

For example:

```vb
Option Explicit

' DLL declaration (macOS)
Public Declare PtrSafe Function LB_CreateA _
    Lib "/Users/Martino/Library/Containers/com.microsoft.Excel/Data/libLookBack.dylib" ( _
    ByVal S0 As Double, _
    ByVal valueDate As String, _
    ByVal maturityDate As String, _
    ByVal sigma As Double, _
    ByVal r As Double, _
    ByVal optAscii As Long, _
    ByVal hBump As Double, _
    ByVal ddc As Long _
) As LongPtr
```

If the library is moved to a different directory or the project is run on another machine, **this path must be updated accordingly**. Failing to do so will result in Excel being unable to load the DLL.

---

## Documentation

The documentation is generated using **Doxygen**.

To generate the documentation:

```bash
doxygen Doxyfile
```

Open the file `html/index.html` in a browser to view the documentation.

---

## References

The theoretical and numerical approach used in this project is based exclusively on the following reference:

* **Stéphane Crépey**, *Financial Modeling: A Backward Stochastic Differential Equations Perspective*, Springer, 2013.

In particular, the Monte Carlo simulation of lookback options follows the methodology described in:

* Chapter 6: *Monte Carlo Methods for Exotic Options*
* Section 6.9: *Lookback Options*
* Subsection 6.9.1.1: *Black–Scholes Case*

The simulation of the joint distribution of the terminal log-price and its running maximum,
$ (X_T, M_T) $, is performed by sampling the conditional distribution of the maximum over each time interval using an auxiliary uniform random variable, as described in equation (6.20) of the reference.

This reference provides the sole theoretical foundation for the pricing methodology implemented in the library.

---

## Authors

* **Marco De Luca**
* **Vincenzo Martino Pio Arena**

---

## Notes

This project is intended for **academic purposes only** and was developed as part of a university course in C++ coding.

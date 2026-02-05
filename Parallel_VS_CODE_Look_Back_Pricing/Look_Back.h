/**
 * @file Look_Back.h
 * @brief Core C++ lookback option pricer interface.
 *
 * @details
 * Defines the `look_back` class which encapsulates contract inputs (dates, option type),
 * market parameters (sigma, rate), and numerical controls for Greeks (finite-difference step).
 *
 * The class offers:
 * - Monte Carlo pricing (option payoff estimated under GBM assumptions).
 * - Greeks computed via finite differences around the stored baseline parameters.
 * - Simple graph helpers for price/delta as a function of spot.
 *
 * Error model:
 * - The core C++ implementation may throw exceptions (e.g., Invalid_Parameters).
 * - The ABI bridge (LookBackDll.cpp) is responsible for catching and reporting them.
 */

/** @defgroup LB_Core Core (C++)
 *  @brief Core pricing logic (C++ exceptions allowed).
 *  @{
 */

#ifndef LOOK_BACK_H
#define LOOK_BACK_H
#include <iostream>
#include <random>

#include "Date_Dealing.h"
#include "Invalid_Parameters.h"

/// Alias used for graph output (x,y vectors).
typedef std::vector<double> vect;


/**
 * @class look_back
 * @brief Monte Carlo lookback option pricer with finite-difference Greeks.
 *
 * @details
 * This class stores contract and market inputs and provides pricing/Greek methods.
 * Dates are converted into a time-to-maturity (`ttm_`) via `yearFraction()`.
 *
 * Notes:
 * - `option_` is normalized to lowercase ('c' or 'p').
 * - The constructor validates inputs via `Look_Back_Validator::validate()`.
 * - Some methods infer Monte Carlo sample size from the finite-difference step.
 *
 * @warning
 * The internal numerical choices (e.g., `N = 1 / h^4`) can explode computational cost.
 */
class look_back
{
    
private:
    mutable std::mt19937_64 gen_; // generatore che si usa su monte carlo, dato da opinioni generiche online
    double S0_;
    
    Date value_date_;
    Date maturity_date_;
    double ttm_; // time to maturity
    double sigma_;
    double interest_rate_;
    char option_;
    double h_;
    
public:
    /**
     * @brief Constructs a pricer instance and validates inputs.
     *
     * @param S0 Initial spot.
     * @param value_date Valuation date.
     * @param maturity_date Maturity date.
     * @param sigma Volatility.
     * @param interest_rate Risk-free rate (non-negative in your validator).
     * @param option Option type ('c' call, 'p' put).
     * @param h Finite-difference step for Greeks.
     * @param ddc Day-count convention used to compute time to maturity.
     *
     * @throws Invalid_Parameters if validation fails.
     */
    explicit look_back(double S0, Date value_date, Date maturity_date, double sigma, double interest_rate, char option, double h, DayCountConv ddc  = DayCountConv::ACT_365F)
    
    :  S0_(S0), value_date_(value_date), maturity_date_(maturity_date), ttm_(yearFraction(value_date, maturity_date, ddc)), sigma_(sigma), interest_rate_(interest_rate), option_(std::tolower(option)), h_(h)
    
    {
        Look_Back_Validator::validate(S0_, sigma_, interest_rate_, option_, ttm_, h_);
    }

    
    /** @brief Monte Carlo price estimate (discounted). */
    double price(double S, double sigma, double interest_rate, double maturity, unsigned int N = 5000000) const;
    
    /** @brief Delta via central finite difference in spot. */
    double delta(double S) const;

    /** @brief Theta via central finite difference in maturity (uses a 7-day bump). */
    double theta() const;

    /** @brief Rho via finite difference in interest rate (may switch to forward scheme). */
    double rho() const;

    /** @brief Vega via finite difference in volatility (scaled by 0.01). */
    double vega() const;

    /** @brief Gamma via second central finite difference in spot. */
    double gamma() const;

    /** @brief Generates (S, price(S)) points for plotting. */
    std::array<vect,2> graphic_price(double dx) const;

    /** @brief Generates (S, delta(S)) points for plotting. */
    std::array<vect,2> graphic_delta(double dx) const;

};

/** @} */ // endgroup LB_Core



#endif

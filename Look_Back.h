/**
 * @file Look_Back.h
 * @brief Core C++ lookback option pricer interface.
 *
 * @author
 * Vincenzo Martino Pio Arena
 * Marco De Luca
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

/**
 * @section References
 *
 * The Monte Carlo simulation of the running maximum used in this library
 * follows the methodology described in:
 *
 *   Stéphane Crépey,
 *   "Financial Modeling: A Backward Stochastic Differential Equations Perspective",
 *   Springer, 2013.
 *
 * - Section 6.9 Monte Carlo Methods for Exotic Options
 * - Subsection 6.9.1 "Lookback Options"
 * - Paragraph 6.9.1.1 Black–Scholes Case
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
 * @brief Monte Carlo pricer for lookback options.
 *
 * @details
 * This class implements a Monte Carlo pricing algorithm for lookback options,
 * where the payoff depends on the running maximum of the underlying process.
 *
 * The pricing methodology follows the approach described in
 * Stéphane Crépey (2013), Section 6.9, and is based on an exact simulation of the
 * joint distribution of the discretized process and its running maximum.
 *
 * Let \f$X_t\f$ denote the log-price process and
 * \f$M_t = \sup_{0 \le s \le t} X_s\f$ its running maximum.
 * Given an Euler discretization \f$\hat{X}\f$, the conditional law of the
 * maximum on each interval is sampled using an independent uniform random
 * variable, as stated in Paragraph 6.9.1.1 Black–Scholes Case.
 *
 * This approach significantly reduces the discretization bias typically
 * encountered when approximating path-dependent extrema.
 *
 * @note
 * The numerical scheme implemented here is valid under the assumptions
 * described in the reference (diffusion dynamics with continuous paths).
 *
 * @warning
 * The Monte Carlo cost can be high, as the simulation of the running maximum
 * is performed at each time step and for each path.
 */

class look_back
{
    
private:
    mutable std::mt19937_64 gen_;
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
     * @param interest_rate Risk-free rate (non-negative in this validator).
     * @param option Option type ('c' call, 'p' put).
     * @param h Finite-difference step for Greeks.
     * @param ddc Day-count convention used to compute time to maturity.
     *
     * @throws Invalid_Parameters if validation fails.
     */
    explicit look_back(double S0, Date value_date, Date maturity_date, double sigma, double interest_rate, char option, double h, DayCountConv ddc  = DayCountConv::ACT_ACT_ISDA)
    
    :  S0_(S0), value_date_(value_date), maturity_date_(maturity_date), ttm_(yearFraction(value_date, maturity_date, ddc)), sigma_(sigma), interest_rate_(interest_rate), option_(std::tolower(option)), h_(h)
    
    {
        Look_Back_Validator::validate(S0_, sigma_, interest_rate_, option_, ttm_, h_);
    }

    
    /**
     * @brief Prices a lookback option using Monte Carlo simulation.
     *
     * @details
     * The option price is computed as the discounted expectation of the payoff
     * \f$\phi(X_T, M_T)\f$, where \f$M_T = \sup_{0 \le t \le T} X_t\f$.
     *
     * The simulation of the pair \f$(X_T, M_T)\f$ follows the algorithm described in
     * Stéphane Crépey (2013), Section 6.9, using:
     * - an Euler discretization for the underlying process, and
     * - an exact conditional sampling of the running maximum on each time interval
     *   via an auxiliary uniform random variable.
     *
     * This avoids the bias that would arise from approximating the maximum by
     * discrete-time sampling alone.
     *
     * @param S Spot price at which the option is priced.
     * @param sigma Volatility parameter.
     * @param interest_rate Risk-free interest rate.
     * @param maturity Time to maturity (in years).
     * @param N Number of Monte Carlo paths.
     *
     * @return Discounted Monte Carlo estimate of the option price.
     *
     * @see Stéphane Crépey, "Financial Modeling: A Backward Stochastic Differential Equations Perspective", Section 6.9.
     *
     * @note
     * This implementation is a direct numerical realization of a published
     * Monte Carlo method and does not claim originality with respect to the
     * underlying mathematical results.
     */

    double price(double S, double sigma, double interest_rate, double maturity, unsigned int N = 5000000) const;
    
    /** @brief Delta via central finite difference in spot. */
    double delta(double S) const;

    /** @brief Theta via central finite difference in maturity (uses a 3-day bump). */
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

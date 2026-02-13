/**
 * @file Look_Back.cpp
 * @brief Implementation of Monte Carlo pricing and finite-difference Greeks for lookback options.
 *
 * @details
 * The pricing method uses OpenMP to parallelize Monte Carlo draws. Each thread uses
 * its own RNG instance seeded deterministically from the thread id to prevent data races.
 *
 * Numerical notes:
 * - Antithetic variates are used (plus/minus Z).
 * - Uniform draws are clamped to avoid log(0).
 *
 * @warning
 * Some Greeks infer the Monte Carlo sample size as N = 1/h^4, which can become huge.
 */

#include "Look_Back.h"
#include <algorithm>
#include <random>
#include <omp.h>

#include "Date_Dealing.h"

double look_back::price(double S, double sigma, double interest_rate, double ttm, unsigned int N) const
{
    double payoff_sum=0;
    
    double log_simulation_plus=0;
    double log_simulation_minus=0;
    double rad=std::sqrt(ttm);
    double mu=(interest_rate-0.5*sigma*sigma)*ttm;
    double logs=std::log(S);
    
    #pragma omp parallel reduction(+:payoff_sum)
    {
        std::mt19937_64 gen(0x9e3779b97f4a7c15ULL ^ (uint64_t)omp_get_thread_num());
        std::normal_distribution<double> gaussian(0.0, 1.0);
        std::uniform_real_distribution<double> uniform(0.0, 1.0);

        const double eps = 1e-15;

        if (option_ == 'c')
        {
            #pragma omp for
            for (int i = 0; i < (int)N; ++i)
            {
                const double Z = gaussian(gen);

                // local variables: no data race
                const double log_simulation_plus  = logs + mu - sigma * rad * Z;
                const double log_simulation_minus = logs + mu + sigma * rad * Z;

                double U1 = uniform(gen);
                U1 = std::min(1.0 - eps, std::max(eps, U1));
                double U2 = uniform(gen);
                U2 = std::min(1.0 - eps, std::max(eps, U2));

                const double d1 = log_simulation_plus  - logs;
                const double d2 = log_simulation_minus - logs;

                double rad1 = d1*d1 - 2.0 * sigma*sigma * ttm * std::log(1-U1);
                double rad2 = d2*d2 - 2.0 * sigma*sigma * ttm * std::log(1-U2);
                
                rad1 = std::max(0.0, rad1);
                rad2 = std::max(0.0, rad2);


                const double min_plus  = std::exp(0.5*(logs + log_simulation_plus ) - 0.5*std::sqrt(rad1));
                const double min_minus = std::exp(0.5*(logs + log_simulation_minus) - 0.5*std::sqrt(rad2));

                payoff_sum += (std::exp(log_simulation_plus)  - min_plus)
                            + (std::exp(log_simulation_minus) - min_minus);
            }
        }
        else
        {
            #pragma omp for
            for (int i = 0; i < (int)N; ++i)
            {
                const double Z = gaussian(gen);

                // local variables: no data race
                const double log_simulation_plus  = logs + mu - sigma * rad * Z;
                const double log_simulation_minus = logs + mu + sigma * rad * Z;

                double U1 = uniform(gen);
                U1 = std::min(1.0 - eps, std::max(eps, U1));
                double U2 = uniform(gen);
                U2 = std::min(1.0 - eps, std::max(eps, U2));

                const double d1 = log_simulation_plus  - logs;
                const double d2 = log_simulation_minus - logs;

                double rad1 = d1*d1 - 2.0 * sigma*sigma * ttm * std::log(1.0 - U1);
                double rad2 = d2*d2 - 2.0 * sigma*sigma * ttm * std::log(1.0 - U2);
                
                rad1 = std::max(0.0, rad1);
                rad2 = std::max(0.0, rad2);


                const double max_plus  = std::exp(0.5*(logs + log_simulation_plus ) + 0.5*std::sqrt(rad1));
                const double max_minus = std::exp(0.5*(logs + log_simulation_minus) + 0.5*std::sqrt(rad2));

                payoff_sum += (-std::exp(log_simulation_plus)  + max_plus)
                            + (-std::exp(log_simulation_minus) + max_minus);
            }
        }
    }

    double payoff=payoff_sum/(2*N);
    return std::exp(-ttm*interest_rate)*payoff;
}




double look_back::delta(double S) const
{
    double h=2*h_;
    unsigned int N =1/(std::pow(h, 4));
    return (price(S0_ + h, sigma_, interest_rate_,ttm_, N) - price(S0_ - h, sigma_, interest_rate_, ttm_, N)) / (2.0 * h);
}

double look_back::vega() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    unsigned int N =1/(std::pow(h_, 4));
    return 0.01*(price(S0_, sigma_+ h_, interest_rate_, ttm_, N) - price(S0_, sigma_- h_, interest_rate_ , ttm_, N)) / (2.0 * h_);
}

double look_back::rho() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    unsigned int N =1/(std::pow(h_, 4));
    
    
    // to avoid using centered scheme if h is bigger than the interest rate
    if (h_<=interest_rate_)
        return 0.01*(price(S0_, sigma_, interest_rate_+ h_, ttm_, N) - price(S0_, sigma_, interest_rate_- h_, ttm_, N)) / (2.0 * h_);
    
    else
    {
        return 0.01*(price(S0_, sigma_, interest_rate_+ h_, ttm_, N) - price(S0_, sigma_, interest_rate_, ttm_, N)) / h_;
    }
    
    
}

double look_back::theta() const
{
    double day=(3.0/365.0);
    unsigned int N =1/(std::pow(day, 4));
    if (ttm_<=4)
    {
        day=(0.5/365.0);
        unsigned int N =1/(std::pow(day, 3));
    }
    return (price(S0_, sigma_, interest_rate_, ttm_-day,N) - price(S0_, sigma_, interest_rate_, ttm_+day, N))/ (2.0*day);
}



double look_back::gamma() const
{
    double h=2*h_;
    unsigned int N =1/(std::pow(h, 4));
    return (price(S0_ + h, sigma_, interest_rate_,ttm_, N) + price(S0_ - h, sigma_, interest_rate_, ttm_, N)-2.0*price(S0_, sigma_, interest_rate_,ttm_, N)) / (h*h);
}


// dx is 1/n_points on the x axis
std::array<vect,2> look_back::graphic_price(double dx) const
{
    
    std::array<vect,2> graph;
    for(double s=0; s<2*S0_; s+=dx*S0_)
    {
        graph[0].push_back(s);
        graph[1].push_back(price(s, sigma_, interest_rate_, ttm_));
    }
    return graph;
}




std::array<vect,2> look_back::graphic_delta(double dx) const

{
    std::array<vect,2> graph;
    for(double s=0; s<2*S0_; s+=dx*S0_)
    {
        graph[0].push_back(s);
        graph[1].push_back(delta(s));
    }
    return graph;
    
}

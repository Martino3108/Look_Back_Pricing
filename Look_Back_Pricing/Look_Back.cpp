//
//  Look_Back.cpp
//  Projet_informatique
//
//  Created by Martino Arena on 27/12/25.
//

#include "Look_Back.h"
#include <algorithm>
#include <random>

double look_back::price(double S, double sigma, double interest_rate, double maturity, unsigned int N) const
{

    // SI POTREBBE FARE UNA CLASSE PER CHECKARE LE EXCEPTION
    double payoff_sum=0;
    
    // CAPIRE QUALE GENERATORE USARE
    std::mt19937_64 gen(41);
    std::normal_distribution<double> gaussian(0,1);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    double simulation_plus=0;
    double simulation_minus=0;
    for(int i=0; i<N; ++i)
    {
        
            double Z=gaussian(gen);
            simulation_plus=S*std::exp((interest_rate-0.5*sigma*sigma)*maturity-sigma*std::sqrt(maturity)*Z);
            simulation_minus=S*std::exp((interest_rate-0.5*sigma*sigma)*maturity-sigma*std::sqrt(maturity)*(-Z));
    
        
        
        if (option_== "CALL")
        {
            double U1=uniform(gen);
            double U2=uniform(gen);
            double min_plus= std::exp(0.5*(std::log(S) + std::log(simulation_plus)) - 0.5*std::sqrt( (std::log(simulation_plus) - std::log(S))*(std::log(simulation_plus) - std::log(S)) - 2.0*sigma*sigma*maturity*std::log(1.0 - U1) ) );

            double min_minus= std::exp(0.5*(std::log(S) + std::log(simulation_minus)) - 0.5*std::sqrt( (std::log(simulation_minus) - std::log(S))*(std::log(simulation_minus) - std::log(S)) - 2.0*sigma*sigma*maturity*std::log(1.0 - U2) ) );
            
            payoff_sum+=(simulation_plus-min_plus)+(simulation_minus-min_minus);
        }
        
        if (option_== "PUT")
        {
            double U1=uniform(gen);
            double U2=uniform(gen);
            double max_plus= std::exp(0.5*(std::log(S) + std::log(simulation_plus)) + 0.5*std::sqrt( (std::log(simulation_plus) - std::log(S))*(std::log(simulation_plus) - std::log(S)) - 2.0*sigma*sigma*maturity*std::log(1.0 - U1) ) );

            double max_minus= std::exp(0.5*(std::log(S) + std::log(simulation_minus)) + 0.5*std::sqrt( (std::log(simulation_minus) - std::log(S))*(std::log(simulation_minus) - std::log(S)) - 2.0*sigma*sigma*maturity*std::log(1.0 - U2) ) );
            
            payoff_sum+=(-simulation_plus+max_plus)+(-simulation_minus+max_minus);
        }
        
        
    }
    
    double payoff=payoff_sum/(2*N);
    return std::exp(-maturity_*interest_rate)*payoff;
}

double look_back::delta(double S) const
{
    unsigned int N =4/(std::pow(h_, 4));
    return (price(S0_ + h_, sigma_, interest_rate_,maturity_, N) - price(S0_ - h_, sigma_, interest_rate_, maturity_, N)) / (2.0 * h_);
}

double look_back::vega() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    unsigned int N =4/(std::pow(h_, 4));
    return 0.01*(price(S0_, sigma_+ h_, interest_rate_, maturity_, N) - price(S0_, sigma_- h_, interest_rate_ , maturity_, N)) / (2.0 * h_);
}

double look_back::rho() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    unsigned int N =4/(std::pow(h_, 4));
    return 0.01*(price(S0_, sigma_, interest_rate_+ h_, maturity_, N) - price(S0_, sigma_, interest_rate_- h_, maturity_, N)) / (2.0 * h_);
}

double look_back::theta() const
{
    //daily theta
    double day=(1.0/252.0);
    unsigned int N =4/(std::pow(h_, 3));
    return (price(S0_, sigma_, interest_rate_, maturity_-day, 4/(std::pow(day, 3))) - price(S0_, sigma_, interest_rate_, maturity_, N));
}

double look_back::gamma() const
{
    unsigned int N =4/(std::pow(h_, 4));
    return (price(S0_ + h_, sigma_, interest_rate_,maturity_, N) + price(S0_ - h_, sigma_, interest_rate_, maturity_, N)-2*price(S0_, sigma_, interest_rate_,maturity_, N)) / (h_*h_);
}


// dx is 1/n_points on the x axis
std::array<vect,2> look_back::graphic_price(double dx) const
{
    
    std::array<vect,2> graph;
    for(double s=0; s<2*S0_; s+=dx*S0_)
    {
        graph[0].push_back(s);
        graph[1].push_back(price(s, sigma_, interest_rate_, maturity_, h_));
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

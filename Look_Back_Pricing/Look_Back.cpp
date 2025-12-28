//
//  Look_Back.cpp
//  Projet_informatique
//
//  Created by Martino Arena on 27/12/25.
//

#include "Look_Back.h"
#include <algorithm>
#include <random>

double look_back::price(double S, double sigma, double interest_rate, double maturity) const
{

    // SI POTREBBE FARE UNA CLASSE PER CHECKARE LE EXCEPTION
    int n_steps=maturity*252.0;
    double payoff_sum=0;
    std::mt19937_64 gen(42);
    std::normal_distribution<double> gaussian(0,1);
    for(int i=0; i<N_; ++i)
    {
        vect simulation_plus={S};
        vect simulation_minus={S};
        
        for(int j=0; j<n_steps; ++j)
        {
            double Z=gaussian(gen);
            simulation_plus.push_back(simulation_plus.back()*std::exp(interest_rate*(1.0/252.0)-0.5*sigma*sigma*(1.0/252.0)+sigma*std::sqrt((1.0/252.0))*Z));
            simulation_minus.push_back(simulation_minus.back()*std::exp(interest_rate*(1.0/252.0)-0.5*sigma*sigma*(1.0/252.0)-sigma*std::sqrt((1.0/252.0))*Z));
    
        }
        
        if (option_== "CALL")
        {
            //call payoff
            auto it_min_plus=std::min_element(simulation_plus.begin(), simulation_plus.end());
            auto it_min_minus=std::min_element(simulation_minus.begin(), simulation_minus.end());
            payoff_sum+=(simulation_plus.back()-*it_min_plus)+(simulation_minus.back()-*it_min_minus);
        }
        
        if (option_== "PUT")
        {
            //put payoff
            auto it_max_plus=std::max_element(simulation_plus.begin(), simulation_plus.end());
            auto it_max_minus=std::max_element(simulation_minus.begin(), simulation_minus.end());
            payoff_sum+=(-simulation_plus.back()+*it_max_plus)+(-simulation_minus.back()+*it_max_minus);
        }
        
        
    }
    
    double payoff=payoff_sum/(2*N_);
    return std::exp(-maturity_*interest_rate)*payoff;
}

double look_back::delta(double S) const
{
    return (price(S0_ + h_, sigma_, interest_rate_,maturity_) - price(S0_ - h_, sigma_, interest_rate_, maturity_)) / (2.0 * h_);
}

double look_back::vega() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    return 0.01*(price(S0_, sigma_+ h_, interest_rate_, maturity_) - price(S0_, sigma_- h_, interest_rate_ , maturity_)) / (2.0 * h_);
}

double look_back::rho() const
{
    //multiplied by 0.01 in order to pass from percentage to numeric value
    return 0.01*(price(S0_, sigma_, interest_rate_+ h_, maturity_) - price(S0_, sigma_, interest_rate_- h_ , maturity_)) / (2.0 * h_);
}

double look_back::theta() const
{
    //annual theta
    return 252*(price(S0_, sigma_, interest_rate_, maturity_-(1.0/252.0)) - price(S0_, sigma_, interest_rate_, maturity_));
}

double look_back::gamma() const
{
    return (price(S0_ + h_, sigma_, interest_rate_,maturity_) + price(S0_ - h_, sigma_, interest_rate_, maturity_)-2*price(S0_, sigma_, interest_rate_,maturity_)) / (h_*h_);
}

std::array<vect,2> look_back::graphic_price(double dx) const
{
    
    std::array<vect,2> graph;
    for(double s=0; s<2*S0_; s+=dx*S0_)
    {
        graph[0].push_back(s);
        graph[1].push_back(price(s, sigma_, interest_rate_, maturity_));
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

//
//  Look_Back.cpp
//  Projet_informatique
//
//  Created by Martino Arena on 27/12/25.
//

#include "Look_Back.h"
#include <algorithm>
#include <random>

double look_back::price(double S) const
{
    double payoff=0;
    std::knuth_b gen(42);
    std::normal_distribution<double> gaussian(0,1);
    for(int i=0; i<N_; ++i)
    {
        vect simulation={S};
        
        for(int j=0; j<maturity_; ++j)
        {
            double Z=gaussian(gen);
            simulation.push_back(simulation.back()*std::exp(interest_rate_-0.5*std::pow(sigma_,2)*(1/252))+sigma_*std::sqrt((1/252))*Z);
        }
        auto it_max=std::min_element(simulation.begin(), simulation.end());
        payoff+= *it_max-simulation.back();
    }
    
    return std::exp(-maturity_*interest_rate_)*(payoff/N_);
    
    
}




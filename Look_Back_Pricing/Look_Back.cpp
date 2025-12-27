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
    double payoff_sum=0;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::normal_distribution<double> gaussian(0,1);
    for(int i=0; i<N_; ++i)
    {
        vect simulation={S};
        
        for(int j=0; j<maturity_; ++j)
        {
            double Z=gaussian(gen);
            simulation.push_back(simulation.back()*std::exp(interest_rate_*(1.0/252.0)-0.5*std::pow(sigma_,2)*(1.0/252.0)+sigma_*std::sqrt((1.0/252.0))*Z));
        }
        auto it_min=std::min_element(simulation.begin(), simulation.end());
        payoff_sum+=simulation.back()-*it_min;
    }
    double payoff=payoff_sum/N_;
    std::cout<<payoff<< " ";
    return std::exp(-maturity_*interest_rate_*(1.0/252.0)*payoff);
    
    
}




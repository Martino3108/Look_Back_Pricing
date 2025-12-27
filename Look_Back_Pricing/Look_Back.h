//
//  Untitled.h
//  Projet_informatique
//
//  Created by Martino Arena on 27/12/25.
//

#ifndef LOOK_BACK_H
#define LOOK_BACK_H
#include <iostream>
#include <vector>
#include <string>


typedef std::vector<double> vect;


class look_back
{
    
private:
    double S0_;
    unsigned int maturity_;
    double sigma_;
    double interest_rate_;
    std::string option_;
    unsigned int N_;
    double h_;
    
public:
    
    explicit look_back(double S0, unsigned int maturity, double sigma, double interest_rate, std::string option, unsigned int N, double h): S0_(S0), maturity_(maturity), sigma_(sigma), interest_rate_(interest_rate), option_(option), N_(N), h_(h) {}
    
    double price(double S) const;
    double delta(double S) const;
    double theta(double S) const;
    double rho(double S) const;
    double vega(double S) const;
    double gamma(double S) const;
    std::vector<vect> graphic_price() const;
    std::vector<vect> graphic_delta() const;

    
};



#endif

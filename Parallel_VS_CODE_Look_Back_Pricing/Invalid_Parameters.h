//
//  invalid_parameters.h
//  Projet_informatique
//
//  Created by Martino Arena on 24/01/26.
//

#ifndef Invalid_Parameters_h
#define Invalid_Parameters_h

#include <exception>
#include <stdexcept>
#include <string>


class Invalid_Parameters : public std::invalid_argument
{
public:
    explicit Invalid_Parameters(const std::string& msg)
        : std::invalid_argument("Error financial parameter : " + msg) {}
    
};

class Look_Back_Validator
{
public:
    static void validate(double S0, double sigma, double interest_rate, char option, double ttm, double h)
    {
        
        if (S0 <= 0)
            throw Invalid_Parameters("S0 must be positive.");
        
        if (sigma <= 0)
            throw Invalid_Parameters("Volatility must be positive.");
            
        if (option != 'c' && option != 'p')
            throw Invalid_Parameters("Option type can only be 'c' (Call) o 'p' (Put).");
            
        if (interest_rate < 0)
            throw Invalid_Parameters("Our model allows only for positive interest rates.");
        if(ttm < 0)
            throw Invalid_Parameters("end date < start date in yearFraction");
        if(h<=0)
            throw Invalid_Parameters("h must be positive");
        
    }
};


#endif

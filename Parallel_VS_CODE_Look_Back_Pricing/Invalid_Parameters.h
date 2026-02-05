/**
 * @file Invalid_Parameters.h
 * @brief Validation helpers and domain-specific exceptions for financial inputs.
 *
 * @details
 * Provides:
 * - `Invalid_Parameters`: exception thrown when financial or numerical inputs are invalid.
 * - `Look_Back_Validator`: centralized validation logic used by the `look_back` constructor.
 *
 * @note
 * The ABI bridge catches these exceptions and exposes the message via `LB_GetLastErrorA()`.
 */

#ifndef Invalid_Parameters_h
#define Invalid_Parameters_h

#include <exception>
#include <stdexcept>
#include <string>

/**
 * @class Invalid_Parameters
 * @brief Exception type for invalid financial/numerical parameters.
 */
class Invalid_Parameters : public std::invalid_argument
{
public:
    explicit Invalid_Parameters(const std::string& msg)
        : std::invalid_argument("Error financial parameter: " + msg) {}
    
};

/**
 * @class Look_Back_Validator
 * @brief Static validation rules for lookback pricer construction.
 *
 * @details
 * Enforces constraints such as:
 * - S0 > 0
 * - sigma > 0
 * - interest_rate >= 0
 * - option in {'c','p'}
 * - ttm >= 0
 * - h >= 0.005
 * - h < 1
 * @throws Invalid_Parameters on first violated rule.
 */
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
            throw Invalid_Parameters("Option type can only be 'c' (Call) or 'p' (Put).");
            
        if (interest_rate < 0)
            throw Invalid_Parameters("Our model allows only positive interest rates.");
        if(ttm < 0)
            throw Invalid_Parameters("maturity date < value date in yearFraction.");
        if(h<0.005)
            throw Invalid_Parameters("h must be positive and greater than 0.005.");
        if(h>=1)
            throw Invalid_Parameters("h bigger than one => M smaller than one.");
        
    }
};


#endif

//
//  main.cpp
//  LookBackPricing
//
//  Created by Vincenzo Martino Pio Arena on 28/10/25.
//  Co-workers: Vincenzo Martino Pio Arena and Marco De Luca
//

#include <iostream>
#include <chrono>
#include "Look_Back.h"
#include "Date_Dealing.h"

int main(int argc, const char * argv[]) {
    
    
    try
    {
        
    using clock = std::chrono::high_resolution_clock;

    auto t0 = clock::now();
    
    Date value_date("01-01-2024"), maturity_date("01-05-2026"); // dd-mm-yyyy
    double ttm = yearFraction(value_date, maturity_date, DayCountConv::ACT_365F);
    std::cout<<"TTM with ACT_365F: "<<yearFraction(value_date, maturity_date, DayCountConv::ACT_365F)<<"\n";
    std::cout<<"TTM with ACT_ACT_ISDA: "<<yearFraction(value_date, maturity_date, DayCountConv::ACT_ACT_ISDA)<<"\n";

    
    look_back l(100, value_date, maturity_date, 0.2, 0.05, 'C', 0.01);
    
    std::cout<<"Price: "<<l.price(100, 0.2, 0.05, ttm)<<std::endl;
    std::cout<<"Delta: "<<l.delta(100)<<std::endl;
    std::cout<<"Rho: "<<l.rho()<<std::endl;
    std::cout<<"Vega: "<<l.vega()<<std::endl;
    std::cout<<"Theta: "<<l.theta()<<std::endl;
    std::cout<<"Gamma: "<<l.gamma()<<std::endl;
    std::array<vect, 2> g1=l.graphic_price(0.1);
    std::array<vect, 2> g2=l.graphic_delta(0.1);
    
    
    auto t1 = clock::now();

    std::chrono::duration<double> dt = t1 - t0; // secondi (double)
    std::cout << "Elapsed: " << dt.count() << " s\n";
    
    //std::cout<<"TTM: "<<ttm<<"\n";
    }
    
    catch (const Invalid_Parameters & e)
    
    {
        std::cerr << e.what() << std::endl;
    }
    
  
}

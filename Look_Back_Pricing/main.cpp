//
//  main.cpp
//  LookBackPricing
//
//  Created by Vincenzo Martino Pio Arena on 28/10/25.
//  Co-workers: Vincenzo Martino Pio Arena and Marco De Luca
//

#include <iostream>

#include "Look_Back.h"
#include "Date_Dealing.h"

int main(int argc, const char * argv[]) {
    
    Date value_date("01-1-2024"), maturity_date("01-1-2025");
    
    look_back l(100, 1, 0.2, 0.05, "CALL", 0.01, value_date, maturity_date, DayCountConv::ACT_360);
    
   
    std::cout<<"Price: "<<l.price(100,0.2,0.05,1, value_date, maturity_date, DayCountConv::ACT_360)<<std::endl;
    //std::cout<<"Delta: "<<l.delta(100)<<std::endl;
    //std::cout<<"Rho: "<<l.rho()<<std::endl;
    //std::cout<<"Vega: "<<l.vega()<<std::endl;
    //std::cout<<"Theta: "<<l.theta()<<std::endl;
    //std::cout<<"Gamma: "<<l.gamma()<<std::endl;
    //std::array<vect, 2> g1=l.graphic_price(0.1);
    //std::array<vect, 2> g2=l.graphic_delta(0.1);
    
}

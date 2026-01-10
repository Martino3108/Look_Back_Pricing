//
//  main.cpp
//  LookBackPricing
//
//  Created by Vincenzo Martino Pio Arena on 28/10/25.
//  Co-workers: Vincenzo Martino Pio Arena and Marco De Luca
//

#include <iostream>
#include "Look_Back.h"

int main(int argc, const char * argv[]) {
    look_back l(100, 1, 0.2, 0.05, "CALL", 0.01);
    
   
   //std::cout<<l.price(100,0.2,0.05,1)<<std::endl;
    //std::cout<<l.delta(100)<<std::endl;
    //std::cout<<l.rho()<<std::endl;
    //std::cout<<l.vega()<<std::endl;
    std::cout<<l.theta()<<std::endl;
    //std::cout<<l.gamma()<<std::endl;
    //std::array<vect, 2> g1=l.graphic_price(0.1);
    //std::array<vect, 2> g2=l.graphic_delta(0.1);
    
}

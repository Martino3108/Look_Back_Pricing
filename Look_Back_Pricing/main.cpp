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
    look_back l(10, 1, 0.1, 0.01, "CALL", 100000, 0.01);
    
   
    std::cout<<l.price(100,0.1,0.01,1)<<std::endl;
    std::cout<<l.delta(100)<<std::endl;
    std::cout<<l.rho()<<std::endl;
    std::cout<<l.vega()<<std::endl;
    std::cout<<l.theta()<<std::endl;
    std::cout<<l.gamma()<<std::endl;
    
    
}

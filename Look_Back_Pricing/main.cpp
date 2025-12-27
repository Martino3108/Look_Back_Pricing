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
    look_back l(100, 252, 0.1, 0.01, "CALL", 10000, 0.01);
    
    
    std::cout<<l.price(100);
}

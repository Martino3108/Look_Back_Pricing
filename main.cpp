/***********************************************************************
 *  LookBackPricing – Main Application
 *
 *  This file contains the program entry point responsible for:
 *      - Date handling and day count calculations
 *      - Lookback option instantiation
 *      - Monte Carlo pricing
 *      - Greeks computation
 *      - Performance timing
 *
 *  Authors: Vincenzo Martino Pio Arena, Marco De Luca
 ***********************************************************************/


#include <array>
#include <chrono>
#include <iostream>

#include "Date_Dealing.h"
#include "Look_Back.h"

int main(int /*argc*/, const char* /*argv*/[]) {
    using clock = std::chrono::high_resolution_clock;

    try {
        const auto t0 = clock::now();

        const Date value_date("01-01-2022"), maturity_date("01-01-2030"); // dd-mm-yyyy

        const double ttm_act365f   = yearFraction(value_date, maturity_date, DayCountConv::ACT_365F);
        const double ttm_actactisd = yearFraction(value_date, maturity_date, DayCountConv::ACT_ACT_ISDA);

        //std::cout << "TTM with ACT_365F: " << ttm_act365f << "\n";
        //std::cout << "TTM with ACT_ACT_ISDA: " << ttm_actactisd << "\n";

        constexpr double S0         = 100.0;
        constexpr double sigma      = 0.2;
        constexpr double r          = 0.05;
        constexpr char   optType    = 'C'; // 'C' for call, 'P' for put
        constexpr double hBump      = 0.01;
        constexpr double graph_step = 0.1;
        constexpr unsigned int N    = 500000000u;

        look_back l(S0, value_date, maturity_date, sigma, r, optType, hBump, DayCountConv::ACT_ACT_ISDA);

        // Pricing & Greeks
        std::cout << "Price: " << l.price(S0, sigma, r, ttm_actactisd, N) << "\n";
        std::cout << "Delta: " << l.delta(S0) << "\n";
        std::cout << "Rho: "   << l.rho()     << "\n";
        std::cout << "Vega: "  << l.vega()    << "\n";
        std::cout << "Theta: " << l.theta()   << "\n";
        std::cout << "Gamma: " << l.gamma()   << "\n";

        [[maybe_unused]] const std::array<vect, 2> g1 = l.graphic_price(graph_step);
        [[maybe_unused]] const std::array<vect, 2> g2 = l.graphic_delta(graph_step);

        const auto t1 = clock::now();
        const std::chrono::duration<double> dt = t1 - t0;

        std::cout << "Elapsed: " << dt.count() << " s\n";
        return 0;
    }
    catch (const Invalid_Parameters& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled std::exception: " << e.what() << "\n";
        return 2;
    }
    catch (...) {
        std::cerr << "Unhandled unknown exception.\n";
        return 3;
    }
}

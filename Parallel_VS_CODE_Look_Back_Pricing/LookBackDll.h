#pragma once
#include <cstddef>

#ifdef _WIN32
  #define LB_CALL __stdcall
  #ifdef LOOKBACKDLL_EXPORTS
    #define LB_API extern "C" __declspec(dllexport)
  #else
    #define LB_API extern "C" __declspec(dllimport)
  #endif
#else
  #define LB_CALL
  #if defined(__APPLE__)
    #define LB_API extern "C" __attribute__((visibility("default")))
  #else
    #define LB_API extern "C"
  #endif
#endif

enum LB_DayCountConv : int {
    LB_ACT_360        = 0,
    LB_ACT_365F       = 1,
    LB_THIRTY_360_US  = 2,
    LB_THIRTY_360_EU  = 3,
    LB_ACT_ACT_ISDA   = 4
};

typedef void* LB_Handle;

// ---- Object lifetime ----
// Mac-friendly
LB_API LB_Handle LB_CALL LB_CreateA(
    double S0,
    const char* value_date_dd_mm_yyyy,
    const char* maturity_date_dd_mm_yyyy,
    double sigma,
    double interest_rate,
    int option_ascii,
    double h,
    int day_count_conv
);

#ifdef _WIN32
// Windows-only Unicode
LB_API LB_Handle LB_CALL LB_CreateW(
    double S0,
    const wchar_t* value_date_dd_mm_yyyy,
    const wchar_t* maturity_date_dd_mm_yyyy,
    double sigma,
    double interest_rate,
    int option_ascii,
    double h,
    int day_count_conv
);
#endif

LB_API void LB_CALL LB_Destroy(LB_Handle h);

// ---- Pricing / Greeks ----
LB_API double LB_CALL LB_Price(LB_Handle h, double S, double sigma, double interest_rate, double maturity, unsigned int N);
LB_API double LB_CALL LB_Delta(LB_Handle h, double S);
LB_API double LB_CALL LB_Theta(LB_Handle h);
LB_API double LB_CALL LB_Rho(LB_Handle h);
LB_API double LB_CALL LB_Vega(LB_Handle h);
LB_API double LB_CALL LB_Gamma(LB_Handle h);

// ---- Graphs ----
LB_API int LB_CALL LB_GraphicPrice(LB_Handle h, double dx, double* x_out, double* y_out, int max_len);
LB_API int LB_CALL LB_GraphicDelta(LB_Handle h, double dx, double* x_out, double* y_out, int max_len);

// Date helper
LB_API double LB_CALL LB_GetYearFraction(const char* start_date, const char* end_date, int day_count_conv);

// ---- Error handling ----
LB_API int LB_CALL LB_GetLastErrorA(char* buffer, int buffer_len);
#ifdef _WIN32
LB_API int LB_CALL LB_GetLastErrorW(wchar_t* buffer, int buffer_len);
#endif
LB_API void LB_CALL LB_ClearLastError();
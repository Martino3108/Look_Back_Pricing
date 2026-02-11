/**
 * @file LookBackDll.h
 * @brief C ABI wrapper for the LookBack pricer (Excel/VBA-friendly).
 *
 * @details
 * This header exposes a stable C interface to the underlying C++ implementation
 * of a lookback option pricer. The API uses an opaque handle (`LB_Handle`) to
 * manage object lifetime across language boundaries (e.g., VBA).
 *
 * Key design points:
 * - **No C++ exceptions cross the ABI boundary**: functions catch exceptions and
 *   store an error message in a thread-local buffer retrievable via
 *   LB_GetLastErrorA().
 * - **Platform calling conventions**: LB_CALL and LB_API handle Windows vs macOS/Linux.
 * - **Day count conventions**: the C enum maps to the C++ `DayCountConv`.
 *
 * Build note:
 * - Paste your exact dynamic library build command here once finalized (clang++ flags,
 *   OpenMP linkage, install_name_tool, etc.) so the generated docs are reproducible.
 */

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

/** @defgroup LB_Bridge Bridge (C ABI)
 *  @brief C ABI entry points used by Excel/VBA (and other foreign language callers).
 *  @{
 */

/**
 * @enum LB_DayCountConv
 * @brief Day-count conventions available at the ABI level.
 * @details These values are mapped internally to the C++ enum class `DayCountConv`.
 */
enum LB_DayCountConv : int {
    LB_ACT_360        = 0,
    LB_ACT_365F       = 1,
    LB_THIRTY_360_US  = 2,
    LB_THIRTY_360_EU  = 3,
    LB_ACT_ACT_ISDA   = 4
};

/**
 * @typedef LB_Handle
 * @brief Opaque handle to a C++ `look_back` instance.
 * @details The caller owns the handle and must release it with LB_Destroy().
 */
typedef void* LB_Handle;

/**
 * @brief Creates a LookBack pricer instance (ASCII dates, portable ABI).
 *
 * @details
 * Parses value/maturity dates formatted as **dd-mm-yyyy**, builds a C++ `look_back`
 * object, and returns it as an opaque handle.
 *
 * Error model:
 * - Returns `nullptr` on failure and sets the thread-local last error.
 * - Retrieve the error with LB_GetLastErrorA().
 *
 * @param S0 Spot price at valuation.
 * @param value_date_dd_mm_yyyy Valuation date string "dd-mm-yyyy".
 * @param maturity_date_dd_mm_yyyy Maturity date string "dd-mm-yyyy".
 * @param sigma Volatility (annualized, numeric).
 * @param interest_rate Risk-free rate (annualized, numeric).
 * @param option_ascii Option type as ASCII (e.g. 'c' for call or 'p' for put).
 * @param h Finite-difference step used by Greeks.
 * @param day_count_conv Day-count convention (LB_DayCountConv).
 * @return LB_Handle Opaque handle, or nullptr on error.
 */
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

/**
 * @brief Destroys a pricer instance created by LB_CreateA / LB_CreateW.
 * @param h Handle returned by LB_CreateA / LB_CreateW. Safe to pass nullptr.
 */
LB_API void LB_CALL LB_Destroy(LB_Handle h);

/**
 * @brief Prices the lookback option by Monte Carlo.
 * @details Returns 0.0 on error and sets the last error message.
 * @param h Valid handle.
 * @param S Spot at which to price.
 * @param sigma Volatility.
 * @param interest_rate Rate.
 * @param maturity Time-to-maturity (year fraction).
 * @param N Number of Monte Carlo samples.
 * @return Option price (discounted), or 0.0 on error.
 */
LB_API double LB_CALL LB_Price(LB_Handle h, double S, double sigma, double interest_rate, double maturity, unsigned int N);

// ---- Greeks ----
LB_API double LB_CALL LB_Delta(LB_Handle h, double S);

LB_API double LB_CALL LB_Theta(LB_Handle h);

LB_API double LB_CALL LB_Rho(LB_Handle h);

LB_API double LB_CALL LB_Vega(LB_Handle h);

LB_API double LB_CALL LB_Gamma(LB_Handle h);

// ---- Graphs ----
LB_API int LB_CALL LB_GraphicPrice(LB_Handle h, double dx, double* x_out, double* y_out, int max_len);
LB_API int LB_CALL LB_GraphicDelta(LB_Handle h, double dx, double* x_out, double* y_out, int max_len);

// Date function
LB_API double LB_CALL LB_GetYearFraction(const char* start_date, const char* end_date, int day_count_conv);

/**
 * @brief Retrieves the last error message (ASCII).
 * @details
 * If `buffer` is null or `buffer_len<=0`, returns the needed buffer size
 * (including the null terminator).
 * @param buffer Output buffer.
 * @param buffer_len Output buffer length.
 * @return Number of chars written including null terminator, or needed size.
 */
LB_API int LB_CALL LB_GetLastErrorA(char* buffer, int buffer_len);

#ifdef _WIN32
LB_API int LB_CALL LB_GetLastErrorW(wchar_t* buffer, int buffer_len);
#endif

/** @brief Clears the thread-local last error buffer. */
LB_API void LB_CALL LB_ClearLastError();


/** @} */ // endgroup LB_Bridge
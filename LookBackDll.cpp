/**
 * @file LookBackDll.cpp
 * @brief C ABI implementation for the LookBack pricer dynamic library.
 *
 * @details
 * This translation unit implements the exported C functions declared in
 * LookBackDll.h. It is responsible for:
 * - Translating primitive ABI inputs into C++ objects (`Date`, `look_back`).
 * - Enforcing ABI safety (null checks, handle checks).
 * - Catching exceptions and exposing error messages through a thread-local
 *   "last error" mechanism (`LB_GetLastErrorA`).
 *
 * Thread-safety:
 * - Error state is stored in a `thread_local` string, so different threads do not
 *   overwrite each other’s last error message.
 *
 * Important:
 * - The exported functions MUST NOT throw across the ABI boundary.
 */

#include "LookBackDll.h"

#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <cstring>   // memcpy

#include "Look_Back.h"
#include "Date_Dealing.h"
#include "Invalid_Parameters.h"

#ifdef _WIN32
  #include <windows.h>
#endif

/** @brief Thread-local buffer storing the last error message for the calling thread. */
static thread_local std::string g_lastErrorA;

static void set_error_from_exception(const char* where, const std::exception& e)
{
    g_lastErrorA = std::string(where) + ": " + (e.what() ? e.what() : "std::exception with null what()");
}


static void set_error_a(const std::string& s)
{
    g_lastErrorA = s;
}

static void clear_error()
{
    g_lastErrorA.clear();
}

static DayCountConv map_ddc(int ddc)
{
    switch (ddc)
    {
        case LB_ACT_360:       return DayCountConv::ACT_360;
        case LB_ACT_365F:      return DayCountConv::ACT_365F;
        case LB_THIRTY_360_US: return DayCountConv::THIRTY_360_US;
        case LB_THIRTY_360_EU: return DayCountConv::THIRTY_360_EU;
        case LB_ACT_ACT_ISDA:  return DayCountConv::ACT_ACT_ISDA;
        default:               return DayCountConv::ACT_365F;
    }
}

static look_back* as_ptr(LB_Handle h)
{
    return reinterpret_cast<look_back*>(h);
}

LB_API LB_Handle LB_CALL LB_CreateA(
    double S0,
    const char* value_date_dd_mm_yyyy,
    const char* maturity_date_dd_mm_yyyy,
    double sigma,
    double interest_rate,
    int option_ascii,
    double h,
    int day_count_conv
)
{
    clear_error();
    try
    {
        if (!value_date_dd_mm_yyyy || !maturity_date_dd_mm_yyyy)
        {
            set_error_a("Null date string in LB_CreateA");
            return nullptr;
        }

        const std::string v(value_date_dd_mm_yyyy);
        const std::string m(maturity_date_dd_mm_yyyy);

        Date vd(v);
        Date md(m);

        const char opt = static_cast<char>(option_ascii);
        const DayCountConv ddc = map_ddc(day_count_conv);

        look_back* obj = new look_back(S0, vd, md, sigma, interest_rate, opt, h, ddc);
        return reinterpret_cast<LB_Handle>(obj);
    }
    catch (const std::exception& e)
    {
        set_error_from_exception("LB_CreateA", e);
        return nullptr;
    }
    catch (...)
    {
        set_error_a("Unknown (non-std) exception in LB_CreateA");
        return nullptr;
    }
}

LB_API void LB_CALL LB_Destroy(LB_Handle h)
{
    clear_error();
    try
    {
        delete as_ptr(h);
    }
    catch (...)
    {
        set_error_a("Unknown error in LB_Destroy");
    }
}

LB_API double LB_CALL LB_Price(LB_Handle h, double S, double sigma, double interest_rate, double maturity, unsigned int N)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Price"); return 0.0; }
        return as_ptr(h)->price(S, sigma, interest_rate, maturity, N);
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Price", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Price"); return 0.0; }
}

LB_API double LB_CALL LB_Delta(LB_Handle h, double S)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Delta"); return 0.0; }
        return as_ptr(h)->delta(S);
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Delta", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Delta"); return 0.0; }
}

LB_API double LB_CALL LB_Theta(LB_Handle h)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Theta"); return 0.0; }
        return as_ptr(h)->theta();
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Theta", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Theta"); return 0.0; }
}

LB_API double LB_CALL LB_Rho(LB_Handle h)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Rho"); return 0.0; }
        return as_ptr(h)->rho();
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Rho", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Rho"); return 0.0; }
}

LB_API double LB_CALL LB_Vega(LB_Handle h)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Vega"); return 0.0; }
        return as_ptr(h)->vega();
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Vega", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Vega"); return 0.0; }
}

LB_API double LB_CALL LB_Gamma(LB_Handle h)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_Gamma"); return 0.0; }
        return as_ptr(h)->gamma();
    }
    catch (const std::exception& e) { set_error_from_exception("LB_Gamma", e); return 0.0; }
    catch (...) { set_error_a("Unknown error in LB_Gamma"); return 0.0; }
}

LB_API int LB_CALL LB_GraphicPrice(LB_Handle h, double dx, double* x_out, double* y_out, int max_len)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_GraphicPrice"); return 0; }

        auto graph = as_ptr(h)->graphic_price(dx);
        const int n = static_cast<int>(graph[0].size());

        if (!x_out || !y_out || max_len <= 0)
            return n;

        const int k = std::min(n, max_len);
        for (int i = 0; i < k; ++i)
        {
            x_out[i] = graph[0][static_cast<size_t>(i)];
            y_out[i] = graph[1][static_cast<size_t>(i)];
        }
        return k;
    }
    catch (const std::exception& e) { set_error_from_exception("LB_GraphicPrice", e); return 0; }
    catch (...) { set_error_a("Unknown error in LB_GraphicPrice"); return 0; }
}

LB_API int LB_CALL LB_GraphicDelta(LB_Handle h, double dx, double* x_out, double* y_out, int max_len)
{
    clear_error();
    try
    {
        if (!h) { set_error_a("Null handle in LB_GraphicDelta"); return 0; }

        auto graph = as_ptr(h)->graphic_delta(dx);
        const int n = static_cast<int>(graph[0].size());

        if (!x_out || !y_out || max_len <= 0)
            return n;

        const int k = std::min(n, max_len);
        for (int i = 0; i < k; ++i)
        {
            x_out[i] = graph[0][static_cast<size_t>(i)];
            y_out[i] = graph[1][static_cast<size_t>(i)];
        }
        return k;
    }
    catch (const std::exception& e) { set_error_from_exception("LB_GraphicDelta", e); return 0; }
    catch (...) { set_error_a("Unknown error in LB_GraphicDelta"); return 0; }
}

LB_API double LB_CALL LB_GetYearFraction(const char* start_date, const char* end_date, int day_count_conv)
{
    clear_error(); // Clear previous error
    try
    {
        if (!start_date || !end_date) {
            set_error_a("Null dates passed to LB_GetYearFraction");
            return 0.0;
        }

        // Convert char* (VBA) to std::string
        std::string s(start_date);
        std::string e(end_date);

        // Create Date objects (using the struct in Date_Dealing.h)
        Date dStart(s);
        Date dEnd(e);

        // Map int from VBA to the DayCountConv enum class
        DayCountConv dc = map_ddc(day_count_conv);

        return yearFraction(dStart, dEnd, dc);
    }
    catch (const std::exception& e)
    {
        set_error_from_exception("LB_GetYearFraction", e);
        return 0.0;
    }
    catch (...)
    {
        set_error_a("Unknown error in LB_GetYearFraction");
        return 0.0;
    }
}

LB_API int LB_CALL LB_GetLastErrorA(char* buffer, int buffer_len)
{
    const int needed = static_cast<int>(g_lastErrorA.size()) + 1;

    if (!buffer || buffer_len <= 0)
        return needed;

    const int to_copy = std::min(buffer_len - 1, static_cast<int>(g_lastErrorA.size()));
    if (to_copy > 0)
        std::memcpy(buffer, g_lastErrorA.data(), static_cast<size_t>(to_copy));

    buffer[to_copy] = '\0';
    return to_copy + 1;
}

LB_API void LB_CALL LB_ClearLastError()
{
    clear_error();
}

#ifdef _WIN32
BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
#endif

#include <iostream> // Required for input/output operations (e.g., std::cout)
#include <cmath>    // Required for mathematical functions like pow() and round()
#include <vector>   // Required for std::vector, used in NPV calculation
#include <iomanip>  // Required for output formatting (e.g., std::fixed, std::setprecision)
#include <limits>   // Required for std::numeric_limits, used in IRR for precision

// Define a namespace to encapsulate financial functions,
// preventing naming conflicts and organizing the code.
namespace FinancialLibrary {

    /**
     * @brief Calculates the Future Value (FV) of a single cash flow.
     *
     * The future value is the value of a current asset at a future date based on
     * an assumed rate of growth.
     *
     * Formula: FV = PV * (1 + r)^n
     *
     * @param present_value The current value of the investment or cash flow.
     * @param annual_interest_rate The annual interest rate (e.g., 0.05 for 5%).
     * @param number_of_periods The number of periods (e.g., years) over which the investment grows.
     * @return The future value of the investment.
     */
    double calculateFutureValue(double present_value, double annual_interest_rate, int number_of_periods) {
        if (number_of_periods < 0) {
            std::cerr << "Error: Number of periods cannot be negative for Future Value calculation.\n";
            return 0.0; // Return 0 or throw an exception for invalid input
        }
        return present_value * std::pow((1.0 + annual_interest_rate), number_of_periods);
    }

    /**
     * @brief Calculates the Present Value (PV) of a single future cash flow.
     *
     * The present value is the current value of a future sum of money or stream of cash flows
     * given a specified rate of return.
     *
     * Formula: PV = FV / (1 + r)^n
     *
     * @param future_value 
     * @param annual_discount_rate .
     * @param number_of_periods 
     * @return 
     */
    double calculatePresentValue(double future_value, double annual_discount_rate, int number_of_periods) {
        if (number_of_periods < 0) {
            std::cerr << "Error: Number of periods cannot be negative for Present Value calculation.\n";
            return 0.0; // Return 0 or throw an exception for invalid input
        }
        if (annual_discount_rate <= -1.0) { // To prevent division by zero or negative base for power
            std::cerr << "Error: Discount rate must be greater than -100%.\n";
            return 0.0;
        }
        return future_value / std::pow((1.0 + annual_discount_rate), number_of_periods);
    }

    /**
     * @brief Calculates the Net Present Value (NPV) of a series of cash flows.
     *
     * NPV is the difference between the present value of cash inflows and the present value
     * of cash outflows over a period of time.
     *
     * Formula: NPV = Sum[CFt / (1 + r)^t] - Initial_Investment
     * where CFt is the cash flow at time t, r is the discount rate, and t is the period.
     *
     * @param discount_rate The discount rate (e.g., 0.10 for 10%).
     * @param cash_flows A vector of cash flows. The first element (index 0) is typically
     * the initial investment (a negative value), followed by positive inflows.
     * @return The Net Present Value.
     */
    double calculateNetPresentValue(double discount_rate, const std::vector<double>& cash_flows) {
        double npv = 0.0;
        if (discount_rate <= -1.0) {
            std::cerr << "Error: Discount rate must be greater than -100% for NPV calculation.\n";
            return std::numeric_limits<double>::quiet_NaN(); // Return NaN for invalid rate
        }

        for (size_t t = 0; t < cash_flows.size(); ++t) {
            // The first cash flow (index 0) is often the initial investment,
            // which is not discounted (t=0). Subsequent cash flows are discounted.
            npv += cash_flows[t] / std::pow((1.0 + discount_rate), t);
        }
        return npv;
    }

    /**
     * @brief Calculates simple interest.
     *
     * Simple interest is calculated only on the principal amount, or on that portion
     * of the principal amount that remains unpaid.
     *
     * Formula: Simple Interest = P * r * t
     *
     * @param principal The initial amount of money (principal).
     * @param annual_interest_rate The annual interest rate (e.g., 0.05 for 5%).
     * @param time_in_years The time period in years.
     * @return The calculated simple interest.
     */
    double calculateSimpleInterest(double principal, double annual_interest_rate, double time_in_years) {
        if (principal < 0 || annual_interest_rate < 0 || time_in_years < 0) {
            std::cerr << "Error: Principal, interest rate, and time cannot be negative for Simple Interest.\n";
            return 0.0;
        }
        return principal * annual_interest_rate * time_in_years;
    }

    /**
     * @brief Calculates compound interest.
     *
     * Compound interest is the interest on a loan or deposit calculated based on
     * both the initial principal and the accumulated interest from previous periods.
     *
     * Formula: A = P * (1 + r/n)^(nt)
     * Where A = amount, P = principal, r = annual interest rate,
     * n = number of times interest is compounded per year, t = time in years.
     * This function returns the total accumulated amount, not just the interest.
     *
     * @param principal The initial amount of money (principal).
     * @param annual_interest_rate The annual interest rate (e.g., 0.05 for 5%).
     * @param compounding_frequency The number of times interest is compounded per year (e.g., 1 for annually, 12 for monthly).
     * @param time_in_years The time period in years.
     * @return The total accumulated amount after compounding.
     */
    double calculateCompoundInterest(double principal, double annual_interest_rate, int compounding_frequency, double time_in_years) {
        if (principal < 0 || annual_interest_rate < 0 || compounding_frequency <= 0 || time_in_years < 0) {
            std::cerr << "Error: Invalid input for Compound Interest calculation. Check principal, rate, frequency, and time.\n";
            return 0.0;
        }
        return principal * std::pow((1.0 + annual_interest_rate / compounding_frequency), (compounding_frequency * time_in_years));
    }

    /**
     * @brief Calculates the Internal Rate of Return (IRR) for a series of cash flows.
     *
     * IRR is the discount rate that makes the Net Present Value (NPV) of all cash flows
     * (both inflows and outflows) from a particular project or investment equal to zero.
     * This implementation uses a numerical approximation method (Newton-Raphson or bisection
     * could be more robust, but a simple iterative approach is shown here).
     *
     * @param cash_flows A vector of cash flows. The first element (index 0) is typically
     * the initial investment (a negative value), followed by positive inflows.
     * @param guess An initial guess for the IRR (optional, default 0.1).
     * @param tolerance The desired precision for the IRR (optional, default 1e-6).
     * @param max_iterations The maximum number of iterations for the approximation (optional, default 1000).
     * @return The calculated IRR, or NaN if convergence is not achieved or inputs are invalid.
     */
    double calculateInternalRateOfReturn(const std::vector<double>& cash_flows, double guess = 0.1, double tolerance = 1e-6, int max_iterations = 1000) {
        if (cash_flows.empty()) {
            std::cerr << "Error: Cash flow vector cannot be empty for IRR calculation.\n";
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Check if there's at least one negative and one positive cash flow for a valid IRR
        bool has_negative = false;
        bool has_positive = false;
        for (double cf : cash_flows) {
            if (cf < 0) has_negative = true;
            if (cf > 0) has_positive = true;
        }
        if (!has_negative || !has_positive) {
            std::cerr << "Warning: IRR requires at least one negative and one positive cash flow.\n";
            return std::numeric_limits<double>::quiet_NaN();
        }

        double irr = guess;
        for (int i = 0; i < max_iterations; ++i) {
            double npv = 0.0;
            double derivative_npv = 0.0; // Derivative of NPV with respect to IRR (for Newton-Raphson like step)

            for (size_t t = 0; t < cash_flows.size(); ++t) {
                double denominator = std::pow((1.0 + irr), t);
                if (denominator == 0) { // Avoid division by zero if irr makes (1+irr) zero
                     std::cerr << "Error: Division by zero encountered during IRR calculation. Try a different guess.\n";
                     return std::numeric_limits<double>::quiet_NaN();
                }
                npv += cash_flows[t] / denominator;
                derivative_npv -= (double)t * cash_flows[t] / std::pow((1.0 + irr), t + 1);
            }

            if (std::abs(npv) < tolerance) {
                return irr; // IRR found within tolerance
            }

            if (derivative_npv == 0) {
                std::cerr << "Warning: Derivative is zero during IRR calculation. Cannot converge.\n";
                return std::numeric_limits<double>::quiet_NaN();
            }

            irr = irr - npv / derivative_npv; // Newton-Raphson step
        }

        std::cerr << "Warning: IRR did not converge within " << max_iterations << " iterations.\n";
        return std::numeric_limits<double>::quiet_NaN(); // Return NaN if not converged
    }

} // namespace FinancialLibrary

// --- Main function to demonstrate the Financial Library ---
int main() {
    // Set output precision for financial values
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "--- Financial Library Demonstrations ---\n\n";

    // --- Future Value Example ---
    double pv_fv = 1000.0;
    double rate_fv = 0.05; // 5% annual interest
    int periods_fv = 10;  // 10 years
    double fv = FinancialLibrary::calculateFutureValue(pv_fv, rate_fv, periods_fv);
    std::cout << "Future Value (FV):\n";
    std::cout << "  Present Value: $" << pv_fv << "\n";
    std::cout << "  Annual Rate: " << rate_fv * 100 << "%\n";
    std::cout << "  Periods: " << periods_fv << " years\n";
    std::cout << "  Calculated FV: $" << fv << "\n\n";

    // --- Present Value Example ---
    double fv_pv = 2000.0;
    double rate_pv = 0.08; // 8% annual discount rate
    int periods_pv = 5;   // 5 years
    double pv = FinancialLibrary::calculatePresentValue(fv_pv, rate_pv, periods_pv);
    std::cout << "Present Value (PV):\n";
    std::cout << "  Future Value: $" << fv_pv << "\n";
    std::cout << "  Annual Discount Rate: " << rate_pv * 100 << "%\n";
    std::cout << "  Periods: " << periods_pv << " years\n";
    std::cout << "  Calculated PV: $" << pv << "\n\n";

    // --- Net Present Value (NPV) Example ---
    // Initial investment (outflow) is negative, subsequent cash flows are inflows.
    std::vector<double> cash_flows_npv = {-10000.0, 3000.0, 4000.0, 5000.0, 3000.0};
    double discount_rate_npv = 0.10; // 10% discount rate
    double npv = FinancialLibrary::calculateNetPresentValue(discount_rate_npv, cash_flows_npv);
    std::cout << "Net Present Value (NPV):\n";
    std::cout << "  Discount Rate: " << discount_rate_npv * 100 << "%\n";
    std::cout << "  Cash Flows: [";
    for (size_t i = 0; i < cash_flows_npv.size(); ++i) {
        std::cout << "$" << cash_flows_npv[i] << (i == cash_flows_npv.size() - 1 ? "" : ", ");
    }
    std::cout << "]\n";
    if (!std::isnan(npv)) {
        std::cout << "  Calculated NPV: $" << npv << "\n\n";
    } else {
        std::cout << "  NPV calculation failed.\n\n";
    }


    // --- Simple Interest Example ---
    double principal_si = 5000.0;
    double rate_si = 0.06; // 6% annual interest
    double time_si = 3.0;  // 3 years
    double simple_interest = FinancialLibrary::calculateSimpleInterest(principal_si, rate_si, time_si);
    std::cout << "Simple Interest:\n";
    std::cout << "  Principal: $" << principal_si << "\n";
    std::cout << "  Annual Rate: " << rate_si * 100 << "%\n";
    std::cout << "  Time: " << time_si << " years\n";
    std::cout << "  Calculated Simple Interest: $" << simple_interest << "\n\n";

    // --- Compound Interest Example ---
    double principal_ci = 1000.0;
    double rate_ci = 0.07; // 7% annual interest
    int compounding_freq_ci = 12; // Compounded monthly
    double time_ci = 5.0;  // 5 years
    double compound_amount = FinancialLibrary::calculateCompoundInterest(principal_ci, rate_ci, compounding_freq_ci, time_ci);
    std::cout << "Compound Interest (Total Amount):\n";
    std::cout << "  Principal: $" << principal_ci << "\n";
    std::cout << "  Annual Rate: " << rate_ci * 100 << "%\n";
    std::cout << "  Compounding Frequency: " << compounding_freq_ci << " (monthly)\n";
    std::cout << "  Time: " << time_ci << " years\n";
    std::cout << "  Calculated Total Amount: $" << compound_amount << "\n\n";

    // --- Internal Rate of Return (IRR) Example ---
    // Cash flows: Initial investment of -1000, then inflows of 300, 400, 500, 600
    std::vector<double> cash_flows_irr = {-1000.0, 300.0, 400.0, 500.0, 600.0};
    double irr = FinancialLibrary::calculateInternalRateOfReturn(cash_flows_irr);
    std::cout << "Internal Rate of Return (IRR):\n";
    std::cout << "  Cash Flows: [";
    for (size_t i = 0; i < cash_flows_irr.size(); ++i) {
        std::cout << "$" << cash_flows_irr[i] << (i == cash_flows_irr.size() - 1 ? "" : ", ");
    }
    std::cout << "]\n";
    if (!std::isnan(irr)) {
        std::cout << "  Calculated IRR: " << irr * 100 << "%\n\n";
    } else {
        std::cout << "  IRR calculation failed or did not converge.\n\n";
    }

    // --- IRR Example with no convergence (or invalid cash flows) ---
    std::vector<double> cash_flows_irr_no_conv = {-1000.0, -200.0, -50.0}; // All negative cash flows
    double irr_no_conv = FinancialLibrary::calculateInternalRateOfReturn(cash_flows_irr_no_conv);
    std::cout << "Internal Rate of Return (IRR) - No Convergence Example:\n";
    std::cout << "  Cash Flows: [";
    for (size_t i = 0; i < cash_flows_irr_no_conv.size(); ++i) {
        std::cout << "$" << cash_flows_irr_no_conv[i] << (i == cash_flows_irr_no_conv.size() - 1 ? "" : ", ");
    }
    std::cout << "]\n";
    if (!std::isnan(irr_no_conv)) {
        std::cout << "  Calculated IRR: " << irr_no_conv * 100 << "%\n\n";
    } else {
        std::cout << "  IRR calculation failed or did not converge (expected).\n\n";
    }


    return 0;
}

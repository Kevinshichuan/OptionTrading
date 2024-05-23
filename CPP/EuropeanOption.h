#ifndef EUROPEAN_OPTION_H
#define EUROPEAN_OPTION_H

#include "Option.h"

class EuropeanOption : public Option {
public:
    using Option::Option;  // Inherit constructor

    double payoff(const std::vector<double>& price_path) const override {
        double final_price = price_path.back();
        return std::max(final_price - strike, 0.0) * exp(-rate * maturity);
    }
};

#endif // EUROPEAN_OPTION_H

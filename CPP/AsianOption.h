#ifndef ASIAN_OPTION_H
#define ASIAN_OPTION_H

#include "Option.h"
#include <numeric>

class AsianOption : public Option {
public:
    using Option::Option;  // Inherit constructor

    double payoff(const std::vector<double>& price_path) const override {
        double average_price = std::accumulate(price_path.begin(), price_path.end(), 0.0) / price_path.size();
        return std::max(average_price - strike, 0.0) * exp(-rate * maturity);
    }
};

#endif // ASIAN_OPTION_H

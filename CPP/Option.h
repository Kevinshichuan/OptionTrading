#ifndef OPTION_H
#define OPTION_H

#include <vector>
#include <random>

// Abstract class for Options
class Option {
public:
    Option(double strike, double maturity, double spot, double rate, double volatility)
    : strike(strike), maturity(maturity), spot(spot), rate(rate), volatility(volatility) {}
    virtual ~Option() = default;

    // Pure virtual function for calculating the payoff
    virtual double payoff(const std::vector<double>& price_path) const = 0;

    // Function to generate a price path
    virtual std::vector<double> generatePath(std::mt19937& rng, std::normal_distribution<double>& dist) const {
        std::vector<double> path;
        double currentPrice = spot;
        double dt = maturity / 252.0; // Assuming 252 trading days in a year
        for (int i = 0; i <= 252; ++i) {
            currentPrice *= exp((rate - 0.5 * volatility * volatility) * dt + volatility * sqrt(dt) * dist(rng));
            path.push_back(currentPrice);
        }
        return path;
    }

protected:
    double strike;
    double maturity;
    double spot;
    double rate;
    double volatility;
};

#endif // OPTION_H

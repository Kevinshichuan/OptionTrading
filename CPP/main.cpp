// #include "MonteCarloSimulator.h"
// #include "AsianOption.h"
// #include "EuropeanOption.h"

// int main() {
//     AsianOption asian(100, 1, 100, 0.05, 0.2);  // Example parameters: strike, maturity, spot, rate, volatility
//     MonteCarloSimulator mc(&asian, 10000);  // Asian option with 10,000 simulations

//     mc.simulate();
//     mc.displayResults();

//     return 0;
// }


#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <cmath>

// Define the Option class
class Option {
public:
    Option(double spot, double strike, double rate, double volatility, double maturity)
        : spot(spot), strike(strike), rate(rate), volatility(volatility), maturity(maturity) {}

    // Generate a price path using geometric Brownian motion
    std::vector<double> generatePath(std::mt19937& rng, int steps) const {
        std::vector<double> path(steps + 1);
        std::normal_distribution<double> dist(0.0, 1.0);
        double dt = maturity / steps;
        double drift = (rate - 0.5 * volatility * volatility) * dt;
        double diffusion = volatility * std::sqrt(dt);

        path[0] = spot;
        for (int i = 1; i <= steps; ++i) {
            double increment = dist(rng);
            path[i] = path[i - 1] * std::exp(drift + diffusion * increment);
        }
        return path;
    }

    // Calculate the Asian option payoff
    double calculatePayoff(const std::vector<double>& path) const {
        double average = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        return std::exp(-rate * maturity) * std::max(average - strike, 0.0);
    }

private:
    double spot;
    double strike;
    double rate;
    double volatility;
    double maturity;
};

// Define the Monte Carlo Simulator class
class MonteCarloSimulator {
public:
    MonteCarloSimulator(const Option& option, int numSimulations, int numSteps)
        : option(option), numSimulations(numSimulations), numSteps(numSteps) {}

    double simulate() {
        std::random_device rd;
        std::mt19937 rng(rd());
        double sumPayoffs = 0.0;

        for (int i = 0; i < numSimulations; ++i) {
            std::vector<double> path = option.generatePath(rng, numSteps);
            double payoff = option.calculatePayoff(path);
            sumPayoffs += payoff;
        }

        return sumPayoffs / numSimulations;
    }

private:
    const Option& option;
    int numSimulations;
    int numSteps;
};

// Main function
int main() {
    double spot = 100;         // Initial stock price
    double strike = 100;       // Strike price
    double rate = 0.05;        // Annual risk-free interest rate
    double volatility = 0.2;   // Volatility of the stock
    double maturity = 1;       // Time to maturity in years
    int numSimulations = 10000; // Number of simulations
    int numSteps = 252;        // Number of steps in the stock price path

    Option asianOption(spot, strike, rate, volatility, maturity);
    MonteCarloSimulator simulator(asianOption, numSimulations, numSteps);

    double price = simulator.simulate();
    std::cout << "The estimated price of the Asian option is: " << price << std::endl;

    return 0;
}

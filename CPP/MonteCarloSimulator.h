#ifndef MONTE_CARLO_SIMULATOR_H
#define MONTE_CARLO_SIMULATOR_H

#include "Option.h"
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <iostream>

class MonteCarloSimulator {
public:
    MonteCarloSimulator(Option* option, int num_simulations)
    : option(option), num_simulations(num_simulations) {}

    void simulate() {
        std::mt19937 rng(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);

        results.reserve(num_simulations);
        for (int i = 0; i < num_simulations; ++i) {
            std::vector<double> path = option->generatePath(rng, dist);
            double payoff = option->payoff(path);
            results.push_back(payoff);
        }
        calculateStatistics();
    }

    void displayResults() const {
        std::cout << "Mean Estimated Price: " << mean_estimate << std::endl;
        std::cout << "Standard Deviation: " << std_dev << std::endl;
    }

protected:
    Option* option;
    int num_simulations;
    std::vector<double> results;
    double mean_estimate = 0.0;
    double std_dev = 0.0;

    void calculateStatistics() {
        double sum = std::accumulate(results.begin(), results.end(), 0.0);
        mean_estimate = sum / num_simulations;
        double sq_sum = std::inner_product(results.begin(), results.end(), results.begin(), 0.0);
        std_dev = std::sqrt(sq_sum / num_simulations - mean_estimate * mean_estimate);
    }
};

#endif // MONTE_CARLO_SIMULATOR_H

#include "swingStrategy.h"
#include <iostream>
#include <vector>
#include <limits>

SwingStrategy::SwingStrategy(int lookback) : lookback(lookback) {}

double SwingStrategy::averageSum(std::vector<double>& swings) {
    if (swings.empty()) return 0.0;

    double sum = 0;
    for (double swing : swings) sum += swing;
    return sum / swings.size();
}

std::vector<double>& SwingStrategy::detect(const std::vector<Candle*>& candles) {
    strategyData.clear();
    strategyData.assign(candles.size(), 0.0);
    std::vector<double> swings;

    // SIZE_MAX is maximum possible value for size_t
    size_t lastChanged = std::numeric_limits<size_t>::max();
    double lastChangedValue = 0.0;

    for (size_t i = 1; i + 1 < candles.size(); ++i) {
        double previous = candles[i - 1]->close;
        double current = candles[i]->close;
        double next = candles[i + 1]->close;

        // Detect local swings
        if (previous < current && next < current) swings.push_back(1.0); // local high
        else if (previous > current && next > current) swings.push_back(-1.0); // local low

        if (i % lookback == 0) {
            // if there are more local highs it will be more than 0.0 (up trend) otherwise more local lows (down trend)
            double avg = averageSum(swings);

            double trend = 0.0;
            if (avg > 0.0) trend = 1.0;
            else if (avg < 0.0) trend = -1.0;

            // fill the space between last swing and current if lastChangedValue was not neutral (0.0)
            if (lastChanged != std::numeric_limits<size_t>::max() && lastChangedValue != 0.0) {
                for (size_t j = lastChanged + 1; j < i; ++j) {
                    strategyData[j] = lastChangedValue;
                }
            }

            strategyData[i] = trend;
            lastChanged = i;
            lastChangedValue = trend;

            swings.clear();
        }
    }

    return strategyData;
}
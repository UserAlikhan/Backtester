#include "swingStrategy.h"
#include <iostream>

SwingStrategy::SwingStrategy(int lookback) : lookback(lookback) {}

double SwingStrategy::sumSwings(std::vector<double>& swings) {
    if (swings.empty()) return 0.0;

    double sum = 0;
    for (double swing : swings) sum += swing;
    return sum / swings.size();
}

std::vector<TrendType> SwingStrategy::detect(const std::vector<Candle*>& candles) {
    std::vector<TrendType> strategyData(candles.size(), TrendType::NEUTRAL);
    std::vector<double> swings;
    // SIZE_MAX is maximum possible value for 
    size_t lastChanged = SIZE_MAX;
    double lastChangedValue = 0.0;

    for (size_t i = 1; i + 1 < candles.size(); ++i) {
        double previous = candles[i - 1]->close;
        double current = candles[i]->close;
        double next = candles[i + 1]->close;

        // Detect local swings
        if (previous < current && next < current) swings.push_back(1.0); // higher high
        else if (previous > current && next > current) swings.push_back(-1.0); // lower low

        if (i % lookback == 0) {
            double avg = sumSwings(swings);

            TrendType trend = TrendType::NEUTRAL;
            if (avg > 0.0) trend = TrendType::BULLISH;
            else if (avg < 0.0) trend = TrendType::BEARISH;

            strategyData[i] = trend;

            // fill the space between last swing and current if lastChange was not neutral (0.0)
            if (lastChanged != SIZE_MAX && lastChangedValue != 0.0) {
                for (size_t j = lastChanged + 1; j < i; ++j) {
                    strategyData[j] = lastChangedValue > 0.0 ? TrendType::BULLISH :
                        (lastChangedValue < 0.0 ? TrendType::BEARISH : TrendType::NEUTRAL);
                }
            }

            lastChanged = i;
            lastChangedValue = avg;

            swings.clear();
        }
    }

    return strategyData;
}
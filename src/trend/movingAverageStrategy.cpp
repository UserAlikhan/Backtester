#include "movingAverageStrategy.h"
#include <iostream>

MovingAverageStrategy::MovingAverageStrategy(int period) : period(period) {}

std::vector<double>& MovingAverageStrategy::detect(const std::vector<Candle*>& candles) {
    // calculate moving averages
    std::vector<double> movingAverageData = SMA::calculateForOnePeriod(candles, period);
    
    // trends is protected variable inside TrendStrategy class
    strategyData.clear();
    strategyData.assign(movingAverageData.size(), 0.0);

    for (size_t i = candles.size() - movingAverageData.size(); i < candles.size(); ++i) {
        if (candles[i]->close > movingAverageData[i]) {
            strategyData[i] = 1.0;
        } else if (candles[i]->close < movingAverageData[i]) {
            strategyData[i] = -1.0;
        }
    }

    return strategyData;
}
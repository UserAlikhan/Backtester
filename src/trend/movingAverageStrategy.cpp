#include "movingAverageStrategy.h"
#include <iostream>

MovingAverageStrategy::MovingAverageStrategy(int period) : period(period) {}

std::vector<TrendType> MovingAverageStrategy::detect(const std::vector<Candle*>& candles) {
    // calculate moving averages
    std::vector<double> movingAverageData = SMA::calculateForOnePeriod(candles, period);
    std::vector<TrendType> strategyData;
    strategyData.assign(movingAverageData.size(), TrendType::NEUTRAL);

    for (size_t i = candles.size() - movingAverageData.size(); i < candles.size(); ++i) {
        if (candles[i]->close > movingAverageData[i]) {
            strategyData[i] = TrendType::BULLISH;
        } else if (candles[i]->close < movingAverageData[i]) {
            strategyData[i] = TrendType::BEARISH;
        }
    }

    return strategyData;
}
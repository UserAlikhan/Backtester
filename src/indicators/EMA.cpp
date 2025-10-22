#include "ema.h"
#include <iostream>
#include <algorithm>

EMA::EMA(std::string name, int p1, int p2) 
    : Indicator(name), period1(p1), period2(p2) {}

void EMA::addIntoPeriod1Dataset(double price) {
    dataPeriod1.push_back(price);
}

void EMA::addIntoPeriod2Dataset(double price) {
    dataPeriod2.push_back(price);
}

std::pair<double, double> EMA::calculateSmoothingFactor() {
    if (smoothing != 0.0) {
        double smoothingFactorPer1 = smoothing / (period1 + 1);
        double smoothingFactorPer2 = smoothing / (period2 + 1);

        return {smoothingFactorPer1, smoothingFactorPer2};
    }

    return {};
}

void EMA::calculate(std::vector<Candle*>& candles) {
    // check if there are enough data points
    if ((int)candles.size() < std::max(period1, period2)) { 
        std::cout << "No data provided or it is smaller than specified periods" << std::endl;
        return; 
    }

    double sumPer1 = 0.0, sumPer2 = 0.0;

    // first data point for ema needs to be calculated like sma
    if (dataPeriod1.size() == 0 && dataPeriod2.size() == 0) {
        for (int i = 0; i < std::max(period1, period2) - 1; ++i) {
            // calculate smaller period
            if (i > std::max(period1, period2) - std::min(period1, period2) - 1) {
                sumPer1 += candles[i]->close;
            }

            sumPer2 += candles[i]->close;
        }

        addIntoPeriod1Dataset(sumPer1 / period1);
        addIntoPeriod2Dataset(sumPer2 / period2);
    }

    std::pair<double, double> result = calculateSmoothingFactor();
    double& smoothingFactorPer1 = result.first;
    double& smoothingFactorPer2 = result.second;

    for (size_t i = std::max(period1, period2); i < candles.size(); ++i) {
        double emaPer1 = smoothingFactorPer1 * (candles[i]->close - dataPeriod1.back()) + dataPeriod1.back();
        double emaPer2 = smoothingFactorPer2 * (candles[i]->close - dataPeriod2.back()) + dataPeriod2.back();

        addIntoPeriod1Dataset(emaPer1);
        addIntoPeriod2Dataset(emaPer2); 
    }

    std::cout << "Data size: " << candles.size() 
        << " Period 1 size: " << dataPeriod1.size() 
        << " Period 2 size: " << dataPeriod2.size() 
        << std::endl;
}

std::vector<std::pair<TradeType, int>> EMA::findIntersections() {
    std::vector<std::pair<TradeType, int>> result;
    // does not matter if we iterate over dataPeriod1 or dataPeriod2
    for (size_t i = 1; i < dataPeriod1.size(); i++) {
        // Golden cross
        if (dataPeriod2[i - 1] > dataPeriod1[i - 1] && dataPeriod1[i] > dataPeriod2[i]) {
            result.push_back({TradeType::LONG, i});
        } else if (dataPeriod1[i - 1] > dataPeriod2[i - 1] && dataPeriod2[i] > dataPeriod1[i]) {
            result.push_back({TradeType::SHORT, i});
        }
    }

    return result;
}
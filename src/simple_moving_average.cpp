#include <simple_moving_average.h>
#include <iostream>
#include <vector>

SMA::SMA(const std::vector<Candle> closeData, int period) 
    : closeData(closeData), period(period) {}

std::vector<double> SMA::calculate() {
    std::vector<double> result;

    if (closeData.size() < period) {
        return result;
    }

    double sum = 0.0;

    // initial data
    for (int i = 0; i < period; ++i) {
        sum += closeData[i].close;
    }

    result.push_back(sum / period);

    // move forward and calculate SMA
    for (int i = period; i < closeData.size(); ++i) {
        sum += closeData[i].close - closeData[i - period].close;
        result.push_back(sum / period);
    }

    return result;
}
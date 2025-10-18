#include "ema.h"
#include <iostream>
#include <algorithm>

EMA::EMA(std::string name, int p1, int p2) 
    : Indicator(name), period1(p1), period2(p2) {}

void EMA::calculate(std::vector<Candle*>& candles) {
    std::vector<double> sma1, sma2;

    // check if there are enough data points
    if (candles.size() < std::max(period1, period2)) { 
        std::cout << "No data provided" << std::endl;
        return; 
    }

    double sumPeriod1 = 0.0, sumPeriod2 = 0.0;

    // initialization
    for (int i = 0; i < period2 - 1; i++) {
        // calculate smaller period
        if (i >= std::max(period1, period2) - std::min(period1, period2) - 1) {
            sumPeriod1 += candles[i]->close;
        }

        sumPeriod2 += candles[i]->close;
    }

    sma1.push_back(sumPeriod1 / period1);
    sma2.push_back(sumPeriod2 / period2);

    // calculate for all data points
    for(int i = std::max(period1, period2); i < candles.size(); i++) {
        sumPeriod1 += candles[i]->close - candles[i - period1]->close;
        sumPeriod2 += candles[i]->close - candles[i - period2]->close;

        sma1.push_back(sumPeriod1 / period1);
        sma2.push_back(sumPeriod2 / period2);
    }

    // save data into class
    dataPeriod1 = sma1;
    dataPeriod2 = sma2;

    std::cout << "Data size: " << candles.size() << " Period 1 size: " << dataPeriod1.size() << " Period 2 size: " << dataPeriod2.size() << std::endl;
}
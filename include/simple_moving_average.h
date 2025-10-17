#pragma once
#include <iostream>
#include <vector>
#include <candle.h>
#include <trade.h>
#include <trade_result.h>
#include <statistic.h>

class SMA {
    private:
        std::vector<Candle> candleData;
        int period1;
        int period2;
    
    public:
        // constructor
        SMA(const std::vector<Candle>& candleData, int period, int period2);
        // loops over data, executes trades and calculates the result
        std::vector<double> calculate();
};
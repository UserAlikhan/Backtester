#pragma once
#include <iostream>
#include <vector>
#include <candle.h>

class SMA {
    private:
        std::vector<Candle> candleData;
        int period1;
        int period2;
    
    public:
        SMA(const std::vector<Candle>& candleData, int period, int period2);
        std::vector<double> calculate();
};
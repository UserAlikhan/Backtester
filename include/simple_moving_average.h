#pragma once
#include <iostream>
#include <vector>
#include <candle.h>

class SMA {
    private:
        std::vector<Candle> closeData;
        int period1;
        int period2;
    
    public:
        SMA(const std::vector<Candle> closeData, int period, int period2);
        std::vector<double> calculate();
};
#pragma once
#include <iostream>
#include "indicator.h"

class EMA : public Indicator {
    private:
        int period1;
        int period2;
        std::vector<double> dataPeriod1;
        std::vector<double> dataPeriod2;
    public:
        EMA(std::string name, int p1, int p2);

        std::vector<double>& getDataPeriod1() { return dataPeriod1;}
        
        std::vector<double>& getDataPeriod2() { return dataPeriod2;}

        void calculate(std::vector<Candle*>& candles) override;
};
#pragma once
#include "indicator.h"
#include "intersectionIndicator.h"
#include "intersectionEnum.h"
#include "tradeTypeEnum.h"
#include <iostream>

class SMA : public Indicator, public IntersectionIndicator {
    private:
        int period1;
        int period2;
        std::vector<double> dataPeriod1;
        std::vector<double> dataPeriod2;
    public:
        SMA(std::string name, int p1, int p2);

        std::vector<double>& getDataPeriod1() { return dataPeriod1; }
        
        std::vector<double>& getDataPeriod2() { return dataPeriod2; }

        void calculate(std::vector<Candle*>& candles) override;

        // returns intersection type and intersection index from data
        std::vector<std::pair<TradeType, int>>& findIntersections() override;
};
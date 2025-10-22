#pragma once
#include "indicator.h"
#include "intersectionIndicator.h"
#include "intersectionEnum.h"
#include "tradeTypeEnum.h"
#include <iostream>

class EMA : public Indicator, public IntersectionIndicator {
    private:
        int period1;
        int period2;
        double smoothing = 2.0;
        std::vector<double> dataPeriod1;
        std::vector<double> dataPeriod2;
    public:
        EMA(std::string name, int p1, int p2);

        std::vector<double>& getDataPeriod1() { return dataPeriod1;}

        void addIntoPeriod1Dataset(double price);
        
        std::vector<double>& getDataPeriod2() { return dataPeriod2;}

        void addIntoPeriod2Dataset(double price);

        std::pair<double, double> calculateSmoothingFactor();

        void calculate(std::vector<Candle*>& candles) override;

        // returns intersection type and intersection index from data
        std::vector<std::pair<TradeType, int>>& findIntersections() override;
};
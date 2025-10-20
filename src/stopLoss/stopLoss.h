#pragma once
#include <iostream>
#include "trade.h"
#include "candle.h"

class StopLoss {
    protected:
        double percentage;
        double price;

    public:
        StopLoss(double percentage);
        virtual ~StopLoss() = default;

        // although trade has entry price we passing another one for trailing stop
        virtual void setPrice(Trade* trade, double& entryPrice) = 0;

        virtual void checkExit(Trade* trade, int& index, std::vector<Candle*>& candles) = 0; // abstact

        double getPrice() { return price; }
};
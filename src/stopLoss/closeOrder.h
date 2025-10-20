#pragma once
#include <iostream>
#include "trade.h"
#include "candle.h"

class CloseOrder {
    protected:
        double percentage;
        double price = 0.0;

    public:
        CloseOrder(double percentage);
        virtual ~CloseOrder() = default;

        // although trade has entry price we passing another one for trailing stop
        virtual void setPrice(Trade* trade, double& entryPrice) = 0;

        virtual void checkExit(Trade* trade, double close, CloseOrder* closeOrder, 
            std::vector<Trade*> trades, std::vector<CloseOrder*> stopLosses
        ) = 0; // abstact

        double getPrice() { return price; }
};
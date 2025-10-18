#pragma once
#include <iostream>
#include "trade.h"

class StopLoss {
    protected:
        double percentage;
        double price;

    public:
        StopLoss(double percentage);

        virtual void setPrice(Trade& trade, double entryPrice) = 0;

        virtual bool checkExit(Trade& trade, double currentPrice) = 0; // abstact
};
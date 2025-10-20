#pragma once
#include <iostream>
#include "closeOrder.h"

class TakeProfit : public CloseOrder {
    public:
        TakeProfit(double percentage);

        void setPrice(Trade* trade, double& entryPrice) override;
        void checkExit(Trade* trade, double close) override;
};
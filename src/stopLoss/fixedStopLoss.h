#pragma once
#include <iostream>
#include "trade.h"
#include "stopLoss.h"
#include "tradeTypeEnum.h"

class FixedStopLoss : public StopLoss {
    public:
        FixedStopLoss(double percentage);
        
        void setPrice(Trade& trade, double entryPrice) override;
        bool checkExit(Trade& trade, double currentPrice) override;
};
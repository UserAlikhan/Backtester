#pragma once
#include <iostream>
#include "trade.h"
#include "stopLoss.h"

class TrailingStopLoss : public StopLoss {
    public:
        TrailingStopLoss(double percentage);
        
        void setPrice(Trade& trade, double entryPrice) override;
        bool checkExit(Trade& trade, double currentPrice) override;
};

#pragma once
#include <iostream>
#include "trade.h"
#include "stopLoss.h"
#include "tradeTypeEnum.h"
#include "candle.h"

class FixedStopLoss : public StopLoss {
    public:
        FixedStopLoss(double percentage);
        
        void setPrice(Trade* trade, double& entryPrice) override;
        void checkExit(Trade* trade, int& index, std::vector<Candle*>& candles) override;
};
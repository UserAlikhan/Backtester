#pragma once
#include <iostream>
#include "trade.h"
#include "stopLoss.h"
#include "tradeTypeEnum.h"

class TrailingStopLoss : public StopLoss {
    public:
        TrailingStopLoss(double percentage);
        
        void setPrice(Trade* trade, double& currentPrice) override;
        void checkExit(Trade* trade, int& index, std::vector<Candle*>& candles) override;
};

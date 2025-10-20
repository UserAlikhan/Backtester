#pragma once
#include <iostream>
#include "trade.h"
#include "closeOrder.h"
#include "tradeTypeEnum.h"
#include "candle.h"

class FixedStopLoss : public CloseOrder {
    public:
        FixedStopLoss(double percentage);
        
        void setPrice(Trade* trade, double& entryPrice) override;
        void checkExit(Trade* trade, double close, CloseOrder* fixedSl, 
            std::vector<Trade*> trades, std::vector<CloseOrder*> stopLosses
        ) override;
};
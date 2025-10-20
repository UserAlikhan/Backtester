#pragma once
#include <iostream>
#include "trade.h"
#include "closeOrder.h"
#include "tradeTypeEnum.h"

class TrailingStopLoss : public CloseOrder {
    public:
        TrailingStopLoss(double percentage);
        
        void setPrice(Trade* trade, double& currentPrice) override;
        void checkExit(Trade* trade, double close) override;
};

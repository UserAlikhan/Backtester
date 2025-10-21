#pragma once
#include <iostream>
#include "tradeTypeEnum.h"

class Trade {
    private:
        TradeType tradeType;
        double transactionAmount;
        double entryPrice;
        double closePrice;
        double PL;
        double PLpercentage;
    
    public:
        // constructor
        Trade(TradeType tradeType, double transactionAmount, double entryPrice);

        TradeType getTradeType() { return tradeType; }
        double getEntryPrice() { return entryPrice; }
        double getClosePrice() { return closePrice; }
        double getPL() { return PL; }

        double calculateLiquidationPrice(double& currentBalance);
        void closeTrade(double closePrice);
        void calculatePL(double& currentBalance);
};
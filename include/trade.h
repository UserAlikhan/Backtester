#pragma once
#include <iostream>
#include <trade_result.h>

enum TradeType {
    LONG,
    SHORT,
};

class Trade {
    private:
        TradeType tradeType;
        double currentBalance;
        double transactionAmount;
        double entryPrice;
        double closePrice;
        double maxLossPercentage;
    
    public:
        Trade(TradeType tradeType, double currentBalance, double transactionAmount, double entryPrice, double closePrice, double maxLossPercentage);
        double result();
        static void trackStopLoss(
            bool& isInDeathCross, bool& isInGoldenCross, TradeType tradeType,
            double& entryPrice, double& closePrice, double close, 
            double& balance, std::vector<TradeResult>& tradeResults,
            int maxLossPercentage
        );
        static void closeTrade(
            double& entryPrice, double closePrice, TradeType tradeType, 
            double& balance, double maxLossPercentage, 
            std::vector<TradeResult>& tradeResults
        );
};
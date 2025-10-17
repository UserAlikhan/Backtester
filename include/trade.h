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
        Trade(
            TradeType tradeType, double currentBalance, double transactionAmount, 
            double entryPrice, double closePrice, double maxLossPercentage
        );
        // this method calculates new balance based closed trade
        double result();

        // this method closes trades
        static void closeTrade(
            double& entryPrice, double closePrice, TradeType tradeType, 
            double& balance, double maxLossPercentage, 
            std::vector<TradeResult>& tradeResults
        );
        
        // this method tracks current loss and compare its with allowed max loss
        // if it is the same or bigger, it closes the trade
        static void trackStopLoss(
            bool& isInDeathCross, bool& isInGoldenCross, TradeType tradeType,
            double& entryPrice, double& closePrice, double close, 
            double& balance, std::vector<TradeResult>& tradeResults,
            int maxLossPercentage
        );
};
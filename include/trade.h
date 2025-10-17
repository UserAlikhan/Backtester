#pragma once
#include <iostream>

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
    
    public:
        Trade(TradeType tradeType, double currentBalance, double transactionAmount, double entryPrice, double closePrice);
        double result();
};
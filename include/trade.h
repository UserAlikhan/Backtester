#pragma once
#include <iostream>

enum TradeType {
    LONG,
    SHORT,
};

class Trade {
    private:
        TradeType tradeType;
        double transactionAmount;
        double entryPrice;
        double closePrice;
    
    public:
        Trade(TradeType tradeType, double transactionAmount, double entryPrice, double closePrice);
        double result();
};
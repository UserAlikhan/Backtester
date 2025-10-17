#pragma once
#include <iostream>
#include <vector>
#include <trade_result.h>

// TODO: Make Statistic as a class and make methods like calculate statistic and print statistic
class Statistic {
    private:
        int totalNumberOfTrades;
        int numOfProfitableTrades;
        int numOfLossTrades;
        int gainAmount;
    
    public:
        Statistic(
            int totalNumberOfTrades, int numOfProfitableTrades, 
            int numOfLossTrades, int gainAmount
        );
        static Statistic calculate(std::vector<TradeResult> tradeResults);
        void print(); 
};
#pragma once
#include <iostream>
#include <vector>
#include <trade_result.h>

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
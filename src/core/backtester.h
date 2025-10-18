#pragma once
#include <vector>
#include "trade.h"

class Backtester {
    private:
        std::vector<Trade*> trades;
        // std::vector<StopLoss*> stopLosses;
    public:
        Backtester();
        ~Backtester();
        
        std::vector<Trade*>& getAllTrades() { return trades; }

        void addTrade();
        void addStopLoss();
        void run();
};
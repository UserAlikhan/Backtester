#pragma once
#include <vector>
#include "trade.h"
#include "stopLoss.h"
#include <candle.h>
#include <indicator.h>
#include "intersectionEnum.h"
#include <intersectionIndicator.h>

class Backtester {
    private:
        std::vector<Trade*> trades;
        std::vector<StopLoss*> stopLosses;
    public:
        // constructor
        Backtester();

        // destructor
        ~Backtester();
        
        // get all trades made during backtest
        std::vector<Trade*>& getAllTrades() { return trades; }

        // add trade if was closed
        void addTrade();

        // add stop loss if happened
        void addStopLoss();

        // run backtest
        void run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators);
};
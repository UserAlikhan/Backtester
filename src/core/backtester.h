#pragma once
#include <vector>
#include "trade.h"
#include "stopLoss.h"
#include "fixedStopLoss.h"
#include "trailingStopLoss.h"
#include "candle.h"
#include "indicator.h"
#include "intersectionEnum.h"
#include "intersectionIndicator.h"

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
        void addTrade(Trade* trade);

        // add stop loss if happened
        void addStopLoss(StopLoss* stopLoss);

        // handles user prompts
        void getUserInputs(double& maxStopLoss, double& trailingStopLoss, double& shareOfBalance);

        // calculates indicators and gathers their intersection signals
        void prepareIndicators(
            std::vector<Indicator*>& indicators,
            std::vector<Candle*>& candles,
            std::unordered_map<std::string, bool>& indicatorTriggerStatus,
            std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections
        );

        // iterates through intersection signals, checks confirmations, and opens/closes trades
        void processTradeSignals(
            std::vector<Candle*>& candles,
            std::vector<Indicator*>& indicators,
            const std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections,
            std::unordered_map<std::string, bool>& indicatorTriggerStatus,
            double* balance,
            double& shareOfBalance,
            double& maxStopLoss
        );

        bool confirmSignalAcrossIndicators(
            std::vector<Indicator*>& indicators,
            const std::vector<std::pair<std::string, std::vector<std::pair<TradeType, int>>>>& intersections,
            std::unordered_map<std::string, bool>& indicatorTriggerStatus,
            const std::string& mainIndicator,
            TradeType& type,
            int& index
        );

        // handles trade creation, stop loss and balance updates. (called inside processTradeSignals)
        void executeTrade(
            std::vector<Candle*>& candles,
            TradeType& type,
            int index,
            double* balance,
            double& shareOfBalance,
            double& maxStopLoss
        );

        // run backtest
        void run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators, double* balance);
};
#pragma once
#include <vector>
#include "trade.h"
#include "closeOrder.h"
#include "fixedStopLoss.h"
#include "trailingStopLoss.h"
#include "takeProfit.h"
#include "candle.h"
#include "indicator.h"
#include "intersectionEnum.h"
#include "intersectionIndicator.h"
#include "oscilatorIndicator.h"
#include "oscilatorStrategyTypesEnum.h"

class Backtester {
    private:
        std::vector<Trade*> trades;
        std::vector<CloseOrder*> stopLosses;
        std::vector<CloseOrder*> takeProfits;
        std::vector<CloseOrder*> trailingStopLosses;
    public:
        // constructor
        Backtester();

        // destructor
        ~Backtester();
        
        // get all trades made during backtest
        std::vector<Trade*>& getAllTrades() { return trades; }

        // add trade
        void addTrade(Trade* trade);

        // add stop loss
        void addStopLoss(CloseOrder* stopLoss);

        // add take profit
        void addTakeProfit(CloseOrder* takeProfit);

        // add trailing stop loss
        void addTrailingStopLosses(CloseOrder* trailingStopLoss);

        // handles user prompts
        void getUserInputs(double& maxStopLoss, double& trailingStopLoss, double& takeProfit, double& shareOfBalance);

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
            double& balance,
            double& shareOfBalance,
            double& maxStopLoss,
            double& takeProfit,
            double& trailingStopLoss
        );

        // iterates through intersection signals and compares it with main indicator
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
            int& index,
            double& balance,
            double& shareOfBalance,
            double& maxStopLoss,
            double& takeProfit,
            double& trailingStopLoss
        );

        void checkAllCloseOrders(
            std::vector<Candle*>& candles,
            Trade* trade,
            int& index,
            double& maxStopLoss,
            double& takeProfit,
            double& trailingStopLoss
        );

        // run backtest
        void run(std::vector<Candle*>& candles, std::vector<Indicator*>& indicators, double& balance);
};
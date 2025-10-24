#pragma once
#include <iostream>
#include <vector>
#include "tradeTypeEnum.h"
#include "oscilatorStrategyTypesEnum.h"

class OscilatorIndicator {
    private:
        OscilatorStrategyTypes strategy;
        
    public:
        OscilatorIndicator(OscilatorStrategyTypes strategy) : strategy(strategy) {}
        virtual ~OscilatorIndicator() = default;
        
        OscilatorStrategyTypes getStrategy() { return strategy; }

        virtual std::vector<std::pair<TradeType, int>> checkExtremes() = 0;
        virtual std::vector<std::pair<TradeType, int>> confirmTrend() = 0;
        virtual std::vector<std::pair<TradeType, int>> detectDivergence() = 0;
};
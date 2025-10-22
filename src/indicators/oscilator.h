#pragma once
#include <iostream>
#include <vector>
#include "tradeTypeEnum.h"
#include "oscilatorStrategyTypesEnum.h"

class Oscilator {
    private:
        OscilatorStrategyTypes strategy;
    public:
        Oscilator(OscilatorStrategyTypes strategy) : strategy(strategy) {}
        virtual ~Oscilator() = default;
        
        virtual std::vector<std::pair<TradeType, int>>& checkExtremes() = 0;
        virtual std::vector<std::pair<TradeType, int>>& confirmTrend() = 0;
        virtual std::vector<std::pair<TradeType, int>>& detectDivergence() = 0;
};
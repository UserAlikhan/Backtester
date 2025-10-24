#pragma once
#include <iostream>
#include <vector>
#include "candle.h"
#include "trendStrategy.h"
#include "sma.h"
#include "trendTypeEnum.h"

class MovingAverageStrategy : public TrendStrategy {
    private:
        int period;

    public:
        // constructor
        MovingAverageStrategy(int period);
        
        std::vector<TrendType> detect(const std::vector<Candle*>& candles) override;
};
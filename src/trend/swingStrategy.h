#pragma once
#include <iostream>
#include "candle.h"
#include "trendStrategy.h"
#include "trendTypeEnum.h"

class SwingStrategy : public TrendStrategy {
    private:
        int lookback;

    public:
        // constructor
        SwingStrategy(int lookback);
        
        std::vector<double>& detect(const std::vector<Candle*>& candles) override;

        static double averageSum(std::vector<double>& swings);  
};
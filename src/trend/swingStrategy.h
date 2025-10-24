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
        
        std::vector<TrendType> detect(const std::vector<Candle*>& candles) override;

        static double sumSwings(std::vector<double>& swings);  
};
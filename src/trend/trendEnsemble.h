#pragma once
#include <iostream>
#include <vector>
#include "weightedStrategyStruct.h"
#include "trendStrategy.h"
#include "trendTypeEnum.h"

class TrendEnsemble {
    private:
        double threshold;
        std::vector<WeightedStrategy*> strategies;  
    
    public:
        TrendEnsemble(double threshold);
        TrendEnsemble(double threshold, std::vector<WeightedStrategy*> strategies);
        ~TrendEnsemble();

        // adds new trend strategy into strategies vector
        void addStrategy(TrendStrategy* strategy, double weight);
        
        // each strategy has weight, this function calculates the signals according to weights
        std::vector<double> computeScore(std::vector<Candle*>& candles);

        // defines trend type based on computed score
        std::vector<TrendType> getTrend(std::vector<double>& computedScores);
};
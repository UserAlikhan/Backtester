#include "trendEnsemble.h"
#include <iostream>
#include <vector>

// constructors
TrendEnsemble::TrendEnsemble(double threshold) : threshold(threshold) {};
TrendEnsemble::TrendEnsemble(double threshold, std::vector<WeightedStrategy*> strategies)
    : threshold(threshold), strategies(strategies) {};

// destructor
TrendEnsemble::~TrendEnsemble() {
    for (auto st : strategies) {
        delete st->strategy;
        delete st;
    }
}

void TrendEnsemble::addStrategy(TrendStrategy* strategy, double weight) {
    WeightedStrategy* weightedStrategy = new WeightedStrategy;

    weightedStrategy->strategy = strategy;
    weightedStrategy->weight = weight;

    strategies.push_back(weightedStrategy);
}

std::vector<double> TrendEnsemble::computeScore(std::vector<Candle*>& candles) {
    std::vector<double> scores(candles.size(), 0.0);
    if (strategies.size() == 0) return scores;
    // if we have only one indicator make his weight 100%
    if (strategies.size() == 1) strategies[0]->weight = 1.0;

    // normalize the weight
    if (strategies.size() > 1) {
        double totalWeight = 0.0;

        // calculate total weight
        for (auto& strategy : strategies) totalWeight += strategy->weight;

        // normalize the weight
        if (totalWeight != 1.0) {
            for (auto& strategy : strategies) strategy->weight /= totalWeight;
        }
    }

    for (auto& ws : strategies) {
        std::vector<double>& data = ws->strategy->detect(candles);

        if (data.size() == candles.size()) {
            for (size_t i = 0; i < candles.size(); ++i) {
                scores[i] += data[i] * ws->weight;
            }
        } else {
            std::cout << "Trend detection is not the same size as dataset!" << std::endl;
        }
    }

    return scores;
}

std::vector<TrendType> TrendEnsemble::getTrend(std::vector<double>& score) {
    std::vector<TrendType> trends(score.size(), TrendType::NEUTRAL);

    for (size_t i = 0; i < score.size(); ++i) {
        if (score[i] > threshold) trends[i] = TrendType::BULLISH;
        else if (score[i] < -threshold) trends[i] = TrendType::BEARISH;
        else trends[i] = TrendType::NEUTRAL;
    }

    return trends;
}
#include "rsi.h"
#include <iostream>
#include <string>

RSI::RSI(std::string name, OscilatorStrategyTypes strategy, int period, double overboughtLevel, double oversoldLevel)
    : Indicator(name), OscilatorIndicator(strategy), period(period),
    overboughtLevel(overboughtLevel), oversoldLevel(oversoldLevel) {}

double calculateAverage(std::vector<double> data, int dataPeriod) {
    if (data.size() < static_cast<size_t>(dataPeriod)) return 0.0;

    double sum = 0.0;
    
    for (size_t i = data.size() - dataPeriod; i < data.size(); ++i) {
        sum += data[i];
    }

    return sum / dataPeriod;
}

std::vector<double>& RSI::getValues() {
    return values;
}

void RSI::calculate(std::vector<Candle*>& candles) {
    if ((int)candles.size() < period) {
        std::cout << "No data provided or it is smaller than specified periods" << std::endl;
        return;
    }

    std::vector<double> gains;
    std::vector<double> losses;
    // prealocte memory
    gains.reserve(candles.size());
    losses.reserve(candles.size());

    // compute price changes
    for (size_t i = 1; i < candles.size(); ++i) {
        double change = candles[i]->close - candles[i - 1]->close;
        gains.push_back(change > 0 ? change : 0);
        losses.push_back(change < 0 ? -change : 0);
    }

    double avgGain = calculateAverage(gains, period);
    double avgLoss = calculateAverage(losses, period);

    // calculate first rsi
    double RS = (avgLoss == 0) ? 0 : avgGain / avgLoss;
    values.resize(candles.size(), 0.0);
    values[period] = 100.0 - (100.0 / (1.0 + RS));
            
    // proceed with other data points
    for (size_t i = period + 1; i < candles.size(); ++i) {
        avgGain = (avgGain * (period - 1) + gains[i - 1]) / period;
        avgLoss = (avgLoss * (period - 1) + losses[i - 1]) / period;

        RS = (avgLoss == 0) ? 0 : avgGain / avgLoss;
        values[i] = 100.0 - (100.0 / (1.0 + RS));
    }
}

std::vector<std::pair<TradeType, int>> RSI::checkExtremes() {
    std::vector<std::pair<TradeType, int>> tradeSignals;
    tradeSignals.reserve(values.size());

    for (size_t i = 0; i < values.size(); ++i) {
        // skip first period trades, because they are 0.0 
        // and were used for initialization
        if (i < static_cast<size_t>(period)) {
            tradeSignals.push_back({TradeType::NONE, i});
            continue;
        }

        double& value = values[i];
        if (value > overboughtLevel) tradeSignals.push_back({TradeType::SHORT, i});
        else if (value < oversoldLevel) {
            tradeSignals.push_back({TradeType::LONG, i});
        }
        else tradeSignals.push_back({TradeType::NONE, i});
    }

    return tradeSignals;
}

/*
Filters trade with trend. Take only RSI oversold buys if the trend is up
Take only RSI overbought if the trend is down.

Ways to identify trend:
 - Moving averages. Price above MA -> uptrend, below MA -> downtrend
    MA slope positive -> uptrend, MA slope negative -> downtrend
 - ADX (Average Directional Index) that measures trend strength
    to confirm trend. [ADX > 25 -> strong trend, ADX < 20 -> weak trend / range]
 - Analyze swings (Swing Trading strategies)
*/
std::vector<std::pair<TradeType, int>> RSI::confirmTrend() {
    std::vector<std::pair<TradeType, int>> tradeSignals;
    tradeSignals.reserve(values.size());

    return tradeSignals;
}

std::vector<std::pair<TradeType, int>> RSI::detectDivergence() {
    std::vector<std::pair<TradeType, int>> tradeSignals;
    tradeSignals.reserve(values.size());
    
    return tradeSignals;
}
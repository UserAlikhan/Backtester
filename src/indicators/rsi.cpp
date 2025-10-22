#include "rsi.h"
#include <iostream>
#include <string>

RSI::RSI(std::string name, OscilatorStrategyTypes strategy, int period, double overboughtLevel, double oversoldLevel)
    : Indicator(name), Oscilator(strategy), period(period),
    overboughtLevel(overboughtLevel), oversoldLevel(oversoldLevel) {}

double calculateAverage(std::vector<double> data, int dataPeriod) {
    double sum = 0.0;
    int counter = 0;

    while (counter < dataPeriod) {
        sum += data[dataPeriod];
        counter++;
    }

    return sum / dataPeriod;
}

std::vector<double>& RSI::getValues() {
    return values;
}

void RSI::calculate(std::vector<Candle*>& candles) {
    if (candles.size() < period) {
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
        losses.push_back(change < 0 ? change : 0);
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

        RS = (avgLoss = 0) ? 0 : avgGain / avgLoss;
        values[i] = 100.0 - (100.0 / (1.0 + RS));
    }
}

std::vector<std::pair<TradeType, int>>& RSI::checkExtremes() {
    std::vector<std::pair<TradeType, int>> extremeValues;

    for (int i = 0; i < values.size(); ++i) {
        double& value = values[i];
        if (value > 70) extremeValues.push_back({TradeType::SHORT, i});
        else if (value < 30) extremeValues.push_back({TradeType::LONG, i});
        else extremeValues.push_back({TradeType::NONE, i});
    }

    return extremeValues;
}
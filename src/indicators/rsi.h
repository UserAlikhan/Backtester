// RSI is a momentum oscilator used primarily to identify overbought and oversold conditions
// Typically an RSI below 30 indicates oversold and above 70 indicates overbought conditions

// RSI tends to stay between 40 and 70, with overbought conditions above 70, indicating
// strong momentum rather than an imminent reversal. Traders often looking for dip-buying
// opportunities when RSI pulls back to around 50 rather than waiting for it to drop below 30
// - something that rarely happens in strong uptrends.

// The trend matters. If move to overbought is a part of an increasing trend, then
// reversals will likely be shorterm or none.

// Failure swings is the strongest way to seel a reversal using the RSI.
// It acts as confirmation of trend reversals and often appears with divergence,
// strengthening the case that a true turning point has been reached.

// Technical analysis tools tend to complement each other. In particular, the RSI's 
// effectiveness increases when combined with other indicators. Pairing RSI with 
// the MACD helps confirm trend reversals as the two measure momentum differently
#pragma once
#include "indicator.h"
#include "oscilator.h"
#include "oscilatorStrategyTypesEnum.h"
#include <iostream>
#include <vector>
#include <string>

class RSI : public Indicator, public Oscilator {
    private:
        int period;
        double overboughtLevel;
        double oversoldLevel;
        std::vector<double> values;
    
    public:
        RSI(std::string name, OscilatorStrategyTypes strategy, int period, double overboughtLevel = 70, double oversoldLevel = 30);

        std::vector<double>& getValues();

        void calculate(std::vector<Candle*>& candles);

        std::vector<std::pair<TradeType, int>>& checkExtremes() override;
        std::vector<std::pair<TradeType, int>>& confirmTrend() override;
        std::vector<std::pair<TradeType, int>>& detectDivergence() override;
};